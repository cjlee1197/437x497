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

#ifndef     D__KEYBOARD
#define     D__KEYBOARD

#include    "common.h"
#include	"cmddef.h"
#include	"tmpackclass.h"
#include	"_iodef.h"
#include	"device.h"



/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     _NULL_KEY           0x0000

#define     _ESC                0x011B
#define     _SHIFT_ESC          0x011B
#define     _CTRL_ESC           0x011B
#define     _ALT_ESC            0x0100

#define     _BKSP               0x0E08
#define     _SHIFT_BKSP         0x0E08
#define     _CTRL_BKSP          0x0E7F
#define     _ALT_BKSP           0x0E00

#define     _TAB                0x0F09
#define     _SHIFT_TAB          0x0F00
#define     _CTRL_TAB           0x9400
#define     _ALT_TAB            0xA500

#define     _ENTER              0x1C0A
#define     _SHIFT_ENTER        0x1C0D
#define     _CTRL_ENTER         0x1C0A
#define     _ALT_ENTER          0x1C00
#define     _EXT_ENTER          0xE00D
#define     _EXT_SHIFT_ENTER    0xE00D
#define     _EXT_CTRL_ENTER     0xE00A
#define     _EXT_ALT_ENTER      0xA600

//add 2016/10/8 9:08:16
#define     _HELP_ENTER   		0xFEFE


#define     _F1                 0x3B00
#define     _F2                 0x3C00
#define     _F3                 0x3D00
#define     _F4                 0x3E00
#define     _F5                 0x3F00
#define     _F6                 0x4000
#define     _F7                 0x4100
#define     _F8                 0x4200
#define     _F9                 0x4300
#define     _F10                0x4400
#define     _F11                0x8500
#define     _F12                0x8600
#define     _SHIFT_F1           0x5400
#define     _SHIFT_F2           0x5500
#define     _SHIFT_F3           0x5600
#define     _SHIFT_F4           0x5700
#define     _SHIFT_F5           0x5800
#define     _SHIFT_F6           0x5900
#define     _SHIFT_F7           0x5A00
#define     _SHIFT_F8           0x5B00
#define     _SHIFT_F9           0x5C00
#define     _SHIFT_F10          0x5D00
#define     _SHIFT_F11          0x8700
#define     _SHIFT_F12          0x8800
#define     _CTRL_F1            0x5E00
#define     _CTRL_F2            0x5F00
#define     _CTRL_F3            0x6000
#define     _CTRL_F4            0x6100
#define     _CTRL_F5            0x6200
#define     _CTRL_F6            0x6300
#define     _CTRL_F7            0x6400
#define     _CTRL_F8            0x6500
#define     _CTRL_F9            0x6600
#define     _CTRL_F10           0x6700
#define     _CTRL_F11           0x8900
#define     _CTRL_F12           0x8A00
#define     _ALT_F1             0x6800
#define     _ALT_F2             0x6900
#define     _ALT_F3             0x6A00
#define     _ALT_F4             0x6B00
#define     _ALT_F5             0x6C00
#define     _ALT_F6             0x6D00
#define     _ALT_F7             0x6E00
#define     _ALT_F8             0x6F00
#define     _ALT_F9             0x7000
#define     _ALT_F10            0x7100
#define     _ALT_F11            0x8B00
#define     _ALT_F12            0x8C00

#define     _ALT_1              0x7800
#define     _ALT_2              0x7900
#define     _ALT_3              0x7A00
#define     _ALT_4              0x7B00
#define     _ALT_5              0x7C00
#define     _ALT_6              0x7D00
#define     _ALT_7              0x7E00
#define     _ALT_8              0x7F00
#define     _ALT_9              0x8000
#define     _ALT_0              0x8100

