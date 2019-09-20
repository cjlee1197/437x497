#include "ring_p.h"
#include "ring.h"
#include "colortable.h"
#include "drawtext.h"
#include "utils.h"

extern BOOL g_bFocusChanged;
enum ClockDirection
{
	AntiClockwise=0,
	Clockwise=1	
};

const 	float max_percent =100.0;
const 	float min_percent =0.0;

CRingPrivate::CRingPrivate()
{
	init();	
}

CRingPrivate::~CRingPrivate()
{
		
}
	
void 	CRingPrivate::init()
{
	
	percent 		=0.0;
	oldPercent 		=0.0;
	
	tickInterval 	=1.0;
	
	clockwise 	=Clockwise; /*0:逆时针,1:顺时针*/
	leftColor 	=Gemini::dark_blue_level0;
	color	  	=Gemini::white_level1;
	penWidth  	=1;
	lastPenWidth =1;
	mgn		  	=CMargin(1,1,1,1);
	size	  	=CSize(100,100);
	redraw	  	=false;
	touched	  	=false;
	transparent =false;
	focus		=false;
	oldTransparent =transparent;
	x =0;
	y =0;
	positive =0;
	lastPercent=0.0;
	oldLastPercent=0.0;
	
	lastColor=color;
	properchanged=false;
	showPercent=true;
	
	llValue=0;
	llMaxValue=0;
	llMinValue=0;
	
	
	maxdb=true;
	mindb=true;
	
	
	
	llLastValue=0;
	llLastMaxValue=0;
	llLastMinValue=0;
	
	
	lastmaxdb=true;
	lastmindb=true;
	
	
	precision=0;
	
	
	memset(szPrompt0,0,tmMAX_TEXT_COL*2);
}

void	CtmRing::SetPercent(float percent)
{
	if(d)
		d->percent=percent;	
}
float	CtmRing::GetPrercent()const
{
	if(d)
		return d->percent;
	return 0;	
}
	
void	CtmRing::SetColor(uint color)
{
	if(d)
		d->color=color;	
}
uint	CtmRing::GetColor()const
{
	if(d)
		return d->color;	
	return 0;
}
	
void	CtmRing::SetLeftColor(uint color)
{
	if(d)
		d->leftColor=color;	
}

uint	CtmRing::GetLeftColor()const
{
	if(d)
		return d->leftColor;
	return 0;	
}
	
void	CtmRing::SetTickInterval(float interval)
{
	if(d)
	 	d->tickInterval=interval;
		
}

float 	CtmRing::GetTickInterval()const
{
	if(d)
		return d->tickInterval;
	return 0;
}	

void	CtmRing::SetClockwise(BOOL clockwise)
{
	if(d)
		d->clockwise=clockwise;
}

BOOL	CtmRing::GetClockwise()const
{
	if(d)
		return d->clockwise;
	return 0;
}

IMPLEMENT_DYNCREATE(CtmRing, CtmWnd)
CtmRing::CtmRing():CtmWnd()
{
	strcpy(TYPEName, "CtmRing");
	d=new CRingPrivate();
	if(d)
	{
		d->fmImage=CImage(d->size,CImage::Format_RGB32);	
	}
	
	dbid=0;
	maxdbid=0;
	mindbid=0;
	lastdbid=0;
	lastmaxdbid=0;
	lastmindbid=0;
	pszPrompt=0;
	pszPromptLan=0;
	pszPromptID=0;	
}

CtmRing::~CtmRing()
{
	delete d;
}

