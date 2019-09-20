/*===========================================================================+
|  Function : TmControl                                                      |
|  Task     : Machine Control Utilities Cpp File	                         |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) V3.2                                                  |
|  Link     : G++(GCC) V3.2                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 05/09/2008                                                     |
|  Revision : 05/09/2008                                                     |
+===========================================================================*/

#include	"tmcontrol.h"
#include	"tmdebug.h"
#include	"utils.h"

/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/
tmDB_EFLAG		m_dberrorflg;
tmDB_EFLAG		m_dberrorflg2;
BOOL			b_AlarmState = TRUE;

WORD	g_wDspStatus 			= 0;
WORD	g_wDsp2Status			= 0;
WORD	g_wErrorStatus 			= 0;
WORD	g_wErrorStatus_Dsp28 	= 0;

tmDB_BUFFER		m_dbbuffer = 
	{
	0xffff,                         // wOperationMode
	0xffff,                         // wOperationStep
	0xffff,                         // wOperationSubStep
	0xffff,                         // Operation mode index
	tmERROR_NULL,               // Error code index
	FALSE,                      // Change operation mode from manual to auto
	0,                          // Shut count after changing operation mode from manual to auto
	0,                          // flMotorStatus;
	0,                          // flHeaterStatus;	
	FALSE,                      // bDelayedSpeech;
	0,                          // lDelayedCount;
	FALSE,                      // bRepeatSpeech;
	0                           // lRepeatCount;
    };

tmDB_BUFFER		m_dbbuffer2 = 
	{
	0xffff,                         // wOperationMode
	0xffff,                         // wOperationStep
	0xffff,                         // wOperationSubStep
	0xffff,                         // Operation mode index
	tmERROR_NULL,               // Error code index
	FALSE,                      // Change operation mode from manual to auto
	0,                          // Shut count after changing operation mode from manual to auto
	0,                          // flMotorStatus;
	0,                          // flHeaterStatus;	
	FALSE,                      // bDelayedSpeech;
	0,                          // lDelayedCount;
	FALSE,                      // bRepeatSpeech;
	0                           // lRepeatCount;
    };
/*===========================================================================+
|           Type	                                                         |
+===========================================================================*/
IMPLEMENT_DYNCREATE(CtmControl, CtmPackClass)
IMPLEMENT_DYNCREATE(CtmControlServer, CtmPackClass)
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmControlServer*	g_ptmControlServer = NULL;
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*---------------------------------------------------------------------------+
|  Function : CtmControlServer::CtmControlServer()				             |
|  Task     :  CtmControlServer篶硑ㄧ计                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControlServer::CtmControlServer()
{
	memset(m_pControl,0,sizeof(m_pControl));
}
/*---------------------------------------------------------------------------+
|  Function : CtmControlServer::~CtmControlServer()				             |
|  Task     :  CtmControlServer猂篶ㄧ计                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControlServer::~CtmControlServer()
{
		
}

/*---------------------------------------------------------------------------+
|  Function : CtmControlServer::CreateControl()                              |
|  Task     :  ﹍てゼ砆﹍てcontrol狝叭                                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControlServer::CreateControl()
{
	for (int i = 0; i < CNT_INT_MOTION_CONTROL_NUM; i++)
	{
		if (m_pControl[i] == NULL)
			m_pControl[i] = new CtmControl;
	}
}

/*---------------------------------------------------------------------------+
|  Function : CtmControlServer::DestroyControl()                             |
|  Task     :  綪反┮Τcontrol狝叭                                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControlServer::DestroyControl()
{
	for (int i = 0; i < CNT_INT_MOTION_CONTROL_NUM; i++)
	{
		if (m_pControl[i] != NULL)
			delete m_pControl[i];
	}	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl::CtmControl()				                         |
|  Task     :  CtmControl篶硑ㄧ计                                            |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl::CtmControl()
{
	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl::~CtmControl()				                         |
|  Task     :  CtmControl猂篶ㄧ计                                            |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl::~CtmControl()
{
	
}

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
//*************************************
//						Function Control
//*************************************
void		OnTimeHeater()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			g_ptmControlServer->FunctionControl()->_OnTimeHeater();
		}	
	}	
}

void		CountZero()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			g_ptmControlServer->FunctionControl()->_CountZero();
		}	
	}
}

void		ClearZeroOil()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			g_ptmControlServer->FunctionControl()->_ClearZeroOil();
		}	
	}
}

void    	GetInitPasswd_Install(char* pcMachineID, char* pcPassword, 
											tmDATE dtMachineDateOut)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			g_ptmControlServer->FunctionControl()->_GetInitPasswd_Install(pcMachineID, pcPassword, dtMachineDateOut);
		}	
	}
}

int       	GetStage_Install(char* pcPassword, char* pcMachineID, 
											tmDATE dtStartStage)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			return g_ptmControlServer->FunctionControl()->_GetStage_Install(pcPassword, pcMachineID, dtStartStage);
		}	
	}
	return 0;
}

BOOL        CheckTimeOut_Install()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			return g_ptmControlServer->FunctionControl()->_CheckTimeOut_Install();
		}	
	}
	return FALSE;
}

WORD        StartUp_Install(char* pcPassword)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			return g_ptmControlServer->FunctionControl()->_StartUp_Install(pcPassword);
		}	
	}
	return 0;
}

WORD		JudgeFactoryOnWork_Install()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			return g_ptmControlServer->FunctionControl()->_JudgeFactoryOnWork_Install();
		}	
	}
	return 0;
}

void		SaveOperation()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			g_ptmControlServer->FunctionControl()->_SaveOperation();
		}	
	}
}

void		DealOperation(OPERATION* pOperation, int nWRflag)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			g_ptmControlServer->FunctionControl()->_DealOperation(pOperation, nWRflag);
		}	
	}
}									

int			ReadOperationFromSRAM(OPERATION* pOperation)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			return g_ptmControlServer->FunctionControl()->_ReadOperationFromSRAM(pOperation);
		}	
	}
	return 0;
}

int			WriteOperationToSRAM(OPERATION* pOperation)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			return g_ptmControlServer->FunctionControl()->_WriteOperationToSRAM(pOperation);
		}	
	}
	return 0;
}

void		DealOperationStartMachine()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			g_ptmControlServer->FunctionControl()->_DealOperationStartMachine();
		}	
	}
}

WORD		CountOperationCRC()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			return g_ptmControlServer->FunctionControl()->_CountOperationCRC();
		}	
	}
	return 0;
}

int			GetBlockValueState(int nBlockID, tmStorageType StorageType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			return g_ptmControlServer->FunctionControl()->_GetBlockValueState(nBlockID, StorageType);
		}	
	}
	return 0;
}

WORD		VER2HEX()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			return g_ptmControlServer->FunctionControl()->_VER2HEX();
		}	
	}
	return 0;
}

void        SetCoreEffectPN(BOOL bSendDSP)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			g_ptmControlServer->FunctionControl()->_SetCoreEffectPN(bSendDSP);
		}	
	}
}

void        SetEjectEffectPN(BOOL bSendDSP)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->FunctionControl() != NULL)
		{
			g_ptmControlServer->FunctionControl()->_SetEjectEffectPN(bSendDSP);
		}	
	}	
}

int			GetMoniStructSize()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoniControl() != NULL)
		{
			return g_ptmControlServer->MoniControl()->_GetMoniStructSize();
		}	
	}
	return 0;	
}