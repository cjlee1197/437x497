/*
* 	Created by J.Wong 2016/7/27 14:56:21
* 	version:v1.0
*/
#include "mbclient.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <queue>

CTaskModbusClient* g_ptaskmbclient;
const int	CACHE_SIZE = 500;

typedef struct {
	uint8_t slave;
	uint16_t func;
	uint16_t rd_start_addr;
	int16_t  rd_nr;
	uint16_t wr_start_addr;
	uint8_t	 wr_data[MODBUS_MAX_PDU_LENGTH];
	int16_t  wr_nr;

} __attribute__((packed, aligned(2))) modbus_private_data_t;

typedef struct {
	int code;
	int len;/*sizeof(type)*/
} __attribute__((packed, aligned(2))) domain_len_t;

typedef struct {
	int code;
	int (CTaskModbusClient::*handler)(int addr, int nb, uint16_t *data);
} __attribute__((packed, aligned(2)))callback_domain_t;

const domain_len_t u_read_domain[] = {
	{MODBUS_FC_READ_COILS, 1},
	{MODBUS_FC_READ_DISCRETE_INPUTS, 1},
	{MODBUS_FC_READ_HOLDING_REGISTERS, 2},
	{MODBUS_FC_READ_INPUT_REGISTERS, 2},
	{MODBUS_FC_READ_EXCEPTION_STATUS, 0},
	{MODBUS_FC_REPORT_SLAVE_ID, 0}
};

const domain_len_t u_write_domain[] = {
	{MODBUS_FC_WRITE_SINGLE_COIL, 1},
	{MODBUS_FC_WRITE_SINGLE_REGISTER, 2},
	{MODBUS_FC_WRITE_MULTIPLE_COILS, 1},
	{MODBUS_FC_WRITE_MULTIPLE_REGISTERS, 2},
	{MODBUS_FC_MASK_WRITE_REGISTER, 2}
};

const domain_len_t u_write_read_domain[] = {
	{MODBUS_FC_WRITE_AND_READ_REGISTERS, 2}
};

const callback_domain_t u_callback_domain[] = {
	{MODBUS_FC_READ_COILS 				, &CTaskModbusClient::ReadModbusCoils},
	{MODBUS_FC_READ_DISCRETE_INPUTS 	, &CTaskModbusClient::ReadModbusInputDiscrete},
	{MODBUS_FC_READ_HOLDING_REGISTERS	, &CTaskModbusClient::ReadModbusHoldingRegisters},
	{MODBUS_FC_READ_INPUT_REGISTERS		, &CTaskModbusClient::ReadModbusInputRegisters},
	{MODBUS_FC_WRITE_SINGLE_COIL		, &CTaskModbusClient::WriteModbusCoil},
	{MODBUS_FC_WRITE_SINGLE_REGISTER	, &CTaskModbusClient::WriteModbusHoldingRegister},
	{MODBUS_FC_READ_EXCEPTION_STATUS	, NULL},
	{MODBUS_FC_WRITE_MULTIPLE_COILS		, &CTaskModbusClient::WriteModbusCoils},
	{MODBUS_FC_WRITE_MULTIPLE_REGISTERS	, &CTaskModbusClient::WriteModbusHoldingRegisters},
	{MODBUS_FC_REPORT_SLAVE_ID			, NULL},
};

/*MODBUS_FC_READ_COILS                0x01
MODBUS_FC_READ_DISCRETE_INPUTS      0x02
MODBUS_FC_READ_HOLDING_REGISTERS    0x03
MODBUS_FC_READ_INPUT_REGISTERS      0x04
MODBUS_FC_WRITE_SINGLE_COIL         0x05
MODBUS_FC_WRITE_SINGLE_REGISTER     0x06
MODBUS_FC_READ_EXCEPTION_STATUS     0x07
MODBUS_FC_WRITE_MULTIPLE_COILS      0x0F
MODBUS_FC_WRITE_MULTIPLE_REGISTERS  0x10
MODBUS_FC_REPORT_SLAVE_ID           0x11
MODBUS_FC_MASK_WRITE_REGISTER       0x16
MODBUS_FC_WRITE_AND_READ_REGISTERS  0x17
*/

typedef enum {
	MODBUS_READ = 0x00,
	MODBUS_WRITE = 0x01,
	MODBUS_RDWR = 0x02,
} rd_wr_mode_t;

