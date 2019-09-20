/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : Cartoon                                              			|
|  Task     : Cartoon Manager header file                         			|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                   |
|  Link     :G++(GCC) 3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                    	|
|  Version  : V1.00                                                         |
|  Creation : 2005/2/16                                                    	|
|  Revision :                                                               |
+==========================================================================*/
#include	"cartoon.h"
#include	"tmdebug.h"

//James add
static	BOOL bsClmpUpdate = FALSE;							//判斷開關模重新刷新
static int	nClmpTempData = 0; 								//存儲上一次開關模位置數據，根據數據變化判斷是否刷新
static int	nEjetTempData = 0;								//存儲最新托模數據，托模為0，則開關模不用刷新
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
/*===========================================================================+
|           Class implementation - StaticBox                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmCartoon, CtmWnd)
CtmCartoon::CtmCartoon(tmWND* pwnd)	: CtmWnd()
{
	SetBorderWidth(0);
	CtmWnd::Create(pwnd);
	
	m_pBMP			= NULL;
	m_iBMPCounter	= 0;
	m_pCurBMP		= NULL;
	m_pcBGImage 	= NULL;
	m_wPreData		= 0;
	m_pParent		= NULL;
	m_wCurIndex		= 0;
	m_wUpdated		= 0;
}

//	fans add
CtmCartoon::CtmCartoon(): CtmWnd()
{
	strcpy(TYPEName, "CtmCartoon");	
	SetBorderWidth(0);
	m_pBMP			= NULL;
	m_pCurBMP		= NULL;
	m_iBMPCounter	= 0;
	m_pcBGImage 	= NULL;
	m_wPreData		= 0;
	m_wMax			= 0;
	m_pParent		= NULL;
	m_wCurIndex		= 0;
	m_wUpdated		= 0;
	m_wClear		= 0;
	m_wPreData		= 0;
	m_wUpdated		= 0;
	m_wBGSaved		= 0;
	m_pszMinDataBaseID = NULL;
	m_pszMaxDataBaseID = NULL;
	m_cartoon.pframe_Table = m_frameTable;
	m_cartoon.wType = CARTOON_AUTO;
	m_cartoon.pwData =(int*)malloc(sizeof(int));// new int;
	m_cartoon.pwMaxData =(int*)malloc(sizeof(int));// new int;
	*(m_cartoon.pwData) = 0;
	*(m_cartoon.pwMaxData) = 0;
	m_cartoon.FrameCounter = 1;
	//m_cartoon.hwndParent = 0;
	m_cartoon.pcartParent = NULL;
	m_cartoon.wBmpIndex = 0xffff;
	for (int i = 0 ; i < MAXFRAME; i++)
	{
		m_frameTable[i].cBMPPath = NULL;
		m_frameTable[i].wMin = 0;
		m_frameTable[i].wMax = 0;
	}
	
}

CtmCartoon::CtmCartoon(tmWND* pwnd, tmCARTOON* pcartoon) : CtmWnd()
{
	SetBorderWidth(0);
	if((CARTOON_RECT & pcartoon->wType) != 0)
		m_rect	= pwnd->rect;
	CtmWnd::Create(pwnd);
	
	m_cartoon		= *pcartoon;
	m_pBMP			= NULL;
	m_iBMPCounter	= 0;
	m_pcBGImage 	= NULL;
	m_wPreData		= 0;
	m_wMax			= 0;
	m_pParent		= NULL;
	m_wCurIndex		= 0;
	m_wUpdated		= 0;
		
	//if(pcartoon->hwndParent == 0 && (CARTOON_RECT & pcartoon->wType) != 0)
	if(pcartoon->pcartParent == NULL && (CARTOON_RECT & pcartoon->wType) != 0)
	{
		return;
	}
	Set(pcartoon);
	
	if (m_wnd.wStyle & tmWS_SHOW)             // Show the content if requested
	{
	    Show();
	}
}

CtmCartoon::~CtmCartoon()
{	
	if(m_pBMP != NULL)
	{
		while(m_pBMP->pNext != NULL)
		{
			delete	m_pBMP->pcBitMap;
			m_pBMP	= m_pBMP->pNext;
			delete	m_pBMP->pPre;
		}
		delete m_pBMP->pcBitMap;
		delete m_pBMP;
	}
	
	//delete m_cartoon.pwData;
	if( m_cartoon.pwData!=NULL)
	{
		free( m_cartoon.pwData);
		m_cartoon.pwData=NULL;
	}
	//delete m_cartoon.pwMaxData;
	if(m_cartoon.pwMaxData!=NULL)
	{
		free( m_cartoon.pwMaxData);
		m_cartoon.pwMaxData=NULL;
	}
	m_pBMP	= NULL;
	free(m_pcBGImage);
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CtmCartoon::Update()           /* Virtual function */
{
	
	int	State = 0;
	if(m_cartoon.pwData == NULL)
	{
		return;
	}	
	
	if(m_pCurBMP == NULL)
	{
		return;
	}
	if(m_pParent != NULL && m_pParent->m_wCurIndex < m_cartoon.wBmpIndex)
	{
		return;
	}
	
	//if( 0 == strcmp(this->Name, "cartoonEJECT") )
	//{
	//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log","cartoonEJECT Update() Start\t" );
	//}
	//else if( 0 == strcmp(this->Name, "cartoonCLAMP") )
	//{
	//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log","cartoonCLAMP Update() Start\t" );
	//}
	//else
	//{
	//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log","Other Update() Start\t this->Name = %s, m_wUpdated = %d", this->Name, m_wUpdated );	
	//}
	
	if(m_cartoon.pwMaxData != NULL && m_wMax != *m_cartoon.pwMaxData)
	{
		MaxChange();
		SetCurBMP();
		Clear();
		//CalcRECT();
		Show();
		m_wUpdated = TRUE;
		return;
	}
	if(m_wMax == 0)	
	{
		if(( 0 == strcmp(this->Name, "cartoonEJECT"))&& (GetDBValue("CONF_AD1_WRITE_CHL3_PHYSICALLENGTH").lValue != 0) )						//防止主機暫時停止發送數據，導致圖片顯示不正常
		{
			m_cartoon.wBmpIndex = 0;
			m_bVisible = FALSE;
			bsClmpUpdate = TRUE;
		}
		else if( 0 == strcmp(this->Name, "cartoonCLAMP") && bsClmpUpdate == TRUE )
		{
			m_cartoon.wBmpIndex = 0;
			bsClmpUpdate = FALSE;
		}
		else
		{
			return;	
		}
	}
	
	
	//if(m_wPreData == *m_cartoon.pwData)					//James mark 2007/9/13 01:39下午
	//{
	//	if( 0 == strcmp(this->Name, "cartoonEJECT") )
	//	printf("m_wPreData == *m_cartoon.pwData\n");
	//	return;
	//}
	
	m_wPreData	= *m_cartoon.pwData;
	
	State	= SetCurBMP();
	//if( 0 == strcmp(this->Name, "cartoonEJECT") )
	//{
	//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log", "EJECT UpS State = %d\t", State );
	//}
	//if( 0 == strcmp(this->Name, "cartoonCLAMP") )
	//{
	//	g_tmDebugInfo->LogDebugInfo(  "/usr/i86/Cartoon.log","CLAMP UpS State = %d\n", State );
	//}
	if( State == 0 && bsClmpUpdate == TRUE && ( 0 == strcmp(this->Name, "cartoonCLAMP") ))					//托模改動為false開關模強行刷新
	{
		State = 1;
		bsClmpUpdate = FALSE;
	}
	if(State == 0)
	{ 
		if(m_pParent == NULL)
		{
			m_wUpdated	= FALSE;
			return;
		}
		else
		{
			if(m_pParent->m_wUpdated == FALSE)
			{
				m_wUpdated	= FALSE;
				return;
			}
		}
	}
	
	Clear();
	
	CalcRECT();
	if(State == 1) 
	{
		Show();
	}
	else if(State == 0 && m_pParent != NULL && m_pParent->m_wUpdated == TRUE)
	{
		Show();
	}
	
	//if( 0 == strcmp(this->Name, "cartoonEJECT") )
	//{
	//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log", "cartoonEJECT Update() END\t" );
	//}
	//if( 0 == strcmp(this->Name, "cartoonCLAMP") )
	//{
	//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log","cartoonCLAMP Update()  END\n" );
	//}
	
	m_wUpdated	= TRUE;
}

