/**
*	CanOpen Master 
*	Author: 
*		
*/
#include	"common.h"
#ifndef     D__CANOPENMASTER
#define     D__CANOPENMASTER

#define		CAN_COMMAND_TEST_AUTO				1
#define		CAN_COMMAND_TEST_NORMALL			2

#define		CAN_GET_SEND_NO_COUNT				1
#define		CAN_GET_HEARTBEAT_ERROR_COUNT		2
#define		CAN_GET_SLAVE_STATES				3
#define		CAN_GET_RECV_NO_COUNT				4

#define		MAX_CANDATALEN						8

#define		MAX_KEYINDEX						4
#define		MAX_LEDINDEX						2

typedef 	struct tagOBJMAPPING
{
	WORD		wNum;
	WORD		wDBSelect[8];
	short int	nOffset[8];
	WORD		wByteOffset[8];
	WORD		wSize[8];	
}OBJMAPPING;

typedef		struct	tagPDOMAPAddr
{
	WORD   	wNum;
	WORD	wSize[8];
	char*	pAddr[8];	
}PDOMAPADDR;

int	 		TaskCanMaster();
void 		TaskCanMasterClose();
int	 		CreatSocketTask();
void		CanConfigTask();

void		ReSetConfig();
void 		SetCommand(int nCommandId);
int 		GetCommand();
char 		GetRecvData(int nIndex);
void 		SetSendData(int nIndex, char data);
int			GetErrorCount(int nType);
int			Can_Write(struct can_frame* frame, int nsize);

//int			SetPDOMapAddr(WORD	wIndex, WORD wSubIndex, char*	pAddr,	int nLen);		//For Rx
//int			PDO_Write(WORD	wIndex, WORD wSubIndex, char*	pAddr, int nLen);			//For Tx

void	processTransmitSDOCInitDomainDownload(WORD wNodeID, WORD wIndex, BYTE cSubIndex, DWORD dwValue, WORD wSize);
void	processTransmitSDOCInitDomainUpload(WORD wNodeID,  WORD wIndex, BYTE cSubIndex);
void	processTransmitPDO(WORD wNodeID, WORD wPDONum);
void	processTransmitNMT(WORD wNodeID, WORD wNMTCS);
int		Can_Send();
void	CANDataCheck();


extern		char	u_acOutPutData_Old[MAX_CANDATALEN];
extern		DWORD	g_dwM3Key[MAX_KEYINDEX];
extern		DWORD	g_dwM3LED[MAX_LEDINDEX];
#endif
