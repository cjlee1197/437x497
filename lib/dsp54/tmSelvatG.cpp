#include	"tmSelvatG.h"
#include	"../../tmnbmsg.h"
#include	"../../app.h"

CtmSelvatG*		g_pSelvatG			= NULL;
int				g_nSelvatCount		= 0;
IMPLEMENT_DYNCREATE(CtmSelvatG, CtmVat_Ancestor)


CIniFile	 m_SelvatIni;	
pthread_mutex_t			m_selvatMutex;
/*===========================================================================+
|           Class implementation - CtmSelvatG		                         |
+===========================================================================*/

CtmSelvatG::CtmSelvatG(): CtmVat_Ancestor()
{
	
}

CtmSelvatG::~CtmSelvatG()
{
	
}


int			CtmSelvatG::CreateSelf()
{
	CtmVat_Ancestor::CreateSelf();
	//printf("dadada\n");
	m_bInTable		= FALSE;
	m_wCurTonValue 	= 0;
	m_wPreTonValue 	= 0;	
	m_nCrossNum		= 0;
    m_nMoldNum		= 0;
    m_pszPath	= NULL;
    	
    
    
    memset(m_aryCross, 0, sizeof(m_aryCross));
    memset(m_aryMold, 0, sizeof(m_aryMold));
    //memset(m_pszPath, 0, sizeof(m_pszPath));
   	
  	pthread_mutex_init(&m_selvatMutex, NULL);

    if (CheckFileExist("outerconfig.ini"))
    {
    
    	m_SelvatIni.OpenFile("outerconfig.ini");
    	//m_SelvatIni.OpenFile("consram.ini");
    
    	//	the readstring will malloc the memory to m_pszPath
    	m_SelvatIni.ReadString("selvat","Path", NULL, &m_pszPath);
    	m_SelvatIni.CloseFile();
    }
    else
    	perror("ERROR READ outerconfig.ini!");
    
    return 0;
   	
}

int			CtmSelvatG::FreeSelf()
{
	delete []m_pszPath;
	
	pthread_mutex_destroy(&m_selvatMutex);
	CtmVat_Ancestor::FreeSelf();
	
	return 0;
}

/*!	\fn BOOL	CtmSelvatG::GetTable()
 *	\brief	獲取當前主機的推力座表
 *	\return HMI是否有主機推力座表，BOOL
 *	\note	
*/
BOOL       	CtmSelvatG::GetTable()
{	
	
	BOOL	bResult = FALSE;
	WORD wTon = GetDBValue("CONF_CODE_READ_NULL_OILVATDATA").lValue & 0xFFFF;	
	//取消F***開頭在外掛文件，若DE的CODE為F***，則外掛文件為***
	if((wTon&0xF000)==0xF000) wTon&=0x0FFF;
	
  pthread_mutex_lock(&m_selvatMutex); /*2016-10-4  開機解析推力座宕機問題處理*/
	if (ReGetTable(wTon))
	{
		bResult 	= InitCrossTable(wTon) & InitMoldTable(wTon);
		m_bInTable 	= bResult;		
	}	
	else bResult = m_bInTable;
	pthread_mutex_unlock(&m_selvatMutex);
	
	return bResult;	
}

/*!	\fn BOOL	CtmSelvatG::ReGetTable(WORD	wTon)
 *	\brief	判斷是否需要重新讀取表（只有當主機code嘛變化的時候，才需要重新獲得）
 *	\return BOOL
 *	\note	
*/
BOOL		CtmSelvatG::ReGetTable(WORD	wTon)
{
	BOOL	bResult = FALSE;
	
	if (!m_bInTable || wTon != m_wPreTonValue) 
	{
		m_wPreTonValue = wTon;		
		bResult = TRUE;
	}
	
	return bResult;
}



