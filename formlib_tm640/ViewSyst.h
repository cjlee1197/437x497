/*===========================================================================+
|  Class    : ViewSyst library                                               |
|  Task     : ViewSyst library  header file                                  |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		          	                                         |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                         |
|  Revision :       		   					                             |
+===========================================================================*/


#ifndef D__VIEWSYST
#define D__VIEWSYST
#include	"../formview.h"
#include	"../tmconfig.h"

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C" WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);

void        CreateSet(CtmWnd* pwndSender);
void		Setselect(CtmWnd* pwndSender);
void   		SetLogoToLanguage();
void        ClearTemp();
void		GetIPAddr();
void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType);
void	GetACTTime(CtmWnd* pwnd);
void	GetOpenTimeDay(CtmWnd* pwnd);
void	GetOpenTimeHour(CtmWnd* pwnd);
void	GetOpenTimeMinute(CtmWnd* pwnd);
void	GetAutoTimeDay(CtmWnd* pwnd);
void	GetAutoTimeHour(CtmWnd* pwnd);
void	GetAutoTimeMinute(CtmWnd* pwnd);
void  	SetSpedValue();
void   	TransferPrsUnit(char* pszData[],int Length);
void	SavePushKeyOilData(); /* tyl  2015/6/19 for 7TR */
char*		GetAdminPassWord();
void		GetVersionToNumber(char* buff);
void 		DateToString(char* buff);
void	SetTempValueFahr();
void	SetTempValueCels();
void  	SetPosValueInch();
void  	SetPosValueMM();
void   	SetPresValuePsi();
void   	SetPresValueBar();
BOOL	SaveBlock(char* pszFileName, WORD wBlockID);
void  	SetConfPosValueInch();
void  	SetConfPosValueMM();
void  	SetPosValueRealInch();
void  	SetPosValueRealMM();

void	GetHour(CtmWnd* pwnd);	
void	GetMinute(CtmWnd* pwnd);
int	ReadBgLight();
void ImgBglight_Update(int Bglight);
void UpdateImg_LG_Choose();
void UpdateLGString();
#endif
