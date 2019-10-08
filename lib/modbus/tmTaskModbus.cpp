/*
*	Created by J.Wong
*	2016/2/25 15:17:42
*/
#include 	"tmTaskModbus.h"
#include	"../../utils.h"
#include 	"../../readdata.h"
#include 	"../../mbclient.h"
#include	<queue>
#include 	<map>
#include 	<string>
#include <iostream>
#include <fstream>

using namespace __gnu_cxx;

struct str_hash{      //hash函数
    size_t operator()(const string& str) const
    {
        unsigned long __h = 0;
        for (size_t i = 0 ; i < str.size() ; i ++)
        {
            __h = 107*__h + str[i];
        }
        return size_t(__h);
    }
};


struct str_equal{      //string 判断相等函数
	bool operator()(const string& s1,const string& s2) const
	{
     	return s1==s2;
	}
};

struct cmp_key 
{
	bool operator()(const modbus_data_mapping_t a, const modbus_data_mapping_t b) {
		if (a.slave==b.slave)
		{
			if(a.reg==b.reg)
			{
				return a.count<b.count;	
			}
			return a.reg<b.reg;
		}
		return a.slave < b.slave;
	}
};


typedef struct{
	int code;
	int len;/*sizeof(type)*/
}__attribute__((packed, aligned(2))) domain_len_t;


const domain_len_t u_read_domain[]={
	{MODBUS_READ_COILS_CODE,1},
	{MODBUS_READ_DISCRETE_INPUTS_CODE,1},
	{MODBUS_READ_REGISTERS_CODE,2},
	{MODBUS_READ_INPUT_REGISTERS_CODE,2},
	{MODBUS_READ_EXCEPTION_STATUS_CODE,0},
	{MODBUS_REPORT_SLAVE_ID_CODE,0}
	
};
const domain_len_t u_write_domain[]={
	{MODBUS_WRITE_SINGLE_COIL_CODE,1},
	{MODBUS_WRITE_REGISTER_CODE,2},
	{MODBUS_WRITE_MULTIPLE_COILS_CODE,1},
	{MODBUS_WRITE_REGISTERS_CODE,2},
	{MODBUS_MASK_WRITE_REGISTER_CODE,2}

};
const domain_len_t u_write_read_domain[]={
	{MODBUS_WRITE_AND_READ_REGISERS_CODE,2}
};

typedef enum{
	MODBUS_READ=0x00,
	MODBUS_WRITE=0x01,
	MODBUS_RDWR=0x02,
}rd_wr_mode_t;




/*================static methods=====================*/
static int check_domain(int code,rd_wr_mode_t mode)
{
	int ret=0;
	switch(mode)
	{
		case MODBUS_READ:
			 for(int i=0; i<sizeof(u_read_domain)/sizeof(domain_len_t); i++)
			 {
			 	if(u_read_domain[i].code==code)
			 	{
			 		ret=u_read_domain[i].len;
			 		break;
			 	}	
			 }
			break;
		case MODBUS_WRITE:
			 for(int i=0; i<sizeof(u_write_domain)/sizeof(domain_len_t); i++)
			 {
			 	if(u_write_domain[i].code==code)
			 	{
			 		ret=u_write_domain[i].len;
			 		break;
			 	}	
			 }
			break;
		case MODBUS_RDWR:
			 for(int i=0; i<sizeof(u_write_read_domain)/sizeof(domain_len_t); i++)
			 {
			 	if(u_write_read_domain[i].code==code)
			 	{
			 		ret=u_write_read_domain[i].len;
			 		break;
			 	}	
			 }
			break;
		default:
			break;	
	}
	
	return ret;	
}

static const std::string read_file(const std::string file)
{
	std::string str;
	std::ifstream in(file.c_str());
	int l=in.tellg(); 
 	in.seekg (0, std::ios::end);  
	int m = in.tellg(); 
	int ncount=m-l; 
	in.seekg (0, std::ios::beg); 
	//std::cout<<ncount<<std::endl; 
	char* readAll=new char[ncount+1];
	memset(readAll,0,ncount+1);
	in.read(readAll,ncount);
	str+=readAll;
	
	std::cout<<str<<std::endl;
	
	delete[] readAll;

	in.close();
	return str;
}



typedef map<modbus_data_mapping_t,string,cmp_key>::iterator id_map_t;
typedef	hash_map<string, modbus_data_mapping_t,str_hash,str_equal>::iterator	reg_map_t;	






//#define KEEP_UPDATE_ACTUAL_COUNTER	2000
#define KEEP_UPDATE_ACTUAL_COUNTER	150


