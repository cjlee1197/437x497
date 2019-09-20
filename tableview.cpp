/* Copyright (C), 2003-2014, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmTableView                                               	 |
|  Task     : CtmTableView   Manager                                      	 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Wong                                                          |
|  Version  : V1.00                                                          |
|  Creation : 11/24/2014                                                     |
|  Revision :                                                                |
+===========================================================================*/


#include 	"tableview.h"
#include	"tmconfig.h"
#include	"commonaction.h"

/*---------------------------------------------------------------------------+
|               Constants											 		 |
+---------------------------------------------------------------------------*/
#define     FLAG_LT_VERTLINE               0x0001  
#define     FLAG_LT_HORILINE               0x0002


IMPLEMENT_DYNCREATE(CtmTableView, CtmWnd)

CtmTableView::CtmTableView():CtmWnd()
{
	strcpy(TYPEName, "CtmTableView");	
	m_wRowCount		=0;
	m_wColCount		=0;
	m_nRowIndex		=0;
	m_nColIndex		=0;
	m_wRowWidth		=60;
	m_wAlign		=0x20;
	m_nRight		=0;
	m_wRealRowCount	=0;
	m_wRealColCount	=0;
	m_bBar			= TRUE;
	m_bGetFocus		= FALSE;
	m_bTableType	= FALSE;
	m_bBackGround	= FALSE;
	m_Context_lValue[MAX_ROWS_COUNT][MAX_COLUMNS_COUNT]=0;
	m_pInterSection_Item=NULL;
	for(int i = 0; i < MAX_COLUMNS_COUNT; i++)
	{
		m_pColumns_Item[i]		=NULL;
		m_Columns[i].wFontSize 	= 16;
		m_Columns[i].wType		= 0x30;
		m_Columns[i].wWidth		=60;
		
		m_Columns[i].bgc		=42260;
		m_Columns[i].pszTextID	= NULL;
		m_Columns[i].pszText	= NULL;
	
		m_Context[i].wFontSize	=16;
		m_Context[i].wType		=0x20;
		m_Context[i].bgc		=5535;
		m_Context[i].fgc		=63488;
		m_Context[i].wPrecision	=0;
		m_Context[i].wMaxchar	=16;
		m_Context[i].bEnable	=TRUE;
		m_Context[i].wAlign		=0x20;
		
	}
	for(int i=0; i<MAX_ROWS_COUNT;i++)
	{
		for(int j=0; j<MAX_COLUMNS_COUNT;j++)
		{
			//m_wItemType[i][j]=0;
			m_pContext_Item[i][j] 	= NULL;
				
		}
		m_pRows_Item[i]		=NULL;
		m_Rows[i].wFontSize = 16;
		m_Rows[i].wType		= 0x30;
		m_Rows[i].bgc		=42260;
		m_Rows[i].pszTextID	= NULL;
		m_Rows[i].pszText	= NULL;
	}
	m_InterSection.wFontSize	=16;
	m_InterSection.wType		=0x30;
	m_InterSection.bgc			=65535;
	m_InterSection.wWidth		=60;
	m_InterSection.pszTextID	=NULL;
	m_InterSection.pszText		=NULL;					
		
}

