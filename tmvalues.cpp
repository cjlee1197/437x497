#include	"tmvalues.h"



IMPLEMENT_DYNAMIC(CtmValueBase, CtmWnd)
IMPLEMENT_DYNCREATE(CtmValueStr, CtmValueBase)
IMPLEMENT_DYNCREATE(CtmValueInt, CtmValueBase)

//========================================================================


void CtmValueBase::InitProps()
{
	for(int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
	{
		memset(ValPropertys[i].pszName, 0, sizeof(ValPropertys[i].pszName));
		ValPropertys[i].nNub		= -1;
		ValPropertys[i].ptype 		= tmUnknown;
		ValPropertys[i].propAddress= NULL;
	}
}

CtmValueBase::CtmValueBase():CtmWnd()
{
	strcpy(TYPEName, "CtmValueBase");
	InitProps();
}

CtmValueBase::~CtmValueBase()
{
	for(int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
	{
		if (ValPropertys[i].pszName[0] == '\0') break;
		if ((ValPropertys[i].ptype == tmUSTR) 		|| 
			(ValPropertys[i].ptype == tmPROCEDURE) ||
			(ValPropertys[i].ptype == tmSTR))
		{
			if (ValPropertys[i].propAddress != NULL && *(char**)ValPropertys[i].propAddress != NULL)
			{
				free(*(char**)ValPropertys[i].propAddress);
				*(char**)ValPropertys[i].propAddress = NULL;			
			}
		}
	}
}




tmPropInfo* 	CtmValueBase::GetProperty(char* pszPropName)
{
	tmPropInfo* pResult = NULL;
	int index;
	index = FindProperty(pszPropName);
	if (index >= 0)
		pResult = &ValPropertys[index];	
	
	return pResult;
}

tmPropInfo*	CtmValueBase::GetProperty(int nNub)
{
	tmPropInfo* pResult = NULL;
	int index;
	index = FindProperty(nNub);
	if (index >= 0)
		pResult = &ValPropertys[index];	
	
	return pResult;
}



int		CtmValueBase::FindProperty(char* pszPropertyName)
{
	int result = -1;
	
//	if(PropCount == CNT_INT_MAX_VALUES_NUM)
//	{
//		for(int i = 0; i < CLASS_MAXCOUNT; i++)
//		{
//			if(strcmp(TYPEName, tm_ClassList.pszClassName[i]) == 0)
//			{
//				for(int j = 0; j < tm_ClassList.dwPropertyCount[i]; j++)
//				{
//			
//					if(strcmp(pszPropertyName, tm_ClassList.pszClassProperty[i][j].pszProperty) == 0)
//					{
//						result = j;
//						break;
//					}	
//				}
//				break;
//			}
//		}
//	}
//	else
	{
		for (int i = 0; i < PropCount; i++)
		{
			if (ValPropertys[i].pszName[0] != '\0')
			if (strcmp(ValPropertys[i].pszName, pszPropertyName) == 0)
			{
				result = i;
				break;
			}
		}
	}
	return result;
}


int		CtmValueBase::FindProperty(int nNub)
{
	int result = -1;
	//printf("nNub=%d\n",nNub);
	if(nNub > CNT_INT_MAX_VALUES_NUM && nNub < 0)
	{
		return result;
	}
	else
	{
		if (ValPropertys[nNub].nNub != -1)
			return ValPropertys[nNub].nNub;
	}
	return result;
}

void 		CtmValueBase::SetPropInfo(char* pszName, PropType PType, void* PropAddress)
{
	if (PropCount >= CNT_INT_MAX_VALUES_NUM)
	{
		return;
	}
	int index ;
	
	index = FindProperty(pszName);	
	if (index < 0)	
	{
		
		strcpy(ValPropertys[PropCount].pszName, pszName);
		//printf("set prop=%d,%s\n",PropCount,ValPropertys[PropCount].pszName);
		ValPropertys[PropCount].ptype = PType;
		ValPropertys[PropCount++].propAddress = PropAddress;
	}
	else
	{
		ValPropertys[index].ptype = PType;
		ValPropertys[index].propAddress = PropAddress;
	}
}

void 		CtmValueBase::SetPropInfo(int nNub, PropType PType, void* PropAddress)
{
	if (nNub >= CNT_INT_MAX_VALUES_NUM) 
	{
		cout << Name << " Error info: Propertys too more!" << PropCount << endl;
		return;
	}
	PropCount = CNT_INT_MAX_VALUES_NUM;
	ValPropertys[nNub].nNub = nNub;
	ValPropertys[nNub].ptype = PType;
	ValPropertys[nNub].propAddress = PropAddress;
}


void*			CtmValueBase::GetPropValue(char* pszPropName)
{
	void* pResult = NULL;
	int index;
	index = FindProperty(pszPropName);
	if (index >= 0)	pResult = ValPropertys[index].propAddress;
	return pResult;	
}

void* 	CtmValueBase::GetPropValue(int nNub)
{
	void* pResult = NULL;
	int index;
	index = FindProperty(nNub);
	if (index >= 0)	pResult = ValPropertys[index].propAddress;
	return pResult;	
}


bool			CtmValueBase::SetProperty(char* pszPropName, char* pszValue)
{
	char* pszTemp = NULL; 
	tmPropInfo* pPropInfo = NULL;
	m_bPropertyChanged = FALSE;
	if (pszPropName == NULL) return false;
	pPropInfo = GetProperty(pszPropName);
	
	if (pPropInfo != NULL)
	{
		int		nTemp = 0;
		short	snTemp = 0;
		long 	lTemp = 0;
		long long llTemp =0;
		WORD	wTemp = 0;
		DWORD	dwTemp = 0;
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				break;
			case tmINT:
				 
				nTemp = (int)strtol(pszValue, NULL, 0);
				if (nTemp != *(int*)(pPropInfo->propAddress))
				{
					*(int*)(pPropInfo->propAddress)= strtol(pszValue, NULL, 0);
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				snTemp = (short)strtol(pszValue, NULL,0);
				if (snTemp != *(short*)(pPropInfo->propAddress)) 
				{
					*(short*)(pPropInfo->propAddress) = snTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmLONG:
				lTemp = strtol(pszValue, NULL, 0);
				if (lTemp != *(long*)(pPropInfo->propAddress)) 
				{
					*(long*)(pPropInfo->propAddress) = lTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT64:
				llTemp =strtoll(pszValue, NULL, 0);
				
				if (llTemp != *(long long*)(pPropInfo->propAddress))
				{
					if (Name[0]!='\0')
					*(long long*)(pPropInfo->propAddress) = llTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				wTemp = (WORD)strtol(pszValue, NULL, 0);
				
				if (wTemp != *(WORD*)(pPropInfo->propAddress))
				{
					*(WORD*)(pPropInfo->propAddress) = wTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				dwTemp = strtol(pszValue, NULL, 0);
				
				if (dwTemp != *(DWORD*)(pPropInfo->propAddress))
				{
					*(DWORD*)(pPropInfo->propAddress) = dwTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				
				if (strcmp(pszValue, "true") == 0 || strcmp(pszValue, "TRUE") == 0)
				{
					if (!*(BOOL*)(pPropInfo->propAddress))
					{
						*(BOOL*)(pPropInfo->propAddress) = TRUE;
						m_bPropertyChanged = TRUE;
					}
				}
				else
				{
					if (*(BOOL*)(pPropInfo->propAddress))
					{
						*(BOOL*)(pPropInfo->propAddress) = FALSE;
						m_bPropertyChanged = TRUE;
					}
				}
				break;
			case tmFLOAT:
				*(float*)(pPropInfo->propAddress) = atof(pszValue);
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				*(double*)(pPropInfo->propAddress) = *(double*)pszValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmSTR:
			case tmPROCEDURE:
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
						
					{
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= strlen(pszValue)) )
				{
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = (char* ) malloc(strlen(pszValue) + 1);//new char[strlen(pszValue) + 1];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char* ) malloc(strlen(pszValue) + 1);//new char[strlen(pszValue) + 1];
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),pszValue);
				break;
			default:
				return false;
		}
		
	}
	return (pPropInfo != NULL);
}


bool			CtmValueBase::SetProperty(int nNub, char* pszValue)
{
	char* pszTemp = NULL; 
	tmPropInfo* pPropInfo = NULL;
	m_bPropertyChanged = FALSE;
	if (nNub > MAX_PROPERTY && nNub < 0) return false;
	pPropInfo = GetProperty(nNub);
	
	if (pPropInfo != NULL)
	{
		int		nTemp = 0;
		short	snTemp = 0;
		long 	lTemp = 0;
		long long llTemp =0;
		WORD	wTemp = 0;
		DWORD	dwTemp = 0;
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				break;
			case tmINT:
				nTemp = (int)strtol(pszValue, NULL, 0);
				if (nTemp != *(int*)(pPropInfo->propAddress))
				{
					*(int*)(pPropInfo->propAddress)= strtol(pszValue, NULL, 0);
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				snTemp = (short)strtol(pszValue, NULL,0);
				if (snTemp != *(short*)(pPropInfo->propAddress)) 
				{
					*(short*)(pPropInfo->propAddress) = snTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmLONG:
				lTemp = strtol(pszValue, NULL, 0);
				if (lTemp != *(long*)(pPropInfo->propAddress)) 
				{
					*(long*)(pPropInfo->propAddress) = lTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT64:
				llTemp =strtoll(pszValue, NULL, 0);
				
				if (llTemp != *(long long*)(pPropInfo->propAddress))
				{
					if (Name[0]!='\0')
					*(long long*)(pPropInfo->propAddress) = llTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				wTemp = (WORD)strtol(pszValue, NULL, 0);
				
				if (wTemp != *(WORD*)(pPropInfo->propAddress))
				{
					*(WORD*)(pPropInfo->propAddress) = wTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				dwTemp = strtol(pszValue, NULL, 0);
				
				if (dwTemp != *(DWORD*)(pPropInfo->propAddress))
				{
					*(DWORD*)(pPropInfo->propAddress) = dwTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				
				if (strcmp(pszValue, "true") == 0 || strcmp(pszValue, "TRUE") == 0)
				{
					if (!*(BOOL*)(pPropInfo->propAddress))
					{
						*(BOOL*)(pPropInfo->propAddress) = TRUE;
						m_bPropertyChanged = TRUE;
					}
				}
				else
				{
					if (*(BOOL*)(pPropInfo->propAddress))
					{
						*(BOOL*)(pPropInfo->propAddress) = FALSE;
						m_bPropertyChanged = TRUE;
					}
				}
				break;
			case tmFLOAT:
				*(float*)(pPropInfo->propAddress) = atof(pszValue);
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				*(double*)(pPropInfo->propAddress) = *(double*)pszValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmSTR:
			case tmPROCEDURE:
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
					free(*(char**)pPropInfo->propAddress);
					*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= strlen(pszValue)) )
				{
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) =(char* ) malloc(strlen(pszValue) + 1);//new char[strlen(pszValue) + 1];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char* )malloc(strlen(pszValue) + 1);//new char[strlen(pszValue) + 1];
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),pszValue);
				break;
			default:
				return false;
		}
		
	}
	
	
	return (pPropInfo != NULL);
}


bool 			CtmValueBase::SetProperty(char* pszPropName, void* pValue)
{
	char* pszTemp = NULL; 
	tmPropInfo* pPropInfo = NULL;
	m_bPropertyChanged = FALSE;
	if (pszPropName == NULL) return false;
	pPropInfo = GetProperty(pszPropName);
	if (pPropInfo != NULL)
	{
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				
				if (*(BYTE*)(pPropInfo->propAddress) != *(BYTE*)pValue)
				{
					*(BYTE*)(pPropInfo->propAddress) = *(BYTE*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT:
				if (*(int*)(pPropInfo->propAddress) != *(int*)pValue)
				{
					*(int*)(pPropInfo->propAddress) = *(int*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				if (*(short*)(pPropInfo->propAddress) != *(short*)pValue)
				{
					*(short*)(pPropInfo->propAddress) = *(short*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmLONG:
				if (*(long*)(pPropInfo->propAddress) != *(long*)pValue)
				{
					*(long*)(pPropInfo->propAddress) = *(long*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT64:
				if (*(long long*)(pPropInfo->propAddress) != *(long long*)pValue)
				{
					*(long long*)(pPropInfo->propAddress) = *(long long*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				if (*(WORD*)(pPropInfo->propAddress) != *(WORD*)pValue)
				{
					*(WORD*)(pPropInfo->propAddress) = *(WORD*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				if (*(DWORD*)(pPropInfo->propAddress) != *(DWORD*)pValue)
				{
					*(DWORD*)(pPropInfo->propAddress) = *(DWORD*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				if ( *(BOOL*)pValue != *(BOOL*)(pPropInfo->propAddress) )
				{
					if (*(BOOL*)pValue) *(BOOL*)(pPropInfo->propAddress) = TRUE;
					else *(BOOL*)(pPropInfo->propAddress) = FALSE;	
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmFLOAT:
				*(float*)(pPropInfo->propAddress) = *(float*)pValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				*(double*)(pPropInfo->propAddress) = *(double*)pValue;
				m_bPropertyChanged = TRUE;
				break;
			
			case tmSTR:
			case tmPROCEDURE:
				
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
						free(*(char** )pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}									
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= strlen((char*)pValue)) )
				{
					free(*(char** )pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) =(char*)malloc(strlen((char*)pValue) + 2);// new char[strlen((char*)pValue) + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(strlen((char*)pValue) + 2);//new char[strlen((char*)pValue) + 2];
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),(char*)pValue);
				break;
			default:
				return false;
		}
		
	}
	
	return (pPropInfo != NULL);
}


bool 			CtmValueBase::SetProperty(int nNub, void* pValue)
{
	char* pszTemp = NULL; 
	tmPropInfo* pPropInfo = NULL;
	m_bPropertyChanged = FALSE;
	if (nNub > MAX_PROPERTY && nNub < 0) return false;
	pPropInfo = GetProperty(nNub);
	if (pPropInfo != NULL)
	{
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				if (*(BYTE*)(pPropInfo->propAddress) != *(BYTE*)pValue)
				{
					*(BYTE*)(pPropInfo->propAddress) = *(BYTE*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT:
				if (*(int*)(pPropInfo->propAddress) != *(int*)pValue)
				{
					*(int*)(pPropInfo->propAddress) = *(int*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				if (*(short*)(pPropInfo->propAddress) != *(short*)pValue)
				{
					*(short*)(pPropInfo->propAddress) = *(short*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmLONG:
				if (*(long*)(pPropInfo->propAddress) != *(long*)pValue)
				{
					*(long*)(pPropInfo->propAddress) = *(long*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT64:
				if (*(long long*)(pPropInfo->propAddress) != *(long long*)pValue)
				{
					*(long long*)(pPropInfo->propAddress) = *(long long*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				if (*(WORD*)(pPropInfo->propAddress) != *(WORD*)pValue)
				{
					*(WORD*)(pPropInfo->propAddress) = *(WORD*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				if (*(DWORD*)(pPropInfo->propAddress) != *(DWORD*)pValue)
				{
					*(DWORD*)(pPropInfo->propAddress) = *(DWORD*)pValue;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				if ( *(BOOL*)pValue != *(BOOL*)(pPropInfo->propAddress) )
				{
					if (*(BOOL*)pValue) *(BOOL*)(pPropInfo->propAddress) = TRUE;
					else *(BOOL*)(pPropInfo->propAddress) = FALSE;	
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmFLOAT:
				*(float*)(pPropInfo->propAddress) = *(float*)pValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				*(double*)(pPropInfo->propAddress) = *(double*)pValue;
				m_bPropertyChanged = TRUE;
				break;
			case tmSTR:
			case tmPROCEDURE:
				
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}									
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= strlen((char*)pValue)) )
				{
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = (char*)malloc(strlen((char*)pValue) + 2);//new char[strlen((char*)pValue) + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(strlen((char*)pValue) + 2);//new char[strlen((char*)pValue) + 2];
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),(char*)pValue);
				break;
			default:
				return false;
		}
	}
	
	return (pPropInfo != NULL);
}


bool 			CtmValueBase::SetPropValueT(char* pszPropName, double Value)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(pszPropName);
	
	BYTE	temp;
	int		ntemp;
	short	snTemp;
	long	lTemp;
	long long llTemp;
	WORD	wTemp;
	DWORD	dwTemp;
	BOOL	BLTemp;
	float	fTemp;
	double	dlTemp;
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				temp = static_cast<BYTE>(Value);
				if (temp != *(BYTE*)(pPropInfo->propAddress))
				{
					*(BYTE*)(pPropInfo->propAddress) = temp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT:
				ntemp = static_cast<int>(Value);
				if (ntemp != *(int*)(pPropInfo->propAddress))
				{
					*(int*)(pPropInfo->propAddress) = ntemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				snTemp = static_cast<short>(Value);
				if (snTemp != *(short*)(pPropInfo->propAddress))
				{                                               
					*(short*)(pPropInfo->propAddress) = snTemp;   
					m_bPropertyChanged = TRUE;                    
				}   
				break;
			case tmLONG:
				lTemp = static_cast<long>(Value);
				if (lTemp != *(long*)(pPropInfo->propAddress))
				{
					*(long*)(pPropInfo->propAddress) = lTemp;
					m_bPropertyChanged = TRUE;   
				}
				break;
			case tmINT64:
				llTemp = static_cast<long long>(Value);				
				if (llTemp != *(long long*)(pPropInfo->propAddress))
				{
					*(long long*)(pPropInfo->propAddress) = llTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				wTemp = static_cast<WORD>(Value);
				if (wTemp != *(WORD*)(pPropInfo->propAddress))
				{
					*(WORD*)(pPropInfo->propAddress) = wTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				dwTemp = static_cast<DWORD>(Value);
				if (dwTemp != *(DWORD*)(pPropInfo->propAddress))
				{
					*(DWORD*)(pPropInfo->propAddress) = dwTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				BLTemp = static_cast<BOOL>(Value);
				if (BLTemp != *(BOOL*)(pPropInfo->propAddress))
				{
					*(BOOL*)(pPropInfo->propAddress) = BLTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmFLOAT:
				fTemp = static_cast<float>(Value);
				*(float*)(pPropInfo->propAddress) = fTemp;
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				dlTemp = Value;
				*(double*)(pPropInfo->propAddress) = dlTemp;
				m_bPropertyChanged = TRUE;
				break;
			default:
				Result = false;
				break;
		}
	}
	
	return Result;
}

bool 			CtmValueBase::SetPropValueT(int nNub, double Value)
{
bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(nNub);
	
	BYTE	temp;
	int		ntemp;
	short	snTemp;
	long	lTemp;
	long long llTemp;
	WORD	wTemp;
	DWORD	dwTemp;
	BOOL	BLTemp;
	float	fTemp;
	double	dlTemp;
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmBYTE:
				temp = static_cast<BYTE>(Value);
				if (temp != *(BYTE*)(pPropInfo->propAddress))
				{
					*(BYTE*)(pPropInfo->propAddress) = temp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmINT:
				ntemp = static_cast<int>(Value);
				if (ntemp != *(int*)(pPropInfo->propAddress))
				{
					*(int*)(pPropInfo->propAddress) = ntemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmSHORT:
				snTemp = static_cast<short>(Value);
				if (snTemp != *(short*)(pPropInfo->propAddress))
				{                                               
					*(short*)(pPropInfo->propAddress) = snTemp;   
					m_bPropertyChanged = TRUE;                    
				}   
				break;
			case tmLONG:
				lTemp = static_cast<long>(Value);
				if (lTemp != *(long*)(pPropInfo->propAddress))
				{
					*(long*)(pPropInfo->propAddress) = lTemp;
					m_bPropertyChanged = TRUE;   
				}
				break;
			case tmINT64:
				llTemp = static_cast<long long>(Value);				
				if (llTemp != *(long long*)(pPropInfo->propAddress))
				{
					*(long long*)(pPropInfo->propAddress) = llTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmWORD:
				wTemp = static_cast<WORD>(Value);
				if (wTemp != *(WORD*)(pPropInfo->propAddress))
				{
					*(WORD*)(pPropInfo->propAddress) = wTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmDWORD:
				dwTemp = static_cast<DWORD>(Value);
				if (dwTemp != *(DWORD*)(pPropInfo->propAddress))
				{
					*(DWORD*)(pPropInfo->propAddress) = dwTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmBOOL:
				BLTemp = static_cast<BOOL>(Value);
				if (BLTemp != *(BOOL*)(pPropInfo->propAddress))
				{
					*(BOOL*)(pPropInfo->propAddress) = BLTemp;
					m_bPropertyChanged = TRUE;
				}
				break;
			case tmFLOAT:
				fTemp = static_cast<float>(Value);
				*(float*)(pPropInfo->propAddress) = fTemp;
				m_bPropertyChanged = TRUE;
				break;
			case tmDOUBLE:
				dlTemp = Value;
				*(double*)(pPropInfo->propAddress) = dlTemp;
				m_bPropertyChanged = TRUE;
				break;
			default:
				Result = false;
				break;
		}
	}
	
	return Result;
}

bool 			CtmValueBase::SetPropValueT(char* pszPropName, char* pszValue, int nLen, int nType)
{
bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(pszPropName);
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmFontStream:
				if (pszValue == NULL)
				{
					if((*(char**)pPropInfo->propAddress) != NULL)
					{
						//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if((*(char**)pPropInfo->propAddress) != NULL)
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = NULL;
					(*(char**)pPropInfo->propAddress) =(char*) malloc(nLen);//new char[nLen];
				}
				else if((*(char**)pPropInfo->propAddress) == NULL)
					(*(char**)pPropInfo->propAddress) =(char*)malloc(nLen);// new char[nLen];
				m_bPropertyChanged = TRUE;
				memcpy((*(char**)pPropInfo->propAddress),pszValue,nLen);	
				break;
			default:
				Result = false;
				break;
		}
		if(strcmp(pszPropName, "stream") == 0)
		{
			SetPropValueT("iNetFontLen", 	nLen);
			SetPropValueT("iNetFontType", 	nType);
		}
	}
	return Result;
}	

bool 			CtmValueBase::SetPropValueT(char* pszPropName, char* pszValue)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(pszPropName);
	char* pszTemp = NULL;
	int nStrLen = 0;
	unsigned short *pwDest, *pwCode  = (unsigned short *)pszValue;
	
	if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
	{
	    if (pszValue != NULL)					//Yang 
	    	nStrLen = strlen(pszValue);
	}
	else
	{
	    if(pwCode != NULL)
	    {
    	    while(*pwCode != 0)
    	    {
    	        nStrLen += 2;
    	        pwCode ++;
    	    }
	    }
	    else
	        nStrLen = 0;
	}
	
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmSTR:
			case tmPROCEDURE:
				if (pszValue != NULL) nStrLen = strlen(pszValue);
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
						//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= nStrLen) )       
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = (char*) malloc(nStrLen + 2);//new char[nStrLen + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(nStrLen + 2);//new char[nStrLen + 2];      
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),pszValue);
				
				
				break;
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{
						//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress );
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= nStrLen) )       //netdigger 2007/4/10
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress );
					(*(char**)pPropInfo->propAddress) =(char*)malloc(nStrLen + 2); //new char[nStrLen + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(nStrLen + 2);//new char[nStrLen + 2];      //netdigger 2007/4/10
				}
				m_bPropertyChanged = TRUE;
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
				{
				    strcpy((*(char**)pPropInfo->propAddress),pszValue);
				}
				else
				{
					if (pwCode == NULL) break;
				    pwCode  = (unsigned short *)pszValue;
				    pwDest  = (unsigned short *)(*(char**)pPropInfo->propAddress);
				    memcpy(pwDest, pwCode, nStrLen + 2);
/*				    while(*pwCode != 0)
				    {
				        *pwDest = *pwCode;
				        pwDest ++;
				        pwCode ++;
				    }
				    *pwDest  = 0;
*/
				}
				break;
			default:
				Result = false;
				break;
		}
	}
	return Result;
}

bool 			CtmValueBase::SetPropValueT(int nNub, char* pszValue)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(nNub);
	char* pszTemp = NULL;
	int nStrLen = 0;
	unsigned short *pwDest, *pwCode  = (unsigned short *)pszValue;
	
	if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
	{
	    if (pszValue != NULL)					//Yang 
	    	nStrLen = strlen(pszValue);
	}
	else
	{
	    if(pwCode != NULL)
	    {
    	    while(*pwCode != 0)
    	    {
    	        nStrLen += 2;
    	        pwCode ++;
    	    }
	    }
	    else
	        nStrLen = 0;
	}
	
	if (pPropInfo != NULL)
	{
		Result = true;
		switch (pPropInfo->ptype)
		{
			case tmSTR:
			case tmPROCEDURE:
				if (pszValue != NULL) nStrLen = strlen(pszValue);
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{	//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);					
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= nStrLen) )       
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) = (char*)malloc(nStrLen + 2);//new char[nStrLen + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) =(char*)malloc(nStrLen + 2) ;//new char[nStrLen + 2];      
				}
				m_bPropertyChanged = TRUE;
				strcpy((*(char**)pPropInfo->propAddress),pszValue);
				
				
				break;
			case tmUSTR:
				pszTemp = *(char**)pPropInfo->propAddress;
				if (pszValue == NULL)
				{
					if ((*(char**)pPropInfo->propAddress) != NULL)
					{	//delete  [](*(char**)pPropInfo->propAddress);
						free(*(char**)pPropInfo->propAddress);
						*(char**)pPropInfo->propAddress = NULL;
					}
					break;
				}
				else if ( ((*(char**)pPropInfo->propAddress) != NULL) &&
					   	 (strlen((*(char**)pPropInfo->propAddress)) <= nStrLen) )       //netdigger 2007/4/10
				{
					//delete [](*(char**)pPropInfo->propAddress);
					free(*(char**)pPropInfo->propAddress);
					(*(char**)pPropInfo->propAddress) =(char*)malloc(nStrLen + 2)  ;//new char[nStrLen + 2];
				}
				else if ((*(char**)pPropInfo->propAddress) == NULL)
				{
					(*(char**)pPropInfo->propAddress) = (char*)malloc(nStrLen + 2) ;//new char[nStrLen + 2];      //netdigger 2007/4/10
				}
				m_bPropertyChanged = TRUE;
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
				{
				    strcpy((*(char**)pPropInfo->propAddress),pszValue);
				}
				else
				{
					if (pwCode == NULL) break;
				    pwCode  = (unsigned short *)pszValue;
				    pwDest  = (unsigned short *)(*(char**)pPropInfo->propAddress);
				    memcpy(pwDest, pwCode, nStrLen + 2);
/*				    while(*pwCode != 0)
				    {
				        *pwDest = *pwCode;
				        pwDest ++;
				        pwCode ++;
				    }
				    *pwDest  = 0;
*/
				}
				break;
			default:
				Result = false;
				break;
		}
	}
	return Result;
}


bool 			CtmValueBase::GetPropValueT(char* pszPropName, void* pValue, int nDataSize )
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(pszPropName);
	if (pPropInfo != NULL)
	{
		Result = true;
		if (nDataSize > 0) memset(pValue, 0, nDataSize);
		switch (pPropInfo->ptype)
		{
			case tmBYTE:				
					*(BYTE*)pValue = *(BYTE*)(pPropInfo->propAddress);					
				break;
			case tmINT:
				if (nDataSize == -1 || nDataSize >= sizeof(int))
					*(int*)pValue = *(int*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmSHORT:
				if (nDataSize == -1 || nDataSize >= sizeof(short))
					*(short*)pValue = *(short*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmLONG:
				if (nDataSize == -1 || nDataSize >= sizeof(long))
					*(long*)pValue = *(long*)(pPropInfo->propAddress);
				else 
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmINT64:
				if (nDataSize == -1 || nDataSize >= sizeof(long long))
					*(long long*)pValue = *(long long*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmWORD:
				if (nDataSize == -1 || nDataSize >= sizeof(WORD))
					*(WORD*)pValue = *(WORD*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmDWORD:
				if (nDataSize == -1 || nDataSize >= sizeof(DWORD))
					*(DWORD*)pValue = *(DWORD*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmBOOL:
				if (nDataSize == -1 || nDataSize >= sizeof(WORD))
					*(BOOL*)pValue = *(BOOL*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);	
				}
				break;
			case tmFLOAT:
				if (nDataSize == -1 || nDataSize == sizeof(float))
					*(float*)pValue = *(float*)(pPropInfo->propAddress);
				else if (nDataSize == sizeof(double))
				{
					*(double*)pValue = *(float*)(pPropInfo->propAddress);
				}
				else if  (nDataSize == 1)
				{
					*(BYTE*)pValue = static_cast<BYTE>(*(float*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 2)
				{
					*(short*)pValue = static_cast<short>(*(float*)(pPropInfo->propAddress));
				}
				else Result = false;
				break;
			case tmDOUBLE:
				if (nDataSize == -1 || nDataSize == sizeof(double))
					*(double*)pValue = *(double*)(pPropInfo->propAddress);
				else if (nDataSize == sizeof(float))
				{
					*(float*)pValue = static_cast<float>(*(double*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 1)
				{
					*(BYTE*)pValue = static_cast<BYTE>(*(double*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 2)
				{
					*(short*)pValue = static_cast<short>(*(double*)(pPropInfo->propAddress));
				}
				else Result = false;
				break;
			case tmSTR:
			case tmPROCEDURE:
			    
				if (*(char**)(pPropInfo->propAddress) != NULL)
				{
					if (nDataSize == -1)
						strcpy((char*)pValue, *(char**)(pPropInfo->propAddress));
					else
						strncpy((char*)pValue, *(char**)(pPropInfo->propAddress), nDataSize);
				}
				else ((char*)pValue)[0] = '\0';
				
				
				break;
			case tmUSTR:
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
			    {
    				if (*(char**)(pPropInfo->propAddress) != NULL)
    				{
    					if (nDataSize == -1)
    						strcpy((char*)pValue, *(char**)(pPropInfo->propAddress));
    					else
    						strncpy((char*)pValue, *(char**)(pPropInfo->propAddress), nDataSize);
    				}
    				else ((char*)pValue)[0] = '\0';
				}
				else
				{
				    WORD    *pszSrc, *pszDst;
				    
				    pszSrc  = (WORD *)(*(char**)(pPropInfo->propAddress));
				    pszDst  = (WORD *)pValue;
				    
				    if (pszSrc != NULL)
				    {
				        if(nDataSize == -1)
				        {
				            while(*pszSrc != 0) *pszDst ++ = *pszSrc ++;
				            *pszDst = 0;
				        }
				        else
				        {
				        	int	nStrNum = 2;
				        	while (*pszSrc++ != 0)
				        	{
				        		nStrNum+=2;
				        	}
				        	if (nDataSize > nStrNum) nDataSize = nStrNum;
				        	pszSrc  = (WORD *)(*(char**)(pPropInfo->propAddress));
				            memcpy(pszDst, pszSrc, nDataSize);
				        }
				    }
				    else
				        *pszDst = 0;
				}
				break;
			default:
				Result = false;
				break;
		}
	}
	return Result;	
}

bool 			CtmValueBase::GetPropValueT(int nNub, void* pValue, int nDataSize)
{
	bool	Result = false;
	tmPropInfo* pPropInfo = NULL;
	pPropInfo = GetProperty(nNub);
	if (pPropInfo != NULL)
	{
		Result = true;
		if (nDataSize > 0) memset(pValue, 0, nDataSize);
		switch (pPropInfo->ptype)
		{
			case tmBYTE:				
					*(BYTE*)pValue = *(BYTE*)(pPropInfo->propAddress);					
				break;
			case tmINT:
				if (nDataSize == -1 || nDataSize >= sizeof(int))
					*(int*)pValue = *(int*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmSHORT:
				if (nDataSize == -1 || nDataSize >= sizeof(short))
					*(short*)pValue = *(short*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmLONG:
				if (nDataSize == -1 || nDataSize >= sizeof(long))
					*(long*)pValue = *(long*)(pPropInfo->propAddress);
				else 
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmINT64:
				if (nDataSize == -1 || nDataSize >= sizeof(long long))
					*(long long*)pValue = *(long long*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmWORD:
				if (nDataSize == -1 || nDataSize >= sizeof(WORD))
					*(WORD*)pValue = *(WORD*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmDWORD:
				if (nDataSize == -1 || nDataSize >= sizeof(DWORD))
					*(DWORD*)pValue = *(DWORD*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);
				}
				break;
			case tmBOOL:
				if (nDataSize == -1 || nDataSize >= sizeof(WORD))
					*(BOOL*)pValue = *(BOOL*)(pPropInfo->propAddress);
				else
				{
					memcpy(pValue, pPropInfo->propAddress, nDataSize);	
				}
				break;
			case tmFLOAT:
				if (nDataSize == -1 || nDataSize == sizeof(float))
					*(float*)pValue = *(float*)(pPropInfo->propAddress);
				else if (nDataSize == sizeof(double))
				{
					*(double*)pValue = *(float*)(pPropInfo->propAddress);
				}
				else if  (nDataSize == 1)
				{
					*(BYTE*)pValue = static_cast<BYTE>(*(float*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 2)
				{
					*(short*)pValue = static_cast<short>(*(float*)(pPropInfo->propAddress));
				}
				else Result = false;
				break;
			case tmDOUBLE:
				if (nDataSize == -1 || nDataSize == sizeof(double))
					*(double*)pValue = *(double*)(pPropInfo->propAddress);
				else if (nDataSize == sizeof(float))
				{
					*(float*)pValue = static_cast<float>(*(double*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 1)
				{
					*(BYTE*)pValue = static_cast<BYTE>(*(double*)(pPropInfo->propAddress));
				}
				else if  (nDataSize == 2)
				{
					*(short*)pValue = static_cast<short>(*(double*)(pPropInfo->propAddress));
				}
				else Result = false;
				break;
			case tmSTR:
			case tmPROCEDURE:
			    
				if (*(char**)(pPropInfo->propAddress) != NULL)
				{
					if (nDataSize == -1)
						strcpy((char*)pValue, *(char**)(pPropInfo->propAddress));
					else
						strncpy((char*)pValue, *(char**)(pPropInfo->propAddress), nDataSize);
				}
				else ((char*)pValue)[0] = '\0';
				
				
				break;
			case tmUSTR:
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
			    {
    				if (*(char**)(pPropInfo->propAddress) != NULL)
    				{
    					if (nDataSize == -1)
    						strcpy((char*)pValue, *(char**)(pPropInfo->propAddress));
    					else
    						strncpy((char*)pValue, *(char**)(pPropInfo->propAddress), nDataSize);
    				}
    				else ((char*)pValue)[0] = '\0';
				}
				else
				{
				    WORD    *pszSrc, *pszDst;
				    
				    pszSrc  = (WORD *)(*(char**)(pPropInfo->propAddress));
				    pszDst  = (WORD *)pValue;
				    
				    if (pszSrc != NULL)
				    {
				        if(nDataSize == -1)
				        {
				            while(*pszSrc != 0) *pszDst ++ = *pszSrc ++;
				            *pszDst = 0;
				        }
				        else
				        {
				        	int	nStrNum = 2;
				        	while (*pszSrc++ != 0)
				        	{
				        		nStrNum+=2;
				        	}
				        	
				        	if (nDataSize > nStrNum) nDataSize = nStrNum;
				        	pszSrc  = (WORD *)(*(char**)(pPropInfo->propAddress));
				            memcpy(pszDst, pszSrc, nDataSize);
				        }
				    }
				    else
				        *pszDst = 0;
				}
				break;
			default:
				Result = false;
				break;
		}
	}
	return Result;	
}

int CtmValueBase::GetPropCount()
{
	return PropCount;
}

/*===========================================================================+
|           Class implementation - CtmValueStr                               |
+===========================================================================*/
CtmValueStr::CtmValueStr():CtmValueBase()
{
	strcpy(TYPEName, "CtmValueStr");
	memset(m_pValues, 0, sizeof(m_pValues));
	m_pszDefault = NULL;
	
}

CtmValueStr::~CtmValueStr()
{
	
}

BOOL 	CtmValueStr::CreateA()
{
	
	
	return TRUE;
}


int		CtmValueStr::GetValuesNum()
{
	int nResult = 0;
	
	for (int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
	{
		if (m_pValues[i] != NULL) nResult++;
	}
	
	return nResult;
	
}

void 	CtmValueStr::SetPropertys()
{
	char sz[10];
	for (int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
	{
		sprintf(sz, "%s%d", "value", i);
		//printf("m_pValues[%d]=%s\n",i,m_pValues[i]);
		SetPropInfo(sz,	tmSTR, &m_pValues[i]);		
	}
	
	SetPropInfo("default", tmSTR, &m_pszDefault);	
}

void 	CtmValueStr::SetPropertys_Nub()
{
	for (int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
	{
		SetPropInfo(VALUESTR_VALUE0 + i,	tmSTR, &m_pValues[i]);		
	}
	SetPropInfo(VALUESTR_DEFAULT, tmSTR, &m_pszDefault);	
}





/*===========================================================================+
|           Class implementation - CtmValueInt                               |
+===========================================================================*/
CtmValueInt::CtmValueInt(): CtmValueBase()
{
	strcpy(TYPEName, "CtmValueInt");
	memset(m_Values, 0, sizeof(m_Values));
	m_Default = 0xFFFFFFFF;
	
	for (int i = 0;  i < CNT_INT_MAX_VALUES_NUM; i++)
	{
		m_Values[i] = m_Default;
	}
	
}

CtmValueInt::~CtmValueInt()
{
	
}


BOOL	CtmValueInt::CreateA()
{
	return TRUE;
}

int		CtmValueInt::GetValuesNum()
{
	int nResult = 0;
	
	for (int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
	{
		if (m_Values[i] != m_Default) nResult++;
	}
	return nResult;
	
}

void 	CtmValueInt::SetPropertys()
{
	//printf("SetPropertys...\n");
	char sz[10];
	for (int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
	{
		sprintf(sz, "%s%d", "value", i);
		//printf("m_pValues[%d]=%d\n",i,m_Values[i]);
		SetPropInfo(sz,	tmINT, &m_Values[i]);	
		
	}
	
	SetPropInfo("default", tmINT, &m_Default);	
}

void 	CtmValueInt::SetPropertys_Nub()
{
	for (int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
	{
		SetPropInfo(VALUEINT_VALUE0 + i,	tmINT, &m_Values[i]);		
	}
	
	SetPropInfo(VALUEINT_DEFAULT, tmINT, &m_Default);	
	
}
