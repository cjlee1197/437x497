
#ifndef     D_TMMESSAGE
#define     D_TMMESSAGE
//=======
//		MSG
//=======
#define 	MSG_NULLMSG         0x0000
#define 	MSG_KEYDOWN         0x0010
#define 	MSG_CHAR            0x0011
#define 	MSG_KEYUP           0x0012
#define 	MSG_SYSKEYDOWN      0x0013
#define 	MSG_SYSCHAR         0x0014
#define 	MSG_SYSKEYUP        0x0015
#define		MSG_DSP54_REVRM		0x0016				//fans add 2008/6/16 11:23上午
#define		MSG_INET_LOGIN		0x0017				//Sunny add 2008/8/26 01:58下午
#define		MSG_INET_NETHDRLIST	0x0018				//Sunny add 2008/8/26 01:58下午
#define		MSG_INET_NETMLDCNT	0x0019
#define		MSG_PANEL_KEY				0x001A
#define		MSG_GPIO_READ				0x001B

//	touch Screen message
#define     MSG_MOUSEMOVE       0x0020
#define     MSG_MOUSELCLICK     0x0021
#define     MSG_MOUSEMCLICK     0x0022
#define     MSG_MOUSERCLICK     0x0023
#define     MSG_MOUSELDCLICK    0x0024
#define     MSG_MOUSELDOWN      0x0025
#define     MSG_MOUSEMDOWN      0x0026
#define     MSG_MOUSERDOWN      0x0027
#define     MSG_MOUSELUP        0x0028
#define     MSG_MOUSEMUP        0x0029
#define     MSG_MOUSERUP        0x003A
#define     MSG_MOUSELADJUST    0x003B
#define     MSG_MOUSEEND        0x003C
#define			MSG_BUTTON_DOWN			0x003D
#define			MSG_BUTTON_UP				0x003E

#define 	MSG_QUIT            0x0140
#define 	MSG_TIMER           0x0144
#define		MSG_COM				0x0200
#define		MSG_MMI				0x0300
#define		MSG_RTAI			0x0400		//Mario debug
#define 	MSG_USER            0x0800
#define		MSG_DEMO			0x1000				//<<yang 2006/3/15 
#define		MSG_I32				0x2000				//yang 2006/6/15 
#define		MSG_SIEM			0x4000				//yang 2006/6/15 
#define		MSG_RS232			0x8000				//yang 2006/6/15 
#define		MSG_KEB				0x0100				//yang 2006/12/17

#define		MSG_TEMP			(MSG_USER + 0x10)

#define		MSG_KEY				MSG_KEYDOWN //netdigger 2005/12/3
//===============================================
//	dsp28 message start
//===============================================
#define		MSG_DSP28				0x0200				//James add 2007/11/22 09:13上午
#define		MSG_DSP28_READ_DIAG		0x0201				//	fans add 2008/9/4 10:43上午; for dsp28's diag reading
#define		MSG_DSP28_WRITE_DIAG	0x0202				//	fans add 2008/9/4 10:43上午; for dsp28's diag writing
#define  	MSG_DSP28_WRITE_DATA 	0x0203				//  James add 

//===============================================
//	dsp54_net message start
//===============================================
#define     MSG_DSP54				0x0400

//===============================================
//	dsp54 Message
//===============================================
#define		MSG_DSP54_GET_OPMODE	0x8001				//	fans add 2009/5/15 12:58下午
#define		MSG_DSP54_GET_SHOTCOUNT	0x8002				//	fans add 2009/5/18 10:51上午
#define		MSG_DSP54_GET_CONFIG	0x8003				//	fans add 2009/6/1 02:30下午

//===============================================
//	Storage Message			fans add 2009-4-14 16:38:57	
//===============================================
#define		MSG_STORAGE_START		0x09000			//	Storage Message Starting
#define		MSG_OUTSTORAGE_MOUNT	0x09101			//	outside storage mount message
#define		MSG_OUTSTORAGE_UNMOUNT	0x09102			//	outside	storage umount message
#define		MSG_OUTSTORAGE_NODEVICE	0x09103			//	no outside storage message
#define		MSG_OUTSTORAGE_WERROR	0x09104			//	write error message of the outside storage
#define		MSG_OUTSTORAGE_RERROR	0x09105			//	read error message of the outside storage
#define		MSG_OUTSTORAGE_NOSPACE	0x09106			//	the available space is not enough
//===============================================
//	TempCurve Message		fans add 2009-4-14 16:38:57 
//===============================================
#define		MSG_TEMP_CURVE_START	0x10000
#define		MSG_TEMP_CURVE_SAVING	0x10001			//	the message of saving the temp Curve
#define		MSG_TEMP_CURVE_SAVED	0x10002			//	the message of saved
#define		MSG_TEMP_CURVE_NODEVICE	0x10003			//	

//===============================================
//	MoldSet message			Fans add 2009-4-15 9:35:04
//===============================================
#define		MSG_MOLDSET_TART		0x20000
#define		MSG_MOLDSET_OVERWRITE	0x20001			//	the message of overwriting the mldset
#define		MSG_MOLDSET_ADD			0x20002			//	the message of adding the mold
#define		MSG_MOLDSET_DELETE		0x20003			//	the messege of deleting the mold
#define		MSG_MOLDSET_DEL_CUR		0x20004			//	the message of deleting the current modlset


//===============================================
//	WND Message Start (the message of the component)
//===============================================
#define		MSG_WND_START		0x01000000
#define		MSG_WND_LOSEFOCUS	0x01000001	//	fans add 2009-4-3 12:46:03
#define		MSG_WND_FORM_CREATE	0x01000002	//	form be changed
#define		MSG_WND_UPDATEVIEW	0x01000003
//===============================================
//	record set
//===============================================
#define		MSG_RECORDSET_START	0x01001000
#define		MSG_RECORDSET_SAVE	0x01001001
#define		MSG_RECORDSET_DEL	0x01001002

//===============================================
//	user define message start
//===============================================
#define		MSG_USER_DEFINE		0x10000000	//	fans 2008/6/18 10:05 


//===============================================
//	message for ammeter
//===============================================
#define		MSG_AMMETER			0x80000

#endif
