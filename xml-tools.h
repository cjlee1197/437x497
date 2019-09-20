/*Created by J.Wong 2016/11/20 17:57:11*/
/*function :  read and write xml by use library xml2 */

#pragma once  
#include <stdio.h>  
#include <stdlib.h>  
#include <libxml/parser.h>   
#include <libxml/tree.h>  
#include <map>
#include <vector>  
#include <string>   
#include <iostream>  

using namespace std;  

 
  
class CXmlNode
{
	public:
		CXmlNode();
		CXmlNode(string name, string text);
		CXmlNode(string name, string text, vector<pair<string, string> > attr);
		~CXmlNode();
		
		int 	attrCount()const;
		int 	size()const;
		pair<string, string> find(int index)const;
		pair<string, string>& find(int index);
		string 	find(string attr)const;
		string& find(string attr);
//		string 	find(int index)const;
		void   	append(string attr, string text);
		void   	append(vector<pair<string, string> >attr);
		
		void   	appendChild(const CXmlNode& );
		const 	CXmlNode& findChild(int index)const;
		CXmlNode&	findChild(int index);
		
		void   setName(string name);
		void   setText(string text);
		string text()const;
		string name()const;
		
		const CXmlNode& operator[](int i)const;
		
		
	private:
		string m_text;
		string m_name;
		vector<pair<string, string> > node_attr;
		vector<CXmlNode> nodes;
		friend class CXml;
};

class CXml{
	
	public:
		enum Mode{
			XML_READ,
			XML_WRITE	
		};
		
		CXml();  
    	~CXml();  
  
    	CXml(const char *xml_file_path, Mode mode=XML_READ);
    	bool open(const char *xml_file_path, Mode mode=XML_READ);
    	bool parse_xml_file();
    	bool save_xml_file(const CXmlNode &xml_node);
    	
    	CXmlNode&	rootNode(){return m_root;}
    	const CXmlNode& rootNode()const{return m_root;}
    	
  private: 
  		CXmlNode m_root;

  		xmlNodePtr search_node_ptr(const char *sz_expr);
  		void addChildNode(xmlNodePtr nodePtr,const CXmlNode& node, int level);
  		void getChildNode(xmlNodePtr nodePtr, CXmlNode& node, int level);
  		char m_sz_path[512];  
    	xmlDocPtr m_pdoc_read;  
    	xmlNodePtr m_proot;   
};
 

