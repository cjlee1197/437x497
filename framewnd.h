/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : FrameWnd                                                       |
|  Task     : Frame window header file                                       |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : netdigger                                                      |
|  Version  : V1.00                                                          |
|  Creation : 2003/09/10                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef D_DFRAMEWND
#define D_DFRAMEWND

#include	"wnd.h"
#include	"ctimer.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - MainFrame                                    |
+===========================================================================*/
class CtmFrameWnd : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:	
	CtmFrameWnd();
	~CtmFrameWnd();
	
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
public:	
	CtmFrameWnd*				m_pNext;
	bool						m_bFocus;
	static	CtmFrameWnd* 		m_pFirst;
	
	void   Clear(RECT rect);
	
	int		Create(tmWND* pwnd);
	virtual int		WndProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
	static int FrameWndProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
	
private:	
	
	void	InitCreateInfo(PMAINWINCREATE pCreateInfo);
	
};

#endif
