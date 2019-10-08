#include	"../../package.h"
#include	"../../timer.h"
#include	"../../main.h"
#include	"../../tmdebug.h"
#include	"../../init.h"
#include	"../../utils.h"
#include	"../../ethnet.h"
#include	"../../tmshmsg.h"
#include	"../../tmnbmsg.h"
#include	"../libComm.h"
#include	"tmTaskDsp28_M3.h"
#include	"../../readdata.h"
#include 	"../../moldControl.h"
#include	"../../iotable.h"

IMPLEMENT_DYNCREATE(CtmTaskDsp28_M3, CTaskHost)
typedef void *(* PRSRUN)(void *);
#define     ABS(n)              ((n >= 0) ? n : -n)

long		CNT_TRANSMIT_FLAG				= 0;

long		CNT_INT_CHECK_ONLNE_TIME 		= 3000;
long		CNT_INT_SEND_LINESTATE_TIME 	= 2000;
long		CNT_INT_CHECK_LINESTATE_TIME 	= 10000;
long		CNT_INT_ACTUALVALUE_TIME		= 500;
static		BOOL	u_bSendOnLineFlag = FALSE;	

BOOL				b_SendN2CFlag = TRUE;

#define		BLOCK_DB_PLC_INJECT_SET			308
#define		BLOCK_DB_PLC_DOSING_SET			310
#define		BLOCK_DB_PLC_OTHER_SET			314
#define		BLOCK_DB_PLC_MOLD_SET			304
#define		BLOCK_DB_PLC_TEMPER				315
#define		BLOCK_DB_PLC_TEMPER2			781
#define		BLOCK_DB_PLC_TEMPER3      		782
#define		BLOCK_DB_PLC_TEMPER4      		783
#define		BLOCK_DB_PLC_TEMPER5      		784
#define		BLOCK_DB_PLC_AXIS1_SET			289
#define		BLOCK_DB_PLC_AXIS2_SET			292
#define   BLOCK_DB_PLC_EJECT_SET    306
#define   BLOCK_DB_PLC_MTHUB_SET    13669
//#define		BLOCK_DB_PLC_AXIS3_SET			295
//#define		BLOCK_DB_PLC_AXIS4_SET

//int 	m_SendMoldNum = 0;

/*ZHBA 2013-4-18 */
//char *g_DspM3ReadTempID[] =
//{
//	"TEMPERCARD1_INTERFACE_WSEG1_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG1_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG2_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG2_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG3_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG3_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG4_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG4_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG5_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG5_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG6_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG6_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG7_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG7_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG8_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG8_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG9_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG9_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG10_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG10_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG11_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG11_CURRENT",
//	"TEMPERCARD1_INTERFACE_WSEG12_STATUS",
//	"TEMPERCARD1_INTERFACE_LSEG12_CURRENT",
//	"INJECT_FAXIS1_LIMITS_OBJECT_ACADEMICMAXVELOCITY",
//	"INJECT_FAXIS1_LIMITS_OBJECT_ACADEMICMAXFORCE",
//	"INJECT_FAXIS1_LIMITS_OBJECT_ACADEMICMAXRPM",
//	"CHARGE_FAXIS1_LIMITS_OBJECT_ACADEMICMAXVELOCITY",
//	//"CHARGE_FAXIS1_LIMITS_OBJECT_ACADEMICMAXFORCE",
//	//"CHARGE_FAXIS1_LIMITS_OBJECT_ACADEMICMAXRPM",
//	"M3_STATE_TEMPERCARD1_LINESTATE",
//	"M3_STATE_POWERCARD1_LINESTATE",
//	"M3_STATE_CARD1_LINESTATE",
//	"M3_STATE_CARD2_LINESTATE",
//	"PWMTRCARD1_INTERFACE_FAVRMS",
//	"PWMTRCARD1_INTERFACE_FBVRMS",
//	"PWMTRCARD1_INTERFACE_FCVRMS",
//	"PWMTRCARD1_INTERFACE_FAIRMS",
//	"PWMTRCARD1_INTERFACE_FBIRMS",
//	"PWMTRCARD1_INTERFACE_FCIRMS",
//	"PWMTRCARD1_INTERFACE_FPOSI_WATTHR",
//	"M3_STATE_TEMPERCARD1_HEATERWARM_HALF_STATE"
//};

char *g_TemperRealPID[] = 
{
	"TEMPERCARD1_INTERFACE_WSEG1_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG1_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG1_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG1_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG1_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG2_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG2_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG2_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG2_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG2_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG3_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG3_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG3_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG3_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG3_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG4_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG4_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG4_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG4_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG4_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG5_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG5_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG5_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG5_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG5_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG6_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG6_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG6_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG6_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG6_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG7_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG7_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG7_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG7_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG7_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG8_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG8_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG8_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG8_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG8_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG9_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG9_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG9_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG9_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG9_PIDREAL_STABLEDUTY",
};

char *g_Temper2RealPID[] = 
{
	"TEMPERCARD1_INTERFACE_WSEG13_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG13_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG13_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG13_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG13_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG14_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG14_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG14_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG14_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG14_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG15_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG15_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG15_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG15_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG15_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG16_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG16_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG16_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG16_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG16_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG17_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG17_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG17_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG17_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG17_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG18_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG18_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG18_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG18_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG18_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG19_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG19_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG19_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG19_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG19_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG20_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG20_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG20_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG20_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG20_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG21_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG21_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG21_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG21_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG21_PIDREAL_STABLEDUTY",
};

char *g_TemperT11RealPID[] = 
{
	"TEMPERCARD1_INTERFACE_WSEG10_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG10_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG10_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG10_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG10_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG11_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG11_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG11_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG11_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG11_PIDREAL_STABLEDUTY",
	
};

char *g_Temper3RealPID[] = 
{
	"TEMPERCARD1_INTERFACE_WSEG25_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG25_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG25_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG25_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG25_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG26_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG26_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG26_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG26_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG26_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG27_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG27_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG27_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG27_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG27_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG28_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG28_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG28_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG28_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG28_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG29_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG29_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG29_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG29_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG29_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG30_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG30_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG30_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG30_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG30_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG31_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG31_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG31_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG31_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG31_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG32_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG32_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG32_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG32_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG32_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG33_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG33_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG33_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG33_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG33_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG34_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG34_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG34_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG34_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG34_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG35_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG35_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG35_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG35_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG35_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG36_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG36_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG36_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG36_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG36_PIDREAL_STABLEDUTY",
};

char *g_Temper4RealPID[] = 
{
	"TEMPERCARD1_INTERFACE_WSEG37_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG37_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG37_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG37_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG37_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG38_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG38_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG38_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG38_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG38_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG39_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG39_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG39_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG39_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG39_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG40_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG40_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG40_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG40_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG40_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG41_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG41_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG41_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG41_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG41_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG42_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG42_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG42_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG42_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG42_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG43_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG43_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG43_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG43_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG43_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG44_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG44_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG44_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG44_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG44_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG45_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG45_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG45_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG45_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG45_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG46_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG46_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG46_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG46_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG46_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG47_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG47_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG47_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG47_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG47_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG48_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG48_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG48_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG48_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG48_PIDREAL_STABLEDUTY",
};

char *g_Temper5RealPID[] = 
{
	"TEMPERCARD1_INTERFACE_WSEG49_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG49_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG49_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG49_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG49_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG50_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG50_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG50_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG50_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG50_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG51_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG51_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG51_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG51_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG51_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG52_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG52_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG52_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG52_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG52_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG53_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG53_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG53_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG53_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG53_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG54_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG54_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG54_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG54_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG54_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG55_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG55_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG55_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG55_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG55_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG56_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG56_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG56_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG56_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG56_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG57_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG57_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG57_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG57_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG57_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG58_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG58_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG58_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG58_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG58_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG59_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG59_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG59_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG59_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG59_PIDREAL_STABLEDUTY",
	
	"TEMPERCARD1_INTERFACE_WSEG60_PIDREAL_KP",
	"TEMPERCARD1_INTERFACE_WSEG60_PIDREAL_KI",
	"TEMPERCARD1_INTERFACE_WSEG60_PIDREAL_KD",
	"TEMPERCARD1_INTERFACE_WSEG60_PIDREAL_RAMP",
	"TEMPERCARD1_INTERFACE_LSEG60_PIDREAL_STABLEDUTY",
};


