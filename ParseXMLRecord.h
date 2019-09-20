/*==========================================================================+
|  Class    : ParseXMLRecord 			                                    |
|  Task     : ParseXMLRecord head file                          			|
|---------------------------------------------------------------------------|
|  Compile  : G++ V4.7.3 -                                                  |
|  Link     : G++ V4.7.3 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James                                                         |
|  Version  : V1.00                                                         |
|  Creation : 2015/04/26                                                    |
|  Revision : 			                                                    |
+==========================================================================*/
#ifndef D_PARSEXMLRecord
#define D_PARSEXMLRecord

#include	"common.h"
#include 	"string.h"
#include	"encode.h"
#include 	<libxml/parser.h>
#include 	<libxml/tree.h>
//#include "lib/hmi_group/tmParseXML.h"

typedef	struct	tagSTRINGUNICODE
{
	int		nLen;
	int		nUTFLen;
	DWORD*	pdwStrUnicode;
	char*	putf8code;	
}STRINGUNICODE;

#define		XML_MAXSTRING		50000
//class Strings;
class CtmParseXMLRecord
{
//	DECLARE_DYNCREATE(CtmParseXMLRecord);

public:
	CtmParseXMLRecord();
	~CtmParseXMLRecord();
	
public:
//	CParseXML 		m_ParseXML;
//	CParseXML* 		m_pParseXML;
	STRINGUNICODE*	m_pStrUnicode;						//record string ID
	char*			m_pszRecordDataID[XML_MAXSTRING];			//record DataID
	
	int				m_nCount;
	int				m_nLength;
	
	int				LoadXMLData(char* pszFileName);
	int				GetRecordIndex(long lDataID);
	int 			GetRecordIndex(char* pszStringID);
	char*			GetRecordStrID(WORD wIndex) {return wIndex<XML_MAXSTRING? m_pszRecordDataID[wIndex]:0 ;}
	int				CalculateCount(char* pszFileName);
	STRINGUNICODE	GetRecordStr(int nIndex);
	STRINGUNICODE*	GetRecordList()		{return m_pStrUnicode;}
private:
	class 	CStrings*		m_pIDList;
	
};

extern CtmParseXMLRecord* 	g_pParseXMLRecord;
extern CtmParseXMLRecord* 	g_pParseXMLViewStr;

#endif
