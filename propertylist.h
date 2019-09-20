/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : Porperty		   				  								|
|  Task     : Porperty header file          			      				|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                   |
|  Link     :G++(GCC) 3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James		                                                  	|
|  Version  : V1.00                                                         |
|  Creation : 2010/08/11                                        	        |
|  Revision :                                                               |
+==========================================================================*/


#ifndef D_PORPERTY
#define D_PORPERTY

#include	"parsepule.h"


#define		CLASS_MAXCOUNT				29
/*===========================================================================+
|           Wnddefine	Property_Name                                        |
+===========================================================================*/
#define		WND_START			0
#define		WND_PRIVILEGE		WND_START + 0
#define		WND_LEFT			WND_START + 1
#define		WND_TOP				WND_START + 2
#define		WND_RIGHT			WND_START + 3
#define		WND_BOTTOM			WND_START + 4
#define		WND_WTYPE			WND_START + 5
#define		WND_TABORDER		WND_START + 6
#define		WND_FGC				WND_START + 7
#define		WND_BGC				WND_START + 8
#define		WND_TITLE			WND_START + 9
#define		WND_TITLEID			WND_START + 10
#define		WND_FONTSIZE		WND_START + 11
#define		WND_TITLEWIDTH		WND_START + 12
#define		WND_TITLEHEIGHT		WND_START + 13
#define		WND_LEFTCONTROL		WND_START + 14
#define		WND_RIGHTCONTROL	WND_START + 15
#define		WND_UPCONTROL		WND_START + 16
#define		WND_DOWNCONTROL		WND_START + 17
#define		WND_VISIBLE			WND_START + 18
#define		WND_ENABLED			WND_START + 19
#define		WND_EXTAG			WND_START + 20
#define		WND_COLORCONTROL	WND_START + 21
#define		WND_COLORCHANGE		WND_START + 22
#define		WND_DESTCOLOR		WND_START + 23
#define		WND_BORDERCOLOR		WND_START + 24
#define		WND_AIOP			WND_START + 25
#define		WND_AIWND			WND_START + 26
#define		WND_AIID			WND_START + 27
#define		WND_AIVALUE			WND_START + 28
#define		WND_AIDVALUE		WND_START + 29
#define		WND_AISVALUE		WND_START + 30
#define		WND_AIMSGID			WND_START + 31
#define		WND_AILPARAM		WND_START + 32
#define		WND_AIWPARAM		WND_START + 33
#define		WND_AIVALUE0		WND_START + 34
#define		WND_AIVALUE1		WND_START + 35
#define		WND_AIVALUE2		WND_START + 36
#define		WND_AIVALUE3		WND_START + 37
#define		WND_AIVALUE4		WND_START + 38
#define		WND_AIVALUE5		WND_START + 39
#define		WND_AIVALUE6		WND_START + 40
#define		WND_AIVALUE7		WND_START + 41
#define		WND_AIVALUE8		WND_START + 42
#define		WND_AIVALUE9		WND_START + 43
#define		WND_END				WND_AIVALUE9

/*===========================================================================+
|           MainFrmdefine	Property_Name                                    |
+===========================================================================*/
#define		MAINFRM_TOOLBAR		WND_END + 1

/*===========================================================================+
|           Statusbar 	Property_Name                                    	|
+===========================================================================*/
#define		STATUSBAR_UPDATEA		WND_END + 1
#define		STATUSBAR_CREATEA		WND_END + 2
#define		STATUSBAR_DESTROYA		WND_END + 3
#define		STATUSBAR_MESSAGEA		WND_END + 4

/*===========================================================================+
|           ImageBox 	Property_Name                                    	|
+===========================================================================*/
#define		IMAGEBOX_TYPE			WND_END + 1
#define		IMAGEBOX_STYPE			WND_END + 2
#define		IMAGEBOX_IMAGEPATH		WND_END + 3
#define		IMAGEBOX_KEYCOLOR		WND_END + 4
#define		IMAGEBOX_DOUBLEBUFFER	WND_END + 5
#define		IMAGEBOX_CLICKVIEW		WND_END + 6


