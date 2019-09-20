/*
*	Created by J.Wong 2016/3/16 15:32:13
*/

#include "onlinehelp.h"
#include "common.h"
#include <map>
using namespace std;


const int		MAXLEN=4096*16;

class CtmOnlineHelp::CtmPrivateData{
	public:
		CtmPrivateData():m_xmlFile(0),m_ncontrols(0)
		{
			memset(m_szdetails,0,sizeof(m_szdetails)/sizeof(char));
			memset(m_szdetail,0,sizeof(m_szdetail)/sizeof(char));
			m_view.title=0;
			m_view.footer=0;
			for(int i=0; i<MAXCONTROLS; i++)
			{
				m_view.controls[i].id=0;
				m_view.controls[i].name=0;
				m_view.controls[i].detail=0;
				m_view.controls[i].img=0;
				m_view.controls[i].buddy=0;	
			}	
		}
		void releaseSource()
		{
			m_ncontrols=0;
			if(m_view.title)
				delete[] m_view.title;
			m_view.title=0;
			if(m_view.footer)
				delete[] m_view.footer;
			m_view.footer=0;
			for(int i=0; i<MAXCONTROLS; i++)
			{
				if(m_view.controls[i].id)
					delete[] 	m_view.controls[i].id;
				if(m_view.controls[i].name)
					delete[]  m_view.controls[i].name;
				if(m_view.controls[i].detail)
					delete[]	m_view.controls[i].detail;
				if(m_view.controls[i].img)
					delete[] m_view.controls[i].img;
				if(m_view.controls[i].buddy)
					delete[] m_view.controls[i].buddy;
				m_view.controls[i].id=0;
				m_view.controls[i].name=0;
				m_view.controls[i].detail=0;
				m_view.controls[i].img=0;
				m_view.controls[i].buddy=0;	
			}
			std::map<int, tmControl>::iterator it_int = m_controls.begin();
			for(; it_int != m_controls.end(); ++it_int) 
			{
				if(it_int->second.id)
					delete[] it_int->second.id;
				if(it_int->second.name)
					delete[] it_int->second.name;
				if(it_int->second.detail)
					delete[] it_int->second.detail;
				if(it_int->second.img)
					delete[] it_int->second.img;
				if(it_int->second.buddy)
					delete it_int->second.buddy;
			}
			m_controls.clear();	
			
			map<string, tmControl>::iterator it_str = m_strKey_controls.begin();
			for(; it_str != m_strKey_controls.end(); ++it_str) 
			{
				if(it_str->second.id)
					delete[] it_str->second.id;
				if(it_str->second.name)
					delete[] it_str->second.name;
				if(it_str->second.detail)
					delete[] it_str->second.detail;
				if(it_str->second.img)
					delete[] it_str->second.img;
				if(it_str->second.buddy)
					delete[] it_str->second.buddy;
			}
			m_strKey_controls.clear();		
		}
		
		
		~CtmPrivateData()
		{
			if(m_xmlFile)
				xmlFreeDoc(m_xmlFile);
			releaseSource();
		}
	public:	
		xmlDocPtr	m_xmlFile;
		tmView  m_view;
		int		m_ncontrols;
		map<int, tmControl> m_controls;
		map<string,tmControl> m_strKey_controls;
		char	m_szdetails[MAXLEN];
		char	m_szdetail[MAXLEN];
};

