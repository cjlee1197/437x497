/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : ScrollBar                                                      |
|  Task     : Windows Extension Manager                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans                                                         |
|  Version  : V1.00                                                          |
|  Creation : 20/10/2008                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include 	"progressbar.h"
#include "utils.h"

/*----------------------------------------------------------------------------+
|               Constants											 |
+----------------------------------------------------------------------------*/
#define		PROGRESS_BORDER_COLOR	55003

#define     tmPROGRESS_START_LEFT   0x1000
#define     tmPROGRESS_START_RIGHT  0x2000
#define     tmPROGRESS_START_TOP    0x4000
#define     tmPROGRESS_START_BOTTOM 0x8000


/*===========================================================================+
|           Class implementation - ScrollBar	                               |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
|----------------------------------------------------------------------------|
|Parameter: pwnd												|
|           nMaxline the total lines				    			     |
|           nCount the length of the list                                    |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmProgressBar, CtmWnd)
CtmProgressBar::CtmProgressBar():CtmWnd()
{
	strcpy(TYPEName, "CtmProgressBar");
	m_wBarType	= tmPROGRESS_H | tmPROGRESS_START_LEFT;
	m_llMax		= 0;
	m_llMin		= 0;
	m_llValue	= 0;
	m_wPos		= 0;
	m_pszDBID	= NULL;
	m_pszMaxID	= NULL;
	m_pszMinID	= NULL;
	m_bLimitDB	= FALSE;
	m_wDestColor= 6478;
	m_bgc		= 57182;
	m_wDestColor= 50975;
	m_bCopyBg	= FALSE;
}

CtmProgressBar::~CtmProgressBar()
{
	FreeBg();
}

BOOL CtmProgressBar::CreateA()
{
	CtmWnd::CreateA();
		
	m_Buffer.SetRegion(m_rectClient);
	m_Buffer.InitBuffer(m_bgc);
	
	GetPosBaseOnDB();
}



/*-----------------------------------------------------------------------------+
|  				Operations									  |
+-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
| Function  : Create()									          |
| Task	  : 					 								|
+----------------------------------------------------------------------------+
|  Call     : 	                                  						|
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmProgressBar::Show()           /* Virtual function */
{
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	//	fans add
  CtmWnd::Show();
	if (m_wnd.wStyle & tmWS_SHOW)
	{
		Clear();
		Paint();	
	}
	else Invisible();
	
	ChangePosNow(GetClientRECT());
}

void    CtmProgressBar::Update()
{
	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)	Show();
}

void	CtmProgressBar::SetRegInBuffer(int nSpace)
{
	
	if (m_wBarType & tmPROGRESS_V)
	{
		if (m_wBarType & tmPROGRESS_START_TOP)
			m_Buffer.SetFillRegInBuffer(0, 0, GetClientRight(), GetClientBottom() - nSpace);
		else 
			m_Buffer.SetFillRegInBuffer(0, nSpace, GetClientRight(), GetClientBottom());
	}
	else
	{
		if (m_wBarType & tmPROGRESS_START_RIGHT)
			m_Buffer.SetFillRegInBuffer(nSpace, 0, GetClientRight(), GetClientBottom());
		else
			m_Buffer.SetFillRegInBuffer(0, 0, GetClientRight() - nSpace, GetClientBottom());
	}
		
		
	//switch	(m_wBarType)
	//{
	//	case tmPROGRESS_V:
	//		m_Buffer.SetFillRegInBuffer(0, nSpace, GetClientRight(), GetClientBottom());
	//		break;
	//	default:
	//		m_Buffer.SetFillRegInBuffer(0, 0, GetClientRight() - nSpace, GetClientBottom());
	//		break;
	//}
}

void   	CtmProgressBar::Clear()
{
	DrawBorder();
	SetRegInBuffer(0);
	
	
	if (m_wBarType & tmPROGRESS_V) 	ClearV();
	else							ClearH();
	
	//switch	(m_wBarType)
	//{
	//	case tmPROGRESS_V:
	//		ClearV();
	//		break;
	//	default:
	//		ClearH();
	//		break;
	//}
	

}