void 		CtmCartoon::Clear()
{
//	RECT	rect;
	if(m_wClear	== 1)	return;
	if(m_pParent != NULL)
	{ 
		if(m_pParent->m_wCurIndex < m_cartoon.wBmpIndex)
		{
			return;
		}
		//	fans	2006/12/8 11:54上午
		//else if(m_wBGSaved == FALSE)
		//{
		//	_SetViewPort(m_rectClient.left, m_rectClient.top, 
		//			 m_rectClient.right, m_rectClient.bottom);
		//	_GetImage(0, 0, GetClientWidth() - 1, 
		//				GetClientHeight() - 1,  m_pcBGImage);
		//	m_wBGSaved	= TRUE;
		//}
	}
	
	if(m_pcBGImage != NULL)
	{	
		if(m_wBGSaved == TRUE)
		{
			_SetViewPort(m_rectClient.left, m_rectClient.top, 
					 m_rectClient.right, m_rectClient.bottom);
			_PutImage(0, 0, m_pcBGImage, _GPSET);
		}	
	}
	else
	{
		CtmWnd::Clear();
		CalcRECT();
	}
		
	m_wClear	= 1;
}

void	CtmCartoon::Show()
{
	RECT rect = {0,0,0,0};
	
	
	if( 0 == strcmp(this->Name, "cartoonEJECT") )				   //當托模顯示TRUE OR FALSE有變動時，開關模需要重新刷新一次
	{
		static BOOL nbLaster = FALSE;
		if( nbLaster != m_bVisible )
		{
			bsClmpUpdate = TRUE;
			//m_wClear = 0;	
		}
		nbLaster = m_bVisible;
		nEjetTempData = *this->m_cartoon.pwData;					
	}
	if( 0 == strcmp(this->Name, "cartoonCLAMP") )
	{
		if( nClmpTempData != *this->m_cartoon.pwData && nEjetTempData > 0 && (GetDBValue("CONF_AD1_WRITE_CHL3_PHYSICALLENGTH").lValue != 0))	//如果開關模數據有變化時，托模位置不為0，就要刷新開關模的圖片
		{
			bsClmpUpdate = TRUE;
		}
		nClmpTempData = *this->m_cartoon.pwData;;
	}
	
	//if( 0 == strcmp(this->Name, "cartoonEJECT") )
	//{
	//	if( m_bVisible == FALSE )
	//	{
	//			
	//	}
	//	cout << "cartoonEJECT  m_bVisible = " << m_bVisible << endl;
	//}
	//if( 0 == strcmp(this->Name, "cartoonCLAMP") )
	//{
	//	cout << "cartoonCLAMP  m_bVisible = " << m_bVisible << endl;
	//}
	//if( 0 == strcmp(this->Name, "cartoonEJECT") )
	//{
	//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log","EJECT SS m_bVisible =%d \t", m_bVisible );
	//}
	//if( 0 == strcmp(this->Name, "cartoonCLAMP") )
	//{
	//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log","CLAMP SS m_bVisible =%d \n", m_bVisible );
	//}
	
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
	
	if (m_wnd.wStyle & tmWS_SHOW)	//	fans add
	{
		if(m_pCurBMP == NULL)
		{
			return;
		}
		
		if((CARTOON_RECT & m_cartoon.wType) != 0 && m_cartoon.pcartParent == NULL)//m_cartoon.hwndParent == 0)
		{
			return;
		}
			
		if(m_pParent != NULL && m_pParent->m_wCurIndex < m_cartoon.wBmpIndex)
		{
			return;
		}
		
		MKRECT(rect,   0, 0,  GetClientWidth()-1,  GetClientHeight()-1 );
		_SetViewPort(m_rectClient.left, m_rectClient.top, 
					 m_rectClient.right, m_rectClient.bottom);
		//CalcRECT();
		m_pCurBMP->pcBitMap->Show(rect, tmBMP_KEYCOLOR);	//tmBMP_KEYCOLOR
		CtmWnd::Show();
			
		//if( 0 == strcmp(this->Name, "cartoonEJECT") )
		//{
		//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log","cartoonEJECT Show() END \t" );
		//}
		//if( 0 == strcmp(this->Name, "cartoonCLAMP") )
		//{
		//	g_tmDebugInfo->LogDebugInfo( "/usr/i86/Cartoon.log","cartoonCLAMP Show() END \n" );
		//}
		m_wClear	= 0;
	}
}