/*===========================================================================+
|           CtmEdit 	Property_Name                                    	|
+===========================================================================*/
#define		EDIT_TYPE				WND_END + 1
#define		EDIT_PROMPT				WND_END + 2
#define		EDIT_PROMPTID			WND_END + 3
#define		EDIT_MAXCHAR			WND_END + 4
#define		EDIT_ECHO				WND_END + 5
#define		EDIT_TEXT				WND_END + 6
#define		EDIT_ALIGN				WND_END + 7
#define		EDIT_BKSP				WND_END + 8

#define		EDIT_END				EDIT_ALIGN

/*===========================================================================+
|           CtmEditX1 	Property_Name                                    	|
+===========================================================================*/
#define		EDITX1_PRECISION		EDIT_END + 1
#define		EDITX1_VALUE			EDIT_END + 2
#define		EDITX1_MIN				EDIT_END + 3
#define		EDITX1_MAX				EDIT_END + 4
#define		EDITX1_MINID			EDIT_END + 5
#define		EDITX1_MAXID			EDIT_END + 6
#define		EDITX1_MAXDB			EDIT_END + 7
#define		EDITX1_MINDB			EDIT_END + 8
#define		EDITX1_MAXEDITNAME		EDIT_END + 9
#define		EDITX1_MINEDITNAME		EDIT_END + 10
#define		EDITX1_SPEPRE			EDIT_END + 11
#define		EDITX1_DBID0			EDIT_END + 12
#define		EDITX1_DBID9			EDIT_END + 21

/*===========================================================================+
|           CtmEditX2 	Property_Name                                    	|
+===========================================================================*/
#define		EDITX2_VALUE			EDIT_END + 1
#define		EDITX2_MIN				EDIT_END + 2
#define		EDITX2_MAX				EDIT_END + 3
#define		EDITX2_DBID0			EDIT_END + 4
#define		EDITX2_DBID9			EDIT_END + 13

/*===========================================================================+
|           Cartoon 	Property_Name                                    	|
+===========================================================================*/
#define		CARTOON_SHOWTYPE			WND_END + 1
#define		CARTOON_CARTOONMIN			WND_END + 2
#define		CARTOON_CARTOONMAX			WND_END + 3
#define		CARTOON_CARTOONMINID		WND_END + 4
#define		CARTOON_CARTOONMAXID		WND_END + 5
#define		CARTOON_FRAMECOUNT			WND_END + 6
#define		CARTOON_BMPINDEX			WND_END + 7
#define		CARTOON_FRAMEBMPPATH0		WND_END + 8
#define		CARTOON_FRAMEBMPMIN0		WND_END + 9
#define		CARTOON_FRAMEBMPMAX0		WND_END + 10
#define		CARTOON_FRAMEBMPPATH22		WND_END + 71
#define		CARTOON_FRAMEBMPMIN22		WND_END + 72
#define		CARTOON_FRAMEBMPMAX22		WND_END + 73

/*===========================================================================+
|           StaticX2 	Property_Name                                    	|
+===========================================================================*/
#define		STATICX2_ALIGN				WND_END + 1
#define		STATICX2_TEXT				WND_END + 2
#define		STATICX2_TEXTID				WND_END + 3
#define		STATICX2_TEXTID9			WND_END + 13

/*===========================================================================+
|           Toolbar 	Property_Name                                    	|
+===========================================================================*/
#define		TOOLBAR_BMPBACKPATH			WND_END + 1
#define		TOOLBAR_BMPUPPATH			WND_END + 2
#define		TOOLBAR_BMPDOWNPAT			WND_END + 3
#define		TOOLBAR_BMPMAINTOOL			WND_END + 4
#define		TOOLBAR_ONKEYA				WND_END + 5
#define		TOOLBAR_ONKEYAEND			WND_END + 6
#define		TOOLBAR_ONMESSAGE			WND_END + 7
#define		TOOLBAR_KEYCOLOR			WND_END + 8

