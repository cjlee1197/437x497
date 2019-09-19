/*===========================================================================+
|  Class    : ViewVersSyst library                                          	 |
|  Task     : ViewVersSyst library  source file                       	 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : J.Wong		                                                 |
|  Version  : V1.00                                                          |
|  Creation : 12/03/2014                                                    |
|  Revision :           			                                         |
+===========================================================================*/


#include 	"ViewVersSyst.h"
#include "../lib/hmi_group/tmTaskN2CAmmeter.h"
//#include  	"../lib/libDataSave.h"
//#include  	"../taskdataprocess.h"

#define		MAX_RMTP12_CNT			8
#define		MAX_PWM_CNT				3
#define		MAX_TMIO_CNT			6

#define		MAX_DIRVER_CNT			2
#define		MAX_AXIS_CNT			3


CtmWnd*		pwndRMTP12Static[MAX_RMTP12_CNT]	= {NULL};
CtmWnd*		pwndRMTP12State[MAX_RMTP12_CNT]		= {NULL};
CtmWnd*		pwndRMTP12Version[MAX_RMTP12_CNT]	= {NULL};

CtmWnd*		pwndPWMStatic[MAX_PWM_CNT]	    	= {NULL};
CtmWnd*		pwndPWMState[MAX_PWM_CNT]	    	= {NULL};
CtmWnd*		pwndPWMVER[MAX_PWM_CNT]	        	= {NULL};

CtmWnd*		pwndTMIOStatic[MAX_TMIO_CNT]		= {NULL};
CtmWnd*		pwndTMIOState[MAX_TMIO_CNT]			= {NULL};
CtmWnd*		pwndTMIOVersion[MAX_TMIO_CNT]		= {NULL};

CtmWnd*		pwnsEditM3Version[32]     	    	= {NULL};

CtmWnd*		pwndeditM3_App	        = NULL;
CtmWnd*		pwndeditM3_Boot         = NULL;

CtmWnd*		pwndDSP55State	        = NULL;
CtmWnd*		pwndDSP28State	        = NULL;

CtmWnd*		pwndImageSystMapPWM  	= NULL;
CtmWnd*		pwndImagePWM  	        = NULL;
CtmWnd*		pwndStaticPWM  	        = NULL;

CtmWnd*		pwndImageSystMapTEMP   	= NULL;
CtmWnd*		pwndImageTEMP  			= NULL;
CtmWnd*		pwndStaticTEMP  		= NULL;

CtmWnd*		pwndImageSystMapTMIO   	= NULL;
CtmWnd*		pwndImageTMIO  	    	= NULL;
CtmWnd*		pwndStaticTMIO  	   	= NULL;
CtmWnd*		pwndImageSystMapUp      = NULL;

CtmWnd*		pwnsEditMMIVersion     	= NULL;
CtmWnd*		pwnsEditMMIDate        	= NULL;
CtmWnd*		pwnsStaticM3Key     	= NULL;
CtmWnd*	  	pwndeditM3KeyVersion 	= NULL;

CtmWnd*	  	pwndEditAxisDate[8]	= {NULL};

char    u_ResImage[128];
int		u_nClassControlCount[16];
int		u_nDSP28State =0;
int		u_nDSP28OldState =0;
int		u_nDSP55OldState =0;
int		u_nRMTP12State[MAX_RMTP12_CNT];
int		u_nRMTP12OldState[MAX_RMTP12_CNT];
int		u_nTemperUsed[MAX_RMTP12_CNT];
int		u_nPWMState =0;
int		u_nPWMOldState =0;
int		u_nPWMUsed[MAX_PWM_CNT];
int     n_vernum =0;
int     nImagePathLenth =0;
int		u_nTMIOUsed[MAX_TMIO_CNT];
int		u_nTMIOType[MAX_TMIO_CNT];
int		u_nTMIOState[MAX_TMIO_CNT];
int		u_nTMIOOldState[MAX_TMIO_CNT];

int		 u_nClassAxisCount[18];
int		u_nAxisUsed[MAX_AXIS_CNT];
int		u_nAxisVers[MAX_AXIS_CNT*2];
int		u_nAxis1Driver1Used[MAX_DIRVER_CNT];
int		u_nAxis1Driver2Used[MAX_DIRVER_CNT];
int		u_nAxis2Driver1Used[MAX_DIRVER_CNT];
int		u_nAxis2Driver2Used[MAX_DIRVER_CNT];
int		u_nAxis3Driver1Used[MAX_DIRVER_CNT];
int		u_nAxis3Driver2Used[MAX_DIRVER_CNT];

CtmWnd*	 pwndAxisVersStatic[MAX_AXIS_CNT]		= {NULL};
CtmWnd*	 pwndAxisVersEdit  [MAX_AXIS_CNT*2]		= {NULL};
CtmWnd*	 pwndAxisVersImage [MAX_AXIS_CNT*2]		= {NULL};

CtmWnd*	 pwndAxis1Driver1Image [1]	            = {NULL};
CtmWnd*	 pwndAxis1Driver1Static[1]				= {NULL};
CtmWnd*	 pwndAxis1Driver2Static[1]				= {NULL};
CtmWnd*	 pwndAxis1Driver1Edit[MAX_DIRVER_CNT]	= {NULL};
CtmWnd*	 pwndAxis1Driver2Edit[MAX_DIRVER_CNT]	= {NULL};

CtmWnd*	 pwndAxis2Driver1Image [1]	            = {NULL};
CtmWnd*	 pwndAxis2Driver1Static[1]				= {NULL};
CtmWnd*	 pwndAxis2Driver2Static[1]				= {NULL};
CtmWnd*	 pwndAxis2Driver1Edit[MAX_DIRVER_CNT]	= {NULL};
CtmWnd*	 pwndAxis2Driver2Edit[MAX_DIRVER_CNT]	= {NULL};

CtmWnd*	 pwndAxis3Driver1Image [1]	            = {NULL};
CtmWnd*	 pwndAxis3Driver1Static[1]				= {NULL};
CtmWnd*	 pwndAxis3Driver2Static[1]				= {NULL};
CtmWnd*	 pwndAxis3Driver1Edit[MAX_DIRVER_CNT]	= {NULL};
CtmWnd*	 pwndAxis3Driver2Edit[MAX_DIRVER_CNT]	= {NULL};


