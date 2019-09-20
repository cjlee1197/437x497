
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
 *  \brief �c�y��Ƥ��ઽ���ե�,�Шϥ�GetInstance()
 */
CtmMoldSet::CtmMoldSet()
{
   // SaveToCurrent();
}

 /*! \fn CtmMoldSet::~CtmMoldSet()
 *  \brief �R�c���
 */
CtmMoldSet::~CtmMoldSet()
{
}

 /*! \fn CtmMoldSet*	CtmMoldSet::GetInstance()
 *  \brief  �u�����c�y���
 *	\return	��^���OCtmMoldSet��ҫ��w
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
 *  \brief �u�����R�c���
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
 *  \brief Ū���S�w���Ҩ���
 *	\param	[in]	pszID	�Ҩ㪺�W�r/ID
 *	\param	[in]	pszMaterial �Ҩ���ƪ��W��/ID
 *	\param	[in]	nColor �C��
 *	\return	�O�_�w�gŪ����F:TRUE/FALSE
 *	\note	�p�G�W���F�N�|�N�Y�M���e�s�x�bm_Head,m_pContent;
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
 *  \brief Ū�����w���Ҩ���
 *	\param	[in]	nIndex	�Ҩ�Ǹ�
 *	\return	�O�_�w�gŪ����F:TRUE/FALSE
 *	\note	�p�G�W���F�N�|�N�Y�M���e�s�x�bm_Head,m_pContent;
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
 *  \brief Ū���S�w���Ҩ���
 *	\param	[in]	pszID	�Ҩ㪺�W�r/ID
 *	\param	[in]	pszMaterial �Ҩ���ƪ��W��/ID
 *	\param	[in]	nColor �C��
 *	\return	�O�_�w�g���\�O�s:TRUE/FALSE
 *	\note	�䥻��O�s�x�Fm_Head�Mm_pContent�����H��
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
 *  \brief �O�s�S�w���Ҩ���(��e)
 *	\param	[in]	nIndex	�Ҩ�Ǹ�
 *	\return	�O�_�w�g���\�O�s:TRUE/FALSE
 *	\note		�䥻��O�s�x�Fm_Head�Mm_pContent�����H��
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
 *  \brief �R���S�w���Ҩ���
 *	\param	[in]	pszID	�Ҩ㪺�W�r/ID
 *	\param	[in]	pszMaterial �Ҩ���ƪ��W��/ID
 *	\param	[in]	nColor �C��
 *	\return	�O�_�w�g���\�R��:TRUE/FALSE
 *	\note		�䥻��O�Nm_Head�����W�ٳ]�m��NULL
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
 *  \brief �R���S�w���Ҩ���
 *	\return	��e���b�ϥΪ��Ҩ�W��
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
 *  \brief �R���S�w���Ҩ���
 *	\param	[in]	nIndex	
 *	\return	�O�_�w�g���\�R��:TRUE/FALSE
 *	\note		�䥻��O�Nm_Head�����W�ٳ]�m��NULL
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
 *  \brief 	�d��S�w���Ҩ���
 *	\param	[in]	pszID	�Ҩ㪺�W�r/ID
 *	\param	[in]	pszMaterial �Ҩ���ƪ��W��/ID
 *	\param	[in]	nColor �C��
 *	\return	��ƪ��Ǹ�,�p�G�O0���ܪ���S�����
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
 *  \brief 	��o�Y�H��
 *	\return	�Y�H�����w
 *	\note		�L�׬O�O�s�٬O�W��,���᪺�ʧ@���ݭn�ϥθӨ�ƨ�����Y�H��
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
 *  \brief 	��o�Y�H���Ʋ�
 *	\param	[in]	nStart �}�l���Ǹ�,�q1�}�}�l
 *	\param	[in]	Num �Ʊ�������ƥ�
 *	\return	�Y�H�����w
 *	\note		�ä���O�Ҧb�o�쪺�Ҧ��H��������,��]�O���i��èS���O�snStart + Num�Ӹ�� \n
 *					�o�˧ڭ̱N�|�o��᭱���H����������
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
 *  \brief 	��o�Y�H��
 *	\return	���e�H�����w
 *	\note		�L�׬O�O�s�٬O�W��,���᪺�ʧ@���ݭn�ϥθӨ�ƨ�������e�H��
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
 *  \brief 	������\�O�s�Ψ㪺�̤j�ƥ�
 *	\return	���\�O�s�Ψ㪺�̤j�ƥ�
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
 *  \brief 	����{�b�O�s�Ψ�ƥ�
 *	\return	�{�b�O�s�Ψ�ƥ�
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
