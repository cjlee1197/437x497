/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision : 09/07/2003  Jeckey                                             |
+===========================================================================*/

#include	"edit.h"
#include	"utils.h"
#include	"stable.h"

#include	"commonaction.h"
#include    "tmdebug.h"
#include	"tmshmsg.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
static char g_pszNum2Char[10][4] = {
            {'1', 'S', 'T', 'U'},
            {'2', 'V', 'W', 'X'},
            {'3', 'Y', 'Z', '3'},
            {'4', 'J', 'K', 'L'},
            {'5', 'M', 'N', 'O'},
            {'6', 'P', 'Q', 'R'},
            {'7', 'A', 'B', 'C'},
            {'8', 'D', 'E', 'F'},
            {'9', 'G', 'H', 'I'},
            {'*', ' ', '\\', '*'}
            };
/*===========================================================================+
|			variable											|
+===========================================================================*/
extern BOOL	g_bFocusChanged;


IMPLEMENT_DYNCREATE(CtmEdit, CtmWnd)
IMPLEMENT_DYNCREATE(CtmEditX1, CtmEdit)
IMPLEMENT_DYNCREATE(CtmEditX2, CtmEdit)

/*===========================================================================+
|           External variable                                                |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - InvisibleBox                              |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmNulledit::CtmNulledit(tmWND* pwnd) : CtmWnd()
{
	SetBorderWidth(m_wndattr.wBorderWidthEdit);
	SetBGColor(m_wndattr.bgc);
	CtmWnd::Create(pwnd);

	SetFGColor(LIGHTCYAN);
	
	if (m_wnd.wStyle & tmWS_SHOW)             // Show the content if requested
		Show();
}

void        CtmNulledit::Show()               /* Virtual function */
{
	SetBGColor(m_wndattr.bgc);
	CtmWnd::Clear();
}

CtmNulledit::~CtmNulledit()
{
}

void CtmNulledit:: Redraw(RECT rect)
{
	DrawBorder();
	Show();
}

/*===========================================================================+
|           Class implementation - EditBox                                   |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CtmEdit::CtmEdit() : CtmWnd()
{
	//	fans add
	strcpy(TYPEName, "CtmEdit");

	m_pszPrompt 	= NULL;
	m_pszPromptLan  =NULL;
	m_pszPromptID 	= NULL;
	m_wEcho 		= _NULL_KEY;
	m_pszText 		= NULL;
	
	m_wType 		= 0;
	m_wMaxChar 		= 1;
	m_nCol 			= 0;
	m_nIndex 		= 0;
	m_nRow 			= 0;
	m_bValueSymbol 	= FALSE;
	m_nOffSet		= 0;		//	fans add 2009-3-31 18:14:34
	
	m_sz[0] 		= '\0';
	m_bgcOld 		= 0;
	m_fgc 			= m_wndattr.fgcEdit1;
	m_bgc 			= m_wndattr.bgcEdit1;
	m_bBackGround 	= FALSE;
	m_bDelete	  	= TRUE;
	m_bFInputFlag 	= TRUE;
	m_bOverPassword = FALSE;
	
	m_paragraph.wAlign = 0;
	
	m_wBKSP 		= 0;
	m_wInsertPos	= 0;
	//when having number key event,but old value equal new value 
	//,ui should update this widget	
	//add by Wong 2014/11/7 10:25:18
	m_bHintFlag=FALSE;
	m_bLoseFocus=FALSE;
}


CtmEdit::~CtmEdit()
{

	if (GetParent() != NULL)
	{
		//WORD	wTemp = 0;
		//
		//GetParent()->GetPropValueT("wtype", &wTemp, sizeof(wTemp));
		//
		//if ((wTemp & tmWS_REDRAW) && (wTemp& tmWS_FRAMEWND))	
		//{
		//	if (m_ptaskCursor != NULL)
		//		m_ptaskCursor->SetOnCursor(FALSE);
		//}
		if (m_ptaskCursor != NULL)
				m_ptaskCursor->SetOnCursor(FALSE);
		EnableCursor(FALSE);
	}

}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/


void        CtmEdit::Get()                /* Virtual function */
{
	if (m_pszText != NULL) 
	{
		//delete []m_pszText;
		free(m_pszText);
		m_pszText = NULL;
	}
	m_pszText=(char*)malloc(strlen(m_sz)+1);
	//m_pszText = new char[strlen(m_sz) + 1];
	strcpy(m_pszText, m_sz);
	//memcpy(m_pszText, m_sz, strlen(m_sz) + 1);
	

}

void        CtmEdit::Set()                /* Virtual function */
{
	
	/*
	SetPropInfo("fgc", 			tmDWORD,	&m_fgc);
	SetPropInfo("bgc", 			tmDWORD,	&m_bgc);
	*/
	SetFGColor(m_fgc);
    SetBGColor(m_bgc);
    	
	if (m_pszText != NULL)
	{
		
		StrNCpy(m_sz, m_pszText, m_wMaxChar);
	}
	ResetPosition();
	
}

//	fans add
void		CtmEdit::ReSet()
{
	CtmEdit::Set();
}

void        CtmEdit::SetAll()             /* Virtual function */
{
	
	
	if ((m_wType & tmEDIT_NUMERIC) || (m_wType & tmEDIT_FLOAT))
    {
	    tmPARAGRAPH   paragraph = {tmPARA_ALIGN_RIGHT};
	    SetParagraph(paragraph);
    }
	if (m_wMaxChar > tmMAX_TEXT_COL) m_wMaxChar = tmMAX_TEXT_COL;
	else if (m_wMaxChar == 0) m_wMaxChar = 1;
	
	Set();
}

void        CtmEdit::Show()               /* Virtual function */
{
	RECT	rect;
	if (g_pWndInputForm != NULL && g_pWndInput == this )
	{
		memcpy(&rect, &g_pWndInputForm->GetWindowRECT(), sizeof(RECT));
		if( (this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
			this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom) ||
			(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
			this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom)||
			(this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
			this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom)||
			(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
			this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom))
		{
			return ;
		}
		else
		{
			;//printf("not in  this area \n");		
		}
	}
	else
	{
		;
	} 
	CtmWnd::SetClientRECT(-1, 0, 1, 0);       // Restore the area for vertical cursor
  	Clear();
  	CtmWnd::SetClientRECT(1, 0, -1, 0);       // Make room for vertical cursor
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}

  	if (m_wnd.wStyle & tmWS_SHOW)	//	fans add
  	{
  		if (!m_bEnabled)
  		{
  			m_bgcOld = m_bgc;
  			m_bgc = m_wndattr.bgcEdit2;
  		}
  		DrawBorder();
  		{
  			
	  		if (m_pszText != NULL&&strcmp(TYPEName,"CtmEditX2")) //add by Wong 2014/11/1 13:42:22
	  		{
	  			StrNCpy(m_sz, m_pszText, m_wMaxChar);	//	fans add
	  		}
  		}
  		CtmWnd::SetClientRECT(-1, 0, 1, 0);       // Restore the area for vertical cursor
  		Clear();
  		CtmWnd::SetClientRECT(1, 0, -1, 0);       // Make room for vertical cursor
  		m_bFInputFlag = FALSE;
  		ShowXY(m_nCol-m_nIndex, m_nRow, m_sz);



  		if (!m_bEnabled)
  			m_bgc = m_bgcOld;
  	}
  	else
  	{
  	if (!(m_wnd.wStyle & tmWS_TRANSPARENT) ||
  		(m_wnd.wStyle & 0x07))
  			DrawBorder();
  		if (m_bgcOld == 0)
  			m_bgcOld = m_bgc;

   		if (GetParent() != NULL )
   			m_bgc = GetParent()->GetBGColor();

   		CtmWnd::SetClientRECT(-1, 0, 1, 0);       // Restore the area for vertical cursor
   		Clear();
   		CtmWnd::SetClientRECT(1, 0, -1, 0);       // Make room for vertical cursor
   		m_bgc = m_bgcOld;

  	}
  		  //	fans add
  CtmWnd::Show(); 
	ChangePosNow(GetClientRECT());
	//ChangePosRegionSum(GetClientRECT());
}

void    CtmEdit::Clear()
{
	RECT	rect;
	if (g_pWndInputForm != NULL && g_pWndInput == this )
	{
		memcpy(&rect, &g_pWndInputForm->GetWindowRECT(), sizeof(RECT));
		if( (this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
			this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom) ||
			(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
			this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom)||
			(this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
			this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom)||
			(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
			this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom))
		{
			return ;
		}
		else
		{
			;//printf("not in  this area \n");		
		}
	}

	/*
	  1. the View is from the buffers, and the component is transparent, the
	*/

    if (m_wnd.wStyle & tmWS_TRANSPARENT)
    {
		if (!m_bBackGround)
		{
			m_BackImage.CopyImage(m_rectClient.left, m_rectClient.top,
								  m_rectClient.right, m_rectClient.bottom);
			m_bBackGround = TRUE;
		}
		else
		{
			m_BackImage.ShowImage(m_rectClient.left, m_rectClient.top,
								  m_rectClient.right, m_rectClient.bottom);
		}

    }
    else
    {
        CtmWnd::Clear();
    }
}

void        CtmEdit::Update()             /* Virtual function */
{
	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)
	{
				RECT	rect;
				if (g_pWndInputForm != NULL && g_pWndInput == this)
				{
					memcpy(&rect, &g_pWndInputForm->GetWindowRECT(), sizeof(RECT));
					if( (this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
						this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom) ||
						(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
						this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom)||
						(this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
						this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom)||
						(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
						this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom))
					{
						return ;
					}
					else
					{
						;//printf("not in  this area \n");		
					}
				}
			//if (!m_bOnFocus)                      // CCombo will update the CtmEdit when a item is selected from CList
			    {
				    if (m_ptaskCursor != NULL) DisplayCursor(FALSE);
				    //CtmEdit::Set();                       // It must be prefixed the "CtmEdit::". Otherwise when called from derived class, the Set() of derived class will be executed
				    ReSet();
				    CtmEdit::Show();
				    if (m_ptaskCursor != NULL)          // This statement is not necessary
				    {
				        SetCursorPosition(m_nCol, m_nRow);
				        DisplayCursor(TRUE);
				    }
			    }
	}
}

void        CtmEdit::UpdateAll()          /* Virtual function */
{
	
	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)
		{
			RECT	rect;
			if (g_pWndInputForm != NULL && g_pWndInput == this )
			{
				memcpy(&rect, &g_pWndInputForm->GetWindowRECT(), sizeof(RECT));
				if( (this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
					this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom) ||
					(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
					this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom)||
					(this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
					this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom)||
					(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
					this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom))
				{
					return ;
				}
				else
				{
					;//printf("not in  this area \n");		
				}
			}
			
			if (m_ptaskCursor != NULL) DisplayCursor(FALSE);
			CtmEdit::SetAll();
			CtmEdit::Show();
			if (m_ptaskCursor != NULL)              // This statement is not necessary
		    {
			    SetCursorPosition(m_nCol, m_nRow);
			    DisplayCursor(TRUE);
		    }
		}
}

void        CtmEdit::OnGetFocus()         /* Virtual function */
{
	OnGetFocus1();
	CtmWnd::DrawGetFocus();
	if (m_pszPrompt != NULL)	
		Prompt(m_pszPrompt);
}

void        CtmEdit::OnLoseFocus()        /* Virtual function */
{
	//printf("123\n");
	m_bOverPassword = FALSE;
	EnableCursor(FALSE);
	CtmWnd::DrawLoseFocus();
	m_bLoseFocus=TRUE;
	Show();
	m_bPropertyChanged=TRUE;		
}

void  CtmEdit::OnGetFocus1()
{
	CtmEdit::Show();
	EnableCursor(TRUE);
	ResetPosition();
	DisplayCursor(TRUE);
	m_bChanged = FALSE;
	
}

void  CtmEdit::OnLoseFocus1()
{
	EnableCursor(FALSE);
}

