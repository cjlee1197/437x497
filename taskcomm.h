/*===========================================================================+
|  Class    : TaskComm                                                       |
|  Task     : Communication Service Routine header file                      |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52 -                                                   |
|  Link     : MSVC V1.52 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/13/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TASKCOMM
#define     D_TASKCOMM

#include    "task.h"
#include	"device.h"
//#include	"control.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

#define		COMM_MAX_DEVICE		10
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
    
typedef	struct tagCOMMDEV
{
	int		nHandle;				// 文件handle
	char	szName[20];				// 設備名稱
	CDevice	*pDevice;
}	COMMDEV;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - TaskComm                                     |
+===========================================================================*/
class CTaskComm : public CTask
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskComm();
    virtual ~CTaskComm();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/

private:
    static	int				m_nResetIndex;
    static	int				m_nCommCount;
	static	pthread_mutex_t	m_mutex;
	static	pthread_mutex_t	m_write;
	static	COMMDEV			m_dev[COMM_MAX_DEVICE];
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
//    void    Run();

	static	int	AddDevice(char *pszName, void *pDevice);
	static	int	DelDevice(int nIndex);
	static	int	ResetDevice(int nIndex);
	static	int	SendData(int nIndex, char *pData, int nCount);
	static	int	GetData(int nIndex, char *pData, int nCount);
	static	int	GetState(int nIndex);
	CDevice*	GetDevice(int nIndex) { return m_dev[nIndex].pDevice; };
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
public:

};

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
/*typedef struct tagCOMMMAP
    {
    WORD    wCmd;
    void    (__near CTaskComm::*npFunc)();
    } COMMMAP;
*/
#endif

