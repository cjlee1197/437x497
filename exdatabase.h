/*==========================================================================+
|  Class    : exDatabase 			                                        |
|  Task     : exDatabase header file                          				|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James                                                         |
|  Version  : V1.00                                                         |
|  Creation : 2011/11/02                                                    |
|  Revision : 			                                                    |
+==========================================================================*/
#ifndef		D_EXDDATABASE
#define		D_EXDDATABASE

/*
	create:  	pszFileName is a xml file
	AddData:	datastruct add
*/

#include	"common.h"
#include	"files.h"
#include	"strings.h"
#include	"database.h"

#define			MAX_EXDBCOUNT		0xFFFF

typedef	struct	tagExDBValue
{
	DWORD			dwID;
	DATATYPE 		DataType;
	WORD			wState;
	WORD			wPercision;
	union
	{
		char 			acData[8];
		unsigned char 	ucData;
		unsigned short	wData;
		unsigned long	dwData;
		signed char 	cData;
		signed short	nData;
		signed long		lData;
		float 			fData;
		double 			dData;
		char			*pcData;
	};
	union
	{
		char 			acMax[8];
		unsigned char 	ucMax;
		unsigned short	wMax;
		unsigned long	dwMax;
		signed char 	cMax;
		signed short	nMax;
		signed long		lMax;
		float 			fMax;
		double 			dMax;
		char			*pcMax;
	};
	union
	{
		char 			acMin[8];
		unsigned char 	ucMin;
		unsigned short	wMin;
		unsigned long	dwMin;
		signed char 	cMin;
		signed short	nMin;
		signed long		lMin;
		float 			fMin;
		double 			dMin;
		char			*pcMin;
	};
	DWORD			dwPrivilege;
	DWORD			dwDataPath;
	
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	EXDBVALUE;
#else 
#ifdef	D_X86
EXDBVALUE;
#endif
#endif

typedef struct	tagExDataHeader
{
	DWORD		dwTotal;
	EXDBVALUE*	dxDBValue[MAX_EXDBCOUNT];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	EXDATAHEADER;
#else 
#ifdef	D_X86
EXDATAHEADER;
#endif
#endif


class	CEXDatabase: public CtmPackClass
{
	DECLARE_DYNCREATE(CEXDatabase);
public:
	CEXDatabase();
	~CEXDatabase();
	
public:
	virtual	int			Create(char*	pszFileName);
	virtual	int			AddData(EXDBVALUE ExDBValue);
	virtual	int			DeleteData(DWORD	dwID);
	virtual	int			SetDBValue(DWORD	dwID, void*	pData);
	virtual	EXDBVALUE	GetDBValue(DWORD	dwID);
	virtual	int			DeleteAll();
	virtual	EXDATAHEADER	GetDataHeader();
};


class	CAustoneXML: public CtmPackClass
{
	DECLARE_DYNCREATE(CAustoneXML);
public:
	CAustoneXML();
	~CAustoneXML();	
public:
	virtual	int			Create(char*	pszFileName)	{printf("create 1 \n");return 0;}
	virtual	int			Delete()						{return 0;}
};

extern	CEXDatabase*	g_pExDatabase;
extern	CAustoneXML*	g_pAustoneXML;

#endif
