/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"msgbox.h"
#include	"parsepule.h"	//	fans add

/*===========================================================================+
|           Class implementation - MsgBox                                    |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

IMPLEMENT_DYNCREATE(CtmMsgBox, CtmFormView)

CtmMsgBox::CtmMsgBox() : CtmFormView()
{
	strcpy(TYPEName, "CtmMsgBox");
	
	/*
	#define     WBUTTON             tmDEFAULT_W_BUTTON
	#define     HBUTTON             tmDEFAULT_H_BUTTON
	
	tmWND         wnd;
	
	SetFont(g_pFont16);
	tmSIZE        size  = GetStringExtent(pmsg->psz);
	
	short       nX    = 16+16;
	short       nWDlg = ((nX+size.w+16) > (16+WBUTTON+16+WBUTTON+16)) ? (nX+size.w+16) : (16+WBUTTON+16+WBUTTON+16);
	short       nHDlg = 16+ size.h+20+HBUTTON+16;
	
	m_pmsg = pmsg;
	
	MKPANEL  (m_panel,        tmBEVEL_SHADOW,4,0, tmBEVEL_NONE,0,0, BLACK, WHITE,FALSE)
	MKSTATIC (m_static,       pmsg->psz)
	MKBUTDEF1(m_buttonOK,     (nWDlg-WBUTTON*2-16)/2, nHDlg-(HBUTTON+16))
	MKBUTDEF1(m_buttonCancel, (nWDlg          +16)/2, nHDlg-(HBUTTON+16))
	
	MKDLG (m_dlg[IDP_],       tmDLG_PANEL,        tmWS_BORDER_NONE|tmWS_TRANSPARENT, 0,0,nWDlg-1,nHDlg-1, &m_panel)
	MKDLG (m_dlg[IDS_],       tmDLG_STATICX2,     tmWS_BORDER_NONE|tmWS_TRANSPARENT, nX,16,nX+size.w+2-1,16+size.h+2-1, &m_static)
	MKDLG1(m_dlg[IDB_OK],     tmDLG_BUTTONOK,     &m_buttonOK)
	MKDLG1(m_dlg[IDB_CANCEL], tmDLG_BUTTONCANCEL, &m_buttonCancel)
	MKDLG2(m_dlg[ID_END],    tmDLG_END)
	
	MKWND(wnd, NULL, tmWS_BORDER_RAISE|tmWS_TRANSPARENT|tmWS_CLEAR|tmWS_REDRAW|tmWS_FRAMEWND, 88,88,88+nWDlg-1,88+nHDlg-1, NULL_HWND, &g_pFont16)
	
	SetBorderWidth(m_wndattr.wBorderWidthMsgBox);
	
	CtmDialog::Create(&wnd, m_dlg);
	GetControl(IDP_)->SetBGColor(m_wndattr.bgc);
	m_bPrompt = FALSE;
	*/
}

CtmMsgBox::~CtmMsgBox()
{
	
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

