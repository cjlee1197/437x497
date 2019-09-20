// 3des.cpp : Defines the entry point for the console application.
//
#include 	"common.h"
#include 	<string.h>

#define     DISENCRYPT        0             // 
#define     ENCRYPT           1             // 


/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
#pragma CODE_SECTION(RotateLeftMove , "ramfuncs");
#pragma CODE_SECTION(ChangeToPC1  , "ramfuncs");
#pragma CODE_SECTION(ChangeToPC2  , "ramfuncs");
#pragma CODE_SECTION(ChangeIP , "ramfuncs");
#pragma CODE_SECTION(ChangeIP_1  , "ramfuncs");
#pragma CODE_SECTION(Expend , "ramfuncs");
#pragma CODE_SECTION(Change_PP , "ramfuncs");
#pragma CODE_SECTION(Change_S , "ramfuncs");
#pragma CODE_SECTION(F_Convert , "ramfuncs");
#pragma CODE_SECTION(Encrypt_8Byte , "ramfuncs");
#pragma CODE_SECTION(DES3_Encrypt_BurnAll , "ramfuncs");
#pragma CODE_SECTION(DES3_DisEncrypt_BurnAll , "ramfuncs");
#pragma CODE_SECTION(DivMod , "ramfuncs");

/*===========================================================================+
|           Function and Class prototype - Error                             |
+===========================================================================*/
long    	RotateLeftMove(LONG lSrc, WORD wMaxBit, WORD wBit);
void    	ChangeToPC1(LONG *plKey, LONG *plPC1);
void    	ChangeToPC2(LONG *plPC1, LONG *plPC2, int nMode);
void    	ChangeIP(LONG *plData);
void    	ChangeIP_1(LONG *pData);
void    	Expend(LONG lData, LONG *plLeft, LONG *plRight);
void    	Change_PP(LONG *plData);
LONG    	Change_S(LONG *plData);
void    	F_Convert(LONG *plData, LONG *plSubKey, int nMode);
void    	Encrypt_8Byte(LONG *plData, LONG *plSubKey, int nMode);
void     	DES3_Encrypt_BurnAll(LONG *plData,WORD	wLength);
void     	DES3_DisEncrypt_BurnAll(LONG *plData,WORD	wLength);
DWORD		DivMod(DWORD dwDividend, DWORD dwDivisor, int	nType);

//+++++
//“ñ
//+++++
int u_acPC1Bit[16] = 
{
    1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28 
};

//+++++
//  
//+++++
int u_acPC1[56] = 
{
    56, 48, 40, 32, 24, 16,  8,
     0, 57, 49, 41, 33, 25, 17,
	 9,  1, 58, 50, 42, 34, 26,     
	18, 10,  2, 59, 51, 43, 35,
	62, 54, 46, 38, 30, 22, 14,
	 6, 61, 53, 45, 37, 29, 21,
	13,  5, 60, 52, 44, 36, 28,
	20, 12,  4, 27, 19, 11,  3
};

//+++++
//  
//+++++
int  u_acPC2[48] =
{
    13, 16, 10, 23,  0,  4,  
     2, 27, 14,  5, 20,  9,
	22, 18, 11,  3, 25,  7, 
	15,  6, 26, 19, 12,  1,
	40, 51, 30, 36, 46, 54, 
	29, 39, 50, 44, 32, 47,
	43, 48, 38, 55, 33, 52, 
	45, 41, 49, 35, 28, 31
};


//+++++
//  
//+++++
int   u_acIP[64] =
{
    57, 49, 41, 33, 25, 17,  9, 1, 
    59, 51, 43, 35, 27, 19, 11, 3, 
    61, 53, 45, 37, 29, 21, 13, 5, 
    63, 55, 47, 39, 31, 23, 15, 7, 
    56, 48, 40, 32, 24, 16,  8, 0, 
    58, 50, 42, 34, 26, 18, 10, 2, 
    60, 52, 44, 36, 28, 20, 12, 4, 
    62, 54, 46, 38, 30, 22, 14, 6
};

//+++++
//  
//+++++
int   u_acIP_1[64] =
{
    39, 7, 47, 15, 55, 23, 63, 31, 
    38, 6, 46, 14, 54, 22, 62, 30, 
    37, 5, 45, 13, 53, 21, 61, 29, 
    36, 4, 44, 12, 52, 20, 60, 28, 
    35, 3, 43, 11, 51, 19, 59, 27, 
    34, 2, 42, 10, 50, 18, 58, 26, 
    33, 1, 41,  9, 49, 17, 57, 25, 
    32, 0, 40,  8, 48, 16, 56, 24 
};


