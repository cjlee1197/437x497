#ifndef 	__D__CTMOUTTABLE
#define		__D__CTMOUTTABLE
#include	"tmpackclass.h"
#include	"database.h"

const	int	CNT_TABLE_IO_PB	= 0;
const	int	CNT_TABLE_IO_PC	= 1;
const	int	CNT_TABLE_STATE	= 2;
const	int	CNT_TABLE_ERROR	= 3;
const	int	CNT_TABLE_AUSTONEVALUE	= 7;// YANGJX 2010-8-20 write for Austone default Value
const	int	CNT_TABLE_MACHTYPESEL	= 8;// YANGJX 2010-10-28 Add For J5&HYB3 MachType Select
const	int	CNT_TABLE_FACTORYCODE	= 9;

class	CtmExtentTable: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmExtentTable)
	public:
		CtmExtentTable();
		~CtmExtentTable();
	public:		
		virtual	WORD		GetTableValue(int nType,int* pResult, void* pEx = NULL)	{return 0;};
		virtual	WORD		GetTableValue(int nType,char* pszID, void* pEx = NULL)	{return 0;};
};


extern	CtmExtentTable*	g_pExTable;
#endif