char *g_TemperPID[] = 
{
"TEMPERCARD1_INTERFACE_WSEG1_PIDSET_KP",
"TEMPERCARD1_INTERFACE_WSEG1_PIDSET_KI",
"TEMPERCARD1_INTERFACE_WSEG1_PIDSET_KD",
"TEMPERCARD1_INTERFACE_WSEG1_PIDSET_RAMP",
"TEMPERCARD1_INTERFACE_LSEG1_PIDSET_STABLEDUTY",

"TEMPERCARD1_INTERFACE_WSEG2_PIDSET_KP",
"TEMPERCARD1_INTERFACE_WSEG2_PIDSET_KI",
"TEMPERCARD1_INTERFACE_WSEG2_PIDSET_KD",
"TEMPERCARD1_INTERFACE_WSEG2_PIDSET_RAMP",
"TEMPERCARD1_INTERFACE_LSEG2_PIDSET_STABLEDUTY",

"TEMPERCARD1_INTERFACE_WSEG3_PIDSET_KP",
"TEMPERCARD1_INTERFACE_WSEG3_PIDSET_KI",
"TEMPERCARD1_INTERFACE_WSEG3_PIDSET_KD",
"TEMPERCARD1_INTERFACE_WSEG3_PIDSET_RAMP",
"TEMPERCARD1_INTERFACE_LSEG3_PIDSET_STABLEDUTY",

"TEMPERCARD1_INTERFACE_WSEG4_PIDSET_KP",
"TEMPERCARD1_INTERFACE_WSEG4_PIDSET_KI",
"TEMPERCARD1_INTERFACE_WSEG4_PIDSET_KD",
"TEMPERCARD1_INTERFACE_WSEG4_PIDSET_RAMP",
"TEMPERCARD1_INTERFACE_LSEG4_PIDSET_STABLEDUTY",

"TEMPERCARD1_INTERFACE_WSEG5_PIDSET_KP",
"TEMPERCARD1_INTERFACE_WSEG5_PIDSET_KI",
"TEMPERCARD1_INTERFACE_WSEG5_PIDSET_KD",
"TEMPERCARD1_INTERFACE_WSEG5_PIDSET_RAMP",
"TEMPERCARD1_INTERFACE_LSEG5_PIDSET_STABLEDUTY",

"TEMPERCARD1_INTERFACE_WSEG6_PIDSET_KP",
"TEMPERCARD1_INTERFACE_WSEG6_PIDSET_KI",
"TEMPERCARD1_INTERFACE_WSEG6_PIDSET_KD",
"TEMPERCARD1_INTERFACE_WSEG6_PIDSET_RAMP",
"TEMPERCARD1_INTERFACE_LSEG6_PIDSET_STABLEDUTY",

"TEMPERCARD1_INTERFACE_WSEG7_PIDSET_KP",
"TEMPERCARD1_INTERFACE_WSEG7_PIDSET_KI",
"TEMPERCARD1_INTERFACE_WSEG7_PIDSET_KD",
"TEMPERCARD1_INTERFACE_WSEG7_PIDSET_RAMP",
"TEMPERCARD1_INTERFACE_LSEG7_PIDSET_STABLEDUTY",

"TEMPERCARD1_INTERFACE_WSEG8_PIDSET_KP",
"TEMPERCARD1_INTERFACE_WSEG8_PIDSET_KI",
"TEMPERCARD1_INTERFACE_WSEG8_PIDSET_KD",
"TEMPERCARD1_INTERFACE_WSEG8_PIDSET_RAMP",
"TEMPERCARD1_INTERFACE_LSEG8_PIDSET_STABLEDUTY",

"TEMPERCARD1_INTERFACE_WSEG9_PIDSET_KP",
"TEMPERCARD1_INTERFACE_WSEG9_PIDSET_KI",
"TEMPERCARD1_INTERFACE_WSEG9_PIDSET_KD",
"TEMPERCARD1_INTERFACE_WSEG9_PIDSET_RAMP",
"TEMPERCARD1_INTERFACE_LSEG9_PIDSET_STABLEDUTY",
};

char *g_Dsp28RealID[] = 
{
	"M3_STATE_TEMPERCARD1_LINESTATE",
	"M3_STATE_POWERCARD1_LINESTATE",
	"M3_STATE_CARD1_LINESTATE",
	"M3_STATE_CARD2_LINESTATE",
	"PWMTRCARD1_INTERFACE_FAVRMS",
	"PWMTRCARD1_INTERFACE_FBVRMS",
	"PWMTRCARD1_INTERFACE_FCVRMS",
	"PWMTRCARD1_INTERFACE_FAIRMS",
	"PWMTRCARD1_INTERFACE_FBIRMS",
	"PWMTRCARD1_INTERFACE_FCIRMS",
	"PWMTRCARD1_INTERFACE_FPOSI_WATTHR",
	//"MOLD_INTERFACE_ACTUALPRESS",
	//"MOLD_INTERFACE_ACTUALVELOCITY",
	//"MOLD_INTERFACE_ACTUALPOSITION",
	//"INJECT_FAXIS1_INTERFACE_ACTUALPRESS",
	//"INJECT_FAXIS1_INTERFACE_ACTUALVELOCITY",
	//"INJECT_FAXIS1_INTERFACE_ACTUALPOSITION",
	//"INJECT_FAXIS1_INTERFACE_ACTUALFORCE",
	//"CHARGE_FAXIS1_INTERFACE_ACTUALPRESS",
	//"CHARGE_FAXIS1_INTERFACE_ACTUALVELOCITY",
	//"CHARGE_FAXIS1_INTERFACE_ACTUALPOSITION",
	//"CHARGE_FAXIS1_INTERFACE_ACTUALOBJECTRPM",
	//"CHARGE_FAXIS1_INTERFACE_ACTUALFORCE",
	//"M3_STATE_MOLD_CMD",
	//"M3_STATE_MOLD_INNER_CMD",
	//"M3_STATE_MOLD_ACTION_STATE",
	//"M3_STATE_INJECT_CMD",
	//"M3_STATE_INJECT_INNER_CMD",
	//"M3_STATE_INJECT_ACTION_STATE",
	//"M3_STATE_CHARGE_CMD",
	//"M3_STATE_CHARGE_INNER_CMD",
	//"M3_STATE_CHARGE_ACTION_STATE",
	//"MOLD_INTERFACE_ERROR1",						//Mold&Hybird Error
	//"INJECT_DWAXIS1_INTERFACE_ERROR1",				//Inject Error
	//"CHARGE_DWAXIS1_INTERFACE_ERROR1",				//Charge Error
	"M3_CONF_TEMPERCARD_AUTOPID_STATE",				
	"M3_CONF_TEMPERCARD_MOLD_AUTOPID_STATE",				
	//"INJECT_FAXIS1_LIMITS_OBJECT_ACADEMICMAXVELOCITY",                   
	//"INJECT_FAXIS1_LIMITS_OBJECT_ACADEMICMAXFORCE",                      
	//"INJECT_FAXIS1_LIMITS_OBJECT_ACADEMICMAXRPM",  
	"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL1",
	"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL2",
	"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL3",
	"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL4",
	"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL5",
	"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL6",
	"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL7",
	"MT_HUB_8A1_WAVEGUIDE_SPEED_VALUE_CHANNEL8",                      
};

char* 	g_Dsp28_RealBackID[18] = {NULL};
int     g_Dsp28_RealElements = 0;


char*	g_Dsp28ReqValueID[] = 
{
	"AUSTON_ACTUALDATA_PROGRAM_VERSION",
	"AUSTON_ACTUALDATA_PROGRAM_DATETIME"
};

CtmTaskDsp28_M3*	CtmTaskDsp28_M3::m_pSelf 	= NULL;
WORD			CtmTaskDsp28_M3::m_wSourceIPPort[] = {192,168,0,123,2310};

