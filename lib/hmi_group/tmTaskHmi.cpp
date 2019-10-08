#include	"tmTaskHmi.h"
#include	"../../freedata.h"
#include	"../../moldControl.h"

CtmTaskHmi*	g_pTaskHmi = NULL;
IMPLEMENT_DYNCREATE(CtmTaskHmi, CtmPackClass)

CtmTaskHmi:: CtmTaskHmi():CTask(), CtmPackClass()
{
	m_nRampConfig = CtmConfig::GetInstance()->GetConfigDealRamp();
}

CtmTaskHmi::~CtmTaskHmi()
{
	
}

int		CtmTaskHmi::CreateSelf()
{
	CtmPackClass::CreateSelf();
		
		
		
	
	//Init_AxialConfig(m_nRampConfig);
	
	//RegisterMessage(MSG_USER_HMI_INIT_MOLD_CLOSE, 	m_idMe);
	//RegisterMessage(MSG_USER_HMI_INIT_MOLD_OPEN, 	m_idMe);
	//RegisterMessage(MSG_USER_HMI_INIT_INJECT, 		m_idMe);
	//RegisterMessage(MSG_USER_HMI_INIT_HOLDPRESS, 	m_idMe);
	//RegisterMessage(MSG_USER_HMI_INIT_EJECT_ADV, 	m_idMe);
	//RegisterMessage(MSG_USER_HMI_INIT_EJECT_RET, 	m_idMe);
	//RegisterMessage(MSG_USER_HMI_INIT_AIR, 			m_idMe);
	//RegisterMessage(MSG_USER_HMI_INIT_CORE, 		m_idMe);
	
	RegisterMessage(MSG_WND_FORM_CREATE, 		m_idMe);
	
	return 0;
}

int		CtmTaskHmi::FreeSelf()
{
	//UnRegisterMessage(MSG_USER_HMI_INIT_MOLD_CLOSE, m_idMe);
	//UnRegisterMessage(MSG_USER_HMI_INIT_MOLD_OPEN, 	m_idMe);
	//UnRegisterMessage(MSG_USER_HMI_INIT_INJECT, 	m_idMe);
	//UnRegisterMessage(MSG_USER_HMI_INIT_HOLDPRESS, 	m_idMe);
	//UnRegisterMessage(MSG_USER_HMI_INIT_EJECT_ADV, 	m_idMe);
	//UnRegisterMessage(MSG_USER_HMI_INIT_EJECT_RET, 	m_idMe);
	//UnRegisterMessage(MSG_USER_HMI_INIT_AIR, 		m_idMe);
	//UnRegisterMessage(MSG_USER_HMI_INIT_CORE, 		m_idMe);
	UnRegisterMessage(MSG_WND_FORM_CREATE, 		m_idMe);
	
	return CtmPackClass::FreeSelf();
}

void	CtmTaskHmi::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case MSG_WND_FORM_CREATE:
			OnFormCreate();
			break;
	//	case MSG_USER_HMI_INIT_MOLD_CLOSE:
	//		Init_CloseMoldConfig(m_nRampConfig);
	//		break;
	//	case MSG_USER_HMI_INIT_MOLD_OPEN:
	//		Init_OpenMoldConfig(m_nRampConfig);
	//		break;
	//	case MSG_USER_HMI_INIT_INJECT:
	//		Init_InjectConfig(m_nRampConfig);
	//		break;
	//	case MSG_USER_HMI_INIT_HOLDPRESS:
	//		Init_HoldConfig(m_nRampConfig);
	//		break;
	//	case MSG_USER_HMI_INIT_EJECT_ADV:
	//		Init_ChargeConfig(m_nRampConfig);
	//		break;
	//	case MSG_USER_HMI_INIT_EJECT_RET:
	//		Init_EjtRETConfig(m_nRampConfig);
	//		break;
	//	case MSG_USER_HMI_INIT_AIR:
	//		Init_AirConfig();
	//		break;
	//	case MSG_USER_HMI_INIT_CORE:
	//		Init_CoreConfig();
	//		break;
		default:
			break;
	}
}

