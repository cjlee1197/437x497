#include "xml-tools.h"  
#include <string.h>  
#include <libxml/xpath.h>  
#include <libxml/xpathInternals.h>  
#include <libxml/xmlmemory.h>  
#include <libxml/xpointer.h>   
  
CXmlNode nullNodes;  
pair<string, string> nullAttr=pair<string, string>(string(), string());
string		nullStr;
CXmlNode::CXmlNode()
:m_name("Item"),nodes(vector<CXmlNode>()),node_attr(vector<pair<string, string> >())
{
	
}

CXmlNode::CXmlNode(string name, string text)
:m_name(name),m_text(text),nodes(vector<CXmlNode>()),node_attr(vector<pair<string, string> >())
{
	
}


CXmlNode::~CXmlNode()
{
	
} 


int 	CXmlNode::attrCount()const
{
	return node_attr.size();
}

int 	CXmlNode::size()const
{
	return nodes.size();
}

pair<string, string> CXmlNode::find(int index)const
{
	if( index<0||index>=attrCount()) return pair<string, string>(string(), string());
	return node_attr[index];	
}

pair<string, string>& CXmlNode::find(int index)
{
	if( index<0||index>=attrCount()) return nullAttr;
	return node_attr[index];	
}

string& CXmlNode::find(string attr)
{
	vector<pair<string, string> >::iterator it=node_attr.begin();
	for(; it!=node_attr.end(); ++it )
	{
		if(it->first==attr)
			return it->second;
	}
	return nullStr;	
}
		
string 	CXmlNode::find(string attr)const
{
	vector<pair<string, string> >::const_iterator it=node_attr.begin();
	for(; it!=node_attr.end(); ++it )
	{
		if(it->first==attr)
			return it->second;
	}
	return string();
}

//string 	CXmlNode::find(int index)const
//{
//	if( index<0||index>=attrCount()) return string();
//	
//	return node_attr[index]->second;
//}

void   	CXmlNode::append(string attr, string text)
{
	node_attr.push_back(pair<string, string>(attr, text));
}

void   	CXmlNode::append(vector<pair<string, string> >attr)
{
	if(attr.empty())	return;
	node_attr.insert(node_attr.end(), attr.begin(), attr.end());
}
		
void   	CXmlNode::appendChild(const CXmlNode& node)
{
	nodes.push_back(node);
}

const 	CXmlNode& 	CXmlNode::findChild(int index)const
{
	if(index<0||index>=size())	return nullNodes;
	return nodes[index];
}

CXmlNode&	CXmlNode::findChild(int index)
{
	if(index<0||index>=size())	return nullNodes;
	return nodes[index];
}
		
string 	CXmlNode::text()const
{
	return m_text;
}

string 	CXmlNode::name()const
{
	return m_name;
}

void   CXmlNode::setName(string name)
{
	m_name=name;
}

void   CXmlNode::setText(string text)
{
	m_text= text;
}

const CXmlNode& CXmlNode::operator[](int i)const
{
	if(i<0||i>=size())	return nullNodes;
	return nodes[i];	
}

CXml::CXml()  
{  
    m_pdoc_read = NULL;  
    m_proot = NULL;  
    bzero(m_sz_path, sizeof(m_sz_path));  
} 

 
CXml::~CXml()  
{  
    if (m_pdoc_read)   
    {  
        xmlFreeDoc(m_pdoc_read);  
        m_pdoc_read = NULL;  
  
        xmlCleanupParser();  
        xmlMemoryDump();  
    }  
}  

CXml::CXml(const char *xml_file_path, Mode mode)  
{  
    if (xml_file_path)   
    {  
        open(xml_file_path, mode);  
    }  
}


