/*==========================================================================+
|  Class    : Strings		                                            	|
|  Task     : Strings header file                          					|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2006/1/24                                                   	|
|  Revision : 			                                                    |
+==========================================================================*/
#ifndef	D_DSTRINGS
#define	D_DSTRINGS

#include "common.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
     
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagSTRINGSINFO
{
	int			nCount;			// ﹃羆计
	int			nSize;			// ﹃
}	STRINGSINFO;

typedef	struct	tagSTRING											//yang 2006/5/18 
{
	int			nIndex;			// ﹃腹
	char*		pStr;
	tagSTRING	*pLNode;		// left node is small value node
	tagSTRING	*pRNode;		// right node is large value node
}	STRING;

//	fans add
typedef enum{NULLKEY = 0,HAVEKEY,DELKEY} tmHaveHashKey;

typedef	struct	tagHASHELEMENT
{
	unsigned int	uiIndex;				//	index, user difine it
	unsigned int	uiSizeOfData;
	void*			pKey;
	void*			pData;
	tagHASHELEMENT* pLNode;
	tagHASHELEMENT*	pRNode;	
}	tmHashElement;

typedef struct 	tagHASHTABLE
{
	unsigned int	uiCount;				//	the elements num in the hashtable
	unsigned int	uiSize;					//	the size of the hashtable, isn't elements	
	tmHashElement*	pElements;	
}	tmHashTable;



/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - CStrings                                    |
+==========================================================================*/
class CStrings
{
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
public:
	CStrings();
	~CStrings();
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
public:
	int		Create(STRINGSINFO *pInfo);
	int		AddString(char *psz);
	
	int		GetIndex(char *psz);
	char*	GetString(int nIndex);

/*--------------------------------------------------------------------------+
|           Attributes                                                      |
+--------------------------------------------------------------------------*/
private:
	int		m_nCount;			// ﹃羆计
	int		m_nListCount;		// 借计
	int		m_nStrSize;			// ﹃羆
	
	char	*m_pStrings;		// ﹃羆
	char	**m_pStringIndex;	// ま﹃癸酚
	STRING	*m_pStringList;		// ﹃獺
	STRINGSINFO	m_Info;
		
	void	FreeList();	
	void	FreeNode(STRING* pString);								//yang 2006/5/18 
	int		GetNearPrime(int nCount);
	int		GetHash(char *psz);
//	int		AddList(char *psz);
	int		AddList(STRING *pStrInfo);
};


//	fans add 2007/3/21 05:57と
//	摸度度ㄏノ瓣粂ē﹃,惠璶ㄏノㄤよ,叫更Hashㄧ计EqualKey单闽ㄧ计
/*==========================================================================+
|           Class declaration - CtmHashTable                             	|
+==========================================================================*/
class CtmHashTable
{
	/*--------------------------------------------------------------------------+
	|           Constructor and destructor                                      |
	+--------------------------------------------------------------------------*/
	public:
		CtmHashTable(unsigned int	uiNum = 6151);
		virtual	~CtmHashTable();
	/*--------------------------------------------------------------------------+
	|           Operations                                                      |
	+--------------------------------------------------------------------------*/
	public:
		virtual	BOOL			AddElement(tmHashElement* pElement);
		virtual	BOOL			DelElement(void* pKey);
		virtual BOOL			FindElement(void* pKey, tmHashElement* pOutElement);
		virtual	const tmHashTable* GetHashContent() {return &m_HashTable;};	
		virtual	void			ClearElement();	
	protected:
		virtual	void			InitHash();
		virtual	void			FreeHash();
		virtual	void			FreeNode(tmHashElement* pNode);
		virtual	int				Hash(void* pKey);
		virtual	int				EqualKey(void* pKey, void* pHashKey);
		virtual	tmHashElement*	FindElementEmptyPos(void* pKey);
		unsigned int			GetPrime(unsigned int uiNum);
	/*--------------------------------------------------------------------------+
	|           Attributes                                                      |
	+--------------------------------------------------------------------------*/
	protected:
		unsigned int	m_iHashSeed;
	private:
		tmHashTable		m_HashTable;
	
};



#endif

