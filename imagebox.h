/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : image box Control						     					|
|  Task     : image box Control header file                         		|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                   |
|  Link     :G++(GCC) 3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                    	|
|  Version  : V1.00                                                         |
|  Creation : 2005/2/18                                                    	|
|  Revision :                                                               |
+==========================================================================*/
#ifndef D_BMP
#define	D_BMP

#include	"wnd.h"
#include    "wndx2.h"
#include	"init.h"
#include	"gui_ex.h"
#include	"cgif.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define     tmIMG_DISABLE		0x0000
#define     tmIMG_ENABLE		0x0001
#define		tmIMG_CPBKGD		0x0002				//copy bachground

#define		GIF_FWD				1
#define		GIF_BWD				2

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef	struct tagtmIMAGEBOX
{
	//char			cImagePath[50];	//	fans mark
	char*			cImagePath;		//	fans add
	WORD			wType;
	WORD			wStyle;
	unsigned long	dwKeyColor;
} tmIMAGEBOX;
/*==========================================================================+
|           Class declaration - CtmImageBox                                	|
+==========================================================================*/
class CtmImageBox : public CtmWnd
{
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmImageBox)
public:
    ~CtmImageBox();
    
    //	fans add
    CtmImageBox();
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
public:
	void    Show();
	void    Update();
	void    Update2();
	void	Clear();
	WORD    OnKey(WORD wKey);
	
	//	fans add
	BOOL 	CreateA();
	void 	SetPropertys();
	void 	SetPropertys_Nub();
protected:
	void	SetDoubleBuffer();		//	fans 2008-10-22 10:55:01
private:
	CtmBitMap*	GetBmpImage();
/*--------------------------------------------------------------------------+
|           Attributes                                                      |
+--------------------------------------------------------------------------*/
protected:
	tmIMAGEBOX*		m_pImageBox;
	tmIMAGEBOX		m_ImageBox;		//	fans
	ImageBackground	m_BgImage;
	
	CtmGDIDoubleBuffer* m_pDoubleBuffer;	//	fans add 2008-10-22 10:55:09
	BOOL			m_bDoubelBuffer;		//	fans add 2008-10-22 10:55:13
	char*			m_pszClickView;			//	James add 2010/2/1
	
	BOOL			m_bCopy; 		//	fans add
private:
	
	CtmBitMap		m_bmpImage;
	CtmBitMap*		m_pbmpImage;	//	fans add
	
};

class CtmImageGif : public CtmImageBox
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmImageGif)
public:
    
    CtmImageGif();
    ~CtmImageGif();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
	long long		m_llMax;
	long long		m_llMin;
	long long		m_llValue;
	
   	long long		m_lMaxFrame;
	long long		m_lMinFrame;
	
	WORD			m_wDirection;
	
	char*			m_pszDBID;
   	char*			m_pszMaxID;
   	char*			m_pszMinID;
   	
	CtmGif		m_GifIamge;
	CtmGif*		m_pGifImage;
	
	WORD		m_FrameIndex;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	BOOL 			CreateA();
	void 			SetPropertys();
	void    		Update();
	void    		Update2();
	void    		Show();
	
	gif_animation* 	GetGifInfo();

private:	
	WORD 			GetFrameIndex();
	CtmGif*			GetGifImage();
		
};
#endif