WORD        CtmEdit::OnKey(WORD wKey)     /* Virtual function */
{
	//徹薦杅趼瑩侂羲岈璃ㄛ瘁寀拸楊妗珋瑩硉з遙
	if((wKey&0xFF00)==0x9400)	return  _NULL_KEY;
	WORD        wKeyReturn = _NULL_KEY;     // Key code process result
	BOOL        bFull = FALSE;              // Field is full flag
	short       nShiftCount;
	char        acKey[2];
	
	//printf(" CtmEdit::OnKey wKey=%04x\n", wKey);
	
	DisplayCursor(FALSE);                   // Disable the cursor to prevent the moving string destroy the cursor

	//=======
	//      Get a normal key from the keyboard
	//=======
	if (tmHIBYTE(wKey) == 0)                  // It is a normal ASCII code
    {
	    acKey[0] = tmLOBYTE(wKey);
	    acKey[1] = '\0';
		if(acKey[0] < 0x20 || acKey[0] > 0x7E) goto _ONKEY_END;
		if(acKey[0]>=0x20&&acKey[0]<=0x7E) 	   m_bHintFlag=TRUE;
	    //=======
	    //      Convert to Text
	    //=======
    	if (((m_wType & 0x00FF) != tmEDIT_NUMERIC) && ((m_wType & 0x00FF) != tmEDIT_HEXNOALPHA) && 
    		((m_wType & 0x00FF) != tmEDIT_BOOL) && ((m_wType & 0x00FF) != tmEDIT_FLOAT)&&
    		 (m_wType != 0x80)) //Sunny<20070726> add密碼框元件不能輸入字母 (m_wType != 0x80) 
        {
        	//printf("(m_wType & 0x00FF) == (tmEDIT_PASSWORD|tmEDIT_HEXADECIMAL)\n");
        	if (((acKey[0] >= '1') && (acKey[0] <= '9')) || (acKey[0] == '*'))
            {
            	if (GetWaitStatus() && (acKey[0] == m_cOldASC))
                {
	                m_nChar    = m_nOldChar;
	                m_nCol     = m_nOldCol;
	                m_nIndex   = m_nOldIndex;
	                m_wInsertPos= m_wOldInsertPos;
	                m_bChanged = m_bOldChanged;
	                switch (acKey[0])
                    {
                    case '3':
                        m_nN2CIndex = (++m_nN2CIndex)%3;
                         break;
                    case '*':
                        m_nN2CIndex = (++m_nN2CIndex)%3;
                        break;
                    default:         
                        m_nN2CIndex = (++m_nN2CIndex)%4;
                        break;
                    }
                } 
            	else
                {
	                m_nN2CIndex = 0;
	                m_cOldASC = acKey[0];
                }
	            if (acKey[0] == '*') acKey[0] = g_pszNum2Char[9][m_nN2CIndex]; 
	            else                 acKey[0] = g_pszNum2Char[acKey[0]-'1'][m_nN2CIndex];
	            CursorWait(TRUE);
	            m_nOldChar    = m_nChar;
	            m_nOldCol     = m_nCol;
	            m_nOldIndex   = m_nIndex;
	            m_wOldInsertPos= m_wInsertPos;
	            m_bOldChanged = m_bChanged;
            }    
	        else 
	            CursorWait(FALSE);
        }

	    //=======
	    //      Validate the input
	    //=======
	    switch (m_wType & 0x00FF)
        {
        case tmEDIT_ALPHA:
            if ((acKey[0] >= '0') && (acKey[0] <= '9'))
			{
				
                ::MsgBeep(g_MultiLanguage["MSG_INPUT_ALPHA"]);
                goto _ONKEY_END;        // Using goto instead of return is to reenable cursor
                //return FALSE;
			}
            break;
        case tmEDIT_NUMERIC:
        case tmEDIT_HEXNOALPHA:    //St0522 for HEX input numeric only
             if(!m_bValueSymbol && (acKey[0] == '-'))
			{
			    ::MsgBeep(g_MultiLanguage["MSG_INPUT_NUMERIC"]);
		        goto _ONKEY_END;
			}
            if (((acKey[0] < '0') || (acKey[0] > '9')) && (acKey[0] != '-'))
            {
                ::MsgBeep(g_MultiLanguage["MSG_INPUT_NUMERIC"]);
                goto _ONKEY_END;
                
            }
            break;
        case tmEDIT_BOOL:
            if(!m_bValueSymbol && (acKey[0] == '-'))
			{
			    ::MsgBeep(g_MultiLanguage["MSG_INPUT_BOOL"]);
		        goto _ONKEY_END;
			}
            if ((acKey[0] < '0') || (acKey[0] > '1'))
            {
                ::MsgBeep(g_MultiLanguage["MSG_INPUT_BOOL"]);
                goto _ONKEY_END;
            }
            break;
        case tmEDIT_FLOAT:
            if(!m_bValueSymbol && (acKey[0] == '-'))
			{
			    ::MsgBeep(g_MultiLanguage["MSG_INPUT_FLOAT"]);
		        goto _ONKEY_END;
			}
            if (((acKey[0] < '0') || (acKey[0] > '9')) && (acKey[0] != '.') && (acKey[0] != '-'))
            {
                ::MsgBeep(g_MultiLanguage["MSG_INPUT_FLOAT"]);
                goto _ONKEY_END;
            }
            break;
        case tmEDIT_HEXADECIMAL:
            if(!m_bValueSymbol && (acKey[0] == '-'))
			{
			    ::MsgBeep(g_MultiLanguage["MSG_INPUT_HEXADECIMAL"]);
		        goto _ONKEY_END;
			}
            if (!(((acKey[0] >= '0') && (acKey[0] <= '9')) ||
                  ((acKey[0] >= 'A') && (acKey[0] <= 'F')) ||
                  ((acKey[0] >= 'a') && (acKey[0] <= 'f'))))
            {
                ::MsgBeep(g_MultiLanguage["MSG_INPUT_HEXADECIMAL"]);
                goto _ONKEY_END;
            }
            if ((acKey[0] >= 'a') && (acKey[0] <= 'f'))
                acKey[0] -= 'a' - 'A';
            break;
        default:
            break;
        }

    //=======
    //      Update the string
    //          m_nIndex - Current character index in the string
    //          m_nCol   - Current cursor position in the edit box
    //              ((m_nIndex - m_nCol) > 0)
    //              ? Number of characters hidden in the left side of the field
    //              : Number of blanks in the left part of the field in case of tmPARA_ALIGN_RIGHT
    //=======
	    if (!m_bChanged)
	    {
	        if(m_wBKSP == 0 || m_wBKSP == 2)
	        {
	        	strcpy(m_sz, acKey);            // Substitude the whole string when input the first character
		        ResetPosition();
		        Clear();
		        m_bChanged = TRUE;
		        m_nIndex++;                     // Set the character index to the next insert position
		        m_wInsertPos++;					//Caozz 2011/11/6 下午 11:40:18
	        }
	        else if(m_wBKSP == 1)
	        {
	        	if(strlen(m_sz) == 0)
	        	{
	        		strcpy(m_sz, acKey);            // Substitude the whole string when input the first character
	        		m_wInsertPos++;
	        		m_nIndex++;                     // Set the character index to the next insert position
	        	}
		        else
		        {
		        	*(m_sz + m_wInsertPos++) = acKey[0];
		        	m_nIndex++;
			        if(m_wInsertPos < m_nIndex)
			        	m_nIndex--;
			        if (m_nIndex > m_nChar)
		            {
			            *(m_sz + m_nIndex) = '\0';
			            m_nChar++;
		            }	
		        }
		        //ResetPosition();
		        //Clear();
		        m_bChanged = TRUE;
		        
	        }
	    }
	    else
        {
	        *(m_sz + m_wInsertPos++) = acKey[0];
	        // *(m_sz + m_nIndex++) = acKey[0];
	        m_nIndex++;
	        if(m_wInsertPos < m_nIndex)
	        	m_nIndex--;
	        if (m_nIndex > m_nChar)
            {
	            *(m_sz + m_nIndex) = '\0';
	            m_nChar++;
            }
        }
       if ((WORD)m_nIndex == m_wMaxChar)
        {
	        //m_nIndex--;
	        //m_wInsertPos--;
	        bFull = TRUE;
        }
	    //=======
	    //      Adjust the field and show it if need
	    //=======
	    m_nCol++;
	    if (m_nCol > m_nClientWidthText)
        {
	        if (bFull) 
	        {
	        	m_nCol -= 2;
	        	m_wInsertPos--;
	        	ShowXY(m_nCol-m_nIndex+1, m_nRow, m_sz);
	        }
	        else
	        {
	        	m_nCol--;
	        	
	        	ShowXY(m_nCol-m_nIndex, m_nRow, m_sz);
	    	}
        }
    	else
        {
        	m_bDelete = TRUE;
        	m_bFInputFlag = TRUE;
	        ShowXY(m_nCol-1, m_nRow, acKey);
	        //ShowXY(m_nCol, m_nRow, acKey);
	        if (bFull && m_wInsertPos == m_wMaxChar) 
	        {
	        	m_nCol--;
	        	m_wInsertPos--;
	        }
        }       

    //=======
    //      Exit if no terminator and input characters reach specified width
    //=======
    	if (!(((m_wType & 0x00FF) != tmEDIT_NUMERIC) && ((m_wType & 0x00FF) != tmEDIT_HEXNOALPHA) && ((m_wType & 0x00FF) != tmEDIT_BOOL) && ((m_wType & 0x00FF) != tmEDIT_FLOAT)))

	    if ((m_wType & tmEDIT_NOTERMINATOR) && bFull)
	    {
	        wKeyReturn = _TAB;
	        goto _ONKEY_END;
	    }
    }

//=======
//      Interpret function key from the keyboard
//=======
	else
	    {
	    if (((m_wType & 0x00FF) != tmEDIT_NUMERIC) && ((m_wType & 0x00FF) != tmEDIT_HEXNOALPHA) && ((m_wType & 0x00FF) != tmEDIT_BOOL) && ((m_wType & 0x00FF) != tmEDIT_FLOAT))
	        CursorWait(FALSE);
	    switch (wKey)
	        {
	        case _HOME:
	        case _EXT_HOME:
	            if (m_nIndex > m_nCol)
	                {
	                //=======
	                //      Reposition the string if there are characters in the left side of the field
	                //=======
	               
	                ShowXY(0, m_nRow, m_sz);
	                }
	
	            m_nIndex = 0;
	            if ((m_paragraph.wAlign == tmPARA_ALIGN_RIGHT) && (m_nClientWidthText > m_nChar))
	                m_nCol = m_nClientWidthText - m_nChar;
	            else
	                m_nCol = 0;
	            break;
	        case _END:
	        case _EXT_END:
	            if (m_nChar > (m_nClientWidthText + m_nIndex - m_nCol))
	                {
	                //=======
	                //      Reposition the string if there are characters in the right side of the field
	                //=======
	         
	                ShowXY(0, m_nRow, m_sz+m_nChar-m_nClientWidthText);
	                }
	
	            m_nIndex = (m_nChar == (short)m_wMaxChar) ? (m_nChar - 1) : m_nChar;
	            if ((m_paragraph.wAlign == tmPARA_ALIGN_RIGHT) || (m_nChar > m_nClientWidthText))
	                m_nCol = (m_nChar == (short)m_wMaxChar) ? (m_nClientWidthText - 1) : m_nClientWidthText;
	            else
	                m_nCol = m_nIndex;
	            break;
	        case _LARROW:
	        case _EXT_LARROW:
	            if (m_nIndex > 0)
	                {
	                //=======
	                //      If character index is within the string
	                //=======
	                m_nIndex--;
	                m_nCol--;
	                if (m_nCol < 1)
	                    {
	                    //=======
	                    //      Reposition the string if cursor is over the left margin
	                    //=======
	                    nShiftCount = (((m_nClientWidthText>>1)+1) > (m_nIndex-m_nCol))
	                                  ? (m_nIndex-m_nCol)
	                                  : ((m_nClientWidthText>>1)+1);
	                    if (nShiftCount > 0)
	                        {
	                        m_nCol += nShiftCount;
	                        ShowXY(0, m_nRow, m_sz+m_nIndex-nShiftCount);
	                        }
	                    }
	                }
	            else
	                {
	                //=======
	                //      Otherwise check the termination condition
	                //=======
	                if (m_wType & tmEDIT_NOTERMINATOR)
	                    {
	                    if ((m_wType & tmEDIT_NOEMPTY) && (m_nChar == 0))
	                        ::MsgBeep(g_MultiLanguage["MSG_INPUT_NOEMPTY"]);
	                    else
	                        {
	                        wKeyReturn = _SHIFT_TAB;
	                        goto _ONKEY_END;
	                        }
	                    }
	                }
	            break;
	        case _RARROW:
	        case _EXT_RARROW:
	            if ((m_nIndex <= (m_nChar-1)) && (m_nIndex < (short)(m_wMaxChar-1)))
	                {
	                //=======
	                //      If character index is within the string
	                //=======
	                m_nIndex++;
	                m_nCol++;
	                if (m_nCol >= m_nClientWidthText)
	                    {
	                    //=======
	                    //      Reposition the string if cursor is over the right margin
	                    //=======
	                    nShiftCount = (((m_nClientWidthText>>1)+1) > (m_nChar-m_nIndex))
	                                  ? (m_nChar-m_nIndex)
	                                  : ((m_nClientWidthText>>1)+1);
	                    if (nShiftCount > 0)
	                        {
	                        m_nCol -= nShiftCount;	
	                        ShowXY(0, m_nRow, m_sz+m_nIndex-m_nClientWidthText+nShiftCount);
	                        }
	                    }
	                }
	            else
	                {
	                //=======
	                //      Otherwise check the termination condition
	                //=======
	                if (m_wType & tmEDIT_NOTERMINATOR)
	                    {
	                    if ((m_wType & tmEDIT_NOEMPTY) && (m_nChar == 0))
	                        ::MsgBeep(g_MultiLanguage["MSG_INPUT_NOEMPTY"]);
	                    else
	                        {
	                        wKeyReturn = _TAB;
	                        goto _ONKEY_END;
	                        }
	                    }
	                }
	            break;
	        case _BKSP:
//	       	case _KEY_CLEAR:
	            //if (m_nIndex > 0)		//Caozz 2011/11/6 下午 11:36:52
	            if (m_wInsertPos > 0)
	           	{
	                //strcpy(m_sz+m_nIndex-1, m_sz+m_nIndex);		//Caozz 2011/11/6 下午 11:37:42
//	                strcpy(m_sz+m_wInsertPos-1, m_sz+m_wInsertPos);
//	                m_nChar--;
//	                m_nIndex--;
//	                if(m_nIndex<0) m_nIndex = 0;
//	                m_wInsertPos--;
					//printf("m_nIndex=%d,m_wMaxChar=%d\n",m_nIndex,m_wMaxChar);
					if ((WORD)m_nIndex > m_wMaxChar)
	                {
	                	strcpy(m_sz, "");
	                	m_nCol=m_nCol-m_wMaxChar+2;
	                	m_nIndex=0;
	                	m_wInsertPos=0;
	                	m_nChar=0;
	                	
	                }
	                else if(m_nIndex==m_wMaxChar)
	                {
	                	strcpy(m_sz+m_wInsertPos, "");
	                	m_nIndex--;
	                	m_nChar--;	
	                	if(m_nIndex<0) m_nIndex = 0;
	                }
	                else
	                {
	                	strcpy(m_sz+m_wInsertPos-1, ""/*m_sz+m_wInsertPos*/);
	                	 m_wInsertPos--;
	                	 m_nChar--;
	               		 m_nIndex--;
	                	 if(m_nIndex<0) m_nIndex = 0;
	                }
	                if (m_paragraph.wAlign == tmPARA_ALIGN_RIGHT)
	                {
	                    //Clear(m_nCol-m_nIndex-1, m_nRow, 1);                // Above statement has problem when echo character is defined
	                    //ShowXY(m_nCol-m_nIndex, m_nRow, m_sz);
	                    
	                    Clear();                			// Above statement has problem when echo character is defined
	                    
	                    ShowXY(m_nClientWidthText-m_nIndex, m_nRow, m_sz);
	                }
	                else
	                {
	                    m_nCol--;
	                    //if (m_nCol < 1)
	                    //    {
	                    //    //=======
	                    //    //      Reposition the string if cursor is over the left margin
	                    //    //=======
	                    //    nShiftCount = (((m_nClientWidthText>>1)+1) > (m_nIndex-m_nCol))
	                    //                  ? (m_nIndex-m_nCol)
	                    //                  : ((m_nClientWidthText>>1)+1);
	                    //    m_nCol += nShiftCount;
	                    //    if (nShiftCount==0) Clear(m_nChar, m_nRow, 1);  // Show a tailing blank
	                    //    	
	                    //    ShowXY(0, m_nRow, m_sz+m_nIndex-nShiftCount);
	                    //    }
	                    //else
	                        {
	                        //Clear(m_nCol+m_nChar-m_nIndex, m_nRow, 1);      // Show a tailing blank if any
							Clear();
	                       
	                        ShowXY(0, m_nRow, m_sz);
	                        }
	                    }
	                }
	            break;
	        case _DELETE:
	        case _EXT_DELETE:
	            if (m_nIndex < m_nChar)
	                {
	                strcpy(m_sz+m_nIndex, m_sz+m_nIndex+1);
	                m_nChar--;
	                if (m_paragraph.wAlign == tmPARA_ALIGN_RIGHT)
	                    {
	                    m_nCol++;
	                    if (m_nCol >= m_nClientWidthText)
	                        {
	                        //=======
	                        //      Reposition the string if cursor is over the right margin
	                        //=======
	                        nShiftCount = (((m_nClientWidthText>>1)+1) > (m_nChar-m_nIndex))
	                                      ? (m_nChar-m_nIndex)
	                                      : ((m_nClientWidthText>>1)+1);
	                        m_nCol -= nShiftCount;
	                        }
	                    Clear(m_nCol-m_nIndex-1, m_nRow, 1);                // Show a leading blank if any
	                   
	                    ShowXY(m_nCol-m_nIndex, m_nRow, m_sz);
	                    }
	                else
	                    {
	                    Clear(m_nCol+m_nChar-m_nIndex, m_nRow, 1);          // Show a tailing blank if any
	                
	                    ShowXY(m_nCol, m_nRow, m_sz+m_nIndex);
	                    }
	                }
	            break;
	        case _KEY_CLEAR:
	            if(m_wBKSP == 0)
	            {
	            	m_bDelete = FALSE;
		            if (!m_bChanged)            //wy061901 update this function as I31
		                {
		                switch (m_wType & 0x00FF)
		                    {
		                    case tmEDIT_NUMERIC:
		                    case tmEDIT_BOOL:
		                    case tmEDIT_FLOAT:
		                        strcpy(m_sz, "0");
		                        break;
		                    case tmEDIT_HEXADECIMAL:
		                        strcpy(m_sz, "0000");
		                        break;
		                    default:
		                        *m_sz = '\0';
		                        break;
		                    }
		                ResetPosition();
		                CtmEdit::Show();
		                m_bChanged = TRUE;
		                }
		            else
		                {
		                CtmEdit::Update();            // If OnKey() is called from CEditX1 then Update() will be the CEditX1::Update()
		                m_bChanged = FALSE;
		                wKeyReturn = wKey;
		                goto _ONKEY_END;
		                }            
				}
				else if(m_wBKSP == 1 || m_wBKSP == 2)
				{
					OnKey(_BKSP);
				}	
	            break;
	        case _ESC:
	                CtmEdit::Update();            // If OnKey() is called from CEditX1 then Update() will be the CEditX1::Update()
	                m_bChanged = FALSE;
	                wKeyReturn = wKey;
	                goto _ONKEY_END;
	            break;
	        case _KEY_LEFTFIELD:
	        	if(m_wBKSP == 1 || m_wBKSP == 2)
				{
					if (m_paragraph.wAlign == tmPARA_ALIGN_RIGHT)
	                {
	                	if(m_nCol > m_nClientWidthText - m_nChar)
						{
							m_nCol--;
							m_wInsertPos--;
						}
					}
					else
					{
						if(m_nCol > 0)
						{
							m_nCol--;
							m_wInsertPos--;
						}
					}
					break;	
				}
	        case _KEY_RIGHTFIELD:
	        	if(m_wBKSP == 1 || m_wBKSP == 2)
				{
					if (m_paragraph.wAlign == tmPARA_ALIGN_RIGHT)
	                {
	                	if(m_nCol < m_nClientWidthText)
						{
							m_nCol++;
							m_wInsertPos++;
						}
					}
					else
					{
						if(m_wInsertPos < m_nChar && m_wInsertPos < m_wMaxChar-1)
						{
							m_nCol++;
							m_wInsertPos++;
						}
					}
					break;	
				}
	        case _KEY_UPFIELD:
	        case _KEY_DOWNFIELD:
	        case _UARROW:
	        case _EXT_UARROW:
	        case _DARROW:
	        case _EXT_DARROW:
	        case _SHIFT_TAB:
	        case _TAB:
	        case _ENTER:
	        case (WORD)_EXT_ENTER:
	            if ((m_wType & tmEDIT_NOEMPTY) && (m_nChar == 0))
	                ::MsgBeep(g_MultiLanguage["MSG_INPUT_NOEMPTY"]);
	            else
	            {
	                wKeyReturn = wKey;
	                goto _ONKEY_END;
	            }
	            break;
	        default:
	            wKeyReturn = wKey;
	            break;
	        }
	    }

_ONKEY_END:
Get();			//	fans add
SetCursorPosition(m_nCol, m_nRow);
DisplayCursor(TRUE);

return wKeyReturn;
}

