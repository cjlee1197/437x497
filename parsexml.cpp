/*==========================================================================+
|  Class    : parsexml 			                                        	|
|  Task     : parsexml source file                          				|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James                                                         |
|  Version  : V1.00                                                         |
|  Creation : 2011/11/03                                                    |
|  Revision : 			                                                    |
+==========================================================================*/

#include	"parsexml.h"


IMPLEMENT_DYNCREATE(CParseXML, CtmPackClass)

//CParseXML*	g_pParseXml = NULL;
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|	function	: CParseXML()												|
|	remark		: 構造函數													|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
CParseXML::CParseXML()
{
	m_ParseXML = this;
}


/*--------------------------------------------------------------------------+
|	function	: ~CParseXML()											|
|	remark		: 析構函數													|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
CParseXML::~CParseXML()
{
		
}

int					CParseXML::InitParse(CParseXML* pParseXML)
{
	if(pParseXML != NULL)
		m_ParseXML = pParseXML;
	else
		m_ParseXML = this;
}

/*--------------------------------------------------------------------------+
|	function	: Open()													|
|	remark		: 打開xml文件												|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
void*				CParseXML::Open(char*	pszFileName)
{
	if(m_ParseXML != NULL)
		return m_ParseXML->Open(pszFileName);
	else
		return NULL;
}

int				CParseXML::Close()
{
	if(m_ParseXML != NULL)
		return m_ParseXML->Close();	
	else
		return NULL;
}

void*		CParseXML::GetRootNode()
{
	if(m_ParseXML != NULL)
		return m_ParseXML->GetRootNode();
	else
		return NULL;
}

void*		CParseXML::GetProper(char* pszProp)
{
	if(m_ParseXML != NULL)
		return m_ParseXML->GetProper(pszProp);
	else
		return NULL;

}

void*		CParseXML::FreeProper(void* pszAttr)
{
	if(m_ParseXML != NULL)
		return m_ParseXML->FreeProper(pszAttr);
	else
		return NULL;
}

void*		CParseXML::GetChildNode()
{
	if(m_ParseXML != NULL)
		return m_ParseXML->GetChildNode();
	else
		return NULL;
}

void*		CParseXML::GetNextNode()
{
	if(m_ParseXML != NULL)
		return m_ParseXML->GetNextNode();
	else
		return NULL;
}

void*		CParseXML::GetPrevNode()
{
	if(m_ParseXML != NULL)
		return m_ParseXML->GetPrevNode();
	else
		return NULL;
}





