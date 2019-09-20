/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Function : Utilities                                                      |
|  Task     : Utilities                                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                         |
|  Version  : V1.00                                                          |
|  Creation : 01/14/2005                                                     |
|  Revision : 01/14/2005                                                     |
+===========================================================================*/
#ifndef		D_TIMER
#define		D_TIMER

#include	"common.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		TIME_MAX		128				// �̤j�w�ɾ��ƥ�

#define		TIME_SUCCESS	0
#define		TIME_E_MAX		1				// �w�ɾ���F�̤j�ƥ�
#define		TIME_E_NOTIMER	2				// �w�ɾ����s�j
#define		TIME_E_EXIST	3				// �w�ɾ��w�g�s�b
/*==========================================================================+
|			Type definition                                           		| 
+==========================================================================*/ 
typedef	struct tagTIMER
{
	long*		plCounter;
}	TIMER;

typedef TIMER* PTIMER;

//>>>zholy061220
typedef void (*SENDFUNC)();

typedef struct tagRoutine
{
	int			nFlag;
	int			nCount;
	SENDFUNC	pFunc;
	long		lCounter;
} Routine;
//<<<zholy061220

void* 	TimerRun (void* data);
int		Init_Timer();
int		Destroy_Timer();
int		AddTimer(long* plCounter);
int		DeleteTimer(long* plCounter);
int		AddRoutine(int nCount, SENDFUNC	pFunc);		//zholy061220
int		DeleteRoutine();							//zholy061220

#endif
