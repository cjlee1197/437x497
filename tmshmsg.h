
#ifndef     D_SH_TMMESSAGE
#define     D_SH_TMMESSAGE

#include	"tmmsg.h"
#define		MSG_USER_DEFINE						0x10000000	//	fans 2008/6/18 10:05 
#define		MSG_SH_HMI_DSP54_START				MSG_USER_DEFINE + 0x0100	//	dsp54 message: >= 0x100 & < 0x1000
#
#define		MSG_SH_HMI_LOGIN_START				MSG_USER_DEFINE + 0x1000	//	login message: >= 0x1000 & < 0x2000
#define		MSG_SH_HMI_CAN_OFFLINE				MSG_USER_DEFINE	+ 0x1001
#define		MSG_SH_HMI_CAN_ONLINE				MSG_USER_DEFINE	+ 0x1002

#define		MSG_USER_SH_MONI_ADD				MSG_USER_DEFINE + 0x0001		//	add monitor2
#define		MSG_USER_SH_ERROR_ADD				MSG_USER_DEFINE + 0x0002		//	add error
#define		MSG_USER_SH_ERROR_FIX				MSG_USER_DEFINE + 0x0003		//	fix error
#define		MSG_USER_SH_REMOTE_KEY				MSG_USER_DEFINE + 0x0004		//	on key
#define		MSG_USER_SH_MACHINEVERSION_DATA		MSG_USER_DEFINE + 0x0005	//	send dsp machine version
#define		MSG_USER_SH_PROMPTERROR				MSG_USER_DEFINE + 0x0006		//	send prompt error
#define		MSG_USER_SH_PROMPTNULL				MSG_USER_DEFINE + 0x0007		// 	send prompt null
#define		MSG_USER_SH_PROMPTSTOPTIMEERROR		MSG_USER_DEFINE + 0x0008		// 	send stopmachine time error
#define		MSG_USER_SH_MONIONLINE_ERROR		MSG_USER_DEFINE + 0x0009		//	Moni Online error
#
//====================================================================
//	dsp54  message
//====================================================================
#define		MSG_SH_HMI_DSP54_CONFIG_START	MSG_SH_HMI_DSP54_START + 0x0000	//	
#define		MSG_SH_HMI_INIT_MOLD_CLOSE		MSG_SH_HMI_DSP54_START + 0x0010	//	close mold
#define		MSG_SH_HMI_INIT_MOLD_OPEN		MSG_SH_HMI_DSP54_START + 0x0011	//	open mold
#define		MSG_SH_HMI_INIT_INJECT			MSG_SH_HMI_DSP54_START + 0x0020	//	inject
#define		MSG_SH_HMI_INIT_HOLDPRESS		MSG_SH_HMI_DSP54_START + 0x0030	//	hold press
#define		MSG_SH_HMI_INIT_EJECT_ADV		MSG_SH_HMI_DSP54_START + 0x0040	//	eject adv
#define		MSG_SH_HMI_INIT_EJECT_RET		MSG_SH_HMI_DSP54_START + 0x0041	//	eject ret
#define		MSG_SH_HMI_INIT_AIR				MSG_SH_HMI_DSP54_START + 0x0050	//	air
#define		MSG_SH_HMI_INIT_CORE			MSG_SH_HMI_DSP54_START + 0x0060	//	core
#define		MSG_SH_HMI_UPDATE_VIEW			MSG_SH_HMI_DSP54_START + 0x0070

//====================================================================
//	Login message
//====================================================================
#define		MSG_SH_HMI_LOGIN_REQUEST		MSG_SH_HMI_LOGIN_START + 0x0001
#define		MSG_SH_LOGIN_RETRUN				MSG_SH_HMI_LOGIN_START + 0x0002
#define		MSG_SH_HMI_LOGOUT_REQUEST		MSG_SH_HMI_LOGIN_START + 0x0003	//WWJ add 2010/7/28 for Logout
#define		MSG_SH_INET_LOGIN_RESTART		MSG_SH_INET_START + 0x0011			//	WWJ	add 2010/08/05 for iNet restart Login
//====================================================================
//	inet message start
//====================================================================
#define		MSG_SH_INET_START					MSG_USER_DEFINE + 0x01000000

