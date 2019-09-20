/*Created by J.Wong 2017/03/01*/

#include "layout_p.h"
#include "layout.h"
#include "colortable.h"
#include "mainfrm.h"


extern		CtmMainFrame*       g_pMainFrame; 

enum Orientation
{
	Horizontal,
	Vertial
	
};

CLayoutPrivate::CLayoutPrivate()
{
	init();

}

CLayoutPrivate::~CLayoutPrivate()
{
	std::list<WndItem>::iterator it=items.begin();
	for(; it!=items.end(); ++it)
	{
		if(it->wndItem&&it->autoDestroy)
		{
			delete it->wndItem;
			it->wndItem=0;
		}
	}
	items.clear();
	
	
	std::list<LayoutItem>::iterator lit=layoutItems.begin();
	for(; lit!=layoutItems.end(); ++lit)
	{
		if(lit->layoutItem&&lit->autoDestroy)
		{
			delete lit->layoutItem;
			lit->layoutItem=0;
		}
	}
	layoutItems.clear();
}


void CLayoutPrivate::init()
{
	x=y=0;
	touched=false;
	focus=false;
	size=CSize(200,200);
	transparent=true;
	item_nums=0;
	oldItem_nums=0;
	show_nums =0;
	properchanged=false;
	redraw=false;
	orientation=Vertial;
	pCurWnd =0;
	coordchanged=false;
	resetfocus=false;
	autoSpace=true;
	memset(space,0,MAX_ITEM*sizeof(WORD));
}


static inline void CalcutateElemensSize(CLayoutPrivate* d)
{
		if(d)
		{
			d->redraw=true;
			CSize size=d->size;
			if(d->show_nums)
			{
				size/=d->show_nums;
			}
			
			//wnd size
			std::list<WndItem>::iterator it=d->items.begin();
			int cnt=0;
			int total_space=0;
			for(; it!=d->items.end(); ++it)
			{
				if(d->orientation==Vertial)
				{
					it->size.setWidth(d->size.width());
					if(d->autoSpace)
						it->size.setHeight(size.height());
					else
						it->size.setHeight(d->space[cnt]);
				}
				else
				{
					if(d->autoSpace)
						it->size.setWidth(size.width());
					else
						it->size.setWidth(d->space[cnt]);
					it->size.setHeight(d->size.height());
				}
				++cnt;
			}
			
			//layout size
			std::list<LayoutItem>::iterator lit=d->layoutItems.begin();
			for(;lit!=d->layoutItems.end(); ++lit)
			{
				if(d->orientation==Vertial)
				{
					lit->size.setWidth(d->size.width());
					if(d->autoSpace)
						lit->size.setHeight(size.height());
					else
						lit->size.setHeight(d->space[cnt]);
				}
				else
				{
					lit->size.setWidth(size.width());
					if(d->autoSpace)
						lit->size.setWidth(size.width());
					else
						lit->size.setWidth(d->space[cnt]);
				}
				++cnt;
			}
			
			
		}
}

static inline void CalculateElementsCoord(CLayoutPrivate* d)
{
	if(d)
	{
		d->redraw=true;
		CSize size=d->size;
		if(d->show_nums)
			size/=d->show_nums;
		std::list<WndItem>::iterator it=d->items.begin();
		int cnt=0;
		int total_space=0;
		for(; it!=d->items.end(); ++it)
		{
			if(it->activate)
			{
				
				if(d->orientation==Vertial)
				{
					it->x=0;
					if(d->autoSpace)
						it->y=size.height()*cnt;
					else
						it->y+=total_space;
				}
				else
				{
					if(d->autoSpace)
						it->x=size.width()*cnt;
					else
						it->x+=total_space;
					it->y=0;
				}
				if(!d->autoSpace)
					total_space+=d->space[cnt];
				++cnt;
			}
		}
		
		std::list<LayoutItem>::iterator lit=d->layoutItems.begin();
		for(; lit!=d->layoutItems.end(); ++lit)
		{
			if(lit->activate)
			{
				
				if(d->orientation==Vertial)
				{
					lit->x=0;
					if(d->autoSpace)
						lit->y=size.height()*cnt;
					else
						lit->y+=total_space;
				}
				else
				{
					if(d->autoSpace)
						lit->x=size.width()*cnt;
					else
						lit->x+=total_space;
					lit->y=0;
				}
				if(!d->autoSpace)
					total_space+=d->space[cnt];
				++cnt;
			}
		}
	}
}

/* class function */
IMPLEMENT_DYNCREATE(CtmLayout, CtmWnd)

CtmLayout::CtmLayout():CtmWnd()
{
	strcpy(TYPEName, "CtmLayout");
	d=new CLayoutPrivate();
	
	if(d)
	{
		d->fmImage=CImage(d->size, CImage::Format_RGB32);
	}
}


