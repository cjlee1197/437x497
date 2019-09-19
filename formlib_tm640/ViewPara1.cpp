/*===========================================================================+
|  Class    : ViewClmpP2 library                                               |
|  Task     : ViewClmpP2 library  header file                                  |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/

#include	"ViewPara1.h"
#include	"../main.h"
#include	"../utils.h"
#include	"../iotable.h"

#define 	PARA13_NAME			"editPARAMETER1_OTHER13"
#define 	PARA14_NAME			"editPARAMETER1_OTHER14"
#define		PARA29_NAME			"editPARAMETER1_OTHER29"	//	machine tone

#define 	LUB10_NAME			"editPARAMETER1_LUBR10"     // 自動潤滑1模數
#define 	LUB11_NAME			"editPARAMETER1_LUBR11"     // 潤滑警報時間
#define 	LUB12_NAME			"editPARAMETER1_LUBR12"     // 自動潤滑1計時    
#define 	LUB13_NAME			"editPARAMETER1_LUBR13"     // 自動潤滑1延遲
#define 	LUB14_NAME			"editPARAMETER1_LUBR14"     // 自動潤滑1總計時
#define 	LUB15_NAME			"editPARAMETER1_LUBR15"     // 自動潤滑次數
#define 	LUB16_NAME			"editPARAMETER1_LUBR16"     // 潤滑檢測次數
#define 	LUB17_NAME			"editPARAMETER1_LUBR17"     // 開機后前2模不做潤滑動作
#define 	LUB20_NAME			"editPARAMETER1_LUBR20"     // 第二組自動潤滑模數
#define 	LUB21_NAME			"editPARAMETER1_LUBR21"     // 第二組自動潤滑計時

#define		EDIT_NAME_MOTORTEMP	"editPARAMETER1_MOTORTEMP"

char*	pszCommandEditName[] = 
{
	"editPARAMETER1_OTHER37", 
	"editPARAMETER1_OTHER38",
	"editPARAMETER1_OTHER39",   //ZHBA  2008-10-6  add
	"editPARAMETER1_OTHER3A",  // YANGJX 為系統流量二增加
	/*"editPARAMETER1_OTHER40",
	"editPARAMETER1_OTHER41",
	"editPARAMETER1_OTHER42",
	"editPARAMETER1_OTHER43",
	"editPARAMETER1_OTHER44",
	"editPARAMETER1_OTHER45",
	"editPARAMETER1_OTHER46",
	"editPARAMETER1_OTHER47",
	"editPARAMETER1_OTHER48"*/
};

CtmWnd*	pwndMeterOther37 = NULL;
CtmWnd*	pwndMeterOther38 = NULL;
CtmWnd*	pwndMeterOther33 = NULL;
CtmWnd*	pwndMeterOther35 = NULL;
CtmWnd* pwndPara13 = NULL;
CtmWnd* pwndPara14 = NULL;
CtmWnd*	pwndPara29 = NULL;

CtmWnd* pwndLub10  = NULL;
CtmWnd* pwndLub11  = NULL;
CtmWnd*	pwndLub12  = NULL;
CtmWnd* pwndLub13  = NULL;
CtmWnd* pwndLub14  = NULL;
CtmWnd*	pwndLub15  = NULL;
CtmWnd* pwndLub16  = NULL;
CtmWnd* pwndLub17  = NULL;
CtmWnd* pwndLub20  = NULL;
CtmWnd*	pwndLub21  = NULL;
CtmWnd*	pwndeOTHER15  = NULL;  //2008-12-12 14:18:11
CtmWnd*	pwndeOTHER16  = NULL;   //2009-10-10 10:18:54

CtmWnd* pwndLUBWORKTIME   = NULL;   //ZHBA  2009-4-24
CtmWnd* pwndLUBONPCTIME   = NULL;   //ZHBA  2009-4-24
CtmWnd* pwndLUBOFFPCTIME  = NULL;   //ZHBA  2009-4-24
CtmWnd* pwndLUB2WORKTIME  = NULL;   //ZHBA  2009-4-24
CtmWnd* pwndLUB2ONPCTIME  = NULL;   //ZHBA  2009-4-24
CtmWnd* pwndLUB2OFFPCTIME = NULL;   //ZHBA  2009-4-24

CtmWnd* pwndSelectBox0  = NULL;   //ZHBA  2009-4-24
CtmWnd* pwndSelectBox1 = NULL;   //ZHBA  2009-4-24

CtmWnd*	pwndMotorTemp  = NULL;

