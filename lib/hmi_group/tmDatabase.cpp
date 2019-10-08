#include	"../../package.h"
#include	"../../main.h"
#include	"tmDatabase.h"

IMPLEMENT_DYNCREATE(CtmDatabase, CDatabase)
typedef void *(* PRSRUN)(void *);

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		LIMIT_TYPE_UNKOWN		0
#define		LIMIT_TYPE_CONST		1
#define		LIMIT_TYPE_INDEX		2
#define		LIMIT_TYPE_MULTIPLY		3
#define		LIMIT_TYPE_PLUS			4
#define		LIMIT_TYPE_SUBTRACT		5
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagLIMITMAP
{
	WORD		wIndex;
	WORD		wType;
	DWORD		dwID1;
	DWORD		dwID2;
	long long	llValue;
	//void	(*npFunc)();
} LIMITMAP;

LIMITMAP	g_LimitMap[] = {
	{INDEX_LLMT_XX_XXX_X_XX0		   ,	LIMIT_TYPE_CONST	,			 0,			0,	0	},
	{INDEX_LLMT_XX_XXX_X_XX1           ,	LIMIT_TYPE_CONST	,			 0,			0,	1	},
	{INDEX_LLMT_XX_XXX_X_XX64          ,	LIMIT_TYPE_CONST	,			 0,			0,	64 	},
	{INDEX_LLMT_XX_XXX_X_XX91          ,	LIMIT_TYPE_CONST	,			 0,			0,	91 	},
	{INDEX_LLMT_XX_XXX_X_XX_N          ,	LIMIT_TYPE_CONST	,			 0,			0,	0xffff},
	{INDEX_LLMT_ES_INJ_N_PS0           ,	LIMIT_TYPE_INDEX	,			 0,			0,	50110001},
	{INDEX_LLMT_ES_INJ_N_PS1           ,	LIMIT_TYPE_INDEX	,			 0,			0,	50100001},
	{INDEX_LLMT_ES_RCV_N_PS0           ,	LIMIT_TYPE_INDEX	,			 0,			0,	30110001},
	{INDEX_LLMT_ES_RCV_N_PS1           ,	LIMIT_TYPE_INDEX	,			 0,			0,	30100001},
	{INDEX_LLMT_ES_EJT_N_PS0           ,	LIMIT_TYPE_INDEX	,			 0,			0,	70210002},
	{INDEX_LLMT_ES_IJU_N_PS0           ,	LIMIT_TYPE_INDEX	,			 0,			0,	13000001},
	{INDEX_LLMT_ES_DCP_N_PS0           ,	LIMIT_TYPE_INDEX	,			 0,			0,	23000001},
	{INDEX_LLMT_ES_MDH_N_PS0           ,	LIMIT_TYPE_INDEX	,			 0,			0,	83000001},
#ifdef	D_X86
	{INDEX_LLMT_XX_XXX_X_XXM1E12       ,	LIMIT_TYPE_CONST	,			 0,			0,	-1000000000000LL},		//Mario debug20141128
#else
#ifdef	D_ARM
	{INDEX_LLMT_XX_XXX_X_XXM1E12       ,	LIMIT_TYPE_CONST	,			 0,			0,	-100000000},
#endif
#endif
	{INDEX_ULMT_XX_XXX_X_XX0           ,	LIMIT_TYPE_CONST	,			 0,			0,	0},
	{INDEX_ULMT_XX_XXX_X_XX1           ,	LIMIT_TYPE_CONST	,			 0,			0,	1},
	{INDEX_ULMT_XX_XXX_X_XX2           ,	LIMIT_TYPE_CONST	,			 0,			0,	2},
	{INDEX_ULMT_XX_XXX_X_XX3           ,	LIMIT_TYPE_CONST	,			 0,			0,	3},
	{INDEX_ULMT_XX_XXX_X_XX4           ,	LIMIT_TYPE_CONST	,			 0,			0,	4},
	{INDEX_ULMT_XX_XXX_X_XX10          ,	LIMIT_TYPE_CONST	,			 0,			0,	10},
	{INDEX_ULMT_XX_XXX_X_XX15          ,	LIMIT_TYPE_CONST	,			 0,			0,	15},
	{INDEX_ULMT_XX_XXX_X_XX16          ,	LIMIT_TYPE_CONST	,			 0,			0,	16},
	{INDEX_ULMT_XX_XXX_X_XX31          ,	LIMIT_TYPE_CONST	,			 0,			0,	31},
	{INDEX_ULMT_XX_XXX_X_XX100         ,	LIMIT_TYPE_CONST	,			 0,			0,	100},
	{INDEX_ULMT_XX_XXX_X_XX150         ,	LIMIT_TYPE_CONST	,			 0,			0,	150},
	{INDEX_ULMT_XX_XXX_X_XX173         ,	LIMIT_TYPE_CONST	,			 0,			0,	173},
	{INDEX_ULMT_XX_XXX_X_XX1000        ,	LIMIT_TYPE_CONST	,			 0,			0,	1000},
	{INDEX_ULMT_XX_XXX_X_XX_N          ,	LIMIT_TYPE_CONST	,			 0,			0,	0xffff},
	{INDEX_ULMT_XX_XXX_X_XX65535       ,	LIMIT_TYPE_CONST	,			 0,			0,	65535},
	{INDEX_ULMT_XX_XXX_X_XX8388608     ,	LIMIT_TYPE_CONST	,			 0,			0,	8388608},
	{INDEX_ULMT_XX_XXX_X_XX4294967295  ,	LIMIT_TYPE_CONST	,			 0,			0,	4294967295},
#ifdef	D_X86
	{INDEX_ULMT_XX_XXX_X_XX1E12        ,	LIMIT_TYPE_CONST	,			 0,			0,	1000000000000LL},	//Mario debug20141128
#else
#ifdef	D_ARM
	{INDEX_ULMT_XX_XXX_X_XX1E12        ,	LIMIT_TYPE_CONST	,			 0,			0,	100000000},
#endif
#endif
	{INDEX_ULMT_XX_XXX_X_XX400         ,	LIMIT_TYPE_CONST	,			 0,			0,	400},
	{INDEX_ULMT_XX_XXX_X_XX1000000     ,	LIMIT_TYPE_CONST	,			 0,			0,	1000000},
	{INDEX_ULMT_XX_XXX_X_XX10000       ,	LIMIT_TYPE_CONST	,			 0,			0,	10000},
	{INDEX_ULMT_XX_XXX_X_XX100000      ,	LIMIT_TYPE_CONST	,			 0,			0,	100000},
	{INDEX_ULMT_ES_MLD_J_VL0           ,	LIMIT_TYPE_INDEX	,	63000002,	60000005,		0},
	{INDEX_ULMT_ES_MLD_N_PS0           ,	LIMIT_TYPE_INDEX	,	60000016,			0,		0},
	{INDEX_ULMT_ES_MLD_N_VL0           ,	LIMIT_TYPE_INDEX	,	63000001,	60000002,		0},
	{INDEX_ULMT_ES_MLD_N_AC0           ,	LIMIT_TYPE_INDEX	,	63000003,	60000006,		0},
	{INDEX_ULMT_ES_MLD_N_DC0           ,	LIMIT_TYPE_INDEX	,	63000004,	60000007,		0},
	{INDEX_ULMT_ES_MLD_N_JA0           ,	LIMIT_TYPE_INDEX	,	63000005,	60000008,		0},
	{INDEX_ULMT_ES_MLD_N_JD0           ,	LIMIT_TYPE_INDEX	,	63000006,	60000009,		0},
	{INDEX_ULMT_ES_MLD_N_DF0           ,	LIMIT_TYPE_INDEX	,	63000007,	60000003,		0},
	{INDEX_ULMT_ES_MLD_N_CF0           ,	LIMIT_TYPE_INDEX	,	63000008,	60000004,		0},
	{INDEX_ULMT_ES_MLD_N_JA1           ,	LIMIT_TYPE_INDEX	,	66000012,			0,		0},
	{INDEX_ULMT_ES_MLD_N_VL1           ,	LIMIT_TYPE_INDEX	,	66000013,			0,		0},
	{INDEX_ULMT_ES_MLD_N_AC1           ,	LIMIT_TYPE_INDEX	,	66000009,			0,		0},
	{INDEX_ULMT_ES_MLD_N_GF0           ,	LIMIT_TYPE_INDEX	,	63000011,			0,		0},
	{INDEX_ULMT_ES_MLD_N_PS1           ,	LIMIT_TYPE_INDEX	,	63000010,			0,		0},
	{INDEX_ULMT_ES_MLD_N_PS2           ,	LIMIT_TYPE_INDEX	,	66010007,			0,		0},
	{INDEX_ULMT_ES_MLD_N_PS3           ,	LIMIT_TYPE_INDEX	,	60110004,			0,		0},
	{INDEX_ULMT_ES_MLD_N_VL2           ,	LIMIT_TYPE_INDEX	,	60110001,			0,		0},
	{INDEX_ULMT_ES_MLD_N_VL3           ,	LIMIT_TYPE_INDEX	,	60120001,			0,		0},
	{INDEX_ULMT_ES_INJ_N_PS0           ,	LIMIT_TYPE_INDEX	,	50110002,			0,		0},
	{INDEX_ULMT_ES_INJ_N_PS1           ,	LIMIT_TYPE_INDEX	,	50100002,			0,		0},
	{INDEX_ULMT_ES_INJ_N_PR2           ,	LIMIT_TYPE_INDEX	,	50120001,			0,		0},
	{INDEX_ULMT_ES_INJ_N_PR1           ,	LIMIT_TYPE_INDEX	,	50110003,			0,		0},
	{INDEX_ULMT_ES_MLD_N_DC1           ,	LIMIT_TYPE_INDEX	,	50100005,			0,		0},
	{INDEX_ULMT_ES_MLD_N_AC2           ,	LIMIT_TYPE_INDEX	,	50100004,			0,		0},
	{INDEX_ULMT_ES_INJ_J_VL0           ,	LIMIT_TYPE_INDEX	,	53000002,	50000006,		0},
	{INDEX_ULMT_ES_INJ_J_AC0           ,	LIMIT_TYPE_INDEX	,	53000003,	50000007,		0},
	{INDEX_ULMT_ES_INJ_J_DC0           ,	LIMIT_TYPE_INDEX	,	53000004,	50000008,		0},
	{INDEX_ULMT_ES_INJ_J_DF0           ,	LIMIT_TYPE_INDEX	,	53000007,	50000011,		0},
	{INDEX_ULMT_ES_INJ_N_VL1           ,	LIMIT_TYPE_INDEX	,	53000001,	50000005,		0},
	{INDEX_ULMT_ES_INJ_N_JA0           ,	LIMIT_TYPE_INDEX	,	53000005,	50000009,		0},
	{INDEX_ULMT_ES_INJ_N_JD0           ,	LIMIT_TYPE_INDEX	,	53000006,	50000010,		0},
	{INDEX_ULMT_ES_INJ_N_PR0           ,	LIMIT_TYPE_INDEX	,	53000008,	50000012,		0},
	{INDEX_ULMT_ES_INJ_N_PG0           ,	LIMIT_TYPE_INDEX	,	53000009,	50000013,		0},
	{INDEX_ULMT_ES_INJ_N_VL2           ,	LIMIT_TYPE_INDEX	,	56000013,			0,		0},
	{INDEX_ULMT_ES_INJ_N_JA1           ,	LIMIT_TYPE_INDEX	,	56000012,			0,		0},
	{INDEX_ULMT_ES_INJ_N_AC2           ,	LIMIT_TYPE_INDEX	,	56000009,			0,		0},
	{INDEX_ULMT_ES_INJ_N_PS2           ,	LIMIT_TYPE_INDEX	,	53000011,			0,		0},
	{INDEX_ULMT_ES_INJ_N_VL3           ,	LIMIT_TYPE_INDEX	,	50100003,			0,		0},
	{INDEX_ULMT_ES_RCV_N_PS0           ,	LIMIT_TYPE_INDEX	,	30110002,			0,		0},
	{INDEX_ULMT_ES_RCV_N_PS1           ,	LIMIT_TYPE_INDEX	,	30100002,			0,		0},
	{INDEX_ULMT_ES_RCV_N_SP0           ,	LIMIT_TYPE_INDEX	,	33000001,	30000006,		0},
	{INDEX_ULMT_ES_RCV_J_SP1           ,	LIMIT_TYPE_INDEX	,	33000002,	30000007,		0},
	{INDEX_ULMT_ES_RCV_N_TQ0           ,	LIMIT_TYPE_INDEX	,	33000003,	30000008,		0},
	{INDEX_ULMT_ES_RCV_N_AC0           ,	LIMIT_TYPE_INDEX	,	33000004,	30000009,		0},
	{INDEX_ULMT_ES_RCV_N_DC0           ,	LIMIT_TYPE_INDEX	,	33000005,	30000010,		0},
	{INDEX_ULMT_ES_RCV_N_JA0           ,	LIMIT_TYPE_INDEX	,	33000006,	30000011,		0},
	{INDEX_ULMT_ES_RCV_N_JD0           ,	LIMIT_TYPE_INDEX	,	33000007,	30000012,		0},
	{INDEX_ULMT_ES_RCV_N_PR0           ,	LIMIT_TYPE_INDEX	,	33000008,	30000013,		0},
	{INDEX_ULMT_ES_RCV_N_PG0           ,	LIMIT_TYPE_INDEX	,	33000009,	30000014,		0},
	{INDEX_ULMT_ES_RCV_N_VL0           ,	LIMIT_TYPE_INDEX	,	36000013,			0,		0},
	{INDEX_ULMT_ES_RCV_N_JA1           ,	LIMIT_TYPE_INDEX	,	36000012,			0,		0},
	{INDEX_ULMT_ES_RCV_N_AC1           ,	LIMIT_TYPE_INDEX	,	36000009,			0,		0},
	{INDEX_ULMT_ES_RCV_N_DC1           ,	LIMIT_TYPE_INDEX	,	33000012,			0,		0},
	{INDEX_ULMT_ES_RCV_N_GF0           ,	LIMIT_TYPE_INDEX	,	33000011,			0,		0},
	{INDEX_ULMT_ES_RCV_N_VL1           ,	LIMIT_TYPE_INDEX	,	33000010,			0,		0},
	{INDEX_ULMT_ES_RCV_N_PG1           ,	LIMIT_TYPE_INDEX	,	30110005,			0,		0},
	{INDEX_ULMT_ES_RCV_N_PG2           ,	LIMIT_TYPE_INDEX	,	30110004,			0,		0},
	{INDEX_ULMT_ES_RCV_N_PR1           ,	LIMIT_TYPE_INDEX	,	30110003,			0,		0},
	{INDEX_ULMT_ES_RCV_N_SP2           ,	LIMIT_TYPE_INDEX	,	30100003,			0,		0},
	{INDEX_ULMT_ES_EJT_N_PS0           ,	LIMIT_TYPE_INDEX	,	70200001,			0,		0},
	{INDEX_ULMT_ES_EJT_N_PS1           ,	LIMIT_TYPE_INDEX	,	70210001,			0,		0},
	{INDEX_ULMT_ES_EJT_J_VL0           ,	LIMIT_TYPE_INDEX	,	73000002,	70000004,		0},
	{INDEX_ULMT_ES_EJT_N_VL1           ,	LIMIT_TYPE_INDEX	,	73000001,	70000002,		0},
	{INDEX_ULMT_ES_EJT_N_AC0           ,	LIMIT_TYPE_INDEX	,	73000003,	70000005,		0},
	{INDEX_ULMT_ES_EJT_N_DC0           ,	LIMIT_TYPE_INDEX	,	73000004,	70000006,		0},
	{INDEX_ULMT_ES_EJT_N_JA0           ,	LIMIT_TYPE_INDEX	,	73000005,	70000007,		0},
	{INDEX_ULMT_ES_EJT_N_JD0           ,	LIMIT_TYPE_INDEX	,	73000006,	70000008,		0},
	{INDEX_ULMT_ES_EJT_N_DF0           ,	LIMIT_TYPE_INDEX	,	73000007,	70000003,		0},
	{INDEX_ULMT_ES_EJT_N_AC1           ,	LIMIT_TYPE_INDEX	,	76000009,			0,		0},
	{INDEX_ULMT_ES_EJT_N_JA1           ,	LIMIT_TYPE_INDEX	,	76000012,			0,		0},
	{INDEX_ULMT_ES_EJT_N_VL2           ,	LIMIT_TYPE_INDEX	,	76000013,			0,		0},
	{INDEX_ULMT_ES_EJT_N_PS3           ,	LIMIT_TYPE_INDEX	,	73000008,			0,		0},
	{INDEX_ULMT_ES_EJT_N_VL3           ,	LIMIT_TYPE_INDEX	,	70200002,			0,		0},
	{INDEX_ULMT_ES_EJT_N_VL4           ,	LIMIT_TYPE_INDEX	,	70210002,			0,		0},
	{INDEX_ULMT_ES_IJU_N_PS0           ,	LIMIT_TYPE_INDEX	,	13000002,			0,		0},
	{INDEX_ULMT_ES_IJU_N_VL0           ,	LIMIT_TYPE_INDEX	,	13000003,	10000004,		0},
	{INDEX_ULMT_ES_IJU_J_VL1           ,	LIMIT_TYPE_INDEX	,	13000003,	10000007,		0},
	{INDEX_ULMT_ES_IJU_N_PS1           ,	LIMIT_TYPE_SUBTRACT	,	13000002,	13000001,		0},
	{INDEX_ULMT_ES_IJU_N_AC0           ,	LIMIT_TYPE_INDEX	,	13000004,	10000005,		0},
	{INDEX_ULMT_ES_IJU_N_DC0           ,	LIMIT_TYPE_INDEX	,	13000005,	10000006,		0},
	{INDEX_ULMT_ES_IJU_N_JA0           ,	LIMIT_TYPE_INDEX	,	13000006,	10000008,		0},
	{INDEX_ULMT_ES_IJU_N_JD0           ,	LIMIT_TYPE_INDEX	,	13000007,	10000009,		0},
	{INDEX_ULMT_ES_IJU_N_DF0           ,	LIMIT_TYPE_INDEX	,	13000008,	10000010,		0},
	{INDEX_ULMT_ES_IJU_N_DF1           ,	LIMIT_TYPE_INDEX	,	10010013,			0,		0},
	{INDEX_ULMT_ES_IJU_N_VL2           ,	LIMIT_TYPE_INDEX	,	16000013,			0,		0},
	{INDEX_ULMT_ES_IJU_N_AC1           ,	LIMIT_TYPE_INDEX	,	16000009,			0,		0},
	{INDEX_ULMT_ES_IJU_N_JA1           ,	LIMIT_TYPE_INDEX	,	16000012,			0,		0},
	{INDEX_ULMT_ES_DCP_N_PS0           ,	LIMIT_TYPE_INDEX	,	23000002,			0,		0},
	{INDEX_ULMT_ES_DCP_N_VL0           ,	LIMIT_TYPE_INDEX	,	23000003,	20000004,		0},
	{INDEX_ULMT_ES_DCP_N_AC0           ,	LIMIT_TYPE_INDEX	,	23000004,	20000005,		0},
	{INDEX_ULMT_ES_DCP_N_DC0           ,	LIMIT_TYPE_INDEX	,	23000005,	20000006,		0},
	{INDEX_ULMT_ES_DCP_N_JA0           ,	LIMIT_TYPE_INDEX	,	23000006,	20000007,		0},
	{INDEX_ULMT_ES_DCP_N_JD0           ,	LIMIT_TYPE_INDEX	,	23000007,	20000008,		0},
	{INDEX_ULMT_ES_DCP_N_DF0           ,	LIMIT_TYPE_INDEX	,	23000008,	20000009,		0},
	{INDEX_ULMT_ES_MDH_J_VL0           ,	LIMIT_TYPE_INDEX	,	83000003,	80000008,		0},
	{INDEX_ULMT_ES_MDH_J_AC0           ,	LIMIT_TYPE_INDEX	,	83000004,	80000009,		0},
	{INDEX_ULMT_ES_MDH_J_DC0           ,	LIMIT_TYPE_INDEX	,	83000005,	80000010,		0},
	{INDEX_ULMT_ES_MDH_J_JA0           ,	LIMIT_TYPE_INDEX	,	83000006,	80000011,		0},
	{INDEX_ULMT_ES_MDH_J_JD0           ,	LIMIT_TYPE_INDEX	,	83000007,	80000012,		0},
	{INDEX_ULMT_ES_MDH_F_DF0           ,	LIMIT_TYPE_INDEX	,	83000008,	80000013,		0},
	{INDEX_ULMT_ES_MDH_N_PS0           ,	LIMIT_TYPE_INDEX	,	83000002,		   0,		0},
	{INDEX_ULMT_ES_MDH_N_VL1           ,	LIMIT_TYPE_INDEX	,	83000003,	80000002,		0},
	{INDEX_ULMT_ES_MDH_N_AC1           ,	LIMIT_TYPE_INDEX	,	83000004,	80000003,		0},
	{INDEX_ULMT_ES_MDH_N_DC1           ,	LIMIT_TYPE_INDEX	,	83000005,	80000004,		0},
	{INDEX_ULMT_ES_MDH_N_JA1           ,	LIMIT_TYPE_INDEX	,	83000006,	80000005,		0},
	{INDEX_ULMT_ES_MDH_N_JD1           ,	LIMIT_TYPE_INDEX	,	83000007,	80000006,		0},
	{INDEX_ULMT_ES_MDH_N_DF1           ,	LIMIT_TYPE_INDEX	,	83000008,	80000007,		0},
	{INDEX_ULMT_ES_MDH_N_VL2           ,	LIMIT_TYPE_INDEX	,	86000013,			0,		0},
	{INDEX_ULMT_ES_MDH_N_JA2           ,	LIMIT_TYPE_INDEX	,	86000012,			0,		0},
	{INDEX_ULMT_ES_MDH_N_AC2           ,	LIMIT_TYPE_INDEX	,	86000009,			0,		0},
	{INDEX_ULMT_XX_XXX_X_XX24			,	LIMIT_TYPE_CONST	,		   0,			0,	   23},
	{INDEX_ULMT_XX_XXX_X_XX60			,	LIMIT_TYPE_CONST	,		   0,			0,	   59},
	{INDEX_ULMT_ES_INJ_N_AC0			,	LIMIT_TYPE_INDEX	,	53000003,	 50000007,		0},
	{INDEX_ULMT_ES_INJ_N_DC0			,	LIMIT_TYPE_INDEX	,	53000004,	 50000008,		0},
	{INDEX_ULMT_ES_MLD_N_PS4			,	LIMIT_TYPE_INDEX	,	60010006,			0,		0},
	{INDEX_LLMT_XX_XXX_X_XX2			,	LIMIT_TYPE_CONST	,		   0,			0,		2},
	//>>>zholy061220
	{INDEX_LLMT_XX_XXX_X_XX3     		,	LIMIT_TYPE_CONST	,			0,			0,	    3},
	{INDEX_ULMT_XX_XXX_X_XX5	 		,	LIMIT_TYPE_CONST	,			0,			0,	    5},
	{INDEX_ULMT_XX_XXX_X_XX6	 		,	LIMIT_TYPE_CONST	,			0,			0,	    6},
	{INDEX_ULMT_XX_XXX_X_XX12    		,	LIMIT_TYPE_CONST	,			0,			0,	   12},
	{INDEX_ULMT_XX_XXX_X_XX59    		,	LIMIT_TYPE_CONST	,			0,			0,	   59},
	{INDEX_ULMT_XX_XXX_X_XX65    		,	LIMIT_TYPE_CONST	,			0,			0,	   65},
	{INDEX_ULMT_XX_XXX_X_XX60000 		,	LIMIT_TYPE_CONST	,			0,			0,  60000},
	{INDEX_ULMT_XX_XXX_X_XX18000 		,	LIMIT_TYPE_CONST	,			0,			0,	18000},
	{INDEX_ULMT_XX_XXX_X_XX23	 		,	LIMIT_TYPE_CONST	,			0,			0,	   23},
	{INDEX_ULMT_XX_XXX_X_XX2099  		,	LIMIT_TYPE_CONST	,			0,			0,	 2099},
	{INDEX_ULMT_XX_XXX_X_XX300   		,	LIMIT_TYPE_CONST	,			0,			0,	  300},
	{INDEX_ULMT_XX_XXX_X_XX30000  		,	LIMIT_TYPE_CONST	,			0,			0,	30000},
	{INDEX_ULMT_XX_XXX_X_XX32000  		,	LIMIT_TYPE_CONST	,			0,			0,	 3200},
	{INDEX_ULMT_XX_XXX_X_XX4095  		,	LIMIT_TYPE_CONST	,			0,			0,	 4095},
	{INDEX_ULMT_XX_XXX_X_XX9	 		,	LIMIT_TYPE_CONST	,			0,			0,	    9},
	{INDEX_ULMT_XX_XXX_X_XX99    		,	LIMIT_TYPE_CONST	,			0,			0,	   99},
	{INDEX_ULMT_XX_XXX_X_XX999   		,	LIMIT_TYPE_CONST	,			0,			0,	  999},
	{INDEX_ULMT_XX_XXX_X_XX9999  		,	LIMIT_TYPE_CONST	,			0,			0,	 9999},
	{INDEX_ULMT_XX_XXX_X_XX99999 		,	LIMIT_TYPE_CONST	,			0,			0,	99999},
	{INDEX_ULMT_XX_XXX_X_XX999999		,	LIMIT_TYPE_CONST	,			0,			0, 999999},
	{INDEX_ULMT_XX_XXX_X_XX9999999		,	LIMIT_TYPE_CONST	,			0,			0,9999999},
	{INDEX_ULMTV_DP_CMC_N_FL0			,	LIMIT_TYPE_INDEX	,	  130001,			0,		0},
	{INDEX_ULMTV_DP_CMC_N_PR0			,	LIMIT_TYPE_INDEX	,	  130002,			0,		0},
	{INDEX_ULMTV_DP_CMC_N_PS0			,	LIMIT_TYPE_INDEX	,	  130003,			0,		0},
	{INDEX_ULMTV_DP_CMO_N_FL0			,	LIMIT_TYPE_INDEX	,	  130004,			0,		0},
	{INDEX_ULMTV_DP_CMO_N_PR0			,	LIMIT_TYPE_INDEX	,	  130005,			0,		0},
	{INDEX_ULMTV_DP_CMO_N_PS0			,	LIMIT_TYPE_INDEX	,	  130006,			0,		0},
	{INDEX_ULMTV_DP_COR_N_FL0			,	LIMIT_TYPE_INDEX	,	  130007,			0,		0},
	{INDEX_ULMTV_DP_COR_N_PR0			,	LIMIT_TYPE_INDEX	,	  130008,			0,		0},
	{INDEX_ULMTV_DP_COR_N_PS0			,	LIMIT_TYPE_INDEX	,	  130009,			0,		0},
	{INDEX_ULMTV_DP_DCP_N_FL0			,	LIMIT_TYPE_INDEX	,	  130010,			0,		0},
	{INDEX_ULMTV_DP_DCP_N_PR0			,	LIMIT_TYPE_INDEX	,	  130011,			0,		0},
	{INDEX_ULMTV_DP_DCP_N_PS0			,	LIMIT_TYPE_INDEX	,	  130012,			0,		0},
	{INDEX_ULMTV_DP_DCP2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130013,			0,		0},
	{INDEX_ULMTV_DP_DCP2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130014,			0,		0},
	{INDEX_ULMTV_DP_DCP2_N_PS0			,	LIMIT_TYPE_INDEX	,	  130015,			0,		0},
	{INDEX_ULMTV_DP_DOR_N_FL0			,	LIMIT_TYPE_INDEX	,	  130016,			0,		0},
	{INDEX_ULMTV_DP_DOR_N_PR0			,	LIMIT_TYPE_INDEX	,	  130017,			0,		0},
	{INDEX_ULMTV_DP_EJB_N_FL0			,	LIMIT_TYPE_INDEX	,	  130018,			0,		0},
	{INDEX_ULMTV_DP_EJB_N_PR0			,	LIMIT_TYPE_INDEX	,	  130019,			0,		0},
	{INDEX_ULMTV_DP_EJB_N_PS0			,	LIMIT_TYPE_INDEX	,	  130020,			0,		0},
	{INDEX_ULMTV_DP_EJB2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130021,			0,		0},
	{INDEX_ULMTV_DP_EJB2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130022,			0,		0},
	{INDEX_ULMTV_DP_EJB2_N_PS0			,	LIMIT_TYPE_INDEX	,	  130023,			0,		0},
	{INDEX_ULMTV_DP_EJF_N_FL0			,	LIMIT_TYPE_INDEX	,	  130024,			0,		0},
	{INDEX_ULMTV_DP_EJF_N_PR0			,	LIMIT_TYPE_INDEX	,	  130025,			0,		0},
	{INDEX_ULMTV_DP_EJF_N_PS0			,	LIMIT_TYPE_INDEX	,	  130026,			0,		0},
	{INDEX_ULMTV_DP_EJF2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130027,			0,		0},
	{INDEX_ULMTV_DP_EJF2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130028,			0,		0},
	{INDEX_ULMTV_DP_EJF2_N_PS0			,	LIMIT_TYPE_INDEX	,	  130029,			0,		0},
	{INDEX_ULMTV_DP_EJT_N_FL0			,	LIMIT_TYPE_INDEX	,	  130030,			0,		0},
	{INDEX_ULMTV_DP_EJT_N_PR0			,	LIMIT_TYPE_INDEX	,	  130031,			0,		0},
	{INDEX_ULMTV_DP_EJT2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130032,			0,		0},
	{INDEX_ULMTV_DP_EJT2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130033,			0,		0},
	{INDEX_ULMTV_DP_HPR_N_FL0			,	LIMIT_TYPE_INDEX	,	  130034,			0,		0},
	{INDEX_ULMTV_DP_HPR_N_PR0			,	LIMIT_TYPE_INDEX	,	  130035,			0,		0},
	{INDEX_ULMTV_DP_HPR2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130036,			0,		0},
	{INDEX_ULMTV_DP_HPR2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130037,			0,		0},
	{INDEX_ULMTV_DP_IJU_N_FL0			,	LIMIT_TYPE_INDEX	,	  130038,			0,		0},
	{INDEX_ULMTV_DP_IJU_N_PR0			,	LIMIT_TYPE_INDEX	,	  130039,			0,		0},
	{INDEX_ULMTV_DP_IJU2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130040,			0,		0},
	{INDEX_ULMTV_DP_IJU2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130041,			0,		0},
	{INDEX_ULMTV_DP_INJ_N_FL0			,	LIMIT_TYPE_INDEX	,	  130042,			0,		0},
	{INDEX_ULMTV_DP_INJ_N_PR0			,	LIMIT_TYPE_INDEX	,	  130043,			0,		0},
	{INDEX_ULMTV_DP_INJ_N_PS0			,	LIMIT_TYPE_INDEX	,	  130044,			0,		0},
	{INDEX_ULMTV_DP_INJ2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130045,			0,		0},
	{INDEX_ULMTV_DP_INJ2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130046,			0,		0},
	{INDEX_ULMTV_DP_INJ2_N_PS0			,	LIMIT_TYPE_INDEX	,	  130047,			0,		0},
	{INDEX_ULMTV_DP_IUB_N_FL0			,	LIMIT_TYPE_INDEX	,	  130048,			0,		0},
	{INDEX_ULMTV_DP_IUB_N_PR0			,	LIMIT_TYPE_INDEX	,	  130049,			0,		0},
	{INDEX_ULMTV_DP_IUB_N_PS0			,	LIMIT_TYPE_INDEX	,	  130050,			0,		0},
	{INDEX_ULMTV_DP_IUB2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130051,			0,		0},
	{INDEX_ULMTV_DP_IUB2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130052,			0,		0},
	{INDEX_ULMTV_DP_IUB2_N_PS0			,	LIMIT_TYPE_INDEX	,	  130053,			0,		0},
	{INDEX_ULMTV_DP_IUF_N_FL0			,	LIMIT_TYPE_INDEX	,	  130054,			0,		0},
	{INDEX_ULMTV_DP_IUF_N_PR0			,	LIMIT_TYPE_INDEX	,	  130055,			0,		0},
	{INDEX_ULMTV_DP_IUF_N_PS0			,	LIMIT_TYPE_INDEX	,	  130056,			0,		0},
	{INDEX_ULMTV_DP_IUF2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130057,			0,		0},
	{INDEX_ULMTV_DP_IUF2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130058,			0,		0},
	{INDEX_ULMTV_DP_IUF2_N_PS0			,	LIMIT_TYPE_INDEX	,	  130059,			0,		0},
	{INDEX_ULMTV_DP_IUR_N_FL0			,	LIMIT_TYPE_INDEX	,	  130060,			0,		0},
	{INDEX_ULMTV_DP_IUR_N_PR0			,	LIMIT_TYPE_INDEX	,	  130061,			0,		0},
	{INDEX_ULMTV_DP_IUR2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130062,			0,		0},
	{INDEX_ULMTV_DP_IUR2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130063,			0,		0},
	{INDEX_ULMTV_DP_LOC_N_FL0			,	LIMIT_TYPE_INDEX	,	  130064,			0,		0},
	{INDEX_ULMTV_DP_LOC_N_PR0			,	LIMIT_TYPE_INDEX	,	  130065,			0,		0},
	{INDEX_ULMTV_DP_LOC2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130066,			0,		0},
	{INDEX_ULMTV_DP_LOC2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130067,			0,		0},
	{INDEX_ULMTV_DP_LOCB2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130068,			0,		0},
	{INDEX_ULMTV_DP_LOCB2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130069,			0,		0},
	{INDEX_ULMTV_DP_MDC_N_CF0			,	LIMIT_TYPE_INDEX	,	  130070,			0,		0},
	{INDEX_ULMTV_DP_MDC_N_FL0			,	LIMIT_TYPE_INDEX	,	  130071,			0,		0},
	{INDEX_ULMTV_DP_MDC_N_PR0			,	LIMIT_TYPE_INDEX	,	  130072,			0,		0},
	{INDEX_ULMTV_DP_MDH_N_FL0			,	LIMIT_TYPE_INDEX	,	  130073,			0,		0},
	{INDEX_ULMTV_DP_MDH_N_PR0			,	LIMIT_TYPE_INDEX	,	  130074,			0,		0},
	{INDEX_ULMTV_DP_MDO_N_FL0			,	LIMIT_TYPE_INDEX	,	  130075,			0,		0},
	{INDEX_ULMTV_DP_MDO_N_PR0			,	LIMIT_TYPE_INDEX	,	  130076,			0,		0},
	{INDEX_ULMTV_DP_MEJF_N_FL0			,	LIMIT_TYPE_INDEX	,	  130077,			0,		0},
	{INDEX_ULMTV_DP_MEJF_N_PR0			,	LIMIT_TYPE_INDEX	,	  130078,			0,		0},
	{INDEX_ULMTV_DP_MEJT_N_FL0			,	LIMIT_TYPE_INDEX	,	  130079,			0,		0},
	{INDEX_ULMTV_DP_MEJT_N_PR0			,	LIMIT_TYPE_INDEX	,	  130080,			0,		0},
	{INDEX_ULMTV_DP_MLD_N_PS0			,	LIMIT_TYPE_INDEX	,	  130081,			0,		0},
	{INDEX_ULMTV_DP_RCV_N_BPR0			,	LIMIT_TYPE_INDEX	,	  130082,			0,		0},
	{INDEX_ULMTV_DP_RCV_N_FL0			,	LIMIT_TYPE_INDEX	,	  130083,			0,		0},
	{INDEX_ULMTV_DP_RCV_N_PR0			,	LIMIT_TYPE_INDEX	,	  130084,			0,		0},
	{INDEX_ULMTV_DP_RCV_N_PS0			,	LIMIT_TYPE_INDEX	,	  130085,			0,		0},
	{INDEX_ULMTV_DP_RCV2_N_BPR0			,	LIMIT_TYPE_INDEX	,	  130086,			0,		0},
	{INDEX_ULMTV_DP_RCV2_N_FL0			,	LIMIT_TYPE_INDEX	,	  130087,			0,		0},
	{INDEX_ULMTV_DP_RCV2_N_PR0			,	LIMIT_TYPE_INDEX	,	  130088,			0,		0},
	{INDEX_ULMTV_DP_RCV2_N_PS0			,	LIMIT_TYPE_INDEX	,	  130089,			0,		0},
	{INDEX_ULMTV_DP_RDR_N_FL0			,	LIMIT_TYPE_INDEX	,	  130090,			0,		0},
	{INDEX_ULMTV_DP_RDR_N_PR0			,	LIMIT_TYPE_INDEX	,	  130091,			0,		0},
	{INDEX_ULMTV_DP_RMDB_N_FL0			,	LIMIT_TYPE_INDEX	,	  130092,			0,		0},
	{INDEX_ULMTV_DP_RMDB_N_PR0			,	LIMIT_TYPE_INDEX	,	  131001,			0,		0},
	{INDEX_ULMTV_DP_RMDB_N_PS0			,	LIMIT_TYPE_INDEX	,	  131002,			0,		0},
	{INDEX_ULMTV_DP_RMDF_N_FL0			,	LIMIT_TYPE_INDEX	,	  131003,			0,		0},
	{INDEX_ULMTV_DP_RMDF_N_PR0			,	LIMIT_TYPE_INDEX	,	  131004,			0,		0},
	{INDEX_ULMTV_DP_RMDF_N_PS0			,	LIMIT_TYPE_INDEX	,	  131005,			0,		0},
	{INDEX_ULMTV_DP_RMLD_N_FL0			,	LIMIT_TYPE_INDEX	,	  131006,			0,		0},
	{INDEX_ULMTV_DP_RMLD_N_PR0			,	LIMIT_TYPE_INDEX	,	  131007,			0,		0},
	{INDEX_ULMTV_DP_RMLD_N_PS0			,	LIMIT_TYPE_INDEX	,	  131008,			0,		0},
	{INDEX_ULMTV_DP_ROT_N_FL0			,	LIMIT_TYPE_INDEX	,	  131009,			0,		0},
	{INDEX_ULMTV_DP_ROT_N_PR0			,	LIMIT_TYPE_INDEX	,	  131010,			0,		0},
	{INDEX_ULMTV_DP_ROT_N_PS0			,	LIMIT_TYPE_INDEX	,	  131011,			0,		0},
	{INDEX_ULMTV_DP_SHT_N_FL0			,	LIMIT_TYPE_INDEX	,	  131012,			0,		0},
	{INDEX_ULMTV_DP_SHT_N_PR0			,	LIMIT_TYPE_INDEX	,	  131013,			0,		0},
	{INDEX_ULMTV_DP_SHT2_N_FL0			,	LIMIT_TYPE_INDEX	,	  131014,			0,		0},
	{INDEX_ULMTV_DP_SHT2_N_PR0			,	LIMIT_TYPE_INDEX	,	  131015,			0,		0},
	{INDEX_ULMTV_DP_SLB_N_FL0			,	LIMIT_TYPE_INDEX	,	  131016,			0,		0},
	{INDEX_ULMTV_DP_SLB_N_PR0			,	LIMIT_TYPE_INDEX	,	  131017,			0,		0},
	{INDEX_ULMTV_DP_SLF_N_FL0			,	LIMIT_TYPE_INDEX	,	  131018,			0,		0},
	{INDEX_ULMTV_DP_SLF_N_PR0			,	LIMIT_TYPE_INDEX	,	  131019,			0,		0},
	{INDEX_ULMTV_DP_UEJB_N_FL0			,	LIMIT_TYPE_INDEX	,	  131020,			0,		0},
	{INDEX_ULMTV_DP_UEJB_N_PR0			,	LIMIT_TYPE_INDEX	,	  131021,			0,		0},
	{INDEX_ULMTV_DP_UEJF_N_FL0			,	LIMIT_TYPE_INDEX	,	  131022,			0,		0},
	{INDEX_ULMTV_DP_UEJF_N_PR0			,	LIMIT_TYPE_INDEX	,	  132001,			0,		0},
	{INDEX_ULMTV_DP_TMP_N_DG0	 		,	LIMIT_TYPE_INDEX	,	  133001,			0,		0},
	{INDEX_ULMTV_DP_TMP2_N_DG0	 		,	LIMIT_TYPE_INDEX	,	  133002,			0,		0},
	{INDEX_ULMT_XX_XXX_X_XX250			,	LIMIT_TYPE_CONST	,	  		0,			0,	  250},
	//<<<zholy061220
	{INDEX_ULMT_XX_XXX_X_XX255			,	LIMIT_TYPE_CONST	,	  		0,			0,	  255},
	{INDEX_ULMT_XX_XXX_X_XX365			,	LIMIT_TYPE_CONST	,	  		0,			0,	  365},
	{INDEX_ULMT_XX_XXX_X_XX450			,	LIMIT_TYPE_CONST	,	  		0,			0,	  450},	//	fans 2007/6/21 03:46¤U¤È
	
		 // fans 2007/6/29 04:55¤U¤È
	 {INDEX_ULMT_ES_HLDPRS_N_AC0    	, LIMIT_TYPE_INDEX ,   		50120002,   		0,  	0},
	 {INDEX_ULMT_ES_HLDPRS_N_DC0    	, LIMIT_TYPE_INDEX ,   		50120003,   		0,  	0},
	 {INDEX_ULMT_ES_MLD_N_DF1			, LIMIT_TYPE_INDEX ,   		66000098,   		0,  	0},
	 //	fans 2007/8/9 03:50¤U¤È
	 {ULMT_XX_XXX_X_XX3000				,	LIMIT_TYPE_CONST	, 			0,			0,	 3000},
	 {ULMT_XX_XXX_X_XX594000			,	LIMIT_TYPE_CONST	, 			0,			0, 594000},
	 {ULMT_XX_XXX_X_XX140				,	LIMIT_TYPE_CONST	, 			0,			0, 	  140},
	 {ULMT_XX_XXX_X_XX15999				,   LIMIT_TYPE_CONST	,			0,			0, 15999},
	 {LLMT_XX_XXX_X_XX100				,	LIMIT_TYPE_CONST	,			0,			0,	 100},
	 {ULMT_XX_XXX_X_XX160				,	LIMIT_TYPE_CONST	,			0,			0,	 160},
	 {ULMTV_DP_RCV2_N_CF0				,	LIMIT_TYPE_CONST	,					0,			0,	 0xFFFF},
	 {ULMT_ES_CLMP_N_PS0				,	LIMIT_TYPE_INDEX	,			110000016,			0,	 0},
	 {ULMT_ES_CLMP_N_JA0				,	LIMIT_TYPE_INDEX	,			113000005,			0,	 0},
	 {ULMT_ES_CLMP_N_JD0				,	LIMIT_TYPE_INDEX	,			113000006,			0,	 0},
	 {ULMT_ES_CLMP_N_DF0				,	LIMIT_TYPE_INDEX	,			113000007,			0,	 0},
	 {ULMT_ES_CLMP_N_VL0				,	LIMIT_TYPE_INDEX	,			113000001,			0,	 0},
	 {ULMT_ES_CLMP_N_AC0				,	LIMIT_TYPE_INDEX	,			113000003,			0,	 0},
	 {ULMT_ES_CLMP_N_DC0				,	LIMIT_TYPE_INDEX	,			113000004,			0,	 0},
	 {ULMT_ES_CLMP_N_PS2				,	LIMIT_TYPE_INDEX	,			116010007,			0,	 0},
	 {ULMT_ES_CLMP_N_GF0				,	LIMIT_TYPE_INDEX	,			113000011,			0,	 0},
	 {ULMT_ES_CLMP_J_VL0				,	LIMIT_TYPE_INDEX	,			113000002,			0,	 0},
	 {ULMT_ES_CLMP_N_PS4				,	LIMIT_TYPE_INDEX	,			110010006,			0,	 0},
	 {ULMT_ES_CLMP_N_PS3				,	LIMIT_TYPE_INDEX	,			110110004,			0,	 0},
	 {ULMT_ES_CLMP_N_VL2				,	LIMIT_TYPE_INDEX	,			110110001,			0,	 0},
	 {ULMT_ES_CLMP_N_VL3				,	LIMIT_TYPE_INDEX	,			110110001,			0,	 0},
	 {ULMT_ES_LOCK_N_JA0				,	LIMIT_TYPE_INDEX	,			123000004,			0,	 0},
	 {ULMT_ES_LOCK_N_JD0				,	LIMIT_TYPE_INDEX	,			123000005,			0,	 0},
	 {ULMT_ES_LOCK_N_VL2				,	LIMIT_TYPE_INDEX	,			110110001,			0,	 0},
	 {LLMT_XX_XXX_X_XX5 				,	LIMIT_TYPE_CONST	,					0,			0,	 0},
	 {ULMT_ES_CLM_N_PS0 				,	LIMIT_TYPE_INDEX	,			110000016,			0,	 0},
	 {ULMT_XX_XXX_X_XX99999999	    	,	LIMIT_TYPE_CONST	,			0,			0,	 99999999},
	 {LINIT_324					    	,	LIMIT_TYPE_CONST	,			0,			0,	 0},
	 {LIMIT_325					    	,	LIMIT_TYPE_CONST	,			0,			0,	 0},
	 {ULMT_XX_XXX_X_XX4500		    	,	LIMIT_TYPE_CONST	,			0,			0,	 4500},
	 {ULMT_MLD_CLOSE_N_FLW		    	,	LIMIT_TYPE_CONST	,			0,			0,	 9999},
	 {ULMT_XX_XXX_X_XX200				,	LIMIT_TYPE_CONST	,			0,			0,	 200},
	 {LLMT_XX_XXX_X_XX160				,	LIMIT_TYPE_CONST	,			0,			0,	 160},
	 {LLMT_XX_XXX_X_XX65535		    	,	LIMIT_TYPE_CONST	,			0,			0,	 65535},
	 {LLMT_XX_XXX_X_XX0			    	,	LIMIT_TYPE_CONST	,			0,			0,	 0},
	 {LLMT_XX_XXX_X_XX2			    	,	LIMIT_TYPE_CONST	,			0,			0,	 2},
	 {LLMT_XX_XXX_X_XX1			    	,	LIMIT_TYPE_CONST	,			0,			0,	 1},
	 {LLMT_XX_XXX_X_XX65535P			,	LIMIT_TYPE_CONST	,			0,			0,	 -65535},
	 {LLMT_XX_XXX_X_XX655				,	LIMIT_TYPE_CONST	,			0,			0,	 655},
	 {LLMT_XX_XXX_X_XX10000		    	,	LIMIT_TYPE_CONST	,			0,			0,	 10000},
	 {LLMT_XX_XXX_X_XX250				,	LIMIT_TYPE_CONST	,			0,			0,	 250},
	 {LLMT_XX_XXX_X_XX255				,	LIMIT_TYPE_CONST	,			0,			0,	 255},
	 {EMETER1LENGTH				    	,	LIMIT_TYPE_CONST	,			0,			0,	 0},
	 {EMETER2LENGTH				    	,	LIMIT_TYPE_CONST	,			0,			0,	 0},
	 {EMETER3LENGTH				    	,	LIMIT_TYPE_CONST	,			0,			0,	 0},
	 {ULMTV_DP_MDC_N_PS0				,	LIMIT_TYPE_CONST	,			0,			0,	 0},
	 {ULMT_XX_XXX_X_XX_65535			,	LIMIT_TYPE_CONST	,			0,			0,	 65535},
	 {LLMT_XX_XXX_X_XX_1				,	LIMIT_TYPE_CONST	,			0,			0,	 1},
	 {LLMT_XX_XXX_X_XX_0				,	LIMIT_TYPE_CONST	,			0,			0,	 0},
	 {ULMT_XX_XXX_X_XX_150		    	,	LIMIT_TYPE_CONST	,			0,			0,	 120/*150*/},
	 {ULMT_XX_XXX_X_XX_1				,	LIMIT_TYPE_CONST	,			0,			0,	 1},
	 {ULMT_XX_XXX_X_XX_2				,	LIMIT_TYPE_CONST	,			0,			0,	 2},
	 {ULMT_XX_XXX_X_XX20000		    	,	LIMIT_TYPE_CONST	,			0,			0,	 20000},
	 {ULMT_XX_XXX_X_XX500				,	LIMIT_TYPE_CONST	,			0,			0,	 500},
	 {ULMT_XX_XXX_X_XX32767		    	,	LIMIT_TYPE_CONST	,			0,			0,	 32767},
	 {LLMT_XX_XXX_X_XX10				,	LIMIT_TYPE_CONST	,			0,			0,	 10},
	 {ULMT_XX_XXX_X_XX20000000	    	,	LIMIT_TYPE_CONST	,			0,			0,	 20000000},
	 {ULMT_XX_XXX_X_XX15000000	    	,	LIMIT_TYPE_CONST	,			0,			0,	 15000000},
	 {ULMT_XX_XXX_X_XX360				,	LIMIT_TYPE_CONST	,			0,			0,	 360},
	 {ULMT_XX_XXX_X_XX17				,	LIMIT_TYPE_CONST	,			0,			0,	 17},
	 {ULMT_XX_XXX_X_XX7			    	,	LIMIT_TYPE_CONST	,			0,			0,	 7},
	 {ULMT_XX_XXX_X_XX2000		    	,	LIMIT_TYPE_CONST	,			0,			0,	 2000},
	 {ULMT_XX_XXX_X_XX2000000			,	LIMIT_TYPE_CONST	,			0,			0,	 2000000},
	 {LLMT_XX_XXX_X_XX150				,	LIMIT_TYPE_CONST	,			0,			0,	 150},
	 {LLMT_XX_XXX_X_XX99999999N	    	,	LIMIT_TYPE_CONST	,			0,			0,	 -99999999},
	 {ULMT_XX_XXX_X_XX2048		    	,	LIMIT_TYPE_CONST	,			0,			0,	 2048},
	 {LLMT_XX_XXX_X_XX360000N			,	LIMIT_TYPE_CONST	,			0,			0,	 -360000},
	 {ULMT_XX_XXX_X_XX360000			,	LIMIT_TYPE_CONST	,			0,			0,	 360000},
	 {LLMT_XX_XXX_X_XX9999N		    	,	LIMIT_TYPE_CONST	,			0,			0,	 -9999},
	 {LLMT_XX_XXX_X_XX50000N			,	LIMIT_TYPE_CONST	,			0,			0,	 -50000},
	 {ULMT_XX_XXX_X_XX50000		    	,	LIMIT_TYPE_CONST	,			0,			0,	 50000},
	 {LLMT_XX_XXX_X_XX999999N			,	LIMIT_TYPE_CONST	,			0,			0,	 -999999},
	 {LLMT_XX_XXX_X_XX10N				,	LIMIT_TYPE_CONST	,			0,			0,	 -10},
	 {LLMT_XX_XXX_X_XX5000N		    	,	LIMIT_TYPE_CONST	,			0,			0,	 -5000},
	 {ULMT_XX_XXX_X_XX5000		    	,	LIMIT_TYPE_CONST	,			0,			0,	 5000},
	 {LLMT_XX_XXX_X_XX9999999N	    	,	LIMIT_TYPE_CONST	,			0,			0,	 -9999999},
	 {LLMT_XX_XXX_X_XX99999N			,	LIMIT_TYPE_CONST	,			0,			0,	 -99999},
	 {LLMT_XX_XXX_X_XXM1				,	LIMIT_TYPE_CONST	,			0,			0,	 -1}
};

