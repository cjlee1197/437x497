/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmFileExplorer                                                |
|  Task     : CtmFileExplorer   Manager                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James                                                          |
|  Version  : V1.00                                                          |
|  Creation : 08/12/2011                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include 	"tmFileExplorer.h"
#include	"tmconfig.h"
#include	"commonaction.h"
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <grp.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>
#include <algorithm>

/*----------------------------------------------------------------------------+
|               Constants											 |
+----------------------------------------------------------------------------*/



/*===========================================================================+
|           Class implementation - ScrollBar	                               |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
|----------------------------------------------------------------------------|
|Parameter: pwnd												|
|           nMaxline the total lines				    			     |
|           nCount the length of the list                                    |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmFileExplorer, CtmWnd)
CtmFileExplorer::CtmFileExplorer():CtmWnd()
{
	strcpy(TYPEName, "CtmFileExplorer");
	
	memset(m_pszShow, 	0, sizeof(m_pszShow));
	memset(m_pszIDShow, 0, sizeof(m_pszIDShow));	
	memset(m_pszDirName, 0, sizeof(m_pszDirName));
	memset(m_pszFileName, 0, sizeof(m_pszFileName));
	memset(m_pszCurrentPath, 0, sizeof(m_pszCurrentPath));
	m_wStartStrIndex 	= 0;
	m_bCopyBg			= FALSE;
	m_bFocus			= TRUE;
	m_wFocusColor		= 0x2008;
	m_wFocusIndex		= 0;
	m_wOldFocusIndex	= m_wFocusIndex;
	m_wFocusStrIndex	= m_wFocusIndex;
	m_bBar				= TRUE;
	m_nCurrentDirectCount	= 0;
	m_nCurrentFileCount		= 0;
	m_head = NULL;
	m_new_node = NULL;
	m_ptr = NULL;
	m_wShowType	= 0;
	memset(&m_sb, 0, sizeof(m_sb));
	nCount = 0;
	ButtonPressFlag = FALSE;
	MoveButtonDownFalg = FALSE;
	MouseDownTextFalg = FALSE;
}

CtmFileExplorer::~CtmFileExplorer()
{
	m_scrollbar.SetParent(NULL);
	for(int i = 0; i < CNT_INT_FILEMEMO_MAX; i++)
	{
		if(m_pszShow[i] != NULL)
		{
			free(m_pszShow[i]);
			m_pszShow[i] = NULL;
		}
		if(m_pszDirName[i] != NULL)
		{
			free(m_pszDirName[i]);
			m_pszDirName[i] = NULL;	
		}
		if(m_pszFileName[i] != NULL)
		{
			free(m_pszFileName[i]);
			m_pszFileName[i] = NULL;	
		}
	}
}

void    CtmFileExplorer::DrawBorder()
{
	
	short nRight  = m_rectWindow.right  - m_rectWindow.left;
	short nBottom = m_rectWindow.bottom - m_rectWindow.top;
	_SetViewPort(	m_rectWindow.left, 
					m_rectWindow.top, 
					m_rectWindow.right, 
					m_rectWindow.bottom
				);
	 _SetColor(0);
   
   
    _Rectangle(_GBORDER, 0, 0, nRight, nBottom);

    _SetColor(m_bgc);
    for (int i=1; (WORD) i< (m_wBorderWidth); i++)
    {
        _Rectangle(_GBORDER, i, i, nRight-i, nBottom-i);
    }
}

BOOL 	CtmFileExplorer::CreateA()
{
	CtmWnd::CreateA();
	SetInitScrollbar();
}

WORD	CtmFileExplorer::GetMaxRows()
{
	return (m_wnd.rect.bottom - m_wnd.rect.top) / m_pcFont->m_wHeight;
}


WORD	CtmFileExplorer::GetStrsNum()
{
	WORD	wResult = 0;
	GetShowText();
	for (int i = 0; i < CNT_INT_FILEMEMO_MAX; i++)
	{
		if (m_pszShow[i] != NULL)
			wResult++;
	}
	return wResult;
}

void	CtmFileExplorer::GetShowText()
{
	for (int i = 0; i < CNT_INT_FILEMEMO_MAX; i++)
	{
		if(m_pszIDShow[i] != NULL)
			StrIDToStr(m_pszIDShow[i], &m_pszShow[i]);
	}
}


void	CtmFileExplorer::SetFocusRow(WORD wOldFocusIndex, WORD wNewFocusIndex)
{
	
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	_SetColor((short)m_bgc);
	_Rectangle(_GFILLINTERIOR, 0, wOldFocusIndex * m_pcFont->m_wHeight, 
			GetClientRight(), (wOldFocusIndex + 1) * m_pcFont->m_wHeight);
	
	_SetColor((short)m_wFocusColor);
	_Rectangle(_GFILLINTERIOR, 0, wNewFocusIndex * m_pcFont->m_wHeight, 
			GetClientRight(), (wNewFocusIndex + 1) * m_pcFont->m_wHeight);
	
}
void	CtmFileExplorer::ShowStrs(WORD wOldFocusIndex, WORD wNewFocusIndex)
{
	int j 	= 0;
	int nY 	= 0;
	int	k	= 0;
	
	SetFocusRow(wOldFocusIndex, wNewFocusIndex);
	
	
	
	for (int i = m_wStartStrIndex; i < nCount; i++)
	{
		if (m_pszShow[i] != NULL)
		{
			nY	= j * m_pcFont->m_wHeight; 
			if (nY + m_pcFont->m_wHeight > m_wnd.rect.bottom - m_wnd.rect.top)	
				break;
			else
			{	
				//printf("i=%d, StrIndex=%d, focusIndex=%d\n", i, m_wStartStrIndex, wNewFocusIndex);
				if (i - m_wStartStrIndex - k == wNewFocusIndex)
				{
					int nOldColor;
					nOldColor 	= m_fgc;					
					m_fgc 		= (m_wFocusColor ^ 0xFFFF); 
					CtmWnd::ShowExtent(nY, m_pszShow[i]);
					m_wFocusStrIndex = i;
					m_fgc		= nOldColor;
				}
				else CtmWnd::ShowExtent(nY, m_pszShow[i]);
			}
			j++;
		}
		else k++;
	}

	ShowScrollbar();
	ChangePosNow(GetClientRECT());
}


void CtmFileExplorer::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("focuscolor", 		tmWORD, &m_wFocusColor);
	SetPropInfo("startstrindex", 	tmWORD, &m_wStartStrIndex);
	SetPropInfo("foucusindex",		tmWORD, &m_wFocusIndex);
	SetPropInfo("foucusstrIndex",	tmWORD, &m_wFocusStrIndex);
	SetPropInfo("bar",				tmBOOL, &m_bBar);
	SetPropInfo("showtype",			tmWORD, &m_wShowType);
	
	
	char Name[10];
	for (int i = 0; i < CNT_INT_FILEMEMO_MAX; i++)
	{
		sprintf(Name, "str%d", i);		
		SetPropInfo(Name, 	tmUSTR,	&m_pszShow[i]);
	
		sprintf(Name, "strID%d", i);
		SetPropInfo(Name, 	tmSTR,	&m_pszIDShow[i]);
	}
}

void CtmFileExplorer::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	SetPropInfo(MEMO_FOCUSCOLOR, 		tmWORD, &m_wFocusColor);
	SetPropInfo(MEMO_STARTSTRINDEX, 	tmWORD, &m_wStartStrIndex);
	SetPropInfo(MEMO_FOCUSINDEX,		tmWORD, &m_wFocusIndex);
	SetPropInfo(MEMO_FOCUSSTRINDEX,		tmWORD, &m_wFocusStrIndex);
	SetPropInfo(MEMO_BAR,				tmBOOL, &m_bBar);
	
	
	char Name[10];
	for (int i = 0; i < CNT_INT_FILEMEMO_MAX; i++)
	{	
		SetPropInfo(MEMO_STR0 + i*2, 	tmUSTR,	&m_pszShow[i]);
		SetPropInfo(MEMO_STRID0 + i*2, 	tmSTR,	&m_pszIDShow[i]);
	}
}

WORD    CtmFileExplorer::OnKey(WORD wKey)
{
	WORD	wResult 	= wKey;
	WORD	wMaxRows	= GetMaxRows();
	WORD	wStrsNum	= nCount;//GetStrsNum();
	WORD	wOldIndex	= m_wStartStrIndex;
	
	//printf("wKey=%x,_KEY_DOWNFIELD=%d \n", wKey,_KEY_DOWNFIELD);
	switch (wKey)
	{
		case _KEY_DOWNFIELD:
		case 0x6C00:		
			//m_wStartStrIndex
			
			//	the bottom line
			//printf("++ m_wStartStrIndex=%d m_wFocusIndex=%d wStrsNum=%d wMaxRows=%d \n", 
			//m_wFocusIndex, m_wFocusIndex, wStrsNum, wMaxRows);
				
			if ((m_wFocusIndex + 1) == wMaxRows)
			{
				//	only m_wStartStrIndex with increment
				if ((m_wStartStrIndex + m_wFocusIndex + 1) < wStrsNum)
				{
					m_wStartStrIndex++;
					Clear();
					DrawBorder();
					//GetShowText();
					ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
				}
			}
			else if (m_wFocusIndex <= wMaxRows)
			{
				if ((m_wStartStrIndex + m_wFocusIndex + 1) < wStrsNum)
				{
					m_wOldFocusIndex = m_wFocusIndex;
					m_wFocusIndex++;
					//printf("++ m_wFocusIndex=%d \n", m_wFocusIndex);
					Clear();
					DrawBorder();
					//GetShowText();
					ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
				}				
			}
			else perror("FoucusIndex > wMaxRows\n");				
			wResult = _NULL_KEY;
			break;
		case _KEY_UPFIELD:
		//case 0x6700:
			if (m_wFocusIndex == 0)
			{
				//	only m_wStartStrIndex with increment
				
				if (m_wStartStrIndex > 0)
					m_wStartStrIndex--;
				else return wResult;	//20120608 add
				Clear();
				DrawBorder();
				GetShowText();
				ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
				
			}
			else
			{
				m_wOldFocusIndex = m_wFocusIndex;
				m_wFocusIndex--;
					//printf("-- m_wFocusIndex=%d \n", m_wFocusIndex);
				Clear();
				DrawBorder();
				GetShowText();
				ShowStrs(m_wOldFocusIndex, m_wFocusIndex);						
			}
			
			wResult	= _NULL_KEY;
			break;
		default:
			return wResult;
	}
	
//	SetScrollbar(wStrsNum);
	return wResult;
}

void        CtmFileExplorer::Show()           /* Virtual function */
{
	//	check the visible property
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
	
	
	if (m_wnd.wStyle & tmWS_SHOW)
	{
		//	wtype == 0x8800		
		if ((m_wnd.wStyle & tmWS_REDRAW) && (m_wnd.wStyle & tmWS_FRAMEWND))
		{
			if (!m_bCopyBg)
			{
				m_bCopyBg = TRUE;		
				CopyBg();						//<<yang 2005/11/23 	
			}
		}
		Clear();
		DrawBorder();
		GetShowText();
		ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
		
		ShowScrollbar();		
	}
	else
	{
		if (m_bCopyBg)
		{
			ShowBg();
			FreeBg();
			m_bCopyBg = FALSE;
		}
		else CtmWnd::Clear();
	}
}