IMPLEMENT_DYNCREATE(CtmTaskModbus, CTaskModbus)

class CtmTaskModbus::CtmPrivateData{
 public:
  CTaskModbusClient* m_client;
  queue<modbus_data_mapping_t> m_read_data_cache;
  queue<modbus_data_mapping_t> m_power_on_data_cache;
  hash_map<string, modbus_data_mapping_t,str_hash,str_equal>m_reg_cache;
  map<modbus_data_mapping_t,string,cmp_key>m_id_cache;
  #ifndef DJSON
  xmlDocPtr m_xmlFile;
  #endif
  modbus_communication_mode  m_mode;
  modbus_tcp_t m_tcp;
  modbus_rtu_t m_rtu;
  char m_ip[32];
  long m_update_actual;
  std::vector<int> m_parse_slave;
  
 public:
  CtmPrivateData():m_mode(MODBUS_NONE),m_update_actual(KEEP_UPDATE_ACTUAL_COUNTER),m_parse_slave(1){
   m_client=new CTaskModbusClient();
   m_tcp.ip="192.168.1.211";
   m_tcp.port=m_tcp.src_port=502;
   m_rtu.port=5;
   m_rtu.buadrate=38400;
   m_rtu.parity='E';
   m_rtu.databits=8;
   m_rtu.stopbits=1;
   
   #ifndef DJSON
   m_xmlFile=0;
   #endif
  }; 
};

CtmTaskModbus::CtmTaskModbus():CTaskModbus()
{
	m_data=new CtmPrivateData();
	#ifdef DJSON
		ParseJson("./lib/modbus/config/modbus.json");
	#else
		OpenXmlFile("./lib/modbus/config/modbus.xml");
		ParseXml();
		CloseXmlFile();
	#endif
	if(m_data->m_mode==MODBUS_TCP)
	{
		m_data->m_client->ReadyModbus(m_data->m_tcp);
	}
	else if(m_data->m_mode==MODBUS_RTU)
	{
//		printf("CtmTaskModbus::CtmTaskModbus():CTaskModbus()   MODBUS_RTU\n");
		m_data->m_client->ReadyModbus(m_data->m_rtu);
	}
	if(m_data->m_mode!=MODBUS_NONE)
	{
		m_data->m_client->ModbusConnect();
		m_data->m_client->SetExternalTask(Task);
		m_data->m_client->StartThreadWork();
		m_data->m_client->DebugModbus(FALSE);//test   m_data->m_client->DebugModbus(FALSE);
		//m_data->m_client->SetUpdateIntervalMs(20);/*wangli2016-10-8 10:09:06 10ms 改成20ms */		//m_data->m_client->SetUpdateIntervalMs(20);/*wangli2016-10-8 10:09:06 10ms 改成20ms */
		m_data->m_client->SetUpdateIntervalMs(45);/*wangli2016-10-8 10:09:06 100ms 改成20ms */		//m_data->m_client->SetUpdateIntervalMs(20);/*wangli2016-10-8 10:09:06 10ms 改成20ms */

	uint32_t secs, usecs;
  	m_data->m_client->GetModbusTimeoutOfResponse(&secs,&usecs);
//  std::cout<<"1. secs:"<<secs<<"usecs:"<<usecs<<std::endl;
 	 m_data->m_client->SetModbusTimeoutOfResponse(0,1000*150);//150ms /*wangli2016-10-17 16:13:03 硬体的超时回复时间*/
  
//  tmTIME  time;
//	GetTime(&time);	
//	printf("  %d:%d:%d\n",time.hour, time.minute, time.second);

 	 m_data->m_client->GetModbusTimeoutOfResponse(&secs,&usecs);
//  std::cout<<"2. secs:"<<secs<<"usecs:"<<usecs<<std::endl;
	}
	
}

CtmTaskModbus::~CtmTaskModbus()
{
	if(m_data)
	{
		m_data->m_client->StopThreadWork();
		delete m_data->m_client;	
	}
	delete m_data;
}

int 	CtmTaskModbus::CreateSelf()
{
	CTaskModbus::CreateSelf();

		

	AddCounter((long int*)&m_data->m_update_actual,m_idMe);
	return 0;
}

int 	CtmTaskModbus::FreeSelf()
{
	DeleteCounter((long int*)&m_data->m_update_actual,m_idMe);
	return CTaskModbus::FreeSelf();
}


