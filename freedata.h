#ifndef 	__D__CTMFREEDATA
#define		__D__CTMFREEDATA
#include	"tmpackclass.h"
#include	"database.h"

class	CtmFreeData: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmFreeData)
	public:
		CtmFreeData();
		~CtmFreeData();
	public:		
		virtual	int		LoadIDTable(char* pszIDTableFile);
		virtual	int		SaveDataToFile(char* pszFile);
		virtual	int		GetDataFromFile(char* pszFile);
		virtual	int		GetDefaultDataFromTable(char* pszIDTableFile);
		virtual	int		SearchIDFromTable(char* cszIDName);
		virtual	int		SearchIndexFromTable(int nIndex);
};


//	the SpecType is HMI , dsp , driver or motor ...?
class	CtmSpecData: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmSpecData)
	public:
		CtmSpecData();
		~CtmSpecData();
	public:
		virtual	int		SaveFixedData(int nSpecType, int nFixedNum, char* pBuf, int nSize);
		virtual	int		SaveDBData(int nSpecType);
		virtual	int		GetFixedData(int nSpecType, int nFixedNum, char* pBuf, int nSize);
		virtual	int		GetDBData(int nSpecType);
		
		virtual	int		SaveFixedDataTo();
		virtual	int		SaveFixedDataTo(int nSpecType);
		virtual	int		SaveDBDataTo();
		virtual	int		SaveDBDataTo(int nSpecType);
		virtual	int 	SaveDataTo();
		virtual	int		SaveAllDataTo();
		
		
		virtual	int		GetFixedDataFrom();
		virtual	int		GetFixedDataFrom(int nSpecType);
		virtual	int		GetDBDataFrom();
		virtual	int		GetDBDataFrom(int nSpecType);
		virtual	int		GetDatFrom();
		virtual	int		GetAllDataFrom();
	
};



extern	CtmFreeData*	g_pFreeData;
extern	CtmSpecData*	g_pSpecData;
#endif
