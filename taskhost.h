/*===========================================================================+
|  Class    : TaskHost                                                       |
|  Task     : Communication Service Routine header file                      |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52 -                                                   |
|  Link     : MSVC V1.52 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 2008/06/05                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TASKHOST
#define     D_TASKHOST

#include	"taskcomm.h"
#include	"tmprotocol.h"
#include	"tmcontrol.h"
#include	"rs232.h"
#include	"oil54struct.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - TaskComm                                     |
+===========================================================================*/
class CTaskHost : public CTaskComm, public CtmPackClass
{
	DECLARE_DYNCREATE(CTaskHost)	
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
public:
		CTaskHost();
		~CTaskHost();
		virtual	int		SendZeroSet(int nZeroType = 0);
		
		//	request value
		virtual	int		ReqValues(int nType, int nElements, char* pszIDs[], void* pEx = NULL);
		virtual	int		ReqValues(int nType, int nElements, int pIndexs[], void* pEx = NULL);
		virtual	int		ReqValues(int nType, char* pszID, void* pEx = NULL);
		
		virtual	int		ReqValues(int nType);
		virtual	int		ReqValues(int nType, int nHostAdreess1, int nHostAdreess2 = 0);
		virtual	int		ReqValues(int nType, void* pOutData, int nSize);
		virtual	int		ReqValues(int nType, void** pInputData, int nData1, int nData2);
		
		virtual int		ReqValues(int nType, int nValue1, int nPrecision1, int nValue2, int nPrecision2);
		
		virtual	int 	ReqValues(int nType, void* pOutData, int nSize, void* pEx);
		
		//	stop request
		virtual	int		StopReq(int nType, int nElements, char* pIDs[], void* pEx = NULL);
		virtual	int		StopReq(int nType, int nElements, int pIndexs[], void* pEx = NULL);
		virtual	int		StopReq(int nType, char* pszID, void* pEx = NULL);
		
		//	write the value to the motor & host; the values are from database
		virtual	int		WriteValue(int nType, int nElements, char* pszIDs[], void* pEx = NULL);
		virtual	int		WriteValue(int type ,int s_arr ,int len ,void* data);
		virtual	int		WriteValue(int len ,void* data);
		virtual	int		WriteValue(int nType, int nElements, int pIndexs[], void* pEx = NULL);
		virtual	int		WriteValue(int nType, char* pszID, void* pEx = NULL);
		virtual	int		WriteValue(int nDataID);
		virtual	int		WriteValue(char* pszID);
		
		
		virtual	int		WriteValue(int nType, int nHostAdreess1, int nHostAdreess2 = 0);
		virtual	int		WriteValue(int nType, void* pInputData, int nSize);
		virtual	int		WriteValue(int nType, void* pInputData, int nData1, int nData2);
		virtual	int		WriteValue(int nType, void** pInputData, int nData1, int nData2);
		
		
		//	diagnose
		virtual	void	StartDiag()	{};
		virtual	void	EndDiag()	{};
		virtual	int		ReqValuesDiag(int pIDs[], int nElements, int nDiagType = 2);
		virtual	int		ReqValuesDiag(int nStartID, int nElements, int nDiagType = 2);
		virtual	int		WriteValuesDiag(int	pIDs[], int pValues[], int nElements, int nDiagType = 3);
		virtual	int		WriteValuesDiag(int nStartID, int pValues[], int nElements, int nDiagType = 3);
		
		
		
		//	0 is on line
		virtual	int		CheckOnLine();
		virtual	int		GetOnLineStatus();
		
		virtual	void	SetHostProtocol(CtmHostProtocol* pHostProtocol);
		virtual	CtmHostProtocol* GetHostProtocol();
	protected:
		virtual	int		GetData(void* pdata, int nLen);
	protected:
		int				m_nConnectStatus;				// 網絡的狀態：0表示OK，1表示已經一段時間沒有了QuickValue或者握手協議，2表示物理斷線
				
		CtmHostProtocol*	m_pHostProtocol;			//	主機協議
		int				m_nMaxElements;					//	協議中允許的最大元素數
		int				m_nMaxLen;						//	協議包的最大長度
		BYTE*			m_pData;						//	封裝以后的包
		int				m_nDeviceID;					//	設備的名稱
	
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

};
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
extern		CTaskHost*			g_ptaskdsp;	
extern		CTaskHost*			g_ptaskCom2dsp;	
extern		CTaskHost*			g_ptaskdsp28;
extern		CTaskHost*			g_ptaskdsp28_Austone;			//James add for austone 2010/5/19
extern		CTaskHost*			g_ptaskdsp28_N2C;
extern		CTaskHost*			g_ptaskdsp28_M3;
extern		CTaskHost*			g_ptaskinet;
extern		CTaskHost*			g_ptaskHmiComm;					//James add for hmi comm 2011/3/30 上午 11:56:46

extern		CTaskHost*			g_ptaskCncComm;	//20120510		//Mario debug
extern		CTaskHost*			g_ptaskremote;
extern		CTaskHost*			g_ptaskpicker;

#endif