void        CtmEdit::CalcRECT()
{
CtmWnd::CalcRECT();
CtmWnd::SetClientRECT(1, 0, -1, 0);       // Make room for vertical cursor
}

void  CtmEdit::ResetPosition()
{
	m_nChar  = strlen(m_sz);
	m_nRow   =
	m_nIndex = 0;                           // Character index in the string
	if(m_wBKSP == 0 || m_wBKSP == 2)    
	{
		if ((m_paragraph.wAlign == tmPARA_ALIGN_RIGHT) && (m_nClientWidthText > m_nChar))
	 	   m_nCol = m_nClientWidthText - m_nChar;
		else
	    	m_nCol = 0;                         // Cursor position in the edit box
	    m_wInsertPos = 0;
	}
	else if(m_wBKSP == 1)
	{
		if(m_nChar > 0)
		{
			if ((m_paragraph.wAlign == tmPARA_ALIGN_RIGHT) && (m_nClientWidthText > m_nChar))
			    m_nCol = m_nClientWidthText;
			else
			    m_nCol = strlen(m_sz);                         // Cursor position in the edit box
			
			m_nIndex = strlen(m_sz);
			m_wInsertPos = strlen(m_sz);
			if(m_wInsertPos == m_wMaxChar) 
			{
				m_wInsertPos--;
				m_nCol--;
			}
		}
		else
		{
			if ((m_paragraph.wAlign == tmPARA_ALIGN_RIGHT) && (m_nClientWidthText > m_nChar))
			    m_nCol = m_nClientWidthText;
			else
			    m_nCol = 0;                         // Cursor position in the edit box
			
			m_nIndex = 0;
			m_wInsertPos = 0;	
		}
	}
	SetCursorPosition(m_nCol, m_nRow);
}

//	fans add
void CtmEdit::SetPropertys()					//	virtual function
{
	CtmWnd::SetPropertys();	

	SetPropInfo("type", 	tmWORD, 	&m_wType);
	SetPropInfo("prompt",	tmUSTR,		&m_pszPromptLan);
	SetPropInfo("promptID",	tmSTR,		&m_pszPromptID);
	SetPropInfo("maxchar", 	tmWORD, 	&m_wMaxChar);
	SetPropInfo("echo",		tmWORD,		&m_wEcho);
	SetPropInfo("text",		tmSTR,		&m_pszText);
	SetPropInfo("backspace",tmWORD,		&m_wBKSP);
	
	
	//	fans add 2009-3-24 13:39:09
	SetPropInfo("align",	tmWORD, &m_paragraph.wAlign);
	
}


void CtmEdit::SetPropertys_Nub()					//	virtual function
{
	CtmWnd::SetPropertys_Nub();	

	SetPropInfo(EDIT_TYPE, 		tmWORD, 	&m_wType);
	SetPropInfo(EDIT_PROMPT,	tmUSTR,		&m_pszPromptLan);
	SetPropInfo(EDIT_PROMPTID,	tmSTR,		&m_pszPromptID);
	SetPropInfo(EDIT_MAXCHAR, 	tmWORD, 	&m_wMaxChar);
	SetPropInfo(EDIT_ECHO,		tmWORD,		&m_wEcho);
	SetPropInfo(EDIT_TEXT,		tmSTR,		&m_pszText);
	SetPropInfo(EDIT_BKSP,		tmWORD,		&m_wBKSP);
	
	//	fans add 2009-3-24 13:39:09
	SetPropInfo(EDIT_ALIGN,	tmWORD, &m_paragraph.wAlign);
	
}

