/*==========================================================================+
|  Class    : kb.cpp														|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : yang                                                       	|
|  Version  : V1.00                                                         |
|  Creation : 2005/01/18                                                    |
|			: 2005/01/31	netdigger										|
|  Remark	: 																|
+==========================================================================*/
#include	"_kb.h"
#include	"main.h"
#include	"utils.h"
#include	"tmconfig.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
IMPLEMENT_DYNCREATE(CtmKeyBoard, CtmPackClass)

CtmKeyBoard*	g_ptaskKB = NULL;

CtmKeyBoard::CtmKeyBoard() : CtmPackClass()
{
	
}

CtmKeyBoard::~CtmKeyBoard()
{
	
}


WORD	_SetLED(WORD wLEDID)
{
	if (g_ptaskKB != NULL)
	{
		 g_ptaskKB->SetLED(wLEDID);
		 return wLEDID;
	}
	else return 0;	
}

WORD	_GetKey()
{
	if (g_ptaskKB != NULL)
	{
		return g_ptaskKB->GetKey();
	}
	else return 0;
	
}



void	_PutKeyTail(WORD keycode)
{
	if (g_ptaskKB != NULL)
	{
		g_ptaskKB->PutKeyTail(keycode);
	}
}

WORD	_PowerOnLED(WORD wLEDID)
{
	if (g_ptaskKB != NULL)
	{
		return g_ptaskKB->PowerOnLED(wLEDID);
	}
	else return 0;	
}

WORD	_PowerOffLED(WORD wLEDID)
{
	if (g_ptaskKB != NULL)
	{
		return g_ptaskKB->PowerOffLED(wLEDID);
	}
	else return 0;	
}

WORD	_TestKeyFlag(FPKEYFLAG fpKeyFlag)
{
	if (g_ptaskKB != NULL)
	{
		return g_ptaskKB->TestKeyFlag(fpKeyFlag);
	}
	else return 0;	
}

void	_Save_Restore_LED(int nState)
{
	if (g_ptaskKB != NULL) g_ptaskKB->TestLed(nState);
}


void	_ActLed(int nActionIndex)
{
	if (g_ptaskKB != NULL) g_ptaskKB->ActLed(nActionIndex);
}

void	_UnActLed(int nActionIndex)
{
	if (g_ptaskKB != NULL) g_ptaskKB->UnActLed(nActionIndex);
}

int		_GetActLed(int nActionIndex)
{
	if (g_ptaskKB != NULL) 	return g_ptaskKB->GetActLed(nActionIndex);
	else					return -1;
}

int		_GetKeyMap(int nKeyIndex)
{
	if (g_ptaskKB != NULL) 	return g_ptaskKB->GetKeyMap(nKeyIndex);
	else					return -1;
}


#ifdef		D_ARM
//void	PanelSensorAutoOn()
//{
//	if (g_ptaskKB != NULL)
//	{
//		if(CtmConfig::GetInstance()->GetKeyFlag()==1 )	//Sunny<20080407> add for 7HT keymap
//		{
//			g_ptaskKB->SetLED(LED_PNL_HT_SENSORAUTO);
//		}
//		else g_ptaskKB->SetLED(LED_PNL_SENSORAUTO);
//	}
//} 
//void	PanelSensorAutoOff()
//{
//	if (g_ptaskKB != NULL)
//	{
//		if(CtmConfig::GetInstance()->GetKeyFlag()==1 )	//Sunny<20080407> add for 7HT keymap
//		{
//			g_ptaskKB->SetLED(LED_PNL_HT_SENSORAUTOOFF);
//		}
//		else g_ptaskKB->SetLED(LED_PNL_SENSORAUTOOFF);
//	}
//} 
//void	PanelMacroAdjOff()
//{
//	if (g_ptaskKB != NULL)
//	{
//		if(CtmConfig::GetInstance()->GetKeyFlag()==1 )	//Sunny<20080407> add for 7HT keymap
//		{
//			g_ptaskKB->SetLED(LED_PNL_HT_MACROADJOFF);
//		}
//		else g_ptaskKB->SetLED(LED_PNL_MACROADJOFF);
//	}
//}  
//
//void	PanelMotorOff()
//{
//	if (g_ptaskKB != NULL)
//	{
//		if(CtmConfig::GetInstance()->GetKeyFlag()==1 )	//Sunny<20080407> add for 7HT keymap
//		{
//			g_ptaskKB->SetLED(LED_PNL_HT_MOTOROFF);
//		}
//		else g_ptaskKB->SetLED(LED_PNL_MOTOROFF);
//	}
//} 
//
//void	PanelHeaterOn()
//{
//	if (g_ptaskKB != NULL)
//	{
//		if(CtmConfig::GetInstance()->GetKeyFlag()==1 )	//Sunny<20080407> add for 7HT keymap
//		{
//			g_ptaskKB->SetLED(LED_PNL_HT_HEATER);
//		}
//		else g_ptaskKB->SetLED(LED_PNL_HEATER);
//	}
//} 
//void	PanelHeaterOff()
//{
//	if (g_ptaskKB != NULL)
//	{
//		if(CtmConfig::GetInstance()->GetKeyFlag()==1 )	//Sunny<20080407> add for 7HT keymap
//		{
//			//g_ptaskKB->SetLED(LED_PNL_HT_HEATEROFF);
//			
//			_PowerOffLED(LED_PNL_HT_HEATER);
//		}
//		else g_ptaskKB->SetLED(LED_PNL_HEATEROFF);
//	}
//} 
#endif
