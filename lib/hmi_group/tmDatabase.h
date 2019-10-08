#ifndef		D__TMDATABASE_V01
#define		D__TMDATABASE_V01
#include	"../../database.h"


//=======
//		Limits index Macro
//=======
#define		INDEX_LLMT_XX_XXX_X_XX0					0  
#define		INDEX_LLMT_XX_XXX_X_XX1                 1
#define		INDEX_LLMT_XX_XXX_X_XX64                2
#define		INDEX_LLMT_XX_XXX_X_XX91                3  
#define		INDEX_LLMT_XX_XXX_X_XX_N                4  
#define		INDEX_LLMT_ES_INJ_N_PS0                 5  
#define		INDEX_LLMT_ES_INJ_N_PS1                 6  
#define		INDEX_LLMT_ES_RCV_N_PS0                 7  
#define		INDEX_LLMT_ES_RCV_N_PS1                 8  
#define		INDEX_LLMT_ES_EJT_N_PS0                 9  
#define		INDEX_LLMT_ES_IJU_N_PS0                 10 
#define		INDEX_LLMT_ES_DCP_N_PS0                 11 
#define		INDEX_LLMT_ES_MDH_N_PS0                 12 
#define		INDEX_LLMT_XX_XXX_X_XXM1E12             13 
#define		INDEX_ULMT_XX_XXX_X_XX0                 14 
#define		INDEX_ULMT_XX_XXX_X_XX1                 15 
#define		INDEX_ULMT_XX_XXX_X_XX2                 16 
#define		INDEX_ULMT_XX_XXX_X_XX3                 17 
#define		INDEX_ULMT_XX_XXX_X_XX4                 18 
#define		INDEX_ULMT_XX_XXX_X_XX10                19 
#define		INDEX_ULMT_XX_XXX_X_XX15                20 
#define		INDEX_ULMT_XX_XXX_X_XX16                21 
#define		INDEX_ULMT_XX_XXX_X_XX31                22 
#define		INDEX_ULMT_XX_XXX_X_XX100               23 
#define		INDEX_ULMT_XX_XXX_X_XX150               24 
#define		INDEX_ULMT_XX_XXX_X_XX173               25 
#define		INDEX_ULMT_XX_XXX_X_XX1000              26 
#define		INDEX_ULMT_XX_XXX_X_XX_N                27 
#define		INDEX_ULMT_XX_XXX_X_XX65535             28 
#define		INDEX_ULMT_XX_XXX_X_XX8388608           29 
#define		INDEX_ULMT_XX_XXX_X_XX4294967295        30 
#define		INDEX_ULMT_XX_XXX_X_XX1E12              31 
#define		INDEX_ULMT_XX_XXX_X_XX400               32 
#define		INDEX_ULMT_XX_XXX_X_XX1000000           33 
#define		INDEX_ULMT_XX_XXX_X_XX10000             34 
#define		INDEX_ULMT_XX_XXX_X_XX100000            35 
#define		INDEX_ULMT_ES_MLD_J_VL0                 36 
#define		INDEX_ULMT_ES_MLD_N_PS0                 37 
#define		INDEX_ULMT_ES_MLD_N_VL0                 38 
#define		INDEX_ULMT_ES_MLD_N_AC0                 39 
#define		INDEX_ULMT_ES_MLD_N_DC0                 40 
#define		INDEX_ULMT_ES_MLD_N_JA0                 41 
#define		INDEX_ULMT_ES_MLD_N_JD0                 42 
#define		INDEX_ULMT_ES_MLD_N_DF0                 43 
#define		INDEX_ULMT_ES_MLD_N_CF0                 44 
#define		INDEX_ULMT_ES_MLD_N_JA1                 45 
#define		INDEX_ULMT_ES_MLD_N_VL1                 46 
#define		INDEX_ULMT_ES_MLD_N_AC1                 47 
#define		INDEX_ULMT_ES_MLD_N_GF0                 48 
#define		INDEX_ULMT_ES_MLD_N_PS1                 49 
#define		INDEX_ULMT_ES_MLD_N_PS2                 50 
#define		INDEX_ULMT_ES_MLD_N_PS3                 51 
#define		INDEX_ULMT_ES_MLD_N_VL2                 52 
#define		INDEX_ULMT_ES_MLD_N_VL3                 53 
#define		INDEX_ULMT_ES_INJ_N_PS0                 54 
#define		INDEX_ULMT_ES_INJ_N_PS1                 55 
#define		INDEX_ULMT_ES_INJ_N_PR2                 56 
#define		INDEX_ULMT_ES_INJ_N_PR1                 57 
#define		INDEX_ULMT_ES_MLD_N_DC1                 58 
#define		INDEX_ULMT_ES_MLD_N_AC2                 59 
#define		INDEX_ULMT_ES_INJ_J_VL0                 60 
#define		INDEX_ULMT_ES_INJ_J_AC0                 61 
#define		INDEX_ULMT_ES_INJ_J_DC0                 62 
#define		INDEX_ULMT_ES_INJ_J_DF0                 63 
#define		INDEX_ULMT_ES_INJ_N_VL1                 64 
#define		INDEX_ULMT_ES_INJ_N_JA0                 65 
#define		INDEX_ULMT_ES_INJ_N_JD0                 66 
#define		INDEX_ULMT_ES_INJ_N_PR0                 67 
#define		INDEX_ULMT_ES_INJ_N_PG0                 68 
#define		INDEX_ULMT_ES_INJ_N_VL2                 69 
#define		INDEX_ULMT_ES_INJ_N_JA1                 70 
#define		INDEX_ULMT_ES_INJ_N_AC2                 71 
#define		INDEX_ULMT_ES_INJ_N_PS2                 72 
#define		INDEX_ULMT_ES_INJ_N_VL3                 73 
#define		INDEX_ULMT_ES_RCV_N_PS0                 74 
#define		INDEX_ULMT_ES_RCV_N_PS1                 75 
#define		INDEX_ULMT_ES_RCV_N_SP0                 76 
#define		INDEX_ULMT_ES_RCV_J_SP1                 77 
#define		INDEX_ULMT_ES_RCV_N_TQ0                 78 
#define		INDEX_ULMT_ES_RCV_N_AC0                 79 
#define		INDEX_ULMT_ES_RCV_N_DC0                 80 
#define		INDEX_ULMT_ES_RCV_N_JA0                 81 
#define		INDEX_ULMT_ES_RCV_N_JD0                 82 
#define		INDEX_ULMT_ES_RCV_N_PR0                 83 
#define		INDEX_ULMT_ES_RCV_N_PG0                 84 
#define		INDEX_ULMT_ES_RCV_N_VL0                 85 
#define		INDEX_ULMT_ES_RCV_N_JA1                 86 
#define		INDEX_ULMT_ES_RCV_N_AC1                 87 
#define		INDEX_ULMT_ES_RCV_N_DC1                 88 
#define		INDEX_ULMT_ES_RCV_N_GF0                 89 
#define		INDEX_ULMT_ES_RCV_N_VL1                 90 
#define		INDEX_ULMT_ES_RCV_N_PG1                 91 
#define		INDEX_ULMT_ES_RCV_N_PG2                 92 
#define		INDEX_ULMT_ES_RCV_N_PR1                 93 
#define		INDEX_ULMT_ES_RCV_N_SP2                 94 
#define		INDEX_ULMT_ES_EJT_N_PS0                 95 
#define		INDEX_ULMT_ES_EJT_N_PS1                 96 
#define		INDEX_ULMT_ES_EJT_J_VL0                 97 
#define		INDEX_ULMT_ES_EJT_N_VL1                 98 
#define		INDEX_ULMT_ES_EJT_N_AC0                 99 
#define		INDEX_ULMT_ES_EJT_N_DC0                 100
#define		INDEX_ULMT_ES_EJT_N_JA0                 101
#define		INDEX_ULMT_ES_EJT_N_JD0                 102
#define		INDEX_ULMT_ES_EJT_N_DF0                 103
#define		INDEX_ULMT_ES_EJT_N_AC1                 104
#define		INDEX_ULMT_ES_EJT_N_JA1                 105
#define		INDEX_ULMT_ES_EJT_N_VL2                 106
#define		INDEX_ULMT_ES_EJT_N_PS3                 107
#define		INDEX_ULMT_ES_EJT_N_VL3                 108
#define		INDEX_ULMT_ES_EJT_N_VL4                 109
#define		INDEX_ULMT_ES_IJU_N_PS0                 110
#define		INDEX_ULMT_ES_IJU_N_VL0                 111
#define		INDEX_ULMT_ES_IJU_J_VL1                 112
#define		INDEX_ULMT_ES_IJU_N_PS1                 113
#define		INDEX_ULMT_ES_IJU_N_AC0                 114
#define		INDEX_ULMT_ES_IJU_N_DC0                 115
#define		INDEX_ULMT_ES_IJU_N_JA0                 116
#define		INDEX_ULMT_ES_IJU_N_JD0                 117
#define		INDEX_ULMT_ES_IJU_N_DF0                 118
#define		INDEX_ULMT_ES_IJU_N_DF1                 119
#define		INDEX_ULMT_ES_IJU_N_VL2                 120
#define		INDEX_ULMT_ES_IJU_N_AC1                 121
#define		INDEX_ULMT_ES_IJU_N_JA1                 122
#define		INDEX_ULMT_ES_DCP_N_PS0                 123
#define		INDEX_ULMT_ES_DCP_N_VL0                 124
#define		INDEX_ULMT_ES_DCP_N_AC0                 125
#define		INDEX_ULMT_ES_DCP_N_DC0                 126
#define		INDEX_ULMT_ES_DCP_N_JA0                 127
#define		INDEX_ULMT_ES_DCP_N_JD0                 128
#define		INDEX_ULMT_ES_DCP_N_DF0                 129
#define		INDEX_ULMT_ES_MDH_J_VL0                 130
#define		INDEX_ULMT_ES_MDH_J_AC0                 131
#define		INDEX_ULMT_ES_MDH_J_DC0                 132
#define		INDEX_ULMT_ES_MDH_J_JA0                 133
#define		INDEX_ULMT_ES_MDH_J_JD0                 134
#define		INDEX_ULMT_ES_MDH_F_DF0                 135
#define		INDEX_ULMT_ES_MDH_N_PS0                 136
#define		INDEX_ULMT_ES_MDH_N_VL1                 137
#define		INDEX_ULMT_ES_MDH_N_AC1                 138
#define		INDEX_ULMT_ES_MDH_N_DC1                 139
#define		INDEX_ULMT_ES_MDH_N_JA1                 140
#define		INDEX_ULMT_ES_MDH_N_JD1                 141
#define		INDEX_ULMT_ES_MDH_N_DF1                 142
#define		INDEX_ULMT_ES_MDH_N_VL2                 143
#define		INDEX_ULMT_ES_MDH_N_JA2                 144
#define		INDEX_ULMT_ES_MDH_N_AC2                 145
#define		INDEX_ULMT_XX_XXX_X_XX24				146
#define		INDEX_ULMT_XX_XXX_X_XX60				147
#define		INDEX_ULMT_ES_INJ_N_AC0					148
#define		INDEX_ULMT_ES_INJ_N_DC0					149
#define		INDEX_ULMT_ES_MLD_N_PS4					150			

