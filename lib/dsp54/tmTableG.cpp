/*===========================================================================+
|  Class    : CtmTableG	                                             |
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
#include	"tmTableG.h"
#include	"../../commonaction.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//分解動作狀態:射出、保壓、儲料、中子、絞牙
char*		pszInjectStepID[]=
{
	"OPS_INJECT",
	"OPS_INJECT1",
	"OPS_INJECT2",
	"OPS_INJECT3",
	"OPS_INJECT4",
	"OPS_INJECT5",
	"OPS_INJECT6",
	"OPS_INJECT7",
	"OPS_INJECT8",
	"OPS_INJECT9",
	"OPS_INJECT10",
};

char*		pszHoldStepID[]=
{
	"OPS_HOLDPRESS",
	"OPS_HOLDPRS1",
	"OPS_HOLDPRS2",
	"OPS_HOLDPRS3",
	"OPS_HOLDPRS4",
	"OPS_HOLDPRS5",
	"OPS_HOLDPRS6",
	"OPS_HOLDPRS7",
	"OPS_HOLDPRS8",
	"OPS_HOLDPRS9",
	"OPS_HOLDPRS10",
};

char*		pszChargeStepID[]=
{
	"OPS_CHARGE",
	"OPS_CHARGE1",
	"OPS_CHARGE2",
	"OPS_CHARGE3",
	"OPS_CHARGE4",
	"OPS_CHARGE5",
};	
		
char*		pszCoreInGroupID[]=
{
	"OPS_COREIN",
	"OPS_COREINA",
	"OPS_COREINB",
	"OPS_COREINC",
	"OPS_COREIND",
	"VW_CORE_EIN",
	"VW_CORE_FIN",
};

char*		pszCoreOutGroupID[]=
{
	"OPS_COREOUT",
	"OPS_COREOUTA",
	"OPS_COREOUTB",
	"OPS_COREOUTC",
	"OPS_COREOUTD",
	"VW_CORE_EOUT",
	"VW_CORE_FOUT",
};

char*		pszScrewInGroupID[]=
{
	"VW_CORE_SCREWIN",
	"OPS_SCREWINA",
	"OPS_SCREWINB",
	"OPS_SCREWINC",
	"OPS_SCREWIND",
	"VW_CORE_SCREWEIN",
	"VW_CORE_SCREWFIN",
};

char*		pszScrewOutGroupID[]=
{
	"VW_CORE_SCREWOUT",
	"OPS_SCREWOUTA",
	"OPS_SCREWOUTB",
	"OPS_SCREWOUTC",
	"OPS_SCREWOUTD",
	"VW_CORE_SCREWEOUT",
	"VW_CORE_SCREWFOUT",
};
//分解警報狀態：中子、機械手
char*		pszCoreActPosErr[]=
{
	"ERR_COREAPACTPOSERR",
	"ERR_COREBPACTPOSERR",
	"ERR_CORECPACTPOSERR",
	"ERR_COREDPACTPOSERR",
	"ERR_COREEPACTPOSERR",
	"ERR_COREFPACTPOSERR",
	"ERR_COREGPACTPOSERR",
	"ERR_COREHPACTPOSERR",
	"ERR_COREIPACTPOSERR",
	"ERR_COREJPACTPOSERR",
	"ERR_COREKPACTPOSERR",
	"ERR_CORELPACTPOSERR",
};

char*		pszCorePullErr[]=
{
	"ERR_COREAINPULLENDERR",
	"ERR_COREBINPULLENDERR",
	"ERR_CORECINPULLENDERR",
	"ERR_COREDINPULLENDERR",
	"ERR_COREEINPULLENDERR",
	"ERR_COREFINPULLENDERR",
	"ERR_COREGINPULLENDERR",
	"ERR_COREHINPULLENDERR",
	"ERR_COREIINPULLENDERR",
	"ERR_COREJINPULLENDERR",
	"ERR_COREKINPULLENDERR",
	"ERR_CORELINPULLENDERR",
	"ERR_COREAOUTPULLENDERR",
	"ERR_COREBOUTPULLENDERR",
	"ERR_CORECOUTPULLENDERR",
	"ERR_COREDOUTPULLENDERR",
	"ERR_COREEOUTPULLENDERR",
	"ERR_COREFOUTPULLENDERR",
	"ERR_COREGOUTPULLENDERR",
	"ERR_COREHOUTPULLENDERR",
	"ERR_COREIOUTPULLENDERR",
	"ERR_COREJOUTPULLENDERR",
	"ERR_COREKOUTPULLENDERR",
	"ERR_CORELOUTPULLENDERR",
};

char*		pszRobotErr[]=
{
	"ERR_WAITINGROBOT1",
	"ERR_WAITINGROBOT1",
	"ERR_ENABLEINJECTIONFORAIRMOLD",
	"ERR_ENABLEEJECTBYROBOT",
	"ERR_ENABLECLOSEBYROBOT",
	"ERR_ENABLEOPENBYROBOT",
	"ERR_ENABLEEJECTBWDBYROBOT",
	"ERR_ENABLERECYCLEBYROBOT",
	"ERR_MOLDAREAFREE",
	"ERR_EMERGENCYSTOPOFHANDLING",
	"ERR_ENABLECOREPULLERFWDBYROBOT",
	"ERR_ENABLECOREPULLERBWDBYROBOT",
	"ERR_WAITINGROBOT3",
	"ERR_WAITINGROBOT4",
	"ERR_WAITINGROBOT5",
	"ERR_WAITINGROBOT6",
};	
/*******
|       Initialize static class member
*******/
CtmTableG*		g_pTableG			= NULL;
int						g_nTableCount	= 0;

CIniFile	 		m_TableIni;	

IMPLEMENT_DYNCREATE(CtmTableG, CtmExtentTable)

/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/