bool CXml::open(const char *xml_file_path, Mode mode)
{
	bool bret =  false;  
      
    m_pdoc_read = NULL;  
    m_proot = NULL;  
    bzero(m_sz_path, sizeof(m_sz_path));  
      
    if (xml_file_path)   
    {  
        strcpy(m_sz_path, xml_file_path);  
  
        if (mode==XML_READ)   
        {  
            xmlKeepBlanksDefault(0);  
            m_pdoc_read = xmlReadFile(xml_file_path,  "UTF-8", XML_PARSE_RECOVER);  
            m_proot = xmlDocGetRootElement(m_pdoc_read);   
        }  
          
        if (m_proot)  
        {  
            bret = true;  
        }  
    }  
  
    return bret;  
}


xmlNodePtr CXml::search_node_ptr(const char *sz_expr)  
{  
    xmlNodePtr node_ret;  
      
    if (sz_expr == NULL)  
    {  
        return NULL;  
    }  
      
    xmlChar *sz_path = BAD_CAST(sz_expr);  
    xmlXPathContextPtr context = xmlXPathNewContext(m_pdoc_read);   
    xmlXPathObjectPtr result = xmlXPathEvalExpression(sz_path, context);  
      
    if (result == NULL)  
    {  
        return NULL;   
    }  
    if (xmlXPathNodeSetIsEmpty(result->nodesetval))  
    {  
        return NULL;   
    }  
  
    xmlXPathFreeContext(context);  
      
    node_ret = xmlXPtrBuildNodeList(result);  
  
    return node_ret;  
}


void CXml::getChildNode(xmlNodePtr nodePtr, CXmlNode& node, int level)
{
	if(nodePtr)
	{
		node.setName(string((const char*)nodePtr->name));
		xmlAttrPtr  attrPtr= nodePtr->properties;
		while(attrPtr){
			string prop=(const char*)xmlGetProp(nodePtr,attrPtr->name)?string((const char*)xmlGetProp(nodePtr,attrPtr->name)):string();
        	node.append(string((const char*)attrPtr->name),prop);
        	attrPtr=attrPtr->next;
		}
		
        if(--level>=0)
        {
        	xmlNodePtr child=nodePtr->xmlChildrenNode;
	        while(child)
	       	{
	       		if(child->type==XML_ELEMENT_NODE)
	       		{
	       			CXmlNode node1;
	       	 		getChildNode(child,node1,level);
	        		node.appendChild(node1);
	        	}
	        	else if(child->type==XML_TEXT_NODE)
	        	{
	        		string text=(const char* )xmlNodeGetContent(nodePtr)?string((const char* )xmlNodeGetContent(nodePtr)):string();
        			node.setText(text);
	        	}
	        	child=child->next;
	       }
	    }	
	}		
}


bool CXml::parse_xml_file() 
{  
    bool bret =  false;  
    if (m_proot)  
    {  
    	m_root.setName(string((const char*)m_proot->name? (const char*)m_proot->name : string()));
    	xmlAttrPtr  attrPtr= m_proot->properties;
    	while(attrPtr)
    	{
        	string prop=(const char*)xmlGetProp(m_proot,attrPtr->name)?string((const char*)xmlGetProp(m_proot,attrPtr->name)):string();
        	m_root.append(string((const char*)attrPtr->name),prop);
        	attrPtr=attrPtr->next;
        }
        
        xmlNodePtr child=m_proot->children;
        while(child)
       {
       	 	if(child->type==XML_ELEMENT_NODE)
	       	{
	       		CXmlNode node;	
	       	 	getChildNode(child,node,8);
	        	m_root.appendChild(node);
	        }
	        else if(child->type==XML_TEXT_NODE)
	        {
	        	string text=(const char* )xmlNodeGetContent(m_proot)?string((const char* )xmlNodeGetContent(m_proot)):string();
        		m_root.setText(text);
	        }
        	child=child->next;
       }
        bret = true;  
    }  
  
    return bret;   
}  