static inline void DrawCircle(CRingPrivate* d)
{
	if(d)
	{
		if(d->redraw)
		{
			CPainter painter(&d->fmImage);
			CPen pen;
			pen.setWidth(d->penWidth);
			pen.setColor(d->leftColor);
			CBrush brush(d->leftColor);
			CPainterPath path;
			painter.setRenderHints(CPainter::Antialiasing,true);
			painter.setPen(pen);
			painter.setBrush(brush);
			int square=mMin(d->size.width(),d->size.height());
			CRectF bgRect(CPoint(0,0),d->size);
			bgRect-=(d->mgn+1);
			CPointF cp=bgRect.center();
			CRectF rect(0,0,square,square);
			
			CPointF rp=rect.center();
			rect.translate((cp-rp));
			rect-=(d->mgn+1);
			path.addEllipse(rect);
	
			float distance= mMin(rect.width(),rect.height())/8;
			rect-=CMargin(distance,distance,distance,distance);
			path.addEllipse(rect);
			painter.drawPath(path);
		}
	}	
}	
static inline void DrawRing(CRingPrivate* d)
{
	if(d)
	{
		CPainter painter(&d->image);
		CBrush brush(d->color);
		painter.setBrush(brush);
		CPen pen;
		pen.setColor(d->color);
		pen.setWidth(d->penWidth);
		painter.setPen(pen);
		CPainterPath path;
		painter.setRenderHints(CPainter::Antialiasing,true);
	
		int square=mMin(d->size.width(),d->size.height());
		CRectF bgRect(CPoint(0,0),d->size);
		bgRect-=(d->mgn+1);
		CPointF cp=bgRect.center();
		CRectF outRect(0,0,square,square);
		
		CPointF rp=outRect.center();
		outRect.translate((cp-rp));
		outRect-=(d->mgn+1);
		float distance= mMin(outRect.width(),outRect.height())/8;
		CRectF inRect=outRect-CMargin(distance,distance,distance,distance);
		
		CPointF cur;
		CPointF start;
		/*0:逆时针*/
		/*1:顺时针*/
		d->percent=mMin(max_percent,mMax(min_percent,d->percent));
		float spanAngle=d->percent/max_percent*360;
		if(AntiClockwise==d->clockwise)
		{
			if(d->percent>min_percent&&d->percent<max_percent)
			{
				path.arcMoveTo(outRect, 90+spanAngle);
				start=path.currentPosition();
				path.arcTo(outRect,90+spanAngle,-spanAngle);
				cur=path.currentPosition();
				path.lineTo(cur.x(),inRect.y());
				path.arcTo(inRect,90,spanAngle);
				path.lineTo(start);
				painter.drawPath(path);
			}
		}
		else if(Clockwise==d->clockwise)
		{
			if(d->percent>min_percent&&d->percent<max_percent)
			{
				path.arcMoveTo(outRect, 90-spanAngle);
				start=path.currentPosition();
				path.arcTo(outRect,90-spanAngle,spanAngle);
				cur=path.currentPosition();
				path.lineTo(cur.x(),inRect.y());
				path.arcTo(inRect,90,-spanAngle);
				path.lineTo(start);
				painter.drawPath(path);
			}	
		}
		if(d->percent==max_percent)
		{
			CPainterPath ellipsePath;
			ellipsePath.addEllipse(outRect);
			ellipsePath.addEllipse(inRect);
			painter.drawPath(ellipsePath);	
		}
		if(d->showPercent)
		{
			if(g_pDrawText)
			{
				int width=inRect.width();
				int height=inRect.height()/2;
				CRectF textRect(inRect.center()-CPointF(width/2,height/2),CSize(width,height));
				if(textRect.isValid())
				{
					unsigned char szText[10];
					sprintf((char*)szText,"%.*f%%",d->precision,d->percent);
					int oldColor=g_pDrawText->GetFontColor();
					int oldFontWidth=g_pDrawText->GetFontWidth();
					int oldFontHeight=g_pDrawText->GetFontHeight();
					int oldAlign=g_pDrawText->GetFontAlign();
					g_pDrawText->SetFontSize(textRect.width()/strlen((const char*)szText),textRect.height(),0x02);
					g_pDrawText->SetFontColor(d->color);
					
					g_pDrawText->DrawText(d->image,szText,textRect.toRect(),strlen((const char*)szText));
					g_pDrawText->SetFontColor(oldColor);
					g_pDrawText->SetFontSize(oldFontWidth,oldFontHeight,oldAlign);
				}	
			}
		}
	}
}

	
void    CtmRing::Show()
{
	if(IsTransparentChanged(d)||IsNeedResize(d)||IsPenChanged(d))
	{
		if(IsNeedResize(d))
			Resize(d);
		if(IsPenChanged(d))
			PenChanged(d);
		if(!d->transparent)
		{
			d->fmImage.fill((uint)m_bgc);	
		}
		else
		{
			if(!d->bgImage.isNull())
				d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
		}
		
		d->redraw =true;
		d->properchanged=true;
	}
	else
	{
		d->redraw =false;	
	}
	if(d->focus)
	{
		d->properchanged=true;
	}
	if(d->properchanged)
	{
		if(m_bVisible)
		{
			DrawCircle(d);
			Clear();
			if(!d->focus)
			{
				GetDBInfo(d);
			}
			DrawRing(d);
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->image);
		}
		else
		{
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->bgImage);
		}
		d->properchanged=false;
	}
	
}