CtmTableG::CtmTableG(): CtmExtentTable()
{
	
}

CtmTableG::~CtmTableG()
{
	
}

int	CtmTableG::CreateSelf()
{
	m_pszFilePAID 			= NULL;
	m_pszFilePBID		  	= NULL;
	m_pszFilePCID		  	= NULL;
	m_pszFileStateID		= NULL;
	m_pszFileErrorID  	= NULL;
	
	m_pszFileStateValue = NULL;
	m_pszFileErrorValue = NULL;
	
	m_pszFileActLedValue 	= NULL; /*ZHBA 2014-2-18 */
	m_pszFileOperConf       = NULL; /*ZHBA 2014-2-18 */
	m_pszFileOperLed        = NULL; //20150513

	m_pszFileViewPath		= NULL;
	m_pszFileViewID  		= NULL;
	m_pszFileViewNID 		= NULL;
	
	m_pszFileExKeyID		= NULL;
	m_pszFileExKeyCode	= NULL;//獲取按鍵交換值所在文件 Andy 20081105
	
	memset(m_szPAID, 			0,	sizeof(m_szPAID) 			);
	memset(m_szPBID,	 		0,	sizeof(m_szPBID) 			);
	memset(m_szPCID,			0, 	sizeof(m_szPCID)			);
	memset(m_szStateID,		0,	sizeof(m_szStateID)	 	);
	memset(m_szErrorID,		0,	sizeof(m_szErrorID)	 	);
                         
  memset(m_szViewName,	0,	sizeof(m_szViewName)	);                       
                         
	memset(m_StateValue,	0,	sizeof(m_StateValue) 	); //Andy 20081029
	memset(m_ErrorValue,	0,	sizeof(m_ErrorValue) 	);
	
	memset(m_ViewID,			0,	sizeof(m_ViewID) 			);

	memset(m_ActLedValue,	0,	sizeof(m_ActLedValue)); /*ZHBA 2014-2-18 */
	memset(m_OperConf,      0,	sizeof(m_OperConf)); /*ZHBA 2014-2-18 */
    memset(m_OperLed,       0,	sizeof(m_OperLed));  //20150513
                         
	memset(m_szExKeyID,		0,	sizeof(m_szExKeyID) 	);//獲取按鍵交換值所在文件 Andy 20081105
	memset(m_nExKeyValue,	0,	sizeof(m_nExKeyValue) );//獲取按鍵交換值所在文件 Andy 20081105
	
	memset(m_szFuncDefine,0,	sizeof(m_szFuncDefine));//Andy 20090908
	
	for (int j = 0; j < 16; j++)
	{
		for (int i = 0; i < CNT_INT_MAX_TABLE_NUM; i++)
		{
			m_szErrorID[j][i] = new char[128];
			memset(m_szErrorID[j][i], 0,128 );
		}
	}
	
	for (int i = 0; i < CNT_INT_MAX_TABLE_NUM; i++)
	{
		m_szPAID[i]		 	= new char[128];
		m_szStateID[i] 	= new char[128];
//		m_szErrorID[i] 	= new char[128];
		m_szExKeyID[i] 	= new char[128];//獲取按鍵交換值所在文件 Andy 20081105
		
		m_szViewName[i] = new char[128];
		
		m_szFuncDefine[i] = NULL;//Andy 20090908
		
		memset(m_szPAID[i], 	 0,	128 );
		memset(m_szStateID[i], 0,	128 );
//		memset(m_szErrorID[i], 0,	128 );
		memset(m_szExKeyID[i], 0,	128 );//獲取按鍵交換值所在文件 Andy 20081105
		
		memset(m_szViewName[i], 0,	128 );

		//memset(m_szFuncDefine[i], 0,	128 );//Andy 20090908
	}

	for	(int j = 0; j < 4; j++)
	{
		for (int i = 0; i < CNT_INT_MAX_TABLE_NUM; i++)
		{
			m_szPBID[j][i] = new char[128];
			m_szPCID[j][i] = new char[128];
			memset(m_szPBID[j][i], 0,128 );
			memset(m_szPCID[j][i], 0,128 );
		}
	}

	memset(m_wMldTmpIndex, 			0,	sizeof(m_wMldTmpIndex));

	GetFileName();
	Init();

	return 0;
}

int	CtmTableG::FreeSelf()
{
	delete m_pszFilePAID; 	  
	delete m_pszFilePBID;		
	delete m_pszFilePCID;		
	delete m_pszFileStateID; 
	delete m_pszFileErrorID; 
	delete m_pszFileStateValue; 
	delete m_pszFileErrorValue;

	delete m_pszFileViewPath;
	delete m_pszFileViewID; 
	delete m_pszFileViewNID;

	delete m_pszFileExKeyID;
	delete m_pszFileExKeyCode; //獲取按鍵交換值所在文件 Andy 20081105

	delete []m_ActLedValue; /*ZHBA 2014-2-18 */
	delete []m_OperConf;/*ZHBA 2014-2-18 */
    delete []m_OperLed; //20150513
		
	delete []m_StateValue;
	delete []m_ErrorValue;
	delete []m_nExKeyValue;//獲取按鍵交換值所在文件 Andy 20081105
	
	delete []m_ViewID;
	
	for (int j = 0; j < 16; j++)
	{
		for (int i = 0; i < CNT_INT_MAX_TABLE_NUM; i++)
		{
			delete [](m_szErrorID[i]);	
		}	
	}	
	
	for (int i = 0; i < CNT_INT_MAX_TABLE_NUM; i++)
	{
		delete [](m_szPAID[i]);
		delete [](m_szStateID[i]);
//		delete [](m_szErrorID[i]);
		delete [](m_szExKeyID[i]);//獲取按鍵交換值所在文件 Andy 20081105
		
		delete [](m_szViewName[i]);
	}
	
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < CNT_INT_MAX_TABLE_NUM; i++)
		{
			delete [](m_szPBID[i]);
			delete [](m_szPCID[i]);			
		}	
	}	
	return CtmExtentTable::FreeSelf();
}

