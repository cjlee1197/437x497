/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager souce  file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Shousu. fan                                                    |
|  Version  : V1.00                                                          |
|  Creation : 09/12/2006                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include	<math.h>
#include	"bindline.h"

//#include	"gui.h"

const	int POINT_SIZE = 6;
const	int	BROKEN_LINE_LEN = 6;//broken line

IMPLEMENT_DYNCREATE(CtmBindLine, CtmWnd)
IMPLEMENT_DYNCREATE(CtmBindCurveLine, CtmBindLine)

CtmBindLine::CtmBindLine():CtmWnd()
{
	strcpy(TYPEName, "CtmBindLine");
	m_pszBindControlName = NULL;
	m_bindControl.pwndBindControl = NULL;
	m_bindControl.iFix = 3;
	m_bindControl.pTag = NULL;
	 
	m_pszText = NULL;
	m_pszTextID = NULL;  
	
	m_bindControl.dwCommonColor = RGB2PIXEL(0xFF,0x00,0x00);//	63488;
	m_bindControl.dwFocusColor = RGB2PIXEL(0x00,0xFF,0x00);	//	2016
	m_dwColor = m_bindControl.dwCommonColor;
	
}

CtmBindLine::~CtmBindLine()
{
	
}

void	CtmBindLine::GetBindControls()
{
	CtmWnd* tempParent = NULL;
	tempParent = GetParent();
	
	if (tempParent != NULL)	
	{
		if (m_pszBindControlName != NULL)
		{
			if ((m_bindControl.pwndBindControl == NULL) || (strcmp(m_bindControl.pwndBindControl->Name, m_pszBindControlName) != 0))
					m_bindControl.pwndBindControl = tempParent->FindControlFromName(m_pszBindControlName);			
		}
		else m_bindControl.pwndBindControl = NULL;
	}
}



void	CtmBindLine::InitControl()
{
	GetBindControls();
	m_wnd.rect = GetClientRECT();	
	
	StrIDToStr(m_pszTextID,  &m_pszText);
	
}

BOOL 	CtmBindLine::CreateA()
{
	CtmWnd::CreateA();
	m_dwColor = m_bindControl.dwCommonColor;
	InitControl();
	return TRUE;
}

void 	CtmBindLine::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("bindcontrol", 	tmSTR, 	&m_pszBindControlName);
	SetPropInfo("bindtype",		tmINT,	&m_bindControl.iType);
	SetPropInfo("fix",			tmINT,	&m_bindControl.iFix);	
	
	SetPropInfo("commoncolor", 	tmDWORD,&m_bindControl.dwCommonColor);
	SetPropInfo("focuscolor", 	tmDWORD,&m_bindControl.dwFocusColor);
	
	//SetPropInfo("color", 		tmDWORD,&m_dwColor);
	
	//SetPropInfo("text", 		tmSTR,	&m_pszText);
	//SetPropInfo("textID", 		tmSTR,	&m_pszTextID);
	//SetPropInfo("tagpoint",		tmUnknown, &m_bindControl.pTag);
	
}

void	CtmBindLine::SetOnFocus()
{
	m_dwColor = m_bindControl.dwFocusColor;
}

void	CtmBindLine::SetLoseFocus()
{
	m_dwColor = m_bindControl.dwCommonColor;
}


void    CtmBindLine::Show()
{
	//SetFocus();
	if (m_wnd.wStyle & tmWS_SHOW)	
	{
		DrawBindLine(m_dwColor);
		CtmWnd::Show();				
	}
}


void    CtmBindLine::Update()
{
	InitControl();
	Show();	
}


void	CtmBindLine::DrawBindLine(DWORD dwColor)
{
	////cout << "CtmBindLine::DrawBindLine()" << endl;
	DrawIdLine(dwColor);
	DrawShowLine(dwColor);
	DrawText(m_pszText);
}

void CtmBindLine::DrawText(char* pszText)
{
	
}


void CtmBindLine::DrawIdLine(DWORD dwColor)
{
	
}

void CtmBindLine::DrawShowLine(DWORD dwColor)
{
	
}



CtmBindCurveLine::CtmBindCurveLine(): CtmBindLine()
{
	strcpy(TYPEName, "CtmBindCurveLine");
	m_wnd.rect.left = 0;
	m_wnd.rect.top = 0;
	m_wnd.rect.right = 0;
	m_wnd.rect.bottom = 0;
	m_iSecondPointIndex = 0;
	m_iFirstPointIndex = 0;
	m_pBindCurve = NULL;
}

CtmBindCurveLine::~CtmBindCurveLine()
{
}

tmPoint		CtmBindCurveLine::GetIDLineFirstPoint()
{
	tmPoint result;
	
	result = m_pBindCurve->GetSpecialPoint(0, m_iFirstPointIndex);
	
	return result;
}

tmPoint 	CtmBindCurveLine::GetIDLineSecondPoint()
{
	tmPoint result;
	result = m_pBindCurve->GetSpecialPoint(0, m_iSecondPointIndex);
	return result;
}


