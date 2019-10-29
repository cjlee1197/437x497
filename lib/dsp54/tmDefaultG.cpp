/*===========================================================================+
|  Class    : CtmDefaultG	                                             |
|  Task     : Default Server Function	    	                     			 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 			                                                     |
|  Version  : 		                                                         |
|  Creation : 				                                                 |
|  Revision : 			  				                                     |
+===========================================================================*/
#include	"tmDefaultG.h"
#include	"../../tmconfig.h"
#include	"../../commonaction.h"
#include  "../../selvat_ancestor.h"
#include	"../../iotable.h"
#include	"../libDataSave.h"
/*===========================================================================+ 
|				                                                            				 | 
+===========================================================================*/
char*   pParaPassword[] = {
						"MSET_MLD_COOLING_NULL_TIME",  
						"MSET_SCRW_CUSHION_NULL_OPTION",
						"MSET_SCRW_INJ_STEP1_TIME", 
						"MSET_SCRW_CUSHION_NULL_POSITION",   
						"MSET_SCRW_CUSHION_NULL_PRESSURE",
						"MSET_SCRW_HLDP_STEP1_TIME",
						"MSET_SCRW_HLDP_STEP2_TIME",
						"MSET_SCRW_HLDP_STEP3_TIME",
						"MSET_SCRW_HLDP_STEP4_TIME",
						"MSET_SCRW_HLDP_STEP5_TIME",
						"MSET_SCRW_HLDP_STEP6_TIME",
						"PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT",/*joyce20121206 �}���`���k�s �ɥΦ������wDisplayPrecision���K�X�O�@�ﶵ*/
            NULL
            };

/*===========================================================================+
+===========================================================================*/
	char*   pDSPLub1[] = {
						"MACH_LUB_ON_NULL_SHOTCOUNT",                   //�۰ʼ��ƼҼ�    
						"MACH_ALARM_ON_NULL_ALARMLUBRICATTIME",         //����ĵ���ɶ�    
						"MACH_LUB_ON_NULL_DELAYTIME",                   //���ƶ��j�ɶ�    
						"MACH_LUB_ON_NULL_COUNTER",                     //�۰ʼ��Ʀ���    
						"MACH_LUB_ON_NULL_TIME",                        //�۰ʼ��ƫO���p��
            NULL
            };
	char*   pDSPLub2[] = {
						"MACH_LUB2_ON_NULL_SHOTCOUNT",                 //�۰ʼ��ƼҼ�    
						"MACH_LUB_ON_NULL_W019E",                      //����ĵ���ɶ�    
						"MACH_SCRW_INJ_NULL_W0184",                    //���ƶ��j�ɶ�    
						"MACH_SCRW_INJ_NULL_W0185",                    //�۰ʼ��Ʀ���    
						"MACH_LUB2_ON_NULL_TIME",                      //�۰ʼ��ƫO���p��
            NULL
            };
	char*   pDSPLub3[] = {
						"MACH_MTR_ON_NULL_W01D9",                      //�۰ʼ��ƼҼ�    
						"MACH_MTR_ON_NULL_W01D7",                      //����ĵ���ɶ�    
						"MACH_MTR_ON_NULL_W01DE",                      //���ƶ��j�ɶ�    
						"MACH_MTR_ON_NULL_W01DF",                      //�۰ʼ��Ʀ���    
						"MACH_MTR_ON_NULL_W01DC",                      //�۰ʼ��ƫO���p��(1DC&1DD)
            NULL
            };
	char*   pHMILub1B[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED20",           //�۰ʼ��ƼҼ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED22",           //����ĵ���ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED23",           //���ƶ��j�ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED24",           //�۰ʼ��Ʀ���    
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED20",         //�۰ʼ��ƫO���p��
            NULL
            };
	char*   pHMILub1A[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED25",           //�۰ʼ��ƼҼ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED26",           //����ĵ���ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED27",           //���ƶ��j�ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED28",           //�۰ʼ��Ʀ���    
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED21",         //�۰ʼ��ƫO���p��
            NULL
            };
	char*   pHMILub2B[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED31",           //�۰ʼ��ƼҼ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED33",           //����ĵ���ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED34",           //���ƶ��j�ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED35",           //�۰ʼ��Ʀ���    
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED22",         //�۰ʼ��ƫO���p��
            NULL
            };
	char*   pHMILub2A[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED36",           //�۰ʼ��ƼҼ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED37",           //����ĵ���ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED38",           //���ƶ��j�ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED39",           //�۰ʼ��Ʀ���    
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED23",         //�۰ʼ��ƫO���p��
          	 NULL
            };
	char*   pHMILub3B[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED42",           //�۰ʼ��ƼҼ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED44",           //����ĵ���ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED45",           //���ƶ��j�ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED46",           //�۰ʼ��Ʀ���    
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED24",         //�۰ʼ��ƫO���p��
            NULL
            };
	char*   pHMILub3A[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED47",           //�۰ʼ��ƼҼ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED48",           //����ĵ���ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED49",           //���ƶ��j�ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED50",           //�۰ʼ��Ʀ���    
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED25",         //�۰ʼ��ƫO���p��
            NULL
            };
/*===========================================================================+
+===========================================================================*/
char*			pszUserID[] = {		//for iNet, 2010-11-15  
	"USER_HMI_NULL_NULL_USER1ID",
	"USER_HMI_NULL_NULL_USER2ID",
	"USER_HMI_NULL_NULL_USER3ID",
	"USER_HMI_NULL_NULL_USER4ID"
};

char*			pszPasswdID[] = {
	"USER_HMI_NULL_NULL_USER1PASSWORD",
	"USER_HMI_NULL_NULL_USER2PASSWORD",
	"USER_HMI_NULL_NULL_USER3PASSWORD",
	"USER_HMI_NULL_NULL_USER4PASSWORD"
};

char*			pszPrivilegeID[] = {
	"USER_HMI_NULL_NULL_USER1PRIVILEGE",
	"USER_HMI_NULL_NULL_USER2PRIVILEGE",
	"USER_HMI_NULL_NULL_USER3PRIVILEGE",
	"USER_HMI_NULL_NULL_USER4PRIVILEGE"
};

char*			pszUserValue[4] = {
	"0001",
	"0002",
	"0003",
	"0004"
};

int			pszPasswdValue[4] = {
	714,
	718,
	716,
	1291
};

int			pszPrivilegeValue[4] = {
	0,
	1,
	5,
	8
};


#define		BLOCK_COM2NEWPOWERMATCH_ID    329//JOYCE2012-2-10 
/*===========================================================================+
+===========================================================================*/
CtmDefaultG*		g_pDefaultG		= NULL;
int							g_nDefaultCount	= 0;

CIniFile	 			m_DefaultIni;	

IMPLEMENT_DYNCREATE(CtmDefaultG, CtmDefault)

CtmDefaultG::CtmDefaultG(): CtmDefault()
{
	
}

CtmDefaultG::~CtmDefaultG()
{
	
}