void	CtmTaskHmi::OnFormCreate()
{
	//如果是新的存儲模式，切換畫面，發現Machine、DA、systemconfig資料有修改就存Flash
	if(GetFileType(CHECKFILETYPE))
	{
		if(g_pDatabase->m_nDAChange > 0)
		{
			CtmBlockControl::GetInstance()->SetBlockIndex(BLOCK_NEWPOWERMATCH_ID);
			CtmBlockControl::GetInstance()->SaveAs(STORAGE_TYPE_FLASH);
			g_pDatabase->m_nDAChange = 0;
		}
		if(g_pDatabase->m_nMachineChange > 0)
		{
			CtmBlockControl::GetInstance()->SetBlockIndex(BLOCK_MACHINE_ID);
			CtmBlockControl::GetInstance()->SaveAs(STORAGE_TYPE_FLASH);
			CtmBlockControl::GetInstance()->SetBlockIndex(BLOCK_MACHINEB_ID);
			CtmBlockControl::GetInstance()->SaveAs(STORAGE_TYPE_FLASH);
			g_pDatabase->m_nMachineChange = 0;	
		}
		if(g_pDatabase->m_nSystemxChange > 0)
		{
			int nResult = 0;
			if(g_pFreeData != NULL)
			{
				nResult = g_pFreeData->LoadIDTable(g_pDatabase->m_pszSystemxName);
				g_pFreeData->SaveDataToFile(g_pDatabase->m_pszSystemxValue);
			}
			//CtmBlockControl::GetInstance()->SetBlockIndex(BLOCK_SYSTEMX_ID);
			//CtmBlockControl::GetInstance()->SaveAs(STORAGE_TYPE_FLASH);
			g_pDatabase->m_nSystemxChange = 0;	
		}
	}
}

void	CtmTaskHmi::InitData(char*	pszRank, char***	pszArys, int wData)
{
	char**	pszTemp = NULL;
	
	int i = 0, j = 0;
	BOOL	bFind = FALSE;
	long temp = GetDBValue(pszRank, &bFind).lValue;
	if (!bFind) return;
	while(pszArys[i] != NULL)
	{
		pszTemp = pszArys[i];
		
		j = 0;
		while(pszTemp[j] != NULL )
		{
			if ( j > (temp - 1))
			{
				SetDBValue(pszTemp[j], wData);
			}
			else   						
			{
				if((GetDBValue(pszTemp[j]).lValue == 0xFFFF)||(GetDBValue(pszTemp[j]).lValue == 0xFFFFFFFF))
				{
					WORD DefaultValue =GetDBDefaultValue(pszTemp[j]).lValue;
					SetDBValue(pszTemp[j],DefaultValue);
				}
			}
			j++;
		}		
		i++;
	}	
}

void	CtmTaskHmi::Init_AxialConfig(int nDealRamp)
{
	//=============
	//	close mold
	//=============
	
	//g_tmDebugInfo->PrintDebugInfo("4CtmInit::Init_AxialConfig\n");
	Init_CloseMoldConfig(nDealRamp);
	
	//=============
	//	open mold
	//=============	
	//printf("init open mold\n");
	Init_OpenMoldConfig(nDealRamp);
	
	//=============
	//	Inject
	//=============	
	//printf("init inject\n");
	Init_InjectConfig(nDealRamp);
	
	//=============
	//	Hold pressure
	//=============	
	//printf("init hold\n");
	Init_HoldConfig(nDealRamp);
	
	//=============
	//	charge
	//=============
	//printf("init charge\n");
	Init_ChargeConfig(nDealRamp);
	
	//Sunny<20070614> add
	//=============
	//	Eject
	//=============
	Init_EjtADVConfig(nDealRamp);
	Init_EjtRETConfig(nDealRamp);
	
	//=============
	//	air blast
	//=============
	//printf("init air\n");
	Init_AirConfig();
	
	//=============
	//	Core
	//=============
	//printf("init core\n");
	Init_CoreConfig();
	//=============
	//	KEB
	//=============
	//g_lKEBPulse		= GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED61").lValue;
	//g_lKEBUM		= GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED62").lValue;
	//g_lKEBRPM		= GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED63").lValue;
	//g_lKEBMMPerSec	= GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED64").lValue;
}


