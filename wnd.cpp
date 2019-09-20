/* Copyright (C), 1995-2000, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows                                                        |
|  Task     : Windows Manager                                                |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 03/10/1995                                                     |
|  Revision : 03/23/1996                                                     |
+===========================================================================*/
#include    "wnd.h"
#include	"init.h"
#include	"tmcontrol.h"

#include	"commonaction.h" 
#include	"font.h"
//#include	"memwatch.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           External                                                         |
+===========================================================================*/
extern      CtmFont*         	g_pDotFont16;			// init.cpp
extern 		CtmFont*			g_pDotFont24;			// init.cpp
extern      tmWNDATTR           g_wndattrDefault;       // Defined in "ddefault.cpp"
//extern      HWND                g_hWndMain;
IMPLEMENT_DYNAMIC(CtmWnd, CObject)

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//=======
//      Initialize static class member
//=======
WORD            CtmWnd::m_wWindowsCount           = 0;
CtmWnd*         CtmWnd::m_pwndList[tmMAX_WINDOWS+1] = {NULL};
CtmWnd*         CtmWnd::m_pwndCurrent               = NULL;
//char            m_skinfilepath[32] = "./res/defaultskin";
tmWNDATTR       CtmWnd::m_wndattr                 = g_wndattrDefault;    // defined in ddefault.cpp
tmPARAGRAPH     CtmWnd::m_paragraphLeft           = {tmPARA_ALIGN_LEFT};
LISTCOORDINATE	g_MemoCurCoordinate;
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - Wnd                                       |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CtmWnd::CtmWnd()
{
	 //	fans add
	 //cout << "CREATED!!!" << m_wWindowsCount << endl;
	strcpy(TYPEName, "CtmWnd");	
	m_plibHandle 		= NULL;
	m_taborder 			= -2	;	
        PropCount 			= 0;
	m_controlCount 		= 0;
	m_parent 			= NULL;	
	m_pszTitleID 		= NULL;
	m_wnd.pszTitle 		= NULL;
	m_wnd.wStyle 		= 0;
	m_iFontType 		= 0;
	m_wnd.rect.left 	= 0;
	m_wnd.rect.top 		= 0;
	m_wnd.rect.right 	= 100;
	m_wnd.rect.bottom 	= 20;


	m_iTitleWidth = tmTITLE_WIDTH;
	m_iTitleHeight = tmTITLE_HEIGHT;
	m_iStatusWidth = tmSTATE_WIDTH;
	m_iMainWidth = tmWIDTH;
	m_bDrawFocus =TRUE;
		
	for(int i = 0; i < MAX_CONTROLS; i++) Controls[i] = NULL;	
	InitProps();	
	//SetBaseInfo();
	
    //if (m_wWindowsCount >= tmMAX_WINDOWS)
    //{
    //    m_hwndMe = NULL_HWND;
    //    return;
    //}
    //m_wWindowsCount++;
    //m_hwndMe = 1;                           // Find handle of myself
    //while (m_pwndList[m_hwndMe] != NULL) m_hwndMe++;
    //m_pwndList[m_hwndMe] = this;            // Save this CtmWnd pointer in global list
    SetFGColor(m_wndattr.fgc);             // Initialize the foreground color
    SetBGColor(m_wndattr.bgc);              // Initialize the background color m_wndattr.bgc
    //SetFont(g_pDotFont16);       			// Initialize the font parameter
    SetParagraph(m_wndattr.paragraph);      // Initialize the paragraph parameter
    SetBorderWidth(m_wndattr.wBorderWidth);
    m_bOnFocus = FALSE;
    m_bChanged = TRUE;                     //St120100 add for get change status
    m_pImage = NULL;						//<<yang 2005/11/23 
    
    m_pszLeftControl 	= NULL;
	m_pszRightControl 	= NULL;
	m_pszUpControl 		= NULL;
	m_pszDownControl 	= NULL;

    m_iExTag 			= -1;
    m_iPrivilege 		= 0;	//	默認的權限是0，表示是最低的
    m_bVisible 			= TRUE;	//	默認的元件總是可用的
    m_iMaxTab 			= -2;
    m_bEnabled	 		= TRUE;
   	m_bPropertyChanged 	= FALSE;
   	m_nBgcColorControl	= 0;
   	m_nColorChange		= -1;
   	m_wDestColor		= 0xFFFF;
   	m_wBorderColor		= 55003;
   	m_bArc				= FALSE;
   	m_bPreControl		= FALSE;
   	
   	m_iFontUNType		= 0;
   	m_iFontUNWidth		= 16;
   	m_iFontUNHeight		= 18;
   	
   	//m_pAi				= NULL;	
   	//m_nAiType			= 0;
	//m_szAiWnd			= NULL;
	//m_szAiID			= NULL;
	//m_llAiValue 		= -1;	
	//m_llAidValue		= AI_DEFAULT;
	//m_llAivValue		= AI_DEFAULT;
	//	fans add 2009-4-7 13:03:46
	m_nMsgID			= 0;
	m_nLParam			= 0;
	m_nWParam			= 0;
	//memset(m_llAiValues, 0xFF, sizeof(m_llAiValues));
	
	m_pszHelpStrID		= NULL;
	//for (int i = 0; i < MAX_HELP_NUM; i++)
	//	m_pszHelpStrIDs[i] = NULL;
	memset(m_pszHelpStrIDs, 0, sizeof(m_pszHelpStrIDs));
	m_nHelpType 		= 0;
}

CtmWnd::~CtmWnd()
{
    WORD        i;
    WORD        wWindowsCount;
    RECT        rect;
   
	if (m_plibHandle != NULL) 
		dlclose(m_plibHandle);

	if ((m_wnd.wStyle & tmWS_REDRAW) && (m_wnd.wStyle & tmWS_FRAMEWND))	
		{
		//ShowBg();																	//<<yang 2005/11/23
		FreeBg();																	//<<yang 2005/11/23 
		}
	
	//	free the contained controls
	for(i = 0; i< MAX_CONTROLS; i++)
	{
		if (m_bPreControl) 
		{
			if(i>0)
				i--;
			m_bPreControl = FALSE;
			//printf("preControl = %s, i=%d\n", Name, i);
		}
		if (Controls[i] != NULL) 
		{
			//printf("i=%d, Name=%s\n", i, Controls[i]->Name);
			delete Controls[i];
			//Controls[i] = NULL;
			//m_controlCount--;
			
		}
		
		
	}
	
    //	fans add
  	//	free the all properties's Name,
	for(i = 0; i < MAX_PROPERTY; i++)
	{
		if (Propertys[i].pszName[0] == '\0') break;
		if ((Propertys[i].ptype == tmUSTR) 		|| 
			(Propertys[i].ptype == tmPROCEDURE) ||
			(Propertys[i].ptype == tmSTR))
		{
			if (Propertys[i].propAddress != NULL && *(char**)Propertys[i].propAddress != NULL)
			{
				//delete [](*(char**)Propertys[i].propAddress);
				free(*(char**)Propertys[i].propAddress);
				*(char**)Propertys[i].propAddress = NULL;			
			}
		}
		//delete []Propertys[i].pszName;
		//Propertys[i].pszName = NULL;
	}

	if (m_parent != NULL) 
	{
		//int nIndex = m_parent->FindControl(this);
		//m_parent->Controls[nIndex] = NULL;
		//m_parent->m_controlCount--;
		
		m_parent->m_bPreControl = TRUE;
		SetParent(NULL);
	}
	
	//	fans add 2009-4-3 11:36:26
	//delete m_pAi;
	
	
	
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function : BOOL Create(WND* pwnd)                                         |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     : bStatus = CtmWnd::Create(pwnd)                                 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL        CtmWnd::Create(tmWND* pwnd)
{    
    return TRUE;
}


/*---------------------------------------------------------------------------+
|  Function : BOOL CreateA(WND* pwnd)                                         |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :								                                 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL		CtmWnd::CreateA()
{
	//if(CtmConfig::GetInstance()->GetLanguageType()==1)
   	//	m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
	//else
	//	m_iFontUNType		= 0;
	//clock_t start, finish;
	//double  duration;
	//start = clock(); 
	//	fans add 2009-4-3 11:35:27
	//if (m_nAiType != 0)
	//{
	//	if(m_pAi != NULL) delete m_pAi;
	//	m_pAi = new CtmAiOperation(this);
	//	m_pAi->GetWndInfo();
	//	m_pAi->ExeAiOp();
	//}
	
	
	
    char* psz = NULL;
    if (m_pszTitleID != NULL)
    {
    	psz = g_MultiLanguage[m_pszTitleID];
    	
    	if (psz != NULL)
    	{
	    	if (m_wnd.pszTitle != NULL)
	    	{
	    		if (strcmp(psz, m_wnd.pszTitle) != 0)
	    		{
	    			if (m_wnd.pszTitle != NULL) 
	    			{
	    				free(m_wnd.pszTitle); //delete []m_wnd.pszTitle; 
	    				   m_wnd.pszTitle=NULL;
	    			} 	
	    			m_wnd.pszTitle =(char*)malloc(strlen(psz) + 1); //new char[strlen(psz) + 1];
	    			strcpy(m_wnd.pszTitle, psz);    	
	    		}
	    	}
	    	else
	    	{
	    		m_wnd.pszTitle =(char*) malloc(strlen(psz) + 1);//new char[strlen(psz) + 1];
	    		strcpy(m_wnd.pszTitle, psz);    
	    	}
	    }
    }
    
	int	nShowFontSize = 0;
    //James show fontsize 2011/8/2
	nShowFontSize = CtmConfig::GetInstance()->GetShowFontSize();
	//printf("nShowFontSize=%d \n", nShowFontSize);
	switch(nShowFontSize)
	{
		case 0:
			break;
		case 16:
			m_iFontType = 0;
			break;
		case 20:
			m_iFontType = 2;
			break;
		case 24:
			m_iFontType = 1;
			break;
		default:
			break;	
	}
	
	if(m_iFontType == 1)
	{
		m_iFontUNWidth = 20;
		m_iFontUNHeight = 24;
	}
	else if(m_iFontType == 2)
	{
		m_iFontUNWidth = 18;
		m_iFontUNHeight = 20;
	}
    // Save basic parameter
    m_pcFont = g_pFont->GetFont(m_iFontType);
    //if(m_iFontUNType > 0)
    //	m_pcFont->SetUNFontType(m_iFontUNType, m_iFontUNWidth, m_iFontUNWidth);
    
	//if (m_iFontType  == 0) 
	//{
	//	m_pcFont	= g_pFont16;
	//}
	//else 
	//{
	//	m_pcFont	= g_pFont24;
	//}
	//if (m_bgc == 0) m_bgc = RGB2PIXEL(236,233,216);
	
    if ((m_wnd.wStyle & tmWS_BORDER) == tmWS_BORDER_ZERO)       SetBorderWidth(0);
    else if ((m_wnd.wStyle & tmWS_BORDER) == tmWS_BORDER_FRAME) SetBorderWidth(2);
    else if ((m_wnd.wStyle & tmWS_BORDER) == tmWS_BORDER_ALLRAISE) SetBorderWidth(2);
    /*
    if ((m_wnd.wStyle & tmWS_BORDER) == tmWS_BORDER_FRAME)
        SetBGColor(RGB2PIXEL(236,233,216));
        */
    
    CalcRECT();                               // Calculate Window's and Client's RECT

	//if ((m_wnd.wStyle & tmWS_REDRAW) && (m_wnd.wStyle & tmWS_FRAMEWND)) 
	//{
	//	CopyBg();						//<<yang 2005/11/23 
	//	Clear();
	//	DrawBorder();
	//}
	
    if (m_wnd.wStyle & tmWS_TRANSPARENT)
    {	
    	
         if (m_parent != NULL)
            m_bgc = m_parent->GetBGColor();
         
    }
    else
    {
        //Clear();
    }
    //DrawBorder();

	int i; 
	for(i = 0; i < GetControlCount(); i++) 
	{
		
		Controls[i]->CreateA(); 
		
	}
	
	m_iMaxTab = FindMaxTabOrder();
	return TRUE;
	
}