void	CtmCartoon::Set(tmCARTOON* pcartoon)
{
	int		i	= 0;
	long	Size = 0;
	WORD	wMax = 0, wMin = 0,wStep = 0;
	RECT	rect = {0,0,0,0};
	
	m_cartoon	= *pcartoon;
	
	//=======
	// 申請背景圖片的空間
	//=======
	if(m_pcBGImage != NULL)
	{
		free(m_pcBGImage);
		m_pcBGImage	= NULL;
	}
	
	//if(m_cartoon.hwndParent != 0)
	if (m_cartoon.pcartParent != NULL)
		//m_pParent	= (CtmCartoon *)GetWindow(m_cartoon.hwndParent);
		m_pParent	= (CtmCartoon *)m_cartoon.pcartParent;
	
	if(m_pParent != NULL)
	{
		rect = m_pParent->GetClientRECT();
		
		m_rectWindow.left   = m_wnd.rect.left   + rect.left;
        m_rectWindow.top    = m_wnd.rect.top    + rect.top;
        m_rectWindow.right  = m_wnd.rect.right  + rect.left;
        m_rectWindow.bottom = m_wnd.rect.bottom + rect.top;
        
        m_rectClient.left   = m_rectWindow.left   + m_wBorderWidth;
    	m_rectClient.top    = m_rectWindow.top    + m_wBorderWidth;
    	m_rectClient.right  = m_rectWindow.right  - m_wBorderWidth;
    	m_rectClient.bottom = m_rectWindow.bottom - m_wBorderWidth;
	}	
	
	if(m_wnd.wStyle & tmWS_TRANSPARENT)
	{
		_SetViewPort(m_rectClient.left, m_rectClient.top, 
					 m_rectClient.right, m_rectClient.bottom);
					 
		Size	= _ImageSize( m_rectClient.left,
							  m_rectClient.top,
							  m_rectClient.right,
							  m_rectClient.bottom
							);
		m_pcBGImage	= (char *)malloc(Size);
		memset(m_pcBGImage, 0, sizeof(m_pcBGImage));
	}
	else
		m_pcBGImage	= NULL;
	
	
	if(m_pcBGImage != NULL)
	{
		if(m_pParent == NULL)		
		{
			_GetImage(0, 0, GetClientWidth() - 1, GetClientHeight() - 1,  m_pcBGImage);
			m_wBGSaved 	= TRUE;
		}
		else if(m_pParent != NULL && m_pParent->m_wCurIndex >= m_cartoon.wBmpIndex)
		{
			_GetImage(0, 0, GetClientWidth() - 1, GetClientHeight() - 1,  m_pcBGImage);
			m_wBGSaved 	= TRUE;
		}
		else
		{
			//m_wBGSaved = TRUE;
			//_GetImage(0, 0, GetClientWidth() - 1, GetClientHeight() - 1,  m_pcBGImage);
			//m_wBGSaved = TRUE;
			m_wBGSaved	= FALSE;
		}
	}
	
	//=======
	// 如果原先已經有幀存在，則清除所有的幀
	//=======
	if(m_pBMP != NULL)
	{
		while(m_pBMP->pNext != NULL)
		{
			delete	m_pBMP->pcBitMap;
			m_pBMP	= m_pBMP->pNext;
			delete	m_pBMP->pPre;
		}
		delete m_pBMP->pcBitMap;
		delete m_pBMP;
		m_pCurBMP	= NULL;
		m_pBMP		= NULL;
		m_iBMPCounter= 0;
	}
	
	
	//=======
	// 創建所需要的楨
	//=======
	m_cartoon		= *pcartoon;	
	
	if(m_cartoon.pwMaxData != NULL)
	{
		m_wMax	= *m_cartoon.pwMaxData;
	}

	wStep	= m_wMax / (m_cartoon.FrameCounter);
	if(wStep == 0)	wStep = 1;
								
	for(i = 0; i < m_cartoon.FrameCounter; i ++)
	{
		if((m_cartoon.wType & CARTOON_AUTO) && (m_cartoon.pwMaxData != NULL))
		{
			if(i == 0)			
				wMin	= 0;
			else
				wMin	= wMax + 1;
				
			wMax	= wMin	+ wStep;
			
			m_cartoon.pframe_Table[i].wMin	= wMin;
			m_cartoon.pframe_Table[i].wMax	= wMax;
		}
		
		InsertBMP(&m_cartoon.pframe_Table[i]);
	}
	
	m_pCurBMP	= m_pBMP;
	i	= 0;
	while(m_pCurBMP != NULL)
	{
		m_pCurBMP->nIndex	= i;
		i ++;
		m_pCurBMP			= m_pCurBMP->pNext;
	}
	m_pCurBMP	= m_pBMP;

	if(SetCurBMP() == 1)
	{
		Show();
	}
}



