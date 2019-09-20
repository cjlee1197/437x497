/* Copyright (C), 1997-2002, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Database                                                       |
|  Task     : Database header file                                           |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 01/01/1997                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include	"wnd.h"
#include	"cmddef.h"

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/

tmWNDATTR	g_wndattrDefault = 
{
            WHITE,                      // fgc
            RGB2PIXEL(40, 72, 93),      // bgc
            RGB2PIXEL(16, 42, 55),      // fgcBorder1
            WHITE,                      // fgcBorder2
            BRIGHTWHITE,                // fgcBorder3
            GRAY,                       // fgcBorder4
            BLACK,                      // fgcShadow
            YELLOW,                     // fgcTitle
            BLUE,                       // bgcTitle
            RGB2PIXEL(16, 42, 55),      // fgcEdit1
            RGB2PIXEL(215, 224, 231),   // bgcEdit1
            RGB2PIXEL(16, 42, 55),      // fgcEdit2
            RGB2PIXEL(170,179,170),   // bgcEdit2
            BRIGHTWHITE,                // fgcOptionBox1
            GRAY,                       // fgcOptionBox2
            BRIGHTWHITE,                // fgcOptionBox3
            BLACK,                      // fgcOptionBox4
            BRIGHTWHITE,                // fgcCheckBox1
            GRAY,                       // fgcCheckBox2
            BRIGHTWHITE,                // fgcCheckBox3
            BLACK,                      // fgcCheckBox4
            WHITE,                      // bgcMeter
            BRIGHTWHITE,                // bgcGauge1
            WHITE,                      // bgcGauge2
            LIGHTGRAY,                  // bgcScrollBar
            RGB2PIXEL(215, 224, 231),		// bgcListBox1
            YELLOW,//BRIGHTWHITE,                // fgcListBox2
            BLUE,                       // bgcListBox2
            BRIGHTWHITE,    //BROWN,    // bgcChart
            RGB2PIXEL(0x99, 0x99, 0x99),// fgcChartScale1
            RGB2PIXEL(0x99, 0x99, 0x99),// fgcChartScale2
            RGB2PIXEL(0x99, 0x99, 0x99),// fgcChartScale3
            BLACK,                      // fgcMenuBar
            WHITE,                      // bgcMenuBar
            BLACK,                      // fgcToolBar1
            WHITE,                      // bgcToolBar1
            BLACK,                     // fgcToolBar2
            BLUE,                       // bgcToolBar2
            BLACK,                      // fgcStatBar
            GRAY,                       // bgcStatBar
            BRIGHTWHITE,                      // fgcView
            BROWN,  //BRIGHTWHITE,                // bgcView
                        BLACK,                      // fgcSelectEdit1<LEO20060714 color for select edit
            RGB2PIXEL(215, 224, 231),   // bgcSelectEdit1<LEO20060714 color for select edit
            YELLOW,                     // fgcSelectEdit2<LEO20060714 color for select edit
            BLUE,                       // bgcSelectEdit2<LEO20060714 color for select edit

            1,                          //<LEO20060714 color for select edit
            {tmPARA_ALIGN_LEFT},          // paragraph
            1,  //4                        // wBorderWidth 
            1,                          // wBorderWidthStatic
            1,                          // wBorderWidthEdit
            1,                          // wBorderWidthOptionBox
            1,                          // wBorderWidthCheckBox
            1,                          // wBorderWidthMeter
            1,                          // wBorderWidthGauge
            0,                          // wBorderWidthGroupBox
            0,                          // wBorderWidthPanel
            1,                          // wBorderWidthButton
            0,                          // wBorderWidthScrollBar
            1,                          // wBorderWidthListBox
            0,                          // wBorderWidthCombo
            0,                          // wBorderWidthTable
            0,                          // wBorderWidthBitMap
            0,                          // wBorderWidthDrawing
            0,                          // wBorderWidthChart
            0,                          // wBorderWidthAnimator
            0,                          // wBorderWidthMsgBox
            2,                          // wWidthFocus   4
            13                          // wWidthCursor
};
