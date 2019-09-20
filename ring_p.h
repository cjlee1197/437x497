/*Created by J.Wong 2017/1/18 11:04:45*/
#pragma once
#include <guilite/IGui>
#include "common.h"
#include "wnd.h"

class CRingPrivate
{
public:
	CRingPrivate();
	~CRingPrivate();
	
	void 	init();
	
	float  	percent;
	float   oldPercent;
	float  	tickInterval;
	
	BOOL 	touched;
	BOOL	transparent;
	BOOL	oldTransparent;
	BOOL  	clockwise;
	uint  	leftColor;
	uint  	color;
	uint  	penWidth;
	CMargin mgn;
	CSize	size;
	int 	x;
	int 	y;
	int 	positive;
	
	bool   focus;
	
	bool 	redraw;
	
	float  lastPercent;
	float  oldLastPercent;
	uint32 lastColor;
	uint   lastPenWidth;
	
	CImage	bgImage;
	CImage	fmImage;
	CImage	image;
	
	BOOL properchanged;
	
	
	BOOL  maxdb;
	BOOL  mindb;
	
	
	
	BOOL  lastmaxdb;
	BOOL  lastmindb;
	
	
	WORD  precision;
	
	
	char szPrompt0[tmMAX_TEXT_COL*2];
	
	long long llValue;
	long long llMaxValue;
	long long llMinValue;
	
	long long llLastValue;
	long long llLastMaxValue;
	long long llLastMinValue;
	
	BOOL showPercent;
	
		
};