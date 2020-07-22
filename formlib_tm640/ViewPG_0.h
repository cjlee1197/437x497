/*===========================================================================+
|  Class    : ViewClamp library                                              |
|  Task     : ViewClamp library  header file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		          	                                 		       |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                    				     |
|  Revision :       		   					                            						 |
+===========================================================================*/

#ifndef D__VIEWONLINEPROGRAM
#define D__VIEWONLINEPROGRAM
#include	"../formview.h"
#include	"../wnd.h"
#include	"../commonaction.h"
#include	"../database.h"
#include	"../utils.h"
#include	"../main.h"

#define	MechType3				0	 // 三軸
#define	MechType5				1  // 五軸
#define	EncWord 				0xFFFF0000 // High Word
#define	MechWord 				0x0000FFFF // Low Word
#define PAGE_NUM 15

#define ITEM 32
#define GRAY_ 0xC618
#define WHITE_ 0xFFFF
#define LBLUE_ 0xD73D // 淺藍
#define GREEN_ 0xCF90
#define MAXDBNUM 100
#define PARA_NUM 10

#define Action_Axis 1
#define Action_Permit 3

#define Bar_Heigh 90
#define Bar_Position 127
#define Bar_Step 55

#define CONST_REQ_COMMAND 6
#define CONST_REQ_WRITE 3
#define CONTROLTYPENUM 13
#define MAXSTEP 100

#define	MODE_SINGLESTEP	0xF400
#define	MODE_MANUAL 0xF300
extern  "C"		BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"		WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C" 	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"		void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"		void	OnDestroyA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);

class ListNode{
public:
    ListNode();
	
	char reference[256];
	bool readFlag;
};

enum MachineState
{
	STATE_IDLE = 1,
	STATE_SINGLESTEP,
	STATE_HAND,
	STATE_FULLAUTO
};

class PG_0	{
public:
	PG_0();
	~PG_0();
	void PageDown();
	void PageUp();
    void PageNext();
	void ShowText();
	void UpdatePageBar();
	void Insert();
	void Delete(int);
	void Clear();
	void Sync();
	void UnSync(int);
	void Clean();
	void GetPosTag();
	void SendCommand(int);
	void Download();
	void Update_Download_Hint();
	inline void AddStr(char*, int &);

	CtmWnd* pwndStaticAct[ITEM];
	CtmWnd*	pwndStaticEditNo[ITEM];
	CtmWnd* pwndCheckBoxAct[ITEM];
	CtmWnd*	pwndBtnPageUp;
	CtmWnd*	pwndBtnPageDown;
	CtmWnd*	pwndImgBar;
	CtmWnd* pwndImgBarMask;
	CtmWnd* pwndBtnFollow;
	CtmWnd* pwndBtnExecute;
	CtmWnd* pwndBtnEdit;
	CtmWnd* pwndBtnClear;
	CtmWnd* pwndBtnDownload;
	CtmWnd* pwndBtnInsert;
	CtmWnd* pwndBtnDelete;
	CtmWnd* pwndBtnCombine;
	CtmWnd* pwndBtnSplit;
	CtmWnd* pwndBtnSingle;
	
	int num_of_action;
	int selectNo;
	int headNo;
	int lastSelect ;
	int iPageturn;
	int editedStepNum;
	int page;
	bool b_Follow;
	DWORD lastSelectColor_1;
	DWORD lastSelectColor_2;
	int cleanhelp;
	int finishPoint;

	vector< ListNode *> strList;
	
};

#endif