CtmOnlineHelp::CtmOnlineHelp (const char* xmlFile)
{
		m_data=new CtmPrivateData();
		OpenXmlFile(xmlFile);
		
		
}
CtmOnlineHelp::~CtmOnlineHelp()
{
	delete m_data;	
}
void	CtmOnlineHelp::OpenXmlFile(const char* xmlFile)
{
	//Check the legality of the file "xx.xml"
	if(!xmlFile)	return;
	if(m_data->m_xmlFile)
	{
		xmlFreeDoc(m_data->m_xmlFile);
		m_data->m_xmlFile=0;	
	}
	if(xmlFile&&strlen(xmlFile)>4)
	{
			if(!strcmp(xmlFile+strlen(xmlFile)-4,".xml")||!strcmp(xmlFile+strlen(xmlFile)-4,".eng")
			||!strcmp(xmlFile+strlen(xmlFile)-4,".chs")||!strcmp(xmlFile+strlen(xmlFile)-4,".chn"))
			{
				m_data->m_xmlFile=xmlParseFile(xmlFile);
				if(!m_data->m_xmlFile)
					printf("open %s error\n",xmlFile);	
			}	
	}
	else
	{
		m_data->m_xmlFile=0;	
	}
	parseHeadInfo();
}

char*	CtmOnlineHelp::GetTitle()
{
		return reinterpret_cast<char*>(m_data->m_view.title);
}

char*	CtmOnlineHelp::GetFooter()
{
	return reinterpret_cast<char*>(m_data->m_view.footer);	
}

char*	CtmOnlineHelp::GetID(int nIndex)
{
		if(nIndex<0||nIndex>=MAXCONTROLS)	return 0;
		return reinterpret_cast<char*>(m_data->m_view.controls[nIndex].id);
}
char*	CtmOnlineHelp::GetName(int nIndex)
{
		if(nIndex<0||nIndex>=MAXCONTROLS)	return 0;
		return reinterpret_cast<char*>(m_data->m_view.controls[nIndex].name);
}

char*	CtmOnlineHelp::GetDetail(int nIndex)
{
		if(nIndex<0||nIndex>=MAXCONTROLS)	return 0;
		return reinterpret_cast<char*>(m_data->m_view.controls[nIndex].detail);
}

char*	CtmOnlineHelp::GetImg(int nIndex)
{
	if(nIndex<0||nIndex>=MAXCONTROLS)	return 0;
	return reinterpret_cast<char*>(m_data->m_view.controls[nIndex].img);	
}

char*	CtmOnlineHelp::GetNameByID(int id)
{
		return reinterpret_cast<char*>(m_data->m_controls[id].name);
}

char*	CtmOnlineHelp::GetDetailByID(int id)
{
	
	memset(m_data->m_szdetail,0, sizeof(m_data->m_szdetail)/sizeof(char));
	tmControl control;
	char sparator='\n';
	//char* LabelDetail="Detail: \n";
	int nOffset=0;
	if(id>=0)
	{
		control=GetControlByID(id);
		if(control.buddy)
		{
			int buddy=strtoul(reinterpret_cast<char*>(control.buddy),NULL,16);
			if(buddy!=ERANGE)
			{
				tmControl temp=GetControlByID(buddy);
				control.detail=temp.detail;
			}		
				
		}
		if(control.detail)
		{
			//memcpy(m_data->m_szdetail+nOffset,LabelDetail,strlen(LabelDetail));
			//nOffset+=strlen(LabelDetail);
			if(xmlStrlen(control.detail)>MAXLEN-1)
			{
				return m_data->m_szdetail;
			}
			memcpy(m_data->m_szdetail+nOffset,control.detail,xmlStrlen(control.detail));	
		}
	}
	else
	{
		//std::map<int, tmControl>::iterator it = m_data->m_controls.begin();
		//for(; it!=m_data->m_controls.end();++it)
		for(int i=0; i<m_data->m_ncontrols; i++)
		{
			//control=it->second;
			control=m_data->m_view.controls[i];
			if(control.buddy)	continue;
			//printf("%s \n%s \n%s\n",control.id,control.name,control.detail);
			if(control.detail)
			{
				//memcpy(m_data->m_szdetail+nOffset,LabelDetail,strlen(LabelDetail));
				//nOffset+=strlen(LabelDetail);
				if(xmlStrlen(control.detail)>MAXLEN-1)
				{
					break;
				}
				memcpy(m_data->m_szdetail+nOffset,control.detail,xmlStrlen(control.detail));
				nOffset+=xmlStrlen(control.detail);
				memcpy(m_data->m_szdetail+nOffset,&sparator,1);
				nOffset+=1;
			}
				
		}
		m_data->m_szdetail[nOffset-1]=0;
				
	}
	//printf("%s\n",m_data->m_szdetails);
	return 	m_data->m_szdetail;
	//return reinterpret_cast<char*>(m_data->m_controls[id].detail);
}

