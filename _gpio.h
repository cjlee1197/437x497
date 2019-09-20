/*===========================================================================+
|  Function : _KEYBOARD                                                      |
|  Task     : Extended Keyboard Access Utility header file                   |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Andrew                                                         |
|  Version  : V1.00                                                          |
|  Creation : 03/26/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D__GPIO
#define     D__GPIO

#include    "common.h"
#include	"cmddef.h"
#include	"tmpackclass.h"
#include	"_iodef.h"
#include	"device.h"



/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/


/*---------------------------------------------------------------------------+
|           Application function key                                         |
+---------------------------------------------------------------------------*/


/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

extern	void	_Gpio_OutputWrite(char* io_status);
extern	void	_Gpio_buzzer(bool buzzer_enable);
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
class	CtmGpio: public	CtmPackClass
{
	friend	void	_Gpio_OutputWrite(char* io_status);
	friend	void	_Gpio_buzzer(bool buzzer_enable);
	DECLARE_DYNCREATE(CtmGpio)
	public:
		CtmGpio();
		~CtmGpio();
	public:
		virtual void 	Gpio_OutputWrite(char* io_status) {};
		virtual	void	Gpio_buzzer(bool buzzer_enable) {};
	protected:
};

extern		CtmGpio*	g_pgpio;

#endif