void	CtmTaskModbus::Run()
{

	{
	if(!m_data->m_client->IsOpen())
	{
		g_bModbusLineState=FALSE;
		//printf("11 g_bModbusLineState=%d\n", g_bModbusLineState);	
		if(m_data->m_mode!=MODBUS_NONE)
			m_data->m_client->ModbusConnect();	
	}
	if(m_data->m_client->IsOpen())
	{
		g_bModbusLineState=TRUE;
		
		//printf("22 g_bModbusLineState=%d\n", g_bModbusLineState);
		//发送开机写资料
	if(m_data->m_update_actual < 0)
		{
		if(!m_data->m_power_on_data_cache.empty())
		{
			modbus_data_mapping_t item=m_data->m_power_on_data_cache.front();
			m_data->m_power_on_data_cache.pop();
			id_map_t id_iterator=m_data->m_id_cache.find(item);
			if(id_iterator!=m_data->m_id_cache.end())
			{
				int nlen=0;
				if((nlen=check_domain(item.func,MODBUS_WRITE)))
				{
					string str=id_iterator->second;
					if(-1!=g_pDatabase->GetIndex((char*)str.c_str()))
					{
						long long lValue=GetDBValue((char*)str.c_str()).lValue;
						m_data->m_client->Write(item.slave,item.func,item.reg,&lValue,item.count/nlen);	
					}
				}
				
			}
		}
		//周期性读资料
		else if(!m_data->m_read_data_cache.empty())
		{
			modbus_data_mapping_t item=m_data->m_read_data_cache.front();
			//modbus_data_mapping_t & item=m_data->m_read_data_cache.front();
			m_data->m_read_data_cache.pop();
			m_data->m_read_data_cache.push(item);
			id_map_t id_iterator=m_data->m_id_cache.find(item);
			if(id_iterator!=m_data->m_id_cache.end())
			{
				int nlen=0;
				if((nlen=check_domain(item.func,MODBUS_READ)))
				{
//					printf("Read item.slave =%x,item.func =%x,item.reg =%x,item.count/nlen =%x\n" ,item.slave,item.func,item.reg,item.count/nlen) ;
					
					if(item.slave < 7) /*wangli2016-10-20 16:19:16 模温机*/
						item.slave = GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED212").lValue;
					else if((item.slave == 11)||(item.slave == 12)) /*wangli2016-10-20 16:19:23 干燥机*/
						item.slave = GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED218").lValue;
						
//					printf("Read item.slave 0000000 =%x\n",item.slave);
					m_data->m_client->Read(item.slave,item.func,item.reg,item.count/nlen);	
				}
			}	
		}
		m_data->m_update_actual = KEEP_UPDATE_ACTUAL_COUNTER;
		}
			
	}
	}
}


/*read and write by configuration database */
void	CtmTaskModbus::Read(char* read_id)
{
	
//	printf("rrrrrrrrrrrrrrrr\n");
		Lock();
		reg_map_t reg_iterator=m_data->m_reg_cache.find(string(read_id));
		if(reg_iterator!=m_data->m_reg_cache.end())
		{
			modbus_data_mapping_t item=reg_iterator->second;
			//modbus_data_mapping_t& item=reg_iterator->second;
				
			int nlen = 0;
			if((nlen=check_domain(item.func,MODBUS_READ)))
			{
					if(item.slave < 7) /*wangli2016-10-20 16:19:16 模温机*/
						item.slave = GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED212").lValue;
					else if((item.slave == 11)||(item.slave == 12)) /*wangli2016-10-20 16:19:23 干燥机*/
						item.slave = GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED218").lValue;

					//printf("ssssssssss Read item.slave =%x,item.func =%x,item.reg =%x,item.count/nlen =%x\n" ,item.slave,item.func,item.reg,item.count/nlen) ;				
					m_data->m_client->Read(item.slave,item.func,item.reg,item.count/nlen);
			}	
		}
		UnLock();	
}

