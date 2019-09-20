/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeff                                                           |
|  Version  : V1.00                                                          |
|  Creation : 18/03/2005                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"plato.h"

/*===========================================================================+
|           Constants	                                                     |
+===========================================================================*/
#define		W_ITEM_DEFAULT		35
#define		W_MIN_ITEM_DEFAULT	5
#define		W_ITEMS_GAP			5
#define		H_PLATO_CAPTION		15
  

tmPLATOITEM ITEM_OTHER ={"OTHER", 0, 0};

char* CaptionItem[10] = 
{
	"E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9"
};
/*===========================================================================+
|           External		                                                 |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - CtmPlato	                                 |
+===========================================================================*/

/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
|----------------------------------------------------------------------------|
|Parameter: pwnd													 		 |
|            							                              		 |
+---------------------------------------------------------------------------*/
CtmPlato::CtmPlato(tmWND* pwnd, tmPLATO* pPlato)	:  CtmWnd()
{
	CtmWnd::Create(pwnd);
	m_pPlato = pPlato;
	
	m_Image.CopyImage(m_rectClient.left, m_rectClient.top, \
	m_rectClient.right, m_rectClient.bottom);

	initial();
}

CtmPlato::~CtmPlato()
{

}

/*----------------------------------------------------------------------------+
|			Operations										 				  |
+----------------------------------------------------------------------------*/
void	CtmPlato::initial()
{
	short  ItemCount = 0;
	
//	m_pPlato->item_count	= m_nItemToltalCount;	
//	m_pPlato->item_show		= m_nItemShowCount;

	//========
	//��l��
	//========	
	//1.�δ��J�ƧǪk,��X�֦Z�����ضi��Ƨ�
		for (int i = 1; i < m_pPlato->item_count ; i++)
		{
			tmPLATOITEM temp  = m_pPlato->tmItem_Show[i];
			int index = i - 1;
			while ((index >= 0) && (temp.item_value > m_pPlato->tmItem_Show[index].item_value))
			{
				m_pPlato->tmItem_Show[index + 1] = m_pPlato->tmItem_Show[index];
				index--;
			}
			m_pPlato->tmItem_Show[index + 1] = temp;
		}
		
	//2.�έp129�ӿ��~�������A�X�{���~������������
	for (int i = 0; i < m_pPlato->item_count; i++)
		if (m_pPlato->tmItem_Show[i].item_value != 0)
			ItemCount++;
//	//printf("ItemCount=%d\n", ItemCount);
	
	if (ItemCount < m_pPlato->item_show)
	{
		m_pPlato->item_show 	= ItemCount;
		m_pPlato->item_count 	= ItemCount;
	}
	else
	{
		m_pPlato->item_count = ItemCount;
	}
	
	//3.�]�w�e10�������D�r�Ŧ�
	for (int i = 0; i < 10 ; i++)
	{
		m_pPlato->tmItem_Show[i].item_caption = CaptionItem[i];
	}
	
	if (m_pPlato->item_count > 10)
		m_pPlato->tmItem_Show[10].item_caption = "Other";
		
	//4.�]�w���PLATO���I��color
	if (m_pPlato->bg_col != 0 )
	{
		_SetColor((m_pPlato->bg_col));
		_Rectangle(_GFILLINTERIOR,1, 1, m_rectClient.right - 3, \
			m_rectClient.bottom - 3);
	}

	_SetViewPort(m_rectClient.left, m_rectClient.top, \
		m_rectClient.right, m_rectClient.bottom);

	//5.�p����PLATO�����שM�e��
	m_plato_height = m_rectClient.bottom - m_rectClient.top-150;
	m_plato_width  = m_rectClient.right  - m_rectClient.left;

	//6.�p����ܶ��ؤ����̤j�ȤΩҦ����حȤ��M
	m_items_sum = 0;
	for (int i = 0; i < m_pPlato->item_count; i++)
	{
		if (m_pPlato->tmItem_Show[i].item_value > m_max_item_value)
		{
			m_max_item_value = m_pPlato->tmItem_Show[i].item_value;
		}
		m_items_sum += m_pPlato->tmItem_Show[i].item_value;
	}
	
	if (m_pPlato->item_count > 10)	
	for (int j = m_pPlato->item_show+1; j < m_pPlato->item_count; j++ )
	{
		m_pPlato->tmItem_Show[m_pPlato->item_show ].item_value += m_pPlato->tmItem_Show[j].item_value;
	}

	//7.�p��C�ӧ��жb���̤p����

	//7.1X�b���̤p����
	if ((m_pPlato->item_show * W_ITEM_DEFAULT + \
		m_pPlato->item_show * W_ITEMS_GAP) > (m_plato_width * 7/10)) 
		m_Xunit_min_value = ((m_plato_width * 7 / 10) / m_pPlato->item_count) - \
		W_ITEMS_GAP;
	else
		m_Xunit_min_value = W_ITEM_DEFAULT;
	
//	//printf("m_items_sum = %d\n", m_items_sum);
	m_AxisYZMAX = m_items_sum;
/*	
	if (m_AxisYZMAX % 5 != 0)
		for (int i = 0; i < 5; i ++)
		{	
			m_AxisYZMAX ++;
			if (m_AxisYZMAX % 5 == 0)	
				break;
		}
*/	
	//7.2Y�b���̤p����
	m_Yunit_min_value = double((m_plato_height * 5 / 7) - 8) / double(m_AxisYZMAX);
	//7.3Z�b���̤p����
	m_Zunit_min_value = float(m_plato_height * 5 / 7 - 8) / float(100);
	
	Set();

}
void	CtmPlato::Set()
{
	//========
	//�]�wX,Y,Z�b���_�I�M���I,�H�έ��I������
	//========
	double	border_x = m_plato_height - (m_plato_height / 7);
	double	border_y = m_plato_width  / 11;
	double	border_z = m_plato_width  - 35;

	axis_x1.x = border_y;
	axis_x1.y = m_plato_height / 7;
	axis_y1.x = border_z;
	axis_y1.y = border_x;
	axis_z1.x = border_z;
	axis_z1.y = m_plato_height / 7;
	m_axis_o.x  = border_y;
	m_axis_o.y  = border_x;

	//========  
	//��ܾ�ӧ��Шt
	//========	
	_SetViewPort(m_rectClient.left, m_rectClient.top, \
		m_rectClient.right, m_rectClient.bottom);
	for (short i = 0; i <= 2; i++)
	{
		_SetColor((m_pPlato->axis_col));
		_MoveTo((int)axis_x1.x + i, (int)axis_x1.y - i);
		_LineTo((int)m_axis_o.x  + i, (int)m_axis_o.y);

		_MoveTo((int)axis_y1.x - i, (int)axis_y1.y + i);
		_LineTo((int)m_axis_o.x  + i, (int)m_axis_o.y  + i);

		_MoveTo((int)axis_z1.x - i, (int)axis_z1.y + i);
		_LineTo((int)axis_y1.x - i, (int)axis_y1.y + i);
		
		if (i > 1)
		{
			_SetColor(BLACK);
			_MoveTo((int)axis_x1.x + 3, (int)axis_x1.y + 8);
			_LineTo((int)axis_z1.x - 3, (int)axis_z1.y + 8);
		}
		
	}

	//========
	//1.���PLATO���D�ΦU���жb���D
	//2.�e�I�������line---X��V
	//========
	CtmWnd::ShowExtent((m_plato_width / 2) - strlen(m_pPlato->plato_caption) * 4, \
		H_PLATO_CAPTION, m_pPlato->plato_caption );
	char sz[5];
		
	for (int i = 0; i < 6; i++)
	{
		int axisy_value = 0, axisz_value = 0;
		
		
		axisy_value = m_AxisYZMAX * (5 - i) / 5;
		axisz_value	= 100 * (5 - i) / 5;

		_SetViewPort(m_rectClient.left, m_rectClient.top, \
			m_rectClient.right, m_rectClient.bottom);

		//Y�b����
		if(i == 0 || i == 5)
		{
		sprintf(sz,"%d",axisy_value);
		int axisy_value_caption = (int)axis_x1.x - strlen(sz) * 8 - 2;
		CtmWnd::ShowExtent(int(axisy_value_caption), (int)(axis_z1.y + \
			i * 20 * m_Zunit_min_value), sz);
		}

		//Z�b����
		sprintf(sz,"%d%%",axisz_value);
		CtmWnd::ShowExtent(int(axis_z1.x) + 3, (int)(axis_z1.y + \
			i * 20 * m_Zunit_min_value), sz);
			
			
		_SetViewPort(m_rectClient.left, m_rectClient.top, \
			m_rectClient.right, m_rectClient.bottom);
		
		//�e�I�������line---X��V
        if (i != 0 && i != 5)
        {
			_SetColor((m_pPlato->block_line_col));
			_MoveTo((int)axis_x1.x+3, (int)(m_axis_o.y - i * (m_AxisYZMAX/5) * m_Yunit_min_value));
			_LineTo((int)axis_z1.x-3, (int)(m_axis_o.y - i * (m_AxisYZMAX/5) * m_Yunit_min_value));
        }
	}

	//�e�I�������line---Y��V
	for (int i = 1; i <= (border_z - border_y) / (m_Xunit_min_value + W_ITEMS_GAP); i++)
	{
		_SetColor((m_pPlato->block_line_col));
		_MoveTo((int)((m_Xunit_min_value + W_ITEMS_GAP) * i) + (int)border_y - 2 + \
			W_ITEMS_GAP, (int)m_axis_o.y-1);
		_LineTo((m_Xunit_min_value + W_ITEMS_GAP) * i + (int)border_y - 2 + \
			W_ITEMS_GAP, m_plato_height / 7 + 9);
	}
	ShowCutline();
}
void	CtmPlato::ShowCutline()
{	
	tm_xycoord xy;
	xy.xcoord = 19;
	xy.ycoord = 332;
	DrawSplit(xy);
	char sz[5];
	_SetViewPort(m_rectClient.left, m_rectClient.top, \
		m_rectClient.right, m_rectClient.bottom);
	CtmWnd::ShowExtent(0, 340, "˵ ��:");
	for (int i = 0; i < 5; i++)
	{
		CtmWnd::ShowExtent(60, 360 + i * 20, m_pPlato->tmItem_Show[i].item_caption);
		CtmWnd::ShowExtent(76, 360 + i * 20, ":");
		if ((m_pPlato->item_show != 0) && (m_pPlato->item_show > i))
		{
			CtmWnd::ShowExtent(84, 360 + i * 20, g_MultiLanguage[m_pPlato->tmItem_Show[i].item_id]);
			CtmWnd::ShowExtent(84+160, 360 + i * 20, "������:");
			sprintf(sz,"%d",m_pPlato->tmItem_Show[i].item_value);
			CtmWnd::ShowExtent(84+216, 360 + i * 20, sz);
				
		}
	}
	
	for (int i = 5; i < 10; i++)
	{
		CtmWnd::ShowExtent(350+60, 360 + (i-5) * 20, m_pPlato->tmItem_Show[i].item_caption);
		CtmWnd::ShowExtent(350+76, 360 + (i-5) * 20, ":");
		if ((m_pPlato->item_show != 0) && (m_pPlato->item_show > i))
		{
			CtmWnd::ShowExtent(350+84, 360 + (i-5) * 20, g_MultiLanguage[m_pPlato->tmItem_Show[i].item_id]);
			CtmWnd::ShowExtent(84+160+350, 360 + (i-5) * 20, "������:");
			sprintf(sz,"%d",m_pPlato->tmItem_Show[i].item_value);
			CtmWnd::ShowExtent(84+216+350, 360 + (i-5) * 20, sz);
		}
	}
	
	 
}
void	CtmPlato::Show() 
{
	char sz[5];
	//========
	//1.�p��C�@�өҭn��ܤ����ت��x�έ��n
	//2.�ھڥΤ���w����ܶ��ت��ƥ���ܡA��l�����جۥ[��@�ݢ�֢Ӣඵ���
	//3.�e�I�������line---Y��V
	//========
	m_axis_o.x += 3.0;			//�ѩ�X,Y,Z�b���[��
	m_axis_o.y -= 1.0;			//�G���Э��I����

	POINTPLATO begin, end;
	begin.x = m_axis_o.x;
	begin.y	= m_axis_o.y;
	end.x   = m_axis_o.x;
	end.y	= m_axis_o.y;

	short ShowItemCount;
	if (m_pPlato->item_show != 0)
	{
		if (m_pPlato->item_count > 10)
			ShowItemCount = m_pPlato->item_show + 1;
		else
			ShowItemCount = m_pPlato->item_show;
			
		for (int i = 0; i < ShowItemCount; i++)
		{	
			end.y -= (m_pPlato->tmItem_Show[i].item_value * m_Yunit_min_value);
			_SetColor((m_pPlato->item_col));
			end.x += (m_Xunit_min_value + W_ITEMS_GAP);
			_SetViewPort(m_rectClient.left, m_rectClient.top, \
				m_rectClient.right, m_rectClient.bottom);
//			//printf("m_Yunit_min_value = %f\n", m_Yunit_min_value);
			//�e�Ҧ��ݭn��ܶ���
			_Rectangle(_GFILLINTERIOR,(int)m_axis_o.x, (int)(m_axis_o.y - \
				(m_pPlato->tmItem_Show[i].item_value) * m_Yunit_min_value), \
				(int)(m_axis_o.x + m_Xunit_min_value), (int)m_axis_o.y);
			
			//��ܶ��ت��y�z
			if ((m_Xunit_min_value + W_ITEMS_GAP) >= \
				(int)(strlen(m_pPlato->tmItem_Show[i].item_caption)) * 8)
			{
				CtmWnd::ShowExtent((int)m_axis_o.x, (int)m_axis_o.y + 6, \
					m_pPlato->tmItem_Show[i].item_caption);
			}
			
			sprintf(sz,"%d",m_pPlato->tmItem_Show[i].item_value);
			CtmWnd::ShowExtent((int)m_axis_o.x + 8, (int)(m_axis_o.y - \
				(m_pPlato->tmItem_Show[i].item_value) * m_Yunit_min_value - 18), sz);
			
			//�e�C�Ӷ��ت�����P����line
			_SetViewPort(m_rectClient.left, m_rectClient.top, \
				m_rectClient.right, m_rectClient.bottom);
			_SetColor(m_wndattr.fgcBorder3);
			_MoveTo((int)m_axis_o.x, (int)m_axis_o.y);
			_LineTo((int)m_axis_o.x, (int)(m_axis_o.y - (m_pPlato->tmItem_Show[i].item_value * \
				m_Yunit_min_value)));
			_LineTo((int)(m_axis_o.x + m_Xunit_min_value), (int)(m_axis_o.y - \
				(m_pPlato->tmItem_Show[i].item_value * m_Yunit_min_value)));
	
			_SetColor(m_wndattr.fgcBorder4);
			_LineTo((int)(m_axis_o.x + m_Xunit_min_value), (int)m_axis_o.y);
			_LineTo((int)m_axis_o.x + 1, (int)m_axis_o.y);
			m_axis_o.x += W_ITEMS_GAP + m_Xunit_min_value;
	
			//�e�Ҧ����ؤ��M����line
			_SetColor(m_pPlato->curve_col);
			
			if (i == ShowItemCount-1)
			{
				end.y = (int)(m_axis_o.y - m_Yunit_min_value * m_items_sum);
				_SetColor(BLUE);
				_MoveTo((int)axis_x1.x + 3, (int)end.y);
				_LineTo((int)axis_z1.x - 3, (int)end.y);
				_SetColor(m_pPlato->curve_col);
			}

			_MoveTo((int)begin.x, (int)begin.y);
			_LineTo((int)end.x, (int)end.y);
			
			begin.x = end.x;
			begin.y = end.y;
		}
	}
}

void	CtmPlato::Update()
{
	//CtmWnd::Clear();
	_SetViewPort(m_rectClient.left, m_rectClient.top, \
		m_rectClient.right, m_rectClient.bottom);
	m_Image.ShowImage(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, \
		m_rectWindow.bottom);
	initial();
	Show();
}