void    	CtmFileExplorer::Update()
{
	if (m_bPropertyChanged) Show();
}

void    	CtmFileExplorer::UpdateAll()
{
	Show();
}


void		CtmFileExplorer::ShowScrollbar()
{
	if (m_bBar)
	{
		if(nCount > GetMaxRows())
		{
			SetScrollbar(nCount/*GetStrsNum()*/);
			UpdateScrollbar();
		}
	}
}

void		CtmFileExplorer::UpdateScrollbar()
{
	if (m_bBar)
		m_scrollbar.Update();
}

void		CtmFileExplorer::SetInitScrollbar()
{
	/*if(CtmConfig::GetInstance()->GetBinaryType() == 1)
		m_scrollbar.SetPropertys_Nub();
	else*/
		m_scrollbar.SetPropertys();
	
	//	===========================
	//	set the scrollbar parent
	//	===========================
	m_scrollbar.SetParent(this);
	
	//	===========================
	//	the scrollbar position and size
	//	===========================
	m_scrollbar.SetPropValueT("left", 	m_wnd.rect.right - m_wnd.rect.left - 40);
	m_scrollbar.SetPropValueT("top", 	double(0)-3);
	m_scrollbar.SetPropValueT("right", 	m_wnd.rect.right - m_wnd.rect.left-1);
	m_scrollbar.SetPropValueT("bottom",	m_wnd.rect.bottom - m_wnd.rect.top-1);
	
	
	//	===========================
	//	the scrollbar type
	//	===========================
	m_scrollbar.SetPropValueT("fgc",	m_fgc);
	m_scrollbar.SetPropValueT("bgc",	m_bgc);
	
	m_scrollbar.CreateA();

	//SetScrollbar(GetStrsNum());
}