void 	CtmTaskModbus::Write(char* write_id,WORD wFunc)
{
		Lock();
		
//		printf("write_id =%s\n",write_id);
		//printf("bHuaRe =%d\n",bHuaRe);
		if(-1!=g_pDatabase->GetIndex(write_id))
		{
			reg_map_t reg_iterator=m_data->m_reg_cache.find(string(write_id));
			if(reg_iterator!=m_data->m_reg_cache.end())
			{
				modbus_data_mapping_t item=reg_iterator->second;
				int nlen;

				if((wFunc == 0x06) ||(wFunc == 0x10))/*可读可写,华热只能用0x06功能码*/
				{
					item.func = wFunc;
					item.attr = 0x03;
				}

				if((nlen=check_domain(item.func,MODBUS_WRITE)))
				{
						long long lValue=GetDBValue(write_id).lValue;
						
//						printf("write_id =%s\n",write_id);
//						printf("Write lValue = %lld\n",lValue);
//						printf("Write item.count = %d\n",item.count);
//						printf("Write sizeof(lValue) = %d\n",sizeof(lValue));
						if(sizeof(lValue)>=item.count)
						{ 
							
//					printf("Write item.slave =%x,item.func =%x,item.reg =%x,item.count/nlen =%x\n" ,item.slave,item.func,item.reg,item.count/nlen) ;
							if(item.slave < 7) /*wangli2016-10-20 16:19:16 模温机*/
								item.slave = GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED212").lValue;
							else if((item.slave == 11)||(item.slave == 12)) /*wangli2016-10-20 16:19:23 干燥机*/
								item.slave = GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED218").lValue;

//						printf("2222222222  Write lValue = %lld\n",lValue);
							m_data->m_client->Write(item.slave,item.func,item.reg,&lValue,item.count/nlen);
						}
				}	
			}
		}
		UnLock();
}

void	CtmTaskModbus::ReadWrite(char* write_id, char* read_id)
{
		Lock();
		if(-1!=g_pDatabase->GetIndex(write_id))
		{
			reg_map_t reg_write_iterator=m_data->m_reg_cache.find(string(write_id));
			reg_map_t reg_read_iterator=m_data->m_reg_cache.find(string(read_id));
			if(reg_write_iterator!=m_data->m_reg_cache.end()&&
				reg_read_iterator!=m_data->m_reg_cache.end())
			{
				modbus_data_mapping_t  write_item=reg_write_iterator->second;
				modbus_data_mapping_t  read_item=reg_read_iterator->second;
				int nlen;
				if(write_item.slave==read_item.slave)
				{
					if((nlen=check_domain(MODBUS_WRITE_AND_READ_REGISERS_CODE,MODBUS_RDWR)))
					{
						long long lValue=GetDBValue(write_id).lValue;
						if(sizeof(lValue)>write_item.count)
						{ 
							UnLock();
							return;
						}
						if(write_item.slave < 7) /*wangli2016-10-20 16:19:16 模温机*/
							write_item.slave = GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED212").lValue;
						else if((write_item.slave == 11)||(write_item.slave == 12)) /*wangli2016-10-20 16:19:23 干燥机*/
							write_item.slave = GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED218").lValue;

						m_data->m_client->ReadWrite(write_item.slave,MODBUS_WRITE_AND_READ_REGISERS_CODE,write_item.reg,&lValue,write_item.count/nlen,read_item.reg, read_item.count/nlen);
					}
				}	
			}
		}
		UnLock();
}

/*read and write by registers*/	
void	CtmTaskModbus::Read(int slave, int func, int start_addr,int nr)
{                                 
//	printf(" Read    slave =%d , func =%d, start_addr =%d , nr =%d\n\n",slave,func,start_addr,nr);
		m_data->m_client->Read(slave, func, start_addr,nr);
}

void	CtmTaskModbus::Write(int slave, int func, int start_addr, void* src,  int nr)
{
		m_data->m_client->Write(slave,func, start_addr, src, nr);
}

void	CtmTaskModbus::ReadWrite(int slave, int func,int write_addr,void* src,int write_nr,int read_addr,int read_nr)
{
	 	m_data->m_client->ReadWrite(slave,func,write_addr,src,write_nr,read_addr,read_nr);
}


int		CtmTaskModbus::GetData(int slave,int start_addr,void* dest, int nr)
{
	return 0;	
}

