/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    :  		                                             			 |
|  Task     :  			                         							 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 10/04/2010	                                                 |
|  Revision : 							                                     |
+===========================================================================*/
#ifndef	D_LIBDATASAVE
#define	D_LIBDATASAVE


#include 	"../oil54struct.h"
#include	"../tmshmsg.h"
#include	"libControl.h"
/*===========================================================================+
|           Msg definition                                                  |
+===========================================================================*/
#define		MSG_SH_DATA_STARTSAVE				MSG_SH_TASKDATA + 0x1000
#define		MSG_SH_DATA_MODE_MANUAL				MSG_SH_DATA_STARTSAVE + 0x00			//���
#define		MSG_SH_DATA_MODE_SEMIAUTO			MSG_SH_DATA_STARTSAVE + 0x01			//�b�۰�
#define		MSG_SH_DATA_MODE_SENSORAUTO			MSG_SH_DATA_STARTSAVE + 0x02			//�q���۰�
#define		MSG_SH_DATA_MODE_TIMEAUTO			MSG_SH_DATA_STARTSAVE + 0x03			//�ɶ��۰�
#define		MSG_SH_DATA_MODE_HANDMOLDADJ		MSG_SH_DATA_STARTSAVE + 0x04			//��ʽռ�
#define		MSG_SH_DATA_MODE_AUTOMOLDADJ		MSG_SH_DATA_STARTSAVE + 0x05			//�۰ʽռ�

#define		MSG_SH_DATA_MODE_HEATERON			MSG_SH_DATA_STARTSAVE + 0x20			//�q���}
#define		MSG_SH_DATA_MODE_HEATEROFF			MSG_SH_DATA_STARTSAVE + 0x21			//�q����
#define		MSG_SH_DATA_MODE_MOTORON			MSG_SH_DATA_STARTSAVE + 0x22			//���F�}
#define		MSG_SH_DATA_MODE_MOTOROFF			MSG_SH_DATA_STARTSAVE + 0x23			//���F��

#define		MSG_SH_DATA_DATAPROCESS_ERROR		MSG_SH_DATA_STARTSAVE + 0x100			//�O�s��ƥ���

/*===========================================================================+
|           StateMode definition                                             |
+===========================================================================*/
#define		DATAPROCESS_STATEMODE_TIMEAUTO			0x01
#define		DATAPROCESS_STATEMODE_SEMIAUTO			0x02
#define		DATAPROCESS_STATEMODE_MANUAL			0x03
#define		DATAPROCESS_STATEMODE_HANDMOLDADJ		0x04
#define		DATAPROCESS_STATEMODE_AUTOMOLDADJ		0x05
#define		DATAPROCESS_STATEMODE_SENSORAUTO		0x06
#define		DATAPROCESS_STATEMODE_MOTORON			0x07
#define		DATAPROCESS_STATEMODE_MOTOROFF			0x08
#define		DATAPROCESS_STATEMODE_HEATERON			0x09
#define		DATAPROCESS_STATEMODE_HEATEROFF			0x0A
#define		DATAPROCESS_STATEMODE_POWERON			0x0B
#define		DATAPROCESS_STATEMODE_POWEROFF			0x0C

/*===========================================================================+
|           ConfigFile definition                                            |
+===========================================================================*/
#define		DATAPROCESS_FILETYPE_CONFIG				0x01
#define		DATAPROCESS_FILETYPE_DATA				0x02

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
#define		DATAPROCESS_TYPE_ERROR					0x01
#define		DATAPROCESS_TYPE_MONITOR				0x02
#define		DATAPROCESS_TYPE_TEMPER					0x03
#define		DATAPROCESS_TYPE_OPSTATE				0x04
#define		DATAPROCESS_TYPE_ENERGY					0x05
#define		DATAPROCESS_TYPE_SPC					0x06
#define		DATAPROCESS_TYPE_ALARM					0x07
#define		DATAPROCESS_TYPE_HARDINFO				0x08
#define		DATAPROCESS_TYPE_USERINFO				0x09
#define		DATAPROCESS_TYPE_MOLDSET				0x0A
#define		DATAPROCESS_TYPE_RECORD					0x0B
#define		DATAPROCESS_TYPE_PRODUCTPLAN			0x0C
#define		DATAPROCESS_TYPE_MAXCOUNT				0x0C

#define		DATAPROCESS_MODE_1						0x01
#define		DATAPROCESS_MODE_2						0x02
#define		DATAPROCESS_MODE_3						0x03

