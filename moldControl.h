/*	MOLDSET	CLASS	*/

//test update to svn


#ifndef	_CONTROL_MOLD
#define	_CONTROL_MOLD

#include	"database.h"
#include	"files.h"
#include	"utils.h"
#include	"sram.h"
#include	"storage.h"
#include	"main.h"

const	int	MAX_MOLDSETDATA = 1000;



typedef struct	tagtmMoldSetHead
{
	char		szID[16];					//      模具名稱                   
	tmDATE		DateSave;					//      儲存日期                   
	char		szMaterial[8];				//      材料                       
	int			nColor;
} tmMoldSetHead;

typedef struct	tagtmMoldSetContent
{
	DBDATA	data[MAX_MOLDSETDATA];
	//DWORD	data
} tmMoldSetContent;

typedef	struct	tagtmBlockMoldSet
{
	tmMoldSetHead	Head;
	tmMoldSetContent	Content;
} tmBlockMoldSet;

/*! \class CtmMoldSet
 *  \brief 模具資料的操作 
 *
 * 需要注意的是,模具的資料最基本的操作有三項:Read, Save, Delete
 */
 
class	CtmMoldSet
{
	//	construcotr and destructor
	public:
		static	CtmMoldSet*	GetInstance();
		static	void		ReleaseInstance();
	private:
		virtual ~CtmMoldSet();
		CtmMoldSet();
	private:
		static	CtmMoldSet*			m_pInstance;		//	
	//	operation
	public:
		BOOL	Save(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM);
		BOOL	Save(int nIndex,tmStorageType StorageType=STORAGE_TYPE_SRAM);
		int		Find(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM);
		BOOL	Read(int nIndex, tmStorageType StorageType = STORAGE_TYPE_SRAM);
		BOOL	Read(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM);
		BOOL	Delete(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM);
		BOOL	Delete(int nIndex,tmStorageType StorageType=STORAGE_TYPE_SRAM);
        BOOL    SaveToCurrentMold(DB_MOLDHDR MoldSetHead, tmStorageType StorageType = STORAGE_TYPE_SRAM);
        BOOL    SaveToCurrentMold();
        //BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead);
        #ifdef	D_NOWRITE_FALSH
        	BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType = STORAGE_TYPE_FLASHEX);
        #else 
        	#ifdef D_X86
        	BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType = STORAGE_TYPE_FLASH);
        	#else 
        	BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType = STORAGE_TYPE_SRAM);
        	#endif
        #endif
        
        char*	GetCurrentMold();
		
		DB_MOLDHDR*	GetMoldSetHead();
		DB_MOLDHDR*	GetMoldSetHead(int nStart, int Num, tmStorageType StorageType = STORAGE_TYPE_SRAM);
        DB_MOLDHDR* GetMoldSetHeadAsynch();
		BYTE*	GetMoldSetContent();
		int		GetMaxMoldSetNum();
		int		GetMoldSetNum(tmStorageType	StorageType = STORAGE_TYPE_SRAM);
		
		int		GetMoldSize();
		int		SendControlMoldSet();
};

class   CtmBlockControl
{
	protected:
	    CtmBlockControl();
	    virtual ~CtmBlockControl();
	public:
	    static void ReleaseInstance();
	    static CtmBlockControl* GetInstance();
	
	public:
	    void SetBlockIndex(int nBlockIndex);
	    BOOL Delete(tmStorageType StorageType);
	    BOOL Read(tmStorageType StorageType);
	    BOOL SaveAs(tmStorageType SourceStorage, tmStorageType DestStorage);
	    BOOL SaveAs(tmStorageType StorageType);
		int	 GetSize(int nBlockIndex);
		int	 GetSize(tmFileIDType FileID);
	private:
		static 	CtmBlockControl* 	m_pInstance;
		static 		int 			m_nCount;
};

extern	BOOL	g_bFindMoldSetMsg;
#endif