/*---------------------------------------------------------------------------+
|  Function :               SetBorder                                        |
|  Task     : Set the frame border                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/

void        CtmWnd::SetBorder()
{
/*	RECT	rect;
	
	m_pBmpBorder = new CtmBitMap;
	MKRECT(rect, 0, 0, 799, 67)
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	m_pBmpBorder->Create(tmBMP_TITLE);
	m_pBmpBorder->Show(rect, 1);
	
	MKRECT(rect, 0, 0, , )
	_SetViewPort(, , , , );
	m_wndBorder.Create(tmBMP_RIGHT);
	m_wndBorder.Show(rect, 1);
	
	MKRECT(rect, 0, 0, , )
	_SetViewPort(, , , , );
	m_wndBorder.Create(tmBMP_BOTTOM);
	m_wndBorder.Show(rect, 1);
	
	MKRECT(rect, 0, 0, , )
	_SetViewPort(, , , , );
	m_wndBorder.Create(tmBMP_TITLE);
	m_wndBorder.Show(rect, 1);
	
	MKRECT(rect, 0, 0, , )
	_SetViewPort(rect, , , , );
	m_wndBorder.Create();
	m_wndBorder.Show(rect, 1);
	
	delete	m_pBmpBorder;*/
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmWnd::CalcRECT()
{
    RECT        rect;
	
	/*
    if (m_wnd.hwndParent != NULL_HWND)
    {
        rect = m_pwndList[m_wnd.hwndParent]->GetClientRECT(); 
        m_rectWindow.left   = m_wnd.rect.left   + rect.left;
        m_rectWindow.top    = m_wnd.rect.top    + rect.top;
        m_rectWindow.right  = m_wnd.rect.right  + rect.left;
        m_rectWindow.bottom = m_wnd.rect.bottom + rect.top; 
    }
    else
    {
        m_rectWindow = m_wnd.rect;
    }
    */
	if (m_parent != NULL)
    {
        rect = m_parent->GetClientRECT(); 
        m_rectWindow.left   = m_wnd.rect.left   + rect.left;
        m_rectWindow.top    = m_wnd.rect.top    + rect.top;
        m_rectWindow.right  = m_wnd.rect.right  + rect.left;
        m_rectWindow.bottom = m_wnd.rect.bottom + rect.top; 

	}
    else
    {
   		//cout << "Parent is NULL" <<  endl;       
        m_rectWindow = m_wnd.rect;   		
    }
    m_rectClient.left   = m_rectWindow.left   + m_wBorderWidth;
    m_rectClient.top    = m_rectWindow.top    + m_wBorderWidth;
    m_rectClient.right  = m_rectWindow.right  - m_wBorderWidth;
    m_rectClient.bottom = m_rectWindow.bottom - m_wBorderWidth;
    
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmWnd::SetFontFormat(WORD	m_wFormat)
{
	if(m_pcFont != NULL)
		m_pcFont->SetFormat(m_wFormat);
}


void	CtmWnd::ScreenToClient(RECT* rect)
{
    rect->left   -= m_rectClient.left;      // Change screen coordinate to view port coordinate
    rect->top    -= m_rectClient.top;
    rect->right  -= m_rectClient.left;
    rect->bottom -= m_rectClient.top;
}

void        CtmWnd::SetClientRECT(short nLDelta, short nTDelta, short nRDelta, short nBDelta)
{
    m_rectClient.left   += nLDelta;
    m_rectClient.top    += nTDelta;
    m_rectClient.right  += nRDelta;
    m_rectClient.bottom += nBDelta;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmWnd::DrawBorder()
{
	short       i;
	short       nRight;
	short       nBottom;
	
	_SetViewPort(	m_rectWindow.left, 
					m_rectWindow.top, 
					m_rectWindow.right, 
					m_rectWindow.bottom
				);
	nRight  = m_rectWindow.right  - m_rectWindow.left;
	nBottom = m_rectWindow.bottom - m_rectWindow.top;
	
	switch (m_wnd.wStyle & tmWS_BORDER)
    {
    case tmWS_BORDER_PLAIN:
        //_SetColor(m_wndattr.fgcBorder4);
        _SetColor(m_wBorderColor);
        if (m_bArc)
        	_ArcRect(0, 0, 0, nRight, nBottom);
        else
        	_Rectangle(_GBORDER, 0, 0, nRight, nBottom);

        _SetColor(m_bgc);
        for (i=1; (WORD)i<(m_wBorderWidth); i++)
        {
        	if (m_bArc)
        		_ArcRect(0, i, i, nRight-i, nBottom-i);
        	else
            	_Rectangle(_GBORDER, i, i, nRight-i, nBottom-i);
        }
        break;
    case tmWS_BORDER_PLAIN2:
        _SetColor(m_wndattr.fgcBorder1);
        _Rectangle(_GBORDER, 0, 0, nRight, nBottom);

        _SetColor(m_bgc);
        for (i=1; (WORD)i<(m_wBorderWidth); i++)
            _Rectangle(_GBORDER, i, i, nRight-i, nBottom-i);
        break;
    case tmWS_BORDER_RAISE:
        _SetColor(m_wndattr.fgcBorder3);
        _MoveTo(     0, nBottom);
        _LineTo(     0, 0);
        _LineTo(nRight, 0);
        _SetColor(m_wndattr.fgcBorder4);
        _LineTo(nRight, nBottom);
        _LineTo(     1, nBottom);

        _SetColor(m_bgc);
        for (i=1; (WORD)i<(m_wBorderWidth); i++)
            _Rectangle(_GBORDER, i, i, nRight-i, nBottom-i);
        break;
    case tmWS_BORDER_INSET:
        _SetColor(m_wndattr.fgcBorder4);
        _MoveTo(     0, nBottom);
        _LineTo(     0, 0);
        _LineTo(nRight, 0);
        _SetColor(m_wndattr.fgcBorder3);
        _LineTo(nRight, nBottom);
        _LineTo(     1, nBottom);

        _SetColor(m_bgc);
        for (i=1; (WORD)i<(m_wBorderWidth); i++)
            _Rectangle(_GBORDER, i, i, nRight-i, nBottom-i);
        break;
    case tmWS_BORDER_FRAME:
        _SetColor(m_wndattr.fgcBorder3);
        _Rectangle(_GBORDER, 1, 1, nRight, nBottom);
        _SetColor(m_wndattr.fgcBorder4);
        _MoveTo(       0,  nBottom);
        _LineTo(       0,        0);
        _LineTo(  nRight,        0);
        _MoveTo(nRight-1,        2);
        _LineTo(nRight-1,nBottom-1);
        _LineTo(       2,nBottom-1);
        break;
    case tmWS_BORDER_ALLRAISE:
    	_SetColor(m_wndattr.fgcBorder3);
        _Rectangle(_GBORDER, 1, 1, nRight, nBottom);
        _SetColor(m_wndattr.fgcBorder4);
        _MoveTo(       0,  nBottom);
        _LineTo(       0,        0);
        _LineTo(  nRight,        0);
        _MoveTo(nRight-1,        2);
        _LineTo(nRight-1,nBottom-1);
        _LineTo(       2,nBottom-1);
    	break;
    case tmWS_BORDER_CLEARALL:         //ST021100  for disappear Item
        //_SetColor(GetWindow(m_wnd.hwndParent)->GetBGColor());	fans mark
        _SetColor(m_parent->GetBGColor());	
        _Rectangle(_GFILLINTERIOR, 0, 0, nRight, nBottom);
        break;
    case tmWS_BORDER_NONE:
    case tmWS_BORDER_ZERO:                  //BW 1225'02
        break;
    default:
    	
        _SetColor(m_bgc);
        for (i=0; (WORD)i<(m_wBorderWidth); i++)
            _Rectangle(_GBORDER, i, i, nRight-i, nBottom-i);
        
        break;
    }
}

void        CtmWnd::DrawGetFocus()
{
    DrawFocus();
    m_bOnFocus = TRUE;
}

void        CtmWnd::DrawLoseFocus()
{
    DrawFocus();
    m_bOnFocus = FALSE;
}

/*---------------------------------------------------------------------------+
|  Function : Show(short nX, short nY, char* pszString)                      |
|  Task     : Show a string in this window                                   |
+----------------------------------------------------------------------------+
|  Call     : CtmWnd::Show(nX, nY, pszString)                                |
|                                                                            |
|  Parameter: nX                        - X position in client area          |
|             nY                        - Y position in client area          |
|             pszString                 - Pointer to string                  |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmWnd::Show(short nX, short nY, char* pszString)
{
	if (m_pcFont == NULL)
	{
		char	sz[256];
		sprintf(sz, "%s have no font!!!\n", Name);
		perror(sz);
		return;
	}
	
	short       i;
	short       nLength = strlen(pszString);
	WORD        wCode;
	
	short       nXGapHalf = m_pcFont->m_wWidth >> 1;   // Gap of Half font - English
	short       nXGapFull = m_pcFont->m_wWidth;        // Gap of Full font - Chinese
	short       nYGap     = m_pcFont->m_wWidth;
	short       nIndex;
	short       nStringWidth = nXGapHalf * nLength;
	short       nClientWidth = GetClientWidth();

	//=======
	//      Check and adjust the position
	//=======
	if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
	{
	    m_pcFont->SetColor(m_fgc);
	    m_pcFont->ShowString(nX, nY, pszString, 3);
	    return;
	}
	
	if ((nY + nYGap) > GetClientHeight()) return;

	if (nX < 0)
    {
	    nIndex = (nXGapHalf-1-nX) / nXGapHalf;  // nIndex = (abs(nX) + (nXgapHalf - 1)) / nXgapHalf
	    if ((BYTE)*(pszString+nIndex-1) >= tmCHINESE_CODE) nIndex++;
	    nX += nXGapHalf * nIndex;
	    nStringWidth = nXGapHalf * (nLength - nIndex);
    }
	else
    	nIndex = 0;

	if (nStringWidth > (nClientWidth - nX))
    {
	    nLength -= (nStringWidth - (nClientWidth - nX) + nXGapHalf - 1) / nXGapHalf;
	    if ((BYTE)*(pszString+nLength-1) >= tmCHINESE_CODE) nLength--;
    }

	//=======
	//      Show the string using the window's background and foreground color
	//=======
	m_pcFont->SetFontColor(m_fgc);
//	nX += m_rectClient.left;
//	nY += m_rectClient.top;
	
	for (i=nIndex; i<nLength; i++)
    {
		if ((wCode = (BYTE)*(pszString+i)) >= tmCHINESE_CODE)
        {
	        wCode = B2W(pszString+i);
	        i++;
	        m_pcFont->Show(nX, nY, wCode);
	        nX += nXGapFull;
        }
    	else
        {
	        m_pcFont->Show(nX, nY+1, wCode);
	        nX += nXGapHalf;
        }
    }
}

/*---------------------------------------------------------------------------+
|  Function :               Show(short nY, char* pszString)                  |
|  Task     : 				                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmWnd::Show(short nY, char* pszString)
{
	if (m_pcFont == NULL)
	{
		char	sz[256];
		sprintf(sz, "%s have no font!!!\n", Name);
		perror(sz);
		return;
	}
	
	
	short       nX = 0;
	short       nXGapHalf = m_pcFont->m_wWidth >> 1;   // Gap of Half font - English
	short       nStringWidth = nXGapHalf * strlen(pszString);
	
	//	fans modify 2009-3-24 18:12:48
	if (m_paragraph.wAlign & tmPARA_ALIGN_CENTER)
	    nX = (GetClientWidth() - nStringWidth) >> 1;
	else if (m_paragraph.wAlign & tmPARA_ALIGN_RIGHT)
	    nX = GetClientWidth() - nStringWidth - m_wBorderWidth;
	else if (m_paragraph.wAlign & tmPARA_ALIGN_LEFT)
	    nX = 0;
	else
		;
	
	Show(nX, nY, pszString);
}


BOOL	CtmWnd::HasOnlineHelp()
{
	return (GetTabOrder() >= 0 && m_bEnabled && m_pszHelpStrIDs[0][0] != '\0');
}

void	CtmWnd::DrawOnlineHelp(BOOL bShow)
{
	int i;
	int nRight  = m_rectWindow.right  - m_rectWindow.left;
	int nBottom = m_rectWindow.bottom - m_rectWindow.top;
	
	if (HasOnlineHelp() && m_nHelpType == 0)
	{
		if (!bShow)
		{
			//printf("UnDraw OnlineHelp\n");
			//_SetColor(BRIGHTWHITE);
			_SetColor(m_bgc);
		}
		else
		{
			_SetColor(LIGHTRED);
		}	
		
		_SetViewPort(m_rectWindow.left, 
					m_rectWindow.top, 
					m_rectWindow.right, 
					m_rectWindow.bottom
		);
	
		
		for (i = 5; i >= 1; i--)
		{
    		;//_MoveTo(nRight - i, 1);
    		//_LineTo(nRight-1, i);
    	} 
	}
	
	
}

void		CtmWnd::SetHelpStrID(char* pszHelpIDs, int nIndex)
{
	if(nIndex < MAX_HELP_NUM)
		strcpy(m_pszHelpStrIDs[nIndex], pszHelpIDs);
}
char*		CtmWnd::GetHelpStrID(int nIndex)
{
	if(nIndex < MAX_HELP_NUM)
		return m_pszHelpStrIDs[nIndex];
}

short	CtmWnd::ShowExtent(short nX, short nY, unsigned int* pdwUnicode,int nLen)
{
	if(pdwUnicode==NULL)	return 0;
	if (m_pcFont == NULL)
	{
		char	sz[256];
		sprintf(sz, "%s have no font!!!\n", Name);
		perror(sz);
		return 0;
	}
 	if(m_iFontUNType > 0)
	{
		_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	    m_pcFont->SetColor(m_fgc);
	    m_pcFont->SetBGColor(m_bgc);			//James add for FreeType 2015/4/27 13:42:28
		if(strcmp(this->TYPEName, "CtmToolButton")==0)
		{
			//m_paragraph.wAlign&=0xF0;
	    	//m_paragraph.wAlign|=0x08;
	    	m_pcFont->SetUNFontType(m_iFontUNType, m_iFontUNWidth, m_iFontUNHeight,m_paragraph.wAlign);
	    }
	    else
	    {
	    	m_pcFont->SetUNFontType(m_iFontUNType, m_iFontUNWidth, m_iFontUNHeight,m_paragraph.wAlign);	
	    }
	    return m_pcFont->ShowString(nX, nY, pdwUnicode,nLen);	
	}
}

short       CtmWnd::ShowExtent(short nX, short nY, char* pszString, int nType)
{
	if(pszString==NULL)	return 0;
	if (m_pcFont == NULL)
	{
		char	sz[256];
		sprintf(sz, "%s have no font!!!\n", Name);
		perror(sz);
		return 0;
	}

	short       i;
	short       nLength = strlen(pszString);
	WORD        wCode;
	BOOL        bUnderLine = FALSE;
	short       nXGap;
	short       nXGapHalf = m_pcFont->m_wWidth >> 1;   // Gap of Half font - English
	short       nXGapFull = m_pcFont->m_wWidth;        // Gap of Full font - Chinese
	short       nYGap     = m_pcFont->m_wWidth;
	short       nIndex;
	short       nStringWidth;
	short       nRight;
	
	short       nX0;
	short       nStringWidth0 = 0;
    
	//=======
	//      Check and adjust the position
	//=======
	//printf("%s Will Update ", Name);
	if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
	{
	    _SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	    m_pcFont->SetColor(m_fgc);
	    m_pcFont->SetBGColor(m_bgc);			//James add for FreeType 2015/4/27 13:42:28
	    //return 0;
	    //printf("%s m_iFontUNType=%d Update\n", Name, m_iFontUNType);
//	    if(CtmConfig::GetInstance()->GetLanguageType()==1)
//   		m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
//		else
//			m_iFontUNType		= 0;
		
	    if(m_iFontUNType > 0)
	    {
			if(strcmp(this->TYPEName, "CtmToolButton")==0)
			{
				//m_paragraph.wAlign&=0xF0;
	    		//m_paragraph.wAlign|=0x08;
	    		m_pcFont->SetUNFontType(m_iFontUNType, m_iFontUNWidth, m_iFontUNHeight,m_paragraph.wAlign);
	    	}
	    	else
	    	{
	    		m_pcFont->SetUNFontType(m_iFontUNType, m_iFontUNWidth, m_iFontUNHeight,m_paragraph.wAlign);	
	    	}
	    	short out = m_pcFont->ShowString(nX, nY, pszString, nType, m_iFontUNType);
				//ChangePosNow(GetClientRECT()); 
	    	return out;	
	    }
	    else
	    {
	    	//printf("%s m_iFontUNType=%d Update\n", Name, m_iFontUNType);
	    	short out = m_pcFont->ShowString(nX, nY, pszString, nType);
				//ChangePosNow(GetClientRECT()); 
	   	 	return out;
	    }
	}

	
	if ((nY + nYGap) > GetClientHeight()) 
	{
		//ChangePosNow(GetClientRECT()); 
		return 0;
  } 
	if (nX < 0)
    {
	    // nIndex = (abs(nX) + (nXgapHalf - 1)) / nXgapHalf
	    nIndex = (nXGapHalf-1-nX) / nXGapHalf;  
	    if ((BYTE)*(pszString+nIndex-1) >= tmCHINESE_CODE) nIndex++;
	    nX += nXGapHalf * nIndex;
    }
	else
    	nIndex = 0;
    
	//=======
	//      Show the string using the window's background and foreground color
	//=======
	m_pcFont->SetColor(m_fgc);
	nX0 = (nX += m_rectClient.left);
	nY += m_rectClient.top;
	nRight = m_rectClient.right + 1;
	_SetViewPort(0, 0, g_infoDrv.nX-1, g_infoDrv.nY-1);
	_SetColor((short)m_fgc);
	nStringWidth = 0;
	for (i=nIndex; i<nLength; i++)
    {
    	if ((wCode = (BYTE)*(pszString+i)) >= tmCHINESE_CODE)
        {
	        if ((nX += nXGapFull) > nRight) break;
	        wCode = B2W(pszString+i);
	        i++;
	        nXGap = nXGapFull;
        }
   		else
        {
        	//if (wCode == tmFONT_C_UNDERLINE)
            //{
            //	if (!bUnderLine)
            //    {
	        //        bUnderLine = TRUE;
	        //        continue;
            //    }
            //}
        	//else if (wCode == tmFONT_C_NEWLINE)
            //{
            //	printf("wCode =%d\n", wCode);
	        //    nY += nYGap;
	        //    if ((nY + nYGap) > (m_rectClient.bottom + 1)) break;
	        //    if (nStringWidth > nStringWidth0) nStringWidth0 = nStringWidth;
	        //    nX = nX0;
	        //    continue;
            //}
        	//if ((nX += nXGapHalf) > nRight) break;
        	//nXGap = nXGapHalf;
        }
        
   		if (m_pcFont->m_wFormat & tmFONT_OPAQUE)
        {
	        _SetColor((short)m_bgc);
	        if (m_pcFont->m_wFormat & tmFONT_BOLD)
            {
            	_Rectangle(_GFILLINTERIOR, nX-nXGap, nY, nX, nY+nYGap-1);
            }
        	else if (m_pcFont->m_wFormat & tmFONT_SHADOW)
            {
            	_Rectangle(_GFILLINTERIOR, nX-nXGap, nY, nX, nY+nYGap);
            }
        	else if (m_pcFont->m_wFormat & tmFONT_EMBOSS)
            {
            	_Rectangle(_GFILLINTERIOR, nX-nXGap-1, nY, nX, nY+nYGap);
            }
        	else
            	_Rectangle(_GFILLINTERIOR, nX-nXGap, nY, nX-1, nY+nYGap-1);
        	_SetColor((short)m_fgc);
        }
    	if ((m_pcFont->m_wFormat & tmFONT_UNDERLINE) || (bUnderLine))
        {
	        _MoveTo(nX-nXGap, nY+nYGap);
	        _LineTo(nX-1,     nY+nYGap);
	        bUnderLine = FALSE;
        }
        
		if (nXGap == nXGapHalf) nY++;
        
    	if (m_pcFont->m_wFormat & tmFONT_BOLD)
        {
        	m_pcFont->Show(nX-nXGap+1, nY, wCode);
        }
    	else if (m_pcFont->m_wFormat & tmFONT_SHADOW)
        {
	        m_pcFont->SetFontColor(~m_fgc);
	        m_pcFont->Show(nX-nXGap+1, nY+1, wCode);
	        m_pcFont->SetFontColor(m_fgc);
        }
   	 	else if (m_pcFont->m_wFormat & tmFONT_EMBOSS)
        {
	        m_pcFont->SetFontColor(BRIGHTWHITE);
	        m_pcFont->Show(nX-nXGap-1, nY, wCode);
	        m_pcFont->SetFontColor(GRAY);
	        m_pcFont->Show(nX-nXGap+1, nY+1, wCode);
	        m_pcFont->SetFontColor(m_bgc);
        }
   		else if (m_pcFont->m_wFormat & tmFONT_INSET)        //St122900 for button inset
        {
	        m_pcFont->SetFontColor(GRAY);
	        m_pcFont->Show(nX-nXGap, nY, wCode);
	        m_pcFont->SetFontColor(BRIGHTWHITE);
	        m_pcFont->Show(nX-nXGap+2, nY+1, wCode);
	        m_pcFont->SetFontColor(m_bgc);
        }
	    if (m_pcFont->m_wFormat & tmFONT_INSET)
	        m_pcFont->Show(nX-nXGap+1, nY+1, wCode);
	    else
        
	       m_pcFont->Show(nX-nXGap, nY, wCode);
    	nStringWidth += nXGap;
		if (nXGap == nXGapHalf) nY--;
    }
	if (nStringWidth0 > nStringWidth) nStringWidth = nStringWidth0;
	
	//ChangePosNow(GetClientRECT()); 
	return nStringWidth;    
	
}

void        CtmWnd::ShowExtent(short nY, char* pszString, int nType)
{
	if (m_pcFont == NULL)
	{
		char	sz[256];
		sprintf(sz, "%s have no font!!!\n", Name);
		perror(sz);
		return;
	}
	
	
	char*       psz1 = pszString;
	char*       psz2 = pszString;
	char*		pszTmp;
	int			strLen = 0;
	int			i;
	
	WORD*		wpsz1 = (WORD*)pszString;
	WORD*		wpsz2 = (WORD*)pszString;
	WORD*		wpszTmp;
	
	if(m_pcFont->GetFontCode() == tmFT_CODE_TECH && nType == 3)
	{
		while(*wpsz1++ != 0) strLen ++;
		wpsz1 	= (WORD*)pszString;	
		wpszTmp	= (WORD*)malloc(strLen * 2);
		i = 0;
		while (TRUE)
    	{
	    	while ((*wpsz1 != '\\') && (*wpsz1 != '\0'))
	    	{ 
	    		 *(wpszTmp + i++) = *wpsz1 ++;
	    		//psz1++;
	    	}
	    	if (*wpsz1 == '\\' && *(++wpsz1) == 'n')
	        {
		        *(wpszTmp + i)	= 0;
		        //*psz1 = 0;                   // Make a null terminator
		        ShowExtent1(nY, (char*)wpszTmp, nType);
		        i = 0;
		        //*psz1++ = '\n';                 // Restore it
		        wpsz1 ++;
		        wpsz2 = wpsz1;
		        nY += m_pcFont->m_wHeight;
	        }
	   		else
	        {
		        ShowExtent1(nY, (char*)wpsz2, nType);
		        break;                          // Stop
	        }
		}		
	    //ShowExtent1(nY, pszString, nType);  
	    free(wpszTmp); 
	    return;
	}
	
	while(*psz1++ != 0) strLen ++;
	psz1 	= pszString;	
	pszTmp	= (char *)malloc(strLen);
	
	while (TRUE)
    {
    	i = 0;
    	while ((*psz1 != '\n') && (*psz1 != '\0'))
    	{ 
    		 *(pszTmp + i++) = *psz1 ++;
    		//psz1++;
    	}
    	if (*psz1 == '\n')
        {
	        *(pszTmp + i)	= 0;
	        //*psz1 = 0;                   // Make a null terminator
	        ShowExtent1(nY, pszTmp, nType);
	        //*psz1++ = '\n';                 // Restore it
	        psz1 ++;
	        psz2 = psz1;
	        nY += m_pcFont->m_wHeight;
        }
   		else
        {
	        ShowExtent1(nY, psz2, nType);
	        break;                          // Stop
        }
    }
    free(pszTmp);
}

void CtmWnd::ShowExtent1(short nY, char* pszString, int nType)
{
	short       nX = 0;
	tmSIZE      size = GetStringExtent(pszString);
	short       nClientWidth = GetClientWidth();
	
	//	fans modify 2009-3-24 18:50:10
	if (m_paragraph.wAlign & tmPARA_ALIGN_CENTER)
	    nX = (nClientWidth - size.w) >> 1;
	else if (m_paragraph.wAlign & tmPARA_ALIGN_RIGHT)
	    nX = nClientWidth - size.w - m_wBorderWidth;
	else if (m_paragraph.wAlign & tmPARA_ALIGN_LEFT)
	    nX = 0;
	else
		;
		
	ShowExtent(nX, nY, pszString, nType);
}
//James add for Freetype 2015/4/27 13:29:07
void	CtmWnd::ShowExtent_UN(short nY, char* pszString, int nType)
{
	//printf("ShowExtent_UN 1 \n");
	short       nX = 0;
//	MARK by Jian.Wang 2015/7/31 14:57:42
//	short       nClientWidth = GetClientWidth();
	//printf("ShowExtent_UN 2 \n");
//	tmSIZE      size = GetUNStringExtent(pszString);
//	//printf("nClientWidth=%d, w=%d m_iFontUNType=%d ,m_wBorderWidth=%d\n", nClientWidth, size.w, m_iFontUNType,m_wBorderWidth);
//	if (m_paragraph.wAlign & tmPARA_ALIGN_CENTER)
//	    nX = (nClientWidth - size.w) >> 1;
//	else if (m_paragraph.wAlign & tmPARA_ALIGN_RIGHT)
//	    nX = nClientWidth - size.w - m_wBorderWidth-3;
//	else if (m_paragraph.wAlign & tmPARA_ALIGN_LEFT)
//	    nX = 0;
//	else
//		;
	//printf("nX=%d size.w=%d \n", nX, size.w);
	//printf("pszString=%s>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n",pszString);
	ShowExtent(nX, nY, pszString, nType);
}	

/*---------------------------------------------------------------------------+
|  Function : Clear(RECT rect)                                               |
|             Clear()                                                        |
|  Task     : Clear data in this window                                      |
+----------------------------------------------------------------------------+
|  Call     : CtmWnd::Clear()                                                  |
|                                                                            |
|  Parameter: rect                      - Upper-left and lower-right corners |
|                                         of a rectangle in client area      |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmWnd::Clear(RECT rect)
{
	//_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	//_SetColor(m_bgc);
	//_Rectangle(	_GFILLINTERIOR, 
    //			rect.left, 
    //			rect.top, 
    //			rect.right, 
    //			rect.bottom
    //		  );
    
	
	_SetViewPort(m_rectClient.left, 
				m_rectClient.top ,
				m_rectClient.right,
				m_rectClient.bottom);
	
    _SetColor(m_bgc);
    if (m_nBgcColorControl != 0)
    {				
	    //_Fill(m_nBgcColorControl, m_bgc, m_bgc, 
	    //		rect.left + (rect.right - rect.left) / 2, 
	    //		rect.top + (rect.bottom - rect.top) / 2);
	    		
	    _FillRect(m_nBgcColorControl, m_bgc, m_bgc, 
	    		rect.left, rect.top, 
	    		rect.right, rect.bottom, m_wDestColor, m_nColorChange);
	}
	else 
		_Rectangle(	_GFILLINTERIOR,    
        			rect.left,         
	       			rect.top,          
        			rect.right,        
   					rect.bottom);
}
   					
void CtmWnd::Clear()
{       		                     
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	_SetColor((short)m_bgc);
	
	if ((tmWS_FRAMEWND & m_wnd.wStyle) || m_nBgcColorControl== 0)
		_Rectangle(_GFILLINTERIOR, 0, 0, GetClientRight(), GetClientBottom());
	else
		_FillRect(m_nBgcColorControl, m_bgc, m_bgc, 
	    		0, 0, GetClientRight(), GetClientBottom(), m_wDestColor, m_nColorChange);
		
		//_Fill(m_nBgcColorControl, m_bgc, m_bgc, GetClientRight() / 2, GetClientBottom() / 2, m_wDestColor, m_nColorChange);
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmWnd::Move(short nX, short nY)
{
    short       nW = m_wnd.rect.right - m_wnd.rect.left;
    short       nH = m_wnd.rect.bottom - m_wnd.rect.top;

    m_wnd.rect.left   = nX;
    m_wnd.rect.top    = nY;
    m_wnd.rect.right  = nX + nW;
    m_wnd.rect.bottom = nY + nH;

    CalcRECT();
    DrawBorder();
    Show();
}

void        CtmWnd::Size(short nW, short nH)
{
    m_wnd.rect.right = m_wnd.rect.left + nW - 1;
    m_wnd.rect.bottom = m_wnd.rect.top + nH - 1;

    CalcRECT();
    DrawBorder();
    Show();
}

BOOL        CtmWnd::ptInRect(short x, short y, RECT *prect)
{
    if(x >= prect->left && x <= prect->right && \
        y >= prect->top && y <= prect->bottom)
        return TRUE;
    
    return FALSE;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
tmSIZE       CtmWnd::GetStringExtent(char* pszString)
{
short       i;
short       nLength = strlen(pszString);
WORD        wCode;
BOOL        bUnderLine = FALSE;

short       nXGapHalf = m_pcFont->m_wWidth >> 1;   // Gap of Half font - English
short       nXGapFull = m_pcFont->m_wWidth;        // Gap of Full font - Chinese
short       nYGap     = m_pcFont->m_wWidth;
short       nXSizeOld = 0;
tmSIZE      size      = {0, nYGap};
WORD        *pwCode;

if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
{
    nLength = 0;
    
    pwCode  = (WORD *)pszString;
    while(*pwCode != 0)
    {
        if(*pwCode >= FT_DIVISION)  size.w += nXGapFull;
        else                        size.w += nXGapHalf;            
        pwCode ++;
        nLength ++;
    }
    
    return size;
}

for (i=0; i<nLength; i++)
{
    if ((wCode = (BYTE)*(pszString+i)) >= tmCHINESE_CODE)
        {
        i++;
        size.w += nXGapFull;
        }
    else
        {
        if (wCode == tmFONT_C_UNDERLINE)
            {
            if (!bUnderLine)
                {
                bUnderLine = TRUE;
                continue;
                }
            }
        else if (wCode == tmFONT_C_NEWLINE)
            {
            size.h += nYGap;
            if (size.w > nXSizeOld) nXSizeOld = size.w;
            size.w = 0;
            continue;
            }
        bUnderLine = FALSE;
        size.w += nXGapHalf;
        }
}

if (nXSizeOld > size.w) size.w = nXSizeOld;
return size;
	
}

tmSIZE       CtmWnd::GetUNStringExtent(char* pszString)
{
	short       nYGap     = m_iFontUNWidth;
	tmSIZE      size      = {0, nYGap};
	int	nIndex = 0, nLen = 0;
	//printf("GetUNStringExtent 1 m_iFontUNType=%d \n", m_iFontUNType);
	if(m_iFontUNType == 2)
		nIndex = g_pParseXMLRecord->GetRecordIndex(pszString);
	else
		nIndex = g_pParseXMLViewStr->GetRecordIndex(pszString);
	//printf("nIndex=%d pszString=%s \n", nIndex, pszString);
	if(nIndex >=0)
	{
		if(m_iFontUNType == 2)
			nLen = g_pParseXMLRecord->GetRecordStr(nIndex).nUTFLen;
		else
			nLen = g_pParseXMLViewStr->GetRecordStr(nIndex).nUTFLen;
		
	}
	else
	{
		    if(pszString[0]=='\0')	return size;
			CtmEncode encode=CtmEncode("utf-8","utf-8");
			char	szUN[512];
			WORD*	pCode;
			char* 	pwch2;
			if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
			{	
				memset(szUN,0,sizeof(szUN));
				char* temp=szUN;
				 pCode   = (WORD *)pszString;
				 while(*pCode != 0x0000)
				 {
				 	*temp=pszString[0];
				 	if(pszString[1]!=0)
				 	{
				 	 	*++temp=pszString[1];		
				 	}
				 	++temp;
				 	++pCode;
				 	++++pszString;	
				 }
				 pwch2=szUN; 	
			}
			else
			{
				pwch2=(char*)pszString;		
			}
			int nLength	= (strlen(pwch2)+2)*4;
			unsigned long* unicode=(unsigned long*)malloc(nLength);
			int  nUTFLen = 0;
			nLen = encode.Utf8ConvertUnicode((unsigned char*)pwch2,unicode, &nUTFLen);
			nLen=nUTFLen;
			if(unicode!=NULL)
			{
				free(unicode);
				unicode=NULL;	
			}	
	}
	//printf("%s\n",pszString);
	//printf("nLen=%d \n", nLen);
	if(nLen > g_Length_X)
		nLen = 0;
	size.w = (nLen * m_iFontUNWidth/2);
	//printf("nLen=%d m_iFontUNWidth=%d w=%d \n", nLen, m_iFontUNWidth, size.w);
	return size;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void CtmWnd::DrawFocus()
{
	if(m_bDrawFocus)
	{
		short       nOldWriteMode = _SetWriteMode(_GXOR);
		short       nF = (short)m_wndattr.wWidthFocus;
		short       nW = m_rectWindow.right-m_rectWindow.left+nF*2;
		short       nH = m_rectWindow.bottom-m_rectWindow.top+nF*2;
		short       i;
		
		_SetViewPort(m_rectWindow.left-nF, m_rectWindow.top-nF, m_rectWindow.right+nF, m_rectWindow.bottom+nF);
		_SetColor(BRIGHTWHITE);
		//_SetLineStyle(0xAAAA);
		for (i=0; i<nF; i++)
			_Rectangle(_GBORDER, i, i, nW-i, nH-i);
		//_SetLineStyle(0xFFFF);
		_SetWriteMode(nOldWriteMode);
		
		RECT focus_rect; 
	
		MKRECT(focus_rect, m_rectWindow.left-nF-1 , m_rectWindow.top-nF-1 , m_rectWindow.right+nF+2 ,m_rectWindow.bottom+nF+2);
		ChangePosNow(focus_rect);
	}
}

void	CtmWnd::DrawSplit(tm_xycoord xy)
{
	CtmBitMap*	pBmpSplit;
	RECT rect;
	
	MKRECT(rect, 0, 0, tmSPLIT_WIDTH-1, tmSPLIT_HEIGHT-1);
	_SetViewPort(xy.xcoord, xy.ycoord+tmHEIGHT_STATUS, xy.xcoord+tmSPLIT_WIDTH-1, xy.ycoord+tmHEIGHT_STATUS+tmSPLIT_HEIGHT-1);
	pBmpSplit = new CtmBitMap;
	pBmpSplit->Create(tmBMP_SPLIT);
	pBmpSplit->SetKeyColor(0xffff, 0, 0);
	pBmpSplit->Show(rect, tmBMP_KEYCOLOR);
	delete pBmpSplit;
}

void	CtmWnd::CopyBg()												//<<yang 2005/11/23 
{
	RECT	rect = m_rectWindow;//m_wnd.rect;
	
	if (m_pImage != NULL) free(m_pImage);
	m_lImageSize = _ImageSize(rect.left, rect.top, rect.right, rect.bottom);
	m_pImage = (char*)malloc(m_lImageSize);
	_SetViewPort(rect.left, rect.top, rect.right, rect.bottom);
	_GetImage(0, 0, rect.right-rect.left, rect.bottom-rect.top, m_pImage);
}

void	CtmWnd::ShowBg()												//<<yang 2005/11/23 
{
	RECT	rect = m_rectWindow;//m_wnd.rect;
	
	if (m_pImage == NULL) return;
	_SetViewPort(rect.left, rect.top, rect.right, rect.bottom);
	_PutImage(0, 0, m_pImage, _GPSET);
}

void	CtmWnd::FreeBg()												//<<yang 2005/11/23 
{
	if (m_pImage == NULL) return;
	free(m_pImage);
	m_pImage = NULL;
}


//	fans add::
/*---------------------------------------------------------------------------+
|  Function : GetPropCount()								                 |
|  Task     : 				                                           		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int CtmWnd::GetPropCount()
{
	return PropCount;
}

/*---------------------------------------------------------------------------+
|  Function : GetControlCount()		 						                 |
|  Task     : 				                                           		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int CtmWnd::GetControlCount()
{
	return m_controlCount;
}

/*---------------------------------------------------------------------------+
|  Function : InitProps()		 							                 |
|  Task     : 				                                           		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmWnd::InitProps()
{
	for(int i = 0; i < MAX_PROPERTY; i++)
	{
		memset(Propertys[i].pszName, 0, sizeof(Propertys[i].pszName));
		//Propertys[i].pszName 	= NULL;
		Propertys[i].nNub		= -1;
		Propertys[i].ptype 		= tmUnknown;
		Propertys[i].propAddress= NULL;
	}
}

/*---------------------------------------------------------------------------+
|  Function : SetPropInfo(char* pszName, PropType PType, void* PropAddress)  |
|  Task     : 				                                           		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmWnd::SetPropInfo(char* pszName, PropType PType, void* PropAddress)
{
	if (PropCount >= MAX_PROPERTY)
	{
		//cout << Name << " Error info: Propertys too more!" << PropCount << endl;
		return;
	}
	int index /*nLen = strlen(pszName)*/;
	
	index = FindProperty(pszName);	//	檢測新增加的屬性是否已經有了
	if (index < 0)	//	增加新的屬性信息
	{
		//Propertys[PropCount].pszName = new char[ nLen+ 1];
		//memcpy(Propertys[PropCount].pszName, pszName, nLen+1);
		//printf("Name=%s,Propertys[%d].pszName=%s\n",this->TYPEName,PropCount,pszName);
		strcpy(Propertys[PropCount].pszName, pszName);
		Propertys[PropCount].ptype = PType;
		Propertys[PropCount++].propAddress = PropAddress;
	}
	else	//	改變已有的屬性信息
	{
		//cout << Name << " modify propertys!!"  << pszName << endl;
		Propertys[index].ptype = PType;
		Propertys[index].propAddress = PropAddress;
	}
}

/*---------------------------------------------------------------------------+
|  Function : SetPropInfo(char* pszName, PropType PType, void* PropAddress)  |
|  Task     : 				                                           		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmWnd::SetPropInfo(int nNub, PropType PType, void* PropAddress)
{
	if (nNub >= MAX_PROPERTY) 
	{
		cout << Name << " Error info: Propertys too more!" << PropCount << endl;
		return;
	}
	PropCount = MAX_PROPERTY;
	Propertys[nNub].nNub = nNub;
	Propertys[nNub].ptype = PType;
	Propertys[nNub].propAddress = PropAddress;
}

/*---------------------------------------------------------------------------+
|  Function : SetParent(CtmWnd* Parent)										 |
|  Task     : 				                                           		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
//add 2014/5/7 17:12:17
void	CtmWnd::SetControlCountZero(CtmWnd* Parent)
{
	m_parent=Parent;
	m_parent->m_controlCount=1;
	m_parent=NULL;
}

void CtmWnd::SetParent(CtmWnd* Parent)
{
	int index;
	
	//==========
	//	設置宿主
	//==========	
	
	if ((m_parent == NULL) && (Parent != NULL) && (Parent != this))
	{
		m_parent = Parent;
		m_parent->Controls[m_parent->m_controlCount++] = this;
		//printf("Set m_parent=%s, Name=%s, count=%d\n", m_parent->Name, Name, m_parent->m_controlCount);
	}
	else if ((m_parent != NULL) && (Parent == NULL))
	{
		index = m_parent->FindControl(this);
		//printf("NULL parent=%s, control=%d; name=%s, index=%\d\n", 
		//		m_parent->Name, m_parent->m_controlCount,
		//		Name, index);
		if (index >= 0)
		{
			
			if (index < (m_parent->m_controlCount))
			{
				memcpy(&m_parent->Controls[index], &m_parent->Controls[index + 1], 
						sizeof(CtmWnd*) * (m_parent->m_controlCount - index-1));
			}
			
			m_parent->Controls[--(m_parent->m_controlCount)] = NULL;
			
		}
		m_parent = Parent;
	}
	else if ((m_parent != NULL) && (Parent != NULL) && 
			 (m_parent != Parent) && (Parent != this))
	{
		SetParent(NULL);
		SetParent(Parent);
	
		
		//index = m_parent->FindControl(this);
		//if (index >= 0)
		//{
		//	for (int i = index; i <= m_parent->m_controlCount - 1; i++)
		//	{
		//		m_parent->Controls[i] = m_parent->Controls[i + 1];
		//	}
		//	m_parent->Controls[(m_parent->m_controlCount)--] = NULL;
		//}
		//
		//m_parent = Parent;
		//m_parent->Controls[m_parent->m_controlCount++] = this;
		
	}
	else return;	
	
}

/*---------------------------------------------------------------------------+
|  Function : SetPropertys()												 |
|  Task     : 				                                           		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmWnd::SetPropertys()
{
	SetPropInfo("privilege",	tmINT,		&m_iPrivilege);
	SetPropInfo("left", 		tmSHORT, 	&m_wnd.rect.left);		
	SetPropInfo("top", 			tmSHORT, 	&m_wnd.rect.top);		
	SetPropInfo("right", 		tmSHORT, 	&m_wnd.rect.right);		
	SetPropInfo("bottom", 		tmSHORT, 	&m_wnd.rect.bottom);		
	SetPropInfo("wtype", 		tmWORD, 	&m_wnd.wStyle);	
	SetPropInfo("taborder",		tmINT,		&m_taborder);
	SetPropInfo("fgc", 			tmDWORD,	&m_fgc);
	SetPropInfo("bgc", 			tmDWORD,	&m_bgc);
	SetPropInfo("title",		tmUSTR,		&m_wnd.pszTitle);
	SetPropInfo("titleID",		tmSTR,		&m_pszTitleID);
	SetPropInfo("fontsize",		tmINT,		&m_iFontType);	
	SetPropInfo("titlewidth",	tmINT, 		&m_iTitleWidth);
	SetPropInfo("titleheight",	tmINT, 		&m_iTitleHeight);
	SetPropInfo("leftcontrol",	tmSTR,		&m_pszLeftControl);	
	SetPropInfo("rightcontrol",	tmSTR,		&m_pszRightControl);
	SetPropInfo("upcontrol",	tmSTR,		&m_pszUpControl);
	SetPropInfo("downcontrol",	tmSTR,		&m_pszDownControl); 
	SetPropInfo("visible",		tmBOOL,		&m_bVisible);
	SetPropInfo("enabled",		tmBOOL,		&m_bEnabled);
	
	SetPropInfo("extag",		tmINT,		&m_iExTag);
	SetPropInfo("colorcontrol", tmINT,		&m_nBgcColorControl);
	SetPropInfo("colorchange",	tmINT,		&m_nColorChange);
	SetPropInfo("destcolor",	tmWORD,		&m_wDestColor);
	SetPropInfo("bordercolor",	tmWORD,		&m_wBorderColor);
	
	char sz[16];
	//for (int i = 0; i < MAX_HELP_NUM; i++)
	//{
	//	sprintf(sz, "%s%d", "helpstrID", i);
	//	SetPropInfo(sz,		tmSTR, 		&m_pszHelpStrIDs[i]);	
	//}
	SetPropInfo("helptype",		tmINT,		&m_nHelpType);
	SetPropInfo("fontUNType",		tmINT,		&m_iFontUNType);	
	SetPropInfo("fontUNWidth",		tmINT,		&m_iFontUNWidth);	
	SetPropInfo("fontUNHeight",		tmINT,		&m_iFontUNHeight);
	SetPropInfo("draw-focus",		tmBOOL, 	&m_bDrawFocus);
	
//	SetPropInfo(WND_AI_FLAG,	tmINT,		&m_nAiType);
//	SetPropInfo(WND_AI_LINKWND,	tmSTR,		&m_szAiWnd);
//	SetPropInfo(WND_AI_LINKDB,	tmSTR,		&m_szAiID);
//	SetPropInfo(WND_AI_VALUE,	tmINT64,	&m_llAiValue);	
//	SetPropInfo(WND_AI_DISIBLE_DEFAULT,	tmINT64,	&m_llAidValue);	
//	SetPropInfo(WND_AI_VISILBE_DEFAULT,	tmINT64,	&m_llAivValue);	
//	
//	//	fans add 2009-4-7 13:04:02
//	SetPropInfo(WND_AI_MSGID, 	tmINT,		&m_nMsgID);
//	SetPropInfo(WND_AI_LPARAM, 	tmINT,		&m_nLParam);
//	SetPropInfo(WND_AI_WPARAM, 	tmINT,		&m_nWParam);
//
//	
//	char	szTemp[12];
//	for (int i = 0; i < sizeof(m_llAiValues) / sizeof(m_llAiValues[0]); i++)
//	{
//		sprintf(szTemp, "%s%d", WND_AI_VALUE, i);
//		SetPropInfo(szTemp,		tmINT64,	&m_llAiValues[i]);
//	}
}

void CtmWnd::SetPropertys_Nub()
{
	SetPropInfo(WND_PRIVILEGE,		tmINT,		&m_iPrivilege);
	SetPropInfo(WND_LEFT, 			tmSHORT, 	&m_wnd.rect.left);		
	SetPropInfo(WND_TOP, 			tmSHORT, 	&m_wnd.rect.top);		
	SetPropInfo(WND_RIGHT, 			tmSHORT, 	&m_wnd.rect.right);		
	SetPropInfo(WND_BOTTOM, 		tmSHORT, 	&m_wnd.rect.bottom);		
	SetPropInfo(WND_WTYPE, 			tmWORD, 	&m_wnd.wStyle);	
	SetPropInfo(WND_TABORDER,		tmINT,		&m_taborder);
	SetPropInfo(WND_FGC, 			tmDWORD,	&m_fgc);
	SetPropInfo(WND_BGC, 			tmDWORD,	&m_bgc);
	SetPropInfo(WND_TITLE,			tmUSTR,		&m_wnd.pszTitle);
	SetPropInfo(WND_TITLEID,		tmSTR,		&m_pszTitleID);
	SetPropInfo(WND_FONTSIZE,		tmINT,		&m_iFontType);	
	SetPropInfo(WND_TITLEWIDTH,		tmINT, 		&m_iTitleWidth);
	SetPropInfo(WND_TITLEHEIGHT,	tmINT, 		&m_iTitleHeight);
	SetPropInfo(WND_LEFTCONTROL,	tmSTR,		&m_pszLeftControl);	
	SetPropInfo(WND_RIGHTCONTROL,	tmSTR,		&m_pszRightControl);
	SetPropInfo(WND_UPCONTROL,		tmSTR,		&m_pszUpControl);
	SetPropInfo(WND_DOWNCONTROL,	tmSTR,		&m_pszDownControl); 
	SetPropInfo(WND_VISIBLE,		tmBOOL,		&m_bVisible);
	SetPropInfo(WND_ENABLED,		tmBOOL,		&m_bEnabled);
	
	SetPropInfo(WND_EXTAG,			tmINT,		&m_iExTag);
	SetPropInfo(WND_COLORCONTROL, 	tmINT,		&m_nBgcColorControl);
	SetPropInfo(WND_COLORCHANGE,	tmINT,		&m_nColorChange);
	SetPropInfo(WND_DESTCOLOR,		tmWORD,		&m_wDestColor);
	SetPropInfo(WND_BORDERCOLOR,	tmWORD,		&m_wBorderColor);
}

//	
/*---------------------------------------------------------------------------+
|  Function : FindProperty(char* pszPropertyName)							 |
|  Task     : 查找在控件是否具有指定的屬性，返回-1表示沒有，否則就是找到了	 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmWnd::FindProperty(char* pszPropertyName)
{
	int result = -1;
//	if(PropCount == MAX_PROPERTY)
//	{
//		for(int i = 0; i < CLASS_MAXCOUNT; i++)
//		{
//			//printf("TYPEName=%s tm_ClassList.pszClassName[i]=%s result=%d \n", TYPEName, tm_ClassList.pszClassName[i], strcmp(TYPEName, tm_ClassList.pszClassName[i]));
//			if(strcmp(TYPEName, tm_ClassList.pszClassName[i]) == 0)
//			{
//				//printf("TYPEName=%s tm_ClassList.pszClassName[i]=%s result=%d \n", TYPEName, tm_ClassList.pszClassName[i], strcmp(TYPEName, tm_ClassList.pszClassName[i]));
//				for(int j = 0; j < tm_ClassList.dwPropertyCount[i]; j++)
//				{
//					//printf("pszPropertyName=%s pszproperty[%d][%d]=%s\n", pszPropertyName, i, j, tm_ClassList.pszClassProperty[i][j].pszProperty);
//					if(strcmp(pszPropertyName, tm_ClassList.pszClassProperty[i][j].pszProperty) == 0)
//					{
//						result = j;
//						break;
//					}	
//				}
//				break;
//			}
//		}
//	}
//	else
	{
		for (int i = 0; i < PropCount; i++)
		{
			if (Propertys[i].pszName[0] != '\0')
			if (strcmp(Propertys[i].pszName, pszPropertyName) == 0)
			{
				result = i;
				break;
			}
		}
	}
	return result;
}

/*---------------------------------------------------------------------------+
|  Function : FindProperty(int nNub)										 |
|  Task     : 查找在控件是否具有指定的屬性，返回-1表示沒有，否則就是找到了	 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmWnd::FindProperty(int nNub)
{
	int result = -1;
	if(nNub > MAX_PROPERTY && nNub < 0)
	{
		return result;
	}
	else
	{
		if (Propertys[nNub].nNub != -1)
			return Propertys[nNub].nNub;
	}
	return result;
}

/*---------------------------------------------------------------------------+
|  Function : GetProperty(char* pszPropertyName)							 |
|  Task     : 	 															 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
tmPropInfo*	 CtmWnd::GetProperty(char* pszPropName)
{
	tmPropInfo* pResult = NULL;
	int index;
	index = FindProperty(pszPropName);
	if (index >= 0)
		pResult = &Propertys[index];	
	
	return pResult;
}

/*---------------------------------------------------------------------------+
|  Function : GetProperty(int nNub)											 |
|  Task     : 	 															 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
tmPropInfo*	 CtmWnd::GetProperty(int nNub)
{
	tmPropInfo* pResult = NULL;
	int index;
	index = FindProperty(nNub);
	if (index >= 0)
		pResult = &Propertys[index];	
	
	return pResult;
}

/*---------------------------------------------------------------------------+
|  Function : GetPropValue(char* pszPropName)								 |
|  Task     : 根據屬性名稱而獲取相關的屬性值								 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void* 	CtmWnd::GetPropValue(char* pszPropName)		//	
{
	void* pResult = NULL;
	int index;
	index = FindProperty(pszPropName);
	if (index >= 0)	pResult = Propertys[index].propAddress;
	return pResult;	
}

/*---------------------------------------------------------------------------+
|  Function : GetPropValue(int nNub)								 		 |
|  Task     : 根據屬性名稱而獲取相關的屬性值								 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void* 	CtmWnd::GetPropValue(int nNub)		//	
{
	void* pResult = NULL;
	int index;
	index = FindProperty(nNub);
	if (index >= 0)	pResult = Propertys[index].propAddress;
	return pResult;	
}

/*---------------------------------------------------------------------------+
|  Function : SetProperty(char* pszPropName, char* pszValue)				 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool 	CtmWnd::SetProperty(char* pszPropName, char* pszValue)
{
	
	char* pszTemp = NULL; 
	tmPropInfo* pPropInfo = NULL;
	m_bPropertyChanged = FALSE;
	if (pszPropName == NULL) return false;
	pPropInfo = GetProperty(pszPropName);
	
	if (pPropInfo != NULL)
	{
		//if (this->Is("CtmEditX2"))
		//printf("%s.%s property changed!!\n", Name, pszPropName);
		int		nTemp = 0;
		short	snTemp = 0;
		long 	lTemp = 0;
		long long llTemp =0;
		WORD	wTemp = 0;
		DWORD	dwTemp = 0;
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				break;
			case tmINT:
				 
				nTemp = (int)strtol(pszValue, NULL, 0);
				if (nTemp != *(int*)(pPropInfo->propAddress))
				{
					*(int*)(pPropInfo->propAddress)= strtol(pszValue, NULL, 0);
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				snTemp = (short)strtol(pszValue, NULL,0);
				if (snTemp != *(short*)(pPropInfo->propAddress)) 
				{
					*(short*)(pPropInfo->propAddress) = snTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmLONG:
				lTemp = strtol(pszValue, NULL, 0);
				if (lTemp != *(long*)(pPropInfo->propAddress)) 
				{
					*(long*)(pPropInfo->propAddress) = lTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT64:
				llTemp =strtoll(pszValue, NULL, 0);
				
				if (llTemp != *(long long*)(pPropInfo->propAddress))
				{
					if (Name[0]!='\0')
					//printf("%s, old=%lld, new=%lld\n", Name, *(long long*)(pPropInfo->propAddress) , 
					//										llTemp);
					*(long long*)(pPropInfo->propAddress) = llTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				wTemp = (WORD)strtol(pszValue, NULL, 0);
				
				if (wTemp != *(WORD*)(pPropInfo->propAddress))
				{
					*(WORD*)(pPropInfo->propAddress) = wTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				dwTemp = strtol(pszValue, NULL, 0);
				
				if (dwTemp != *(DWORD*)(pPropInfo->propAddress))
				{
					*(DWORD*)(pPropInfo->propAddress) = dwTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				
				if (strcmp(pszValue, "true") == 0 || strcmp(pszValue, "TRUE") == 0)
				{
					if (!*(BOOL*)(pPropInfo->propAddress))
					{
						*(BOOL*)(pPropInfo->propAddress) = TRUE;
						m_bPropertyChanged = TRUE;
					}
				}
				else
				{
					if (*(BOOL*)(pPropInfo->propAddress))
					{
						*(BOOL*)(pPropInfo->propAddress) = FALSE;
						m_bPropertyChanged = TRUE;
					}
				}
				break;
			case tmFLOAT:
				*(float*)(pPropInfo->propAddress) = atof(pszValue);
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				*(double*)(pPropInfo->propAddress) = *(double*)pszValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmSTR:
			case tmPROCEDURE:
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
						
					{
						//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= strlen(pszValue)) )
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = (char* ) malloc(strlen(pszValue) + 1);//new char[strlen(pszValue) + 1];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char* ) malloc(strlen(pszValue) + 1);//new char[strlen(pszValue) + 1];
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),pszValue);
				break;
			default:
				return false;
		}
		
	}
	return (pPropInfo != NULL);
}

/*---------------------------------------------------------------------------+
|  Function : SetProperty(int nNub, char* pszValue)         				 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool 	CtmWnd::SetProperty(int nNub, char* pszValue)
{
	char* pszTemp = NULL; 
	tmPropInfo* pPropInfo = NULL;
	m_bPropertyChanged = FALSE;
	if (nNub > MAX_PROPERTY && nNub < 0) return false;
	pPropInfo = GetProperty(nNub);
	
	if (pPropInfo != NULL)
	{
		int		nTemp = 0;
		short	snTemp = 0;
		long 	lTemp = 0;
		long long llTemp =0;
		WORD	wTemp = 0;
		DWORD	dwTemp = 0;
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				break;
			case tmINT:
				nTemp = (int)strtol(pszValue, NULL, 0);
				if (nTemp != *(int*)(pPropInfo->propAddress))
				{
					*(int*)(pPropInfo->propAddress)= strtol(pszValue, NULL, 0);
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				snTemp = (short)strtol(pszValue, NULL,0);
				if (snTemp != *(short*)(pPropInfo->propAddress)) 
				{
					*(short*)(pPropInfo->propAddress) = snTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmLONG:
				lTemp = strtol(pszValue, NULL, 0);
				if (lTemp != *(long*)(pPropInfo->propAddress)) 
				{
					*(long*)(pPropInfo->propAddress) = lTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT64:
				llTemp =strtoll(pszValue, NULL, 0);
				
				if (llTemp != *(long long*)(pPropInfo->propAddress))
				{
					if (Name[0]!='\0')
					//printf("%s, old=%lld, new=%lld\n", Name, *(long long*)(pPropInfo->propAddress) , 
					//										llTemp);
					*(long long*)(pPropInfo->propAddress) = llTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				wTemp = (WORD)strtol(pszValue, NULL, 0);
				
				if (wTemp != *(WORD*)(pPropInfo->propAddress))
				{
					*(WORD*)(pPropInfo->propAddress) = wTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				dwTemp = strtol(pszValue, NULL, 0);
				
				if (dwTemp != *(DWORD*)(pPropInfo->propAddress))
				{
					*(DWORD*)(pPropInfo->propAddress) = dwTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				
				if (strcmp(pszValue, "true") == 0 || strcmp(pszValue, "TRUE") == 0)
				{
					if (!*(BOOL*)(pPropInfo->propAddress))
					{
						*(BOOL*)(pPropInfo->propAddress) = TRUE;
						m_bPropertyChanged = TRUE;
					}
				}
				else
				{
					if (*(BOOL*)(pPropInfo->propAddress))
					{
						*(BOOL*)(pPropInfo->propAddress) = FALSE;
						m_bPropertyChanged = TRUE;
					}
				}
				break;
			case tmFLOAT:
				*(float*)(pPropInfo->propAddress) = atof(pszValue);
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				*(double*)(pPropInfo->propAddress) = *(double*)pszValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmSTR:
			case tmPROCEDURE:
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
						//delete  [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= strlen(pszValue)) )
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) =(char* ) malloc(strlen(pszValue) + 1);//new char[strlen(pszValue) + 1];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char* )malloc(strlen(pszValue) + 1);//new char[strlen(pszValue) + 1];
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),pszValue);
				break;
			default:
				return false;
		}
		
	}
	
	
	return (pPropInfo != NULL);
	
}

/*---------------------------------------------------------------------------+
|  Function : SetProperty(char* pszPropName, void* pValue)					 |
|  Task     : 根據屬性名稱而對屬性進行相關的設置							 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool 	CtmWnd::SetProperty(char* pszPropName, void* pValue)//	
{
	char* pszTemp = NULL; 
	tmPropInfo* pPropInfo = NULL;
	m_bPropertyChanged = FALSE;
	if (pszPropName == NULL) return false;
	pPropInfo = GetProperty(pszPropName);
	if (pPropInfo != NULL)
	{
		//if (this->Is("CtmEditX2"))
		//printf("%s.%s property changed!!\n", Name, pszPropName);
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				
				if (*(BYTE*)(pPropInfo->propAddress) != *(BYTE*)pValue)
				{
					*(BYTE*)(pPropInfo->propAddress) = *(BYTE*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT:
				if (*(int*)(pPropInfo->propAddress) != *(int*)pValue)
				{
					*(int*)(pPropInfo->propAddress) = *(int*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				if (*(short*)(pPropInfo->propAddress) != *(short*)pValue)
				{
					*(short*)(pPropInfo->propAddress) = *(short*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmLONG:
				if (*(long*)(pPropInfo->propAddress) != *(long*)pValue)
				{
					*(long*)(pPropInfo->propAddress) = *(long*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT64:
				if (*(long long*)(pPropInfo->propAddress) != *(long long*)pValue)
				{
					//printf("%s, old=%lld, new=%lld\n", Name, *(long long*)(pPropInfo->propAddress) , 
					//										*(long long*)pValue);
					*(long long*)(pPropInfo->propAddress) = *(long long*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				if (*(WORD*)(pPropInfo->propAddress) != *(WORD*)pValue)
				{
					*(WORD*)(pPropInfo->propAddress) = *(WORD*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				if (*(DWORD*)(pPropInfo->propAddress) != *(DWORD*)pValue)
				{
					*(DWORD*)(pPropInfo->propAddress) = *(DWORD*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				if ( *(BOOL*)pValue != *(BOOL*)(pPropInfo->propAddress) )
				{
					if (*(BOOL*)pValue) *(BOOL*)(pPropInfo->propAddress) = TRUE;
					else *(BOOL*)(pPropInfo->propAddress) = FALSE;	
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmFLOAT:
				*(float*)(pPropInfo->propAddress) = *(float*)pValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				*(double*)(pPropInfo->propAddress) = *(double*)pValue;
				m_bPropertyChanged = TRUE;
				break;
			
			case tmSTR:
			case tmPROCEDURE:
				
				//break;
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
						//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char** )pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}									
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= strlen((char*)pValue)) )
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char** )pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) =(char*)malloc(strlen((char*)pValue) + 2);// new char[strlen((char*)pValue) + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(strlen((char*)pValue) + 2);//new char[strlen((char*)pValue) + 2];
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),(char*)pValue);
				break;
			default:
				return false;
		}
		
	}
	
	return (pPropInfo != NULL);
}

/*---------------------------------------------------------------------------+
|  Function : SetProperty(int nNub, void* pValue)							 |
|  Task     : 根據屬性名稱而對屬性進行相關的設置							 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool 	CtmWnd::SetProperty(int nNub, void* pValue)//	
{
	char* pszTemp = NULL; 
	tmPropInfo* pPropInfo = NULL;
	m_bPropertyChanged = FALSE;
	if (nNub > MAX_PROPERTY && nNub < 0) return false;
	pPropInfo = GetProperty(nNub);
	if (pPropInfo != NULL)
	{
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				if (*(BYTE*)(pPropInfo->propAddress) != *(BYTE*)pValue)
				{
					*(BYTE*)(pPropInfo->propAddress) = *(BYTE*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT:
				if (*(int*)(pPropInfo->propAddress) != *(int*)pValue)
				{
					*(int*)(pPropInfo->propAddress) = *(int*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				if (*(short*)(pPropInfo->propAddress) != *(short*)pValue)
				{
					*(short*)(pPropInfo->propAddress) = *(short*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmLONG:
				if (*(long*)(pPropInfo->propAddress) != *(long*)pValue)
				{
					*(long*)(pPropInfo->propAddress) = *(long*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT64:
				if (*(long long*)(pPropInfo->propAddress) != *(long long*)pValue)
				{
					*(long long*)(pPropInfo->propAddress) = *(long long*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				if (*(WORD*)(pPropInfo->propAddress) != *(WORD*)pValue)
				{
					*(WORD*)(pPropInfo->propAddress) = *(WORD*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				if (*(DWORD*)(pPropInfo->propAddress) != *(DWORD*)pValue)
				{
					*(DWORD*)(pPropInfo->propAddress) = *(DWORD*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				if ( *(BOOL*)pValue != *(BOOL*)(pPropInfo->propAddress) )
				{
					if (*(BOOL*)pValue) *(BOOL*)(pPropInfo->propAddress) = TRUE;
					else *(BOOL*)(pPropInfo->propAddress) = FALSE;	
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmFLOAT:
				*(float*)(pPropInfo->propAddress) = *(float*)pValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				*(double*)(pPropInfo->propAddress) = *(double*)pValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmSTR:
			case tmPROCEDURE:
				
				//break;
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
						//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}									
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= strlen((char*)pValue)) )
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = (char*)malloc(strlen((char*)pValue) + 2);//new char[strlen((char*)pValue) + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(strlen((char*)pValue) + 2);//new char[strlen((char*)pValue) + 2];
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),(char*)pValue);
				break;
			default:
				return false;
		}
	}
	
	return (pPropInfo != NULL);
}

/*---------------------------------------------------------------------------+
|  Function : GetParent()													 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmWnd* CtmWnd::GetParent()
{
	return m_parent;
}

/*---------------------------------------------------------------------------+
|  Function : SetTypeName(char* pszName)									 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmWnd::SetTypeName(char* pszName)
{
}


/*---------------------------------------------------------------------------+
|  Function : SetBaseInfo()													 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmWnd::SetBaseInfo()
{

}

/*---------------------------------------------------------------------------+
|  Function : FindControl(CtmWnd* pControl)									 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
//	fans add
int		CtmWnd::FindControl(CtmWnd* pControl)
{
	int iResult = -1;
	for (int i = 0; i <= m_controlCount; i++)
	{
		if (Controls[i] == pControl) 
		{
			iResult = i;
			break;
		}
	}
	return iResult;
}

//	fans add
/*---------------------------------------------------------------------------+
|  Function : FindControlFromTab(int iTabOrder)								 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmWnd*	CtmWnd::FindControlFromTab(int iTabOrder)
{
	CtmWnd* pwndResult = NULL;
	for (int i = 0; i < GetControlCount(); i++)
	{		
		if (Controls[i]->GetTabOrder() == iTabOrder)
		{
			pwndResult = Controls[i];
			break;
		}
	}
	
	//if (pwndResult == NULL)
	//{
	//	char szError[256];
	//	sprintf(szError, "%s can't find the Component that taborder is %d\n", Name, iTabOrder);
	//	perror(szError);
	//}
	
	return pwndResult;
}

CtmWnd*	CtmWnd::FindControlFromName(char* pszName)
{
	CtmWnd* pwndResult = NULL;
	for (int i = 0; i < GetControlCount(); i++)
	{	
		if (strcmp(Controls[i]->Name, pszName) == 0)
		{
			pwndResult = Controls[i];
			break;
		}
	}
	
	//if (pwndResult == NULL)
	//{
	//	char szError[256];
	//	sprintf(szError, "%s can't find the Component that name is %s\n", Name, pszName);
	//	perror(szError);
	//}
	
	return pwndResult;
}

/*---------------------------------------------------------------------------+
|  Function : GetControlA(int index)										 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmWnd*	CtmWnd::GetControlA(int index)
{
	if ((index < 0) || (index >= GetControlCount()))return NULL;
	else return Controls[index];
}

/*---------------------------------------------------------------------------+
|  Function : FindMaxTabOrder()												 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmWnd::FindMaxTabOrder()
{
	int iResult = -2;
	
	for (int i =0; i < GetControlCount(); i++)
	{
		if (Controls[i]->GetTabOrder() >= iResult) iResult = Controls[i]->GetTabOrder();
	}
	return iResult;
	
}

/*---------------------------------------------------------------------------+
|  Function : Is(char* pszType)												 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool	CtmWnd::Is(char* pszType)
{
	if (pszType == NULL) return false;
	return (strcmp(TYPEName, pszType) == 0);
}

/*---------------------------------------------------------------------------+
|  Function : StrIDToStr													 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmWnd::StrIDToStr(char* pszID, char** psz, BOOL bOnlineHelp)
{
	char* pszTemp = NULL;
	int len = 0;
	unsigned short *pwCode;
	if(m_iFontUNType>0)
	{
		if(pszID!=NULL)
		{
			if (*psz != NULL) 
			{
				free(*psz);
				*psz = NULL;
			}
			len = strlen(pszID);
    		*psz = (char*)malloc(len+3);
    		memset(*psz,0,len+3);
    		strcpy(*psz, pszID);
		}	
	}
	else
	{
		if (pszID != NULL)
		{
			if (!bOnlineHelp)
			{
				g_MultiLanguage.SetMultiCode(1);
				pszTemp = g_MultiLanguage[pszID];
			}
			else
			{
				g_MultiLanguageHelp.SetMultiCode(1);
				pszTemp = g_MultiLanguageHelp[pszID];
			}
			if (pszTemp != NULL)
			{
				if (*psz != NULL) 
				{
					//delete [](*psz);
					free(*psz);
					*psz = NULL;
				}
				if(m_pcFont==NULL)	return ;
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
				{
	    			len = strlen(pszTemp);
	    			*psz = (char*)malloc(len+1);//new char[len + 1];
	    			strcpy(*psz, pszTemp);
			    }
			    else
			    {
			        pwCode  = (unsigned short *)pszTemp;
			        while(pwCode[len ++] != 0);
			        *psz = (char*)malloc(len<<1);//new char[(len << 1)];
			        memcpy(*psz, pszTemp, len << 1);
			    }
			}
	  }
	}
}

/*---------------------------------------------------------------------------+
|  Function : SetLibHandle													 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmWnd::SetLibHandle(void* pLib)				//	設置lib 的句柄
{
	if (m_plibHandle != NULL) 
	{
		if (pLib != m_plibHandle) dlclose(m_plibHandle);		
	}
	m_plibHandle = pLib;
}


/*---------------------------------------------------------------------------+
|  Function : GetLibHandle													 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void*	CtmWnd::GetLibHandle()							//	得到lib 的句柄
{
	return m_plibHandle;
}

/*---------------------------------------------------------------------------+
|  Function : Show															 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void    CtmWnd::Show()                      
{
	if ((m_wnd.wStyle & tmWS_REDRAW) && (m_wnd.wStyle & tmWS_FRAMEWND)) 
	{
		CopyBg();						//<<yang 2005/11/23 
		Clear();  
		DrawBorder();
	}
	
	for(int i = 0; i < GetControlCount(); i++)
	{
		//	fans add 2009-7-15 14:27:32 For iNet
		/*if (g_iPrivilege == 0)	
		{
			if (m_parent != NULL &&	m_parent->m_iExTag != 10000 && Controls[i]->m_taborder >= 0)
			{
				Controls[i]->m_taborder = -2;
				Controls[i]->m_bEnabled = FALSE;
			}
		}*/
		
		//if (Controls[i]->m_iPrivilege <= g_iPrivilege)
		//{
			//printf("111.CtmWnd::Show() Name = %s \n",Controls[i]->Name);
			Controls[i]->Show();
		//}
		//else 
		//{
		//	Controls[i]->DrawBorder();
		//	Controls[i]->m_bVisible = FALSE;
		//}
	}

}

