/*===========================================================================+
|  Class    : ViewReset library                                              |
|  Task     : ViewReset library  action source file                          |
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
#include    "ViewRevert.h"
#include	"../_kb.h"
#include    "../optionbox.h"
#include    "../utils.h"
#include	"../sram.h"
#include	"../_kb.h"
#include	"../sram.h"
#include 	"../qualitycontrol.h"
#include	"../freedata.h"
#include	"../recordset.h"
#include	"../checkbox.h"
#include 	"../tmFileExplorer.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		DATABASE_LIST 1
#define		PARAM_LIST 2
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//重置
CtmWnd*		pwndCheckRESETDSP		   	= NULL;
CtmWnd*		pwndCheckRESETALL		   	= NULL;
CtmWnd*		pwndCheckRESETDB				= NULL;
CtmWnd*		pwndCheckRESETHISTORY		= NULL;
CtmWnd*		pwndCheckTeachPoint			= NULL;
CtmWnd*		pwndCheckCalibration		= NULL;
CtmWnd*		pwndPassWord						= NULL;
CtmWnd*		pwndCheckFactoryMachine	= NULL;
CtmWnd*		pwndBtnCONFIRM 		   		= NULL;
CtmWnd*		pwndBtnCANCEL 		   		= NULL;

CtmWnd*		pwndExplorerDatebase		=	NULL;
CtmWnd*		pwndBtnBackupDatabase		=	NULL;
CtmWnd*		pwndBtnDeleteDatabase		=	NULL;
CtmWnd*		pwndBtnRevertDatabase		=	NULL;
CtmWnd*		pwndExplorerParam				=	NULL;
CtmWnd*		pwndBtnBackupParam			=	NULL;
CtmWnd*		pwndBtnDeleteParam			=	NULL;
CtmWnd*		pwndBtnReNameParam			=	NULL;
CtmWnd*		pwndBtnRevertParam			=	NULL;

int			m_iKey 				   	= 0;
BOOL		EditLock 					= FALSE; 
/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	/*--------------重置-------------*/
	//主機
	pwndCheckRESETDSP			= pwndSender->FindControlFromName("CheckBoxRESET_DSP");
	//面板
	pwndCheckRESETALL 		= pwndSender->FindControlFromName("CheckBoxRESET_ALL");
	pwndCheckRESETDB			= pwndSender->FindControlFromName("CheckBoxRESET_DB");
	pwndCheckRESETHISTORY	= pwndSender->FindControlFromName("CheckBoxRESET_HISTORY");
	//工廠
	pwndPassWord					= pwndSender->FindControlFromName("editPASSWORD");
	pwndCheckFactoryMachine	= pwndSender->FindControlFromName("CheckBoxRESET_FACTORMACHINE");
	//確認/取消
	pwndBtnCONFIRM				= pwndSender->FindControlFromName("buttonCONFIRM");
	pwndBtnCANCEL					= pwndSender->FindControlFromName("buttonCANCEL");
	
	pwndCheckTeachPoint		= pwndSender->FindControlFromName("CheckBoxTeachPointFile");
	pwndCheckCalibration	= pwndSender->FindControlFromName("CheckBoxCalibrationFile");
	/*------------備份還原-------------*/
	//資料庫
	/*pwndExplorerDatebase		= pwndSender->FindControlFromName("DatabaseExplorer");
	pwndBtnBackupDatabase		= pwndSender->FindControlFromName("BtnDataBackup");
	pwndBtnDeleteDatabase		= pwndSender->FindControlFromName("BtnDataDelete");
	pwndBtnRevertDatabase		= pwndSender->FindControlFromName("BtnDataRe");*/
	
	//控制參數
	pwndExplorerParam				= pwndSender->FindControlFromName("DatabaseExplorer");
	pwndBtnBackupParam			= pwndSender->FindControlFromName("BtnDataBackup");
	pwndBtnDeleteParam			= pwndSender->FindControlFromName("BtnDataDelete");
	pwndBtnReNameParam			= pwndSender->FindControlFromName("BtnDataReName");
	pwndBtnRevertParam			= pwndSender->FindControlFromName("BtnDataRe");
	
	/*pwndExplorerParam				= pwndSender->FindControlFromName("ParamExplorer");
	pwndBtnBackupParam			= pwndSender->FindControlFromName("BtnParamBackup");
	pwndBtnDeleteParam			= pwndSender->FindControlFromName("BtnParamDelete");
	pwndBtnRevertParam			= pwndSender->FindControlFromName("BtnParamRe");*/
	
	//((CtmFileExplorer*)pwndExplorerDatebase)->Opendir_File("./Data/DatabaseBackup/");
	((CtmFileExplorer*)pwndExplorerParam)->Opendir_File("./Data/ParamBackUp/");
	//((CtmFileExplorer*)pwndExplorerParam)->Opendir_File("./Data/ParamBackUp/");
	//pwndExplorerDatebase->Update();
	pwndExplorerParam->Update();
	RobotStatus = OP_EDIT_MODE;
	return TRUE;
}


WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwnd = NULL;
	BOOL	bInitialDSP = FALSE;
	BOOL	bInitialMMI = FALSE;
	BOOL 	bTemp = FALSE;
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	
	if (pwnd == pwndBtnCONFIRM && !EditLock)
	{
		MsgBox(g_MultiLanguage["ROBOT_STR_RESET_MESSAGE"], tmFT_CODE_TECH);
		if(g_bMsgBoxFlag)
		{
			if(pwndCheckRESETDSP != NULL)
				  pwndCheckRESETDSP->GetPropValueT("checked", &bTemp,sizeof(bTemp));
	  	if (bTemp == TRUE)
	  	{		
			  bInitialDSP = TRUE;
	  	  ResetDSP();		
	  	  Delay(2000); //2 second
	  	  //MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);//Andy 20071008
	 		}
	  	if(pwndCheckRESETALL != NULL)
	  	{
	  		pwndCheckRESETALL->GetPropValueT("checked", &bTemp,sizeof(bTemp));
	  		if(bTemp ==TRUE)
	  		{
	  			Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); //Andy 20071008
	  			Reset_DB(RESET_DB_ALL);
					CtmConfig::GetInstance()->SetSramInit(0);	//	fans add 2007/9/24 10:18上午
	  			#ifdef D_NOWRITE_FALSH
		  		tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
	   			if(GetFileType(CHECKFILETYPE))
	    			nStorageType = STORAGE_TYPE_FLASH;
		  		CtmSarmRecordFileControl::GetInstance()->DeleteRecord(ID_MOLDSET_SRAMFILE, nStorageType);
		  		usleep(50*1000);
		  		system("rm -rf /usr/Data/moldset/moldset.cdb");
		  		#else	
		  		CtmSarmRecordFileControl::GetInstance()->DeleteRecord(ID_MOLDSET_SRAMFILE);
		  		#endif
		  		bInitialMMI = TRUE;
		  		SetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION", TRUE, TRUE);
		  	}
	  	}
			if(pwndCheckRESETHISTORY != NULL)	//fuxy, 2008-9-1, 警報、監測、記錄等歷史資料
			{
	  		pwndCheckRESETHISTORY->GetPropValueT("checked", &bTemp,sizeof(bTemp));
				if(bTemp ==TRUE)	
				{
					g_ptmControlServer->ErrorControl()->ClearErrorLog(); 	//2008-9-28, ClearErrorLog();	//警報
					CRecordSetControl::GetInstance()->DeleteRecord();	//記錄
					CtmQualityControl::GetInstance()->Clear();	//監測資料
					bInitialMMI = TRUE;
				}			
			}
			if(pwndCheckRESETDB != NULL)
			{
				pwndCheckRESETDB->GetPropValueT("checked", &bTemp,sizeof(bTemp));
				if(bTemp ==TRUE)	
	  	  {
					g_ptmControlServer->ErrorControl()->BackupErrorLog();  //先備份警告
	  	  	Reset_DB(RESET_DB_ALL);
	  	  	//CtmConfig::GetInstance()->SetSramInit(0);
	  	  	bInitialMMI = TRUE;
					g_ptmControlServer->ErrorControl()->RevertErrorLog();	//還原警告
	  	  }
			}
			
			if(pwndCheckTeachPoint != NULL)
			{
				pwndCheckTeachPoint->GetPropValueT("checked", &bTemp,sizeof(bTemp));
				if(bTemp ==TRUE)	
	  	  {
					CreateTeachPoint();
	  	  }
			}
			
			if(pwndCheckFactoryMachine != NULL)
			{
				pwndCheckFactoryMachine->GetPropValueT("checked", &bTemp,sizeof(bTemp));
				if(bTemp ==TRUE)	
	  	  {
	  	  	tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
   				if(GetFileType(CHECKFILETYPE))
    				nStorageType = STORAGE_TYPE_FLASH;
			  	//CtmSarmRecordFileControl::GetInstance()->DeleteRecord(ID_MACHINE_FILE, nStorageType);
	  	  	//CtmSarmRecordFileControl::GetInstance()->DeleteRecord(ID_DATABLE_FILE, nStorageType);
	  	  	usleep(50*1000);
	  	  	system("rm -rf /usr/Data/machine/machine.cdb");
	  	  	usleep(50*1000);
	  	  	system("rm -rf /usr/Data/machine/machine_mach.cdb");
	  	  	usleep(50*1000);
	  	  	system("rm -rf /usr/Data/DA/da.cdb");
	  	  	usleep(50*1000);
	  	  	system("rm -rf /usr/Data/DA/da_mach.cdb");
	  	  	usleep(50*1000);
	  	  	system("rm -rf /usr/Data/systemx/systemx.cdb");
	  	  	usleep(50*1000);
	  	  	system("rm -rf /usr/Data/systemx/systemx_mach.cdb");
	  	  	Reset_DB(RESET_DB_MACHINE);
	  	  	Reset_DB(RESET_DB_DA);
	  	  	if(g_pFreeData != NULL)
	  	  		g_pFreeData->GetDefaultDataFromTable("./lib/hmi_group/SystemID");
	  	  	bInitialMMI = TRUE;
	  	  }
			}
			
			
			if(bInitialMMI == TRUE)	ResetMMI();
				
			if(bInitialMMI || bInitialDSP) 	
			{
				Prompt(g_MultiLanguage["MSG_DUMMY"],0);
				MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);//Andy 20071008 
			}	
		}	
	}
	else if(pwnd == pwndBtnCONFIRM && EditLock)
		MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
	else if (pwnd == pwndBtnCANCEL)
	{
		//((CtmFormView*)pwndSender)->OnLoseFocus();
		BOOL 	bTemp = FALSE;
		if(pwndCheckRESETALL 	!= NULL)
		{
			pwndCheckRESETALL->SetPropValueT("checked", bTemp);
			pwndCheckRESETALL->Update();
		}
		if(pwndCheckTeachPoint 	!= NULL)
		{
			pwndCheckTeachPoint->SetPropValueT("checked", bTemp);
			pwndCheckTeachPoint->Update();
		}
		if(pwndCheckRESETDSP 	!= NULL)
		{
			pwndCheckRESETDSP->SetPropValueT("checked", bTemp);
			pwndCheckRESETDSP->Update();
		}	
		if(pwndCheckRESETDB 	!= NULL)
		{
			pwndCheckRESETDB->SetPropValueT("checked", bTemp);
			pwndCheckRESETDB->Update();
		}	
		if(pwndCheckRESETHISTORY 	!= NULL)
		{
			pwndCheckRESETHISTORY->SetPropValueT("checked", bTemp);
			pwndCheckRESETHISTORY->Update();
		}	
		if(pwndCheckFactoryMachine 	!= NULL)
		{
			pwndCheckFactoryMachine->SetPropValueT("checked", bTemp);
			pwndCheckFactoryMachine->Update();
		}	
		if(pwndPassWord 	!= NULL)
		{
			pwndPassWord->SetPropValueT("text", "");
			pwndPassWord->Update();
			pwndCheckFactoryMachine->SetPropValueT("taborder",-2);
			pwndCheckFactoryMachine->Update();
		}	
		((CtmFormView*)pwndSender)->Goto(0);
	}
	if(pwnd == pwndBtnBackupParam)
		BackUpParam();
	else if(pwnd == pwndBtnDeleteParam)
	{
		int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
		CtmWnd*	pwndTemp	= NULL;
		pwndTemp = pwndSender->FindControlFromTab(ViewTabStop);
		if(pwndTemp == pwndExplorerParam)
		{
			MsgBox(g_MultiLanguage["ROBOT_MSG_FILE_DEL_SURE"], tmFT_CODE_TECH);
			if(g_bMsgBoxFlag)
			{
				char ptr[128]; 
				((CtmFileExplorer*)pwndExplorerParam)->GetCurrentFilePath(ptr);
				char Command[128];
				sprintf(Command,"rm -r %s",ptr);
				system(Command);
				Delay(2000);
				((CtmFileExplorer*)pwndExplorerParam)->Opendir_File("./Data/ParamBackUp/");
				pwndExplorerParam->Update();
			}
		}
	}
	else if(pwnd == pwndBtnRevertParam )
	{
		if(EditLock)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			return	wIDControl;
		}
		else
		{
			int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
			CtmWnd*	pwndTemp	= NULL;
			pwndTemp = pwndSender->FindControlFromTab(ViewTabStop);
			if(pwndTemp == pwndExplorerParam)
			{
				MsgBox(g_MultiLanguage["ROBOT_STR_REVERT_PARAM_MESSAGE"], tmFT_CODE_TECH);
				if(g_bMsgBoxFlag)
				{
					char ptr[128]; 
					((CtmFileExplorer*)pwndExplorerParam)->GetCurrentFilePath(ptr);
					ReParam(ptr);	
					MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3); 
				}
			}
		}
	}
	else if(pwnd == pwndBtnReNameParam)
	{
		int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
		CtmWnd*	pwndTemp	= NULL;
		pwndTemp = pwndSender->FindControlFromTab(ViewTabStop);
		if(pwndTemp == pwndExplorerParam)
		{
			//更改檔名 g_Now_FileName
			memset(g_Now_FileName,'\0',sizeof(g_Now_FileName));
			MsgBoxCall("msgboxFileReName.txt",0);
			if(g_Now_FileName[0]!='\0')
			{
				char FileName1[128];	//被更改檔名
				char FileName2[128];	//新檔名
				char cmd[256];				//命令
				memset(FileName1,0,sizeof(FileName1));
				memset(FileName2,0,sizeof(FileName2));
				memset(cmd,0,sizeof(cmd));
				
				sprintf(FileName2,"./Data/ParamBackUp/%s",g_Now_FileName);
				//printf("FileName2=%s\n",FileName2);
				if(file_exists(FileName2))	//新檔名已存在
				{
					MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_THESAME_FILENAME");
					return wIDControl; 
				}
				else	g_bMsgBoxFlag=TRUE;
				memset(g_Now_FileName,'\0',sizeof(g_Now_FileName));
				if(g_bMsgBoxFlag)
				{
					g_bMsgBoxFlag=FALSE;
					((CtmFileExplorer*)pwndExplorerParam)->GetCurrentFilePath(FileName1);
					sprintf(cmd,"mv %s %s",FileName1,FileName2);
					system(cmd);
					system("sync");
					((CtmFileExplorer*)pwndExplorerParam)->Opendir_File("./Data/ParamBackUp/");
					pwndExplorerParam->Update();
				}
			}
		}
	}
  return wIDControl;
}


WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)	
{
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
	/*
	if(wKey == _KEY_TEXTINPUT)
	{
		int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
		CtmWnd*	pwndTemp	= NULL;
		pwndTemp = pwndSender->FindControlFromTab(ViewTabStop);
		SetPanelGroup(pwndTemp);
	}*/
	return wKey;
}


WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwnd = NULL;
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	char	cszValue[256] = "\0";
	if(pwnd == pwndPassWord)
	{
		pwndPassWord->GetPropValueT("text", cszValue, sizeof(cszValue));
		if(0 == strcmp("1234", cszValue))
		{
			pwndCheckFactoryMachine->SetPropValueT("taborder", 5);
			pwndCheckFactoryMachine->Update();
		}
		else
		{
			pwndCheckFactoryMachine->SetPropValueT("taborder",-2);
			pwndCheckFactoryMachine->Update();
		}
	}	
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	if(g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MEMORY_MODE && !EditLock) 
		EditLock = TRUE;
	else if(g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE && EditLock)
		EditLock = FALSE;
}

void    ResetMMI()
{
	
	//Init_Mold();
	//Init_Config();
	  //MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3); //Andy 20071008 MSG_DISK_PROCESSING
	  //Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1);
    //Reset(); //Andy test 20071008
}

void    ResetDSP()
{
	BOOL 	bTemp = TRUE;
	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1);
	SetDBValue("CONF_CODE_WRITE_NULL_DATA14",0);
	SetDBValue("CONF_CODE_WRITE_NULL_DATA7",1);
