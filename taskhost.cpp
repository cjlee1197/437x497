#include 	<stdlib.h>
#include	"taskhost.h"
#include	"main.h"
#include	"init.h"
#include	"cblock.h"
#include	"tmdebug.h"
#include	"utils.h"

IMPLEMENT_DYNCREATE(CTaskHost, CtmPackClass)
CTaskHost*			g_ptaskdsp		= NULL;	
CTaskHost*			g_ptaskdsp28 	= NULL;
CTaskHost*			g_ptaskdsp28_Austone = NULL;		//James add for austone 2010/5/19
CTaskHost*			g_ptaskdsp28_N2C= NULL;				//Caozz add 2010/10/21 02:11¤U¤È
CTaskHost*			g_ptaskdsp28_M3	= NULL;
CTaskHost*			g_ptaskCom2dsp 	= NULL;
CTaskHost*			g_ptaskinet		= NULL;
CTaskHost*			g_ptaskHmiComm	= NULL;				//James add for hmi comm 2011/3/30 ¤W¤È 11:56:46

CTaskHost*			g_ptaskCncComm	= NULL;	//20120510		//Mario debug
CTaskHost*			g_ptaskremote	= NULL;
CTaskHost*			g_ptaskpicker	= NULL;
/*==== =======================================================================+
|            Class implementation - CTaskHost                                  |
+===== ======================================================================*/
/*---- -----------------------------------------------------------------------+
|            Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskHost::CTaskHost(): CTaskComm(), CtmPackClass()
{
	m_pHostProtocol		= NULL;
	m_pData				= NULL;
	m_nDeviceID			= -1;
	
}

CTaskHost::~CTaskHost()
{
	
}

int		CTaskHost::SendZeroSet(int nZeroType)
{
	return 0;
}

int		CTaskHost::ReqValues(int nType, int nElements, char* pszIDs[], void* pEx)
{
	return 0;
}


int		CTaskHost::ReqValues(int nType, int nElements, int pIndexs[], void* pEx)
{
	return 0;
}

int		CTaskHost::ReqValues(int nType, char* pszID, void* pEx)
{
	return 0;
}

//by RS232 Command Type Data
int		CTaskHost::ReqValues(int nType)
{
	return 0;	
}

int		CTaskHost::ReqValues(int nType, int nHostAdreess1, int nHostAdreess2)
{
	return 0;	
}

int		CTaskHost::ReqValues(int nType, void** pInputData, int nData1, int nData2)
{
	return 0;	
}

int		CTaskHost::ReqValues(int nType, void* pOutData, int nSize)
{
	return 0;	
}

int		CTaskHost::ReqValues(int nType, void* pOutData, int nSize, void* pEx)
{
	return 0;	
}

int		CTaskHost::ReqValues(int nType, int nValue1, int nPrecision1, int nValue2, int nPrecision2)
{
	return 0;
}

int		CTaskHost::StopReq(int nType, int nElements, char* pIDs[], void* pEx)
{
	//return 	StopReq(nType, NULL);
	return 0;
	
}

int		CTaskHost::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
{
	//return 	StopReq(nType, NULL);
	return 0;
}

int		CTaskHost::StopReq(int nType, char* pszID, void* pEx)
{
	
	return 0;
}

int		CTaskHost::WriteValue(int nType, int nElements, char* pszIDs[], void* pEx)
{
	//return ReqValues(nType, nElements, pszIDs, pEx);
	return 0;
}

int		CTaskHost::WriteValue(int nType, int nElements, int pIndexs[], void* pEx)
{
	//return ReqValues(nType, nElements, pIndexs, pEx);
	return 0;
}

int		CTaskHost::WriteValue(int nType, char* pszID, void* pEx)
{
	//return ReqValues(nType, pszID, pEx);
}

int		CTaskHost::WriteValue(int nDataID)
{
	return 0;
}

int		CTaskHost::WriteValue(char* pszID)
{
	return 0;
}

int		CTaskHost::WriteValue(int nType, int nHostAdreess1, int nHostAdreess2)
{
	return 0;
}

int		CTaskHost::WriteValue(int nType, void* pInputData, int nData1, int nData2)
{
	return 0;	
}

int		CTaskHost::WriteValue(int nType, void** pInputData, int nData1, int nData2)
{
	return 0;	
}

int		CTaskHost::WriteValue(int nType, void* pInputData, int nSize)
{
	return 0;
}


int		CTaskHost::WriteValue(int type ,int s_arr ,int len ,void* data)
{
	return 0;
}	

int		CTaskHost::WriteValue(int len ,void* data)
{
	return 0;
}	
		
int		CTaskHost::CheckOnLine()
{
	return 0;
}

int		CTaskHost::GetOnLineStatus()
{
	return 0;
}

void	CTaskHost::SetHostProtocol(CtmHostProtocol* pHostProtocol)
{
	m_pHostProtocol = pHostProtocol;
}

CtmHostProtocol* CTaskHost::GetHostProtocol()
{
	return m_pHostProtocol;
}


int		CTaskHost::GetData(void* pdata, int nLen)
{
	//return m_pHostProtocol->ParseProtocol(pdata, nLen);
	return 0;
}

int		CTaskHost::ReqValuesDiag(int pIDs[], int nElements, int nDiagType)
{
	return 0;
}


int		CTaskHost::ReqValuesDiag(int nStartID, int nElements, int nDiagType)
{
	return 0;
}

int		CTaskHost::WriteValuesDiag(int	pIDs[], int pValues[], int nElements, int nDiagType)
{
	return 0;
}

int		CTaskHost::WriteValuesDiag(int nStartID, int pValues[], int nElements, int nDiagType)
{
	return 0;
}

