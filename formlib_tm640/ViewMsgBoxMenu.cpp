/*============================================================================+
|  Class    : ViewMagBox library                                            	|
|  Task     : ViewMagBox library action source file                          	|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario write

#include	"ViewMsgBoxMenu.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

#define		MENU_LEVEL_01_START	0x00	//Menu Level 01 taborder start
#define		MENU_LEVEL_02_START	0x10	//Menu Level 02 taborder start
#define		MENU_LEVEL_03_START	0x20	//Menu Level 03 taborder start
/*========================================================+
|						Global Variable																|
+========================================================*/
//§PÂ_¬O§_«ö¤UENTERÁä
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnLevel01[12]				= {NULL};
CtmWnd*		u_pwndBtnLevel02[12]				= {NULL};
CtmWnd*		u_pwndBtnLevel03[12]				= {NULL};
CtmWnd*		u_pwndImageLevel01[12]				= {NULL};
CtmWnd*		u_pwndImageLevel02[12]				= {NULL};
CtmWnd*		u_pwndImageLevel03[12]				= {NULL};

int		u_nBtnLevel01Cnt		=0;
int		u_nBtnLevel02Cnt		=0;
int		u_nBtnLevel03Cnt		=0;
int		u_nImageLevel01Cnt		=0;
int		u_nImageLevel02Cnt		=0;
int		u_nImageLevel03Cnt		=0;
	
CtmWnd*		u_pwndLevel01Bk				= NULL;
CtmWnd*		u_pwndLevel02eBk				= NULL;
CtmWnd*		u_pwndLevel03Bk				= NULL;

CtmWnd*		u_pwndBtnExit				= NULL;

CtmWnd*		u_pwndLastPressL01	= NULL;
CtmWnd*		u_pwndLastPressL02	= NULL;
char strtmp[64];
/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy¡õ
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	Toolbar_MenuIndex[0]=0;
	Toolbar_MenuIndex[1]=0;
	Toolbar_MenuIndex[2]=0;
	u_nBtnLevel01Cnt				= GetSpecialControlNum(pwndSender, "button_L1", "CtmToolButton", u_pwndBtnLevel01);
	u_nBtnLevel02Cnt				= GetSpecialControlNum(pwndSender, "button_L2", "CtmToolButton", u_pwndBtnLevel02);
	u_nBtnLevel03Cnt			= GetSpecialControlNum(pwndSender, "button_L3", "CtmToolButton", u_pwndBtnLevel03);
	u_nImageLevel01Cnt			= GetSpecialControlNum(pwndSender, "Image_L1", "CtmImageBox", u_pwndImageLevel01);
	u_nImageLevel02Cnt			= GetSpecialControlNum(pwndSender, "Image_L2", "CtmImageBox", u_pwndImageLevel02);
	u_nImageLevel03Cnt		= GetSpecialControlNum(pwndSender, "Image_L3", "CtmImageBox", u_pwndImageLevel03);
	
	u_pwndLevel01Bk			= pwndSender->FindControlFromName("ImageLevel01");
	u_pwndLevel02eBk			= pwndSender->FindControlFromName("ImageLevel02");
	u_pwndLevel03Bk		= pwndSender->FindControlFromName("ImageLevel03");
	
	u_pwndBtnExit					= pwndSender->FindControlFromName("btnExit");
	
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	

	return wIDControl;
}
//20140624¡ô

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
		if(pwndTemp == u_pwndBtnExit)
		{
			Toolbar_MenuIndex[0]=0;
			Toolbar_MenuIndex[1]=0;
			Toolbar_MenuIndex[2]=0;
			Exit();
		}
		
		for(int i = 0 ; i<u_nBtnLevel01Cnt ; i++)
		{
			if(pwndTemp == u_pwndBtnLevel01[i])
			{
				Toolbar_MenuIndex[0]=i+1;
				Toolbar_MenuIndex[1]=0;
				Toolbar_MenuIndex[2]=0;
				if(CheckAction())	Exit();
				else	
				{
					if(u_pwndLastPressL01 != NULL)
					{
						u_pwndLastPressL01->SetPropValueT("upbitmap",GetImagePath("ButtonUp2.png",cfg.Machine,strtmp));
						u_pwndLastPressL01->CreateA();
						u_pwndLastPressL01->Update();
					}
					u_pwndLastPressL01 = pwndTemp;
					u_pwndLastPressL01->SetPropValueT("upbitmap",GetImagePath("ButtonDown2.png",cfg.Machine,strtmp));
					u_pwndLastPressL01->CreateA();
					u_pwndLastPressL01->Update();
					
					ShowLevelTwo();
				}
			}
		}
		for(int i = 0 ; i<u_nBtnLevel02Cnt ; i++)
		{
			if(pwndTemp == u_pwndBtnLevel02[i])
			{
				Toolbar_MenuIndex[1]=i+1;
				Toolbar_MenuIndex[2]=0;
				if(CheckAction())	Exit();
				else	
				{
					if(u_pwndLastPressL02 != NULL)
					{
						u_pwndLastPressL02->SetPropValueT("upbitmap",GetImagePath("ButtonUp2.png",cfg.Machine,strtmp));
						u_pwndLastPressL02->CreateA();
						u_pwndLastPressL02->Update();
					}
					u_pwndLastPressL02 = pwndTemp;
					u_pwndLastPressL02->SetPropValueT("upbitmap",GetImagePath("ButtonDown2.png",cfg.Machine,strtmp));
					u_pwndLastPressL02->CreateA();
					u_pwndLastPressL02->Update();
					
					ShowLevelThree();
				}
			}
		}
		
		for(int i = 0 ; i<u_nBtnLevel03Cnt ; i++)
		{
			if(pwndTemp == u_pwndBtnLevel03[i])
			{
				Toolbar_MenuIndex[2]=i+1;
				if(CheckAction())	Exit();
			}
		}
	return wIDControl;
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	static int StartRun=1;
	if(StartRun > 0)
	{
		StartRun --;
		//ShowLevelOne();
		ShowLevelTwo();
		ShowLevelThree();
	}
}