//	g_ptaskrs232->SendConfig();
	if(g_ptaskdsp != NULL)
		g_ptaskdsp->WriteValue(COMM_SENDCONFIG);	//2008-9-28
	if(!bTemp)
	{
		CtmBlockControl::GetInstance()->SetBlockIndex(BLOCK_NEWPOWERMATCH_ID);
		CtmBlockControl::GetInstance()->Read(STORAGE_TYPE_FLASH);
	}
	if(!bTemp)
	{
		CtmBlockControl::GetInstance()->SetBlockIndex(BLOCK_ZSET_ID);
		CtmBlockControl::GetInstance()->Read(STORAGE_TYPE_FLASH);
	}
}

/*
void  SetPanelGroup(CtmWnd*	pwndTemp)
{
	BOOL	bTemp = FALSE;
	if(pwndTemp == pwndCheckRESETALL)
	{
		if(((CtmCheckBox *)pwndCheckRESETALL)->GetCheck())
			bTemp = TRUE;
		else
			bTemp = FALSE;
		pwndCheckRESETDB->SetPropValueT("checked", bTemp);
		pwndCheckRESETHISTORY->SetPropValueT("checked", bTemp);
		pwndCheckRESETDB->Update();
		pwndCheckRESETHISTORY->Update();
	}
	else if(pwndTemp == pwndCheckRESETDB || pwndTemp == pwndCheckRESETHISTORY)
	{
		if(((CtmCheckBox*)pwndCheckRESETDB)->GetCheck() && ((CtmCheckBox*)pwndCheckRESETHISTORY)->GetCheck())
			bTemp = TRUE;
		else
			bTemp = FALSE;
		pwndCheckRESETALL->SetPropValueT("checked", bTemp);
		pwndCheckRESETALL->Update();
	}
	
}
*/