#define     _A_                 0x1E41
#define     _B_                 0x3042
#define     _C_                 0x2E43
#define     _D_                 0x2044
#define     _E_                 0x1245
#define     _F_                 0x2146
#define     _G_                 0x2247
#define     _H_                 0x2348
#define     _I_                 0x1749
#define     _J_                 0x244A
#define     _K_                 0x254B
#define     _L_                 0x264C
#define     _M_                 0x324D
#define     _N_                 0x314E
#define     _O_                 0x184F
#define     _P_                 0x1950
#define     _Q_                 0x1051
#define     _R_                 0x1352
#define     _S_                 0x1F53
#define     _T_                 0x1454
#define     _U_                 0x1655
#define     _V_                 0x2F56
#define     _W_                 0x1157
#define     _X_                 0x2D58
#define     _Y_                 0x1559
#define     _Z_                 0x2C5A
#define     _a_                 0x1E61
#define     _b_                 0x3062
#define     _c_                 0x2E63
#define     _d_                 0x2064
#define     _e_                 0x1265
#define     _f_                 0x2166
#define     _g_                 0x2267
#define     _h_                 0x2368
#define     _i_                 0x1769
#define     _j_                 0x246A
#define     _k_                 0x256B
#define     _l_                 0x266C
#define     _m_                 0x326D
#define     _n_                 0x316E
#define     _o_                 0x186F
#define     _p_                 0x1970
#define     _q_                 0x1071
#define     _r_                 0x1372
#define     _s_                 0x1F73
#define     _t_                 0x1474
#define     _u_                 0x1675
#define     _v_                 0x2F76
#define     _w_                 0x1177
#define     _x_                 0x2D78
#define     _y_                 0x1579
#define     _z_                 0x2C7A

#define     _CTRL_A             0x1E01
#define     _CTRL_B             0x3002
#define     _CTRL_C             0x2E03
#define     _CTRL_D             0x2004
#define     _CTRL_E             0x1205
#define     _CTRL_F             0x2106
#define     _CTRL_G             0x2207
#define     _CTRL_H             0x2308
#define     _CTRL_I             0x1709
#define     _CTRL_J             0x240A
#define     _CTRL_K             0x250B
#define     _CTRL_L             0x260C
#define     _CTRL_M             0x320D
#define     _CTRL_N             0x310E
#define     _CTRL_O             0x180F
#define     _CTRL_P             0x1910
#define     _CTRL_Q             0x1011
#define     _CTRL_R             0x1312
#define     _CTRL_S             0x1F13
#define     _CTRL_T             0x1414
#define     _CTRL_U             0x1615
#define     _CTRL_V             0x2F16
#define     _CTRL_W             0x1117
#define     _CTRL_X             0x2D18
#define     _CTRL_Y             0x1519
#define     _CTRL_Z             0x2C1A
#define     _ALT_A              0x1E00
#define     _ALT_B              0x3000
#define     _ALT_C              0x2E00
#define     _ALT_D              0x2000
#define     _ALT_E              0x1200
#define     _ALT_F              0x2100
#define     _ALT_G              0x2200
#define     _ALT_H              0x2300
#define     _ALT_I              0x1700
#define     _ALT_J              0x2400
#define     _ALT_K              0x2500
#define     _ALT_L              0x2600
#define     _ALT_M              0x3200
#define     _ALT_N              0x3100
#define     _ALT_O              0x1800
#define     _ALT_P              0x1900
#define     _ALT_Q              0x1000
#define     _ALT_R              0x1300
#define     _ALT_S              0x1F00
#define     _ALT_T              0x1400
#define     _ALT_U              0x1600
#define     _ALT_V              0x2F00
#define     _ALT_W              0x1100
#define     _ALT_X              0x2D00
#define     _ALT_Y              0x1500
#define     _ALT_Z              0x2C00

#define     _HOME               0x6600//0x47E0		//netdigger 2005/2/24
#define     _UARROW             0x6700//0x48E0		//netdigger 2005/2/15
#define     _PAGEUP             0x6800//0x49E0		//netdigger 2005/2/24
#define     _LARROW             0x6900//0x4BE0		//netdigger 2005/2/15
#define     _RARROW             0x6A00//0x4DE0		//netdigger 2005/2/15
#define     _END                0x6B00//0x4FE0		//netdigger 2005/2/24
#define     _DARROW             0x6C00//0x50E0		//netdigger 2005/2/15
#define     _PAGEDN             0x6D00//0x51E0		//netdigger 2005/2/24
#define     _INSERT             0x6E00//0x52E0		//netdigger 2005/2/24
#define     _DELETE             0x6F00//0x53E0		//netdigger 2005/2/24
#define     _EXT_HOME           0x4700
#define     _EXT_UARROW         0x4800
#define     _EXT_PAGEUP         0x4900
#define     _EXT_LARROW         0x4B00
#define     _EXT_5              0x4C00
#define     _EXT_RARROW         0x4D00
#define     _EXT_END            0x4F00
#define     _EXT_DARROW         0x5000
#define     _EXT_PAGEDN         0x5100
#define     _EXT_INSERT         0x5200
#define     _EXT_DELETE         0x5300

