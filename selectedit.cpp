/*==========================================================================+
|  Class    : Select Edit		                                            |
|  Task     : Select Edit source file                        				|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : Leo                                                           |
|  Version  : V1.00                                                         |
|  Creation : 2006/7/12                                                    	|
|  Revision : 			                                                    |
+==========================================================================*/

#include    "selectedit.h"
#include	"init.h"
#include    "utils.h"

#include	"commonaction.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

	extern	BOOL g_bFocusChanged;
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/

/*==========================================================================+
|           Class implementation - CtmSelectEdit                            |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmSelectEdit, CtmWnd)
CtmSelectEdit::CtmSelectEdit(tmWND* pwnd,tmSELECTEDIT* pselectedit) : CtmWnd()
{

}

CtmSelectEdit::CtmSelectEdit(): CtmWnd()
{
	strcpy(TYPEName, "CtmSelectEdit");

	m_nIndex  				= 0;
	m_nCount 				= 0;
	m_bOnFocus    			= m_bChanged = FALSE;
	m_szString[0] 			= '\0';
	m_szString[1] 			= 0;
	m_psz         			= m_szString;
	m_nCol        			= 0;
	m_selectedit.wType 		= 0;
	m_wType 				= 0;
	m_selectedit.pszPrompt 	= NULL;
	m_pszPromptID 			= NULL;
	for (int i = 0; i < MAX_STRING_COUNT; i++)
	{
		m_pszShow[i] 		= NULL;
		m_pszIDShow[i] 		= NULL;
		m_pszStream[i] 		= NULL;
		m_wszStreamLen[i]	= 0;
		m_wStreamType[i]	= 0;
		m_pszImagePath[i] = NULL;
	}
	//m_pcstring        = new CStrings();
	m_fgc 			= m_wndattr.fgcEdit1;
	m_bgc 			= m_wndattr.bgcEdit1;
	m_pszDBID[0] 	= NULL;
	m_iPrivilege 	= 0;
	m_iStartVaule 	= 0;
	m_bgcOld 		= 0;
	m_nOldIndex 	= -1;
	memset(m_pszOldIDShow, 0, sizeof(m_pszOldIDShow));
	memset(m_cszOldStream, 0, sizeof(m_cszOldStream));
	m_bSightless 	= TRUE;
	m_bBackGround 	= FALSE;
	m_bUpdate		= FALSE;
	m_bgcOnFocus	= 52989;//52833;	//	fans for test
	
	m_wListNum		= 0;
	m_bList			= FALSE;
	
	m_wMemType		= MEMO_TYPE_PLAIN_TEXT;
	m_dwKeyColor	= 0;
	m_pImage		= NULL;
	
	for (int i = 0; i < CNT_INT_MAX_DB_NUM; i++)	m_pszDBID[i] = NULL;
	m_nInterval = 1;	
	
	m_nBmpWidth			= 0;
	m_bCalcSize			= FALSE;
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
	else
		m_iFontUNType		= 0;
	strcpy(m_memo.Name, "TST");
    
}
	

CtmSelectEdit::~CtmSelectEdit()
{
	//printf("selecteEdit memo.parent=%s\n", Name);
	m_memo.SetParent(NULL);
	if (m_pImage)
	{
		m_pImage->SetParent(NULL);
		delete m_pImage;
	}
}
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :                Get()                                           |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmSelectEdit::Get()				/* Virtual function */
{
    DBVIEWVALUE dbViewValue; 
	
	if (!m_bOnFocus)
	{
		if (m_pszDBID[0] != NULL )
		{
			if(g_pDatabase != NULL && g_pDatabase->GetIndex(m_pszDBID[0]) != -1)
			{
				dbViewValue = GetDBValue(m_pszDBID[0]);
				m_nIndex = dbViewValue.lValue - m_iStartVaule;
				if ((m_nIndex >= MAX_STRING_COUNT) || m_nIndex < 0) m_nIndex = 0;
				m_iPrivilege = dbViewValue.dwPrivilege;
			}
		}
	}
	
	if((m_nIndex < 0) || (m_nIndex >= m_nCount))  return;
	if(m_pszStream[m_nIndex] == NULL)
    {	
    	m_psz = m_pszShow[m_nIndex];
    }
	//m_psz = m_pcstring->GetString(m_nIndex);
}