/*================static methods=====================*/
static int check_domain(int code, rd_wr_mode_t mode)
{
	int ret = 0;
	switch (mode)
	{
	case MODBUS_READ:
		for (int i = 0; i < sizeof(u_read_domain) / sizeof(domain_len_t); i++)
		{
			if (u_read_domain[i].code == code)
			{
				ret = u_read_domain[i].len;
				break;
			}
		}
		break;
	case MODBUS_WRITE:
		for (int i = 0; i < sizeof(u_write_domain) / sizeof(domain_len_t); i++)
		{
			if (u_write_domain[i].code == code)
			{
				ret = u_write_domain[i].len;
				break;
			}
		}
		break;
	case MODBUS_RDWR:
		for (int i = 0; i < sizeof(u_write_read_domain) / sizeof(domain_len_t); i++)
		{
			if (u_write_read_domain[i].code == code)
			{
				ret = u_write_read_domain[i].len;
				break;
			}
		}
		break;
	default:
		break;
	}

	return ret;
}

const		int 	MIN_TIME_MS		= 10;
IMPLEMENT_DYNCREATE(CTaskModbusClient, CtmPackClass)
typedef void * (* PRSRUN)(void *);

typedef void (*pTask)(uint8_t slave, uint16_t func, uint16_t start_addr, void* rsp, int nr);
class CTaskModbusClient::CtmPrivateData {
public:
	/*member-variants*/
	modbus_t *m_ctx;
	modbus_communication_mode m_mode;
	pthread_mutex_t			  m_lock;
	pthread_t 				  m_thread;
	pthread_attr_t  		  m_thread_attr;
	queue<modbus_private_data_t> m_data_cache;
	pTask					  m_task;
	int						  m_timer;
	int						  m_debug_flag;
public:
	CtmPrivateData(): m_ctx(0), m_mode(MODBUS_NONE), m_task(0), m_timer(200), m_debug_flag(0) {
	};
};

CTaskModbusClient::CTaskModbusClient()
{
	m_data = new CtmPrivateData();
	pthread_mutex_init(&m_data->m_lock, 0);
}

CTaskModbusClient::CTaskModbusClient(const modbus_rtu_t& rtu)
{
	m_data = new CtmPrivateData();
	pthread_mutex_init(&m_data->m_lock, 0);
	ReadyModbus(rtu);
}

CTaskModbusClient::CTaskModbusClient(const modbus_tcp_t& tcp)
{
	m_data = new CtmPrivateData();
	pthread_mutex_init(&m_data->m_lock, 0);
	ReadyModbus(tcp);
}

CTaskModbusClient::~CTaskModbusClient()
{
	pthread_cancel(m_data->m_thread);
	if (m_data->m_ctx)
	{
		modbus_close(m_data->m_ctx);
		modbus_free(m_data->m_ctx);
		pthread_mutex_destroy(&m_data->m_lock);
	}
	delete m_data;
}

void CTaskModbusClient::ReadyModbus(const modbus_rtu_t& rtu)
{
	if (m_data->m_ctx)
	{
		modbus_close(m_data->m_ctx);
		modbus_free(m_data->m_ctx);
	}
	char szDevice[32];
#ifdef D_3354
	strcpy(szDevice, "/dev/ttyO");
	sprintf(szDevice, "%s%d", szDevice, rtu.port);
#else
	strcpy(szDevice, "/dev/ttyS");
	sprintf(szDevice, "%s%d", szDevice, rtu.port);
#endif
	m_data->m_ctx = modbus_new_rtu(szDevice, rtu.buadrate, rtu.parity, rtu.databits, rtu.stopbits);
	if (m_data->m_ctx)
	{
		m_data->m_mode = MODBUS_RTU;
	}
	else
	{
		fprintf(stderr, "Unable to create the libmodbus context\n");
	}
}

void CTaskModbusClient::ReadyModbus(const modbus_tcp_t& tcp, bool mix)
{
	if (mix)
	{
		m_data->m_ctx = modbus_new_rtu_by_tcp_ex(tcp.ip, tcp.port, tcp.src_port);

		if (m_data->m_ctx)
		{
			m_data->m_mode = MODBUS_MIX;
		}
	}
	else
	{
		m_data->m_ctx = modbus_new_tcp_ex(tcp.ip, tcp.port, tcp.src_port);
		if (m_data->m_ctx)
		{
			m_data->m_mode = MODBUS_TCP;
		}
	}
}

