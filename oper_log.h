

#ifndef D_OPER_LOG
#define	D_OPER_LOG

#include	"database.h"
#include	"cmdswitch.h"

//=================================================================
//rs232
//=================================================================
/*---------------------------------------------------------------------------+
|           RS232 device driver                                              |
+---------------------------------------------------------------------------*/

	
typedef struct tagtmOper_log
{
	//DWORD			wIndex;
	DWORD			wDBIndex;                        		
	DWORD			dOldValue;
	DWORD			dNewValue;
	tmDATE		dateStart;                          // 故障起始日期
	tmTIME		timeStart;                          // 故障起始時間
}tmOper_log;


#endif
