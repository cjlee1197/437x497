/*===========================================================================+
|  Class    : DataProcessSave                                               |
|  Task     : Command Service Routine header file                            |
|----------------------------------------------------------------------------|
|  Compile  : gcc V3.2 -                	                                 |
|  Link     : gcc V3.2 -                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 10/02/2010                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_DATAPROCESSSAVE
#define     D_DATAPROCESSSAVE

#include    "wnd.h"
#include    "task.h"
#include	"tmpackclass.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - CDataProcessSave                             |
+===========================================================================*/
class CDataProcessSave : public CtmPackClass
{
	DECLARE_DYNCREATE(CDataProcessSave)	
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CDataProcessSave();
    ~CDataProcessSave();
public:
 	virtual		int			SetDataPath(char*	pszName)			{return 0;}
	virtual		int			SetDataConfigPath(char*	pszName)		{return 0;}
	virtual		int			SetConfigName(char*	pszName)			{return 0;}
	virtual		int			SetDataFileName(char*	pszName)		{return 0;}
	virtual		int			SetDataFileName_Config(char*	pszName){return 0;}
	virtual		int			GetAllCount()							{return 0;}
	virtual		int			Read(int	nType,int	nIndex, 		int nSize, 	void* pData)				{return 0;}
	virtual		int			ReadS(int	nType,int	nStartIndex,	int nCount, int nSize, 	void* pData)	{return 0;}
	virtual		int			Write(int	nType,int	nIndex, 		int nSize, 	void* pData)				{return 0;}
	virtual		int			WriteS(int	nType,int	nStartIndex,	int nCount, int nSize,	void* pData)	{return 0;}
	virtual		int			SetFileMaxCount(DWORD	dwFileMaxCount)	{return 0;}
	virtual		int			SetMaxCount(DWORD	dwMaxCount)			{return 0;}	
};

extern	CDataProcessSave*	g_pDataProcessSave;

#endif