CtmWnd* pwndMOLDFLOWPERCENT  = NULL;   //ZHBA  2009-9-17
CtmWnd* pwndCHRGFLOWPERCENT  = NULL;   //ZHBA  2009-9-17
CtmWnd* pwndOTHERFLOWPERCENT = NULL;   //ZHBA  2009-9-17

CtmWnd* pwndTIMEAUTOTIME   	= NULL;
CtmWnd* pwndSEMIAUTOTIME		= NULL;

CtmWnd*   pwndRampDown[16]; //ZHBA 2011-1-25
CtmWnd*   pwndSRampDown[16]; //ZHBA 2011-1-25
int		    iRampDown  = 0;   //ZHBA 2011-1-25
int		    iSRampDown  = 0;   //ZHBA 2011-1-25
CtmWnd*		pwndStaticRampDown		= NULL;//ZHBA 2011-1-2
CtmWnd*		pwndStaticValveEarlyTime		= NULL;// YANGJX 2011-2-21 閥提前時間
CtmWnd*		pwndEditValveEarlyTime		= NULL;
CtmWnd*		pwndStaticPValveEarlyTime		= NULL;// YANGJX 2011-2-21 比例閥提前時間
CtmWnd*		pwndEditPValveEarlyTime		= NULL;

CtmWnd* pwndEDITLUBCOUNTS2TIME = NULL;   // YANGJX 2010-7-197YJ處理兩個Word轉為Dword

CtmWnd* m_pwndEditOperationHour  = NULL;
CtmWnd*	  pwndOPENCOUNTDOWN  = NULL;
CtmWnd*	  pwndMotorAutoOff   = NULL;
CtmWnd*	  pwndTempAutoWarm   = NULL;
CtmWnd*	  pwndEditAll[256]		={NULL};

char psz[256];
int   	OldData = -1;
long	lMTempOptOld,lMTempOpt;
BOOL 		m_b7TR  	=	FALSE;
BOOL 		b_7AKBMC  	=	FALSE;
int		EditNum	= 0;
int		EditValue = 0;

long long lTemp,lTemp1,lTemp2;
long long lMoldFP,lChrgFP,lOtherFP;