char*	CtmOnlineHelp::GetImgByID(int id)
{
	return reinterpret_cast<char*>(m_data->m_controls[id].img);	
}

tmControl	CtmOnlineHelp::GetControlByID(int id)
{
		return m_data->m_controls[id]; 	
}


char*	CtmOnlineHelp::GetDetailsByID(int id)
{
	memset(m_data->m_szdetails,0, sizeof(m_data->m_szdetails)/sizeof(char));
	tmControl control;
	char sparator='\n';
	char* LabelID="ID: ";
	char* LabelName="Name: ";
	char* LabelDetail="Detail: \n";
	int nOffset=0;
	if(id>=0)
	{
		control=GetControlByID(id);
		if(control.buddy)
		{
			int buddy=strtoul(reinterpret_cast<char*>(control.buddy),NULL,16);
			if(buddy!=ERANGE)
			{
				tmControl temp=GetControlByID(buddy);
				control.detail=temp.detail;
			}			
		}
		if(control.id)
		{
			memcpy(m_data->m_szdetails,LabelID,strlen(LabelID));
			nOffset+=strlen(LabelID);
			memcpy(m_data->m_szdetails+nOffset,control.id,xmlStrlen(control.id));
			nOffset+=xmlStrlen(control.id);
			memcpy(m_data->m_szdetails+nOffset,&sparator,1);
			nOffset+=1;
		}
		if(control.name)
		{
			memcpy(m_data->m_szdetails+nOffset,LabelName,strlen(LabelName));
			nOffset+=strlen(LabelName);
			memcpy(m_data->m_szdetails+nOffset,control.name,xmlStrlen(control.name));
			nOffset+=xmlStrlen(control.name);
			memcpy(m_data->m_szdetails+nOffset,&sparator,1);
			nOffset+=1;
		}
		if(control.detail)
		{
			memcpy(m_data->m_szdetails+nOffset,LabelDetail,strlen(LabelDetail));
			nOffset+=strlen(LabelDetail);
			if(xmlStrlen(control.detail)>MAXLEN-1)
			{
				return m_data->m_szdetails;
			}
			memcpy(m_data->m_szdetails+nOffset,control.detail,xmlStrlen(control.detail));	
		}
	}
	else
	{
		//std::map<int, tmControl>::iterator it = m_data->m_controls.begin();
		//for(; it!=m_data->m_controls.end();++it)
		for(int i=0; i<m_data->m_ncontrols; i++)
		{
			//control=it->second;
			if(nOffset>MAXLEN-1)	break;
			control=m_data->m_view.controls[i];
			if(control.buddy)	continue;
			//printf("%s \n%s \n%s\n",control.id,control.name,control.detail);
			if(control.id)
			{
				memcpy(m_data->m_szdetails+nOffset,LabelID,strlen(LabelID));
				nOffset+=strlen(LabelID);
				memcpy(m_data->m_szdetails+nOffset,control.id,xmlStrlen(control.id));
				nOffset+=xmlStrlen(control.id);
				memcpy(m_data->m_szdetails+nOffset,&sparator,1);
				nOffset+=1;
			}
			if(control.name)
			{
				memcpy(m_data->m_szdetails+nOffset,LabelName,strlen(LabelName));
				nOffset+=strlen(LabelName);
				memcpy(m_data->m_szdetails+nOffset,control.name,xmlStrlen(control.name));
				nOffset+=xmlStrlen(control.name);
				memcpy(m_data->m_szdetails+nOffset,&sparator,1);
				nOffset+=1;
			}
		
			if(control.detail)
			{
				memcpy(m_data->m_szdetails+nOffset,LabelDetail,strlen(LabelDetail));
				nOffset+=strlen(LabelDetail);
				if(xmlStrlen(control.detail)>MAXLEN-1)
				{
					break;
				}
				memcpy(m_data->m_szdetails+nOffset,control.detail,xmlStrlen(control.detail));
				nOffset+=xmlStrlen(control.detail);
				memcpy(m_data->m_szdetails+nOffset,&sparator,1);
				nOffset+=1;
			}
				
		}
		m_data->m_szdetails[nOffset-1]=0;
				
	}
	//printf("%s\n",m_data->m_szdetails);
	return 	m_data->m_szdetails;
}

