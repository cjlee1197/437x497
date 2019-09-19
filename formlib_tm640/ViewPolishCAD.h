/*============================================================================+
|  Class    : ViewManualMode library                        	    	     			|
|  Task     : ViewManualMode library  header file           	  	         	 	|
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
#ifndef		D__VIEWPOLISHCAD
#define		D__VIEWPOLISHCAD

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"

typedef struct tag_COORD_MEM{	
	double coord[3];
}COORD_MEM;

typedef struct tag_FILENAME FILENAME;
struct tag_FILENAME
{
	char*	name;
  FILENAME* next;    
};

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);




void 	FindCADFile();
void	SetSelectEditSTR(int no, char* str);
int 	CalPath();
char* GetCDFFileName();
int 	CreateProg();

void  MToC6_SixAxis(double E[4][4], double Pos[6]);
void	printf_M(double a[4][4],char *s);
void 	Inv_matrix_4( double Mtrix_A[4][4], double Inv_Mtrix_A[4][4]);
void 	mt_DHmul(double A[4][4], double B[4][4], double output[4][4]);
void 	Cross_product( double *a,double *b,double *c);
void 	coordinate_transformation(COORD_MEM Path[]);
void	CalInvWorkCoordSystem()	;
void	GetPathPoint(COORD_MEM Path[],int path_num);
int 	pathplanningfeasibility(int* path_cnt);
void	GetTargetPoint(COORD_MEM* TargetPoint,COORD_MEM Path_1,COORD_MEM Path_2);
int 	PolishCal();
int 	ReadPolishConfig();
void	SetInfoShow(CtmWnd* pwndSender);
void	ReadValue(char* str, void* out , int type);
int pathpointproduct(double distance_ratio,double theta,char* file);
#endif
