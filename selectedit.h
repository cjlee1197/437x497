/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/

/*===========================================================================+
|  Class    : Select Edit                                                    |
|  Task     : Select Edit header file                                        |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Leo                                                            |
|  Version  : V1.00                                                          |
|  Creation : 11/07/2006                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef 	D_SELECTEDIT
#define		D_SELECTEDIT

#include 	"wnd.h"
#include	"wndx2.h"
#include	"memo.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
#define     RIGHT_BUTTON_UP     1
#define     RIGHT_BUTTON_DOWN   2
#define     RIGHT_BUTTON_ARROW  3

#define     tmMAX_SEDIT_COL     40  //�W�[�ɳ̤j��J�r�ꬰ40�Ӧr��
#define     MAX_STRING_COUNT    20

#define     tmWS_SEDIT_FILL     0x000F
#define     tmSEDIT_DISABLE     0x0010

const	int	CNT_INT_MAX_DB_NUM = 10;	
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagSELECTEDIT
    {
    WORD            wType;          //�ȫO�d, �i��0
    WORD            wMaxChar;       //����ܪ��r�ż�, �PSelectEdit�e�׮t��Ӧr��, �]�λP��ܥk�䪺��V
    short           nStringCount;   //�s��r�ꪺ�ƶq
    char*           pszPrompt;      //���ܦr����w
    char**          paStrings;      //�r��Ʋի��w
    WORD*           paIDString;     //�r��ID�Ʋի��w, �G�̿��@
    } tmSELECTEDIT;
/*============================================================================+
|            Function and Class prototype                                     |
+============================================================================*/
/*============================================================================+
|           Class declaration -- CtmListBox                                   |
+============================================================================*/
class		CtmSelectEdit  : public CtmWnd
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmSelectEdit)
	public:
		CtmSelectEdit(tmWND* pwnd,tmSELECTEDIT* pselectedit);
		CtmSelectEdit();	//	fans
		~CtmSelectEdit();
	
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
	    tmSELECTEDIT    m_selectedit;
	    tmSELECTEDIT*   m_pselectedit;
	    short       	m_nIndex;               // Current selection
	    short			m_nOldIndex;
	    short       	m_nCount;
	    BOOL            m_bOnFocus;
	    BOOL            m_bChanged;
	    BOOL			m_bSightless;
	    //CStrings*       m_pcstring;
	    char*           m_psz;
	    short           m_nCol;
	
	    WORD            m_wType;
	    WORD            m_wMaxChar;
	    char            m_szString[tmMAX_TEXT_COL+1];
	    int				m_iStartVaule;
	    int				m_nInterval;
	    
	    char*			m_pszShow[MAX_STRING_COUNT];//	fans add,
	    char*			m_pszIDShow[MAX_STRING_COUNT];//	fans add,
	    char*			m_pszStream[MAX_STRING_COUNT];
	    WORD			m_wszStreamLen[MAX_STRING_COUNT];		// rw.ying 2011/3/17 03:36�U��
	    char			m_pszOldIDShow[256];				  // James add ���F��s�r��
	    char			m_cszOldStream[2048];
	    char*			m_pszPromptID;
	    char*			m_pszDbID0;
	    BGC				m_bgcOld;
	    
	    WORD			m_wStreamType[MAX_STRING_COUNT];
	    ImageBackground	m_BackImage;
		BOOL        	m_bBackGround;
		BOOL			m_bUpdate;
		
		//	2008/4/17 02:31�U�� fans add
	    BGC				m_bgcLoseFocus;
	    BGC				m_bgcOnFocus;
	
		char*			m_pszDBID[CNT_INT_MAX_DB_NUM];	//	fans add:������DataBase��ID,�i�H�P�ɹ�����
		
		char*			m_pszImagePath[MAX_STRING_COUNT];
		WORD			m_wMemType;
		DWORD			m_dwKeyColor;
		
		CtmMemo			m_memo;
		WORD			m_wListNum;
		
		CtmImageBox* 	m_pImage;
		int				m_nBmpWidth;
		int				m_nBmpHeight;
		BOOL			m_bCalcSize;
		DWORD			m_triColor;
		DWORD			m_btnColor;
		
    short			m_nOffSet;					//	mario add 2018/8/17 �W�� 11:39:02
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
	/**/void    Get();
	    int     GetIndex();
	/**/void    Set();
	/**/void    Show();
	/**/void    Update();
		void 	UpdateAll();
		void	Clear();
	
	/**/void    OnGetFocus();
	/**/void    OnLoseFocus();
	/**/WORD    OnKey(WORD wKey);
		BOOL    SetIndex(int nIndex);
		bool 	SetPropValueT(char* pszPropName, char* pszValue, int nLen, int nType);		//rw.ying 2011/3/17 02:52�U��
		bool 	SetPropValueT(char* pszPropName, double Value);
		bool 	SetPropValueT(char* pszPropName, char* pszValue);
		//	fans add
		BOOL 	CreateA();
		void 	SetPropertys();
		void 	SetPropertys_Nub();
		void    DrawBorder();
	
	/*---------------------------------------------------------------------------+
	|           Helpers                                                          |
	+---------------------------------------------------------------------------*/
	    int     AddString(char* psz);
	    int     AddString(WORD  szID);
	    
	    int		OnLose();		//	fans add 2010-1-15 15:47:07
	private:
	    void    DrawRightButton(short nUpDown);
	    void    DrawFillTriangle(short nRow, short nCol, short nMaxRow, BOOL bUp);
	    void    CheckCharacter(char* pacCheck);
	    
	    //	fans add	2008/10/7 10:57�W��
	    BOOL	m_bList;				//	the list is show?
	    WORD	OnKey_List(WORD wKey);
	    void	EnableList();
	    void	DisableList();
	    
	    void	ShowList();
	    void	InitListMemo();	    
	    
	    void	ShowImage(int nIndex);
	    CtmBitMap*	GetBitMap(char *imagePath);
		void		CalcSize();
		
		//Mario add for touch
		void		WndProc(int message, WPARAM wParam, LPARAM lParam);
		BOOL		MouseDown(short x_pos,short y_pos,int key_flag);
		BOOL		MouseUp(short x_pos,short y_pos,int key_flag);
};
#endif