void	CtmTaskHmi::Init_CloseMoldConfig(int nDealRamp)
{
	printf("init close mold\n");
	// pressure for close mold
	char*	pszCloseMoldPres[] =
	{
		"MSET_MLD_CLOSE_STEP1_PRESSURE",
		"NULL							",
		"NULL							",
		"MSET_MLD_CLOSE_STEP2_PRESSURE",
		"MSET_MLD_CLOSE_STEP3_PRESSURE",		
		NULL
	};	
	//	speed for close mold
	char*	pszCloseMoldSpeed[] =
	{
		"MSET_MLD_CLOSE_STEP1_FLOW",
		"NULL							",
		"NULL							",
		"MSET_MLD_CLOSE_STEP2_FLOW",
		"MSET_MLD_CLOSE_STEP3_FLOW",
		NULL
	};	
	//	position for close mold
	char*	pszCloseMoldPos[] =
	{
		"MSET_MLD_CLOSE_STEP1_POSITION",
		"NULL							",
		"NULL							",
		"MSET_MLD_CLOSE_STEP2_POSITION",
		"MSET_MLD_CLOSE_STEP3_POSITION",
		NULL
	};	
	char*	pszCloseMoldPresRamp[] =
	{
		"MACH_MLD_CLOSE_RAMP_PRESTIME",
		"NULL",
		"NULL",
		"MACH_MLD_CLOSE_RAMP2_PRESTIME",
		"MACH_MLD_CLOSE_RAMP3_PRESTIME",
		NULL
	};
	char*	pszCloseMoldFlowRamp[] =
	{
		"MACH_MLD_CLOSE_RAMP_FLOWTIME",
		"NULL",
		"NULL",
		"MACH_MLD_CLOSE_RAMP2_FLOWTIME",
		"MACH_MLD_CLOSE_RAMP3_FLOWTIME",
		NULL
	};	
	char**	pszCloseMold[] =
	{
		pszCloseMoldPres, 
		pszCloseMoldSpeed, 
		pszCloseMoldPos,
		pszCloseMoldPresRamp,
		pszCloseMoldFlowRamp,
		NULL
	};
	
	if(nDealRamp == 0)
	{
		pszCloseMold[3] = NULL;
		pszCloseMold[4] = NULL;
	}
	
	InitData("MCONF_MOLD_CLOSE_NULL_RANK", pszCloseMold, 0xFFFF);
}

void	CtmTaskHmi::Init_OpenMoldConfig(int nDealRamp)
{
	//g_tmDebugInfo->PrintDebugInfo("5CtmInit::Init_OpenMoldConfig\n");
	//	pressure for open mold
	char*	pszOpenMoldPres[] =
	{
		"MSET_MLD_OPEN_STEP1_PRESSURE",
		"MSET_MLD_OPEN_STEP5_PRESSURE",
		"MSET_MLD_OPEN_STEP2_PRESSURE",
		"MSET_MLD_OPEN_STEP3_PRESSURE",
		"MSET_MLD_OPEN_STEP4_PRESSURE",
		
		NULL
	};	
	//	speed for open mold
	char*	pszOpenMoldSpeed[] =
	{
		"MSET_MLD_OPEN_STEP1_FLOW",
		"MSET_MLD_OPEN_STEP5_FLOW",
		"MSET_MLD_OPEN_STEP2_FLOW",
		"MSET_MLD_OPEN_STEP3_FLOW",
		"MSET_MLD_OPEN_STEP4_FLOW",
		
		NULL
	};	
	//	position for open mold
	char*	pszOpenMoldPos[] =
	{
		"MSET_MLD_OPEN_STEP1_POSITION",
		"MSET_MLD_OPEN_STEP5_POSITION",
		"MSET_MLD_OPEN_STEP2_POSITION",
		"MSET_MLD_OPEN_STEP3_POSITION",
		"MSET_MLD_OPEN_STEP4_POSITION",
		
		NULL
	};	
	char*	pszOpenMoldPresRamp[] =
	{
		"MACH_MLD_OPEN_RAMP_PRESTIME",
		"MACH_MLD_OPEN_RAMP5_PRESTIME",
		"MACH_MLD_OPEN_FASTRAMP_PRESTIME",
		"MACH_MLD_OPEN_RAMP3_PRESTIME",
		"MACH_MLD_OPEN_RAMP4_PRESTIME",
		NULL
	};
	char*	pszOpenMoldFlowRamp[] =
	{
		"MACH_MLD_OPEN_RAMP_FLOWTIME",
		"MACH_MLD_OPEN_FASTRAMP_FLOWTIME",
		"MACH_MLD_OPEN_RAMP3_FLOWTIME",
		"MACH_MLD_OPEN_RAMP4_FLOWTIME",
		"MACH_MLD_OPEN_RAMP52_FLOWTIME",
		NULL
	};	
	char**	pszOpenMold[] =
	{
		pszOpenMoldPres, 
		pszOpenMoldSpeed,
		pszOpenMoldPos,
		pszOpenMoldPresRamp,
		pszOpenMoldFlowRamp,
		NULL
	};
	
	if(nDealRamp == 0)
	{
		pszOpenMold[3] = NULL;
		pszOpenMold[4] = NULL;
	}
	InitData("MCONF_MOLD_OPEN_NULL_RANK", pszOpenMold, 0xFFFF);
}

