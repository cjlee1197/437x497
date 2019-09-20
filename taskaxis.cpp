#include "taskaxis.h"


#include    "utils.h"
#include 	"package.h"




IMPLEMENT_DYNCREATE(CTaskAxis, CtmPackClass)

static hash_map<string, CTaskAxis*,str_hash,str_equal> u_instances;
typedef	hash_map<string, CTaskAxis*,str_hash,str_equal>::iterator	instance_map_t;



CTaskAxis::CTaskAxis():CTask(), CtmPackClass()
{
	m_data=new CtmAxisPrivateData();
	pthread_mutex_init(&m_data->m_lock,0);
}

CTaskAxis::~CTaskAxis()
{
	if(m_data)
	{
		pthread_mutex_destroy(&m_data->m_lock);	
	}
	delete m_data;
}

int		CTaskAxis::CreateSelf()
{
	
	return 0;
}

int		CTaskAxis::FreeSelf()
{
	return 0;
}


int		CTaskAxis::Lock()
{
	return pthread_mutex_lock(&m_data->m_lock);
}

int 	CTaskAxis::UnLock()
{
	return pthread_mutex_unlock(&m_data->m_lock);
}


int		CTaskAxis::Import(char* pszFileName, AXIS axis, bool dirty)
{
	#if 0
	Lock();
	if(axis<0||axis>=AXIS_ALL)	return 0;
	if(pszFileName==0||strstr(pszFileName,".xml")==0) return 0;
	
	CXml xml_read(pszFileName,CXml::XML_READ);
	CXmlNode root;
	xml_read.parse_xml_file();
	root=xml_read.rootNode();
	m_data->m_root=root;
	
	std::cout<<"root: "<<root.name()<<" size: "<<root.size()<<std::endl;
	for(int i=0; i<root.attrCount();++i)
	{
		std::cout<<root.find(i).first<<" : "<<root.find(i).second<<std::endl;	
	}
	
	for(int i=0; i<root.size(); ++i)
	{
		CXmlNode node= root[i];
		std::cout<<node.name()<<" size: "<<node.size()<<std::endl;
		for(int j=0; j<node.attrCount(); ++j)
		{
			std::cout<<node.find(j).first<<" : "<<node.find(j).second<<std::endl;	
		}
		for(int j=0; j<node.size();++j)
		{
			CXmlNode items=node[j];
			std::cout<<"Name: "<<items.name()<<" size: "<<items.size()<<std::endl;
			for(int i=0; i<items.size(); i++)
			{
				CXmlNode item=items[i];
				std::cout<<"Name: "<<item.name()<<std::endl;
				for(int j=0; j<item.attrCount();++j)
				{
					std::cout<<item.find(j).first<<" : "<<item.find(j).second<<std::endl;	
					if(item.find(j).first=="ID")
					{	
						std::cout<<"convert:"<<atoi(item.find(j).second.c_str())<<std::endl;
					}
				}
			}
		}
	}
	UnLock();
	#endif		
}

int 	CTaskAxis::Export(char* pszFileName, AXIS axis)
{
	#if 0
	Lock();
	if(axis<0||axis>=AXIS_ALL)	return 0;
	if(pszFileName==0||strstr(pszFileName,".xml")==0) return 0;
	
	CXml xml_write(pszFileName,CXml::XML_WRITE);
	xml_write.save_xml_file(m_data->m_root);
	UnLock();
	#endif
}


int		CTaskAxis::CreateInstance(char* cls, char* lib)
{
	instance_map_t instance=u_instances.find(string(cls));
	if(instance==u_instances.end())
	{
		if(cls)
		{
			CTaskAxis* element=static_cast<CTaskAxis*>(g_pPackage->CreateClassInstance(lib));
			if(!element) return -1;
			u_instances.insert(pair<string,CTaskAxis*>(string(cls),element));
			return 0;
		}
		return -1;	
	}
	return -1;
}

CTaskAxis* 	CTaskAxis::GetInstance(char* cls)
{
	instance_map_t instance=u_instances.find(string(cls));
	if(instance!=u_instances.end())
	{
		return instance->second;
	}
	return 0;
}


int		CTaskAxis::GetAxisValue(int id)const 
{
	if(m_data->m_axis>=AXIS_ALL) return 0;
	map<ID, AxisItem>::const_iterator it=m_data->m_item_write[m_data->m_axis].find(id);
	if(it==m_data->m_item_write[m_data->m_axis].end())
	{
		map<ID, AxisItem>::const_iterator it=m_data->m_item_read[m_data->m_axis].find(id);
		if(it==m_data->m_item_read[m_data->m_axis].end())
			return 0;	
	}
	if(it->second.dataType<8)
		return it->second.lValue;
	return it->second.fValue;
}

