/*==========================================================================+
|  Class    : Class TouchScreen	                                            |
|  Task     : Class TouchScreen file                          				|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2006/12/21                                                    |
|  Revision : 2006/12/21                                                    |
+==========================================================================*/
#include "touchscreen.h"


CTouchScreen	*g_pTouch;    //netdigger 2007/4/17

IMPLEMENT_DYNCREATE(CTouchScreen, CtmPackClass)


CTouchScreen::CTouchScreen() : CtmPackClass()
{
	
}

CTouchScreen::~CTouchScreen()
{
	
}