//	與函數Set(tmCARTOON* pcartoon)的區別在於不需要傳入參數
//	fans add
void	CtmCartoon::Set()
{
	int		i	= 0;
	long	Size = 0;
	WORD	wMax = 0, wMin = 0,wStep = 0;
	RECT	rect = {0,0,0,0};
	

	//=======
	// 申請背景圖片的空間
	//=======
	if(m_pcBGImage != NULL)
	{
		free(m_pcBGImage);
		m_pcBGImage	= NULL;
	}
	
	/*
	//if(m_cartoon.hwndParent != 0)
	if (m_cartoon.pcartParent != NULL)
		//m_pParent	= (CtmCartoon *)GetWindow(m_cartoon.hwndParent);
		m_pParent	= (CtmCartoon *)m_cartoon.pcartParent;
	*/
	
	if (GetParent() != NULL && GetParent()->Is("CtmCartoon"))
		m_pParent = (CtmCartoon*)GetParent();
		
	if(m_pParent != NULL)
	{
		rect = m_pParent->GetClientRECT();
		
		m_rectWindow.left   = m_wnd.rect.left   + rect.left;
        m_rectWindow.top    = m_wnd.rect.top    + rect.top;
        m_rectWindow.right  = m_wnd.rect.right  + rect.left;
        m_rectWindow.bottom = m_wnd.rect.bottom + rect.top;
        
        m_rectClient.left   = m_rectWindow.left   + m_wBorderWidth;
    	m_rectClient.top    = m_rectWindow.top    + m_wBorderWidth;
    	m_rectClient.right  = m_rectWindow.right  - m_wBorderWidth;
    	m_rectClient.bottom = m_rectWindow.bottom - m_wBorderWidth;
    	
	}	
	
	if(m_wnd.wStyle & tmWS_TRANSPARENT)
	{
		_SetViewPort(m_rectClient.left, m_rectClient.top, 
					 m_rectClient.right, m_rectClient.bottom);
					 
		Size	= _ImageSize( m_rectClient.left,
							  m_rectClient.top,
							  m_rectClient.right,
							  m_rectClient.bottom
							);
		m_pcBGImage	= (char *)malloc(Size);	
		memset(m_pcBGImage, 0, sizeof(m_pcBGImage));
	}
	else
	{
		
		m_pcBGImage	= NULL;
	}
	
	if(m_pcBGImage != NULL)
	{
		if(m_pParent == NULL)		
		{
			_GetImage(0, 0, GetClientWidth() - 1, GetClientHeight() - 1,  m_pcBGImage);
			m_wBGSaved 	= TRUE;
		}
		else if(m_pParent != NULL && m_pParent->m_wCurIndex >= m_cartoon.wBmpIndex)
		{
			_GetImage(0, 0, GetClientWidth() - 1, GetClientHeight() - 1,  m_pcBGImage);
			m_wBGSaved 	= TRUE;
		}
		else
		{
			//_GetImage(0, 0, GetClientWidth() - 1, GetClientHeight() - 1,  m_pcBGImage);
			//m_wBGSaved 	= TRUE;
			m_wBGSaved	= FALSE;
		}
	}
	
	//=======
	// 如果原先已經有幀存在，則清除所有的幀
	//=======
	if(m_pBMP != NULL)
	{
		while(m_pBMP->pNext != NULL)
		{
			delete	m_pBMP->pcBitMap;
			m_pBMP	= m_pBMP->pNext;
			delete	m_pBMP->pPre;
		}
		delete m_pBMP->pcBitMap;
		delete m_pBMP;
		m_pCurBMP	= NULL;
		m_pBMP		= NULL;
		m_iBMPCounter= 0;
	}
	
	
	//=======
	// 創建所需要的楨
	//=======

	
	if(m_cartoon.pwMaxData != NULL)
	{
		m_wMax	= *m_cartoon.pwMaxData;
	}

	wStep	= m_wMax / (m_cartoon.FrameCounter);
	if(wStep == 0)	wStep = 1;
								
	for(i = 0; i < m_cartoon.FrameCounter; i ++)
	{
		if((m_cartoon.wType & CARTOON_AUTO) && (m_cartoon.pwMaxData != NULL))
		{
			if(i == 0)			
				wMin	= 0;
			else
				wMin	= wMax + 1;
				
			wMax	= wMin	+ wStep;
			
			m_cartoon.pframe_Table[i].wMin	= wMin;
			m_cartoon.pframe_Table[i].wMax	= wMax;
		}
		
		InsertBMP(&m_cartoon.pframe_Table[i]);
	}
	
	m_pCurBMP	= m_pBMP;
	i	= 0;
	while(m_pCurBMP != NULL)
	{
		m_pCurBMP->nIndex	= i;
		i ++;
		m_pCurBMP			= m_pCurBMP->pNext;
	}
	m_pCurBMP	= m_pBMP;

	if(SetCurBMP() == 1)
	{
		Show();
	}
}