ULMTDATAID_SCOPE	m_DataIDScope[ULMT_TYPE_COUNT][ULMT_STEP_COUNT]	=
{
	{{120001, 120092}, {130001, 130092}},//PVL
	{{121001, 121022}, {131001, 131022}},//POS
	{{122001, 122001}, {132001, 132001}},//TORQUE
	{{123001, 123002}, {133001, 133002}}//TEMP
};//zholy061221

char	*m_pcStartChlStrID[ULMT_TYPE_COUNT]	=
{
	CONF_PVL_STARTSTRID,
	CONF_CHL_STARTSTRID,
	CONF_TORQUE_STARTSTRID,
	CONF_TEM_STARTSTRID
};//zholy061221

CtmDatabase::CtmDatabase(): CDatabase()
{
	
}

CtmDatabase::~CtmDatabase()
{

}

//	execute it atfer the contructor
int		CtmDatabase::CreateSelf()
{
	CDatabase::CreateSelf();
	return 0;
}

//	execute it before the destructor
int		CtmDatabase::FreeSelf()
{
	return CDatabase::FreeSelf();
}

double	CtmDatabase::GetLimitValue(int nIndex, int nType)
{
	WORD	wCount = sizeof(g_LimitMap) / sizeof(LIMITMAP);
	DWORD	dwDBIndex;
	double	dValue = 0;

	if (nIndex < 0 || nIndex > wCount || nIndex != g_LimitMap[nIndex].wIndex)
		{
		//printf("LimitID[%ld] Index=%ld ID1=%ld ID2=%ld Value=%ld\n", nIndex, (int)g_LimitMap[nIndex].wIndex, (int)g_LimitMap[nIndex].dwID1, (int)g_LimitMap[nIndex].dwID2, g_LimitMap[nIndex].llValue);
		return dValue;
		}
		
	switch (g_LimitMap[nIndex].wType)
		{
		case LIMIT_TYPE_CONST:
			if (g_LimitMap[nIndex].llValue == 0xffff)
				switch (nType)
					{
					case  TYPE_FLOAT:
#ifdef	D_X86
						dValue = 900000000000LL; //Mario debug 20141128
#else
#ifdef	D_ARM
						dValue = 90000000;
#endif
#endif
						break;
					case TYPE_DOUBLE:
#ifdef	D_X86
						dValue = 900000000000LL;
#else
#ifdef	D_ARM
						dValue = 90000000;
#endif
#endif
						break;
					case TYPE_WORD:
						dValue = 65535;
						break;
					case TYPE_DWORD:
						dValue = 900000000;
						break;
					case TYPE_INT:
						dValue = 900000000;
						break;
					case TYPE_CHAR:
						dValue = 127;
						break;
					case TYPE_BYTE:
						dValue = 0;//255;
						break;
					default :
						dValue = 0;
						break;
					}
			else dValue = g_LimitMap[nIndex].llValue;
			////printf("LimitID[%d] %f\n", nIndex, dValue);
			break;
		case LIMIT_TYPE_INDEX:
			dwDBIndex = g_pDatabase->DataIDToIndex(g_LimitMap[nIndex].dwID1);
			dValue = GetValue(g_pDatabase->Read(dwDBIndex));
			////printf("LimitID[%d] %ld=%f\n", nIndex, g_LimitMap[nIndex].dwID1, dValue);
			break;
		case LIMIT_TYPE_MULTIPLY:
			dwDBIndex = g_pDatabase->DataIDToIndex(g_LimitMap[nIndex].dwID1);
			dValue = GetValue(g_pDatabase->Read(dwDBIndex));
			////printf("ID1=%ld Value=%f", g_LimitMap[nIndex].dwID1, dValue);
			dwDBIndex = g_pDatabase->DataIDToIndex(g_LimitMap[nIndex].dwID2);
			////printf("ID1=%ld Value=%f", g_LimitMap[nIndex].dwID2, GetValue(g_pDatabase->Read(dwDBIndex)));
			dValue = dValue * GetValue(g_pDatabase->Read(dwDBIndex)) / 100;
			////printf("LimitID[%d] %ld*%ld=%f\n", nIndex, g_LimitMap[nIndex].dwID1, g_LimitMap[nIndex].dwID2, dValue);
			break;
		case LIMIT_TYPE_SUBTRACT:
			dwDBIndex = g_pDatabase->DataIDToIndex(g_LimitMap[nIndex].dwID1);
			dValue = GetValue(g_pDatabase->Read(dwDBIndex));
			dwDBIndex = g_pDatabase->DataIDToIndex(g_LimitMap[nIndex].dwID2);
			dValue = dValue - GetValue(g_pDatabase->Read(dwDBIndex));
			////printf("LimitID[%d] %ld-%ld=%f\n", nIndex, g_LimitMap[nIndex].dwID1, g_LimitMap[nIndex].dwID2, dValue);
			break;
		case LIMIT_TYPE_PLUS:
			dwDBIndex = g_pDatabase->DataIDToIndex(g_LimitMap[nIndex].dwID1);
			dValue = GetValue(g_pDatabase->Read(dwDBIndex));
			dwDBIndex = g_pDatabase->DataIDToIndex(g_LimitMap[nIndex].dwID2);
			dValue = dValue + GetValue(g_pDatabase->Read(dwDBIndex));
			////printf("LimitID[%d] %ld+%ld=%f\n", nIndex, g_LimitMap[nIndex].dwID1, g_LimitMap[nIndex].dwID2, dValue);
			break;
		case LIMIT_TYPE_UNKOWN:
		default:
			//printf("Unknown Type \n");
			break;
		}

	return dValue;
}