//+++++
//  
//+++++
int  u_acExp[48] =
{
    31,  0,  1,  2,  3,  4, 
     3,  4,  5,  6,  7,  8, 
     7,  8,  9, 10, 11, 12, 
    11, 12, 13, 14, 15, 16, 
    15, 16, 17, 18, 19, 20, 
    19, 20, 21, 22, 23, 24, 
    23, 24, 25, 26, 27, 28, 
    27, 28, 29, 30, 31,  0
};


//+++++
//  
//+++++
int u_acPP[32] =
{
    15,  6, 19, 20, 
    28, 11, 27, 16, 
     0, 14, 22, 25, 
     4, 17, 30,  9, 
     1,  7, 23, 13, 
    31, 26,  2,  8, 
    18, 12, 29,  5, 
    21, 10,  3, 24
};




// 
int u_acS1[64] = {

    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13 
};
// “Q
int u_acS2[64] = 
{

    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,   
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,   
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,    
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 
};
// 
int u_acS3[64] = 
{

    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12 
};
// 
int u_acS4[64] = 
{

     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14 
};
// 
int u_acS5[64] = 
{
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,    
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,    
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,    
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3 
};
// 
int u_acS6[64] = 
{
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13 
};
// “Q
int u_acS7[64] =
{
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,    
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,    
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,    
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12 
};
// “Q
int u_acS8[64] = 
{
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11 
};


//+++++
//  “Q
//+++++
int *u_paSBox[8] =
{
    u_acS1,
    u_acS2,
    u_acS3,
    u_acS4,
    u_acS5,
    u_acS6,
    u_acS7,
    u_acS8    
};   

LONG    alKey1[2] = {0x61016418, 0x22181686};
LONG    alKey2[2] = {0x22181686, 0x86987285};
LONG    alKey3[2] = {0x86987285, 0x61016418};

/*------------------------------------------------------------------------------------+
|  Function : RotateLeftMove(LONG lSrc, WORD wMaxBit, WORD wBit)                      |
|  Task     : Œ¢                                       |
+-------------------------------------------------------------------------------------+
|  Call     : RotateLeftMove(lSrc, wMaxBit, wBit)                                     |
|                                                                                     |
|  Parameter: ”µ                         |
|  Return   : Ÿo                                                                      |
+------------------------------------------------------------------------------------*/
long    RotateLeftMove(LONG lSrc, WORD wMaxBit, WORD wBit)
{
	WORD    wLeftBit = wMaxBit - wBit;
    LONG    lMask1   = (1L<<wMaxBit) -1;
    LONG    lMask2   = (1L<<wBit) -1;
    
    lSrc    &=  lMask1;
    
    return  ((lSrc>>wBit) | ((lSrc & lMask2) << wLeftBit));
}
/*------------------------------------------------------------------------------------+
|  Function : ChangeToPC1(BYTE *pKey, LONG *plDst)                                    |
|  Task     : Œ¢                                        |
+-------------------------------------------------------------------------------------+
|  Call     : ChangeToPC1(pKey, plDst)                                                |
|                                                                                     |
|  Parameter: pKey:                            |
|  Return   : Ÿo                                                                      |
+------------------------------------------------------------------------------------*/
void    ChangeToPC1(LONG *plKey, LONG *plPC1)
{
    int     i =0;
    LONG    lPC1 = 0;
    //+++++
    //  
    //+++++
    for (i = 0; i < 28; i++)
    {        
      	lPC1 |= ((plKey[u_acPC1[i]>>5] & (1L<<((long)u_acPC1[i]&0x1F))) ? 1L<<i: 0);       
    }    
    *plPC1 = lPC1;
    
    //+++++
    //  
    //+++++
    lPC1   = 0;    
    for (i = 28; i < 56; i++)
    {
        lPC1 |= ((plKey[u_acPC1[i]>>5] & (1L<<(u_acPC1[i]&0x1F))) ? 1L<<(i-28): 0);
    }   
    *(plPC1+1) = lPC1;    
}