BOOL 	CtmSelectEdit::CreateA()
{

	STRINGSINFO stringinfo;
	short       i, nCurrent, nMaxLen = 0;

	SetBorderWidth(m_wndattr.wBorderWidthSelectEdit);
	CtmWnd::CreateA();
		
	if ((m_wType & tmEDIT_DISABLE) || (GetTabOrder() == -2))
	{
		
		m_fgc = m_wndattr.fgcEdit2;
		m_bgc = m_wndattr.bgcEdit2;	
	}
	
	SetFGColor(m_fgc);
    SetBGColor(m_bgc);
   
	m_bgcLoseFocus = m_bgc;
	
	StrIDToStr(m_pszPromptID, &m_selectedit.pszPrompt);
	
	
	
//	m_pselectedit = &m_selectedit;
	m_wMaxChar    = m_selectedit.wMaxChar;
	//m_wType  = m_selectedit.wType;
    nMaxLen = 0;
    
    
    char* pszTemp = NULL;
    int iLen = 0;
    for (i = 0; i < MAX_STRING_COUNT; i++)
    {
    	iLen = 0;
    	StrIDToStr(m_pszIDShow[i], &m_pszShow[i]);
    	if (m_pszShow[i] != NULL)
    	{   		
    		iLen = strlen(m_pszShow[i]);
    		if (iLen > nMaxLen) nMaxLen = iLen;
    	}
    }
//    
    stringinfo.nCount = MAX_STRING_COUNT;
    if(nMaxLen > 0) stringinfo.nSize  = stringinfo.nCount * nMaxLen;
    else            stringinfo.nSize  = stringinfo.nCount * (tmMAX_SEDIT_COL + 1);
    //m_pcstring->Create(&stringinfo);   	//  fans
    int Count = 0;							//	yangpeng 2007/5/17 08:55上午
    
  
    for (i = 0; i < MAX_STRING_COUNT; i++)
    {
    	if (m_pszShow[i] != NULL)
    	//if (m_pszShow[i] != NULL && *(WORD*)m_pszShow[i] != 0)
    	{
    		CheckCharacter(m_pszShow[i]);    		
    		//m_pcstring->AddString(m_pszShow[i]);  //	fans 
    		Count++; 								//	yangpeng 2007/5/17 08:55上午
    	}
    	else break;
    }
    if(m_nCount == 0) 	m_nCount = Count;			//	如果沒有指定字串個數,默認為最大的字串個數.yangpeng 2007/5/17 08:55上午
    
 
    
    if (m_nCount > 0)  m_nIndex = 0;
    DBVIEWVALUE dbViewValue; 
	
	if (m_pszDBID[0] != NULL)
	{
		//char pszTemp[256];
		//strcspn(m_pszDBID[0], " ");
		dbViewValue = GetDBValue(m_pszDBID[0]);
		m_nIndex = dbViewValue.lValue - m_iStartVaule;
		if ((m_nIndex >= MAX_STRING_COUNT) || m_nIndex < 0) m_nIndex = 0;
		//	需要獲取權限
		m_iPrivilege = dbViewValue.dwPrivilege;
	}
    //m_nOldIndex = m_nIndex;
    Set();
    
    
    if(CtmConfig::GetInstance()->GetBinaryType() == 1)
    	m_memo.SetPropertys_Nub();
    else
    	m_memo.SetPropertys();
    //=========================================
	//	set the memolist's style
	//=========================================
	WORD	wStyle = tmWS_REDRAW | tmWS_FRAMEWND | tmWS_SHOW;
	m_memo.SetPropValueT("wtype",	wStyle);
    //=========================================
	//	set the memolist's font
	//=========================================
	m_memo.SetPropValueT("fontsize",	m_iFontType);
	
	//=========================================
	//	set the memolist's bgc & fgc
	//=========================================
	m_memo.SetPropValueT("fgc",	m_fgc);
	m_memo.SetPropValueT("bgc",	m_bgcOnFocus);
	
	WORD	wHeight = 0;
	if(m_iFontUNType >= 1)	
		wHeight = m_iFontUNHeight ;//+ 2;
	else
		wHeight = m_pcFont->m_wHeight;	
	
	//if (m_paragraph.wAlign & tmPARA_ALIGN_CENTERY)
	//{		
		m_nOffSet = (GetClientRECT().bottom - GetClientRECT().top - wHeight) / 2;
		//printf("m_nOffSet = %d\n",m_nOffSet);
	//}
	//else m_nOffSet=0;
    return TRUE;
}

void 	CtmSelectEdit::SetPropertys()
{
	char Name[10];
	char temp[3];
	CtmWnd::SetPropertys();

	//SetPropInfo("dbid0",		tmSTR,	&m_pszDBID[0]);
	char sz[10];
	for (int i = 0; i < CNT_INT_MAX_DB_NUM; i++)
	{
		sprintf(sz, "%s%d", "dbid", i);
		SetPropInfo(sz,		tmSTR, 		&m_pszDBID[i]);	
	}
	
	SetPropInfo("startvalue", 	tmINT,	&m_iStartVaule);
	SetPropInfo("maxindex",		tmWORD, &m_nCount);			//yangpeng 2007/5/17 09:22上午
	SetPropInfo("align",	tmWORD, &m_paragraph.wAlign);
	for (int i = 0; i < MAX_STRING_COUNT; i++)
	{
		temp[0] = '\0';
		strcpy(Name, "str");
		sprintf(temp, "%d", i);
		strcat(Name, temp);
		
		SetPropInfo(Name, 	tmUSTR,	&m_pszShow[i]);
	
		strcpy(Name, "strID");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_pszIDShow[i]);
		
		strcpy(Name, "stream");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmFontStream,	&m_pszStream[i]);
		
		strcpy(Name, "imagepath");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_pszImagePath[i]);
	}
	
	SetPropInfo("maxchar", 		tmWORD,		&m_selectedit.wMaxChar);
	SetPropInfo("stype", 		tmWORD,		&m_wType);
	SetPropInfo("prompt", 		tmUSTR,		&m_selectedit.pszPrompt);
	SetPropInfo("promptID",		tmSTR,		&m_pszPromptID);
	SetPropInfo("focuscolor",	tmDWORD,	&m_bgcOnFocus);
	SetPropInfo("listnum", 		tmWORD,		&m_wListNum);
	SetPropInfo("memtype",		tmWORD, 	&m_wMemType);
	SetPropInfo("keycolor",		tmLONG, 	&m_dwKeyColor);
	SetPropInfo("button-fgc",	tmDWORD,	&m_triColor);
	SetPropInfo("button-bgc",	tmDWORD,	&m_btnColor);
}


void 	CtmSelectEdit::SetPropertys_Nub()
{
	char Name[10];
	char temp[3];
	CtmWnd::SetPropertys_Nub();

	for (int i = 0; i < CNT_INT_MAX_DB_NUM; i++)
	{
		SetPropInfo(SELECTEDIT_DBID0 + i,		tmSTR, 		&m_pszDBID[i]);	
	}
	
	SetPropInfo(SELECTEDIT_STARTVALUE, 		tmINT,	&m_iStartVaule);
	SetPropInfo(SELECTEDIT_MAXINDEX,		tmWORD, &m_nCount);			//yangpeng 2007/5/17 09:22上午
	for (int i = 0; i < MAX_STRING_COUNT; i++)
	{
		SetPropInfo(SELECTEDIT_STR0 + 2*i, 	tmUSTR,	&m_pszShow[i]);
		SetPropInfo(SELECTEDIT_STRID0 + 2*i, tmSTR,	&m_pszIDShow[i]);
		
	}
	
	SetPropInfo(SELECTEDIT_MAXCHAR, 		tmWORD,		&m_selectedit.wMaxChar);
	SetPropInfo(SELECTEDIT_STYPE, 			tmWORD,		&m_wType);
	SetPropInfo(SELECTEDIT_PROMPT, 			tmUSTR,		&m_selectedit.pszPrompt);
	SetPropInfo(SELECTEDIT_PROMPTID,		tmSTR,		&m_pszPromptID);
	SetPropInfo(SELECTEDIT_FOCUSCOLOR,		tmDWORD,	&m_bgcOnFocus);
	SetPropInfo(SELECTEDIT_LISTNUM, 		tmWORD,		&m_wListNum);

}