WORD	CtmDatabase::ComputeLimitValue(char* pcID)//zholy061221
{
	int			i, nDataID, nConfStartID, nOffset, nRegion = -1;
	long		lValue, lOffset;
	
	if (pcID != NULL)
	{
		nDataID	= g_pDatabase->GetDataID(pcID);
		if (nDataID < 0)	return COMPUTE_ERROR;
		lValue	= (long)(GetIDValue(nDataID).lValue);
		if (lValue == 0)	return COMPUTE_NO;//if offsetchl = 0, not need to compute
		
		for (i = 0; i < ULMT_TYPE_COUNT; i++)//look for region
		{
			if (((DWORD)nDataID >= m_DataIDScope[i][ULMT_STEP_ULMTC].dwStart) && ((DWORD)nDataID <= m_DataIDScope[i][ULMT_STEP_ULMTC].dwEnd))
			{
				nOffset	= nDataID - m_DataIDScope[i][ULMT_STEP_ULMTC].dwStart;
				nRegion	= i;
				break;
			}
		}
		if (nRegion == -1) return COMPUTE_ERROR;//not look for region , error
		
		nConfStartID	= g_pDatabase->GetDataID(m_pcStartChlStrID[nRegion]);
		lValue			= (long)(GetIDValue(nConfStartID + lValue-1).lValue);
		if (SetIDValue(m_DataIDScope[nRegion][ULMT_STEP_ULMTV].dwStart + nOffset, lValue) != DB_SUCCESS) return COMPUTE_ERROR;
	}
	else
	{
		DWORD		j;
		for (i = 0; i < ULMT_TYPE_COUNT; i++)
		{
			nConfStartID	= g_pDatabase->GetDataID(m_pcStartChlStrID[i]);
			for (j = 0; j <= m_DataIDScope[i][ULMT_STEP_ULMTC].dwEnd - m_DataIDScope[i][ULMT_STEP_ULMTC].dwStart; j++)
			{
				lOffset	= GetIDValue(m_DataIDScope[i][ULMT_STEP_ULMTC].dwStart + j).lValue;
				if (lOffset == 0) continue;
				lValue	= GetIDValue(nConfStartID + lOffset-1).lValue;
				SetIDValue(m_DataIDScope[i][ULMT_STEP_ULMTV].dwStart + j, lValue);
//				printf("StartID=%d ChanelID=%d lOffset=%d ConfigID=%d LimitID=%d Value=%d\n", nConfStartID, m_DataIDScope[i][ULMT_STEP_ULMTC].dwStart + j, 
//					lOffset, nConfStartID + lOffset, m_DataIDScope[i][ULMT_STEP_ULMTV].dwStart + j, lValue);
			}
		}
	}
	return	COMPUTE_SUCCESS;
}