void	GetData(CtmWnd * pwnd);
void	SetData(CtmWnd * pwnd);
void	SetDisabledControl();
BOOL	OnCreateA(CtmWnd* pwndSender)
{
//    BOOL         bVisible =FALSE;
//    DWORD        dwValue;
//	DWORD dwValue = GetDBValue("CONF_CODE_WRITE_NULL_DATA4").lValue;
	
	pwndPara13 = pwndSender->FindControlFromName(PARA13_NAME);
	pwndPara14 = pwndSender->FindControlFromName(PARA14_NAME);
	//pwndPara29 = pwndSender->FindControlFromName(PARA29_NAME);	 
    pwndLub10  = pwndSender->FindControlFromName(LUB10_NAME);
    pwndLub11  = pwndSender->FindControlFromName(LUB11_NAME);
    pwndLub12  = pwndSender->FindControlFromName(LUB12_NAME);
    pwndLub13  = pwndSender->FindControlFromName(LUB13_NAME);
    pwndLub14  = pwndSender->FindControlFromName(LUB14_NAME);
    pwndLub15  = pwndSender->FindControlFromName(LUB15_NAME);
    pwndLub16  = pwndSender->FindControlFromName(LUB16_NAME);
    pwndLub17  = pwndSender->FindControlFromName(LUB17_NAME);
    pwndLub20  = pwndSender->FindControlFromName(LUB20_NAME);
    pwndLub21  = pwndSender->FindControlFromName(LUB21_NAME);
    pwndMeterOther37 = pwndSender->FindControlFromName("editPARAMETER1_OTHER37");			
    pwndMeterOther38 = pwndSender->FindControlFromName("editPARAMETER1_OTHER38");
    pwndMeterOther33 = pwndSender->FindControlFromName("editPARAMETER1_OTHER33");
    pwndMeterOther35 = pwndSender->FindControlFromName("editPARAMETER1_OTHER35");
    m_pwndEditOperationHour = pwndSender->FindControlFromName("editOperationHour");
    pwndMotorTemp  = pwndSender->FindControlFromName(EDIT_NAME_MOTORTEMP);	//fuxy,2008-12-9, add for 7HX, Motor Temperature
    if(pwndMotorTemp != NULL)
    	pwndMotorTemp->GetPropValueT("value", &lMTempOptOld,sizeof(lMTempOptOld));
    	
   iRampDown  =  GetSpecialControlNum(pwndSender, "RAMPDOWN",  "CtmEditX1", pwndRampDown);
  iSRampDown =  GetSpecialControlNum(pwndSender, "RAMPDOWN",  "CtmStaticX2", pwndSRampDown);
  pwndStaticRampDown          = pwndSender->FindControlFromName("StaticRAMPDOWN");
  pwndStaticValveEarlyTime    = pwndSender->FindControlFromName("StaticVW_PARA_VALVEEARLYTIM");// YANGJX 2011-2-21 閥提前時間
  pwndEditValveEarlyTime      = pwndSender->FindControlFromName("editVALVEEARLYTIM");
  pwndStaticPValveEarlyTime   = pwndSender->FindControlFromName("StaticVW_PARA_PVALVEEARLYTIM");// YANGJX 2011-2-21 比例閥提前時間
  pwndEditPValveEarlyTime     = pwndSender->FindControlFromName("editPVALVEEARLYTIM");
  
  EditNum		= GetSpecialControlNum(pwndSender,	"edit"	,	"CtmEditX1",	pwndEditAll);
  
  pwndTIMEAUTOTIME		= pwndSender->FindControlFromName("editSEMIAUTOTIME");   
	pwndSEMIAUTOTIME	= pwndSender->FindControlFromName("editTIMEAUTOTIME");    
  pwndOPENCOUNTDOWN = pwndSender->FindControlFromName("editCLAMPOPENCOUNTDOWN");     
  pwndMotorAutoOff  = pwndSender->FindControlFromName("editPARAMETER1_AUTOOFF");
  pwndTempAutoWarm  = pwndSender->FindControlFromName("editPARAMETER1_AUTOWARM");
   
   m_b7TR 		= g_pExTable->GetTableValue(0,"D_7TR");
   if(g_pExTable->GetTableValue(0,"D_7AKBMC") || g_pExTable->GetTableValue(0,"D_7TYBMC"))
   	b_7AKBMC 	= TRUE;
   
   SetSegment();
   
	long long lMaxSysPres 	  = HEX2BCD(GetDBValue("CONF_CODE_WRITE_NULL_MAXPRESSURE").lValue);				//系統壓力上限值
  	long long lMaxHydraulic1  = GetDBValue("CONF_PVL1_READ_NULL_MAXDATA").lValue;	//比例閥1最大值, //ZHBA  2008-7-1
        if(lMaxSysPres == 0)  lMaxSysPres = 140;
  else	if(lMaxSysPres == 1) 	lMaxSysPres = lMaxHydraulic1;
    
    if(pwndMeterOther37!=NULL)
    	{    	
				pwndMeterOther37->SetPropValueT("max", lMaxSysPres);
				pwndMeterOther37->Update();
		}

       if(pwndMeterOther33!=NULL)
       	{
				pwndMeterOther33->SetPropValueT("max", lMaxSysPres);	
				pwndMeterOther33->Update();
		}
			 if(pwndMeterOther35!=NULL)
       	{
				pwndMeterOther35->SetPropValueT("max", lMaxSysPres);	
				pwndMeterOther35->Update();
		}

	GetData(pwndPara13);
	GetData(pwndPara14);
	if(b_7AKBMC)
	{
		GetData(pwndMotorAutoOff);
		GetData(pwndTempAutoWarm);
	}

   pwndLUBWORKTIME   = pwndSender->FindControlFromName("editPARAMETER1_LUBWORKTIME");   //ZHBA  2009-4-24
   pwndLUBONPCTIME   = pwndSender->FindControlFromName("editPARAMETER1_LUBONPCTIME");   //ZHBA  2009-4-24
   pwndLUBOFFPCTIME  = pwndSender->FindControlFromName("editPARAMETER1_LUBOFFPCTIME");   //ZHBA  2009-4-24
   pwndLUB2WORKTIME  = pwndSender->FindControlFromName("editPARAMETER1_LUB2WORKTIME");   //ZHBA  2009-4-24
   pwndLUB2ONPCTIME  = pwndSender->FindControlFromName("editPARAMETER1_LUB2ONPCTIME");   //ZHBA  2009-4-24
   pwndLUB2OFFPCTIME = pwndSender->FindControlFromName("editPARAMETER1_LUB2OFFPCTIME");  //ZHBA  2009-4-24
   pwndSelectBox0    = pwndSender->FindControlFromName("SelectBox0");  //ZHBA  2009-4-24
   pwndSelectBox1    = pwndSender->FindControlFromName("SelectBox1");  //ZHBA  2009-4-24
   
   pwndEDITLUBCOUNTS2TIME= pwndSender->FindControlFromName("editLUBCOUNTS2TIME");  // YANGJX 2010-7-197YJ處理兩個Word轉為Dword

////ZHBA  2009-4-24 處理兩個Word轉為Dword

if(pwndLUBWORKTIME   != NULL)
	{
	lTemp1 =GetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE9").lValue;
	lTemp2 =GetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE10").lValue ;
    GetDWordValve(pwndLUBWORKTIME);

	pwndLUBONPCTIME->SetPropValueT("max", lTemp);	
	pwndLUBONPCTIME->Update();
	pwndLUBOFFPCTIME->SetPropValueT("max", lTemp);	
	pwndLUBOFFPCTIME->Update();
	}
if(pwndLUBONPCTIME   != NULL)
	{
	lTemp1 =GetDBValue("MACH_ROTA_ROTA_STEP1_POSITION").lValue;
	lTemp2 =GetDBValue("MACH_ROTA_ROTA_STEP2_POSITION").lValue ;
   GetDWordValve(pwndLUBONPCTIME);

	}
if(pwndLUBOFFPCTIME   != NULL)
	{
	lTemp1 =GetDBValue("MACH_ROTA_ROTA_STEP3_POSITION").lValue;
	lTemp2 =GetDBValue("MACH_ROTA_ROTA_NULL_COUNT2RATATE").lValue ;
    GetDWordValve(pwndLUBOFFPCTIME);
	}
if(pwndLUB2WORKTIME   != NULL)
	{
	lTemp1 =GetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE15").lValue;
	lTemp2 =GetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE16").lValue ;
    GetDWordValve(pwndLUB2WORKTIME);
	pwndLUB2ONPCTIME->SetPropValueT("max", lTemp);	
	pwndLUB2ONPCTIME->Update();
	pwndLUB2OFFPCTIME->SetPropValueT("max", lTemp);
	pwndLUB2OFFPCTIME->Update();
	}
if(pwndLUB2ONPCTIME   != NULL)
	{
	lTemp1 =GetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE11").lValue;
	lTemp2 =GetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE12").lValue ;
    GetDWordValve(pwndLUB2ONPCTIME);
	}
if(pwndLUB2OFFPCTIME   != NULL)
	{
	lTemp1 =GetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE13").lValue;
	lTemp2 =GetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE14").lValue ;
    GetDWordValve(pwndLUB2OFFPCTIME);
	}
////ZHBA  2009-4-24 處理兩個Word轉為Dword
	WORD  PBValue =(WORD)GetDBValue("STATE_IO1_READ_NULL_REALDATA2").lValue;
	BOOL  bSelected1  = FALSE;
	BOOL  bSelected2 = FALSE;
     
//ZHBA  2009-4-24 處理PB for 7LG
   if(pwndSelectBox0 != NULL)
   {
     if(_TestBit(PBValue, 13))	 bSelected1 = TRUE;
	 else 					     bSelected1 = FALSE;
	 pwndSelectBox0->SetPropValueT("selected", bSelected1);
	 }
   
   if(pwndSelectBox1 != NULL)
   {
     if(_TestBit(PBValue, 14))	 bSelected2 = TRUE;
	 else 					     bSelected2 = FALSE;
	 pwndSelectBox1->SetPropValueT("selected", bSelected2);
   }
   
   if(g_pExTable->GetTableValue(0,"D_7YJDWORD"))// YANGJX 2010-7-19 7YJ7YJ處理兩個Word轉為Dword
		{
	    if(pwndEDITLUBCOUNTS2TIME   != NULL)
	    {
	    lTemp1 =GetDBValue("MACH_SCRW_INJ_NULL_W018A").lValue;
	    lTemp2 =GetDBValue("MACH_SCRW_INJ_NULL_W018B").lValue ;
      GetDWordValve(pwndEDITLUBCOUNTS2TIME);
      }
    }
    
    if(g_pExTable->GetTableValue(0,"D_7ASFASTMACH"))// YANGJX 2011-3-5 海雄快速機
    	{
    		pwndMOLDFLOWPERCENT  = pwndSender->FindControlFromName("editPARAMETER1_MOLDFLOWPERCENT");
  	    pwndCHRGFLOWPERCENT  = pwndSender->FindControlFromName("editPARAMETER1_CHRGFLOWPERCENT");
  	    pwndOTHERFLOWPERCENT = pwndSender->FindControlFromName("editPARAMETER1_OTHERFLOWPERCENT");
        
        lMoldFP =GetDBValue("SYS_HMI_NULL_NULL_RESERVED39").lValue;
        lChrgFP =GetDBValue("SYS_HMI_NULL_NULL_RESERVED41").lValue;
        lOtherFP=GetDBValue("SYS_HMI_NULL_NULL_RESERVED42").lValue;
    	}
   
   if(g_pExTable->GetTableValue(0,"D_J5")) //ZHBA 2011-1-25 
		{
		   for(int i = 0; i < iRampDown; i++ )
        {
        if(pwndRampDown[i]!=NULL)
			   { 
			    pwndRampDown[i]->SetPropValueT("min",1);
			    pwndRampDown[i]->Update();
			   }
        }
		}
		else
		{
       if(!m_b7TR)
       {
       SetVisible(pwndStaticRampDown,  FALSE, 0);
       
       SetVisible(pwndStaticValveEarlyTime,  FALSE, 0);// YANGJX 2011-2-21 閥提前時間
       SetVisible(pwndEditValveEarlyTime,  FALSE, 0);
       SetVisible(pwndStaticPValveEarlyTime,  FALSE, 0);// YANGJX 2011-2-21 比例閥提前時間
       SetVisible(pwndEditPValveEarlyTime,  FALSE, 0);
       
       for(int i = 0; i < iRampDown; i++ )
       SetVisible(pwndRampDown[i],  FALSE, 0);
       for(int i = 0; i < iSRampDown; i++ )
       SetVisible(pwndSRampDown[i],  FALSE, 0);
       }
		}
		
	if(g_pExTable->GetTableValue(0,"D_ACTTIME"))
	  {
	   GetACTTime(pwndTIMEAUTOTIME);
	   GetACTTime(pwndSEMIAUTOTIME);
	  }
	
	return TRUE;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	WORD  PBValue =(WORD)GetDBValue("STATE_IO1_READ_NULL_REALDATA2").lValue;
	BOOL  bSelected1  = FALSE;
	BOOL  bSelected2 = FALSE;
   
   if(pwndSelectBox0 != NULL)  //ZHBA  2009-4-24 處理PB for 7LG
   {
     if(_TestBit(PBValue, 13))	 bSelected1 = TRUE;
	 else 					     bSelected1 = FALSE;
	 pwndSelectBox0->SetPropValueT("selected", bSelected1);
	 pwndSelectBox0->Update();
	 }
	 
	 if(pwndSelectBox1 != NULL) //ZHBA  2009-4-24 處理PB for 7LG
	 {
     if(_TestBit(PBValue, 14))	 bSelected2 = TRUE;
	 else 					     bSelected2 = FALSE;
	 pwndSelectBox1->SetPropValueT("selected", bSelected2);
	 pwndSelectBox1->Update();
	 //printf("%X, %d, %d\n",PBValue,bSelected1,bSelected2);
	 }
	     SetDisabledControl();
	
	if(g_pExTable->GetTableValue(0,"D_ACTTIME"))
	  {
	   GetACTTime(pwndTIMEAUTOTIME);
	   GetACTTime(pwndSEMIAUTOTIME);
	  }
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	long long temp = 0;
	CtmWnd* pwnd = NULL;
	char	pszDB[256];
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)	return wIDControl;
	
	if(pwnd == pwndPara13 || pwnd == pwndPara14)
	{
		pwnd->GetPropValueT("value", &temp,sizeof(temp));
		SetData(pwnd);
	}
	else if (pwnd == pwndPara29)
	{
		pwnd->GetPropValueT("value", &temp,sizeof(temp));
		WORD wData;
		wData = (_DA)((WORD)temp);
		pwnd->GetPropValueT("dbid1", pszDB,sizeof(pszDB));		
		if (pszDB != NULL && pszDB[0] != '\0')	SetViewDBValue(pszDB, wData);
	}
	else if (pwnd == pwndMotorTemp)		//fuxy,2008-12-9, add for 7HX, Motor Temperature
	{
		pwnd->GetPropValueT("value", &lMTempOpt,sizeof(lMTempOpt));
		if(lMTempOpt != lMTempOptOld)
		{
			if(lMTempOpt)
			{
				SetDBValue("MSET_TMP1_HEATERON_CHL9_DEGREE",0);
				if(!(GetDBValue("MACH_TMP1_HEATERONOFF_RAMP9_DEGREE").lValue&0x0001))
					SetDBValue("MACH_TMP1_HEATERONOFF_RAMP9_DEGREE",GetDBValue("MACH_TMP1_HEATERONOFF_RAMP9_DEGREE").lValue |0x0001);
			}
			lMTempOptOld = lMTempOpt;
		}
	}
	else 
	{
		//	fans add 2007/1/22 09:10上午
		for (int i = 0; i < (int)(sizeof(pszCommandEditName) / sizeof(char*)); i++)
		{
			if ((pwndSender->FindControlFromName(pszCommandEditName[i]) != NULL)&&(pwndSender->FindControlFromName(pszCommandEditName[i]) == pwnd))
			{
				WORD	wData;
				pwnd->GetPropValueT("value", &temp,sizeof(temp));
				wData = (_DA)((WORD)temp);
				//if(g_ptaskrs232 != NULL) 
				//{
				//	g_ptaskrs232->SendDiagWO(0xFFE0 + i, wData);
					//printf("Chanel =%d, data=%d\n", i, wData);
				if(g_ptaskdsp != NULL) 
				{
					g_ptaskdsp->WriteValue(COMM_SENDDIAGWO,0xFFE0 + i, wData);
				}
				break;
			}
		}
	}

   if(g_pExTable->GetTableValue(0,"D_7LL"))  /*ZHBA 2012-9-20*/
   	  {
   	   long long ltemp = 0;
   	   
   	   if(pwnd =pwndOPENCOUNTDOWN)
   	   	{
		     ltemp=GetDBValue("MSET_SCRW_HLDP_STEP10_TIME").lValue-GetDBValue("MSET_SCRW_HLDP_STEP7_TIME").lValue;
		     SetDBValue("MSET_SCRW_HLDP_STEP8_TIME",ltemp);
   	   	}
   	  }