int  CtmSelectEdit::GetIndex()
{
    return  m_nIndex;
}

void CtmSelectEdit::Set()				/* Virtual function */
{
    Get();
    if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
    {
    	if(m_iFontUNType > 0)
    	{
    		//printf("select set \n");;	
    		;//strcpy(m_szString,m_psz[m_nCol]);
    	}
    	else
    	{
	        int nStrLen = 0;
	        WORD    *pwSrc, *pwDst;
	        pwDst   = (WORD *)m_szString;
	        pwSrc   = (WORD *)&m_psz[m_nCol];
	        while(pwSrc != 0 && *pwSrc != 0)
	        {
	            *pwDst ++   = *pwSrc ++;
	            nStrLen += 2;
	            if(nStrLen + 2 >= m_wMaxChar)   break;
	        }
	        *pwDst  = 0;
	    }
    }
    else
    {    
        if(strlen(m_psz) > m_wMaxChar)
        {
            StrNCpy(m_szString, &m_psz[m_nCol], m_wMaxChar);
        }
        else    strcpy(m_szString, m_psz);
    }
}

void    CtmSelectEdit::Clear()
{
	if (m_wnd.wStyle & tmWS_TRANSPARENT) 
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
|  Function :                Show()                                          |
|  Task     : Create the items and show them                                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmSelectEdit::Show()				/* Virtual function */
{
	
	CalcSize();
	int nLeft = 1;
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
	else
		m_iFontUNType		= 0;
	if (m_wMemType & MEMO_TYPE_IMAGE)
		nLeft = m_nBmpWidth;
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
		m_bSightless = FALSE;
		if (!m_bEnabled)
		{
			m_bgcOld = m_bgc;
			m_bgc = m_wndattr.bgcEdit2;	
		}
		
		DrawBorder();
		int iLen = 0, nMaxLen = 0;
	    for (int i = 0; i < MAX_STRING_COUNT; i++)
	    {
	    	iLen = 0;
	    	StrIDToStr(m_pszIDShow[i], &m_pszShow[i]);
	    	if (m_pszShow[i] != NULL)
	    	{ 		
	    		iLen = strlen(m_pszShow[i]);
	    		if (iLen > nMaxLen) nMaxLen = iLen;
	    	}
	    }
	    	 
	    //James modify  add strcmp(m_pszIDShow[m_nIndex], m_pszOldIDShow)  為了刷新字串
	    if(m_pszIDShow[m_nIndex] != NULL && m_pszOldIDShow != NULL)
	    {
	    	if(0 != strcmp(m_pszIDShow[m_nIndex], m_pszOldIDShow))
	    		m_bUpdate = TRUE;
	    }
	    else if(m_cszOldStream != NULL && m_pszStream[m_nIndex] != NULL)		//rw.ying 2011/3/25 05:49下午
	    {
	    	if(0 != memcmp(m_pszStream[m_nIndex], m_cszOldStream, m_wszStreamLen[m_nIndex]));
	    		m_bUpdate = TRUE;
	    }
	    if (m_nOldIndex != m_nIndex || m_bUpdate)
		{
			m_nOldIndex = m_nIndex;
			if(m_pszIDShow[m_nIndex] != NULL)
				strcpy(m_pszOldIDShow, m_pszIDShow[m_nIndex]);
			if(m_pszStream[m_nIndex] != NULL && m_wszStreamLen[m_nIndex] < sizeof(m_cszOldStream))		//rw.ying
				memcpy(m_cszOldStream, m_pszStream[m_nIndex], m_wszStreamLen[m_nIndex]);
			//CtmWnd::Clear();	// <FANS> 2006/10/16 05:47下午 James modify 2008/4/10 03:49下午
			Clear();
			
			if (m_wMemType & MEMO_TYPE_PLAIN_TEXT)
			{
				if(m_iFontUNType > 0)
				{
					//CtmWnd::Show(nLeft, 2, m_psz);
					//J.Wong Add for freetype 2015/5/28 8:50:12
					  if(m_pszShow[m_nIndex]!=NULL)
	   		 				m_psz=m_pszShow[m_nIndex];
					CtmWnd::ShowExtent(m_nOffSet,m_psz);
				}
				else
					CtmWnd::Show(nLeft, 2, m_szString);
			}
			else if(m_wszStreamLen[m_nIndex] == 0 && m_pszStream[m_nIndex] == NULL && m_wMemType & MEMO_TYPE_PLAIN_TEXT)
			{
				if(m_iFontUNType > 0)
				{
					//CtmWnd::Show(nLeft, 2, m_psz);
					//J.Wong add for freetype 2015/5/28 8:50:32
					printf("m_nOffSet = %d\n",m_nOffSet);
					CtmWnd::ShowExtent(m_nOffSet,m_psz);
				}
				else
					CtmWnd::Show(nLeft, 2, m_szString);//CtmWnd::Show(1, 2, m_szString);
			}
			else
				CtmWnd::ShowFontStream(1, 2, m_pszStream[m_nIndex], m_wszStreamLen[m_nIndex], m_wStreamType[m_nIndex]);
			if(m_nIndex >= 0)
			{
	    		if(m_nIndex == m_nCount - 1)    DrawRightButton(RIGHT_BUTTON_UP);
	    		else if(m_nIndex == 0)          DrawRightButton(RIGHT_BUTTON_DOWN);
	    		else                            DrawRightButton(RIGHT_BUTTON_ARROW);
	    		
	    	}
	    	
	    	ShowImage(m_nIndex);
	    	
	    	CtmWnd::Show();
	    	m_bUpdate = FALSE;
	    }
	    
    		
    	if (!m_bEnabled)
			m_bgc = m_bgcOld;
	}
	else
	{
		m_nOldIndex = -1;
		memset(m_pszOldIDShow, 0, sizeof(m_pszOldIDShow));
		DrawBorder();
		if (!m_bSightless)
		{
			
			if (m_bgcOld == 0)
				m_bgcOld = m_bgc;
			
	 		if (GetParent() != NULL) 
	 			m_bgc = GetParent()->GetBGColor();
	 		
	 		CtmWnd::SetClientRECT(-1, 0, 1, 0);       // Restore the area for vertical cursor
	 		//CtmWnd::Clear();							//James modify 2008/4/10 03:49下午
	 		Clear();
	 		CtmWnd::SetClientRECT(1, 0, -1, 0);       // Make room for vertical cursor	
	 		m_bgc = m_bgcOld;
 		
 			m_bSightless = TRUE;
 			
 			//	Fans add 2008/10/10 09:19上午
 			DisableList();
 		}
	}

	ChangePosNow(GetClientRECT());
}

