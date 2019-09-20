/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmListView                                               	 |
|  Task     : CtmListView header file                                    	 |
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
#ifndef 	D_CTMLISTVIEW
#define		D_CTMLISTVIEW

#include 	"wnd.h"
#include	"imagebox.h"
#include	"static.h"
#include	"scrollbar.h"

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

#define		MAX_COLUMNS_COUNT			10					//定義列表最大列數
#define		MAX_ITEM_COUNT				1000				//定義列表最大行數
#define		MAX_SELECT_ITEM				5					//定義可選中最多文件個數
#define		MAX_HIDEFILE_COUNT			10					//定義最多可隱藏文件名個數

#define		LISTVIEW_FILE				0					//顯示圖標用-表示文件
#define		LISTVIEW_DIR				1					//顯示圖標用-表示文件夾
#define		LISTVIEW_BACK				2					//顯示圖標用-表示上一級目錄
#define		LISTVIEW_NCASE			0x0000
#define		LISTVIEW_CASE				0x0100
//鍊錶 - 讀取文件或文件夾排序用
typedef struct node
{
    char filename[256];
    char *TMFileName;
	struct stat sb;
	struct node* next;
}Node,*LinkNode;

//列表列數上的第一行屬性
typedef	struct	columns
{
	WORD	wWidth;
	WORD	wHeight;
	WORD	fgc;
	WORD	bgc;
	WORD	wType;
	WORD	wFontSize;
	char*	pszTextID;
	char*	pszText;
}COLUMNS;

class	CtmItem	:public CtmWnd
{
	DECLARE_DYNCREATE(CtmItem)
	
	public:
		CtmItem();
		~CtmItem();
	private:   
		CtmImageBox*	m_pImageBox;
		CtmStaticX2*	m_pStaticX2;
		
		tmIMAGEBOX	m_tagImageBox;					//圖片信息結構體
		WORD		m_wAlign;						//文字對齊方式
		char*		m_pszTextID;					//字串ID
		char*		m_pszText;						//字串
			
		WORD		m_wImageWidth;					//圖片寬度
		WORD		m_wTextWidth;					//文字寬度
		WORD		m_wItemHeight;					//Item高度
		WORD		m_wFgc;							//字體顏色
		WORD		m_wBgc;							//背景色
		WORD		m_Static_wType;					//static元件wtype
		
		RECT		m_RectImage;					//圖片坐標
		RECT		m_RectStatic;					//static坐標
	public:
		void    	Show();
		void    	Update();
		void   		Clear();
		BOOL 		CreateA();	
		void 		SetPropertys();
		WORD    	OnKey(WORD wKey);
		void		SetStaticAttr();
		void		SetImageBoxAttr();
		void		CalRECT();
};

/*============================================================================+
|            Function and Class prototype                                     |
+============================================================================*/
/*============================================================================+
|           Class declaration -- ScrollBar                                    |
+============================================================================*/
class CtmListView :public CtmWnd
{
	DECLARE_DYNCREATE(CtmListView)
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmListView();
		~CtmListView();
	
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:   
		CtmItem*	m_pColumns_Item[MAX_COLUMNS_COUNT];		//最上面的Title列表
		CtmItem*	m_pContext_Item[MAX_COLUMNS_COUNT][MAX_ITEM_COUNT];	//下面顯示內容列表
	   	COLUMNS		m_Columns[MAX_COLUMNS_COUNT];			//Columns列表
	   	WORD		m_wColHeight;
		
	    char*		m_pszItemShow[MAX_COLUMNS_COUNT][MAX_ITEM_COUNT];			// 顯示Item字串列表
		char*		m_pszItemIDShow[MAX_COLUMNS_COUNT][MAX_ITEM_COUNT];			// 顯示Item字串ID列表
		
	    char*		m_pszShow[MAX_ITEM_COUNT];				//顯示內容
		char*		m_pszIDShow[MAX_ITEM_COUNT];			//
		char*		m_pszDirName[MAX_ITEM_COUNT];			//文件夾名稱
		char*		m_pszFileName[MAX_ITEM_COUNT];			//文件名稱
	    char*		m_pszDataShow[MAX_ITEM_COUNT];			//文件日期
	    char*		m_pszSizeShow[MAX_ITEM_COUNT];			//文件大小
	    
		char*		m_pszHidFileName[MAX_HIDEFILE_COUNT];	//隱藏文件列表
		
		char		m_pszCurrentPath[MAX_ITEM_COUNT];		// 顯示當前路徑
		char* 		m_pszExRegDir;
		char		m_szKeyWord1[64];
		char		m_szKeyWord2[64];
		int			m_nFileNameLen;		
		WORD		m_wItemType[MAX_COLUMNS_COUNT][MAX_ITEM_COUNT];			//顯示Item類型，文件/文件夾/上一級文件夾
		
		WORD		m_wAlign;								//字串對齊方式
		
