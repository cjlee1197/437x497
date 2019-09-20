/*
*	Created by J.Wong
*	2016/2/25 10:09:06
* 	Function: modbus-rtu service interface
*/

/*
*	type definition 
*/
#ifndef 	D_TASKMODBUS_H
#define		D_TASKMODBUS_H

#include    "task.h"
#include	"tmpackclass.h"
#include 	<queue>

#define		MODBUS_MAX_QUEUE_DATA_COUNT			10
#define		MODBUS_RTU_FRAME_MAXLEN				256
#define		MODBUS_QUEUE_FULL					1
#define		MODBUS_QUEUE_EMPTY					2

typedef	struct  tagModbusData{
	WORD	wBytes;
	BYTE	cBuffer[MODBUS_RTU_FRAME_MAXLEN];
}MODBUSDATA;

typedef	struct	tagRS485Conf
{
	struct termios 	newtermios;
	int				nBaudRate;
	WORD			wChannel;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	RS485CONF;
#else 
#ifdef	D_X86
RS485CONF;
#endif
#endif

class CTaskModbus:public CTask,public CtmPackClass
{
	DECLARE_DYNCREATE(CTaskModbus)
	public:
		CTaskModbus();
		virtual ~CTaskModbus();
		
		int			CreateSelf();
		int			FreeSelf();
	public:
		/*read and write by configuration database */
		virtual 	void	Read(char* 	read_id);
		virtual		void 	Write(char* write_id, WORD wFunc =0);
		virtual		void	ReadWrite(char* write_id, char* read_id);
		virtual 	void	Read(char* 	start_id,	int nr);
		virtual		void 	Write(char* start_id, void* src, int nr=1);
		virtual		void	ReadWrite(char* write_start_id, void* src, int wr_nr, char* read_start_id, int rd_nr);
		/*read and write by registers*/	
		virtual		void	Read(int slave, int func, int start_addr,	int nr);
		virtual		void	Write(int slave, int func, int start_addr, void* src,  int nr);
		virtual		void	ReadWrite(int slave, int func,int write_addr,void* src,int write_nr,int read_addr,int read_nr);
		
		virtual		int		GetData(int slave,int start_addr,void* dest, int nr);
	
		virtual 	void*	Merbromin(void* param);
		
		static 		CTaskModbus* 	GetInstance(char* cls);
		static		int				CreateInstance(char* cls, char* lib);
	protected:
		int			Lock();
		int 		UnLock();
	private:
		class 	CtmPrivateData;
		CtmPrivateData*	m_data;
};

extern 	CTaskModbus*		g_pTaskModbus;
extern	BOOL 				g_bModbusLineState;

#endif
