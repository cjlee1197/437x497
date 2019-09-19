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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*============================================================================+
|           Constant                                                     			|
+============================================================================*/

#define  UPDATEYEAR 2018
#define  UPDATEMONTH 03
#define  UPDATEDAY 05
#define  UPDATEVERSION 1

#define  BUFFER_SIZE 256
#define  INFO_TOTAL_NUM 32
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

CtmWnd*		u_pwndEditMachineNO	=	NULL; // 機台編號

CtmWnd*		u_pwndImgMachinePic					= NULL; // Machine Picture
CtmWnd*		u_pwndStaticMachTable01	    = NULL; // Machine type
CtmWnd*		u_pwndStaticMachTable02			= NULL; // Machine load
CtmWnd*		u_pwndStaticMachTable03			= NULL; // Machine J1
CtmWnd*		u_pwndStaticMachTable04			= NULL; // Machine J2
CtmWnd*		u_pwndStaticMachTable05			= NULL; // Machine J3
CtmWnd*		u_pwndStaticMachTable06			= NULL; // Machine J4
CtmWnd*		u_pwndStaticMachTable07			= NULL; // Machine J5
CtmWnd*		u_pwndStaticMachTable08			= NULL; // Machine J6
CtmWnd*		u_pwndImgAgentsPic					= NULL; // Agents Picture
CtmWnd*		u_pwndStaticComTable01			= NULL; // Agents TEL
CtmWnd*		u_pwndStaticComTable02			= NULL; // Agents FAX
CtmWnd*		u_pwndStaticComTable03			= NULL; // Agents ADD

char data_record[INFO_TOTAL_NUM][256];
//char MotionData[9][256] = {{"[ImgMachinePic]"}, {"[MachineType]"}, {"[Load]"}, {"[J1_range]"}, {"[J2_range]"}, {"[J3_range]"}, {"[J4_range]"}, {"[J5_range]"}, {"[J6_range]"}};
//char CompanyData[4][256] = {{"[ImgAgentsPic]"}, {"[AgentsTEL]"}, {"[AgentsFAX]"}, {"[AgentsADD]"}};

/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndEditMachineNO					= pwndSender->FindControlFromName("EditMachineNO");
	//取得各元件指針
	u_pwndImgMachinePic					= pwndSender->FindControlFromName("ImgMachinePic");
	u_pwndStaticMachTable01 		= pwndSender->FindControlFromName("StaticMachTable01");
	u_pwndStaticMachTable02 		= pwndSender->FindControlFromName("StaticMachTable02");
	u_pwndStaticMachTable03 		= pwndSender->FindControlFromName("StaticMachTable03");
	u_pwndStaticMachTable04 		= pwndSender->FindControlFromName("StaticMachTable04");
	u_pwndStaticMachTable05 		= pwndSender->FindControlFromName("StaticMachTable05");
	u_pwndStaticMachTable06 		= pwndSender->FindControlFromName("StaticMachTable06");
	u_pwndStaticMachTable07 		= pwndSender->FindControlFromName("StaticMachTable07");
	u_pwndStaticMachTable08 		= pwndSender->FindControlFromName("StaticMachTable08");
	u_pwndImgAgentsPic					= pwndSender->FindControlFromName("ImgAgentsPic");
	u_pwndStaticComTable01			= pwndSender->FindControlFromName("StaticComTable01");
	u_pwndStaticComTable02			= pwndSender->FindControlFromName("StaticComTable02");
	u_pwndStaticComTable03			= pwndSender->FindControlFromName("StaticComTable03");
	
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
	//取得檔案中手臂機型和公司資訊的資料
	GetInfoData();
	//手臂機型
	MotionDataShow();
	//版本
	VersionShow();
	//機台資訊
	MachineShow();
	//公司資訊
	CompanyShow();
	
}

void GetInfoData()
{	
	char data[256];
	char buffer[BUFFER_SIZE];  
  char *line;   
  int i = 0;
  //char *CoordEmpty[ROW][6];
	char file_path[256];
	sprintf(file_path, "%s", "Data/information");
	
	FILE *fp = NULL;		
	fp = fopen(file_path, "r");	
	
	if(fp == NULL)
	{		
	  printf("No File.");
	  //return 0;
	}	
	else
	{
		//memset(info, 0, sizeof(info));  //初始化
		while((line = fgets(buffer, sizeof(buffer), fp)) != NULL)
		{ 
			//buffer[strlen(buffer)-1] = '\0';		
			memcpy(data_record[i], line, strlen(line)-1);	
			//strncpy(data_record[i], line, strlen(buffer)-1);
	  	
			strcpy(data, data_record[i]);
			data[strlen(data)-1]='\0';
			strcpy(data_record[i], data);
			
			i++;
		} 
	}
	
  fclose(fp);   
  
}