void CXml::addChildNode(xmlNodePtr nodePtr,const CXmlNode& node, int level)
{
	
	xmlNodePtr  child= xmlNewNode(NULL, BAD_CAST(node.name().c_str()));
	vector<pair<string, string> >::const_iterator it=node.node_attr.begin();
    if(node.size()==0)	
    {
    	xmlNodePtr  content =xmlNewText(BAD_CAST(node.text().c_str()));
		xmlAddChild(child,content);
	}
    for(; it!=node.node_attr.end(); ++it)
    {
    	xmlNewProp(child,BAD_CAST(it->first.c_str()),BAD_CAST(it->second.c_str()));	
    }
    xmlAddChild(nodePtr,child);
    if(--level>=0)
    {
		for(int i=0; i<node.size(); ++i)
		{
	   	 		addChildNode(child,node[i],level);
		}
    }
   return ;	
}

bool CXml::save_xml_file(const CXmlNode &xml_node)  
{  
    if (NULL == m_sz_path)  
    {  
        return false;  
    }  
  
    char sz_temp[32];  
    m_root=xml_node;
    xmlDocPtr pdoc = xmlNewDoc(BAD_CAST"1.0");
    xmlNodePtr root=xmlNewNode(NULL, BAD_CAST(m_root.name().c_str()));
    if(m_root.size()==0)
    {
    	xmlNodePtr  content =xmlNewText(BAD_CAST(m_root.text().c_str()));
    	xmlAddChild(root,content);
    }
    
    vector<pair<string, string> >::const_iterator it=m_root.node_attr.begin();
    for(; it!=m_root.node_attr.end(); ++it)
    {
    	xmlNewProp(root,BAD_CAST(it->first.c_str()),BAD_CAST(it->second.c_str()));	
    }
    
	xmlDocSetRootElement(pdoc, root);
	for(int i=0; i<m_root.size(); ++i)
	{
		addChildNode(root,m_root[i],8);
	}
    
    xmlSaveFormatFileEnc(m_sz_path, pdoc,  "UTF-8", 1);  
    xmlFreeDoc(pdoc);  
    return true;  
}





#if 0 
//////////²âÊÔ////////////////////////// 
int main(void) 
{ 
	CXml lib("rmc1.xml",CXml::XML_WRITE);
	CXmlNode root;
	root.setName("root");
	root.append("website","http://www.techmation.com");
	root.append("attr","read");
	CXmlNode list;
	list.setName("item_list");
	
	CXmlNode item;
	item.setName("item");
	item.setText("item1");
	item.append("id","3456");
	item.append("attr","write");
	list.appendChild(item);
	
	CXmlNode item2;
	item2.setName("item");
	item2.setText("item1");
	item2.append("id","3456");
	item2.append("attr","write");
	list.appendChild(item2);
	
	CXmlNode item3;
	item3.setName("item");
	item3.setText("item3");
	list.appendChild(item3);
	CXmlNode item4;
	item4.setName("item");
	item4.setText("item4");
	list.appendChild(item4);
	CXmlNode item5;
	item5.setName("item");
	item5.setText("item5");
	list.appendChild(item5);
	CXmlNode item6;
	item6.setName("item");
	item6.setText("item6");
	list.appendChild(item6);
	
	root.appendChild(list);
	root.appendChild(list);
	
	lib.save_xml_file(root); 
    //read///////////////////////////////////// 
     CXmlNode xml_info; 
   	 CXml readLib("rmc1.xml", CXml::XML_READ); 
     readLib.parse_xml_file();
     xml_info=readLib.rootNode();
     std::cout<<xml_info.name()<<" : "<<xml_info.text()<<std::endl; 
     std::cout<<"size:"<<xml_info.size()<<std::endl;
     for(int j=0; j<xml_info.size(); j++)
     {
	     for(int i=0; i<xml_info[j].size();i++)
	     {
	     	CXmlNode node=xml_info[0].findChild(i);
	     	std::cout<<node.name()<<" : "<<node.text()<<std::endl;
	     	for(int i=0; i<node.attrCount();i++)
	     	{
	     		std::cout<<node.find(i).first<<" : "<<node.find(i).second<<std::endl;	
	     	}
	     }
	  }

    //////////////////////////////////////////// 
	
   
    return 0; 
} 
#endif