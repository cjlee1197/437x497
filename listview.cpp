/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmListView                                               	 |
|  Task     : CtmListView   Manager                                      	 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James                                                          |
|  Version  : V1.00                                                          |
|  Creation : 01/24/2013                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include 	"listview.h"
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

/*---------------------------------------------------------------------------+
|               Constants											 		 |
+---------------------------------------------------------------------------*/
extern char temppath[PATH_MAX];
/*===========================================================================+
|           Class implementation - ScrollBar	                             |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
|----------------------------------------------------------------------------|
|Parameter: pwnd															 |
|           nMaxline the total lines				    			     	 |
|           nCount the length of the list                                    |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmListView, CtmWnd)
IMPLEMENT_DYNCREATE(CtmItem, CtmWnd)
CtmItem::CtmItem():CtmWnd()
{
	strcpy(TYPEName, "CtmItem");
	m_tagImageBox.wStyle 		= 1;
	m_tagImageBox.cImagePath 	= NULL;
	m_tagImageBox.dwKeyColor	= 0xFFFF;
	m_tagImageBox.wType	= 0x30;			//設置image類型
	
	m_wAlign 	= tmPARA_ALIGN_LEFT;	//默認向左對齊
	m_pszTextID	= NULL;
	m_pszText	= NULL;
	
	m_wImageWidth	= 0;				//默認圖片width
	m_wTextWidth	= 0;				//默認文字width
	m_wItemHeight	= 16;				//Height
	m_pImageBox		= NULL;				//默認圖片路徑
	m_pStaticX2		= NULL;
	m_wFgc			= 0;				//設置字體顏色
	m_wBgc			= 46486;			//設置背景顏色
	m_Static_wType	= 0x30;				//設置static類型
}

CtmItem::~CtmItem()
{
	if(m_pImageBox != NULL)
	{
		m_pImageBox->SetParent(NULL);
		delete 	m_pImageBox;
		m_pImageBox = NULL;
	}
	if(m_pStaticX2 != NULL)
	{
		m_pStaticX2->SetParent(NULL);
		delete m_pStaticX2;
		m_pStaticX2 = NULL;	
	}
}

BOOL 	CtmItem::CreateA()
{
	CtmWnd::CreateA();
}
	
void 	CtmItem::SetPropertys()
{
	CtmWnd::SetPropertys();
	//ImageBox
	//SetPropInfo("image_type", 		tmWORD, 	&m_tagImageBox.wType);
	SetPropInfo("image_wtype", 		tmWORD, 	&m_tagImageBox.wType);
	SetPropInfo("image_style",		tmWORD,		&m_tagImageBox.wStyle);
	SetPropInfo("item_imagepath",	tmSTR,		&m_tagImageBox.cImagePath);
	SetPropInfo("keycolor", 	tmLONG,		&m_tagImageBox.dwKeyColor);
	//staticx2
	SetPropInfo("align",		tmWORD,	 	&m_wAlign);
	SetPropInfo("textID", 		tmSTR, 		&m_pszTextID);
	SetPropInfo("text", 		tmUSTR, 	&m_pszText);
	SetPropInfo("fgc", 			tmWORD, 	&m_wFgc);
	SetPropInfo("bgc", 			tmWORD, 	&m_wBgc);
	SetPropInfo("static_wtype", tmWORD, 	&m_Static_wType);
	//Item
	SetPropInfo("imagewidth",	tmWORD,	 	&m_wImageWidth);
	SetPropInfo("textwidth", 	tmWORD, 	&m_wTextWidth);
	SetPropInfo("itemheight", 	tmWORD, 	&m_wItemHeight);
}

WORD    CtmItem::OnKey(WORD wKey)
{
	
}

void    CtmItem::Show()
{
	CalRECT();
	SetImageBoxAttr();
	SetStaticAttr();
	CtmWnd::Show();
}

void	CtmItem::CalRECT()
{
	m_RectImage.left	= 0;
	m_RectImage.right	= m_wImageWidth;
	m_RectImage.top		= 0;
	m_RectImage.bottom	= m_wItemHeight;
	
	m_RectStatic.left	= m_RectImage.right;
	m_RectStatic.right	= m_RectImage.left  + m_wTextWidth;
	m_RectStatic.top	= 0;
	m_RectStatic.bottom	=  m_wItemHeight ;
	//printf("client left=%d, right=%d ,top=%d ,bottom=%d \n", m_rectClient.left, m_rectClient.right, m_rectClient.top, m_rectClient.bottom );
	//printf("Image left=%d, right=%d, top=%d, bottom=%d \n", m_RectImage.left, m_RectImage.right, m_RectImage.top, m_RectImage.bottom);
	//printf("Static left=%d, right=%d, top=%d, bottom=%d \n", m_RectStatic.left, m_RectStatic.right, m_RectStatic.top, m_RectStatic.bottom);
}

void	CtmItem::SetStaticAttr()
{
	if(m_pStaticX2 == NULL)
	{
		m_pStaticX2	 = new CtmStaticX2();
		m_pStaticX2->SetPropertys();
		m_pStaticX2->SetParent(this);
		
		//m_pStaticX2->SetPropValueT("style", 3);
		//m_pStaticX2->SetPropValueT("type", 2);
		//m_pStaticX2->SetPropValueT("wtype", m_Static_wType);
		//m_pStaticX2->SetPropValueT("fgc", 	m_wFgc);
		//m_pStaticX2->SetPropValueT("bgc", 	m_wBgc);
		//m_pStaticX2->SetPropValueT("align", m_wAlign);
		//m_pStaticX2->SetPropValueT("left", 	m_RectStatic.left);
		//m_pStaticX2->SetPropValueT("right", m_RectStatic.right);
		//m_pStaticX2->SetPropValueT("top", 	m_RectStatic.top);
		//m_pStaticX2->SetPropValueT("bottom", m_RectStatic.bottom);
	}
	m_pStaticX2->SetPropValueT("type", 2);
	m_pStaticX2->SetPropValueT("wtype", m_Static_wType);
	m_pStaticX2->SetPropValueT("fgc", 	m_wFgc);
	m_pStaticX2->SetPropValueT("bgc", 	m_wBgc);
	m_pStaticX2->SetPropValueT("align", m_wAlign);

	m_pStaticX2->SetPropValueT("left", 	m_RectStatic.left);
	m_pStaticX2->SetPropValueT("right", m_RectStatic.right);
	m_pStaticX2->SetPropValueT("top", 	m_RectStatic.top);
	m_pStaticX2->SetPropValueT("bottom", m_RectStatic.bottom);
	m_pStaticX2->CreateA();
	if(m_pszTextID != NULL)
	{
		m_pStaticX2->SetPropValueT("textID", m_pszTextID);	
	}
	else
	{
		m_pStaticX2->SetPropValueT("text", m_pszText);	
	}
	m_pStaticX2->UpdateAll();
}

void	CtmItem::SetImageBoxAttr()
{
	if(m_pImageBox == NULL)
	{
		m_pImageBox  = new CtmImageBox();
		m_pImageBox->SetPropertys();
		m_pImageBox->SetParent(this);
		
		m_pImageBox->SetPropValueT("style", 3);
		m_pImageBox->SetPropValueT("type", m_tagImageBox.wStyle);
		m_pImageBox->SetPropValueT("wtype", m_tagImageBox.wType);
		m_pImageBox->SetPropValueT("keycolor", m_tagImageBox.dwKeyColor);

		m_pImageBox->SetPropValueT("left", (double)0);
		m_pImageBox->SetPropValueT("right", m_wImageWidth);
		m_pImageBox->SetPropValueT("top", (double)0);
		m_pImageBox->SetPropValueT("bottom", m_wItemHeight);
		
		m_pImageBox->CreateA();
	}
	m_pImageBox->SetPropValueT("imagepath", m_tagImageBox.cImagePath);
	//printf("m_tagImageBox.cImagePath=%s \n", m_tagImageBox.cImagePath);
	m_pImageBox->Update();
}

void    CtmItem::Update()
{
	CalRECT();
	SetStaticAttr();
	SetImageBoxAttr();
}

void   	CtmItem::Clear()
{
	CtmWnd::Clear();	
}
/*---------------------------------------------------------------------------+
|               Class 	CtmListView										 	 |
+---------------------------------------------------------------------------*/