#define		MAX_POWERONOFF_NUM						200
#define		MAX_STATE_NUM							200
#define		MAX_ENERGY_NUM							200

/*===========================================================================+
|           OnOffState definition                                            |
+===========================================================================*/
#define		DATAPROCESS_ONFFSTATE_ON				0x01
#define		DATAPROCESS_ONFFSTATE_OFF				0x02

#define		DATAPROCESS_ONFFDEVICE_55				0x01
#define		DATAPROCESS_ONFFDEVICE_28				0x02
#define		DATAPROCESS_ONFFDEVICE_RMTP12			0x03
#define		DATAPROCESS_ONFFDEVICE_PWM				0x04
#define		DATAPROCESS_ONFFDEVICE_TMIOT			0x05
#define		DATAPROCESS_ONFFDEVICE_CANRULER			0x06


typedef struct	tag_StateMode
{
	struct 
	{
		int		nCount;
		int		nIndex;
	};
	struct
	{
		int		nMode;
		DWORD	dwOpenMoldCount;
		tmDATE	date;
		tmTIME	time;
		char	cszUserID[10];
	};
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	STATEMODE;
#else 
#ifdef	D_X86
STATEMODE;
#endif
#endif

typedef struct	tag_OnOffState
{
	struct 
	{
		int		nCount;
		int		nIndex;
	};
	struct
	{
		int		nDevice;					//Device Type
		WORD	wSubIndex;					//Sub Device Index
		WORD	wState;						//Device State
		DWORD	dwOpenMoldCount;
		tmDATE	date;
		tmTIME	time;
	};
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	ONOFFSTATE;
#else 
#ifdef	D_X86
ONOFFSTATE;
#endif
#endif

typedef struct	tag_tmCapacity
{
	struct 
	{
		int		nCount;
		int		nIndex;
	};
	struct
	{
		tmDATETIME	starttime;				//�_�l����ɶ�
		tmDATETIME	endtime;				//��������ɶ�
		DWORD		dwMoldCountStart;		//�_�l�}�ҧǸ�
		DWORD		dwMoldCountEnd;			//�����}�ҧǸ�
		DWORD		dwGoodProduct;			//�}�~��
		DWORD		dwBadProduct;			//���}�~��
		DWORD		dwEnergyStart;			//�_�l���
		DWORD		dwEnergyEnd;			//�������
	};
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TMCAPACITY;
#else 
#ifdef	D_X86
TMCAPACITY;
#endif
#endif

typedef	struct	tag_tmPowerOnOff
{
	struct 
	{
		int		nCount;
		int		nIndex;
	};
	struct
	{
		tmDATETIME	powerontime;			//�}���ɶ�
		tmDATETIME	powerofftime;			//�����ɶ�
	};
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TMPOWERONOFF;
#else 
#ifdef	D_X86
TMPOWERONOFF;
#endif
#endif

typedef	struct	tag_tmMoniData
{
	struct 
	{
		int		nCount;
		int		nIndex;
	};
	struct
	{
		tmDB_MONI2	db_moni2;				//�C�Һʴ����
		tmDATETIME	datetime;				//�C�ҰO���ɶ�
		DWORD		dwEnergy;				//��ӰO��
	};
	
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TMMONIDATA;
#else 
#ifdef	D_X86
TMMONIDATA;
#endif
#endif

typedef	struct	tag_DataConfig_FileInfo
{
	DWORD	dwLen;				//���W����
	char	cszFileName[128];	//���W
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DATACONFIG_FILEINFO;
#else 
#ifdef	D_X86
DATACONFIG_FILEINFO;
#endif
#endif


typedef	struct	tag_DataConfigInfo
{
	DWORD	dwVersion;			//������T
	DWORD	dwType;				//�������
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DATACONFIGINFO;
#else 
#ifdef	D_X86
DATACONFIGINFO;
#endif
#endif

typedef	struct	tag_DataHeadInfo
{
	DWORD		dwVersion;		//������T
	DWORD		dwType;			//�������
	DWORD		dwMode;			//��Ʀs�x�Ҧ�(0=�T�w�s�x 1=�s��s�x 2=�����s�x)
	DWORD		dwCount;		//��Ƶ���
	DWORD		dwLen;	
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DATAHEADINFO;
#else 
#ifdef	D_X86
DATAHEADINFO;
#endif
#endif

#endif
