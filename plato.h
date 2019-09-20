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
#ifndef	D_PLATO
#define	D_PLATO

#include	"wnd.h"
#include	"wndx2.h"

/*===========================================================================+
|           Constants	                                                     |
+===========================================================================*/
#define 		tmMAX_ITEM		10

/*===========================================================================+
|          Type definition                                                   |
+===========================================================================*/
typedef struct tagtmITEMS
{
	char*			 item_caption;
	short			 item_id;
	int				 item_value;
}tmPLATOITEM;

typedef struct tagtmPLATO
{
	int              bg_col;
	int				 axis_col;
	int				 item_col;
	int				 item_sum_col;
	int				 curve_col;
	int				 block_line_col;
	short 			 item_count;
	short			 item_show;
	char*			 plato_caption;
	char*			 tmItem;
	tmPLATOITEM*	 tmItem_Show; 
}tmPLATO;

typedef struct tagtmPOINTPLATO
{
	double x;
	double y;
}POINTPLATO;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - CtmPlato                                     |
+===========================================================================*/
class	CtmPlato	:	public CtmWnd
{
/*----------------------------------------------------------------------------+
|           Constructor and destructor                                        |
+----------------------------------------------------------------------------*/
public:
	CtmPlato(tmWND* pwnd, tmPLATO* pPlato);
	~CtmPlato();
/*------------------------------------------------------------------------------+
|           Attributes                                                          |
+------------------------------------------------------------------------------*/
private:
	tmPLATO*		m_pPlato;
	tmPLATOITEM*    m_Plato_item;
	int				m_max_item_value;			//��ܶ��ؤ����̤j��
	short			m_Xunit_min_value;			//X�b�̤p���
	double			m_Yunit_min_value;			//Y�b�̤p���
	double			m_Zunit_min_value;			//Z�b�̤p���
	int 			m_plato_height;				//���PLATO������
	int				m_plato_width;				//���PLATO���e��
	int				m_items_sum;				//�Ҧ����حȤ��M
	int 			m_AxisYZMAX;
	POINTPLATO		m_axis_o;
	POINTPLATO   	axis_x1;
	POINTPLATO	    axis_y1;
	POINTPLATO		axis_z1;
	
	ImageBackground m_Image;
/*-------------------------------------------------------------------------------+
|        Operations                                                              |
+-------------------------------------------------------------------------------*/
public:
	void	Show();
	void	Redraw();
	void	Set();
	void	initial();
	void	Update();
private:
	void	ShowCutline();
};

#endif
