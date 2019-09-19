/*===========================================================================+
|  Class    : ViewScreenSaver  library                                       |
|  Task     : ViewScreenSaver  library header file                           |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny                                                          |
|  Version  : V1.00                                                          |
|  Creation : 04/03/2007                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef D__VIEWSCREENSAVER
#define D__VIEWSCREENSAVER

#include	"../formview.h"

typedef	struct tagtmNUMBERBMP
{
	//RECT    	rect;
	int		Part[7];
	//int	UpHori0;
	//int	MidHori1;
	//int	DownHori2;
	//int	UpLBVert0;
	//int	UpRVert1;
	//int	DownLVert2;
	//int	DownRVert3;
} tmNUMBERBMP;


//extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	void	OnDestroyA(CtmWnd* pwndSender);

void	ShowNumber(int NumPart,int No);				//Sunny<20070403>
void	SelectBit(long Value,int Line,int VisibleNum);
void	SelectBitDB(long Value,int Line,int iPrecision);

void	AllControlMove(CtmWnd* pwndSender);
void	GetOPStatus(); 	//Sunny<20070428>判斷機器當前操作狀態

extern	    tmNUMBERBMP		m_NumberBmp[];	//Sunny<20070403>

#endif
