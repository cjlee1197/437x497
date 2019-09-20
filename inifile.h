/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : CIniFile					   				  					|
|  Task     : CIniFile header file                   			      		|
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


#ifndef D_CINIFILE_H
#define D_CINIFILE_H

#include	<fstream>
#include	<string>
#include 	<iostream>
using namespace std;


/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
const int MAXSTRINGLEN = 1024;


/*==========================================================================+
|           Class declaration - CIniFile                                	|
// CIniFile stores and retrieves application-specific information and settings
// from INI files.
+==========================================================================*/
class CIniFile
{	
	/*--------------------------------------------------------------------------+
	|           Constructor and destructor                                      |
	+--------------------------------------------------------------------------*/
  public:  	
  	CIniFile();
    CIniFile(const char* pszFileName);    
    virtual ~CIniFile();
  	/*--------------------------------------------------------------------------+
	|           Operations                                                      |
	+--------------------------------------------------------------------------*/
  public:
    // 標記讀取值得時候,是讀取整行的,還是讀取ID,還是Value
    enum 	tmIniValueType {tmLine, tmKey, tmValue };
    
    
    
    void 	DeleteKey(const char* pszSection, const char* pszIdent);    
    void 	EraseSection(const char* pszSection);
    int		ReadSection(const char* pszSection, char** pszOutIdents, int iNum = 0);
    int 	ReadString(const char* pszSection, const char* pszIdent, const char* pszDefault, 
    				   char** pszValue);
    int		ReadInteger(const char* pszSection, const char* pszIdent, const int nDefault);
    void 	WriteString(const char* pszSection, const char* pszIdent, const char* pszValue);   
    void	WriteInteger( const char* pszSection, const char* pszIdent, const int nValue);
    bool 	OpenFile(const char* pszFileName);
    void 	CloseFile();   
    int 	ReadSectionValues(const char* pszSection, char** pszValues,
        					  int iValueNum = 0);
    int		CheckVaildSection(const char*	pszSection, const char* pszIdet);
	void    SetValueCode(int nCode)     {m_nValueCode = nCode;}
  protected:    
    virtual int 	GetOneVaildLine(char* pszValue);
    virtual int 	GetVaildLines(const char* pszSection, char** pszValues, 
        					  tmIniValueType ValueType = tmValue, 
        					  int iValueNum = 0);
    virtual char* 	DealLine(char* pszOldStr, char* pszNewStr, tmIniValueType DealType);
        
        
	/*--------------------------------------------------------------------------+
	|           Attributes                                                      |
	+--------------------------------------------------------------------------*/
  private:
    bool m_IsClosed;
    int     m_nValueCode;       //netdigger 2007/4/11
    fstream m_IniFile;

};



#endif 
