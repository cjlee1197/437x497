/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : image box Control						     					|
|  Task     : image box Control file                      					|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                   |
|  Link     :G++(GCC) 3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                    	|
|  Version  : V1.00                                                         |
|  Creation : 2005/2/18                                                    	|
|  Revision :                                                               |
+==========================================================================*/
#include	"imagebox.h"
#include	"utils.h"
//#include	"memwatch.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

/*==========================================================================+
|           Class implementation - ImageBox                                 |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmImageBox, CtmWnd)
IMPLEMENT_DYNCREATE(CtmImageGif, CtmImageBox)
CtmImageBox::~CtmImageBox()
{

	if (m_pImageBox->wStyle & tmIMG_CPBKGD)
		m_BgImage.FreeImage();   
	 delete m_pDoubleBuffer;
}

//	fans add
CtmImageBox::CtmImageBox(): CtmWnd()
{
	
	strcpy(TYPEName, "CtmImageBox");

	m_ImageBox.wStyle 		= 1;
	m_pbmpImage 			= NULL;
	m_ImageBox.cImagePath 	= NULL;
	m_bCopy 				= FALSE;
	m_pDoubleBuffer			= NULL;
	m_bDoubelBuffer			= FALSE;
	m_ImageBox.dwKeyColor	= 0xFFFF;
	m_pszClickView			= NULL;
	m_ImageBox.wType		= 0;
}
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
void    CtmImageBox::Show()
{  
	//printf("Show m_pImageBox->cImagePath=%s type=%x\n", m_pImageBox->cImagePath, m_pImageBox->wType);
	if(m_pImageBox==NULL)	return ;
	if (!m_bCopy)
	{
		if (m_pImageBox->wStyle & tmIMG_CPBKGD)
		{ 
			m_BgImage.CopyImage(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right,m_rectWindow.bottom);
			m_bCopy = TRUE;
		}
	}
			

	if (m_pImageBox->cImagePath == NULL || m_pImageBox->cImagePath[0] == '\0') 
	{  
		
		return;
	}
	m_pbmpImage = GetBmpImage();
	
	m_pbmpImage->SetKeyColor(m_pImageBox->dwKeyColor);
	
	
	RECT rect;
	
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
	if ((m_pImageBox->wStyle & tmIMG_ENABLE) && (m_wnd.wStyle & tmWS_SHOW))
	//if (m_pImageBox->wStyle & tmIMG_ENABLE)
	{    
		MKRECT(rect,   0, 0,  GetClientWidth()-1,  GetClientHeight()-1);
		_SetViewPort(m_rectClient.left, m_rectClient.top, 
					 m_rectClient.right, m_rectClient.bottom);
		//m_bmpImage.Show(rect, m_pImageBox->wType);	//	fans mark
		m_pbmpImage->SetDoubleBuffer(m_pDoubleBuffer);
		//printf("m_pImageBox->cImagePath=%s type=%x\n", m_pImageBox->cImagePath, m_pImageBox->wType);
		//printf("rect top =%d bottom =%d left =%d right =%d \n",rect.top,rect.bottom,rect.left,rect.right);
		m_pbmpImage->Show(rect, m_pImageBox->wType);	//	fans add
		
		//	fans add
		CtmWnd::Show();
	}
	ChangePosNow(GetClientRECT());
	//ChangePosRegionSum(GetClientRECT());
}

void    CtmImageBox::Update()
{
	SetDoubleBuffer();	//	fans add 2008-10-22 10:57:49
	
	if (m_pDoubleBuffer == NULL)
		Clear();
/*	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)*/
		Show();
}
void    CtmImageBox::Update2()
{
	SetDoubleBuffer();	//	fans add 2008-10-22 10:57:49
	
/*	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)*/
		Show();
}
void	CtmImageBox::Clear()
{
	//SetDoubleBuffer();
	if (m_pImageBox->wStyle & tmIMG_CPBKGD)
	{
		if (m_bCopy)
		{
			m_BgImage.ShowImage(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom);
		}
	}
	else
	{
		CtmWnd::Clear();
		CalcRECT();
	}
	ChangePosNow(GetClientRECT());
}


//	fans add
void 	CtmImageBox::SetPropertys()
{
	CtmWnd::SetPropertys();

	SetPropInfo("type", 		tmWORD, 	&m_ImageBox.wType);
	SetPropInfo("style",		tmWORD,		&m_ImageBox.wStyle);
	SetPropInfo("imagepath",	tmSTR,		&m_ImageBox.cImagePath);
	SetPropInfo("keycolor", 	tmLONG,		&m_ImageBox.dwKeyColor);
	SetPropInfo("doublebuffer",	tmBOOL,		&m_bDoubelBuffer);
	SetPropInfo("clickview",	tmSTR,		&m_pszClickView);
}