#ifndef DJSON
 void	CtmTaskModbus::OpenXmlFile(char* xmlFile)
 {
 	if(!xmlFile)	return;
 	if(-1==access(xmlFile,F_OK))
 	{
 		return ;	
 	}
 	if(strcmp(xmlFile+strlen(xmlFile)-4,".xml")==0)
 	{
 		m_data->m_xmlFile=xmlParseFile(xmlFile);
 	}
 }
 void	CtmTaskModbus::CloseXmlFile()
 {
 	if(m_data->m_xmlFile)
 	{
 		xmlFreeDoc(m_data->m_xmlFile);
 		m_data->m_xmlFile=0;	
 	}	
 }
 
 void	CtmTaskModbus::ParseXml()
 {
 	if(m_data->m_xmlFile)
 	{
 		xmlNsPtr ns;
    	xmlNodePtr cur;
		/*
		*	check file is of right kind
		*/	
		if(!m_data->m_xmlFile)	return;
    	cur = xmlDocGetRootElement(m_data->m_xmlFile);
    	if (!cur) 
    	{
       		fprintf(stderr,"empty document\n");
       		return ;
		}
		ns = xmlSearchNsByHref(m_data->m_xmlFile, cur,
	    	(const xmlChar *) "http://www.techmation.com.cn");
		if(!ns)
		{
		 	fprintf(stderr,
	        	"document of the wrong type, Moudle not found\n");
	     	CloseXmlFile();
	     	return ;	
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "TCP"))
		{
				m_data->m_mode=MODBUS_TCP;
				xmlChar* ip=xmlGetProp(cur,(const xmlChar*)"ip");
				xmlChar* port=xmlGetProp(cur,(const xmlChar*)"port");
				xmlChar* src_port=xmlGetProp(cur,(const xmlChar*)"src_port");
				if(ip&&port)
				{
					strcpy(m_data->m_ip,(char*)ip);
					m_data->m_tcp.ip=m_data->m_ip;
					int nport=	strtoul(reinterpret_cast<const char*>(port),NULL,10);
					if(nport!=ERANGE)
					{
						m_data->m_tcp.port=nport;	
					}
					if(src_port)
					{
						nport=strtoul(reinterpret_cast<const char*>(src_port),NULL,10);
						if(nport!=ERANGE)
						{
							m_data->m_tcp.src_port=nport;
								
						}	
					}
				}
				
				//printf("ip=%s,port=%d,src_port=%d\n",m_data->m_tcp.ip,m_data->m_tcp.port,m_data->m_tcp.src_port);
    	}
    	else if (!xmlStrcmp(cur->name, (const xmlChar *) "RTU"))
    	{
    		   m_data->m_mode=MODBUS_RTU;
    		   int channel, buadrate,databits,stopbits;
    		   char parity;

    		   xmlChar* xChannel=xmlGetProp(cur,(const xmlChar*)"channel");
    		   xmlChar* xBuadrate=xmlGetProp(cur,(const xmlChar*)"buadrate");
    		   xmlChar* xParity=xmlGetProp(cur,(const xmlChar*)"parity");
    		   xmlChar* xDatabits=xmlGetProp(cur,(const xmlChar*)"databits");
    		   xmlChar* xStopbits=xmlGetProp(cur,(const xmlChar*)"stopbits");
    		   //printf("%s-%s-%s-%s-%s\n",xChannel,xBuadrate,xParity,xDatabits,xStopbits);
    		   if(xChannel&&xBuadrate&&xParity&&xDatabits&&xStopbits)
    		   {
    		
    		   		channel=strtoul(reinterpret_cast<const char*>(xChannel),NULL,10);
    		   		if(channel!=ERANGE)
    		   		{
    		  			m_data->m_rtu.port=channel;	
    		   		}	
    		   		buadrate=strtoul(reinterpret_cast<const char*>(xBuadrate),NULL,10);
    		   		if(buadrate!=ERANGE)
    		   		{
    		  			m_data->m_rtu.buadrate=buadrate;	
    		   		}
    		   		parity=xParity[0];
    		   		switch(parity)
    		   		{
    		   			case 'N':
    		   			case 'E':
    		   			case 'O':
    		   				m_data->m_rtu.parity=parity;
    		   			default:
    		   				break;		
    		   		}
    		   		databits=strtoul(reinterpret_cast<const char*>(xDatabits),NULL,10);
    		   		if(databits!=ERANGE)
    		   		{
    		   			switch(databits)
    		   			{
    		   				case 5:
    		   				case 6:
    		   				case 7:
    		   				case 8:
    		   					m_data->m_rtu.databits=databits;
    		   				default:
    		   					break;	
    		   			}	
    		   		}
    		   		stopbits=strtoul(reinterpret_cast<const char*>(xStopbits),NULL,10);	
    		   		if(stopbits!=ERANGE)
    		   		{
    		   			switch(stopbits)
    		   			{
    		   				case 1:
    		   				case 2:
    		   					m_data->m_rtu.stopbits=stopbits;
    		   				default:
    		   					break;	
    		   			}	
    		   		}
    		   		
    		   		//printf("port=%d,buadrate=%d,parity=%c,databits=%d,stopbits=%d\n",m_data->m_rtu.port,
    		   		//m_data->m_rtu.buadrate,m_data->m_rtu.parity,m_data->m_rtu.databits, m_data->m_rtu.stopbits);
    		   }
    		   
    		   
    	}
    	else
    	{
    		CloseXmlFile();
    		fprintf(stderr,"document of the wrong type, root node not find in regular\n");
			return;	
    	}
    	
    	cur = cur->xmlChildrenNode;
    	while(cur && xmlIsBlankNode(cur)) 
    	{
			cur = cur->next;
    	}
    	if(!cur)
    	{
    		CloseXmlFile();
    		return;
		}
		while(cur)
    	{
    		if ((!xmlStrcmp(cur->name, (const xmlChar *) "Component")) &&(cur->ns == ns)) 
	    	{
	   			ParseComponent(m_data->m_xmlFile,ns,cur);
			}
			cur = cur->next;	
    	}
 	}	
 }
 