/*------------------------------------------------------------------------------------+
|  Function : ChangeToPC2(LONG *plPC1, LONG *plPC2)                                   |
|  Task     : Œ¢                                        |
+-------------------------------------------------------------------------------------+
|  Call     : ChangeToPC2(plPC1, plPC2)                                               |
|                                                                                     |
|  Parameter:        |
|  Return   : Ÿo                                                                      |
+------------------------------------------------------------------------------------*/
void    ChangeToPC2(LONG *plPC1, LONG *plPC2, int nMode)
{
    int     i =0, j = 0;
    
    LONG    lPC1L       = *plPC1, lPC1R = *(plPC1+1);       // 
    LONG    alMove[2]   = {0, 0};                           // 
    LONG    lPC2        = 0;                                // 
    WORD    wBit        = 0;
    
    for (i =0; i < 16; i++)
    {
        wBit = (ENCRYPT == nMode) ? u_acPC1Bit[i] : u_acPC1Bit[15 - i];
		alMove[0] = alMove[1]	= 0;
		lPC2					= 0;
      
        alMove[0] = RotateLeftMove(lPC1L, 28, wBit);
        alMove[1] = RotateLeftMove(lPC1R, 28, wBit);
        //+++++
        //  
        //+++++
        lPC2  = 0;
        for (j = 0; j < 24; j++)
        {
           	lPC2 |= ((alMove[DivMod((DWORD)u_acPC2[j],(DWORD)28,(int)1)] & (1L<<(DivMod((DWORD)u_acPC2[j],(DWORD)28,(int)0)))) ? 1L<<j: 0);
            //lPC2 |= ((alMove[u_acPC2[j]/28] & (1L<<(u_acPC2[j]%28))) ? 1L<<j: 0);
        }        
        *(plPC2+i*2) = lPC2;
        
        //+++++
        //  
        //+++++
        lPC2  = 0;
        for (j = 24; j < 48; j++)
        {
           	lPC2 |= ((alMove[DivMod((DWORD)u_acPC2[j],(DWORD)28,(int)1)] & (1L<<(DivMod((DWORD)u_acPC2[j],(DWORD)28,(int)0)))) ? 1L<<(j-24): 0);
//            lPC2 |= ((alMove[u_acPC2[j]/28] & (1L<<(u_acPC2[j]%28))) ? 1L<<(j-24): 0);
        }
        *(plPC2 + i*2 + 1) = lPC2;
    }
}

/*------------------------------------------------------------------------------------+
|  Function : ChangeIP(LONG *pData, LONG *plLeft, LONG *plRight)                      |
|  Task     :                        |
+-------------------------------------------------------------------------------------+
|  Call     : ChangeIP(pData, plLeft, plRight)                                        |
|                                                                                     |
|  Parameter:                                              |
|  Return   : Ÿo                                                                      |
+------------------------------------------------------------------------------------*/
void    ChangeIP(LONG *plData)
{
	int		i;
    long   lLeft = 0, lRight = 0;
    
    for (i = 0; i < 32; i++)
    {
        lLeft |= ((plData[u_acIP[i]>>5] & (1L<<(u_acIP[i]&0x1F))) ? 1L<<i: 0);
    }
    
    for (i = 32; i < 64; i++)
    {
        lRight |= ((plData[u_acIP[i]>>5] & (1L<<(u_acIP[i]&0x1F))) ? 1L<<(i-32): 0);
    }
    
    plData[0]     = lLeft;
    plData[1]     = lRight;
    
}
/*------------------------------------------------------------------------------------+
|  Function : ChangeIP_1(LONG *pData, LONG *plLeft, LONG *plRight)                    |
|  Task     : Œ¢                     |
+-------------------------------------------------------------------------------------+
|  Call     : ChangeIP_1(pData, plLeft, plRight)                                      |
|                                                                                     |
|  Parameter:                                                   |
|  Return   : Ÿo                                                                      |
+------------------------------------------------------------------------------------*/
void    ChangeIP_1(LONG *pData)
{
	int    i;
    long   lLeft = 0, lRight = 0;
    
    for (i = 0; i < 32; i++)
    {
        lLeft |= ((pData[u_acIP_1[i]>>5] & (1L<<(u_acIP_1[i]&0x1F))) ? 1L<<i: 0);
    }
    
    for (i = 32; i < 64; i++)
    {
        lRight |= ((pData[u_acIP_1[i]>>5] & (1L<<(u_acIP_1[i]&0x1F)))? 1L<<(i-32): 0);
    }
    
    *pData          = lLeft;
    *(pData+1)      = lRight;
    
}
/*------------------------------------------------------------------------------------+
|  Function : Expend(LONG lData, LONG *plLeft, LONG *plRight)                         |
|  Task     :                    |
+-------------------------------------------------------------------------------------+
|  Call     : Expend(lData, plLeft, plRight)                                          |
|                                                                                     |
|  Parameter:         |
|  Return   : Ÿo                                                                      |
+------------------------------------------------------------------------------------*/
void    Expend(LONG lData, LONG *plLeft, LONG *plRight)
{
	int    i;
    LONG   lLeft = 0, lRight = 0;
    
    for (i = 0; i < 24; i++)
    {
        lLeft |= ((lData & (1L<<u_acExp[i])) ? 1L<<i : 0);
    }

    
    for (i = 24; i < 48; i++)
    {
        lRight |= ((lData & (1L<<u_acExp[i])) ? 1L<<(i-24) : 0);
    }

    *plLeft     = lLeft;
    *plRight    = lRight;
}



