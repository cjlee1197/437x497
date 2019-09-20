/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : CParse/CParseObject		   				  					|
|  Task     : CParse/CParseObject header file          			      		|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                   |
|  Link     :G++(GCC) 3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : shousu fan                                                   	|
|  Version  : V1.00                                                         |
|  Creation : 2006                                        	             	|
|  Revision :                                                               |
+==========================================================================*/


#ifndef D_PARSERULE
#define D_PARSERULE
#include <fstream>
#include <string>
#include <iostream>
#include "wnd.h"
#include "propertylist.h"


/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
const int MAX_CONTROL_NUM =512;//512;
const int MAX_STRING_NUM =1024; //1024;

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
class	CtmWnd;
//	一個名稱對應一個類的實例，名稱是類的名稱
typedef struct tagControl_Name
{
	char* 	controlName;
	CtmWnd* control;
} Control_Name;

/*==========================================================================+
|           Class declaration - CParse	                                	|
// 通用的解析規則
+==========================================================================*/
class CParse
{
	/*--------------------------------------------------------------------------+
	|           Constructor and destructor                                      |
	+--------------------------------------------------------------------------*/
public:
	CParse();
	virtual ~CParse();
		
	/*--------------------------------------------------------------------------+
	|           Operations                                                      |
	+--------------------------------------------------------------------------*/	
public:	
	virtual	bool OpenFile(char* psFileName);
	void	CloseFile();
	int 	ParseLine(char* psKey, char* Value, char Separate = '=');	
	
	FILE*	pFile;
protected:	
	int 	GetOneLine(char* pszValue, bool isVaild = true);
	virtual bool ReduceLine(char* psLine, char* psKey, char* pszValue, char Separate = '=');
	virtual char* LTrim(char* pszValue);	//	去除左邊的空格
	virtual bool RTrim(char* pszValue);	//	去除右邊的空格
	
	/*--------------------------------------------------------------------------+
	|           Attributes                                                      |
	+--------------------------------------------------------------------------*/
protected:
	//fstream m_file;
	BOOL	m_bBinary;
private:
	bool isFileOpen;
};


/*==========================================================================+
|           Class declaration - CParseObject	                            |
// 解析對象的
+==========================================================================*/
class CParseObject: public CParse
{
	
	/*--------------------------------------------------------------------------+
	|           Constructor and destructor                                      |
	+--------------------------------------------------------------------------*/
public:
	CParseObject();
	~CParseObject();	
	
	/*--------------------------------------------------------------------------+
	|           Operations                                                      |
	+--------------------------------------------------------------------------*/
public:
	bool	OpenFile(char* psFileName);
	int		ParseObject(char DefineSymbol = ':', char Separate = '=', char* pszEndStr = "end");
	int		ParseObject_Bin(char cEndStr = 0xFF);
	int		ParseObject_Bin2(char cEndStr = 0x00);
	int		ParseObject_Bin3(char cEndStr = 0x00);
	CtmWnd*	GetMainControl();
	CtmWnd*	GetControl(int index);
	void 	ReInitData();
protected:
	void* 	CreateObject(char* pszName);
private:	
	void 	InitData();
	CtmWnd* FindParent(int iStartIndex, int iLevel);
	bool 	IsNum(char* pszExp);

	/*--------------------------------------------------------------------------+
	|           Attributes                                                      |
	+--------------------------------------------------------------------------*/	
public:
	Control_Name m_aryControls[MAX_CONTROL_NUM];
	int		GetControlCount() {return m_iControlCount;};
	bool 	SetPropertyInfo(CtmWnd* pwndSender, char* PropertyName, void* Value);
	bool 	SetPropertyInfo_bin(CtmWnd* pwndSender, char* PropertyName, void* Value);
	bool 	SetPropertyInfo_bin(CtmWnd* pwndSender, DWORD dwIndex, void* Value);
private:
	int 	m_aryLevel[MAX_CONTROL_NUM];	//	
	int		m_iControlCount;
	pthread_mutex_t 	m_pthreadParseFileLock;
	pthread_mutex_t		m_pthreadInitFileLock;

};

#endif
