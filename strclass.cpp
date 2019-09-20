/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmStrings			                                             |
|  Task     : Manager string				                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sh.su.fan                                                      |
|  Version  : V1.00                                                          |
|  Creation :                                                                |
|  Revision : 09/19/2003   Jeckey                                            |
+===========================================================================*/

#include 	<iostream>
#include 	"strclass.h"

using namespace std;


CtmStrings::CtmStrings()
{
	m_StrTotalNum = 0;
}



CtmStrings::~CtmStrings()
{
	

}

/*---------------------------------------------------------------------------+
|  Function :              AddString	                                     |
|  Task     :	添加字串到列表中去，需要注意的是這個字串不能被用戶釋放       |
|				且不能與列表中的字串重復，本函數並不保證這一點				 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  返回值標記是第幾個，從0開始     	                             |
+---------------------------------------------------------------------------*/
//	
int CtmStrings::AddString(char* psz)
{
	m_indexMap[psz] = ++m_StrTotalNum;
	m_strMap[m_StrTotalNum-1] = psz;
	return m_StrTotalNum - 1;
}

/*---------------------------------------------------------------------------+
|  Function :              GetIndex	       		                             |
|  Task     :  根據給定的字串，獲取其在列表中的位置
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  返回值標記是第幾個，如果沒有的話，返回值為-1                  |
+---------------------------------------------------------------------------*/
int CtmStrings::GetIndex(char* psz)
{		
	//printf("string total:%d\n", m_indexMap.bucket_count());
	return m_indexMap[psz] - 1;
}


/*---------------------------------------------------------------------------+
|  Function :              GetString       		                             |
|  Task     :  根據位置，列表中的字串										 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  如果不在列表中，返回NULL						                 |
+---------------------------------------------------------------------------*/
char* CtmStrings::GetString(int nIndex)
{
	
	if ((nIndex < 0) && (nIndex >= m_StrTotalNum)) return NULL;
	return m_strMap[nIndex];

}