void    CtmRing::Update()
{
	//auto test coding

	/* if(d->positive)
	{
		d->percent+=5;
	}
	else
	{
		d->percent-=5;	
	}
	if(d->percent>=max_percent)
	{
		d->positive=0;
	}
	else if(d->percent<=min_percent)
	{
		d->positive=1;
	} */
	if(!d->focus)
	{
		GetDBInfo(d);
		if(d->oldPercent!=d->percent)
		{
			d->oldPercent=d->percent;
			d->properchanged=true;
		}
		if(d->oldLastPercent!=d->lastPercent)
		{
			d->oldLastPercent=d->lastPercent;
			d->properchanged=true;
		}
	}
	Show();
}

void    CtmRing::Clear()
{
	if(d)
		d->image=d->fmImage.copy(CRect(CPoint(0,0),d->size));
}

BOOL 	CtmRing::CreateA()
{
	m_wnd.wStyle=0x27;
	CtmWnd::CreateA();
}

void 	CtmRing::SetPropertys()
{
	CtmWnd::SetPropertys();
	if(d)
	{
		SetPropInfo((char*)"percent", tmFLOAT, &d->percent);
		SetPropInfo((char*)"tick-interval",tmFLOAT, &d->tickInterval);
		SetPropInfo((char*)"color", tmDWORD, &d->color);
		SetPropInfo((char*)"left-color",tmDWORD,&d->leftColor);
		SetPropInfo((char*)"touched",tmBOOL,&d->touched);
		SetPropInfo((char*)"transparent",tmBOOL,&d->transparent);
		SetPropInfo((char*)"clockwise",tmBOOL,&d->clockwise);
		SetPropInfo((char*)"private-changed", tmBOOL, 	&d->properchanged);
		SetPropInfo((char*)"show-percent",tmBOOL,&d->showPercent);
		SetPropInfo((char*)"dbid0",tmSTR, &dbid);
		SetPropInfo((char*)"maxid",tmSTR,&maxdbid);
		SetPropInfo((char*)"minid",tmSTR,&mindbid);
		SetPropInfo((char*)"maxdb",tmBOOL,&d->maxdb);
		SetPropInfo((char*)"mindb",tmBOOL,&d->mindb);
		
		SetPropInfo((char*)"precision",tmWORD, &d->precision);
		SetPropInfo((char*)"prompt",tmSTR, &pszPromptLan);
		SetPropInfo((char*)"promptID",tmSTR, &pszPromptID);
	
	}
}

static inline void Tick(CRingPrivate* d,bool valuences)
{
	if(d)
	{
			if(valuences)
			{
				d->percent+=d->tickInterval;
				d->percent=mMin(d->percent,max_percent);
			}
			else
			{
				d->percent-=d->tickInterval;
				d->percent=mMax(d->percent,min_percent);
			}
	}	
}

//process event
WORD    CtmRing::OnKey(WORD wKey)
{
	WORD			wResult=wKey;
	if(!d)			return wKey;
	if(!d->focus)	return wKey;
	switch (wKey)
	{
		case 0x6D00: 	//down
		case _KEY_DOWNFIELD:
			Tick(d,0);
			Update();
			wResult=_NULL_KEY;
			break;
		case 0x6800:	//up
		case _KEY_UPFIELD:
			Tick(d,1);
			Update();
			wResult=_NULL_KEY;
			break;
		default:
			break;	
	}
	return wResult;

}


