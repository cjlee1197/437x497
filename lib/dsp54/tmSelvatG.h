#ifndef     D__TMSELVATG_LIB
#define     D__TMSELVATG_LIB

#include	"../../selvat_ancestor.h"
#include	"../../tmvalues.h"
#include	"../../parsepule.h"
//#include	"../../tmconfig.h"
#include	"../../inifile.h"


class	CtmSelvatG: public CtmVat_Ancestor
{
	DECLARE_DYNCREATE(CtmSelvatG)
	public:
		CtmSelvatG();
		~CtmSelvatG();
	public:
		BOOL       	GetTable();							// 眔讽玡痉计癸莱崩畒
    	int         GetCrossPos(int x);					// 锣传家狾竚猳竚
    	int         GetMoldPos(int y);					// 锣传猳竚家狾竚
    	BOOL		TransferToCrossPos(BYTE* pBuffer);	// 锣传┮Τ崩畒把计猳竚
    	BOOL		SendToHost();						// 肚癳┮Τ崩畒把计诀
    	BOOL		TransferToMoldPos();				// 锣传┮Τ崩畒把计家狾竚
    	int			GetCrossNum();						// 莉眔繷
    	int			GetMoldNum();						// 莉眔家狾 
    	
    	 
    	int			CreateSelf()	;	//	overwrite
    	int			FreeSelf();
    
 	
 	protected:
 		BOOL		InitCrossTable(WORD wTon);
 		BOOL		InitMoldTable(WORD wTon);
    	BOOL		ReGetTable(WORD	wTon);
    	
    private:
    	BOOL		ComposeCrossTableName(WORD wTon, char* pszOutName);
    	BOOL		ComposeMoldTableName(WORD wTon, char* pszOutName);
    	int			GetFileTable(CParseObject*	pParse, char* pszFileName, 
    								int* pTable, int nMaxTableNum);
    	BOOL		CheckFileExist(char* pszFileName);
    	int			SetTable(CtmValueInt* pObj, int* pTable, int nMaxTableNum);
    protected:
    	CParseObject m_ParseCross;		//	繷
    	CParseObject m_ParseMold;		//	家狾
    	
    	
    	
    	char*		 m_pszPath;
   
    	
    	int			 m_nCrossNum;
    	int			 m_nMoldNum;
    	int			 m_aryCross[256];
    	int			 m_aryMold[256];
    private:
    	WORD            m_wCurTonValue;         // 讽玡痉计
    	WORD            m_wPreTonValue;         // Ω痉计
    	BOOL			m_bInTable;
    	
    	
    	
    	
};


#ifndef	D_BEOBJECT_CTMSELVATG
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
