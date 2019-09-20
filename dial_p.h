/*Created by J.Wong 2017/1/20 9:52:49*/
#pragma once
#include <guilite/IGui>
#include <guilite/IUtils>
#include "common.h"
#include "wnd.h"

class CDialPrivate
{
public:
	CDialPrivate();
	~CDialPrivate();
	void init();
	
	
	float  	value;
	float	min;
	float	max;
	float  	tickInterval;
	float 	oldValue;
	
	BOOL 	touched;
	BOOL	transparent;
	BOOL	oldTransparent;
	BOOL  	clockwise;
	BOOL 	oldClockwise;
	
	uint	innerColor;
	uint	outerColor;
	uint  	color;
	float	spanAngle;
	uint	oldSpanAngle;
	
	WORD	scaleDiv;
	
	
	uint  	penWidth;
	
	CMargin mgn;
	CSize	size;
	int 	x;
	int 	y;
	bool    focus;
	bool 	redraw;
	int 	positive;
	
	CImage	bgImage;
	CImage	fmImage;
	CImage	image;
	
	BOOL properchanged;
	
	
	BOOL maxdb;
	BOOL mindb;
	WORD precision;
	
	WORD wDialStyle;
	WORD wOldDialStyle;
	WORD offset;
	
	char szPrompt0[tmMAX_TEXT_COL*2];
};