#define     _ONE                0x0231  // '1'
#define     _TWO                0x0332  // '2'
#define     _THREE              0x0433  // '3'
#define     _FOUR               0x0534  // '4'
#define     _FIVE               0x0635  // '5'
#define     _SIX                0x0736  // '6'
#define     _SEVEN              0x0837  // '7'
#define     _EIGHT              0x0938  // '8'
#define     _NINE               0x0A39  // '9'
#define     _ZERO               0x0B30  // '0'
#define     _N1                 0x0231  // '1'
#define     _N2                 0x0332  // '2'
#define     _N3                 0x0433  // '3'
#define     _N4                 0x0534  // '4'
#define     _N5                 0x0635  // '5'
#define     _N6                 0x0736  // '6'
#define     _N7                 0x0837  // '7'
#define     _N8                 0x0938  // '8'
#define     _N9                 0x0A39  // '9'
#define     _N0                 0x0B30  // '0'
#define     _STAR               0x092A  // '*'
#define     _MINUS              0x0C2D  // '-'
#define     _PLUS               0x0D2B  // '+'
#define     _DOT                0x342E  // '.'
#define     _SLASH              0x352F  // '/'
#define     _SPACE              0x3920  // ' '

#define     _EXT_MINUS          0x4A2D  // '-'
#define     _EXT_PLUS           0x4E2B  // '+'

#define     _SHIFT              0x3600  // Define the scan code as a key code
#define     _CTRL               0x1D00  // Define the scan code as a key code
#define     _ALT                0x3800  // Define the scan code as a key code
#define     _CTRL_SPACE         0x1D01  // Define the _CTRL_SPACE key code
#define     _ALT_SPACE          0x3801  // Define the _ALT_SPACE  key code

#define     ASCII_NORMAL_START  0x20    // ' '
#define     ASCII_NORMAL_END    0x7E    // '~'

#define		MASK_LED			0x8000	// Mask of LED	LEO20070620


/*---------------------------------------------------------------------------+
|           Application function key                                         |
+---------------------------------------------------------------------------*/
#define     _KEY__MOLD          _F9
#define     _KEY__VIEW          _F10
#define     _KEY__SETUP         _F11
#define     _KEY__OTHER         _F12
#define     _KEY__SHELL         _ALT_F3
#define     _KEY__EXIT          _ALT_F4
#define     _KEY__COPY          _ALT_C

#define     _KEY_CLEAR          _CTRL_C
#define     _KEY_ALPHA          _CTRL_SPACE
#define     _KEY_SELECT         ' '
#define     _KEY_HEXADECIMAL    '*'
//#define     _KEY_TEXTINPUT      '*'	//BW<WY2003/01/20
#define     _KEY_TEXTINPUT      '.'
#define     _KEY_LANGUAGE       _CTRL_X
#define     _KEY_PRINT          _CTRL_P

#define     _KEY_LEFTFIELD      _CTRL_L
#define     _KEY_RIGHTFIELD     _CTRL_R
#define     _KEY_UPFIELD        _CTRL_U
#define     _KEY_DOWNFIELD      _CTRL_D

#define     _KEY_LEFTFIELD_UP	_CTRL_V		//yang 2007/4/9 
#define     _KEY_RIGHTFIELD_UP	_CTRL_W		
#define     _KEY_UPFIELD_UP		_CTRL_Y		
#define     _KEY_DOWNFIELD_UP	_CTRL_Z		
#define		_ENTER_UP			_EXT_CTRL_ENTER		


#define     _KEY_UPVIEW         _PAGEUP
#define     _KEY_DOWNVIEW       _PAGEDN

#define     _KEY_INCREASE       _UARROW
#define     _KEY_DECREASE       _DARROW