/*===========================================================================+
|           Button 	Property_Name                                    	|
+===========================================================================*/
#define		BUTTON_BUTTONTYPE			WND_END + 1
#define		BUTTON_ALIGN				WND_END + 2
#define		BUTTON_SPACE				WND_END + 3
#define		BUTTON_CAPTION				WND_END + 4
#define		BUTTON_CAPTIONID			WND_END + 5
#define		BUTTON_UPBITMAP				WND_END + 6
#define		BUTTON_DOWNBITMAP			WND_END + 7
#define		BUTTON_CAPTIONID0			WND_END + 8
#define		BUTTON_CAPTIONID9			WND_END + 17
#define		BUTTON_KEYCOLOR				WND_END + 18
#define		BUTTON_CAPTIONTYPE			WND_END + 19
#define		BUTTON_END					BUTTON_CAPTIONTYPE

/*===========================================================================+
|           ToolButton 	Property_Name                                    	|
+===========================================================================*/
#define		TOOLBUTTON_KEY				BUTTON_END + 1
#define		TOOLBUTTON_GROUP			BUTTON_END + 2
#define		TOOLBUTTON_ACTION			BUTTON_END + 3
#define		TOOLBUTTON_FORMNAME			BUTTON_END + 4
#define		TOOLBUTTON_BARNAME			BUTTON_END + 5

/*===========================================================================+
|           ValueInt 	Property_Name                                    	|
+===========================================================================*/
#define		VALUEINT_VALUE0				WND_END + 1
#define		VALUEINT_VALUE255			WND_END + 256
#define		VALUEINT_DEFAULT			WND_END + 257
/*===========================================================================+
|           ValueStr 	Property_Name                                    	|
+===========================================================================*/
#define		VALUESTR_VALUE0				WND_END + 1
#define		VALUESTR_VALUE255			WND_END + 256
#define		VALUESTR_DEFAULT			WND_END + 257

/*===========================================================================+
|           View 	Property_Name                                    	|
+===========================================================================*/
#define		VIEW_OVERVIEWNAME			WND_END + 1
#define		VIEW_BMPBACKPATH			WND_END + 2

/*===========================================================================+
|           Splash 	Property_Name                                    	|
+===========================================================================*/
#define		SPLASH_LOGOPATH				WND_END + 1

/*===========================================================================+
|          	LineTable 	Property_Name                                    	|
+===========================================================================*/
#define		LINETABLE_MAINLINEWIDTH		WND_END + 1
#define		LINETABLE_STYPE				WND_END + 2
#define		LINETABLE_MAINCOLOR_R		WND_END + 3
#define		LINETABLE_MAINCOLOR_G		WND_END + 4
#define		LINETABLE_MAINCOLOR_B		WND_END + 5
#define		LINETABLE_ACCRCOLOR1_R		WND_END + 6
#define		LINETABLE_ACCRCOLOR1_G		WND_END + 7
#define		LINETABLE_ACCRCOLOR1_B		WND_END + 8
#define		LINETABLE_ACCRCOLOR2_R		WND_END + 9
#define		LINETABLE_ACCRCOLOR2_G		WND_END + 10
#define		LINETABLE_ACCRCOLOR2_B		WND_END + 11
#define		LINETABLE_VERTROWS			WND_END + 12
#define		LINETABLE_HORICOLS			WND_END + 13
#define		LINETABLE_VERTPOINTS0		WND_END + 14
#define		LINETABLE_HORIPOINTS0		WND_END + 15
#define		LINETABLE_VERTPOINTS19		WND_END + 52
#define		LINETABLE_HORIPOINTS19		WND_END + 53

/*===========================================================================+
|          	SelectEdit 	Property_Name                                    	|
+===========================================================================*/
#define		SELECTEDIT_DBID0			WND_END + 1
#define		SELECTEDIT_DBID9			WND_END + 10
#define		SELECTEDIT_STARTVALUE		WND_END + 11
#define		SELECTEDIT_MAXINDEX			WND_END + 12
#define		SELECTEDIT_STR0				WND_END + 13
#define		SELECTEDIT_STRID0			WND_END + 14
#define		SELECTEDIT_STR19			WND_END + 51
#define		SELECTEDIT_STRID19			WND_END + 52
#define		SELECTEDIT_MAXCHAR			WND_END + 53
#define		SELECTEDIT_STYPE			WND_END + 54
#define		SELECTEDIT_PROMPT			WND_END + 55
#define		SELECTEDIT_PROMPTID			WND_END + 56
#define		SELECTEDIT_FOCUSCOLOR		WND_END + 57
#define		SELECTEDIT_LISTNUM			WND_END + 58
#define		SELECTEDIT_END				SELECTEDIT_LISTNUM
/*===========================================================================+
|          	SelectBit 	Property_Name                                    	|
+===========================================================================*/
#define		SELECTBIT_DBID0				SELECTEDIT_END + 1
#define		SELECTBIT_BITINDEX			SELECTEDIT_END + 2
#define		SELECTBIT_BITVALUE			SELECTEDIT_END + 3