CtmTaskDsp28_M3::CtmTaskDsp28_M3(): CTaskHost()
{
	//printf("CtmTaskDsp28_M3::CtmTaskDsp28_M3(): CTaskHost()\n");
	m_pSelf				= this;
	m_nConnecting		= 1;
	m_nConnectStatus	= 2;	//
	
	m_nMaxElements		= 48;//18;
	m_nMaxLen			= 1460;
	m_lCheckQuickTimer	= CNT_INT_SEND_LINESTATE_TIME;	//	5s
	m_lOnLineTime		= CNT_INT_ACTUALVALUE_TIME;
	m_lReqReadDataTime	= 3000;  //100ms
	//m_lReqReadDataTime	= 15000;  //100ms
  	m_lTempReqTime		= CNT_INT_ACTUALVALUE_TIME*2;
	m_lMoldGetNum 		= -1;
	m_lMoldSendNum		= 0;
	m_bCheckData		= FALSE;
	m_lCheckResult		= -1;
	m_bFlashSaved		= FALSE;
	m_bReqOnLineData	= FALSE;
	m_nSyncState		= FALSE;
	m_wRealReadNum  	= 0;
	m_wReadIDIndex		= 0;
	m_pReadIDs			= NULL;
	m_wReadIDNum		= 0;
	m_nSyncDBIndex		= 0;
	m_OldHeadCount		= 0xFFFF;

	m_bDC   = CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC;
	m_bT11Port19 = g_pExTable->GetTableValue(0,"D_T11PORT19");/*wangli2015-10-12 16:30:03  5528 3300TON 特殊機，料溫11段，第二塊RMTP12的CH7做落料口*/
	m_bMold36     = g_pExTable->GetTableValue(0,"D_MDT36");
	m_bEtherCat   = g_pExTable->GetTableValue(0,"D_ETHERCAT");
	m_bJUIII   	  = g_pExTable->GetTableValue(0,"D_JUIII");

	if(m_bEtherCat)
		m_bJ6Redy = TRUE;
	
	memset(m_nBlockIndex, 0, sizeof(m_nBlockIndex));
	m_nBlockIndex[0]	= BLOCK_DB_PLC_OTHER_SET;
//	m_nBlockIndex[1]	= BLOCK_DB_PLC_INJECT_SET;
//	m_nBlockIndex[2]	= BLOCK_DB_PLC_DOSING_SET;
	m_nBlockIndex[3]	= BLOCK_DB_PLC_MOLD_SET;
	m_nBlockIndex[4]	= BLOCK_DB_PLC_TEMPER;	
	
	if(m_bMold36 && m_bT11Port19)
	{
		m_nBlockIndex[5]	= BLOCK_DB_PLC_TEMPER2;	/*wangli2015-10-14 10:29:19 第19段做落料口*/
		m_nBlockIndex[6]	= BLOCK_DB_PLC_TEMPER3;	/*wangli2015-10-14 10:28:35 模溫36段，用第3,4,5塊溫度卡*/
		m_nBlockIndex[7]	= BLOCK_DB_PLC_TEMPER4;	
		m_nBlockIndex[8]	= BLOCK_DB_PLC_TEMPER5;	
		m_nBlockIndex[9]	= BLOCK_DB_PLC_AXIS1_SET;
		
	}
	else if(m_bMold36)
	{
		m_nBlockIndex[5]	= BLOCK_DB_PLC_TEMPER3;	/*wangli2015-10-14 10:28:35 模溫36段，用第3,4,5塊溫度卡*/
		m_nBlockIndex[6]	= BLOCK_DB_PLC_TEMPER4;	
		m_nBlockIndex[7]	= BLOCK_DB_PLC_TEMPER5;	
		m_nBlockIndex[8]	= BLOCK_DB_PLC_AXIS1_SET;
		
	}	
	else	if(m_bDC || m_bT11Port19)
	{	
		m_nBlockIndex[5]	= BLOCK_DB_PLC_TEMPER2;	/*雙色機第2塊溫度卡  13~22*/
		m_nBlockIndex[6]	= BLOCK_DB_PLC_AXIS1_SET;
	}
	else
	{
		m_nBlockIndex[5]	= BLOCK_DB_PLC_AXIS1_SET;
	}	
	
	if(m_bEtherCat)
	{
		m_nBlockIndex[10]	= BLOCK_DB_PLC_EJECT_SET;
	}
	if(m_bJUIII)
	{
		m_nBlockIndex[11]	= BLOCK_DB_PLC_MTHUB_SET;
		
	}
	
	
//	m_nBlockIndex[6]	= BLOCK_DB_PLC_AXIS2_SET;
	m_nMoldsetCount		= CalMoldsetCount();
	
	//printf("m_nMoldsetCount=%d \n", m_nMoldsetCount);
	m_pMoldSetIndexs	= new int[m_nMoldsetCount];
	SetMoldsetID();
	//m_lUpdateTimer		= 100;	//	update the form view within 100ms
		
	//AddCounter(&m_lUpdateTimer, 	m_idMe);
	AddCounter(&m_lReqReadDataTime, m_idMe);
	AddCounter(&m_lCheckQuickTimer, m_idMe);
	AddCounter(&m_lOnLineTime, 		m_idMe);
	AddCounter(&m_lTempReqTime,		m_idMe);
	
	RegisterMessage(MSG_DSP28, m_idMe);
}

CtmTaskDsp28_M3::~CtmTaskDsp28_M3()
{
	DeleteCounter(&m_lReqReadDataTime, 	m_idMe);
	DeleteCounter(&m_lCheckQuickTimer, m_idMe);
	DeleteCounter(&m_lOnLineTime, m_idMe);
	DeleteCounter(&m_lTempReqTime, m_idMe);
	
	delete [] m_pMoldSetIndexs;
	UnRegisterAllMessage(MSG_DSP28);
}

//	execute it atfer the contructor
int		CtmTaskDsp28_M3::CreateSelf()
{
	//printf("CtmTaskDsp28_M3::CreateSelf()\n");
	CTaskHost::CreateSelf();
	m_pData			= new BYTE[m_nMaxLen];
	
	m_bDiagFlag		= FALSE;
	m_nDiagReqNum	= 0;
	//m_nWriteDiagFlag= 0;	
	//	create m_pHostProtocol
	if(m_pHostProtocol == NULL)
		m_pHostProtocol	= static_cast<CtmHostProtocol*>(g_pPackage->CreateClassInstance("HostM3Protocol"));
	
	g_Dsp28_RealElements = sizeof(g_Dsp28RealID) / sizeof(g_Dsp28RealID[0]);

	return 0;
}

//	execute it before the destructor
int		CtmTaskDsp28_M3::FreeSelf()
{
	//	destroy m_pHostProtocol
	g_pPackage->ReleaseClassInstance("HostM3Protocol", m_pHostProtocol);
	
	if (m_nDeviceID	!= -1) 
		DelDevice(m_nDeviceID);
	delete []m_pData;
	return CTaskHost::FreeSelf();
}

void	CtmTaskDsp28_M3::InitData()
{
//	int	nNum 	= 0;
//	if(m_nSyncState == 1)
//	{
//		
//	}
//	nNum = GetBlockToDsp28(BLOCK_DB_PLC_OTHER_SET, CONST_REQ_MOLDSET);
//	nNum += GetBlockToDsp28(BLOCK_DB_PLC_INJECT_SET, CONST_REQ_MOLDSET);
//	nNum += GetBlockToDsp28(BLOCK_DB_PLC_DOSING_SET, CONST_REQ_MOLDSET);
//	nNum += GetBlockToDsp28(BLOCK_DB_PLC_MOLD_SET, CONST_REQ_MOLDSET);
//	m_SendMoldNum = nNum;
//	ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &nNum, NULL);
//	printf("InitData m_SendMoldNum=%d \n", m_SendMoldNum);
}


int		CtmTaskDsp28_M3::SetMoldsetID()
{
	BLOCKDATA*		pBlockData = NULL;
	int i = 0, j = 0, nIndex = 0;
	for(i = 0; i < m_nBlockNum; i++)
	{
		//printf("m_nBlockIndex[i]=%d \n", m_nBlockIndex[i]);
		if(m_nBlockIndex[i] != 0)
		{
			if((pBlockData	= g_pBlock->GetBlockData(m_nBlockIndex[i])) != NULL)
			{
				for (j = 0; j < (int)pBlockData->dwFactorTotal; j++)
				{
					if(nIndex < m_nMoldsetCount)
					{
						m_pMoldSetIndexs[nIndex] = g_pDatabase->IndexToDataID(pBlockData->pwDBIndex[j]);
						//printf("m_pMoldSetIndexs[nIndex]=%d \n", m_pMoldSetIndexs[nIndex]); 
						nIndex++;
					}	
				}
				//printf("init Index =%d \n", nIndex);
			}
		}	
	}
}

int		CtmTaskDsp28_M3::CalMoldsetCount()
{
	BLOCKDATA*		pBlockData = NULL;
	int		nCount = 0;
	for(int i = 0; i < m_nBlockNum; i++)
	{
		if(m_nBlockIndex[i] != 0)
		{
			if((pBlockData	= g_pBlock->GetBlockData(m_nBlockIndex[i])) != NULL)
			{
				nCount += pBlockData->dwFactorTotal;
			}
		}
	}
	return nCount;
}

int		CtmTaskDsp28_M3::SendMoldsetData(int nIndex)
{
	int nInnerIndex = nIndex * m_nMaxElements;
	int	nSendCount = m_nMaxElements;
	//printf("SendMoldsetData nIndex=%d nInnerIndex=%d \n", nIndex, nInnerIndex);
	if(nInnerIndex < m_nMoldsetCount)
	{
		if((m_nMoldsetCount - nInnerIndex) >= m_nMaxElements)
		{
			nSendCount = m_nMaxElements;
			WriteValue(CONST_REQ_MOLDSET, nSendCount, &m_pMoldSetIndexs[nInnerIndex], NULL);
			//printf("1 SendMoldsetData nIndex=%d nSendCount=%d \n", nIndex, nSendCount);	
		}
		else
		{
			nSendCount = m_nMoldsetCount - nInnerIndex;
			WriteValue(CONST_REQ_MOLDSET, nSendCount, &m_pMoldSetIndexs[nInnerIndex], NULL);	
			//ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &m_nMoldsetCount, NULL);	
			//printf("2 SendMoldsetData nIndex=%d nSendCount=%d m_nMoldsetCount=%d \n", nIndex, nSendCount, m_nMoldsetCount);	
		}
	}
		
	
	return nSendCount;		
}