void		CtmFileExplorer::SetScrollbar(WORD wRange)
{
	int nRange = wRange - GetMaxRows();
	if (nRange < 0) nRange = 0;
	m_scrollbar.SetMaxReg(nRange/* 總數 - 一次可顯示的數量 */);	//捲動元件共可以移動幾段
	m_scrollbar.SetButtonHight(GetMaxRows()/* 一次可顯示的數量 */);	//捲動元件中間按鈕長度 需先設定SetMaxReg()
	m_scrollbar.SetPos(m_wStartStrIndex);	//目前在第幾段
}

void CtmFileExplorer::Add_sort(LinkNode new_node,LinkNode head)
{
	LinkNode temp,pp;
	temp = head;
	pp = head;
	int flag = 0;
    
	if(head->next == NULL)
	{	
		//lise is null
		head->next = new_node;
		return ;
	}

	while(temp->next)
	{
		//list has elements
		temp = pp->next;
		if(strcasecmp(temp->filename,new_node->filename) > 0)
		{
			flag = 1;
			break;
		}
		if(temp->next != NULL)
			pp = temp;
	}	

	if(flag == 1)
	{
		//insert into the middle of pp and temp
		pp->next = new_node;
        new_node->next = temp;
	}
	else
	{
		//insert into the end of list
		new_node->next = NULL;
        temp->next = new_node;
	}
}