void 	CtmTaskModbus::ParseComponent(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur)
{
		xmlChar* slave;
		xmlChar* func;
		xmlChar* reg;
		xmlChar* count;
		xmlChar* id;
		xmlChar* attr;
		modbus_data_mapping_t	item;
		id=xmlGetProp(cur,(const xmlChar*)"ID");
		slave=xmlGetProp(cur,(const xmlChar*)"Slave");
		func=xmlGetProp(cur,(const xmlChar*)"Function");
		reg=xmlGetProp(cur,(const xmlChar*)"Reg");
		count=xmlGetProp(cur,(const xmlChar*)"Count");
		attr=xmlGetProp(cur,(const xmlChar*)"Attr");
		printf("id=%s,slave=%s, func=%s, reg=%s, count=%s, attr=%s\n",
		 id, slave, func, reg, count, attr);
		if(!id||!slave||!func||!reg||!count||!attr)	return ;
		string name(reinterpret_cast<const char*>(id));
		item.slave=strtoul(reinterpret_cast<const char*>(slave),NULL,16);

	  /*wangli2016-10-27 14:19:04 收到辅机恢复的包后，不去判断slave*/
	   std::vector<int>::iterator it=m_data->m_parse_slave.begin();
	   for(;it!=m_data->m_parse_slave.end(); ++it)
	   {
	    if(*it==item.slave) break;
	   }
	   if(it==m_data->m_parse_slave.end())
	    m_data->m_parse_slave.push_back(item.slave);

		if(item.slave==ERANGE)	return;
		item.func=strtoul(reinterpret_cast<const char*>(func),NULL,16);
		if(item.func==ERANGE)	return;
		item.reg=strtoul(reinterpret_cast<const char*>(reg),NULL,16);
		if(item.reg==ERANGE)	return;
		item.count=strtoul(reinterpret_cast<const char*>(count),NULL,16);
		if(item.count==ERANGE)  return;
		item.attr=strtoul(reinterpret_cast<const char*>(attr),NULL,16);
		if(item.attr==ERANGE)	return;
		if(item.attr==0x01)
		{
			m_data->m_power_on_data_cache.push(item);	
		}
		else if(item.attr==0x02)
		{
			m_data->m_read_data_cache.push(item);	
		}
		//printf("slave=%x,func=%x,reg=%x,count=%x,attr=%x\n",item.slave,item.func,item.reg,item.count,item.attr);
		
		reg_map_t reg_iterator=m_data->m_reg_cache.find(name);
		if(reg_iterator==m_data->m_reg_cache.end())
			m_data->m_reg_cache.insert(pair<string,modbus_data_mapping_t>(name,item));
		
		id_map_t id_iterator=m_data->m_id_cache.find(item);
		if(id_iterator==m_data->m_id_cache.end())
			m_data->m_id_cache.insert(pair<modbus_data_mapping_t,string>(item,name));
}
#else
void  CtmTaskModbus::ParseJsonTCP(JSONNode header,modbus_tcp_t& tcp)
{

	char 	sz[125];
	WORD	wIP0 = 0,	wIP1 = 0,	wIP2 = 0,	wIP3 = 0;
	if(std::string("ip")==header.name())
	{
		strcpy(m_data->m_ip,header.as_string().c_str());
		//tcp.ip=m_data->m_ip;
		
		wIP0 =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED176").lValue;
		wIP1 =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED177").lValue;
		wIP2 =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED178").lValue;
		wIP3 =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED179").lValue;
		
		sprintf(sz,"%d.%d.%d.%d",wIP0,wIP1,wIP2,wIP3);

		strcpy(m_data->m_ip,sz);
		tcp.ip=m_data->m_ip;
  

//	printf("tcp.ip =%s\n ",tcp.ip);
		
		//std::cout<<header.as_string()<<std::endl;
	}
	else if(std::string("port")==header.name())
	{
		//tcp.port=header.as_int();
		tcp.port= GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED180").lValue;
		std::cout<<tcp.port<<std::endl;
	}
	else if(std::string("src_port")==header.name())
	{
		tcp.src_port=header.as_int();
		//std::cout<<tcp.src_port<<std::endl;
	}
}
//
void  CtmTaskModbus::ParseJsonRTU(JSONNode header,modbus_rtu_t& rtu)
{

	
	if(std::string("channel")==header.name())
	{
		rtu.port=header.as_int();
		//std::cout<<"port:"<<int(rtu.port)<<std::endl;
	}
	else if(std::string("buadrate")==header.name())
	{
		rtu.buadrate=header.as_int();
		//std::cout<<"buad:"<<int(rtu.buadrate)<<std::endl;
	}
	else if(std::string("parity")==header.name())
	{
		rtu.parity=header.as_string().c_str()[0];
		//std::cout<<"parity:"<<rtu.parity<<std::endl;
	}
	else if(std::string("databits")==header.name())
	{
		rtu.databits=header.as_int();
		//std::cout<<"databits:"<<int(rtu.databits)<<std::endl;
	}
	else if(std::string("stopbits")==header.name())
	{
		rtu.stopbits=header.as_int();
		//std::cout<<"stopbits:"<<int(rtu.stopbits)<<std::endl;
	}
}  
//
const modbus_data_mapping_t CtmTaskModbus::ParseJsonChild(JSONNode child)
{
 modbus_data_mapping_t item;
 string name;
 for(int i=0; i<child.size(); i++)
 {
  if(std::string("id")==child[i].name())
  {
   name=child[i].as_string();
   ;//std::cout<<child[i].as_string()<<std::endl;
  }
  else if(std::string("slave")==child[i].name())
  {
   item.slave=child[i].as_int();
   std::vector<int>::iterator it=m_data->m_parse_slave.begin();
   for(;it!=m_data->m_parse_slave.end(); ++it)
   {
    if(*it==item.slave) break;
   }
   if(it==m_data->m_parse_slave.end())
    m_data->m_parse_slave.push_back(item.slave);
   //std::cout<<item.slave<<std::endl;
  }
  else if(std::string("fuction")==child[i].name())
  {
   item.func=child[i].as_int();
   //std::cout<<item.func<<std::endl;
  }
  else if(std::string("reg")==child[i].name())
  {
   item.reg=child[i].as_int();
   //std::cout<<item.reg<<std::endl;
  }
  else if(std::string("count")==child[i].name())
  {
   item.count=child[i].as_int();
   //std::cout<<item.count<<std::endl;
  }
  else if(std::string("attr")==child[i].name())
  {
   item.attr=child[i].as_int();
   //std::cout<<item.attr<<std::endl;
  }
 }
 if(item.attr==0x01)
 {
   m_data->m_power_on_data_cache.push(item); 
 }
 else if(item.attr==0x02)
 {
   m_data->m_read_data_cache.push(item); 
 }
// printf("slave=%x,func=%x,reg=%x,count=%x,attr=%x\n",item.slave,item.func,item.reg,item.count,item.attr);
  
 reg_map_t reg_iterator=m_data->m_reg_cache.find(name);
 if(reg_iterator==m_data->m_reg_cache.end())
  m_data->m_reg_cache.insert(pair<string,modbus_data_mapping_t>(name,item));
  
 id_map_t id_iterator=m_data->m_id_cache.find(item);
 if(id_iterator==m_data->m_id_cache.end())
  m_data->m_id_cache.insert(pair<modbus_data_mapping_t,string>(item,name));
 return item;
}

