/*============================================================================+
|  Class    : ViewMessageBoxTeachPoint library                               |
|  Task     : ViewMessageBoxTeachPoint library  header file									|
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
#ifndef D__VIEWMSGBOXTHREEPOINTTOWORKC
#define D__VIEWMSGBOXTHREEPOINTTOWORKC

#include	"../formview.h"
#include	"../utils.h"

extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void		OnUpdateA(CtmWnd* pwndSender);

double	CnangeMMtoIN(double f_value);		//公轉英 且精度+1
void		UpdateWorkCoord();
double	pow2(double a);
void		SetDB_WorkCoordValue(int NO);
void		AbsPos2MachinePos(LONG_AXIS u_AbsPos,double outMachinePos[3]);
void 		LONG_AXIS2double(LONG_AXIS Pos , double outPos[6] , double unit);
void  	CartesianToMatrix(double pos[6] , double E[4][4]);
void 		Inv_matrix_4( double Mtrix_A[][4], double Inv_Mtrix_A[][4]);
void 		mt_DHmul(double A[4][4], double B[4][4], double output[4][4]);

#endif
