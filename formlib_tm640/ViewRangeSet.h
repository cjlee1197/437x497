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


void		ShowRange();							//show���`�P�d��B�ʽd��(����Ѽƶ}�l��index , ��or�^��)
long		GetBDValuewithIndex(int Index);
void		SetUnit(int UnitType);																				//�]�w���	�H�Τp���I���				(��쫬�A(0 = ���� , 1 = �^��))
void		SetRangeValue(CtmWnd* u_pwnd_tempEdit ,int DataType ,int	editNO ,BOOL Type);		//Type = TRUE -> SAVE DB   Type = FALSE -> �٭�Range
void		ShowDHValue();		//Type = TRUE -> SAVE DB   Type = FALSE -> �٭�DH
void	GetDH_Param();
char *mystrtok1(char *str, char *ctrl);

#endif
