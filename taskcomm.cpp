/*===========================================================================+
|  Class    : TaskComm                                                       |
|  Task     : Communication Service Routine                                  |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/13/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include    "taskcomm.h"

#include    "main.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
typedef void *(* PRSRUN)(void *);

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
extern	WORD			g_wTaskCommStat;
extern	WORD			g_wI32;

int				CTaskComm::m_nResetIndex = 0;
pthread_mutex_t	CTaskComm::m_mutex;
pthread_mutex_t	CTaskComm::m_write;
COMMDEV			CTaskComm::m_dev[COMM_MAX_DEVICE] = {NULL};
int				CTaskComm::m_nCommCount	= 0;
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - TaskComm                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskComm::CTaskComm() : CTask()
{
	if (m_nCommCount == 0) 
		{
		memset(m_dev, 0, sizeof(COMMDEV) * COMM_MAX_DEVICE);	// 初始值清零
		pthread_mutex_init(&m_mutex, NULL);
		pthread_mutex_init(&m_write, NULL);
	}
	m_nCommCount++;
}

CTaskComm::~CTaskComm()
{
	int	i;
	
	m_nCommCount--;
	if (m_nCommCount == 0)
	{
		for(i = 0; i < COMM_MAX_DEVICE; i ++)
		{
			if(m_dev[i].pDevice != NULL)
			{
				m_dev[i].pDevice->Close();
				delete 	m_dev[i].pDevice;
			}
		}
		pthread_mutex_destroy(&m_mutex);
		pthread_mutex_destroy(&m_write);
	}
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
/*void        CTaskComm::Run()
{
	int i;
	
	for(i = 0; i < COMM_MAX_DEVICE; i ++)
	{
		if(m_dev[i].pDevice != NULL)
		{
			g_wTaskCommStat = 0;
			if(m_dev[i].pDevice->GetOpStatus() != DEVICE_S_OPENED)
				ResetDevice(i);
			else
			{
				if(i == m_nResetIndex)	m_nResetIndex = -1;
				if (SIMD_Status(1))		g_wTaskCommStat = 1;
			}
		}
	}
	if (I32_Status() >= 0)	g_wI32 = 1;
	else					g_wI32 = 0;

	if (g_ptaskCmd->GetFocus() != NULL)
		g_ptaskCmd->GetFocus()->Update();//<<yang 2006/2/20 View Update()
}
*/
/*--------------------------------------------------------------------------+
|  Function	:	AddDevice(char pszName, void *pDevice)						|
|  Task		:	增加一種通訊界面											|
+---------------------------------------------------------------------------+
|  Call		:	nDevID	= AddDevice(&Device)								|
|																			|
|  Parameter:	pszName			-   設備名稱								|
|				pDevice			- 	指向設備信息結構的指針					|
|																			|
|  Return	: 	nDevID			-  反回通訊設備的ID							|
|				< 0				-  增加通訊設備失敗							|
+--------------------------------------------------------------------------*/
int		CTaskComm::AddDevice(char *pszName, void *pDevice)
{
	int			nIndex	= 0;
	int			nState;
	CDevice *	pDev = NULL;
	int			i;
	
	if(pszName == NULL)	return 0;
	pthread_mutex_lock(&m_mutex);
	while(m_dev[nIndex].pDevice != NULL && nIndex < COMM_MAX_DEVICE) nIndex ++;
//	//printf("before szName=%s m_nCommCount=%d \n", pszName, m_nCommCount);
	if(nIndex >= COMM_MAX_DEVICE)	
		{
		pthread_mutex_unlock(&m_mutex);
		return -1;
		}
	
	//=======
	// 創建對應的設備
	//=======
	pDev = (CDevice *)::Create(pszName);
	if(pDev == NULL)	
		{
		pthread_mutex_unlock(&m_mutex);
		return -1;
		}
	
	strncpy(m_dev[nIndex].szName, pszName, 20);		
	
	nState = pDev->Create(pDevice);
	if(nState != DEVICE_S_OPENED)
	{
		pDev->Close();
		delete pDev;
		pDev	= NULL;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
//	//printf("szName=%s nIndex=%d nState=%ld pDev=%ld\n", m_dev[nIndex].szName, nIndex, nState, pDev);
	m_dev[nIndex].pDevice = pDev;
	pthread_mutex_unlock(&m_mutex);
	return nIndex;
}

/*--------------------------------------------------------------------------+
|  Function	:	DelDevice(void *pDevice)									|
|  Task		:	增加一種通訊界面											|
+---------------------------------------------------------------------------+
|  Call		:	nDevID	= DelDevice(nIndex)									|
|																			|
|  Parameter:	nIndex			- 	指向設備信息結構的指針					|
|																			|
|  Return	: 	nDevID			-  刪除的通訊設備的ID						|
|				< 0				-  沒有此設備								|
+--------------------------------------------------------------------------*/
int		CTaskComm::DelDevice(int nIndex)
{
	if(nIndex >= COMM_MAX_DEVICE || nIndex < 0)	return -1;
	
	memset(m_dev[nIndex].szName, 0, sizeof(m_dev[nIndex].szName));
	if(m_dev[nIndex].pDevice != NULL)
	{
		m_dev[nIndex].pDevice->Close();
		delete 	m_dev[nIndex].pDevice;
	}
	m_dev[nIndex].pDevice	= NULL;
	return nIndex;
}

/*--------------------------------------------------------------------------+
|  Function	:	ResetDevice(int	nIndex)										|
|  Task		:	把一种通訊界面复位											|
+---------------------------------------------------------------------------+
|  Call		:	nDevID	= DelDevice(nIndex)									|
|																			|
|  Parameter:	nIndex			- 	設備的index								|
|																			|
|  Return	: 	nDevID			-  設備ID									|
|				< 0				-  复位失敗									|
+--------------------------------------------------------------------------*/

int	CTaskComm::ResetDevice(int nIndex)
{
	pthread_t	threadID;
	
	if(m_nResetIndex != -1)	return -1;
	pthread_mutex_lock(&m_mutex);
	//printf("CTaskComm::ResetDevice nIndex =%d\n", nIndex);
	if(nIndex >= COMM_MAX_DEVICE || nIndex < 0)	return -1;
	if(m_dev[nIndex].pDevice == NULL)			return -1;
		
	m_dev[nIndex].pDevice->Close();
	m_nResetIndex	= nIndex;
	
	pthread_create(&threadID, NULL, (PRSRUN)m_dev[nIndex].pDevice->Open(), NULL);

	pthread_mutex_unlock(&m_mutex);
	return nIndex;	
}

int		CTaskComm::SendData(int nIndex, char *pData, int nCount)
{
	if(nIndex >= COMM_MAX_DEVICE || nIndex < 0)	return -1;
	if(m_dev[nIndex].pDevice == NULL)			return -1;
	pthread_mutex_lock(&m_write);
	int	nResult=m_dev[nIndex].pDevice->Write((BYTE *)pData, nCount, &nCount);
	pthread_mutex_unlock(&m_write);
	return nResult;
}

int		CTaskComm::GetData(int nIndex, char *pData, int nCount)
{
	if(nIndex >= COMM_MAX_DEVICE || nIndex < 0)	return -1;
	if(m_dev[nIndex].pDevice == NULL)			return -1;
	
	if(m_dev[nIndex].pDevice->Read((BYTE *)pData, nCount, &nCount) == DEVICE_SUCCESS)
	{
		return nCount;
	}
	return -1;
}

int		CTaskComm::GetState(int nIndex)
{
		if(nIndex == -1 || nIndex >= COMM_MAX_DEVICE || m_dev[nIndex].pDevice == NULL)	return -1;

	return m_dev[nIndex].pDevice->GetOpStatus();
}