const modbus_communication_mode& CTaskModbusClient::GetModbusMode()
{
	return m_data->m_mode;
}

const char* CTaskModbusClient::ModbusStrErr(int errnum)
{
	return modbus_strerror(errnum);
}

int		CTaskModbusClient::DebugModbus(int flag)
{
	m_data->m_debug_flag = flag;
	return modbus_set_debug(m_data->m_ctx, flag);
}

int 	CTaskModbusClient::SetModbusSlave(int slave)
{
	return modbus_set_slave(m_data->m_ctx, slave);
}

int 	CTaskModbusClient::ModbusConnect()
{
	return modbus_connect(m_data->m_ctx);
}
void 	CTaskModbusClient::FreeModbus()
{
	return modbus_free(m_data->m_ctx);
}
void 	CTaskModbusClient::CloseModbus()
{
	return modbus_close(m_data->m_ctx);
}
int 	CTaskModbusClient::FlushModbus()
{
	return modbus_flush(m_data->m_ctx);
}

int 	CTaskModbusClient::GetModbusTimeoutOfByte(uint32_t *to_sec, uint32_t* to_usec)
{
	return modbus_get_byte_timeout(m_data->m_ctx, to_sec, to_usec);
}

int 	CTaskModbusClient::GetModbusLengthOfHeader()
{
	return  modbus_get_header_length(m_data->m_ctx);
}

int  	CTaskModbusClient::GetModbusTimeoutOfResponse(uint32_t *to_sec, uint32_t *to_usec)
{
	return  modbus_get_response_timeout(m_data->m_ctx, to_sec, to_usec);
}

int 	CTaskModbusClient::GetModbusSocket()
{
	return modbus_get_socket(m_data->m_ctx);
}

/*
modbus_mapping_t* CTaskModbusClient::MallocModbusMapping(int nb_bits, int nb_input_bits,
                          int nb_registers, int nb_input_registers)
{
		return modbus_mapping_new(nb_bits,nb_input_bits,
							nb_registers,nb_input_registers);
}
*/

void 	CTaskModbusClient::FreeModbusMapping(modbus_mapping_t *mb_mapping)
{
	return modbus_mapping_free(mb_mapping);
}

modbus_mapping_t* CTaskModbusClient::MallocModbusMappingStartAddress(
    uint32_t start_bits, uint32_t nb_bits, uint32_t start_input_bits, uint32_t nb_input_bits,
    uint32_t start_registers, uint32_t nb_registers, uint32_t start_input_registers,
    uint32_t nb_input_registers)
{
	return modbus_mapping_new_start_address(start_bits, nb_bits, start_input_bits, nb_input_bits,
	                                        start_registers, nb_registers, start_input_registers, nb_input_registers);
}

int 	CTaskModbusClient::WriteModbusRegisterByMask(int addr, uint16_t and_mask, uint16_t or_mask)
{
	return modbus_mask_write_register(m_data->m_ctx, addr, and_mask, or_mask);
}

int 	CTaskModbusClient::ReadModbusCoils(int addr, int nb, uint16_t *dest)
{
	return modbus_read_bits(m_data->m_ctx, addr, nb, (uint8_t*)dest);
}

int 	CTaskModbusClient::ReadModbusInputDiscrete(int addr, int nb, uint16_t* dest)
{
	return modbus_read_input_bits(m_data->m_ctx, addr, nb, (uint8_t*)dest);
}
int 	CTaskModbusClient::ReadModbusInputRegisters(int addr, int nb, uint16_t *dest)
{
	return modbus_read_input_registers(m_data->m_ctx, addr, nb, dest);
}

int 	CTaskModbusClient::ReadModbusHoldingRegisters( int addr, int nb, uint16_t *dest)
{
	return modbus_read_registers(m_data->m_ctx, addr, nb, dest);
}

int 	CTaskModbusClient::ReceiveModbusConfirm(uint8_t *rsp)
{
	return modbus_receive_confirmation(m_data->m_ctx, rsp);
}

int 	CTaskModbusClient::ReportModbusSlaveID(int max_dest, uint8_t *dest)
{
	return modbus_report_slave_id(m_data->m_ctx, max_dest, dest);
}

int 	CTaskModbusClient::SendModbusRequestByRaw( uint8_t *raw_req, int raw_req_length)
{
	return modbus_send_raw_request(m_data->m_ctx, raw_req, raw_req_length);
}

