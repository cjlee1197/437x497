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


// debug模式
//#define PATH_T_DBG
// 圓弧最大半徑,最小半徑
#define ARC_R (20)
#define ARC_R_MIN (10)
// 教導點數量,生成路徑點數量
#define MAX_TEACH_POINT (50)
#define MAX_PATH_POINT (3*(MAX_TEACH_POINT-2)+2)

typedef enum {
	POINT_t = 1,	// 點到點直線
	ARC_t,			// 圓弧通過點
	END_t			// 圓弧終點
} POINTTYPE_T;

// 點的類型,座標 xyz,旋轉xyz,先保留當作設定IO開關
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

// 教導點, 生成的路徑點, 總數
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
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);  //不能用!!!!!  用了selectedit會GG
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);

//=====當前機械手臂的位置=====
void		ShowCurrentCoordinate();
void		SetLimitFlag(int Axis,int Value);

//=====操控機械手臂=====
void		SetAndShowSpeed(int nType);
void		SetRobotPosition(int nCoordinateType, int nBtnType, int nAxisNum);
void		UpdateMoveControlType(int nCoordinateType);

//=====教導點處理=====
void		SetUnit(int UnitType);			//20140915		by Mario
void 		SwitchJogMode();							

//=====純測試=====
//add by mario 2015/3/2 下午 02:15:12
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