/*========================================================================+
|	接口函數，使用g_pExTable->GetTableValue(CNT_TABLE_IO_**,(int*)&wAssign)	|
|	參數：int nType ，外掛類型																							|
|	參數：int* pResult，傳入定義，傳出返回值																|
+========================================================================*/
WORD	CtmTableG::GetTableValue(int nType,int* pResult,void* pEx)
{
	int	wAssign = *pResult;
	char	ViewName[128];
	memset(ViewName,0,sizeof(ViewName));

	switch(nType)
	{
		case	CNT_TABLE_IO_PB:	//0
			return	GetPBTableValue((WORD)wAssign,(BOOL*)pEx);
			break;
		case	CNT_TABLE_IO_PC:	//1
			return	GetPCTableValue((WORD)wAssign,(BOOL*)pEx);
			break;
		case	CNT_TABLE_STATE:	//2
			return	GetStateTableValue((WORD)wAssign);
			break;
		case	CNT_TABLE_ERROR:	//3
			return	GetErrotTableValue(wAssign,pResult,pEx);
			break;
		case	4:
			return  GetExKey((WORD)wAssign);
			break;
		case	5://獲取畫面名稱
			if(GetViewTableName(pResult,ViewName))
			{
				strcpy((char*)pEx,ViewName);
				return 1;
			}	
			else
				return 0;  
			break;
		case	6://根據INDEX獲取DEFINE名稱
			if(m_szFuncDefine[wAssign] != NULL)
				{
					strcpy((char*)pEx,m_szFuncDefine[wAssign]);
					return 1;
				}	
			else
				return 0; 
			break;
		case	7:
			break;
		case	10:		
      		if (m_pszFileActLedValue 	!= NULL) 	 /*ZHBA 2014-2-18 */
	    	{
			    return m_ActLedValue[(WORD)wAssign];
	    	}
	    	//printf("%d m_ActLedValue[wAssign]=%d \n", wAssign,m_ActLedValue[wAssign]);
			break;
		case	11:	
			if (m_pszFileOperConf != NULL) 	 /*ZHBA 2014-2-18 */
		    {
			    return m_OperConf[(WORD)wAssign];
		    }
			break;
		case	12:	
			if (m_pszFileOperLed != NULL) 	 //20150513
	        {
		        return m_OperLed[(WORD)wAssign];
	        }
			break;
		default:
			break;			
	}
	return *pResult;	
}

WORD	CtmTableG::GetTableValue(int nType,char* pszID,void* pEx)
{
	switch(nType)
	{
		case	0://判斷DEFINE是否存在
			for(int i=0;i<CNT_INT_MAX_TABLE_NUM;i++)
			{		
				if(m_szFuncDefine[i] == NULL)	break;
				if(strcmp(m_szFuncDefine[i],pszID) == 0) return	1;
			}
			break;
		case	1:
			break;
		default:
			break;
}	
	return 0;	
}
//獲取按鍵交換值 Andy 20081105
WORD	CtmTableG::GetExKey(WORD wIndex)
{
	long long lValue = -1;

	for(int i = 0; i < CNT_INT_MAX_TABLE_NUM; i++)
	{
		if(wIndex == m_nExKeyValue[i][0])
		{
			if( m_szExKeyID[i][0] != '\0' )
			{
				lValue = GetDBValue(m_szExKeyID[i]).lValue;
				if( lValue < 0 || lValue >= 10)
					lValue = 0;
				return m_nExKeyValue[i][lValue];
			}
			else
			{
				return wIndex;
			}	
		}
	}
	
	return wIndex;
}

