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
#ifndef		D__VIEWGLUE
#define		D__VIEWGLUE


// debug�Ҧ�
//#define PATH_T_DBG
// �꩷�̤j�b�|,�̤p�b�|
#define ARC_R (20)
#define ARC_R_MIN (10)
// �о��I�ƶq,�ͦ����|�I�ƶq
#define MAX_TEACH_POINT (50)
#define MAX_PATH_POINT (3*(MAX_TEACH_POINT-2)+2)

typedef enum {
	POINT_t = 1,	// �I���I���u
	ARC_t,			// �꩷�q�L�I
	END_t			// �꩷���I
} POINTTYPE_T;

// �I������,�y�� xyz,����xyz,���O�d��@�]�wIO�}��
typedef struct tag_POINTDATA_T{
	POINTTYPE_T type;
	double x;
	double y;
	double z;
	double rx;
	double ry;
	double rz;
	double a;	//reserve
	double b;	//reserve
	double c;	//reserve
}POINTDATA_T;

// �о��I, �ͦ������|�I, �`��
typedef struct PATHDATA_T{
POINTDATA_T aPoint_t[MAX_TEACH_POINT];
POINTDATA_T aPath_t[MAX_PATH_POINT];
int iPoint_top;
int iPath_top;
}PATHDATA_T;

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);  //�����!!!!!  �ΤFselectedit�|GG
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);

//=====��e������u����m=====
void		ShowCurrentCoordinate();
void		SetLimitFlag(int Axis,int Value);

//=====�ޱ�������u=====
void		SetAndShowSpeed(int nType);
void		SetRobotPosition(int nCoordinateType, int nBtnType, int nAxisNum);
void		UpdateMoveControlType(int nCoordinateType);

//=====�о��I�B�z=====
void		SetUnit(int UnitType);			//20140915		by Mario
void 		SwitchJogMode();							

//=====�´���=====
//add by mario 2015/3/2 �U�� 02:15:12
void		SetJogMoveDate(int nCoordinateType, int nBtnType, int nAxisNum);
void		RestJogMoveDate();
void		ini(CtmWnd* pwndSender);
void 		SetBtn_JogMove(BOOL f_JogLock);
void		SetJogSelectEditMode();

//GLUE
int 		Show_path(PATHDATA_T *v);
int 		Show_point(PATHDATA_T *v);
int 		Print_point(POINTDATA_T p);
int 		Pop_pathdata(PATHDATA_T *v);
int 		Push_pathdata(PATHDATA_T *v,POINTDATA_T p);
int 		Pop_pointdata(PATHDATA_T *v);
int 		Push_pointdata(PATHDATA_T *v,POINTDATA_T p);
int 		CreateProg();
char* 	get_coord(double vlaue , int num , char* str);
void		InsertPos();
#endif