bool CtmRing::TouchAreaChanged(CRingPrivate* d, const CPoint& pt)
{
	bool res=false;
	CPainterPath spanPath;
	CPainterPath leftPath;
	float spanAngle=d->percent/max_percent*360;
	CRectF spanRect=CRectF(CPoint(0,0),d->size);
	CPointF cen=spanRect.center();
	CPoint cmp=pt+CPoint(-m_rectClient.left,-m_rectClient.top);
	if(d->clockwise==Clockwise)
	{
		spanPath.arcMoveTo(spanRect,90);
		CPointF start=spanPath.currentPosition();
		spanPath.arcTo(spanRect,90,-spanAngle);
		spanPath.lineTo(cen);
		spanPath.lineTo(start);
		
		leftPath.arcMoveTo(spanRect,90);
		leftPath.arcTo(spanRect,90,360-spanAngle);
		leftPath.lineTo(cen);
		leftPath.lineTo(start);
	}
	else
	{
		spanPath.arcMoveTo(spanRect,90);
		CPointF start=spanPath.currentPosition();
		spanPath.arcTo(spanRect,90,spanAngle);
		spanPath.lineTo(cen);
		spanPath.lineTo(start);
		
		leftPath.arcMoveTo(spanRect,90);
		leftPath.arcTo(spanRect,90,spanAngle-360);
		leftPath.lineTo(cen);
		leftPath.lineTo(start);	
	}
	
	if(spanPath.contains(cmp))
	{
		Tick(d,0);
		res=true;
	}
	else if(leftPath.contains(cmp))
	{
		Tick(d,1);
		res=true;	
	}
	return res;	
}

void	CtmRing::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	if(!d->touched) return ;
	int x=wParam>>16;
	int y=wParam&0x0000FFFF;
	switch(message)
	{
		case MSG_MOUSELUP:
			{
				bool changed=TouchAreaChanged(d,CPoint(x,y));
				if(changed)	Update();
			}
			break;
		case MSG_MOUSELDOWN:
			break;
		default:
			break;
	}	
}
		
void   	CtmRing::OnGetFocus()
{
	if(d)
	{
		GetDBInfo(d);
		Update();
		d->oldPercent=d->percent;
		d->oldLastPercent=d->lastPercent;
		d->focus	=true;
		CtmWnd::DrawGetFocus();
		Prepare(d);
		Prompt(d->szPrompt0);
	}
}

void    CtmRing::OnLoseFocus()
{
	if(d)
	{
		d->focus = false;
		CtmWnd::DrawLoseFocus();
		if(!g_bFocusChanged)
		{
			SyncDBValue(d);
		}
		d->properchanged=true;
		Show();
	}
}


bool CtmRing::IsNeedResize(CRingPrivate* d)
{
	if(d)
		return m_rectClient.left!=d->x||m_rectClient.top!=d->y||CSize(GetClientWidth(),GetClientHeight())!=d->size;
	else
		return false;
}

void CtmRing::Resize(CRingPrivate* d)
{
	if(d)
	{
		if(d->transparent)
		{
			//透明色,如果buffer中存在背景怎将其贴到原位置
			// if(!d->bgImage.isNull())
			// {
				// _SetViewPort(d->x,d->y,d->x+d->size.width(),d->y+d->size.height());	
				// _PutImage(0,0,d->bgImage);
			// }
		}
		d->fmImage=CImage(CSize(GetClientWidth(),GetClientHeight()),CImage::Format_RGB32);
		d->size=CSize(GetClientWidth(),GetClientHeight());
		d->x=m_rectClient.left;
		d->y=m_rectClient.top;
		_SetViewPort(d->x,d->y,d->x+d->size.width(),d->y+d->size.height());
		d->bgImage=d->fmImage.copy(CRect(CPoint(0,0),d->size));
		//获取新位置处的背景色
		if(!d->bgImage.isNull())
			_GetImage(0,0,d->bgImage);
	}		
}
	
bool CtmRing::IsPenChanged(CRingPrivate* d)
{
	if(d)
	{
		int tmp=mMin(d->size.width(),d->size.height())/50;
		tmp=tmp>1?tmp:1;
		return (d->penWidth!=tmp);	
	}
	else
		return false;
}

void CtmRing::PenChanged(CRingPrivate* d)
{
	if(d)
	{
		d->penWidth=mMin(d->size.width(),d->size.height())/50;
		d->penWidth=d->penWidth>1?d->penWidth:1;
		d->mgn=CMargin(1,1,1,1)+d->penWidth;
	}
}
	