void	CtmTaskDsp28_M3::_Open(void *pData)
{	
	//printf("CtmTaskDsp28_M3::_Open \n");
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		;
	}
	m_pSelf->m_nConnecting				= 1;
	int	nTime 	= 0;
	int	nNum 	= 0;
	static	BOOL	bSync = FALSE;
	while(1)
	{
		long	lDsp28CommState = GetDBValue("STATE_OTHR_OTHR_NULL_RSVCOFG12").lValue;		//James add 2010/5/14 當dsp28不在線，不需要發送, 54-rs232轉發暫時不需要判斷
		//printf("m_pSelf->m_nConnectStatus=%d \n", m_pSelf->m_nConnectStatus);
		if(m_pSelf->m_nConnectStatus != 0)
		{
			//printf("g_wDspStatus=%d lDsp28CommState=%d \n", g_wDspStatus, lDsp28CommState);
			if(g_wDspStatus == TRUE && lDsp28CommState != 0)
			{
				//	7 is LineStatus,	
				nTime = CNT_INT_CHECK_ONLNE_TIME;
				m_pSelf->ReqValues(CONST_REQ_LINESTATUS, 1, &nTime, NULL);				//	
			
				//	1 is Actual value
				nTime = 500;	//	the unit is ms, 
				
				m_pSelf->ReqValues(CONST_REQ_ACTUAL, g_Dsp28_RealElements, g_Dsp28RealID, &nTime);
				//printf("Request \n");
				bSync = TRUE;
			}
		}
		else
		{
			//printf("bSync =%d \n", bSync);
			if(g_wDspStatus == TRUE && lDsp28CommState != 0)
			{
				if(bSync)
				{
					nNum = m_pSelf->GetBlockToDsp28(BLOCK_DB_PLC_OTHER_SET, CONST_REQ_MOLDSET);
					nNum += m_pSelf->GetBlockToDsp28(BLOCK_DB_PLC_INJECT_SET, CONST_REQ_MOLDSET);
					nNum += m_pSelf->GetBlockToDsp28(BLOCK_DB_PLC_DOSING_SET, CONST_REQ_MOLDSET);
					nNum += m_pSelf->GetBlockToDsp28(BLOCK_DB_PLC_MOLD_SET, CONST_REQ_MOLDSET);
					m_pSelf->m_nMoldsetCount = nNum;
					m_pSelf->ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &nNum, NULL);
					//printf("bSync =====nNum=%d \n", nNum);
					m_pSelf->m_nConnecting	= 0;
					bSync = FALSE;	
				}
			}	
		}
		usleep(500 * 1000);
	}
	//	send the moldset and machine data to dsp28 when the network is connected
	//	9 is moldset, the moldset concept is abstract include the mold and machine data
	//nNum = m_pSelf->GetBlockToDsp28(BLOCK_DB_PLC_OTHER_SET, CONST_REQ_MOLDSET);
	//nNum += m_pSelf->GetBlockToDsp28(BLOCK_DB_PLC_INJECT_SET, CONST_REQ_MOLDSET);
	//nNum += m_pSelf->GetBlockToDsp28(BLOCK_DB_PLC_DOSING_SET, CONST_REQ_MOLDSET);
	//nNum += m_pSelf->GetBlockToDsp28(BLOCK_DB_PLC_MOLD_SET, CONST_REQ_MOLDSET);
	//m_pSelf->ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &nNum, NULL);
	//printf("=====nNum=%d \n", nNum);
	//m_pSelf->m_nConnecting	= 0;
}

/*!	\fn int	CtmHmiPackage::ReqValues(int nType, int nElements, char* pszIDs[], void* pEx)
 *	\brief	向主機請求數據(發送請求的信息給主機)
 *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pszIDs		請求數據的ID(String類型)
 *	\param	[in]	pEx			附加信息(一般狀況下，是cycle Time)
 *	\return 錯誤數
 */
int		CtmTaskDsp28_M3::ReqValues(int nType, int nElements, char* pszIDs[], void* pEx)
{
	int nResult = 0;
	
	if (m_pHostProtocol != NULL)
	{
		if (nElements > 0)
		{
			//if(nType == CONST_REQ_ACTUAL)
			//{
			//	g_Dsp28_RealElements = nElements;
			//	for(int i = 0; i < g_Dsp28_RealElements; i++)
			//	{
			//		g_Dsp28_RealBackID[i] = pszIDs[i];
			//		//printf("g_Dsp28_RealBackID [%d]= %s\n", i, g_Dsp28_RealBackID[i]);
			//	}
			//}
			int*	pIndex = new int[nElements];
			char 	cszStrID[256];
			int		j = 0;		//統計資料ID不存在的
	
			memset(pIndex, 0, sizeof(int) * nElements);
			
			for (int i = 0; i < nElements; i++)
			{
				if(pszIDs[i] != NULL)			//James add 2008/5/7 05:47下午
				{
					memset(cszStrID, 0, sizeof(cszStrID));
					Trim(cszStrID, pszIDs[i], sizeof(cszStrID));
					pIndex[i-j] = g_pDatabase->GetDataID(cszStrID);
					if(pIndex[i-j] == -1)		//James add 2010/7/8 08:31下午
					{
						j++;	
					}
				}
			}
			//printf("nType=%d, nElements=%d \n", nType, nElements);
			nResult = ReqValues(nType, nElements-j, pIndex, pEx);
			if(pIndex != NULL)
			{
				delete []pIndex;
				pIndex = NULL;
			}
		}
	}
		
	return nResult;
}

/*!	\fn int	CtmHmiPackage::ReqValues(int nType, int nElements, int pIndexs[], void* pEx)
 *	\brief	向主機請求數據(發送請求的信息給主機)(此請求信息有可能是寫、也有可能是讀)
 *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pIndexs		請求數據的ID(int),根據實際需求決定是Index還是DataID
 *	\param	[in]	pEx			附加信息(一般狀況下，是cycle Time)
 *	\return 錯誤數
 */
int		CtmTaskDsp28_M3::ReqValues(int nType, int nElements, int pIndexs[], void* pEx)
{
	memset(m_pData, 0, m_nMaxLen);
	//static	int nPackageNum = 0;
	//printf("ReqValues nType=%d nElements=%d m_nMaxElements=%d \n", nType, nElements, m_nMaxElements);
	if (m_pHostProtocol != NULL)
	{
		
		//char	szName[128];
		//if(nType == CONST_REQ_READMOTOR&& nElements > m_nMaxElements)
		//{
		//	m_pReadIDs = pIndexs;
		//	m_wReadIDNum = nElements;
		//	m_wReadIDIndex		= 0;
		//	//ReqReadData();
		//}
		//else
		{
			int	nNum = nElements / m_nMaxElements ;
			int	nLen = 0;
			
			if(nType == CONST_REQ_SIEMREAD_DIAG || nType == CONST_REQ_SIEMWRITE_DIAG) //診斷西門子資料
			{
				nLen = m_pHostProtocol->Compose(nType, m_pData,  pIndexs, nElements,pEx);
				
				if(CNT_TRANSMIT_FLAG == 0)
				{
					if(nLen > 0 && g_ptaskdsp != NULL && g_wDspStatus == TRUE)
					{
						g_ptaskdsp->WriteValue(COMM_SENDTRANSMITDSP28, (void*)m_pData, nLen);
					}
				}
				else
				{
					if (nLen > 0 && m_nDeviceID >= 0)
					{
						SendData(m_nDeviceID, (char*)m_pData, nLen);
						//usleep(30 * 1000);
					}
					else
						g_tmDebugInfo->PrintDebugInfo("nLen =%d, Device id =%d\n", nLen, m_nDeviceID);
				}	
				return 0;	
			}
		
			//printf("ReqValues 1 nLen=%d nType=%d nElements=%d \n", nLen, nType, nElements);
			for (int i = 0; i < nNum; i++)
			{
				memset(m_pData, 0, m_nMaxLen);
				nLen 	= m_pHostProtocol->Compose(nType, m_pData, pIndexs, m_nMaxElements, pEx);
				pIndexs = pIndexs + m_nMaxElements;
				
				//printf("g_ptaskdsp = %d, g_wDspStatus = %d\n", g_ptaskdsp, g_wDspStatus);
				
				if(CNT_TRANSMIT_FLAG == 0)
				{
					if(nLen > 0 && g_ptaskdsp != NULL && g_wDspStatus == TRUE)		//James add for test Austone 2010/5/18 下午 04:58:02
					{
						g_ptaskdsp->WriteValue(COMM_SENDTRANSMITDSP28, (void*)m_pData, nLen);
					}
				}
				else
				{
					if (nLen > 0 && m_nDeviceID >= 0)
					{
						SendData(m_nDeviceID, (char*)m_pData, nLen);
						//usleep(30 * 1000);
					}
				}
			}
			
			int	nLast = nElements - nNum * m_nMaxElements;
			
			if (nLast > 0)
			{
				//getchar();
				memset(m_pData, 0, m_nMaxLen);
				nLen = m_pHostProtocol->Compose(nType, m_pData,  pIndexs, nLast,pEx);
				
				//printf("ReqValues 2 nLen=%d g_ptaskdsp=%x , g_wDspStatus = %d, nLen = %d\n", nLen, g_ptaskdsp, g_wDspStatus, nLen);
				
				if(CNT_TRANSMIT_FLAG == 0)
				{
					if(nLen > 0 && g_ptaskdsp != NULL && g_wDspStatus == TRUE)		//James add for test happy plus 2010/3/2 上午 11:48:13
					{
						g_ptaskdsp->WriteValue(COMM_SENDTRANSMITDSP28, (void*)m_pData, nLen);
					}
				}
				else
				{
					if (nLen > 0 && m_nDeviceID >= 0)
					{
						SendData(m_nDeviceID, (char*)m_pData, nLen);
						//usleep(30 * 1000);
					}
					else
						g_tmDebugInfo->PrintDebugInfo("nLen =%d, Device id =%d\n", nLen, m_nDeviceID);
				}
			}
		}
	}
	return 0;
}