int 	CTaskModbusClient::SetModbusTimeoutOfByte( uint32_t to_sec, uint32_t to_usec)
{
	return modbus_set_byte_timeout(m_data->m_ctx, to_sec, to_usec);
}

int 	CTaskModbusClient::SetModbusErrRecovery(modbus_error_recovery_mode error_recovery)
{
	return modbus_set_error_recovery(m_data->m_ctx, error_recovery);
}

int 	CTaskModbusClient::SetModbusTimeoutOfResponse(uint32_t to_sec, uint32_t to_usec)
{
	return modbus_set_response_timeout(m_data->m_ctx, to_sec, to_usec);
}

int 	CTaskModbusClient::WriteReadModbusRegisters(int write_addr, int write_nb, const uint16_t *src,
        int read_addr, int read_nb, uint16_t *dest)
{
	return  modbus_write_and_read_registers(m_data->m_ctx, write_addr, write_nb, src,
	                                        read_addr, read_nb, dest);
}

int 	CTaskModbusClient::WriteModbusCoil(int addr, int nb, uint16_t* status)
{
	return modbus_write_bit(m_data->m_ctx, addr, *status);
}

int 	CTaskModbusClient::WriteModbusCoils(int addr, int nb, uint16_t *src)
{
	return modbus_write_bits(m_data->m_ctx, addr, nb, (uint8_t*)src);
}

int 	CTaskModbusClient::WriteModbusHoldingRegister(int addr, int nb, uint16_t* value)
{
	return modbus_write_register(m_data->m_ctx, addr, *value);
}

int 	CTaskModbusClient::WriteModbusHoldingRegisters(int addr, int nb,  uint16_t *src)
{
	return modbus_write_registers(m_data->m_ctx, addr, nb, src);
}

void	CTaskModbusClient::Lock()
{
	pthread_mutex_lock(&m_data->m_lock);
}
void	CTaskModbusClient::UnLock()
{
	pthread_mutex_unlock(&m_data->m_lock);
}

bool		CTaskModbusClient::IsOpen()
{
	return m_data->m_ctx ? (modbus_get_socket(m_data->m_ctx) >= 0 ? 1 : 0) : 0;
}

void* 	CTaskModbusClient::Update(void* pSelf)
{
	CTaskModbusClient* client = (CTaskModbusClient*)pSelf;
	pthread_detach(pthread_self());
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("Update setcanceltype");
	}
	do
	{
		if (!client->m_data) break;
		if (client->IsOpen())
		{
			client->Lock();
			if (!client->m_data->m_data_cache.empty())
			{
				modbus_private_data_t item = client->m_data->m_data_cache.front();
				client->m_data->m_data_cache.pop();
				client->UnLock();
				uint8_t reg[MODBUS_MAX_PDU_LENGTH] = {0};
				//if(client->m_data->m_mode==MODBUS_RTU)
				client->SetModbusSlave(item.slave);
				int nr = -1;
				for (int i = 0; i < sizeof(u_callback_domain) / sizeof(callback_domain_t); i++)
				{
					if (u_callback_domain[i].code == item.func)
					{
						if (u_callback_domain[i].handler == NULL)	break;
						if (item.rd_nr > 0)
							nr = (client->*(u_callback_domain[i].handler))(item.rd_start_addr, item.rd_nr, (uint16_t*)&reg);
						else if (item.wr_nr > 0)
							nr = (client->*(u_callback_domain[i].handler))(item.wr_start_addr, item.wr_nr, (uint16_t*)item.wr_data);
						if (client->m_data->m_task)
						{
							client->m_data->m_task(item.slave, item.func, item.rd_start_addr, reg, nr);
						}
						else
							client->Task(item.slave, item.func, item.rd_start_addr, reg, nr);
						break;
					}
				}
				if (item.func == MODBUS_FC_MASK_WRITE_REGISTER)
				{
					if (item.wr_nr >= 2)
					{
						nr = client->WriteModbusRegisterByMask(item.wr_start_addr, *(uint16_t*)&item.wr_data[0], *(uint16_t*)&item.wr_data[2]);
						if (client->m_data->m_task)
						{
							client->m_data->m_task(item.slave, item.func, item.rd_start_addr, reg, nr);
						}
						else
							client->Task(item.slave, item.func, item.rd_start_addr, reg, nr);
					}
				}
				else if (item.func == MODBUS_FC_WRITE_AND_READ_REGISTERS)
				{
					nr = client->WriteReadModbusRegisters(item.wr_start_addr, item.wr_nr, (uint16_t*)item.wr_data,
					                                      item.rd_start_addr, item.rd_nr, (uint16_t*)&reg);
					if (client->m_data->m_task)
					{
						client->m_data->m_task(item.slave, item.func, item.rd_start_addr, reg, nr);
					}
					else
						client->Task(item.slave, item.func, item.rd_start_addr, reg, nr);
				}
			}
			else
			{
				client->UnLock();
			}
			usleep(client->m_data->m_timer * 1000);
		}
		else
		{
			usleep(500 * 1000);
		}
	} while (1);
}

