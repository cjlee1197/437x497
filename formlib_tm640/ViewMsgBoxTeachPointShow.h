/*============================================================================+
|  Class    : ViewMessageBoxCommandExplanation library                   	    |
|  Task     : ViewMessageBoxCommandExplanation library  header file						|
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

//Mario rewrite
#ifndef D__MESSAGEBOXTEACHPOINTSHOW
#define D__MESSAGEBOXTEACHPOINTSHOW

#include	"../formview.h"

extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);

void		TeachPointCheck(WORD wStartNo);		//�P�_�|��ܴX���о��I���
void		UpdateTable(WORD wStartNo);				//��ܱо��I�C��
void		NoUseTablePart(WORD wPartNo);			//�N�S���ϥΪ��϶��M��
void		SetUnit();

#endif
