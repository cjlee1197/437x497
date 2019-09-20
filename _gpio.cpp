/*==========================================================================+
|  Class    : kb.cpp														|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : yang                                                       	|
|  Version  : V1.00                                                         |
|  Creation : 2005/01/18                                                    |
|			: 2005/01/31	netdigger										|
|  Remark	: 																|
+==========================================================================*/
#include	"_gpio.h"
#include	"main.h"
#include	"utils.h"
#include	"tmconfig.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
IMPLEMENT_DYNCREATE(CtmGpio, CtmPackClass)

CtmGpio*	g_pgpio = NULL;

CtmGpio::CtmGpio() : CtmPackClass()
{
	
}

CtmGpio::~CtmGpio()
{
	
}

void	_Gpio_OutputWrite(char* io_status)
{
	//printf("_Gpio_OutputWrite\n");
	if (g_pgpio != NULL)
	{
		//printf("g_pgpio != NULL\n");
		g_pgpio->Gpio_OutputWrite(io_status);
	}
}

void	_Gpio_buzzer(bool buzzer_enable)
{
	printf("_Gpio_OutputWrite\n");
	if (g_pgpio != NULL)
	{
		//printf("g_pgpio != NULL\n");
		g_pgpio->Gpio_buzzer(buzzer_enable);
	}
}