CtmBitMap*	CtmSelectEdit::GetBitMap(char *imagePath)
{
	int i = 0;
	bool bFind = FALSE;
	CtmBitMap *pBitMap = NULL;
	
	while (g_aIniBmp[i].cBMPPath[0] != '\0')
	{
		if (strcmp(g_aIniBmp[i].cBMPPath, imagePath) == 0)
		{
			pBitMap = g_aIniBmp[i].pIniBmp; 
			bFind = TRUE;
			
			break;
		}
		i++;
	}
	if (!bFind)
	{
		g_aIniBmp[i].pIniBmp = new CtmBitMap;
		g_aIniBmp[i].pIniBmp->Create(imagePath);
		strncpy(g_aIniBmp[i].cBMPPath, imagePath, 256);

		pBitMap = g_aIniBmp[i].pIniBmp; 
	}
	
	return pBitMap;
}

void	CtmSelectEdit::CalcSize()
{
	int nWidth = 1;
	int nHeight = 0;
	CtmBitMap *pBitMap = NULL;
	
	if (m_bCalcSize)
		return;
	
	for (int i = 0; i < MAX_STRING_COUNT; i++)
	{
		if (m_pszImagePath[i] != NULL)
		{
			pBitMap = GetBitMap(m_pszImagePath[i]);
			
			if (nWidth < pBitMap->GetWidth())
				nWidth = pBitMap->GetWidth();
			if (nHeight < pBitMap->GetHeight())
				nHeight = pBitMap->GetHeight();
		}
	}
	
	if (nHeight < MEMO_UNIT_HEIGHT)
		nHeight = MEMO_UNIT_HEIGHT;
		
	m_nBmpWidth = nWidth;
	m_nBmpHeight = nHeight;
	
	m_bCalcSize = TRUE;
	//printf("width:%d\n", m_nBmpWidth);
}


void	CtmSelectEdit::ShowImage(int nIndex)
{
	CalcSize();
	
	if (m_wMemType & MEMO_TYPE_IMAGE)
	{
		if (!m_pImage)
		{
			m_pImage = new CtmImageBox();
		
			m_pImage->SetPropertys();
			m_pImage->SetParent(this);
		
			m_pImage->SetPropValueT("style", 3);
			m_pImage->SetPropValueT("type", 2);
			m_pImage->SetPropValueT("wtype", 0x30);
			m_pImage->SetPropValueT("keycolor", m_dwKeyColor);
			m_pImage->SetPropValueT("left", (double)0);
			m_pImage->SetPropValueT("right", m_nBmpWidth);
			m_pImage->SetPropValueT("top", (double)0);
			m_pImage->SetPropValueT("bottom", GetClientHeight() - 1);
		
			m_pImage->CreateA();
		}
		
		m_pImage->SetPropValueT("imagepath", m_pszImagePath[nIndex]);
		m_pImage->Update();
	}
}
void CtmSelectEdit::Update()			/* Virtual function */
{
	int Count = 0;
    int iLen = 0,nMaxLen = 0;
    
	for (int i = 0; i < MAX_STRING_COUNT; i++)
    {
    	iLen = 0;
    	StrIDToStr(m_pszIDShow[i], &m_pszShow[i]);
    	//printf("m_pszShow[%d]=%s\n",i,m_pszShow[i]);
    	if (m_pszShow[i] != NULL)
    	{   		
    		iLen = strlen(m_pszShow[i]);
    		if (iLen > nMaxLen) nMaxLen = iLen;
    	}
    }
    
	for (int i = 0; i < MAX_STRING_COUNT; i++)
    {
    	if (m_pszShow[i] != NULL)
    	{
    		CheckCharacter(m_pszShow[i]);    		
    		//m_pcstring->AddString(m_pszShow[i]);  //	fans 
    		Count++;			// 	yangpeng 2007/5/17 08:59上午
    	}
    }
    if(m_nCount == 0) 	m_nCount = Count; 
	//if (m_iPrivilege > g_iPrivilege) return;
		
	if (!m_bOnFocus || m_bChanged)
    {
        m_bChanged = FALSE;
        //if(m_wType & tmSEDIT_DISABLE)
        //{
        //    SetFGColor(m_wndattr.fgcEdit2);
        //    SetBGColor(m_wndattr.bgcEdit2);
        //}
        //Clear();
	    Set();
	    Show();
		
    }
    else
    {   
	    Set();
			m_bUpdate = TRUE;
			Show();
    }

}

