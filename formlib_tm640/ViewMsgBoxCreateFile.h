/*============================================================================+
|  Class    : statusbar library                                             	|
|  Task     : statusbar library  header file                                 	|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		          	                                           	|
|  Version  : V1.00                                                          	|
|  Creation : 			    		                                                 	|
|  Revision :       		   					                                         	|
+============================================================================*/

#ifndef D__MESSAGEBOXCREATEFILE
#define D__MESSAGEBOXCREATEFILE

#include	"../formview.h"

//extern	char				u_sOperate_FileName_Source[64];
//extern	char				u_sOperate_FileName_Object[64];

extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C" WORD		OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	void		OnUpdateA(CtmWnd* pwndSender);

void		ShowDocument(int nStartNo);
void		SaveDocument();

#endif