/*========================================================+
|						Helper																				|
+========================================================*/

BOOL	CheckAction()
{
	char toolbarfile_path[64];
	memset(toolbarfile_path,0,sizeof(toolbarfile_path));
	if(cfg.Machine==SIXROBOT)
		sprintf(toolbarfile_path,"res_tm640/toolbar_sixjoint/toolbar");
	else
		sprintf(toolbarfile_path,"res_tm640/toolbar/toolbar");
	for(int i = 0; i<3 ; i++)
	{
		if(Toolbar_MenuIndex[i]!=0)
		{
			sprintf(toolbarfile_path,"%s_%02d",toolbarfile_path,Toolbar_MenuIndex[i]);
		}
		else 
		{
			if(i==0) return FALSE;
			break;
		}
	}
	sprintf(toolbarfile_path,"%s.txt",toolbarfile_path);
	//printf("toolbarfile_path = %s\n",toolbarfile_path);
	
	if(file_exists(toolbarfile_path)) return TRUE;
	else return FALSE;
}

void ShowLevelOne()
{
	u_pwndLastPressL01 = NULL;
	u_pwndLevel01Bk->Show();
	u_pwndLevel02eBk->Show();
	u_pwndLevel03Bk->Show();
	char image_path[64];
	char str_file_path[64];
	char next_str_file_path[64];
	char str_id[64];
	FILE *str_ptr = NULL;
	sprintf(str_file_path,GetImagePath("Menu_Level.txt",cfg.Machine,strtmp));
	str_ptr=fopen(str_file_path,"rt");
	for(int i = 0; i<u_nBtnLevel01Cnt ; i++)
	{
		memset(image_path,0,sizeof(image_path));
		sprintf(image_path,"%s%02d.png",GetImagePath("Menu_",cfg.Machine,strtmp),i+1);
		sprintf(next_str_file_path,"%s%02d.txt",GetImagePath("Menu_Level_",cfg.Machine,strtmp),i+1);
		if(file_exists(image_path))	
		{
			if(u_pwndBtnLevel01[i] != NULL)
			{
				if(fgets(str_id, sizeof(str_id), str_ptr)!=NULL && strlen(str_id)>2)
				{
					str_id[strlen(str_id)-2] = '\0';
					u_pwndBtnLevel01[i]->SetPropValueT("captionID",str_id);
				}
				else
					u_pwndBtnLevel01[i]->SetPropValueT("captionID","MSG_DUMMY");
				if(file_exists(next_str_file_path))	
				{
					u_pwndBtnLevel01[i]->SetPropValueT("upbitmap",GetImagePath("ButtonUp2.png",cfg.Machine,strtmp));
					u_pwndBtnLevel01[i]->SetPropValueT("downbitmap",GetImagePath("ButtonDown2.png",cfg.Machine,strtmp));
				}
				else
				{
					u_pwndBtnLevel01[i]->SetPropValueT("upbitmap",GetImagePath("ButtonUp.png",cfg.Machine,strtmp));
					u_pwndBtnLevel01[i]->SetPropValueT("downbitmap",GetImagePath("ButtonDown.png",cfg.Machine,strtmp));
				}
				u_pwndBtnLevel01[i]->SetPropValueT("taborder",MENU_LEVEL_01_START + i);
				u_pwndBtnLevel01[i]->CreateA();
				for (int j = 0; j < u_pwndBtnLevel01[i]->GetControlCount(); j++)
				{
					if (strcmp(u_pwndBtnLevel01[i]->Controls[j]->TYPEName, "CtmImageBox") != 0) 
						continue;
					u_pwndBtnLevel01[i]->Controls[j]->SetPropValueT("imagepath",image_path);
					u_pwndBtnLevel01[i]->Controls[j]->Update();
					break;
				}
				u_pwndBtnLevel01[i]->Update();
			}	
		}
		else 
		{
				u_pwndBtnLevel01[i]->SetPropValueT("taborder",-2);
		}
		
	}
	if(str_ptr != NULL) fclose(str_ptr);
}