int		u_nAxisState[4];
int		u_nAxisOldState[4];

char	u_cszAxisDate[16];

extern	DWORD	g_dwM3KeyboardVers;

char*	ControlName[]=
{
	"StaticSystMapR1_TEMP",
	"ImageSystMapR2_TEMPSTATE",
	"editM3Version_Temp",
	
	"StaticSystMapR3_PWRMT",
	"ImageSystMapR4_PWMSTATE",
	"editM3Version_Ammter",
	
	"StaticSystMapL3_TMIO",
	"ImageSystMapL4_TMIOSTATE",
	"editM3Version_TMIO",
	NULL		
};

char*	AxisName[]=
{
	"StaticAxisVers",
	"ImageAxisVers",
	"editAxisVers",
	
	"ImageAxis1Driver1",
	"StaticAxis1Driver1",
	"StaticAxis1Driver2",
	"editAxis1Driver1",
	"editAxis1Driver2",

	"ImageAxis2Driver1",
	"StaticAxis2Driver1",
	"StaticAxis2Driver2",
	"editAxis2Driver1",
	"editAxis2Driver2",

	"ImageAxis3Driver1",
	"StaticAxis3Driver1",
	"StaticAxis3Driver2",
	"editAxis3Driver1",
	"editAxis3Driver2",
	NULL
};

char* 	ImageName[]=
{
	"DriverTempGray.bmp",
	"DriverTempGreen.bmp",
	"DriverTempRed.bmp",
	"DriverTempYellow.bmp",
	NULL 
};

char*	pszPWMNodeID[]=
{
	"M3_CONF_PWMCARD1_NODE_ID",
	"M3_CONF_PWMCARD2_NODE_ID",
	"M3_CONF_PWMCARD3_NODE_ID",
};

char *g_DspM3VersionID[] =
{
	"M3_CONF_CARD1_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_CARD3_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_CARD4_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_TEMPER_CARD1_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_TEMPER_CARD2_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_TEMPER_CARD3_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_TEMPER_CARD4_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_TEMPER_CARD5_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_TEMPER_CARD6_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_TEMPER_CARD7_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_TEMPER_CARD8_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_CONFIGURATION_VERSIONINFO2",
	"M3_CONF_REMOTEIO_CARD1_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_REMOTEIO_CARD2_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_REMOTEIO_CARD3_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_REMOTEIO_CARD4_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_REMOTEIO_CARD5_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_REMOTEIO_CARD6_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_REMOTEIO_CARD7_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_REMOTEIO_CARD8_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_REMOTEIO_CARD9_CONFIGURATION_VERSIONINFO1",
	"M3_CONF_REMOTEIO_CARD10_CONFIGURATION_VERSIONINFO1",
};