//	fans add	
BOOL CtmEdit::CreateA()							//	virtual function
{	
	
	SetBorderWidth(m_wndattr.wBorderWidthEdit);
	CtmWnd::CreateA();

    if ((m_wType & tmEDIT_DISABLE) || (GetTabOrder() == -2))
	{
		
		m_fgc = m_wndattr.fgcEdit2;
		m_bgc = m_wndattr.bgcEdit2;	
	}
    
    
    SetFGColor(m_fgc);
    SetBGColor(m_bgc);
//    if(m_pcFont->m_nUNType==1)
//    {
//	    m_nXGapHalf = m_pcFont->m_wUNWidth >> 1;           // m_cf->m_wWidth / 2 is the half font width
//		m_nYGapHalf = m_pcFont->m_wHeight;                // m_cf->m_wHeight is the half font height
//		
//	}
//	else
	{
		 m_nXGapHalf = m_pcFont->m_wWidth >> 1;           // m_cf->m_wWidth / 2 is the half font width
		m_nYGapHalf = m_pcFont->m_wHeight;                // m_cf->m_wHeight is the half font height	
	}
	m_nClientWidthText = GetClientWidth() / m_nXGapHalf;
	m_ptaskCursor = NULL;                   // Must be assigned before call Set()
	m_bChanged = FALSE;
	
	//StrIDToStr(m_pszPromptID, &m_pszPrompt);
	if(m_pszPromptID)
		m_pszPrompt=g_MultiLanguage[m_pszPromptID];
	else
		m_pszPrompt=m_pszPromptLan;
	
	//	fans add 2009-3-24 13:58:41
	if (m_paragraph.wAlign & tmPARA_ALIGN_CENTERY)
	{		
		m_nOffSet = (m_wnd.rect.bottom - m_wnd.rect.top - m_pcFont->m_wHeight) / 2;
	}
	else if (m_paragraph.wAlign & tmPARA_ALIGN_TOP)
	{
		m_nOffSet = 0;
	}
	else if (m_paragraph.wAlign & tmPARA_ALIGN_BOTTOM)
	{
		m_nOffSet = m_wnd.rect.bottom - m_wnd.rect.top - m_pcFont->m_wHeight - m_wBorderWidth;
	}
	else 
		;
	
	if (m_nOffSet < 0) m_nOffSet = 0;
	
	SetAll();

	return TRUE;
}

void	CtmEdit::SetText(char* psz)
{
	int len = 0;
	if (psz == NULL) m_sz[0] = '\0';
	else 
	{
		len = strlen(psz);
		if (len > tmMAX_TEXT_COL) len = tmMAX_TEXT_COL;
		StrNCpy(m_sz, psz, len);		
	}
}


/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void  CtmEdit::ShowXY(short nCol, short nRow, char* psz)
{
	RECT	rect;
	if(m_rectClient.right<=0||m_rectWindow.bottom<=0)	return;
	if (g_pWndInputForm != NULL && g_pWndInput == this )
	{
		memcpy(&rect, &g_pWndInputForm->GetWindowRECT(), sizeof(RECT));
		if( (this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
			this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom) ||
			(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
			this->m_rectWindow.top >= rect.top && this->m_rectWindow.top <= rect.bottom)||
			(this->m_rectWindow.left >= rect.left  && this->m_rectWindow.left <= rect.right &&
			this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom)||
			(this->m_rectWindow.right >= rect.left  && this->m_rectWindow.right <= rect.right &&
			this->m_rectWindow.bottom >= rect.top && this->m_rectWindow.bottom <= rect.bottom))
		{
			return ;
		}
		else
		{
			;//printf("not in  this area \n");		
		}
	}
	
	//printf("%s nRow=%d , m_nYGapHalf=%d\n", Name, nRow, m_nYGapHalf);
	
	char        szEcho[tmMAX_TEXT_COL+1];
	memset(szEcho, 0, sizeof(szEcho));
	if ((tmEDIT_PASSWORD & m_wType))	//	passWord
	{
		m_wEcho = '*';
	}
	else m_wEcho = _NULL_KEY;
	
	Clear(nCol, nRow, strlen(psz));
	if(m_wEcho == _NULL_KEY)
    	CtmWnd::ShowExtent(nCol*m_nXGapHalf, nRow*m_nYGapHalf + m_nOffSet, psz, 0);
	else
    {
		int  len = strlen(psz);
	    if (len >= 1)
	    {
	        StrNSet(szEcho, (char)m_wEcho, len);
	        CtmWnd::Clear();	//	fans add 
	        int i = 0;
	        if( m_wType != 0x85 )
	        {
	        	//cout << "m_bFInputFlag =" << m_bFInputFlag << endl;
	        	if( nCol >= 0 && m_bFInputFlag == TRUE)
			        m_bDelete = TRUE;
			    else
		        	m_bDelete = FALSE;
		        
		        for (i = 0; i < nCol; i++)
		       	{
		        	CtmWnd::ShowExtent(i * m_nXGapHalf, nRow*m_nYGapHalf + m_nOffSet, szEcho, 0);
		        }
		        CtmWnd::ShowExtent(i * m_nXGapHalf, nRow*m_nYGapHalf + m_nOffSet, psz, 0);  				//設置密碼輸入  James add 2007/9/5 04:53下午
		        if( m_bDelete )
		       	{
		        	Delay(200);
		 		}
		        CtmWnd::Clear();
		        for( int j =0; j <= nCol; j++)
		        {
		        	CtmWnd::ShowExtent(j * m_nXGapHalf, nRow*m_nYGapHalf + m_nOffSet, szEcho, 0);	
		        }
		        m_bDelete = TRUE;
		     }
		     else
		     {
		     	if( m_bOverPassword == TRUE )
		     	{
		     		//cout << "m_bFInputFlag =" << m_bFInputFlag << endl;
		     		if( nCol >= 0 && m_bFInputFlag == TRUE )
			        	m_bDelete = TRUE;
			    	else
		        		m_bDelete = FALSE;
		        
			     	for (i = 0; i < nCol; i++)
			       	{
			        	CtmWnd::ShowExtent(i * m_nXGapHalf, nRow*m_nYGapHalf + m_nOffSet, szEcho, 0);
			        }
			        CtmWnd::ShowExtent(i * m_nXGapHalf, nRow*m_nYGapHalf + m_nOffSet, psz, 0);  				//設置密碼輸入  James add 2007/9/5 04:53下午
			        if( m_bDelete )
			       	{
			        	Delay(200);
			 		}
			        CtmWnd::Clear();
			        for( int j =0; j <= nCol; j++)
			        {
			        	CtmWnd::ShowExtent(j * m_nXGapHalf, nRow*m_nYGapHalf + m_nOffSet, szEcho, 0);	
			        }
			        m_bDelete = TRUE;
			        m_bFInputFlag = TRUE;
			    }
			    m_bOverPassword = TRUE;
		     }
	    }
	    else
	    {
	    	CtmWnd::Clear();	//	fans add
			/*
			CtmWnd::ShowExtent(nCol*m_nXGapHalf, nRow*m_nYGapHalf, psz, 0);
	        WriteEcho(nCol, nRow, (char)m_wEcho);
	        */
	    }
    
    }
  
}

void  CtmEdit::Clear(short nCol, short nRow, short nCount)
{
	
	//if (!(m_wnd.wStyle & tmWS_TRANSPARENT))
	//{
	//	RECT        rect;
	//	
	//	rect.left   = nCol * m_nXGapHalf;
	//	rect.top    = nRow * m_nYGapHalf;
	//	rect.right  = rect.left + m_nXGapHalf * nCount - 1;
	//	rect.bottom = rect.top  + m_nYGapHalf - 1;
	//	
	//	
	//	CtmWnd::Clear(rect);
	//}
	
	
	//	fans 2008-11-18 14:27:59 
	if (!(m_wnd.wStyle & tmWS_TRANSPARENT))
	{
		//Mario mark 避免使用漸層效果時，受到此功能影響
		/*RECT        rect;
		
		rect.left   = nCol * m_nXGapHalf;
		rect.top    = nRow * m_nYGapHalf;// + m_nOffSet;
		rect.right  = rect.left + m_nXGapHalf * nCount - 1;		
		rect.bottom	= rect.top + GetClientBottom();
		printf("123\n");
		CtmWnd::Clear(rect);*/
		CtmWnd::Clear();
	}
	else		//James add 2010/12/7 上午 09:14:24
	{
		if(m_taborder >= 0)
		{	
			if (!m_bBackGround) 
	        {
	            m_BackImage.CopyImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
	                              m_rectClient.bottom);
	            m_bBackGround = TRUE;
	        }
	        else
	        {
	            m_BackImage.ShowImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
	                              m_rectClient.bottom);
	        }
	    }
	}
}

void  CtmEdit::EnableCursor(BOOL bEnable)
{
	TASKCURSOR  taskcursor;
	
	if (bEnable)
	{
		if (m_ptaskCursor == NULL)
		{
			taskcursor.pwnd = this;
			m_ptaskCursor = new CTaskCursor(NULL, &taskcursor);
		}
	}
	else
	{
		if (m_ptaskCursor != NULL)
		{
			DisplayCursor(FALSE);
			delete m_ptaskCursor;
			m_ptaskCursor = NULL;
		}
	}
}

void  CtmEdit::DisplayCursor(BOOL bOn)
{
	if (m_ptaskCursor != NULL)
    	m_ptaskCursor->DisplayCursor(bOn);
}

void  CtmEdit::SetCursorPosition(short nCol, short nRow)
{

if (m_ptaskCursor != NULL)
    m_ptaskCursor->SetCursorPosition(nCol*m_nXGapHalf, nRow*m_nYGapHalf + m_nOffSet);

}

BOOL  CtmEdit::GetWaitStatus()           //St1116'02 for ABC input
{
if (m_ptaskCursor != NULL)
    return m_ptaskCursor->IsWaiting();
else
    return FALSE;
}

void  CtmEdit::CursorWait(BOOL bWait)              //St1116'02 for ABC input
{
if (m_ptaskCursor != NULL)
    m_ptaskCursor->SetWaitStatus(bWait);
}

void  CtmEdit::WriteEcho(short nX, short nY, char cEcho)              //St1116'02 for ABC input
{
if (m_ptaskCursor != NULL)
    m_ptaskCursor->SetEcho( nX*m_nXGapHalf, nY*m_nYGapHalf, cEcho);
}

/*===========================================================================+
|           Class implementation - EditBox                                   |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/


//	fans add
CtmEditX1::CtmEditX1(): CtmEdit()
{
	strcpy(TYPEName, "CtmEditX1");

	m_lMin = 0;
	m_lMax = 0;
	m_lValue =0;
	m_nPrecision = 0;
	m_wType = tmEDIT_FLOAT|tmEDIT_NOEMPTY;
	m_bGetDB = FALSE;
	for (int i = 0; i < MAXDBNUM; i++)	m_pszDBID[i] = NULL;	
	m_pszMaxDBID = NULL;
	m_pszMinDBID = NULL;
	m_pszMaxEditName = NULL;
	m_pszMinEditName = NULL;

	m_lOldValue = 0;
	m_nExPrecision = 0;
	
	m_bDBMax 		= TRUE;
	m_bDBMin 		= TRUE;

}


CtmEditX1::~CtmEditX1()
{
	
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
int		CtmEditX1::OnLose()
{
	CtmEdit::OnLoseFocus();
	m_bPropertyChanged = TRUE;
	m_lValue = m_lOldValue;
	Update();
	return 0;
}

void        CtmEditX1::Get()              /* Virtual function */
{
	CtmEdit::Get();
	if (m_wType & tmEDIT_TIMEDIVTEN)            //Steven 2004/3/5 for Time div 10
	    m_lValue = A2F(m_sz, m_nPrecision)*10;
	else
	    m_lValue = A2F(m_sz, m_nPrecision);

}