/*===========================================================================+
|          	Curve 	Property_Name                                    	|
+===========================================================================*/
#define		CURVE_CURVETYPE			WND_END + 1
#define		CURVE_XDIGITS			WND_END + 2
#define		CURVE_YDIGITS			WND_END + 3
#define		CURVE_COLS				WND_END + 4
#define		CURVE_ROWS				WND_END + 5
#define		CURVE_CURVENAME			WND_END + 6
#define		CURVE_CURVENAMEID		WND_END + 7
#define		CURVE_XNAME				WND_END + 8
#define		CURVE_YNAME				WND_END + 9
#define		CURVE_XNAMEID			WND_END + 10
#define		CURVE_YNAMEID			WND_END + 11
#define		CURVE_XMIN				WND_END + 12
#define		CURVE_XMAX				WND_END + 13
#define		CURVE_YMIN				WND_END + 14
#define		CURVE_YMAX				WND_END + 15
#define		CURVE_XSTART			WND_END + 16
#define		CURVE_XEND				WND_END + 17
#define		CURVE_YSTART			WND_END + 18
#define		CURVE_YEND				WND_END + 19
#define		CURVE_XUNIT				WND_END + 20
#define		CURVE_YUNIT				WND_END + 21
#define		CURVE_LINECOLOR0		WND_END + 22
#define		CURVE_LINENAME0			WND_END + 23
#define		CURVE_LINENAMEID0		WND_END + 24
#define		CURVE_LINETYPE0			WND_END + 25
#define		CURVE_LINECOLOR9		WND_END + 58
#define		CURVE_LINENAME9			WND_END + 59
#define		CURVE_LINENAMEID9		WND_END + 60
#define		CURVE_LINETYPE9			WND_END + 61
#define		CURVE_END				CURVE_LINETYPE9