bool	CTaskModbusClient::StartThreadWork()
{
	pthread_attr_init(&m_data->m_thread_attr);
	pthread_attr_setdetachstate(&m_data->m_thread_attr, PTHREAD_CREATE_DETACHED);
	return pthread_create(&m_data->m_thread, NULL, (PRSRUN)Update, this);
}

bool CTaskModbusClient::StopThreadWork()
{
	return pthread_cancel(m_data->m_thread);
}

void CTaskModbusClient::Task(uint8_t slave, uint16_t func, uint16_t start_addr, void* rsp, int nr)
{
	if (m_data->m_debug_flag)
		if (nr > 0)
			printf("slave=%d,func=%d, start_adr=%d,nr=%d\n", slave, func, start_addr, nr);
}

void	CTaskModbusClient::Read(uint8_t slave, uint16_t func, uint16_t start_addr, uint16_t nr)
{
	Lock();
	if ((check_domain(func, MODBUS_READ)))
	{
		modbus_private_data_t item;
		item.slave = slave;
		item.func = func;
		item.rd_start_addr = start_addr;
		item.rd_nr = nr;
		item.wr_nr = -1;
		if (m_data->m_data_cache.size() <= CACHE_SIZE)
		{
			m_data->m_data_cache.push(item);
		}
		else
		{
			m_data->m_data_cache.pop();
			m_data->m_data_cache.push(item);
			if (m_data->m_debug_flag)
				printf("modbus cache crash, auto pop cache and push last data....\n");
		}
	}
	UnLock();
}

void	CTaskModbusClient::Write(uint8_t slave, uint16_t func, uint16_t start_addr, void* src, uint16_t nr)
{
	Lock();
	int len = 0;
	if ((len = check_domain(func, MODBUS_WRITE)))
	{
		modbus_private_data_t item;
		item.slave = slave;
		item.func = func;
		item.rd_nr = -1;
		item.wr_start_addr = start_addr;
		memcpy(item.wr_data, src, nr * len);
		item.wr_nr = nr;
		if (m_data->m_data_cache.size() <= CACHE_SIZE)
			m_data->m_data_cache.push(item);
		else
		{
			m_data->m_data_cache.pop();
			m_data->m_data_cache.push(item);
			if (m_data->m_debug_flag)
				printf("modbus cache crash, auto pop cache and push last data....\n");
		}
	}
	UnLock();
}

void	CTaskModbusClient::ReadWrite(uint8_t slave, uint16_t func, int write_addr, void* src, int write_nr, int read_addr, int read_nr)
{
	Lock();
	int len = 0;
	if ((len = check_domain(func, MODBUS_RDWR)))
	{
		modbus_private_data_t item;
		item.slave = slave;
		item.func = func;
		item.rd_start_addr = read_addr;
		item.rd_nr = read_nr;
		item.wr_start_addr = write_addr;
		memcpy(item.wr_data, src, write_nr * len);
		item.wr_nr = write_nr;
		if (m_data->m_data_cache.size() <= CACHE_SIZE)
			m_data->m_data_cache.push(item);
		else
		{
			m_data->m_data_cache.pop();
			m_data->m_data_cache.push(item);
			if (m_data->m_debug_flag)
				printf("modbus cache crash, auto pop cache and push last data....\n");
		}
	}
	UnLock();
}

void	CTaskModbusClient::SetExternalTask(pTask task)
{
	Lock();
	m_data->m_task = task;
	UnLock();
}

int		CTaskModbusClient::SetUpdateIntervalMs(int timer)
{
	if (timer < MIN_TIME_MS)	timer = MIN_TIME_MS;
	m_data->m_timer = timer;
	return timer;
}