CtmListView::CtmListView():CtmWnd()
{
	strcpy(TYPEName, "CtmListView");
	m_wColCount		= 0;		
	m_wItemCount	= 0;
	m_wMaxItemCount	= 0;
	m_wColHeight	= 0;
	m_cImagePath_file	= NULL;
	m_cImagePath_dir	= NULL;
	m_cImagePath_back	= NULL;
	m_bBar				= TRUE;
	
	m_bGetFocus			= FALSE;
	m_wStartItemIndex	= 0;
	m_wFocusIndex		= 0;
	m_wOldFocusIndex	= 0;
	m_wFocusColor		= 65000;
	m_wLostFocusColor	= 0;
	m_wSelectColor		= 4008;
	m_pszExRegDir		=NULL;
	m_wItemImageWidth	= 16;
	m_wItemKeyColor		= 63519;
	
	m_bSelectUse		= TRUE;
	//memset(m_wSelectIndex, 0,	sizeof(m_wSelectIndex));	
	memset(m_wszSelectFile, 0, sizeof(m_wszSelectFile));	
	
	for(int i = 0; i < MAX_COLUMNS_COUNT; i++)
	{
		m_pColumns_Item[i] = NULL;
		for(int j = 0; j  < MAX_ITEM_COUNT; j++)
		{
			m_pszItemShow[i][j] 	= NULL;
			m_pszItemIDShow[i][j]	= NULL;
			m_pContext_Item[i][j] 	= NULL;
			m_wItemType[i][j]		= 0;
		}
		m_Columns[i].wFontSize 	= 16;
		m_Columns[i].wType		= 0x30;
		m_Columns[i].pszTextID	= NULL;
		m_Columns[i].pszText	= NULL;
	}
	
	memset(m_pszShow, 	0, sizeof(m_pszShow));
	memset(m_pszIDShow, 0, sizeof(m_pszIDShow));	
	memset(m_pszDirName, 0, sizeof(m_pszDirName));
	memset(m_pszFileName, 0, sizeof(m_pszFileName));
	memset(m_pszDataShow, 0, sizeof(m_pszDataShow));
	memset(m_pszSizeShow, 0, sizeof(m_pszSizeShow));
	memset(m_szKeyWord1,0, sizeof(m_szKeyWord1));	
	memset(m_szKeyWord2,0, sizeof(m_szKeyWord2));	
	m_nFileNameLen=0;	
	
	memset(m_pszHidFileName, 0, sizeof(m_pszHidFileName));		//設置隱藏文件列表
	
	memset(m_pszCurrentPath, 0, sizeof(m_pszCurrentPath));
	
	//File Manage 
	m_file_node 	= NULL;
	m_dir_node 		= NULL;
	m_ptr 			= NULL;
	m_file_head 	= NULL;
	m_dir_head 		= NULL;
	memset(&m_sb, 0, sizeof(m_sb));
	m_nCurrentFileCount		= 0;
	m_nCurrentDirectCount	= 0;
	m_nCurrentCount			= 0;
	m_wMaxSelectItemCount	= 1;
}

CtmListView::~CtmListView()
{
	if(m_pszExRegDir!=NULL)
	{
		free(m_pszExRegDir);
		m_pszExRegDir=NULL;	
	}
	m_scrollbar.SetParent(NULL);
	for(int i = 0; i < m_wColCount; i++)
	{
		if(m_pColumns_Item[i] != NULL)
		{
			delete  m_pColumns_Item[i];
			m_pColumns_Item[i] = NULL;
		}
		for(int j = 0; j < m_wItemCount; j++)
		{
			if(m_pContext_Item[i][j] != NULL)
			{
				delete m_pContext_Item[i][j];
				m_pContext_Item[i][j] = NULL;	
			}
		}
	}
	for(int i = 0; i < MAX_ITEM_COUNT; i++)
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
		if(m_pszDataShow[i] != NULL)
		{
			free(m_pszDataShow[i]);
			m_pszDataShow[i] = NULL;	
		}
		if(m_pszSizeShow[i] != NULL)
		{
			free(m_pszSizeShow[i]);
			m_pszSizeShow[i] = NULL;	
		}
	}
	for(int i = 0; i < MAX_HIDEFILE_COUNT; i++)
	{
		if(m_pszHidFileName[i] != NULL)
		{
			delete m_pszHidFileName[i];	
			m_pszHidFileName[i] = NULL;
		}	
	}
	Free_Node();
}

void    CtmListView::DrawBorder()
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

BOOL 	CtmListView::CreateA()
{
	CtmWnd::CreateA();
	SetInitScrollbar();
	//自動計算m_wItemCount 個數
	if(m_wColHeight != 0)
	{	
		m_wItemCount = (m_rectWindow.bottom - m_rectWindow.top)/m_wColHeight - 1;
	}
	if(m_wColHeight > 0)
		m_wMaxItemCount =m_wItemCount;
	//printf("m_wItemCount=%d \n", m_wItemCount);
}

void CtmListView::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("colcount", 	tmWORD, &m_wColCount);
	SetPropInfo("itemcount", 	tmWORD, &m_wItemCount);
	SetPropInfo("align", 		tmWORD, &m_wAlign);
	SetPropInfo("item_imagepath_file", 	tmSTR,  &m_cImagePath_file);
	SetPropInfo("item_imagepath_dir", 	tmSTR,  &m_cImagePath_dir);
	SetPropInfo("item_imagepath_back", 	tmSTR,  &m_cImagePath_back);	
	SetPropInfo("ColHeight", 	tmWORD,	&m_wColHeight);
	SetPropInfo("focuscolor", 	tmWORD,	&m_wFocusColor);
	SetPropInfo("lostfocuscolor", 	tmWORD,	&m_wLostFocusColor);
	SetPropInfo("selectcolor", 	tmWORD,	&m_wSelectColor);
	SetPropInfo("item_imagewidth", 	tmWORD,	&m_wItemImageWidth);
	SetPropInfo("item_keycolor", 	tmWORD,	&m_wItemKeyColor);
	SetPropInfo("itemtype",			tmWORD,	&m_wShowType);
	
	char Name[20];
	for (int i = 0; i < MAX_COLUMNS_COUNT; i++)
	{
		sprintf(Name, "ColWidth%d", i);		
		SetPropInfo(Name, 	tmWORD,	&m_Columns[i].wWidth);
		sprintf(Name, "Colfgc%d", i);
		SetPropInfo(Name, 	tmWORD,	&m_Columns[i].fgc);
		sprintf(Name, "Colbgc%d", i);
		SetPropInfo(Name, 	tmWORD,	&m_Columns[i].bgc);
		sprintf(Name, "ColType%d", i);
		SetPropInfo(Name, 	tmWORD,	&m_Columns[i].wType);
		sprintf(Name, "ColFontSize%d", i);
		SetPropInfo(Name, 	tmWORD,	&m_Columns[i].wFontSize);
		sprintf(Name, "ColText%d", i);
		SetPropInfo(Name, 	tmSTR,	&m_Columns[i].pszText);
		sprintf(Name, "ColTextID%d", i);
		SetPropInfo(Name, 	tmUSTR,	&m_Columns[i].pszTextID);
	}
}