void    Change_PP(LONG *plData)
{
    int     i;
    LONG    lData = 0;
    
    for (i = 0; i < 32; i++)
    {
        lData |= (((*plData) & (1L<<u_acPP[i])) ? 1L<<i: 0);
    }
    
    *plData = lData;
    
}

LONG    Change_S(LONG *plData)
{
    int    i;
    LONG   lData        = *plData;
    BYTE   bSBox        = 0;
    LONG   lSChange     = 0; 
    
    int    nLine        = 0;        //  
    int    nRow         = 0;        // 
    char   cBox         = 0;
    
//    int	   nShift		= 0;
//    BYTE   nTemp		= 0;
    
    for (i = 0; i < 8; i++)
    {
		//+++++
		//	
		//+++++
        lData       = *(plData+(i>>2));				
        
//        switch(i)
//        {
//        	case 0:
//        	case 4:
//        		nShift = 0;
//        		break;
//        	case 1:
//        	case 5:
//        		nShift = 6;
//        		break;
//        	case 2:
//        	case 6:
//        		nShift = 12;
//        		break;
//        	case 3:
//        	case 7:
//        		nShift = 18;
//        		break;	
//        }
//        bSBox       = (lData>>nShift) & 0x3F; 
        bSBox       = (lData>>((DivMod(i,4,0))*6)) & 0x3F; 
		
		//+++++
		//	
		//+++++
        nLine       = ((bSBox&0x1)<<1) + (bSBox>>5);

        nRow        = (((bSBox>>1)&0x1)<<3) + (((bSBox>>2)&0x1)<<2) + (((bSBox>>3)&0x1)<<1) + ((bSBox>>4)&0x1);

//        nRow        = ((bSBox>>1)&0x1L)*8 + ((bSBox>>2)&0x1L)*4 + ((bSBox>>3)&0x1L)*2 + ((bSBox>>4)&0x1L);
		cBox        = (u_paSBox[i])[(nLine<<4) + nRow];
		
		//+++++
		// “Q
		//+++++
        lSChange   |= (((long)((cBox>>3))+ (((cBox>>2)&0x1)<<1) + (((cBox>>1)&0x1)<<2) + ((cBox&0x1)<<3)) << (i<<2));
//        lSChange   |= ((long)((cBox>>3)+ ((cBox>>2)&0x1)*2 + ((cBox>>1)&0x1)*4 + (cBox&0x1)*8) << (i*4));
    }
    return  lSChange;   
}



void    F_Convert(LONG *plData, LONG *plSubKey, int nMode)
{
    int     i;
    LONG    lLeft = plData[0], lRight = plData[1], lLocal = plData[1];
    LONG    alExpend[2] = {0, 0}; 
    
    for (i = 0; i < 16; i++)
    {
        //+++++
        //  ”U
        //+++++
        Expend(lLocal, &alExpend[0], &alExpend[1]);
        //+++++
        // 
        //+++++
        alExpend[0] ^=  plSubKey[i<<1];
        alExpend[1] ^=  plSubKey[(i<<1) + 1];

		//alExpend[0] &= 0xFFFFFF;
		//alExpend[1] &= 0xFFFFFF;
        //+++++
        //  “Q
        //+++++
        lLocal = Change_S(alExpend);
        //+++++
        //  
        //+++++
        Change_PP(&lLocal);
        //+++++
        // 
        //+++++
        lLocal ^=  lLeft;
     
       		
		lLeft = lRight;
		
		lRight = lLocal;

		
    }
	
	plData[0] = lRight;
	plData[1] = lLeft;

}



