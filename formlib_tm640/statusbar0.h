/*===========================================================================+
|  Class    : statusbar library                                              |
|  Task     : statusbar library  header file                                 |
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
#ifndef D__STATUSBAR0
#define D__STATUSBAR0

#include	"../formview.h"
#include	"../utils.h"
#include	"../database.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);

void	ChangeImage(CtmWnd* pimage, char* pszpath,WORD FLASH_FLAG,WORD TIMER_NUM);
void	Init_Common_User();
void	Double_byte_String_Merger(char* out, char* str1 , char* str2);
void	SetGpioLed(BOOL status,int bit);
void	SetEditValue(CtmWnd* pwnd);
void	SendCommand(int	CommandID);

void	GetValueFrom28(); // ���o28�]�w��
void	SetChosenDB(char* dbIDName, int Param497, int Param28, int iDBSelect); // �]�w���DB�ƭ�
void	SprintfDBValue(int iParam497, int iParam28); // �B�zDB�ƭ� ��ܤ�r
void	SprintfDBValue(char* strParam497,char* strParam28); // �B�zDB�ƭ� ��ܤ�r

typedef struct tagParam
{
	int iMechType;			// �������
	int iPickerType;  	// ������� ����
	int iEncType;				// ������� �s�X��
	int iTransType; 		// �ǰʤ覡
	int iDouble[5];			// ���ƾ��c
	int iPosInv[5];			// ��m�ϦV
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