WORD	CtmDatabase::ComputeLimitValue(WORD wRegion, char* pcID)//zholy061221
{
	int			nDataID, nOffset, nConfStartID;
	long		lValue;
	
	nDataID	= g_pDatabase->GetDataID(pcID);
	
	lValue	= (long)(GetIDValue(nDataID).lValue);
	if (lValue	== 0) return COMPUTE_NO;//if offsetchl = 0, not need to compute
	
	nOffset	= nDataID - m_DataIDScope[wRegion][ULMT_STEP_ULMTC].dwStart;
	
	nConfStartID	= g_pDatabase->GetDataID(m_pcStartChlStrID[wRegion]);
	
	lValue	= (long)(GetIDValue(nConfStartID + lValue -1).lValue);
	
	if (SetIDValue(m_DataIDScope[wRegion][ULMT_STEP_ULMTV].dwStart + nOffset, lValue) != DB_SUCCESS) return COMPUTE_ERROR;
	else return COMPUTE_SUCCESS;
}


#ifndef	D_BEOBJECT_CtmDatabase
CObject*	CreateInstance(char* Name)
{
	if (g_pDatabase == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CDatabase*>(pResult))->CreateSelf();
		g_pDatabase = static_cast<CDatabase*>(pResult);
	}
	return g_pDatabase;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CDatabase*>(pInstance))->FreeSelf();
	delete pInstance;
	g_pDatabase = NULL;
	pInstance = NULL;
}
#endif