////ZHBA  2009-4-24 處理兩個Word轉為Dword
  if((pwnd == pwndLUBWORKTIME)&&(pwndLUBWORKTIME != NULL))
	{
      SendDWordValve(pwndLUBWORKTIME);
      SetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE9",lTemp1);
      SetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE10",lTemp2);        	    	    

	pwndLUBONPCTIME->SetPropValueT("max", lTemp);	
	pwndLUBONPCTIME->Update();
	pwndLUBOFFPCTIME->SetPropValueT("max", lTemp);	
	pwndLUBOFFPCTIME->Update();
	}
	else if((pwnd == pwndLUBONPCTIME)&&(pwndLUBONPCTIME != NULL))
	{
      SendDWordValve(pwndLUBONPCTIME);
      SetDBValue("MACH_ROTA_ROTA_STEP1_POSITION",lTemp1);
      SetDBValue("MACH_ROTA_ROTA_STEP2_POSITION",lTemp2);        	    	    
	}
	else if((pwnd == pwndLUBOFFPCTIME)&&(pwndLUBOFFPCTIME != NULL))
	{
      SendDWordValve(pwndLUBOFFPCTIME);
      SetDBValue("MACH_ROTA_ROTA_STEP3_POSITION",lTemp1);
      SetDBValue("MACH_ROTA_ROTA_NULL_COUNT2RATATE",lTemp2);        	    	    
	}
	else if((pwnd == pwndLUB2WORKTIME)&&(pwndLUB2WORKTIME != NULL))
	{
      SendDWordValve(pwndLUB2WORKTIME);
      SetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE15",lTemp1);
      SetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE16",lTemp2);        	    	    
	pwndLUB2ONPCTIME->SetPropValueT("max", lTemp);	
	pwndLUB2ONPCTIME->Update();
	pwndLUB2OFFPCTIME->SetPropValueT("max", lTemp);	
	pwndLUB2OFFPCTIME->Update();
	}
	else if((pwnd == pwndLUB2ONPCTIME)&&(pwndLUB2ONPCTIME != NULL))
	{
      SendDWordValve(pwndLUB2ONPCTIME);
      SetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE11",lTemp1);
      SetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE12",lTemp2);        	    	    
	}
	else if((pwnd == pwndLUB2OFFPCTIME)&&(pwndLUB2OFFPCTIME != NULL))
	{
      SendDWordValve(pwndLUB2OFFPCTIME);
      SetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE13",lTemp1);
      SetDBValue("MACH_OTHR_OTHR_NULL_OTHERMACHINE14",lTemp2);        	    	    
	}