//獲取PB字串INDEX，含PB轉PA
WORD	CtmTableG::GetPBTableValue(WORD wAssign,BOOL* pEx)
{
	int			iStrIndex 	 = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY");
	WORD		ASSIGN_PB2PA = 0x0200;

	long lDspLibDate;
	long lOtherCode0;
	
	if(pEx&&*pEx)
		{
			lDspLibDate = GetDBValue("CONF2_CODE_WRITE_NULL_DATA16").lValue;
			lOtherCode0 = GetDBValue("CONF2_CODE_WRITE_NULL_MCHINEDATA1").lValue;
		}
	else
		{
			lDspLibDate = GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
			lOtherCode0 = GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;			
		}	

	if(lDspLibDate >= 0x7B27 && lOtherCode0 >= 2007 || lOtherCode0 >= 2008) ASSIGN_PB2PA = 0x0800;		//new PB2PA
	
	if(wAssign&ASSIGN_PB2PA)
		iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szPAID[wAssign&0xFF]);
	else	
		{
			if(lDspLibDate >= 0x7B27 && lOtherCode0 >= 2007 || lOtherCode0 >= 2008)	wAssign &= 0x03FF;//過濾高位bit
			else																							wAssign &= 0x01FF;
			
			if(wAssign < 0x0400)
			{	
				GetMoldTempType();  /*JOYCE2013-10-21*/
			
				switch(wAssign) //獲取PB字串ID
				{
					case	0x210:   /*JOYCE2013-10-21 COM1料溫作模溫, 加熱X區斷線顯示為模溫Y區斷線*/
						iStrIndex = GetMoldTempPBString(wAssign, 1);
						break;
					case	0x211:
						iStrIndex = GetMoldTempPBString(wAssign, 2);
						break;
					case	0x212:
						iStrIndex = GetMoldTempPBString(wAssign, 3);
						break;
					case	0x213:
						iStrIndex = GetMoldTempPBString(wAssign, 4);
						break;
					case	0x214:
						iStrIndex = GetMoldTempPBString(wAssign, 5);
						break;
					case	0x215:
						iStrIndex = GetMoldTempPBString(wAssign, 6);
						break;
					case	0x216:
						iStrIndex = GetMoldTempPBString(wAssign, 7);
						break;
					case	0x217:
						iStrIndex = GetMoldTempPBString(wAssign, 8);
						break;
					case	0x218:
						iStrIndex = GetMoldTempPBString(wAssign, 9);
						break;
					case	0x219:
						iStrIndex = GetMoldTempPBString(wAssign, 11);
						break;
					case	0x21A:
						iStrIndex = GetMoldTempPBString(wAssign, 12);
						break;
					case	0x21B:
						iStrIndex = GetMoldTempPBString(wAssign, 13);
						break;
					case	0x21C:
						iStrIndex = GetMoldTempPBString(wAssign, 14);
						break;
					case	0x21D:
						iStrIndex = GetMoldTempPBString(wAssign, 15);
						break;
					case	0x21E:
						iStrIndex = GetMoldTempPBString(wAssign, 16);
						break;
					case	0x21F:
						iStrIndex = GetMoldTempPBString(wAssign, 17);
						break;
					case	0x220:
						iStrIndex = GetMoldTempPBString(wAssign, 18);
						break;
					case	0x221:
						iStrIndex = GetMoldTempPBString(wAssign, 19);
						break;
					case	0x23A:
						iStrIndex = GetMoldTempPBString(wAssign, 20);
						break;
				default:					
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szPBID[wAssign/256][wAssign%256]);
					break;
				}
			}
		}	
		
	return	(WORD)iStrIndex;
}
//獲取PC字串INDEX	
WORD	CtmTableG::GetPCTableValue(WORD wAssign,BOOL* pEx)
{
	int			iStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY");
	
	long lDspLibDate;
	long lOtherCode0;
	
	if(pEx&&*pEx)
		{
			lDspLibDate = GetDBValue("CONF2_CODE_WRITE_NULL_DATA16").lValue;
			lOtherCode0 = GetDBValue("CONF2_CODE_WRITE_NULL_MCHINEDATA1").lValue;
		}
	else
		{
			lDspLibDate = GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
			lOtherCode0 = GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;			
		}	

	if(lDspLibDate >= 0x7B27 && lOtherCode0 >= 2007 || lOtherCode0 >= 2008)	wAssign &= 0x03FF;
	else																							wAssign &= 0x01FF;
	
	if(wAssign < 0x0400)
		iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szPCID[wAssign/256][wAssign%256]);
	
	return	(WORD)iStrIndex;	
}
//獲取動作狀態字串INDEX
WORD	CtmTableG::GetStateTableValue(WORD wAssign)
{
	int			iStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY");
	WORD		wSubStep1	= (WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERSUBSTEP1").lValue;
	//WORD		wSubStep2	= (WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERSUBSTEP2").lValue;
	
	//wAssign -= 1; //變成INDEX
	if(wAssign < 0x0050)
		{
			switch(wAssign)
			{
				case	3: //射出
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszInjectStepID[(wSubStep1>>8)&0x000F]);
					break;
				case	5: //儲料
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszChargeStepID[(wSubStep1>>12)&0x000F]);
					break;
				case	10://中子進
					{
						char	pszDBID[128] = "\0";
						if(wSubStep1&0x000F)
							{
								sprintf(pszDBID,"MSET_COR%c_IN_NULL_MODEOPTION",64 + (wSubStep1&0x000F));
								if(GetDBValue(pszDBID).lValue == 2)
									iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszScrewInGroupID[wSubStep1&0x000F]);
								else
									iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszCoreInGroupID[wSubStep1&0x000F]);	
							}	
						else
							iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszCoreInGroupID[wSubStep1&0x000F]);
					}	
					break;
				case	11://中子退
					{
						char	pszDBID[128] = "\0";
						if(wSubStep1&0x000F)
							{
								sprintf(pszDBID,"MSET_COR%c_IN_NULL_MODEOPTION",64 + (wSubStep1&0x000F));
								if(GetDBValue(pszDBID).lValue == 2)
									iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszScrewOutGroupID[wSubStep1&0x000F]);
								else
									iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszCoreOutGroupID[wSubStep1&0x000F]);	
							}	
						else
							iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszCoreOutGroupID[wSubStep1&0x000F]);
					}	
					break;
				case	14://保壓
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszHoldStepID[(wSubStep1>>8)&0x000F]);
					break;
				default:					
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szStateID[wAssign%256]);
					break;
			}	
		}	
	
	return	(WORD)iStrIndex;		
}
//獲取警報狀態字串INDEX
WORD	CtmTableG::GetErrotTableValue(int wAssign,int* pResult,void* pEx)
{
	int			iStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY");
	WORD		wNum;
	WORD		wNowNum		= (WORD)GetDBValue("STATE_ALARM_ON_STEP15_ERRORDATA").lValue;
	WORD		wOldNum		= ((tmDB_EFLAG*)pEx)->wErrorFlag[14];
	BOOL		bUseOld		= FALSE;
	*pResult = 0;//Andy 20090602 initial;
	WORD	wDSPLIBDaysCode=GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD	wDSPLIBYearCode=GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;

	//新增ERROR2處理方式 //JOYCE2010-7-9 
if(wAssign&0x10000000)
{	
		WORD	wError2 = (wAssign&0x0FFF);
		if(wError2 >= 0x0100)	
			{
				GetMoldTempType();  /*JOYCE2013-10-21*/

				switch(wError2) //獲取警報字串ID
				{
					case	0x3C0:      /*JOYCE2013-10-21 COM1料溫作模溫, 加熱X區斷線顯示為模溫Y區斷線*/
						iStrIndex = GetMoldTempErrorString(wError2, 1);
						break;
					case	0x3C1:
						iStrIndex = GetMoldTempErrorString(wError2, 2);
						break;
					case	0x3C2:
						iStrIndex = GetMoldTempErrorString(wError2, 3);
						break;
					case	0x3C3:
						iStrIndex = GetMoldTempErrorString(wError2, 4);
						break;
					case	0x3C4:
						iStrIndex = GetMoldTempErrorString(wError2, 5);
						break;
					case	0x3C5:
						iStrIndex = GetMoldTempErrorString(wError2, 6);
						break;
					case	0x3C6:
						iStrIndex = GetMoldTempErrorString(wError2, 7);
						break;
					case	0x3C7:
						iStrIndex = GetMoldTempErrorString(wError2, 8);
						break;
					case	0x3C8:
						iStrIndex = GetMoldTempErrorString(wError2, 9);
						break;
					case	0x3C9:
						iStrIndex = GetMoldTempErrorString(wError2, 11);
						break;
					case	0x3CA:
						iStrIndex = GetMoldTempErrorString(wError2, 12);
						break;
					case	0x3CB:
						iStrIndex = GetMoldTempErrorString(wError2, 13);
						break;
					case	0x3CC:
						iStrIndex = GetMoldTempErrorString(wError2, 14);
						break;
					case	0x3CD:
						iStrIndex = GetMoldTempErrorString(wError2, 15);
						break;
					case	0x3CE:
						iStrIndex = GetMoldTempErrorString(wError2, 16);
						break;
					case	0x3CF:
						iStrIndex = GetMoldTempErrorString(wError2, 17);
						break;
					case	0x3D0:
						iStrIndex = GetMoldTempErrorString(wError2, 18);
						break;
					case	0x3D1:
						iStrIndex = GetMoldTempErrorString(wError2, 19);
						break;
					case	0x3D5:
						iStrIndex = GetMoldTempErrorString(wError2, 20);
						break;
					default:				
						iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szErrorID[wError2/256][wError2%256]);
						break;
				}
				
				*pResult = m_ErrorValue[wError2/256][wError2%256]; //獲取警報處理結果：警報級別10最高，0最低
				//printf("ooResult: %d,%d\n",*pResult,wAssign);
				
				return	(WORD)iStrIndex;	
			}
}	
/////////////
else //STAT.ERROR
{	
	if(wAssign&0x0100)	
		{
			bUseOld		= TRUE;
			wAssign	-= 0x0100;
		}
	//wAssign -= 1; //變成INDEX
	//printf("Assign: %X\n",wAssign);
	if(wAssign < 0x0100)
		{
			switch(wAssign) //獲取警報字串ID
			{
				case	0:
					if(((GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue & 0x0008) && ((wDSPLIBDaysCode<0x4401)&&(wDSPLIBYearCode==0x2014)||(wDSPLIBYearCode<=0x2013))) /*JOYCE2014-4-2 模溫偏差,主機在20140401開始直接使用ERB_TMLD*/
						 || g_pExTable->GetTableValue(0,"D_TP2MTP"))  /* 無料溫, 料溫作模溫*/
						iStrIndex	=	g_MultiLanguage.GetStrKeyIndex("ERR_B6_ALARMC06");
					else
						iStrIndex	=	g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR");
					break;
				case	1:  //安全門未關
					if(GetDBValue("STATE_ALARM_ON_STEP1_ERRORDATA").lValue&0x0002)//現在有，代表過去沒有,副條件用現在的
						wNum	= wNowNum;
					else//過去有，但現在不一定有,副條件用過去的
						wNum	= wOldNum;
					if(bUseOld)	wNum	= wOldNum;
					if(wNum&0x0020)	*pResult = m_ErrorValue[0][wAssign]; //不記錄
					else	*pResult = 10;//記錄
						
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szErrorID[0][wAssign%256]);
					return	(WORD)iStrIndex;	
					break;
				case	27: //中子未到定位 ERR_14_COREPULLENDERR	
					if(GetDBValue("STATE_ALARM_ON_STEP2_ERRORDATA").lValue&0x0800)//現在有，代表過去沒有,副條件用現在的
						wNum	= wNowNum;
					else//過去有，但現在不一定有,副條件用過去的
						wNum	= wOldNum;
					if(bUseOld)	wNum	= wOldNum;
					cout << "wNum=" << wNum << endl;
					if(wNum&0x10)
						iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszCorePullErr[(wNum&0x000F) + 12]);
					else
						iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszCorePullErr[wNum&0x000F]);
					break;
				case	35: //中子動作位置偏 ERR_2C_CPACTPOSERR
					if(GetDBValue("STATE_ALARM_ON_STEP3_ERRORDATA").lValue&0x0008)//現在有，代表過去沒有,副條件用現在的
						wNum	= wNowNum;
					else//過去有，但現在不一定有,副條件用過去的
						wNum	= wOldNum;
					if(bUseOld)	wNum	= wOldNum;
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszCoreActPosErr[wNum&0x000F]);
					break;
				case	41: //等待機械手 ERR_26_WAITROBOT
					if(GetDBValue("STATE_ALARM_ON_STEP3_ERRORDATA").lValue&0x0200)//現在有，代表過去沒有,副條件用現在的
						wNum	= wNowNum;
					else//過去有，但現在不一定有,副條件用過去的
						wNum	= wOldNum;
					if(bUseOld)	wNum	= wOldNum;
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(pszRobotErr[wNum&0x000F]);
					break;
				case	62:  //安全門未關
					if(GetDBValue("STATE_ALARM_ON_STEP4_ERRORDATA").lValue&0x4000)//現在有，代表過去沒有,副條件用現在的
						wNum	= wNowNum;
					else//過去有，但現在不一定有,副條件用過去的
						wNum	= wOldNum;
					if(bUseOld)	wNum	= wOldNum;
					if(wNum&0x0020)	*pResult = m_ErrorValue[0][wAssign]; //不記錄
					else	*pResult = 10;//記錄
						
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szErrorID[0][wAssign%256]);
					return	(WORD)iStrIndex;	
					break;
				default:				
					iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szErrorID[0][wAssign%256]);
					break;
			}
			
			*pResult = m_ErrorValue[0][wAssign]; //獲取警報處理結果：警報級別10最高，0最低
			//printf("ooResult: %d\n",*pResult);
			/*
				case	10:
					pResult
					break;
				case	9:
					break;
				case	8:
					break;
				case	7:
					break;
				case	6:
					break;
				case	5:
					break;
				case	4:
					break;
				case	3:
					break;
				case	2:
					break;
				case	1:
					break;
				case	0:
					break;																																																		
				default:
					break;	
			*/
		}	
	return	(WORD)iStrIndex;	
}	
}	