int	CtmDefaultG::CreateSelf()
{
//	printf("int	CtmDefaultG::CreateSelf() \n");
	m_pszFileDefaultID 		= NULL;
	m_pszFileDefaultValue = NULL;
	m_pszFileAttrID		 		= NULL;
	m_pszFileAttrValue		= NULL;
	m_pszFileDefaultSID		= NULL;//FOR STRING TYPE
	m_pszFileDefaultSValue= NULL;//FOR STRING TYPE
	m_pszFileVersionSID 	= NULL;//FOR STRING VERSION
	m_pszFileVersionSValue= NULL;//FOR STRING VERSION
	
	memset(m_szDefaultID, 	0,sizeof(m_szDefaultID)  );
	memset(m_DefaultValue,	0,sizeof(m_DefaultValue) );
	memset(m_szDefaultSID, 	0,sizeof(m_szDefaultSID) );//FOR STRING TYPE
	memset(m_DefaultSValue,	0,sizeof(m_DefaultSValue));//FOR STRING TYPE
	memset(m_szVersionSID, 	0,sizeof(m_szVersionSID) );//FOR STRING VERSION
	memset(m_VersionSValue,	0,sizeof(m_VersionSValue));//FOR STRING VERSION
		
	for (int i = 0; i < CNT_INT_MAX_DEFAULT_NUM; i++)
	{
		m_szDefaultID[i] = new char[128];
		memset(m_szDefaultID[i], 	0, 128);
	}
	
	for (int i = 0; i < 16; i++)
	{
		m_szDefaultSID[i] = new char[128];//FOR STRING TYPE
		memset(m_szDefaultSID[i], 0, 128);//FOR STRING TYPE
		m_DefaultSValue[i] = new char[128];//FOR STRING TYPE //new for string value
		memset(m_DefaultSValue[i],0, 128);//FOR STRING TYPE

		m_szVersionSID[i] = new char[128];//FOR STRING VERSION
		memset(m_szVersionSID[i], 0,	128);//FOR STRING VERSION
		m_VersionSValue[i] = new char[128];//FOR STRING VERSION
		memset(m_VersionSValue[i],0,	128);//FOR STRING VERSION		
	}

	m_bVrt  = g_pExTable->GetTableValue(0,"D_VRT");
	m_bDC   = CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC;
	m_biNet = CtmConfig::GetInstance()->GetINETFlag(); //JOYCE2012-2-1 
	m_bStepLub = g_pExTable->GetTableValue(0,"D_STEPLUB"); /*JOYCE2012-7-12 No.12-028 ���i������*/
	m_bAmmeter = CtmConfig::GetInstance()->GetAmmeterFlag();
	m_bPMTEST = g_pExTable->GetTableValue(0,"D_PMTEST");
	m_bSVRT    = g_pExTable->GetTableValue(0,"D_SVRT");/*JOYCE2014-1-17 �¤G�O*/
	m_b28Clmp = g_pExTable->GetTableValue(0,"D_28CLAMP");/*28����}����*/
	m_bT11Port19 = g_pExTable->GetTableValue(0,"D_T11PORT19");/*wangli2015-10-12 16:30:03  5528 3300TON �S�����A�Ʒ�11�q�A�ĤG��RMTP12��CH7�����Ƥf*/
	m_bMold36  = g_pExTable->GetTableValue(0,"D_MDT36");
	m_bJUIII  = g_pExTable->GetTableValue(0,"D_JUIII");/*wangli2016-7-22 9:00:44 JUIII�N��*/
		
	GetFileName();
	Init();
	
	ReadLubOpt();/*JOYCE2013-10-30 Ū�� �ռҼ���/�ĤT�ռ��� �ﶵ����*/
	m_bALUB = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED83").lValue;  //JOYCE2013-10-30 g_pExTable->GetTableValue(0,"D_ALUB");  //JOYCE2011-8-23

	if((GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 2) || (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 3))
		m_b3LUB = TRUE;
	
	if(!m_bStepLub)
		ReadLubType();	/*//JOYCE2012-1-17  Ū���s/�¼���flag;��ܥ\��X*/
	
	//JOYCE20130410  if(m_bVrt | m_bDC)
	//JOYCE20130410  	 SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70", 1, FALSE); /*������B�ƦX��,���¼���*/
	
	m_bOLDLUB = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70").lValue;//0-NEW LUB 1-OLD LUB

	SetVersionInf(); //for version
	SetNodeID();//for set tempcard or powercard nodeid
	if(CtmConfig::GetInstance()->GetSramInit() <= 0) //�W�[���m�P�_
		{
		SetDefaultInit(); //�W�[default�]�w
		if(g_pExTable->GetTableValue(0,"D_ENERGY") || m_bAmmeter)//JOYCE2010-7-22 
  	 	{
    	ReadEnergyDataFrom();
   		}	
		//if(g_pExTable->GetTableValue(0,"D_MLDWGHT"))//JOYCE2011-6-7 
		//	{	
		//		ReadMoldWeight();
		//	}
		//if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED98").lValue & 0x0001)  //JOYCE2013-10-30 if(g_pExTable->GetTableValue(0,"D_SDOOR"))//JOYCE2011-11-2 
		//	{	
				ReadSDOOR();
		//	}
		if(g_pExTable->GetTableValue(0,"D_DACOM2"))//JOYCE2012-2-16 
				ReadBlock("/usr/Data/DA/daCom2_reset.cdb", BLOCK_COM2NEWPOWERMATCH_ID);
		
		ReadShotCount();//JOYCE2010-10-12 �ֿn�B��Ҽ�
		ReadAutoRunTime();//wangli2014-6-5 11:35:59 Auto Run Time
		ReadMoldWeight();/*JOYCE2012-5-3 �Ҩ㭫�q,�зǶ}��*/
		ReadFlowRatio();	/*JOYCE2012-6-7 �y�q�t�Ƥ��i�Q���m*/
		ReadEjtData();	/*JOYCE2013-6-13 ���ƺާ��h��,����A�t�ο�� ���i�Q���m*/
		ReadRoteTimeOpt();//wangli2014-9-10 15:45:53 
		if(m_bSVRT)
		{
			ReadParallelCnt();/*zhongjw 2015-4-13 11:33:51*/
			ReadOpnCntData();
			ReadWarningCnt();/*zhongjw 2015-12-23 �Үy�պA�e���A������O�ʴ��Oĵ�����Ƥ��i���m*/
		}
		if(m_bVrt || m_bSVRT)
		{
			ReadInjhSynchro();/*zhongjw 2016-5-19 �G�O�� ��ҦP�B�`�g�\�� ���i�Q���m*/
		}
		CtmConfig::GetInstance()->SetLogoTime(30); //wangli2014-12-22 16:03:19  Aoto into overview time
//	  if(!m_bDC)//wangli2014-12-19 15:52:07 mark  ����`�g�\������אּ�M������@�P
//		{
//			ReadOneJarInjOpt();	
//		    if(GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED17").lValue)/*JOYCE2014-9-30  ������bReadOneJarInjOpt���Z���A����`�g��ܨϥΦZ�A�x�e�g�h�Z���q�{0.1*/
//		    {
//				SetDBValue("MSET_SCRW_DCMP_NULL_BEFORERECOVERYOFFSET", 1);	
//		    }
//		}
//	  else
//	   	{
			ReadOneJarInjOpt();		   		
//	   	}
		
		if((!m_bOLDLUB) && (!m_bStepLub))
		{
			ReadLubricate2();//JOYCE2011-8-3 lubricate 
		}
			
		 /*wangli2016-1-11 9:31:14�¼���/���i�����ƮɡA�}���۰ʼ��ƤѼƤ]�O���i���m*/
		ReadLubricate3();/*wangli2015-11-13 16:03:29*/

		if(m_bSVRT && (!m_bDC))
			SetDefaultBySVRT();
			
		/*���m�Z�ϥΪ�l�ȧY�i,�R�������,�קK�����m�ĤG���}���ɥhŪ�������*/
//			{
//   			usleep(50*1000);
//   			system("rm -rf /conf/oldlub.cdb");
//			}	

		if(m_bDC)
			SetMrtTempNUse();

		ReadNotclmptime();/*wangli2015-12-10 17:03:05*/

			usleep(50*1000);
			system("rm -rf /usr/Data/temp/TempOil.cdb");

	 }

	if(m_bOLDLUB && (!m_bStepLub))
 		ReadOldLub();		//fuxy, 2012-1-16, for lub old  ��s�{�ǥB�����m�ɨ��^��ƭ�  /*JOYCE2014-9-29 �¼��Ƥ]�אּ���i���m,�}��Ū��*/

	if((!m_bOLDLUB) && (!m_bStepLub))
  	{
  		ReadLubricate1();//JOYCE2011-9-28 ���ưѼƪ�l�ȵL�ݭ��m�Y�iŪ��
  		SetLubricate(); //JOYCE2011-8-3 lubricate,�}���P�B
  	}
	
	if(!m_bDC)
		ReadParaPassword();//JOYCE2012-3-15 
	
	ReadCOM2AmmeterOpt();/*JOYCE2013-12-12 Ū�� COM2�q��opt*/
  ReadWarmTime();      /*JOYCE2013-12-230 ���N�Ұʭp��,�אּ�L�ݭ��m�NŪ�� �ɤB������*/
	if(m_bDC)
		{
			ReadRotateOpt();     /*JOYCE2013-12-26 Ū�� ��L�ﶵ*/ //wangli2015-3-10 11:50:08������bReadCOM2AmmeterOpt()���Z��
			//Read28RotateData();
			SetDBValue("MACHB_IJU2_IJU_NULL_W10AD",0, FALSE); /*JOYCE2014-6-18  ��L���տﶵ�}���ƭ쬰0*/
			if(!(GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED32").lValue))
	 				SetDBValue("MACHB_SCRW2_INJ_NULL_W10CE", 0, FALSE);       /*��ҫO�@�p��*/
		}
			
	//�B�z�۰ʲM��
	if(!g_pExTable->GetTableValue(0,"D_TM3160") && !g_pExTable->GetTableValue(0,"D_TM3360"))//JOYCE2011-8-25 //�t�X��Z replace 386 ,add judgement
	{
	if(GetDBValue("MSET_SCRW_INJ_NULL_AUTOPURGEOPTION").lValue == 1) 
		SetDBValue("MSET_SCRW_INJ_NULL_AUTOPURGEOPTION",0,FALSE);
		
	if(GetDBValue("MSETB_SCRW2_INJ_NULL_AUTOPURGEOPTION").lValue == 1) 
		SetDBValue("MSETB_SCRW2_INJ_NULL_AUTOPURGEOPTION",0,FALSE);
	}
	
	//�B�z�¤h�Z�x�����O
	if(g_pExTable->GetTableValue(0,"D_WSMVAT"))
		{
			int chgseg = GetDBValue("MCONF_SCREW_RECOVERY_NULL_RANK").lValue;
			for (int i = 0; i < chgseg; i++)
			{
				char pszID[128]="\0";
				long long lChgPresMax = GetDBValue("SYSX_SCREW_RECOVERY_NULL_MAXPRESSURE").lValue;
				sprintf(pszID,"MSET_SCRW_RCV_STEP%d_PRESSURE",i+1);	
				if(GetDBValue(pszID).lValue != 0xFFFF)
					SetDBValue(pszID,lChgPresMax,FALSE);
			}
			
			if(g_pExTable->GetTableValue(0,"D_DC"))
				{
					int chgsegB = GetDBValue("MCONF_SCREW2_RECOVERY_NULL_RANK").lValue;
					for (int i = 0; i < chgsegB; i++)
					{
						char pszBID[128]="\0";
						long long lChgBPresMax = GetDBValue("SYSX_SCREW2_RECOVERY_NULL_MAXPRESSURE").lValue;
						sprintf(pszBID,"MSETB_SCRW2_RCV_STEP%d_PRESSURE",i+1);	
						if(GetDBValue(pszBID).lValue != 0xFFFF)
							SetDBValue(pszBID,lChgBPresMax,FALSE);
					}
				}
			
			long	lMachTon = GetDBValue("MACH_MLD_CLOSE_CLAMPUP_ALARMTONES").lValue;
  		SetDBValue("SYS_HMI_NULL_NULL_MACHINETONES",lMachTon);
  		SetDBValue("SYSX_MOLD_CLOSE_CLAMPUP_MAXFORCE",lMachTon);
			
			SetDBValue("MSET_MLD_CLOSE_CLAMPUP_PRESSURE",140,FALSE);
			
			if((GetDBValue("MSET_MLD_CLOSE_NULL_W04B7").lValue == 0)	|| (GetDBValue("MSET_MLD_CLOSE_NULL_W04B7").lValue == 0xFFFF))								
				SetDBValue("MSET_MLD_CLOSE_NULL_W04B7",OilToPosi(GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue),FALSE);	
			
			SetAdjCnt();//JOYCE2012-8-1
		}
	
 	if(g_pExTable->GetTableValue(0,"D_AUTOADJ"))/*JOYCE2014-6-20 */
 	{
			long	lMachTon = GetDBValue("MACH_MLD_CLOSE_CLAMPUP_ALARMTONES").lValue;
  		SetDBValue("SYS_HMI_NULL_NULL_MACHINETONES",lMachTon);
  		SetDBValue("SYSX_MOLD_CLOSE_CLAMPUP_MAXFORCE",lMachTon);			
			
			SetDBValue("MSET_MLD_CLOSE_CLAMPUP_PRESSURE",140,FALSE);
			
			SetAdjCnt();
 	}	

	//�B�zstateconfig.ini
	if(m_bALUB)
		{
			tmDATE	date;
			long		count=0;
			int			Py=360,Pm=30; //�t��
			
			GetDate(&date);
			//printf("y:%d,m:%d,d:%d\n",date.year,date.month,date.day);
			count	= Py*(date.year%2000)+Pm*(date.month-1)+date.day;
			if(CtmConfig::GetInstance()->GetSramInit() <= 0)
				SetDBValue("SYSX_MOLDHEGHT_MOLDHEGHT_NULL_KEEPOPTION",count);
				//printf("%d\n",GetDBValue("SYSX_MOLDHEGHT_MOLDHEGHT_NULL_KEEPOPTION").lValue);
		}
		
	if(g_pExTable->GetTableValue(0,"D_MUCELL"))
	{
		if((GetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA1").lValue) == 1) //fuxy, 2012-1-13 
			SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA1",0);  		//���q�Z��}��, �o�w��������A  
		
		if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED12").lValue) //fuxy, 2012-3-13  
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED12",0, FALSE); 
	}

	if(m_bSVRT)/*JOYCE2014-1-17*/
	{
		SetDBValue("MACH_SHUT_SHUT_NULL_W026F",0);/*�ԧ����,�}���Z�ƭ쬰0*/
		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED80",0);/*��ʹh�Okey���,�}���Z�ƭ쬰0*/
		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED88",0);/*���\�ե���Ǧ�m���,�}���Z�ƭ쬰0*/
		SetDBValue("MACH_SLUC_SLUC_NULL_W0278",0);/*�Үy�պA�e���ը��Ҧ��A�}���Z�ƭ쬰���ΡAZhongjw 2015-8-10*/
		SetDBValue("MACH_SLUC_SLUC_NULL_W0279",0);/*�Ա�w���ʴ�BYPASS�A�}���Z�ƭ쬰���ΡAzhongjw 2015-8-12*/
		ReadTBARData();
		if(!m_bJUIII)
			TestMode();
	}	

	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED13").lValue)/*JOYCE2012-7-12 �}���ݵ�overview�X�ӦZ,�A�ھڱ���۰ʸ��X�e��AutoToQMC.txt*/
		{
			int	nTemp  = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED11").lValue;
			nTemp |= 0x8100;
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED11", nTemp);
		}

	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED100").lValue) /*JOYCE2013-12-11 �t��->�պA�e��,�ѼƱK�X�O�@�\��,flag�}���ɲM0*/  
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED100",0, FALSE); 

	if(GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED31").lValue) /*JOYCE2013-12-25�ū�->�պA�e��,���N�Ұʴ���,flag�}���ɲM0*/ 
		SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED31",0, FALSE); 

	if(GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED6").lValue) /*JOYCE2014-1-8 ���N�Ұʮɶ�,�˭p��*/ 
		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED6",0, FALSE); 

 	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED74", 0, FALSE);/*JOYCE2014-9-30 flag�M0*/

	if(GetDBValue("MSET_PRODUCT_PRODUCT_NULL_W0705").lValue == 0xFFFF)//wangli2015-1-7 11:15:16�ª��{�ǨS�����\��A�Ȭ�65535�A��0
		SetDBValue("MSET_PRODUCT_PRODUCT_NULL_W0705", 0); 
 	SetDBValue("M3_CONF_TEMPERCARD_AUTOPID_CMD", 0, FALSE);     /*JOYCE2015-3-20 �Ʒ�AUTOPID OPT �M0*/
 	if(m_bMold36)
 		SetDBValue("M3_CONF_TEMPERCARD_MOLD_AUTOPID_CMD", 0, FALSE);     /*wangli2015-10-14 15:53:12 �ҷ�AUTOPID OPT �M0*/
 	

	MoldTempSet();//wangli2014-11-6 11:26:24
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77", 0,FALSE); /*wangli2015-5-22 12:44:44 ���ҧC���פ��m��׬�2 ��flag �A�}���M0*/
	SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED40", 0x12, FALSE); 
	/*wangli2015-5-22 �������O�y�ɡA���ҧC���פ��m��׬�2 ���Ҩ�flag�]�G�O���B�¤G�O�������|�����O�y�^
	��Ū�ª��Ҩ�ɡAMCONF_OTHERS_OTHERS_NULL_RESERVED40 ���Ȭ�0 */

	if(m_bJUIII)
		ReadSetGrad();

	
		usleep(50*1000);
		system("rm -rf /usr/Data/DA/daCom2_reset.cdb");//DACOM2
		
	if(m_b28Clmp)
		{
			char pDataID[256] = "\0";
			for(int i=0; i<4; i++)
			{
				sprintf(pDataID,"M3_CONF_CARD_LOCAL_INTERFACE_IO_DAOUT%d", i+1);
			
				if(pDataID != NULL && pDataID[0] != '\0')
				{
					SetDBValue(pDataID, 0, FALSE);
				}
			}

			SetDBValue("M3_CONF_CARD_LOCAL_CONFIG_ID", 0);
		}	
	

	return 0;
}

int	CtmDefaultG::FreeSelf()
{
	delete []m_pszFileDefaultID;
	delete []m_pszFileDefaultValue;
	delete []m_pszFileAttrID;
	delete []m_pszFileAttrValue;
	delete []m_pszFileDefaultSID;//FOR STRING TYPE
	delete []m_pszFileDefaultSValue;//FOR STRING TYPE
	delete []m_pszFileVersionSID;//FOR STRING VERSION
	delete []m_pszFileVersionSValue;//FOR STRING VERSION
	
	for (int i = 0; i < CNT_INT_MAX_DEFAULT_NUM; i++)
	{
		delete [](m_szDefaultID[i]);
	}
	
	for (int i = 0; i < 16; i++)	
	{
		delete [](m_szDefaultSID[i]);//FOR STRING TYPE
		delete [](m_DefaultSValue[i]);//FOR STRING TYPE

		delete [](m_szVersionSID[i]);//FOR STRING VERSION
		delete [](m_VersionSValue[i]);//FOR STRING VERSION
	}
		
	return CtmDefault::FreeSelf();
}


int		CtmDefaultG::SetDefault()
{
	int	nResult = 0;
	SetDefaultBy8C();
	SetRUBDefaultBy86();
	//SetDefaultJ5();  //Andy 20081018
	SetDefaultByDC();  //Andy 20090724
	//JOYCE2012-1-13 if(!g_pExTable->GetTableValue(0,"D_SLUB")) //JOYCE2011-3-28 ADD judgemet
	//JOYCE2012-1-13 SetDefaultByF2();  //JOYCE2011-3-3 FOR LUB2
	if(m_bOLDLUB && (!m_bStepLub))
		SetDefaultByF2();  //JOYCE2011-3-3 FOR LUB2
	
	SetDefaultByF4(); //JOYCE2011-7-7
	return nResult;	
}

void	CtmDefaultG::SetDefaultBy8C()
{
	DWORD		dwValue8C,dwValue9A,dwValueDE;
	char*		pszIDName	= NULL;
	char*		pszValue	= NULL;
	char		szValueName[32];
	
	dwValue8C = GetDBValue("CONF_CODE_WRITE_NULL_DATA7").lValue;		
	dwValue9A = GetDBValue("CONF_CODE_WRITE_NULL_DATA14").lValue;	
	dwValueDE = (GetDBValue("CONF_CODE_READ_NULL_OILVATDATA").lValue&0x0FFF);		

	if 		(dwValue8C <= 0x8420)	sprintf(szValueName, "%s%s", "Default", "8420");
	else if (dwValue8C <= 0x8467) sprintf(szValueName, "%s%s", "Default", "8467");
	else if (dwValue8C <  0x8470) sprintf(szValueName, "%s%s", "Default", "8470");
	else if (dwValue8C <= 0x848B) sprintf(szValueName, "%s%s", "Default", "848B");
	else						sprintf(szValueName, "%s%s", "Default", "84A0");
	
	/*JOYCE2013-12-25 mark //�B�z���N�Ұʭp��
	if(!m_bPMTEST)
	{
	if((dwValue9A	== 0x0660)||(dwValue9A	== 0x0680))
																		SetDBValue("MSET3_TMP1_HEATERON_WARM_TIME", (long)10*6000,FALSE);
	else
		{
			if(dwValue8C >= 0x8490)				SetDBValue("MSET3_TMP1_HEATERON_WARM_TIME", (long)20*6000,FALSE);
			else if(dwValue8C >= 0x8470)	SetDBValue("MSET3_TMP1_HEATERON_WARM_TIME", (long)15*6000,FALSE);
			else													SetDBValue("MSET3_TMP1_HEATERON_WARM_TIME", (long)10*6000,FALSE);
		}
	}
	*/
	//uper
	//�B�z��Ҭy�q�t��
	if ((CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC)&&!g_pExTable->GetTableValue(0,"D_VRDC"))
	{	
					if(dwValueDE >= 0x0800)	SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA5", (long)35,FALSE);
		else	if(dwValueDE >= 0x0530)	SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA5", (long)50,FALSE);
		else													SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA5", (long)85,FALSE);
	}		
	//uper
	m_DefaultIni.OpenFile("outerconfig.ini");
	m_DefaultIni.ReadString("default_config", "Default8CID", 	NULL, &pszIDName);
	m_DefaultIni.ReadString("default_config", szValueName,		NULL, &pszValue);
	m_DefaultIni.CloseFile();
	
	if ((pszIDName != NULL) && (pszValue != NULL))
	{
		m_ReadData.OpenFileStr(pszIDName);
		m_ReadData.OpenFileInt(pszValue);
		int nNum = m_ReadData.ReadData(m_szDefaultID);
		m_ReadData.ReadData(&m_DefaultValue[0]);
		
		for (int i = 0 ; i < nNum; i++)
		{
			SetDBValue(m_szDefaultID[i], m_DefaultValue[i],FALSE);
    	}
    } 
    delete []pszIDName;
    delete []pszValue;
}

void	CtmDefaultG::SetRUBDefaultBy86()
{
	DWORD		dwValue;
	char*		pszIDName	= NULL;
	char*		pszValue	= NULL;
	char		szValueName[32];
	
	dwValue = GetDBValue("CONF_CODE_WRITE_NULL_DATA4").lValue;
	if ((dwValue & 0x0030) == 0x0030)
	{
		sprintf(szValueName, "%s", "DefaultRubE44");
		SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED60", 1);		//judge RUB,flag;
	}
	else
	{
		dwValue = GetDBValue("CONF_CODE_WRITE_NULL_DATA7").lValue;
		if (dwValue <= 0x8420)
			sprintf(szValueName, "%s", "DefaultRubE028420");
		else
			sprintf(szValueName, "%s", "DefaultRubE02");
		
		SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED60", 0);	//judge RUB,flag; 
		
	}
	
	m_DefaultIni.OpenFile("outerconfig.ini");
	m_DefaultIni.ReadString("default_config", "DefaultRubID", 	NULL, &pszIDName);
	m_DefaultIni.ReadString("default_config", szValueName,		NULL, &pszValue);
	m_DefaultIni.CloseFile();
	
	if ((pszIDName != NULL) && (pszValue != NULL))
	{
		m_ReadData.OpenFileStr(pszIDName);
		m_ReadData.OpenFileInt(pszValue);
		
		int nNum = m_ReadData.ReadData(m_szDefaultID);
		m_ReadData.ReadData(&m_DefaultValue[0]);

		for (int i = 0 ; i < nNum; i++)
    		SetDBValue(m_szDefaultID[i], m_DefaultValue[i],FALSE);
    }    
	delete []pszIDName;
    delete []pszValue;

}

void	CtmDefaultG::SetDefaultByDC() //Andy 20090724
{
	DWORD		dwValueDC,dwValue8C;
	char*		pszIDName	= NULL;
	char*		pszValue	= NULL;
	char		szValueName[32];
	char		psz[5];
	memset(psz,0,sizeof(psz));
	
	dwValueDC = GetDBValue("CONF_CODE_READ_NULL_CREATEDATA").lValue;
	dwValue8C = GetDBValue("CONF_CODE_WRITE_NULL_DATA7").lValue;
	
	if ((dwValueDC & 0x0002) &&((dwValue8C&0xFF00) == 0x8400)) //J5�O���ѱM������
		SetDefaultJ5();

	HEX2String((WORD)dwValueDC,psz);
	sprintf(szValueName, "%s%s", "DefaultDC", psz);
	
	m_DefaultIni.OpenFile("outerconfig.ini");
	m_DefaultIni.ReadString("default_config", "DefaultDCID", 			NULL, &pszIDName);
	m_DefaultIni.ReadString("default_config", szValueName,				NULL, &pszValue);
	m_DefaultIni.CloseFile();
	
	if ((pszIDName != NULL) && (pszValue != NULL))
		{
		m_ReadData.OpenFileStr(pszIDName);
		m_ReadData.OpenFileInt(pszValue);
		
		int nNum = m_ReadData.ReadData(m_szDefaultID);
		m_ReadData.ReadData(&m_DefaultValue[0]);

		for (int i = 0 ; i < nNum; i++)
    		SetDBValue(m_szDefaultID[i], m_DefaultValue[i],FALSE);
    }    	
	delete []pszIDName;
  delete []pszValue;
}	

void	CtmDefaultG::SetDefaultJ5() //Andy 20081017
{
	char*		pszIDName	= NULL;
	char*		pszValue	= NULL;

	m_DefaultIni.OpenFile("outerconfig.ini");
	m_DefaultIni.ReadString("default_config", "J5DefID", 			NULL, &pszIDName);
	m_DefaultIni.ReadString("default_config", "J5DefValue",		NULL, &pszValue);
	m_DefaultIni.CloseFile();

	if ((pszIDName != NULL) && (pszValue != NULL))
		{
		m_ReadData.OpenFileStr(pszIDName);
		m_ReadData.OpenFileInt(pszValue);
		
		int nNum = m_ReadData.ReadData(m_szDefaultID);
		m_ReadData.ReadData(&m_DefaultValue[0]);

		for (int i = 0 ; i < nNum; i++)
    		SetDBValue(m_szDefaultID[i], m_DefaultValue[i],FALSE);
    }    	
	SetDBValue("SYSX_INJUNIT_INJUNIT_NULL_MAXPRESSURE",30);
	SetDBValue("SYSX_INJUNIT2_INJUNIT_NULL_MAXPRESSURE",30);
	SetDBValue("SYSX_INJUNIT_INJUNIT_NULL_MAXFLOW",30);

	if(m_bSVRT)
	{
		SetDBValue("MACH_MLD_CLOSE_RAMP4_PRESTIME", 0);
		SetDBValue("MACH_MLD_CLOSE_RAMP4_FLOWTIME", 0);
	}	
	
	if(GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED32").lValue == 1)/*wangli2015-5-13 11:22:15 �G����L*/
  {
 		SetDBValue("MACH_TMPOIL_COOLERON_NULL_DEGREEOFFSET", 0);	/*�o�ŧN�o���}*/
  }

	//SetMrtTempNUse();
	delete []pszIDName;
  delete []pszValue;
}	

void	CtmDefaultG::SetDefaultByF2()  //JOYCE2011-3-3 FOR LUB2
{
	DWORD		dwValueF2;
	dwValueF2 = GetDBValue("CONF_CODE_READ_NULL_XCODEDATA10").lValue;

	if (dwValueF2 & 0x1000)
		{
		SetDBValue("MACH_LUB2_ON_NULL_SHOTCOUNT", 3000);
		SetDBValue("MACH_LUB_ON_NULL_W019E", 12000);
		SetDBValue("MACH_SCRW_INJ_NULL_W0184", 3000);
		SetDBValue("MACH_SCRW_INJ_NULL_W0185", 8);
		}
}

void	CtmDefaultG::SetDefaultByF4()  //JOYCE2011-7-7
{
	DWORD		dwValueF4;
	dwValueF4 = GetDBValue("CONF_CODE_READ_NULL_XCODEDATA11").lValue;

	if (dwValueF4 & 0x0002)
		{
		SetDBValue("MACH_MLD_CLOSE_END_DELAYTIME", 10);
		SetDBValue("MACH_MLD_CLOSE_NULL_W0097", 5);
		}
}

BOOL	CtmDefaultG::GetFileName()
{
	m_DefaultIni.OpenFile("outerconfig.ini");
	
	m_DefaultIni.ReadString("default","DefaultID", 		NULL, &m_pszFileDefaultID);
	m_DefaultIni.ReadString("default","DefaultValue", NULL, &m_pszFileDefaultValue);	
	m_DefaultIni.ReadString("default","AttrID", 			NULL, &m_pszFileAttrID);
	m_DefaultIni.ReadString("default","AttrValue", 		NULL, &m_pszFileAttrValue);	
	m_DefaultIni.ReadString("default","DefaultSID", 	NULL, &m_pszFileDefaultSID);  //FOR STRING TYPE
	m_DefaultIni.ReadString("default","DefaultSValue",NULL, &m_pszFileDefaultSValue);	//FOR STRING TYPE
	m_DefaultIni.ReadString("default","VersionSID", 	NULL, &m_pszFileVersionSID);  //FOR STRING VERSION
	m_DefaultIni.ReadString("default","VersionSValue",NULL, &m_pszFileVersionSValue);	//FOR STRING VERSION
	
	m_DefaultIni.CloseFile();
	return TRUE;
}

void	CtmDefaultG::HEX2String(WORD wCode,char* psz)
{	
	for(int i=0; i<4; i++)
	{
		WORD	wTemp = (wCode>>(i*4))&0x000F;
		if(wTemp < 0x000A)
			psz[3-i] = wTemp + 0x30;
		else
			psz[3-i] = wTemp + 0x41;
	}	
}

void	CtmDefaultG::Init()
{
	if (m_pszFileDefaultID 		!= NULL) 	
		{
			m_ReadData.OpenFileStr(m_pszFileDefaultID);
			m_ReadData.ReadData(m_szDefaultID);
		}	
	if (m_pszFileDefaultValue != NULL)	
		{
			m_ReadData.OpenFileInt(m_pszFileDefaultValue);	
			m_ReadData.ReadData(&m_DefaultValue[0]);
		}	
	
	if (m_pszFileDefaultSID 		!= NULL) 	
		{
			m_ReadData.OpenFileStr(m_pszFileDefaultSID);//FOR STRING TYPE
			m_ReadData.ReadData(m_szDefaultSID);				//FOR STRING TYPE
		}	
	//printf("File: %s\n",m_pszFileDefaultSValue);
	if (m_pszFileDefaultSValue  != NULL)	
		{
			m_ReadData.OpenFileStr(m_pszFileDefaultSValue);	//FOR STRING TYPE		
			m_ReadData.ReadData(m_DefaultSValue);						//FOR STRING TYPE
		}	
	
	if (m_pszFileVersionSID 		!= NULL) 	
		{
			m_ReadData.OpenFileStr(m_pszFileVersionSID);	//FOR VERSION ID
			m_ReadData.ReadData(m_szVersionSID);					//FOR VERSION ID
		}	
	//printf("File: %s\n",m_pszFileDefaultSValue);
	if (m_pszFileVersionSValue  != NULL)	
		{
			m_ReadData.OpenFileStr(m_pszFileVersionSValue);	//FOR VERSION	VALUE	
			m_ReadData.ReadData(m_VersionSValue);						//FOR VERSION VALUE
		}	
}

int		CtmDefaultG::GetVerInfo()
{
	CtmFile File;
	char ver = '0';
	char szTemp3[256], szTemp4[256];
	sprintf(szTemp3, "%s", CtmConfig::GetInstance()->GetDBDefault());
	
	memset(szTemp4, 0, 512);
	memcpy(szTemp4, szTemp3, 512);
	char *p = strrchr(szTemp4, '/');
	if(p != NULL)
		strcpy(p+1, "ver");
	if(File.Open(szTemp4) == tmFILE_GOOD)
	{
		File.Seek(14, SEEK_SET);
		File.Read(&ver,1);
		File.Close();
	}
	if(ver == '3')
	{
		return DATABASE_VERSION_3;
	}
	else
	{
		return DATABASE_VERSION_2;
	}	
}

int		CtmDefaultG::LoadAttr(char*	cszDBID, int nPrecision)
{
	CtmFile File;
	DB_DATAATTRIBUTE*	m_pDataAttr;
	char 		cszTemp[256], sz[256];
	DWORD		m_dwLength = 0;
	int nFileLength = 0, nIndex = 0;
	
	sprintf(sz,"%s",CtmConfig::GetInstance()->GetConfigPath());
	if(strcmp("/usr/i86/config.ini",sz) == 0)
		sprintf(cszTemp, "/usr/i86/%s", CtmConfig::GetInstance()->GetDBAttr());
	else  if(strcmp("config.ini",sz) == 0)
		sprintf(cszTemp, "%s", CtmConfig::GetInstance()->GetDBAttr());

	if (File.Open(cszTemp) != tmFILE_GOOD) 
		return DB_ERR_DATAATTR;
		
	nFileLength = File.Seek(0, SEEK_END) - sizeof(DWORD);	
	//if ((m_dwVers == DATABASE_VERSION_2 && m_nIDCount != nFileLength/DATABASE2_ATTR_LEN) ||
	//	(m_dwVers == DATABASE_VERSION_3 && m_nIDCount != nFileLength/DATABASE3_ATTR_LEN))
	//	return DB_ERR_DATAATTR;
		
	m_pDataAttr = (DB_DATAATTRIBUTE *)malloc(sizeof(DB_DATAATTRIBUTE));
	
	if(m_pDataAttr == NULL)
		return DB_ERR_DB;
	
	nIndex = g_pDatabase->GetIndex(cszDBID);
	
	//printf("nIndex=%d m_dwVers=%x nPrecision=%d\n", nIndex, m_dwVers, nPrecision);
	//File.Seek(nIndex*DATABASE2_ATTR_LEN+sizeof(m_dwLength) + , SEEK_SET);
	if (m_dwVers == DATABASE_VERSION_3)
	{
		File.Seek(nIndex*DATABASE3_ATTR_LEN+sizeof(m_dwLength) + 22 , SEEK_SET);
	}
	else
	{
		File.Seek(nIndex*DATABASE2_ATTR_LEN+sizeof(m_dwLength) + 22, SEEK_SET);
	}
	File.Write(&nPrecision, sizeof(WORD));
	File.Close();
	free(m_pDataAttr);
	return	DB_SUCCESS;
}

#ifndef	D_BEOBJECT_CTMDEFAULTG

CObject*	CreateInstance(char* Name)
{
	if (g_pDefaultG == NULL)
	{
		CObject*	pResult = Create(Name);
		
		if (pResult != NULL)
		{
			g_nDefaultCount++;
			g_pDefaultG = static_cast<CtmDefaultG*>(pResult);			
			g_pDefaultG->CreateSelf();	
		}		
	}
	else g_nDefaultCount++;
		
	//printf("g_pSelevatG=%X\n", g_pDefaultG);
	
	return static_cast<CObject*>(g_pDefaultG);
}

void	ReleaseInstance(CObject* pInstance)
{
	if (g_nDefaultCount > 0)
		g_nDefaultCount--;
	
	if (g_pDefaultG <= 0)
	{
		if (g_pDefaultG != NULL)
			g_pDefaultG->FreeSelf();
		delete g_pDefaultG;
		g_pDefaultG = NULL;
	}
}
#endif

void  CtmDefaultG::SetDefaultInit()
{
	for (int i = 0; i < CNT_INT_MAX_DEFAULT_NUM; i++)
    {
    	if (m_szDefaultID[i][0] != '\0')
        {
        	if (g_pDatabase != NULL)
            	{
            			if(0 == strcmp(m_szDefaultID[i], "double"))
            			{
            					double dbValue = 0;
            					dbValue = m_DefaultValue[i] >> 32;
            					dbValue += m_DefaultValue[i+1];
            					g_pDatabase->WriteDefault(m_szDefaultID[++i], &dbValue, TRUE);
            			}
            			else
            			{
                		g_pDatabase->WriteDefault(m_szDefaultID[i], &m_DefaultValue[i], TRUE);
                	}
               }
        }
        else break;
		} 
	//FOR STRING TYPE
	for (int i = 0; i < 16; i++)
    {
    	if (m_szDefaultSID[i][0] != '\0')
        {
        	if (g_pDatabase != NULL)
            	{
                	//printf("ID: %s\n",m_szDefaultSID[i]);
                	//printf("VL: %s\n",m_DefaultSValue[i]);
                	g_pDatabase->WriteDefaults(m_szDefaultSID[i], m_DefaultSValue[i], strlen(m_DefaultSValue[i]),TRUE);
                }
        }
        else break;
		}       
	
	m_dwVers = GetVerInfo();		
	SetDefaultByFunc();	
//	SetDefaultUser();//JOYCE2012-2-1 for inet
	//SetAttrInit();
}

void  CtmDefaultG::SetVersionInf()
{
	//FOR mmi
	for (int i = 0; i < 16; i++)
    {
    	if (m_szVersionSID[i][0] != '\0')
        {
        	if (g_pDatabase != NULL)
            	{
                	//printf("ID: %s\n",m_szDefaultSID[i]);
                	//printf("VL: %s\n",m_DefaultSValue[i]);
                	g_pDatabase->Write(m_szVersionSID[i], m_VersionSValue[i], TRUE);
              }
        }
        else break;
		}  	
	//for formlib && res	
			char*	pszVersion=NULL;
			int 	nTemp = 0;
			
			g_StateIni.OpenFile("stateconfig.ini");
			g_StateIni.ReadString("version","version", NULL,&pszVersion);
			//printf("pszVersion :%s\n",pszVersion);
			g_pDatabase->Write("VERS_HMI_NULL_NULL_PROGRAMFORMLIBVERS", pszVersion,TRUE);
			g_StateIni.ReadString("version","date", NULL,&pszVersion);
			//printf("pszVersion :%s\n",pszVersion);
			g_pDatabase->Write("VERS_HMI_NULL_NULL_PROGRAMFORMLIBDATE", pszVersion,TRUE);
			g_StateIni.ReadString("version","function", NULL,&pszVersion);
			//printf("pszVersion :%s\n",pszVersion);
			int	iLanType	= CtmConfig::GetInstance()->GetLanguagePatch(); /*wangli2016-3-31 15:45:55 ��FLAG�P�ڶq�r��Ĭ�A���flag��LanguagePatch*/
			//printf("iLanType =%d\n",iLanType);
			
			if(iLanType)																pszVersion[0] = '1'+iLanType; //�h��y������

			/*-----------------------pszVersion[1]�\�����*/
			//char ASCII to int, fuxy, 2012-1-17 
			if((pszVersion[1] >= 48) && (pszVersion[1]) <= 57)
				nTemp = pszVersion[1] -48;					//ASCII to Num 0x0-0x9 
			else  if((pszVersion[1] >= 65) && (pszVersion[1]) <= 70)
				nTemp = pszVersion[1] -(65 -0xA);			//ASCII to Num 0xA-0xF 
				
			if(m_biNet == 1)	 
				nTemp |=1;  	//pszVersion[1] = '1'; 		//iNet�\����� //JOYCE2009-12-31 
			if(m_bAmmeter)	//�q�O�p�\��  
				nTemp |=2;
			if(CtmConfig::GetInstance()->GetRemoteFlag())  
				nTemp |=4;  	//pszVersion[1] = '4';		//3G �\����� //JOYCE2011-6-2
			if(m_bOLDLUB && (!m_bStepLub))
				nTemp |=8;		//fuxy, 2012-1-17, for Old lub 
			
			//int to char ASCII, fuxy, 2012-1-17 
			if((nTemp >= 0)&&(nTemp <= 9))
				pszVersion[1] = nTemp +48;					//Num 0x0-0x9 to ASCII 
			else if((nTemp >= 0xA)&&(nTemp <= 0xF))
				pszVersion[1] = nTemp +(65 -0xA);			//Num 0xA-0xF to ASCII 
			//printf(" pszVersion :%s\n",pszVersion);
			/*-----------------------pszVersion[1]�\�����*/     
     
			g_pDatabase->Write("VERS_HMI_NULL_NULL_PROGRAMRESVERS", pszVersion,TRUE);
			g_StateIni.CloseFile();			
}

void	CtmDefaultG::SetNodeID()
{
  WORD wTemp = 0;
	
	if(m_bDC || m_bT11Port19 || m_bMold36)
	{
		if(GetDBValue("M3_CONF_TEMPERCARD2_NODE_ID").lValue != 4)
			{
				wTemp = 4;
				g_pDatabase->WriteDefault("M3_CONF_TEMPERCARD2_NODE_ID", (WORD*)&wTemp, TRUE);/*�������2���ūץd*/
			}
	}
	if(m_bMold36)
	{
		if(GetDBValue("M3_CONF_TEMPERCARD3_NODE_ID").lValue != 5)
			{
				wTemp = 5;
				g_pDatabase->WriteDefault("M3_CONF_TEMPERCARD3_NODE_ID", (WORD*)&wTemp, TRUE);/*�������3���ūץd*/
			}
		if(GetDBValue("M3_CONF_TEMPERCARD4_NODE_ID").lValue != 6)
			{
				wTemp = 6;
				g_pDatabase->WriteDefault("M3_CONF_TEMPERCARD4_NODE_ID", (WORD*)&wTemp, TRUE);/*�������4���ūץd*/
			}
		if(GetDBValue("M3_CONF_TEMPERCARD5_NODE_ID").lValue != 7)
			{
				wTemp = 7;
				g_pDatabase->WriteDefault("M3_CONF_TEMPERCARD5_NODE_ID", (WORD*)&wTemp, TRUE);/*�������5���ūץd*/
			}
	}
	
}




void	CtmDefaultG::SetDefaultByFunc()
{
	char*		pszPath	= NULL;
	char		pszIDName[64];
	char		pszValue[64];
	char		pszDef[64];
	//char*		m_szFuncID[256] = {NULL};
	//int			m_FuncValue[256]= {0};
	
	m_DefaultIni.OpenFile("outerconfig.ini");
	m_DefaultIni.ReadString("default_function","Path", 		NULL, &pszPath);
	m_DefaultIni.CloseFile();
	
	for(int i=0;i<256;i++)
	{		
		memset(pszDef,0,sizeof(pszDef));
		if(g_pExTable->GetTableValue(6,&i,pszDef))
			{
				sprintf(pszIDName,		"%s%s_ID",		pszPath,pszDef);
				sprintf(pszValue,			"%s%s_Value",	pszPath,pszDef);
				//printf("pszIDName :%s\npszValue :%s\n",pszIDName,pszValue);
				m_ReadData.OpenFileStr(pszIDName);
				m_ReadData.OpenFileInt(pszValue);
				int nNum = m_ReadData.ReadData(m_szDefaultID);
				m_ReadData.ReadData(&m_DefaultValue[0]);				
				//printf("nNum: %d\n",nNum);
				for (int j = 0 ; j < nNum; j++)
				{
					if(g_pDatabase->GetDataID(m_szDefaultID[j])!=-1) /*2012-1-17  �D�ŧP�_*/ 
						SetDBValue(m_szDefaultID[j], m_DefaultValue[j],FALSE);
				}

				if ((!(strcmp(pszDef,"D_INJ2PREC"))) || (!(strcmp(pszDef,"D_FLOW1"))))	//Precision; 	
         {
						sprintf(pszIDName,	"%s%s_ID_Attr",		pszPath, pszDef);
						sprintf(pszValue,	"%s%s_Value_Attr",	pszPath, pszDef);
						m_ReadData.OpenFileStr(pszIDName);
						m_ReadData.OpenFileInt(pszValue);
						nNum = m_ReadData.ReadData(m_szDefaultID);
						m_ReadData.ReadData(&m_DefaultValue[0]);
						for (int i = 0 ; i < nNum; i++)
						{
							//�g���󤤥h
							LoadAttr(m_szDefaultID[i], m_DefaultValue[i]);
							//�g�줺�s���h
							int nIndex = g_pDatabase->GetIndex(m_szDefaultID[i]);
							DB_DATAATTRIBUTE* pTemp = g_pDatabase->GetAttr();
							if(nIndex) /*2013-9-13 �D�ŧP�_*/
								pTemp[nIndex].wPrecision = m_DefaultValue[i];	// 1, 2, 3
						} 	
				}
				if(!(strcmp(pszDef,"D_INJSPDPREC")))	//Precision; 
               {
						sprintf(pszIDName,	"%s%s_ID_Attr",		pszPath, pszDef);
						sprintf(pszValue,	"%s%s_Value_Attr",	pszPath, pszDef);
						m_ReadData.OpenFileStr(pszIDName);
						m_ReadData.OpenFileInt(pszValue);
						nNum = m_ReadData.ReadData(m_szDefaultID);
						m_ReadData.ReadData(&m_DefaultValue[0]);
						for (int i = 0 ; i < nNum; i++)
						{
							//�g���󤤥h
							LoadAttr(m_szDefaultID[i], m_DefaultValue[i]);
							//�g�줺�s���h
							int nIndex = g_pDatabase->GetIndex(m_szDefaultID[i]);
							DB_DATAATTRIBUTE* pTemp = g_pDatabase->GetAttr();
							pTemp[nIndex].wPrecision = m_DefaultValue[i];	// 1, 2, 3
						} 	
				}
			}
		else	break;
	}	
}

void	CtmDefaultG::SetDefaultUser() //JOYCE2012-2-1  for inet
{
	if(m_biNet == 1)
	{
		for(int i=0; i<4; i++)
		{
			g_pDatabase->WriteDefaults(pszUserID[i], pszUserValue[i], 6 /*strlen(pszUserValue[i])*/,TRUE);
			g_pDatabase->WriteDefault(pszPasswdID[i], &pszPasswdValue[i], TRUE);
			g_pDatabase->WriteDefault(pszPrivilegeID[i], &pszPrivilegeValue[i], TRUE);
		}
	}
}

void	CtmDefaultG::SetAttrInit()
{
	int nNum = 0;
	if (m_pszFileAttrID 		!= NULL) 	
		{
			m_ReadData.OpenFileStr(m_pszFileAttrID);
			nNum = m_ReadData.ReadData(m_szDefaultID);
		}	
	if (m_pszFileAttrValue != NULL)	
		{
			m_ReadData.OpenFileInt(m_pszFileAttrValue);	
			m_ReadData.ReadData(&m_DefaultValue[0]);
		}	

	for (int i = 0 ; i < nNum; i++)
		{
			//�g���󤤥h
			LoadAttr(m_szDefaultID[i], m_DefaultValue[i]);
			//�g�줺�s���h
			int nIndex = g_pDatabase->GetIndex(m_szDefaultID[i]);
			DB_DATAATTRIBUTE* pTemp = g_pDatabase->GetAttr();
			pTemp[nIndex].wPrecision = m_DefaultValue[i];// 1, 2, 3
    } 	
}

void	CtmDefaultG::ReadEnergyDataFrom()//JOYCE2011-5-13 
{
	DWORD dwEnergyData[22];
 	char	szPath[256];
	char	m_szCopyFile[256];
    char	pszDB[256];

	memset(dwEnergyData, 0, sizeof(dwEnergyData));
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/energy");
	sprintf(m_szCopyFile, "%s/energy.cdb", szPath);

	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	//printf("nFd:%d\n",nFd);
    if (nFd >= 0)
	{
		read(nFd, &dwEnergyData, sizeof(dwEnergyData));
	}
 	close(nFd);

	for (int i = 0; i < 22; i++)
	{
    	//printf("dwEnergyData[%d]:%d\n",i,dwEnergyData[i]);
    	if(i >= 17)	
    		sprintf(pszDB, "SYSX_OTHERS_OTHERS_DWORD_RESERVED%d", i+14);
    	else if((i==12)||(i == 13))	//fuxy, 2011-9-19  �MAustone��ƭ��_  
    		sprintf(pszDB, "SYS_HMI_NULL_NULL_RESERVED%d", i+23);
    	else
    		sprintf(pszDB, "SYS_HMI_NULL_NULL_RESERVED%d", i+18);
    	SetDBValue(pszDB, dwEnergyData[i],FALSE);
    }
}

void	CtmDefaultG::ReadNotclmptime()/*wangli2015-12-10 16:56:21 �D�}���Ҫ��A�ɶ�*/
{
	DWORD dwNotclmptime[2];
 	char	szPath[256];
	char	m_szCopyFile[256];
    char	pszDB[256];

	memset(dwNotclmptime, 0, sizeof(dwNotclmptime));
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/notclmptime.cdb", szPath);

	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	//printf("nFd:%d\n",nFd);
    if (nFd >= 0)
	{
		read(nFd, &dwNotclmptime, sizeof(dwNotclmptime));
	}
 	close(nFd);

	for (int i = 0; i < 2; i++)
	{
    	//printf("defaultg  dwNotclmptime[%d]:%d\n",i,dwNotclmptime[i]);
	    sprintf(pszDB, "SYSX_OTHERS_OTHERS_DWORD_RESERVED%d", i+56);
	    	
    	SetDBValue(pszDB, dwNotclmptime[i],FALSE);
    }
    
}

void	CtmDefaultG::ReadShotCount()//JOYCE2010-10-12 �ֿn�B��Ҽ�
{
		WORD 	wShotCount[2];
 	  char	szPath[256];
	  char	m_szCopyFile[256];
    char	pszDB[256];

	       memset(wShotCount, 0, sizeof(wShotCount));
	       memset(pszDB, 0, sizeof(szPath));
	       memset(szPath, 0, sizeof(szPath));
	       memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	       sprintf(szPath, "%s", "/conf/shotcount");
	       sprintf(m_szCopyFile, "%s/shotcount.cdb", szPath);
	       int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
      
	       //printf("nFd:%d\n",nFd);

	       if (nFd >= 0)
					{
						 read(nFd, &wShotCount, sizeof(wShotCount));
					}
 		    close(nFd);

		for (int i = 0; i < 2; i++)
		{
    //printf("wShotCount[%d]:%d\n",i,wShotCount[i]);
    
    sprintf(pszDB, "SYS_HMI_NULL_NULL_RESERVED%d", i+35);
    SetDBValue(pszDB, wShotCount[i],FALSE);
    }
}

void	CtmDefaultG::ReadAutoRunTime()//wangli2014-6-5 11:35:00 Auto Run Time
{
	DWORD 	dwAutoRunTime[1];
 	char	szPath[256];
	char	m_szCopyFile[256];
    char	pszDB[256];

	memset(dwAutoRunTime, 0, sizeof(dwAutoRunTime));
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/autoruntime");
	sprintf(m_szCopyFile, "%s/autoruntime.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
    //printf("nFd:%d\n",nFd);

	if (nFd >= 0)
	{
		read(nFd, &dwAutoRunTime, sizeof(dwAutoRunTime));
	}
 	close(nFd);

	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED10", dwAutoRunTime[0],FALSE);
}

void	CtmDefaultG::ReadMoldWeight() //JOYCE2011-6-7 
{
	  WORD wMoldWeight[4];
 	  char	szPath[256];
	  char	m_szCopyFile[256];
    char	pszDB[256];

	       memset(wMoldWeight, 0, sizeof(wMoldWeight));
	       memset(pszDB, 0, sizeof(szPath));
	       memset(szPath, 0, sizeof(szPath));
	       memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	       sprintf(szPath, "%s", "/conf/moldweight");
	       sprintf(m_szCopyFile, "%s/moldweight.cdb", szPath);
	       int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
      
	       //printf("nFd:%d\n",nFd);

	       if (nFd >= 0)
					{
						 read(nFd, &wMoldWeight, sizeof(wMoldWeight));
					}
 		    close(nFd);

		for (int i = 0; i < 4; i++)
		{
    //printf("wMoldWeight[%d]:%d\n",i,wMoldWeight[i]);
    
    sprintf(pszDB, "SYSX_OTHERS_OTHERS_INT_RESERVED%d", i+1);
    SetDBValue(pszDB, wMoldWeight[i],FALSE);
    }
}

void	CtmDefaultG::ReadSDOOR()//JOYCE2011-11-2 SAFEDOOR No.11-045
{
		WORD 	wSDOOR[4];
 	  char	szPath[256];
	  char	m_szCopyFile[256];
    char	pszDB[256];

	       memset(wSDOOR, 0, sizeof(wSDOOR));
	       memset(pszDB, 0, sizeof(szPath));
	       memset(szPath, 0, sizeof(szPath));
	       memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	       sprintf(szPath, "%s", "/conf/safedoor");
	       sprintf(m_szCopyFile, "%s/safedoor.cdb", szPath);
	       int nFd =  open(m_szCopyFile, O_RDWR);
      
	      // printf("nFd:%d\n",nFd);

	       if (nFd >= 0)
					{
						 read(nFd, &wSDOOR, sizeof(wSDOOR));
					}
			else
				{
					wSDOOR[3] = 5000;
					
				}
 		    close(nFd);

		for (int i = 0; i < 4; i++)
		{
			if( i < 3 )
			{
		    	sprintf(pszDB, "SYSX_OTHERS_OTHERS_INT_RESERVED%d", i+80);
			}
			else if(i == 3)
		    	sprintf(pszDB, "SYSX_OTHERS_OTHERS_INT_RESERVED%d", i+115);
				
		    SetDBValue(pszDB, wSDOOR[i],FALSE);
				
    	}
}

void	CtmDefaultG::ReadLubricate1()//JOYCE2011-8-3 lubricate
{
	  BOOL	bFileExit = FALSE;
	  DWORD dwLubrData1[55];
	  DWORD dwLubrData2[3];
	  char	szPath[256];
	  char	m_szCopyFile[256];
	  char	pszDB[256];
		char*   pDataID[] = {
            "MACH_LUB_ON_NULL_BYPASSOPTION",
						"SYSX_OTHERS_OTHERS_INT_RESERVED19",
						"SYSX_OTHERS_OTHERS_INT_RESERVED20",
						"SYSX_OTHERS_OTHERS_INT_RESERVED21",
						"SYSX_OTHERS_OTHERS_INT_RESERVED22",
						"SYSX_OTHERS_OTHERS_INT_RESERVED23",
						"SYSX_OTHERS_OTHERS_INT_RESERVED24",
						"SYSX_OTHERS_OTHERS_INT_RESERVED25",
						"SYSX_OTHERS_OTHERS_INT_RESERVED26",
						"SYSX_OTHERS_OTHERS_INT_RESERVED27",
						"SYSX_OTHERS_OTHERS_INT_RESERVED28",
						"SYSX_OTHERS_OTHERS_INT_RESERVED29",
						"SYSX_OTHERS_OTHERS_INT_RESERVED30",
						"SYSX_OTHERS_OTHERS_INT_RESERVED31",
						"SYSX_OTHERS_OTHERS_INT_RESERVED32",
						"SYSX_OTHERS_OTHERS_INT_RESERVED33",
						"SYSX_OTHERS_OTHERS_INT_RESERVED34",
						"SYSX_OTHERS_OTHERS_INT_RESERVED35",
						"SYSX_OTHERS_OTHERS_INT_RESERVED36",
						"SYSX_OTHERS_OTHERS_INT_RESERVED37",
						"SYSX_OTHERS_OTHERS_INT_RESERVED38",
						"SYSX_OTHERS_OTHERS_INT_RESERVED39",
						"SYSX_OTHERS_OTHERS_INT_RESERVED40",
						"SYSX_OTHERS_OTHERS_INT_RESERVED41",
						"SYSX_OTHERS_OTHERS_INT_RESERVED42",
						"SYSX_OTHERS_OTHERS_INT_RESERVED43",
						"SYSX_OTHERS_OTHERS_INT_RESERVED44",
						"SYSX_OTHERS_OTHERS_INT_RESERVED45",
						"SYSX_OTHERS_OTHERS_INT_RESERVED46",
						"SYSX_OTHERS_OTHERS_INT_RESERVED47",
						"SYSX_OTHERS_OTHERS_INT_RESERVED48",
						"SYSX_OTHERS_OTHERS_INT_RESERVED49",
						"SYSX_OTHERS_OTHERS_INT_RESERVED50",
						"SYSX_OTHERS_OTHERS_INT_RESERVED51",
						"SYSX_OTHERS_OTHERS_INT_RESERVED52",
						"SYSX_OTHERS_OTHERS_INT_RESERVED53",
						"SYSX_OTHERS_OTHERS_INT_RESERVED54",
						"SYSX_OTHERS_OTHERS_INT_RESERVED55",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED20",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED21",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED22",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED23",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED24",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED25",					
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED26",					
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED27",
						"SYSX_OTHERS_OTHERS_INT_RESERVED56",      /*JOYCE2012-6-25 add 6�� ���Ʀ��ƤW�U�����w*/
						"SYSX_OTHERS_OTHERS_INT_RESERVED57",
						"SYSX_OTHERS_OTHERS_INT_RESERVED58",
						"SYSX_OTHERS_OTHERS_INT_RESERVED59",
						"SYSX_OTHERS_OTHERS_INT_RESERVED60",
						"SYSX_OTHERS_OTHERS_INT_RESERVED61",
						"SYSX_OTHERS_OTHERS_INT_RESERVED62",     /*JOYCE2012-9-14 �t�m�����W�[���ѽX*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED50",   /*JOYCE20130809 ��1�ռ��Ƥ����Ҽ�*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED51",   /*JOYCE20130809 ��2�ռ��Ƥ����Ҽ�*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED52",   /*JOYCE20130809 ��3�ռ��Ƥ����Ҽ�*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED53",/*wangli2016-1-5 16:59:51  �}�����Ʈɼ� lub1*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED54",/*wangli2016-1-5 16:59:51  �}�����Ʈɼ� lub2*/
            NULL
            };

    memset(dwLubrData1, 0, sizeof(dwLubrData1));
    memset(dwLubrData2, 0, sizeof(dwLubrData2));
    memset(pszDB, 0, sizeof(szPath));
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf");
    sprintf(m_szCopyFile, "%s/lubricate1.cdb", szPath);
    int nFd =  open(m_szCopyFile, O_RDWR);      
    //printf("nFd:%d\n",nFd);
     
		if (nFd == -1) /*file not exit;set default*/
		{
			bFileExit = FALSE;
		}	
		else	if (nFd >= 0)
		{
			bFileExit = TRUE;
			read(nFd, &dwLubrData1, sizeof(dwLubrData1));
		}
    close(nFd);

     sprintf(m_szCopyFile, "%s/lubricate2.cdb", szPath);
     nFd =  open(m_szCopyFile, O_RDWR);      
     if (nFd >= 0)
		 {
			 read(nFd, &dwLubrData2, sizeof(dwLubrData2));
		 }
    close(nFd);

	  if(bFileExit)
	  { 
			for (int i = 0; i < 36; i++)
			{ 
	    sprintf(pszDB, "SYSX_OTHERS_OTHERS_INT_RESERVED%d", i+20);
	    SetDBValue(pszDB, (WORD)dwLubrData1[i],FALSE);
	    }
	
			for (int i = 36; i < 43; i++) //�۰ʼ��ƫO���p��
			{
	    sprintf(pszDB, "SYSX_OTHERS_OTHERS_DWORD_RESERVED%d", i-16);
	    SetDBValue(pszDB, dwLubrData1[i],FALSE);
	    }
			
			for (int i = 43; i < 50; i++) /*JOYCE2012-6-25 add 6�� ���Ʀ��ƤW�U�����w; //JOYCE2012-9-14  �t�m�����W�[���ѽX*/
			{
   			sprintf(pszDB, "SYSX_OTHERS_OTHERS_INT_RESERVED%d", i+13);
   			SetDBValue(pszDB, (WORD)dwLubrData1[i],FALSE);
			}
			
			for(int i = 50; i < 55; i++)/*JOYCE2013-8-9 ���Ƥ����Ҽ�*/	
			{
   			sprintf(pszDB, "SYSX_OTHERS_OTHERS_DWORD_RESERVED%d", i);
   			SetDBValue(pszDB, (WORD)dwLubrData1[i],FALSE);
			}
			/*---------------------------------------*/
			
	    SetDBValue("MACH_LUB_ON_NULL_BYPASSOPTION", (WORD)dwLubrData2[0],FALSE); //�}����e��Ҥ������ưʧ@
	  }
	  else /*file not exit;set default*/
		{
			for(int i=0; i<58 ;i++)
			{
				if(pDataID[i] != NULL)
					{
					SetDBValue(pDataID[i], g_pDatabase->ReadDefault(pDataID[i]).dwData, FALSE);	//read��l��	
					}
			}

			if(m_bDC)/*JOYCE2013-9-9 ������{��,�q�{��LUB2����*/
			{
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED24", 8, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED28", 8, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED27", 9000, FALSE);
			}	
			else	if(m_bSVRT)/*16-1-14 11:21:47 �¤G�O �q�{�� LUB13����*/
			{
				//LUB1
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED20", 100, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED20", 800, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED23", 3000, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED24", 2, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED25", 1000, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED29", 2000, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED30", 250, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED21", 800, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED27", 3000, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED28", 2, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED56", 2, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED57", 1, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED53", 120, FALSE);
				
				//LUB2
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED31", 10000, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED34", 0, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED35", 1, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED36", 20000, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40", 30000, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", 1000, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED38", 0, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39", 1, FALSE);
				SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED54", 360, FALSE);
				
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED58", 1, FALSE);
			}	
		}

	/*OLD LUB FILE��,���s�b ���Ʀ��ƤW�U�����w,�ݭݮe�B�z-------20130927add*/
	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED56").lValue == 0) 
		{
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED56", 16, FALSE);
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED57",  4, FALSE);
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED58",  2, FALSE);
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED59",  1, FALSE);
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED60", 10, FALSE);
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED61",  1, FALSE);
		}

	/*OLD LUB FILE��,���s�b ���Ƥ����Ҽ�,�ݭݮe�B�z-------20130927add*/
	if(GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue == 0) 
		{
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50", 10000, FALSE);
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED51", 10000, FALSE);
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED52", 10000, FALSE);
		}
	  	
}

void	CtmDefaultG::ReadLubricate2()//JOYCE2011-8-3 lubricate
{
	  DWORD dwLubrData2[3];
 	  char	szPath[256];
	  char	m_szCopyFile[256];
    char	pszDB[256];

	       memset(dwLubrData2, 0, sizeof(dwLubrData2));
	       memset(pszDB, 0, sizeof(szPath));
	       memset(szPath, 0, sizeof(szPath));
	       memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	       sprintf(szPath, "%s", "/conf");
	       sprintf(m_szCopyFile, "%s/lubricate2.cdb", szPath);
	       int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);      
	       if (nFd >= 0)
					{
						 read(nFd, &dwLubrData2, sizeof(dwLubrData2));
					}
 		    close(nFd);

	    SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19", (WORD)dwLubrData2[1],FALSE);//�֭p�Ҽ�flag
	    SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27", (WORD)dwLubrData2[2],FALSE);//�ظm�e���}�񪺦Z���J���ƼҼ�COUNT
	    
}

void	CtmDefaultG::ReadLubricate3()/*wangli2015-11-13 15:56:24*/
{
	  DWORD dwLubrData3[1];
 	  char	szPath[256];
	  char	m_szCopyFile[256];
    char	pszDB[256];

	       memset(dwLubrData3, 0, sizeof(dwLubrData3));
	       memset(pszDB, 0, sizeof(szPath));
	       memset(szPath, 0, sizeof(szPath));
	       memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	       sprintf(szPath, "%s", "/conf");
	       sprintf(m_szCopyFile, "%s/lubricate3.cdb", szPath);
	       int nFd =  open(m_szCopyFile, O_RDWR );      
	       if (nFd >= 0)
				{
					read(nFd, &dwLubrData3, sizeof(dwLubrData3));
				    SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED8", (WORD)dwLubrData3[0],FALSE);/*wangli2015-11-13 14:24:02 �}���۰ʼ��ƤѼ�*/
				}
 		    close(nFd);
	    	
}

void	CtmDefaultG::SetLubricate()  //JOYCE2011-8-3 lubricate,After read �}���P�B
{
	long	lShotCntTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27").lValue;/*��ڹB��Ҽ�*/
	DWORD	dwLubrData = 0;
	long  lTemp = 0;

/*JOYCE2013-9-9 mark*/
//	char*   pDataID[] = {
//            "MACH_LUB_ON_NULL_BYPASSOPTION",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED19",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED20",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED21",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED22",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED23",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED24",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED25",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED26",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED27",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED28",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED29",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED30",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED31",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED32",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED33",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED34",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED35",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED36",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED37",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED38",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED39",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED40",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED41",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED42",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED43",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED44",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED45",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED46",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED47",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED48",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED49",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED50",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED51",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED52",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED53",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED54",
//						"SYSX_OTHERS_OTHERS_INT_RESERVED55",
//						"SYSX_OTHERS_OTHERS_DWORD_RESERVED20",
//						"SYSX_OTHERS_OTHERS_DWORD_RESERVED21",
//						"SYSX_OTHERS_OTHERS_DWORD_RESERVED22",
//						"SYSX_OTHERS_OTHERS_DWORD_RESERVED23",
//						"SYSX_OTHERS_OTHERS_DWORD_RESERVED24",
//						"SYSX_OTHERS_OTHERS_DWORD_RESERVED25",					
//						"SYSX_OTHERS_OTHERS_DWORD_RESERVED26",					
//						"SYSX_OTHERS_OTHERS_DWORD_RESERVED27",
//            NULL
//            };
//
//  if((GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED20").lValue == 0)&&(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED25").lValue == 0)
//  	 &&(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED31").lValue == 0)&&(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED36").lValue == 0))//not reset, FOR defalut,�P�_ �۰ʼ��ƼҼ�>0
//	{
//		for(int i=0; i<46 ;i++)
//		{
//			if(pDataID[i] != NULL)
//				{
//				SetDBValue(pDataID[i], g_pDatabase->ReadDefault(pDataID[i]).dwData, FALSE);	//read��l��	
//				}
//		}
//	}
//
//	/*OLD LUB FILE,���s�b ���Ʀ��ƤW�U�����w,�ݭݮe�B�z-------*/
//	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED56").lValue == 0) 
//		{
//			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED56", 18, FALSE);
//			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED57", 1, FALSE);
//			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED58", 12, FALSE);
//			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED59", 1, FALSE);
//			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED60", 10, FALSE);
//			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED61", 1, FALSE);
//		}
//
//	/*OLD LUB FILE,���s�b ���Ƥ����Ҽ�,�ݭݮe�B�z-------*/
//	if(GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue == 0) 
//		{
//			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50", 10000, FALSE);
//			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED51", 10000, FALSE);
//			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED52", 10000, FALSE);
//		}
	
	long  lChangCntLub1= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue;/*��1�ռ��Ƥ����Ҽ�*/
	long  lChangCntLub2= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED51").lValue;/*��2�ռ��Ƥ����Ҽ�*/
	long  lChangCntLub3= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED52").lValue;/*��3�ռ��Ƥ����Ҽ�*/

	if(lShotCntTemp > lChangCntLub1)  //LUB1
	{
		for(int i=0; i<5 ;i++)
			{
			if(pHMILub1A[i] != NULL)	dwLubrData = GetDBValue(pHMILub1A[i]).lValue;
   		if(pDSPLub1[i] != NULL)
   				{
   					SetDBValue(pDSPLub1[i], dwLubrData);
   					
	 	   			if(i == 0) /*wangli2016-11-14 16:05:33 lub1�۰ʼ��ƼҼ� * ���O��[���ưѼƫY�� = ���O��[���ƼҼ�*/
	 	   			{
					lTemp = dwLubrData * GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED31").lValue /100 ; 
					SetDBValue("MACH_OTHR_OTHR_NULL_W01AF",lTemp);
					}
  				}
			}
	}
	else
	{
		for(int i=0; i<5 ;i++)
			{
			if(pHMILub1B[i] != NULL)	dwLubrData = GetDBValue(pHMILub1B[i]).lValue;
   		if(pDSPLub1[i] != NULL)	
   				{
   				
	   				SetDBValue(pDSPLub1[i], dwLubrData);

	 	   			if(i == 0) /*wangli2016-11-14 16:05:33 lub1�۰ʼ��ƼҼ� * ���O��[���ưѼƫY�� = ���O��[���ƼҼ�*/
	 	   			{
					lTemp = dwLubrData * GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED31").lValue /100 ; 
					SetDBValue("MACH_OTHR_OTHR_NULL_W01AF",lTemp);
					}
   				}
			}
	}

	if(lShotCntTemp > lChangCntLub2)  //LUB2
	{
		for(int i=0; i<5 ;i++)
			{
			if(pHMILub2A[i] != NULL)	dwLubrData = GetDBValue(pHMILub2A[i]).lValue;
   		if(pDSPLub2[i] != NULL)	SetDBValue(pDSPLub2[i], dwLubrData);
			}
	}
	else
	{
		for(int i=0; i<5 ;i++)
			{
			if(pHMILub2B[i] != NULL)	dwLubrData = GetDBValue(pHMILub2B[i]).lValue;
   		if(pDSPLub2[i] != NULL)	SetDBValue(pDSPLub2[i], dwLubrData);
			}
	}

	if(m_b3LUB)
	{
		if(lShotCntTemp > lChangCntLub3)  //LUB3
		{
			for(int i=0; i<5 ;i++)
				{
					if(pHMILub3A[i] != NULL)	dwLubrData = GetDBValue(pHMILub3A[i]).lValue;
	   			
	   			if(i == 4)
	   				{
						SetDBValue("MACH_MTR_ON_NULL_W01DD",dwLubrData>>16);  //�۰ʼ��ƫO���p��
						SetDBValue("MACH_MTR_ON_NULL_W01DC",dwLubrData&0xFFFF);
	   				}
	   			else
	   				{
		   			if(pDSPLub3[i] != NULL)	SetDBValue(pDSPLub3[i], dwLubrData);
	   				}	
				}
		}
		else
		{
			for(int i=0; i<5 ;i++)
				{
					if(pHMILub3B[i] != NULL)	dwLubrData = GetDBValue(pHMILub3B[i]).lValue;
	   			
	   			if(i == 4)
	   				{
						SetDBValue("MACH_MTR_ON_NULL_W01DD",dwLubrData>>16);  //�۰ʼ��ƫO���p��
						SetDBValue("MACH_MTR_ON_NULL_W01DC",dwLubrData&0xFFFF);
	   				}
	   			else
	   				{
		   			if(pDSPLub3[i] != NULL)	SetDBValue(pDSPLub3[i], dwLubrData);
	   				}	
				}
		}
	}

	if(m_bALUB)
	{
		lTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED53").lValue;
		SetDBValue("MACH_MDH_MDH_NULL_W0027",lTemp);  //�ռҼ��ƶ��j�Ѽ�

 		//JOYCE2012-4-26 lTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54").lValue;
 		//JOYCE2012-4-26 SetDBValue("MACH_MDH_MDH_NULL_W0028",lTemp);  //�ռҼ��Ʋ֭p�Ѽ�
    lTemp = GetDBValue("MACH_MDH_MDH_NULL_W0028").lValue;
    if(lTemp == 0xFFFF)	lTemp = 0;
    SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54",lTemp);  /*�ռҼ��Ʋ֭p�Ѽ� �i�Q���m��*/

		lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED26").lValue;
		SetDBValue("MACH_LUB_ON_START_ADJUSTDELAYTIME",lTemp);  //�ռҼ���ĵ���ɶ�

		lTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED55").lValue;
		SetDBValue("MACH_MDH_MDH_NULL_W0026",lTemp);  //�ռҼ��ƫO���ɶ�
	}
}

void	CtmDefaultG::ReadLubType() 		//fuxy, 2012-1-13 
{
	WORD 	wLubType[2];
 	char	szPath[16];
	char	m_szCopyFile[64];

	memset(wLubType, 0, sizeof(wLubType));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/lubtype.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);

	if (nFd >= 0)
	{
		read(nFd, &wLubType, sizeof(wLubType));
	}
 	close(nFd);
	
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70", wLubType[0], FALSE);
}

void	CtmDefaultG::ReadOldLub() 		//fuxy, 2012-1-16  ��s�{�ǥB�����m�ɨ��^��ƭ� 
{
	DWORD 	dwOldLubData[23];
 	char	szPath[16];
	char	m_szCopyFile[64];
	int 	nFd = -1, nTemp = -1;
	
	memset(dwOldLubData, 0, sizeof(dwOldLubData));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/oldlub.cdb", szPath);
	nTemp = nFd =  open(m_szCopyFile, O_RDWR);		//if exist 
	
	if (nFd >= 0)
	{
		read(nFd, &dwOldLubData, sizeof(dwOldLubData));
	}
 	close(nFd);

	if(nTemp >= 0)		//�Yoldlub.cdb���s�b,�h�٬OŪ��sram������ƭ� 
	{
		SetDBValue("MACH_LUB_ON_NULL_SHOTCOUNT", 	dwOldLubData[0], FALSE);	//lub1 
		SetDBValue("MACH_ALARM_ON_NULL_ALARMLUBRICATTIME", dwOldLubData[1],FALSE);
		SetDBValue("MACH_LUB_ON_NULL_TIME", 		dwOldLubData[2], FALSE);
		SetDBValue("MACH_LUB_ON_NULL_DELAYTIME", 	dwOldLubData[3], FALSE);
		SetDBValue("MACH_LUB_ON_NULL_COUNTER", 		dwOldLubData[4], FALSE);
		SetDBValue("MACH_LUB_ON_NULL_CHECKCOUNTER", dwOldLubData[5], FALSE);
		SetDBValue("MACH_LUB_ON_NULL_BYPASSOPTION", dwOldLubData[6], FALSE);
		
		SetDBValue("MACH_LUB2_ON_NULL_SHOTCOUNT", 	dwOldLubData[7], FALSE);	//lub2 
		SetDBValue("MACH_LUB_ON_NULL_W019E", 		dwOldLubData[8], FALSE);
		SetDBValue("MACH_LUB2_ON_NULL_TIME", 		dwOldLubData[9], FALSE);
		SetDBValue("MACH_SCRW_INJ_NULL_W0184", 		dwOldLubData[10],FALSE);
		SetDBValue("MACH_SCRW_INJ_NULL_W0185", 		dwOldLubData[11],FALSE);
		
		SetDBValue("MACH_MTR_ON_NULL_W01D9", 		dwOldLubData[12],FALSE);	//lub3 
		SetDBValue("MACH_MTR_ON_NULL_W01D7", 		dwOldLubData[13],FALSE);
		SetDBValue("MACH_MTR_ON_NULL_W01DC", 		dwOldLubData[14],FALSE);
		SetDBValue("MACH_MTR_ON_NULL_W01DD", 		dwOldLubData[15],FALSE);
		SetDBValue("MACH_MTR_ON_NULL_W01DE", 		dwOldLubData[16],FALSE);
		SetDBValue("MACH_MTR_ON_NULL_W01DF", 		dwOldLubData[17],FALSE);
		
		SetDBValue("MACH_MDH_MDH_NULL_W0027", 		dwOldLubData[18],FALSE);	//adj lub  
		SetDBValue("MACH_LUB_ON_START_ADJUSTDELAYTIME", dwOldLubData[19],FALSE);
		SetDBValue("MACH_MDH_MDH_NULL_W0026", 		dwOldLubData[20],FALSE);
		
		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED53", dwOldLubData[21],FALSE);
		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED54", dwOldLubData[22],FALSE);
	}

	/*JOYCE2014-10-7 ���m�ZMACH_MDH_MDH_NULL_W0028����b�~��id_std������l��0,������65535�~�i�H,��]�O���m�Z�Y��0�|�i�JtmTaskMoni.cpp��run�]�^����SaveLubricate(),�ɭP���ưѼƭ��m�Z�ƭ�F*/
	long lTemp = GetDBValue("MACH_MDH_MDH_NULL_W0028").lValue; /*�ռҼ��Ʋ֭p�Ѽ�*/
  if(lTemp == 0xFFFF)	lTemp = 0;
  SetDBValue("MACH_MDH_MDH_NULL_W0028",lTemp);  /*�ռҼ��Ʋ֭p�Ѽ� ���m�Z��65535,�¼��Ʈɵe���W�|�ݤ��즹�Ѽ�,�ҥH��0*/
}


void	CtmDefaultG::ReadLubOpt()
{
	WORD 	wLubOpt[2];
 	char	szPath[16];
	char	m_szCopyFile[64];

	memset(wLubOpt, 0, sizeof(wLubOpt));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/lubOpt.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);

	if (nFd >= 0)
	{
		read(nFd, &wLubOpt, sizeof(wLubOpt));
	}
 	close(nFd);

	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED83", wLubOpt[0], FALSE);
	
//	if(m_bDC)/*JOYCE2013-10-30 ���O���b���MBMC��,�h���ݭn*/ /*JOYCE2014-5-28 �G�O��/�¤G�O �зǶ}�� ����3�ﶵ*/
//		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84", 0, FALSE);
//	else	
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84", wLubOpt[1], FALSE);
}

void	CtmDefaultG::ReadCOM2AmmeterOpt()
{
	WORD 	wCOM2AmmeterOpt[1];
	int		nAmmeterFlag = 0,nN2Cflag =0;
 	char	szPath[16];
	char	m_szCopyFile[64];

	memset(wCOM2AmmeterOpt, 0, sizeof(wCOM2AmmeterOpt));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/COM2Ammeter.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);

	if (nFd >= 0)
	{
		read(nFd, &wCOM2AmmeterOpt, sizeof(wCOM2AmmeterOpt));
	}
 	close(nFd);

	SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED29", wCOM2AmmeterOpt[0], FALSE);

	/*�p�G���s��F���㪩,function.ini��AmmeterFlag�|�ƭ쬰0,�ݸ�COM2�q���ﶵ�P�B*/
	nAmmeterFlag = CtmConfig::GetInstance()->GetAmmeterFlag();
	nN2Cflag     = CtmConfig::GetInstance()->GetN2CFlag();		
	if(wCOM2AmmeterOpt[0] == 1) /*�ب��q��*/
	{	
		if(nAmmeterFlag != 1)
			CtmConfig::GetInstance()->SetAmmeterFlag(1);
		if(nN2Cflag != 0)
			CtmConfig::GetInstance()->SetN2CFlag(0);
	}
	else if(wCOM2AmmeterOpt[0] == 2)	/*�Q�U�q��*/
	{	
		if(nAmmeterFlag != 4)
			CtmConfig::GetInstance()->SetAmmeterFlag(4);
		if(nN2Cflag != 0)
			CtmConfig::GetInstance()->SetN2CFlag(0);
	}
	else if(wCOM2AmmeterOpt[0] == 3)	/*CAN �q�O�p*/
	{	
		if(nAmmeterFlag != 2)
			CtmConfig::GetInstance()->SetAmmeterFlag(2);
		if(nN2Cflag != 1)
			{
			CtmConfig::GetInstance()->SetN2CFlag(1);
			}
		//SetDBValue("AUSTON_PARAMETER_N2C_TYPE", 4);	/*N2C TYPE*/
	}
	else if(wCOM2AmmeterOpt[0] == 4)	/*COM2���T 485*/
	{	
	 if(m_biNet)	 
	 	{
		if(nAmmeterFlag != 3)
			CtmConfig::GetInstance()->SetAmmeterFlag(3);
				printf("nN2Cflag =%d\n",nN2Cflag);
		if(nN2Cflag != 0)
			{
			CtmConfig::GetInstance()->SetN2CFlag(0);
				printf("nN2Cflag =%d\n",nN2Cflag);
				
			}
		}
	 else
	 	{
		if(nAmmeterFlag != 0)
			CtmConfig::GetInstance()->SetAmmeterFlag(0);
		if(nN2Cflag != 0)
			{
			CtmConfig::GetInstance()->SetN2CFlag(0);
			}
	 		
	 	}
			
	}
	else
	{	
		if(nAmmeterFlag != 0)
			CtmConfig::GetInstance()->SetAmmeterFlag(0);
		if(nN2Cflag != 0)
			CtmConfig::GetInstance()->SetN2CFlag(0);
	}
}

void	CtmDefaultG::ReadWarmTime()
{
  DWORD  dwWarmTime[3];
	char	szPath[256];
  char	m_szCopyFile[256];
  char	pszDB[256];

	memset(dwWarmTime, 0, sizeof(dwWarmTime));
	memset(pszDB, 0, sizeof(szPath));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/warmtime.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);      
	
	//printf("CtmDefaultG::ReadWarmTime nFd=%d\n",nFd);
	if (nFd == -1) /*file not exit;set default*/
	{
		SetDBValue("MSET3_TMP1_HEATERON_WARM_TIME", (long)10*6000, FALSE);/*���N�Ұʭp��*/
	}	
	else	if (nFd >= 0)
	{
		read(nFd, &dwWarmTime, sizeof(dwWarmTime));

	  //printf("dwWarmTime=%d,%d,%d\n",dwWarmTime[0],dwWarmTime[1],dwWarmTime[2]);
		SetDBValue("MSET3_TMP1_HEATERON_WARM_TIME", (long)dwWarmTime[0]*6000, FALSE);/*���N�Ұʭp��*/
	}
  close(nFd);
}

void	CtmDefaultG::ReadRotateOpt()
{
	WORD 	wRotateOpt[1];
 	char	szPath[16];
	char	m_szCopyFile[64];
	int		nN2Cflag =0/*, nDSP28flag =0*/;

	memset(wRotateOpt, 0, sizeof(wRotateOpt));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/RotateOpt.cdb", szPath);
	//int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd >= 0)
	{
		read(nFd, &wRotateOpt, sizeof(wRotateOpt));
		//SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED32", wRotateOpt[0], FALSE);//wangli2015-3-6 9:34:09 mark
	}
	else if (nFd == -1) // file not exit
	{
		wRotateOpt[0] = 1;//wangli2015-3-6 9:38:34 �q�{�G����L
		//SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED32", 1, FALSE); //wangli2015-3-6 9:34:18 mark
	}

 	close(nFd);

	SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED32", wRotateOpt[0], FALSE);
	//wangli2015-3-6 9:34:03  �ץ���󤣦s�b�� �Y���G����L, MCONF_OTHERS_OTHERS_NULL_RESERVED32�P wRotateOpt[0]���Ȥ��@�P�AN2Cflag��0�����D

	/*�p�G���s��F���㪩,function.ini��N2CFlag�|�ƭ쬰0,�ݸ�ﶵ�P�B; ��L�ﶵ:0=���ƲG��1=�s�X���G��2=���A�q��*/
	nN2Cflag     = CtmConfig::GetInstance()->GetN2CFlag();
	nDSP28flag   = CtmConfig::GetInstance()->GetDSP28Flag();
	if(wRotateOpt[0] == 1) /*�s�X���G��*/
	{	
		if(nN2Cflag != 1)
			CtmConfig::GetInstance()->SetN2CFlag(1);
		if(nDSP28flag != 1)
			CtmConfig::GetInstance()->SetDSP28Flag(1);
		if((CtmConfig::GetInstance()->GetSramInit() <= 0) && (!g_pExTable->GetTableValue(0,"D_4INJ")))
			{
				SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED43", 120 , FALSE);
				SetDBValue("MACHB_RMLD_ROTA_NULL_W10B9", 80 , FALSE); /*��Ҹ˼����O*/
 				SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED25", 100 , FALSE);	/*��Ҹ˼����O�W�� Zhongjw 2015-5-11 12:45:20*/	
        SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED73", 0 , FALSE);	/*�o�ŧN�o��*/
 				SetDBValue("MACH_TMPOIL_COOLERON_NULL_DEGREEOFFSET", 0 , FALSE);	/*�o�ŧN�o���}*/
 				SetDBValue("MACH_TMPOIL_COOLEROFF_NULL_DEGREEOFFSET", 0 , FALSE);	/*�o�ŧN�o����*/
			}
	}
	else
	{	
		if((nN2Cflag != 0) && (GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED29").lValue != 3))//wangli2015-3-10 10:00:14 �W�[�P�_���OCAN�q�O�p
			CtmConfig::GetInstance()->SetN2CFlag(0);
  	if(nDSP28flag != 0)
			CtmConfig::GetInstance()->SetDSP28Flag(0);
	}
}

/*
void	CtmDefaultG::Read28RotateData()
{
	DWORD 	dw28Rotate[12];
 	char	szPath[16];
	char	m_szCopyFile[64];
	char*   pDataID[] = {
						"MACH_OTHR_OTHR_NULL_W030C",  
						"MACH_OTHR_OTHR_NULL_W030D",  
						"MACH_OTHR_OTHR_NULL_W030E",  
						"MACH_OTHR_OTHR_NULL_W030F",  
						"MACH_OTHR_OTHR_NULL_W0331",  
						"MACH_OTHR_OTHR_NULL_W0355",  
						"MACH_OTHR_OTHR_NULL_W0356",  
						"MACH_OTHR_OTHR_NULL_W0357",  
						"MACH_OTHR_OTHR_NULL_W0361",  
						"MACH_OTHR_OTHR_NULL_W0362",  
						"MACH_OTHR_OTHR_NULL_W0363",  
						"MACH_OTHR_OTHR_NULL_W0364",  
            NULL
            };

	memset(dw28Rotate, 0, sizeof(dw28Rotate));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/28RotateData.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);
	
	//printf("Read28RotateData nFd=%d\n",nFd);
	if (nFd >= 0)//file exit
	{
		read(nFd, &dw28Rotate, sizeof(dw28Rotate));

		for(int i=0; i<12; i++)
		{
			if(pDataID[i] != NULL)
				SetDBValue(pDataID[i], dw28Rotate[i], FALSE);
		}
	}
 	close(nFd);

}
*/
BOOL	CtmDefaultG::ReadBlock(char* pszFileName, WORD wBlockID)
{
	int nSize = 0, nFileSize = 0;
	int nFd = 0;
	//BYTE* pbyBuffer = NULL;
	
	if(wBlockID >= 0)
	{
		nSize = g_pBlock->GetBlockSize(wBlockID);
		
		if(nSize > 0)
		{	
			nFd = open(pszFileName, O_RDWR | O_CREAT );
			if (nFd > 0)
			{
				nFileSize = lseek(nFd, 0, SEEK_END);
				if(nSize != nFileSize)
					return FALSE;
				
				BYTE* pbyBuffer = new BYTE[nSize];
				lseek(nFd,0,SEEK_SET);
				read(nFd, pbyBuffer, nSize);
				close(nFd);
				
				g_pBlock->GetBlock(pbyBuffer, wBlockID);
				
				delete [] pbyBuffer;
				return TRUE;
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

void	CtmDefaultG::ReadParaPassword()//JOYCE2012-3-15 
{
	  WORD wParaPassword[12];
	  int  nIndex = 0;
 	  char	szPath[256];
	  char	m_szCopyFile[256];
    char	pszDB[256];

    memset(wParaPassword, 0, sizeof(wParaPassword));
    memset(pszDB, 0, sizeof(szPath));
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/usr/Data/ParaPassword");
    sprintf(m_szCopyFile, "%s/ParaPassword.cdb", szPath);
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);      
    //printf("nFd:%d\n",nFd);
    if (nFd >= 0)
		{
		 read(nFd, &wParaPassword, sizeof(wParaPassword));
		}
   	close(nFd);

		for(int i=0; i<12;i++)
		{
		 	if(pParaPassword[i] != NULL)
			{
				DB_DATAATTRIBUTE* pAttr = NULL;
			 	pAttr = g_pDatabase->GetAttr();
			 	nIndex = g_pDatabase->GetIndex(pParaPassword[i]);
				pAttr[nIndex].wDisplayPrecision = (pAttr[nIndex].wDisplayPrecision & 0xFFFE) | (wParaPassword[i] & 0x0001);
				//printf("%d=%d,%d\n",i,nIndex,pAttr[nIndex].wDisplayPrecision);
			}
		}
}

void	CtmDefaultG::ReadFlowRatio()	/*JOYCE2012-6-7 �y�q�t�Ƥ��i�Q���m*/
{
	WORD 	wFlowRatio[7];
 	char	szPath[256];
	char	m_szCopyFile[256];
    char	pszDB[256];

	char*   pDataID[] = {
						"MACH_MLD_CLOSE_NULL_W0096",				//close 
						"MACH_MLD_OPEN_BEFOREEND_POSITIONOFFSET",	//open 
						"MACH_SCRW_INJ_NULL_W017D",					//inject
						"MACH_SCRW_RCV_NULL_W0061",					//charge 
						"MACH_EJT_EJT_NULL_W00F3",					//eject 
						"MACHB_EJT2_EJT_NULL_W1033",				//eject B   /*JOYCE2013-6-5 11:38:54*/
						"MACHB_SCRW2_INJ_NULL_W107D",                 //injet B   //wangli2014-2-21 13:53:06
            NULL
            };
    
	memset(wFlowRatio, 0, sizeof(wFlowRatio));
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/flowratio.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd == -1) /*file not exit;set default*/
	{
		wFlowRatio[4] = 66;
		wFlowRatio[5] = 66; /*JOYCE2013-6-5 11:40:13*/
	}	
	else	if (nFd >= 0)
	{
		read(nFd, &wFlowRatio, sizeof(wFlowRatio));
	}
 	close(nFd);
	
	//if(m_bDC)/*������{��,���}�񦫼Ҭy�q�t��;  JOYCE2013-6-5 11:40:29 mark*/
	//	wFlowRatio[4] = 0;
		
	for(WORD i=0; i< sizeof(pDataID)/sizeof(char*) -1 ;i++)
	{
		//printf("wFlowRatio[%d]:%d \n", i,wFlowRatio[i]);
		if(pDataID[i] != NULL)
			SetDBValue(pDataID[i], wFlowRatio[i], FALSE);
	}
}

void	CtmDefaultG::ReadSetGrad()	/*wangli2016-7-22 8:54:34 JUII�� �]�w��� ���i�Q���m*/
{
	DWORD 	ReadSetGrad[8];
 	char	szPath[256];
	char	m_szCopyFile[256];
    char	pszDB[256];

	char*   pDataID[] = {
						"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL1",
						"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL2",
						"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL3",
						"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL4",
						"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL5",
						"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL6",
						"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL7",
						"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL8",
            NULL
            };
    
	memset(ReadSetGrad, 0, sizeof(ReadSetGrad));
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/setgrad.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);
	printf("nFd = %d\n",nFd);
	if (nFd == -1) /*file not exit;set default*/
	{
			for(int i=0; i<8 ;i++)
			{
				ReadSetGrad[i] = 280000;
			}
	}	
	else	if (nFd >= 0)
	{
		read(nFd, &ReadSetGrad, sizeof(ReadSetGrad));
	}
 	close(nFd);
	
		
	for(WORD i=0; i< sizeof(pDataID)/sizeof(char*) -1 ;i++)
	{
		printf("ReadSetGrad[%d]:%d \n", i,ReadSetGrad[i]);
		if(pDataID[i] != NULL)
			SetDBValue(pDataID[i], ReadSetGrad[i], FALSE);
	}
}

void	CtmDefaultG::ReadEjtData()  /*JOYCE2013-6-13 ���ƺާ��h��,����A�t�ο�� ���i�Q���m*/
{
	WORD 	wEjtData[1] = {0};
 	char	szPath[256];
	char	m_szCopyFile[256];
  char	pszDB[256];

	char*   pDataID[] = {
						"MACH_EJT_EJT_NULL_W00F5",
            NULL
            };
    
	memset(wEjtData, 0, sizeof(wEjtData));
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/ejtdata.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd >= 0)
	{
		read(nFd, &wEjtData, sizeof(wEjtData));
	}
 	close(nFd);
			
	for(WORD i=0; i< sizeof(pDataID)/sizeof(char*) -1 ;i++)
	{
		//printf("wEjtData[%d]:%d \n", i,wEjtData[i]);
		if(pDataID[i] != NULL)
			SetDBValue(pDataID[i], wEjtData[i], FALSE);
	}
}

void	CtmDefaultG::ReadRoteTimeOpt()  //wangli2014-9-10 15:46:42��L�ɶ����s���i���m
{
	WORD 	wRoteTimeOpt[1] = {0};
 	char	szPath[256];
	char	m_szCopyFile[256];
  char	pszDB[256];

	char*   pDataID[] = {
						"SYSX_OTHERS_OTHERS_BYTE_RESERVED3",
            NULL
            };
    
	memset(wRoteTimeOpt, 0, sizeof(wRoteTimeOpt));
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/rotetimeopt.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd >= 0)
	{
		read(nFd, &wRoteTimeOpt, sizeof(wRoteTimeOpt));
	}
 	close(nFd);
			
	for(WORD i=0; i< sizeof(pDataID)/sizeof(char*) -1 ;i++)
	{
		//printf("wRoteTimeOpt[%d]:%d \n", i,wRoteTimeOpt[i]);
		if(pDataID[i] != NULL)
			SetDBValue(pDataID[i], wRoteTimeOpt[i], FALSE);
	}
}


void	CtmDefaultG::ReadParallelCnt()  /*zhongjw 2015-4-13 11:07:32 �¤G�������t�p�ƩM�Ա������m���i���m*/
{
	DWORD 	dwParallelCnt[11];
 	char	szPath[256];
	char	m_szCopyFile[256];
  char	pszDB[256];

	char*   pDataID[] = {
									"MSET_OTHR_OTHR_NULL_W084C",
            			"MSET_OTHR_OTHR_NULL_W084E",
            			"MSET_OTHR_OTHR_NULL_W0850",
            			"MSET_OTHR_OTHR_NULL_W0853",
            			"MSET_OTHR_OTHR_NULL_W0854",
            			"MSET_OTHR_OTHR_NULL_W0855",
            			"MSET_OTHR_OTHR_NULL_W0856",
            			"SYSX_OTHERS_OTHERS_DWORD_RESERVED99",
            			"SYSX_OTHERS_OTHERS_DWORD_RESERVED100",
            			"SYSX_OTHERS_OTHERS_DWORD_RESERVED101",
            NULL
            };
    
	memset(dwParallelCnt, 0, sizeof(dwParallelCnt));
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/ParallelCnt.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd >= 0)
	{
		read(nFd, &dwParallelCnt, sizeof(dwParallelCnt));
	}
 	close(nFd);
			
	for(WORD i=0; i< sizeof(pDataID)/sizeof(char*) -1 ;i++)
	{
		//printf("wRoteTimeOpt[%d]:%d \n", i,wRoteTimeOpt[i]);
		if(pDataID[i] != NULL)
			SetDBValue(pDataID[i], dwParallelCnt[i], FALSE);
	}
}

void	CtmDefaultG::ReadOpnCntData()  /*zhongjw 2015-11-13 8:51:12�ը��Ҧ���W���}���`�Ƥ��i���m*/
{
	 WORD 	wOpnCntData = 0;
 	char	szPath[256];
	char	m_szCopyFile[256];
  char	pszDB[256];

	char*   pDataID = "SYSX_OTHERS_OTHERS_INT_RESERVED113";  
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/OpnCntData");
	sprintf(m_szCopyFile, "%s/OpnCntData.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd >= 0)
	{
		read(nFd, &wOpnCntData, sizeof(wOpnCntData));
	}
 	close(nFd);
 	SetDBValue(pDataID, wOpnCntData, FALSE);
}

void	CtmDefaultG::ReadWarningCnt()/*zhongjw 2015-12-23 �Үy�պA�e���A������O�ʴ��Oĵ�����Ƥ��i���m*/
{
	WORD 	wWarningCntData = 0;
 	char	szPath[256];
	char	m_szCopyFile[256];
  char	pszDB[256];

	char*   pDataID = "MSET_SCRW_HLDP_NULL_W068E";  
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/WarningCntData");
	sprintf(m_szCopyFile, "%s/WarningCntData.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd >= 0)
	{
		read(nFd, &wWarningCntData, sizeof(wWarningCntData));
	}
 	close(nFd);
 	SetDBValue(pDataID, wWarningCntData, FALSE);
}


void	CtmDefaultG::ReadTBARData()
{
	  WORD wTBARData[20];
 	  char	szPath[256];
	  char	m_szCopyFile[256];
    char	pszDB[256];

    memset(wTBARData, 0, sizeof(wTBARData));
    memset(pszDB, 0, sizeof(szPath));
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf/TBARData");
    sprintf(m_szCopyFile, "%s/TBARData.cdb", szPath);
    int nFd =  open(m_szCopyFile, O_RDWR);
  
    //printf("nFd:%d\n",nFd);

		if (nFd == -1) /*file not exit;set default*/
		{
			wTBARData[1] = 3500;  /*4�өԧ�����h����I����l��35.00*/
			wTBARData[2] = 3500;
			wTBARData[3] = 3500;
			wTBARData[4] = 3500;
		}	
		else	if (nFd >= 0)
		{
			 read(nFd, &wTBARData, sizeof(wTBARData));
		}
    close(nFd);

		for (int i = 0; i < 20; i++)
		{
    	//printf("wTBARData[%d]:%d\n",i,wTBARData[i]);
    	if(i < 6)
    	{
    		sprintf(pszDB, "MACH_SHUT_SHUT_NULL_W026%X", i+8);/*268~26D*/
    		SetDBValue(pszDB, wTBARData[i],FALSE);
    	}
    	else if(i < 12)
    	{
    		sprintf(pszDB, "MSET_SCRW_INJ_NULL_W077%X", i-2);/*774~779*/
    		SetDBValue(pszDB, wTBARData[i],FALSE);
    	}
    	else if(i < 16)
    	{
    		sprintf(pszDB, "MSET_SCRW_INJ_NULL_W077%X", i);/*77C~77F*/
    		SetDBValue(pszDB, wTBARData[i],FALSE);
    	}    	
    	else if(i == 16)
    	{	
    		SetDBValue("SYS_HMI_NULL_NULL_RESERVED42", wTBARData[i],FALSE);
    	}
    	else if(i < 19)
    	{
    		sprintf(pszDB, "MACH_MDH_MDH_NULL_W002%X", i-5);/*02C~02D*/
    		SetDBValue(pszDB, wTBARData[i],FALSE);
    	}
    	else
    	{
    		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED87", wTBARData[i],FALSE);
    	}	    	    		
    }

 			/*JOYCE2014-5-4  ���e�Ҩ�p��=E(��h�ؼЪ��Үy�����m) + X(�X�Ҿ���̤p��m) - A(�ϩʼҪO�p��) - C(�ҪO�����m)*/
 			long lTemp1 = 0;
 			lTemp1 = GetDBValue("MSET_SCRW_INJ_NULL_W0775").lValue + GetDBValue("MACH_MDH_MDH_NULL_W002D").lValue -GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue - GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED87").lValue;
 			if(lTemp1 < 0)
 				lTemp1 = 0;
 			SetDBValue("MSET_OTHR_OTHR_NULL_W083C", lTemp1);

			/*�D�����ϥγo�Ǽƾ�,�i�u�b�e�����i���s*/	
//			 /*20140504 ��ڼҪO�Z��=�}�Ҧ�m(�Үy�ع�ڭ�) + A(�ϩʼҪO�p��) + B(���e�Ҩ�p��)*/
//			lTemp1 = GetDBValue("METER_AD1_READ_CHL2_RELATIVEPOSITION").lValue + GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue + GetDBValue("MSET_OTHR_OTHR_NULL_W083C").lValue;
//			if(lTemp1 < 0)
//				lTemp1 = 0;
//			SetDBValue("MSET_OTHR_OTHR_NULL_W083D", lTemp1, FALSE);
//
//			lTemp2 = lTemp1 - GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue;/*��ڼҫp�Z��=�i��ڼҪO�Z���j-�i�ϩʼҪO�p�סj*/
//			if(lTemp2 < 0)
//				lTemp2 = 0;
//			SetDBValue("MSET_OTHR_OTHR_NULL_W083E", lTemp2, FALSE); 
}


void	CtmDefaultG::ReadInjhSynchro()  /*zhongjw 2016-5-19 �G�O�� ��ҦP�B�`�g�\�� ���i�Q���m*/
{
	WORD 	wSynchroData[1] = {0};
 	char	szPath[256];
	char	m_szCopyFile[256];
  char	pszDB[256];

	char*   pDataID[] = {
						"MSET_MLD_CLOSE_NULL_W049D",
            NULL
            };
    
	memset(wSynchroData, 0, sizeof(wSynchroData));
	memset(pszDB, 0, sizeof(pszDB));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/SynchroData.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd >= 0)
	{
		read(nFd, &wSynchroData, sizeof(wSynchroData));
	}
 	close(nFd);
			
	for(WORD i=0; i< sizeof(pDataID)/sizeof(char*) -1 ;i++)
	{
		if(pDataID[i] != NULL)
			SetDBValue(pDataID[i], wSynchroData[i], FALSE);
	}
}



//wangli2014-11-7 13:49:49 �зǵ{�Ǩϥηūצʤ���A�A��J�ҷŰt�m�A�p�G�����m�A�h�ݭn�M�������q���[���ʤ���			
void  CtmDefaultG::TempPrecentClear(int nMTBegin, int nMTEnd, BOOL bMTFlag,int nGroup)
{
	char	pszDB[256];
	long 	lTemp =0;
	if(nGroup)
		{
			sprintf(pszDB,"SYSX_OTHERS_OTHERS_DWORD_RESERVED5");
			nMTBegin-=10;
			nMTEnd-=10;
		}
	else
		sprintf(pszDB,"SYSX_OTHERS_OTHERS_DWORD_RESERVED3");
	
	lTemp = GetDBValue(pszDB).lValue;
	for(int i=nMTBegin;i<(nMTEnd+1);i++)
	{
		if(bMTFlag) 
			{
				if((lTemp >> (i-1)) & 0x0001)
				{
					lTemp = _ClearBit(lTemp, i-1);//�M�������q���[���ʤ���
					SetDBValue(pszDB, lTemp);	
				}
			}
	}
}


//wangli2014-11-7 13:53:02�зǪ�����J�ҷŰt�m�A�M���ƷŨϥΡA�p�G���O�ҷŰt�m�A�A��зǪ��A�ɲM���ҷ�
void  CtmDefaultG::MoldTempUse(int nMTBegin, int nMTEnd, BOOL bMTFlag)
{
	char 	sz1[125];
	char 	sz2[125];
	
	for(int i=nMTBegin;i<(nMTEnd+1);i++)
	{
		sprintf(sz1,"MACH_TMP1_HEATERONOFF_RAMP%d_DEGREE",i);
		sprintf(sz2,"MSET_ALARM_ON_CHL%d_MAXTMPDEGREEOFFSET",i);
		
		if(bMTFlag)
			{
				SetDBValue(sz1,GetDBValue(sz1).lValue & 0xFFFE,FALSE);//�M���Ʒ�bit0
				SetDBValue(sz1,GetDBValue(sz1).lValue | 0x0100,FALSE);
				
				//SetDBValue(sz2,GetDBValue(sz2).lValue | 0x2000,FALSE);//wangli2015-1-6 16:17:06 mark �ץ��ҷų]�����ΦZ�A�}���۰��ܬ��ֳt�B���t�B�C�t�����D
				
			}
		else
			{
			
			if(GetDBValue(sz1).lValue != 0xFFFF)
			{
				if(!(GetDBValue(sz1).lValue | 0x0001))
					SetDBValue(sz1,GetDBValue(sz1).lValue | 0x0001,FALSE);//�Ʒ�bit0
				if(GetDBValue(sz1).lValue & 0x0100)
					SetDBValue(sz1,GetDBValue(sz1).lValue & 0xFEFF,FALSE);//�M���ҷ�
			}
				
			if(GetDBValue(sz2).lValue != 0xFFFF)
			{
			if(GetDBValue(sz2).lValue & 0x2000)
				SetDBValue(sz2,GetDBValue(sz2).lValue & 0xDFFF,FALSE);
			}
			
			}
	}
}


//wangli2014-11-6 11:25:14 �зǵ{�ǡA��WCOM1�ҷŵ{�ǡA��COM1�ҷŵ{����W�зǵ{�ǡA����S�����m�o�ͥ[�Ų��`
void	CtmDefaultG::MoldTempSet()
{
	char	psz[5];
	GetDBString("VERS_HMI_NULL_NULL_PROGRAMRESVERS",psz,4);//�P�_�ҷż���
	switch(psz[2])
	{
		case	'1':				//D_MDT10  com1, channel 11-20 
			MoldTempUse(11,20,TRUE);
			TempPrecentClear(11,20,TRUE,1);
			break;
		case	'2':
			break;
		case	'3':				//D_MDT4  com1, channel 6-9 
			MoldTempUse(6,9,TRUE);
			TempPrecentClear(6,9,TRUE,0);
			break;
		case	'4':				//D_T3M6  com1, channel 4-9 
			MoldTempUse(4,9,TRUE);
			TempPrecentClear(4,9,TRUE,0);
			break;
		case	'5':				//D_T3M5  com1, channel 4-8
			MoldTempUse(4,8,TRUE);
			TempPrecentClear(4,8,TRUE,0);
			break;
		case	'6':
			break;
		case	'7':
			break;
		case	'8':
			break;
		case	'9':				//D_T7M2  com1, channel 8-9
			MoldTempUse(8,9,TRUE);
			TempPrecentClear(8,9,TRUE,0);
			break;
		case	'A':				//COM2
			break;
		case	'B':				//COM2
			break;
		case	'C':
			break;
		case	'D':				//D_T5M14   com1, channel 6-9&&11-20
			MoldTempUse(6,9,TRUE);
			MoldTempUse(11,20,TRUE);
			TempPrecentClear(6,9,TRUE,0);
			TempPrecentClear(11,20,TRUE,1);
			break;
		case	'E':				//com2 40/com1 chnl8/9
			MoldTempUse(8,9,TRUE);
			TempPrecentClear(8,9,TRUE,0);
			break;
		case	'F':				//D_T5M3 com1 CHL6~8
			MoldTempUse(6,8,TRUE);
			TempPrecentClear(6,8,TRUE,0);
			break;
		case	'H':				//D_T12M4   com1, channel 7-8&&17-18
			MoldTempUse(7,8,TRUE);
			MoldTempUse(17,18,TRUE);
			TempPrecentClear(7,8,TRUE,0);
			TempPrecentClear(17,18,TRUE,1);
			break;
		case	'I':				//D_T14M4   com1, channel 8-9&&18-19
			MoldTempUse(8,9,TRUE);
			MoldTempUse(18,19,TRUE);
			TempPrecentClear(8,9,TRUE,0);
			TempPrecentClear(18,19,TRUE,1);
			break;
		case	'J':				//D_T5M14   com1, channel 4-9&&11-20
			MoldTempUse(4,9,TRUE);
			MoldTempUse(11,20,TRUE);
			TempPrecentClear(4,9,TRUE,0);
			TempPrecentClear(11,20,TRUE,1);
			break;
		case	'K':				//D_MDT8    com1, channel 1-8
			MoldTempUse(1,8,TRUE);
			TempPrecentClear(1,8,TRUE,0);
			break;
		case	'L':				//D_T5M2 com1 CHL6~7;
			MoldTempUse(6,7,TRUE);
			TempPrecentClear(6,7,TRUE,0);
			break;
		case	'M':				//D_T6M2 com1, channel 7-8
			MoldTempUse(7,8,TRUE);
			TempPrecentClear(7,8,TRUE,0);
			break;
		case	'P':				//T11M4 com1, channel 13-16
			MoldTempUse(13,16,TRUE);
			TempPrecentClear(13,16,TRUE,0);
			break;
		case	'Q':				/*wangli2015-6-1 13:30:54 T16M2 com1, channel 8,18*/
			MoldTempUse(18,18,TRUE);
			TempPrecentClear(18,18,TRUE,0);
			break;
		default:
			MoldTempUse(1,20,FALSE);	//wangli2014-11-6 10:58:36 �S���ҷšACH1-20: bit8�M0.		
			break;	
	}		
}

void	CtmDefaultG::TestMode()/*zhongjw 2015-11-13 15:18:38�M�s�Z�ը��Ҧ��ܬ��ϥΡA���Ұ����W���ȧאּ175*/
{
    	long long  lTemp1 = 0;
    	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED113").lValue < 1000)
    	{
    		SetDBValue("MACH_SLUC_SLUC_NULL_W0278", 1,FALSE);
    		lTemp1 = 175;	
       	long lHiClsPress = GetDBValue("MSET_MLD_CLOSE_CLAMPUP_PRESSURE").lValue;	
    		if(lHiClsPress > lTemp1)
    		{	
    			SetDBValue("MSET_MLD_CLOSE_CLAMPUP_PRESSURE", lTemp1,FALSE);
    		} 
   	 }  
}


void	CtmDefaultG::SetAdjCnt()
{
    int         pos,deltaT;
    int         x, y;
		long        k;
		char				pszID1[128],pszID2[128];
     deltaT = GetDBValue("SYSX_MOLD_CLOSE_CLAMPUP_MAXFORCE").lValue;//HEX2BCD(wTon);
     if (deltaT == 0) return;
     x = GetDBValue("SYS_MOLD_MOLD_NULL_POSN2COUNTRATIO").lValue;
     x *=10;
     pos = x/deltaT;     
   	sprintf(pszID1,"SYS_MOLD_MOLD_NULL_TONSTABLE%d",pos+1);
   	sprintf(pszID2,"SYS_MOLD_MOLD_NULL_TONSTABLE%d",pos+2);
     k   = ((GetDBValue(pszID2).lValue - GetDBValue(pszID1).lValue)*( x%deltaT )+deltaT/2)/deltaT;
     y   = k + GetDBValue(pszID1).lValue;          
     SetDBValue("MSET_MDH_FWD_NULL_COUNTER",y);
}


//void	CtmDefaultG::ReadOneJarInjOpt()	//wangli2014-9-29 9:35:47 ����`�g
//{
//	DWORD 	wOneJarInjData[1];
// 	char	szPath[16];
//	char	m_szCopyFile[64];
//
//	memset(wOneJarInjData, 0, sizeof(wOneJarInjData));
//	memset(szPath, 0, sizeof(szPath));
//	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
//	sprintf(szPath, "%s", "/conf");
//	sprintf(m_szCopyFile, "%s/OnejarInje.cdb", szPath);
//	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
//
//	if (nFd >= 0)
//	{
//		read(nFd, &wOneJarInjData, sizeof(wOneJarInjData));
//	}
// 	close(nFd);
//	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED17", wOneJarInjData[0], FALSE);
//}

void	CtmDefaultG::ReadOneJarInjOpt()	//wangli2014-10-27 11:52:59 ����� ����`�g //wangli2014-12-19 15:53:28 
{
	DWORD 	wOneJarInjData[2];
 	char	szPath[16];
	char	m_szCopyFile[64];

	memset(wOneJarInjData, 0, sizeof(wOneJarInjData));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	sprintf(m_szCopyFile, "%s/OnejarInje.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd == -1) // file not exit
	{
		//printf(" OneJarInje not exit!\n");
	SetDBValue("MACH_SCRW_RCV_NULL_W0066", 1, FALSE);
	SetDBValue("MACHB_SCRW2_RCV_NULL_W1014", 1, FALSE);
			
	}
	else if (nFd >= 0)
	{
		//printf(" OneJarInje !\n");
		read(nFd, &wOneJarInjData, sizeof(wOneJarInjData));

		SetDBValue("MACH_SCRW_RCV_NULL_W0066", wOneJarInjData[0], FALSE);
		SetDBValue("MACHB_SCRW2_RCV_NULL_W1014", wOneJarInjData[1], FALSE);
	}
 	close(nFd);
}

WORD	CtmDefaultG::GetMoldTempType()
{
	char	psz[5];
	GetDBString("VERS_HMI_NULL_NULL_PROGRAMRESVERS",psz,4);//�P�_�ҷż��� 
	
	switch(psz[2])
	{
		case	'1':
			return	1;
			break;
		case	'2':
			return	2;
			break;
		case	'3':
			return	3;
			break;
		case	'4':
			return	4;
			break;
		case	'5':
			return	5;
			break;
		case	'6':
			return	6;
			break;
		case	'7':
			return	7;
			break;
		case	'8':
			return	8;
			break;
		case	'9':
			return	9;
			break;
		case	'A':
			return	10;
			break;
		case	'B':
			return	11;
			break;
		case	'C':
			return	12;
			break;
		case	'D':
			return	13;
			break;
		case	'E':
			return	14;
			break;
		case	'F':
			return	15;
			break;
		case	'G':
			return	16;
			break;
		case	'H':
			return	17;
			break;
		case	'I':
			return	18;
			break;
		case	'J':
			return	19;
			break;
		case	'K':
			return	20;
			break;
		case	'L':
			return	21;
			break;
		case	'M':
			return	22;
			break;
		case	'P':
			return	25;
			break;
		case	'Q':
			return	26;
			break;
		default:
			return	0;
			break;	
	}		
}

//wangli2015-3-13 14:38:33 ˫ɫ�� ����¶�Ĭ��Ϊʹ�ã����ǵ�Ϊ���µ�9����ģ��ʱ��ֻ��Ϊ���á�
void	CtmDefaultG::SetMrtTempNUse()
{
		int n_MoldTempType = GetMoldTempType();

		switch(n_MoldTempType)
			{
				case	14: //com2 40/com1 chnl8/9
				case	18: //D_T14M4   com1, channel 8-9&&18-19 //JOYCE2012-5-31 
					SetDBValue("MACH_OTHR_OTHR_NULL_W01CF", 0);
					SetDBValue("SYSX_TEMPER_HEATERON_NULL_SHOWHEATWAY", 0);
					SetDBValue("MACH_TMP1_HEATERONOFF_RAMP9_DEGREE", 0x0100);
					SetDBValue("MSET_TMP1_HEATERON_CHL9_DEGREE", 25,FALSE);
					SetDBValue("MSET_TMP1_HEATERON_CHL19_DEGREE", 25,FALSE);
					if(g_pExTable->GetTableValue(0,"D_T14M4"))
						SetDBValue("MACH_TMP1_HEATERONOFF_RAMP19_DEGREE", 0x0100);
					else if(g_pExTable->GetTableValue(0,"D_MDT2"))
						SetDBValue("MACH_TMP1_HEATERONOFF_RAMP19_DEGREE", 0);
					break;
				default:
					break;
			}
}

void CtmDefaultG::SetDefaultBySVRT() /*Zhongjw �¤G�O�����A�Үy�ѼƤG�M�պA�G�e���q�{�ȭק�2015-3-18 14:14:05*/
{
	if(!m_bJUIII)
		SetDBValue("MACH_OTHR_OTHR_NULL_W01B3", 210, FALSE);
	SetDBValue("MACH_SLUC_SLUC_NULL_W0275", 0, FALSE);	
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED96", 10, FALSE);
	SetDBValue("MSET_CMPXMLD_OPEN_CLAMPDN_FLOW",8,FALSE);
}
