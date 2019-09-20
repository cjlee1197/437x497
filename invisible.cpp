/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include	"invisible.h"

/*===========================================================================+
|           Class implementation - StaticBox                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmInvisible::CtmInvisible() : CtmWnd()
{
tmWND         wnd = {NULL, tmWS_BORDER_NONE|tmWS_TRANSPARENT, {0, 0, 0, 0}, NULL, &m_pcFont};

SetBorderWidth(0);
CtmWnd::Create(&wnd);
}

CtmInvisible::~CtmInvisible()
{
}