CtmLayout::~CtmLayout()
{
	delete d;
}

void CtmLayout::Show()
{
	if(IsNeedResize(d))
	{
		
		
		if(IsNeedResize(d))
		{
			d->coordchanged=true;
			Resize(d);
		}
		
		if(d->transparent)
		{
			if(!d->bgImage.isNull())
				d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));
			else
				d->fmImage.fill((uint)m_bgc);
		}
		else
		{
			d->fmImage.fill((uint)m_bgc);
		}
	
		d->properchanged=true;
	}
	//if(d->focus)
	//	d->properchanged=true;
	if(d->properchanged)
	{
		if(m_bVisible)
		{
			//显示处理
			//Layout层显示
			Clear();
			//test code
			// CPainter painter(&d->image);
			// CPen pen(Gemini::blue_level0);
			// painter.setPen(pen);
			// painter.drawRect(CRect(CPoint(0,0),d->size)-CMargin(0,0,2,2));
			
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->image);
			
			//控件显示
			//if(ElementsChanged(d))
			//{
			//	CalcutateElemensSize(d);
			//}
			if(ElementsCoordinateChanged(d))
			{
				CalcutateElemensSize(d);
				CalculateElementsCoord(d);
			}
			
			std::list<WndItem>::iterator it=d->items.begin();
			CRect layRect=CRect(CPoint(m_wnd.rect.left, m_wnd.rect.top),d->size);
			for(; it!=d->items.end(); ++it)
			{
				if(it->activate)
				{
					if(it->wndItem)
					{
						//set coordinate
						CtmWnd* pwnd=it->wndItem;
						if(d->redraw)
						{
							CRect rect=CRect(CPoint(it->x, it->y),it->size);
							rect.translate(m_wnd.rect.left, m_wnd.rect.top);
							rect&=layRect;
							//右侧偏移一个像素
							rect-=CMargin(1,1,1,1);
							pwnd->SetPropValueT("left",rect.left());
							pwnd->SetPropValueT("right",rect.right());
							pwnd->SetPropValueT("bottom",rect.bottom());
							pwnd->SetPropValueT("top",rect.top());
							pwnd->CreateA();
						}
						
						//private-changed
						pwnd->SetPropValueT("private-changed",d->properchanged);
						pwnd->Update();
					}
				}
			}
			
			std::list<LayoutItem>::iterator lit=d->layoutItems.begin();
			for(; lit!=d->layoutItems.end(); ++lit)
			{
				if(lit->activate)
				{
					if(lit->layoutItem)
					{
						//set coordinate
						CtmWnd* pwnd=lit->layoutItem;
						if(d->redraw)
						{
							CRect rect=CRect(CPoint(lit->x, lit->y),lit->size);
							rect.translate(m_wnd.rect.left, m_wnd.rect.top);
							//右侧偏移一个像素
							//rect-=CMargin(1,1,1,1);
							pwnd->SetPropValueT("left",rect.left());
							pwnd->SetPropValueT("right",rect.right());
							pwnd->SetPropValueT("bottom",rect.bottom());
							pwnd->SetPropValueT("top",rect.top());
							pwnd->CreateA();
						}
						pwnd->Update();
					}
				}
			}
			
		}
		else
		{
			_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
			_PutImage(0,0,d->bgImage);
		}
		d->properchanged=false;
		d->redraw=false;
		d->coordchanged=false;
	}
	
}

void CtmLayout::Update()
{

	if(d->item_nums!=d->oldItem_nums)
	{
		d->properchanged=true;
	}
	
	Show();
	if(d->resetfocus)
	{
		g_pMainFrame->GetCurrentView()->OnGetFocus();
		d->resetfocus=false;
	}
}

void CtmLayout::Clear()
{
	if(d)
		d->image=d->fmImage.copy(CRect(CPoint(0,0),d->size));
}

BOOL CtmLayout::CreateA()
{
	m_wnd.wStyle=0x27;
	CtmWnd::CreateA();
}

void CtmLayout::SetPropertys()
{
	CtmWnd::SetPropertys();
	if(d)
	{
		SetPropInfo((char*)"touched", tmBOOL, &d->touched);
		SetPropInfo((char*)"orientation", tmWORD, &d->orientation);
		char sz[10];
		for(int i=0; i<CLayoutPrivate::MAX_ITEM; ++i)
		{
			sprintf(sz,"space%d",i);
			SetPropInfo(sz,tmWORD,&d->space[i]);
		} 
		SetPropInfo((char*)"auto-space",tmBOOL, &d->autoSpace);
		SetPropInfo((char*)"private-changed", tmBOOL, 	&d->properchanged);
	}
}

void CtmLayout::OnGetFocus()
{
	if(d)
	{
		CtmWnd::OnGetFocus();
	}
}