void	CtmProgressBar::ClearH()
{
	m_Buffer.SetColorInBuffer(m_bgc);
	m_Buffer.FillInBuffer(m_nBgcColorControl, PROGRESS_BORDER_COLOR, m_bgc, 
						  GetClientRight() / 2, GetClientBottom() / 2);
}
	
void	CtmProgressBar::ClearV()
{
	m_Buffer.SetColorInBuffer(m_bgc);
	m_Buffer.FillInBuffer(m_nBgcColorControl, PROGRESS_BORDER_COLOR, m_bgc, 
						  GetClientRight() / 2, GetClientBottom() / 2, 0xFFFF, 
						  GetClientRight() / 2);
	
}

void    CtmProgressBar::DrawBorder()
{
	SetRegInBuffer(0);
	m_Buffer.SetColorInBuffer(PROGRESS_BORDER_COLOR);
	m_Buffer.DrawRectangleInBuffer(_GBORDER, 0, 0, GetClientRight(), GetClientBottom());
	//m_Buffer.DrawWithDoubleBuffer();
}

void	CtmProgressBar::Invisible()
{
	if (m_bCopyBg) ShowBg();
}


bool 	 CtmProgressBar::SetPropValueT(char* pszPropName, double Value)
{
	bool bResult = CtmWnd::SetPropValueT(pszPropName, double(Value));

	if (m_pszDBID == NULL)
	{
		if (strcmp(pszPropName, "value") == 0)
		{
			if (m_llMax != m_llMin)
				m_wPos	= m_llValue * 100 / (m_llMax - m_llMin);
			else
				m_wPos	= 0;
			if (m_wPos > 100)	m_wPos = 100;
			////printf("%s pos=%d\n", Name, m_wPos);
		}
		else if (strcmp(pszPropName, "position") == 0)
		{
			m_llValue = m_wPos * (m_llMax - m_llMin) / 100;
		}
	}
	
	
	return bResult;
}

void 	CtmProgressBar::SetPropertys()
{
	CtmWnd::SetPropertys();
	
	SetPropInfo("dbid0",		tmSTR, 		&m_pszDBID);
	SetPropInfo("maxid",		tmSTR, 		&m_pszMaxID);
	SetPropInfo("minid",		tmSTR, 		&m_pszMinID);
	SetPropInfo("value", 		tmINT64, 	&m_llValue);	
	SetPropInfo("min", 			tmINT64, 	&m_llMin);
	SetPropInfo("max", 			tmINT64, 	&m_llMax);
	SetPropInfo("position", 	tmWORD, 	&m_wPos);
	SetPropInfo("dblimit", 		tmBOOL, 	&m_bLimitDB);
	
	SetPropInfo("bartype",		tmWORD,		&m_wBarType);
	
}

void 	CtmProgressBar::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	
	SetPropInfo(PROGRESSBAR_DBID0,		tmSTR, 		&m_pszDBID);
	SetPropInfo(PROGRESSBAR_MAXID,		tmSTR, 		&m_pszMaxID);
	SetPropInfo(PROGRESSBAR_MINID,		tmSTR, 		&m_pszMinID);
	SetPropInfo(PROGRESSBAR_VALUE, 		tmINT64, 	&m_llValue);	
	SetPropInfo(PROGRESSBAR_MIN, 		tmINT64, 	&m_llMin);
	SetPropInfo(PROGRESSBAR_MAX, 		tmINT64, 	&m_llMax);
	SetPropInfo(PROGRESSBAR_POSITION, 	tmWORD, 	&m_wPos);
	SetPropInfo(PROGRESSBAR_DBLIMIT, 	tmBOOL, 	&m_bLimitDB);
	
	SetPropInfo(PROGRESSBAR_BARTYPE,	tmWORD,		&m_wBarType);
	
}