/*******
|       Simulated key
*******/
#define     _KEY_OK             _F1     // _ENTER
#define     _KEY_CANCEL         _F8     // _ESC


#define     PanelManualOn()             _ActLed(LED_PNL_TM_MANUAL)					//	手動         	
#define		PanelSemiAutoOn()			_ActLed(LED_PNL_TM_SEMIATUO)				//	半自動          
#define		PanelTimeAutoOn()			_ActLed(LED_PNL_TM_ATUO)					//	全自動          
#define		PanelSensorAutoOn()			_ActLed(LED_PNL_TM_SESORATUO)				//	電眼自動        
#define		PanelSensorAutoOff()		_ActLed(LED_PNL_TM_SESORATUO_OFF)			//	電眼自動關      
#define		PanelHeaterOn()				_ActLed(LED_PNL_TM_HEAT_ON)					//	電熱開          
#define		PanelHeaterOff()			_ActLed(LED_PNL_TM_HEAT_OFF)				//	電熱關          
#define		PanelMotorOn()				_ActLed(LED_PNL_TM_MOTOR_ON)				//	馬達開關        
#define		PanelMotorOff()				_ActLed(LED_PNL_TM_MOTOR_OFF)				//	馬達關          
#define		PanelDoorOpen()				_ActLed(LED_PNL_TM_DOOR_OPEN)				//	安全門開        
#define		PanelDoorClose()			_ActLed(LED_PNL_TM_DOOR_CLOSE)				//	安全門關        
#define		PanelAutoAdjOn()			_ActLed(LED_PNL_TM_ADJUST_ATUO)				//	自動調模        
#define		PanelAutoAdjOff()			_ActLed(LED_PNL_TM_ADJUSTO_ATUO_OFF)		//	自動調模關      
#define		PanelAdjFWD()				_ActLed(LED_PNL_TM_ADJUST_FWD)				//	調模進          
#define		PanelAdjBWD()				_ActLed(LED_PNL_TM_ADJUST_BWD)				//	調模退          
#define		PanelCloseMld()				_ActLed(LED_PNL_TM_CLOSEMLD)				//	關模            
#define		PanelOpenMld()				_ActLed(LED_PNL_TM_OPENMLD)					//	開模            
#define		PanelInject()				_ActLed(LED_PNL_TM_INJECT)					//	射出            
#define		PanelInjectB()				_ActLed(LED_PNL_TM_INJECTB)					//	射出B           
#define		PanelInjectBWD()			_ActLed(LED_PNL_TM_INJECT_BWD)				//	射退            
#define		PanelInjectBBWD()			_ActLed(LED_PNL_TM_INJECTB_BWD)				//	射退B           
#define		PanelICharge()				_ActLed(LED_PNL_TM_CHARGE)					//	儲料            
#define		PanelIChargeB()				_ActLed(LED_PNL_TM_CHARGEB)					//	儲料B           
#define		PanelAutoPurge()			_ActLed(LED_PNL_TM_PURGE_ATUO)				//	自動清料        
#define		PanelAutoPurgeB()			_ActLed(LED_PNL_TM_PURGEB_ATUO)				//	自動清料B       
#define		PanelEjectFWD()				_ActLed(LED_PNL_TM_EJECT_FWD)				//	托模進          
#define		PanelEjectBWD()				_ActLed(LED_PNL_TM_EJECT_BWD)				//	托模退          
#define		PanelEjectBFWD()			_ActLed(LED_PNL_TM_EJECTB_FWD)				//	托模進B         
#define		PanelEjectBBWD()			_ActLed(LED_PNL_TM_EJECTB_BWD)				//	托模退B         
#define		PanelUEjectFWD()			_ActLed(LED_PNL_TM_UEJECT_FWD)				//	上托進
#define		PanelUEjectBWD()			_ActLed(LED_PNL_TM_UEJECT_BWD)				//	上托退
#define		PanelMEjectFWD()			_ActLed(LED_PNL_TM_MEJECT_FWD)				//	中托進             
#define		PanelMEjectBWD()			_ActLed(LED_PNL_TM_MEJECT_BWD)				//	中托退             
#define		PanelDEjectFWD()			_ActLed(LED_PNL_TM_DEJECT_FWD)				//	下托進                 
#define		PanelDEjectBWD()			_ActLed(LED_PNL_TM_DEJECT_BWD)				//	下托退                 
#define		PanelNozzleFWD()			_ActLed(LED_PNL_TM_NOZZLE_FWD)				//	座台進          
#define		PanelNozzleBWD()			_ActLed(LED_PNL_TM_NOZZLE_BWD)				//	座台退          
#define		PanelNozzleOut()			_ActLed(LED_PNL_TM_DIAL_OUT)				//	轉盤出          
#define		PanelNozzleIn()				_ActLed(LED_PNL_TM_DIAL_IN)					//	轉盤進          
#define		PanelAnchorBWD()			_ActLed(LED_PNL_TM_ANCHOR_BWD)				//	定位退          
#define		PanelAnchorFWD()			_ActLed(LED_PNL_TM_ANCHOR_FWD)				//	定位進          
#define		PanelBlastM()				_ActLed(LED_PNL_TM_BLASTM)					//	公模吹氣        
#define		PanelBlastS()				_ActLed(LED_PNL_TM_BLASTS)					//	母模吹氣        
#define		PanelBlastC()				_ActLed(LED_PNL_TM_BLASTC)					//	吹氣C           
#define		PanelBlastD()				_ActLed(LED_PNL_TM_BLASTD)					//	吹氣D           
#define		PanelUBlast()				_ActLed(LED_PNL_TM_UBLAST)					//	上吹氣          
#define		PanelDBlast()				_ActLed(LED_PNL_TM_DBLAST)					//	下吹氣          
#define		PanelRub()					_ActLed(LED_PNL_TM_RUB)						//	潤滑            
#define		PanelRub2()					_ActLed(LED_PNL_TM_RUB2)					//	潤滑2           
#define		PanelSlideMold()			_ActLed(LED_PNL_TM_SLIDEMOLD)				//	滑模            
#define		PanelCoreAFWD()				_ActLed(LED_PNL_TM_COREA_FWD)				//	中子A進         
#define		PanelCoreABWD()				_ActLed(LED_PNL_TM_COREA_BWD)				//	中子A退         
#define		PanelCoreBFWD()				_ActLed(LED_PNL_TM_COREB_FWD)				//	中子B進         
#define		PanelCoreBBWD()				_ActLed(LED_PNL_TM_COREB_BWD)				//	中子B退         
#define		PanelCoreCFWD()				_ActLed(LED_PNL_TM_COREC_FWD)				//	中子C進         
#define		PanelCoreCBWD()				_ActLed(LED_PNL_TM_COREC_BWD)				//	中子C退         
#define		PanelCoreDFWD()				_ActLed(LED_PNL_TM_CORED_FWD)				//	中子D進         
#define		PanelCoreDBWD()				_ActLed(LED_PNL_TM_CORED_BWD)				//	中子D退         
#define		PanelCoreEFWD()				_ActLed(LED_PNL_TM_COREE_FWD)				//	中子E進         
#define		PanelCoreEBWD()				_ActLed(LED_PNL_TM_COREE_BWD)				//	中子E退         
#define		PanelCoreFFWD()				_ActLed(LED_PNL_TM_COREF_FWD)				//	中子F進         
#define		PanelCoreFBWD()				_ActLed(LED_PNL_TM_COREF_BWD)				//	中子F退         
#define		PanelUCoreFWD()				_ActLed(LED_PNL_TM_UCORE_FWD)				//	上中子進        
#define		PanelUCoreBWD()				_ActLed(LED_PNL_TM_UCORE_BWD)				//	上中子退        
#define		PanelDCoreFWD()				_ActLed(LED_PNL_TM_DCORE_FWD)				//	下中子進        
#define		PanelDCoreBWD()				_ActLed(LED_PNL_TM_DCORE_BWD)				//	下中子退        
#define		PanelMasterState()			_ActLed(LED_PNL_TM_MASTER_STATE)			//	主機狀態(燈)    
#define		PanelMasterUp()				_ActLed(LED_PNL_TM_MASTER_UP)				//	主機升速        
#define		PanelMaster()				_ActLed(LED_PNL_TM_MASTER)					//	主機            
#define		PanelHeaterOnOff()			_ActLed(LED_PNL_TM_HEATERONOFF)				//	電熱啟停        
#define		PanelMotorOnOff()			_ActLed(LED_PNL_TM_MOTORONOFF)				//	電機啟停        
#define		PanelMotorUp()				_ActLed(LED_PNL_TM_MOTORUP)					//	電機升速        
#define		PanelMotorDown()			_ActLed(LED_PNL_TM_MOTORDOWN)				//	電機降速        
#define		PanelTowOnOff()				_ActLed(LED_PNL_TM_TOWONOFF)				//	牽引啟停        
#define		PanelTowUp()				_ActLed(LED_PNL_TM_TOWUP)					//	牽引升速        
#define		PanelTowDown()				_ActLed(LED_PNL_TM_TOWDOWN)					//	牽引降速        
#define		PanelSynUp()				_ActLed(LED_PNL_TM_SYNCHROUP)				//	同步升速        
#define		PanelSynDown()				_ActLed(LED_PNL_TM_SYNCHRODOWN)				//	同步降速        
#define		PanelOilPump()				_ActLed(LED_PNL_TM_OILPUMP)					//	油泵            
#define		PanelVacuumPump()			_ActLed(LED_PNL_TM_VACUUMPUMP)				//	真空泵          
#define		PanelSlaveState1()			_ActLed(LED_PNL_TM_SLAVESTATE1)				//	共擠1狀態(燈)   
#define		PanelSlaveState2()			_ActLed(LED_PNL_TM_SLAVESTATE2)				//	共擠2狀態(燈)   
#define		PanelSlaveState3()			_ActLed(LED_PNL_TM_SLAVESTATE3)				//	共擠3狀態(燈)   
#define		PanelSlave1()				_ActLed(LED_PNL_TM_SLAVE1)					//	共擠1           
#define		PanelSlave2()				_ActLed(LED_PNL_TM_SLAVE2)					//	共擠2           
#define		PanelSlave3()				_ActLed(LED_PNL_TM_SLAVE3)					//	共擠3           
#define		PanelFeedOnOff()			_ActLed(LED_PNL_TM_FEEDONOFF)				//	餵料啟停        
#define		PanelFeedUp()				_ActLed(LED_PNL_TM_FEEDUP)					//	餵料升速        
#define		PanelFeedDown()				_ActLed(LED_PNL_TM_FEEDDOWN)				//	餵料降速      
#define		PanelAdjSlow7GL()				_ActLed(LED_PNL_GL_ADJSLOW)				//JOYCE2008-9-24 調模慢速LED ON
#define		PanelAutoAdjON7GL()			_ActLed(LED_PNL_GL_AUTOADJ)				//JOYCE2008-9-24 自動調模LED ON      
#define		PanelAutoAdjOFF7GL()		_ActLed(LED_PNL_GL_AUTOADJ_OFF)	  //JOYCE2008-9-24 自動調模LED OFF      
#define		PanelIChargeOFF7GL()		_ActLed(LED_PNL_GL_CHARGE_OFF)	  //JOYCE2008-9-24 儲料LED OFF
#define		PanelTransferOn()		  	PanelSensorAutoOn()			//傳送帶      //JOYCE2009-1-13 ADD  注吹機
#define		PanelTransferOff()			PanelSensorAutoOff()		//傳送帶      //JOYCE2009-1-13 ADD  注吹機