WORD    CtmListView::OnKey(WORD wKey)
{
	WORD	wResult 	= wKey;
	WORD	wMaxRows	= m_wMaxItemCount;
	WORD	wStrsNum	= m_nCurrentCount-1;
	WORD	wOldIndex	= m_wStartItemIndex;
	//printf("wKey =%x \n", wKey);
	switch (wKey)
	{
		case 0x6D00:		//page down
			if((m_wStartItemIndex + m_wItemCount) <= wStrsNum)
			{
				m_wStartItemIndex+=m_wItemCount;	
				//printf("strnum=%d, start=%d m_wFocusIndex=%d \n", wStrsNum, m_wStartItemIndex);
				if(m_wFocusIndex > (wStrsNum-m_wStartItemIndex))
					m_wFocusIndex= 0;
				ShowItems();
			}
			wResult = _NULL_KEY;
			break;
		case 0x6800:		//page up
			if(m_wStartItemIndex > m_wItemCount)
			{
				m_wStartItemIndex = m_wStartItemIndex - m_wItemCount;
			}
			else
			{
				m_wStartItemIndex = 0;	
			}
			ShowItems();		
			wResult = _NULL_KEY;
			break;
		case _KEY_DOWNFIELD:
			//printf("down wMaxRows=%d m_wFocusIndex=%d m_wStartItemIndex=%d wStrsNum=%d \n", wMaxRows, m_wFocusIndex, m_wStartItemIndex, wStrsNum);
			if ((m_wFocusIndex + 1) == wMaxRows)
			{
				if ((m_wStartItemIndex + m_wFocusIndex + 1) <= wStrsNum)
				{
					m_wStartItemIndex++;
					ShowItems();
				}
			}
			else if (m_wFocusIndex <= wMaxRows)
			{
				if ((m_wStartItemIndex + m_wFocusIndex + 1) <= wStrsNum)
				{
					m_wOldFocusIndex = m_wFocusIndex;
					m_wFocusIndex++;
					ShowItems();
				}				
			}
			else
				perror("FoucusIndex > wMaxRows\n");				
			wResult = _NULL_KEY;
			break;
		case _KEY_UPFIELD:
			//printf("up wMaxRows=%d m_wFocusIndex=%d m_wStartItemIndex=%d wStrsNum=%d \n", wMaxRows, m_wFocusIndex, m_wStartItemIndex, wStrsNum);
			if (m_wFocusIndex == 0)
			{
				if (m_wStartItemIndex > 0)
					m_wStartItemIndex--;
				else
				{
					wResult	= _NULL_KEY;
					return wResult;
				}
				ShowItems();
			}
			else
			{
				m_wOldFocusIndex = m_wFocusIndex;
				m_wFocusIndex--;
				ShowItems();	
			}
			wResult	= _NULL_KEY;
			break;
		case _ENTER:
			//printf("OpenNewDir \n");
			//printf("m_wFocusIndex=%d,m_wStartItemIndex=%d\n",m_wFocusIndex,m_wStartItemIndex);
			OpenNewDir(m_wFocusIndex, m_wStartItemIndex);
			ShowItems();	
			wResult	= _NULL_KEY;
			if(m_wFocusIndex+m_wStartItemIndex<MAX_ITEM_COUNT)
			{
				if(m_wItemType[0][m_wFocusIndex+m_wStartItemIndex] == LISTVIEW_FILE)
				{
					wResult=_ENTER;
				}
			}
			break;
		default:
			return wResult;
	}
	return wResult;
}

void        CtmListView::Show()           /* Virtual function */
{
	DrawBorder();			//顯示外框
	ShowColumns();			//顯示title
	ShowItems();			//顯示列表內容
	ShowScrollbar();		//顯示右邊scrollbar
	CtmWnd::Show();
}

void    	CtmListView::Update()
{
	Show();
}

void		CtmListView::UpdateList(int	nType, char*	pszName)
{
	int	nOldStartIndex = 0, nOldFocusIndex = 0;
	char	szPath[512];
	memset(szPath, 0, sizeof(szPath));
	strcpy(szPath, m_pszCurrentPath);
	if(nType == 0)			//only open current file dir
	{
		Opendir_File(szPath);	
	}
	else if(nType == 1)		//open current file dir and startindex = old startindex
	{
		Opendir_File(szPath);
		if(pszName != NULL)
		{
			for(int i = 0; i < m_nCurrentCount; i++)
			{
				if(strcmp(pszName, m_pszFileName[i]) == 0)
				{
					if(m_wItemCount > 0)
					{
						m_wStartItemIndex = i;
					}
					break;
				}	
			}
		}
	}
	Update();
}

void	CtmListView::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
    {
		case MSG_MOUSELUP:
			MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
       		break;
        case MSG_MOUSELDOWN:
        	MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        default:
        	break;
    }	
}

void	CtmListView::ShowColumns()
{
	RECT	rect;
	WORD	wLeftStart = 0;
	for(int i = 0; i < m_wColCount; i++)
	{
		if(m_pColumns_Item[i] == NULL)
		{
			m_pColumns_Item[i] = new CtmItem();
			m_pColumns_Item[i]->SetPropertys();
			m_pColumns_Item[i]->SetParent(this);
		}
		m_pColumns_Item[i]->SetPropValueT("type", 2);
		m_pColumns_Item[i]->SetPropValueT("static_wtype", 	0x22);
		m_pColumns_Item[i]->SetPropValueT("fgc", 	m_Columns[i].fgc);
		m_pColumns_Item[i]->SetPropValueT("bgc", 	m_Columns[i].bgc);
		//m_pColumns_Item[i]->SetPropValueT("fontsize", 	m_Columns[i].wFontSize);
		m_pColumns_Item[i]->SetPropValueT("align", 	m_wAlign);

		rect.left	=	wLeftStart;
		rect.right	= 	rect.left + m_Columns[i].wWidth;
		rect.top	=	0;
		rect.bottom	=	m_wColHeight;
		wLeftStart += m_Columns[i].wWidth;
		
		m_pColumns_Item[i]->SetPropValueT("left", 	rect.left);
		m_pColumns_Item[i]->SetPropValueT("right", 	rect.right);
		m_pColumns_Item[i]->SetPropValueT("top", 	rect.top);
		m_pColumns_Item[i]->SetPropValueT("bottom", rect.bottom);
		m_pColumns_Item[i]->SetPropValueT("textwidth", m_Columns[i].wWidth);
		m_pColumns_Item[i]->SetPropValueT("itemheight", m_wColHeight);
		
		m_pColumns_Item[i]->CreateA();
		m_pColumns_Item[i]->Show();
		if(m_Columns[i].pszTextID != NULL)
			m_pColumns_Item[i]->SetPropValueT("textID", m_Columns[i].pszTextID);
		else
			m_pColumns_Item[i]->SetPropValueT("text", m_Columns[i].pszText);
		m_pColumns_Item[i]->Update();	
	}
}

