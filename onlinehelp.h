/*
*	Created by J.Wong 2016/3/16 14:41:10
*/

#ifndef		D_ONLINEHELP
#define		D_ONLINEHELP
/*
*	解析xml格式的帮助文档
*	帮助文档布局: title--[ID--Name--Detail--ID--Name--Detail....]
*/

#include "libxmlcomm.h"
#include <map>
#include <string>

const int		MAXCONTROLS=256;



typedef struct tagtmView{
	xmlChar*		title;
	xmlChar*		footer;
	tmControl		controls[MAXCONTROLS]; 
}tmView,*tmViewPtr;

class		CtmOnlineHelp{
	public:
		CtmOnlineHelp (const char* xmlFile=0);
		~CtmOnlineHelp();
		void	OpenXmlFile(const char* xmlFile=0);
		char*	GetTitle();
		char*	GetFooter();
		char*	GetID(int nIndex);
		char*	GetName(int nIndex);
		char*	GetDetail(int nIndex);
		char*	GetImg(int nIndex);
		char*	GetNameByID(int id);
		char*	GetDetailByID(int id);
		char*	GetDetailByID(char* pszName);
		char*	GetImgByID(int id);
		std::map<int ,tmControl>	GetContext(int id);
		std::map<std::string,tmControl>	GetContext(char* pszName);
		tmControl GetControlByID(int id);
		
		char*	GetDetailsByID(int id);
		
		
	private:
		void			parseHeadInfo();
		tmControl 		parseControl(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur);
	private:
		class CtmPrivateData;
		CtmPrivateData*		m_data;
};
#endif