/*===========================================================================+
|          	CurveEX 	Property_Name                                    	|
+===========================================================================*/
#define		CURVEEX_DOUBLEBUFFER	WND_END + 1
#define		CURVEEX_CURVETYPE		WND_END + 2
#define		CURVEEX_XDIGITS			WND_END + 3
#define		CURVEEX_YDIGITS			WND_END + 4
#define		CURVEEX_COLS			WND_END + 5
#define		CURVEEX_ROWS			WND_END + 6
#define		CURVEEX_CURVENAME		WND_END + 7
#define		CURVEEX_CURVENAMEID		WND_END + 8
#define		CURVEEX_XNAME			WND_END + 9
#define		CURVEEX_YNAME			WND_END + 10
#define		CURVEEX_XNAMEID			WND_END + 11
#define		CURVEEX_YNAMEID			WND_END + 12
#define		CURVEEX_XMIN			WND_END + 13
#define		CURVEEX_XMAX			WND_END + 14
#define		CURVEEX_YMIN			WND_END + 15
#define		CURVEEX_YMAX			WND_END + 16
#define		CURVEEX_YMIN2			WND_END + 17
#define		CURVEEX_YMAX2			WND_END + 18
#define		CURVEEX_XSTART			WND_END + 19
#define		CURVEEX_XEND			WND_END + 20
#define		CURVEEX_YSTART			WND_END + 21
#define		CURVEEX_YEND			WND_END + 22
#define		CURVEEX_XUNIT			WND_END + 23
#define		CURVEEX_YUNIT			WND_END + 24
#define		CURVEEX_XNAMESHOW		WND_END + 25
#define		CURVEEX_YNAMESHOW		WND_END + 26
#define		CURVEEX_XARROWSHOW		WND_END + 27
#define		CURVEEX_YARROWSHOW		WND_END + 28
#define		CURVEEX_XAXESTYPE		WND_END + 29
#define		CURVEEX_YAXESTYPE		WND_END + 30
#define		CURVEEX_XSCALETYPE		WND_END + 31
#define		CURVEEX_YSCALETYPE		WND_END + 32
#define		CURVEEX_TITLETYPE		WND_END + 33
#define		CURVEEX_CUTLINETYPE		WND_END + 34
#define		CURVEEX_XGRIDCOLOR		WND_END + 35
#define		CURVEEX_YGRIDCOLOR		WND_END + 36
#define		CURVEEX_XAXESCOLOR		WND_END + 37
#define		CURVEEX_YAXESCOLOR		WND_END + 38
#define		CURVEEX_REFERENCE		WND_END + 39
#define		CURVEEX_YRULECOLOR		WND_END + 40
#define		CURVEEX_XRULECOLOR		WND_END + 41
#define		CURVEEX_XMOVE			WND_END + 42
#define		CURVEEX_YMOVE			WND_END + 43
#define		CURVEEX_PROMPT			WND_END + 44
#define		CURVEEX_PROMPTID		WND_END + 45
#define		CURVEEX_LINECOLOR0		WND_END + 46
#define		CURVEEX_LINENAME0		WND_END + 47
#define		CURVEEX_LINENAMEID0		WND_END + 48
#define		CURVEEX_LINETYPE0		WND_END + 49
#define		CURVEEX_LINECOLOR9		WND_END + 82
#define		CURVEEX_LINENAME9		WND_END + 83
#define		CURVEEX_LINENAMEID9		WND_END + 84
#define		CURVEEX_LINETYPE9		WND_END + 85
/*===========================================================================+
|          	SelectBox 	Property_Name                                    	|
+===========================================================================*/
#define		SELECTBOX_SELECTED		WND_END + 1
#define		SELECTBOX_TEXT			WND_END + 2
#define		SELECTBOX_PROMPT		WND_END + 3
#define		SELECTBOX_TEXTID		WND_END + 4
#define		SELECTBOX_PROMPTID		WND_END + 5
#define		SELECTBOX_GROUP			WND_END + 6

/*===========================================================================+
|          	Dialog	Property_Name                                    		|
+===========================================================================*/
//#define		DIALOG_TITLE			WND_END + 1
//#define		DIALOG_TITLEID			WND_END + 2
#define		DIALOG_BMPBACKPATH		WND_END + 1
#define		DIALOG_END				DIALOG_BMPBACKPATH

/*===========================================================================+
|          	CtmCheckBox 	Property_Name                                   |
+===========================================================================*/
#define		CHECKBOX_TEXT			WND_END+1
#define		CHECKBOX_TEXTID			WND_END+2
#define		CHECKBOX_PROMPT			WND_END+3
#define		CHECKBOX_PROMPTID		WND_END+4
#define		CHECKBOX_CHECEKED		WND_END+5

/*===========================================================================+
|          	CtmProgressbar 	Property_Name                                   |
+===========================================================================*/
#define		PROGRESSBAR_DBID0		WND_END+1
#define		PROGRESSBAR_MAXID		WND_END+2
#define		PROGRESSBAR_MINID		WND_END+3
#define		PROGRESSBAR_VALUE		WND_END+4
#define		PROGRESSBAR_MIN			WND_END+5
#define		PROGRESSBAR_MAX			WND_END+6
#define		PROGRESSBAR_POSITION	WND_END+7
#define		PROGRESSBAR_DBLIMIT		WND_END+8
#define		PROGRESSBAR_BARTYPE		WND_END+9

/*===========================================================================+
|          	CtmMemo 	Property_Name                                  		 |	
+===========================================================================*/
#define		MEMO_FOCUSCOLOR			WND_END+1
#define		MEMO_STARTSTRINDEX		WND_END+2
#define		MEMO_FOCUSINDEX			WND_END+3
#define		MEMO_FOCUSSTRINDEX		WND_END+4
#define		MEMO_BAR				WND_END+5
#define		MEMO_STR0				WND_END+6
#define		MEMO_STRID0				WND_END+7
#define		MEMO_STR19				WND_END+44
#define		MEMO_STRID19			WND_END+45

