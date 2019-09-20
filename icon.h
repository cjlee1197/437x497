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
	
	int		m_nType;				// �ϼ�����
	int		m_nCount;				// �ϼмƥ�
	
	int		m_nWidth;				// �e��
	int		m_nHeight;				// ����
	int		m_nColorCount;			// �C��ƥ�
	int		m_nBitCount;			// �I���C��ƥ�
	int		m_nLength;				// ����
	int		m_nOffset;				// ������m
	int		m_nPointX;				// �����IX
	int		m_nPointY;				// �����IY
	
	int		m_nLineByte;			// �C����Φr�`��
	
	int		m_nQuadCount;			// �C���j�p
	unsigned long	*m_pwQuad;				// �C���
	
	char	*m_pIconData;			// ���
	char	*m_pXorData;			// ���X���
	int		m_nXorLineByte;			// ���X�C����Φr�`��
private:
	int     Draw1(int x, int y, int nWidth, int nHeight) {return 0;};
	int     Draw4(int x, int y, int nWidth, int nHeight);
	int		Draw1(int x, int y);
	int		Draw4(int x, int y);
};


#endif