/*---------------------------------------------------------------------------+
|  Function :               Show(short nY, char* pszString)                  |
|  Task     : 				                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmWnd::ShowFontStream(short nX, short nY, char* pszString, int nLen, int nType)		//rw.ying 2011/1/13 09:58上午
{

	if (m_pcFont == NULL)
	{
		char	sz[256];
		sprintf(sz, "%s have no font!!!\n", Name);
		perror(sz);
		return;
	}
	m_pcFont->SetColor(m_fgc);
	m_pcFont->ShowFontString(nX, nY, pszString, nLen, nType);
	return;

}

/*---------------------------------------------------------------------------+
|  Function : SetPropValueT(char* pszPropName, double Value)				 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool 	 CtmWnd::SetPropValueT(char* pszPropName, double Value)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(pszPropName);
	
	BYTE	temp;
	int		ntemp;
	short	snTemp;
	long	lTemp;
	long long llTemp;
	WORD	wTemp;
	DWORD	dwTemp;
	BOOL	BLTemp;
	float	fTemp;
	double	dlTemp;
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				temp = static_cast<BYTE>(Value);
				if (temp != *(BYTE*)(pPropInfo->propAddress))
				{
					*(BYTE*)(pPropInfo->propAddress) = temp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT:
				ntemp = static_cast<int>(Value);
				if (ntemp != *(int*)(pPropInfo->propAddress))
				{
					*(int*)(pPropInfo->propAddress) = ntemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				snTemp = static_cast<short>(Value);
				if (snTemp != *(short*)(pPropInfo->propAddress))
				{                                               
					*(short*)(pPropInfo->propAddress) = snTemp;   
					m_bPropertyChanged = TRUE;                    
				}   
				break;
			case tmLONG:
				lTemp = static_cast<long>(Value);
				if (lTemp != *(long*)(pPropInfo->propAddress))
				{
					*(long*)(pPropInfo->propAddress) = lTemp;
					m_bPropertyChanged = TRUE;   
				}
				break;
			case tmINT64:
				llTemp = static_cast<long long>(Value);				
				if (llTemp != *(long long*)(pPropInfo->propAddress))
				{
					*(long long*)(pPropInfo->propAddress) = llTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				wTemp = static_cast<WORD>(Value);
				if (wTemp != *(WORD*)(pPropInfo->propAddress))
				{
					*(WORD*)(pPropInfo->propAddress) = wTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				dwTemp = static_cast<DWORD>(Value);
				if (dwTemp != *(DWORD*)(pPropInfo->propAddress))
				{
					*(DWORD*)(pPropInfo->propAddress) = dwTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				BLTemp = static_cast<BOOL>(Value);
				if (BLTemp != *(BOOL*)(pPropInfo->propAddress))
				{
					*(BOOL*)(pPropInfo->propAddress) = BLTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmFLOAT:
				fTemp = static_cast<float>(Value);
				*(float*)(pPropInfo->propAddress) = fTemp;
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				dlTemp = Value;
				*(double*)(pPropInfo->propAddress) = dlTemp;
				m_bPropertyChanged = TRUE;
				break;
			default:
				Result = false;
				break;
		}
	}
	
	return Result;
}

/*---------------------------------------------------------------------------+
|  Function : SetPropValueT(char* pszPropName, char* pszValue)				 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool 	CtmWnd::SetPropValueT(char* pszPropName, char* pszValue, int nLen, int nType)			//rw.ying 2011/3/17 02:52下午
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(pszPropName);
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmFontStream:
				if (pszValue == NULL)
				{
					if((*(char**)pPropInfo->propAddress) != NULL)
					{
						//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if((*(char**)pPropInfo->propAddress) != NULL)
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = NULL;
					(*(char**)pPropInfo->propAddress) =(char*) malloc(nLen+3);//new char[nLen];
				}
				else if((*(char**)pPropInfo->propAddress) == NULL)
					(*(char**)pPropInfo->propAddress) =(char*)malloc(nLen+3);// new char[nLen];
				m_bPropertyChanged = TRUE;
				memset((*(char**)pPropInfo->propAddress),0,nLen+3);
				memcpy((*(char**)pPropInfo->propAddress),pszValue,nLen+3);	
				break;
			default:
				Result = false;
				break;
		}
		if(strcmp(pszPropName, "stream") == 0)
		{
			SetPropValueT("iNetFontLen", 	nLen);
			SetPropValueT("iNetFontType", 	nType);
		}
	}
	return Result;
}
/*---------------------------------------------------------------------------+
|  Function : SetPropValueT(int nNub, double Value)				 			 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool 	 CtmWnd::SetPropValueT(int nNub, double Value)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(nNub);
	
	BYTE	temp;
	int		ntemp;
	short	snTemp;
	long	lTemp;
	long long llTemp;
	WORD	wTemp;
	DWORD	dwTemp;
	BOOL	BLTemp;
	float	fTemp;
	double	dlTemp;
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				temp = static_cast<BYTE>(Value);
				if (temp != *(BYTE*)(pPropInfo->propAddress))
				{
					*(BYTE*)(pPropInfo->propAddress) = temp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT:
				ntemp = static_cast<int>(Value);
				if (ntemp != *(int*)(pPropInfo->propAddress))
				{
					*(int*)(pPropInfo->propAddress) = ntemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				snTemp = static_cast<short>(Value);
				if (snTemp != *(short*)(pPropInfo->propAddress))
				{                                               
					*(short*)(pPropInfo->propAddress) = snTemp;   
					m_bPropertyChanged = TRUE;                    
				}   
				break;
			case tmLONG:
				lTemp = static_cast<long>(Value);
				if (lTemp != *(long*)(pPropInfo->propAddress))
				{
					*(long*)(pPropInfo->propAddress) = lTemp;
					m_bPropertyChanged = TRUE;   
				}
				break;
			case tmINT64:
				llTemp = static_cast<long long>(Value);				
				if (llTemp != *(long long*)(pPropInfo->propAddress))
				{
					*(long long*)(pPropInfo->propAddress) = llTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				wTemp = static_cast<WORD>(Value);
				if (wTemp != *(WORD*)(pPropInfo->propAddress))
				{
					*(WORD*)(pPropInfo->propAddress) = wTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				dwTemp = static_cast<DWORD>(Value);
				if (dwTemp != *(DWORD*)(pPropInfo->propAddress))
				{
					*(DWORD*)(pPropInfo->propAddress) = dwTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				BLTemp = static_cast<BOOL>(Value);
				if (BLTemp != *(BOOL*)(pPropInfo->propAddress))
				{
					*(BOOL*)(pPropInfo->propAddress) = BLTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmFLOAT:
				fTemp = static_cast<float>(Value);
				*(float*)(pPropInfo->propAddress) = fTemp;
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				dlTemp = Value;
				*(double*)(pPropInfo->propAddress) = dlTemp;
				m_bPropertyChanged = TRUE;
				break;
			default:
				Result = false;
				break;
		}
	}
	
	return Result;
}


/*---------------------------------------------------------------------------+
|  Function : SetPropValueT(char* pszPropName, char* pszValue)				 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool 	CtmWnd::SetPropValueT(char* pszPropName, char* pszValue)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(pszPropName);
	char* pszTemp = NULL;
	int nStrLen = 0;
	unsigned short *pwDest, *pwCode  = (unsigned short *)pszValue;
	
	if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
	{
	    if (pszValue != NULL)					//Yang 
	    	nStrLen = strlen(pszValue);
	}
	else
	{
	    if(pwCode != NULL)
	    {
    	    while(*pwCode != 0)
    	    {
    	        nStrLen += 2;
    	        pwCode ++;
    	    }
	    }
	    else
	        nStrLen = 0;
	}
	
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmSTR:
			case tmPROCEDURE:
				if (pszValue != NULL) nStrLen = strlen(pszValue);
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
						//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= nStrLen) )       
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = (char*) malloc(nStrLen + 2);//new char[nStrLen + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(nStrLen + 2);//new char[nStrLen + 2];      
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),pszValue);
				
				
				break;
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
						//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress );
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= nStrLen) )       //netdigger 2007/4/10
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress );
					(*(char**)pPropInfo->propAddress) =(char*)malloc(nStrLen + 2); //new char[nStrLen + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(nStrLen + 2);//new char[nStrLen + 2];      //netdigger 2007/4/10
				}
				m_bPropertyChanged = TRUE;
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
				{
				    strcpy((*(char**)pPropInfo->propAddress),pszValue);
				}
				else
				{
					if (pwCode == NULL) break;
				    pwCode  = (unsigned short *)pszValue;
				    pwDest  = (unsigned short *)(*(char**)pPropInfo->propAddress);
				    memcpy(pwDest, pwCode, nStrLen + 2);
/*				    while(*pwCode != 0)
				    {
				        *pwDest = *pwCode;
				        pwDest ++;
				        pwCode ++;
				    }
				    *pwDest  = 0;
*/
				}
				break;
			default:
				Result = false;
				break;
		}
	}
	return Result;
}

