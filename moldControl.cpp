
#include	"moldControl.h"
#include	"tmconfig.h"
#include	"common.h"
#include	"machinefactor.h"
#include	"main.h"
#include	"commonaction.h"
#include    "tmdebug.h"

BOOL	g_bFindMoldSetMsg = TRUE;
//CtmBlock*			g_pBlock;
CtmMoldSet*	CtmMoldSet::m_pInstance = NULL;
int 		CtmBlockControl::m_nCount = 0;
	
CtmBlockControl* CtmBlockControl::m_pInstance = NULL;

 /*! \fn CtmMoldSet::CtmMoldSet() 
 *  \brief 構造函數不能直接調用,請使用GetInstance()
 */
CtmMoldSet::CtmMoldSet()
{
   // SaveToCurrent();
}

 /*! \fn CtmMoldSet::~CtmMoldSet()
 *  \brief 析構函數
 */
CtmMoldSet::~CtmMoldSet()
{
}

 /*! \fn CtmMoldSet*	CtmMoldSet::GetInstance()
 *  \brief  真正的構造函數
 *	\return	返回的是CtmMoldSet實例指針
 */
CtmMoldSet*	CtmMoldSet::GetInstance()
{
	if (m_pInstance == NULL) 
	{
		m_pInstance = new CtmMoldSet();
	}
	return m_pInstance;
}

 /*! \fn CtmMoldSet::ReleaseInstance()
 *  \brief 真正的析構函數
 */
void		CtmMoldSet::ReleaseInstance()
{
	if (m_pInstance != NULL) 
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

 /*! \fn CtmMoldSet::Read(char* pszID, char *pszMaterial, int nColor)
 *  \brief 讀取特定的模具資料
 *	\param	[in]	pszID	模具的名字/ID
 *	\param	[in]	pszMaterial 模具材料的名稱/ID
 *	\param	[in]	nColor 顏色
 *	\return	是否已經讀取到了:TRUE/FALSE
 *	\note	如果獨取了將會將頭和內容存儲在m_Head,m_pContent;
 */
BOOL	CtmMoldSet::Read(DB_MOLDHDR MoldSetHead, tmStorageType StorageType)
{
    if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->Read(MoldSetHead, StorageType);
		}	
	}
	return FALSE;
}


 /*! \fn CtmMoldSet::Read(int nIndex)
 *  \brief 讀取指定的模具資料
 *	\param	[in]	nIndex	模具序號
 *	\return	是否已經讀取到了:TRUE/FALSE
 *	\note	如果獨取了將會將頭和內容存儲在m_Head,m_pContent;
 */
BOOL	CtmMoldSet::Read(int nIndex, tmStorageType StorageType)
{
    if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->Read(nIndex, StorageType);
		}	
	}
	return FALSE;
}


 /*! \fn CtmMoldSet::Save(char* pszID, char *pszMaterial, int nColor)
 *  \brief 讀取特定的模具資料
 *	\param	[in]	pszID	模具的名字/ID
 *	\param	[in]	pszMaterial 模具材料的名稱/ID
 *	\param	[in]	nColor 顏色
 *	\return	是否已經成功保存:TRUE/FALSE
 *	\note	其本質是存儲了m_Head和m_pContent中的信息
 */
BOOL	CtmMoldSet::Save(DB_MOLDHDR MoldSetHead, tmStorageType StorageType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->Save(MoldSetHead, StorageType);
		}	
	}
	return FALSE;
}

 /*! \fn CtmMoldSet::Save(int nIndex)
 *  \brief 保存特定的模具資料(當前)
 *	\param	[in]	nIndex	模具序號
 *	\return	是否已經成功保存:TRUE/FALSE
 *	\note		其本質是存儲了m_Head和m_pContent中的信息
 */
BOOL	CtmMoldSet::Save(int nIndex,tmStorageType StorageType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->Save(nIndex, StorageType);
		}	
	}
	return FALSE;
}

 /*! \fn CtmMoldSet::Delete(char* pszID, char *pszMaterial, int nColor)
 *  \brief 刪除特定的模具資料
 *	\param	[in]	pszID	模具的名字/ID
 *	\param	[in]	pszMaterial 模具材料的名稱/ID
 *	\param	[in]	nColor 顏色
 *	\return	是否已經成功刪除:TRUE/FALSE
 *	\note		其本質是將m_Head中的名稱設置為NULL
 */
BOOL	CtmMoldSet::Delete(DB_MOLDHDR MoldSetHead, tmStorageType StorageType)
{
   if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->Delete(MoldSetHead, StorageType);
		}	
	}
	return FALSE;
}

 /*! \fn CtmMoldSet::GetCurrentMold
 *  \brief 刪除特定的模具資料
 *	\return	當前正在使用的模具名稱
 */
char*	CtmMoldSet::GetCurrentMold()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetCurrentMold();
		}	
	}
	return NULL;
}

 /*! \fn CtmMoldSet::Delete(int nIndex)
 *  \brief 刪除特定的模具資料
 *	\param	[in]	nIndex	
 *	\return	是否已經成功刪除:TRUE/FALSE
 *	\note		其本質是將m_Head中的名稱設置為NULL
 */
BOOL	CtmMoldSet::Delete(int nIndex,tmStorageType StorageType)
{
   	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->Delete(nIndex, StorageType);
		}	
	}
	return FALSE;
}