#define		PanelLeftAutoOn7JM()			_ActLed(LED_PNL_JM_LEFTAUTO_ON) 	//JOYCE2009-3-5 LEFT自動       
#define		PanelLeftAutoOff7JM()			_ActLed(LED_PNL_JM_LEFTAUTO_OFF) 	//JOYCE2009-3-5 LEFT自動       
#define		PanelRightAutoOn7JM()		  _ActLed(LED_PNL_JM_RIGHTAUTO_ON)  //JOYCE2009-3-5 RIGHT自動       
#define		PanelRightAutoOff7JM()		_ActLed(LED_PNL_JM_RIGHTAUTO_OFF) //JOYCE2009-3-5 RIGHT自動       
#define		PanelChargMotorOn7JM()		_ActLed(LED_PNL_JM_CHARGEMOTOR_ON)//JOYCE2009-3-5 擠料電機                                            
#define		PanelChargMotorOff7JM()		_ActLed(LED_PNL_JM_CHARGEMOTOR_OFF)//JOYCE2009-3-5 擠料電機                                          
#define		PanelRightHeaterOn()		_ActLed(LED_PNL_TM_RIGHTHEATER_ON)		//2012-6-20                                             
#define		PanelRightHeaterOff()		_ActLed(LED_PNL_TM_RIGHTHEATER_OFF)		//2012-6-20                                        
#define		PanelLeftHeaterOn()			_ActLed(LED_PNL_TM_LEFTHEATER_ON)		//2012-6-20                                             
#define		PanelLeftHeaterOff()		_ActLed(LED_PNL_TM_LEFTHEATER_OFF)		//2012-6-20                                        

