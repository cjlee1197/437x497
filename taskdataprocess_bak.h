/*===========================================================================+
|  Class    : TaskDataProcess                                                |
|  Task     : Command Service Routine header file                            |
|----------------------------------------------------------------------------|
|  Compile  : gcc V3.2 -                	                                 |
|  Link     : gcc V3.2 -                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 10/02/2010                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TASKDATAPROCESS
#define     D_TASKDATAPROCESS

#include    "wnd.h"
#include    "task.h"
#include	"tmpackclass.h"

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
|           Class declaration - CTaskDataProcess                             |
+===========================================================================*/
class CTaskDataProcess : public CTask, public CtmPackClass
{
	DECLARE_DYNCREATE(CTaskDataProcess)	
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskDataProcess();
    ~CTaskDataProcess();
public:
 	virtual	int		GetData(int		nType, 	int nIndex, 	int nSize, 	void* pData)				{return 0;}
	virtual	int		GetDataS(int	nType,	int	nStartIndex,int nCount, int nSize, 	void* pData)	{return 0;}
	virtual	int		SetData(int		nType,	int	nIndex, 	int nSize, 	void* pData)				{return 0;}
	virtual	int		SetDataS(int	nType,	int	nStartIndex,int nCount, int nSize,	void* pData)	{return 0;}
	
	virtual	int		SetDataPath(int nType,	int	nPathFlag)											{return 0;}
	virtual int		SaveEnergy(int nType, int nIndex = -1)											{return FALSE;}
	virtual int		SaveState(int nMode, int nIndex = -1)											{return FALSE;}
	virtual int		ClearEnergyLog()																{return FALSE;}
	virtual int		ClearStateLog()																	{return FALSE;}
	virtual int		ClearPowerOnOff()																{return FALSE;}
	virtual int		ClearOnOffState()																{return FALSE;}
	virtual int		CalculateTime(int	nType, tmDATETIME datetime_start, tmDATETIME datetime_end)	{return FALSE;}
	virtual UTIME   MakeTime(tmDATETIME *tm, int nType = 0)											{return 0;}
};

extern	CTaskDataProcess*	g_pTaskDataProcess;

#endif