/*! \fn CtmMoldSet::Find(char* pszID, char *pszMaterial, int nColor)
 *  \brief 	查找特定的模具資料
 *	\param	[in]	pszID	模具的名字/ID
 *	\param	[in]	pszMaterial 模具材料的名稱/ID
 *	\param	[in]	nColor 顏色
 *	\return	資料的序號,如果是0的話表明沒有找到
 */
int		CtmMoldSet::Find(DB_MOLDHDR MoldSetHead, tmStorageType	StorageType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->Find(MoldSetHead, StorageType);
		}	
	}
	return 0;
}

/*! \fn CtmMoldSet::GetMoldSetHead()
 *  \brief 	獲得頭信息
 *	\return	頭信息指針
 *	\note		無論是保存還是獨取,之後的動作都需要使用該函數來獲取頭信息
 */
DB_MOLDHDR*	CtmMoldSet::GetMoldSetHead()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetMoldSetHead();
		}	
	}
	return NULL;
}

/*! \overload CtmMoldSet::GetMoldSetHead(int nStart, int Num)
 *  \brief 	獲得頭信息數組
 *	\param	[in]	nStart 開始的序號,從1開開始
 *	\param	[in]	Num 希望獲取的數目
 *	\return	頭信息指針
 *	\note		並不能保證在得到的所有信息不為空,原因是有可能並沒有保存nStart + Num個資料 \n
 *					這樣我們將會得到後面的信息全部為空
 */
DB_MOLDHDR*	CtmMoldSet::GetMoldSetHead(int nStart, int Num, tmStorageType StorageType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetMoldSetHead(nStart, Num, StorageType);
		}	
	}
	return NULL;
}

/*! \fn CtmMoldSet::GetMoldSetContent()
 *  \brief 	獲得頭信息
 *	\return	內容信息指針
 *	\note		無論是保存還是獨取,之後的動作都需要使用該函數來獲取內容信息
 */
BYTE*	CtmMoldSet::GetMoldSetContent()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetMoldSetContent();
		}	
	}
	return NULL;
}

/*! \fn CtmMoldSet::GetMaxMoldSetNum()
 *  \brief 	獲取允許保存慔具的最大數目
 *	\return	允許保存慔具的最大數目
 */
int		CtmMoldSet::GetMaxMoldSetNum()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetMaxMoldSetNum();
		}	
	}
	return 0;
}

/*! \fn CtmMoldSet::GetMoldSetContent()
 *  \brief 	獲取現在保存慔具數目
 *	\return	現在保存慔具數目
 */
int		CtmMoldSet::GetMoldSetNum(tmStorageType	StorageType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetMoldSetNum(StorageType);
		}	
	}
	return 0;
}

int		CtmMoldSet::GetMoldSize()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetMoldSize();
		}
	}
	return 0;
}

int		CtmMoldSet::SendControlMoldSet()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->SendControlMoldSet();
		}
	}
	return 0;
}


/*! \fn CtmMoldSet::SaveToCurrentMold()
 *  \brief 	
 *	\return	
 */
BOOL CtmMoldSet::SaveToCurrentMold(DB_MOLDHDR MoldSetHead, tmStorageType StorageType ) 
{
	printf("CtmMoldSet::SaveToCurrentMold\n");
   	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			printf("g_ptmControlServer->MoldControl()->SaveToCurrentMold(\n");
			return g_ptmControlServer->MoldControl()->SaveToCurrentMold(MoldSetHead, StorageType);
		}	
	}
	return FALSE;
}

BOOL CtmMoldSet::SaveToCurrentMold() 
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->SaveToCurrentMold();
		}	
	}
	return FALSE;
}

DB_MOLDHDR* CtmMoldSet::GetMoldSetHeadAsynch() 
{
    if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetMoldSetHeadAsynch();
		}	
	}
	return NULL;
}

BOOL	CtmMoldSet::SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->SaveCurrentDB(MoldSetHead, StorageType);
		}	
	}
	return FALSE;
}

CtmBlockControl::CtmBlockControl()
{
	
}

CtmBlockControl::~CtmBlockControl()
{
   
}

CtmBlockControl* CtmBlockControl::GetInstance()
{
    m_nCount++;
    if (m_pInstance == NULL)
    {
        m_pInstance = new CtmBlockControl();
    }
    return m_pInstance;
}

void CtmBlockControl::ReleaseInstance()
{
    m_nCount--;
    if (m_nCount <= 0)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

BOOL CtmBlockControl::SaveAs(tmStorageType StorageType)
{
    if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->SaveAs(StorageType);
		}	
	}
    return FALSE;
}

BOOL CtmBlockControl::SaveAs(tmStorageType SourceStorage, tmStorageType DestStorage)
{
    if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->SaveAs(SourceStorage, DestStorage);
		}	
	}
    return FALSE;
}

BOOL CtmBlockControl::Read(tmStorageType StorageType)
{
    if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->Read(StorageType);
		}	
	}
    return FALSE;
}

BOOL CtmBlockControl::Delete(tmStorageType StorageType)
{
    if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->Delete(StorageType);
		}	
	}
    return FALSE;
}

void CtmBlockControl::SetBlockIndex(int nBlockIndex)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->SetBlockIndex(nBlockIndex);
		}	
	}
}

int	 CtmBlockControl::GetSize(int nBlockIndex)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetSize(nBlockIndex);
		}	
	}
    return 0;
}

int	 CtmBlockControl::GetSize(tmFileIDType FileID)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->MoldControl() != NULL)
		{
			return g_ptmControlServer->MoldControl()->GetSize(FileID);
		}	
	}
    return 0;
}