void	CtmTaskHmi::Init_InjectConfig(int nDealRamp)
{
	//	pressure for inject
	char*	pszInjectPress[] =
	{
		"MSET_SCRW_INJ_STEP1_PRESSURE",
		"MSET_SCRW_INJ_STEP2_PRESSURE",
		"MSET_SCRW_INJ_STEP3_PRESSURE",
		"MSET_SCRW_INJ_STEP4_PRESSURE",
		"MSET_SCRW_INJ_STEP5_PRESSURE",
		"MSET_SCRW_INJ_STEP6_PRESSURE",
		NULL
	};	
	//	speed for inject
	char*	pszInjectSpeed[] =
	{
		"MSET_SCRW_INJ_STEP1_FLOW",
		"MSET_SCRW_INJ_STEP2_FLOW",
		"MSET_SCRW_INJ_STEP3_FLOW",
		"MSET_SCRW_INJ_STEP4_FLOW",
		"MSET_SCRW_INJ_STEP5_FLOW",
		"MSET_SCRW_INJ_STEP6_FLOW",
		NULL
	};	
	//	Position for inject
	char*	pszInjectPos[] =
	{
		"MSET_SCRW_INJ_STEP1_POSITION",			//	
		"MSET_SCRW_INJ_STEP2_POSITION",
		"MSET_SCRW_INJ_STEP3_POSITION",
		"MSET_SCRW_INJ_STEP4_POSITION",
		"MSET_SCRW_INJ_STEP5_POSITION",
		"MSET_SCRW_INJ_STEP6_POSITION",
		NULL
	};
	char*	pszInjectPresRamp[] =
	{
		"MACH_SCRW_INJ_RAMP_PRESTIME",
		"MACH_SCRW_INJ_RAMP2_PRESTIME",
		"MACH_SCRW_INJ_RAMP3_PRESTIME",
		"MACH_SCRW_INJ_RAMP4_PRESTIME",
		"MACH_SCRW_INJ_RAMP5_PRESTIME",
		"MACH_SCRW_INJ_RAMP6_PRESTIME",
		NULL
	};
	char*	pszInjectFlowRamp[] =
	{
		"MACH_SCRW_INJ_RAMP_FLOWTIME",
		"MACH_SCRW_INJ_RAMP2_FLOWTIME",
		"MACH_SCRW_INJ_RAMP3_FLOWTIME",
		"MACH_SCRW_INJ_RAMP4_FLOWTIME",
		"MACH_SCRW_INJ_RAMP5_FLOWTIME",
		"MACH_SCRW_INJ_RAMP6_FLOWTIME",
		NULL
	};	
	char**	pszInject[] =
	{
		pszInjectPress, 
		pszInjectSpeed,
		pszInjectPos,
		pszInjectPresRamp,
		pszInjectFlowRamp,
		NULL
	};
	if(nDealRamp==0)
	{
		pszInject[3] = NULL;
		pszInject[4] = NULL;	
	}
	InitData("MCONF_SCREW_INJECT_NULL_RANK", pszInject, 0xFFFF);
}

void	CtmTaskHmi::Init_HoldConfig(int nDealRamp)
{
	//	pressure for hold
	char*	pszHoldPres[] =
	{
		"MSET_SCRW_HLDP_STEP1_PRESSURE",
		"MSET_SCRW_HLDP_STEP2_PRESSURE",
		"MSET_SCRW_HLDP_STEP3_PRESSURE",
		"MSET_SCRW_HLDP_STEP4_PRESSURE",
		"MSET_SCRW_HLDP_STEP5_PRESSURE",
		"MSET_SCRW_HLDP_STEP6_PRESSURE",
		NULL
	};
	//	speed for hold
	char*	pszHoldSpeed[] =
	{
		"MSET_SCRW_HLDP_STEP1_FLOW",
		"MSET_SCRW_HLDP_STEP2_FLOW",
		"MSET_SCRW_HLDP_STEP3_FLOW",
		"MSET_SCRW_HLDP_STEP4_FLOW",
		"MSET_SCRW_HLDP_STEP5_FLOW",
		"MSET_SCRW_HLDP_STEP6_FLOW",
		NULL
	};
	//	position for hold
	char*	pszHoldPos[] =
	{
		"MSET_SCRW_HLDP_STEP1_TIME",
		"MSET_SCRW_HLDP_STEP2_TIME",
		"MSET_SCRW_HLDP_STEP3_TIME",
		"MSET_SCRW_HLDP_STEP4_TIME",
		"MSET_SCRW_HLDP_STEP5_TIME",
		"MSET_SCRW_HLDP_STEP6_TIME",
		NULL
	};
	char*	pszHoldPresRamp[] =
	{
		"MACH_SCRW_HLDP_RAMP_PRESTIME",
		"MACH_SCRW_HLDP_RAMP2_PRESTIME",
		"MACH_SCRW_HLDP_RAMP3_PRESTIME",
		"MACH_SCRW_HLDP_RAMP4_PRESTIME",
		"MACH_SCRW_HLDP_RAMP5_PRESTIME",
		"MACH_SCRW_HLDP_RAMP6_PRESTIME",
		NULL
	};
	char*	pszHoldFlowRamp[] =
	{
		"MACH_SCRW_HLDP_RAMP_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP2_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP3_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP4_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP5_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP6_FLOWTIME",
		NULL
	};	
	char**	pszHold[] =
	{
		pszHoldPres, 
		pszHoldSpeed,
		pszHoldPos,
		pszHoldPresRamp,
		pszHoldFlowRamp,
		NULL
	};
	
	if(nDealRamp==0)
	{
		pszHold[3] = NULL;
		pszHold[4] = NULL;	
	}
	if(CtmConfig::GetInstance()->GetKeyFlag()==0)
	{
		pszHoldPres[5] = NULL;
		pszHoldSpeed[5] = NULL;
		pszHoldPos[5] = NULL;
		pszHoldPresRamp[5] = NULL;
		pszHoldFlowRamp[5] = NULL;
	}
	InitData("MCONF_SCREW_HOLDPRESSURE_NULL_RANK", pszHold, 0xFFFFFFFF);
}

