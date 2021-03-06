/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include	"static.h"
#include	"edit.h"
#include	"stable.h"
#include	"dialog.h"
#include	"panel.h"
#include	"init.h"
#include	"wndx2.h"
#include	"formview.h"



typedef struct tagMSGBOX
    {
    WORD        wType;
    char*       psz;
    } tmMSGBOX;
/*===========================================================================+
|           Class declaration - MsgBox                                       |
+===========================================================================*/
class CtmMsgBox : public CtmFormView
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmMsgBox)
public:
    CtmMsgBox();
    ~CtmMsgBox();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
  

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
};
