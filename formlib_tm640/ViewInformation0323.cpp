/*============================================================================+
|  Class    : ViewManualMode library                	                  			|
|  Task     : ViewManualMode library  header file  	  	                 			|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewInformation.h"
#include	"../tmdebug.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"
#include	"../tmshmsg.h"
#include 	"../taskcnc.h"
/*============================================================================+
|           Constant                                                     			|
+============================================================================*/

#define  UPDATEYEAR 2018
#define  UPDATEMONTH 03
#define  UPDATEDAY 05
#define  UPDATEVERSION 1
/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/
CtmWnd*		u_pwndEditFactoryDate[3] 		= {NULL};
int				u_nEditFactoryDateCnt    		= 0;
CtmWnd*		u_pwndEditTradeDate[3] 			= {NULL};
int				u_nEditTradeDateCnt    			= 0;
CtmWnd*		u_pwndStaticMachTable[8] 		= {NULL};
int				u_nStaticMachTableCnt   		= 0;
CtmWnd*		u_pwndStaticComTalbe[3] 		= {NULL};
int				u_nStaticComTalbeCnt    		= 0;
CtmWnd*		u_pwndStaticVersionTable[3] = {NULL};
int				u_nStaticVersionTableCnt    = 0;


CtmWnd*		u_pwndEditMachineNO	=	NULL;

/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndEditMachineNO			= pwndSender->FindControlFromName("EditMachineNO");
	
	u_nEditFactoryDateCnt			= GetSpecialControlNum(pwndSender, "EditFactoryDate", "CtmEditX1", 	u_pwndEditFactoryDate);
	u_nEditTradeDateCnt				= GetSpecialControlNum(pwndSender, "EditTradeDate"	, "CtmEditX1", 	u_pwndEditTradeDate);
	u_nStaticMachTableCnt			= GetSpecialControlNum(pwndSender, "EditTradeDate"	, "CtmStaticX2", u_pwndStaticMachTable);
	u_nStaticComTalbeCnt			= GetSpecialControlNum(pwndSender, "StaticComTalbe"	, "CtmStaticX2", u_pwndStaticComTalbe);
	u_nStaticVersionTableCnt	= GetSpecialControlNum(pwndSender, "StaticVersionTable"	, "CtmStaticX2", u_pwndStaticVersionTable);
	UpdateData();
	
	return	TRUE;
}  

/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/
void 	UpdateData()
{
	//手臂機型
	//MotionDataShow();
	//版本
	VersionShow();
	//機台資訊
	MachineShow();
	//公司資訊
	//CompanyShow();
	
}


void	MotionDataShow()
{
	//u_pwndStaticMachTable
}

void	VersionShow()
{
	char HMIVersion[10], CNCVersion[10], PLCVersion[10];
	char two_HMIVersion[20], two_CNCVersion[20], two_PLCVersion[20];
	GetHMIVersion(HMIVersion);
	strcpy(CNCVersion,g_pRxCNCData->CNC_nc_version);
	strcpy(PLCVersion,g_pRxCNCData->CNC_plc_version);
	CodeChange(two_HMIVersion,HMIVersion);
	CodeChange(two_CNCVersion,CNCVersion);
	CodeChange(two_PLCVersion,PLCVersion);
	u_pwndStaticVersionTable[0]->SetPropValueT("text",two_HMIVersion);
	u_pwndStaticVersionTable[0]->Update();
	u_pwndStaticVersionTable[1]->SetPropValueT("text",two_CNCVersion);
	u_pwndStaticVersionTable[1]->Update();
	u_pwndStaticVersionTable[2]->SetPropValueT("text",two_PLCVersion);
	u_pwndStaticVersionTable[2]->Update();
	//
}

void	MachineShow()
{
	char temp[10];
	memset(temp,0,10);
	GetDBString("OPER_HMI_NULL_NULL_MACHINEID",temp,9);
	u_pwndEditMachineNO			->SetPropValueT("text",temp);
	
	u_pwndEditFactoryDate[0]	->SetPropValueT("value",GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR").lValue);
	u_pwndEditFactoryDate[1]	->SetPropValueT("value",GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH").lValue);
	u_pwndEditFactoryDate[2]	->SetPropValueT("value",GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY").lValue);
	u_pwndEditTradeDate[0]		->SetPropValueT("value",GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLYEAR").lValue);
	u_pwndEditTradeDate[1]		->SetPropValueT("value",GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLMONTH").lValue);
	u_pwndEditTradeDate[2]		->SetPropValueT("value",GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLDAY").lValue);
	
		
	u_pwndEditMachineNO				->Update();
	u_pwndEditFactoryDate[0]	->Update();
	u_pwndEditFactoryDate[1]	->Update();
	u_pwndEditFactoryDate[2]	->Update();
	u_pwndEditTradeDate[0]		->Update();
	u_pwndEditTradeDate[1]		->Update();
	u_pwndEditTradeDate[2]		->Update();
	
}

void	CompanyShow()
{
	//u_pwndStaticComTalbe
}

void	GetHMIVersion(char* VerStr)
{
	//char temp_Version[10];
	char mon_str,day_str;
	int temp1 = 0;
	
	temp1=UPDATEMONTH;
  if(temp1<10) mon_str=temp1+'1'-1;
  else  mon_str=temp1+'A'-10;
  
	temp1=UPDATEDAY;
  if(temp1<10) day_str=temp1+'1'-1;
  else  day_str=temp1+'A'-10;
	
	sprintf(VerStr,"R%c%c%dH%02d",mon_str,day_str,UPDATEYEAR%100,UPDATEVERSION);
	//printf("VerStr is %s\n",VerStr);
}

void    CodeChange(char *pDst, char *pSrc)		//輔助用函式，單字節轉化為雙字節處理
{
	//供StaticX2元件的text屬性使用(目前)
	WORD	*pwDst = (WORD *)pDst;
    
  while(*pSrc != 0)      
  	*pwDst++ = *pSrc++;
    
  *pwDst =0;  
}