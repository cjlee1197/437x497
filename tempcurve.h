/*===========================================================================+
|  Class    : TaskDBLog                                                      |
|  Task     : TaskDBLog header file                                          |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeff.Xiao                                                      |
|  Version  : V1.00                                                          |
|  Creation : 08/23/2005                                                     |
|  Revision : V1.10                                                          |
+===========================================================================*/

#ifndef     D_TEMPCURVE
#define     D_TEMPCURVE

#include    "edit.h"
#include    "static.h"
#include	"formview.h"
#include	"curve.h"
#include	"files.h"

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
|           Class declaration - ViewTemperature2                              |
+===========================================================================*/
class CtmTempCurve : public CtmFormView
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmTempCurve(tmWND* pwnd);
    ~CtmTempCurve();

/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    enum    ITEMID {
										IDS_BEGIN,                                        
            IDS_CAPTION		           =IDS_BEGIN,
                                        IDS_END,
                                        IDE_BEGIN = IDS_END,
            IDE_RANKSELECT             =IDE_BEGIN,
					                    IDE_END,
					                    IDC_BEGIN = IDE_END,
			IDC_TEMPCURVE1              = IDC_BEGIN,
			IDC_TEMPCURVE2,
			IDC_TEMPCURVE3,
			IDC_TEMPCURVE4,
										IDC_END,

            ID_END                     =IDC_END,
            ID_MAX_STATIC              =IDS_END - IDS_BEGIN,
            ID_MAX_EDITX1              =IDE_END - IDE_BEGIN,
            ID_MAX_CURVE			   =IDC_END - IDC_BEGIN,
            ID_MAX_DIALOGITEM          =ID_END  + 1,
            };
 
    static  WORD        	m_wStatic	 [ID_MAX_STATIC];
    static  tmSTATIC      	m_static	 [ID_MAX_STATIC];
    static  tmEDITX1      	m_editx1	 [ID_MAX_EDITX1];
    static  tmCURVE      	m_curve	 	 [ID_MAX_CURVE];
    
    static  tmDIALOG_1    	m_dlg_1		 [ID_MAX_DIALOGITEM];
            tmDIALOG      	m_dlg		 [ID_MAX_DIALOGITEM];
	CtmRecordFile tmTemp;
	WORD   m_AllInterface[21];
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Set();                      // Virtual function
    void    Update();                   // Virtual function
    WORD    OnChange(WORD wIDControl);
    void    CaptionShow();

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	int		m_nRankIndex;			// ·Å«×­¶¼ÆID
	void	ReadInterface();
};
#endif