#define		PanelSpare1()				_ActLed(LED_PNL_TM_Spare1)  
#define		PanelSpare2()				_ActLed(LED_PNL_TM_Spare2)
#define		PanelSpare3()				_ActLed(LED_PNL_TM_Spare3)
#define		PanelSpare4()				_ActLed(LED_PNL_TM_Spare4)
#define		PanelSpare5()				_ActLed(LED_PNL_TM_Spare5)  
#define		PanelSpare6()				_ActLed(LED_PNL_TM_Spare6)
#define		PanelSpare7()				_ActLed(LED_PNL_TM_Spare7)
#define		PanelSpare8()				_ActLed(LED_PNL_TM_Spare8)
#define		PanelSpare9()				_ActLed(LED_PNL_TM_Spare9)  
#define		PanelSpare10()				_ActLed(LED_PNL_TM_Spare10)
#define		PanelSpare11()				_ActLed(LED_PNL_TM_Spare11)
#define		PanelSpare12()				_ActLed(LED_PNL_TM_Spare12)
#define		PanelSpare13()				_ActLed(LED_PNL_TM_Spare13)  
#define		PanelSpare14()				_ActLed(LED_PNL_TM_Spare14)
#define		PanelSpare15()				_ActLed(LED_PNL_TM_Spare15)
#define		PanelSpare16()				_ActLed(LED_PNL_TM_Spare16)

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct                          // Note the bit field sequence in C is in the reverse order as what in Assembly record
    {
    BYTE    RShift      : 1;            // Bit 0
    BYTE    LShift      : 1;            // Bit 1
    BYTE    Ctrl        : 1;            // Bit 2
    BYTE    Alt         : 1;            // Bit 3
    BYTE    ScrollLock  : 1;            // Bit 4
    BYTE    NumLock     : 1;            // Bit 5
    BYTE    CapsLock    : 1;            // Bit 6
    BYTE    Insert      : 1;            // Bit 7
    } KEYFLAG1;