void    CtmTableView::DrawTable()
{
	m_nRight  = m_rectWindow.right  - m_rectWindow.left;
	short nBottom = m_rectWindow.bottom - m_rectWindow.top;
	short       nXRemain = 0, nIntervalX = 0;
    short       nYRemain = 0, nIntervalY = 0;
    short       nXPoint  = 0, nYPoint    = 0;
    short       nLines;
    short		nLineWidth=1;
    if(m_bTableType)
	{
		m_nRight=m_InterSection.wWidth;
		for(int i=0;i<m_wColCount;++i)
			m_nRight+=m_Columns[i].wWidth;	
	}
	_SetViewPort(	m_rectWindow.left, 
					m_rectWindow.top, 
					m_nRight+m_rectWindow.left, 
					m_rectWindow.bottom
				);
	//printf("nRight=%d\n",m_nRight);
	 _SetColor(m_wBorderColor);
    _Rectangle(_GBORDER, 0, 0, m_nRight, nBottom);

   // _SetColor(m_bgc);
    for (int i=1; (WORD) i< (m_wBorderWidth); i++)
    {
        _Rectangle(_GBORDER, i, i, m_nRight-i, nBottom-i);
    }
    m_wRealRowCount=m_wRowCount+1;
    m_wRealColCount=m_wColCount+1;   
    m_wColHeight=nIntervalY=nBottom/m_wRealRowCount;
    printf("DrawTable:m_wColHeight=%d\n",m_wColHeight);
    nYRemain=nBottom%m_wRealRowCount;
    nLines=m_wRealRowCount;
    if(m_wBorderWidth>=3)	
  	 	nLineWidth=m_wBorderWidth-1;
  	else
  	 	nLineWidth=m_wBorderWidth;
    for(int i=0; i<nLines-1;i++)
    {
   		nYPoint+=nIntervalY;
   		if(nYRemain>0)
   		{
   			nYRemain--;
   			nYPoint++;
   		}
   		DrawLine( 0, nYPoint-(nLineWidth>>1), m_nRight-1, nYPoint-(nLineWidth>>1), nLineWidth, FLAG_LT_VERTLINE);
    }
	   
	 //  printf("Tabletype=%d\n",m_bTableType);	
	if(!m_bTableType)
    {  
	   	m_InterSection.wWidth=m_wRowWidth=nIntervalX=m_nRight/m_wRealColCount;
	   	nXRemain=m_nRight%m_wRealColCount;
	   	nLines=m_wRealColCount;
		for(int i=0; i<nLines-1; i++)
		{
		   nXPoint+=nIntervalX;
		   if(nXRemain>0)
		   {
		   		nXRemain--;
		   		nXPoint++;	
		   }
		   		DrawLine(nXPoint-(nLineWidth>>1), 0, nXPoint-(nLineWidth>>1), nBottom-1, nLineWidth, FLAG_LT_HORILINE);
		}
	}
	else
	{
		nLines=m_wRealColCount;
		for(int i=0; i<nLines-1; i++)
		{
			/// drawline
			if(i==0)
			{
				m_wRowWidth=m_InterSection.wWidth;	
			}
			else
			{
				m_wRowWidth=m_Columns[i-1].wWidth;
			}
			 nXPoint+=m_wRowWidth;
			printf("nXPoint=%d\n",nXPoint);
			DrawLine(nXPoint-(nLineWidth>>1), 0, nXPoint-(nLineWidth>>1), nBottom-1, nLineWidth, FLAG_LT_HORILINE);		
		} 	
	}
    
}

CtmTableView::~CtmTableView()
{
	if(m_pInterSection_Item!=NULL)
	{
		delete 	m_pInterSection_Item;
		m_pInterSection_Item=NULL;
	}
	for(int i=0; i<m_wColCount; i++)
	{
		if(m_pColumns_Item[i]!=NULL)
		{
			delete m_pColumns_Item[i];
			m_pColumns_Item[i]=NULL;
		}
	}
	for(int i=0; i<m_wRowCount; i++)
	{
		if(m_pRows_Item[i]!=NULL)
		{
			delete m_pRows_Item[i];
			m_pRows_Item[i]=NULL;
		}
		for(int j=0; j<m_wColCount; j++)
		{
			if(m_pContext_Item[i][j] != NULL)
			{
				delete m_pContext_Item[i][j];
				m_pContext_Item[i][j] = NULL;	
			}	
		}
	}
}

BOOL	CtmTableView::CreateA()
{
	//Show();
	CtmWnd::CreateA();
	if(m_wColCount>MAX_COLUMNS_COUNT)	m_wColCount=MAX_COLUMNS_COUNT;
	if(m_wRowCount>MAX_ROWS_COUNT)		m_wRowCount=MAX_ROWS_COUNT;
	//printf("m_wRowCount=%d,m_wColCount=%d\n",m_wRowCount,m_wColCount);
	//SetInitScrollbar();
	CreateColumns();
	CreateRows();
	CreateInterSection();
	CreateItems();
	
	return TRUE;
}
void    CtmTableView::Show()
{
	//printf("Show row=%d,col=%d\n",m_wRowCount,m_wColCount);
	
	SetBorderWidth(3);
	DrawTable();
	ShowInterSection();
	ShowColumns();
	ShowRows();
	ShowItems();
	//ShowScrollbar();
	CtmWnd::Show();
}
void    CtmTableView::Update()
{
	Clear();
	Show();
	CtmWnd::Update();
}
void   	CtmTableView::Clear()
{
	if (m_wnd.wStyle & tmWS_TRANSPARENT) 
    {
        if (!m_bBackGround) 
        {
            m_BackImage.CopyImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                              m_rectClient.bottom);
            m_bBackGround = TRUE;
        }
        else
        {
            m_BackImage.ShowImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                              m_rectClient.bottom);
        }
    }
    else
    {
        CtmWnd::Clear();
    }
}

