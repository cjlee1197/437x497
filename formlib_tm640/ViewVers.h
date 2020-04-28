/*===========================================================================+
|  Class    : ViewVersion library                                         	 |
|  Task     : ViewVersion library  header file                            	 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/

#ifndef D__VIEWVERSION
#define D__VIEWVERSION
#include	"../formview.h"
#include	"../commonaction.h"
#include	"../moldControl.h"
#include	"../lib/libComm.h"


extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);
extern	"C" WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey);

void		SetDBRM(tmDB_DIAGRM* db_diagrm, CtmWnd* pwnd[], int nCount);

void       	CreateSet(CtmWnd* pwndSender);
void		ShowTransmitCom(CtmWnd* pwndSender);
void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType);
void   		ReadDingDanFrom();

char*		DW2Str_ASCII(DWORD dw_dbvalue); //cjlee 2019/5/12 �U�� 09:51:01

void	GetValueFrom28(); // ���o28�]�w��
void	SetChosenDB(char* dbIDName, int Param497, int Param28, int iDBSelect); // �]�w���DB�ƭ�
typedef struct tagAxis
{
	int iDouble; 				// ���ƾ��c 
	int iPosInv; 				// ��m�ϦV
	int iENC_REZ;				// �s�X���ѪR��
	int iMotorRatio;		// ��t��
	int iMotorDiameter; // �`�ꪽ�|
	int iToothNum;			// ����
	int iM_Num;					// �Ҽ�
	int iMaxSpeed;			// �̰���t
	int iSpeedACCT;			// �[�t�ɶ�
	int iSpeedDCCT;			// ��t�ɶ�
	int iACC_SmoothR;   // �[�t���Ƥ��
	int iDCC_SmoothR;   // ��t���Ƥ��
	int iPosition_KP;		// ����T�� ��mKP
	int iSpeed_KP;			// �l�H�T�� �t��KP
}tmAxis;



typedef struct tagParam
{
	int iMechType;			// �������
	int iPickerType;  	// ������� ����
	int iEncType;				// ������� �s�X��
	int iTransType; 		// �ǰʤ覡
	int Axis[5][5];			// �b�Ѽ�
	int Speed[5][7];		// �t�װѼ�
	int ZoneLimit[6][3];// �ϰ�]�w�Ѽ�
	int Mode[3];				// �Ҧ����
	int Detect[6];			// �˴����
	int Delay[4];				// ���ɿ��
	int Prod[2];				// �Ͳ��Ѽ�
	int	Pile[4][6];			// ���|�Ѽ�
}tmParam;



#endif








