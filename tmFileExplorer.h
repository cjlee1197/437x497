/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmFileExplorer                                                |
|  Task     : CtmFileExplorer header file                                    |
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
#ifndef 	D_CTMFILEEXPLORER
#define		D_CTMFILEEXPLORER

#include 	"wnd.h"
#include	"scrollbar.h"


const	int	CNT_INT_FILEMEMO_MAX	= 100;
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

char temppath[PATH_MAX];

typedef struct node
{
    char *filename;
    char *TMFileName;
	struct stat sb;
	struct node* next;
}Node,*LinkNode;

/*============================================================================+
|            Function and Class prototype                                     |
+============================================================================*/
/*============================================================================+
|           Class declaration -- ScrollBar                                    |
+============================================================================*/
class CtmFileExplorer :public CtmWnd
{
	DECLARE_DYNCREATE(CtmFileExplorer)
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmFileExplorer();
		~CtmFileExplorer();
	
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:   
	    char*	m_pszShow[CNT_INT_FILEMEMO_MAX];		//	fans add,
		char*	m_pszIDShow[CNT_INT_FILEMEMO_MAX];		//	fans add,
		char*	m_pszDirName[CNT_INT_FILEMEMO_MAX];		//  James add 2011/8/30 上午 09:42:38
		char*	m_pszFileName[CNT_INT_FILEMEMO_MAX];	//  James add 2011/10/6 09:23下午
		char	m_pszCurrentPath[CNT_INT_FILEMEMO_MAX];	//	James add 2011/8/31 下午 05:59:51
		BOOL	m_bCopyBg;
		WORD	m_wStartStrIndex;					//	
		WORD	m_wFocusColor;
		int						m_wFocusIndex;						//the memo's index, and is not Strings Index	//Mario WORD->int  20150113
		WORD	m_wOldFocusIndex;					//
		WORD	m_wFocusStrIndex;
		WORD	m_wShowType;						//0= show direct  1= show file 2= show all
		int		m_nCurrentDirectCount;	
		int		m_nCurrentFileCount;
		CtmScrollBar	m_scrollbar;
		BOOL	m_bBar;
		int 					nCount;
		BOOL					MoveButtonDownFalg;		//捲動元件是否被壓下
		WORD					Down_Y;								//按在捲動元件時的Y座標值
		BOOL					MouseDownTextFalg;		//捲動元件是否被壓下
		BOOL					ButtonPressFlag;
	protected:
		BOOL	m_bFocus;
		
		struct 	dirent *m_ptr;
		struct 	stat 	m_sb;
    	LinkNode m_head,	m_new_node;
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	private:
		void		ShowScrollbar();
		void		UpdateScrollbar();
		void		SetInitScrollbar();
		void		SetScrollbar(WORD wRange);
	protected:
		void	GetShowText();
		void	ShowStrs(WORD wOldFocusIndex, WORD wNewFocusIndex);
		void	SetFocusRow(WORD wOldFocusIndex, WORD wNewFocusIndex);
		WORD	GetMaxRows();
		WORD	GetStrsNum();
	public:
	/**/void    Show();
		void    Update();
		void    UpdateAll();
		BOOL 	CreateA();	
		void 	SetPropertys();
		void 	SetPropertys_Nub();
		WORD    OnKey(WORD wKey);
		void    DrawBorder();
		void    OnGetFocus();
		void    OnLoseFocus();
	public:
		void 	Add_sort(LinkNode new_node,LinkNode head);
		char* 	Sperm(mode_t mode);
		void 	Print_dir_info(struct stat statbuf,char *name);
		void 	Print_dir(LinkNode head);
		int 		Opendir_File(char*	pszDir, int nFlag = 0);
		int		GetFocusIndex();
		int		GetCurrentIndex();
		char*	GetCurrentDirName();
		char*	GetCurrentPath();
		int		Free_Node();
		int		SetFocusIndex(int nIndex);
		char*	GetCurrentFileName(int nIndex);
		int		GetCurrentFileCount();
		int		GetCurrentDirCount();
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		BOOL	MouseDown(short x_pos,short y_pos,int key_flag);
		BOOL		MouseUp(short x_pos,short y_pos,int key_flag);
		BOOL		MouseMove(short x_pos,short y_pos,int key_flag);
		void	ScrollUp();
		void	ScrollDown();
		//New add by Roy↓
		BOOL		FileCheck(char* sFileName);		//檢查輸入的檔案名稱是否重複
		//20140623↑
		void		ShowFileList();		//顯示目前m_pszFileName[]裡儲存的檔案名稱
		//20140624↑
		int			GetFileIndex(char* FileName);	//取得該檔名的index  Mario add
		void		GetCurrentFilePath(char* path);
};

class	CtmFileDir
{
	DECLARE_DYNCREATE(CtmFileDir)
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmFileDir();
		~CtmFileDir();
	protected:
		
		
};

#endif