/*!	\fn int	CtmHmiPackage::ReqValues(int nType, char* pszID, void* pEx)
 *	\brief	向主機請求數據(發送請求的信息給主機)(此請求信息有可能是寫、也有可能是讀)
 *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	pszID		數據ID
 *	\param	[in]	pEx			附加信息(一般狀況下，是cycle Time)
 *	\return 錯誤數
 */
int		CtmTaskDsp28_M3::ReqValues(int nType, char* pszID, void* pEx)
{
	if (m_pHostProtocol != NULL)
	{
		char 	cszStrID[256];
		int		nLen	= 0;
		memset(cszStrID, 0, sizeof(cszStrID));
		Trim(cszStrID, pszID, sizeof(cszStrID));		
		int nIndex 	= g_pDatabase->GetDataID(cszStrID);
		memset(m_pData, 0, m_nMaxLen);
		if(nIndex != -1)
			nLen 	= m_pHostProtocol->Compose(nType, m_pData, &nIndex, 1, pEx);
		
		if(CNT_TRANSMIT_FLAG == 0)
		{
			if(nLen > 0 && g_ptaskdsp != NULL && g_wDspStatus == TRUE)		//James add for test happy plus 2010/3/2 上午 11:48:13
			{
				g_ptaskdsp->WriteValue(COMM_SENDTRANSMITDSP28, (void*)m_pData, nLen);
			}
		}
		else
		{
			if (nLen > 0 && m_nDeviceID >= 0)
			{
				//g_tmDebugInfo->LogData(szName, m_pData, nLen);
				SendData(m_nDeviceID, (char*)m_pData, nLen);
				//usleep(20 * 1000);
			}
		}
	}
	return 0;
}

int 	CtmTaskDsp28_M3::ReqValues(int nType, void* pOutData, int nSize)
{
	
	
    memset(m_pData, 0, m_nMaxLen);
    int nTime = 500;
   // if(nType == 0xff)
   // {
   // 	m_pSelf->ReqValues(CONST_REQ_ACTUAL, sizeof(g_Dsp28RealID) / sizeof(g_Dsp28RealID[0]), g_Dsp28RealID, &nTime);
   // 	return 0;
   // }
   // else if(nType == CONST_REQ_PWMTRDATA && m_pHostProtocol != NULL)
    {
    	int	nLen = m_pHostProtocol->Compose(nType, m_pData,  pOutData, nSize, NULL);
			
		if(CNT_TRANSMIT_FLAG == 0)
		{
			if(nLen > 0 && g_ptaskdsp != NULL && g_wDspStatus == TRUE)		//James add for test happy plus 2010/3/2 上午 11:48:13
			{
				g_ptaskdsp->WriteValue(COMM_SENDTRANSMITDSP28, (void*)m_pData, nLen);
			}
		}
		else
		{
			if (nLen > 0 && m_nDeviceID >= 0)
			{
				SendData(m_nDeviceID, (char*)m_pData, nLen);
				//usleep(30 * 1000);
			}
			else
				g_tmDebugInfo->PrintDebugInfo("nLen =%d, Device id =%d\n", nLen, m_nDeviceID);
		}
		return 0;
    }
    return m_pHostProtocol->Compose(nType, pOutData, NULL, nSize, NULL);
}

/*!	\fn int	CtmHmiPackage::StopReq(int nType, int nElements, char* pIDs[], void* pEx)
 *	\brief	停止向主機請求數據
  *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pszIDs		請求數據的ID(String類型)
 *	\param	[in]	pEx			附加信息
 *	\return 錯誤數
 */
int		CtmTaskDsp28_M3::StopReq(int nType, int nElements, char* pIDs[], void* pEx)
{
	return 	StopReq(nType, NULL);
	
}

/*!	\fn int	CtmHmiPackage::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
 *	\brief	停止向主機請求數據
  *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pIndexs		請求數據的ID
 *	\param	[in]	pEx			附加信息
 *	\return 錯誤數
 */
int		CtmTaskDsp28_M3::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
{
	return 	StopReq(nType, NULL);
}


/*!	\fn int	CtmHmiPackage::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
 *	\brief	停止向主機請求數據
  *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	pszIDs		請求數據的ID(String類型)
 *	\param	[in]	pEx			附加信息
 *	\return 錯誤數
 */
int		CtmTaskDsp28_M3::StopReq(int nType, char* pszID, void* pEx)
{
	memset(m_pData, 0, m_nMaxLen);
	int nLen = m_pHostProtocol->Compose(nType, m_pData, NULL, 0, pEx);
	if (nLen > 0 && m_nDeviceID >= 0)
	{		
		SendData(m_nDeviceID, (char*)m_pData, nLen);
		//usleep(20 * 1000);
	}
	return 0;
}
/*!	\fn int	CtmHmiPackage::WriteValue(int nType, int nElements, char* pszIDs[], void* pEx)
 *	\brief	寫數據到主機(利用ReqValues是相同的道理)
  *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pszIDs		請求數據的ID(String類型)
 *	\param	[in]	pEx			附加信息
 *	\return 錯誤數
 */
int		CtmTaskDsp28_M3::WriteValue(int nType, int nElements, char* pszIDs[], void* pEx)
{
	if(nType == CONST_REQ_MOLDSET)
	{
		m_nSyncState = 1;
		m_nSyncDBIndex = 0;	
		m_pHostProtocol->Compose(CONST_REQ_MOLDSET, NULL, NULL, 0, NULL);
		SendMoldsetData(m_nSyncDBIndex);
	}
	else
		{
		return ReqValues(nType, nElements, pszIDs, pEx);
	}
}

int		CtmTaskDsp28_M3::WriteValue(int nType, int nElements, int pIndexs[], void* pEx)
{
	return ReqValues(nType, nElements, pIndexs, pEx);
}

int		CtmTaskDsp28_M3::WriteValue(int nType, char* pszID, void* pEx)
{
	return ReqValues(nType, pszID, pEx);
}

int		CtmTaskDsp28_M3::WriteValue(int nDataID)
{
	int	nTempID = nDataID;
	
	return WriteValue(3, 1, &nTempID);
}

int		CtmTaskDsp28_M3::WriteValue(char* pszID)
{
	return WriteValue(3, 1, &pszID);
}

int 	CtmTaskDsp28_M3::WriteValue(int nType, void* pInputData, int nSize)
{
	WORD	wCmdType = 0;
	memset(m_pData, 0, m_nMaxLen);
	//printf("nType=%d nSize=%d \n", nType, nSize);
	if(nType == COMM_GETAUSTIONDRIVEINFO)		//讀取dsp28資料
	{
		GetData(pInputData, nSize);
		return nSize;
	}
    return m_pHostProtocol->Compose(nType, NULL, pInputData, nSize, NULL);
}

/*!	\fn int	CtmTaskDsp28_M3::GetData(void* pdata, int nLen)
 *	\brief	接收發送過來的數據
 *	\param	[in]	pdata	接收的數據
 *	\param	[in]	nLen	接收的數據長度
 *	\return CTaskHost::GetData(pdata, nLen)
 */