WORD	CtmTableG::GetViewTableName(int* pViewID,char* ViewName)
{
	int	index = *pViewID;
	if(index == -2) return 0;
		
	for(int i=0;i<CNT_INT_MAX_TABLE_NUM;i++)
	{
		if(m_ViewID[i] == -1)	break;
		if((index&0x0000000F) == 0x00000001)	index	= 0x00010001;
		if((index == m_ViewID[i])||((index ^ m_ViewID[i]) == 0x10000000))
		{
			strcpy(ViewName,m_szViewName[i]);
			*pViewID	=	m_ViewID[i];
			return	1;
		}		
	}
	
	return	0;
}

BOOL	CtmTableG::GetFileName()
{
	m_TableIni.OpenFile("outerconfig.ini");
	
	m_TableIni.ReadString("table",	"PAtable", 		NULL, &m_pszFilePAID);
	m_TableIni.ReadString("table",	"PBtable",		NULL, &m_pszFilePBID);
	m_TableIni.ReadString("table",	"PCtable",		NULL, &m_pszFilePCID);	
	m_TableIni.ReadString("table",	"STtable",		NULL, &m_pszFileStateID);
	m_TableIni.ReadString("table",	"ERtable",		NULL, &m_pszFileErrorID);

	m_TableIni.ReadString("table",	"STvalue",		NULL, &m_pszFileStateValue);
	m_TableIni.ReadString("table",	"ERvalue",		NULL, &m_pszFileErrorValue);	

	m_TableIni.ReadString("keyled",	"ATvalue",		NULL, &m_pszFileActLedValue);  /*ZHBA 2014-2-18 */
	m_TableIni.ReadString("keyled",	"OperConf",	    NULL, &m_pszFileOperConf); /*ZHBA 2014-2-18 */	
    m_TableIni.ReadString("keyled",	"OperLed",	    NULL, &m_pszFileOperLed);  //20150513

	m_TableIni.ReadString("keyled",	"ExKeyID",		NULL, &m_pszFileExKeyID);
	m_TableIni.ReadString("keyled",	"ExKeyCode",	NULL, &m_pszFileExKeyCode);

	m_TableIni.ReadString("view",		"Path",				NULL, &m_pszFileViewPath);
	if(m_pszFileViewPath == NULL)
		{
			m_TableIni.ReadString("view",		"ViewID",			NULL, &m_pszFileViewID);
			m_TableIni.ReadString("view",		"ViewName",		NULL, &m_pszFileViewNID);
		}
	else
		{
			char*	pszVersion=NULL;
			
			g_StateIni.OpenFile("stateconfig.ini");
			g_StateIni.ReadString("version","version", NULL,&pszVersion);
			g_StateIni.CloseFile();	
			m_pszFileViewID = new char[128];
			m_pszFileViewNID= new char[128];
			sprintf(m_pszFileViewID,	"%sViewID_%s",		m_pszFileViewPath,pszVersion);
			sprintf(m_pszFileViewNID,	"%sViewName_%s",	m_pszFileViewPath,pszVersion);
		}	

	m_TableIni.CloseFile();

	return TRUE;
}