void CtmLayout::OnLoseFocus()
{
	if(d)
	{
		
		CtmWnd::OnLoseFocus();
	}
}

WORD CtmLayout::OnKey(WORD wKey)
{
	
}


void CtmLayout::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	
}


bool CtmLayout::IsNeedResize(CLayoutPrivate* d)
{
	if(d)
		return m_rectClient.left!=d->x||m_rectClient.top!=d->y||CSize(GetClientWidth(),GetClientHeight())!=d->size;
	else
		return false;
}


void CtmLayout::Resize(CLayoutPrivate* d)
{
	if(d)
	{
		if(d->transparent)
		{
			//透明色,如果buffer中存在背景将其贴到原位置
			/* if(!d->bgImage.isNull())
			{
				_SetViewPort(d->x,d->y,d->x+d->size.width(),d->y+d->size.height());	
				_PutImage(0,0,d->bgImage);
			} */
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




void CtmLayout::addWnd(CtmWnd* wnd, bool autoDestroy)
{
	if(d)
	{
		if(wnd)
		{
			std::list<WndItem>::iterator it=d->items.begin();
			for(; it!=d->items.end(); ++it)
			{
				if(it->wndItem==wnd)
				{
					it->autoDestroy=autoDestroy;
					if(!it->activate)
					{
						it->activate=true;
						d->properchanged=true;
						it->wndItem->SetPropValueT("visible",(double)TRUE);
						g_pMainFrame->GetCurrentView()->OnLoseFocus();
						d->resetfocus=true;
					}
					break;
				}
			}
			if(it==d->items.end())
			{
				WndItem item;
				item.x=0;
				item.y=0;
				item.wndItem=wnd;
				item.activate=true;
				item.size=CSize(0,0);
				item.autoDestroy=autoDestroy;
				d->items.push_back(item);
				g_pMainFrame->GetCurrentView()->OnLoseFocus();
				d->resetfocus=true;
				d->item_nums++;
			}
		}
	}
}

void CtmLayout::addWnds(CtmWnd** wnds, int nums, bool autoDestroy)
{
	if(d)
	{
		
		for(int i=0; i<nums; ++i)
		{
			if(wnds[i])
			{
				addWnd(wnds[i]);
			}
		}
	}
}

void CtmLayout::addLayout(CtmLayout* layout, bool autoDestroy)
{
	if(d)
	{
		if(layout)
		{
			std::list<LayoutItem>::iterator it=d->layoutItems.begin();
			for(; it!=d->layoutItems.end(); ++it)
			{
				if(it->layoutItem==layout)
				{
					it->autoDestroy=autoDestroy;
					if(!it->activate)
					{
						it->activate=true;
						d->properchanged=true;
					}
					break;
				}
			}
			if(it==d->layoutItems.end())
			{
				LayoutItem item;
				item.x=0;
				item.y=0;
				item.layoutItem=layout;
				item.activate=true;
				item.size=CSize(0,0);
				item.autoDestroy=autoDestroy;
				d->layoutItems.push_back(item);
				d->item_nums++;
			}
		}
	}
}

void CtmLayout::addLayouts(CtmLayout** layouts, int nums, bool autoDestroy)
{
	if(d)
	{
		for(int i=0; i<nums; ++i)
		{
			if(layouts[i])
			{
				addLayout(layouts[i]);
			}
		}
	}
}


	
bool CtmLayout::removeWnd(CtmWnd* wnd)
{
	bool res=false;
	if(d)
	{
		std::list<WndItem>::iterator it=d->items.begin();
		for(; it!=d->items.end(); ++it)
		{
			if(it->wndItem==wnd)
			{
				if(it->activate)
				{
					it->activate=false;
					d->properchanged=true;
					it->wndItem->SetPropValueT("visible",(double)FALSE);
					g_pMainFrame->GetCurrentView()->OnLoseFocus();
					d->resetfocus=true;
					res =true;
				}
				break;
			}
		}

	}
	return res;
}

bool CtmLayout::removeWnd(int pos)
{
	if(d->items.size()<=pos||pos<0) return false;
	std::list<WndItem>::iterator it=d->items.begin();
	std::advance(it,pos);
	if(it->activate)
	{
		it->activate=false;
		d->properchanged=true;
		it->wndItem->SetPropValueT("visible",(double)FALSE);
		g_pMainFrame->GetCurrentView()->OnLoseFocus();
		d->resetfocus=true;
	}
	return true;

}

bool CtmLayout::removeWnds(char* className)
{
	bool res=false;
	if(className==0)	return false;
	std::list<WndItem>::iterator it=d->items.begin();
	for(; it!=d->items.end(); ++it)
	{
		if(it->wndItem->Is(className))
		{
			if(it->activate)
			{
				it->activate=false;
				d->properchanged=true;
				it->wndItem->SetPropValueT("visible",(double)FALSE);
				g_pMainFrame->GetCurrentView()->OnLoseFocus();
				d->resetfocus=true;
				res=true;
			}
		}
	}
	return res;
	
}

bool CtmLayout::removeWnd(char* Name)
{
	bool res=false;
	if(Name==0)	return false;
	std::list<WndItem>::iterator it=d->items.begin();
	for(; it!=d->items.end(); ++it)
	{
		if(strcmp(it->wndItem->Name,Name)==0)
		{
			if(it->activate)
			{
				it->activate=false;
				d->properchanged=true;
				it->wndItem->SetPropValueT("visible",(double)FALSE);
				g_pMainFrame->GetCurrentView()->OnLoseFocus();
				d->resetfocus=true;
				res=true;
			}
			break;
			
		}
	}
	return res;
}
		
bool CtmLayout::removeLayout(CtmLayout* layout)
{
	bool res=false;
	if(d)
	{
		std::list<LayoutItem>::iterator it=d->layoutItems.begin();
		for(; it!=d->layoutItems.end(); ++it)
		{
			if(it->layoutItem==layout)
			{
				if(it->activate)
				{
					it->activate=false;
					d->properchanged=true;
					res=true;
				}
				break;
				
			}
		}

	}
	return res;
}

bool CtmLayout::removeLayout(int pos)
{
	if(d->items.size()<=pos||pos<0) return false;
	std::list<WndItem>::iterator it=d->items.begin();
	std::advance(it,pos);
	if(it->activate)
	{
		it->activate=false;
		d->properchanged=true;
	}
	return true;
}


bool CtmLayout::ElementsChanged(CLayoutPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->item_nums!=d->oldItem_nums)
		{
			res=true;
			d->oldItem_nums=d->item_nums;
		}
		else if(d->coordchanged)
		{
			res=true;
		}
	}
	return res;
	
}

bool CtmLayout::ElementsCoordinateChanged(CLayoutPrivate* d)
{
	bool res=false;
	if(d)
	{
		int nums=0;
		std::list<WndItem>::iterator it=d->items.begin();
		for(; it!=d->items.end(); ++it)
		{
			if(it->activate)
				++nums;
		}
		std::list<LayoutItem>::iterator lit=d->layoutItems.begin();
		for(; lit!=d->layoutItems.end(); ++lit)
		{
			if(lit->activate)
				++nums;
		}
		if(nums!=d->show_nums)
		{
			d->show_nums=nums;
			res=true;
		}
		else if(d->coordchanged)
		{
			res=true;
		}
	}
	return res;
}

CtmWnd* CtmLayout::getWidget(char* Name)
{
	CtmWnd* pwnd=0;
	if(Name==0)	return 0;
	std::list<WndItem>::iterator it=d->items.begin();
	for(; it!=d->items.end(); ++it)
	{
		if(strcmp(it->wndItem->Name,Name)==0)
		{
			pwnd=it->wndItem;
		}
	}
	return pwnd;
}


CtmLayout* CtmLayout::getLayout(char* Name)
{
	CtmLayout* playout=0;
	if(Name==0)	return 0;
	std::list<LayoutItem>::iterator it=d->layoutItems.begin();
	for(; it!=d->layoutItems.end(); ++it)
	{
		if(strcmp(it->layoutItem->Name,Name)==0)
		{
			playout=it->layoutItem;
		}
	}
	return playout;
}

CtmWnd* CtmLayout::TouchAreaControls(CLayoutPrivate* d, const CPoint& pt)
{
	CtmWnd* pwnd=NULL;
	bool bFind=false;
	if(d)
	{
		CPoint cmp=pt+CPoint(-m_rectClient.left, -m_rectClient.top);
		std::list<WndItem>::iterator it=d->items.begin();
		for(; it!=d->items.end(); ++it)
		{
			if(it->activate)
			{
				CRect rect=CRect(CPoint(it->x,it->y), it->size);
				if(rect.contains(cmp))
				{
					int taborder=-1;
					it->wndItem->GetPropValueT("taborder",&taborder);
					if(taborder>=0)
					{
						pwnd=it->wndItem;
					}
					bFind=true;
					break;
			
				}
			}
		}
		if(!bFind)
		{
			std::list<LayoutItem>::iterator lit=d->layoutItems.begin();
			for(; lit!=d->layoutItems.end(); ++lit)
			{
				if(it->activate)
				{
					CRect rect=CRect(CPoint(lit->x,lit->y), lit->size);
					if(rect.contains(cmp))
					{
						pwnd=lit->layoutItem;
						bFind=true;
						break;
					}
				}
			}
		}
	}
	
	return pwnd;
}

void CtmLayout::SetSpace(int pos, WORD space)
{
	if(pos<0||pos>=CLayoutPrivate::MAX_ITEM) return;
	if(d)
	{
		d->space[pos]=space;
	}
}





