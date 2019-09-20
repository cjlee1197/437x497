#include	"freedata.h"


CtmFreeData*	g_pFreeData	= NULL;
CtmSpecData*	g_pSpecData = NULL;
IMPLEMENT_DYNCREATE(CtmFreeData, CtmPackClass)
IMPLEMENT_DYNCREATE(CtmSpecData, CtmPackClass)


CtmFreeData::CtmFreeData(): CtmPackClass()
{
	
}

CtmFreeData::~CtmFreeData()
{
	
}

int		CtmFreeData::LoadIDTable(char* pszIDTableFile)
{
	return 0;
}

int		CtmFreeData::SaveDataToFile(char* pszFile)
{
	return 0;
}

int		CtmFreeData::GetDataFromFile(char* pszFile)
{
	return 0;
}

int		CtmFreeData::GetDefaultDataFromTable(char* pszIDTableFile)
{
	return 0;	
}

int		CtmFreeData::SearchIDFromTable(char* cszIDName)
{
	return 0;	
}

int		CtmFreeData::SearchIndexFromTable(int nIndex)
{
	return 0;	
}


CtmSpecData::CtmSpecData()
{
	
}

CtmSpecData::~CtmSpecData()
{
	
}

int CtmSpecData::SaveFixedData(int nSpecType, int nFixedNum, char* pBuf, int nSize)
{
	return 0;
}


int CtmSpecData::CtmSpecData::SaveDBData(int nSpecType)
{
	return 0;
}

int CtmSpecData::GetFixedData(int nSpecType, int nFixedNum, char* pBuf, int nSize)
{                
	return 0;    
}                

int CtmSpecData::GetDBData(int nSpecType)
{                
	return 0;    
}           
     
int CtmSpecData::SaveFixedDataTo()
{                
	return 0;    
}                

int CtmSpecData::SaveFixedDataTo(int nSpecType)
{                
	return 0;    
}                

int CtmSpecData::SaveDBDataTo()
{                
	return 0;    
}                

int CtmSpecData::SaveDBDataTo(int nSpecType)
{                
	return 0;    
}                

int CtmSpecData::SaveDataTo()
{                
	return 0;    
}                


int CtmSpecData::SaveAllDataTo()
{                
	return 0;    
}                

int CtmSpecData::GetFixedDataFrom()
{                
	return 0;    
}                


int CtmSpecData::GetFixedDataFrom(int nSpecType)
{                
	return 0;    
}                

int CtmSpecData::GetDBDataFrom()
{                
	return 0;    
}                


int CtmSpecData::GetDBDataFrom(int nSpecType)
{                
	return 0;    
}                


int CtmSpecData::GetDatFrom()
{                
	return 0;    
}                


int CtmSpecData::GetAllDataFrom()
{                
	return 0;    
}