/*--------------------------------------------------------------------------+
|           Helpers                                                         |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  	Function	: FindCurBMP                             					|
|	Remark		: 設置動畫的當前幀											|
|---------------------------------------------------------------------------|
|	Parameter	: 															|
|	Return		: 0			:當前幀沒有變化	 		                    	|
|				  1			:當前幀有變化									|
|				  2			:沒有找到匹配的幀								|
|				 -1			:程序錯誤										|
+--------------------------------------------------------------------------*/
int		CtmCartoon::SetCurBMP()
{
	CARTOONBMP*	pCurBMP = NULL;
	static int nTempShow = 0;
	static BOOL	nbTempUpdate = TRUE;
	
	if(m_pCurBMP == NULL) return -1;
	if(m_cartoon.pwData == NULL) return -1;
	
	pCurBMP		= m_pCurBMP;
	//=======
	// 根據pwData的值，決定顯示的BMP圖片
	//=======
	if(*m_cartoon.pwData > pCurBMP->pFrame->wMax)
	{
		while( *m_cartoon.pwData > pCurBMP->pFrame->wMax && 
				pCurBMP->pNext != NULL )
		{
			pCurBMP	= pCurBMP->pNext;
		}
	}
	else
	{
		while( *m_cartoon.pwData < pCurBMP->pFrame->wMin && 
				pCurBMP->pPre != NULL )
		{
			pCurBMP	= pCurBMP->pPre;
		}
	}
	
	if( (*m_cartoon.pwData < pCurBMP->pFrame->wMin || 
		*m_cartoon.pwData > pCurBMP->pFrame->wMax) &&
		(m_cartoon.wType & CARTOON_CLEAR) )
	{
		////printf("Data=%d wMin=%d wMax=%d\n",*m_cartoon.pwData, pCurBMP->pFrame->wMin, pCurBMP->pFrame->wMin);
		return 2;
	}
	
	if(pCurBMP	== m_pCurBMP && m_wClear == 0 )
	{
		return 0;
	}
	//if( nbTempUpdate == TRUE && (0 == strcmp(this->Name, "cartoonCLAMP")) )
	//{
	//	nTempShow++;
	//}
	m_pCurBMP	= pCurBMP;
	m_wCurIndex	= m_pCurBMP->nIndex;
	return 1;
}

