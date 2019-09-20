/*
*	Created by J.Wong 2016/7/27 14:56:06
* 	version: v1.0
*/
#ifndef __MODBUSCLIENT_H
#define __MODBUSCLIENT_H

#include "mb.h"
#include "task.h"
#include "tmpackclass.h"

class CTaskModbusClient:public CTask,public CtmPackClass{
	
	DECLARE_DYNCREATE(CTaskModbusClient)
	public:
		CTaskModbusClient();
		CTaskModbusClient(const modbus_rtu_t& rtu);
		CTaskModbusClient(const modbus_tcp_t& tcp);
		virtual ~CTaskModbusClient();
	public:
		bool	StartThreadWork();
		bool	StopThreadWork();
		bool	IsOpen();
		void	Lock();
		void	UnLock();
		void 	ReadyModbus(const modbus_rtu_t& rtu);
		void 	ReadyModbus(const modbus_tcp_t& tcp,bool mix=false);
		
		int 	ModbusConnect();
		
		const 	modbus_communication_mode& GetModbusMode();
		
		/*===============User Interface =========================*/
		void	Read(uint8_t slave, uint16_t func, uint16_t start_addr,uint16_t nr);
		void	Write(uint8_t slave, uint16_t func, uint16_t start_addr, void* src,  uint16_t nr);
		void	ReadWrite(uint8_t slave, uint16_t func,int write_addr,void* src,int write_nr,int read_addr,int read_nr);
		void	SetExternalTask(void (*pTask)(uint8_t slave, uint16_t func, uint16_t start_addr, void* rsp,int nr));	
		int		SetUpdateIntervalMs(int timer);	
			
		const char* 	ModbusStrErr(int errnum);
		int				DebugModbus(int flag);
		int 			FlushModbus();
		
		/*
			modbus_mapping_t* MallocModbusMapping(int nb_bits, int nb_input_bits,
                          int nb_registers, int nb_input_registers);
         */
		void 	FreeModbusMapping(modbus_mapping_t *mb_mapping);
		
        modbus_mapping_t* MallocModbusMappingStartAddress(
    	uint32_t start_bits,uint32_t nb_bits,uint32_t start_input_bits,uint32_t nb_input_bits,
    	uint32_t start_registers,uint32_t nb_registers,uint32_t start_input_registers,
    	uint32_t nb_input_registers);
    	
		int 	GetModbusTimeoutOfByte(uint32_t *to_sec, uint32_t *to_usec);
		
		int 	GetModbusLengthOfHeader();
		int 	GetModbusTimeoutOfResponse(uint32_t *to_sec, uint32_t *to_usec);
		int 	GetModbusSocket();
		
		int 	ReceiveModbusConfirm(uint8_t *rsp);
		int 	ReportModbusSlaveID(int max_dest, uint8_t *dest);
		int 	SendModbusRequestByRaw(uint8_t *raw_req, int raw_req_length);
		int 	SetModbusTimeoutOfByte(uint32_t to_sec, uint32_t to_usec);
		int 	SetModbusErrRecovery(modbus_error_recovery_mode error_recovery);
		int 	SetModbusTimeoutOfResponse(uint32_t to_sec, uint32_t to_usec);
	protected:
		/*Subclass must override this method for processing data*/
		virtual void Task(uint8_t slave, uint16_t func, uint16_t start_addr, void* rsp,int nr);

	/* system callback, not allowed use in external*/

	public:
		int 	SetModbusSlave(int slave);
		void 	FreeModbus();
		void 	CloseModbus();
		
		int 	ReadModbusCoils(int addr, int nb, uint16_t *dest);
		int 	ReadModbusInputDiscrete(int addr, int nb, uint16_t* dest);
		int 	ReadModbusInputRegisters(int addr, int nb, uint16_t *dest);
		int 	ReadModbusHoldingRegisters(int addr, int nb, uint16_t *dest);
		
		int 	WriteModbusCoil(int addr, int nb,uint16_t* status);
		int 	WriteModbusCoils(int addr, int nb,  uint16_t *src);
		int 	WriteModbusHoldingRegister(int addr, int nb,uint16_t* value);
		int 	WriteModbusHoldingRegisters(int addr, int nb,  uint16_t *src);
		
		int 	WriteModbusRegisterByMask(int addr, uint16_t and_mask, uint16_t or_mask);
		int 	WriteReadModbusRegisters(int write_addr, int write_nb,const uint16_t *src,
                                    int read_addr, int read_nb,uint16_t *dest);

	private:
		static 	void * 	Update(void* );
		class CtmPrivateData;
		CtmPrivateData* m_data;
		private:   
		CTaskModbusClient(const CTaskModbusClient&);   
		CTaskModbusClient& operator=(const CTaskModbusClient&);

};

extern CTaskModbusClient* g_ptaskmbclient;

#endif