void	CtmTableG::Init()
{
	CtmWnd* pTemp = NULL;
	int k = 0;
	BOOL  bResult = 0; 
	
	//讀取PB2PA外掛
	if (m_pszFilePAID 		!= NULL) 	
		{
			bResult = m_ReadData.OpenFileStr(m_pszFilePAID);
			m_ReadData.ReadData(m_szPAID);
		}	
	
	//讀取PB外掛
	if (m_pszFilePBID 		!= NULL) 	
		{
			bResult = m_ReadData.OpenFileStr(m_pszFilePBID);
	
			for(int i = 0; i < m_ReadData.GetControlStr()->GetControlCount(); i++) //GetControlCount()獲取元件數目
			{
				pTemp = m_ReadData.GetControlStr()->Controls[i]; //獲取元件指針
  		
				for (int j = 0; j < 16; j++)
				{
					char	psz[64];
					sprintf(psz,"value%d",j);
					if(pTemp != NULL)
					pTemp->GetPropValueT(psz, m_szPBID[k/256][k%256], 128);
					if (m_szPBID[k/256][k%256] == '\0') break;
					else k++;
				}
			}
		}
	//讀取PC外掛
	if (m_pszFilePCID 		!= NULL) 	
		{
			m_ReadData.OpenFileStr(m_pszFilePCID);
			k = 0;
			for(int i = 0; i < m_ReadData.GetControlStr()->GetControlCount(); i++) //GetControlCount()獲取元件數目
			{
				pTemp = m_ReadData.GetControlStr()->Controls[i]; //獲取元件指針
				for (int j = 0; j < 16; j++)
				{
					char	psz[64];
					sprintf(psz,"value%d",j);
					pTemp->GetPropValueT(psz, m_szPCID[k/256][k%256], 128);
					if (m_szPCID[k/256][k%256] == '\0') break;
					else k++;
				}
			}
		}
		
	//讀取STATE外掛
	if (m_pszFileStateID 		!= NULL) 	
		{
			bResult = m_ReadData.OpenFileStr(m_pszFileStateID);
			m_ReadData.ReadData(m_szStateID);
		}	
		
	if (m_pszFileStateValue 		!= NULL) 	
		{
			bResult = m_ReadData.OpenFileInt(m_pszFileStateValue);
			m_ReadData.ReadData(&m_StateValue[0]);
		}		
	//讀取ERROR外掛
	if (m_pszFileErrorID 		!= NULL) 	
		{
			//bResult = m_ReadData.OpenFileStr(m_pszFileErrorID);
			//m_ReadData.ReadData(m_szErrorID);
			m_ReadData.OpenFileStr(m_pszFileErrorID);
			k = 0;
			for(int i = 0; i < m_ReadData.GetControlStr()->GetControlCount(); i++) //GetControlCount()獲取元件數目
			{
				pTemp = m_ReadData.GetControlStr()->Controls[i]; //獲取元件指針
				for (int j = 0; j < 256; j++)
				{
					char	psz[64];
					sprintf(psz,"value%d",j);
					pTemp->GetPropValueT(psz, m_szErrorID[k/256][k%256], 128);
					if (m_szErrorID[k/256][k%256] == '\0') break;
					else k++;
				}
			}			
		}	

	if (m_pszFileErrorValue 		!= NULL) 	
		{
			//bResult = m_ReadData.OpenFileInt(m_pszFileErrorValue);
			//m_ReadData.ReadData(&m_ErrorValue[0]);
			m_ReadData.OpenFileStr(m_pszFileErrorValue);
			for(int i = 0; i < m_ReadData.GetControlStr()->GetControlCount(); i++) //GetControlCount()獲取元件數目
			{
				pTemp = m_ReadData.GetControlStr()->Controls[i]; //獲取元件指針
				for (int j = 0; j < 256; j++)
				{
					char	psz[64];
					sprintf(psz,"value%d",j);
					pTemp->GetPropValueT(psz, &m_ErrorValue[i][j], sizeof(int));
					//if (&m_ErrorValue[k/256][k] == '\0') break;
					//else k++;
				}
			}				
		}				

	if (m_pszFileActLedValue 	!= NULL) 	 /*ZHBA 2014-2-18 */
	{
		bResult = m_ReadData.OpenFileInt(m_pszFileActLedValue);
		m_ReadData.ReadData(&m_ActLedValue[0]);
	}		
	if (m_pszFileOperConf 	!= NULL) 	 /*ZHBA 2014-2-18 */
	{
		bResult = m_ReadData.OpenFileInt(m_pszFileOperConf);
		m_ReadData.ReadData(&m_OperConf[0]);
	}
	if (m_pszFileOperLed 	!= NULL)  //20150513
	{
		bResult = m_ReadData.OpenFileInt(m_pszFileOperLed);
		m_ReadData.ReadData(&m_OperLed[0]);
	}


	//Read SpareKeyID 2008/11/04 7:25PM
	if (m_pszFileExKeyID		!= NULL)
	{
		m_ReadData.OpenFileStr(m_pszFileExKeyID);
		m_ReadData.ReadData(m_szExKeyID);
	}
	//獲取按鍵交換值外掛
	if (m_pszFileExKeyCode		!= NULL)
	{
		m_ReadData.OpenFileInt(m_pszFileExKeyCode);
		for(int i = 0; i < m_ReadData.GetControlInt()->GetControlCount(); i++) //GetControlCount()獲取元件數目
		{
			pTemp = m_ReadData.GetControlInt()->Controls[i]; //獲取元件指針
			if(pTemp != NULL)
			{
				//獲取按鍵交換值
				for (int j = 0; j < 10; j++)
				{
					char	psz[64];
					sprintf(psz,"value%d",j);
					pTemp->GetPropValueT(psz, &m_nExKeyValue[i][j], sizeof(int));
					if(0 == m_nExKeyValue[i][j])
						break;
				}
			}
		}
	}		
	//讀取畫面名稱外掛
	if (m_pszFileViewID 		!= NULL) 	
		{
			bResult = m_ReadData.OpenFileInt(m_pszFileViewID);
			m_ReadData.ReadData(&m_ViewID[0]);
		}	

	if (m_pszFileViewNID 		!= NULL) 	
		{
			bResult = m_ReadData.OpenFileStr(m_pszFileViewNID);
			m_ReadData.ReadData(m_szViewName);
		}	
	//獲取stateconfig.ini中在define
	char	sz_def[16];
	memset(sz_def,0,sizeof(sz_def));
	g_StateIni.OpenFile("stateconfig.ini");
	for(int i=0;i<CNT_INT_MAX_TABLE_NUM;i++)
	{
		sprintf(sz_def,"Define%d",i);
		g_StateIni.ReadString("define", sz_def, NULL,&m_szFuncDefine[i]);
		if(m_szFuncDefine[i] == NULL)	break;
	}	
	g_StateIni.CloseFile();
}