void	BackUpParam()
{
	fstream  paramfile;
	
	char ParamBackupName[32];
	char ParamBackupPath[256];
	time_t t = time(0);
	
	strftime( ParamBackupName, sizeof(ParamBackupName), "PA_%04Y%02m%02d_%02H%02M%02S",localtime(&t));
	sprintf(ParamBackupPath,"Data/ParamBackUp/%s",ParamBackupName);
	
	paramfile.open(ParamBackupPath, ios::out); 	
	char  paramstr[32];
	for(int	servoNum = 0 ; servoNum < MAX_SERVO_NO ; servoNum++)
	{
		sprintf(paramstr,"SERVO_PARAM%d = %d ",servoNum,g_TxCNCData.shmCNCData.PARAM.servo_param[servoNum]);
		paramfile<<paramstr;
	}
	for(int	tuneNum = 0 ; tuneNum < MAX_TUNE_NO ; tuneNum++)
	{
		sprintf(paramstr,"TUNE_PARAM%d = %d ",tuneNum,g_TxCNCData.shmCNCData.PARAM.tune_param[tuneNum]);
		paramfile<<paramstr;
	}
	for(int	makerNum = 0 ; makerNum < MAX_MAKER_NO ; makerNum++)
	{
		sprintf(paramstr,"MAKER_PARAM%d = %d ",makerNum,g_TxCNCData.shmCNCData.PARAM.maker_param[makerNum]);
		paramfile<<paramstr;
	}
	for(int	userNum = 0 ; userNum < MAX_USER_NO ; userNum++)
	{
		sprintf(paramstr,"USER_PARAM%d = %d ",userNum,g_TxCNCData.shmCNCData.PARAM.user_param[userNum]);
		paramfile<<paramstr;
	}
	paramfile.close();
	Delay(2000);
	((CtmFileExplorer*)pwndExplorerParam)->Opendir_File("./Data/ParamBackUp/");
	pwndExplorerParam->Update();
}

void	ReParam(char* ParamRevertPath)
{
	//printf("ParamRevertPath = %s\n",ParamRevertPath);
	fstream  paramfile;
	char  line[15];
	char  line1[15];
	int		ParamValue = 0;
	paramfile.open(ParamRevertPath,ios::in);
	if(!paramfile.is_open())
		return ;
  while(paramfile.getline(line,sizeof(line),' '))
  {
  	//printf("1.line = %s\n",line);
  	paramfile.getline(line1,sizeof(line1),' ');
  	paramfile.getline(line1,sizeof(line1),' ');
  	//printf("2.line1 = %s\n",line1);
  	ParamValue = atoi(line1);
  	SetDBValue(line , (long)ParamValue);
  }
}

void	BackUpDatabase()
{
	//SaveParaToFile();
	/*char DatabaseBackupName[32];
	char SystemCommand[256];
	time_t t = time(0);
	
	strftime( DatabaseBackupName, sizeof(DatabaseBackupName), "DB_%04Y%02m%02d_%02H%02M%02S",localtime(&t));
	sprintf(SystemCommand,"cp /dev/sram0 Data/DatabaseBackup/%s",DatabaseBackupName);
	//printf("SystemCommand is %s\n",SystemCommand);
	system(SystemCommand);
	Delay(2000);
	((CtmFileExplorer*)pwndExplorerDatebase)->Opendir_File("./Data/DatabaseBackup/");
	pwndExplorerDatebase->Update();*/
}

BOOL file_exists (char * fileName)
{
  struct stat buf;
	int i = stat ( fileName, &buf );
  /* File found */
  if ( i == 0 )
    return TRUE;
  return FALSE;
} 