typedef struct
    {
    BYTE    LCtrl       : 1;
    BYTE    LAlt        : 1;
    BYTE    RCtrl       : 1;
    BYTE    RAlt        : 1;
    BYTE    ScrollLock_ : 1;
    BYTE    NumLock_    : 1;
    BYTE    CapsLock_   : 1;
    BYTE    SysReq      : 1;
    } KEYFLAG2;

typedef struct
    {
    WORD    RShift      : 1;
    WORD    LShift      : 1;
    WORD    Ctrl        : 1;
    WORD    Alt         : 1;
    WORD    ScrollLock  : 1;
    WORD    NumLock     : 1;
    WORD    CapsLock    : 1;
    WORD    Insert      : 1;
    WORD    LCtrl       : 1;
    WORD    LAlt        : 1;
    WORD    RCtrl       : 1;
    WORD    RAlt        : 1;
    WORD    ScrollLock_ : 1;
    WORD    NumLock_    : 1;
    WORD    CapsLock_   : 1;
    WORD    SysReq      : 1;
    } KEYFLAG3;

typedef union
{
    WORD	wCode;
    struct 
    {
	    WORD    ScrollLock  : 1;
	    WORD    NumLock     : 1;
	    WORD    CapsLock    : 1;
	    WORD    RShift      : 1;
	    WORD    LShift      : 1;
	    WORD    Ctrl        : 1;
	    WORD    Alt         : 1;
	    WORD    Insert      : 1;
	    WORD    LCtrl       : 1;
	    WORD    LAlt        : 1;
	    WORD    RCtrl       : 1;
	    WORD    RAlt        : 1;
	    WORD    ScrollLock_ : 1;
	    WORD    NumLock_    : 1;
	    WORD    CapsLock_   : 1;
	    WORD    SysReq      : 1;
	};
}	KEYFLAG;

