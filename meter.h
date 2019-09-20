/*==========================================================================+
|  Class    : Meter  Control						     					|
|  Task     : Meter  Control file          	            					|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                   |
|  Link     :G++(GCC) 3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : Caozz	                                                    	|
|  Version  : V1.00                                                         |
|  Creation : 2014/05/19                                                    |
|  Revision :                                                               |
+==========================================================================*/
#ifndef D_METER
#define	D_METER

#include	"wnd.h"
#include    "wndx2.h"
#include	"init.h"
#include	"gui_ex.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

#define		PI					3.1415926535898
#define		INTERVAL_MIN		2

#define		POINTERR_FORWARD		0
#define		POINTERR_BACKWARD		1

typedef struct tagtmPoint
{
	int ix;
	int iy;
}tmPoint;

/*==========================================================================+
|           Class declaration - CtmMeter                                	|
+==========================================================================*/
class CtmMeter : public CtmWnd
{
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmMeter)
public:
    ~CtmMeter();
    CtmMeter();
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
public:
	void    Show();
	void    Update();
	void	Clear();
	
	BOOL 	CreateA();
	void 	SetPropertys();
private:
	void	SetDoubleBuffer();
	CtmBitMap*	GetBmpImage(char* pszImagePath);
	
	void	InitBuff();
	void	DestroyBuffer();
	void	SetImageBuffer();
	void	GetDBInfo();
	void	CopyBackground(WORD left, WORD top, WORD right, WORD bottom);
	void	GetPointerORGCoordinate();
	
	void	SetDialPlateToDoubleBuffer();
	void	SetPointerToDoubleBuffer();
	
	float	GetPointerAngle();
	tmPoint	GetPointCoordinate(float fAngle, tmPoint point, tmPoint	pointORG, int nPointerStartY);
	
	void	InitPointerFlag();
	void	OptimizePoint();
	
	void	ShowBuffer();
/*--------------------------------------------------------------------------+
|           Attributes                                                      |
+--------------------------------------------------------------------------*/
private:
	WORD			m_wViewHeight;
	WORD			m_wViewWidth;
	WORD			m_wImageType;
	WORD			m_wDirection;
	WORD			m_wInterval;
	DWORD			m_dwKeyColor;
	char*			m_pszMeterPath;
	char*			m_pszPointerPath;
	
	WORD			m_wPointerWidth;
	WORD			m_wPointerHeight;
	tmPoint			m_PointerORG;
	
	tmPoint			m_pointMeterORG;
	float			m_fStartAngle;
	float			m_fEndAngle;
	float			m_fMaxAngle;
	char*			m_pszDBID;
	char*			m_pszMaxID;
	char*			m_pszMinID;
	
	float			m_fValue;
	float			m_fMin;
	float			m_fMax;
	
	float			m_fUnit;
	
	ImageBackground	m_BgImage;
	
	CtmGDIDoubleBuffer* m_pDoubleBuffer;
	
	BOOL			m_bCopy;
	
	CtmBitMap		m_bmpImage;
	
	CtmBitMap*		m_pbmpMeter;
	CtmBitMap*		m_pbmpPointer;
	
	#ifdef	D_32BIT
	DWORD**  		m_pDialPlateBuff;
	DWORD**  		m_pPointerBuff;
	DWORD**			m_pBackBuff;
	#else
	WORD**  		m_pDialPlateBuff;
	WORD**  		m_pPointerBuff;
	WORD**			m_pBackBuff;
	#endif
	tmPoint**		m_pPointerORGCoordinate;
	tmPoint**		m_pPointerCoordinate;
	
	BOOL**			m_pPointerCoordinateFlag;
		
	
};

#endif
