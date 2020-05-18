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
void	SprintfDBValue(int iParam497, int iParam28); // �B�zDB�ƭ� ��ܤ�r
void	SprintfDBValue(char* strParam497,char* strParam28); // �B�zDB�ƭ� ��ܤ�r

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
	int iDouble;				// ���ƾ��c
	int iPosInv;				// ��m�ϦV
	int Axis[5][5];			// �b�Ѽ�
	int Speed[5][7];		// �t�װѼ�
	int ZoneLimit[6][3];// �ϰ�]�w�Ѽ�
	int Mode[3];				// �Ҧ����
	int Detect[6];			// �˴����
	int Delay[4];				// ���ɿ��
	int Prod[2];				// �Ͳ��Ѽ�
	int	Pile[4][6];			// ���|�Ѽ�
}tmParam;

enum PrecisionList		// �Ѽƺ�� �p���I��m��
{
	PAxis,			// �b�Ѽ�
	PSpeed,			// �t�װѼ�
	PZoneLimit,	// �ϰ�Ѽ�
};

enum
{
	/*���c�Ѽ�*/
	MECH_MECHTYPE, //0 �������
	TRANS_TYPE,		 //1 �ǰʤ覡
	MECH_ENCTYPE,	 //2 �s�X�����
	MOTOR_ENC_REZ, //3 �s�X���ѪR��
	MOTOR_RATIO,	 //4 ��t��
	MECH_GEAR_D,	 //5 �C��Z��
	MECH_DOUB_MECH,//6 ���ƾ��c 
	MECH_POS_INV,	 //7 ��m�ϦV
	/*�t�װѼ�*/
	SPEED_MAX,	 // �̰���t
	SPEED_ACC_T,	 // �[�t�ɶ�
	SPEED_DCC_T,	 // ��t�ɶ�
	SPEED_RATIO,	 // ���Ƥ��
	SPEED_POS_KP,	 // ����T��
	SPEED_SPEED_KP,	 // �l�H�T��
	/*�ϰ�]�w*/
	ZONELIMIT_Zone,	 // ���ʳ̤j��m
	ZONELIMIT_X1_MIN,	 // 1.���\�U���Ҥ��̤p��m
	ZONELIMIT_X1_MAX,	 // 2.���\�U���Ҥ��̤j��m
	ZONELIMIT_Y1_MIN,	 // 3.���\��X�̤j����
	ZONELIMIT_Y1_MAX,	 // 4.���\���ҳ̤j�ݾ�����
	ZONELIMIT_Z_MIN,	 // 5.���\�U���Ҥ��̤j��m
	ZONELIMIT_Z_MAX,	 // 6.���\�m�����~�̤p��m
	ZONELIMIT_X2_MIN,	 // 7.���\�U���Ҥ��̤p��m
	ZONELIMIT_X2_MAX,	 // 8.���\�U���Ҥ��̤j��m
	ZONELIMIT_Y2_MIN,	 // 9.���\��X�̤p����
	ZONELIMIT_Y2_MAX,	 // 10.���\���ҳ̤j�ݾ�����
	ARM_SAFE_DIS,	 // �D���u�w���Z��
};

#endif








