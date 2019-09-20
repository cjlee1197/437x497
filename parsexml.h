/*==========================================================================+
|  Class    : parsexml	 			                                        |
|  Task     : parsexml header file                          				|
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
#ifndef		D_PARSEXML
#define		D_PARSEXML

#include	"common.h"
#include	"files.h"
#include	"strings.h"
#include	"database.h"
#include 	<libxml/parser.h>
#include 	<libxml/tree.h>

//
//struct _xmlNode {
//    void           *_private;	/* application data */
//    xmlElementType   type;		/* type number, must be second ! */
//    const xmlChar   *name;      /* the name of the node, or the entity */
//    struct _xmlNode *children;	/* parent->childs link */
//    struct _xmlNode *last;	/* last child link */
//    struct _xmlNode *parent;	/* child->parent link */
//    struct _xmlNode *next;	/* next sibling link  */
//    struct _xmlNode *prev;	/* previous sibling link  */
//    struct _xmlDoc  *doc;	/* the containing document */
//
//    /* End of common part */
//    xmlNs           *ns;        /* pointer to the associated namespace */
//    xmlChar         *content;   /* the content */
//    struct _xmlAttr *properties;/* properties list */
//    xmlNs           *nsDef;     /* namespace definitions on this node */
//    void            *psvi;	/* for type/PSVI informations */
//    unsigned short   line;	/* line number */
//    unsigned short   extra;	/* extra data for XPath/XSLT */
//};

//struct _xmlDoc {
//    void           *_private;	/* application data */
//    xmlElementType  type;       /* XML_DOCUMENT_NODE, must be second ! */
//    char           *name;	/* name/filename/URI of the document */
//    struct _xmlNode *children;	/* the document tree */
//    struct _xmlNode *last;	/* last child link */
//    struct _xmlNode *parent;	/* child->parent link */
//    struct _xmlNode *next;	/* next sibling link  */
//    struct _xmlNode *prev;	/* previous sibling link  */
//    struct _xmlDoc  *doc;	/* autoreference to itself */
//
//    /* End of common part */
//    int             compression;/* level of zlib compression */
//    int             standalone; /* standalone document (no external refs) 
//				     1 if standalone="yes"
//				     0 if standalone="no"
//				    -1 if there is no XML declaration
//				    -2 if there is an XML declaration, but no
//					standalone attribute was specified */
//    struct _xmlDtd  *intSubset;	/* the document internal subset */
//    struct _xmlDtd  *extSubset;	/* the document external subset */
//    struct _xmlNs   *oldNs;	/* Global namespace, the old way */
//    const xmlChar  *version;	/* the XML version string */
//    const xmlChar  *encoding;   /* external initial encoding, if any */
//    void           *ids;        /* Hash table for ID attributes if any */
//    void           *refs;       /* Hash table for IDREFs attributes if any */
//    const xmlChar  *URL;	/* The URI for that document */
//    int             charset;    /* encoding of the in-memory content
//				   actually an xmlCharEncoding */
//    struct _xmlDict *dict;      /* dict used to allocate names or NULL */
//    void           *psvi;	/* for type/PSVI informations */
//};

class	CParseXML: public CtmPackClass
{
	DECLARE_DYNCREATE(CParseXML);
public:
	CParseXML();
	~CParseXML();
public:
	virtual	int							InitParse(CParseXML* pParseXML);
	virtual	void*	/*xmlDocPtr*/		Open(char*	pszFileName);
	virtual	int							Close();
	virtual	void*	/*xmlNodePtr*/		GetRootNode();
	virtual	void*	/*xmlChar**/		GetProper(char* pszProp);
	virtual	void*	/*xmlChar**/		FreeProper(void* pszAttr);
	virtual	void*	/*xmlNodePtr*/		GetChildNode();
	virtual	void*	/*xmlNodePtr*/		GetNextNode();
	virtual	void*	/*xmlNodePtr*/		GetPrevNode();
public:
	CParseXML*	m_ParseXML;
};

//extern	CParseXML*	g_pParseXml;

#endif
