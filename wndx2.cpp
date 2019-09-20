/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager                                      |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Andrew                                                         |
|  Version  : V1.00                                                          |
|  Creation : 07/08/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include    "wndx2.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - ImageBackground                           |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
ImageBackground::ImageBackground()
{
	m_lImageSize = 0;
	m_pImage = NULL;
}

ImageBackground::~ImageBackground()
{
	if (m_pImage != NULL) free(m_pImage);
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void	ImageBackground::CopyImage(int left,int top,int right,int bottom)
{
	if (m_pImage != NULL) free(m_pImage);
	m_lImageSize = _ImageSize(left, top, right, bottom);
	m_pImage = (char*)malloc(m_lImageSize);
	_SetViewPort(left, top, right, bottom);
	_GetImage(0, 0, right-left, bottom-top, m_pImage);
}

void	ImageBackground::ShowImage(int left,int top,int right,int bottom)
{
	if (m_pImage != NULL)
	{
		_SetViewPort(left, top, right, bottom);
		_PutImage(0, 0, m_pImage, _GPSET);
	}
//	//printf("ImageBackground::ShowImage\n");
}

void	ImageBackground::FreeImage()
{
	if (m_pImage != NULL)
	{
		free(m_pImage);
		m_pImage = NULL;
	}
}