/*!	\fn int         CtmSelvatG::GetCrossPos(int x)
 *	\brief	從模板位置轉換成十字頭位置
 *	\param	[in]	int x 模板位置
 *	\return int,十字頭位置
 *	\note	當HMI無對應的推力表,返回x；當x=-1返回-1；當x<=0返回0
*/
int         CtmSelvatG::GetCrossPos(int x)
{
	int     pos;
	int     low, high, mid;
	int     y;
	long    k,deltaX,deltaY;
	DWORD   tempMold;
	
		if (x == 0xFFFF)             return -1;
		else if (x <= 0)         return 0;         //Steven 2004/3/22 add judge x<=0
		else if (!GetTable()) return x;
	
	low  = 0;
	high = m_nMoldNum;
	while (high - low > 1)                     /* stop?                 */
	    {
	    mid = (low + high) / 2;
	    if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77").lValue)
	    	tempMold = 10*(*(m_aryMold+mid));
	    else
	    	tempMold =*(m_aryMold+mid);	
	    if (x >= tempMold)
	        low  = mid;                        /* try right             */
	    else
	        high =mid;                         /* try left              */  
	    }
	pos    = low;     
	deltaX = *(m_aryMold+pos+1) - *(m_aryMold+pos);
	deltaY = *(m_aryCross+pos+1) - *(m_aryCross+pos);
	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77").lValue)/*wangli2015-5-20 18:12:03 關模低壓終止位置精度為2 的flag*/
		y      = (long)(deltaY * (x - 10*(*(m_aryMold+pos))) + 10*deltaX/2) / deltaX /10 + *(m_aryCross+pos);/*wangli2015-5-20 18:28:11 因為精度為2，所以要先放大10倍，然後再除以10*/
	else
	{
		k      = (long)(deltaY * (x - *(m_aryMold+pos)) + deltaX/2) / deltaX;
		y      = (int)k+*(m_aryCross+pos);
  }
  if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77").lValue)/*wangli2015-5-20 18:12:03 關模低壓終止位置精度為2 的flag*/
  {
  	cout <<"come in!!" << endl;
		SendMsg(MSG_NB_HMI_SEND_SELVATGEND, 0, 0, NULL);/*wangli2015-5-20 18:29:51 發消息清除 關模低壓終止位置*/
	}
	
	return y;
		
}


/*!	\fn int         CtmSelvatG::GetMoldPos(int y)
 *	\brief	從十字頭位置轉換成模板位置
  *	\param	[in]	int y 十字頭位置
 *	\return int,模板位置
 *	\note	當HMI無對應的推力表或y=-1,返回y
*/
int         CtmSelvatG::GetMoldPos(int y)
{
	int     pos;
	int     low, high, mid;
	int     x;
	long    k,deltaX,deltaY;
	
		if (!GetTable()||(y == 0xFFFF)) return y;
		if (y >= *(m_aryCross+m_nCrossNum)) return *(m_aryMold+m_nCrossNum);
	
	low = 0;
	high = m_nCrossNum;
	while (high - low > 1)                     /* stop?                 */
	    {
	    mid = (low + high) / 2;
	    if (y >= *(m_aryCross+mid))
	        low  = mid;                        /* try right             */
	    else
	        high =mid;                         /* try left              */  
	    }
	pos    = low;     
	deltaX = *(m_aryMold+pos+1) - *(m_aryMold+pos);
	deltaY = *(m_aryCross+pos+1) - *(m_aryCross+pos);
	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77").lValue)/*wangli2015-5-20 18:10:13 當有推力座時，關模低壓終止位置精度為2（二板機、純二板本身不會有推力座）*/
	{
		k      = (long)(10*deltaX * (y-*(m_aryCross+pos)) + deltaY/2) / deltaY;
		x      = (int)k+10*(*(m_aryMold+pos));
	}
	else
	{
  		k      = (long)(deltaX * (y-*(m_aryCross+pos)) + deltaY/2) / deltaY;
 		x      = (int)k+*(m_aryMold+pos);
 	}
	return x;
	
}

/*!	\fn int			CtmSelvatG::GetCrossNum()
 *	\brief	獲得十字頭表的元素個數
 *	\return int,個數
 *	\note	
*/
int			CtmSelvatG::GetCrossNum()
{
	if (m_bInTable) return m_nCrossNum;
	else return -1;
}


/*!	\fn int			CtmSelvatG::GetMoldNum()
 *	\brief	獲得模板表的元素個數
 *	\return int,個數
 *	\note	
*/
int			CtmSelvatG::GetMoldNum()
{
	if (m_bInTable) return m_nMoldNum;
	else return -1;
	
}


BOOL		CtmSelvatG::TransferToCrossPos(BYTE* pBuffer)
{
	return FALSE;
}