bool CtmRing::IsTransparentChanged(CRingPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->transparent!=d->oldTransparent)
		{
			d->oldTransparent=d->transparent;	
			res=true;
		}
	}	
	return res;
}


void CtmRing::GetDBInfo(CRingPrivate* d)
{

	if(d)
	{
		if (dbid!= NULL)
		{
			DBVIEWVALUE dbViewValue; 
			dbViewValue = GetDBValue(dbid);
			d->llValue = dbViewValue.lValue;
			if (d->maxdb)
			{
				d->llMaxValue = dbViewValue.lMaxValue;
			}
			else
			{
				if (maxdbid != NULL)
				{
					d->llMaxValue = GetDBValue(maxdbid).lValue;
				}
			}
			
			if (d->mindb)
			{
				d->llMinValue = dbViewValue.lMinValue;
			}
			else
			{
				if (mindbid != NULL)
				{
					d->llMinValue = GetDBValue(mindbid).lValue;
				}
			}
			d->percent=(float)(d->llValue-d->llMinValue)/(d->llMaxValue-d->llMinValue)*(max_percent-min_percent)+min_percent;
		}
		
		//bind last dbid
		if (lastdbid!= NULL)
		{
			DBVIEWVALUE dbViewValue; 
			dbViewValue = GetDBValue(lastdbid);
			d->llLastValue = dbViewValue.lValue;
			if (d->lastmaxdb)
			{
				d->llLastMaxValue = dbViewValue.lMaxValue;
			}
			else
			{
				if (lastmaxdbid != NULL)
				{
					d->llLastMaxValue = GetDBValue(lastmaxdbid).lValue;
				}
			}
			
			if (d->lastmindb)
			{
				d->llLastMinValue = dbViewValue.lMinValue;
			}
			else
			{
				if (lastmindbid != NULL)
				{
					d->llLastMinValue = GetDBValue(lastmindbid).lValue;
				}
			}
			d->lastPercent=(float)(d->llLastValue-d->llLastMinValue)/(d->llLastMaxValue-d->llLastMinValue)*(max_percent-min_percent)+min_percent;
		}
	}
}

void CtmRing::SyncDBValue(CRingPrivate* d)
{
	if(d)
	{
		if(IsOverLimited(d))
		{
			if(dbid)
			{
				if(d->percent!=d->oldPercent)
				{
					long long llValue=mRound(d->percent/(max_percent-min_percent)*(d->llMaxValue-d->llMinValue)+d->llMinValue);
					SetDBValue(dbid,llValue);
				}
			}
		}
		
/* 		if(IsOverLastLimited(d))
		{
			if(lastdbid)
			{
				if(d->lastPercent!=d->oldLastPercent)
				{
					long long llValue=mRound(d->lastPercent/(max_percent-min_percent)*(d->llLastMaxValue-d->llLastMinValue)+d->llLastMinValue);
					SetDBValue(lastdbid,llValue);
				}
			}
		} */
	}
	
}

bool CtmRing::IsOverLimited(CRingPrivate* d)
{
	bool res=false;
	if(d->percent>=min_percent&&d->percent<=max_percent)
	{
		res=true;
	}
	return res;
}

/* bool CtmRings::IsOverLastLimited(CRingPrivate* d)
{
	bool res=false;
	if(d->lastPercent>=min_percent&&d->lastPercent<=max_percent)
	{
		res=true;
	}
	return res;
} */


void  CtmRing::ResetPrompt(CRingPrivate* d)
{
	if(d)
	{
		if(pszPromptID)
			pszPrompt=g_MultiLanguage[pszPromptID];
		else
			pszPrompt=pszPromptLan;	
	}
}


