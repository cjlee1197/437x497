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
#define		MSG_SH_DATA_MODE_MANUAL				MSG_SH_DATA_STARTSAVE + 0x00			//手動
#define		MSG_SH_DATA_MODE_SEMIAUTO			MSG_SH_DATA_STARTSAVE + 0x01			//半自動
#define		MSG_SH_DATA_MODE_SENSORAUTO			MSG_SH_DATA_STARTSAVE + 0x02			//電眼自動
#define		MSG_SH_DATA_MODE_TIMEAUTO			MSG_SH_DATA_STARTSAVE + 0x03			//時間自動
#define		MSG_SH_DATA_MODE_HANDMOLDADJ		MSG_SH_DATA_STARTSAVE + 0x04			//手動調模
#define		MSG_SH_DATA_MODE_AUTOMOLDADJ		MSG_SH_DATA_STARTSAVE + 0x05			//自動調模

#define		MSG_SH_DATA_MODE_HEATERON			MSG_SH_DATA_STARTSAVE + 0x20			//電熱開
#define		MSG_SH_DATA_MODE_HEATEROFF			MSG_SH_DATA_STARTSAVE + 0x21			//電熱關
#define		MSG_SH_DATA_MODE_MOTORON			MSG_SH_DATA_STARTSAVE + 0x22			//馬達開
#define		MSG_SH_DATA_MODE_MOTOROFF			MSG_SH_DATA_STARTSAVE + 0x23			//馬達關

#define		MSG_SH_DATA_DATAPROCESS_ERROR		MSG_SH_DATA_STARTSAVE + 0x100			//保存資料失敗

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
		tmDATETIME	starttime;				//起始日期時間
		tmDATETIME	endtime;				//結束日期時間
		DWORD		dwMoldCountStart;		//起始開模序號
		DWORD		dwMoldCountEnd;			//結束開模序號
		DWORD		dwGoodProduct;			//良品數
		DWORD		dwBadProduct;			//不良品數
		DWORD		dwEnergyStart;			//起始能耗
		DWORD		dwEnergyEnd;			//結束能耗
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
		tmDATETIME	powerontime;			//開機時間
		tmDATETIME	powerofftime;			//關機時間
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
		tmDB_MONI2	db_moni2;				//每模監測資料
		tmDATETIME	datetime;				//每模記錄時間
		DWORD		dwEnergy;				//能耗記錄
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
	DWORD	dwLen;				//文件名長度
	char	cszFileName[128];	//文件名
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
	DWORD	dwVersion;			//版本資訊
	DWORD	dwType;				//資料類型
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
	DWORD		dwVersion;		//版本資訊
	DWORD		dwType;			//資料類型
	DWORD		dwMode;			//資料存儲模式(0=固定存儲 1=連續存儲 2=分文件存儲)
	DWORD		dwCount;		//資料筆數
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
