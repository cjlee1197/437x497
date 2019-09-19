/*============================================================================+
|  Class    : ViewRangeSet library                        	  	  	     			|
|  Task     : ViewRangeSet library  header file          	 	  	         	 		|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#ifndef		D__VIEWRANGESET
#define		D__VIEWRANGESET

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);


void		ShowRange();							//show關節與卡氏運動範圍(控制參數開始的index , 公or英制)
long		GetBDValuewithIndex(int Index);
void		SetUnit(int UnitType);																				//設定單位	以及小數點位數				(單位型態(0 = 公制 , 1 = 英制))
void		SetRangeValue(CtmWnd* u_pwnd_tempEdit ,int DataType ,int	editNO ,BOOL Type);		//Type = TRUE -> SAVE DB   Type = FALSE -> 還原Range
void		ShowDHValue();		//Type = TRUE -> SAVE DB   Type = FALSE -> 還原DH
void	GetDH_Param();
char *mystrtok1(char *str, char *ctrl);

#endif