void ShowLevelTwo()
{
	u_pwndLastPressL02 = NULL;
	u_pwndLevel02eBk->Show();
	u_pwndLevel03Bk->Show();
	char image_path[64];
	char str_file_path[64];
	char next_str_file_path[64];
	char str_id[64];
	FILE *str_ptr = NULL;
	sprintf(str_file_path,"%s%02d.txt",GetImagePath("Menu_Level_",cfg.Machine,strtmp),Toolbar_MenuIndex[0]);
	str_ptr=fopen(str_file_path,"rt");
	for(int i = 0; i<u_nBtnLevel02Cnt ; i++)
	{
		memset(image_path,0,sizeof(image_path));
		sprintf(image_path,"%s%02d_%02d.png",GetImagePath("Menu_",cfg.Machine,strtmp),Toolbar_MenuIndex[0],i+1);
		sprintf(next_str_file_path,"%s%02d_%02d.txt",GetImagePath("Menu_Level_",cfg.Machine,strtmp),Toolbar_MenuIndex[0],i+1);
		//printf("image_path = %s\n",image_path);
		//printf("next_str_file_path = %s\n",next_str_file_path);
		if(file_exists(image_path))	
		{
			if(u_pwndBtnLevel02[i] != NULL)
			{
				if(fgets(str_id, sizeof(str_id), str_ptr)!=NULL && strlen(str_id)>2)
				{
					str_id[strlen(str_id)-2] = '\0';
					u_pwndBtnLevel02[i]->SetPropValueT("captionID",str_id);
				}
				else
					u_pwndBtnLevel02[i]->SetPropValueT("captionID","MSG_DUMMY");
				if(file_exists(next_str_file_path))	
				{
					u_pwndBtnLevel02[i]->SetPropValueT("upbitmap",GetImagePath("ButtonUp2.png",cfg.Machine,strtmp));
					u_pwndBtnLevel02[i]->SetPropValueT("downbitmap",GetImagePath("ButtonDown2.png",cfg.Machine,strtmp));
				}
				else
				{
					u_pwndBtnLevel02[i]->SetPropValueT("upbitmap",GetImagePath("ButtonUp.png",cfg.Machine,strtmp));
					u_pwndBtnLevel02[i]->SetPropValueT("downbitmap",GetImagePath("ButtonDown.png",cfg.Machine,strtmp));
				}
				u_pwndBtnLevel02[i]->SetPropValueT("taborder",MENU_LEVEL_02_START + i);
				u_pwndBtnLevel02[i]->CreateA();
				for (int j = 0; j < u_pwndBtnLevel02[i]->GetControlCount(); j++)
				{
					if (strcmp(u_pwndBtnLevel02[i]->Controls[j]->TYPEName, "CtmImageBox") != 0) 
						continue;
					u_pwndBtnLevel02[i]->Controls[j]->SetPropValueT("imagepath",image_path);
					u_pwndBtnLevel02[i]->Controls[j]->Update();
					break;
				}
				u_pwndBtnLevel02[i]->Update();
			}
		}
		else 
		{
				u_pwndBtnLevel02[i]->SetPropValueT("taborder",-2);
		}
	}
	if(str_ptr != NULL) fclose(str_ptr);
}