void	CtmListView::ShowItems()
{
	RECT	rect;
	WORD	wLeftStart = 0, wTopStart = m_wColHeight;
	char	szSelectName[512];
	if(m_wItemCount > MAX_ITEM_COUNT)
		m_wItemCount = MAX_ITEM_COUNT;
	
	for(int i = 0; i < m_wColCount; i++)
	{
		wTopStart = m_wColHeight;
		//if(i == 0)
		{
			//for(int j = m_wStartItemIndex; j < m_nCurrentCount; j++)
			for(int j = 0; j < m_wItemCount; j++)
			{
				if(m_pContext_Item[i][j] == NULL)
				{
					m_pContext_Item[i][j] = new CtmItem();
					m_pContext_Item[i][j]->SetPropertys();
					m_pContext_Item[i][j]->SetParent(this);
				}
				m_pContext_Item[i][j]->SetPropValueT("image_style", 2);
				m_pContext_Item[i][j]->SetPropValueT("image_wtype", 	0x30);
				
				//Focus Row
				//printf("m_wFocusIndex=%d start=%d j=%d count=%d \n", m_wFocusIndex, m_wStartItemIndex, j, m_wFocusIndex + m_wStartItemIndex);
				m_pContext_Item[i][j]->SetPropValueT("fgc", 	m_Columns[i].fgc);
				m_pContext_Item[i][j]->SetPropValueT("bgc", 	m_Columns[i].bgc);
				m_pContext_Item[i][j]->SetPropValueT("static_wtype", 	0x30);
				m_pContext_Item[i][j]->SetPropValueT("align", 	m_wAlign);
				
				//因為左邊要顯示文件屬性圖片，所以要特殊處理
				if(i == 0)
				{
					m_pContext_Item[i][j]->SetPropValueT("imagewidth", 	m_wItemImageWidth);
					m_pContext_Item[i][j]->SetPropValueT("keycolor", m_wItemKeyColor);
				}
				m_pContext_Item[i][j]->SetPropValueT("textwidth", m_Columns[i].wWidth);
				m_pContext_Item[i][j]->SetPropValueT("itemheight", m_wColHeight);
				rect.left	=	wLeftStart;
				if(i == 0)
					rect.right	= 	rect.left + 16 + m_Columns[i].wWidth;
				else
					rect.right	= 	rect.left + m_Columns[i].wWidth;
				rect.top	=	wTopStart;
				rect.bottom	=	rect.top + m_wColHeight;
				wTopStart += m_wColHeight;
				
				m_pContext_Item[i][j]->SetPropValueT("left", 	rect.left);
				m_pContext_Item[i][j]->SetPropValueT("right", 	rect.right);
				m_pContext_Item[i][j]->SetPropValueT("top", 	rect.top);
				m_pContext_Item[i][j]->SetPropValueT("bottom", 	rect.bottom);
			
				m_pContext_Item[i][j]->CreateA();
				m_pContext_Item[i][j]->Show();
				m_pContext_Item[i][j]->Update();
				
				if((j + m_wStartItemIndex) >= m_nCurrentCount)
				{
					char pszPath[256] = "\0";
					if(i == 0)
						m_pContext_Item[i][j]->SetPropValueT("item_imagepath", pszPath);
					m_pContext_Item[i][j]->SetPropValueT("text", pszPath);
					m_pContext_Item[i][j]->SetPropValueT("fgc", 	m_Columns[i].fgc);
					m_pContext_Item[i][j]->SetPropValueT("bgc", 	m_Columns[i].bgc);
					m_pContext_Item[i][j]->SetPropValueT("static_wtype", 	0x30);
				}
				else
				{
					if(i == 0)
					{
						switch(m_wItemType[i][j+m_wStartItemIndex])
						{
							case LISTVIEW_FILE:
								m_pContext_Item[i][j]->SetPropValueT("item_imagepath", m_cImagePath_file);
								break;
							case LISTVIEW_DIR:
								m_pContext_Item[i][j]->SetPropValueT("item_imagepath", m_cImagePath_dir);
								break;
							case LISTVIEW_BACK:
								m_pContext_Item[i][j]->SetPropValueT("item_imagepath", m_cImagePath_back);
								break;
							default:
								m_pContext_Item[i][j]->SetPropValueT("item_imagepath", m_cImagePath_file);
								break;	
						}
					}
					if(m_pszItemIDShow[i][j] != NULL)
						m_pContext_Item[i][j]->SetPropValueT("textID", m_pszItemIDShow[i][m_wStartItemIndex+j]);
					else
						m_pContext_Item[i][j]->SetPropValueT("text", m_pszItemShow[i][m_wStartItemIndex+j]);
					
					if(m_wFocusIndex == j)
					{
						BOOL	bSelect = FALSE;
						if(m_bGetFocus)
						{
							m_pContext_Item[0/*i*/][j]->SetPropValueT("fgc", 	m_Columns[i].fgc);	//2013/3/20 modify
							m_pContext_Item[0/*i*/][j]->SetPropValueT("bgc", 	m_wFocusColor);		//2013/3/20 modify
							m_pContext_Item[0/*i*/][j]->SetPropValueT("static_wtype", 	0x28);		//2013/3/20 modify
						}
						else
						{
							m_pContext_Item[i][j]->SetPropValueT("fgc", 	m_Columns[i].fgc);
							
							if(m_bSelectUse)
							{
								for(int nA = 0; nA < m_wMaxSelectItemCount; nA++)
								{
									memset(szSelectName, 0, sizeof(szSelectName));
									sprintf(szSelectName, "%s%s", m_pszCurrentPath, m_pszFileName[j+ m_wStartItemIndex]);
									if(m_wszSelectFile[nA][0] != '\0' && (strcmp(m_wszSelectFile[nA], szSelectName) == 0))
									{
										bSelect = TRUE;
										break;	
									}
								}
							}
							if(bSelect == TRUE)
								m_pContext_Item[i][j]->SetPropValueT("bgc", 	m_wSelectColor);
							else
								m_pContext_Item[i][j]->SetPropValueT("bgc", 	m_wLostFocusColor);
							if(m_wLostFocusColor == 0 && bSelect == FALSE)
								m_pContext_Item[i][j]->SetPropValueT("static_wtype", 	0x30);
							else
								m_pContext_Item[i][j]->SetPropValueT("static_wtype", 	0x28);
						}
					}
					else
					{
						BOOL	bSelect = FALSE;
						//如果選擇複選文件，則從列表中找出  路徑+文件名一致的文件，標識背景色
						if(m_bSelectUse)
						{
							for(int nA = 0; nA < m_wMaxSelectItemCount; nA++)
							{
								memset(szSelectName, 0, sizeof(szSelectName));
								sprintf(szSelectName, "%s%s", m_pszCurrentPath, m_pszFileName[j+ m_wStartItemIndex]);
								if(m_wszSelectFile[nA][0] != '\0' && (strcmp(m_wszSelectFile[nA], szSelectName) == 0))
								{
									bSelect = TRUE;
									break;	
								}
							}
						}
						if(bSelect)
						{
							//選中
							m_pContext_Item[i][j]->SetPropValueT("fgc", 	m_Columns[i].fgc);
							m_pContext_Item[i][j]->SetPropValueT("bgc", 	m_wSelectColor);
							m_pContext_Item[i][j]->SetPropValueT("static_wtype", 	0x28);	
						}
						else
						{	
							//未選中
							m_pContext_Item[i][j]->SetPropValueT("fgc", 	m_Columns[i].fgc);
							m_pContext_Item[i][j]->SetPropValueT("bgc", 	m_Columns[i].bgc);
							m_pContext_Item[i][j]->SetPropValueT("static_wtype", 	0x30);
						}
					}
				}
				m_pContext_Item[i][j]->Show();
				m_pContext_Item[i][j]->Update();
			}
		}
		wLeftStart += m_Columns[i].wWidth;
	}
	ShowScrollbar();
}

