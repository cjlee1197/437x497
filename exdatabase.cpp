/*==========================================================================+
|  Class    : exDatabase 			                                        |
|  Task     : exDatabase source file                          				|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James                                                         |
|  Version  : V1.00                                                         |
|  Creation : 2011/11/02                                                    |
|  Revision : 			                                                    |
+==========================================================================*/

#include	"exdatabase.h"


IMPLEMENT_DYNCREATE(CEXDatabase, CtmPackClass)
IMPLEMENT_DYNCREATE(CAustoneXML, CtmPackClass)

CEXDatabase*	g_pExDatabase = NULL;
CAustoneXML*	g_pAustoneXML = NULL;
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|	function	: CEXDatabase()												|
|	remark		: 構造函數													|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
CEXDatabase::CEXDatabase()
{
	//for(int i = 0; i < MAX_EXDBCOUNT; i++)
	//	m_DataHeader.dxDBValue[i] = NULL;
	//m_nIDCount		= 0;
}


/*--------------------------------------------------------------------------+
|	function	: ~CEXDatabase()											|
|	remark		: 析構函數													|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
CEXDatabase::~CEXDatabase()
{
		
}

/*--------------------------------------------------------------------------+
|	function	: Create()													|
|	remark		: 創建daabase													|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
int			CEXDatabase::Create(char*	pszFileName)
{
	
	return 0;	
}

/*--------------------------------------------------------------------------+
|	function	: AddData(EXDBVALUE ExDBValue)								|
|	remark		: 添加database成員											|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
int			CEXDatabase::AddData(EXDBVALUE ExDBValue)
{
	return -1;	
}

/*--------------------------------------------------------------------------+
|	function	: DeleteData(DWORD	dwID)									|
|	remark		: 刪除database成員											|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
int			CEXDatabase::DeleteData(DWORD	dwID)
{
	return -1;	
}

/*--------------------------------------------------------------------------+
|	function	: SetDBValue(DWORD	dwID, void*	pData)						|
|	remark		: 設置資料													|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
int			CEXDatabase::SetDBValue(DWORD	dwID, void*	pData)
{
	return -1;	
}

/*--------------------------------------------------------------------------+
|	function	: GetDBValue(DWORD	dwID)									|
|	remark		: 讀取資料													|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
EXDBVALUE	CEXDatabase::GetDBValue(DWORD	dwID)
{
	
}

/*--------------------------------------------------------------------------+
|	function	: DeleteAll()												|
|	remark		: 刪除所有資料												|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
int			CEXDatabase::DeleteAll()
{
	return 0;	
}

/*--------------------------------------------------------------------------+
|	function	: GetDataHeader()											|
|	remark		: 獲取資料Head列表											|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: 															|
|																			|
|	return 		:															|
|				:															|
+--------------------------------------------------------------------------*/
EXDATAHEADER	CEXDatabase::GetDataHeader()
{
	;
}

CAustoneXML::CAustoneXML()
{
	
}

CAustoneXML::~CAustoneXML()
{
		
}	