//	the common message for inet send and revice; just database
#define		MSG_SH_INET_COMMON_SEND				MSG_SH_INET_START + 0x0001
#define		MSG_SH_INET_COMMON_REV				MSG_SH_INET_START + 0x0002
//	use the global variant
#define		MSG_SH_INET_VARIANT_SEND			MSG_SH_INET_START + 0x0003
#define		MSG_SH_INET_VARIANT_REV				MSG_SH_INET_START + 0x0004
//	use the dynamic record; for example error
#define		MSG_SH_INET_DYREC_SEND				MSG_SH_INET_START + 0x0005
#define		MSG_SH_INET_DYREC_REV				MSG_SH_INET_START + 0x0006
#define		MSG_SH_INET_MULTI_SEND				MSG_SH_INET_START + 0x0007
#define		MSG_SH_INET_MULTI_REV				MSG_SH_INET_START + 0x0008
#define		MSG_SH_INET_SEND_STATUS				MSG_SH_INET_START + 0x0009
#define		MSG_SH_INET_CHANGE_IP				MSG_SH_INET_START + 0x000A
#define		MSG_SH_INET_RESTART					MSG_SH_INET_START + 0x000B
#define		MSG_SH_INET_INJECT_DATA				MSG_SH_INET_START + 0x000C			//James add 2009/9/21 for inject data
#define		MSG_SH_INET_INJECTB_DATA			MSG_SH_INET_START + 0x000D			//James add 2009/9/24 for injectB data
#define		MSG_SH_INET_GPS_DATA_REV			MSG_SH_INET_START + 0x000E			//	fans add 2009-12-22 14:49:30
#define		MSG_SH_INET_REGION_SEND				MSG_SH_INET_START + 0x000F			//	fans add 2009-12-22 14:49:30

#define		MSG_SH_INET_LIMIT_SAVE_REV			MSG_SH_INET_START + 0x0010			//	WWJ	add 2010/08/05 for iNet UpLow Limit
#define		MSG_SH_INET_LOGIN_RESTART			MSG_SH_INET_START + 0x0011			//	WWJ	add 2010/08/05 for iNet restart Login
#define		MSG_SH_INET_PUMPTEST_START			MSG_SH_INET_START + 0x0012			//Caozz 2010/12/9 下午 03:57:40
#define		MSG_SH_INET_PUMPTEST_END			MSG_SH_INET_START + 0x0013
#define		MSG_SH_INET_PUMPTEST_RESULT			MSG_SH_INET_START + 0x0014
#define		MSG_SH_INET_PUMPTEST_INFO			MSG_SH_INET_START + 0x0015
#define		MSG_SH_INET_BADPRODUCT_COUNT		MSG_SH_INET_START + 0x0016			//Caozz 2011/8/11 下午 12:48:52
#define		MSG_SH_INET_SENDMOLDSET				MSG_SH_INET_START + 0x0017
#define 	MSG_SH_INET_SHOTCOUNT				MSG_SH_INET_START + 0x0018
#define		MSG_SH_INET_PARADATAID				MSG_SH_INET_START + 0x0019

//====================================================================
//	dsp54 update message start
//====================================================================
#define		MSG_SH_DSP54UPDATE_START			MSG_SH_INET_START + 0x11000000			//0x01000000
#define		MSG_SH_DSP54UPDATE_FILENOEXIST		MSG_SH_DSP54UPDATE_START + 0x0001
#define		MSG_SH_DSP54UPDATE_DLEND			MSG_SH_DSP54UPDATE_START + 0x0002
#define		MSG_SH_DSP54UPDATE_NOWRITE			MSG_SH_DSP54UPDATE_START + 0x0003
#define		MSG_SH_DSP54UDDATE_TIMEOUT			MSG_SH_DSP54UPDATE_START + 0x0004
#define		MSG_SH_DSP54UPDATE_DLSMALLEND		MSG_SH_DSP54UPDATE_START + 0x0005
#define		MSG_SH_DSP54UPDATE_DLFILEERR		MSG_SH_DSP54UPDATE_START + 0x0006	//daniel add 2010/01/29 for select main or burn err
#define		MSG_SH_DSP54UPDATE_OFFLINE			MSG_SH_DSP54UPDATE_START + 0x0007   //check offline add by Wong 2014/10/25 10:02:25
#define		MSG_SH_DSP54UPDATE_OPERATE			MSG_SH_DSP54UPDATE_START + 0x0008	//check mode