void CtmSelectEdit::UpdateAll()
{
	int Count = 0;
    int iLen = 0,nMaxLen = 0;
	for (int i = 0; i < MAX_STRING_COUNT; i++)
    {
    	iLen = 0;
    	StrIDToStr(m_pszIDShow[i], &m_pszShow[i]);
    	if (m_pszShow[i] != NULL)
    	{   		
    		iLen = strlen(m_pszShow[i]);
    		if (iLen > nMaxLen) nMaxLen = iLen;
    	}
    }
    
	for (int i = 0; i < MAX_STRING_COUNT; i++)
    {
    	if (m_pszShow[i] != NULL)
    	{
    		CheckCharacter(m_pszShow[i]);    		
    		//m_pcstring->AddString(m_pszShow[i]);  //	fans 
    		Count++;			// 	yangpeng 2007/5/17 08:59上午
    	}
    }
    if(m_nCount == 0) 	m_nCount = Count; 

	m_bUpdate = TRUE;
  Set();
	Show();
}
/*---------------------------------------------------------------------------+
| Function  : void OnGetFocus()							                     |
| Task	  : 					 								             |
+----------------------------------------------------------------------------+
|  Call     : 	                                  						     |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmSelectEdit::OnGetFocus()		/* Virtual function */
{
	char	szPrompt[256];
	long	lDataID = 0;
	m_bOnFocus = TRUE;
	/* if(m_wType  ==  tmWS_SEDIT_FILL)
	{
	     SetBGColor(m_wndattr.bgcSelectEdit2);
	    SetFGColor(m_wndattr.fgcSelectEdit2); 
	    Clear();
	    Show();
	}
	else */	
	{
		//	fans add 2008/4/17 02:33下午
		m_bgcLoseFocus 	= m_bgc;
		m_bgc 			= m_bgcOnFocus;//	equal the m_bgc = 52833		
		m_nOldIndex 	= -1;
		
	    CtmWnd::DrawGetFocus();
	    Update();
	}
	if (m_selectedit.pszPrompt != NULL)
	{
		if(g_bDemoDataID)
		{
			memset(szPrompt, 0, sizeof(szPrompt));
			
			if(m_pszDBID[0] != NULL)
			{
				lDataID  = g_pDatabase->GetDataID(m_pszDBID[0]);
			}
			if(CtmConfig::GetInstance()->GetLanguageType()==1)
				tm_sprintf(szPrompt, "%x %t", lDataID, g_MultiLanguage[m_selectedit.pszPrompt]); 
			else
				tm_sprintf(szPrompt, "%x %t", lDataID, m_selectedit.pszPrompt); 
			Prompt(szPrompt);
		}
		else
		{
			if(CtmConfig::GetInstance()->GetLanguageType()==1)
				Prompt(g_MultiLanguage[m_selectedit.pszPrompt]);
			else
				Prompt(m_selectedit.pszPrompt);
		}
	}
}

/*---------------------------------------------------------------------------+
| Function  : void OnLoseFocus()							          		 |
| Task	  : 					 											 |
+----------------------------------------------------------------------------+
|  Call     : 	                                  							 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmSelectEdit::OnLoseFocus()		/* Virtual function */
{

	m_bOnFocus = FALSE;
	/* if(m_wType == tmWS_SEDIT_FILL)
	{
	    SetBGColor(m_wndattr.bgcSelectEdit1);
	    SetFGColor(m_wndattr.fgcSelectEdit1);
    }
    else    */ CtmWnd::DrawLoseFocus();
    	
    if (m_pszDBID[0] != NULL)
    {
    	if(g_bFocusChanged&0xF0||!g_bFocusChanged)
    	SetViewDBValue(m_pszDBID[0], m_nIndex + m_iStartVaule);
	}
	m_nCol = 0;
	
	//	fans add 2008/4/17 02:37下午
	m_bgc 		= m_bgcLoseFocus;
	m_nOldIndex = -1;
	
    Update();
	
	DisableList();
}

/*---------------------------------------------------------------------------+
| Function  : WORD OnKey() 								          			 |
| Task	  : deal with the Key press message									 |
+----------------------------------------------------------------------------+
|  Call     : 	                                  							 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/

WORD 	CtmSelectEdit::OnKey(WORD wKey)		/* Virtual function */
{
	WORD	wResult = wKey;
	
	//printf("m_wListNum = %d\n",m_wListNum);
	//printf("m_bList = %d\n",m_bList);
	if (m_wListNum >= 1)
	{
		if (m_bList)
		{
			switch (wKey)
			{
				case _ENTER:
					m_memo.GetPropValueT("foucusstrIndex", &m_nIndex);
					//(nIndex >=0) && (nIndex < m_nCount)
					if (m_nIndex >= m_nCount) m_nIndex = m_nCount - 1;
					
					DisableList();
					m_bChanged = TRUE;
					Update();		
					wResult = _NULL_KEY;
					_PutKeyTail(_ENTER);
					break;
				case _KEY_CLEAR:
					DisableList();
					break;
				default:
					wResult = OnKey_List(wKey);
					break;
			}
				
		}
		else
		{
			switch (wKey)
			{
				case 46://
					//printf("46\n");
					EnableList();
					wResult	= _NULL_KEY;
					break;
				default:
					break;
			}
			
		}
	}
	else
	{
		short   nKeyFlag    = -1;

    
	    switch (wKey)	
	    {
	    	case 46://_DOT:
	    		if(m_nIndex >= m_nCount - 1)   m_nIndex = 0;
		        else                           
		        {
		        	//printf("index =%d\n", m_nIndex);
		        	m_nIndex ++;
		        }
		        if (m_nIndex < 0) m_nIndex = 0;
		        nKeyFlag = 1;
	    		break;
	    	default:
	    		break;
	    }
	 
	    if((nKeyFlag == 1) || (nKeyFlag == 2))
	    {
	        if(nKeyFlag == 1)   m_nCol= 0;
	        m_bChanged = TRUE;
	        Update();
	        wResult = _NULL_KEY;
	    }
	}
	
	return wResult;
	
}