void		CtmListView::ShowScrollbar()
{
	if (m_bBar)
	{
		SetScrollbar(m_nCurrentCount);
		UpdateScrollbar();
	}
}

void		CtmListView::UpdateScrollbar()
{
	if (m_bBar)
		m_scrollbar.Update();
}

void		CtmListView::SetInitScrollbar()
{
	m_scrollbar.SetPropertys();
	
	//	===========================
	//	set the scrollbar parent
	//	===========================
	m_scrollbar.SetParent(this);
	
	//	===========================
	//	the scrollbar position and size
	//	===========================
	m_scrollbar.SetPropValueT("IncreType", 2);		//James add 2013/2/1 上午 10:24:30
	m_scrollbar.SetPropValueT("left", 	m_wnd.rect.right - m_wnd.rect.left );
	m_scrollbar.SetPropValueT("top", 	double(0) /*+ m_wColHeight*/);
	m_scrollbar.SetPropValueT("right", 	m_wnd.rect.right - m_wnd.rect.left + 20);
	m_scrollbar.SetPropValueT("bottom",	m_wnd.rect.bottom - m_wnd.rect.top /*- m_wColHeight*/);
	//	===========================
	//	the scrollbar type
	//	===========================
	m_scrollbar.SetPropValueT("fgc",	m_fgc);
	m_scrollbar.SetPropValueT("bgc",	42260);
	//printf("mbgc=%d \n", m_bgc);
	m_scrollbar.CreateA();	
}


char	g_pszPath[512] = "\0";
void   		CtmListView::Clear()
{
	RECT	rect;
	WORD	wLeftStart = 0, wTopStart = m_wColHeight;
	
	for(int i = 0; i < m_wColCount; i++)
	{
		wTopStart = m_wColHeight;
		for(int j = 0; j < m_wItemCount; j++)
		{
			if(m_pContext_Item[i][j] == NULL)
			{
				m_pContext_Item[i][j] = new CtmItem();
				m_pContext_Item[i][j]->SetPropertys();
				m_pContext_Item[i][j]->SetParent(this);
			}
			
			m_pContext_Item[i][j]->SetPropValueT("fgc", 	m_Columns[i].fgc);
			m_pContext_Item[i][j]->SetPropValueT("bgc", 	m_Columns[i].bgc);
			m_pContext_Item[i][j]->SetPropValueT("static_wtype", 	0x30);
			
			m_pszItemShow[i][j] = NULL;
			m_pszItemIDShow[i][j] = NULL;
			m_wItemType[i][j] 	= 0;
			
			m_pContext_Item[i][j]->SetPropValueT("item_imagepath", g_pszPath);
			m_pContext_Item[i][j]->SetPropValueT("text", g_pszPath);
			m_pContext_Item[i][j]->CreateA();
			m_pContext_Item[i][j]->Show();
			m_pContext_Item[i][j]->Update();
		}
		wLeftStart += m_Columns[i].wWidth;
	}
}

void		CtmListView::SetScrollbar(WORD wRange)
{
	int nRange = wRange-1;//wRange - (m_wnd.rect.bottom - m_wnd.rect.top) / (m_pcFont->m_wHeight + 1);
	//printf("nRange=%d wRange=%d m_wFocusIndex=%d m_wStartItemIndex=%d bottom=%d, top=%d \n", nRange, wRange, m_wFocusIndex, m_wStartItemIndex,
	//m_wnd.rect.bottom, m_wnd.rect.top);
	if (nRange < 0) nRange = 0;
	m_scrollbar.SetMaxReg(nRange+1);
	m_scrollbar.SetPos(m_wFocusIndex+m_wStartItemIndex);
}

void    	CtmListView::OnGetFocus()
{
	m_bGetFocus = true;
	SetSelectUse(0);
	ShowItems();
}

void    	CtmListView::OnLoseFocus()
{
	m_bGetFocus = false;
	ShowItems();
}

WORD		CtmListView::GetMaxItems()
{
	return m_wMaxItemCount;
}

int			CtmListView::GetItemCount()
{
	return	m_nCurrentCount;	
}

void		CtmListView::SetItemCount(WORD	wCount)
{
	m_wItemCount = wCount;
}

void		CtmListView::SetSelectUse(BOOL	bUse)
{
	if(bUse)
	{
		//m_bSelectUse = TRUE;
		m_wMaxSelectItemCount = MAX_SELECT_ITEM;
	}
	else
	{
		//m_bSelectUse = FALSE;
		m_wMaxSelectItemCount = 1;
		//memset(m_wSelectIndex, 0, sizeof(m_wSelectIndex));	
		memset(m_wszSelectFile, 0, sizeof(m_wszSelectFile));	
	}	
}

char*		CtmListView::GetSelectFileName(int nIndex)
{
	if(nIndex >=0 && nIndex < m_wMaxSelectItemCount)
		return 	m_wszSelectFile[nIndex];
	else 
		return NULL;
}

