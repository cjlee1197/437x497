/*===========================================================================+
|  Class    : CtmControl_Mold			                                     |
|  Task     : Mold Service Function 				    	         		 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 05/11/2008	                                                 |
|  Revision : 05/11/2008				                                     |
+===========================================================================*/
#ifndef	D_CTMCONTROL_MOLD
#define	D_CTMCONTROL_MOLD
#include	"../../tmcontrol.h"
#include	"../../tmpackclass.h"
#include	"../../package.h"
#include	"../../tmcontrol.h"
#include	"../libControl.h"

//add by J.Wong 2015/11/18 13:48:30
#define		MAX_MOLD_ID_COUNT		8000
#define		MAX_STARTID_COUNT		6000


typedef struct	tmtagStupid{
	WORD		wCount;
	long		lStartID;	
}STUPID;

class	CtmControl_Mold: public	CtmControl
{
	DECLARE_DYNCREATE(CtmControl_Mold)
	public:
		CtmControl_Mold();
		~CtmControl_Mold();
		
	public:
		/*virtual*/BOOL	AddControl(void *pParas[])						;
		/*virtual*/BOOL	ModifyControl(void *pParas[]) 					;
		/*virtual*/BOOL	FindControl(void *pParas[], void *pFindPara)	;
		/*virtual*/BOOL	DeleteControl(void *pDeletePara = NULL)			;
		/*virtual*/BOOL	FixControl(void *pFixPara)						;
		/*virtual*/void	InitControl()									;
	private:
		pthread_mutex_t		m_MutexMold;				//多線程鎖
		//tmDB_ERROR			m_MoldCurrent;				//當前監測
		//tmDB_ERROR			m_MoniIndex;				//記錄當前最新的個數和序號
	protected:
		CtmSarmRecordFileControl*	m_pMoldSetFile;			//	save moldset's infomation, include head and content
		DB_MOLDHDR					m_Head;
		DB_MOLDHDR*					m_pHeads;
		BYTE*						m_pContent;
		int							m_nMaxMoldSet;
		int							m_nMoldSetSize;			//	size of the moldset content		
		char						m_szCurrentMoldName[40];
		char						m_pszMoldSetPath[256];
    	int 						m_nBlockIndex;
    	tmFileIDType 				m_nDataType;
    	long						m_lMoldID[MAX_MOLD_ID_COUNT];
    	DWORD						m_dwMoldIDCount;
    	STUPID						m_StartID[MAX_STARTID_COUNT];
    	WORD						m_wGroupCount;
	private:		
		int							m_nCurrentIndex;
		static	tmBlockInfo 		m_aBlockInfo[];
	public:
		virtual BOOL	Save(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	;
		virtual BOOL	Save(int nIndex,tmStorageType StorageType=STORAGE_TYPE_SRAM)				;
		virtual int		Find(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	;
		virtual BOOL	Read(int nIndex, tmStorageType StorageType = STORAGE_TYPE_SRAM)				;
		virtual BOOL	Read(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	;
		virtual BOOL	Delete(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	;
		virtual BOOL	Delete(int nIndex,tmStorageType StorageType=STORAGE_TYPE_SRAM)				;
        virtual BOOL    SaveToCurrentMold(DB_MOLDHDR MoldSetHead, tmStorageType StorageType = STORAGE_TYPE_SRAM)	;
        virtual BOOL    SaveToCurrentMold()															;
        #ifdef	D_NOWRITE_FALSH
        virtual BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType = STORAGE_TYPE_FLASHEX)		;
        #else 
        	#ifdef D_X86
        virtual BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType = STORAGE_TYPE_FLASH)		;
        	#else 
        virtual BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType = STORAGE_TYPE_SRAM)			;
        	#endif
        #endif
        virtual char*	GetCurrentMold()															;
		virtual DB_MOLDHDR*	GetMoldSetHead()														;
		virtual DB_MOLDHDR*	GetMoldSetHead(int nStart, int Num, tmStorageType StorageType = STORAGE_TYPE_SRAM)	;
        virtual DB_MOLDHDR* GetMoldSetHeadAsynch()																;
		virtual BYTE*	GetMoldSetContent()																		;
		virtual int		GetMaxMoldSetNum()																		;
		virtual int		GetMoldSetNum(tmStorageType	StorageType = STORAGE_TYPE_SRAM)							;
		virtual int		GetMoldSize()																			;
		
		virtual void SetBlockIndex(int nBlockIndex)													;
   	 	virtual BOOL Delete(tmStorageType StorageType)												;
    	virtual BOOL Read(tmStorageType StorageType)												;
		virtual BOOL SaveAs(tmStorageType SourceStorage, tmStorageType DestStorage)				;
    	virtual BOOL SaveAs(tmStorageType StorageType)											;
		virtual int	 GetSize(int nBlockIndex)														;
		virtual int	 GetSize(tmFileIDType FileID)													;
		virtual	int	 ComposeHyb5MoldSetData(DB_MOLDHDR MoldSetHead, BYTE* pData);
		virtual	int		CalculateSize();
		virtual	int		CalculateNewSize();
		virtual	char*	GetMoldSetPath();
		virtual	char*	SetMoldSetPath(char*	pszPath);
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
	protected:
		int		FindFree(tmStorageType StorageType=STORAGE_TYPE_SRAM);
	private:
		int	 	FindBlockInfoNo(int nBlockIndex);
		int	 	FindBlockInfoNo(tmFileIDType FileID);
};


class CtmControl_MoldHyb5: public CtmControl_Mold
{
	DECLARE_DYNCREATE(CtmControl_MoldHyb5)
	public:
		CtmControl_MoldHyb5();
		~CtmControl_MoldHyb5();
	public:
//		/*virtual*/void	InitControl();
		virtual BOOL	Save(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	;
		virtual BOOL	Read(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	;
		virtual BOOL	Delete(DB_MOLDHDR MoldSetHead, tmStorageType StorageType);
		virtual DB_MOLDHDR*	GetMoldSetHead(int nStart, int Num, tmStorageType StorageType = STORAGE_TYPE_SRAM)	;
		virtual int		GetMoldSetNum(tmStorageType	StorageType = STORAGE_TYPE_SRAM)							;
		virtual	int		Find(DB_MOLDHDR MoldSetHead, tmStorageType	StorageType=STORAGE_TYPE_SRAM);	
		virtual BOOL  	CheckExistOldFile();
		virtual	int	 	ComposeHyb5MoldSetData(DB_MOLDHDR MoldSetHead, BYTE* pData);
		virtual int		CalculateSize();
		virtual	int		CalculateNewSize();
		virtual BOOL 	SaveToCurrentMold(DB_MOLDHDR MoldSetHead, tmStorageType StorageType = STORAGE_TYPE_FLASH);
    virtual BOOL    SaveToCurrentMold()															;
	private:
		HYB5MOLDSETDATA	m_MoldSetData;
		void	ReadWarningCnt();
};

#ifndef	D_BEOBJECT_CTMCONTROL_MOLD	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