void	CtmTableG::GetMoldTempType()  /*JOYCE2013-10-21*/
{
	char	psz[5];
	GetDBString("VERS_HMI_NULL_NULL_PROGRAMRESVERS",psz,4);//判斷模溫標識
	switch(psz[2])
	{
		case	'1'://D_MDT10  com1, channel 11-20 
			for(int i=0; i<10; i++)
				m_wMldTmpIndex[i+10] = i+1;
			break;
		case	'2':
			break;
		case	'3'://D_MDT4 	 com1, channel 6-9
			for(int i=0; i<4; i++)
				m_wMldTmpIndex[i+5] = i+1;
			break;
		case	'4'://D_T3M6  com1, channel 4-9
			for(int i=0; i<6; i++)
				m_wMldTmpIndex[i+3] = i+1;
			break;
		case	'5'://D_T3M5  com1, channel 4-8
			for(int i=0; i<5; i++)
				m_wMldTmpIndex[i+3] = i+1;
			break;
		case	'6':
			break;
		case	'7':
			break;
		case	'8':
			break;
		case	'9'://D_T7M2  com1, channel 8-9
		case	'E'://COM1 2段模溫(CH8~9),40段COM2模溫
			for(int i=0; i<2; i++)
				m_wMldTmpIndex[i+7] = i+1;
			break;
		case	'A':
			break;
		case	'B':
			break;
		case	'C':
			break;
		case	'D'://D_T5M14   com1, channel 6-9&&11-20
			for(int i=0; i<14; i++)
			{
				if(i < 4)
					m_wMldTmpIndex[i+5] = i+1;
				else	
					m_wMldTmpIndex[i+6] = i+1;
			}
			break;
		case	'F'://D_T5M3    com1, channel 6-8
			for(int i=0; i<3; i++)
				m_wMldTmpIndex[i+5] = i+1;
			break;
		case	'H'://D_T12M4   com1, channel 7-8&&17-18
			for(int i=0; i<4; i++)
			{
				if(i < 2)
					m_wMldTmpIndex[i+6] = i+1;
				else	
					m_wMldTmpIndex[i+14] = i+1;
			}
			break;
		case	'I'://D_T14M4   com1, channel 8-9&&18-19
			for(int i=0; i<4; i++)
			{
				if(i < 2)
					m_wMldTmpIndex[i+7] = i+1;
				else	
					m_wMldTmpIndex[i+15] = i+1;
			}
			break;
		case	'J'://com1, channel 4-9&&11-20
			for(int i=0; i<16; i++)
			{
				if(i < 6)
					m_wMldTmpIndex[i+3] = i+1;
				else	
					m_wMldTmpIndex[i+4] = i+1;
			}
			break;
		case	'K'://com1, channel 1-8
			for(int i=0; i<8; i++)
				m_wMldTmpIndex[i] = i+1;
			break;
		case	'L'://D_T5M2    com1, channel 6-7
			for(int i=0; i<2; i++)
				m_wMldTmpIndex[i+5] = i+1;
			break;
		case	'M'://D_T6M2    com1, channel 7-8
			for(int i=0; i<2; i++)
				m_wMldTmpIndex[i+6] = i+1;
			break;
		case	'P'://wangli2014-12-3 9:41:10 T11M4    com1, channel 13-16
			for(int i=0; i<4; i++)
				m_wMldTmpIndex[i+12] = i+1;
			break;
		case	'Q':/*wangli2015-6-1 13:30:54 T16M2 com1, channel 8,18*/
			for(int i=0; i<2; i++)
			{
				if(i < 1)
					m_wMldTmpIndex[i+7] = i+1;
				else	
					m_wMldTmpIndex[i+16] = i+1;
			}
			break;
		default:
			for(int i=0; i<20; i++)
				m_wMldTmpIndex[i] = 0;
			break;	
	}		
}