/*===========================================================================+
|          	CtmScrollBar 	Property_Name                                  		 |	
+===========================================================================*/
#define		SCROLLBAR_BARRANGE		WND_END+1
#define		SCROLLBAR_POSITION		WND_END+2
#define		SCROLLBAR_TYPE			WND_END+3

/*===========================================================================+
|          	FormView 	Property_Name                                    	|
+===========================================================================*/
#define		FORMVIEW_ONCHANGEA			DIALOG_END + 1
#define		FORMVIEW_ONCHANGEBM			DIALOG_END + 2
#define		FORMVIEW_ONCREATEA			DIALOG_END + 3
#define		FORMVIEW_ONSHOWA			DIALOG_END + 4
#define		FORMVIEW_ONKEYA				DIALOG_END + 5
#define		FORMVIEW_ONUPDATEA			DIALOG_END + 6
#define		FORMVIEW_ONDESTROYA			DIALOG_END + 7
#define		FORMVIEW_ONMESSAGE			DIALOG_END + 8
#define		FORMVIEW_PAGENO				DIALOG_END + 9
#define		FORMVIEW_ONUPDATEB			DIALOG_END + 10
#define		FORMVIEW_PRINTNAME			DIALOG_END + 11
#define		FORMVIEW_CANSTOP			DIALOG_END + 12
#define		FORMVIEW_PRIVATEUPDATE		DIALOG_END + 13
#define		FORMVIEW_SAVEKEY			DIALOG_END + 14


typedef struct 	tagClassInfo
{
	DWORD 	dwClassInstanceAdd;
	DWORD	dwClassInstanceLen;
}CLASSINFO;

typedef	struct	tagClassProperty
{
	char*	pszProperty;
	DWORD	dwPropType;
}CLASSPROPERTY;

typedef	struct	tagClassList
{
	char**	pszClassName;
	CLASSPROPERTY*	pszClassProperty[CLASS_MAXCOUNT];
	DWORD	dwPropertyCount[CLASS_MAXCOUNT];
}CLASSLIST;

extern		CLASSLIST		tm_ClassList;
extern		char*			u_cszClassName[];
extern		CLASSPROPERTY	tm_cszWnd[];
extern		CLASSPROPERTY	tm_cszEdit[];
extern		CLASSPROPERTY	tm_cszEditX1[];
extern		CLASSPROPERTY	tm_cszEditX2[];
extern		CLASSPROPERTY	tm_cszSelectEdit[];
extern		CLASSPROPERTY	tm_cszSelectBox[];
extern		CLASSPROPERTY	tm_cszSelectBit[];
extern		CLASSPROPERTY	tm_cszStaticX2[];
extern		CLASSPROPERTY	tm_cszButton[];
extern		CLASSPROPERTY	tm_cszToolButton[];
extern		CLASSPROPERTY	tm_cszImageBox[];
extern		CLASSPROPERTY	tm_cszToolBar[];
extern		CLASSPROPERTY	tm_cszPromptBar[];
extern		CLASSPROPERTY	tm_cszStatusBar[];
extern		CLASSPROPERTY	tm_cszMainFrame[];
extern		CLASSPROPERTY	tm_cszView[];   
extern		CLASSPROPERTY	tm_cszFormView[];
extern		CLASSPROPERTY	tm_cszDialog[];  
extern		CLASSPROPERTY	tm_cszWndSplash[];
extern		CLASSPROPERTY	tm_cszCartoon[];  
extern		CLASSPROPERTY	tm_cszValueInt[]; 
extern		CLASSPROPERTY	tm_cszLineTable[]; 
extern		CLASSPROPERTY	tm_cszCurve[];    
extern		CLASSPROPERTY	tm_cszCurveEx[];   
extern      CLASSPROPERTY	tm_cszValueStr[];
extern		CLASSPROPERTY	tm_cszProgressbar[];
extern		CLASSPROPERTY	tm_cszCheckBox[];
extern		CLASSPROPERTY	tm_cszProgressbar[];
extern		CLASSPROPERTY	tm_cszMemo[];
extern		CLASSPROPERTY	tm_cszScrollBar[];
//
void		SetPropertyListValue();

#endif
