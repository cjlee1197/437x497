/*Created by J.Wong 2017/1/13 14:47:13*/
#pragma once
#include <guilite/IGui>
#include "common.h"
#include "wnd.h"


class CSliderPrivate{
public:
	CSliderPrivate();
	~CSliderPrivate();
	void init();
	float max;
	float min;
	float value;
	float oldValue;
	float tickInterval;
	
	CSize size;
	int   	penWidth;
	int   	outlineWidth;
	CMargin mgn;
//	CPainterPath path;
	
	uint slotColor;
	uint oldSlotColor;
	uint outlineColor;

	uint flowColor;
	
	float scaleMin;
	float scaleMax;
	float oldScaleMin;
	float oldScaleMax;
	BOOL orientation;
	BOOL oldOrientation;
	BOOL showOutline;
	BOOL oldShowOutline;
	BOOL showFlowCap;
	BOOL showSlotCap;
	BOOL oldShowSlotCap;
	BOOL showScale;
	BOOL oldShowScale;
	BOOL redrawSlot;	
	WORD flowDirection;
	WORD scaleStyle;
	WORD oldScaleStyle;
	BOOL touched;
	BOOL transparent;
	BOOL oldTransparent;
	
	uint scaleDiv;
	uint oldScaleDiv;
	int  x;
	int  y;
	bool focus;	
	int positive;	
	CImage			image;
	CImage			bgImage;
	CImage			fmImage;

	BOOL properchanged;

	
	char szPrompt0[tmMAX_TEXT_COL*2];
	
	BOOL  maxdb;
	BOOL  mindb;
	WORD  precision;
	
	
};