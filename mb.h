/*
*	Created by J.Wong 2016/7/27 14:58:13
*	version: v1.0
*/
#ifndef __MB_H
#define __MB_H
#include<modbus/modbus.h>

typedef enum{
		MODBUS_NONE=0x00,
		MODBUS_RTU=0x01,
		MODBUS_TCP=0x02,
		MODBUS_MIX=0x03
}modbus_communication_mode;

typedef struct{
	uint8_t 	port;
	uint32_t	buadrate;
	uint8_t		parity;	/*'N'-none 'E'-even 'O'-odd */
	uint8_t		databits;
	uint8_t		stopbits;
	
	
	
}__attribute__((packed, aligned(2))) modbus_rtu_t;

typedef struct{
	char* ip;
	uint32_t port;
	uint32_t src_port;
	
}__attribute__((packed, aligned(2))) modbus_tcp_t;
#endif