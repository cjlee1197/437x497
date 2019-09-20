/*Created by J.Wong 2017/2/10 16:56:09*/
#pragma once
#include <guilite/IGui>
#include "common.h"
#include "wnd.h"

class CTaskCursor;

class CSpinBoxPrivate{
	public:
		CSpinBoxPrivate();
		~CSpinBoxPrivate();
		void init();

		int64 llValue;
		int64 llMinValue;
		int64 llMaxValue;
		int64 llReInputValue;
		
		int64 llLastValue;
		
		
		double dValue;
		double dMinValue;
		double dMaxValue;
		double dReInputValue;
		double dLastValue;
		
		/*显示进制*/
		uint16 displayIntegerBase;
		/*添加后缀信息*/
		
		char  preffix;
		/*步进大小*/
		int  	tickInterval;
		double  dTickInterval;
		
		BOOL touched;
		BOOL transparent;
		BOOL oldTransparent;
		WORD wFrameShadow;
		WORD wFrameShape;
		WORD wOldFrameShadow;
		WORD wOldFrameShape;
		BOOL hiddenTick;
		CSize size;
		CMargin mgn;
		WORD wAlign;
		WORD tickStyle;
		int  x;
		int  y;
		bool focus;	
		int  positive;	
		CImage			image;
		CImage			bgImage;
		CImage			fmImage;
		
		uint	linecolor0;
		uint 	oldLineColor0;
		uint	linecolor1;
		uint 	destcolor;
		uint	oldDestColor;
		float	colorAt;
		float 	oldColorAt;
		bool	pressed;
		int     btnIndex;
		WORD	wGradientDirection;
		WORD	wGradientControl;
		WORD	wOldGradientDirection;
		WORD	wOldGradientControl;
		
		
		BOOL 	maxdb;
		BOOL 	mindb;
		WORD	precision;
		int8	curdecimal;
		WORD 	showBits;
		int		integerpart;
		int		decimalpart;
		
		CTaskCursor* ptaskCursor;
		
		uint8 nKeyIndex;
		uint8 lastacKey;
		int16 nFontSize;
		CRect ftRect;
		bool  symbols;
		bool  dotsymbols;
		
		
		char  szPrompt0[tmMAX_TEXT_COL * 2];
		
		CRect btnRect[2];
		
		bool  hint;
		BOOL  properchanged;
		bool coordchanged;
		BOOL oldEnabled;
		uint oldBgc;
};