char * CtmFileExplorer::Sperm(mode_t mode)
{
    static char temp[11]="----------";
    if(S_ISREG(mode) > 0)				//normal file  S_ISDIR(mode) >0 is normal direction
		temp[0] = '-';
	else if(S_ISDIR(mode) > 0)
		temp[0] = 'd';
    else if(S_ISLNK(mode) > 0)
	    temp[0] = 'l';

    if((mode & S_IRUSR) == S_IRUSR)
	    temp[1] = 'r';
    if((mode & S_IWUSR) == S_IWUSR)
	    temp[2] = 'w';	
	if((mode & S_IXUSR) == S_IXUSR)
		temp[3] = 'x';

    if((mode & S_IRGRP) == S_IRGRP)
	    temp[4] = 'r';
    if((mode & S_IWGRP) == S_IWGRP)
	    temp[5] = 'w';	
	if((mode & S_IXGRP) == S_IXGRP)
		temp[6] = 'x';

    if((mode & S_IROTH) == S_IROTH)
	    temp[7] = 'r';
    if((mode & S_IWOTH) == S_IWOTH)
	    temp[8] = 'w';	
	if((mode & S_IXOTH) == S_IXOTH)
		temp[9] = 'x';
	//if(S_ISDIR(mode) == 0)
	//{
	//	printf("is file \n");	
	//}
	//else
	//{
	//	printf("is direction \n");	
	//}
	temp[10] = '\0';
	return temp;
	
}