#define		INDEX_LLMT_XX_XXX_X_XX2					151
#define		INDEX_LLMT_XX_XXX_X_XX3                 152			//<<<zholy061221
#define		INDEX_ULMT_XX_XXX_X_XX5	            	153
#define		INDEX_ULMT_XX_XXX_X_XX6	            	154
#define		INDEX_ULMT_XX_XXX_X_XX12                155
#define		INDEX_ULMT_XX_XXX_X_XX59            	156
#define		INDEX_ULMT_XX_XXX_X_XX65            	157
#define		INDEX_ULMT_XX_XXX_X_XX60000            	158
#define		INDEX_ULMT_XX_XXX_X_XX18000             159	
#define		INDEX_ULMT_XX_XXX_X_XX23	            160	
#define		INDEX_ULMT_XX_XXX_X_XX2099            	161	
#define		INDEX_ULMT_XX_XXX_X_XX300            	162	
#define		INDEX_ULMT_XX_XXX_X_XX30000            	163	
#define		INDEX_ULMT_XX_XXX_X_XX32000            	164	
#define		INDEX_ULMT_XX_XXX_X_XX4095            	165	
#define		INDEX_ULMT_XX_XXX_X_XX9	            	166	
#define		INDEX_ULMT_XX_XXX_X_XX99            	167	
#define		INDEX_ULMT_XX_XXX_X_XX999            	168	
#define		INDEX_ULMT_XX_XXX_X_XX9999            	169	
#define		INDEX_ULMT_XX_XXX_X_XX99999            	170	
#define		INDEX_ULMT_XX_XXX_X_XX999999           	171	
#define		INDEX_ULMT_XX_XXX_X_XX9999999			172
#define		INDEX_ULMTV_DP_CMC_N_FL0				173
#define		INDEX_ULMTV_DP_CMC_N_PR0				174
#define		INDEX_ULMTV_DP_CMC_N_PS0				175
#define		INDEX_ULMTV_DP_CMO_N_FL0				176
#define		INDEX_ULMTV_DP_CMO_N_PR0				177
#define		INDEX_ULMTV_DP_CMO_N_PS0				178
#define		INDEX_ULMTV_DP_COR_N_FL0				179
#define		INDEX_ULMTV_DP_COR_N_PR0				180
#define		INDEX_ULMTV_DP_COR_N_PS0				181
#define		INDEX_ULMTV_DP_DCP_N_FL0				182
#define		INDEX_ULMTV_DP_DCP_N_PR0				183
#define		INDEX_ULMTV_DP_DCP_N_PS0				184
#define		INDEX_ULMTV_DP_DCP2_N_FL0				185
#define		INDEX_ULMTV_DP_DCP2_N_PR0				186
#define		INDEX_ULMTV_DP_DCP2_N_PS0				187
#define		INDEX_ULMTV_DP_DOR_N_FL0				188
#define		INDEX_ULMTV_DP_DOR_N_PR0				189
#define		INDEX_ULMTV_DP_EJB_N_FL0				190
#define		INDEX_ULMTV_DP_EJB_N_PR0				191
#define		INDEX_ULMTV_DP_EJB_N_PS0				192
#define		INDEX_ULMTV_DP_EJB2_N_FL0				193
#define		INDEX_ULMTV_DP_EJB2_N_PR0				194
#define		INDEX_ULMTV_DP_EJB2_N_PS0				195
#define		INDEX_ULMTV_DP_EJF_N_FL0				196
#define		INDEX_ULMTV_DP_EJF_N_PR0				197
#define		INDEX_ULMTV_DP_EJF_N_PS0				198
#define		INDEX_ULMTV_DP_EJF2_N_FL0				199
#define		INDEX_ULMTV_DP_EJF2_N_PR0				200
#define		INDEX_ULMTV_DP_EJF2_N_PS0				201
#define		INDEX_ULMTV_DP_EJT_N_FL0				202
#define		INDEX_ULMTV_DP_EJT_N_PR0				203
#define		INDEX_ULMTV_DP_EJT2_N_FL0				204
#define		INDEX_ULMTV_DP_EJT2_N_PR0				205
#define		INDEX_ULMTV_DP_HPR_N_FL0				206
#define		INDEX_ULMTV_DP_HPR_N_PR0				207
#define		INDEX_ULMTV_DP_HPR2_N_FL0				208
#define		INDEX_ULMTV_DP_HPR2_N_PR0				209
#define		INDEX_ULMTV_DP_IJU_N_FL0				210
#define		INDEX_ULMTV_DP_IJU_N_PR0				211
#define		INDEX_ULMTV_DP_IJU2_N_FL0				212
#define		INDEX_ULMTV_DP_IJU2_N_PR0				213
#define		INDEX_ULMTV_DP_INJ_N_FL0				214
#define		INDEX_ULMTV_DP_INJ_N_PR0				215
#define		INDEX_ULMTV_DP_INJ_N_PS0				216
#define		INDEX_ULMTV_DP_INJ2_N_FL0				217
#define		INDEX_ULMTV_DP_INJ2_N_PR0				218
#define		INDEX_ULMTV_DP_INJ2_N_PS0				219
#define		INDEX_ULMTV_DP_IUB_N_FL0				220
#define		INDEX_ULMTV_DP_IUB_N_PR0				221
#define		INDEX_ULMTV_DP_IUB_N_PS0				222
#define		INDEX_ULMTV_DP_IUB2_N_FL0				223
#define		INDEX_ULMTV_DP_IUB2_N_PR0				224
#define		INDEX_ULMTV_DP_IUB2_N_PS0				225
#define		INDEX_ULMTV_DP_IUF_N_FL0				226
#define		INDEX_ULMTV_DP_IUF_N_PR0				227
#define		INDEX_ULMTV_DP_IUF_N_PS0				228
#define		INDEX_ULMTV_DP_IUF2_N_FL0				229
#define		INDEX_ULMTV_DP_IUF2_N_PR0				230
#define		INDEX_ULMTV_DP_IUF2_N_PS0				231
#define		INDEX_ULMTV_DP_IUR_N_FL0				232
#define		INDEX_ULMTV_DP_IUR_N_PR0				233
#define		INDEX_ULMTV_DP_IUR2_N_FL0				234
#define		INDEX_ULMTV_DP_IUR2_N_PR0				235
#define		INDEX_ULMTV_DP_LOC_N_FL0				236
#define		INDEX_ULMTV_DP_LOC_N_PR0				237
#define		INDEX_ULMTV_DP_LOC2_N_FL0				238
#define		INDEX_ULMTV_DP_LOC2_N_PR0				239
#define		INDEX_ULMTV_DP_LOCB2_N_FL0				240
#define		INDEX_ULMTV_DP_LOCB2_N_PR0				241
#define		INDEX_ULMTV_DP_MDC_N_CF0				242
#define		INDEX_ULMTV_DP_MDC_N_FL0				243
#define		INDEX_ULMTV_DP_MDC_N_PR0				244
#define		INDEX_ULMTV_DP_MDH_N_FL0				245
#define		INDEX_ULMTV_DP_MDH_N_PR0				246
#define		INDEX_ULMTV_DP_MDO_N_FL0				247
#define		INDEX_ULMTV_DP_MDO_N_PR0				248
#define		INDEX_ULMTV_DP_MEJF_N_FL0				249
#define		INDEX_ULMTV_DP_MEJF_N_PR0				250
#define		INDEX_ULMTV_DP_MEJT_N_FL0				251
#define		INDEX_ULMTV_DP_MEJT_N_PR0				252
#define		INDEX_ULMTV_DP_MLD_N_PS0				253
#define		INDEX_ULMTV_DP_RCV_N_BPR0				254
#define		INDEX_ULMTV_DP_RCV_N_FL0				255
#define		INDEX_ULMTV_DP_RCV_N_PR0				256
#define		INDEX_ULMTV_DP_RCV_N_PS0				257
#define		INDEX_ULMTV_DP_RCV2_N_BPR0				258
#define		INDEX_ULMTV_DP_RCV2_N_FL0				259
#define		INDEX_ULMTV_DP_RCV2_N_PR0				260
#define		INDEX_ULMTV_DP_RCV2_N_PS0				261
#define		INDEX_ULMTV_DP_RDR_N_FL0				262
#define		INDEX_ULMTV_DP_RDR_N_PR0				263
#define		INDEX_ULMTV_DP_RMDB_N_FL0				264
#define		INDEX_ULMTV_DP_RMDB_N_PR0				265
#define		INDEX_ULMTV_DP_RMDB_N_PS0				266
#define		INDEX_ULMTV_DP_RMDF_N_FL0				267
#define		INDEX_ULMTV_DP_RMDF_N_PR0				268
#define		INDEX_ULMTV_DP_RMDF_N_PS0				269
#define		INDEX_ULMTV_DP_RMLD_N_FL0				270
#define		INDEX_ULMTV_DP_RMLD_N_PR0				271
#define		INDEX_ULMTV_DP_RMLD_N_PS0				272
#define		INDEX_ULMTV_DP_ROT_N_FL0				273
#define		INDEX_ULMTV_DP_ROT_N_PR0				274
#define		INDEX_ULMTV_DP_ROT_N_PS0				275
#define		INDEX_ULMTV_DP_SHT_N_FL0				276
#define		INDEX_ULMTV_DP_SHT_N_PR0				277
#define		INDEX_ULMTV_DP_SHT2_N_FL0				278
#define		INDEX_ULMTV_DP_SHT2_N_PR0				279
#define		INDEX_ULMTV_DP_SLB_N_FL0				280
#define		INDEX_ULMTV_DP_SLB_N_PR0				281
#define		INDEX_ULMTV_DP_SLF_N_FL0				282
#define		INDEX_ULMTV_DP_SLF_N_PR0				283
#define		INDEX_ULMTV_DP_UEJB_N_FL0				284
#define		INDEX_ULMTV_DP_UEJB_N_PR0				285
#define		INDEX_ULMTV_DP_UEJF_N_FL0				286
#define		INDEX_ULMTV_DP_UEJF_N_PR0				287
#define		INDEX_ULMTV_DP_TMP_N_DG0				288
#define		INDEX_ULMTV_DP_TMP2_N_DG0				289
#define		INDEX_ULMT_XX_XXX_X_XX250				290
#define		INDEX_ULMT_XX_XXX_X_XX255				291
#define		INDEX_ULMT_XX_XXX_X_XX365				292
#define		INDEX_ULMT_XX_XXX_X_XX450				293
#define		INDEX_ULMT_ES_HLDPRS_N_AC0				294
#define		INDEX_ULMT_ES_HLDPRS_N_DC0				295
#define		INDEX_ULMT_ES_MLD_N_DF1					296
#define		ULMT_XX_XXX_X_XX3000					297
#define	    ULMT_XX_XXX_X_XX594000					298
#define		ULMT_XX_XXX_X_XX140						299
#define		ULMT_XX_XXX_X_XX15999					300
#define		LLMT_XX_XXX_X_XX100						301
#define		ULMT_XX_XXX_X_XX160						302
#define		ULMTV_DP_RCV2_N_CF0						303
#define		ULMT_ES_CLMP_N_PS0						304
#define		ULMT_ES_CLMP_N_JA0						305
#define		ULMT_ES_CLMP_N_JD0						306
#define		ULMT_ES_CLMP_N_DF0						307
#define		ULMT_ES_CLMP_N_VL0						308	
#define		ULMT_ES_CLMP_N_AC0						309
#define		ULMT_ES_CLMP_N_DC0						310
#define		ULMT_ES_CLMP_N_PS2						311
#define		ULMT_ES_CLMP_N_GF0						312
#define		ULMT_ES_CLMP_J_VL0						313	
#define		ULMT_ES_CLMP_N_PS4						314
#define		ULMT_ES_CLMP_N_PS3						315
#define		ULMT_ES_CLMP_N_VL2						316																												
#define		ULMT_ES_CLMP_N_VL3						317																													
#define		ULMT_ES_LOCK_N_JA0						318																														
#define		ULMT_ES_LOCK_N_JD0						319																														
#define		ULMT_ES_LOCK_N_VL2						320																														
#define		LLMT_XX_XXX_X_XX5 						321																														
#define		ULMT_ES_CLM_N_PS0 						322																														
#define		ULMT_XX_XXX_X_XX99999999				323
#define		LINIT_324								324
#define		LIMIT_325								325
#define		ULMT_XX_XXX_X_XX4500					326
#define		ULMT_MLD_CLOSE_N_FLW					327
#define		ULMT_XX_XXX_X_XX200						328
#define		LLMT_XX_XXX_X_XX160						329
#define		LLMT_XX_XXX_X_XX65535					330
#define		LLMT_XX_XXX_X_XX0						331
#define		LLMT_XX_XXX_X_XX2						332
#define		LLMT_XX_XXX_X_XX1						333
#define		LLMT_XX_XXX_X_XX65535P					334
#define		LLMT_XX_XXX_X_XX655						335
#define		LLMT_XX_XXX_X_XX10000					336
#define		LLMT_XX_XXX_X_XX250						337
#define		LLMT_XX_XXX_X_XX255						338
#define		EMETER1LENGTH							339
#define		EMETER2LENGTH							340
#define		EMETER3LENGTH							341
#define		ULMTV_DP_MDC_N_PS0						342
#define		ULMT_XX_XXX_X_XX_65535					343
#define		LLMT_XX_XXX_X_XX_1						344
#define		LLMT_XX_XXX_X_XX_0						345
#define		ULMT_XX_XXX_X_XX_150					346
#define		ULMT_XX_XXX_X_XX_1						347
#define		ULMT_XX_XXX_X_XX_2						348
#define		ULMT_XX_XXX_X_XX20000					349
#define		ULMT_XX_XXX_X_XX500						350
#define		ULMT_XX_XXX_X_XX32767					351
#define		LLMT_XX_XXX_X_XX10						352
#define		ULMT_XX_XXX_X_XX20000000				353
#define		ULMT_XX_XXX_X_XX15000000				354
#define		ULMT_XX_XXX_X_XX360						355
#define		ULMT_XX_XXX_X_XX17						356
#define		ULMT_XX_XXX_X_XX7						357
#define		ULMT_XX_XXX_X_XX2000					358
#define		ULMT_XX_XXX_X_XX2000000					359
#define		LLMT_XX_XXX_X_XX150						360
#define		LLMT_XX_XXX_X_XX99999999N				361
#define		ULMT_XX_XXX_X_XX2048					362
#define		LLMT_XX_XXX_X_XX360000N					363
#define		ULMT_XX_XXX_X_XX360000					364
#define		LLMT_XX_XXX_X_XX9999N					365
#define		LLMT_XX_XXX_X_XX50000N					366
#define		ULMT_XX_XXX_X_XX50000					367
#define		LLMT_XX_XXX_X_XX999999N					368
#define		LLMT_XX_XXX_X_XX10N						369
#define		LLMT_XX_XXX_X_XX5000N					370
#define		ULMT_XX_XXX_X_XX5000					371
#define		LLMT_XX_XXX_X_XX9999999N				372
#define		LLMT_XX_XXX_X_XX99999N					373
#define		LLMT_XX_XXX_X_XXM1						374






















































  

class  CtmDatabase: public CDatabase
{
	DECLARE_DYNCREATE(CtmDatabase)
	public:
		CtmDatabase();
		~CtmDatabase();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		
		/*virtual*/	double	GetLimitValue(int nIndex, int nType)				;
		/*virtual*/	WORD		ComputeLimitValue(char* pcID)					;
		/*virtual*/	WORD		ComputeLimitValue(WORD wRegion, char* pcID)		;	
	
};




#ifndef	D_BEOBJECT_CTMDATABASE	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

