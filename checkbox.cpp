/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager file                                 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeckey	                                                        |
|  Version  : V1.00                                                          |
|  Creation : 29/10/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"checkbox.h"
#include	"utils.h"
//#include	"memwatch.h"
/*===========================================================================+
|           External variable                                                |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - OptionBox                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmCheckBox, CtmWnd)

CtmCheckBox::CtmCheckBox(tmWND* pwnd,tmCHECK* pcheck) : CtmWnd()
{

}

//	fans add
CtmCheckBox::CtmCheckBox(): CtmWnd()
{
	strcpy(TYPEName, "CtmCheckBox");
	m_check.psz = NULL;
	m_check.pszPrompt = NULL;
	m_pszPrompt =NULL;
	m_pszTextID = NULL;
	m_pszPromptID = NULL;
	m_bChanged = FALSE;
	m_bCheck	= FALSE;
	m_bBackGround = FALSE;
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
		m_iFontUNType	=1;
	else
		m_iFontUNType	=0;
}


CtmCheckBox::~CtmCheckBox()
{
	
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void	CtmCheckBox::Update()
{
		Clear();
		Show();
}
/*---------------------------------------------------------------------------+
| Function  : Show()										                 |
| Task	  : show a checkbox and its extern							         |
+----------------------------------------------------------------------------+
|  Call     : 	                                  						     |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmCheckBox::Show()
{
  //	fans add
  	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
	else
		m_iFontUNType		= 0;
	CtmWnd::Show();
	if (m_wnd.wStyle & tmWS_SHOW)	//	fans add
	{
		short       nYOffset = (m_pcFont->m_wHeight - 16) >> 1;
    short		size = 	m_rectClient.bottom-m_rectClient.top;
    
		_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
		_SetColor(m_wndattr.fgcCheckBox1);
		
		
		
		_Rectangle(_tmGFILLINTERIOR, 0,0/* nYOffset*/, size, /*nYOffset+*/size);
		_SetColor(m_wndattr.fgcCheckBox2);
		_MoveTo( 0, /*nYOffset+*/size);
		_LineTo( 0, 0/*nYOffset*/);
		_LineTo(size-1, 0/*nYOffset*/);
		
		/*_Rectangle(_tmGFILLINTERIOR, 0, nYOffset, 22, nYOffset+22);
		_SetColor(m_wndattr.fgcCheckBox2);
		_MoveTo( 0, nYOffset+22);
		_LineTo( 0, nYOffset);
		_LineTo(21, nYOffset);*/
		
    	
    	Check();
		if(m_pcheck->psz != NULL)
		    CtmWnd::ShowExtent(21, 0, m_pcheck->psz);
	}
	ChangePosNow(GetClientRECT());
}

void    CtmCheckBox::Clear()
{
  //  fans modify 2014/1/19 21:51
	if ((m_wnd.wStyle & tmWS_TRANSPARENT) )
    {
        if (!m_bBackGround) 
        {
            m_BackImage.CopyImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                              m_rectClient.bottom);
            m_bBackGround = TRUE;
        }
        m_BackImage.ShowImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                              m_rectClient.bottom);
        
    }
    else
    {
        CtmWnd::Clear();
    }
}
/*---------------------------------------------------------------------------+
| Function  : 		 OnGetFocus()							                 |
| Task	  : 					 								             |
+----------------------------------------------------------------------------+
|  Call     : 	                                  						     |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmCheckBox::OnGetFocus()
{
	CtmWnd::DrawGetFocus();
	::Prompt(m_pcheck->pszPrompt);
}
/*---------------------------------------------------------------------------+
| Function  : OnLoseFocus()								                     |
| Task	  : 					 								             |
+----------------------------------------------------------------------------+
|  Call     : 	                                  						     |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmCheckBox::OnLoseFocus()
{
	CtmWnd::DrawLoseFocus();
}

WORD	CtmCheckBox::OnKey(WORD wKey)
{	
switch (wKey)
    {
    //case _KEY_SELECT:
    case 46:
        m_bCheck = !m_bCheck;
        Check();
        m_bChanged = TRUE;                     // WJ 0620'03 add
        break;
    case _ESC:
        //m_bCheck = m_pcheck->bCheck;
        m_bCheck = FALSE;
        Check();
        m_bChanged = TRUE;                     // WJ 0620'03 add
        return wKey;
    default:
        return wKey;
    }

return _NULL_KEY;
}

void CtmCheckBox::Check()
{
	//short       nYOffset = (m_pcFont->m_wHeight - 16) >> 1;
  short		size = 	m_rectClient.bottom-m_rectClient.top;

	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	if (m_bCheck)
	    {
    	_SetColor(m_wndattr.fgcCheckBox3);
    	_Rectangle(_tmGFILLINTERIOR, 2, /*nYOffset+*/2, size-2, /*nYOffset+*/(size-2));
	    _SetColor(0);
	    _Rectangle(_GFILLINTERIOR,3, /*nYOffset+*/3, size-3, /*nYOffset+*/(size-3));
	    }
	else
	    {
	    _SetColor(m_wndattr.fgcCheckBox3);
	    _Rectangle(_tmGFILLINTERIOR, 2,/* nYOffset+*/2, size+2,/* nYOffset+*/(size-2));
	    }
	ChangePosNow(GetClientRECT());
}



//	fans add
void 	CtmCheckBox::SetPropertys()
{
	CtmWnd::SetPropertys();
				
	SetPropInfo("text", 	tmSTR, 	&m_check.psz);
	SetPropInfo("textID",	tmSTR,	&m_pszTextID);
	SetPropInfo("prompt",	tmSTR,	&m_pszPrompt);
	SetPropInfo("promptID",	tmSTR,	&m_pszPromptID);
	SetPropInfo("checked",	tmBOOL,	&m_bCheck);		
}

void 	CtmCheckBox::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
				
	SetPropInfo(CHECKBOX_TEXT, 		tmSTR, 	&m_check.psz);
	SetPropInfo(CHECKBOX_TEXTID,	tmSTR,	&m_pszTextID);
	SetPropInfo(CHECKBOX_PROMPT,	tmSTR,	&m_pszPrompt);
	SetPropInfo(CHECKBOX_PROMPTID,	tmSTR,	&m_pszPromptID);
	SetPropInfo(CHECKBOX_CHECEKED,	tmBOOL,	&m_bCheck);		
}

//	fans add
BOOL 	CtmCheckBox::CreateA()
{
	SetBorderWidth(m_wndattr.wBorderWidthCheckBox);		
	CtmWnd::CreateA();	
	StrIDToStr(m_pszTextID, &m_check.psz);
//	StrIDToStr(m_pszPromptID, &m_check.pszPrompt);
	if(m_pszPromptID)
		m_check.pszPrompt=g_MultiLanguage[m_pszPromptID];
	else
		m_check.pszPrompt=m_pszPrompt;
	m_pcheck = &m_check;
	return TRUE;
}