/*---------------------------------------------------------------------------+
|  Function : SetPropValueT(int nNub, char* pszValue)						 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool 	CtmWnd::SetPropValueT(int nNub, char* pszValue)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(nNub);
	char* pszTemp = NULL;
	int nStrLen = 0;
	unsigned short *pwDest, *pwCode  = (unsigned short *)pszValue;
	
	if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
	{
	    if (pszValue != NULL)					//Yang 
	    	nStrLen = strlen(pszValue);
	}
	else
	{
	    if(pwCode != NULL)
	    {
    	    while(*pwCode != 0)
    	    {
    	        nStrLen += 2;
    	        pwCode ++;
    	    }
	    }
	    else
	        nStrLen = 0;
	}
	
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmSTR:
			case tmPROCEDURE:
				if (pszValue != NULL) nStrLen = strlen(pszValue);
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{	//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);					
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= nStrLen) )       
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = (char*)malloc(nStrLen + 2);//new char[nStrLen + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) =(char*)malloc(nStrLen + 2) ;//new char[nStrLen + 2];      
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),pszValue);
				
				
				break;
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{	//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= nStrLen) )       //netdigger 2007/4/10
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) =(char*)malloc(nStrLen + 2)  ;//new char[nStrLen + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(nStrLen + 2) ;//new char[nStrLen + 2];      //netdigger 2007/4/10
				}
				m_bPropertyChanged = TRUE;
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
				{
				    strcpy((*(char**)pPropInfo->propAddress),pszValue);
				}
				else
				{
					if (pwCode == NULL) break;
				    pwCode  = (unsigned short *)pszValue;
				    pwDest  = (unsigned short *)(*(char**)pPropInfo->propAddress);
				    memcpy(pwDest, pwCode, nStrLen + 2);
/*				    while(*pwCode != 0)
				    {
				        *pwDest = *pwCode;
				        pwDest ++;
				        pwCode ++;
				    }
				    *pwDest  = 0;
*/
				}
				break;
			default:
				Result = false;
				break;
		}
	}
	return Result;
}