BOOL		CtmSelvatG::SendToHost()
{
	return FALSE;
}

BOOL		CtmSelvatG::TransferToMoldPos()
{
	return FALSE;
}

/*!	\fn BOOL		CtmSelvatG::InitCrossTable(WORD wTon)
 *	\brief	獲得十字頭表
 *	\param	[in]	wTon 主機當前的噸數Code
 *	\return BOOL,是否成功獲取了表
 *	\note	會將獲得表存儲在m_aryCross中，請注意不能超過256個
*/
BOOL		CtmSelvatG::InitCrossTable(WORD wTon)
{
	BOOL	bResult = FALSE;
	
	if (wTon > 0)
	{
		char	szName[256];		
		if (ComposeCrossTableName(wTon, szName))
		{
			m_nCrossNum = GetFileTable(&m_ParseCross, szName, m_aryCross, sizeof(m_aryCross) / sizeof(m_aryCross[0])) - 1;
			if (m_nCrossNum >= 0)
				bResult = TRUE;
			else 
				bResult = FALSE;
		}
		else bResult = FALSE;
	}
	return bResult;
}

/*!	\fn BOOL		CtmSelvatG::InitMoldTable(WORD wTon)
 *	\brief	獲得模板表
 *	\param	[in]	wTon 主機當前的噸數Code
 *	\return BOOL,是否成功獲取了表
 *	\note	會將獲得表存儲在m_aryMold中，請注意不能超過256個
*/ 
BOOL		CtmSelvatG::InitMoldTable(WORD wTon)
{
	BOOL	bResult = FALSE;
	
	if (wTon > 0)
	{
		char	szName[256];		
		if (ComposeMoldTableName(wTon, szName))
		{
			//printf("InitMoldTable=%s\n", szName);
			m_nMoldNum = GetFileTable(&m_ParseMold, szName, m_aryMold,sizeof(m_aryMold) / sizeof(m_aryMold[0])) - 1;
			if (m_nMoldNum >= 0)
				bResult = TRUE;
			else 
				bResult = FALSE;
		}
		else bResult = FALSE;
	}
	
	//printf("InitMoldTable=%d\n", bResult);
	return bResult;
}

/*!	\fn BOOL		CtmSelvatG::ComposeCrossTableName(WORD wTon, char* pszOutName)
 *	\brief	獲得十字頭外部表的文件名稱
 *	\param	[in]	wTon 主機當前的噸數Code
 *	\param	[out]	pszOutName 獲得的文件名稱
 *	\return BOOL,是否成功獲取了文件名稱
 *	\note	文件由config54.ini來決定存放的目錄，如果需要獲得文件名稱，需要wTon>0
*/ 
BOOL		CtmSelvatG::ComposeCrossTableName(WORD wTon, char* pszOutName)
{
	BOOL	bResult = FALSE;
	if (wTon > 0)
	{
		
		//strcpy(m_pszPath, "./lib/dsp54/ht/");
		sprintf(pszOutName, "%s%X%s", m_pszPath, wTon, "Cross");	
		bResult = TRUE;
	}
		
	return bResult;
}

/*!	\fn BOOL		CtmSelvatG::ComposeCrossTableName(WORD wTon, char* pszOutName)
 *	\brief	獲得模板外部表的文件名稱
 *	\param	[in]	wTon 主機當前的噸數Code
 *	\param	[out]	pszOutName 獲得的文件名稱
 *	\return BOOL,是否成功獲取了文件名稱
 *	\note	文件由config54.ini來決定存放的目錄，如果需要獲得文件名稱，需要wTon>0
*/ 
BOOL		CtmSelvatG::ComposeMoldTableName(WORD wTon, char* pszOutName)
{
	BOOL	bResult = FALSE;
	if (wTon > 0)
	{
		//strcpy(m_pszPath, "./lib/dsp54/ht/");
		//printf("ComposeCrossTableName=%X, m_pszPath=%s\n",this, m_pszPath);
		sprintf(pszOutName, "%s%X%s",m_pszPath, wTon, "Mold");		
		bResult = TRUE;
	}
		
	return bResult;
}