void	CtmSelectEdit::EnableList()
{
	if (!m_bList)
	{
		BOOL	bCanStop = FALSE;
		
		if (m_parent != NULL)
			m_parent->GetPropValueT("canstop", &bCanStop);
		if (bCanStop)
		{
			BOOL	bUpdate = FALSE;
			m_parent->SetPropValueT("privateupdate", (double)bUpdate);
		}
		
		InitListMemo();
		ShowList();
		m_bList = TRUE;
	}

}


void	CtmSelectEdit::DisableList()
{
	
	if (m_bList)
	{
		//=========================================
		//	set the memolist visible
		//=========================================
		BOOL	bVisible = FALSE;
		m_memo.SetPropValueT("visible",	(double)bVisible);
		m_memo.Update();
		
		
		//	fans add  2009/3/17 01:59下午
		//=========================================
		//	
		//=========================================
		BOOL	bCanStop = FALSE;		
		if (m_parent != NULL) m_parent->GetPropValueT("canstop", &bCanStop);
		if (bCanStop)
		{
			BOOL	bUpdate = TRUE;
			m_parent->SetPropValueT("privateupdate", (double)bUpdate);
		}
		
		//=========================================
		//	set the memolist's parent 
		//=========================================
		m_memo.SetParent(NULL);
		m_bList	= FALSE;
		g_MemoCurCoordinate.nTop=-1;
		g_MemoCurCoordinate.nRight=-1;
		g_MemoCurCoordinate.nBottom=-1;
		g_MemoCurCoordinate.nLeft=-1;	
		
		
	}
		
}


void	CtmSelectEdit::ShowList()
{
	//printf("ShowList\n");
	if (!m_bList)
		m_memo.Show();
}

void	CtmSelectEdit::InitListMemo()
{
	CalcSize();
	int		nCount = 0;
	for(int i = 0; i < 20; i++)
	{
		if(m_wszStreamLen[i] != 0)
			nCount++;
	}
	if(nCount != 0)
		m_nCount = nCount;
	//=========================================
	//	set the memolist's parent and register the its propertys
	//=========================================
	
	m_memo.SetParent(m_parent);
	
	//=========================================
	//	set the memolist's position and the size
	//=========================================
	short int nLeft 	= 0;
	short int nTop		= 0;
	short int nRight	= 0;
	short int nBottom	= 0;
	nLeft	= m_wnd.rect.left - 3;
	nTop	= m_wnd.rect.bottom + 3;
	nRight	= m_wnd.rect.right + 3;
	nBottom	= nTop + m_wListNum * m_nBmpHeight;//MEMO_UNIT_HEIGHT;
	
	//	not olny drawing down,	
	if (nBottom + m_parent->GetClientRECT().top > m_parent->GetClientRECT().bottom)
	{
		nBottom = m_wnd.rect.top - 3;
		nTop	= nBottom - m_wListNum * m_nBmpHeight;//MEMO_UNIT_HEIGHT;
	}
	
	m_memo.SetPropValueT("left", 	nLeft);
	m_memo.SetPropValueT("top", 	nTop);
	m_memo.SetPropValueT("right", 	nRight);
	m_memo.SetPropValueT("bottom",	nBottom);
	
	//=========================================
	//	set the memolist's style
	//=========================================
	WORD	wStyle = tmWS_REDRAW | tmWS_FRAMEWND | tmWS_SHOW | tmWS_BORDER_PLAIN;
	m_memo.SetPropValueT("wtype",	wStyle);
	
	
		
	BOOL	bVisible = TRUE;
	m_memo.SetPropValueT("visible",	(double)bVisible);
	
	
	
	//=========================================
	//	set the memolist's StringIDs
	//=========================================
	m_memo.SetFont(m_pcFont);
	char	szName[10], *cszTempNULL = NULL;
	for (int i = 0; i < CNT_INT_MEMO_MAX; i++)
	{
		sprintf(szName, "str%d", i);
		m_memo.SetPropValueT(szName, cszTempNULL);
		
		sprintf(szName, "strID%d", i);
		m_memo.SetPropValueT(szName, cszTempNULL);
		
		sprintf(szName, "stream%d", i);
		m_memo.SetPropValueT(szName, cszTempNULL, 0, 0);
		
		sprintf(szName, "imagepath%d", i);
		m_memo.SetPropValueT(szName, cszTempNULL);
	}
	
	m_memo.SetPropValueT("fontUNType", m_iFontUNType);
	m_memo.SetPropValueT("fontUNWidth", m_iFontUNWidth);
	m_memo.SetPropValueT("fontUNHeight", m_iFontUNHeight);
	
	for (int i = 0; i < m_nCount; i++)
	{
		if(m_pszShow[i] != NULL)
		{
			sprintf(szName, "str%d", i);
			m_memo.SetPropValueT(szName, m_pszShow[i]);
		
			sprintf(szName, "strID%d", i);
			m_memo.SetPropValueT(szName, m_pszIDShow[i]);
		}
		sprintf(szName, "imagepath%d", i);
		m_memo.SetPropValueT(szName, m_pszImagePath[i]);
	}
	for(int i = 0; i <m_nCount; i++)
	{
		if(m_pszStream[i] != NULL)
		{
			sprintf(szName, "stream%d", i);
			m_memo.SetPropValueT(szName, m_pszStream[i], m_wszStreamLen[i], 0);
		}
	}
	
	m_memo.SetPropValueT("memtype", m_wMemType);
	
	m_memo.SetPropValueT("keycolor", m_dwKeyColor);
	
	//=========================================
	//	set the memolist's StringIDs
	//=========================================
	WORD	wIndex 		= m_nIndex;
	WORD	wFocusIndex	= 0;
	
	if (m_nIndex + m_wListNum < m_nCount) 
	{
		wIndex		= m_nIndex;
		wFocusIndex	= 0;
	}
	else if (m_nCount < m_wListNum)
	{
		wIndex 		= 0;	
		wFocusIndex	= m_nIndex;
	}
	else
	{	
		if ((m_nCount - m_nIndex - 1) < m_wListNum)
		{
			wIndex 			= m_nCount - m_wListNum;
			wFocusIndex 	= m_nIndex - wIndex;
		}
		else
		{
			wIndex			= m_nIndex;
			wFocusIndex		= 0;
		}
		
	}
	
	if (wIndex < 0)			wIndex 		= 0;
	if (wFocusIndex < 0) 	wFocusIndex	= 0;
	
	m_memo.SetPropValueT("startstrindex", 	wIndex);
	m_memo.SetPropValueT("foucusindex", 	wFocusIndex);

	//=========================================
	//	execute the memo create function
	//=========================================
	m_memo.CreateA();
}