void 	CtmImageBox::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();

	SetPropInfo(IMAGEBOX_TYPE, 		tmWORD, 	&m_ImageBox.wType);
	SetPropInfo(IMAGEBOX_STYPE,		tmWORD,		&m_ImageBox.wStyle);
	SetPropInfo(IMAGEBOX_IMAGEPATH,	tmSTR,		&m_ImageBox.cImagePath);
	SetPropInfo(IMAGEBOX_KEYCOLOR, 	tmLONG,		&m_ImageBox.dwKeyColor);
	SetPropInfo(IMAGEBOX_DOUBLEBUFFER,	tmBOOL,		&m_bDoubelBuffer);
	SetPropInfo(IMAGEBOX_CLICKVIEW,	tmSTR,		&m_pszClickView);
}


//	fans add
BOOL 	CtmImageBox::CreateA()
{
	CtmWnd::CreateA();

	m_pImageBox = &m_ImageBox;
	
	SetDoubleBuffer();

}

WORD    CtmImageBox::OnKey(WORD wKey)
{
	char        acKey[2];
	
	if (tmHIBYTE(wKey) == 0)
	{
		acKey[0] = tmLOBYTE(wKey);
	    acKey[1] = '\0';
	}
	switch(wKey)
	{
		case _ENTER:
			if(m_pszClickView != NULL)
			{
				::PutCommand(m_pszClickView);
			}
			break;
		default:
			break;
	}
}

//	fans 2008-10-22 10:54:50
void	CtmImageBox::SetDoubleBuffer()
{
	if (m_bDoubelBuffer && m_pDoubleBuffer == NULL)
	{
		m_pDoubleBuffer = new CtmGDIDoubleBuffer(m_rectClient);
		m_pDoubleBuffer->InitBuffer(m_bgc);
	}
	else if(m_pDoubleBuffer!=NULL)
	{
		m_pDoubleBuffer->InitFrameBuffer();
		m_pDoubleBuffer->SetRegion(m_rectClient);	
	}
}

CtmBitMap*		CtmImageBox::GetBmpImage()
{
	int i = 0;
	bool b_find = false;
	
	CtmBitMap* pBmpImage = NULL;
	while (g_aIniBmp[i].cBMPPath[0] != '\0' && i < IMAGE_MAXNUM)
	{
		if (strcmp(g_aIniBmp[i].cBMPPath, m_pImageBox->cImagePath) == 0)
		{
			pBmpImage = g_aIniBmp[i].pIniBmp; 
			b_find = true;
			//cout << Name << " finded the bitmap!!" << endl;
			break;
		}
		i++;
	}	
	if (!b_find)
	{
		if(i < IMAGE_MAXNUM)
		{
			g_aIniBmp[i].pIniBmp = new CtmBitMap;
			g_aIniBmp[i].pIniBmp->Create(m_pImageBox->cImagePath);
			strncpy(g_aIniBmp[i].cBMPPath, m_pImageBox->cImagePath, 256);
			pBmpImage = g_aIniBmp[i].pIniBmp; 
		}
		else
		{
			m_bmpImage.Create(m_pImageBox->cImagePath);	
			pBmpImage = &m_bmpImage;	
		}
		
	}
	
	return pBmpImage;
}


CtmImageGif::CtmImageGif(): CtmImageBox()
{
	strcpy(TYPEName, "CtmImageGif");
	m_pGifImage 			= NULL;
	m_FrameIndex			= 0;
	
	m_llMax		= 0;
	m_llMin		= 0;
	m_llValue	= 0;
	
	m_lMaxFrame= 0;
	m_lMinFrame= 0;
	m_wDirection = GIF_FWD;
	m_pszDBID	= NULL;
	m_pszMaxID	= NULL;
	m_pszMinID	= NULL;
	
}
CtmImageGif::~CtmImageGif()
{
	
}

BOOL 	CtmImageGif::CreateA()
{
	CtmImageBox::CreateA();
	
	//printf("m_lMinFrame = %d\n", m_lMinFrame);
	//printf("m_lMaxFrame = %d\n", m_lMaxFrame);
}

void 	CtmImageGif::SetPropertys()
{
	CtmImageBox::SetPropertys();

	//SetPropInfo("frameindex", 	tmWORD, 	&m_FrameIndex);
	
	SetPropInfo("dbid",			tmSTR, 		&m_pszDBID);
	SetPropInfo("maxid",		tmSTR, 		&m_pszMaxID);
	SetPropInfo("minid",		tmSTR, 		&m_pszMinID);
	SetPropInfo("value", 		tmINT64, 	&m_llValue);	
	SetPropInfo("min", 			tmINT64, 	&m_llMin);
	SetPropInfo("max", 			tmINT64, 	&m_llMax);	
	SetPropInfo("minframe", 	tmINT, 		&m_lMinFrame);
	SetPropInfo("maxframe", 	tmINT, 		&m_lMaxFrame);
	SetPropInfo("direction", 	tmWORD, 	&m_wDirection);
}