void	CtmOnlineHelp::parseHeadInfo()
{
	xmlNsPtr ns;
    xmlNodePtr cur;
    tmControl control;
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
	        "document of the wrong type, View not found\n");
	     return ;	
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "Helping"))
	{
        fprintf(stderr,"document of the wrong type, root node!= Helping\n");
		return;
    }
    
    /*
     * walk the tree.
     */
    /* First level Controls */
    cur = cur->xmlChildrenNode;
    while(cur && xmlIsBlankNode(cur)) 
    {
		cur = cur->next;
    }
    if(!cur)	return;
    if ((xmlStrcmp(cur->name, (const xmlChar *) "Controls")) || (cur->ns != ns))
    {
        fprintf(stderr,"document of the wrong type, was '%s', Controls expected\n",
		cur->name);
		return;
    }
    xmlChar* title = xmlGetProp(cur, (const xmlChar *) "Title");
    //释放已有资源
    m_data->releaseSource();
    //获取标题信息
    if(title)
    {
    	m_data->m_view.title=new xmlChar[strlen(reinterpret_cast<char*>(title))+1];
    	memcpy(m_data->m_view.title,title,strlen(reinterpret_cast<char*>(title))+1);
    	//printf("Title:%s\n",reinterpret_cast<char*>(m_data->m_view.title));
    		
    }
    xmlChar* footer=xmlGetProp(cur,(const xmlChar*)"Footer");
    if(footer)
    {
    	m_data->m_view.footer=new xmlChar[strlen(reinterpret_cast<char*>(footer))+1];
    	memcpy(m_data->m_view.footer,footer,strlen(reinterpret_cast<char*>(footer))+1);
    }
    
    /*
    *	second level control
    */
    //解析控件
    cur = cur->xmlChildrenNode;
    while(cur)
    {
    	if ((!xmlStrcmp(cur->name, (const xmlChar *) "Control")) &&(cur->ns == ns)) 
	    {
	    	xmlChar* id = xmlGetProp(cur, (const xmlChar *) "ID");
	    	control = parseControl(m_data->m_xmlFile, ns,cur);
	    	if(id>=0)
	    	{
	    		//存储map中
	    		//int nID=strtoul(reinterpret_cast<char*>(id),NULL,16);
	    		//if(nID>=0)
	    		{
	    			tmControl temp;
	    			memset(&temp,0,sizeof(tmControl));
	    			temp.id=	new xmlChar[xmlStrlen(id)+1];
	    			memcpy(temp.id,id,xmlStrlen(id)+1);
	    			if(control.buddy)
	    			{
	    				temp.buddy= new xmlChar[xmlStrlen(control.buddy)+1];
	    				memcpy(temp.buddy,control.buddy,xmlStrlen(control.buddy)+1);
	    					
	    			}
	    			if(control.name)
	    			{
	    				temp.name=	new xmlChar[xmlStrlen(control.name)+1];
	    				memcpy(temp.name,control.name,xmlStrlen(control.name)+1);
	    			}
	    			if(control.detail)
	    			{
	    				temp.detail= new xmlChar[xmlStrlen(control.detail)+1];
	    				memcpy(temp.detail,control.detail,xmlStrlen(control.detail)+1);
	    			}
	    			if(control.img)
	    			{
	    				temp.img= new xmlChar[xmlStrlen(control.img)+1];
	    				memcpy(temp.img,control.img,xmlStrlen(control.img)+1);	
	    			}
	    			//m_data->m_controls[nID]=temp;
	    			string ID(reinterpret_cast<char*>(id));
	    			m_data->m_strKey_controls.insert(pair<string, tmControl>(ID,temp));
	    		}
	    		//存储到数组中
	    		m_data->m_view.controls[m_data->m_ncontrols].id=new xmlChar[xmlStrlen(id)+1];
	    		memcpy(m_data->m_view.controls[m_data->m_ncontrols].id,
	    				id,xmlStrlen(id)+1);
	    		if(control.buddy)
	    		{
	    			m_data->m_view.controls[m_data->m_ncontrols].buddy=new xmlChar[xmlStrlen(control.buddy)+1];
	    			memcpy(m_data->m_view.controls[m_data->m_ncontrols].buddy,
	    			control.buddy,xmlStrlen(control.buddy)+1);	
	    		}
	    		if(control.name)
	    		{
	    			m_data->m_view.controls[m_data->m_ncontrols].name=new xmlChar[xmlStrlen(control.name)+1];
	    			memcpy(m_data->m_view.controls[m_data->m_ncontrols].name,
	    					control.name,xmlStrlen(control.name)+1);
	    		}
	    		if(control.detail)
	    		{
	    			m_data->m_view.controls[m_data->m_ncontrols].detail=new xmlChar[xmlStrlen(control.detail)+1];
	    			memcpy(m_data->m_view.controls[m_data->m_ncontrols].detail,
	    			control.detail,xmlStrlen(control.detail)+1);
	    		}
	    		if(control.img)
	    		{
	    			m_data->m_view.controls[m_data->m_ncontrols].img=new xmlChar[xmlStrlen(control.img)+1];
	    			memcpy(m_data->m_view.controls[m_data->m_ncontrols].img,
	    			control.img,xmlStrlen(control.img)+1);	
	    		}
	    		
	    		m_data->m_ncontrols++;
	    		if(m_data->m_ncontrols >= MAXCONTROLS)	break;
	    		
	    			
	    	}
	    	//printf("id=%s,name=%s,detail=%s\n",id,control.name,control.detail);
	   
		}
		cur = cur->next;	
    }  
}