void CtmTaskModbus::ParseJson(const std::string file) 
{
	std::string str=read_file(file);
	if(!libjson::is_valid(str))  {      
  
//        printf("Parse faild!\n");      

        return; 

    }
    //printf("start....\n");
	JSONNode rn=libjson::parse(str);
	int mode=0;
	if(rn.size())
	{
		if(std::string("RTU")==rn[0].as_string())
		{
			mode=1;	
		}
		else if(std::string("TCP")==rn[0].as_string())
		{
			mode=2;
		}
		else
		{
			mode=0;
		}
	}	
//	printf("mode=%d\n",mode);
	if(mode==1)
	{
		m_data->m_mode=MODBUS_RTU;
		for(int i=1; i<rn.size(); i++)
		{
			if(!rn[i].size())
			{
				ParseJsonRTU(rn[i],m_data->m_rtu);
			}
			if(rn[i].name()==std::string("component"))	
			{
				JSONNode child=rn[i].as_node();
				ParseJsonChild(child);
			}
		}
	}
	else if(mode==2)
	{
		m_data->m_mode=MODBUS_TCP;
		for(int i=1; i<rn.size(); i++)
		{
			if(!rn[i].size())
			{
				ParseJsonTCP(rn[i],m_data->m_tcp);
			}
			if(rn[i].name()==std::string("component"))	
			{
				JSONNode child=rn[i].as_node();
				ParseJsonChild(child);
			}
		}
	}
	//printf("end......\n");
}
#endif
/** ================CallBack Task Work=====================**/