void 	CtmTableView::SetPropertys()
{
	char  PropName[64];
	CtmWnd::SetPropertys();
	SetPropInfo("colcount", 	tmWORD, &m_wColCount);
	SetPropInfo("rowcount", 	tmWORD, &m_wRowCount);
	SetPropInfo("align", 	    tmWORD, &m_wAlign);
	//set intersection attribute
	SetPropInfo("ibgc",			tmWORD, &m_InterSection.bgc);
	SetPropInfo("ifgc",			tmWORD, &m_InterSection.fgc);
	SetPropInfo("iwtype",		tmWORD, &m_InterSection.wType);
	SetPropInfo("ifontsize",	tmWORD, &m_InterSection.wFontSize);
	SetPropInfo("itext",		tmSTR,  &m_InterSection.pszText);
	SetPropInfo("itextID",		tmUSTR, &m_InterSection.pszTextID);
	SetPropInfo("iwidth",		tmWORD,	&m_InterSection.wWidth);
	SetPropInfo("iHeight",		tmWORD, &m_InterSection.wHeight);
	SetPropInfo("tabletype",	tmBOOL, &m_bTableType);
	
	for (int i = 0; i < MAX_COLUMNS_COUNT; i++)
	{
		sprintf(PropName, "ColWidth%d", i);		
		SetPropInfo(PropName, 	tmWORD,	&m_Columns[i].wWidth);
		sprintf(PropName, "Colfgc%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Columns[i].fgc);
		sprintf(PropName, "Colbgc%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Columns[i].bgc);
		sprintf(PropName, "ColType%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Columns[i].wType);
		sprintf(PropName, "ColFontSize%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Columns[i].wFontSize);
		sprintf(PropName, "ColText%d", i);
		SetPropInfo(PropName, 	tmSTR,	&m_Columns[i].pszText);
		sprintf(PropName, "ColTextID%d", i);
		SetPropInfo(PropName, 	tmUSTR,	&m_Columns[i].pszTextID);
		
		sprintf(PropName, "Itemfgc%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Context[i].fgc);
		sprintf(PropName, "Itembgc%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Context[i].bgc);
		sprintf(PropName, "ItemType%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Context[i].wType);
		sprintf(PropName, "ItemFontSize%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Context[i].wFontSize);
		sprintf(PropName,"ItemPrecision%d",i);
		SetPropInfo(PropName, tmWORD,	&m_Context[i].wPrecision);
		sprintf(PropName,"ItemMaxchar%d",i);
		SetPropInfo(PropName, tmWORD,	&m_Context[i].wMaxchar);
		sprintf(PropName,"ItemEnabled%d",i);
		SetPropInfo(PropName, tmBOOL,	&m_Context[i].bEnable);
		sprintf(PropName,"ItemwAlign%d",i);
		SetPropInfo(PropName,tmWORD, &m_Context[i].wAlign);
	}
	for (int i = 0; i < MAX_ROWS_COUNT; i++)
	{
		sprintf(PropName, "RowHeight%d", i);		
		SetPropInfo(PropName, 	tmWORD,	&m_Rows[i].wHeight);
		sprintf(PropName, "Rowfgc%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Rows[i].fgc);
		sprintf(PropName, "Rowbgc%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Rows[i].bgc);
		sprintf(PropName, "RowType%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Rows[i].wType);
		sprintf(PropName, "RowFontSize%d", i);
		SetPropInfo(PropName, 	tmWORD,	&m_Rows[i].wFontSize);
		sprintf(PropName, "RowText%d", i);
		SetPropInfo(PropName, 	tmSTR,	&m_Rows[i].pszText);
		sprintf(PropName, "RowTextID%d", i);
		SetPropInfo(PropName, 	tmUSTR,	&m_Rows[i].pszTextID);
	}
}

void   	 	CtmTableView::OnGetFocus()
{
	m_bGetFocus		= TRUE;
	//ShowItems();
}

void    	CtmTableView::OnLoseFocus()
{
	m_bGetFocus		= FALSE;
	//ShowItems();
}


void    CtmTableView::DrawLine(short left, short top, short right, short bottom, short nLineWidth, WORD wType)
{
    short   i;

    if((left < 0) || (top < 0) || (right < 0) || (bottom < 0))  return;
    _SetColor(m_wBorderColor);
    if(wType & FLAG_LT_VERTLINE)   _Line(left+4, top, right-6, bottom);
    if(wType & FLAG_LT_HORILINE)   _Line(left, top+4, right, bottom - 6);
    //_SetColor(m_wBorderColor);
    for(i=0; i<nLineWidth; i++)
    {
        if(wType & FLAG_LT_VERTLINE)   _Line(left+1, top+i+1, right-1, bottom+i+1);
        if(wType & FLAG_LT_HORILINE)   _Line(left+i+1, top+1, right+i+1, bottom-1);
    }
   // _SetColor(m_wBorderColor);
    if(wType & FLAG_LT_VERTLINE)   _Line(left+4, top+nLineWidth+1, right-6, bottom+nLineWidth+1);
    if(wType & FLAG_LT_HORILINE)   _Line(left+nLineWidth+1, top+4, right+nLineWidth+1, bottom - 6);
}

void		CtmTableView::ShowColumns()
{
	WORD wLeftStart=0;
	RECT	rect;
	int nRemainX=0;	
	nRemainX=m_nRight%m_wRealColCount;
	if(!m_bTableType)
	{
		for(int i = 0; i < m_wColCount; i++)
		{
			if(m_pColumns_Item[i]!=NULL)
			{
				m_pColumns_Item[i]->SetPropValueT("type", 2);
				m_pColumns_Item[i]->SetPropValueT("static_wtype", 	0x22);
				m_pColumns_Item[i]->SetPropValueT("fgc", 	m_Columns[i].fgc);
				m_pColumns_Item[i]->SetPropValueT("bgc", 	m_Columns[i].bgc);
				m_pColumns_Item[i]->SetPropValueT("align", 	m_wAlign);
		
				wLeftStart +=m_wRowWidth;	
				rect.left	=	wLeftStart;
				rect.right	= 	rect.left + m_wRowWidth;
				rect.top	=	0;
				rect.bottom	=	m_wColHeight;
				m_pColumns_Item[i]->SetPropValueT("left", 	rect.left);
				m_pColumns_Item[i]->SetPropValueT("right", 	rect.right);
				m_pColumns_Item[i]->SetPropValueT("top", 	rect.top);
				m_pColumns_Item[i]->SetPropValueT("bottom", rect.bottom);
				m_pColumns_Item[i]->SetPropValueT("textwidth", m_wRowWidth-3);
				m_pColumns_Item[i]->SetPropValueT("itemheight", m_wColHeight-3);
				
				m_pColumns_Item[i]->CreateA();
				printf(" %d:m_Columns[i].pszTextID=%s,m_Columns[i].pszText=%s\n",i,m_Columns[i].pszTextID,m_Columns[i].pszText);
					m_pColumns_Item[i]->Show();
					if(m_Columns[i].pszTextID != NULL)
						m_pColumns_Item[i]->SetPropValueT("textID", m_Columns[i].pszTextID);
					else
						m_pColumns_Item[i]->SetPropValueT("text", m_Columns[i].pszText);
					m_pColumns_Item[i]->Update();
			}
			if(nRemainX>0)
			{
				nRemainX--;
				wLeftStart++;	
			}
		}
	}
	else
	{
		wLeftStart +=m_InterSection.wWidth;	
		for(int i = 0; i < m_wColCount; i++)
		{
			
			if(m_pColumns_Item[i]!=NULL)
			{
				m_pColumns_Item[i]->SetPropValueT("type", 2);
				m_pColumns_Item[i]->SetPropValueT("static_wtype", 	0x22);
				m_pColumns_Item[i]->SetPropValueT("fgc", 	m_Columns[i].fgc);
				m_pColumns_Item[i]->SetPropValueT("bgc", 	m_Columns[i].bgc);
				m_pColumns_Item[i]->SetPropValueT("align", 	m_wAlign);
				
				m_wRowWidth	=	m_Columns[i].wWidth;
				rect.left	=	wLeftStart;
				rect.right	= 	rect.left + m_wRowWidth;
				rect.top	=	0;
				rect.bottom	=	m_wColHeight;
				wLeftStart +=	m_wRowWidth;
				printf("ShowColumns:wWidth=%d,rect.left:%d,rect.right=%d\n",m_Columns[i].wWidth,rect.left,rect.right);
				m_pColumns_Item[i]->SetPropValueT("left", 	rect.left);
				m_pColumns_Item[i]->SetPropValueT("right", 	rect.right);
				m_pColumns_Item[i]->SetPropValueT("top", 	rect.top);
				m_pColumns_Item[i]->SetPropValueT("bottom", rect.bottom);
				m_pColumns_Item[i]->SetPropValueT("textwidth", m_wRowWidth-3);
				m_pColumns_Item[i]->SetPropValueT("itemheight", m_wColHeight-3);
				
				m_pColumns_Item[i]->CreateA();
				//printf(" %d:m_Columns[i].pszTextID=%s,m_Columns[i].pszText=%s\n",i,m_Columns[i].pszTextID,m_Columns[i].pszText);
					m_pColumns_Item[i]->Show();
					if(m_Columns[i].pszTextID != NULL)
						m_pColumns_Item[i]->SetPropValueT("textID", m_Columns[i].pszTextID);
					else
						m_pColumns_Item[i]->SetPropValueT("text", m_Columns[i].pszText);
					m_pColumns_Item[i]->Update();
			}
		}		
	}
	
}
void		CtmTableView::ShowRows()
{
	WORD wTopStart=0;
	RECT	rect;
	int nRemainY=0;
	nRemainY=(m_rectWindow.bottom-m_rectWindow.top)%m_wRealRowCount;
	for(int i = 0; i < m_wRowCount; i++)
	{
		if(m_pRows_Item[i]!=NULL)
		{
				m_pRows_Item[i]->SetPropValueT("type", 2);
				m_pRows_Item[i]->SetPropValueT("static_wtype", 	0x22);
				m_pRows_Item[i]->SetPropValueT("fgc", 	m_Rows[i].fgc);
				m_pRows_Item[i]->SetPropValueT("bgc", 	m_Rows[i].bgc);
				m_pRows_Item[i]->SetPropValueT("align", 	m_wAlign);
				
				wTopStart +=m_wColHeight;
				rect.left	=	0;
				rect.right	= 	m_InterSection.wWidth;
				rect.top	=	wTopStart;
				rect.bottom	=	rect.top+m_wColHeight;
				
				
				m_pRows_Item[i]->SetPropValueT("left", 	rect.left);
				m_pRows_Item[i]->SetPropValueT("right", 	rect.right);
				m_pRows_Item[i]->SetPropValueT("top", 	rect.top);
				m_pRows_Item[i]->SetPropValueT("bottom", rect.bottom);
				m_pRows_Item[i]->SetPropValueT("textwidth", m_InterSection.wWidth-3);
				m_pRows_Item[i]->SetPropValueT("itemheight", m_wColHeight-3);
				
				m_pRows_Item[i]->CreateA();
				m_pRows_Item[i]->Show();
					if(m_Rows[i].pszTextID != NULL)
						m_pRows_Item[i]->SetPropValueT("textID", m_Rows[i].pszTextID);
					else
						m_pRows_Item[i]->SetPropValueT("text", m_Rows[i].pszText);
					m_pRows_Item[i]->Update();
				}
				if(nRemainY>0)
				{
					nRemainY--;
					wTopStart++;	
				}
	}
}

void		CtmTableView::ShowInterSection()
{
	RECT	rect;
	if(m_pInterSection_Item!=NULL)
	{
		
		m_pInterSection_Item->SetPropValueT("type", 2);
		m_pInterSection_Item->SetPropValueT("static_wtype", 	0x22);
		m_pInterSection_Item->SetPropValueT("fgc", 	m_InterSection.fgc);
		m_pInterSection_Item->SetPropValueT("bgc", 	m_InterSection.bgc);
		m_pInterSection_Item->SetPropValueT("align", 	m_wAlign);
		rect.left	=	0;
		rect.top	=	0;
		if(!m_bTableType)
		{
			rect.right	= 	m_wRowWidth;
		}
		else
		{
			rect.right	=m_wRowWidth=m_InterSection.wWidth;
		}
		rect.bottom	=	m_wColHeight;
		m_pInterSection_Item->SetPropValueT("left", 	rect.left);
		m_pInterSection_Item->SetPropValueT("right", 	rect.right);
		m_pInterSection_Item->SetPropValueT("top", 	rect.top);
		m_pInterSection_Item->SetPropValueT("bottom", rect.bottom);
		m_pInterSection_Item->SetPropValueT("textwidth", m_wRowWidth-3);
		m_pInterSection_Item->SetPropValueT("itemheight", m_wColHeight-3);
		
		m_pInterSection_Item->CreateA();
		m_pInterSection_Item->Show();
			if(m_InterSection.pszTextID != NULL)
				m_pInterSection_Item->SetPropValueT("textID", m_InterSection.pszTextID);
			else
				m_pInterSection_Item->SetPropValueT("text", m_InterSection.pszText);
			m_pInterSection_Item->Update();
	}
}


void		CtmTableView::ShowItems()
{
	RECT	rect;
	int nRemainY=0;
	WORD wTopStart=0;
	nRemainY=(m_rectWindow.bottom-m_rectWindow.top)%m_wRealRowCount;
	for(int i=0; i< m_wRowCount; i++)
	{
		printf("i=%d,m_wRowCount=%d\n",i,m_wRowCount);
		wTopStart+=m_wColHeight;
		int nRemainX=0;
		WORD wLeftStart =0;
		wLeftStart+=m_InterSection.wWidth;
		nRemainX=(m_rectWindow.right  - m_rectWindow.left)%m_wRealColCount;
		if(!m_bTableType)
		{
			for(int j=0; j<m_wColCount; j++)
			{
				if(m_pContext_Item[i][j]!=NULL)
				{
					m_pContext_Item[i][j]->SetPropValueT("enabled",m_Context[j].bEnable);
					m_pContext_Item[i][j]->SetPropValueT("wtype", m_Context[j].wType);
					m_pContext_Item[i][j]->SetPropValueT("maxchar",m_Context[j].wMaxchar);
					m_pContext_Item[i][j]->SetPropValueT("precision", 	m_Context[j].wPrecision);
					m_pContext_Item[i][j]->SetPropValueT("align", 	m_Context[j].wAlign);
					//printf("bgc=%d,fgc=%d,enabled=%d\n",m_Context[j].bgc,m_Context[j].fgc,m_Context[j].bEnable);
					//printf("i=%d,j=%d,m_Context_lValue[i][j]=%ld\n",i,j,m_Context_lValue[i][j]);
					m_pContext_Item[i][j]->SetPropValueT("value",m_Context_lValue[i][j]);	
					rect.left	=	wLeftStart;
					rect.right	= 	rect.left + m_wRowWidth;
					rect.top	=	wTopStart;
					rect.bottom	=	rect.top+m_wColHeight;
					wLeftStart +=m_wRowWidth;
					m_pContext_Item[i][j]->SetPropValueT("left", 	rect.left);
					m_pContext_Item[i][j]->SetPropValueT("right", 	rect.right-1);
					m_pContext_Item[i][j]->SetPropValueT("top", 	rect.top);
					m_pContext_Item[i][j]->SetPropValueT("bottom", rect.bottom-1);
					m_pContext_Item[i][j]->CreateA();
					m_pContext_Item[i][j]->SetPropValueT("fgc", 	m_Context[j].fgc);
					m_pContext_Item[i][j]->SetPropValueT("bgc", 	m_Context[j].bgc);
					m_pContext_Item[i][j]->Show();
					m_pContext_Item[i][j]->UpdateAll();
				}
				if(nRemainX>0)
				{
					nRemainX--;
					wLeftStart++;	
				}
			}	
		}
		else
		{
			
			for(int j=0; j<m_wColCount; j++)
			{
				printf("j=%d,m_wColCount=%d\n",j,m_wColCount);
				if(m_pContext_Item[i][j]!=NULL)
				{
					m_pContext_Item[i][j]->SetPropValueT("enabled",m_Context[j].bEnable);
					m_pContext_Item[i][j]->SetPropValueT("wtype", m_Context[j].wType);
					m_pContext_Item[i][j]->SetPropValueT("maxchar",m_Context[j].wMaxchar);
					m_pContext_Item[i][j]->SetPropValueT("precision", 	m_Context[j].wPrecision);
					m_pContext_Item[i][j]->SetPropValueT("align", 	m_Context[j].wAlign);
					//printf("bgc=%d,fgc=%d,enabled=%d\n",m_Context[j].bgc,m_Context[j].fgc,m_Context[j].bEnable);
					//printf("i=%d,j=%d,m_Context_lValue[i][j]=%ld\n",i,j,m_Context_lValue[i][j]);
					m_pContext_Item[i][j]->SetPropValueT("value",m_Context_lValue[i][j]);	
					m_wRowWidth	=   m_Columns[j].wWidth;
					rect.left	=	wLeftStart;
					rect.right	= 	rect.left + m_wRowWidth;
					rect.top	=	wTopStart;
					rect.bottom	=	rect.top+m_wColHeight;
					wLeftStart +=	m_wRowWidth;
					printf("j=%d,width=%d,left=%d,right=%d, top=%d,bottom=%d\n",j, m_Columns[i].wWidth,rect.left,rect.right,rect.top,rect.bottom);
					m_pContext_Item[i][j]->SetPropValueT("left", 	rect.left);
					m_pContext_Item[i][j]->SetPropValueT("right", 	rect.right-1);
					m_pContext_Item[i][j]->SetPropValueT("top", 	rect.top);
					m_pContext_Item[i][j]->SetPropValueT("bottom", rect.bottom-1);
					m_pContext_Item[i][j]->CreateA();
					m_pContext_Item[i][j]->SetPropValueT("fgc", 	m_Context[j].fgc);
					m_pContext_Item[i][j]->SetPropValueT("bgc", 	m_Context[j].bgc);
					m_pContext_Item[i][j]->Show();
					m_pContext_Item[i][j]->UpdateAll();
				}
			}
		}
		if(nRemainY>0)
		{
			nRemainY--;
			wTopStart++;	
		}
	}
}

void		CtmTableView::SetItemData(int nRows, int nCols,long lValue,WORD wPrecision)
{
	if(nRows<0||nRows>(m_wRowCount-1))	return ;
	if(nCols<0||nCols>(m_wColCount-1))	return ;
	if(m_pContext_Item[nRows][nCols]!=NULL)
	{
		//printf("lValue=%d\n",lValue);
		m_Context_lValue[nRows][nCols]=lValue;
		m_pContext_Item[nRows][nCols]->SetPropValueT("value",(double)lValue);
		//m_pContext_Item[nRows][nCols]->Update();
	}
}

CtmWnd*		CtmTableView::GetItem(int nRows, int nCols)
{
	if(nRows<0||nRows>(m_wRowCount-1))	return NULL;
	if(nCols<0||nCols>(m_wColCount-1))	return NULL;
	return 	m_pContext_Item[nRows][nCols];
}

long		CtmTableView::GetItemData(int nRows, int nCols)
{
	if(nRows<0||nRows>(m_wRowCount-1))	return -1;
	if(nCols<0||nCols>(m_wColCount-1))	return -1;
	long lValue=-1;
	if(m_pContext_Item[nRows][nCols]!=NULL)
	{
		m_pContext_Item[nRows][nCols]->GetPropValueT("value",&lValue,sizeof(lValue));
		m_Context_lValue[nRows][nCols]=lValue;
	}
	return lValue;
}

void		CtmTableView::CreateColumns()
{
	for(int i = 0; i < m_wColCount; i++)
	{
		if(m_pColumns_Item[i] == NULL)
		{
			m_pColumns_Item[i] = new CtmItem();
			m_pColumns_Item[i]->SetPropertys();
			m_pColumns_Item[i]->SetParent(this);
		}
	}
}

void		CtmTableView::CreateRows()
{
	for(int i = 0; i < m_wRowCount; i++)
	{
		if(m_pRows_Item[i] == NULL)
		{
			m_pRows_Item[i] = new CtmItem();
			m_pRows_Item[i]->SetPropertys();
			m_pRows_Item[i]->SetParent(this);
		}
	}
}

void		CtmTableView::CreateInterSection()
{
	RECT	rect;
	if(m_pInterSection_Item==NULL)
	{
		m_pInterSection_Item= new CtmItem();
		m_pInterSection_Item->SetPropertys();
		m_pInterSection_Item->SetParent(this);	
	}
}

void		CtmTableView::CreateItems()
{
	RECT	rect;
	for(int i=0; i< m_wRowCount; i++)
	{
		
		for(int j=0; j<m_wColCount; j++)
		{
			//printf("Precision=%d\n",m_Context[j].wPrecision);
			if(m_pContext_Item[i][j]==NULL)
			{
				m_pContext_Item[i][j]=new CtmEditX1();
				m_pContext_Item[i][j]->SetPropertys();
				m_pContext_Item[i][j]->SetParent(this);	
			}
		}
		
	}
}


void		CtmTableView::ShowScrollbar()
{
	if (m_bBar)
	{
		SetScrollbar(m_wRealRowCount);
		UpdateScrollbar();
	}
}
				
void		CtmTableView::UpdateScrollbar()
{
	if (m_bBar)
		m_scrollbar.Update();
}
					
void		CtmTableView::SetInitScrollbar()
{
	m_nRight  =m_wnd.rect.right  - m_wnd.rect.left;
	 if(m_bTableType)
	{
		m_nRight=m_InterSection.wWidth;
		for(int i=0;i<m_wColCount;++i)
			m_nRight+=m_Columns[i].wWidth;	
	}
	m_scrollbar.SetPropertys();
	m_scrollbar.SetParent(this);
	m_scrollbar.SetPropValueT("IncreType", 2);	
	m_scrollbar.SetPropValueT("left", 	m_nRight);
	m_scrollbar.SetPropValueT("top", 	double(0));
	m_scrollbar.SetPropValueT("right", 	m_nRight + 20);
	m_scrollbar.SetPropValueT("bottom",	m_wnd.rect.bottom - m_wnd.rect.top );
	m_scrollbar.SetPropValueT("fgc",	m_fgc);
	m_scrollbar.SetPropValueT("bgc",	42260);
	m_scrollbar.CreateA();	
}
					
void		CtmTableView::SetScrollbar(WORD wRange)
{
	int nRange = wRange-1;
	if (nRange < 0) nRange = 0;
	m_scrollbar.SetMaxReg(nRange);
		m_scrollbar.SetPos(m_nRowIndex);
}

WORD		CtmTableView::OnKey(WORD wKey)
{
	WORD	wResult 	= wKey;
	printf("CtmTableView::wKey=%x\n",wKey);
	switch (wKey)
	{
		case _KEY_DOWNFIELD:
			{
				if(m_nRowIndex<m_wRowCount)
				{
					m_nRowIndex++;
				}
				else
				{
					m_nRowIndex=m_wRowCount;	
				}
				ShowScrollbar();
				wResult	=_NULL_KEY;
			}
				break;
			case _KEY_UPFIELD:
			{
				if(m_nRowIndex>0)
				{
					m_nRowIndex--;
				}
				else
				{
					m_nRowIndex=0;	
				}
				ShowScrollbar();
				wResult	=_NULL_KEY;
			}
				break;
		case _ENTER:
			wResult=_ENTER;
			break;
		default:
				break;
	}
	return wResult;
}

void		CtmTableView::DeleteRowColCount(int nRows,int nCols)
{
	if(nCols>MAX_COLUMNS_COUNT)		return ;
	if(m_wColCount-nCols<0)			m_wColCount=0;
	else							m_wColCount	-=nCols;
	if(nRows>MAX_ROWS_COUNT)		return ;
	if(m_wRowCount-nRows<0)			m_wRowCount=0;
	else							m_wRowCount-=nRows;
	for(int i=m_wRowCount;i<m_wRowCount+nRows;i++)
	{
		if(m_pRows_Item[i]!=NULL)
		{
			delete m_pRows_Item[i];
			m_pRows_Item[i]=NULL;	
		}
		for(int j=m_wColCount;j<m_wColCount+nCols;j++)
		{
			
			if(m_pColumns_Item[j]!=NULL)
			{
				delete m_pColumns_Item[j];
				m_pColumns_Item[j]=NULL;	
			}	
			if(m_pContext_Item[i][j]!=NULL)
			{
				delete m_pContext_Item[i][j];
				m_pContext_Item[i][j]=NULL;	
			}
		}	
	}
	for(int i=0;i<m_wRowCount;i++)
	{
		for(int j=m_wColCount;j<m_wColCount+nCols;j++)
		{
			if(m_pContext_Item[i][j]!=NULL)
			{
				delete m_pContext_Item[i][j];
				m_pContext_Item[i][j]=NULL;	
			}	
		}
	}
	for(int j=0; j<m_wColCount; j++)
	{
		for(int i=m_wRowCount; i<m_wRowCount+nRows; i++)
		{
			if(m_pContext_Item[i][j]!=NULL)
			{
				delete m_pContext_Item[i][j];
				m_pContext_Item[i][j]=NULL;	
			}		
		}	
	}
	Update();
}

void		CtmTableView::AddRowColCount(int nRows, int nCols)
{
	if(nCols<0)						return ;
	if(m_wColCount+nCols>MAX_COLUMNS_COUNT)			m_wColCount=MAX_COLUMNS_COUNT;
	else							m_wColCount	+=nCols;
	if(nRows<0)						return ;
	if(m_wRowCount+nRows>MAX_ROWS_COUNT)			m_wRowCount=MAX_ROWS_COUNT;
	else							m_wRowCount+=nRows;
	for(int i=m_wRowCount-nRows;i<m_wRowCount;i++)
	{
		if(m_pRows_Item[i]==NULL)
		{
			m_pRows_Item[i]=new CtmItem();
			m_pRows_Item[i]->SetPropertys();
			m_pRows_Item[i]->SetParent(this);	
		}
		for(int j=m_wColCount-nCols;j<m_wColCount;j++)
		{	
			if(m_pColumns_Item[j]==NULL)
			{
				m_pColumns_Item[j]=new CtmItem();
				m_pColumns_Item[j]->SetPropertys();
				m_pColumns_Item[j]->SetParent(this);
			}	
			if(m_pContext_Item[i][j]==NULL)
			{
				m_pContext_Item[i][j]=new CtmEditX1();
				m_pContext_Item[i][j]->SetPropertys();
				m_pContext_Item[i][j]->SetParent(this);	
				printf("m_pContext_Item[%d][%d]=%x\n",i,j,m_pContext_Item[i][j]);
			}
		}	
	}
	for(int i=0;i<m_wRowCount;i++)
	{
		for(int j=m_wColCount-nCols;j<m_wColCount;j++)
		{
			if(m_pContext_Item[i][j]==NULL)
			{
				m_pContext_Item[i][j]=new CtmEditX1();
				m_pContext_Item[i][j]->SetPropertys();
				m_pContext_Item[i][j]->SetParent(this);	
				printf("m_pContext_Item[%d][%d]=%x\n",i,j,m_pContext_Item[i][j]);
			}
		}
	}
	for(int j=0; j<m_wColCount; j++)
	{
		for(int i=m_wRowCount-nRows; i<m_wRowCount; i++)
		{
			if(m_pContext_Item[i][j]==NULL)
			{
				m_pContext_Item[i][j]=new CtmEditX1();
				m_pContext_Item[i][j]->SetPropertys();
				m_pContext_Item[i][j]->SetParent(this);	
				printf("m_pContext_Item[%d][%d]=%x\n",i,j,m_pContext_Item[i][j]);
			}	
		}	
	}
	Update();	
}

CtmWnd*		CtmTableView::GetColWnd(int nCols)
{
	if(nCols<0||nCols>m_wColCount-1)	return NULL;
	return m_pColumns_Item[nCols];
}
CtmWnd* 	CtmTableView::GetRowWnd(int nRows)
{
	if(nRows<0||nRows>m_wRowCount-1)	return NULL;
	return m_pRows_Item[nRows];
}