WORD	CtmTableG::GetMoldTempErrorString(WORD wError2, int nMoldTempIndex)  /*JOYCE2013-10-21*/
{
	char		m_szMldTmp[128];
	BOOL		m_bMldTmp = FALSE;
	int     iStrIndex = 0;
	
	for(int i=0; i<20; i++)
	{
		//printf("wError2=%4x, %d m_wMldTmpIndex[%d]=%d\n",wError2,i,nMoldTempIndex-1,m_wMldTmpIndex[nMoldTempIndex-1]);
		if(m_wMldTmpIndex[nMoldTempIndex-1] == i+1)
		{	
			sprintf(m_szMldTmp, "MOLDTEMPBREAK_%d", i+1);
			m_bMldTmp = TRUE;
			break;
		}
	}

	if(m_bMldTmp)
		iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szMldTmp);
	else	
		iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szErrorID[wError2/256][wError2%256]);

	return iStrIndex;			
}

WORD	CtmTableG::GetMoldTempPBString(WORD wAssign, int nMoldTempIndex)  /*JOYCE2013-10-21*/
{
	char		m_szMldTmp[128];
	BOOL		m_bMldTmp = FALSE;
	int     iStrIndex = 0;
	
	for(int i=0; i<20; i++)
	{
		//printf("wAssign=%4x, %d m_wMldTmpIndex[%d]=%d\n",wAssign,i,nMoldTempIndex-1,m_wMldTmpIndex[nMoldTempIndex-1]);
		if(m_wMldTmpIndex[nMoldTempIndex-1] == i+1)
		{	
			sprintf(m_szMldTmp, "MOLDTEMPBREAK_%d", i+1);
			m_bMldTmp = TRUE;
			break;
		}
	}

	if(m_bMldTmp)
		iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szMldTmp);
	else	
		iStrIndex	=	g_MultiLanguage.GetStrKeyIndex(m_szPBID[wAssign/256][wAssign%256]);

	return iStrIndex;			
}

#ifndef	D_BEOBJECT_CTMTABLEG

CObject*	CreateInstance(char* Name)
{
	if (g_pTableG == NULL)
	{
		CObject*	pResult = Create(Name);
		
		if (pResult != NULL)
		{
			g_nTableCount++;
			g_pTableG = static_cast<CtmTableG*>(pResult);			
			g_pTableG->CreateSelf();	
		}		
	}
	else g_nTableCount++;
		
	//printf("g_pSelevatG=%X\n", g_pTableG);
	
	return static_cast<CObject*>(g_pTableG);
}

void	ReleaseInstance(CObject* pInstance)
{
	if (g_nTableCount > 0)
		g_nTableCount--;
	
	if (g_pTableG <= 0)
	{
		if (g_pTableG != NULL)
			g_pTableG->FreeSelf();
		delete g_pTableG;
		g_pTableG = NULL;
	}
}
#endif