//ZHBA  2009-4-24 處理兩個Word轉為Dword	

if(g_pExTable->GetTableValue(0,"D_7YJDWORD"))// YANGJX 2010-7-19 7YJ處理兩個Word轉為Dword
		{
	    if(pwndEDITLUBCOUNTS2TIME   != NULL)
	    {
      SendDWordValve(pwndEDITLUBCOUNTS2TIME);
      SetDBValue("MACH_SCRW_INJ_NULL_W018A",lTemp1);
      SetDBValue("MACH_SCRW_INJ_NULL_W018B",lTemp2); 
      }
    }

if(g_pExTable->GetTableValue(0,"D_7ASFASTMACH"))
	{
	 if(pwnd == pwndMOLDFLOWPERCENT)
      {
       	pwndMOLDFLOWPERCENT->GetPropValueT("value", &temp,sizeof(temp));
        if(lMoldFP !=temp)
         {
        	char* pszID;;
        	g_ptaskdsp28->WriteValue(9, pszID); 
        	lMoldFP =temp;
         }
      }
	 if(pwnd == pwndCHRGFLOWPERCENT)
      {
       	pwndCHRGFLOWPERCENT->GetPropValueT("value", &temp,sizeof(temp));
       if(lChrgFP !=temp)
         {
        	char* pszID;;
        	g_ptaskdsp28->WriteValue(9, pszID); 
        	lChrgFP =temp;
         }
      }
	 if(pwnd == pwndOTHERFLOWPERCENT)
      {
       	pwndOTHERFLOWPERCENT->GetPropValueT("value", &temp,sizeof(temp));
       if(lOtherFP !=temp)
         {
        	char* pszID;;
        	g_ptaskdsp28->WriteValue(9, pszID); 
        	lOtherFP =temp;
         }
      }
	}
	
	if(b_7AKBMC)
	{
		long lTemp = 0;
		if(pwnd == pwndMotorAutoOff && pwndMotorAutoOff != NULL)
		{
			SetData(pwndMotorAutoOff);
		}
		
		if(pwnd == pwndTempAutoWarm && pwndTempAutoWarm != NULL)
		{
			SetData(pwndTempAutoWarm);
		}
		
		if((pwnd == pwndMeterOther37) && (g_ptaskdsp28 != NULL))    //ZHBA  2008-9-18
		{
			pwnd->GetPropValueT("value", &temp,sizeof(temp));
			SendDSP28Data(pwnd);
		}
	}

	return wIDControl;
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	WORD	wOPStatus;
	wOPStatus 	= _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue) + 1;
	if(wOPStatus == 1)
	{
		for(int j = wOPStatus; j < 16; j++) 
		{
			if( _TestBit((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue,j))	
			{
				wOPStatus = j+1;
				break;
			}
		}
	}
	else if(wOPStatus == 0xFFFF) 	wOPStatus = 0;
	
	
	if (wOPStatus == 0)
		for (int i = 0; i < (int)(sizeof(pszCommandEditName) / sizeof(char*)); i++) //Andy 20071008
		{
				WORD	wData;
				long long temp = 0;
			if (pwndSender->FindControlFromName(pszCommandEditName[i]) != NULL)
				pwndSender->FindControlFromName(pszCommandEditName[i])->GetPropValueT("value", &temp,sizeof(temp));
				wData = (_DA)((WORD)temp);
				if(wData)
					{
					//ZHBA  2008-8-27	if(g_ptaskrs232 != NULL) 
					//ZHBA  2008-8-27	g_ptaskrs232->SendDiagWO(0xFFE0 + i, 0);
					if(g_ptaskdsp != NULL) 
					g_ptaskdsp->WriteValue(COMM_SENDDIAGWO,0xFFE0 + i, 0);
					}	
		}		
		//for (int i = 0; i < 12; i++)
		//{
		//	if(g_ptaskrs232 != NULL) g_ptaskrs232->SendDiagWO(0xFFE0 + i, 0);
		//}
   if(g_ptaskdsp28 != NULL)		
	  {
	   SetDBValue("MACH_OTHR_OTHR_NULL_W033A", 0);
		 SetDBValue("MACH_OTHR_OTHR_NULL_W033B", 0);
	  }
}