/*!	\fn int			CtmSelvatG::GetFileTable(CParseObject*	pParse, char* pszFileName, 
									 int* pTable, int nMaxTableNum)
 *	\brief	獲得外放表的內容
 *	\param	[in]	pParse 使用解析類
 *	\param	[out]	pszFileName	文件名稱
 *	\param	[out]	pTable	表內容放置的數組
 *	\param	[in]	nMaxTableNum 數組的最大個數
 *	\return int,從文件中讀取了多少個元素
 *	\note	
*/ 
int			CtmSelvatG::GetFileTable(CParseObject*	pParse, char* pszFileName, 
									 int* pTable, int nMaxTableNum)
{
	int nResult = 0;
	
	
	//m_ParseMold.OpenFile(pszFileName);
	
	if (CheckFileExist(pszFileName))	//	 the file is exist
	{
		if (pParse != NULL)
		{
			pParse->OpenFile(pszFileName);
			pParse->CloseFile();	
			if (pParse->m_aryControls[0].control != NULL && 
				pParse->m_aryControls[0].control->Is("CtmValueInt"))
			{
				CtmValueInt* pOutObj;
				pOutObj = static_cast<CtmValueInt*>(pParse->m_aryControls[0].control);
				
				nResult = SetTable(pOutObj, pTable, nMaxTableNum);
			}
		}
	}
	
	
	return nResult;
}

/*!	\fn BOOL		CtmSelvatG::CheckFileExist(char* pszFileName)
 *	\brief	檢測文件是否存在
 *	\param	[in]	pszFileName 文件名稱
 *	\return BOOL,是否存在
 *	\note	
*/ 
BOOL		CtmSelvatG::CheckFileExist(char* pszFileName)
{
	BOOL	bResult = FALSE;
	
	if (pszFileName != NULL)
	{
		int nFd = open(pszFileName, O_RDONLY);		
		if (nFd > 0)
		{
			bResult = TRUE;
			close(nFd);
		}
	}
	
	//printf("CheckFileExist %s=%d\n", pszFileName, bResult);
	
	return bResult;
}

/*!	\fn int			CtmSelvatG::SetTable(CtmValueInt* pObj, int* pTable, int nMaxTableNum)
 *	\brief	將指定的CtmValueInt中的內容付給指定的數組
 *	\param	[in]	pObj CtmValueInt類的實例指針
 *	\param	[out]	pTable	放置的數組
 *	\param	[in]	pObj CtmValueInt類的實例指針
 *	\return int,一共得到了多少數據
 *	\note	
*/ 
int			CtmSelvatG::SetTable(CtmValueInt* pObj, int* pTable, int nMaxTableNum)
{
	int nResult = 0;
	
	if (pObj != NULL)
	{
		int nValue	= 0;
		int nNum 	= 0;
		int nTotal 	= pObj->GetValuesNum();
		
		
		int	nIgnore = -1;
		pObj->GetPropValueT("default", &nIgnore);
		
		char sz[10];
		for (int i = 0; i < nMaxTableNum; i++)
		{
			nValue = -1;
			sprintf(sz, "%s%d", "value", i);
			pObj->GetPropValueT(sz, &nValue);
			//printf("%s.%s=%d\n",pObj->Name, sz, nValue);
			
			if (nValue != nIgnore)
			{
				*(pTable + nNum) = nValue;
				nNum++;
				if (nNum >= nTotal) break;
			}			
		}
		
		nResult = nNum;		
	}
	
	return nResult;
}

#ifndef	D_BEOBJECT_CTMSELVATG
CObject*	CreateInstance(char* Name)
{
	if (g_pSelvatG == NULL)
	{
		CObject*	pResult = Create(Name);
		
		if (pResult != NULL)
		{
			g_nSelvatCount++;
			g_pSelvatG = static_cast<CtmSelvatG*>(pResult);			
			g_pSelvatG->CreateSelf();	
		}		
	}
	else g_nSelvatCount++;
		
	//printf("g_pSelevatG=%X\n", g_pSelvatG);
	
	return static_cast<CObject*>(g_pSelvatG);
}

void	ReleaseInstance(CObject* pInstance)
{
	if (g_nSelvatCount > 0)
		g_nSelvatCount--;
	
	if (g_pSelvatG <= 0)
	{
		if (g_pSelvatG != NULL)
			g_pSelvatG->FreeSelf();
		delete g_pSelvatG;
		g_pSelvatG = NULL;
	}
}

#endif