void        CtmEditX1::Set()              /* Virtual function */
{
	// ==============
	//	以下的註釋是需要特別注意的
	// ==============

	GetDBInfo(TRUE);
	
	if (m_pszDBID[0] != NULL) m_lOldValue = m_lValue;
	/*
	if (m_pszDBID[0] != NULL)
	{
		DBVIEWVALUE dbViewValue; 
		//char pszTemp[256];
		//m_nExPrecision = 3;
		dbViewValue = GetDBValue(m_pszDBID[0]);
		m_lValue = dbViewValue.lValue / Power(10, m_nExPrecision);
		m_lOldValue = m_lValue;
		if (m_pszMaxDBID == NULL)
		{
			if (m_lMax == 0)
				m_lMax = dbViewValue.lMaxValue;
		}
		else
		{
			m_lMax = GetDBValue(m_pszMaxDBID).lValue;
		}
		
		if (m_pszMinDBID == NULL)
		{
			if (m_lMin == 0)
				m_lMin = dbViewValue.lMinValue;
		}
		else
		{
			m_lMin = GetDBValue(m_pszMinDBID).lValue;
		}
		m_nPrecisionData = dbViewValue.wPrecision;
		if (m_nPrecision  == 0) m_nPrecision += m_nPrecisionData;
	}
	*/
	
	if (m_wType & tmEDIT_TIMEDIVTEN)            //Steven 2004/3/5 for Time div 10
	    F2A(m_sz, m_nPrecision, m_lValue/10);
	else
	    F2A(m_sz, m_nPrecision, m_lValue);
	CtmEdit::Get();
	ResetPosition();
	
	//CtmEdit::Set();
}

//	fans add
void		CtmEditX1::ReSet()
{
	// ==============
	//	以下的註釋是需要特別注意的
	// ==============
	
	
	GetDBInfo(TRUE);
	
//	if (m_pszPromptID != NULL)    
//		StrIDToStr(m_pszPromptID, &m_pszPrompt);
	if(m_pszPromptID)
		m_pszPrompt=g_MultiLanguage[m_pszPromptID];
	else
		m_pszPrompt=m_pszPromptLan;	
	Prepare();
	CtmEdit::Get();
	ResetPosition();
}

void        CtmEditX1::SetAll()           /* Virtual function */
{
	Prepare();
	CtmEdit::SetAll();
}


void        CtmEditX1::Update()           /* Virtual function */
{
	//F2A(m_sz, m_nPrecision, m_lValue);
	DBVIEWVALUE dbViewValue; 
	dbViewValue = GetDBValue(m_pszDBID[0]);

	if (m_pszMaxDBID == NULL)
	{
		if (m_bDBMax)
			m_lMax = dbViewValue.lMaxValue;
	}
	else
	{
		if (!m_bDBMax)
			m_lMax = GetDBValue(m_pszMaxDBID).lValue;
	}
	
	if (m_pszMinDBID == NULL)
	{
		if (m_bDBMin)
			m_lMin = dbViewValue.lMinValue;
	}
	else
	{
		if (!m_bDBMin)
			m_lMin = GetDBValue(m_pszMinDBID).lValue;
	}
	
	
	/*if (m_iPrivilege > g_iPrivilege) 
	{
		DrawBorder();
		return;
	}*/
	if (!m_bOnFocus)
  {
		ReSet();
		char sz[256];
		
		if ((m_lValue != m_lOldValue) || m_bPropertyChanged)
		{
			F2A(m_pszText, m_nPrecision, m_lValue);
			CtmEdit::Show();
			m_lOldValue = m_lValue;
		}
		else if (m_lValue == m_lOldValue)
		{
			F2A(sz, m_nPrecision, m_lValue);
			if (strcmp(sz, m_pszText) != 0)
			{
				strcpy(m_pszText, sz);
				CtmEdit::Show();
			}
		}
  }
  m_bPropertyChanged = FALSE;
}

void        CtmEditX1::UpdateAll()        /* Virtual function */
{
	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
	this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)
	{
		SetAll();
		CtmEdit::Show();
	}
}

WORD        CtmEditX1::OnKey(WORD wKey)   /* Virtual function */
{
	//m_wMaxChar
	
	char	szTemp[256];
	F2A(szTemp, m_nPrecision, m_lMax);
	m_wMaxChar = strlen(szTemp);
	
	//James add 增加負號輸入的時候 長度不夠 2009/2/3 10:37上午
	if(m_lMin < 0 )
	{
		F2A(szTemp, m_nPrecision, m_lMin);
		if(m_wMaxChar < strlen(szTemp))
		{
			m_wMaxChar = strlen(szTemp);
		}
	}
	
	long long   llValue = 0;
	long long   editx1Max = m_lMax;			// wy080800
	char		cKey, *pStr, acStr[10]; 	// zholy07803
	int			nDecimalPosi = 0;				// zholy07803

//=======
//      Interpret function key from the keyboard
//=======
	switch (wKey)
    {
    case _KEY_INCREASE:                 // Intend to use _KEY_INCREASE instead of _UARROW
    case _KEY_DECREASE:
        
        	
        llValue = A2F(m_sz, m_nPrecision);
        if (wKey == _KEY_INCREASE) llValue++;
        else                       llValue--;
     
        if ((llValue >= m_lMin) && (llValue <= editx1Max))
		{
            F2A(m_sz, m_nPrecision, llValue);
        	CtmEdit::Get();    
            CtmEdit::Update();
		}
        return _NULL_KEY;
   //茞极豖跡瑩ㄛ瑩硉2015/10/21 16:55:08
   	case _KEY_PRINT:
	    OnKey(_BKSP);
	break;
    case _KEY_CLEAR:
    	//printf("EditX1 OnKey_Key_Clear!!\n");
    	m_lValue=m_lOldValue;
    	if(m_wBKSP == 0)
	    {
	    	F2A(m_sz, m_nPrecision, m_lOldValue);
    		CtmEdit::Get();
	    	CtmEdit::Update();
	    	m_bChanged = FALSE;
	    	return _NULL_KEY;
	    }
	    else if(m_wBKSP == 1 || m_wBKSP == 2)
	    {
	    	OnKey(_BKSP);
	    	break;
	    }
    default:
    	//printf("m_nIndex= %d, m_nChar= %d, m_nCol= %d, m_wMaxChar= %d\n", m_nIndex,m_nChar,m_nCol, m_wMaxChar);
    	if (tmHIBYTE(wKey) == 0)			// zholy07803 Validate the decimal
    	{
		    cKey = tmLOBYTE(wKey);
		    if(cKey < 0x20 || cKey > 0x7E) return wKey;
		    if (((cKey >= '0') && (cKey <= '9')) || (cKey == '.') || (cKey == '-'))
		    {
		    	if (!m_bChanged) pStr = NULL;
		    	else pStr = strchr(m_sz, '.');
		    	if (!pStr) nDecimalPosi = 0;
		    	else
		    	{
		    		memset(acStr, '\0', sizeof(acStr));
		    		strncpy(acStr, pStr, sizeof(acStr));
		    		nDecimalPosi = strlen(acStr);
		    	}
		    	
		    	if (nDecimalPosi > 0)
		    	{
		    		//if (cKey == '.') return _NULL_KEY;
		    		//else
		    		//{
					//	if ((m_nPrecision == nDecimalPosi-1) && (m_nIndex == m_nChar)) return _NULL_KEY;
		    		//}
		    	}
		    	else
		    	{
		    		if (m_nPrecision == 0)
		    		{
		    			if (cKey == '.') return _NULL_KEY;
		    		}
		    		//else
		    		//{
		    		//	if (m_bChanged)
		    		//	{
		    		//		if (m_nPrecision < m_nChar-m_nIndex-1) return _NULL_KEY;
		    		//	}
		    		//}
		    	}
		    }
    	}
    		
    	wKey = CtmEdit::OnKey(wKey);
		break;
	}

	switch (wKey)
    {
    case _KEY_LEFTFIELD:
    case _KEY_RIGHTFIELD:
    case _KEY_UPFIELD:
    case _KEY_DOWNFIELD:
    case _UARROW:
    case _EXT_UARROW:
    case _DARROW:
    case _EXT_DARROW:
    case _TAB:
    case _SHIFT_TAB:
    case _ENTER:    	
    case (WORD)_EXT_ENTER:
        CtmEdit::Get();
        
        llValue = A2F(m_sz, m_nPrecision);  
        //	fans mark 2006/12/30 11:55上午
        //if ((llValue < m_lMin) ||  (llValue > editx1Max)) 
		//{
		//	m_lValue = llValue;
        //    CtmEdit::ResetPosition();
        //    return _NULL_KEY;
		//}
		
		
        //	fans add 2006/12/30 11:47上午
        if (m_lMin == 0xFFFF && m_lMax == 0xFFFF)
        {
        	//James add 2008/4/29 01:27下午 當輸入的值超過0xFFFF不能存
        	if  (llValue > editx1Max)
        	{
				m_lValue = llValue;
	            CtmEdit::ResetPosition();
	            SendMsg(MSG_SH_SET_DBVALUE_OVERMAX, 0, 0, NULL);	//James add 2011/5/20 09:25:26
	            return _NULL_KEY;
			}
			else
			{
	        	break;
	        }
			//m_lValue = llValue;
            //return _NULL_KEY;
			
		}
		else if (m_lMin == 0xFFFF &&  m_lMax != 0xFFFF)
		{
			if  (llValue > editx1Max)
			{
				m_lValue = llValue;
	            CtmEdit::ResetPosition();
	            SendMsg(MSG_SH_SET_DBVALUE_OVERMAX, 0, 0, NULL);	//James add 2011/5/20 09:25:26
	            return _NULL_KEY;
			}
		}
		else if (m_lMin != 0xFFFF &&  m_lMax == 0xFFFF)
		{
			 if  (llValue < m_lMin || llValue > editx1Max)
			{
				m_lValue = llValue;
	            CtmEdit::ResetPosition();
	            	
	            if(llValue < m_lMin)								//James add 2011/5/20 09:25:26
	            {
	            	SendMsg(MSG_SH_SET_DBVALUE_OVERMIN, 0, 0, NULL);
	            }
	            else if(llValue > editx1Max)
	            {
	            	SendMsg(MSG_SH_SET_DBVALUE_OVERMAX, 0, 0, NULL);	
	            }
	            return _NULL_KEY;
			}
		}
		else
		{
			if ((llValue < m_lMin) ||  (llValue > editx1Max))  
			{                                                  
				m_lValue = llValue;                              
			    CtmEdit::ResetPosition(); 
			    m_bChanged = FALSE; 
                 
                if(llValue < m_lMin)								//James add 2011/5/20 09:25:26
	            {
	            	SendMsg(MSG_SH_SET_DBVALUE_OVERMIN, 0, 0, NULL);
	            }
	            else if(llValue > editx1Max)
	            {
	            	SendMsg(MSG_SH_SET_DBVALUE_OVERMAX, 0, 0, NULL);	
	            }   
			    return _NULL_KEY;                              
			}        
		}
		
		
        break;
    
    default:
        break;
    }

	return wKey;
}

//	fans add
void 	CtmEditX1::SetPropertys()
{
	CtmEdit::SetPropertys();

	SetPropInfo("precision", 	tmWORD, 	&m_nPrecision);
	SetPropInfo("value", 		tmINT64, 	&m_lValue);	
	SetPropInfo("min", 			tmINT64, 	&m_lMin);
	SetPropInfo("max", 			tmINT64, 	&m_lMax);
	SetPropInfo("minid", 		tmSTR,		&m_pszMinDBID);
	SetPropInfo("maxid", 		tmSTR,		&m_pszMaxDBID);
	SetPropInfo("maxdb",		tmBOOL,		&m_bDBMax);
	SetPropInfo("mindb",		tmBOOL,		&m_bDBMin);
	SetPropInfo("maxEditName", 	tmSTR, 		&m_pszMaxEditName);
	SetPropInfo("minEditName", 	tmSTR, 		&m_pszMinEditName);
	SetPropInfo("spepre",	tmINT,		&m_nExPrecision);
	
	char sz[10];
	for (int i = 0; i < MAXDBNUM; i++)
	{
		sprintf(sz, "%s%d", "dbid", i);
		SetPropInfo(sz,		tmSTR, 		&m_pszDBID[i]);	
	}

}