void  CtmRing::Prepare(CRingPrivate* d)
{
	if(d)
	{
		ResetPrompt(d);
		
		char        szMin[256];
		char        szMax[256];
		long 		lDataID = 0;
		char*		pszMin=szMin;
		char* 		pszMax=szMax;
		sprintf(pszMin, "%.*lf%%",d->precision,min_percent);
		sprintf(pszMax, "%.*lf%%",d->precision,max_percent);
		A2A(szMin, 0, m_pcFont->GetFontCode());
		A2A(szMax, 0, m_pcFont->GetFontCode());
		
		if(g_bDemoDataID)
		{
			if(dbid != NULL)
			{
				lDataID  = g_pDatabase->GetDataID(dbid);
			}
		}
		if(CtmConfig::GetInstance()->GetFontConvert())
		{
			if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
			{
				if(g_bDemoDataID)
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
					tm_sprintf(d->szPrompt0, "%x %t: %t %t: %t %t", lDataID, 
								g_MultiLanguage["MSG_MIN"], szMin,
								g_MultiLanguage["MSG_MAX"], szMax,pszPrompt);        
				}
				else
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
				
					tm_sprintf(d->szPrompt0, "%t: %t %t: %t %t", 
									g_MultiLanguage["MSG_MIN"], szMin,
									g_MultiLanguage["MSG_MAX"], szMax,pszPrompt);
				}
			}
			else
			{	
				if (pszPrompt != NULL)
				{
					if ((strlen(pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
					strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL*2)
					{
						sprintf(d->szPrompt0, "%s:%s %s:%s %s",g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax, pszPrompt);
					}
				}
				else sprintf(d->szPrompt0, "%s:%s %s:%s"   , g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
			}	
		}
		else
		{
			if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
			{
				if(g_bDemoDataID)
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
					tm_sprintf(d->szPrompt0, "%x %t %t: %t %t: %t", lDataID, 
							pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
							g_MultiLanguage["MSG_MAX"], szMax);        
				}
				else
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
					tm_sprintf(d->szPrompt0, "%t %t: %t %t: %t", 
									pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
									g_MultiLanguage["MSG_MAX"], szMax);
				}
			}
			else
			{	
				if (pszPrompt != NULL)
				{
					if ((strlen(pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
					strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL*2)
					{
						sprintf(d->szPrompt0, "%s %s:%s %s:%s", pszPrompt, g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
					}
				}
				else sprintf(d->szPrompt0, "%s:%s %s:%s", g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
			}
		}
	}
	
}



IMPLEMENT_DYNCREATE(CtmRings, CtmRing)
CtmRings::CtmRings():CtmRing()
{
	strcpy(TYPEName, "CtmRings");
}

CtmRings::~CtmRings()
{
	
}


static inline void DrawRings(CRingPrivate* d)
{
	if(d)
	{

		CPainter painter(&d->image);
		CPen pen;
		painter.setPen(pen);
		painter.setRenderHints(CPainter::Antialiasing,true);
		CRectF bgRect(CPoint(0,0),d->size);
		CPointF cp=bgRect.center();
		
		int square=mMin(d->size.width(),d->size.height());
		CRectF border=CRectF(CPoint(0,0),CSize(square,square));
		CRectF outborder=border-(d->mgn+1);
		border-=(d->mgn+1+d->penWidth);
		CMargin inMgn=CMargin(1,1,1,1)+d->lastPenWidth;
		CRectF inborder=border-inMgn;
		CPointF rp=border.center();
		border.translate(cp-rp);
		rp=outborder.center();
		outborder.translate(cp-rp);
		rp=inborder.center();
		inborder.translate(cp-rp);
		
		d->percent=mMin(max_percent,mMax(min_percent,d->percent));
		d->lastPercent=mMin(max_percent,mMax(min_percent,d->lastPercent));
		float spanAngle=d->percent/max_percent*360;
		float lastSpanAngle=d->lastPercent/max_percent*360;
		if(d->clockwise==Clockwise)
		{
			//draw border ring
			painter.drawEllipse(border);
			//draw out ring
			pen.setColor(d->color);
			pen.setWidth(d->penWidth);
			painter.setPen(pen);
			painter.drawArc(outborder,90*16,-spanAngle*16);
			//draw in ring
			pen.setColor(d->lastColor);
			pen.setWidth(d->lastPenWidth);
			painter.setPen(pen);
			painter.drawArc(inborder,90*16,-lastSpanAngle*16);
		}
		else if(d->clockwise==AntiClockwise)
		{
			//draw border ring
			painter.drawEllipse(border);
			//draw out ring
			pen.setColor(d->color);
			pen.setWidth(d->penWidth);
			painter.setPen(pen);
			painter.drawArc(outborder,90*16,spanAngle*16);
			//draw in ring
			pen.setColor(d->lastColor);
			pen.setWidth(d->lastPenWidth);
			painter.setPen(pen);
			painter.drawArc(inborder,90*16,lastSpanAngle*16);	
		}
//		d->lastPercent=d->percent;
		if(d->showPercent)
		{
			if(g_pDrawText)
			{
				int width=inborder.width();
				int height=inborder.height()/2;
				CRectF textRect(inborder.center()-CPointF(width/2,height/2),CSize(width,height));
				if(textRect.isValid())
				{
					unsigned char szText[10];
					sprintf((char*)szText,"%.*f%%",d->precision,d->percent);
					int oldColor=g_pDrawText->GetFontColor();
					int oldFontWidth=g_pDrawText->GetFontWidth();
					int oldFontHeight=g_pDrawText->GetFontHeight();
					int oldAlign=g_pDrawText->GetFontAlign();
					g_pDrawText->SetFontSize(textRect.width()/strlen((const char*)szText),textRect.height(),0x02);
					g_pDrawText->SetFontColor(d->color);
					
					g_pDrawText->DrawText(d->image,szText,textRect.toRect(),strlen((const char*)szText)+1);
					g_pDrawText->SetFontColor(oldColor);
					g_pDrawText->SetFontSize(oldFontWidth,oldFontHeight,oldAlign);
				}	
			}
		}
	}
}

void    CtmRings::Show()
{
	if(IsTransparentChanged(d)||IsNeedResize(d)||IsPenChanged(d))
	{
		if(IsNeedResize(d))
			Resize(d);
		if(IsPenChanged(d))
			PenChanged(d);
		if(!d->transparent)
		{
			d->fmImage.fill((uint)m_bgc);	
		}
		else
		{
			if(!d->bgImage.isNull())
				d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
		}
		
		d->redraw =true;
		d->properchanged=true;
	}
	else
	{
		d->redraw =false;	
	}
	if(d->focus)
	{
		d->properchanged=true;
	}
	if(d->properchanged)
	{
		if(m_bVisible)
		{
			Clear();
			if(!d->focus)
			{
				GetDBInfo(d);
			}
			DrawRings(d);
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->image);
		}
		else
		{
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->bgImage);	
		}
		d->properchanged=false;
	}
}

void 	CtmRings::SetPropertys()
{
	CtmRing::SetPropertys();
	if(d)
	{
		SetPropInfo((char*)"last-color",tmDWORD, &d->lastColor);
		SetPropInfo((char*)"last-percent",tmFLOAT,&d->lastPercent);
		
		SetPropInfo((char*)"last-maxdb",tmBOOL,&d->lastmaxdb);
		SetPropInfo((char*)"last-mindb", tmBOOL,&d->lastmindb);
		SetPropInfo((char*)"last-dbid", tmSTR, &lastdbid);
		SetPropInfo((char*)"last-minid", tmSTR, &lastmindbid);
		SetPropInfo((char*)"last-maxid",tmSTR, &lastmaxdbid);
		
	}	
}


void	CtmRings::SetLastColor(uint color)
{
	if(d)
		d->lastColor=color;
}

uint	CtmRings::GetLastColor()const
{
	if(d)
		return d->lastColor;
	return 0;
}

void	CtmRings::SetLastPercent(float percent)
{
	if(d)
		d->lastPercent=percent;
}

float 	CtmRings::GetLastPercent()const
{
	if(d)
		return d->lastPercent;
	return 0;
}

bool CtmRings::IsPenChanged(CRingPrivate* d)
{
	if(d)
	{
		int tmp=mMin(d->size.width(),d->size.height())/20;
		tmp=tmp>1?tmp:1;
		return (d->penWidth!=tmp);	
	}
	else
		return false;	
}

void CtmRings::PenChanged(CRingPrivate* d)
{
	if(d)
	{
		d->penWidth=mMin(d->size.width(),d->size.height())/20;
		d->penWidth=d->penWidth>1?d->penWidth:1;
		d->mgn=CMargin(1,1,1,1)+d->penWidth;
		d->lastPenWidth=d->penWidth/2;
		d->lastPenWidth=d->lastPenWidth>1?d->lastPenWidth:1;
	}
}