/*---------------------------------------------------------------------------+
|  Function : GetPropValueT(char* pszPropName, void* pValue, int nDataSize)	 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool CtmWnd::GetPropValueT(char* pszPropName, void* pValue, int nDataSize)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(pszPropName);
	if (pPropInfo != NULL)
	{
		Result = true;
		if (nDataSize > 0) memset(pValue, 0, nDataSize);
		switch (pPropInfo->ptype)
		{
			case tmBYTE:				
					*(BYTE*)pValue = *(BYTE*)(pPropInfo->propAddress);					
				break;
			case tmINT:
				if (nDataSize == -1 || nDataSize >= sizeof(int))
					*(int*)pValue = *(int*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmSHORT:
				if (nDataSize == -1 || nDataSize >= sizeof(short))
					*(short*)pValue = *(short*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmLONG:
				if (nDataSize == -1 || nDataSize >= sizeof(long))
					*(long*)pValue = *(long*)(pPropInfo->propAddress);
				else 
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmINT64:
				if (nDataSize == -1 || nDataSize >= sizeof(long long))
					*(long long*)pValue = *(long long*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmWORD:
				if (nDataSize == -1 || nDataSize >= sizeof(WORD))
					*(WORD*)pValue = *(WORD*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmDWORD:
				if (nDataSize == -1 || nDataSize >= sizeof(DWORD))
					*(DWORD*)pValue = *(DWORD*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmBOOL:
				if (nDataSize == -1 || nDataSize >= sizeof(WORD))
					*(BOOL*)pValue = *(BOOL*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);	
				}
				break;
			case tmFLOAT:
				if (nDataSize == -1 || nDataSize == sizeof(float))
					*(float*)pValue = *(float*)(pPropInfo->propAddress);
				else if (nDataSize == sizeof(double))
				{
					*(double*)pValue = *(float*)(pPropInfo->propAddress);
				}
				else if  (nDataSize == 1)
				{
					*(BYTE*)pValue = static_cast<BYTE>(*(float*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 2)
				{
					*(short*)pValue = static_cast<short>(*(float*)(pPropInfo->propAddress));
				}
				else Result = false;
				break;
			case tmDOUBLE:
				if (nDataSize == -1 || nDataSize == sizeof(double))
					*(double*)pValue = *(double*)(pPropInfo->propAddress);
				else if (nDataSize == sizeof(float))
				{
					*(float*)pValue = static_cast<float>(*(double*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 1)
				{
					*(BYTE*)pValue = static_cast<BYTE>(*(double*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 2)
				{
					*(short*)pValue = static_cast<short>(*(double*)(pPropInfo->propAddress));
				}
				else Result = false;
				break;
			case tmSTR:
			case tmPROCEDURE:
			    
				//(char*)pValue = *(char**)(pPropInfo->propAddress);
				if (*(char**)(pPropInfo->propAddress) != NULL)
				{
					if (nDataSize == -1)
						strcpy((char*)pValue, *(char**)(pPropInfo->propAddress));
					else
						strncpy((char*)pValue, *(char**)(pPropInfo->propAddress), nDataSize);
				}
				else ((char*)pValue)[0] = '\0';
				
				
				break;
			case tmUSTR:
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
			    {
    				//(char*)pValue = *(char**)(pPropInfo->propAddress);
    				if (*(char**)(pPropInfo->propAddress) != NULL)
    				{
    					if (nDataSize == -1)
    						strcpy((char*)pValue, *(char**)(pPropInfo->propAddress));
    					else
    						strncpy((char*)pValue, *(char**)(pPropInfo->propAddress), nDataSize);
    				}
    				else ((char*)pValue)[0] = '\0';
				}
				else
				{
				    WORD    *pszSrc, *pszDst;
				    
				    pszSrc  = (WORD *)(*(char**)(pPropInfo->propAddress));
				    pszDst  = (WORD *)pValue;
				    
				    if (pszSrc != NULL)
				    {
				        if(nDataSize == -1)
				        {
				            while(*pszSrc != 0) *pszDst ++ = *pszSrc ++;
				            *pszDst = 0;
				        }
				        else
				        {
				        	//	fans add 2007/9/25 03:24下午
				        	int	nStrNum = 2;
				        	while (*pszSrc++ != 0)
				        	{
				        		nStrNum+=2;
				        	}
				        	//while(*pszSrc != 0);
				        	
				        	if (nDataSize > nStrNum) nDataSize = nStrNum;
				        	pszSrc  = (WORD *)(*(char**)(pPropInfo->propAddress));
				            memcpy(pszDst, pszSrc, nDataSize);
				        }
				    }
				    else
				        *pszDst = 0;
				}
				break;
			default:
				Result = false;
				break;
		}
	}
	return Result;	
}

/*---------------------------------------------------------------------------+
|  Function : GetPropValueT(int nNub, void* pValue, int nDataSize = -1)		 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
bool CtmWnd::GetPropValueT(int nNub, void* pValue, int nDataSize)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(nNub);
	if (pPropInfo != NULL)
	{
		Result = true;
		if (nDataSize > 0) memset(pValue, 0, nDataSize);
		switch (pPropInfo->ptype)
		{
			case tmBYTE:				
					*(BYTE*)pValue = *(BYTE*)(pPropInfo->propAddress);					
				break;
			case tmINT:
				if (nDataSize == -1 || nDataSize >= sizeof(int))
					*(int*)pValue = *(int*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmSHORT:
				if (nDataSize == -1 || nDataSize >= sizeof(short))
					*(short*)pValue = *(short*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmLONG:
				if (nDataSize == -1 || nDataSize >= sizeof(long))
					*(long*)pValue = *(long*)(pPropInfo->propAddress);
				else 
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmINT64:
				if (nDataSize == -1 || nDataSize >= sizeof(long long))
					*(long long*)pValue = *(long long*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmWORD:
				if (nDataSize == -1 || nDataSize >= sizeof(WORD))
					*(WORD*)pValue = *(WORD*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmDWORD:
				if (nDataSize == -1 || nDataSize >= sizeof(DWORD))
					*(DWORD*)pValue = *(DWORD*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmBOOL:
				if (nDataSize == -1 || nDataSize >= sizeof(WORD))
					*(BOOL*)pValue = *(BOOL*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);	
				}
				break;
			case tmFLOAT:
				if (nDataSize == -1 || nDataSize == sizeof(float))
					*(float*)pValue = *(float*)(pPropInfo->propAddress);
				else if (nDataSize == sizeof(double))
				{
					*(double*)pValue = *(float*)(pPropInfo->propAddress);
				}
				else if  (nDataSize == 1)
				{
					*(BYTE*)pValue = static_cast<BYTE>(*(float*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 2)
				{
					*(short*)pValue = static_cast<short>(*(float*)(pPropInfo->propAddress));
				}
				else Result = false;
				break;
			case tmDOUBLE:
				if (nDataSize == -1 || nDataSize == sizeof(double))
					*(double*)pValue = *(double*)(pPropInfo->propAddress);
				else if (nDataSize == sizeof(float))
				{
					*(float*)pValue = static_cast<float>(*(double*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 1)
				{
					*(BYTE*)pValue = static_cast<BYTE>(*(double*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 2)
				{
					*(short*)pValue = static_cast<short>(*(double*)(pPropInfo->propAddress));
				}
				else Result = false;
				break;
			case tmSTR:
			case tmPROCEDURE:
			    
				//(char*)pValue = *(char**)(pPropInfo->propAddress);
				if (*(char**)(pPropInfo->propAddress) != NULL)
				{
					if (nDataSize == -1)
						strcpy((char*)pValue, *(char**)(pPropInfo->propAddress));
					else
						strncpy((char*)pValue, *(char**)(pPropInfo->propAddress), nDataSize);
				}
				else ((char*)pValue)[0] = '\0';
				
				
				break;
			case tmUSTR:
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
			    {
    				//(char*)pValue = *(char**)(pPropInfo->propAddress);
    				if (*(char**)(pPropInfo->propAddress) != NULL)
    				{
    					if (nDataSize == -1)
    						strcpy((char*)pValue, *(char**)(pPropInfo->propAddress));
    					else
    						strncpy((char*)pValue, *(char**)(pPropInfo->propAddress), nDataSize);
    				}
    				else ((char*)pValue)[0] = '\0';
				}
				else
				{
				    WORD    *pszSrc, *pszDst;
				    
				    pszSrc  = (WORD *)(*(char**)(pPropInfo->propAddress));
				    pszDst  = (WORD *)pValue;
				    
				    if (pszSrc != NULL)
				    {
				        if(nDataSize == -1)
				        {
				            while(*pszSrc != 0) *pszDst ++ = *pszSrc ++;
				            *pszDst = 0;
				        }
				        else
				        {
				        	//	fans add 2007/9/25 03:24下午
				        	int	nStrNum = 2;
				        	while (*pszSrc++ != 0)
				        	{
				        		nStrNum+=2;
				        	}
				        	//while(*pszSrc != 0);
				        	
				        	if (nDataSize > nStrNum) nDataSize = nStrNum;
				        	pszSrc  = (WORD *)(*(char**)(pPropInfo->propAddress));
				            memcpy(pszDst, pszSrc, nDataSize);
				        }
				    }
				    else
				        *pszDst = 0;
				}
				break;
			default:
				Result = false;
				break;
		}
	}
	return Result;	
}