int		CtmTaskDsp28_M3::GetData(void* pdata, int nLen)
{
	int nGetType = m_pHostProtocol->GetType(pdata, nLen);
	int	nResult = 0, nRecvNum = 0;
	static	int nGetCount = 0;
	int		nTime = CNT_INT_CHECK_ONLNE_TIME;
	WORD	wHeadCount =  *(WORD*)((BYTE*)pdata + 6);
//	int	nRecvNum = 0;
	//printf("GetData nGetType=%d nLen=%d m_bDiagFlag=%d \n", nGetType, nLen, m_bDiagFlag);  
	switch(nGetType)
	{
		case CONST_RES_WRITEMOTOR:	//CONST_REQ_WRITEMOTOR: 2010/8/16 上午 11:36:19
			if (m_bDiagFlag)
			{		
				nGetType = nGetType | 0x8000;
				*(WORD*)((BYTE*)pdata + 4) = (WORD)nGetType;
			}
			m_pHostProtocol->Parse(pdata, nLen);
			break;
		case CONST_RES_READMOTOR:
			if (m_bDiagFlag)
			{		
				nGetType = nGetType | 0x8000;
				*(WORD*)((BYTE*)pdata + 4) = (WORD)nGetType;
			}
			m_pHostProtocol->Parse(pdata, nLen);
				
			break;
		case CONST_RES_LINESTATUS:		//每個1s 發送一次連線，收到回复表示連線正常
			if(m_nConnectStatus != 0)	//剛連上線
			{
				u_bSendOnLineFlag = TRUE;
			}
			if(m_nSyncState == 0)
			{
				m_nSyncState = 1;
				m_nSyncDBIndex = 0;	
				
				//printf("========m_nSyncState \n");
				//m_nCounter	= 0;
				if(CNT_TRANSMIT_FLAG == 0)
				{
					//if(g_ptaskdsp != NULL)			//James add 2014/6/23 21:48:45 for onoffline clear buff
					//	g_ptaskdsp->WriteValue(0xFF7771, NULL, 0);
				}			
				m_pHostProtocol->Compose(CONST_REQ_MOLDSET, NULL, NULL, 0, NULL);
				//WriteValue(CONST_REQ_MOLDSET, 0, (int*)NULL, NULL);		//Reset Counter
				SendMoldsetData(m_nSyncDBIndex);
				//InitData();
			}
			break;
		case CONST_RES_ACTUAL:
			m_pHostProtocol->Parse(pdata, nLen);
			m_pSelf->m_nConnectStatus 	= 0;
			//m_pSelf->m_lOnLineTime = CNT_INT_CHECK_ONLNE_TIME;
			if(m_nSyncState != 1)
				m_pSelf->m_lCheckQuickTimer	= /*CNT_INT_CHECK_ONLNE_TIME + */5000;
			//else
			//	m_pSelf->m_lCheckQuickTimer	= CNT_INT_CHECK_ONLNE_TIME;
			//printf("CONST_RES_ACTUAL  m_nSyncState = %d\n", m_nSyncState);
			break;
		case CONST_RES_MOLDSETSTATUS:
			memcpy(&nRecvNum,((BYTE*)pdata+12),sizeof(long));//m_pHostProtocol->Parse(pdata, nLen); /*wangli2016-7-8 14:10:02 修正有時 與28通訊不上的問題*/
			//printf("nRecvNum=%d m_nMoldsetCount=%d  sync=%d\n", nRecvNum, m_nMoldsetCount,m_nSyncDBIndex);
			m_lOnLineTime=CNT_INT_ACTUALVALUE_TIME*2;
			if(m_nMoldsetCount != nRecvNum)
			{
				m_nSyncState 	= 0;
				m_nSyncDBIndex 	= 0;
				//m_nCounter		= 0;
				m_pHostProtocol->Compose(CONST_REQ_MOLDSET, NULL, NULL, 0, NULL);
				//WriteValue(CONST_REQ_MOLDSET, 0, (int*)NULL, NULL);		//Reset Counter
				//m_SendMoldNum = CtmMoldSet::GetInstance()->SendControlMoldSet();
				//WriteValue(CONST_REQ_MOLDSETSTATUS, 1, &m_SendMoldNum,NULL);
			}
			else if(m_nMoldsetCount == nRecvNum)
			{
				m_nSyncState 	= 2;	
				m_nSyncDBIndex 	= 0;
				//m_nCounter		= 0;
				m_pHostProtocol->Compose(CONST_REQ_MOLDSET, NULL, NULL, 0, NULL);
				//WriteValue(CONST_REQ_MOLDSET, 0, (int*)NULL, NULL);		//Reset Counter
				nTime = 500;
				ReqValues(CONST_REQ_ACTUAL, g_Dsp28_RealElements, g_Dsp28RealID, &nTime);
			}
			break;
		case CONST_RES_MOLDSET:
			//printf("m_nSyncState=%d m_OldHeadCount=%d wHeadCount=%d \n", m_nSyncState, m_OldHeadCount, wHeadCount);
			//if(m_OldHeadCount != wHeadCount)
			//{
			//	m_OldHeadCount = wHeadCount;	
			//}
			//else
			//{
				//printf("===================break=====================\n");
			//	break;
			//}
			//printf("m_nSyncDBIndex=%d  m_nSyncState=%d \n", m_nSyncDBIndex, m_nSyncState);
			m_lOnLineTime=CNT_INT_ACTUALVALUE_TIME/2;
			if(m_nSyncState == 1)
			{
				//add by J.Wong 賤昴婦唗瘍 2016/8/23 15:59:40
				m_pHostProtocol->Parse(pdata, nLen);
				
				if(((m_nSyncDBIndex+1)*m_nMaxElements) >= m_nMoldsetCount)
				{
					ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &m_nMoldsetCount, NULL);
					//m_nSyncState = 2;
					//printf(" ============================end m_nMoldsetCount=%d \n", m_nMoldsetCount);
				}
				else
				{	
					//add by J.Wong MoldSet唗瘍梓暮党淏,悵痐肮祭2016/8/23 15:58:50
					if(m_nSyncDBIndex<=wHeadCount)
						m_nSyncDBIndex =wHeadCount++;
					SendMoldsetData(m_nSyncDBIndex);
					m_pSelf->m_lCheckQuickTimer	= CNT_INT_CHECK_ONLNE_TIME ;
				}
				
				m_pSelf->m_lCheckQuickTimer	= CNT_INT_CHECK_ONLNE_TIME ;
			}
			break;
		default:
			if (m_bDiagFlag)
			{		
				nGetType = nGetType | 0x8000;
				*(WORD*)((BYTE*)pdata + 4) = (WORD)nGetType;
			}
			
			m_pHostProtocol->Parse(pdata, nLen);
			break;	
	}

	
	return CTaskHost::GetData(pdata, nLen);
}

/*!	\fn int	CtmTaskDsp28_M3::ResValueFunc(char *pData, int nLength)
 *	\brief	接收主機發送過來的數據(采用輪回方式接收數據)
 *	\param	[in]	pdata	接收的數據
 *	\param	[in]	nLen	接收的數據長度
 *	\return 
 */
void	CtmTaskDsp28_M3::ResValueFunc(char *pData, int nLength)
{
	m_pSelf->GetData(pData, nLength);
	
	MSG		msg;
	msg.message	= MSG_DSP28;
	g_pApplication->QueueMessage(&msg);
}

int		CtmTaskDsp28_M3::SendZeroSet(int nZeroType)
{
	return 0;
}

/*!	\fn int	CtmTaskDsp28_M3::GetOnLineStatus()
 *	\brief	判斷連線是否正常		
 *	\return 連線狀態
 */
int		CtmTaskDsp28_M3::GetOnLineStatus()
{   
	long	lDsp28CommState = GetDBValue("STATE_OTHR_OTHR_NULL_RSVCOFG12").lValue;		//James add 2010/5/14 當dsp28不在線，不需要發送, 54-rs232轉發暫時不需要判斷
	int		nTime = 0;
	//printf("lDsp28CommState=%d m_lCheckQuickTimer=%d \n", lDsp28CommState, m_lCheckQuickTimer);
	
	if(CNT_TRANSMIT_FLAG == 0)
	{
		if(g_wDspStatus != 0 && lDsp28CommState == 1)
		{
			;
		}
		else
		{	
			m_nConnectStatus = m_nConnectStatus | 1;	
			return m_nConnectStatus;
		}
	}
	
	if (m_lCheckQuickTimer <= 0)
	{
		//m_nConnectStatus = 1 表示重新鏈接  =2表示斷線狀態 =0表示連接上
		m_nConnectStatus = m_nConnectStatus | 1;
		BOOL	bConnect	= FALSE;
		//printf("1  m_nConnectStatus=%d\n",m_nConnectStatus);
		//printf("Reopen bConnect=%d m_nConnecting=%d \n", bConnect, m_nConnecting);
		
		if(CNT_TRANSMIT_FLAG == 0)
		{
			m_nConnectStatus = m_nConnectStatus | 2;
			nTime = CNT_INT_CHECK_ONLNE_TIME;
			ReqValues(CONST_REQ_LINESTATUS, 1, &nTime, NULL);		
			nTime = 100;	//	the unit is ms, 
			//ReqValues(CONST_REQ_ACTUAL, g_Dsp28_RealElements, g_Dsp28RealID, &nTime);
			bConnect		= TRUE;
			m_nSyncState	= 0;
			m_nSyncDBIndex  = 0;	
			m_lCheckQuickTimer = CNT_INT_SEND_LINESTATE_TIME;
		}
		else
		{
			if ((GetState(m_nDeviceID) != DEVICE_S_OPENED) &&
			(m_nConnecting != 1))
			{
				m_nConnecting	= 1;
				m_nConnectStatus = m_nConnectStatus | 2;
				bConnect		= TRUE;
				pthread_cancel(m_OpenThreadID);
				
				pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
			}
			if ((!bConnect) && (m_nConnecting != 1))
			{
				m_nConnecting	= 1;
				m_nConnectStatus = m_nConnectStatus | 2;
				pthread_cancel(m_OpenThreadID);
				pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
			}
			m_lCheckQuickTimer = CNT_INT_SEND_LINESTATE_TIME;
		}
		
		//printf("2  m_nConnectStatus=%d\n",m_nConnectStatus);
		//printf("cancel 1 \n");
			//pthread_cancel(m_OpenThreadID);
			
			//pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
		//}
		
		//if ((!bConnect) && (m_nConnecting != 1))
		//{
		//	m_nConnecting	= 1;
		//	m_nConnectStatus = m_nConnectStatus | 2;
		//	printf("cancel 1 \n");
		//	//pthread_cancel(m_OpenThreadID);
		//	//pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
		//}
		
	} 
		//printf("3  m_nConnectStatus=%d\n",m_nConnectStatus);
	return m_nConnectStatus;
}