tmControl CtmOnlineHelp::parseControl(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur)
{
	tmControl	ret;
	memset(&ret,0,sizeof(tmControl));
	ret.buddy=xmlGetProp(cur,(const xmlChar*)"Buddy");
	cur = cur->xmlChildrenNode;
	while(cur)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "Name")) 
			&&(cur->ns == ns))
	    ret.name = 
			xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "Detail")) &&
	    (cur->ns == ns))
	    ret.detail=
			xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "Image")) &&
	    (cur->ns == ns))
	    ret.img=
			xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
		cur = cur->next;	
	}
	return ret;
}

std::map<int ,tmControl>	CtmOnlineHelp::GetContext(int id)
{
		std::map<int, tmControl>	temp;
		tmControl control;
		if(id<0)
		{
			//temp=m_data->m_controls;
			for(int i=0; i<m_data->m_ncontrols; i++)
			{
				//if(m_data->m_view.controls[i].buddy)	continue;
				temp.insert(std::pair<int,tmControl>(i,m_data->m_view.controls[i]));
				
			}
		}
		std::map<int, tmControl>::iterator it=m_data->m_controls.find(id);
		if(it!=m_data->m_controls.end())
		{
			if(it->second.buddy)
			{
				int buddy=strtoul(reinterpret_cast<char*>(it->second.buddy),NULL,16);
				if(buddy!=ERANGE)
				{
					control=GetControlByID(buddy);
					control.id=it->second.id;
					control.name=it->second.name;
					control.img=it->second.img;
					
				}			
			}
			else
			{
				control=it->second;	
			}
			temp.insert(std::pair<int,tmControl>(it->first,control));	
		}
	return temp;
}