void CtmFileExplorer::Print_dir_info(struct stat statbuf,char *name)
{
    struct passwd *pwd;
    struct group *grp;
    struct tm *tm;
	char datestring[PATH_MAX];
    char *link_name;

    /*printf file info*/
    link_name=Sperm(statbuf.st_mode);
	if(link_name[0] == 'l')					//soft link file
	{
		char buf[256];
		int i;
       	strcat(temppath,name);
		i = readlink(temppath,buf,255);
		buf[i] = '\0';
		strcat(name," -> ");
		strcat(name,buf);    
	}
	//printf("%10.10s",Sperm(statbuf.st_mode));	//type,permissions
	//printf("%4d",statbuf.st_nlink);				//number of links
	
//	if((pwd=getpwuid(statbuf.st_uid)) != NULL)	//owner's name
//		printf(" %-8.8s",pwd->pw_name);
//	else
//		printf(" %-8d",statbuf.st_uid);
//        
//	if((grp = getgrgid(statbuf.st_gid)) != NULL)//group name
//	    printf(" %-8.8s",grp->gr_name);	
//	else
//	    printf(" %-8d",statbuf.st_gid);

	//printf(" %9jd",(intmax_t)statbuf.st_size);	//size
        
    tm=localtime(&statbuf.st_mtime);			//modify time
    sprintf(datestring,"%d-%d-%d %d:%d:%d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
   // printf(" %-20.20s %-s\n",datestring,name);		
}

void CtmFileExplorer::Print_dir(LinkNode head)
{
    LinkNode temp;
	temp = head;
	while(temp->next)
	{
		temp = temp->next;
		Print_dir_info(temp->sb,temp->filename);
		// printf("%-s\n",temp->filename);
	}
}

int		CtmFileExplorer::Opendir_File(char*	pszDir, int nFlag)
{
    DIR * dir;	
    int	nIndex = 0, nFileIndex = 0;
    char path[PATH_MAX];
    char cPath_C[64] = ".", cPath_B[64] = "..";
	umask(0);
	Free_Node();
	m_wFocusIndex = 0;
	
	m_nCurrentFileCount		= nFileIndex;
	m_nCurrentDirectCount 	= nIndex;
	if(nFlag == 1)
	{
		//printf("1 m_pszCurrentPath=%s pszDir=%s \n", m_pszCurrentPath, pszDir);
		if(strcmp(pszDir, cPath_C) == 0)		//current path
		{
			;
		}
		else if(strcmp(pszDir, cPath_B) == 0)	// top level path
		{
			if(strlen(m_pszCurrentPath) == 1)
			{
				m_pszCurrentPath[strlen(m_pszCurrentPath)] = '\0';;
			}
			else
			{
				for(int i = 1; i <= strlen(m_pszCurrentPath); i++)
				{
					if(m_pszCurrentPath[strlen(m_pszCurrentPath)-i-1] == '/')
					{
					 	m_pszCurrentPath[strlen(m_pszCurrentPath)-i-1] = '\0';
					 	break;
					}
				}
			}
		}
		else
			sprintf(m_pszCurrentPath, "%s%s", m_pszCurrentPath, pszDir);
		//printf("2 m_pszCurrentPath=%s pszDir=%s \n", m_pszCurrentPath, pszDir);
	}
	else
	{
		memset(m_pszCurrentPath, 0, sizeof(m_pszCurrentPath));
		strcpy(m_pszCurrentPath, pszDir);	
	}
	
	//printf("2 m_pszCurrentPath=%s pszDir=%s \n", m_pszCurrentPath, pszDir);
	if(pszDir == NULL)
	{
		strcpy(path,"./");
    }
	else
	{
		strcpy(path, m_pszCurrentPath);
		if(path[strlen(path)-1] !='/')
			strcat(path,"/");
		if(m_pszCurrentPath[strlen(m_pszCurrentPath)-1] !='/')
			strcat(m_pszCurrentPath,"/");
	}
	
   // printf("path=%s \n", path);
	if(stat(path,&m_sb))
	{
		perror("stat");
		for(int i = 1; i <= strlen(m_pszCurrentPath); i++)
		{
			if(m_pszCurrentPath[strlen(m_pszCurrentPath)-i-1] == '/')
			{
			 	m_pszCurrentPath[strlen(m_pszCurrentPath)-i-1] = '\0';
			 	break;
			}
		}
		return 1;
	}
	//printf("1 m_pszCurrentPath=%s \n", m_pszCurrentPath);
	if(S_ISREG(m_sb.st_mode)> 0)  //is a file , not dir
	{
		//printf("this is a file pszDir=%s \n", pszDir);
		for(int i = 1; i <= strlen(m_pszCurrentPath); i++)
		{
			if(m_pszCurrentPath[strlen(m_pszCurrentPath)-i-1] == '/')
			{
			 	m_pszCurrentPath[strlen(m_pszCurrentPath)-i-1] = '\0';
			 	break;
			}
		}
	}
	//printf("2 m_pszCurrentPath=%s \n", m_pszCurrentPath);
	
    if((m_sb.st_mode & S_IFMT) == S_IFDIR)
    {
    	//dir
		if((dir=opendir(path)) == NULL)
	    {
			//printf("Cannot open %s.\n",path);
		    return 1;
	    }

        m_head=(LinkNode)malloc(sizeof(Node));
		m_head->next = NULL;//init list head

    strcpy(temppath,path);    
	    
	  {
	    /*20120507 mark <<<<<
	    while((m_ptr = readdir(dir)) != NULL)
	    {
			strcat(temppath,m_ptr->d_name);
            strcat(temppath,"\0"); 
			if(lstat(temppath,&m_sb) == -1)
			{
				perror("stat");
		        return 0;
			}
        
            m_new_node=(LinkNode)malloc(sizeof(Node));//init new_node
			if(m_new_node == NULL){
				return 0;	 
			}
			//set value to new_node
			m_new_node->filename = m_ptr->d_name;
			m_new_node->sb = m_sb;
			m_new_node->next = NULL;
			//printf("m_ptr->d_name=%s \n", m_ptr->d_name);
			
			if(S_ISREG(m_new_node->sb.st_mode) > 0)
			{	
				//printf("is a file \n");
				if(nFileIndex < CNT_INT_FILEMEMO_MAX)
				{
					if(m_pszShow[nFileIndex] != NULL)
						;//free(m_pszShow[nIndex]);
					else
						m_pszShow[nFileIndex]  = (char*)malloc(512);
						
					if(m_pszFileName[nFileIndex] != NULL)
						;
					else
						m_pszFileName[nFileIndex] = (char*)malloc(512);
					
					memset(m_pszFileName[nFileIndex], 0, 512);
					strcpy(m_pszFileName[nFileIndex], m_ptr->d_name);
					
					if(m_wShowType == 1)
					{
						//printf("m_ptr->d_name:%s \n",m_ptr->d_name);
						CodeChange(m_pszShow[nFileIndex], m_ptr->d_name);
						m_bPropertyChanged = TRUE;
					}
					nFileIndex ++;
					m_nCurrentFileCount = nFileIndex;
				}
				else
				{
					return 0;	
				}
			}
			else
			{
				if(nIndex < CNT_INT_FILEMEMO_MAX)
				{
					if(m_pszShow[nIndex] != NULL)
						;//free(m_pszShow[nIndex]);
					else
						m_pszShow[nIndex]  = (char*)malloc(512);
					
					if(m_pszDirName[nIndex] != NULL)
						;
					else
						m_pszDirName[nIndex] = (char*)malloc(512);
					
					memset(m_pszDirName[nIndex], 0, 512);
					strcpy(m_pszDirName[nIndex], m_ptr->d_name);
					
					if(m_wShowType == 0)
					{
						CodeChange(m_pszShow[nIndex], m_ptr->d_name);
						m_bPropertyChanged = TRUE;
					}
					nIndex++;
					m_nCurrentDirectCount = nIndex;
				}
				else
				{
					return 0;	
				}
			}
			Add_sort(m_new_node,m_head);
        
			strcpy(temppath,path);
	    }
	    20120507 mark >>>>>*/
	  }  
	  
		//20120507 add <<<<<
		struct dirent **namelist;
		int n = scandir(path, &namelist, 0, alphasort);

		if(n >=0)
		{
			for(int i=0; i<n; i++)
			{
    		m_new_node=(LinkNode)malloc(sizeof(Node));//init new_node
				if(m_new_node == NULL)
				{
					return 0;
				}
				
				//20120510 add <<<<<<	
				if(strncmp(namelist[i]->d_name, "O9",2) == 0)	//Don't list the file name begin of "O9"
				{
					free(namelist[i]);
					continue;
				}
				//20120510 add>>>>>>
				
				strcat(temppath,namelist[i]->d_name);
				strcat(temppath,"\0"); 
				if(lstat(temppath,&m_sb) == -1)
				{
					perror("stat");
				  return 0;
				}
				
				//set value to new_node
				m_new_node->filename = namelist[i]->d_name;
				m_new_node->sb = m_sb;
				m_new_node->next = NULL;
				//printf("i=%d namelist[i]->d_name=%s \n",i,namelist[i]->d_name);
				
				if(S_ISREG(m_new_node->sb.st_mode) > 0)
				{
					//printf("is a file \n");
					if(nFileIndex < CNT_INT_FILEMEMO_MAX)
					{
						if(m_pszShow[nFileIndex] != NULL)
							;//free(m_pszShow[nIndex]);
						else
							m_pszShow[nFileIndex]  = (char*)malloc(512);
							
						if(m_pszFileName[nFileIndex] != NULL)
							;
						else
							m_pszFileName[nFileIndex] = (char*)malloc(512);
						
						memset(m_pszFileName[nFileIndex], 0, 512);
						strcpy(m_pszFileName[nFileIndex], namelist[i]->d_name);
						
						if(m_wShowType == 1)
						{
							//printf("namelist[n]->d_name:%s \n",namelist[n]->d_name);
							CodeChange(m_pszShow[nFileIndex], namelist[i]->d_name);
							m_bPropertyChanged = TRUE;
						}
						nFileIndex ++;
						m_nCurrentFileCount = nFileIndex;
					}
					else
					{
						return 0;	
					}
				}
				else
				{
					if(nIndex < CNT_INT_FILEMEMO_MAX)
					{
						if(m_pszShow[nIndex] != NULL)
							;//free(m_pszShow[nIndex]);
						else
							m_pszShow[nIndex]  = (char*)malloc(512);
						
						if(m_pszDirName[nIndex] != NULL)
							;
						else
							m_pszDirName[nIndex] = (char*)malloc(512);
						
						memset(m_pszDirName[nIndex], 0, 512);
						strcpy(m_pszDirName[nIndex], namelist[i]->d_name);
						
						if(m_wShowType == 0)
						{
							CodeChange(m_pszShow[nIndex], namelist[i]->d_name);
							m_bPropertyChanged = TRUE;
						}
						nIndex++;
						m_nCurrentDirectCount = nIndex;
					}
					else
					{
						return 0;	
					}
				}
				Add_sort(m_new_node,m_head);
				strcpy(temppath,path);
    			
    		free(namelist[i]);
    	}
    }
    //20120507 add >>>>>
	    
	  Print_dir(m_head);
		closedir(dir);
	}
	else
	{
		//file
        Print_dir_info(m_sb,m_ptr->d_name);
		//	printf("%s\n",ptr->d_name);
	}
	//printf("3 m_pszCurrentPath=%s \n", m_pszCurrentPath);
	
	if(m_wShowType == 0)
		nCount = m_nCurrentDirectCount;
	else if(m_wShowType == 1)
		nCount = m_nCurrentFileCount;
	else
		nCount = m_nCurrentDirectCount > m_nCurrentFileCount ? m_nCurrentFileCount : m_nCurrentFileCount;
	//printf("nCount is %d\n",nCount);
	
	return 0;
}

int		CtmFileExplorer::Free_Node()
{
	LinkNode pTemp;
	while(m_head != NULL)
	{
		pTemp = m_head;
		m_head = m_head->next;
		//if(pTemp != NULL && pTemp->filename != NULL)
		//	free(pTemp->filename);
		//pTemp->filename = NULL;
		//free(pTemp->TMFileName);
		//pTemp->TMFileName = NULL;
		if(pTemp != NULL)
		{
			free(pTemp);
			pTemp = NULL;
		}
	}
	for(int i = 0; i < CNT_INT_FILEMEMO_MAX; i++)
	{
		if(m_pszShow[i] != NULL)
		{
			free(m_pszShow[i]);
			m_pszShow[i] = NULL;
		}
		if(m_pszDirName[i] != NULL)
		{
			free(m_pszDirName[i]);
			m_pszDirName[i] = NULL;	
		}
	}
}

int		CtmFileExplorer::GetFocusIndex()
{
	return m_wFocusIndex;
}

int		CtmFileExplorer::GetCurrentIndex()
{
	return 	m_wStartStrIndex + m_wFocusIndex;
}

char*		CtmFileExplorer::GetCurrentDirName()
{
	//printf("m_wFocusIndex=%d m_wStartStrIndex=%d \n", m_wFocusIndex, m_wStartStrIndex);
	return m_pszDirName[m_wFocusIndex+m_wStartStrIndex];
}

int			CtmFileExplorer::GetCurrentDirCount()
{
	return m_nCurrentDirectCount;
}

int			CtmFileExplorer::GetCurrentFileCount()
{
	return m_nCurrentFileCount;
}

char*		CtmFileExplorer::GetCurrentFileName(int nIndex)
{
	if(nIndex > 0 && nIndex < m_nCurrentFileCount)
		return m_pszFileName[nIndex];
	else
		return m_pszFileName[m_wFocusIndex+m_wStartStrIndex];
}

char*		CtmFileExplorer::GetCurrentPath()
{
	char	temp[CNT_INT_FILEMEMO_MAX];
	strcpy(temp,m_pszCurrentPath);
	if(temp[strlen(temp)-1] == '/')
		temp[strlen(temp)-1] = NULL;
		
	return 	m_pszCurrentPath;
}

int		CtmFileExplorer::SetFocusIndex(int nIndex)
{
	if(nIndex < nCount && nIndex >=0 )
	{/*
		m_wOldFocusIndex	= m_wFocusIndex;
		//m_wFocusIndex	=nIndex;
		m_wStartStrIndex = nIndex;
		int temp = 0;
		temp = nCount - (nIndex+GetMaxRows());
		printf("nCount is %d\tm_wFocusIndex is %d\tGetMaxRows is %d\ttempis %d\n",nCount,m_wFocusIndex,GetMaxRows(),temp);
		if(temp<0)
		m_wStartStrIndex 	+= temp;
		m_wFocusIndex = nIndex-m_wStartStrIndex;
		m_wFocusStrIndex	= m_wFocusIndex;
		Clear();
		DrawBorder();
		ShowStrs(m_wOldFocusIndex, m_wFocusIndex);*/
		
		m_wOldFocusIndex	= m_wFocusIndex;
		if(nCount <= GetMaxRows())
		{
			m_wFocusIndex	=nIndex;
			m_wStartStrIndex = 0;
		}
		else
		{
		int temp = 0;
			temp = nCount - (nIndex+GetMaxRows());
			//printf("nCount is %d\tm_wFocusIndex is %d\tGetMaxRows is %d\ttempis %d\n",nCount,m_wFocusIndex,GetMaxRows(),temp);
			if(temp<0)
				m_wStartStrIndex 	=nIndex + temp;
			else
				m_wStartStrIndex 	=nIndex;
			m_wFocusIndex = nIndex-m_wStartStrIndex;
		}
		m_wFocusStrIndex	= m_wFocusIndex;
		Clear();
		DrawBorder();
		ShowStrs(m_wOldFocusIndex, m_wFocusIndex);
	}
}

void	CtmFileExplorer::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
    {
		case MSG_MOUSELUP:
			MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
       		break;
        case MSG_MOUSELDOWN:
        	MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        case MSG_MOUSEMOVE:
        	MouseMove(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        default:
        	break;
    }	
}


BOOL		CtmFileExplorer::MouseMove(short x_pos,short y_pos,int key_flag)
{
	//ShowFileList();
	if(MoveButtonDownFalg)
	{
		int jumpreg = m_scrollbar.GetIncrement();
		if( (y_pos-Down_Y) > jumpreg)
		{		
			if(m_wStartStrIndex < nCount-GetMaxRows())
			{
				Down_Y+=jumpreg;
				ScrollDown();
				
			}
		}
		else if( (y_pos-Down_Y) < !jumpreg )
		{
			if(m_wStartStrIndex > 0)
			{
				Down_Y-=jumpreg;
				ScrollUp();
			}	
		}
	}
	if(MouseDownTextFalg)
	{
		RECT    rect 		= GetWindowRECT();
		int i = (y_pos - rect.top)/m_pcFont->m_wHeight;
	 	if (i > nCount)
	 		return 1;
 	
	 	if (i > GetMaxRows())
	 		i = GetMaxRows();
 	
	 	if (m_wFocusIndex != i)
	 	{
	
	 		m_wOldFocusIndex = m_wFocusIndex;
	 		m_wFocusIndex = i;
	 		
	 		Clear();
			DrawBorder();
			ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
	 	}
	 	//else
	 	//	_PutKeyTail(_ENTER);
	}
	return TRUE;
}


BOOL		CtmFileExplorer::MouseDown(short x_pos,short y_pos,int key_flag)
{
	RECT    rect 		= GetWindowRECT();
  RECT  ButtonRect = m_scrollbar.GetMoveButtonRECT();
	int 	i 			= 0;

	//if(x_pos > rect.right || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
  //{
  //	if (m_bVisible)
  //		_PutKeyTail(_ENTER);
  //    return 1;
  //}
  
  if (x_pos > m_scrollbar.GetWindowRECT().left && x_pos < rect.right && y_pos > rect.top && y_pos < rect.bottom)//A2±±|i﹐m§PA_﹐‥|bscrollbar
  {
  	if(nCount > GetMaxRows())
  	{
	  	if (y_pos < m_scrollbar.GetWindowRECT().top + tmSCROLLBAR_BUTTON)		//A2±±|i﹐m|b|V?W?o?s
	  	{
	  		m_scrollbar.SetButtonPress(1);
	  		ButtonPressFlag = TRUE;
	  		ScrollUp();
	  	}
	  	else if (y_pos > m_scrollbar.GetWindowRECT().bottom - tmSCROLLBAR_BUTTON)		//A2±±|i﹐m|b|V?U?o?s
	  	{
	  		m_scrollbar.SetButtonPress(3);
	  		ButtonPressFlag = TRUE;
	  		ScrollDown();
	  	}
	  	else if(	y_pos > ButtonRect.top&& y_pos < ButtonRect.bottom&& x_pos > ButtonRect.left&& x_pos < ButtonRect.right)
	  	{
	  		MoveButtonDownFalg = TRUE;
	  		m_scrollbar.SetButtonPress(2);
	  		ButtonPressFlag = TRUE;
	  		Down_Y = y_pos;
	  	}
  	}
  		
  	return TRUE;
  }
  
  if (x_pos > rect.left && x_pos < m_scrollbar.GetWindowRECT().left && y_pos > rect.top && y_pos < rect.bottom)
  {
  	MouseDownTextFalg = TRUE;
  	/*for (i = 1; i <= wMaxRows; i++)
 		{
 			if (y_pos <= rect.top + i * m_pcFont->m_wHeight)		//§PA_A2±±﹐‥2A’X|a
 				break;
 		}*/
 		int i = (y_pos - rect.top)/m_pcFont->m_wHeight;
	 	if (i > nCount)
	 		return 1;
 	
	 	if (i > GetMaxRows())
	 		i = GetMaxRows();
 	
	 	if (m_wFocusIndex != i)
	 	{
	 		m_wOldFocusIndex = m_wFocusIndex;
	 		m_wFocusIndex = i;
	 		
	 		Clear();
			DrawBorder();
			ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
	 	}
	 	//else
	 	//	_PutKeyTail(_ENTER);
 	}
 	return TRUE;
}

BOOL	CtmFileExplorer::MouseUp(short x_pos,short y_pos,int key_flag)
{
	MouseDownTextFalg = FALSE;
	MoveButtonDownFalg = FALSE;
	
	if(ButtonPressFlag == TRUE)
	{
   	ButtonPressFlag = FALSE;
		m_scrollbar.SetAllButtonUp();
	}
	/*
	if (x_pos > rect.left && x_pos < m_scrollbar.GetWindowRECT().left && y_pos > rect.top && y_pos < rect.bottom)//A2±±|i﹐m§PA_﹐‥|bscrollbar
    {
    	for (i = 1; i <= wMaxRows; i++)
 			{
 				if (y_pos <= rect.top + i * m_pcFont->m_wHeight)		//§PA_A2±±﹐‥2A’X|a
 					break;
 			}
 	
 			if (i > wStrsNum)	 
 				return 1;
 	
 			if (i > wMaxRows)	
 				i = wMaxRows;
 	
 			if (m_wFocusIndex != i - 1)
 			{

 				m_wOldFocusIndex = m_wFocusIndex;
 				m_wFocusIndex = i - 1;
 		
 				Clear();
				DrawBorder();
				ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
 			}
 			else		//|paG?i|P?@-O!A￥Nai?e?JENTER
 				_PutKeyTail(_ENTER);
 		}*/
 	
 	return TRUE;
}

void		CtmFileExplorer::ScrollUp()
{
	if (m_wStartStrIndex <= 0)
		return;
	
    m_wStartStrIndex--;
    
    if (m_wFocusIndex < GetMaxRows() - 1)
    {
    	m_wOldFocusIndex = m_wFocusIndex;
    	m_wFocusIndex++;
    }
    	
    Clear();
	DrawBorder();
	ShowStrs(m_wOldFocusIndex, m_wFocusIndex);
}

void		CtmFileExplorer::ScrollDown()
{
	if (m_wStartStrIndex + GetMaxRows() >= nCount)
		return;
    
    m_wStartStrIndex++;
    
    if (m_wFocusIndex > 0)
    {
    	m_wOldFocusIndex = m_wFocusIndex;
    	m_wFocusIndex--;
    }	
    Clear();
	DrawBorder();
	ShowStrs(m_wOldFocusIndex, m_wFocusIndex);
    
}

void		CtmFileExplorer::OnGetFocus()
{
	m_wFocusColor		= 65000;
	Update();
	//CtmWnd::DrawGetFocus();
}

void		CtmFileExplorer::OnLoseFocus()
{
	m_wFocusColor		= 0x2008;
	Update();
	//CtmWnd::DrawLoseFocus();
}

//New add by Roy!o
//ROY!GAE?d?e?JaoAE|W?O§_cM2{|3aoAER×-??A -> TRUE		‥S|3°YAD!AμL-??A
//																				-> FALSE	?w|3|s|baoAE|W
BOOL		CtmFileExplorer::FileCheck(char* sFileName)
{
	for(int i=0; i<CNT_INT_FILEMEMO_MAX; i++)
	{
		if(m_pszFileName[i] == NULL)
			break;
		if(!strcmp(sFileName, m_pszFileName[i]))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//20140623!o
void		CtmFileExplorer::ShowFileList()
{
	for(int i=0; i<nCount; i++)
	{
		if(m_pszFileName[i] == NULL)
			break;
		printf("File[%d] = %s\n", i, m_pszFileName[i]);
	}
}
//20140624!o
int			CtmFileExplorer::GetFileIndex(char* FileName)	//‥u±o﹐OAE|Waoindex  Mario add
{
	int i=0;
	for( i=0 ; i<nCount; i++)
	{
		if(strcmp(m_pszFileName[i],FileName)==0)
			break;
	}
	return i;
}

void CtmFileExplorer::GetCurrentFilePath(char* path)
{
	sprintf(path,"%s%s",m_pszCurrentPath,m_pszFileName[m_wFocusIndex+m_wStartStrIndex]);
}