void 	CtmEditX1::SetPropertys_Nub()
{
	CtmEdit::SetPropertys_Nub();

	SetPropInfo(EDITX1_PRECISION, 	tmWORD, 	&m_nPrecision);
	SetPropInfo(EDITX1_VALUE, 		tmINT64, 	&m_lValue);	
	SetPropInfo(EDITX1_MIN, 		tmINT64, 	&m_lMin);
	SetPropInfo(EDITX1_MAX, 		tmINT64, 	&m_lMax);
	SetPropInfo(EDITX1_MINID, 		tmSTR,		&m_pszMinDBID);
	SetPropInfo(EDITX1_MAXID, 		tmSTR,		&m_pszMaxDBID);
	SetPropInfo(EDITX1_MAXDB,		tmBOOL,		&m_bDBMax);
	SetPropInfo(EDITX1_MINDB,		tmBOOL,		&m_bDBMin);
	SetPropInfo(EDITX1_MAXEDITNAME, tmSTR, 		&m_pszMaxEditName);
	SetPropInfo(EDITX1_MINEDITNAME, tmSTR, 		&m_pszMinEditName);
	SetPropInfo(EDITX1_SPEPRE,		tmINT,			&m_nExPrecision);
	
	int	nIndex = EDITX1_DBID0;
	for (int i = 0; i < MAXDBNUM; i++)
	{
		SetPropInfo(nIndex+i,		tmSTR, 		&m_pszDBID[i]);	
	}

}

void	CtmEditX1::GetDBInfo(BOOL	bPrecisionZero)
{
	DBVIEWVALUE dbViewValue; 
	if (m_pszDBID[0] != NULL)
	{
		//char pszTemp[256];
		dbViewValue = GetDBValue(m_pszDBID[0]);
		m_lValue = dbViewValue.lValue / Power(10, m_nExPrecision); 
		//m_lOldValue = m_lValue;
		
		if (m_bDBMax)
			m_lMax = dbViewValue.lMaxValue;
		else
		{
			if (m_pszMaxDBID != NULL)
			{
				if (!m_bDBMax)
					m_lMax = GetDBValue(m_pszMaxDBID).lValue;
			}
		}
		
		if (m_bDBMin)
			m_lMin = dbViewValue.lMinValue;
		else
		{
			if (m_pszMinDBID != NULL)
			{
				if (!m_bDBMin)
					m_lMin = GetDBValue(m_pszMinDBID).lValue;
			}
		}
		
		m_nPrecisionData = dbViewValue.wPrecision;
		
		if (!bPrecisionZero) m_nPrecision += m_nPrecisionData;
		else
		{
			if (m_nPrecision == 0) m_nPrecision = m_nPrecisionData;
		}
		//cout << Name << " SET():: m_nPrecision:"<< m_nPrecision << " :m_nPrecisionData" <<m_nPrecisionData << endl;
		//	需要獲取權限
		m_iPrivilege = dbViewValue.dwPrivilege;
		//m_bGetDB = TRUE;
	}
	
	//	get max for edit
	if (m_pszMaxDBID == NULL)
	{
		
		if (m_pszMaxEditName != NULL)
		{
			if (!m_bDBMax)
			{
				CtmWnd*	pwndTemp = this->GetParent()->FindControlFromName(m_pszMaxEditName);
				if (pwndTemp != NULL && pwndTemp->Is(TYPEName))
					pwndTemp->GetPropValueT("value", &m_lMax, sizeof(m_lMax));
			}
		}
	}
	
	if (m_pszMinDBID == NULL)
	{
		if (m_pszMinEditName != NULL)
		{
			if (!m_bDBMin)
			{	
				CtmWnd*	pwndTemp = this->GetParent()->FindControlFromName(m_pszMinEditName);
				if (pwndTemp != NULL && pwndTemp->Is(TYPEName))
					pwndTemp->GetPropValueT("value", &m_lMin, sizeof(m_lMin));
			}
		}
	}
}

//	fans add
BOOL 	CtmEditX1::CreateA()		//	fans add
{
	//Set();

	DBVIEWVALUE dbViewValue; 
	
	if (!m_bGetDB)
	{
		if (m_pszDBID[0] != NULL)
		{
			GetDBInfo(FALSE);
			m_lOldValue = m_lValue;
			m_bGetDB = TRUE;
		}
	}

	CtmEdit::CreateA();	
	return TRUE;
}

//	fans add
void    CtmEditX1::Show()
{

	if (m_wnd.wStyle & tmWS_REDRAW)  ShowBg();
	//Set();
	CtmEdit::Show();
	
}

//	fans add
void    CtmEditX1::OnLoseFocus()
{

	int iTemp = 0;
	//=============
	//	檢測有幾個對應的資料
	//=============	
	for (int i = 0; i < MAXDBNUM; i++)
	{
		if (m_pszDBID[i] != NULL) iTemp++;
	}
	
	//=============
	//	只有一個的話,將會自動保存;因為如果有多個的話,有可能會有各種計算,將不會有自動的操作
	//=============	
	if (m_pszDBID[0] != NULL&&!g_bFocusChanged)
	{	
		//	2006/12/30 12:40下午 for oil
		if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)
		{
			if ((m_lValue >= m_lMin) && (m_lValue <= m_lMax))
			{
				if (m_lOldValue != m_lValue)
				{
					m_lValue = m_lValue * Power(10, m_nExPrecision);
					SetViewDBValue(m_pszDBID[0], m_lValue);
					m_lValue /= Power(10, m_nExPrecision);
				}
			}
		}
		//	2006/12/30 12:51下午	fans add
		else
		{
			if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
			{
				//James add ---- && (m_lValue <= m_lMax) 2008/4/29 11:10上午
				if ((m_lValue >= m_lMin) && (m_lValue <= m_lMax))
				{
					if (m_lOldValue != m_lValue)
					{
						m_lValue = m_lValue * Power(10, m_nExPrecision);
						SetViewDBValue(m_pszDBID[0], m_lValue);
						m_lValue /= Power(10, m_nExPrecision);
					}
				}
			}
			else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
			{
				if ((m_lValue <= m_lMax))
				{
					if (m_lOldValue != m_lValue)
					{
						m_lValue = m_lValue * Power(10, m_nExPrecision);
						SetViewDBValue(m_pszDBID[0], m_lValue);
						m_lValue /= Power(10, m_nExPrecision);
					}
				}
			}
			else
			{
				//James add ---- && (m_lValue <= m_lMax) 2008/4/29 11:10上午
				if (m_lOldValue != m_lValue && (m_lValue <= m_lMax))
				{
					m_lValue = m_lValue * Power(10, m_nExPrecision);
					SetViewDBValue(m_pszDBID[0], m_lValue);
					m_lValue /= Power(10, m_nExPrecision);
				}
			}
			
			
		}
		
	}
	CtmEdit::OnLoseFocus();
	Update();
	m_lOldValue = m_lValue;
}