void	CtmProgressBar::Paint()
{
	
	
	if (!m_bCopyBg) 
	{
		CopyBg();
		m_bCopyBg = TRUE;
	}
	
	GetPosBaseOnDB();
	
	if (m_wBarType & tmPROGRESS_V) 	PaintV();
	else							PaintH();
	
	//switch	(m_wBarType)
	//{
	//	case tmPROGRESS_V:
	//		PaintV();
	//		break;
	//	default:
	//		PaintH();
	//		break;
	//}
	
	m_Buffer.DrawWithDoubleBuffer();

}

void	CtmProgressBar::PaintH()
{
	int nSpace = GetClientRight() - m_wPos * GetClientRight() / 100;
	SetRegInBuffer(nSpace);
	if (m_wBarType & tmPROGRESS_START_RIGHT)
		m_Buffer.FillInBuffer(m_nBgcColorControl, PROGRESS_BORDER_COLOR, m_fgc, 
							 nSpace + (GetClientRight() - nSpace) / 2, GetClientBottom() / 2, m_wDestColor, 
							  m_nColorChange);
	else
		m_Buffer.FillInBuffer(m_nBgcColorControl, PROGRESS_BORDER_COLOR, m_fgc, 
							 (GetClientRight() - nSpace) / 2, GetClientBottom() / 2, m_wDestColor, 
							  m_nColorChange);
}

void	CtmProgressBar::PaintV()
{
	int nSpace = GetClientBottom() - m_wPos * GetClientBottom() / 100;
	SetRegInBuffer(nSpace);
	
	if (m_wBarType & tmPROGRESS_START_TOP)
		m_Buffer.FillInBuffer(m_nBgcColorControl, PROGRESS_BORDER_COLOR, m_fgc, 
							  GetClientRight() / 2, (GetClientBottom() - nSpace) / 2, m_wDestColor, 
							  m_nColorChange);
	else
		m_Buffer.FillInBuffer(m_nBgcColorControl, PROGRESS_BORDER_COLOR, m_fgc, 
							  GetClientRight() / 2, nSpace + (GetClientBottom() - nSpace) / 2, m_wDestColor, 
							  m_nColorChange);
	
}

void	CtmProgressBar::GetPosBaseOnDB()
{
	if (m_bLimitDB)
	{
		if (m_pszMaxID != NULL)
		{
			DBVIEWVALUE dbViewValue; 
			dbViewValue	= GetDBValue(m_pszMaxID);
			m_llMax		= dbViewValue.lValue;			
		}
		
		if (m_pszMinID != NULL)
		{
			DBVIEWVALUE dbViewValue; 
			dbViewValue	= GetDBValue(m_pszMinID);
			m_llMin		= dbViewValue.lValue;
		}
		
		if (m_pszDBID != NULL)
		{
			DBVIEWVALUE dbViewValue; 
			dbViewValue	= GetDBValue(m_pszDBID);
			m_llValue 	= dbViewValue.lValue;
			
			if (m_pszMinID == NULL)
				m_llMin		= dbViewValue.lMinValue;
			
			if (m_pszMaxID == NULL)
				m_llMax		= dbViewValue.lMaxValue;
				
			if (m_llMax != m_llMin)
				m_wPos	= m_llValue * 100 / (m_llMax - m_llMin);
			else
				m_wPos	= 0;
				
			if (m_wPos > 100)	m_wPos = 100;
		}
	}
	else
	{
		if (m_pszDBID != NULL)
		{
			DBVIEWVALUE dbViewValue; 
			dbViewValue	= GetDBValue(m_pszDBID);
			m_llValue 	= dbViewValue.lValue;
			if (m_llMax != m_llMin)
				m_wPos	= m_llValue * 100 / (m_llMax - m_llMin);
			else
				m_wPos	= 0;
				
			if (m_wPos > 100)	m_wPos = 100;
		}
	}
	
	
}