int		CtmCartoon::InsertBMP(tmCARTOON_FRAME* pFrame)
{
	int		iIndex = 0;
	
	CARTOONBMP*		pbmp	= new CARTOONBMP;	
	CARTOONBMP*		pCurBMP = NULL;
	CARTOONBMP*		pPreBMP = NULL;
	
	if(pFrame->wMax < pFrame->wMin)	return -1;		//數據錯誤，返回
	
	//=======
	// 鏈表為空
	//=======
	if(m_pBMP	== NULL)
	{
		m_pBMP	= pbmp;
		
		m_pBMP->pNext	= NULL;
		m_pBMP->pPre	= NULL;
		m_pBMP->pcBitMap= new CtmBitMap();
		m_pBMP->pFrame	= pFrame;
		m_pBMP->pcBitMap->Create(pFrame->cBMPPath);
		
		m_iBMPCounter ++;
		
		return 0;		
	}
	
	pCurBMP	= m_pBMP;
	iIndex	= 0;
	
	//=======
	// 搜索插入的位置。
	//=======
	do
	{
		if(pFrame->wMax < pCurBMP->pFrame->wMin)
			break;

		iIndex++;
		
		if(iIndex == m_iBMPCounter)			// 是否已經到了最後節點
			break;
		pCurBMP	= pCurBMP->pNext;		
	} while(pCurBMP != NULL);
	
	
	//=======
	// 加到最後節點之後
	//=======
	if(iIndex == m_iBMPCounter && pCurBMP->pFrame->wMax < pFrame->wMin)
	{
		pbmp->pNext		= NULL;
		pbmp->pPre		= pCurBMP;
		pbmp->pcBitMap	= new CtmBitMap();
		pbmp->pFrame	= pFrame;
		pbmp->pcBitMap->Create(pFrame->cBMPPath);
				
		pCurBMP->pNext	= pbmp;
		m_iBMPCounter ++;
		
		return iIndex;		
	}
	
	//=======
	// 成為第一個節點
	//=======
	if(iIndex == 0)
	{
		pbmp->pNext		= pCurBMP;
		pbmp->pPre		= NULL;
		pbmp->pcBitMap	= new CtmBitMap();
		pbmp->pFrame	= pFrame;
		pbmp->pcBitMap->Create(pFrame->cBMPPath);
		
		pCurBMP->pPre	= pbmp;				
		m_iBMPCounter ++;
		
		m_pBMP			= pbmp;
		
		return iIndex;	
	}
	
	//=======
	// 插入兩個節點中間
	//=======	
	pPreBMP		= pCurBMP->pPre;
	
	if(pPreBMP->pFrame->wMax < pFrame->wMin)
	{
		pbmp->pNext		= pCurBMP;
		pbmp->pPre		= pPreBMP;
		pbmp->pcBitMap	= new CtmBitMap();
		pbmp->pFrame	= pFrame;
		pbmp->pcBitMap->Create(pFrame->cBMPPath);
		
		pPreBMP->pNext	= pbmp;
		pCurBMP->pPre	= pbmp;
		
		m_iBMPCounter ++;
		return iIndex;
	}
	
	delete pbmp;
	return -1;
}

