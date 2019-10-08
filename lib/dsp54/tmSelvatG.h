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
		BOOL       	GetTable();							// ���o��e���ƹ��������O�y��
    	int         GetCrossPos(int x);					// �ഫ�ҪO��m���o����m
    	int         GetMoldPos(int y);					// �ഫ�o����m���ҪO��m
    	BOOL		TransferToCrossPos(BYTE* pBuffer);	// �ഫ�Ҧ������O�y�ѼƬ��o����m
    	BOOL		SendToHost();						// �ǰe�Ҧ������O�y�Ѽƨ�D��
    	BOOL		TransferToMoldPos();				// �ഫ�Ҧ������O�y�ѼƬ��ҪO��m
    	int			GetCrossNum();						// ��o�Q�r�Y������
    	int			GetMoldNum();						// ��o�ҪO������ 
    	
    	 
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
    	CParseObject m_ParseCross;		//	�Q�r�Y
    	CParseObject m_ParseMold;		//	�ҪO
    	
    	
    	
    	char*		 m_pszPath;
   
    	
    	int			 m_nCrossNum;
    	int			 m_nMoldNum;
    	int			 m_aryCross[256];
    	int			 m_aryMold[256];
    private:
    	WORD            m_wCurTonValue;         // ��e����
    	WORD            m_wPreTonValue;         // �W������
    	BOOL			m_bInTable;
    	
    	
    	
    	
};


#ifndef	D_BEOBJECT_CTMSELVATG
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