void		GetData(CtmWnd * pwnd)
{
	long long temp = 0;
	if(pwnd != NULL)
	{
//		printf("that isn't NULL!\n");
		pwnd->GetPropValueT("dbid1", psz,sizeof(psz));
		if(psz != NULL && psz[0] != '\0')
		{
			temp = GetDBValue(psz).lValue/6000;
			pwnd->SetPropValueT("value",temp);
			pwnd->Update();
		}
	}
	else
	{
		;//printf("that is NULL!\n");
	}
	OldData = temp;
}

void		SetData(CtmWnd * pwnd)
{
//	printf("editname =%s\n", pwnd->Name);
	long long temp = 0;
	if(pwnd != NULL)
	{
//		printf("that isn't NULL!\n");
		pwnd->GetPropValueT("value", &temp,sizeof(temp));
		pwnd->GetPropValueT("dbid1", psz,sizeof(psz));
		if(psz[0] != '\0')
			SetViewDBValue(psz,(temp*6000));
	}
}


void	SendDWordValve(CtmWnd * pwnd)  //ZHBA  2009-4-24
{
           if(pwnd != NULL)
			{
		    pwnd->GetPropValueT("value", &lTemp,sizeof(lTemp));	        
			lTemp1 = (lTemp & 0x0000FFFF);
            lTemp2 = lTemp>>16;            
            }
           
}
void	GetDWordValve(CtmWnd * pwnd)  //ZHBA  2009-4-24
{
      if(pwnd != NULL)
			{
      lTemp =(lTemp1 &0x0000FFFF) + (lTemp2 <<16);
      
      //printf("lTemp:%lld  lTemp1:%lld  lTemp2:%lld\n",lTemp,lTemp1,lTemp2);

			pwnd->SetPropValueT("value",lTemp);
			pwnd->Update();
     }
           
}

