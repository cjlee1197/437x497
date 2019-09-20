/*Created by J.Wong 2017/2/6 9:04:19*/
#pragma once
#include <guilite/IGui>
#include "common.h"
#include "wnd.h"
#include "cbitmap.h"


class CTogglePrivate
{
	public:
		CTogglePrivate();
		~CTogglePrivate();
		void init();
		BOOL  status;
		BOOL  oldStatus;
		uint  selectColor;
		uint  unSelectColor;
		uint  outlineColor;
		uint  oldOutlineColor;
		BOOL  touched;
		BOOL  transparent;
		BOOL  oldTransparent;
		
		CSize size;
		CMargin mgn;
		bool redraw;
		int	 x;
		int  y;
		bool focus;
		int  positive;
		CImage			image;
		CImage			bgImage;
		CImage			fmImage;
		
		BOOL 	properchanged;
		BOOL	usebit;
		WORD	bit;
		
		char 	szPrompt0[tmMAX_TEXT_COL*2];
		
		WORD 	startIndex;
		
		BOOL    replaceImage;
		CImage 	selectImage;
		CImage 	unSelectImage;
		CtmBitMap m_bmpImage[2];
};