//======================================//
// inet deal the moldset message		//
//======================================//                   
#define		MSG_SH_MOLDSET_START				MSG_SH_INET_START + 0x0100
#define		MSG_SH_INET_DEAL_MOLD				MSG_SH_MOLDSET_START + 0x0001
#define		MSG_SH_REQUEST_INET_MOLD_COUNT		MSG_SH_MOLDSET_START + 0x0002
#define		MSG_SH_REQUEST_INET_MOLD_LIST		MSG_SH_MOLDSET_START + 0x0003
#define		MSG_SH_REQUEST_INET_MOLD			MSG_SH_MOLDSET_START + 0x0004
#define		MSG_SH_INET_MOLD_DEL				MSG_SH_MOLDSET_START + 0x0005
#define		MSG_SH_REV_INET_MOLD_EXIST			MSG_SH_MOLDSET_START + 0x0006
#define		MSG_SH_REV_INET_MOLD_LIMIT			MSG_SH_MOLDSET_START + 0x0007

//======================================//
// inet deal the manufacture message	//
//======================================//
#define		MSG_SH_INET_MANU_START				MSG_SH_INET_START + 0x0200
#define		MSG_SH_INET_MANU_PRODUCE_STATUS		MSG_SH_INET_MANU_START + 0x0001
#define		MSG_SH_INET_MANU_SCHEDULE_STATUS	MSG_SH_INET_MANU_START + 0x0002
#define		MSG_SH_INET_MANU_PRODUCT_COUNT		MSG_SH_INET_MANU_START + 0x0003

//====================================================================
//	Manufacture message start
//====================================================================
#define		MSG_SH_MANUFACTURE_START					MSG_USER_DEFINE + 0x02000000
#define		MSG_SH_MANUFACTURE_SCHEDULE_STATUS			MSG_SH_MANUFACTURE_START + 0x0001
#define		MSG_SH_MANUFACTURE_ACTION_SAVE				MSG_SH_MANUFACTURE_START + 0x0002
#define		MSG_SH_MANUFACTURE_ACTION_DEL				MSG_SH_MANUFACTURE_START + 0x0003
#define		MSG_SH_MANUFACTURE_SHOTCOUNT				MSG_SH_MANUFACTURE_START + 0x0004	//Caozz 2010/9/16 01:35下午
#define		MSG_SH_MANUFACTURE_CONTINUEPROD				MSG_SH_MANUFACTURE_START + 0x0005	//Caozz 2010/9/16 01:35下午
#define		MSG_SH_MANUFACTURE_ENDPROD					MSG_SH_MANUFACTURE_START + 0x0006	//Caozz 2010/9/29 02:28下午

//====================================================================
//	SH Form & Database Message start
//====================================================================
#define		MSG_SH_FORM_START					MSG_USER_DEFINE		+ 0x03000000
#define		MSG_SH_FORM_OVERVIEW_SHOW			MSG_SH_FORM_START	+ 0x0001			//	overview show

#define		MSG_SH_SAVESCREEEN_NOVIEWNAME		MSG_SH_FORM_START	+ 0x0002
#define		MSG_SH_SAVESCREEEN_SAVINGDATA		MSG_SH_FORM_START	+ 0x0003
#define		MSG_SH_SAVESCREEEN_DATASAVE			MSG_SH_FORM_START	+ 0x0004

//*************************************//
#define		MSG_SH_DATABASE						MSG_SH_FORM_START	+ 0x1000			//	fans add 2009-12-23 16:24:08
#define		MSG_SH_SET_DBVALUE					MSG_SH_DATABASE		+ 0x0001			//	set dbvalue message
#define		MSG_SH_GET_DBVALUE					MSG_SH_DATABASE		+ 0x0002			//	get dbvalue message
#define		MSG_SH_SET_DBVALUE_OVERMAX			MSG_SH_DATABASE		+ 0x0003			//	set dbvalue over max message
#define		MSG_SH_SET_DBVALUE_OVERMIN			MSG_SH_DATABASE		+ 0x0004			//	set dbvalue over min message

//====================================================================
//	Austone message start
//====================================================================
#define		MSG_SH_AUSTONE_START				MSG_USER_DEFINE + 0x04000000
#define		MSG_SH_AUSTONE_COMMANDACTUAL		MSG_SH_AUSTONE_START + 0x0000
#define		MSG_SH_AUSTONE_COMMANDREAD			MSG_SH_AUSTONE_START + 0x0001
#define		MSG_SH_AUSTONE_COMMANDWRITE			MSG_SH_AUSTONE_START + 0x0002
#define		MSG_SH_AUSTONE_COMMANDALARM			MSG_SH_AUSTONE_START + 0x0003
#define		MSG_SH_AUSTONE_COMMANDCURVE			MSG_SH_AUSTONE_START + 0x0004
#define		MSG_SH_AUSTONE_COMMANDCMD			MSG_SH_AUSTONE_START + 0x0005
#define		MSG_SH_AUSTONE_COMMANDLINESTATE		MSG_SH_AUSTONE_START + 0x0006
#define		MSG_SH_AUSTONE_COMMANDMOLDSET		MSG_SH_AUSTONE_START + 0x0007
#define		MSG_SH_AUSTONE_COMMANDMOLDSETSTATE	MSG_SH_AUSTONE_START + 0x0008
#define		MSH_SH_AUSTONE_DIFFDATAERROR		MSG_SH_AUSTONE_START + 0x0010