void	CtmTaskHmi::Init_ChargeConfig(int nDealRamp)
{
	//	pressure for charge
	char*	pszRcvPress[] =
	{
		"MSET_SCRW_RCV_STEP1_PRESSURE",
		"MSET_SCRW_RCV_STEP2_PRESSURE",
		"MSET_SCRW_RCV_STEP3_PRESSURE",
		"MSET_SCRW_RCV_STEP4_PRESSURE",
		"MSET_SCRW_RCV_STEP5_PRESSURE",
		NULL
	};
	//	speed for charge
	char*	pszRcvSpeed[] =
	{
		"MSET_SCRW_RCV_STEP1_FLOW",
		"MSET_SCRW_RCV_STEP2_FLOW",
		"MSET_SCRW_RCV_STEP3_FLOW",
		"MSET_SCRW_RCV_STEP4_FLOW",
		"MSET_SCRW_RCV_STEP5_FLOW",
		NULL
	};
	//	Position for charge
	char*	pszRcvPos[] =
	{
		"MSET_SCRW_RCV_STEP1_POSITION",
		"MSET_SCRW_RCV_STEP2_POSITION",
		"MSET_SCRW_RCV_STEP3_POSITION",
		"MSET_SCRW_RCV_STEP4_POSITION",
		"MSET_SCRW_RCV_STEP5_POSITION",
		NULL
	};
	//	back pressure for charge
	char*	pszRcvBackPress[] =
	{
		"MSET_SCRW_RCV_STEP1_BACKPRESSURE",
		"MSET_SCRW_RCV_STEP2_BACKPRESSURE",
		"MSET_SCRW_RCV_STEP3_BACKPRESSURE",
		"MSET_SCRW_RCV_STEP4_BACKPRESSURE",
		"MSET_SCRW_RCV_STEP5_BACKPRESSURE",
		NULL
	};
	char*	pszRcvPresRamp[] =
	{
		"MACH_SCRW_RCV_RAMP_PRESTIME",
		"MACH_SCRW_RCV_RAMP2_PRESTIME",
		"MACH_SCRW_RCV_RAMP3_PRESTIME",
		"MACH_SCRW_RCV_RAMP4_PRESTIME",
		"MACH_SCRW_RCV_RAMP5_PRESTIME",
		NULL
	};
	char*	pszRcvFlowRamp[] =
	{
		"MACH_SCRW_RCV_RAMP_FLOWTIME",
		"MACH_SCRW_RCV_RAMP2_FLOWTIME",
		"MACH_SCRW_RCV_RAMP3_FLOWTIME",
		"MACH_SCRW_RCV_RAMP4_FLOWTIME",
		"MACH_SCRW_RCV_RAMP5_FLOWTIME",
		NULL
	};	
	char**	pszRcv[] =
	{
		pszRcvPress, 
		pszRcvSpeed,
		pszRcvPos,
		pszRcvBackPress,
		pszRcvPresRamp,
		pszRcvFlowRamp,
		NULL
	};
	
	if(nDealRamp ==0)
	{
		pszRcv[4] = NULL;
		pszRcv[5] = NULL;	
	}
	InitData("MCONF_SCREW_RECOVERY_NULL_RANK", pszRcv, 0xFFFF);
}

