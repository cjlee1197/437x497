/*==========================================================================+
|  Class    : strlist		                                            	|
|  Task     : strlist	head file                          					|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : Fasm		                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2009/11/02                                                   	|
|  Revision : 			                                                    |
+==========================================================================*/
#ifndef	D_DSTRLIST
#define	D_DSTRLIST

#include "common.h"

#include	<vector>
#include	<string>
#include	<iostream>
#include  <fstream>
#include 	<vector>   
#include 	<stdexcept>  

#define		STRINGLIST_LOAD_ONETIME_CNT				64 //4096//32



using namespace std;

class	CtmStringList
{
public:
	CtmStringList(void);
	virtual ~CtmStringList(void);
public:
	//	Inserts an element or a number of elements into the vector at a specified position
	virtual	int						Insert(int nIndex, const char* psz);
	virtual	int						Insert(int nIndex, string* pStr);

	//	Add an element to the end of the vector
	virtual	int						Add(const char* psz);
	virtual	int						Add(string* pstr);

	virtual	const char* 	IndexOf(int nIndex);
	virtual	string* 			IndexOfStr(int nIndex);

	//	Removes an element or a range of elements in a vector from specified positions.
	virtual	int						Del(int nIndex);
	virtual	int						Del();
	virtual	int						Clear();

	virtual	int						Modify(int nIndex, const char* psz);
	virtual	int						Modify(int nIndex, string* pStr);

	//	Returns the number of elements in the vector
	virtual	unsigned int	GetNum();
	virtual	void					SetNum(unsigned int nNum);
	//	File
	virtual	int						SaveToFile(const char* pszFile);
	virtual	int						LoadFromFile(const char* pszFile,int nStartRow =0,BOOL bLoadOnceFlag = TRUE);

	virtual	int						Find(char* psz);
	virtual	int						Find(int nStartIndex, int nEndIndex, char* psz);
	
	string* 							assign(int index);	//20111202 add
	
	//string*								at(int index){return m_Strs.at(index);};
	
	int										AppendStr(int nIndex, char* psz);
protected:

	vector <string*>			m_Strs;
	fstream								m_file;
	int										m_FileLenth;
	int										m_LocalStringCnt;

};

#endif

