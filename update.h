/*===========================================================================+
|  Class    : Device                                                         |
|  Task     : Device service routine header file - Virtual base class        |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 09/01/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef     D_UPDATE
#define     D_UPDATE

#include    "common.h"
#include	"wnd.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		ACTION_NOP					0
#define		ACTION_UPDATEAPP			1
#define		ACTION_UPDATEKERNEL			2
#define		ACTION_RESTORE				3

#define		CMDTYPE_NONEPARA			0
#define		CMDTYPE_ONEPARA				1
#define		CMDTYPE_TWOPARA				2
#define		CMDTYPE_UPKERNEL			3
#define		CMDTYPE_TWOPARASP			4
#define		CMDTYPE_TARPARA				5	//add for tar only

#define		E_UPDATE_NULL				0	// no error
#define		E_UPDATE_FCREATE			1	// file create fail
#define		E_UPDATE_FNOTEXIST			2	// file not exist
#define		E_UPDATE_FOPEN				3	// file open fail
#define		E_UPDATE_FSIZE				4	// file size error
#define		E_UPDATE_DCREATE			5	// directory create error
#define		E_UPDATE_NOBEGIN			6	// no [BEG] signal in mmiupdate file
#define		E_UPDATE_MEMALLOC			7	// memory alloc error
#define		E_UPDATE_MOUNT				8	// mount device error
#define		E_UPDATE_PASSWD				9	// password error
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef	struct	tagCMDTYPE
{
	WORD		wCmdType;
	char*		pcVirtualCmd;
	char*		pcRealCmd;
} tmCMDTYPE;

typedef struct tagSTORAGEDEVICE
{
	int		nDeviceNO;
	int		nDeviceHandle;
	const	char*	pDevicePath;
	const	char*	pMountPath;
} STORAGEDEV;
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - Device                                       |
+===========================================================================*/

/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void		UpdateMMIFile(CtmWnd* pWnd, int nState, long lPassword);
WORD		GetUpdateWarningFlag();
/*===========================================================================+
|           External                                                         |
+===========================================================================*/

#endif