void        CtmEditX1::OnGetFocus()         /* Virtual function */
{
	 
	if(m_lMin < 0 )
	{
		m_bValueSymbol = TRUE;
	}
	else
	{
		m_bValueSymbol = FALSE;	
	}

	Update();	//	fans	2009/1/5 02:30下午

	OnGetFocus1();
	CtmWnd::DrawGetFocus();
	Prompt(m_szPrompt0);

}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	CtmEditX1::Prepare()
{
	char        szMin[256];    //netdigger 2007/4/12
	char        szMax[256];    //netdigger 2007/4/12
	long long   editx1Max;      //wy080800               
	int         nPosition;
	WORD    *pwSrc, *pwDst;
	int 		nDBIndex ;
	long 		lDataID;
	
    if(m_pszDBID[0] != NULL && g_biNetLimit) //Caozz 2010/8/17 10:43上午 
    {
    	//if(g_iPrivilege < CtmConfig::GetInstance()->GetiNetPrivFlag())
    	//{
    		nDBIndex = g_pDatabase->GetIndex(m_pszDBID[0]);
    		lDataID  = g_pDatabase->IndexToDataID(nDBIndex);
    		for(int i = 0; i < g_wiNetLimitDBNum; i++)
			{
				if(lDataID == g_piNetLimit[i].dwDBID)
				{
					m_lMax = g_piNetLimit[i].dwUpperLimit ;
					m_lMin = g_piNetLimit[i].dwLowerLimit ;
					break;
				}
			}
		//}
    }
    editx1Max  = m_lMax;
    
    LL2A(szMin, m_nPrecision, m_lMin, m_pcFont->GetFontCode());    //netdigger 2007/4/12
	LL2A(szMax, m_nPrecision, editx1Max, m_pcFont->GetFontCode());    //netdigger 2007/4/12
    
	if(g_bDemoDataID)
	{
		if(m_pszDBID[0] != NULL)
		{
			lDataID  = g_pDatabase->GetDataID(m_pszDBID[0]);
		}
	}
	//add by J.Wang for freetype
    if(CtmConfig::GetInstance()->GetFontConvert())
    {	
	    if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
	    {
			if(g_bDemoDataID)
			{
				memset(m_szPrompt0, 0, tmMAX_TEXT_COL);  
				if (m_lMin == 0xFFFF &&  m_lMax == 0xFFFF)			//James add 2015/1/20 11:32:25
					tm_sprintf(m_szPrompt0, "%x %t", lDataID, m_pszPrompt);      
				else if (m_lMin == 0xFFFFFFFF &&  m_lMax == 0xFFFFFFFF)
					tm_sprintf(m_szPrompt0, "%x %t", lDataID, m_pszPrompt);
				else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t: %t %t", lDataID,g_MultiLanguage["MSG_MAX"], szMax, m_pszPrompt );
				else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t: %t %t", lDataID,g_MultiLanguage["MSG_MIN"], szMin, m_pszPrompt );
				else if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t: %t %t: %t  %t", lDataID, 
	        					g_MultiLanguage["MSG_MIN"], szMin,
	        				 	g_MultiLanguage["MSG_MAX"], szMax,m_pszPrompt);
	//        	printf("1:m_szPrompt0=%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
	//				m_szPrompt0[0],m_szPrompt0[1],m_szPrompt0[2],m_szPrompt0[3],m_szPrompt0[4],
	//				m_szPrompt0[5],m_szPrompt0[6],m_szPrompt0[7],m_szPrompt0[8],m_szPrompt0[9],
	//				m_szPrompt0[10],m_szPrompt0[11],m_szPrompt0[12],m_szPrompt0[13],m_szPrompt0[14],
	//				m_szPrompt0[15],m_szPrompt0[16],m_szPrompt0[17],m_szPrompt0[18],m_szPrompt0[19],
	//				m_szPrompt0[20],m_szPrompt0[21],m_szPrompt0[22],m_szPrompt0[23],m_szPrompt0[24],
	//				m_szPrompt0[25],m_szPrompt0[26],m_szPrompt0[27],m_szPrompt0[28],m_szPrompt0[29],
	//				m_szPrompt0[30],m_szPrompt0[31],m_szPrompt0[32],m_szPrompt0[33],m_szPrompt0[34],
	//				m_szPrompt0[35],m_szPrompt0[36],m_szPrompt0[37],m_szPrompt0[38],m_szPrompt0[39],
	//				m_szPrompt0[40],m_szPrompt0[41],m_szPrompt0[42],m_szPrompt0[43],m_szPrompt0[44],
	//				m_szPrompt0[45],m_szPrompt0[46],m_szPrompt0[47],m_szPrompt0[48],m_szPrompt0[49]);
			}
			else
			{
				memset(m_szPrompt0, 0, tmMAX_TEXT_COL);  
				if (m_lMin == 0xFFFF &&  m_lMax == 0xFFFF)			//James add 2015/1/20 11:32:25
					tm_sprintf(m_szPrompt0, "%t", m_pszPrompt);      
				else if (m_lMin == 0xFFFFFFFF &&  m_lMax == 0xFFFFFFFF)
					tm_sprintf(m_szPrompt0, "%t", m_pszPrompt);
				else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t: %t %t", g_MultiLanguage["MSG_MAX"], szMax, m_pszPrompt);
				else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t: %t %t", g_MultiLanguage["MSG_MIN"], szMin, m_pszPrompt);
				else if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t: %t %t: %t %t", 
									g_MultiLanguage["MSG_MIN"], szMin,
									g_MultiLanguage["MSG_MAX"], szMax,m_pszPrompt);
	//				printf("2:m_szPrompt0=%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
	//				m_szPrompt0[0],m_szPrompt0[1],m_szPrompt0[2],m_szPrompt0[3],m_szPrompt0[4],
	//				m_szPrompt0[5],m_szPrompt0[6],m_szPrompt0[7],m_szPrompt0[8],m_szPrompt0[9],
	//				m_szPrompt0[10],m_szPrompt0[11],m_szPrompt0[12],m_szPrompt0[13],m_szPrompt0[14],
	//				m_szPrompt0[15],m_szPrompt0[16],m_szPrompt0[17],m_szPrompt0[18],m_szPrompt0[19],
	//				m_szPrompt0[20],m_szPrompt0[21],m_szPrompt0[22],m_szPrompt0[23],m_szPrompt0[24],
	//				m_szPrompt0[25],m_szPrompt0[26],m_szPrompt0[27],m_szPrompt0[28],m_szPrompt0[29],
	//				m_szPrompt0[30],m_szPrompt0[31],m_szPrompt0[32],m_szPrompt0[33],m_szPrompt0[34],
	//				m_szPrompt0[35],m_szPrompt0[36],m_szPrompt0[37],m_szPrompt0[38],m_szPrompt0[39],
	//				m_szPrompt0[40],m_szPrompt0[41],m_szPrompt0[42],m_szPrompt0[43],m_szPrompt0[44],
	//				m_szPrompt0[45],m_szPrompt0[46],m_szPrompt0[47],m_szPrompt0[48],m_szPrompt0[49]);
			}
	    }
	    else
	    {	
	        if (m_pszPrompt != NULL)
	        {
	        	if ((strlen(m_pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
	    		strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL)
	    		{
	        		if (m_lMin != 0)
	           			sprintf(m_szPrompt0, "%s:%s %s:%s %s",  g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax,m_pszPrompt);
	        		else
	           			sprintf(m_szPrompt0, "%s:%s %s",g_MultiLanguage["MSG_MAX"], szMax, m_pszPrompt);
	           	}
	        }
	    	else sprintf(m_szPrompt0, "%s:%s %s:%s"   , g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
	    	
	    	//	fans add 2006/12/30 11:59上午
			if (m_lMin == 0xFFFF && m_lMax == 0xFFFF) 
			{
				if (m_pszPrompt != NULL) strncpy(m_szPrompt0, m_pszPrompt, tmMAX_TEXT_COL);
				else m_szPrompt0[0]='\0';
			}
			else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF) 
			{
				sprintf(m_szPrompt0, "%s:%s %s",g_MultiLanguage["MSG_MIN"], szMin, m_pszPrompt);
			}
			else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
		
	    		sprintf(m_szPrompt0, "%s:%s %s",g_MultiLanguage["MSG_MAX"], szMax,m_pszPrompt);
	    }
	}
	else
	{
		
	    if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
	    {
			if(g_bDemoDataID)
			{
				memset(m_szPrompt0, 0, tmMAX_TEXT_COL);  
				if (m_lMin == 0xFFFF &&  m_lMax == 0xFFFF)			//James add 2015/1/20 11:32:25
					tm_sprintf(m_szPrompt0, "%x %t", lDataID, m_pszPrompt);      
				else if (m_lMin == 0xFFFFFFFF &&  m_lMax == 0xFFFFFFFF)
					tm_sprintf(m_szPrompt0, "%x %t", lDataID, m_pszPrompt);
				else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t %t: %t", lDataID, m_pszPrompt, g_MultiLanguage["MSG_MAX"], szMax);
				else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t %t: %t", lDataID, m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin);
				else if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t %t: %t %t: %t", lDataID, 
	        				m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
	        				g_MultiLanguage["MSG_MAX"], szMax);
	//        	printf("1:m_szPrompt0=%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
	//				m_szPrompt0[0],m_szPrompt0[1],m_szPrompt0[2],m_szPrompt0[3],m_szPrompt0[4],
	//				m_szPrompt0[5],m_szPrompt0[6],m_szPrompt0[7],m_szPrompt0[8],m_szPrompt0[9],
	//				m_szPrompt0[10],m_szPrompt0[11],m_szPrompt0[12],m_szPrompt0[13],m_szPrompt0[14],
	//				m_szPrompt0[15],m_szPrompt0[16],m_szPrompt0[17],m_szPrompt0[18],m_szPrompt0[19],
	//				m_szPrompt0[20],m_szPrompt0[21],m_szPrompt0[22],m_szPrompt0[23],m_szPrompt0[24],
	//				m_szPrompt0[25],m_szPrompt0[26],m_szPrompt0[27],m_szPrompt0[28],m_szPrompt0[29],
	//				m_szPrompt0[30],m_szPrompt0[31],m_szPrompt0[32],m_szPrompt0[33],m_szPrompt0[34],
	//				m_szPrompt0[35],m_szPrompt0[36],m_szPrompt0[37],m_szPrompt0[38],m_szPrompt0[39],
	//				m_szPrompt0[40],m_szPrompt0[41],m_szPrompt0[42],m_szPrompt0[43],m_szPrompt0[44],
	//				m_szPrompt0[45],m_szPrompt0[46],m_szPrompt0[47],m_szPrompt0[48],m_szPrompt0[49]);
			}
			else
			{
				memset(m_szPrompt0, 0, tmMAX_TEXT_COL);  
				if (m_lMin == 0xFFFF &&  m_lMax == 0xFFFF)			//James add 2015/1/20 11:32:25
					tm_sprintf(m_szPrompt0, "%t", m_pszPrompt);      
				else if (m_lMin == 0xFFFFFFFF &&  m_lMax == 0xFFFFFFFF)
					tm_sprintf(m_szPrompt0, "%t", m_pszPrompt);
				else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t %t: %t", m_pszPrompt, g_MultiLanguage["MSG_MAX"], szMax);
				else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t %t: %t", m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin);
				else if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t %t: %t %t: %t", 
									m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
									g_MultiLanguage["MSG_MAX"], szMax);
	//				printf("2:m_szPrompt0=%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
	//				m_szPrompt0[0],m_szPrompt0[1],m_szPrompt0[2],m_szPrompt0[3],m_szPrompt0[4],
	//				m_szPrompt0[5],m_szPrompt0[6],m_szPrompt0[7],m_szPrompt0[8],m_szPrompt0[9],
	//				m_szPrompt0[10],m_szPrompt0[11],m_szPrompt0[12],m_szPrompt0[13],m_szPrompt0[14],
	//				m_szPrompt0[15],m_szPrompt0[16],m_szPrompt0[17],m_szPrompt0[18],m_szPrompt0[19],
	//				m_szPrompt0[20],m_szPrompt0[21],m_szPrompt0[22],m_szPrompt0[23],m_szPrompt0[24],
	//				m_szPrompt0[25],m_szPrompt0[26],m_szPrompt0[27],m_szPrompt0[28],m_szPrompt0[29],
	//				m_szPrompt0[30],m_szPrompt0[31],m_szPrompt0[32],m_szPrompt0[33],m_szPrompt0[34],
	//				m_szPrompt0[35],m_szPrompt0[36],m_szPrompt0[37],m_szPrompt0[38],m_szPrompt0[39],
	//				m_szPrompt0[40],m_szPrompt0[41],m_szPrompt0[42],m_szPrompt0[43],m_szPrompt0[44],
	//				m_szPrompt0[45],m_szPrompt0[46],m_szPrompt0[47],m_szPrompt0[48],m_szPrompt0[49]);
			}
	    }
	    else
	    {	
	        if (m_pszPrompt != NULL)
	        {
	        	if ((strlen(m_pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
	    		strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL)
	    		{
	        		if (m_lMin != 0)
	           			sprintf(m_szPrompt0, "%s %s:%s %s:%s", m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
	        		else
	           			sprintf(m_szPrompt0, "%s %s:%s", m_pszPrompt, g_MultiLanguage["MSG_MAX"], szMax);
	           	}
	        }
	    	else sprintf(m_szPrompt0, "%s:%s %s:%s"   , g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
	    	
	    	//	fans add 2006/12/30 11:59上午
			if (m_lMin == 0xFFFF && m_lMax == 0xFFFF) 
			{
				if (m_pszPrompt != NULL) strncpy(m_szPrompt0, m_pszPrompt, tmMAX_TEXT_COL);
				else m_szPrompt0[0]='\0';
			}
			else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF) 
			{
				sprintf(m_szPrompt0, "%s %s:%s",m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin);
			}
			else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
		
	    		sprintf(m_szPrompt0, "%s %s:%s",m_pszPrompt, g_MultiLanguage["MSG_MAX"], szMax);
	    }	
	}
	
	//	fans add
	if (m_wType & tmEDIT_TIMEDIVTEN)            //Steven 2004/3/5 for Time div 10
	    F2A(m_sz, m_nPrecision, m_lValue/10);
	else
	    F2A(m_sz, m_nPrecision, m_lValue);
	
}
/*===========================================================================+
|           Class implementation - EditBox                                   |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/


//	fans add
CtmEditX2::CtmEditX2(): CtmEdit()
{
	strcpy(TYPEName, "CtmEditX2");

	m_lOldValue = 0;
	m_lValue = 0;
	//Sunny<20070823> add	<<<<<<
	m_lMin = 0;
	m_lMax = 0;
	m_nPrecision = 0;
	//Sunny<20070823> add	>>>>>>	
	for (int i = 0; i < MAXDBNUM; i++)	m_pszDBID[i] = NULL;

	//m_wType = tmEDIT_HEXNOALPHA;
}

BOOL 	CtmEditX2::CreateA()
{
	CtmEdit::CreateA();
		

	if (m_pszDBID[0] != NULL)
	{
		DBVIEWVALUE dbViewValue;
		//char pszTemp[256];
		dbViewValue = GetDBValue(m_pszDBID[0]);
		m_lValue = dbViewValue.lValue; 
		m_lOldValue = m_lValue;
	}

	
	return TRUE;
}

CtmEditX2::~CtmEditX2()
{
	
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
int		CtmEditX2::OnLose()
{
	CtmEdit::OnLoseFocus();
	m_bPropertyChanged = TRUE;
	m_lValue = m_lOldValue;
	Update();
	return 0;
}


void        CtmEditX2::Get()              /* Virtual function */
{
	CtmEdit::Get();

	sscanf(m_sz, "%X", &m_lValue);
}

void        CtmEditX2::Set()              /* Virtual function */
{
	
	char        szFormat[5];
	
	sprintf(szFormat, "%%0%dX", m_wMaxChar);
	sprintf(m_sz, szFormat, m_lValue);
	CtmEdit::Set();
	
}


WORD        CtmEditX2::OnKey(WORD wKey)   /* Virtual function */
{

	DWORD        lValue=0;	
	long long   editx2Max = m_lMax;			
	//=======
	//      Interpret function key from the keyboard
	//=======
	wKey = CtmEdit::OnKey(wKey);              // Key code process status
	switch (wKey)
	{
    case _KEY_LEFTFIELD:
    case _KEY_RIGHTFIELD:
    case _KEY_UPFIELD:
    case _KEY_DOWNFIELD:
    case _UARROW:
    case _EXT_UARROW:
    case _DARROW:
    case _EXT_DARROW:
    case _TAB:
    case _SHIFT_TAB:
    case _ENTER:
	case (WORD)_EXT_ENTER:		
		m_bDelete	  = FALSE;
        //CtmEdit::Get();
       //Sunny<20070909> amend
        //sscanf(m_sz, "%04X", &lValue);
	    if (m_wType & tmEDIT_HEXADECIMAL)   sscanf(m_sz, "%X", &lValue);
	    else                                sscanf(m_sz, "%d", &lValue);
       
			m_lValue = lValue;
                   //	Sunny<20070908> add<<<<<
        if (m_lMin == 0xFFFF && m_lMax == 0xFFFF)
        {
	        //James add 2008/4/29 01:27下午 當輸入的值超過0xFFFF不能存
        	if  (lValue > editx2Max)
        	{
				m_lValue = lValue;
	            CtmEdit::ResetPosition();
	            return _NULL_KEY;
			}
			else
			{
	        	break;
	        }
		}
		else if (m_lMin == 0xFFFF &&  m_lMax != 0xFFFF)
		{
			 if  (lValue > editx2Max)
			{
				m_lValue = lValue;
	            CtmEdit::ResetPosition();
	            return _NULL_KEY;
			}
		}
		else if (m_lMin != 0xFFFF &&  m_lMax == 0xFFFF)
		{
			 if  (lValue < m_lMin || lValue > editx2Max)
			{
				m_lValue = lValue;
	            CtmEdit::ResetPosition();
	            return _NULL_KEY;
			}
		}
		else
		{
			if ((lValue < m_lMin) ||  (lValue > editx2Max))  
			{                                                  
				m_lValue = lValue;                              
			    CtmEdit::ResetPosition();                      
			    return _NULL_KEY;                              
			}        
		}
            //CtmEdit::ResetPosition();
        //	Sunny<20070908> add>>>>>>
		      
			break;
		default:
			break;
	}

	return wKey;
}

void    CtmEditX2::Show()			//	fans add
{

	char        szFormat[15];
	if(!m_bLoseFocus)
	{
		sprintf(szFormat, "%%0%dX", m_wMaxChar);
		sprintf(m_sz, szFormat, m_lValue);
	}
	else
	{
		m_bLoseFocus=FALSE;	
	}
	CtmEdit::Get();		// remove by Wong	2014/11/1 10:37:37 
	ResetPosition();		//recalculated show position	Caozz add 2012/10/12 下午 04:58:17
	CtmEdit::Show();
	m_bPropertyChanged = FALSE;
	
}

void 	CtmEditX2::SetPropertys()
{
	CtmEdit::SetPropertys();

	SetPropInfo("value", 		tmINT64, 	&m_lValue);
	//Sunny<20070823> add	<<<<<<	
	SetPropInfo("min", 			tmINT64, 	&m_lMin);
	SetPropInfo("max", 			tmINT64, 	&m_lMax);
	//Sunny<20070823> add	>>>>>>
	char sz[10];
	for (int i = 0; i < MAXDBNUM; i++)
	{
		sprintf(sz, "%s%d", "dbid", i);
		SetPropInfo(sz,		tmSTR, 		&m_pszDBID[i]);	
	}
	
}
void 	CtmEditX2::SetPropertys_Nub()
{
	CtmEdit::SetPropertys_Nub();

	SetPropInfo(EDITX2_VALUE, 			tmINT64, 	&m_lValue);
	//Sunny<20070823> add	<<<<<<	
	SetPropInfo(EDITX2_MIN, 			tmINT64, 	&m_lMin);
	SetPropInfo(EDITX2_MAX, 			tmINT64, 	&m_lMax);
	//Sunny<20070823> add	>>>>>>
	
	int	nIndex = EDITX2_DBID0;
	for (int i = 0; i < MAXDBNUM; i++)
	{
		SetPropInfo(nIndex+i,		tmSTR, 		&m_pszDBID[i]);	
	}
}


void 	CtmEditX2::Update()
{
		//int	i = 0;
		//if (i > 1000) cout << Name << " update" << endl;
		//i++;
		//
	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)
	{
			/*if (m_iPrivilege > g_iPrivilege) 
			{
				DrawBorder();
				return;
			}*/
			if (!m_bOnFocus)
		    {
		    	//Set(); remove by Wong 2014/11/1 12:28:28
		    	if (m_pszDBID[0] != NULL)
		    	{
			    	DBVIEWVALUE dbViewValue;
			    	dbViewValue = GetDBValue(m_pszDBID[0]);
			    	m_iPrivilege = dbViewValue.dwPrivilege;
			    	m_lValue = dbViewValue.lValue;
			    }
			//    
			//    //sscanf(m_sz, "%X", &m_lValue);
			//    
			//  
		    	if ((m_lValue != m_lOldValue) || m_bPropertyChanged)
				{
					//if (m_bPropertyChanged) printf("%s property changed!!\n", Name);
					Show();
					m_lOldValue = m_lValue;
				}
		    //	
			//		
		    }
		    //else	//	fans add 2008/9/10 03:26下午
		    //{
			//    if (m_bPropertyChanged)
			//	{
			//		CtmEdit::Get();
			//		//printf("pszText=%s, Precision=%d, value=%lld\n", m_pszText, m_nPrecision, m_lValue);
			//		sscanf(m_pszText, "%X", &m_lValue);
			//		CtmEdit::Show();
			//	}
			//}
		    m_bPropertyChanged = FALSE;
	}
}