float		CTaskAxis::GetAxisValueFloat(int id)const
{
	if(m_data->m_axis>=AXIS_ALL) return 0;
	map<ID, AxisItem>::const_iterator it=m_data->m_item_write[m_data->m_axis].find(id);
	if(it==m_data->m_item_write[m_data->m_axis].end())
	{
		map<ID, AxisItem>::const_iterator it=m_data->m_item_read[m_data->m_axis].find(id);
		if(it==m_data->m_item_read[m_data->m_axis].end())
			return 0;	
	}
	if(it->second.dataType<8)
		return it->second.lValue;
	return it->second.fValue;
		
}

AxisItem	CTaskAxis::GetAxisItem(int id)const
{
	if(m_data->m_axis>=AXIS_ALL) return  AxisItem();
	map<ID, AxisItem>::const_iterator it=m_data->m_item_write[m_data->m_axis].find(id);
	if(it==m_data->m_item_write[m_data->m_axis].end())
	{
		map<ID, AxisItem>::const_iterator it=m_data->m_item_read[m_data->m_axis].find(id);
		if(it==m_data->m_item_read[m_data->m_axis].end())
			return AxisItem();	
	}
	return it->second;
}
		
int			CTaskAxis::SetAxisValue(int id, int value, int dirty)
{
	Lock();
	map<ID, AxisItem>::const_iterator it=m_data->m_item_write[m_data->m_axis].find(id);
	if(it!=m_data->m_item_write[m_data->m_axis].end())
	{
		if(it->second.dataType<8)
			m_data->m_item_write[m_data->m_axis][id].lValue=value;
		else
			m_data->m_item_write[m_data->m_axis][id].fValue=value;
		m_data->m_item_write[m_data->m_axis][id].dirty=dirty;
	}else{
		map<ID, AxisItem>::const_iterator it=m_data->m_item_read[m_data->m_axis].find(id);
		if(it!=m_data->m_item_read[m_data->m_axis].end())
		{
			if(it->second.dataType<8)
				m_data->m_item_read[m_data->m_axis][id].lValue=value;
			else
				m_data->m_item_read[m_data->m_axis][id].fValue=value;	
			m_data->m_item_read[m_data->m_axis][id].dirty=dirty;
		}
	}
	
	UnLock();
}

int 		CTaskAxis::SetAxisValue(int id, float value, int dirty)
{
	Lock();
	map<ID, AxisItem>::const_iterator it=m_data->m_item_write[m_data->m_axis].find(id);
	if(it!=m_data->m_item_write[m_data->m_axis].end())
	{
		if(it->second.dataType<8)
			m_data->m_item_write[m_data->m_axis][id].lValue=value;
		else
			m_data->m_item_write[m_data->m_axis][id].fValue=value;
		m_data->m_item_write[m_data->m_axis][id].dirty=dirty;
	}else{
		map<ID, AxisItem>::const_iterator it=m_data->m_item_read[m_data->m_axis].find(id);
		if(it!=m_data->m_item_read[m_data->m_axis].end())
		{
			if(it->second.dataType<8)
				m_data->m_item_read[m_data->m_axis][id].lValue=value;
			else
				m_data->m_item_read[m_data->m_axis][id].fValue=value;	
			m_data->m_item_read[m_data->m_axis][id].dirty=dirty;
		}
	}
	UnLock();
}

bool		CTaskAxis::CheckWriteRegister(DWORD reg)
{
	return 0;	
}

bool		CTaskAxis::CheckRegister(DWORD reg)
{
	return 0;	
}

int 		CTaskAxis::GetAxisDataType(int id)const
{
	if(m_data->m_axis>=AXIS_ALL) return -1;
	map<ID, AxisItem>::const_iterator it=m_data->m_item_write[m_data->m_axis].find(id);
	if(it==m_data->m_item_write[m_data->m_axis].end())
	{
		map<ID, AxisItem>::const_iterator it=m_data->m_item_read[m_data->m_axis].find(id);
		if(it==m_data->m_item_read[m_data->m_axis].end())
			return -1;	
	}
	return it->second.dataType;
	
}

int			CTaskAxis::SetDirty(int id, int dirty)
{
		if(m_data->m_axis>=AXIS_ALL) return -1;
		map<ID, AxisItem>::const_iterator it=m_data->m_item_write[m_data->m_axis].find(id);
		if(it!=m_data->m_item_write[m_data->m_axis].end())
			m_data->m_item_write[m_data->m_axis][id].dirty=dirty;
		return 0;
}
		
/*手动读取一次属性为可写的所有资料*/
int			CTaskAxis::ReadAxisSync()
{
	return 0;		
}

void		CTaskAxis::SetResponeState(int state)
{
	m_data->m_rsp=state;
}

int		CTaskAxis::GetResponeState()const
{
	return m_data->m_rsp;	
}

bool	CTaskAxis::CheckStateRegister(DWORD reg)
{
		return false;
}

int		CTaskAxis::DownloadAxisSync()
{
		return 0;
}

int CTaskAxis::DownloadDirtyData()
{
	return 0;
}




