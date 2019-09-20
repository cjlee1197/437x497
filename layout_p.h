/*Created by J.Wong 2017/03/01*/

#pragma once

#include <list>
#include <guilite/IGui>
#include "common.h"

class CtmWnd;
typedef struct WndItem
{
	CtmWnd* wndItem;
	int x,  y;
	CSize  size;
	bool activate;
	bool autoDestroy;
	
}_WndItem;

class CtmLayout;

typedef struct LayoutItem
{
	CtmLayout* layoutItem;
	int x, y;
	CSize size;
	bool activate;
	bool autoDestroy;
}_LayoutItem;

class CLayoutPrivate{	
public:
	enum{MAX_ITEM=16};
	CLayoutPrivate();
	~CLayoutPrivate();
	void init();
	
	int item_nums;
	int oldItem_nums;
	int show_nums;
	
	BOOL transparent;
	BOOL touched;
	WORD orientation;
	
	int x;
	int y;
	CSize size;
	WORD space[MAX_ITEM];
	BOOL autoSpace;
	
	bool  focus;
	bool  properchanged;
	bool  redraw;
	
	int  positive;
	
	CImage			image;
	CImage			bgImage;
	CImage			fmImage;
	
	CtmWnd*			pCurWnd;
	std::list<WndItem> items;
	std::list<LayoutItem> layoutItems;
	
	bool 		coordchanged;
	bool		resetfocus;
};