void ShowLevelThree()
{
	u_pwndLevel03Bk->Show();
	char image_path[64];
	char str_file_path[64];
	char str_id[64];
	FILE *str_ptr = NULL;
	sprintf(str_file_path,"%s%02d_%02d.txt",GetImagePath("Menu_Level_",cfg.Machine,strtmp),Toolbar_MenuIndex[0],Toolbar_MenuIndex[1]);
	str_ptr=fopen(str_file_path,"rt");
	for(int i = 0; i<u_nBtnLevel03Cnt ; i++)
	{
		memset(image_path,0,sizeof(image_path));
		sprintf(image_path,"%s%02d_%02d_%02d.png",GetImagePath("Menu_",cfg.Machine,strtmp),Toolbar_MenuIndex[0],Toolbar_MenuIndex[1],i+1);
		if(file_exists(image_path))	
		{
			if(u_pwndBtnLevel03[i] != NULL)
			{
				if(fgets(str_id, sizeof(str_id), str_ptr)!=NULL && strlen(str_id)>2)
				{
					str_id[strlen(str_id)-2] = '\0';
					u_pwndBtnLevel03[i]->SetPropValueT("captionID",str_id);
				}
				else
					u_pwndBtnLevel03[i]->SetPropValueT("captionID","MSG_DUMMY");
				u_pwndBtnLevel03[i]->SetPropValueT("upbitmap",GetImagePath("ButtonUp.png",cfg.Machine,strtmp));
				u_pwndBtnLevel03[i]->SetPropValueT("downbitmap",GetImagePath("ButtonDown.png",cfg.Machine,strtmp));
				u_pwndBtnLevel03[i]->SetPropValueT("taborder",MENU_LEVEL_03_START + i);
				u_pwndBtnLevel03[i]->CreateA();
				for (int j = 0; j < u_pwndBtnLevel03[i]->GetControlCount(); j++)
				{
					if (strcmp(u_pwndBtnLevel03[i]->Controls[j]->TYPEName, "CtmImageBox") != 0) 
						continue;
					u_pwndBtnLevel03[i]->Controls[j]->SetPropValueT("imagepath",image_path);
					u_pwndBtnLevel03[i]->Controls[j]->Update();
					break;
				}
				u_pwndBtnLevel03[i]->Update();
			}
		}
		else 
		{
				u_pwndBtnLevel03[i]->SetPropValueT("taborder",-2);
		}
	}
	if(str_ptr != NULL) fclose(str_ptr);
}


char* GetImagePath(char* imgname , int RobotMode, char*str)
{
	if(RobotMode==SIXROBOT)
		sprintf(str,"res_tm640/MenuImage_sixjoint/%s\0",imgname);
	else
		sprintf(str,"res_tm640/MenuImage/%s\0",imgname);
	return str;
}