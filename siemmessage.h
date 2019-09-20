/*==========================================================================+
|  Class    : Message 			                                            |
|  Task     : Message header file  		                        			|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : yang		                                                    |
|  Version  : V1.00                                                         |
|  Creation : 2006/3/5                                                    	|
|  Revision : 			                                                    |
+==========================================================================*/
#include	"common.h"
#include	"tasksiem.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		FO_FAULT				1
#define		TO_FAULT				2
#define		TIME_FAULT				3
#define		EXECUTION_FAULT			4
#define		PERIPHERAL_FAULT		5
#define		TELEGRAM_FAULT			6
#define		APP_FAULT				8				//<<yang 2006/10/29 
#define		DRIVECOMM_FAULT			9
#define		I32_FAULT				0x10
				

#define		FO_INJECTIONUNIT		1
#define		FO_PREDECOMPRESSION		2
#define		FO_RECOVERY				3
#define		FO_POSTDECOMPRESSION	4
#define		FO_INJECTION			5
#define		FO_MOLD					6
#define		FO_EJECTOR				7
#define		FO_MOLDHEIGHT			8

#define		TO_DRIVEAXIS			1
#define		TO_OUTPUTCAMTYPE		2
#define		TO_CAMTRACKTYPE			3
#define		TO_MEASURINGINPUTTYPE	4
#define		TO_EXTERNALENCODERTYPE	5
#define		TO_POSAXIS				6
#define		TO_FOLLOWINGAXIS		7
#define		TO_FOLLOWINGOBJECTTYPE	8
#define		TO_CAMTYPE				9
#define		TO_ADDITIONOBJECTTYPE	10
#define		TO_FIXEDGEARTYPE		11
#define		TO_FORMULAOBJECTTYPE	12
#define		TO_CONTROLLEROBJECTTYPE	13
#define		TO_SENSORTYPE			14
		
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/



/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
//char*	GetMessageStr(SIMD_MSG siemMsg);





