void	MotionDataShow()
{  
	char data_show[256];
	int i = 0;
	
	for(i = 0; i < INFO_TOTAL_NUM; i++)
	{
		strcpy(data_show, data_record[i]);
		
		if(strncmp("[ImgMachinePic]", data_show, strlen("[ImgMachinePic]")) == 0)
		{
			u_pwndImgMachinePic->SetPropValueT("imagepath", data_record[i+1]);
			((CtmImageBox*)u_pwndImgMachinePic)->Update2();
		}
		else if(strncmp("[MachineType]", data_show, strlen("[MachineType]")) == 0)
		{
			u_pwndStaticMachTable01->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticMachTable01->Update();
		}
		else if(strncmp("[Load]", data_show, strlen("[Load]")) == 0)
		{
			u_pwndStaticMachTable02->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticMachTable02->Update();
		}
		else if(strncmp("[J1_range]", data_show, strlen("[J1_range]")) == 0)
		{
			u_pwndStaticMachTable03->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticMachTable03->Update();
		}
		else if(strncmp("[J2_range]", data_show, strlen("[J2_range]")) == 0)
		{
			u_pwndStaticMachTable04->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticMachTable04->Update();
		}
		else if(strncmp("[J3_range]", data_show, strlen("[J3_range]")) == 0)
		{
			u_pwndStaticMachTable05->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticMachTable05->Update();
		}
		else if(strncmp("[J4_range]", data_show, strlen("[J4_range]")) == 0)
		{
			u_pwndStaticMachTable06->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticMachTable06->Update();
		}
		else if(strncmp("[J5_range]", data_show, strlen("[J5_range]")) == 0)
		{
			u_pwndStaticMachTable07->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticMachTable07->Update();
		}
		else if(strncmp("[J6_range]", data_show, strlen("[J6_range]")) == 0)
		{
			u_pwndStaticMachTable08->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticMachTable08->Update();
		}
		else
		{
		}
	}
	
}

void	VersionShow()
{
	char HMIVersion[20], CNCVersion[10], PLCVersion[10];
	char two_HMIVersion[40], two_CNCVersion[20], two_PLCVersion[20];
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
	
	char data_show[256];
	int i = 0;
	
	for(i = 0; i < INFO_TOTAL_NUM; i++)
	{
		strcpy(data_show, data_record[i]);
  	
		if(strncmp("[ImgAgentsPic]", data_show, strlen("[ImgAgentsPic]")) == 0)
		{
			//strcpy(data_show, data_record[i+1]);
			//data_show[strlen(data_show)-1]='\0';
			printf("u_pwndImgAgentsPic ----> %s\n", data_record[i+1]);
			u_pwndImgAgentsPic->SetPropValueT("imagepath", data_record[i+1]);
			u_pwndImgAgentsPic->Update();
		}
		else if(strncmp("[AgentsTel]", data_show, strlen("[AgentsTel]")) == 0)
		{
			u_pwndStaticComTable01->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticComTable01->Update();
		}
		else if(strncmp("[AgentsFax]", data_show, strlen("[AgentsFax]")) == 0)
		{
			u_pwndStaticComTable02->SetPropValueT("textID", data_record[i+1]);
			u_pwndStaticComTable02->Update();
		}
		else if(strncmp("[AgentsAdd]", data_show, strlen("[AgentsAdd]")) == 0)
		{
			//strcpy(data_show, data_record[i+1]);
			printf("AgentsAdd ----> %s\n", data_record[i+1]);
			u_pwndStaticComTable03->SetPropValueT("textID", data_record[i+1]);  // (編碼問題，導致卡關，中文會有問題，英文正常)
			//u_pwndStaticComTable03->SetPropValueT("textID", "台中市一一一一一一一一一一一一一一一一號");
			//u_pwndStaticComTable03->SetPropValueT("textID", "No. 36, Lane 211, Taiming Road, Wuri District, Taichung City");
			u_pwndStaticComTable03->Update();
		}
		else
		{
		}
	}
	
}

void	GetHMIVersion(char* VerStr)
{
	char str[256];
	char* ptr;
	FILE *str_ptr = NULL;
	str_ptr=fopen("./hmiversion.ini","rt");
	sprintf(VerStr,"No Found Data");
	while(fgets(str, sizeof(str), str_ptr)!=NULL)
	{
		printf("str = %s\n",str);
		if(strstr(str,"HMIVersion") != NULL)
		{
			ptr=str;
			printf("ptr = %s\n",ptr);
			while(*ptr!=':' && *ptr!='\0') ptr++;
			if((*ptr)==':')
			{
				printf("ptr1 = %s\n",ptr);
				ptr++;
				printf("ptr2 = %s\n",ptr);
				while(*ptr==' ') ptr++;
				printf("ptr3 = %s\n",ptr);
				if(*ptr == '\0' || strlen(ptr) > 39)
				{
					printf("ptr = %s\n",ptr);
					sprintf(VerStr,"No Found Data");
					return ;
				}
				else
				{
					printf("ptr = %s\n",ptr);
					memset(VerStr,'\0',strlen(ptr));
					strncpy(VerStr,ptr,strlen(ptr)-2);
					return ;
				}
			}
			else
			{
				printf("ptr = %s\n",ptr);
				sprintf(VerStr,"No Found Data");
				return ;
			}
		}
	}
}

void    CodeChange(char *pDst, char *pSrc)		//輔助用函式，單字節轉化為雙字節處理
{
	//供StaticX2元件的text屬性使用(目前)
	WORD	*pwDst = (WORD *)pDst;
    
  while(*pSrc != 0)      
  	*pwDst++ = *pSrc++;
    
  *pwDst =0;  
}