//void    Encrypt_8Byte(LONG *plData, LONG *plKey, int nMode)
//{
//	LONG    alPC[2]  = {0, 0};
//	LONG    alSubKey[32];
//	
//	//+++++
//	//
//	//+++++
//	ChangeToPC1(plKey, alPC);  	
//    ChangeToPC2(alPC, alSubKey, nMode);
//    //+++++
//    //  
//    //+++++
//    ChangeIP(plData);
//    //+++++
//    //  
//    //+++++
//    F_Convert(plData, alSubKey, nMode);
//
//    //+++++
//    //  
//    //+++++
//    ChangeIP_1(plData);
//}

void    Encrypt_8Byte(LONG *plData, LONG *plSubKey, int nMode)
{
    //+++++
    //  
    //+++++
    ChangeIP(plData);
    //+++++
    //  
    //+++++
    F_Convert(plData, plSubKey, nMode);

    //+++++
    // “Q
    //+++++
    ChangeIP_1(plData);
}

void     DES3_Encrypt_BurnAll(LONG *plData,WORD	wLength)
{   
	int 	i=0;
	LONG    alPC[2]  = {0, 0};
	LONG    alSubKey1[32];
	LONG    alSubKey2[32];
	LONG    alSubKey3[32];
	
	//+++++
	//
	//+++++
	ChangeToPC1(alKey1, alPC);  	
    ChangeToPC2(alPC, alSubKey1, ENCRYPT);	
	ChangeToPC1(alKey2, alPC);  	
    ChangeToPC2(alPC, alSubKey2, DISENCRYPT);	
	ChangeToPC1(alKey3, alPC);  	
    ChangeToPC2(alPC, alSubKey3, ENCRYPT);	
    
	for(i=0;i<(wLength>>2);i++)
    {
    	Encrypt_8Byte((plData+(i<<1)), alSubKey1, ENCRYPT);    
    	Encrypt_8Byte((plData+(i<<1)), alSubKey2, DISENCRYPT);   
    	Encrypt_8Byte((plData+(i<<1)), alSubKey3, ENCRYPT);  
	}  
}

void     DES3_DisEncrypt_BurnAll(LONG *plData,WORD	wLength)
{
	int 	i=0;
	LONG    alPC[2]  = {0, 0};
	LONG    alSubKey1[32];
	LONG    alSubKey2[32];
	LONG    alSubKey3[32];
	
	//+++++
	//
	//+++++
	ChangeToPC1(alKey1, alPC);  	
    ChangeToPC2(alPC, alSubKey1, DISENCRYPT);	
	ChangeToPC1(alKey2, alPC);  	
    ChangeToPC2(alPC, alSubKey2, ENCRYPT);	
	ChangeToPC1(alKey3, alPC);  	
    ChangeToPC2(alPC, alSubKey3, DISENCRYPT);	
    
	for(i=0;i<(wLength>>2);i++)
    {
    	Encrypt_8Byte((plData+(i<<1)), alSubKey3, DISENCRYPT);   
    	Encrypt_8Byte((plData+(i<<1)), alSubKey2, ENCRYPT);  
    	Encrypt_8Byte((plData+(i<<1)), alSubKey1, DISENCRYPT);
    }
}

//nType: 1 div;0 :mod
DWORD	DivMod(DWORD dwDividend, DWORD dwDivisor, int	nType)
{
	DWORD	dwTemp1 = dwDivisor;
	DWORD	dwTemp2 = dwDividend;
	DWORD	dwResult = 0;
	int		i;
	
	if(dwDivisor > dwDividend) 
		return	nType ? 0 : dwDividend;
	else if(dwDivisor == dwDividend) 
		return	nType ? 1 : 0;
	else
	{
		do
		{
			i = 0;
			while((dwTemp1 = dwDivisor<<(i+1)) < dwTemp2)
			{
				i++;
				if(dwTemp1>=0x80000000)
					break;
			}
			dwResult |= 1L<<i;
			dwTemp2 -= dwDivisor<<i;
		}
		while(dwTemp2 > dwDivisor);
		return nType ? dwResult : dwTemp2;
	}	
}