void    CtmImageGif::Show()
{
	//printf("m_bCopy = %d, m_pImageBox->wStyle = %d\n", m_bCopy, m_pImageBox->wStyle);
	//for(int y = 0; y < 600; y++)
	//{
	//	for(int x = 0; x < 800; x++)
	//	{
	//		printf("%d ", g_Rows[y][x]);
	//	}
	//	printf("\n");
	//}
	if (!m_bCopy)
	{
		
		if (m_pImageBox->wStyle & tmIMG_CPBKGD)
		{
			m_BgImage.CopyImage(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right,m_rectWindow.bottom);
			m_bCopy = TRUE;
		}
	}
			

	if (m_pImageBox->cImagePath == NULL || m_pImageBox->cImagePath[0] == '\0') 
	{
		return;
	}
	
	m_pGifImage = GetGifImage();
	
	m_pGifImage->SetKeyColor(m_pImageBox->dwKeyColor);
	
	RECT rect;
	
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
	if ((m_pImageBox->wStyle & tmIMG_ENABLE) && (m_wnd.wStyle & tmWS_SHOW))
	{
		MKRECT(rect,   0, 0,  GetClientWidth()-1,  GetClientHeight()-1)
		
		_SetViewPort(m_rectClient.left, m_rectClient.top, 
					 m_rectClient.right, m_rectClient.bottom);
		m_pGifImage->SetDoubleBuffer(m_pDoubleBuffer);
		
		m_FrameIndex = GetFrameIndex();
		//printf("Name = %s, m_FrameIndex = %d\n", Name, m_FrameIndex);
		m_pGifImage->Show(rect, m_pImageBox->wType, m_FrameIndex);
		
		//CtmWnd::Show();
	}

}

void    CtmImageGif::Update()
{
	SetDoubleBuffer();	//	fans add 2008-10-22 10:57:49
	
	if (m_pDoubleBuffer == NULL)
		Clear();
	Show();
}

void    CtmImageGif::Update2()
{
	SetDoubleBuffer();	//	fans add 2008-10-22 10:57:49
	
	//if (m_pDoubleBuffer == NULL)
	//	Clear();
	Show();
}

gif_animation* 	CtmImageGif::GetGifInfo()
{
	if(m_pGifImage!=NULL)
		return m_pGifImage->GetInfo();
	else
		return NULL;
}

WORD	CtmImageGif::GetFrameIndex()
{
	WORD wFrameIndex = 0;
	WORD wFrameCount = m_lMaxFrame - m_lMinFrame+1;
	
	if (m_pszMaxID != NULL)
		m_llMax	= GetDBValue(m_pszMaxID).lValue;
	
	
	if (m_pszMinID != NULL)
		m_llMin	= GetDBValue(m_pszMinID).lValue;
	
	if (m_pszDBID != NULL)
	{
		m_llValue	= GetDBValue(m_pszDBID).lValue;
		
		if (m_pszMinID == NULL)
			m_llMin		= GetDBValue(m_pszDBID).lMinValue;
		
		if (m_pszMaxID == NULL)
			m_llMax		= GetDBValue(m_pszDBID).lMaxValue;	
	}
	
	if (m_llMax > m_llMin)
		wFrameIndex	= wFrameCount * (m_llValue - m_llMin+1)/ (m_llMax - m_llMin+1) -1;
	else
		wFrameIndex	= 0;	
	
	wFrameIndex += m_lMinFrame;
	
	if(m_wDirection == GIF_BWD) wFrameIndex = m_lMaxFrame - wFrameIndex + m_lMinFrame;
	return  wFrameIndex;	
}

CtmGif*		CtmImageGif::GetGifImage()
{
	int i = 0;
	bool b_find = false;
	
	CtmGif* pGifImage = NULL;
	while (g_aIniGif[i].cGifPath[0] != '\0' && i < IMAGE_MAXNUM)
	{
		if (strcmp(g_aIniGif[i].cGifPath, m_pImageBox->cImagePath) == 0)
		{
			pGifImage = g_aIniGif[i].pIniGif; 
			b_find = true;
			break;
		}
		i++;
	}	
	if (!b_find)
	{
		if(i < IMAGE_MAXNUM)
		{
			g_aIniGif[i].pIniGif = new CtmGif;
			g_aIniGif[i].pIniGif->Create(m_pImageBox->cImagePath);
			g_aIniGif[i].pIniGif->SetRegion(m_rectClient);
			strncpy(g_aIniGif[i].cGifPath, m_pImageBox->cImagePath, 256);
			pGifImage = g_aIniGif[i].pIniGif;
		}
		else
		{
			m_GifIamge.Create(m_pImageBox->cImagePath);	
			m_GifIamge.SetRegion(m_rectClient);
			pGifImage = &m_GifIamge;
		} 
		
	}
	return pGifImage;
}
