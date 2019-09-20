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
#ifndef     D_DEVICE
#define     D_DEVICE

#include    "common.h"
#include 	"CObject.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
//=======
//      Device state code
//=======
#define     DEVICE_S_CLOSED        0
#define     DEVICE_S_OPENED        1
#define     DEVICE_S_IDLE          2
#define     DEVICE_S_BUSY          3
#define     DEVICE_S_HOLD          4
#define     DEVICE_S_ERROR         5

//=======
//      Device operation status
//=======
#define     DEVICE_SUCCESS      	0x0000
#define     DEVICE_OK              	0x0000  // Operation status : OK
#define     DEVICE_ERR_DEVICE      	0x0001  // Operation status : Illegal device code
#define     DEVICE_ERR_FUNCTION    	0x0002  // Operation status : Illegal function code

#define     DEVICE_E_FAILURE       	0x0010
#define     DEVICE_E_NOPORT        	0x0020
#define     DEVICE_E_OPENED        	0x0030
#define     DEVICE_E_CLOSED        	0x0040
#define     DEVICE_E_BUSY          	0x0050
#define     DEVICE_E_HOLD          	0x0060

#define     DEVICE_E_DSR_NOTREADY  	0x0080
#define     DEVICE_E_CTS_NOTREADY  	0x0090
#define     DEVICE_E_RX_EMPTY      	0x00A0
#define     DEVICE_E_TX_FULL       	0x00B0

#define     DEVICE_E_TOKEN_NONE    	0x10C0
#define     DEVICE_E_TOKEN_STX     	0x20C1
#define     DEVICE_E_TOKEN_PKL     	0x30C2
#define     DEVICE_E_TOKEN_DATA    	0x40C3
#define     DEVICE_E_TOKEN_CRC     	0x50C4
#define     DEVICE_E_TOKEN_ETX     	0x60C5
#define     DEVICE_E_BUFFER_SMALL  	0x70C6

#define     DEVICE_E_TOKEN_WAIT    	0x80C8

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
//=======
//      Device channel number
//=======
enum COM    {
            COM1,                       // Starts from 0 for system compatibility
            COM2,
            COM3,
            COM4,
            COM5,
            COM6
            };

enum LPT    {
            LPT1,                       // Starts from 0 for system compatibility
            LPT2,
            LPT3
            };

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - Device                                       |
+===========================================================================*/
class CDevice : public CObject
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNAMIC(CDevice)
public:
    CDevice();
    virtual ~CDevice();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	virtual	int		Create(void* pDevice)	= 0;
    virtual WORD    Open() = 0;
    virtual WORD    Close() = 0;
    virtual WORD    Read (BYTE* pch, int nCount,       int* pnReadCount) = 0;
    virtual WORD    Read (BYTE* pch, BYTE cTerminator, int* pnReadCount) = 0;
    virtual WORD    Write(BYTE  ch) = 0;
    virtual WORD    Write(BYTE* pch, int nCount,       int* pnWriteCount)= 0;
    virtual WORD    ReadToken (BYTE* pch, int nBufferSize, int* pnReadCount) = 0;
    virtual WORD    WriteToken(BYTE* pch, int nCount,      int* pnWriteCount)= 0;
    
    virtual WORD    GetRxState(WORD* pwState) = 0;
    virtual WORD    GetTxState(WORD* pwState) = 0;

    virtual WORD    AbortRx() = 0;
    virtual WORD    AbortTx() = 0;

    virtual WORD    GetOpStatus()= 0;

    virtual void    SetRTimeOut(WORD nRTimeOut) = 0;
    virtual void    SetWTimeOut(WORD nWTimeOut) = 0;
};

/*===========================================================================+
|           External                                                         |
+===========================================================================*/

#endif