void Task(uint8_t slave, uint16_t func, uint16_t start_addr, void* rsp,int nr)
{
//	printf("Task      slave=%x,func=%x,start_addr=%x,rsp =%s, nr=%x\n",slave,func,start_addr,rsp,nr);
	int nlen;
	if((nlen=check_domain(func,MODBUS_READ))||(nlen=check_domain(func,MODBUS_RDWR)))
	{
//		printf("Task 111111111111111\n");
		if(nr<=0)	return;
		modbus_data_mapping_t item={slave,func,start_addr,nr*nlen,0};
		char* id=0;		
    	if(g_pTaskModbus)
    	{
    		id=(char*)g_pTaskModbus->Merbromin(&item);
    	}	

//   	if(CTaskModbus::GetInstance("CtmTaskModbus"))
//    	{
//    		printf("aaaaaaaaaaaaaaaaaaaaaaaa\n");
//    		id =(char*)CTaskModbus::GetInstance("CtmTaskModbus")->Merbromin(&item);
//    	}
//
//		printf("id =%d\n",id);
//		printf("id =%s\n",id);
    	if(id)
    	{
    		long lValue =0;
    		int  size=nr*nlen;
    		if(sizeof(lValue)<nr*nlen)
    			size=sizeof(lValue);
    		memcpy(&lValue,rsp,size);
    		
//    		printf("lValue =%d\n",lValue);
//    		printf("size =%d\n",size);
    		if(!strcmp(id,"SYSX_OTHERS_OTHERS_INT_RESERVED164")) /*wangli2016-10-25 9:52:53 冷水机设定温度 可读可写*/
    			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED169",lValue);
    		else	if(!strcmp(id,"SYSX_OTHERS_OTHERS_INT_RESERVED160")) /*wangli2016-10-25 9:52:53 华热模温机 设定温度 可读可写（不包括信易）,由于信易模温机设定值 配成写，所以代码这样写不影响兼容性*/
    			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED168",lValue);
    		else	if(!strcmp(id,"SYSX_OTHERS_OTHERS_INT_RESERVED185")) /*wangli2016-11-2 14:35:39 华热三机一体 设定干燥温度 可读可写*/
    			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED186",lValue);
    		else
	    		SetDBValue(id,lValue);
    	}	
		
				
	}
	else if((nlen=check_domain(func,MODBUS_WRITE)))
	{
//		printf("Write budbus\n");
		/*if write failed,you can resend*/
		
	}
}


void* CtmTaskModbus::Merbromin(void * param)
{
//	printf("Merbromin :m_data->m_id_cache.size() =%d\n",m_data->m_id_cache.size());
  char* pcID=0;
  modbus_data_mapping_t* item=(modbus_data_mapping_t*)param;
  id_map_t id_iterator=m_data->m_id_cache.find(*item);
  if(id_iterator!=m_data->m_id_cache.end())
  {
   pcID=(char*)id_iterator->second.c_str();

  }

//	printf("pcID =%s\n",pcID);
  if(pcID!=0) return pcID;

//	printf("m_data->m_parse_slave.size() =%d\n",m_data->m_parse_slave.size());

  for(int i=0; i<m_data->m_parse_slave.size(); i++)
  {
   item->slave=m_data->m_parse_slave[i];
   id_map_t id_iterator=m_data->m_id_cache.find(*item);
   if(id_iterator!=m_data->m_id_cache.end())
   {
    pcID=(char*)id_iterator->second.c_str();
//	printf("pcID 2222=%s\n",pcID);
   }
   if(pcID) break;
  }
  
  return pcID;
}

#ifndef	D_BEOBJECT_CTMTASKMODBUS
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);

	if (pResult != NULL)
	{
		(static_cast<CTaskModbus*>(pResult))->CreateSelf();
	}

	return pResult;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskModbus*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