/*!	\fn int	CtmTaskDsp28_M3::RestartNet(BOOL bFlag)
 *	\brief	重新啟動網絡		
 *	\return 連線狀態
 */
BOOL	CtmTaskDsp28_M3::RestartNet(BOOL bFlag)
{

}


void	CtmTaskDsp28_M3::Run()
{
	static BOOL b_Flag = TRUE;;
	static BOOL b_TemperAutoPID = FALSE;
	static BOOL b_TemperMoldAutoPID = FALSE;
	int	nTime = CNT_INT_CHECK_ONLNE_TIME;
	//BOOL	bDsp28CommState = TRUE;
	long	lDsp28CommState = GetDBValue("STATE_OTHR_OTHR_NULL_RSVCOFG12").lValue;
	long	lTemperAdjState = 0 , lTemperMoldAdjState = 0;
	m_nConnectStatus = GetOnLineStatus();

	//printf("lDsp28CommState=%d g_wDspStatus=%d m_nConnectStatus=%d\n", lDsp28CommState, g_wDspStatus,m_nConnectStatus);
	/*ZHBA 2013-4-18 */
	if(g_wDspStatus == TRUE && lDsp28CommState != 0)
	{
		//add by J.Wong  MoldSet訧蹋隍婦笭楷 2016/8/23 15:55:55
		if(m_nSyncState ==1)
		{
			if(m_lOnLineTime<=0)
			{
				
				m_lOnLineTime=CNT_INT_ACTUALVALUE_TIME/2;
				SendMoldsetData(m_nSyncDBIndex);	
			}
		}
		if(m_lTempReqTime <=0)	
		{	
			if(m_nConnectStatus==0 && m_nSyncState == 2/*&& m_nConnecting == 0*/)
			{
				//ReqValues(2, sizeof(g_DspM3ReadTempID) / sizeof(g_DspM3ReadTempID[0]), g_DspM3ReadTempID);
				
				nTime = CNT_INT_CHECK_ONLNE_TIME;
				
				//AutoPID Ok, Request PID Data
				//James add 2013/5/5 21:00:19
				lTemperAdjState = GetDBValue("M3_CONF_TEMPERCARD_AUTOPID_STATE").lValue;
				if(lTemperAdjState == 2)
				{
					//printf("=========lTemperAdjState=%d \n", lTemperAdjState);
					ReqValues(2, sizeof(g_TemperRealPID) / sizeof(g_TemperRealPID[0]), g_TemperRealPID);
						
					if(m_bDC)
						ReqValues(2, sizeof(g_Temper2RealPID) / sizeof(g_Temper2RealPID[0]), g_Temper2RealPID); /*雙色機第2塊溫度卡*/
					else	if(m_bT11Port19)
						ReqValues(2, sizeof(g_TemperT11RealPID) / sizeof(g_TemperT11RealPID[0]), g_TemperT11RealPID); /*雙色機第2塊溫度卡*/

					b_TemperAutoPID = TRUE;
					SetDBValue("M3_CONF_TEMPERCARD_AUTOPID_CMD", 0);	
				}
				else
				{
					if(b_TemperAutoPID)
					{
						WORD	wPIDValue = 0;
						char	pszPID[256];
						int   nTempPIDSeg = 9, nTempStartNum = 0;
						
						if(m_bDC)  /*雙色機第2塊溫度卡  13~21*/
						{
							nTempPIDSeg = 18; 
						}
							
						for(int i = 0; i < nTempPIDSeg; i++)
						{	
							if(i > 8)
								nTempStartNum = 3;
							
							memset(pszPID, 0, sizeof(pszPID));
							sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_USE",i+1+nTempStartNum);
							wPIDValue = GetDBValue(pszPID).lValue;
							if(wPIDValue == 1)
							{
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDREAL_KP",i+1+nTempStartNum);
								wPIDValue = GetDBValue(pszPID).lValue;
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDSET_KP",i+1+nTempStartNum);
								SetDBValue(pszPID, wPIDValue);
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDREAL_KI",i+1);
								wPIDValue = GetDBValue(pszPID).lValue;
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDSET_KI",i+1+nTempStartNum);
								SetDBValue(pszPID, wPIDValue);
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDREAL_KD",i+1+nTempStartNum);
								wPIDValue = GetDBValue(pszPID).lValue;
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDSET_KD",i+1+nTempStartNum);
								SetDBValue(pszPID, wPIDValue);
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDREAL_RAMP",i+1+nTempStartNum);
								wPIDValue = GetDBValue(pszPID).lValue;
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDSET_RAMP",i+1+nTempStartNum);
								SetDBValue(pszPID, wPIDValue);
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_LSEG%d_PIDREAL_STABLEDUTY",i+1+nTempStartNum);
								wPIDValue = GetDBValue(pszPID).lValue;
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_LSEG%d_PIDSET_STABLEDUTY",i+1+nTempStartNum);
								SetDBValue(pszPID, wPIDValue);
								//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
							}
							//printf("g_TemperPID[i]=%s wPIDValue=%d \n", g_TemperRealPID[i], wPIDValue);
						}
						b_TemperAutoPID = FALSE;
						SendMsg(MSG_HYB5_TEMPAUTOPID_COMPLETED, 0, 0, NULL);  /*JOYCE2015-3-12  AUTOPID finished*/	
					}		
				
					if((lTemperAdjState == 1)&&(GetDBValue("M3_CONF_TEMPERCARD_AUTOPID_CMD").lValue==1))
						SendMsg(MSG_HYB5_TEMPAUTOPID_DOING, 0, 0, NULL);      /*JOYCE2015-3-12  AUTOPID doing*/
				}
				
				if(m_bMold36)
				{
					lTemperMoldAdjState = GetDBValue("M3_CONF_TEMPERCARD_MOLD_AUTOPID_STATE").lValue;
					
					//printf("lTemperMoldAdjState =%d\n",lTemperMoldAdjState);
					if(lTemperMoldAdjState == 2)
					{
						ReqValues(2, sizeof(g_Temper3RealPID) / sizeof(g_Temper3RealPID[0]), g_Temper3RealPID); /*第3塊溫度卡*/
						ReqValues(2, sizeof(g_Temper4RealPID) / sizeof(g_Temper4RealPID[0]), g_Temper4RealPID); /*第4塊溫度卡*/
						ReqValues(2, sizeof(g_Temper5RealPID) / sizeof(g_Temper5RealPID[0]), g_Temper5RealPID); /*第5塊溫度卡*/
						b_TemperMoldAutoPID = TRUE;
						SetDBValue("M3_CONF_TEMPERCARD_MOLD_AUTOPID_CMD", 0);	
					}
					else
					{
						if(b_TemperMoldAutoPID)
						{
							WORD	wPIDValue = 0;
							char	pszPID[256];
							int   nTempPIDSeg = 60, nTempStartNum = 0;
															
							for(int i = 24; i < nTempPIDSeg; i++)
							{	
								
								memset(pszPID, 0, sizeof(pszPID));
								sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_USE",i+1+nTempStartNum);
								wPIDValue = GetDBValue(pszPID).lValue;
								if(wPIDValue == 1)
								{
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDREAL_KP",i+1+nTempStartNum);
									wPIDValue = GetDBValue(pszPID).lValue;
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
									
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDSET_KP",i+1+nTempStartNum);
									SetDBValue(pszPID, wPIDValue);
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
									
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDREAL_KI",i+1);
									wPIDValue = GetDBValue(pszPID).lValue;
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
									
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDSET_KI",i+1+nTempStartNum);
									SetDBValue(pszPID, wPIDValue);
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
									
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDREAL_KD",i+1+nTempStartNum);
									wPIDValue = GetDBValue(pszPID).lValue;
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
									
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDSET_KD",i+1+nTempStartNum);
									SetDBValue(pszPID, wPIDValue);
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
									
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDREAL_RAMP",i+1+nTempStartNum);
									wPIDValue = GetDBValue(pszPID).lValue;
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
									
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_WSEG%d_PIDSET_RAMP",i+1+nTempStartNum);
									SetDBValue(pszPID, wPIDValue);
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
									
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_LSEG%d_PIDREAL_STABLEDUTY",i+1+nTempStartNum);
									wPIDValue = GetDBValue(pszPID).lValue;
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
									
									memset(pszPID, 0, sizeof(pszPID));
									sprintf(pszPID, "TEMPERCARD1_INTERFACE_LSEG%d_PIDSET_STABLEDUTY",i+1+nTempStartNum);
									SetDBValue(pszPID, wPIDValue);
									//printf("pszPID=%s wPIDValue=%d \n", pszPID, wPIDValue);
								}
								//printf("g_TemperPID[i]=%s wPIDValue=%d \n", g_TemperRealPID[i], wPIDValue);
							}
							b_TemperMoldAutoPID = FALSE;
							SendMsg(MSG_HYB5_TEMPMOLDAUTOPID_COMPLETED, 0, 0, NULL);  /*JOYCE2015-3-12  AUTOPID finished*/	
						}		
						
						if((lTemperMoldAdjState == 1)&&(GetDBValue("M3_CONF_TEMPERCARD_MOLD_AUTOPID_CMD").lValue==1))
							SendMsg(MSG_HYB5_TEMPMOLDAUTOPID_DOING, 0, 0, NULL);      /*JOYCE2015-3-12  AUTOPID doing*/
						
					}
				}

				if(m_bJ6Redy)
				{
					WORD wJ6State = GetDBValue("M3_STATE_CARD2_LINESTATE").lValue;
					if(wJ6State == 15)
					{
						cout << "Set Time To J6" << endl;
						SetDBValue("M3_MASTER_WATCH_TYPE",0,FALSE);/*先附此值但不傳28，為了避免DSP單獨斷電或者上一次在當前畫面關電，開機后直接進此畫面，導致無法映射的問題*/
						SetDBValue("M3_MASTER_WATCH_TYPE",1);/*啟用SDO功能*/
						SetDBValue("M3_MASTER_WATCH_ADDR", 0xFFFFFFFF, FALSE);
						SetDBValue("M3_MASTER_WATCH_ADDR", 0x57450020);  /*zhongjw 2016-7-23 寫入參數的地址,同步系統時間*/
						SetDBValue("M3_MASTER_WATCH_OFFSET0_VALUE", 0xFFFFFFFF, FALSE); /*先附此值但不傳28，為了避免出現跟別的參數同樣的value值，導致無法寫入*/
						SetDBValue("M3_MASTER_WATCH_OFFSET0_VALUE", SetTimeToJ6()); /*寫入的值*/
						SetDBValue("M3_MASTER_WATCH_TYPE",0);/*關閉SDO功能*/
						m_bJ6Redy = FALSE;
					}
				}
			}
			ReqValues(CONST_REQ_LINESTATUS, 1, &nTime, NULL);				//	
			//printf("ReqValue CONST_REQ_LINESTATUS  \n");
			m_lTempReqTime	= CNT_INT_ACTUALVALUE_TIME*2;
		}
	}
}

