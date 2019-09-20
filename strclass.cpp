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
|  Task     :	�K�[�r���C���h�A�ݭn�`�N���O�o�Ӧr�ꤣ��Q�Τ�����       |
|				�B����P�C�����r�꭫�_�A����ƨä��O�ҳo�@�I				 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  ��^�ȼаO�O�ĴX�ӡA�q0�}�l     	                             |
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
|  Task     :  �ھڵ��w���r��A�����b�C������m
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  ��^�ȼаO�O�ĴX�ӡA�p�G�S�����ܡA��^�Ȭ�-1                  |
+---------------------------------------------------------------------------*/
int CtmStrings::GetIndex(char* psz)
{		
	//printf("string total:%d\n", m_indexMap.bucket_count());
	return m_indexMap[psz] - 1;
}


/*---------------------------------------------------------------------------+
|  Function :              GetString       		                             |
|  Task     :  �ھڦ�m�A�C�����r��										 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  �p�G���b�C���A��^NULL						                 |
+---------------------------------------------------------------------------*/
char* CtmStrings::GetString(int nIndex)
{
	
	if ((nIndex < 0) && (nIndex >= m_StrTotalNum)) return NULL;
	return m_strMap[nIndex];

}



