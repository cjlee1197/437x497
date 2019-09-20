/*==========================================================================+
|  Class    : Class Icon	                                                |
|  Task     : Class Icon header file                              			|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2006/12/18                                                    |
|  Revision : 2006/12/18                                                    |
+==========================================================================*/
#ifndef D_DICON
#define	D_DICON

#include "common.h"
#include "CObject.h"

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagICON
{
	char	*pszName;
}	ICON;

/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - CIcon                                     	|
+==========================================================================*/
class CIcon : public CObject
{
DECLARE_DYNCREATE(CIcon)
public:
	CIcon();
	
	~CIcon();
public:
	int		Create(void *pData);
	int		Destroy();
	
	int		Draw(int x, int y);
	int     Draw(int x, int y, int nWidth, int nHeight);
	POINT	GetPoint();
	int		GetWidth()			{return m_nWidth;};
	int		GetHeight()			{return m_nHeight;};
private:
	char	*m_pszName;
	
	int		m_nType;				// 圖標類型
	int		m_nCount;				// 圖標數目
	
	int		m_nWidth;				// 寬度
	int		m_nHeight;				// 高度
	int		m_nColorCount;			// 顏色數目
	int		m_nBitCount;			// 點的顏色數目
	int		m_nLength;				// 長度
	int		m_nOffset;				// 偏移位置
	int		m_nPointX;				// 跟蹤點X
	int		m_nPointY;				// 跟蹤點Y
	
	int		m_nLineByte;			// 每行佔用字節數
	
	int		m_nQuadCount;			// 顏色表大小
	unsigned long	*m_pwQuad;				// 顏色表
	
	char	*m_pIconData;			// 資料
	char	*m_pXorData;			// 掩碼資料
	int		m_nXorLineByte;			// 掩碼每行佔用字節數
private:
	int     Draw1(int x, int y, int nWidth, int nHeight) {return 0;};
	int     Draw4(int x, int y, int nWidth, int nHeight);
	int		Draw1(int x, int y);
	int		Draw4(int x, int y);
};


#endif