WORD	CtmSelectEdit::OnKey_List(WORD wKey)
{
	WORD	wResult = wKey;
	if (m_bList)
	{
		wResult = m_memo.OnKey(wKey);
	}
	
	return wResult;
}


int     CtmSelectEdit::AddString(char* psz)
{ 
	char    msz[tmMAX_TEXT_COL+1];                  //<<LEO20060721
	
    if((m_nCount < MAX_STRING_COUNT) && (strlen(psz) <= tmMAX_SEDIT_COL))
    {
	    strcpy(msz, psz);                           //<<LEO20060721
        CheckCharacter(msz);                        //<<LEO20060721
        //m_pcstring->AddString(psz);
        m_nCount ++;
        if(m_nIndex < 0) m_nIndex = 0;
        m_bChanged = TRUE;
        Update();
        return  0;
    }
    else    return  -1;
}

void CtmSelectEdit::CheckCharacter(char* pacCheck)          //<<LEO20060721
{
//    int     i = 0,j, nCount;
//    int     nLen = strlen(pacCheck);
//    char    cTemp, cWrite;
//    BOOL    bDeal = FALSE;
//
//    while(i < nLen)
//    {
//        if(((BYTE)pacCheck[i+m_wMaxChar-1] > 0x7F) || ((BYTE)pacCheck[i+m_wMaxChar-1] < 0x20))
//        {
//            nCount = 0;
//            for(j=m_wMaxChar-2; j>=0; j--)
//            {
//                if(((BYTE)pacCheck[i+j] <= 0x7F) && ((BYTE)pacCheck[i+j] >= 0x20))
//                {
//                    if(nCount%2 == 1)   bDeal = FALSE;
//                    else                bDeal = TRUE;
//                    break;
//                }
//                else if(j == 0)
//                {
//                    if(nCount%2 == 1)   bDeal = FALSE;
//                    else                bDeal = TRUE;
//                }
//                else    nCount++;
//            }
//            if(bDeal)
//            {
//                if(nLen >= tmMAX_TEXT_COL)  return;
//                cWrite = ' ';
//                for(j=(i+m_wMaxChar-1); j < (nLen+2); j++)
//                {
//                    cTemp       = pacCheck[j];
//                    pacCheck[j] = cWrite;
//                    cWrite      = cTemp;
//                }
//                nLen++;
//            }
//        }
//        i = i + m_wMaxChar;
//    }
}

int     CtmSelectEdit::AddString(WORD  szID)
{
    //return  AddString(g_pStr[szID]);
    return 0;
}

void    CtmSelectEdit::DrawRightButton(short nUpDown)
{
    short       nBottom = m_rectWindow.bottom - m_rectWindow.top;
    int         nOldColor;
    short       nWidth = m_wndattr.wBorderWidthSelectEdit;

    _SetViewPort(m_rectWindow.right - 15, m_rectWindow.top + nWidth, m_rectWindow.right - nWidth, m_rectWindow.bottom - nWidth);

    nOldColor = _SetColor(m_wndattr.fgcBorder4);
	if((m_wnd.wStyle&tmWS_UNDERLINE_BORDER)!=tmWS_UNDERLINE_BORDER)
		_Line(0, 0, 0, nBottom - nWidth*2);
    if(m_wType  ==  tmWS_SEDIT_FILL)
    {
       // if(m_bOnFocus)  _SetColor(m_btnColor);
       // else            _SetColor(m_btnColor);
	    _SetColor(m_btnColor);
        _Rectangle(_GFILLINTERIOR, 1, 0, m_rectWindow.right - nWidth, m_rectWindow.bottom - nWidth);
        //if(m_bOnFocus)  _SetColor(m_triColor);
        //else            _SetColor(m_triColor);
		 _SetColor(m_triColor);
    }
    else
    {
        _SetColor(m_triColor);
    }

    if(nUpDown == RIGHT_BUTTON_UP)
    {
        DrawFillTriangle(nBottom/2 + 2, 4, 4, TRUE);
/*
		_MoveTo(       3, nBottom/3);
		_LineTo(       7, nBottom*2/3);
		_LineTo(      11, nBottom/3);
		_LineTo(       3, nBottom/3);
*/
    }
    else if(nUpDown == RIGHT_BUTTON_DOWN)
    {
        DrawFillTriangle(nBottom/2 - 2, 4, 4, FALSE);
/*
		_MoveTo(       3, nBottom*2/3);
		_LineTo(       7, nBottom/3);
		_LineTo(      11, nBottom*2/3);
		_LineTo(       3, nBottom*2/3);
*/
    }
    else if(nUpDown == RIGHT_BUTTON_ARROW)
    {
        DrawFillTriangle(nBottom/2 - 2, 4, 4, TRUE);
        DrawFillTriangle(nBottom/2 + 1, 4, 4, FALSE);
/*
		_MoveTo(       3, nBottom/2);
		_LineTo(       7, nBottom/6);
		_LineTo(      11, nBottom/2);
		_LineTo(       3, nBottom/2);
		_MoveTo(       3, nBottom/2+1);
		_LineTo(       7, nBottom*5/6);
		_LineTo(      11, nBottom/2+1);
		_LineTo(       3, nBottom/2+1);
*/
    }
    _SetColor(nOldColor);
}