//	2006/12/29 10:01上午
void    CtmEditX2::OnLoseFocus()
{
	
	int iTemp = 0;
	//=============
	//	檢測有幾個對應的資料
	//=============	
	for (int i = 0; i < MAXDBNUM; i++)
	{
		if (m_pszDBID[i] != NULL) iTemp++;
	}
	
	//=============
	//	只有一個的話,將會自動保存;因為如果有多個的話,有可能會有各種計算,將不會有自動的操作
	//=============	
	if ((iTemp == 1) && (m_pszDBID[0] != NULL)&&!g_bFocusChanged) //&& !g_bFocusChanged add by Wong 2014/11/3 15:24:37
	{
		if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)       //Sunny<20070908> add 增加最大/最小值限制資料<<<<<
		{
			if ((m_lValue >= m_lMin) && (m_lValue <= m_lMax))
			{
				if (m_lOldValue != m_lValue)
				{
		            SetViewDBValue(m_pszDBID[0], m_lValue);
				}
			}
		}
		else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
		{
			//James add && (m_lValue <= m_lMax) 2008/4/29 01:08下午
			if ((m_lValue >= m_lMin) && (m_lValue <= m_lMax))
			{
				if (m_lOldValue != m_lValue)
				{
		            SetViewDBValue(m_pszDBID[0], m_lValue);
				}
			}
		}
		else if(m_lMin == 0xFFFF && m_lMax != 0xFFFF)
		{
			if (m_lOldValue != m_lValue )
			{
		        SetViewDBValue(m_pszDBID[0], m_lValue);
			}
		}                                        //Sunny<20070908> add >>>>>>    
	}
	CtmEdit::OnLoseFocus();
	m_lOldValue = m_lValue;
	

}

void        CtmEditX2::OnGetFocus()         /* Virtual function */
{

	CtmEdit::OnGetFocus1();
	CtmWnd::DrawGetFocus();
	//Sunny<20070823> add	<<<<<<
	Prepare();
	Prompt(m_szPrompt0);
	//Sunny<20070823> add	>>>>>>
}

//Sunny<20070823> add	<<<<<<
void	CtmEditX2::Prepare()
{
	char        szMin[256];    //netdigger 2007/4/12
	char        szMax[256];    //netdigger 2007/4/12
	
	long long	editx2Max = m_lMax;      //wy080800
	int         nPosition;
	WORD    *pwSrc, *pwDst;
	long 		lDataID = 0;
	
	if (m_wType & tmEDIT_HEXADECIMAL)
	{
		sprintf(szMin, "%X", m_lMin);
		sprintf(szMax, "%X", editx2Max);
		A2A(szMin, 0, m_pcFont->GetFontCode());
		A2A(szMax, 0, m_pcFont->GetFontCode());
	}
	else
	{
		LL2A(szMin, 0, m_lMin, m_pcFont->GetFontCode());    //netdigger 2007/4/12
		LL2A(szMax, 0, editx2Max, m_pcFont->GetFontCode());    //netdigger 2007/4/12	
	}
    
	if(g_bDemoDataID)
	{
		if(m_pszDBID[0] != NULL)
		{
			lDataID  = g_pDatabase->GetDataID(m_pszDBID[0]);
		}
	}
	if(CtmConfig::GetInstance()->GetFontConvert())
	{
		if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
	    {
			if(g_bDemoDataID)
			{
				memset(m_szPrompt0, 0, tmMAX_TEXT_COL); 
				
				if (m_lMin == 0xFFFF &&  m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t", lDataID, m_pszPrompt);
				else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t: %t %t ", lDataID,  g_MultiLanguage["MSG_MAX"], szMax, m_pszPrompt);
				else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t: %t %t", lDataID, g_MultiLanguage["MSG_MIN"], szMin,m_pszPrompt);
				else if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t: %t %t: %t %t", lDataID, 
	        				g_MultiLanguage["MSG_MIN"], szMin,
	        				g_MultiLanguage["MSG_MAX"], szMax,m_pszPrompt);        
			}
			else
			{
				memset(m_szPrompt0, 0, tmMAX_TEXT_COL); 
	    	
				if (m_lMin ==0xFFFF &&  m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t", m_pszPrompt);
				else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t: %t %t", g_MultiLanguage["MSG_MAX"], szMax, m_pszPrompt);
				else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t: %t %t", g_MultiLanguage["MSG_MIN"], szMin,m_pszPrompt);
				else if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t: %t %t: %t %t", 
								g_MultiLanguage["MSG_MIN"], szMin,
								g_MultiLanguage["MSG_MAX"], szMax,m_pszPrompt);
			}
	    }
	    else
	    {	
	        if (m_pszPrompt != NULL)
	        {
	        	if ((strlen(m_pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
	    		strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL)
	    		{
	        		if (m_lMin != 0)
	           			sprintf(m_szPrompt0, "%s:%s %s:%s %s",g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax, m_pszPrompt);
	        		else
	           			sprintf(m_szPrompt0, "%s:%s %s",g_MultiLanguage["MSG_MAX"], szMax, m_pszPrompt);
	           	}
	        }
	    	else sprintf(m_szPrompt0, "%s:%s %s:%s"   , g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
	    	
	    	//	fans add 2006/12/30 11:59上午
	  	 	if (m_lMin == 0xFFFF && m_lMax == 0xFFFF) 
	  	 	{
	  	 		if (m_pszPrompt != NULL) strncpy(m_szPrompt0, m_pszPrompt, tmMAX_TEXT_COL);
	  	 		else m_szPrompt0[0]='\0';
	  	 	}
	  	 	else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF) 
	  	 	{
	  	 		sprintf(m_szPrompt0, "%s:%s %s",g_MultiLanguage["MSG_MIN"], szMin,m_pszPrompt);
	  	 	}
	  	 	else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
	    	{
	    		sprintf(m_szPrompt0, "%s:%s %s", g_MultiLanguage["MSG_MAX"], szMax,m_pszPrompt);
	    	}
	    }	
	}
	else
	{
	    if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
	    {
			if(g_bDemoDataID)
			{
				memset(m_szPrompt0, 0, tmMAX_TEXT_COL); 
				
				if (m_lMin == 0xFFFF &&  m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t", lDataID, m_pszPrompt);
				else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t %t: %t", lDataID, m_pszPrompt, g_MultiLanguage["MSG_MAX"], szMax);
				else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t %t: %t", lDataID, m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin);
				else if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%x %t %t: %t %t: %t", lDataID, 
	        				m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
	        				g_MultiLanguage["MSG_MAX"], szMax);        
			}
			else
			{
				memset(m_szPrompt0, 0, tmMAX_TEXT_COL); 
	    	
				if (m_lMin ==0xFFFF &&  m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t", m_pszPrompt);
				else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t %t: %t", m_pszPrompt, g_MultiLanguage["MSG_MAX"], szMax);
				else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t %t: %t", m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin);
				else if (m_lMin != 0xFFFF && m_lMax != 0xFFFF)
					tm_sprintf(m_szPrompt0, "%t %t: %t %t: %t", 
								m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
								g_MultiLanguage["MSG_MAX"], szMax);
			}
	    }
	    else
	    {	
	        if (m_pszPrompt != NULL)
	        {
	        	if ((strlen(m_pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
	    		strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL)
	    		{
	        		if (m_lMin != 0)
	           			sprintf(m_szPrompt0, "%s %s:%s %s:%s", m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
	        		else
	           			sprintf(m_szPrompt0, "%s %s:%s", m_pszPrompt, g_MultiLanguage["MSG_MAX"], szMax);
	           	}
	        }
	    	else sprintf(m_szPrompt0, "%s:%s %s:%s"   , g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
	    	
	    	//	fans add 2006/12/30 11:59上午
	  	 	if (m_lMin == 0xFFFF && m_lMax == 0xFFFF) 
	  	 	{
	  	 		if (m_pszPrompt != NULL) strncpy(m_szPrompt0, m_pszPrompt, tmMAX_TEXT_COL);
	  	 		else m_szPrompt0[0]='\0';
	  	 	}
	  	 	else if (m_lMin != 0xFFFF && m_lMax == 0xFFFF) 
	  	 	{
	  	 		sprintf(m_szPrompt0, "%s %s:%s",m_pszPrompt, g_MultiLanguage["MSG_MIN"], szMin);
	  	 	}
	  	 	else if (m_lMin == 0xFFFF && m_lMax != 0xFFFF)
	    	{
	    		sprintf(m_szPrompt0, "%s %s:%s",m_pszPrompt, g_MultiLanguage["MSG_MAX"], szMax);
	    	}
	    }
	}

	
}
//Sunny<20070823> add	>>>>>>