void	CtmCartoon::MaxChange()
{
	int		i = 0;
	WORD	wMax = 0, wMin = 0,wStep = 0;
	
	CARTOONBMP*	pCurBMP = NULL;

	m_wMax	= *m_cartoon.pwMaxData;
	
	if((m_cartoon.wType & CARTOON_AUTO) != 1 || m_wMax == 0)
	{
		//if( 0 == strcmp(this->Name, "cartoonEJECT") )
		//cout << "MaxChange() CARTOON_AUTO Show();"<< endl;
		return;
	}
		
	pCurBMP	= m_pBMP;
	wStep	= m_wMax / m_iBMPCounter;
	
	for(i = 0; i < m_iBMPCounter; i ++)
	{
		if(i == 0)			
			wMin	= 0;
		else
			wMin	= wMax + 1;
			
		if(i == m_iBMPCounter - 1)
			wMax	= m_wMax;
		else
			wMax	= wMin	+ wStep;
		
		pCurBMP->pFrame->wMin		= wMin;
		pCurBMP->pFrame->wMax		= wMax;
		pCurBMP		= pCurBMP->pNext;
	}
	
	SetCurBMP();
	//if( 0 == strcmp(this->Name, "cartoonEJECT") )
	//cout << "MaxChange() Show();"<< endl;
	Show();
	
}



