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

#define		MAX_COLUMNS_COUNT			10					//�w�q�C��̤j�C��
#define		MAX_ITEM_COUNT				1000				//�w�q�C��̤j���
#define		MAX_SELECT_ITEM				5					//�w�q�i�襤�̦h���Ӽ�
#define		MAX_HIDEFILE_COUNT			10					//�w�q�̦h�i���ä��W�Ӽ�

#define		LISTVIEW_FILE				0					//��ܹϼХ�-��ܤ��
#define		LISTVIEW_DIR				1					//��ܹϼХ�-��ܤ��
#define		LISTVIEW_BACK				2					//��ܹϼХ�-��ܤW�@�ťؿ�
#define		LISTVIEW_NCASE			0x0000
#define		LISTVIEW_CASE				0x0100
//��� - Ū�����Τ�󧨱Ƨǥ�
typedef struct node
{
    char filename[256];
    char *TMFileName;
	struct stat sb;
	struct node* next;
}Node,*LinkNode;

//�C��C�ƤW���Ĥ@���ݩ�
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
		
		tmIMAGEBOX	m_tagImageBox;					//�Ϥ��H�����c��
		WORD		m_wAlign;						//��r����覡
		char*		m_pszTextID;					//�r��ID
		char*		m_pszText;						//�r��
			
		WORD		m_wImageWidth;					//�Ϥ��e��
		WORD		m_wTextWidth;					//��r�e��
		WORD		m_wItemHeight;					//Item����
		WORD		m_wFgc;							//�r���C��
		WORD		m_wBgc;							//�I����
		WORD		m_Static_wType;					//static����wtype
		
		RECT		m_RectImage;					//�Ϥ�����
		RECT		m_RectStatic;					//static����
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
		CtmItem*	m_pColumns_Item[MAX_COLUMNS_COUNT];		//�̤W����Title�C��
		CtmItem*	m_pContext_Item[MAX_COLUMNS_COUNT][MAX_ITEM_COUNT];	//�U����ܤ��e�C��
	   	COLUMNS		m_Columns[MAX_COLUMNS_COUNT];			//Columns�C��
	   	WORD		m_wColHeight;
		
	    char*		m_pszItemShow[MAX_COLUMNS_COUNT][MAX_ITEM_COUNT];			// ���Item�r��C��
		char*		m_pszItemIDShow[MAX_COLUMNS_COUNT][MAX_ITEM_COUNT];			// ���Item�r��ID�C��
		
	    char*		m_pszShow[MAX_ITEM_COUNT];				//��ܤ��e
		char*		m_pszIDShow[MAX_ITEM_COUNT];			//
		char*		m_pszDirName[MAX_ITEM_COUNT];			//��󧨦W��
		char*		m_pszFileName[MAX_ITEM_COUNT];			//���W��
	    char*		m_pszDataShow[MAX_ITEM_COUNT];			//�����
	    char*		m_pszSizeShow[MAX_ITEM_COUNT];			//���j�p
	    
		char*		m_pszHidFileName[MAX_HIDEFILE_COUNT];	//���ä��C��
		
		char		m_pszCurrentPath[MAX_ITEM_COUNT];		// ��ܷ�e���|
		char* 		m_pszExRegDir;
		char		m_szKeyWord1[64];
		char		m_szKeyWord2[64];
		int			m_nFileNameLen;		
		WORD		m_wItemType[MAX_COLUMNS_COUNT][MAX_ITEM_COUNT];			//���Item�����A���/���/�W�@�Ť��
		
		WORD		m_wAlign;								//�r�����覡
		
	   	WORD		m_wColCount;							//Columns�Ӽ�
	   	WORD		m_wItemCount;							//Items�Ӽ�
	   	WORD		m_wMaxItemCount;						//�̤j�i�e�U��Items�Ӽ�
	   	WORD		m_wMaxSelectItemCount;					//�̤j�i��ܪ�Items�Ӽ�
	   	char*		m_cImagePath_file;						//���Ϥ����|
	   	char*		m_cImagePath_dir;						//��󧨹Ϥ����|
	   	char*		m_cImagePath_back;						//�W�@�Ť�󧨹Ϥ����|
		BOOL		m_bBar;									//�k��scrollbar �O�_�ݭn���
		CtmScrollBar	m_scrollbar;						//�k��scrollbar
		
		BOOL		m_bGetFocus;
		WORD		m_wStartItemIndex;						//�_�lItems Index 
		WORD		m_wFocusColor;							//����Focus�C��
		WORD		m_wLostFocusColor;						//����Lost Focus�C��
		WORD		m_wFocusIndex;							//����Focus Index
		WORD		m_wOldFocusIndex;						//�W�@��Focus Index
		WORD		m_wSelectColor;							//�襤Items���C��
		WORD		m_wShowType;							//��󤺮e��ܤ覡 0= show direct  1= show file 2= show all
		int			m_nCurrentDirectCount;					//��e���|�U��󧨭Ӽ�
		int			m_nCurrentFileCount;					//��e���|�U���Ӽ�
		int			m_nCurrentCount;						//��e���|�@������`��
		
		char		m_wszSelectFile[MAX_SELECT_ITEM][512];  //�襤�������|�M���W
		WORD		m_wSelectIndex[MAX_SELECT_ITEM];		//�襤��Index�C��
		BOOL		m_bSelectUse;
		
		WORD		m_wItemImageWidth;						//�Ϥ��e��
		WORD		m_wItemKeyColor;						//�Ϥ�key color
		
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
		void		ShowScrollbar();						//��ܥk��scrollbar
		void		UpdateScrollbar();						//��s�k��scrollbar
		void		SetInitScrollbar();						//��l�ƥk��scrollbar
		void		SetScrollbar(WORD wRange);				//�]�m�k��scrollbar �d��
	public:
		void    	Show();
		void    	Update();
		void		UpdateList(int	nType, char*	pszName = NULL);			//nType=0��s��e��󧨦C��  nType=1��s��e��󧨦C��A�ë��w����pszName���a��
		void   		Clear();
		BOOL 		CreateA();	
		void 		SetPropertys();
		WORD    	OnKey(WORD wKey);
		void    	DrawBorder();
		void		ShowColumns();
		void		ShowItems();							//��ܩҦ�Items
		void   	 	OnGetFocus();
		void    	OnLoseFocus();				
		WORD		GetMaxItems();							//�Ϊ̷�e�̦h�i�e�Ǫ�Items�Ӽ�
		void		SetItemsStr(WORD	wColIndex, WORD	wRowIndex, char*	pszStr, WORD wType);		//�]�mItems ���e
		void		SetItemsIDStr(WORD	wColIndex, WORD	wRowIndex, char*	pszStr, WORD wType);		//�]�mItems ID���e	
		void		SetItemCount(WORD	wCount);
		void		ShowStrs(WORD wOldFocusIndex, WORD wNewFocusIndex);
		BOOL		MouseUp(short x_pos,short y_pos,int key_flag);
		BOOL	 	MouseDown(short x_pos,short y_pos,int key_flag);
		//File Manage
		int 		Opendir_File(char*	pszDir, int nFlag = 0, char* pszName = NULL, char* pszName2=NULL,int nLen=0,char* pszExRegDir=NULL);					//���}��󧨦C��
		void 		Add_sort(LinkNode new_node,LinkNode head);											//�����ǱƧ�
		void 		Print_dir_info(struct stat statbuf,char *name);										//�C�L��󧨸̭����H��
		void 		Print_dir(LinkNode head);															//�C�L���
		char* 		Sperm(mode_t mode);																
		int			Free_Node();																		//���񤺦s
		char*		GetEachFileName(int nFileIndex);													//��o��e���|�U�U���W��			//2013/2/25 add
		char*		GetCurrentPath();																	//��o��e���|
		char*		GetCurrentSelectName();																//�����e�襤�����W��
		WORD		GetCurrentSelectType();																//�����e�襤���������  ���/���
		int			GetItemCount();																		//�����e���|�@�����Ӽ�
		void		SetSelectUse(BOOL	bUse);															//�]�m�i�ƿ�h�Ӥ��
		char*		GetSelectFileName(int nIndex);														//Ū���襤�����W��
		BOOL		SetHideFileName(int nIndex, char*	pszFileName);									//�]�m���ä��W��
		
		void		OpenNewDir(WORD	wFocusIndex, WORD	wStartItemIndex);								//���}���
		void		AddDirToList(LinkNode head);														//�N��󧨦W�٥[�J�C��
		void		AddFileToList(LinkNode head);														//�N���W�٥[�J�C��
		
	public:
		void		WndProc(int message, WPARAM wParam, LPARAM lParam);
};

#endif
