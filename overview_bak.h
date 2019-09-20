/*===========================================================================+
|  Class    : View OverView                                                  |
|  Task     : View OverView header file                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Woody                                                          |
|  Version  : V1.00                                                          |
|  Creation : 11/08/1998                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef     D_VOVER
#define     D_VOVER

#include    "formview.h"
#include	"static.h"
#include	"edit.h"
#include	"cartoon.h"
#include	"imagebox.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - ViewOverView                                 |
+===========================================================================*/
class CtmViewOverview : public CtmFormView
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmViewOverview(tmWND* pwnd);
    ~CtmViewOverview();

/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    enum    ITEMID 
	{
									IDI_BEGIN,
		IDI_ALARM				   =IDI_BEGIN,
		IDI_NOZZOLE,
		IDI_TEMPER,
		
									 IDI_END,
                                     IDS_BEGIN = IDI_END,
		IDS_ACTION            =IDS_BEGIN,
		IDS_CURRENT,
        IDS_ACTIONTIME,
		IDS_CYCLETIME,
		
		IDS_ACTUAL_CLAMP,
		IDS_ACTUAL_CLAMPVEL,
		IDS_ACTUAL_INJECT,
		IDS_ACTUAL_INJECTVEL,
		IDS_ACTUAL_INJECTUNIT,
		IDS_ACTUAL_INJECTUNITVEL,
		IDS_ACTUAL_EJECT,
		IDS_ACTUAL_EJECTVEL,
		IDS_ACTUAL_RECOVERY,
		IDS_ACTUAL_RECOVERYVEL,

									IDS_TEMP_BEGIN,
		IDS_REALTEMP1              =IDS_TEMP_BEGIN,
		IDS_REALTEMP2,
		IDS_REALTEMP3,
		IDS_REALTEMP4,
		IDS_REALTEMP5,
		IDS_REALTEMP6,
		IDS_REALTEMP7,
		IDS_REALTEMP8,
		IDS_REALTEMP9,
									IDS_TEMP_END,
									IDS_END = IDS_TEMP_END,
		
									IDE_BEGIN  =IDS_END,
		                            IDE_ACTUAL_BEGIN=IDE_BEGIN,
		IDE_SHOTCOUNT        =IDE_ACTUAL_BEGIN,					//<<yang 2006/3/14 
        IDE_ACTIONTIME,
		IDE_CYCLETIME,
		
		IDE_ACTUAL_CLAMP_VEL,
		IDE_ACTUAL_INJ_VEL,
		IDE_ACTUAL_INJUNIT_VEL,
		IDE_ACTUAL_EJECT_VEL,
		IDE_ACTUAL_RECOVERY_VEL,

		IDE_ACTUAL_CLAMP_POS,
		IDE_ACTUAL_INJ_POS,
		IDE_ACTUAL_INJUNIT_POS,
		IDE_ACTUAL_EJECT_POS,
		IDE_ACTUAL_RECOVERY_POS,

		                            IDE_ACTUAL_END,

									IDE_TEMP_BEGIN=IDE_ACTUAL_END,
		IDE_REALTEMP1              =IDE_TEMP_BEGIN,
		IDE_REALTEMP2,
		IDE_REALTEMP3,
		IDE_REALTEMP4,
		IDE_REALTEMP5,
		IDE_REALTEMP6,
		IDE_REALTEMP7,
		IDE_REALTEMP8,
		IDE_REALTEMP9,
									IDE_TEMP_END,
/*
		IDE_OILTEMP                =IDE_TEMP_END,
		IDE_MOTORTEMP,
		IDE_SCREWSPEED,
*/
									IDE_END=IDE_TEMP_END,
									IDC_BEGIN = IDE_END,
		IDC_CLAMP				   =IDC_BEGIN,
		IDC_INJECT,
		IDC_CLAMP1,
		IDC_ENJECT,
									IDC_END,	  
		ID_END                     =IDC_END,
		ID_MAX_TEMP                =IDE_TEMP_END - IDE_TEMP_BEGIN,
		ID_MAX_STATIC              =IDS_END - IDS_BEGIN,
		ID_MAX_EDITX1              =IDE_END - IDE_BEGIN,
		ID_MAX_IMAGEBOX			   =IDI_END - IDI_BEGIN,
		ID_MAX_CARTOON			   =IDC_END	- IDC_BEGIN,
		ID_MAX_DIALOGITEM          =ID_END  + 1,
    };

	WORD        m_wTempC [ID_MAX_TEMP];
	WORD        m_wOperationMode;
	WORD        m_wOperationStep;
	WORD        m_wOperationSubStep;
	DWORD       m_dwShotCountCurrent;         // 秨家羆计龟悔
	char        m_szCurrent[10];
	
	static  WORD          m_wStatic [ID_MAX_STATIC];
    static  tmSTATIC      m_static  [ID_MAX_STATIC];
    static  tmEDITX1      m_editx1 	[ID_MAX_EDITX1];
    
    static	tmCARTOON	  	m_cartoon [ID_MAX_CARTOON];
    static	tmIMAGEBOX		m_imagebox[ID_MAX_IMAGEBOX];
    
    static	WORD	m_nClamp;			// 家畒龟悔
	static	WORD	m_nInject;			// 甮龟悔
	static	WORD	m_nEnject;			// 叉家龟悔
	static	WORD	m_nClampMax;		// 家畒程
	static	WORD	m_nInjectMax;		// 甮程
	static	WORD	m_nEnjectMax;		// 叉家程
    
    static  tmDIALOG_1    m_dlg_1   [ID_MAX_DIALOGITEM];
            tmDIALOG      m_dlg     [ID_MAX_DIALOGITEM];
	
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Set();                      // Virtual function
    void    Update();                   // Virtual function
    WORD    OnChange(WORD wIDControl);

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:

};

#endif