//	fans add
BOOL	CtmCartoon::CreateA()
{
	CtmWnd::CreateA();
	//==================
	//	下面的兩個屬性暫時不開放
	//==================	
	//if (m_pszMinDataBaseID != NULL)	*(m_cartoon.pwData) = GetDBValue(m_pszMinDataBaseID, 1).lValue;		
	//if (m_pszMaxDataBaseID != NULL)	*(m_cartoon.pwMaxData) = GetDBValue(m_pszMaxDataBaseID, 1).lValue;	
	if (GetParent() != NULL)
	{
		if (GetParent()->Is(TYPEName))
		{
			m_cartoon.pcartParent = GetParent();
		}
	}
	
	if(m_cartoon.pcartParent == NULL && (CARTOON_RECT & m_cartoon.wType) != 0)
	{
		return	FALSE;
	}
	Set();
	
	return TRUE;
}

void	CtmCartoon::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("showtype", 	tmWORD,		&m_cartoon.wType);
	SetPropInfo("cartoonmin",	tmINT,		m_cartoon.pwData);
	SetPropInfo("cartoonmax",	tmINT,		m_cartoon.pwMaxData);
	SetPropInfo("cartoonminID",	tmSTR,		&m_pszMinDataBaseID);
	SetPropInfo("cartoonmaxID",	tmSTR,		&m_pszMaxDataBaseID);
	SetPropInfo("framecount",	tmWORD,		&m_cartoon.FrameCounter);
	SetPropInfo("bmpindex",		tmWORD,		&m_cartoon.wBmpIndex);
	
	//===============
	//	設置每一幀的信息，包括圖片的路徑，取值範圍
	//===============
	char Name[20];
	char temp[3];
	for (int i = 0 ; i < MAXFRAME; i++)
	{
		temp[0] = '\0';	
		sprintf(temp, "%d", i);
		
		strcpy(Name, "framebmppath");		
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_frameTable[i].cBMPPath);
	
		strcpy(Name, "framemin");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmINT,	&m_frameTable[i].wMin);
		
		strcpy(Name, "framemax");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmINT,	&m_frameTable[i].wMax);
	}
}

void	CtmCartoon::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	SetPropInfo(CARTOON_SHOWTYPE, 		tmWORD,		&m_cartoon.wType);
	SetPropInfo(CARTOON_CARTOONMIN,		tmINT,		m_cartoon.pwData);
	SetPropInfo(CARTOON_CARTOONMAX,		tmINT,		m_cartoon.pwMaxData);
	SetPropInfo(CARTOON_CARTOONMINID,	tmSTR,		&m_pszMinDataBaseID);
	SetPropInfo(CARTOON_CARTOONMAXID,	tmSTR,		&m_pszMaxDataBaseID);
	SetPropInfo(CARTOON_FRAMECOUNT,		tmWORD,		&m_cartoon.FrameCounter);
	SetPropInfo(CARTOON_BMPINDEX,		tmWORD,		&m_cartoon.wBmpIndex);
	
	//===============
	//	設置每一幀的信息，包括圖片的路徑，取值範圍
	//===============
	int	nIndex = CARTOON_FRAMEBMPPATH0;
	for (int i = 0 ; i < MAXFRAME; i++)
	{
		SetPropInfo(nIndex + i*3, 	tmSTR,	&m_frameTable[i].cBMPPath);
	
		SetPropInfo(nIndex + i*3+1, 	tmINT,	&m_frameTable[i].wMin);
		
		SetPropInfo(nIndex + i*3+2, 	tmINT,	&m_frameTable[i].wMax);
	}
}

