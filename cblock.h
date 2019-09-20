/*===========================================================================+
|  Class    : Block	                                                         |
|  Task     : Database Analysis for Communication                            |
|----------------------------------------------------------------------------|
|  Compile  : GCC V3.3.2 -                                                   |
|  Link     : GCC V3.3.2 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Yang		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 12/08/2006                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef		D_BLOCK
#define		D_BLOCK
#include	"common.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
//********************
//				BLOCK ID
//********************
#define		BLOCK_MACHINE_ID			1
#define		BLOCK_MACHINE_MACH_ID		100001
#define		BLOCK_MOLDSET_ID			2
#define		BLOCK_POWERMATCH_ID     	3
#define		BLOCK_CONFIG_ID         	4
#define		BLOCK_CONFIGLIMIT_ID        158		//AD & DA¤W­­­È
#define		BLOCK_STATE_ID          	5
#define		BLOCK_STATUS_ID         	6
#define		BLOCK_METER_ID          	7
#define		BLOCK_TEMP_ID           	8
#define		BLOCK_DAOUT_ID          	9
#define		BLOCK_MONIMAXMIN_ID     	10
#define		BLOCK_MACHINEB_ID       	11
#define		BLOCK_MACHINEB_MACH_ID      100011
#define		BLOCK_MOLDSETB_ID       	12
#define		BLOCK_MONIMAXMINB_ID    	13
#define		BLOCK_ZSET_ID           	14
#define		BLOCK_MONICURRENT_ID        15
#define		BLOCK_MONILAST_ID           16
#define		BLOCK_MONIBCURRENT_ID		17
#define		BLOCK_MONIBLAST_ID          18
#define		BLOCK_MOLDSETX_ID			19
#define		BLOCK_DAOUT2_ID				20
#define		BLOCK_POWERMATCH2_ID     	21
#define		BLOCK_NEWDAOUT_ID			22
#define		BLOCK_NEWPOWERMATCH_ID     	23
#define		BLOCK_NEWPOWERMATCH_MACH_ID 100023
#define		BLOCK_OPERATION_ID			24
#define		BLOCK_MMICONFIG_ID			26
#define		BLOCK_MOLDHDR_ID			27
#define		BLOCK_SYSTEMX_ID			50
#define		BLOCK_MONIUP_ID				57
#define		BLOCK_MONIDN_ID				58
#define		BLOCK_MONITOR_ID			64
#define		BLOCK_MONIBUP_ID			75
#define		BLOCK_MONIBDN_ID			76
#define		BLOCK_USERM_ID				80
#define		BLOCK_USER1_ID				56
#define		BLOCK_USER2_ID				68
#define		BLOCK_USER3_ID				69
#define		BLOCK_USER4_ID				70
#define		BLOCK_USER5_ID				71
#define		BLOCK_USER6_ID				72
#define		BLOCK_USER7_ID				109
#define		BLOCK_USER8_ID				101
#define		BLOCK_USER9_ID				110
#define		BLOCK_USER10_ID				111

#define		BLOCK_DB_PLC_INJECT_SET			308
#define		BLOCK_DB_PLC_DOSING_SET			310
#define		BLOCK_DB_PLC_OTHER_SET			314
#define		BLOCK_DB_PLC_MOLD_SET			304
#define		BLOCK_DB_PLC_TEMPER				315
#define		BLOCK_DB_PLC_AXIS1_SET			289
#define		BLOCK_DB_PLC_AXIS2_SET			292

#define		BLOCK_FREECORE_ID			343
#define		BLOCK_NEWMACHINE2_ID		421
#define		BLOCK_NEWMOLDSET2_ID		422
#define		BLOCK_NEWMACHINE3_ID		441
#define		BLOCK_NEWMOLDSET3_ID		442
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagBLOCKITEM
    {
	WORD			wIndex;																		
	DWORD			dwOffset;																		
	}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	BLOCKITEM;
#else 
#ifdef	D_X86
BLOCKITEM;
#endif
#endif

typedef struct tagBLOCKHEADER
    {
    DWORD			wTotal;
    BLOCKITEM		pBlockAttr[];
    } 
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	BLOCKHEADER;
#else 
#ifdef	D_X86
BLOCKHEADER;
#endif
#endif

typedef struct tagBLOCKDATA
    {
    DWORD			dwFactorTotal;
    WORD			pwDBIndex[]; 
    } 
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	BLOCKDATA;
#else 
#ifdef	D_X86
BLOCKDATA;
#endif
#endif

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - CtmBlock                                     |
+===========================================================================*/
class	CtmBlock
{
public:
	CtmBlock();
	~CtmBlock();

private:
	char*			m_BlockBuff;
    BLOCKHEADER*	m_pBlockHeader;
    
public:
	BOOL			Create(char* pszFileName);
	WORD			SetBlock(BYTE* pcData, WORD	wBlockID);
    WORD			GetBlock(BYTE* pcData, WORD	wBlockID, BOOL bSave=TRUE);
    WORD			GetBlockCount(WORD	wBlockID);
    BOOL			SetBlockValue(BYTE* pcData, WORD	wBlockID, char* pszID, DWORD   wValue);
    BOOL			GetBlockValue(BYTE* pcData, WORD	wBlockID, char* pszID, DWORD* pdwValue);
    DWORD			GetBlockSize(WORD wBlockID);
    BLOCKDATA*		GetBlockData(WORD wBlockID);
    WORD			SetDefBlock(BYTE* pcData, WORD	wBlockID);
    long			SearchIndexFromBlock(WORD	wBlockID, DWORD	dwIndex);
    WORD			GetDataID(WORD	wBlockID, WORD	wOffsetID);
private:
		
};

#endif