void	CtmTaskHmi::Init_EjtADVConfig(int nDealRamp)
{
	//	pressure for EjtADV
	char*	pszRcvPress[] =
	{
		"MSET_EJT_FWD_STEP1_PRESSURE",
		"MSET_EJT_FWD_NULL_PRESSURE",
		NULL
	};
	//	speed for EjtADV
	char*	pszRcvSpeed[] =
	{
		"MSET_EJT_FWD_STEP1_FLOW",
		"MSET_EJT_FWD_NULL_FLOW",
		NULL
	};
	//	Position for EjtADV
	char*	pszRcvPos[] =
	{
		"MSET_EJT_FWD_STEP1_POSITION",
		"MSET_EJT_FWD_NULL_POSITION",
		NULL
	};
	char**	pszRcv[] =
	{
		pszRcvPress, 
		pszRcvSpeed,
		pszRcvPos,
		NULL
	};
	InitData("MCONF_EJECTOR_FORWARD_NULL_RANK", pszRcv, 0xFFFF);
}

void	CtmTaskHmi::Init_EjtRETConfig(int nDealRamp)
{
	//	pressure for EjtADV
	char*	pszRcvPress[] =
	{
		"MSET_EJT_BWD_NULL_VIBRATEPRESSURE",
		"MSET_EJT_BWD_NULL_PRESSURE",
		NULL
	};
	//	speed for EjtADV
	char*	pszRcvSpeed[] =
	{
		"MSET_EJT_BWD_NULL_VIBRATEFLOW",
		"MSET_EJT_BWD_NULL_FLOW",
		NULL
	};
	//	Position for EjtADV
	char*	pszRcvPos[] =
	{
		"MSET_EJT_BWD_STEP1_POSITION",
		"MSET_EJT_BWD_NULL_POSITION",
		NULL
	};
	char**	pszRcv[] =
	{
		pszRcvPress, 
		pszRcvSpeed,
		pszRcvPos,
		NULL
	};
	InitData("MCONF_EJECTOR_BACKWORD_NULL_RANK", pszRcv, 0xFFFF);
}

void	CtmTaskHmi::Init_AirConfig()
{
	//	Position for air
	char*	pszAirPos[] =
	{
		"MSET2_BLST1_ON_START_POSITION",
		"MSET2_MBLST1_ON_START_POSITION",
		"MSET2_BLST2_ON_START_POSITION",
		"MSET2_MBLST2_ON_START_POSITION",
		"MSET2_BLST3_ON_START_POSITION",
		"MSET2_MBLST3_ON_START_POSITION",
		NULL
	};
	//	delay time for air
	char*	pszAirDelayTime[] =
	{
		"MSET2_BLST1_ON_START_DELAYTIME",
		"MSET2_MBLST1_ON_START_DELAYTIME",
		"MSET2_BLST2_ON_START_DELAYTIME",
		"MSET2_MBLST2_ON_START_DELAYTIME",
		"MSET2_BLST3_ON_START_DELAYTIME",
		"MSET2_MBLST3_ON_START_DELAYTIME",
		NULL
	};
	//	total time for air
	char*	pszAirTotalTime[] =
	{
		"MSET2_BLST1_ON_NULL_TOTALTIME",
		"MSET2_MBLST1_ON_NULL_TOTALTIME",
		"MSET2_BLST2_ON_NULL_TOTALTIME",
		"MSET2_MBLST2_ON_NULL_TOTALTIME",
		"MSET2_BLST3_ON_NULL_TOTALTIME",
		"MSET2_MBLST3_ON_NULL_TOTALTIME",
		NULL
	};
	char**	pszAir[] =
	{
		pszAirPos, 
		pszAirDelayTime,
		pszAirTotalTime,
		NULL
	};
	InitData("MCONF_AIRBLAST_AIRBLAST_NULL_SET", pszAir, 0xFFFF);
}