	   	WORD		m_wColCount;							//Columns個數
	   	WORD		m_wItemCount;							//Items個數
	   	WORD		m_wMaxItemCount;						//最大可容下的Items個數
	   	WORD		m_wMaxSelectItemCount;					//最大可選擇的Items個數
	   	char*		m_cImagePath_file;						//文件圖片路徑
	   	char*		m_cImagePath_dir;						//文件夾圖片路徑
	   	char*		m_cImagePath_back;						//上一級文件夾圖片路徑
		BOOL		m_bBar;									//右邊scrollbar 是否需要顯示
		CtmScrollBar	m_scrollbar;						//右邊scrollbar
		
		BOOL		m_bGetFocus;
		WORD		m_wStartItemIndex;						//起始Items Index 
		WORD		m_wFocusColor;							//光標Focus顏色
		WORD		m_wLostFocusColor;						//光標Lost Focus顏色
		WORD		m_wFocusIndex;							//光標Focus Index
		WORD		m_wOldFocusIndex;						//上一次Focus Index
		WORD		m_wSelectColor;							//選中Items的顏色
		WORD		m_wShowType;							//文件內容顯示方式 0= show direct  1= show file 2= show all
		int			m_nCurrentDirectCount;					//當前路徑下文件夾個數
		int			m_nCurrentFileCount;					//當前路徑下文件個數
		int			m_nCurrentCount;						//當前路徑一頁文件總數
		
		char		m_wszSelectFile[MAX_SELECT_ITEM][512];  //選中的文件路徑和文件名
		WORD		m_wSelectIndex[MAX_SELECT_ITEM];		//選中的Index列表
		BOOL		m_bSelectUse;
		
		WORD		m_wItemImageWidth;						//圖片寬度
		WORD		m_wItemKeyColor;						//圖片key color
		
		//File Manage
		struct 	dirent *m_ptr;
		struct 	stat 	m_sb;
    	LinkNode m_file_head,	m_file_node;
    	LinkNode m_dir_head,	m_dir_node;
	protected:
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	private:
	protected:
		void		ShowScrollbar();						//顯示右邊scrollbar
		void		UpdateScrollbar();						//刷新右邊scrollbar
		void		SetInitScrollbar();						//初始化右邊scrollbar
		void		SetScrollbar(WORD wRange);				//設置右邊scrollbar 範圍
	public:
		void    	Show();
		void    	Update();
		void		UpdateList(int	nType, char*	pszName = NULL);			//nType=0刷新當前文件夾列表  nType=1刷新當前文件夾列表，並指定跳到pszName的地方
		void   		Clear();
		BOOL 		CreateA();	
		void 		SetPropertys();
		WORD    	OnKey(WORD wKey);
		void    	DrawBorder();
		void		ShowColumns();
		void		ShowItems();							//顯示所有Items
		void   	 	OnGetFocus();
		void    	OnLoseFocus();				
		WORD		GetMaxItems();							//或者當前最多可容納的Items個數
		void		SetItemsStr(WORD	wColIndex, WORD	wRowIndex, char*	pszStr, WORD wType);		//設置Items 內容
		void		SetItemsIDStr(WORD	wColIndex, WORD	wRowIndex, char*	pszStr, WORD wType);		//設置Items ID內容	
		void		SetItemCount(WORD	wCount);
		void		ShowStrs(WORD wOldFocusIndex, WORD wNewFocusIndex);
		BOOL		MouseUp(short x_pos,short y_pos,int key_flag);
		BOOL	 	MouseDown(short x_pos,short y_pos,int key_flag);
		//File Manage
		int 		Opendir_File(char*	pszDir, int nFlag = 0, char* pszName = NULL, char* pszName2=NULL,int nLen=0,char* pszExRegDir=NULL);					//打開文件夾列表
		void 		Add_sort(LinkNode new_node,LinkNode head);											//按順序排序
		void 		Print_dir_info(struct stat statbuf,char *name);										//列印文件夾裡面的信息
		void 		Print_dir(LinkNode head);															//列印文件夾
		char* 		Sperm(mode_t mode);																
		int			Free_Node();																		//釋放內存
		char*		GetEachFileName(int nFileIndex);													//獲得當前路徑下各文件名稱			//2013/2/25 add
		char*		GetCurrentPath();																	//獲得當前路徑
		char*		GetCurrentSelectName();																//獲取當前選中的文件名稱
		WORD		GetCurrentSelectType();																//獲取當前選中的文件類型  文件/文件夾
		int			GetItemCount();																		//獲取當前路徑一頁文件個數
		void		SetSelectUse(BOOL	bUse);															//設置可複選多個文件
		char*		GetSelectFileName(int nIndex);														//讀取選中的文件名稱
		BOOL		SetHideFileName(int nIndex, char*	pszFileName);									//設置隱藏文件名稱
		
		void		OpenNewDir(WORD	wFocusIndex, WORD	wStartItemIndex);								//打開文件夾
		void		AddDirToList(LinkNode head);														//將文件夾名稱加入列表
		void		AddFileToList(LinkNode head);														//將文件名稱加入列表
		
	public:
		void		WndProc(int message, WPARAM wParam, LPARAM lParam);
};

#endif