typedef union
{
    struct
    {
        BYTE        cASCII;
        BYTE        cScan;
    };
    struct
    {
    	WORD        wCode;
    	KEYFLAG		wFlag;
    };
} KEYCODE;

typedef struct tagKB
    {
    BOOL    bPressed;                   // Key was pressed flag     
    BOOL    bRepeat;                    // Start repeat flag        
    short   nDebounce;                  // Delay count for debounce 
    short   nRptDelay;                  // Delay count before repeat
    short   nRptRate;                   // Repeat rate              
    WORD    wIndex;                     // Last keyboard matrix     
    } KB;

typedef struct tagSWITCH
    {
    BOOL    bPressed;
    BOOL    bRepeat;
    short   nDebounce;
    } SWITCH;
    
typedef struct tag_KEYQUEUE
{
    WORD    	wSize;
    WORD 		wEnd;
    WORD 		wInput;
    WORD 		wOutput;
    BOOL    	bFull;
    KEYCODE*	pKeyBuff;
} 	_KEYQUEUE;

typedef     KEYFLAG	*FPKEYFLAG;
typedef     KEYCODE	*FPKEYCODE;

#define		KB_MAX_TOKEN			10
#define		ACT_LED_OFF_SELF_FLAG	0x0100
extern	WORD	_GetKey();
extern	void	_PutKeyTail(WORD keycode);	
extern	WORD	_SetLED(WORD wLEDID);

extern	WORD	_PowerOnLED(WORD wLEDID);
extern	WORD	_PowerOffLED(WORD wLEDID);
extern	WORD	_TestKeyFlag(FPKEYFLAG fpKeyFlag);	
extern	void	_Save_Restore_LED(int nState);
extern	void	_ActLed(int nActionIndex);
extern	void	_UnActLed(int nActionIndex);
extern	int		_GetActLed(int nActionIndex);
extern	int		_GetKeyMap(int nKeyIndex);
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
class	CtmKeyBoard: public	CtmPackClass
{
	friend	WORD	_GetKey();
	friend	void	_PutKeyTail(WORD keycode);	
	friend	WORD	_SetLED(WORD wLEDID);
	
	friend	WORD	_PowerOnLED(WORD wLEDID);
	friend	WORD	_PowerOffLED(WORD wLEDID);
	friend	WORD	_TestKeyFlag(FPKEYFLAG fpKeyFlag);	
	friend	void	_Save_Restore_LED(int nState = 0xFFFF);
	friend	void	_ActLed(int nActionIndex);
	friend	void	_UnActLed(int nActionIndex);
	friend	int		_GetActLed(int nActionIndex);
	friend	int		_GetKeyMap(int nKeyIndex);
	DECLARE_DYNCREATE(CtmKeyBoard)
	public:
		CtmKeyBoard();
		~CtmKeyBoard();
	public:
		virtual	WORD	GetKey()					{return 0;};
		virtual void 	PutKeyTail(WORD keycode)	{};
		virtual	void	SetLED(WORD wLEDID)			{};		
		virtual	WORD 	TestKeyFlag(FPKEYFLAG fpKeyFlag)	{return 0;};
		virtual	void	TestLed(int nState = 0xFFFF){};		
		virtual	void	ActLed(int nActionIndex)	{};
		virtual	void	UnActLed(int nActionIndex)	{};
		virtual	BOOL	GetKeyUp()					{return FALSE;};
	protected:
		virtual	WORD	PowerOnLED(WORD wLEDID)		{return 0;};
		virtual	WORD	PowerOffLED(WORD wLEDID)	{return 0;};
		
		virtual	int		GetActLed(int nActionIndex)	{return -1;};
		virtual	int		GetKeyMap(int nKeyIndex)	{return -1;};
	
};

extern		CtmKeyBoard*	g_ptaskKB;



#endif

