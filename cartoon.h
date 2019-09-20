/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : Cartoon                                              			|
|  Task     : Cartoon Manager header file                         			|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                   |
|  Link     :G++(GCC) 3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                    	|
|  Version  : V1.00                                                         |
|  Creation : 2005/2/16                                                    	|
|  Revision :                                                               |
+==========================================================================*/
#ifndef	D_CARTOON
#define	D_CARTOON

#include	"wnd.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		CARTOON_USER	0x0000		// �Τ���w�C�V���̤j�ȳ̤p��
#define		CARTOON_AUTO	0x0001		// �ھڳ̤j�ȥ������t�C�V���̤j�ȳ̤p��
#define		CARTOON_CLEAR	0x0002		// �䤣��ǰt���A�N�����
#define		CARTOON_RECT	0x0100		// �ʵe����m�ѤW�@�h�ʵe���w

const	int	MAXFRAME = 23;
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef	struct tagtmCARTOON_FRAME
{
	char*	cBMPPath;//[50];	fans modify
	int		wMin;
	int		wMax;
}	tmCARTOON_FRAME;

typedef	struct tagtmCARTOON
{
	WORD				FrameCounter;
	int*				pwData;
	int*				pwMaxData;
	WORD				wType;
	tmCARTOON_FRAME*	pframe_Table;
	//WORD				hwndParent;
	CtmWnd*				pcartParent;
	WORD				wBmpIndex;			// �W�h�ʵe���s�V
}	tmCARTOON;

struct tagtmCARTOONBMP
{
	tagtmCARTOONBMP*	pNext;
	tagtmCARTOONBMP*	pPre;
	CtmBitMap*			pcBitMap;
	tmCARTOON_FRAME*	pFrame;
	int					nIndex;
};
typedef	tagtmCARTOONBMP	CARTOONBMP;

/*==========================================================================+
|           Class declaration - CtmCartoon                                  |
+==========================================================================*/
class CtmCartoon : public CtmWnd
{
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmCartoon)
public:
	CtmCartoon(tmWND* pwnd);
    CtmCartoon(tmWND* pwnd, tmCARTOON* pcartoon);
    CtmCartoon();	//	fans add
    ~CtmCartoon();
    
/*--------------------------------------------------------------------------+
|           Attributes                                                      |
+--------------------------------------------------------------------------*/
private:
    tmCARTOON		m_cartoon;
	CARTOONBMP*		m_pBMP;
	CARTOONBMP*		m_pCurBMP;
	CtmCartoon*		m_pParent;
	RECT			m_rect;
	
	int		m_iBMPCounter;
	char*	m_pcBGImage;
	WORD	m_wMax;
	WORD	m_wClear;
	WORD	m_wPreData;
	WORD	m_wUpdated;
	WORD	m_wBGSaved;
	
	
	//	fans add
	tmCARTOON_FRAME	m_frameTable[MAXFRAME];
	char*	m_pszMinDataBaseID;
	char*	m_pszMaxDataBaseID;
	
	int		SetCurBMP();
	int		InsertBMP(tmCARTOON_FRAME* pFrame);
	void	MaxChange();

/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
public:
	WORD	m_wCurIndex;

	void    Show();
	void    Update();
	void	Clear();
	
	void	Set(tmCARTOON* pcartoon);
	
	
	//	fans add
	void	Set();	// virtual function
	BOOL	CreateA();
	void	SetPropertys();
	void	SetPropertys_Nub();
};

#endif
