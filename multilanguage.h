/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : CMulStrings					   				  					|
|  Task     : CMulStrings header file                   			      		|
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



#ifndef D_CMULTILANGUAGE_H
#define D_CMULTILANGUAGE_H

#include "inifile.h"
#include "strclass.h"
#include "strings.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
const int MULSTR_MAXNUM = 20000;

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagMULSTRINFO
{
	char* strDir;		//	�h��y���~�񪺥ؿ�
	char* strFileName;	//	���W��
    int     nValueCode;     //netdigger 2007/4/11
}	MULSTRINFO;


/*==========================================================================+
|           Class declaration - CMulStrings                                	|
+==========================================================================*/
class CMulStrings
{
	/*--------------------------------------------------------------------------+
	|           Constructor and destructor                                      |
	+--------------------------------------------------------------------------*/
  public:
  	CMulStrings(MULSTRINFO* pInfo);
    CMulStrings();
    virtual ~CMulStrings();
    
    /*--------------------------------------------------------------------------+
	|           Operations                                                      |
	+--------------------------------------------------------------------------*/
  public:    
    void AppendString(char* pszKey, char* pszValue);	// �Nkey�ȩMValue�ȲK�[��hash��    
    char* GetString(char* pszKey);						// �ھ�key�ȱo��Value    
    char*	GetString(int	nIndex);
    char* operator[] (char* pszKey);					// ����[]    
    char*	operator[] (int nIndex);
    int ReadMulStrings(MULSTRINFO* pInfo);
    
    int	GetStrKeyIndex(char* pszKey);					//	�ھ�key�ȱo��key�ұƦC��index
    //int	
  private:
    void InitData();
    
  	/*--------------------------------------------------------------------------+
	|           Attributes                                                      |
	+--------------------------------------------------------------------------*/
  private:
    CIniFile 	m_strFile;
//    char		m_szOut[1024];
    CtmStrings* m_pStrs;
    char* 		m_pszMulStr[MULSTR_MAXNUM];			//	�r�ꤺ�e
    char* 		m_pszMulStrID[MULSTR_MAXNUM];			//	�r��ID
   

};

//	fans add 2007/3/21 05:57�U��
/*==========================================================================+
|           Class declaration - CtmMultiLanguage                          	|
+==========================================================================*/
class CtmMultiLanguage
{
	/*--------------------------------------------------------------------------+
	|           Constructor and destructor                                      |
	+--------------------------------------------------------------------------*/
  public:
  	CtmMultiLanguage(MULSTRINFO* pInfo);
    CtmMultiLanguage();
    virtual ~CtmMultiLanguage();
    
    /*--------------------------------------------------------------------------+
	|           Operations                                                      |
	+--------------------------------------------------------------------------*/
  public:    
    void 	AppendString(char* pszKey, char* pszValue);	// �Nkey�ȩMValue�ȲK�[��hash��    
    char* 	GetString(char* pszKey);						// �ھ�key�ȱo��Value    
    char*	GetString(int	nIndex);
    char*	GetStringID(int	nIndex);					// James add 2011/12/29 �W�� 11:18:09
    char* 	operator[] (char* pszKey);					// ����[]    
    char*	operator[] (int nIndex);
 	int		SetMultiCode(int nCode=0);	
    int 	ReadMulStrings(MULSTRINFO* pInfo);
    BOOL	SetMulStr(char*	pszKey,char*pszMulStr);		//James add 2010/10/26 for change string context
    int		GetStrKeyIndex(char* pszKey);				//	�ھ�key�ȱo��key�ұƦC��index
    //int	
  private:
    void 	InitData();
    
  	/*--------------------------------------------------------------------------+
	|           Attributes                                                      |
	+--------------------------------------------------------------------------*/
  private:
    CIniFile 	m_strFile;
//    char		m_szOut[1024];
    CtmHashTable* m_pStrs;
    int			m_nCode;
    //CtmHashTable* m_pStrsValueToKey;
    char* 		m_pszMulStr[MULSTR_MAXNUM];				//	�r�ꤺ�e
    char* 		m_pszMulStrID[MULSTR_MAXNUM];			//	�r��ID
   	tmHashElement	m_ElementOut;

};


#endif 