char *g_DspAxisID[] =
{
	"M3_CONF_CONFIGURATION_VERSIONINFO2",
	"M3_CONF_CONFIGURATION_VERSIONINFO10",
	
	"M3_CONF_CARD1_CONFIGURATION_CARDID",
	"M3_CONF_CARD1_CONFIGURATION_VERSIONINFO2",
	"M3_CONF_CARD1_CONFIGURATION_VERSIONINFO4",
	"M3_CONF_CARD1_CONFIGURATION_VERSIONINFO6",
	"M3_CONF_CARD1_CONFIGURATION_VERSIONINFO8",
	"M3_CONF_CARD1_CONFIGURATION_VERSIONINF10",
	
	"M3_CONF_CARD2_CONFIGURATION_CARDID",
	"M3_CONF_CARD2_CONFIGURATION_VERSIONINFO2",
	"M3_CONF_CARD2_CONFIGURATION_VERSIONINFO4",
	"M3_CONF_CARD2_CONFIGURATION_VERSIONINFO6",
	"M3_CONF_CARD2_CONFIGURATION_VERSIONINFO8",
	"M3_CONF_CARD2_CONFIGURATION_VERSIONINF10",

	"M3_CONF_CARD3_CONFIGURATION_CARDID",
	"M3_CONF_CARD3_CONFIGURATION_VERSIONINFO2",
	"M3_CONF_CARD3_CONFIGURATION_VERSIONINFO4",
	"M3_CONF_CARD3_CONFIGURATION_VERSIONINFO6",
	"M3_CONF_CARD3_CONFIGURATION_VERSIONINFO8",
	"M3_CONF_CARD3_CONFIGURATION_VERSIONINF10",
};
/*===========================================================================+
|           Form Function                                                    |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
    int nTime = 500;
	
	pwndDSP55State	= pwndSender->FindControlFromName("ImageSysMapDsp55_STATE");
	pwndDSP28State	= pwndSender->FindControlFromName("ImageSysMapDsp28_STATE");
    pwndeditM3_App  = pwndSender->FindControlFromName("editM3Version_Self");
	pwndeditM3_Boot = pwndSender->FindControlFromName("editM3Version_Boot");
	
	pwndImageSystMapPWM	= pwndSender->FindControlFromName("ImageSystMap_Right2");
	pwndImagePWM  	= pwndSender->FindControlFromName("ImageSystMapR4_PWM1");
	pwndStaticPWM	= pwndSender->FindControlFromName("StaticSystMap_PWRMT");
	
	pwndImageSystMapTEMP	= pwndSender->FindControlFromName("ImageSystMap_Right1");
	pwndImageTEMP	= pwndSender->FindControlFromName("ImageSystMapR1_TEMP1");
	pwndStaticTEMP	= pwndSender->FindControlFromName("StaticSystMap_TEMP");
	
	pwndImageSystMapTMIO	= pwndSender->FindControlFromName("ImageSystMap_Right3");
	pwndImageTMIO  	= pwndSender->FindControlFromName("ImageSystMap_TMIO");
	pwndStaticTMIO	= pwndSender->FindControlFromName("StaticSystMap_TMIO");
	pwndImageSystMapUp	= pwndSender->FindControlFromName("ImageSystMap_Left3");
		
	pwnsEditMMIVersion		= pwndSender->FindControlFromName("editMMIVersion"); 
	pwnsEditMMIDate         = pwndSender->FindControlFromName("editMMIDate");
	pwnsStaticM3Key	 		= pwndSender->FindControlFromName("StaticM3KEY");
  	pwndeditM3KeyVersion	= pwndSender->FindControlFromName("editM3KeyboardVersion");
  	
    n_vernum    = GetSpecialControlNum(pwndSender,	"M3Version"	,	"CtmEditX2",	pwnsEditM3Version);
	GetSpecialControlNum(pwndSender,	"editDriver_Date"	,	"CtmEdit",	pwndEditAxisDate);
	  
  /*  if(g_ptaskdsp28_M3 !=NULL)
	    g_ptaskdsp28_M3->ReqValues(2, sizeof(g_DspM3VersionID) / sizeof(g_DspM3VersionID[0]), g_DspM3VersionID,&nTime);

    if(g_ptaskdsp28_M3 !=NULL)
	    g_ptaskdsp28_M3->ReqValues(2, sizeof(g_DspAxisID) / sizeof(g_DspAxisID[0]), g_DspAxisID,&nTime);
    */
	memset(u_nRMTP12State, 0, sizeof(u_nRMTP12State));
	memset(u_nRMTP12OldState, 0, sizeof(u_nRMTP12OldState));
	memset(u_nTemperUsed, 0, sizeof(u_nTemperUsed));
	memset(u_nPWMUsed, 0, sizeof(u_nPWMUsed));
	
	memset(u_nTMIOUsed, 0, sizeof(u_nTMIOUsed));
	memset(u_nTMIOType, 0, sizeof(u_nTMIOType));
	memset(u_nTMIOState, 0, sizeof(u_nTMIOState));
	memset(u_nTMIOOldState, 0, sizeof(u_nTMIOOldState));
	
	memset(u_nAxisUsed, 0, sizeof(u_nAxisUsed));
	memset(u_nAxisVers, 0, sizeof(u_nAxisVers));
	memset(u_nAxis1Driver1Used, 0, sizeof(u_nAxis1Driver1Used));
	memset(u_nAxis1Driver2Used, 0, sizeof(u_nAxis1Driver2Used));
	memset(u_nAxis2Driver1Used, 0, sizeof(u_nAxis2Driver1Used));
	memset(u_nAxis2Driver2Used, 0, sizeof(u_nAxis2Driver2Used));
	memset(u_nAxis3Driver1Used, 0, sizeof(u_nAxis3Driver1Used));
	memset(u_nAxis3Driver2Used, 0, sizeof(u_nAxis3Driver2Used));
	
	SetVisible(pwndImageSystMapUp, FALSE, 0);
	
	//temper card node id
	char szNodeID[128];
	for(unsigned int i=0; i<MAX_RMTP12_CNT; ++i)
	{
		sprintf(szNodeID, "M3_CONF_TEMPERCARD%d_NODE_ID", i+1);
		u_nTemperUsed[i] = GetDBValue(szNodeID).lValue;
	}
	if(u_nTemperUsed[0] == 0) //20160129
	{
		SetVisible(pwndImageSystMapTEMP, FALSE, 0);
		SetVisible(pwndImageTEMP, FALSE, 0);
		SetVisible(pwndStaticTEMP, FALSE, 0);
	}

	for(unsigned int i=0; i<MAX_TMIO_CNT; ++i)
	{
		if(i <GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED14").lValue) //20150623, TMIO個數
			u_nTMIOUsed[i] =1;
		else
			u_nTMIOUsed[i] =0;
		
		if(GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED16").lValue &(0x01<<i)) //20150722, TMIO 區分101還是102
			u_nTMIOType[i] =1;
		//printf("i:%d, TMIOUsed:%d, u_nTMIOType[i]:%d\n", i, u_nTMIOUsed[i], u_nTMIOType[i]);
	}
	if(GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED14").lValue ==0) //20150624, TMIO
	{
		SetVisible(pwndImageSystMapTMIO, FALSE, 0);
		SetVisible(pwndImageTMIO, FALSE, 0);
		SetVisible(pwndStaticTMIO, FALSE, 0);
	}
	//pwm    card node id
//	for(int i=0; i<sizeof(pszPWMNodeID)/sizeof(pszPWMNodeID[0]); ++i)
//	{
//		u_nPWMUsed[i]=GetDBValue(pszPWMNodeID[i]).lValue;
//	}
	//int AmmeterUseOpt = GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED29").lValue;
	
	AMMETERCONFIG	AmmeterConf;
	memset(&AmmeterConf, 0, sizeof(AmmeterConf));
	if(g_ptaskAmmeter != NULL)
		AmmeterConf = *((AMMETERCONFIG*)(g_ptaskAmmeter->ReadConf()));
		
	//printf("AmmeterConf.wCount=%d\n",GetDBValue("PWMTRCARD1_INTERFACE_DWVERSION1").lValue);
	if(AmmeterConf.wCount==0)
	{	
		//if(GetDBValue("M3_STATE_POWERCARD1_LINESTATE").lValue == 1)		
		if(GetDBValue("PWMTRCARD1_INTERFACE_DWVERSION1").lValue)
			u_nPWMUsed[0] =1;
		else
			u_nPWMUsed[0] =0;
	}
	else
	{
		for(unsigned int i=0; i<sizeof(pszPWMNodeID)/sizeof(pszPWMNodeID[0]); ++i)
		{
			//if(AmmeterUseOpt == 3)/*CAN*/
			{	
				if(i < AmmeterConf.wCount)
					u_nPWMUsed[i] =1;
				else
					u_nPWMUsed[i] = 0;
			}
		}
	}
	//if(AmmeterUseOpt != 3)/*CAN*/
	//if(GetDBValue("M3_STATE_POWERCARD1_LINESTATE").lValue == 0)
	if(GetDBValue("PWMTRCARD1_INTERFACE_DWVERSION1").lValue ==0)
	{
		SetVisible(pwndImageSystMapPWM, FALSE, 0);
		SetVisible(pwndImagePWM, FALSE, 0);
		SetVisible(pwndStaticPWM, FALSE, 0);
	}
		
	memset(u_ResImage,0,sizeof(u_ResImage));
	strncpy(u_ResImage, CtmConfig::GetInstance()->GetBmpPath(), 127);
	nImagePathLenth =strlen(u_ResImage);
	
	u_nClassControlCount[0] =GetSpecialControlNum(pwndSender,ControlName[0],"CtmStaticX2",pwndRMTP12Static);
	u_nClassControlCount[1] =GetSpecialControlNum(pwndSender,ControlName[1],"CtmImageBox",pwndRMTP12State);
	u_nClassControlCount[2] =GetSpecialControlNum(pwndSender,ControlName[2],"CtmEditX2",pwndRMTP12Version);
	
	u_nClassControlCount[3] =GetSpecialControlNum(pwndSender,ControlName[3],"CtmStaticX2",pwndPWMStatic);
	u_nClassControlCount[4] =GetSpecialControlNum(pwndSender,ControlName[4],"CtmImageBox",pwndPWMState);
	u_nClassControlCount[5] =GetSpecialControlNum(pwndSender,ControlName[5],"CtmEditX2",pwndPWMVER);
	
	u_nClassControlCount[6] =GetSpecialControlNum(pwndSender,ControlName[6],"CtmStaticX2",pwndTMIOStatic);
	u_nClassControlCount[7] =GetSpecialControlNum(pwndSender,ControlName[7],"CtmImageBox",pwndTMIOState);
	u_nClassControlCount[8] =GetSpecialControlNum(pwndSender,ControlName[8],"CtmEditX2",pwndTMIOVersion);
	
	//printf("verSyst: %d,%d,%d,%d\n",u_nClassControlCount[4],u_nClassControlCount[5],u_nClassControlCount[6],u_nClassControlCount[3]);
	SetAllVisible(pwndRMTP12Static, u_nTemperUsed, u_nClassControlCount[0]);
	SetAllVisible(pwndRMTP12State, u_nTemperUsed, u_nClassControlCount[1]);
	SetAllVisible(pwndRMTP12Version, u_nTemperUsed,u_nClassControlCount[2], TRUE);
	
	SetAllVisible(pwndPWMStatic, u_nPWMUsed, u_nClassControlCount[3]);
	SetAllVisible(pwndPWMState, u_nPWMUsed, u_nClassControlCount[4]);
	SetAllVisible(pwndPWMVER,  u_nPWMUsed,u_nClassControlCount[5], TRUE);
	
	SetAllVisible(pwndTMIOStatic, u_nTMIOUsed, u_nClassControlCount[6]);
	SetAllVisible(pwndTMIOState, u_nTMIOUsed, u_nClassControlCount[7]);
	SetAllVisible(pwndTMIOVersion, u_nTMIOUsed, u_nClassControlCount[8]);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	u_nClassAxisCount[0] =GetSpecialControlNum(pwndSender,AxisName[0],"CtmStaticX2",pwndAxisVersStatic);
	u_nClassAxisCount[1] =GetSpecialControlNum(pwndSender,AxisName[1],"CtmImageBox",pwndAxisVersImage);
	u_nClassAxisCount[2] =GetSpecialControlNum(pwndSender,AxisName[2],"CtmEdit"    ,pwndAxisVersEdit);    
    for(int i=0; i< u_nClassAxisCount[0]; i++)
    {
    	u_nAxisUsed[i] = GetDeviceUsed(pwndAxisVersEdit[i*2]);
    }    
	SetAllVisible(pwndAxisVersStatic, u_nAxisUsed, u_nClassAxisCount[0]);
    for(int i=0; i< u_nClassAxisCount[2]; i++)
    {
    	u_nAxisVers[i] = GetDeviceUsed(pwndAxisVersEdit[i]);
    	if(u_nAxisVers[i])
    	{
    		if(i%2)
    			SetAxisDate(pwndAxisVersEdit[i],2);
    		else
    			SetAxisDate(pwndAxisVersEdit[i],1);
    	}

    }	
	SetAllVisible(pwndAxisVersImage,  u_nAxisVers, u_nClassAxisCount[1]);
	SetAllVisible(pwndAxisVersEdit,   u_nAxisVers, u_nClassAxisCount[2]);


	u_nClassAxisCount[3] =GetSpecialControlNum(pwndSender,AxisName[3],"CtmImageBox",pwndAxis1Driver1Image);
	u_nClassAxisCount[4] =GetSpecialControlNum(pwndSender,AxisName[4],"CtmStaticX2",pwndAxis1Driver1Static);
	u_nClassAxisCount[5] =GetSpecialControlNum(pwndSender,AxisName[5],"CtmStaticX2",pwndAxis1Driver2Static);
	u_nClassAxisCount[6] =GetSpecialControlNum(pwndSender,AxisName[6],"CtmEdit",	pwndAxis1Driver1Edit);
	u_nClassAxisCount[7] =GetSpecialControlNum(pwndSender,AxisName[7],"CtmEdit",	pwndAxis1Driver2Edit);
    
    for(int i=0; i< u_nClassAxisCount[6]; i++)
    {
    	u_nAxis1Driver1Used[i] = GetDeviceUsed(pwndAxis1Driver1Edit[i]);
    	if(u_nAxis1Driver1Used[i])
    	{
    		if(i)
    		{
    			SetAxisDate(pwndAxis1Driver1Edit[i-1],i+2);
    			SetAxisDate(pwndAxis1Driver1Edit[i],i+3);
    		}
    		else SetAxisDate(pwndAxis1Driver1Edit[i],0);    		
    	}
    }
	SetAllVisible(pwndAxis1Driver1Image,    u_nAxis1Driver1Used, u_nClassAxisCount[3]);
	SetAllVisible(pwndAxis1Driver1Static,   u_nAxis1Driver1Used, u_nClassAxisCount[4]);
	SetAllVisible(pwndAxis1Driver1Edit,     u_nAxis1Driver1Used, u_nClassAxisCount[6]);

    for(int i=0; i< u_nClassAxisCount[7]; i++)
    {
    	u_nAxis1Driver2Used[i] = GetDeviceUsed(pwndAxis1Driver2Edit[i]);
    	if(u_nAxis1Driver2Used[i])
    	{
    		if(i)
    		{
    			SetAxisDate(pwndAxis1Driver2Edit[i-1],i+2);
    			SetAxisDate(pwndAxis1Driver2Edit[i],i+3);
    		}
    		else SetAxisDate(pwndAxis1Driver2Edit[i],0);    		
    	}
    }
	SetAllVisible(pwndAxis1Driver2Static,   u_nAxis1Driver2Used, u_nClassAxisCount[5]);
	SetAllVisible(pwndAxis1Driver2Edit,     u_nAxis1Driver2Used, u_nClassAxisCount[7]);
	
	u_nClassAxisCount[8] =GetSpecialControlNum(pwndSender,AxisName[8] ,"CtmImageBox",pwndAxis2Driver1Image); 
	u_nClassAxisCount[9] =GetSpecialControlNum(pwndSender,AxisName[9] ,"CtmStaticX2",pwndAxis2Driver1Static);
	u_nClassAxisCount[10]=GetSpecialControlNum(pwndSender,AxisName[10],"CtmStaticX2",pwndAxis2Driver2Static);
	u_nClassAxisCount[11]=GetSpecialControlNum(pwndSender,AxisName[11],"CtmEdit",	 pwndAxis2Driver1Edit);  
	u_nClassAxisCount[12]=GetSpecialControlNum(pwndSender,AxisName[12],"CtmEdit",    pwndAxis2Driver2Edit);  
    for(int i=0; i< u_nClassAxisCount[11]; i++)
    {
    	u_nAxis2Driver1Used[i] = GetDeviceUsed(pwndAxis2Driver1Edit[i]);
    	if(u_nAxis2Driver1Used[i])
    	{
    		if(i)
    		{
    			SetAxisDate(pwndAxis2Driver1Edit[i-1],i+2);
    			SetAxisDate(pwndAxis2Driver1Edit[i],i+3);
    		}
    		else SetAxisDate(pwndAxis2Driver1Edit[i],0);    		
    	}
    }
	SetAllVisible(pwndAxis2Driver1Image,    u_nAxis2Driver1Used, u_nClassAxisCount[8]);
	SetAllVisible(pwndAxis2Driver1Static,   u_nAxis2Driver1Used, u_nClassAxisCount[9]);
	SetAllVisible(pwndAxis2Driver1Edit,     u_nAxis2Driver1Used, u_nClassAxisCount[11]);
    for(int i=0; i< u_nClassAxisCount[12]; i++)
    {
    	u_nAxis2Driver2Used[i] = GetDeviceUsed(pwndAxis2Driver2Edit[i]);

    	if(u_nAxis2Driver2Used[i])
    	{
    		if(i)
    		{
    			SetAxisDate(pwndAxis2Driver2Edit[i-1],i+2);
    			SetAxisDate(pwndAxis2Driver2Edit[i],i+3);
    		}
    		else SetAxisDate(pwndAxis2Driver2Edit[i],0);
    	}
    }
	SetAllVisible(pwndAxis2Driver2Static,   u_nAxis2Driver2Used, u_nClassAxisCount[10]);
	SetAllVisible(pwndAxis2Driver2Edit,     u_nAxis2Driver2Used, u_nClassAxisCount[12]);
                                                                                                            
	u_nClassAxisCount[13]=GetSpecialControlNum(pwndSender,AxisName[13],"CtmImageBox",pwndAxis3Driver1Image); 
	u_nClassAxisCount[14]=GetSpecialControlNum(pwndSender,AxisName[14],"CtmStaticX2",pwndAxis3Driver1Static);
	u_nClassAxisCount[15]=GetSpecialControlNum(pwndSender,AxisName[15],"CtmStaticX2",pwndAxis3Driver2Static);
	u_nClassAxisCount[16]=GetSpecialControlNum(pwndSender,AxisName[16],"CtmEdit",    pwndAxis3Driver1Edit);  
	u_nClassAxisCount[17]=GetSpecialControlNum(pwndSender,AxisName[17],"CtmEdit",    pwndAxis3Driver2Edit);  
    for(int i=0; i< u_nClassAxisCount[16]; i++)
    {
    	u_nAxis3Driver1Used[i] = GetDeviceUsed(pwndAxis3Driver1Edit[i]);
    	if(u_nAxis3Driver1Used[i])
    	{
    		if(i)
    		{
    			SetAxisDate(pwndAxis3Driver1Edit[i-1],i+2);
    			SetAxisDate(pwndAxis3Driver1Edit[i],i+3);
    		}
    		else SetAxisDate(pwndAxis3Driver1Edit[i],0);    		
    	}
    }
	SetAllVisible(pwndAxis3Driver1Image,    u_nAxis3Driver1Used, u_nClassAxisCount[13]);
	SetAllVisible(pwndAxis3Driver1Static,   u_nAxis3Driver1Used, u_nClassAxisCount[14]);
	SetAllVisible(pwndAxis3Driver1Edit,     u_nAxis3Driver1Used, u_nClassAxisCount[16]);
    for(int i=0; i< u_nClassAxisCount[17]; i++)
    {
    	u_nAxis3Driver2Used[i] = GetDeviceUsed(pwndAxis3Driver2Edit[i]);
    	if(u_nAxis3Driver2Used[i])
    	{
    		if(i)
    		{
    			SetAxisDate(pwndAxis3Driver2Edit[i-1],i+2);
    			SetAxisDate(pwndAxis3Driver2Edit[i],i+3);
    		}
    		else SetAxisDate(pwndAxis3Driver2Edit[i],0);    		
    	}
    }
	SetAllVisible(pwndAxis3Driver2Static,   u_nAxis3Driver2Used, u_nClassAxisCount[15]);
	SetAllVisible(pwndAxis3Driver2Edit,     u_nAxis3Driver2Used, u_nClassAxisCount[17]);
	                                                                                                        
    //for(int i=0; i< 18; ++i)
	//	printf("u_nClassAxisCount[%d],%d\n",i,u_nClassAxisCount[i]);

    char    szMMIVersion[10];
    strncpy(szMMIVersion, g_pDatabase->Read("VERS_HMI_NULL_NULL_PROGRAMMMIVERS").pcData, 8);
	if(pwnsEditMMIVersion 	  != NULL) 
		pwnsEditMMIVersion->SetPropValueT("text", szMMIVersion);
    
    char    szMMIDate[10];
    strncpy(szMMIDate, g_pDatabase->Read("VERS_HMI_NULL_NULL_PROGRAMMMIDATE").pcData, 8);
	if(pwnsEditMMIDate 	  != NULL) //mark 2008-7-3
		pwnsEditMMIDate->SetPropValueT("text", szMMIDate);

#ifdef	D_CANMASTER
  	if(pwndeditM3KeyVersion != NULL) /* zyh 2015-4-10  M3Key版本顯示*/
  	{
		pwndeditM3KeyVersion->SetPropValueT("value", (g_dwM3KeyboardVers+0x20000000)); /* 加0x20000000是為了在版本前顯示20 150410->20150410  */
		pwndeditM3KeyVersion->Update();
  	}
#else
	if(pwnsStaticM3Key != NULL)
  	{
		pwnsStaticM3Key->SetPropValueT("visible", (double)FALSE);
		pwnsStaticM3Key->Update();
  	}
	if(pwndeditM3KeyVersion != NULL)
  	{
		pwndeditM3KeyVersion->SetPropValueT("visible", (double)FALSE);
		pwndeditM3KeyVersion->Update();
  	}
#endif
  	
  	char	szPath[128];
  	char    sz[256]; 
  	strncpy(szPath, CtmConfig::GetInstance()->GetBmpPath(), 127);
  	sprintf(sz,"%s%s",szPath,"SystMap_TMIO102.png");
  	for(unsigned int i=0; i<MAX_TMIO_CNT; ++i)
	{
		if((u_nTMIOUsed[i] ==1) &&(u_nTMIOType[i] ==1))  //20150722, TMIO 圖片區分101還是102
		{
			;
			//if(pwndTMIOImage[i] != NULL)
			//{
			//	pwndTMIOImage[i]->SetPropValueT("imagepath", sz); 
			//	pwndTMIOImage[i]->Update();
			//}
		}
	}
  	
  	SetAxisDate(pwndeditM3_App,1);  
  	SetAxisDate(pwndeditM3_Boot,2); 
  	
  	
	u_nDSP55OldState = g_wDspStatus;
	if(g_wDspStatus)
	{
		memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
		sprintf(u_ResImage+strlen(u_ResImage),"%s",ImageName[1]);
		//printf("imagepath=%s\n",u_ResImage);
		if(pwndDSP55State != NULL)
		    pwndDSP55State->SetPropValueT("imagepath",u_ResImage);
		//printf("g_ptaskdsp28_M3=%x\n",g_ptaskdsp28_M3);
		/*if(g_ptaskdsp28_M3!=NULL)
		{
			u_nDSP28OldState = u_nDSP28State = !g_ptaskdsp28_M3->GetOnLineStatus();
			if(u_nDSP28State)
			{
				memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
				sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[1]);
				//printf("imagepath=%s\n",u_ResImage);
				if(pwndDSP28State != NULL)
				    pwndDSP28State->SetPropValueT("imagepath",u_ResImage);
				
				for(int i=0; i<u_nClassControlCount[1]; ++i)
				{
					u_nRMTP12OldState[i] = u_nRMTP12State[i] = (GetDBValue("M3_STATE_TEMPERCARD1_LINESTATE").lValue>>i) &0x01;
					//printf("i:%d, u_nRMTP12State:%d\n", i, u_nRMTP12State[i]);
					if(u_nRMTP12State[i])
					{
						memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
						sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[1]);
						if(pwndRMTP12State[i] != NULL)
						    pwndRMTP12State[i]->SetPropValueT("imagepath",u_ResImage);
					}
					else
					{
						memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
						sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
						if(pwndRMTP12State[i] != NULL)
						    pwndRMTP12State[i]->SetPropValueT("imagepath",u_ResImage);
					}
				}
			}
			else
			{
				memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
				sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
				//printf("imagepath=%s\n",u_ResImage);
				if(pwndDSP28State != NULL)
				    pwndDSP28State->SetPropValueT("imagepath",u_ResImage);
				
				for(int i=0; i<u_nClassControlCount[1]; ++i)
				{	
					memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
					sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
					if(pwndRMTP12State[i] != NULL)
					    pwndRMTP12State[i]->SetPropValueT("imagepath",u_ResImage);
				}
			}
		}*/
	}
	else
	{
		memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
		sprintf(u_ResImage+strlen(u_ResImage),"%s",ImageName[0]);	
		//printf("0 imagepath=%s, %d\n",u_ResImage, strlen(u_ResImage));
		if(pwndDSP55State != NULL)
		    pwndDSP55State->SetPropValueT("imagepath",u_ResImage);
		
		memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
		sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
		//printf("imagepath=%s\n",u_ResImage);
		if(pwndDSP28State != NULL)
		    pwndDSP28State->SetPropValueT("imagepath",u_ResImage);
		for(int i=0; i<u_nClassControlCount[1]; ++i)
		{	
			memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
			sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
			if(pwndRMTP12State[i] != NULL)
			    pwndRMTP12State[i]->SetPropValueT("imagepath",u_ResImage);
		}
	}
	
	return TRUE;	
}
void	OnUpdateA(CtmWnd* pwndSender)
{
	UpdateState();
	
	static	int nCount = 0;
  	int nTime = 500;
	if(nCount >=10)
	{
		nCount =0;
	  /*  if(g_ptaskdsp28_M3 !=NULL)
		    g_ptaskdsp28_M3->ReqValues(2, sizeof(g_DspM3VersionID) / sizeof(g_DspM3VersionID[0]), g_DspM3VersionID,&nTime);
    	if(g_ptaskdsp28_M3 !=NULL)
	    	g_ptaskdsp28_M3->ReqValues(2, sizeof(g_DspAxisID) / sizeof(g_DspAxisID[0]), g_DspAxisID,&nTime);
*/
		for(int i = 0; i < n_vernum; i++)
		{
			if(pwnsEditM3Version[i] != NULL)
				pwnsEditM3Version[i]->Update();
		}
	}
	
	SetAxisDate(pwndeditM3_App,1);  
	SetAxisDate(pwndeditM3_Boot,2); 
	nCount++;
}

void  	OnDestroyA(CtmWnd* pwndSender)
{
	
}

void	UpdateState()
{
	if(g_wDspStatus)
	{
		if(u_nDSP55OldState != g_wDspStatus)
		{
			u_nDSP55OldState =g_wDspStatus;
			memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
			sprintf(u_ResImage+strlen(u_ResImage),"%s",ImageName[1]);
			//printf("55:imagepath=%s\n",u_ResImage);
			if(pwndDSP55State != NULL)
			{
			    pwndDSP55State->SetPropValueT("imagepath",u_ResImage);
			    pwndDSP55State->Update();
			}
		}
		//printf("g_ptaskdsp28_M3=%x\n",g_ptaskdsp28_M3);
		/*if(g_ptaskdsp28_M3!=NULL)
		{
			u_nDSP28State = !g_ptaskdsp28_M3->GetOnLineStatus();
			if(u_nDSP28State)
			{
				if(u_nDSP28OldState != u_nDSP28State)
				{
					u_nDSP28OldState =u_nDSP28State;
					memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
					sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[1]);
					//printf("28:imagepath=%s\n",u_ResImage);
					if(pwndDSP28State != NULL)
					{
					    pwndDSP28State->SetPropValueT("imagepath",u_ResImage);
					    pwndDSP28State->Update();
					}
				}
				
				for(int i=0; i<u_nClassControlCount[1]; ++i)
				{
					u_nRMTP12State[i] = (GetDBValue("M3_STATE_TEMPERCARD1_LINESTATE").lValue>>i) &0x01;
					if(u_nRMTP12State[i])
					{
						if(u_nRMTP12OldState[i] != u_nRMTP12State[i])
						{
							u_nRMTP12OldState[i] =u_nRMTP12State[i];
							memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
							sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[1]);
							if(pwndRMTP12State[i] != NULL)
							{
							    pwndRMTP12State[i]->SetPropValueT("imagepath",u_ResImage);
							    pwndRMTP12State[i]->Update();
							}
						}
					}
					else
					{
						if(u_nRMTP12OldState[i] != u_nRMTP12State[i])
						{
							u_nRMTP12OldState[i] =u_nRMTP12State[i];
							memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
							sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
							if(pwndRMTP12State[i] != NULL)
							{
							    pwndRMTP12State[i]->SetPropValueT("imagepath",u_ResImage);
							    pwndRMTP12State[i]->Update();
							}
						}
					}
				}

      //AMMETER
				//u_nPWMState= GetDBValue("M3_STATE_POWERCARD1_LINESTATE").lValue;
				u_nPWMState= GetDBValue("PWMTRCARD1_INTERFACE_DWVERSION1").lValue;
				for(int i=0; i<u_nClassControlCount[3]; ++i)
				{
					if(u_nPWMState)
					{
						if(u_nPWMOldState != u_nPWMState)
						{
							u_nPWMOldState =u_nPWMState;
							memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
							sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[1]);
							if(pwndPWMState[i] != NULL)
							{
							    pwndPWMState[i]->SetPropValueT("imagepath",u_ResImage);
							    pwndPWMState[i]->Update();
							}
						}
					}
					else
					{
						if(u_nPWMOldState != u_nPWMState)
						{
							u_nPWMOldState =u_nPWMState;
							memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
							sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
							if(pwndPWMState[i] != NULL)
							{
							    pwndPWMState[i]->SetPropValueT("imagepath",u_ResImage);
							    pwndPWMState[i]->Update();
							}
						}
					}
				}
				
				//TMIO
				for(int i=0; i<u_nClassControlCount[8]; ++i)
				{
					u_nTMIOState[i]= (GetDBValue("M3_STATE_REMOTEIO_LINESTATE").lValue>>i) &0x01;
					if(u_nTMIOState[i])
					{
						if(u_nTMIOOldState[i] != u_nTMIOState[i])
						{
							u_nTMIOOldState[i] =u_nTMIOState[i];
							memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
							sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[1]);
							if(pwndTMIOState[i] != NULL)
							{
							    pwndTMIOState[i]->SetPropValueT("imagepath",u_ResImage);
							    pwndTMIOState[i]->Update();
							}
						}
					}
					else
					{
						if(u_nTMIOOldState[i] != u_nTMIOState[i])
						{
							u_nTMIOOldState[i] =u_nTMIOState[i];
							memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
							sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
							if(pwndTMIOState[i] != NULL)
							{
							    pwndTMIOState[i]->SetPropValueT("imagepath",u_ResImage);
							    pwndTMIOState[i]->Update();
							}
						}
					}
				}
				
				//Axis
				for(int i=0; i<u_nClassControlCount[10]; ++i)
				{
					if(u_nAxisState[i])
					{
						if(u_nAxisOldState[i] != u_nAxisState[i])
						{

						}
					}
					else
					{
						if(u_nAxisOldState[i] != u_nAxisState[i])
						{
							u_nAxisOldState[i] =u_nAxisState[i];

						}
					}
				}
			}
			else
			{
				if(u_nDSP28OldState != u_nDSP28State)
				{
					u_nDSP28OldState =u_nDSP28State;
					memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
					sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
					//printf("imagepath=%s\n",u_ResImage);
					if(pwndDSP28State != NULL)
					{
					    pwndDSP28State->SetPropValueT("imagepath",u_ResImage);
					    pwndDSP28State->Update();
					}
					//	rmtp12 offline
					for(int i=0; i<u_nClassControlCount[1]; ++i)
					{
						u_nRMTP12OldState[i] =FALSE;
						memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
						sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
						if(pwndRMTP12State[i] != NULL)
						{
						    pwndRMTP12State[i]->SetPropValueT("imagepath",u_ResImage);
						    pwndRMTP12State[i]->Update();	
						}
					}
					
					//AMMETER
					for(int i=0; i<u_nClassControlCount[3]; ++i)
					{
						u_nPWMOldState =FALSE;
						memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
						sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
						if(pwndPWMState[i] != NULL)
						{
						    pwndPWMState[i]->SetPropValueT("imagepath",u_ResImage);
						    pwndPWMState[i]->Update();
						}
					}
					//TMIO
					for(int i=0; i<u_nClassControlCount[8]; ++i)
					{
						u_nTMIOOldState[i] =FALSE;
						memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
						sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
						if(pwndTMIOState[i] != NULL)
						{
						    pwndTMIOState[i]->SetPropValueT("imagepath",u_ResImage);
						    pwndTMIOState[i]->Update();
						}
					}
					//Axis
					for(int i=0; i<u_nClassControlCount[10]; ++i)
					{
						u_nAxisOldState[i] =FALSE;
						memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
						sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
					}
				}
			}
		}*/
	}
	else
	{ 
		if(u_nDSP55OldState != g_wDspStatus)
		{
			//dsp55 offline
			u_nDSP55OldState =g_wDspStatus;
			memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
			sprintf(u_ResImage+strlen(u_ResImage),"%s",ImageName[0]);	
			//printf("imagepath=%s\n",u_ResImage);
			if(pwndDSP55State != NULL)
			{
				pwndDSP55State->SetPropValueT("imagepath",u_ResImage);
			    pwndDSP55State->Update();
			}
			
			//dsp28 offline
			u_nDSP28OldState =FALSE;
			memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
			sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
			//printf("imagepath=%s\n",u_ResImage);
			if(pwndDSP28State != NULL)
			{
			    pwndDSP28State->SetPropValueT("imagepath",u_ResImage);
			    pwndDSP28State->Update();
			}
			
			//rmtp12 offline
			for(int i=0; i<u_nClassControlCount[1]; ++i)
			{
				u_nRMTP12OldState[i] =FALSE;
				memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
				sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
				if(pwndRMTP12State[i] != NULL)
				{
				    pwndRMTP12State[i]->SetPropValueT("imagepath",u_ResImage);
				    pwndRMTP12State[i]->Update();
				}
			}
						
			//AMMETER
			for(int i=0; i<u_nClassControlCount[3]; ++i)
			{
				u_nPWMOldState =FALSE;
				memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
				sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
				if(pwndPWMState[i] != NULL)
				{
				    pwndPWMState[i]->SetPropValueT("imagepath",u_ResImage);
				    pwndPWMState[i]->Update();
				}
			}
			//TMIO
			for(int i=0; i<u_nClassControlCount[8]; ++i)
			{
				u_nTMIOOldState[i] =FALSE;
				memset(u_ResImage+nImagePathLenth,0,sizeof(u_ResImage)-strlen(u_ResImage));
				sprintf(u_ResImage+nImagePathLenth,"%s",ImageName[0]);
				if(pwndTMIOState[i] != NULL)
				{
				    pwndTMIOState[i]->SetPropValueT("imagepath",u_ResImage);
				    pwndTMIOState[i]->Update();
				}
			}
			//Axis
		}
	}
}

void  SetAxisDate(CtmWnd* pwnd,int index) //20160127, 日期由ASCII碼轉化為char數組
{
	char pDataID[256],sz[256];

	DWORD dwValue = 0x30303030;
	if(pwnd != NULL)
	{
			pwnd->GetPropValueT("promptID", &pDataID, sizeof(pDataID));
			//printf("pDataID:%s\n",pDataID);
			if((pDataID != NULL) && (pDataID[0] != '\0'))
				dwValue = GetDBValue(pDataID).lValue;
			//printf("dwValue:%x\n",dwValue);
			if(index==1)
				sprintf(sz,"APP: %s",ChangeDwordTOChar(dwValue));
			else if(index==2)
				sprintf(sz,"BOOT:%s",ChangeDwordTOChar(dwValue));
			else if(index==3)
				sprintf(sz,"1:%s",ChangeDwordTOChar(dwValue));
			else if(index==4)
				sprintf(sz,"2:%s",ChangeDwordTOChar(dwValue));
			else
				sprintf(sz,"%s",ChangeDwordTOChar(dwValue));
			pwnd->SetPropValueT("text", sz);
			pwnd->Update();
	}
}

BOOL  GetDeviceUsed(CtmWnd* pwnd)
{
	BOOL bResult = FALSE;
	char pDataID[256];
	DWORD dwValue = 0;
	if(pwnd != NULL)
	{
			pwnd->GetPropValueT("promptID", &pDataID, sizeof(pDataID));
			//printf("pDataID:%s\n",pDataID);
			if((pDataID != NULL) && (pDataID[0] != '\0'))
				dwValue = GetDBValue(pDataID).lValue;
			//printf("dwValue:%x\n",dwValue);
			if(dwValue)
			  return bResult = TRUE;
	}
	return bResult;
}

char*  ChangeDwordTOChar(DWORD dwValue)
{
    memset(u_cszAxisDate, 0, sizeof(u_cszAxisDate));
    if((!(dwValue > 0x30303131) && (dwValue <0x39394356))) //20160127, 0x0011-0x99CV
        dwValue =0x30303030;
    u_cszAxisDate[3] = *(BYTE*)&dwValue;
    u_cszAxisDate[2] = *((BYTE*)&dwValue+1);
    u_cszAxisDate[1] = *((BYTE*)&dwValue+2);
    u_cszAxisDate[0] = *((BYTE*)&dwValue+3);
    //printf("u_cszAxisDate:%s\n", u_cszAxisDate);
    
    return u_cszAxisDate;
}

void	SetAllVisible(CtmWnd** pwnd, int* NodeID, int nCount, BOOL bUpdate)
{
	for(int i=0;i<nCount;++i)
	{
		if(pwnd[i] != NULL)	
		{
			if(NodeID[i]==0)
			{
				pwnd[i]->SetPropValueT("wtype", 0x30);
				pwnd[i]->SetPropValueT("visible",(double)FALSE);
				if(bUpdate==TRUE)
				{	
					pwnd[i]->Update();
				}
			}	
		}
	}		
}

void		SetVisible(CtmWnd* pwnd,BOOL  bVisible, WORD  wType)
{
	if(pwnd != NULL)
	{
		if(wType)
		{
			if(bVisible)
			    pwnd->SetPropValueT("wtype",wType);
			else
				pwnd->SetPropValueT("wtype",wType|0x30); 
		}
		else
		{
			if(bVisible)
				pwnd->SetPropValueT("wtype",0x23);
			else
        		pwnd->SetPropValueT("wtype",0x30);
        }
		pwnd->Update();
			
		pwnd->SetPropValueT("visible",(double)bVisible);
		pwnd->Update();
	}
}
