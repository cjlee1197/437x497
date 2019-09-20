/*===========================================================================+
|  Class    : TaskCANMaster                                                  |
|  Task     :													             |
|----------------------------------------------------------------------------|
|  Compile  : Gcc 4.3    -                                                   |
|  Link     : Gcc 4.3    -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James                                                          |
|  Version  : V1.00                                                          |
|  Creation : 2013/04/08                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TASKCANMASTER
#define     D_TASKCANMASTER

#include    "task.h"
#include	"tmpackclass.h"
#include	"canMaster.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
/*===========================================================================+
|           Class declaration - TaskMoni                                     |
+===========================================================================*/
class CTaskCanMaster : public CTask, public CtmPackClass
{
	DECLARE_DYNCREATE(CTaskCanMaster);
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskCanMaster();
    ~CTaskCanMaster();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
public:
	virtual	int		OpenMaster();
	virtual	int		CloseMaster();
	
	virtual	void	CanInitConfig();
	virtual	void	ReSetConfig();
	
	virtual	void	Process_NMT(WORD wNodeID, WORD wNMTCS);
	virtual	void	Process_PDO(WORD wNodeID, WORD wPDONum);
	virtual	void	Process_SDORead(WORD wNodeID,  WORD wIndex, BYTE cSubIndex);
	virtual	void	Process_SDOWrite(WORD wNodeID, WORD wIndex, BYTE cSubIndex, DWORD dwValue, WORD wSize);
	virtual	void	Can_Read();
	virtual	int		M3Led_Send(){return 0;}
	virtual	void	SetCommand(int nCommandId){;};
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	int 	m_nskt;

};
extern		CTaskCanMaster*          g_ptaskCanMaster;
//extern		char	u_acOutPutData_Old[MAX_CANDATALEN];
extern		DWORD	g_dwM3Key[MAX_KEYINDEX];
extern		DWORD	g_dwM3LED[MAX_LEDINDEX];
extern		DWORD	g_dwM3LEDOld[MAX_LEDINDEX];

#endif
