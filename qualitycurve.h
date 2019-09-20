/*===========================================================================+
|  Class    : CtmQualityCurve                                                |
|  Task     : CtmQualityCurve header file                                    |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeff.Xiao                                                      |
|  Version  : V1.00                                                          |
|  Creation : 08/30/2005                                                     |
|  Revision : V1.10                                                          |
+===========================================================================*/

#ifndef     D_QUALITYCURVE
#define     D_QUALITYCURVE

#include    "static.h"
#include	"formview.h"
#include	"curve.h"
#include	"selectbox.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
#define     tmMAX_MONITOR         100
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - ViewTemperature2                             |
+===========================================================================*/
class CtmQualityCurve : public CtmFormView
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmQualityCurve(tmWND* pwnd);
    ~CtmQualityCurve();

/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    enum    ITEMID {
										IDS_BEGIN,                                        
            IDS_CAPTION		           =IDS_BEGIN,
                                        IDS_END,
                                        IDE_BEGIN = IDS_END,
            IDE_SELECTBOX1             =IDE_BEGIN,
            IDE_SELECTBOX2,
            IDE_SELECTBOX3,
            IDE_SELECTBOX4,
            IDE_SELECTBOX5,
            IDE_SELECTBOX6,
            IDE_SELECTBOX7,
            IDE_SELECTBOX8,
            IDE_SELECTBOX9,
            IDE_SELECTBOX10,
            IDE_SELECTBOX11,
            IDE_SELECTBOX12,
            IDE_SELECTBOX13,
//            IDE_SELECTBOX14,
//            IDE_SELECTBOX15,
//            IDE_SELECTBOX16,
					                    IDE_END,
					                    IDC_BEGIN = IDE_END,
			IDC_QUALITYCURVE              = IDC_BEGIN,
										IDC_END,
																				
            ID_END                     =IDC_END,
            ID_MAX_STATIC              =IDS_END - IDS_BEGIN,
            ID_MAX_SELECT              =IDE_END - IDE_BEGIN,
            ID_MAX_CURVE			   =IDC_END - IDC_BEGIN,
            ID_MAX_DIALOGITEM          =ID_END  + 1,
            };

    static  WORD        	m_wStatic	 [ID_MAX_STATIC];
    static  tmSTATIC      	m_static	 [ID_MAX_STATIC];
    static  tmSELECT      	m_selectbox	 [ID_MAX_SELECT];
    static  tmCURVE      	m_curve	 	 [ID_MAX_CURVE];
    
    static  tmDIALOG_1    	m_dlg_1		 [ID_MAX_DIALOGITEM];
            tmDIALOG      	m_dlg		 [ID_MAX_DIALOGITEM];
	WORD					m_KeyFlag;

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Set();                      // Virtual function
    void    Update();                   // Virtual function
    void	ReadData(int Index);
    WORD    OnChange(WORD wIDControl);
    void	CaptionShow();

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	
};
#endif