void		CtmBindCurveLine::DrawTrianglePoint(tmPoint PointVertex, int len, int iFix)
{
	switch(iFix)
	{
		case DRAW_FIX_LEFT:		//	頂點在最右側，所以需要向左繪
			_MoveTo(PointVertex.ix, PointVertex.iy);
			_LineTo(PointVertex.ix - len / 2, PointVertex.iy - len / 2);
			_LineTo(PointVertex.ix - len / 2, PointVertex.iy + len / 2);
			_LineTo(PointVertex.ix, PointVertex.iy);
			break;
		case DRAW_FIX_RIGHT:	//	頂點在最坐側，所以需要向右繪畫
			_MoveTo(PointVertex.ix, PointVertex.iy);
			_LineTo(PointVertex.ix + len / 2, PointVertex.iy - len / 2);
			_LineTo(PointVertex.ix + len / 2, PointVertex.iy + len / 2);
			_LineTo(PointVertex.ix, PointVertex.iy);
			break;
		case DRAW_FIX_UP:		//	定點在最下測，所以需要向上繪畫
			_MoveTo(PointVertex.ix, PointVertex.iy);
			_LineTo(PointVertex.ix - len / 2, PointVertex.iy - len / 2);
			_LineTo(PointVertex.ix + len / 2, PointVertex.iy - len / 2);
			_LineTo(PointVertex.ix, PointVertex.iy);
			break;				
		case DRAW_FIX_DOWN:		//	定點在最上側，所以需要向下繪畫
			_MoveTo(PointVertex.ix, PointVertex.iy);
			_LineTo(PointVertex.ix - len / 2, PointVertex.iy + len / 2);
			_LineTo(PointVertex.ix + len / 2, PointVertex.iy + len / 2);
			_LineTo(PointVertex.ix, PointVertex.iy);
			break;
	}
}


void		CtmBindCurveLine::DrawPoint(tmPoint Point, int size, int iFix)
{
	if (size <= 0) return;
	DrawTrianglePoint(Point, size, iFix);
}

void		CtmBindCurveLine::DrawIdLine(DWORD dwColor)
{
	short iLeft = 0, iRight = 0, iTop = 0, iBottom = 0;
	m_pBindCurve->GetPropValueT("left", &iLeft, sizeof(iLeft));
	m_pBindCurve->GetPropValueT("right", &iRight, sizeof(iRight));
	m_pBindCurve->GetPropValueT("top", &iTop, sizeof(iTop));
	m_pBindCurve->GetPropValueT("bottom", &iBottom, sizeof(iBottom));
	tmPoint point = GetIDLineFirstPoint();
	tmPoint point1 = GetIDLineSecondPoint();
	tmPoint Vertex = point;
	_SetColor(dwColor);
	_SetViewPort(GetParent()->GetmClientLeft(), GetParent()->GetmClientTop(),
				 GetParent()->GetmClientRight(), GetParent()->GetmClientBottom());
	
	switch	(m_bindControl.iType)
	{
		case 0x10:		//	僅僅需要繪點出來即可		
			_MoveTo(point.ix, point.iy);
			
			switch (m_bindControl.iFix)
			{
				case DRAW_FIX_LEFT:
					//_LineTo(iLeft, point.iy);
					Vertex.ix = iLeft + POINT_SIZE;
					break;
				case DRAW_FIX_RIGHT:
					//_LineTo(iRight, point.iy);
					Vertex.ix = iRight - POINT_SIZE;
					break;
				case DRAW_FIX_UP:
					//_LineTo(point.ix, iTop);
					Vertex.iy = iTop + POINT_SIZE;
					break;
				case DRAW_FIX_DOWN:
					//_LineTo(point.ix, iBottom);
					Vertex.iy = iBottom - POINT_SIZE;
					break;				
			}
			DrawDashed(point, Vertex, 4);
			DrawPoint(Vertex, POINT_SIZE, m_bindControl.iFix);
			break;
		default:
			break;
	}
}

void		CtmBindCurveLine::GetBindControls()
{
	CtmBindLine::GetBindControls();
	if (m_bindControl.pwndBindControl != NULL)
		m_pBindCurve = (CCurve*)m_bindControl.pwndBindControl;
}

void 		CtmBindCurveLine::SetPropertys()
{
	CtmBindLine::SetPropertys();
	SetPropInfo("pointindex0",	tmINT,	&m_iFirstPointIndex);
	SetPropInfo("pointindex1",	tmINT,	&m_iSecondPointIndex);
}	

void		CtmBindCurveLine::DrawDashed(tmPoint SPoint, tmPoint EPoint, int iBrokenLen)
{
	
	int	xD = EPoint.ix - SPoint.ix;		//	X軸上的距離
	int yD = EPoint.iy - SPoint.iy;		//	Y軸上的距離
	double temp = xD * xD + yD * yD;	
	int distance = (int)sqrt(temp);		//	平面距離
	
	int Num = distance / iBrokenLen;	//	
	int	xNum = xD / Num;
	int yNum = yD / Num;
	
	int X = SPoint.ix;
	int Y = SPoint.iy;
	
	int i = 0;	
	_MoveTo(SPoint.ix, SPoint.iy);
	while (i < Num)
	{
		_MoveTo(X + i * xNum, Y + i * yNum);
		_LineTo(X + (i + 1) * xNum, Y + (i + 1) * yNum);
		i += 2;
	}
	
}
