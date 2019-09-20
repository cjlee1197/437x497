/*==========================================================================+
|  Class    :                                                  				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 童宇揚                                                        |
|  Version  : V1.00                                                         |
|  Creation : 2004/05/11                                                    |
|  Remark	: 初始化各種資源。		                                        |
+==========================================================================*/
#include	<iostream>
#include	"init.h"
#include	"tmdebug.h"
using namespace std;

/*==========================================================================+
|	字體的全局變量															|
+==========================================================================*/
CtmBlock*			g_pBlock		= NULL;							//<<yang 2006/12/8 

tmINIBMP			g_aIniBmp[IMAGE_MAXNUM] = {
	{ tmBMP_OVERVIEW,			NULL},
	{ tmBMP_BACKGROUND, 		NULL},
	{ PROMPT_BAR_DIR,			NULL},
	{"PromptBarBGR.bmp",		NULL},	//	fans add 2007/8/7 07:46下午
	{ STATE_BAR_DIR,			NULL},
	{ tmTOOLBAR_BACKGROUND_DIR, NULL},
	{ tmBMP_TITLE,				NULL},
	{ tmBMP_ACTUALBG,			NULL},
	{ tmBMP_ACTUALBGW,		    NULL},
	{ "SubToolBar.bmp",		    NULL},
	{ NULL,						NULL}
	};
	
tmINIGIF			g_aIniGif[IMAGE_MAXNUM];

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

IMPLEMENT_DYNCREATE(CInit, CtmPackClass)

CInit*	g_pInit = NULL;

CInit::CInit() : CtmPackClass()
{

}

CInit::~CInit()
{

}