void	CtmTaskHmi::Init_CoreConfig()
{
	//	function for core
	char*	pszCoreFunction[] =
	{
		"MSET_CORA_CORA_NULL_CONTROLOPTION",
		"MSET_CORB_CORB_NULL_CONTROLOPTION",
		"MSET_CORC_CORC_NULL_CONTROLOPTION",
		"MSET_CORD_CORD_NULL_CONTROLOPTION",
		"MSET_CORE_CORE_NULL_CONTROLOPTION",
		"MSET_CORF_CORF_NULL_CONTROLOPTION",
		"MSET2_CORG_CORG_NULL_CONTROLOPTION",
		"MSET2_CORH_CORH_NULL_CONTROLOPTION",
		NULL
	};
	//	in mode for core
	char*	pszCoreInMode[] =
	{
		"MSET_CORA_IN_NULL_MODEOPTION",
		"MSET_CORB_IN_NULL_MODEOPTION",
		"MSET_CORC_IN_NULL_MODEOPTION",
		"MSET_CORD_IN_NULL_MODEOPTION",
		"MSET_CORE_IN_NULL_MODEOPTION",
		"MSET_CORF_IN_NULL_MODEOPTION",
		"MSET2_CORG_IN_NULL_MODEOPTION",
		"MSET2_CORH_IN_NULL_MODEOPTION",
		NULL
	};
	//	input pressure for core
	char*	pszCoreInPress[] =
	{
		"MSET_CORA_IN_NULL_PRESSURE",
		"MSET_CORB_IN_NULL_PRESSURE",
		"MSET_CORC_IN_NULL_PRESSURE",
		"MSET_CORD_IN_NULL_PRESSURE",
		"MSET_CORE_IN_NULL_PRESSURE",
		"MSET_CORF_IN_NULL_PRESSURE",
		"MSET2_CORG_IN_NULL_PRESSURE",
		"MSET2_CORH_IN_NULL_PRESSURE",
		NULL
	};
	//	input Speed for core
	char*	pszCoreInSpeed[] =
	{
		"MSET_CORA_IN_NULL_FLOW",
		"MSET_CORB_IN_NULL_FLOW",
		"MSET_CORC_IN_NULL_FLOW",
		"MSET_CORD_IN_NULL_FLOW",
		"MSET_CORE_IN_NULL_FLOW",
		"MSET_CORF_IN_NULL_FLOW",
		"MSET2_CORG_IN_NULL_FLOW",
		"MSET2_CORH_IN_NULL_FLOW",
		NULL
	};
	//	input Time for core
	char*	pszCoreInTime[] =
	{
		"MSET_CORA_IN_NULL_TIME",
		"MSET_CORB_IN_NULL_TIME",
		"MSET_CORC_IN_NULL_TIME",
		"MSET_CORD_IN_NULL_TIME",
		"MSET_CORE_IN_NULL_TIME",
		"MSET_CORF_IN_NULL_TIME",
		"MSET2_CORG_IN_NULL_TIME",
		"MSET2_CORH_IN_NULL_TIME",
		NULL
	};
	//	input Counter for core
	char*	pszCoreInCount[] =
	{
		"MSET_CORA_IN_NULL_COUNTER",
		"MSET_CORB_IN_NULL_COUNTER",
		"MSET_CORC_IN_NULL_COUNTER",
		"MSET_CORD_IN_NULL_COUNTER",
		"MSET_CORE_IN_NULL_COUNTER",
		"MSET_CORF_IN_NULL_COUNTER",
		"MSET2_CORG_IN_NULL_COUNTER",
		"MSET2_CORH_IN_NULL_COUNTER",
		NULL
	};
	//	input act position point for core
	char*	pszCoreInActPoint[] =
	{
		"MSET_CORA_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORB_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORC_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORD_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORE_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORF_IN_NULL_ACTPOSITIONOPTION",
		"MSET2_CORG_IN_NULL_ACTPOSITIONOPTION",
		"MSET2_CORH_IN_NULL_ACTPOSITIONOPTION",
		NULL
	};
	//	input set mold's position  for core
	char*	pszCoreInSetPos[] =
	{
		"MSET_CORA_IN_NULL_SETMOLDPOSITION",
		"MSET_CORB_IN_NULL_SETMOLDPOSITION",
		"MSET_CORC_IN_NULL_SETMOLDPOSITION",
		"MSET_CORD_IN_NULL_SETMOLDPOSITION",
		"MSET_CORE_IN_NULL_SETMOLDPOSITION",
		"MSET_CORF_IN_NULL_SETMOLDPOSITION",
		"MSET2_CORG_IN_NULL_SETMOLDPOSITION",
		"MSET2_CORH_IN_NULL_SETMOLDPOSITION",
		NULL
	};	
	//	out mode for core
	char*	pszCoreOutMode[] =
	{
		"MSET_CORA_OUT_NULL_MODEOPTION",
		"MSET_CORB_OUT_NULL_MODEOPTION",
		"MSET_CORC_OUT_NULL_MODEOPTION",
		"MSET_CORD_OUT_NULL_MODEOPTION",
		"MSET_CORE_OUT_NULL_MODEOPTION",
		"MSET_CORF_OUT_NULL_MODEOPTION",
		"MSET2_CORG_OUT_NULL_MODEOPTION",
		"MSET2_CORH_OUT_NULL_MODEOPTION",
		NULL
	};
	//	out pressure for core
	char*	pszCoreOutPress[] =
	{
		"MSET_CORA_OUT_NULL_PRESSURE",
		"MSET_CORB_OUT_NULL_PRESSURE",
		"MSET_CORC_OUT_NULL_PRESSURE",
		"MSET_CORD_OUT_NULL_PRESSURE",
		"MSET_CORE_OUT_NULL_PRESSURE",
		"MSET_CORF_OUT_NULL_PRESSURE",
		"MSET2_CORG_OUT_NULL_PRESSURE",
		"MSET2_CORH_OUT_NULL_PRESSURE",
		NULL
	};
	//	out speed for core
	char*	pszCoreOutSpeed[] =
	{
		"MSET_CORA_OUT_NULL_FLOW",
		"MSET_CORB_OUT_NULL_FLOW",
		"MSET_CORC_OUT_NULL_FLOW",
		"MSET_CORD_OUT_NULL_FLOW",
		"MSET_CORE_OUT_NULL_FLOW",
		"MSET_CORF_OUT_NULL_FLOW",
		"MSET2_CORG_OUT_NULL_FLOW",
		"MSET2_CORH_OUT_NULL_FLOW",
		NULL
	};
	//	out Time for core
	char*	pszCoreOutTime[] =
	{
		"MSET_CORA_OUT_NULL_TIME",
		"MSET_CORB_OUT_NULL_TIME",
		"MSET_CORC_OUT_NULL_TIME",
		"MSET_CORD_OUT_NULL_TIME",
		"MSET_CORE_OUT_NULL_TIME",
		"MSET_CORF_OUT_NULL_TIME",
		"MSET2_CORG_OUT_NULL_TIME",
		"MSET2_CORH_OUT_NULL_TIME",
		NULL
	};
	//	out count for core
	char*	pszCoreOutCount[] =
	{
		"MSET_CORA_OUT_NULL_COUNTER",
		"MSET_CORB_OUT_NULL_COUNTER",
		"MSET_CORC_OUT_NULL_COUNTER",
		"MSET_CORD_OUT_NULL_COUNTER",
		"MSET_CORE_OUT_NULL_COUNTER",
		"MSET_CORF_OUT_NULL_COUNTER",
		"MSET2_CORG_OUT_NULL_COUNTER",
		"MSET2_CORH_OUT_NULL_COUNTER",
		NULL
	};
	//	out act point for core
	char*	pszCoreOutActPoint[] =
	{
		"MSET_CORA_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORB_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORC_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORD_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORE_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORF_OUT_NULL_ACTPOSITIONOPTION",
		"MSET2_CORG_OUT_NULL_ACTPOSITIONOPTION",
		"MSET2_CORH_OUT_NULL_ACTPOSITIONOPTION",
		NULL
	};
	//	out set mold's position for core
	char*	pszCoreOutSetPos[] =
	{
		"MSET_CORA_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORB_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORC_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORD_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORE_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORF_OUT_NULL_SETMOLDPOSITION",
		"MSET2_CORG_OUT_NULL_SETMOLDPOSITION",
		"MSET2_CORH_OUT_NULL_SETMOLDPOSITION",
		NULL
	};
	char**	pszCore[] =
	{
		pszCoreFunction, 
		pszCoreInMode,
		pszCoreInPress,
		pszCoreInSpeed,
		pszCoreInTime,
		pszCoreInCount,
		pszCoreInActPoint,
		pszCoreInSetPos,
		pszCoreOutMode,
		pszCoreOutPress,
		pszCoreOutSpeed,
		pszCoreOutTime,
		pszCoreOutCount,
		pszCoreOutActPoint,
		pszCoreOutSetPos,
		NULL
	};
	InitData("MCONF_COREPULLER_COREPULLER_NULL_SET", pszCore, 0xFFFF);
}


#ifndef	D_BEOBJECT_CTMTASKHMI
CObject*	CreateInstance(char* Name)
{
	if (g_pTaskHmi == NULL)
	{	
		CObject*	pResult = Create(Name);
	
		if (pResult != NULL)
		{
			g_pTaskHmi = static_cast<CtmTaskHmi*>(pResult);
			g_pTaskHmi->CreateSelf();
			
			
		}
	}
	
	return g_pTaskHmi;
	
}

void	ReleaseInstance(CObject* pInstance)
{
	if (g_pTaskHmi != NULL)
		(static_cast<CtmTaskHmi*>(g_pTaskHmi))->FreeSelf();
	delete g_pTaskHmi;
	g_pTaskHmi = NULL;
}
#endif
