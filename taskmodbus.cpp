/*
*	Created by J.Wong
*	2016/2/25 14:59:59
*/
#include "taskmodbus.h"
#include    "utils.h"
#include "package.h"

#include 	<map>
#include 	<string>
using namespace __gnu_cxx;

struct str_hash {     //hash函数
	size_t operator()(const string& str) const
	{
		unsigned long __h = 0;
		for (size_t i = 0 ; i < str.size() ; i ++)
		{
			__h = 107 * __h + str[i];
		}
		return size_t(__h);
	}
};
struct str_equal {     //string 判断相等函数
	bool operator()(const string& s1, const string& s2) const
	{
		return s1 == s2;
	}
};

IMPLEMENT_DYNCREATE(CTaskModbus, CtmPackClass)

CTaskModbus*		g_pTaskModbus = NULL;
BOOL				g_bModbusLineState = FALSE;

static hash_map<string, CTaskModbus*, str_hash, str_equal> u_instances;

typedef	hash_map<string, CTaskModbus*, str_hash, str_equal>::iterator	instance_map_t;

class CTaskModbus::CtmPrivateData {
public:
	pthread_mutex_t 	m_lock;

public:
	CtmPrivateData() {

	}
};

CTaskModbus::CTaskModbus(): CTask(), CtmPackClass()
{
	m_data = new CtmPrivateData();
	pthread_mutex_init(&m_data->m_lock, 0);
}

CTaskModbus::~CTaskModbus()
{
	if (m_data)
	{
		pthread_mutex_destroy(&m_data->m_lock);
	}
	delete m_data;
}

int		CTaskModbus::CreateSelf()
{
	return 0;
}

int		CTaskModbus::FreeSelf()
{
	return 0;
}

/*read and write by configuration database */
void	CTaskModbus::Read(char* 	start_id,	int nr)
{

}

void 	CTaskModbus::Write(char* start_id, void* src, int nr)
{

}

void	CTaskModbus::ReadWrite(char* write_start_id, void* src, int wr_nr, char* read_start_id, int rd_nr)
{

}

void	CTaskModbus::Read(char* 	read_id)
{

}

void 	CTaskModbus::Write(char* write_id, WORD wFunc)
{

}

void	CTaskModbus::ReadWrite(char* write_id, char* read_id)
{

}

/*read and write by registers*/
void	CTaskModbus::Read(int slave, int func, int start_addr,	int nr)
{

}

void	CTaskModbus::Write(int slave, int func, int start_addr, void* src,  int nr)
{

}

void	CTaskModbus::ReadWrite(int slave, int func, int write_addr, void* src, int write_nr, int read_addr, int read_nr)
{

}

int		CTaskModbus::GetData(int slave, int start_addr, void* dest, int nr)
{
	return 0;
}

int		CTaskModbus::Lock()
{
	return pthread_mutex_lock(&m_data->m_lock);
}

int 	CTaskModbus::UnLock()
{
	return pthread_mutex_unlock(&m_data->m_lock);
}

void*	CTaskModbus::Merbromin(void* param)
{
	return 0;
}

CTaskModbus* 	CTaskModbus::GetInstance(char* cls)
{
	instance_map_t instance = u_instances.find(string(cls));
	if (instance != u_instances.end())
	{
		return instance->second;
	}
	return 0;
}

int		CTaskModbus::CreateInstance(char* cls, char* lib)
{
	instance_map_t instance = u_instances.find(string(cls));
	if (instance == u_instances.end())
	{
		if (cls)
		{
			CTaskModbus* element = static_cast<CTaskModbus*>(g_pPackage->CreateClassInstance(lib));
			if (!element) return -1;
			u_instances.insert(pair<string, CTaskModbus*>(string(cls), element));
			return 0;
		}
		return -1;
	}
	return -1;
}