BOOL		CtmListView::SetHideFileName(int nIndex, char*	pszFileName)
{
	int	nStrLen = 0;
	if(nIndex >=0 && nIndex <= MAX_HIDEFILE_COUNT && pszFileName != NULL)
	{
		nStrLen = strlen(pszFileName);
		if(nStrLen < 512)
		{
			if(m_pszHidFileName[nIndex] == NULL)
			{
				m_pszHidFileName[nIndex] = new char[512];
				if(m_pszHidFileName[nIndex] == NULL)
					return FALSE;
			}
			memset(m_pszHidFileName[nIndex], 0, sizeof(m_pszHidFileName[nIndex]));
			strcpy(m_pszHidFileName[nIndex], pszFileName);
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

void	CtmListView::SetItemsStr(WORD	wColIndex, WORD	wRowIndex, char*	pszStr, WORD wType)
{
	if(wColIndex < MAX_COLUMNS_COUNT)
	{
		if(wRowIndex < MAX_ITEM_COUNT)
		{
			m_pszItemShow[wColIndex][wRowIndex] = pszStr;
			m_wItemType[wColIndex][wRowIndex] 	= wType;
		}
	}
}

void	CtmListView::SetItemsIDStr(WORD	wColIndex, WORD	wRowIndex, char*	pszStr, WORD wType)
{
	if(wColIndex < MAX_COLUMNS_COUNT)
	{
		if(wRowIndex < MAX_ITEM_COUNT)
		{
			m_pszItemIDShow[wColIndex][wRowIndex] = pszStr;
			m_wItemType[wColIndex][wRowIndex] 	= wType;
		}
	}
}

//==============================================================================================
//	File Manage 
//==============================================================================================
void 	CtmListView::Print_dir_info(struct stat statbuf,char *name)
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
	printf("%10.10s",Sperm(statbuf.st_mode));	//type,permissions
	printf("%4d",statbuf.st_nlink);				//number of links
	
	if((pwd=getpwuid(statbuf.st_uid)) != NULL)	//owner's name
		printf(" %-8.8s \n",pwd->pw_name);
	else
		printf(" %-8d",statbuf.st_uid);
        
	if((grp = getgrgid(statbuf.st_gid)) != NULL)//group name
	    printf(" %-8.8s",grp->gr_name);	
	else
	    printf(" %-8d",statbuf.st_gid);

	printf(" %9jd",(intmax_t)statbuf.st_size);	//size
        
    tm=localtime(&statbuf.st_mtime);			//modify time
    sprintf(datestring,"%d-%d-%d %d:%d:%d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
  	//printf(" %-20.20s %-s\n",datestring,name);		
}

void 	CtmListView::Print_dir(LinkNode head)
{
    LinkNode temp;
	temp = head;
	while(temp->next)
	{
		temp = temp->next;
		Print_dir_info(temp->sb,temp->filename);
		//printf("Print_dir %-s\n",temp->filename);
	}
}

int 	CtmListView::Opendir_File(char*	pszDir, int nFlag, char* pszName,char* pszName2,int nLen,char* pszExRegDir)
{
    DIR * dir;				//文件結構體
    char tempFileName[256];
    memset(tempFileName,0,sizeof(tempFileName));
    if(pszName!=NULL)
    {
    	memset(m_szKeyWord1,0,sizeof(m_szKeyWord1));
		memcpy(m_szKeyWord1,pszName,strlen(pszName));	
    }
    if(pszName2!=NULL)
    {
    	memset(m_szKeyWord2,0,sizeof(m_szKeyWord2));
		memcpy(m_szKeyWord2,pszName2,strlen(pszName2));	
    }
    if(nLen>0)
    {
    	m_nFileNameLen=nLen;
    }
    if(pszExRegDir!=NULL)
    {
    	if(m_pszExRegDir!=NULL)
    	{
    		free(m_pszExRegDir);
    		m_pszExRegDir=NULL;	
    	}
   		m_pszExRegDir=(char*)malloc(strlen(pszExRegDir)+2);
   		memset(m_pszExRegDir,0,strlen(pszExRegDir)+2);
   		memcpy(m_pszExRegDir,pszExRegDir,strlen(pszExRegDir));
   		if(m_pszExRegDir[strlen(m_pszExRegDir)-1]!='/');
   				m_pszExRegDir[strlen(m_pszExRegDir)]='/';
   	}
   	//printf("pszExRegDir1=%s\n",m_pszExRegDir);
    int	nIndex = 0, nFileIndex = 0, nDirIndex = 0;
    char path[512];
    char cPath_C[512] = ".", cPath_B[512] = "..";
	char datestring[512], sizestring[512];
    struct tm *tm;
   
   	umask(0);
    Free_Node();
	m_wFocusIndex 		= 0;
	m_wStartItemIndex	= 0;
	
	m_nCurrentFileCount		= nFileIndex;
	m_nCurrentDirectCount 	= nDirIndex;
	m_nCurrentCount 		= nIndex;
	if((nFlag&0xFF) == 1)
	{
		   if(m_szKeyWord1[0]!='\0')
		   {
		   		if(pszName==NULL)
		   		{
		   			pszName=m_szKeyWord1;	
		   		}	
		   }
		   if(m_szKeyWord2[0]!='\0')
		   {
		   		 if(pszName2==NULL)
		   		 {
		   		 	pszName2=m_szKeyWord2;	
		   		 }
		   }
		   if(nLen<=0)
    	   {
    			nLen=m_nFileNameLen;	
   		   }
		//printf("pszDir=%s m_pszCurrentPath=%s pszExRegDir=%s\n", pszDir, m_pszCurrentPath,pszExRegDir);
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
	}
	else
	{
		memset(m_pszCurrentPath, 0, sizeof(m_pszCurrentPath));
		strcpy(m_pszCurrentPath, pszDir);	
	}
				
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
	
	if(stat(path,&m_sb))
	{
		perror("dir stat");
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
	if(S_ISREG(m_sb.st_mode)> 0)  //is a file , not dir
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
	
	//printf("open dir \n");
    if((m_sb.st_mode & S_IFMT) == S_IFDIR)
    {
    	//dir
		if((dir=opendir(path)) == NULL)
	    {
		    return 1;
	    }
		//printf("dir=%p path=%s \n", dir,path);
        m_dir_head=(LinkNode)malloc(sizeof(Node));
		memset(m_dir_head, 0, sizeof(m_dir_head));
		m_dir_head->next = NULL;//init list head
        strcpy(temppath,path);    
	    
	    m_file_head=(LinkNode)malloc(sizeof(Node));
		memset(m_file_head, 0, sizeof(m_file_head));
		m_file_head->next = NULL;//init list head
        //strcpy(temppath,path);    
	    
	    while((m_ptr = readdir(dir)) != NULL)
	    {
            	if(strcmp(m_ptr->d_name, cPath_C) != 0 &&
            		strcmp(m_ptr->d_name, cPath_B) != 0 )
           	 {
			if(m_ptr->d_type!=DT_DIR)
			{
					
	            if(pszName != NULL)
				{
					if(LISTVIEW_NCASE==(nFlag&0xFF00))
					{
						for(int i=0; i<strlen(pszName);++i)
						{
							m_szKeyWord1[i]=toupper(pszName[i]);	
						}
						memset(tempFileName,0,sizeof(tempFileName));
						for(int i=0; i<strlen(m_ptr->d_name);++i)
						{
							tempFileName[i]=toupper(m_ptr->d_name[i]);	
						}
					 }
					 char* pszLastKey=strrchr(tempFileName, m_szKeyWord1[0]);
					 if(pszLastKey!=NULL)
					 {
							if(strcmp(pszLastKey, m_szKeyWord1))
							{
								strcpy(temppath,path);
								continue;
							//nHave = FALSE;
							}
							else
							{
								if(pszName2!=NULL)
								{
									if(LISTVIEW_NCASE==(nFlag&0xFF00))
									{
										for(int i=0; i<strlen(pszName2);++i)
										{
											m_szKeyWord2[i]=toupper(pszName2[i]);	
										}
									}
									if(!strstr(tempFileName,m_szKeyWord2))
									{
										strcpy(temppath,path);
										continue;	
									}
								}
							}	
						}
						else
						{
							strcpy(temppath,path);
							continue;	
						}
					}
			}
				else if(m_ptr->d_type==DT_DIR&&(1==m_wShowType))
				{
					continue;	
				}
				strcat(temppath,m_ptr->d_name);
		 }
            strcat(temppath,"\0"); 
            printf("temppath=%s \n", temppath);
			if(lstat(temppath,&m_sb) == -1)
			{
				perror("stat");
				strcpy(temppath,path);
				//closedir(dir);
				//printf("close \n");
				continue;
		        //return 0;
			}
			
			//check hide file name
			BOOL	bShow = TRUE;
			 if(nLen>0)
            {
            	if(m_ptr->d_name!=NULL)	
            	{
            		if(strlen(m_ptr->d_name)>nLen)
            		{
            			bShow=FALSE;	
            		}
            		
            	}
            }
			for(int i = 0; i < MAX_HIDEFILE_COUNT; i++)
			{
				if(m_pszHidFileName[i] != NULL)
				{	
					if(strncmp(m_ptr->d_name, m_pszHidFileName[i], strlen(m_pszHidFileName[i])) == 0)	//Don't list the file name begin of "O9" or "MCANG"	2012/8/20 add "MCANG"
					{
						bShow = FALSE;
						break;
					}
					
					//2013/2/28 <<<<< unallowed goto root directory
					// when local in /mnt/usb/ ,unallawed goto .. directory,  Wong modify 2014/11/14 19:09:37
					if(strcmp(m_pszCurrentPath,m_pszExRegDir) == 0)
					{		
						if(strncmp(m_ptr->d_name, "..",2) == 0)
						{
							bShow = FALSE;
							break;
						}
					}
					//2013/2/28 >>>>>>
				}
			}
			if(strcmp(m_pszCurrentPath,m_pszExRegDir) == 0)
			{		
				if(strncmp(m_ptr->d_name, "..",2) == 0)
				{
					bShow = FALSE;
				}
			}
			if(bShow)
			{
				if((m_sb.st_mode & S_IFMT) == S_IFDIR)
				{
		            m_dir_node=(LinkNode)malloc(sizeof(Node));//init new_node
					if(m_dir_node == NULL){
						closedir(dir);
						return 0;	 
					}
					//set value to new_node
					memset(m_dir_node, 0, sizeof(m_dir_node));
					strcpy(m_dir_node->filename, m_ptr->d_name);
					m_dir_node->sb = m_sb;
					m_dir_node->next = NULL;
					//printf("dir m_ptr->d_name=%s \n", m_ptr->d_name);
					Add_sort(m_dir_node,m_dir_head);
				}
				else
				//if(S_ISREG(m_sb.st_mode) > 0) //is a file
				{
	          		m_file_node=(LinkNode)malloc(sizeof(Node));//init new_node
					if(m_file_node == NULL){
						closedir(dir);
						return 0;
					}
					//set value to new_node
					//m_file_node->filename = m_ptr->d_name;
					memset(m_file_node, 0, sizeof(m_file_node));
					strcpy(m_file_node->filename, m_ptr->d_name);
					m_file_node->sb = m_sb;
					m_file_node->next = NULL;
					//printf("file m_ptr->d_name=%s \n", m_ptr->d_name);
					Add_sort(m_file_node,m_file_head);
				}
			}
			strcpy(temppath,path);
	    }
	    //Print_dir(m_head);
	    //pszExRegDir1=NULL;
	    AddDirToList(m_dir_head);
	    AddFileToList(m_file_head);
		closedir(dir);
	}
	else
	{
		//file
        ;//Print_dir_info(m_sb,m_ptr->d_name);
	}
	//printf("3 m_pszCurrentPath=%s \n", m_pszCurrentPath);
	return 0;
}

void	CtmListView::AddDirToList(LinkNode head)
{
	LinkNode temp;
	temp 	= head;
	int		nIndex = m_nCurrentCount;
    int		nFileIndex = 0, nDirIndex = 0;
	char datestring[512], sizestring[512];
    struct tm *tm;
	
	while(temp->next)
	{
		temp = temp->next;
		
		if(nIndex < MAX_ITEM_COUNT)
		{
			if(m_pszShow[nIndex] != NULL)
			;//free(m_pszShow[nIndex]);
			else
				m_pszShow[nIndex]  = (char*)malloc(512);
			
			if(m_pszFileName[nIndex] != NULL)
				;
			else
				m_pszFileName[nIndex] = (char*)malloc(512);
			
			memset(m_pszFileName[nIndex], 0, sizeof(m_pszFileName[nIndex]));
			memset(m_pszShow[nIndex], 0, sizeof(m_pszShow[nIndex]));
			//printf("dir filename=%s \n", temp->filename);
			
			if(strcmp(temp->filename, "..") == 0)
			{
				strcpy(m_pszFileName[nIndex], temp->filename);
				CodeChange(m_pszShow[nIndex], temp->filename);
				SetItemsStr(0, nIndex, m_pszShow[nIndex], LISTVIEW_BACK); //James add 2013/1/28 下午 03:32:17
				m_bPropertyChanged = TRUE;
			}
			else if(strcmp(temp->filename, ".") == 0)
			{
				continue;
			}
			else
			{
				strcpy(m_pszFileName[nIndex], temp->filename);
				CodeChange(m_pszShow[nIndex], temp->filename);
				SetItemsStr(0, nIndex, m_pszShow[nIndex], LISTVIEW_DIR); //James add 2013/1/28 下午 03:32:17
				m_bPropertyChanged = TRUE;
				nDirIndex++;
			}
			m_nCurrentDirectCount 	= nDirIndex;
			m_nCurrentCount			= nIndex;
			
			//set file size
			if(m_pszSizeShow[nIndex] != NULL)
				;//free(m_pszShow[nIndex]);
			else
				m_pszSizeShow[nIndex]  = (char*)malloc(512);
			memset(m_pszSizeShow[nIndex], 0, sizeof(m_pszSizeShow[nIndex]));
			SetItemsStr(1, nIndex, m_pszSizeShow[nIndex], LISTVIEW_DIR); //James add 2013/1/28 下午 03:32:17
			
			//set local time
			tm=localtime(&(temp->sb.st_mtime));			//modify time
			if(m_pszDataShow[nIndex] != NULL)
				;//free(m_pszShow[nIndex]);
			else
				m_pszDataShow[nIndex]  = (char*)malloc(512);
			
			memset(datestring, 0, sizeof(datestring));
			memset(m_pszDataShow[nIndex], 0, sizeof(m_pszDataShow[nIndex]));
			sprintf(datestring,"%d-%d-%d %d:%d:%d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
			CodeChange(m_pszDataShow[nIndex], datestring);
			SetItemsStr(2, nIndex, m_pszDataShow[nIndex], LISTVIEW_DIR); //James add 2013/1/28 下午 03:32:17	
		}
		nIndex++;
		m_nCurrentCount ++;
	}
}


void	CtmListView::AddFileToList(LinkNode head)
{
	LinkNode temp;
	temp 	= head;
	int		nIndex = m_nCurrentCount;
    int		nFileIndex = 0, nDirIndex = 0;
	char datestring[512], sizestring[512];
    struct tm *tm;
	
	while(temp->next)
	{
		temp = temp->next;
		
		if(nIndex < MAX_ITEM_COUNT)
		{
			if(m_pszShow[nIndex] != NULL)
				;//free(m_pszShow[nIndex]);
			else
				m_pszShow[nIndex]  = (char*)malloc(512);
			
			if(m_pszFileName[nIndex] != NULL)
				;
			else
				m_pszFileName[nIndex] = (char*)malloc(512);
			
			memset(m_pszFileName[nIndex], 0, sizeof(m_pszFileName[nIndex]));
			memset(m_pszShow[nIndex], 0, sizeof(m_pszShow[nIndex]));
			
			//printf("m_ptr->d_name=%s \n", m_ptr->d_name);
			strcpy(m_pszFileName[nIndex], temp->filename);
			CodeChange(m_pszShow[nIndex], temp->filename);
			SetItemsStr(0, nIndex, m_pszShow[nIndex], LISTVIEW_FILE); //James add 2013/1/28 下午 03:32:17
			m_bPropertyChanged = TRUE;
			nFileIndex ++;
			
			//set file size
			if(m_pszSizeShow[nIndex] != NULL)
				;//free(m_pszShow[nIndex]);
			else
				m_pszSizeShow[nIndex]  = (char*)malloc(512);
			memset(sizestring, 0, sizeof(sizestring));
			memset(m_pszSizeShow[nIndex], 0, sizeof(m_pszSizeShow[nIndex]));
			sprintf(sizestring, "%d KB", temp->sb.st_size/1024 + 1);
			CodeChange(m_pszSizeShow[nIndex], sizestring);
			SetItemsStr(1, nIndex, m_pszSizeShow[nIndex], LISTVIEW_FILE); //James add 2013/1/28 下午 03:32:17
    		
			//set local time
			tm=localtime(&(temp->sb.st_mtime));			//modify time
			if(m_pszDataShow[nIndex] != NULL)
				;//free(m_pszShow[nIndex]);
			else
				m_pszDataShow[nIndex]  = (char*)malloc(512);
			
			memset(datestring, 0, sizeof(datestring));
			memset(m_pszDataShow[nIndex], 0, sizeof(m_pszDataShow[nIndex]));
    		sprintf(datestring,"%d-%d-%d %d:%d:%d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
			CodeChange(m_pszDataShow[nIndex], datestring);
    		SetItemsStr(2, nIndex, m_pszDataShow[nIndex], LISTVIEW_FILE); //James add 2013/1/28 下午 03:32:17
		}
		nIndex++;
		m_nCurrentCount ++;
	}
}

char*		CtmListView::GetEachFileName(int nFileIndex)		//2013/2/25 add
{
	//printf("nFileIndex=%d m_nCurrentFileCount=%d \n",nFileIndex,m_nCurrentFileCount);
	return 	m_pszFileName[nFileIndex];
}

char*		CtmListView::GetCurrentPath()
{
	return 	m_pszCurrentPath;
}

char*		CtmListView::GetCurrentSelectName()
{
	return m_pszFileName[m_wFocusIndex+m_wStartItemIndex];	
}

WORD		CtmListView::GetCurrentSelectType()
{
	return m_wItemType[0][m_wFocusIndex+m_wStartItemIndex];	
}

void 		CtmListView::Add_sort(LinkNode new_node,LinkNode head)
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

char* 		CtmListView::Sperm(mode_t mode)
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
	if(S_ISDIR(mode) == 0)
	{
		;//printf("is file \n");	
	}
	else
	{
		;//printf("is direction \n");	
	}
	temp[10] = '\0';
	return temp;
}

int		CtmListView::Free_Node()
{
	LinkNode pTemp;
	while(m_file_head != NULL)
	{
		pTemp = m_file_head;
		m_file_head = m_file_head->next;
		if(pTemp != NULL)
		{
			free(pTemp);
			pTemp = NULL;
		}
	}
	while(m_dir_head != NULL)
	{
		pTemp = m_dir_head;
		m_dir_head = m_dir_head->next;
		if(pTemp != NULL)
		{
			free(pTemp);
			pTemp = NULL;
		}
	}
	for(int i = 0; i < MAX_ITEM_COUNT; i++)
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
		if(m_pszDataShow[i] != NULL)
		{
			free(m_pszDataShow[i]);
			m_pszDataShow[i] = NULL;	
		}
		if(m_pszSizeShow[i] != NULL)
		{
			free(m_pszSizeShow[i]);
			m_pszSizeShow[i] = NULL;	
		}
	}
}

/*============================================================================
|Function	:	OpenNewDir(WORD	wFocusIndex, WORD	wStartItemIndex)
|Para		:	wFocusIndex:打開指定文件的FocusIndex，
|				wStartItemIndex:文件的start Items
|Other		:    
==============================================================================*/
void	CtmListView::OpenNewDir(WORD	wFocusIndex, WORD	wStartItemIndex)
{
	char	szDir[512];
	char	szSelectName[512];
	if( (wFocusIndex+wStartItemIndex) < MAX_ITEM_COUNT)
	{
		if(m_wItemType[0][wFocusIndex+wStartItemIndex] == LISTVIEW_DIR ||
			m_wItemType[0][wFocusIndex+wStartItemIndex] == LISTVIEW_BACK)
		{
			//打開下一級菜單
			//printf("m_pszFileName[wFocusIndex+wStartItemIndex]=%s \n", m_pszFileName[wFocusIndex+wStartItemIndex]);
			memset(szDir, 0, sizeof(szDir));
			strcpy(szDir, m_pszFileName[wFocusIndex+wStartItemIndex]);
			Clear();
			Opendir_File(szDir, 1);
		}
		else if(m_bSelectUse && m_wItemType[0][wFocusIndex+wStartItemIndex] == LISTVIEW_FILE)
		{
			//選中文件
			if(m_wMaxSelectItemCount == 1)
			{
				memset(szSelectName, 0, sizeof(szSelectName));
				sprintf(szSelectName, "%s%s", m_pszCurrentPath, m_pszFileName[wFocusIndex+wStartItemIndex]);
				memset(m_wszSelectFile[0], 0, sizeof(m_wszSelectFile[0]));
				strcpy(m_wszSelectFile[0], szSelectName);	
			}
			else
			{
				for(int i = 0; i < m_wMaxSelectItemCount; i++)
				{
					memset(szSelectName, 0, sizeof(szSelectName));
					sprintf(szSelectName, "%s%s", m_pszCurrentPath, m_pszFileName[wFocusIndex+wStartItemIndex]);
					if(m_wszSelectFile[i][0] != '\0' && (strcmp(m_wszSelectFile[i], szSelectName) == 0))
					{
						memset(m_wszSelectFile[i], 0, sizeof(m_wszSelectFile[i]));
						for(int j = 0; j < m_wMaxSelectItemCount - i-1; j++)
						{
							strcpy(m_wszSelectFile[i+j],m_wszSelectFile[i+j+1]);
							memset(m_wszSelectFile[i+j+1], 0, sizeof(m_wszSelectFile[i+j+1]));	
						}
						break;
					}
					else if(m_wszSelectFile[i][0] == '\0')
					{
						strcpy(m_wszSelectFile[i],szSelectName);	
						break;
					}
					//printf("m_wszSelectFile[%d ]=%s, m_pszFileName=%s \n", i, m_wszSelectFile[i], m_pszFileName[wFocusIndex+wStartItemIndex]);
				}
			}
		}
	}
}

BOOL	CtmListView::MouseDown(short x_pos,short y_pos,int key_flag)
{
	RECT    rect 		=GetWindowRECT();
  	WORD	wMaxRows	= m_wMaxItemCount;
	WORD	wStrsNum	= m_nCurrentCount-1;
	int 	i 			= _NULL_KEY;
	WORD	wResult	=0;
	if(x_pos > rect.right+2 || x_pos < rect.left-2 || y_pos > rect.bottom+2 || y_pos < rect.top-2)
 	{
 		
 		int j=0;
 		for (j = 0; j < GetControlCount(); j++)
		{
	    	RECT rect1    = Controls[j]->GetWindowRECT();
	    	if(x_pos > rect1.right+2 || x_pos < rect1.left-2 || y_pos > rect1.bottom+2 || y_pos < rect1.top-2)
	        	continue;
	        else
	        {
	        	if(!Controls[i]->GetVisible()||!Controls[i]->GetEnabled())
	        	{
	        		continue;	
	        	}		
	        }
		}
		if(j>=GetControlCount())
		{
			printf("here .........\n");
	 		//if(m_bGetFocus)
	 			_PutKeyTail(_ENTER);
	 	}
	 	return 1;
	}
	printf("wMaxRows=%d,wStrsNum=%d\n",wMaxRows,wStrsNum);
	for (i = 1; i <= wMaxRows; i++)
	{
	 	if (y_pos <=rect.top + (i+1) * m_wColHeight)
	 	{
	 			//printf("i=%d\n",i);
	 			break;
	 	}
	 }
	 	
	 if (i > (wStrsNum+1))
	 	return 1;
	 	
	 if (i > wMaxRows)
	 	i = wMaxRows;
	 	
	 if (m_wFocusIndex != i - 1)
	 {
			//printf("4...\n");
	 		m_wOldFocusIndex = m_wFocusIndex;
	 		m_wFocusIndex = i - 1;
	 		ShowItems();
	 }
 	return TRUE;
}

BOOL	CtmListView::MouseUp(short x_pos,short y_pos,int key_flag)
{
	RECT    rect;
 
	rect    = GetWindowRECT();
	
	if(x_pos > m_scrollbar.GetWindowRECT().left || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
    {
        return 1;
    }
 	
 	return TRUE;
}