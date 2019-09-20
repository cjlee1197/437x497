/*==========================================================================+
|  Class    : Class TouchScreen                                             |
|  Task     : Class TouchScreen header file                          		|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2006/12/21                                                    |
|  Revision : 2006/12/21                                                    |
+==========================================================================*/
#ifndef D_DTOUCHSCREEN
#define	D_DTOUCHSCREEN

#include 	"rs232.h"
#include 	"device.h"
#include	"tmpackclass.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define	TOUCH_DATA_LEN		5
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagTOUCHDATA
{
    int     nDataType;      // The Received data type
    int     nDataPos;       // The input position of DataBuff
    unsigned char   bDataBuff[TOUCH_DATA_LEN];
}   TOUCHDATA;
/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/
/*==========================================================================+
|           Class declaration - CIcon                                     	|
+==========================================================================*/
class CTouchScreen : public CtmPackClass
{
	DECLARE_DYNCREATE(CTouchScreen)
	public:
		CTouchScreen();
		~CTouchScreen();
	public:
		virtual	void    SetAngle(int nAngle)				{};
		virtual	void    SetOffset(int nX, int nY)			{};
		virtual	void    SetScale(int nCoor, double dScale)	{};
		virtual int		SetAdjustData(XY *lcdPoints, XY *tsPoints, int nCount){return 0;};
		virtual	BOOL	SetAdjustState(int	nState)			{return FALSE;};
	

};
extern	CTouchScreen		*g_pTouch;    //netdigger 2007/4/17

#endif