void	CtmTaskDsp28_M3::SetPowerAmmeter()
{
	long	lValue1 = 0, lValue2 = 0, lValue3 = 0;
	
	lValue1 = GetDBValue("PWMTRCARD1_INTERFACE_FAVRMS").lValue;
	lValue2 = GetDBValue("PWMTRCARD1_INTERFACE_FBVRMS").lValue;
	lValue3 = sqrt(lValue1*lValue1 + lValue2*lValue2 + lValue1*lValue2);
	SetDBValue("PWMTRCARD1_INTERFACE_FABVRMS", lValue3, FALSE);
	
	lValue1 = GetDBValue("PWMTRCARD1_INTERFACE_FBVRMS").lValue;
	lValue2 = GetDBValue("PWMTRCARD1_INTERFACE_FCVRMS").lValue;
	lValue3 = sqrt(lValue1*lValue1 + lValue2*lValue2 + lValue1*lValue2);
	SetDBValue("PWMTRCARD1_INTERFACE_FBCVRMS", lValue3, FALSE);
	
	lValue1 = GetDBValue("PWMTRCARD1_INTERFACE_FAVRMS").lValue;
	lValue2 = GetDBValue("PWMTRCARD1_INTERFACE_FCVRMS").lValue;
	lValue3 = sqrt(lValue1*lValue1 + lValue2*lValue2 + lValue1*lValue2);
	SetDBValue("PWMTRCARD1_INTERFACE_FCAVRMS", lValue3, FALSE);
}
		
/*!	\fn int	CtmTaskDsp28_M3::GetBlockToDsp28(WORD wIndex, int nType)
 *	\brief	發送指定Block數據給主機		
 *	\param	[in]	wIndex	Block類型
 *	\param	[in]	nType	數據類型(協議定的)
 *	\return 連線狀態
 */
int		CtmTaskDsp28_M3::GetBlockToDsp28(WORD wIndex, int nType)
{
	BLOCKDATA*		pBlockData = NULL;
	if ((pBlockData	= g_pBlock->GetBlockData(wIndex)) == NULL)	return 0;
		
	if (pBlockData->dwFactorTotal == 0) 						return 0;
		
	int* pIndexs = new int[pBlockData->dwFactorTotal];
	//char	szName[128];
	
	//sprintf(szName, "Block%d.txt", wIndex);
	
	for (int i = 0; i < (int)pBlockData->dwFactorTotal; i++)
	{
		pIndexs[i] = g_pDatabase->IndexToDataID(pBlockData->pwDBIndex[i]);
		//g_tmDebugInfo->LogDebugInfo(szName, "wIndex = %d, ReqType = %d, ID = %3x, Index = %3x", 
		//									wIndex, nType, pIndexs[i], pBlockData->pwDBIndex[i]);
	}
	
	//g_tmDebugInfo->LogDebugInfo(szName, "total=%d\n", pBlockData->dwFactorTotal);
	
	WriteValue(nType, pBlockData->dwFactorTotal, pIndexs, NULL);
	int nResult = pBlockData->dwFactorTotal;
	delete []pIndexs;
	
	return nResult;
	
}

void	CtmTaskDsp28_M3::StartDiag()
{
	m_bDiagFlag = TRUE;
}

void	CtmTaskDsp28_M3::EndDiag()
{
	m_bDiagFlag = FALSE;
}

int		CtmTaskDsp28_M3::ReqValuesDiag(int pIDs[], int nElements, int nDiagType)
{
	int	nType	= nDiagType | 0x8000;
	int	nResult	= 0;
	if (nElements > 0)
	{
		nResult		= ReqValues(nType, nElements, pIDs, NULL);
	}
	return nResult;
}

int		CtmTaskDsp28_M3::ReqValuesDiag(int nStartID, int nElements, int nDiagType)
{
	int	nResult	= 0;
	if(nDiagType == CONST_REQ_SIEMREAD_DIAG)//西門子診斷
	{
		int  *pIDs = new int[1] ;
		pIDs[0] = nStartID;	
		
		nResult = ReqValues(nDiagType, nElements, pIDs, NULL);
		
		delete []pIDs;
		return nResult;
	}
	else if (nElements > 0 && nStartID >= 0)
	{
		int*	pIDs = new int[nElements];
		
		for (int i = 0; i < nElements; i++)
		{
			pIDs[i] = nStartID + i;
		}
		
		nResult = ReqValuesDiag(pIDs, nElements, nDiagType);
		delete []pIDs;
	}
	
	return nResult;
	
}


int		CtmTaskDsp28_M3::WriteValuesDiag(int	pIDs[], int pValues[], int nElements, int nDiagType)
{
	int	nType	= nDiagType | 0x8000;
	int	nResult	= 0;
	if (nElements > 0)
	{
		nResult		= ReqValues(nType, nElements, pIDs, pValues);
	}
	return nResult;
}
		
		
int		CtmTaskDsp28_M3::WriteValuesDiag(int nStartID, int pValues[], int nElements, int nDiagType)
{
	int	nResult	= 0;
	if(nDiagType == CONST_REQ_SIEMWRITE_DIAG)//西門子診斷
	{
		int  *pIDs = new int[1] ;
		pIDs[0] = nStartID;	
		
		nResult = ReqValues(nDiagType, nElements, pIDs, pValues);
		
		delete []pIDs;
		
		return nResult;
	}
	else if (nElements > 0 && nStartID >= 0)
	{
		int*	pIDs = new int[nElements];
		
		for (int i = 0; i < nElements; i++)
		{
			pIDs[i] = nStartID + i;
		}
		
		WriteValuesDiag(pIDs, pValues, nElements, nDiagType);
		delete []pIDs;
	}
	
	return nResult;
}

long CtmTaskDsp28_M3::SetTimeToJ6()/*zhongjw 2016-7-22 面板系統時間給J6卡*/
{
	//char sz[256];
	long year = 0;
	long Time = 0; 
	tmDATE    date;
	tmTIME    time;
	GetTime(&time);  		
  GetDate(&date);
  year = date.year - 1980;
  //date.year, date.month, date.day, time.hour, time.minute second
  //sprintf(sz, "%04u.%02u.%02u/%02u:%02u:%02u\n", date.year, date.month, date.day, time.hour, time.minute,time.second);
  //cout << "Time = " << sz  << endl;
  Time = (year  << 25) | (date.month << 21) | (date.day << 16) | (time.hour  << 11)
         | (time.minute << 5) | (time.second >> 1);
  return Time;
}

#ifndef	D_BEOBJECT_CTMTASKDSP28_M3
CObject*	CreateInstance(char* Name)
{
	//printf("Name=%s \n", Name);
	if (g_ptaskdsp28_M3 == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CTaskHost*>(pResult))->CreateSelf();
		g_ptaskdsp28_M3 = static_cast<CTaskHost*>(pResult);
	}
	return g_ptaskdsp28_M3;
	
	
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskHost*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