std::map<string ,tmControl>	CtmOnlineHelp::GetContext(char* pszName)
{
		std::map<string, tmControl>	temp;
		tmControl control;
		if(!pszName)
		{
			//temp=m_data->m_controls;
			for(int i=0; i<m_data->m_ncontrols; i++)
			{
				//if(m_data->m_view.controls[i].buddy)	continue;
				string ID(reinterpret_cast<const char*>(m_data->m_view.controls[i].id));
				temp.insert(std::pair<string,tmControl>(ID,m_data->m_view.controls[i]));
				
			}
		}
		else
		{
			std::string ID(pszName);
			std::map<std::string, tmControl>::iterator it=m_data->m_strKey_controls.find(ID);
			if(it!=m_data->m_strKey_controls.end())
			{
				if(it->second.buddy)
				{
					string Buddy(reinterpret_cast<const char*>(it->second.buddy));
					map<string, tmControl>::iterator it_buddy=m_data->m_strKey_controls.find(Buddy);
					if(it_buddy!=m_data->m_strKey_controls.end())
					{
						control.id=it->second.id;
						control.name=it->second.name;
						control.img=it->second.img;
					}
									
				}
				else
				{
					control=it->second;	
				}
				temp.insert(pair<string,tmControl>(it->first,control));	
			}
		}
	return temp;
}


char*	CtmOnlineHelp::GetDetailByID(char* pszName)
{
	
	memset(m_data->m_szdetail,0, sizeof(m_data->m_szdetail)/sizeof(char));
	tmControl control;
	char sparator='\n';
	//char* LabelDetail="Detail: \n";
	int nOffset=0;
	if(pszName)
	{
		string ID(pszName);
		map<std::string, tmControl>::iterator it=m_data->m_strKey_controls.find(ID);
		if(it!=m_data->m_strKey_controls.end())
		{
			if(it->second.buddy)
			{
				string Buddy(reinterpret_cast<const char*>(it->second.buddy));
				map<string, tmControl>::iterator it_buddy=m_data->m_strKey_controls.find(Buddy);
				if(it_buddy!=m_data->m_strKey_controls.end())
				{
					tmControl temp=it_buddy->second;
					control.detail=temp.detail;		
				}	
			}else
			{
				control.detail=it->second.detail;	
			}
			if(control.detail)
			{
				//memcpy(m_data->m_szdetail+nOffset,LabelDetail,strlen(LabelDetail));
				//nOffset+=strlen(LabelDetail);
				if(xmlStrlen(control.detail)>MAXLEN-1)
				{
					return m_data->m_szdetail;
				}
				memcpy(m_data->m_szdetail+nOffset,control.detail,xmlStrlen(control.detail));	
			}
		}
	}
	else
	{
		//std::map<int, tmControl>::iterator it = m_data->m_controls.begin();
		//for(; it!=m_data->m_controls.end();++it)
		for(int i=0; i<m_data->m_ncontrols; i++)
		{
			//control=it->second;
			if(nOffset>MAXLEN-1)	break;
			control=m_data->m_view.controls[i];
			if(control.buddy)	continue;
			//printf("%s \n%s \n%s\n",control.id,control.name,control.detail);
			if(control.detail)
			{
				//memcpy(m_data->m_szdetail+nOffset,LabelDetail,strlen(LabelDetail));
				//nOffset+=strlen(LabelDetail);
				if(xmlStrlen(control.detail)>MAXLEN-1)
				{
					break;
				}
				memcpy(m_data->m_szdetail+nOffset,control.detail,xmlStrlen(control.detail));
				nOffset+=xmlStrlen(control.detail);
				memcpy(m_data->m_szdetail+nOffset,&sparator,1);
				nOffset+=1;
			}
				
		}
		m_data->m_szdetail[nOffset-1]=0;
				
	}
	//printf("%s\n",m_data->m_szdetails);
	return 	m_data->m_szdetail;
	//return reinterpret_cast<char*>(m_data->m_controls[id].detail);
}






