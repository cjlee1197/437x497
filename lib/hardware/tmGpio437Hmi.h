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
|			: 06/06/2008      James											 |
|  Revision :                                                                |
+===========================================================================*/
#ifndef     D__TMGPIO
#define     D__TMGPIO

#include    "../../common.h"
#include	"../../_gpio.h"
#include 	"../../commonaction.h"


class	CtmGpio437Hmi: public CtmGpio
{
	DECLARE_DYNCREATE(CtmGpio437Hmi)
	public:
		CtmGpio437Hmi();
		~CtmGpio437Hmi();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		void	Gpio_OutputWrite(char* io_status);
		void	Gpio_buzzer(bool buzzer_enable);
	protected:
		void 	Init_Gpio();					//	calling it in the CreateSelf
		void 	Destroy_Gpio();				//	calling it in the FreeSelf
		sem_t		m_Write;
		pthread_t	m_GpioReadInput;
	
		
	private:
  protected:
};

extern	void 	Gpio_InputRun();
extern	void		ReadGpio(char /*KEYCODE*/ io);
#ifndef	D_BEOBJECT_CtmGpio437Hmi	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif
#endif