void	SetDisabledControl()
{
	long lTemp;
  if(m_pwndEditOperationHour   != NULL)	//Disabled
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_MINUTE").lValue;
		lTemp = lTemp * 100 / 60;
		m_pwndEditOperationHour->SetPropValueT("value", lTemp);	
	}
	if(m_pwndEditOperationHour!= NULL)	m_pwndEditOperationHour->Update();
}

void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType)
{
	if(pwnd != NULL)
	{
		if(wType)
			  pwnd->SetPropValueT("wtype",wType); 
		else
		{
		   	if(bVisible)
				pwnd->SetPropValueT("wtype",0x21);
			else
        		pwnd->SetPropValueT("wtype",0x30);
        }
			pwnd->Update();

			pwnd->SetPropValueT("visible",(double)bVisible);
			pwnd->Update();
	}		
}

void	GetACTTime(CtmWnd* pwnd)	
{
   char	pszDB[256];

 if(pwnd != NULL)	
	{
	  pwnd->GetPropValueT("dbid1",pszDB,sizeof(pszDB));
		long lTemp = GetDBValue(pszDB).lValue;
		lTemp = lTemp * 100/ 60;
		pwnd ->SetPropValueT("value", lTemp);
		pwnd ->Update();
	}
}

void		SendDSP28Data(CtmWnd * pwnd)
{
	long long lMaxSysPres 	  = HEX2BCD(GetDBValue("CONF_CODE_WRITE_NULL_MAXPRESSURE").lValue);				//系統壓力上限值
    long long lMaxHydraulic1  = GetDBValue("CONF_PVL1_READ_NULL_MAXDATA").lValue;	//比例閥1最大值, //ZHBA  2008-7-1
    
        if(lMaxSysPres == 0)  lMaxSysPres = 140;
  else	if(lMaxSysPres == 1)  lMaxSysPres = lMaxHydraulic1;
	long long temp = 0;
	if(pwnd != NULL)
	{
		pwnd->GetPropValueT("value", &temp,sizeof(temp));
		pwnd->GetPropValueT("dbid1", psz,sizeof(psz));
		SetViewDBValue(psz,(temp*4095/lMaxSysPres));	
	}
//printf("data = %d\n",temp*4095/lMaxSysPres);
}

void  	SetSegment()
{	
	//處理VALUE為-1的EDIT
	for( int i = 0; i < EditNum; i++ )  //Andy 20080109
		{
			if(pwndEditAll[i] != NULL)
				{
					pwndEditAll[i]->GetPropValueT("value", &EditValue,	sizeof(int));
	        //printf("EditValue:%d\n",EditValue);
					if((EditValue == 0xFFFF)||(EditValue == (int)0xFFFFFFFF)||(EditValue == -1))   //If Edit.value == -1,visible!;
						SetVisible( pwndEditAll[i], FALSE,0);
				}			
		}			
}