void    CtmSelectEdit::DrawFillTriangle(short nRow, short nCol, short nMaxRow, BOOL bUp)
{
    short   i;

    for(i=0; i<nMaxRow; i++)
    {
        if((i == 0) || (i == nMaxRow-1))      _Line(nCol+i, nRow, nCol-i + 7, nRow);
        else
        {
            _Line(nCol+i, nRow, nCol-i + 7, nRow);
            if(bUp) nRow --;
            else    nRow ++;
            _Line(nCol+i, nRow, nCol-i + 7, nRow);
        }
        if(bUp) nRow --;
        else    nRow ++;
    }
}

BOOL    CtmSelectEdit::SetIndex(int nIndex)   
{
	//m_nCount = 0;
	for (int i = 0; i < MAX_STRING_COUNT; i++)
    {
    	if (m_pszShow[i] != NULL)
    	{
    		CheckCharacter(m_pszShow[i]);    		
    		//m_pcstring->AddString(m_pszShow[i]);  //	fans 
    		//if(m_nCount == 0)	m_nCount++;			//yangpeng 2007/5/17 08:57上午
    	}
    }
    if((m_nIndex != nIndex) && (nIndex >=0) && (nIndex < m_nCount))
    {
        m_bChanged = TRUE;
        m_nIndex = nIndex;
         if (m_pszDBID[0] != NULL)
    			SetViewDBValue(m_pszDBID[0], m_nIndex + m_iStartVaule);
        return  TRUE;
    }
    else    return  FALSE; 
}

bool 	CtmSelectEdit::SetPropValueT(char* pszPropName, char* pszValue, int nLen, int nType)//rw.ying 2011/3/17 02:52下午
{
	bool	bResult = FALSE;
	if(nLen <= 0)
		return bResult;
	
	char	pszName[48];
	memset(pszName, 0, sizeof(pszName));
	for(int i = 0; i < MAX_STRING_COUNT; i++)
	{
		sprintf(pszName, "stream%d", i);
		if(strcmp(pszName, pszPropName) == 0)
		{
			bResult = CtmWnd::SetPropValueT(pszPropName, pszValue, nLen, nType);
		}
		if(bResult)
		{
			m_wszStreamLen[i] = nLen;
			m_wStreamType[i] = nType;
			if(m_wListNum >= 2)
				m_memo.SetFontLen(i, nLen, nType);
			break;
		}
	}
	return bResult;
}

int		CtmSelectEdit::OnLose()
{
	DBVIEWVALUE dbViewValue; 
	
	if (m_pszDBID[0] != NULL)
	{
		dbViewValue = GetDBValue(m_pszDBID[0]);
		m_nIndex = dbViewValue.lValue - m_iStartVaule;
		if ((m_nIndex >= MAX_STRING_COUNT) || m_nIndex < 0) m_nIndex = 0;
		m_iPrivilege = dbViewValue.dwPrivilege;
	}
	else m_nIndex = m_nOldIndex;
    //m_nOldIndex = m_nIndex;
    Set();
    
    
    m_bOnFocus = FALSE;
	/* if(m_wType == tmWS_SEDIT_FILL)
	{
	    SetBGColor(m_wndattr.bgcSelectEdit1);
	    SetFGColor(m_wndattr.fgcSelectEdit1);
    }
    else */   
	CtmWnd::DrawLoseFocus();
    	
    m_nCol 		= 0;
	m_bgc 		= m_bgcLoseFocus;
	m_nOldIndex = -1;
	
    Update();
	
	DisableList();
	
	
	return 0;
}


bool 	CtmSelectEdit::SetPropValueT(char* pszPropName, double Value)
{
	return CtmWnd::SetPropValueT(pszPropName, Value);
}

bool 	CtmSelectEdit::SetPropValueT(char* pszPropName, char* pszValue)
{
	return CtmWnd::SetPropValueT(pszPropName, pszValue);
}

void        CtmSelectEdit::DrawBorder()
{
	short       nRight;
	short       nBottom;
	//add new style by J.Wong
//	printf("selectedit %d %d %d....\n",m_wnd.wStyle&tmWS_UNDERLINE_BORDER,m_wnd.wStyle,tmWS_UNDERLINE_BORDER);
	if((m_wnd.wStyle&tmWS_UNDERLINE_BORDER)==tmWS_UNDERLINE_BORDER)
	{
		_SetViewPort(m_rectWindow.left, 
					m_rectWindow.top, 
					m_rectWindow.right, 
					m_rectWindow.bottom
				);
		nRight  = m_rectWindow.right  - m_rectWindow.left;
		nBottom = m_rectWindow.bottom - m_rectWindow.top;
		_SetColor(m_wndattr.fgcBorder4);
		
		for(int i=0; i<2; ++i)
		{
			_MoveTo(     0, nBottom-i);
			_LineTo(  nRight-1,nBottom-i);
		}
		
	}
	else
	{
		CtmWnd::DrawBorder();
	}

}
void		CtmSelectEdit::WndProc(int message, WPARAM wParam, LPARAM lParam)
{/*
	switch(message)
    {
		case MSG_MOUSELUP:
			MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
       		break;
        case MSG_MOUSELDOWN:
        	MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        default:
        	break;
    }	*/
    ;
}

BOOL		CtmSelectEdit::MouseDown(short x_pos,short y_pos,int key_flag)
{/*
		RECT    rect 		= GetWindowRECT();
		int 	i 			= 0;
		if (x_pos > rect.left && x_pos < rect.right && y_pos > rect.top && y_pos < rect.bottom)
		{
			if (!m_bList)
				EnableList();
			else
				DisableList();
		}
		*/
 	return TRUE;
}

BOOL		CtmSelectEdit::MouseUp(short x_pos,short y_pos,int key_flag)
{/*
	if(m_bList)
	{
		RECT		memorect	=	m_memo.GetWindowRECT();
		if (x_pos > memorect.left && x_pos < memorect.right && y_pos > memorect.top && y_pos < memorect.bottom)
			{
					SetIndex(m_memo.GetIndex());
					DisableList();
					Update();
			}
	}*/
 	return TRUE;
}