//===================================================================
// DataProcess message start
//===================================================================
#define		MSG_SH_TASKDATA						MSG_USER_DEFINE + 0x05000000

//===================================================================
// Remote message start
//===================================================================
#define		MSG_SH_REMOTE_START					MSG_USER_DEFINE + 0x06000000
#define		MSG_SH_REMOTE_REQ_CONTROL			MSG_SH_REMOTE_START + 0x0001
#define		MSG_SH_REMOTE_REV_CONTROL			MSG_SH_REMOTE_START + 0x0002
#define		MSG_SH_REMOTE_CANCEL_KEY			MSG_SH_REMOTE_START + 0x0003
#define		MSG_SH_REMOTE_HMI_REQUEST			MSG_SH_REMOTE_START + 0x0004
#define		MSG_SH_REMOTE_SEND_STATUS			MSG_SH_REMOTE_START + 0x0005
#define		MSG_SH_REMOTE_NET_WARNING			MSG_SH_REMOTE_START + 0x0006
#define		MSG_SH_REMOTE_NET_RESTART			MSG_SH_REMOTE_START + 0x0007

#define		MSG_SH_ONLINEHELP_REQUEST			MSG_SH_REMOTE_START + 0x0010
#define		MSG_SH_ONLINEHELP_ONCHANGE			MSG_SH_REMOTE_START + 0x0011
#define		MSG_SH_ONLINEHELP_RESULT			MSG_SH_REMOTE_START + 0x0012
#define		MSG_SH_ONLINEHELP_REVIMAGE			MSG_SH_REMOTE_START + 0x0013


#define		MSG_SH_AMMETER_START				MSG_USER_DEFINE + 0x07000000
#define		MSG_SH_AMMETER_COMMERROR			MSG_SH_AMMETER_START + 0x0001
#define		MSG_SH_AMMETER_IDERROR				MSG_SH_AMMETER_START + 0x0002
#define		MSG_SH_AMMETER_CONFIG				MSG_SH_AMMETER_START + 0x0003
#define		MSG_SH_AMMETER_CHECK				MSG_SH_AMMETER_START + 0x0004

#define		MSG_SH_APP_START						MSG_USER_DEFINE + 0x08000000
#define		MSG_SH_APP_TOOLBAR_ONKEY		MSG_SH_APP_START + 0x0001		//TOOLBAR«ö¤U®É §iª¾µe­±°õ¦æ°Ê§@
//#define		MSG_SH_APP_SCROLLBAR				MSG_SH_APP_START + 0x0002		//±²°Ê¤¸¥ó³æ¿W¨Ï¥Î®É °e¥X°T¸¹¨Ó§iª¾µe­±¬O§_±²°Ê
#define		MSG_SH_APP_FORMVIEW					MSG_SH_APP_START + 0x0003		//±qformview.cpp°e¥X¡A°e¦Üµe­±¨Ó°õ¦æ¯S®í³B²z
#define		MSG_SH_APP_CNC_RESET_FLAG		MSG_SH_APP_START + 0x0004		//Reset_Flag
#define		MSG_SH_APP_CREATE_ONKEY			MSG_SH_APP_START + 0x0005		//­n¨DToolBar°õ¦æOnKey

// used by Robot pickercomm 
#define   MSG_SH_PICKER_RECV_FILE_ERR         MSG_SH_AMMETER_START + 0x0010  // @Wang.z.w add 2015/9/30 9:47:43
#define   MSG_SH_PICKER_RECV_FILE_OK          MSG_SH_AMMETER_START + 0x0011  // @Wang.z.w add 2015/9/30 9:47:45
#define   MSG_SH_PICKER_CHECK_MOULDNUM_ERR    MSG_SH_AMMETER_START + 0x0012  // @Wang.z.w add 2015/9/30 9:47:46
#define   MSG_SH_PICKER_CHECK_MOULDNUM_OK     MSG_SH_AMMETER_START + 0x0013  // @Wang.z.w add 2015/10/29 10:00:55

#endif
