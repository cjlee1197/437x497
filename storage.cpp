/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : storage 											             |
|  Task     : all storage's write and read			                         |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision : 							                                     |
+===========================================================================*/
#include	<sys/vfs.h>
#include 	<unistd.h>
#include	"tmconfig.h"
#include	"storage.h"
#include	"utils.h"
#include	"tmdebug.h"
#include 	"commonaction.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//WORD	g_wMMCStatus =0;	// For statusbar0.cpp to check if usb/mmc was mounted done.<yuc080118>

typedef void *(* PRSRUN)(void *);
	
/*===========================================================================+
|           Global const	                                                  |
+===========================================================================*/	
#ifdef	D_PXA270
const   char*		DEVICE_MMC				= "/dev/mmca1";		//James modify sda1--->mmca1 2009/3/18 10:37¤W¤È
const	char*		DEVICE_FLASHEX			= "/dev/mtdblock3";
#else
const   char*		DEVICE_MMC				= "/dev/mmcda1";
const	char*		DEVICE_FLASHEX			= "/dev/mtdblock4";
#endif
//	fans modify 2010-5-17 15:41:56
//const   char*		MOUNT_MMCARD_PATH		= "/mnt/mmc";	
const	char*		MOUNT_MMCARD_PATH		= "/mnt/usb";	

#ifdef	D_3354
const   char*       MOUNT_USB_PATH  		= "/media/sda1";
#else
const   char*       MOUNT_USB_PATH  		= "/mnt/usb";
#endif

const   char*		FILESYSTEM_TYPE			= "vfat";

int		g_SramWriteFd 	= -1;
int		g_SramWriteFlag = 0;
//=======
//      Initialize static class member
//=======								
CtmSram*	    CtmSram::m_pInstance        = NULL;
CtmFlashEx*     CtmFlashEx::m_pInstance     = NULL; 
int     		CtmFlashEx::m_nReadFd		= 0;
int				CtmFlashEx::m_nWriteFd		= 0;
unsigned int	CtmFlashEx::m_nPosition		= 0;
int				CtmFlashEx::m_nSize			= 0;
BYTE* 			CtmFlashEx::m_pcBuffer      = NULL;
pthread_mutex_t CtmFlashEx::m_ReadMutex;
pthread_mutex_t CtmFlashEx::m_WriteMutex;
CtmMMCardFile*  CtmMMCardFile::m_pInstance  = NULL;
CtmUsbFile*     CtmUsbFile::m_pInstance     = NULL;
CtmUsbFile*		CtmUsbFile::m_pSelf			= NULL;
int             CtmFlashEx::m_nFlashSize    = 1280 * 1024;
	
pthread_mutex_t	CtmMountFile::m_mutex;
	
BOOL			g_bAutoHotPlug = FALSE;
tmHmiMMCardFile	CtmMMCardFile::m_MMCardFiles[] = 
{
	{ID_MONI_SRAMFILE,		"/mnt/usb/monitor/",	"moni.cdb"},
	//{ID_MONI_SRAMFILE,		"/mnt/nfs/j6/",	"moni.cdb"},
	{ID_ERROR_SRAMFILE,		"/mnt/usb/error/",		"error.cdb"},
	{ID_MOLDSET_SRAMFILE,	"/mnt/usb/moldset/",	"moldset.cdb"},
	{ID_MMI_PROGRAM_FILE,	"/mnt/usb/hmi/",		"mmi"},
	
	{ID_MACHINE_FILE      ,	 "/mnt/usb/machine/",	"machine.cdb"   }, //Sunny<20070911><<<<<
	{ID_MACHINEB_FILE     ,	 "/mnt/usb/machine/",	"machineb.cdb"   },
	{ID_DATABLE_FILE	  ,	 "/mnt/usb/DA/"		,	"da.cdb"},
	{ID_MACHINE_MACH_FILE ,	 "/mnt/usb/machine/",	"machine_mach.cdb" },
	{ID_MACHINEB_MACH_FILE,  "/mnt/usb/machine/",	"machineb_mach.cdb" },
	{ID_DATABLE_MACH_FILE ,	 "/mnt/usb/DA/"		,	"da_mach.cdb"},
	{ID_SYSTEMX_FILE	,	 "/mnt/usb/systemx/",	"systemx.cdb"},
	{ID_USERM_FILE		,	 "/mnt/usb/user/",		"userm.cdb"},
	{ID_DB_SRAMFILE		,	 "/mnt/usb/Default/",   "default.cdb"},
	//{ID_VERSION_FILE	  ,  "/mnt/usb/version/",   "version.cdb"   },
	//{ID_DEFAULTVALUE_FILE ,  "/mnt/usb/defvalue/",  "defvalue.cdb"  },
	//{ID_MINVALUE_FILE	  ,  "/mnt/usb/minvalue/",  "minvalue.cdb"  },  
	//{ID_MAXVALUE_FILE	  ,  "/mnt/usb/maxvalue/",  "maxvalue.cdb"  }, //Sunny<20070911>>>>>>
	    
	{ID_TEMP_CURVE1_FILE  , "/mnt/usb/temp/",       "Temp1.cdb"},  
	{ID_TEMP_CURVE2_FILE  , "/mnt/usb/temp/",       "Temp2.cdb"},  
	{ID_TEMP_CURVE3_FILE  , "/mnt/usb/temp/",       "Temp3.cdb"},  
	{ID_TEMP_CURVE4_FILE  , "/mnt/usb/temp/",       "Temp4.cdb"},  
	{ID_TEMP_CURVE5_FILE  , "/mnt/usb/temp/",       "Temp5.cdb"},  
	{ID_TEMP_CURVE6_FILE  , "/mnt/usb/temp/",       "Temp6.cdb"},  
	{ID_TEMP_CURVE7_FILE  , "/mnt/usb/temp/",       "Temp7.cdb"},  
	{ID_TEMP_CURVE8_FILE  , "/mnt/usb/temp/",       "Temp8.cdb"},  
	{ID_TEMP_CURVE9_FILE  , "/mnt/usb/temp/",       "Temp9.cdb"},  
	{ID_TEMP_CURVE10_FILE , "/mnt/usb/temp/",       "Temp10.cdb"},  
	{ID_TEMP_CURVE11_FILE , "/mnt/usb/temp/",       "Temp11.cdb"},  
	{ID_TEMP_CURVE12_FILE , "/mnt/usb/temp/",       "Temp12.cdb"},  
	{ID_TEMP_CURVE13_FILE , "/mnt/usb/temp/",       "Temp13.cdb"},  
	{ID_TEMP_CURVE14_FILE , "/mnt/usb/temp/",       "Temp14.cdb"},  
	{ID_TEMP_CURVE15_FILE , "/mnt/usb/temp/",       "Temp15.cdb"},  
	{ID_TEMP_CURVE16_FILE , "/mnt/usb/temp/",       "Temp16.cdb"},  
	{ID_TEMP_CURVE17_FILE , "/mnt/usb/temp/",       "Temp17.cdb"},  
	{ID_TEMP_CURVE18_FILE , "/mnt/usb/temp/",       "Temp18.cdb"},  
	{ID_TEMP_CURVE19_FILE , "/mnt/usb/temp/",       "Temp19.cdb"},  
	{ID_TEMP_CURVE20_FILE , "/mnt/usb/temp/",       "Temp20.cdb"},  
	{ID_TEMP_CURVE21_FILE , "/mnt/usb/temp/",       "Temp21.cdb"},  
	{ID_TEMP_CURVE22_FILE , "/mnt/usb/temp/",       "Temp22.cdb"},  
	{ID_TEMP_CURVE23_FILE , "/mnt/usb/temp/",       "Temp23.cdb"},  
	{ID_TEMP_CURVE24_FILE , "/mnt/usb/temp/",       "Temp24.cdb"},
	{ID_TEMP_CURVE25_FILE , "/mnt/usb/temp/",       "Temp25.cdb"},  
	{ID_TEMP_CURVE26_FILE , "/mnt/usb/temp/",       "Temp26.cdb"},  
	{ID_TEMP_CURVE27_FILE , "/mnt/usb/temp/",       "Temp27.cdb"},  
	{ID_TEMP_CURVE28_FILE , "/mnt/usb/temp/",       "Temp28.cdb"},  
	{ID_TEMP_CURVE29_FILE , "/mnt/usb/temp/",       "Temp29.cdb"},  
	{ID_TEMP_CURVE30_FILE , "/mnt/usb/temp/",       "Temp30.cdb"},  
	{ID_TEMP_CURVE31_FILE , "/mnt/usb/temp/",       "Temp31.cdb"},  
	{ID_TEMP_CURVE32_FILE , "/mnt/usb/temp/",       "Temp32.cdb"},  
	{ID_TEMP_CURVE33_FILE , "/mnt/usb/temp/",       "Temp33.cdb"},  
	{ID_TEMP_CURVE34_FILE , "/mnt/usb/temp/",       "Temp34.cdb"},
	{ID_TEMP_CURVE35_FILE , "/mnt/usb/temp/",       "Temp35.cdb"},  
	{ID_TEMP_CURVE36_FILE , "/mnt/usb/temp/",       "Temp36.cdb"},  
	{ID_TEMP_CURVE37_FILE , "/mnt/usb/temp/",       "Temp37.cdb"},  
	{ID_TEMP_CURVE38_FILE , "/mnt/usb/temp/",       "Temp38.cdb"},  
	{ID_TEMP_CURVE39_FILE , "/mnt/usb/temp/",       "Temp39.cdb"},  
	{ID_TEMP_CURVE40_FILE , "/mnt/usb/temp/",       "Temp40.cdb"},  
	{ID_TEMP_CURVE41_FILE , "/mnt/usb/temp/",       "Temp41.cdb"},  
	{ID_TEMP_CURVE42_FILE , "/mnt/usb/temp/",       "Temp42.cdb"},  
	{ID_TEMP_CURVE43_FILE , "/mnt/usb/temp/",       "Temp43.cdb"},  
	{ID_TEMP_CURVE44_FILE , "/mnt/usb/temp/",       "Temp44.cdb"},
	{ID_TEMP_CURVE45_FILE , "/mnt/usb/temp/",       "Temp45.cdb"},  
	{ID_TEMP_CURVE46_FILE , "/mnt/usb/temp/",       "Temp46.cdb"},  
	{ID_TEMP_CURVE47_FILE , "/mnt/usb/temp/",       "Temp47.cdb"},  
	{ID_TEMP_CURVE48_FILE , "/mnt/usb/temp/",       "Temp48.cdb"},  
	{ID_TEMP_CURVE49_FILE , "/mnt/usb/temp/",       "Temp49.cdb"},  
	{ID_TEMP_CURVE50_FILE , "/mnt/usb/temp/",       "Temp50.cdb"},  
	{ID_TEMP_CURVE51_FILE , "/mnt/usb/temp/",       "Temp51.cdb"},  
	{ID_TEMP_CURVE52_FILE , "/mnt/usb/temp/",       "Temp52.cdb"},  
	{ID_TEMP_CURVE53_FILE , "/mnt/usb/temp/",       "Temp53.cdb"},  
	{ID_TEMP_CURVE54_FILE , "/mnt/usb/temp/",       "Temp54.cdb"},
	{ID_TEMP_CURVE55_FILE , "/mnt/usb/temp/",       "Temp55.cdb"},  
	{ID_TEMP_CURVE56_FILE , "/mnt/usb/temp/",       "Temp56.cdb"},  
	{ID_TEMP_CURVE57_FILE , "/mnt/usb/temp/",       "Temp57.cdb"},  
	{ID_TEMP_CURVE58_FILE , "/mnt/usb/temp/",       "Temp58.cdb"},  
	{ID_TEMP_CURVE59_FILE , "/mnt/usb/temp/",       "Temp59.cdb"},  
	{ID_TEMP_CURVE60_FILE , "/mnt/usb/temp/",       "Temp60.cdb"},  
	{ID_TEMP_CURVE61_FILE , "/mnt/usb/temp/",       "Temp61.cdb"},  
	{ID_TEMP_CURVE62_FILE , "/mnt/usb/temp/",       "Temp62.cdb"},  
	{ID_TEMP_CURVE63_FILE , "/mnt/usb/temp/",       "Temp63.cdb"},  
	{ID_TEMP_CURVE64_FILE , "/mnt/usb/temp/",       "Temp64.cdb"},
	{ID_TEMP_MESSAGE_FILE , "/mnt/usb/temp/",       "TempMessage.cdb"},
	{ID_WARM_CURVE1_FILE  , "/mnt/usb/temp/",       "WarmTemp1.cdb"},  
	{ID_WARM_CURVE2_FILE  , "/mnt/usb/temp/",       "WarmTemp2.cdb"},  
	{ID_WARM_CURVE3_FILE  , "/mnt/usb/temp/",       "WarmTemp3.cdb"},  
	{ID_WARM_CURVE4_FILE  , "/mnt/usb/temp/",       "WarmTemp4.cdb"},  
	{ID_WARM_CURVE5_FILE  , "/mnt/usb/temp/",       "WarmTemp5.cdb"},  
	{ID_WARM_CURVE6_FILE  , "/mnt/usb/temp/",       "WarmTemp6.cdb"},  
	{ID_WARM_CURVE7_FILE  , "/mnt/usb/temp/",       "WarmTemp7.cdb"},  
	{ID_WARM_CURVE8_FILE  , "/mnt/usb/temp/",       "WarmTemp8.cdb"},  
	{ID_WARM_CURVE9_FILE  , "/mnt/usb/temp/",       "WarmTemp9.cdb"},  
	{ID_WARM_CURVE10_FILE , "/mnt/usb/temp/",       "WarmTemp10.cdb"},  
	{ID_WARM_CURVE11_FILE , "/mnt/usb/temp/",       "WarmTemp11.cdb"},  
	{ID_WARM_CURVE12_FILE , "/mnt/usb/temp/",       "WarmTemp12.cdb"},  
	{ID_WARM_CURVE13_FILE , "/mnt/usb/temp/",       "WarmTemp13.cdb"},  
	{ID_WARM_CURVE14_FILE , "/mnt/usb/temp/",       "WarmTemp14.cdb"},  
	{ID_WARM_CURVE15_FILE , "/mnt/usb/temp/",       "WarmTemp15.cdb"},  
	{ID_WARM_CURVE16_FILE , "/mnt/usb/temp/",       "WarmTemp16.cdb"},  
	{ID_WARM_CURVE17_FILE , "/mnt/usb/temp/",       "WarmTemp17.cdb"},  
	{ID_WARM_CURVE18_FILE , "/mnt/usb/temp/",       "WarmTemp18.cdb"},  
	{ID_WARM_CURVE19_FILE , "/mnt/usb/temp/",       "WarmTemp19.cdb"},  
	{ID_WARM_CURVE20_FILE , "/mnt/usb/temp/",       "WarmTemp20.cdb"},  
	{ID_WARM_CURVE21_FILE  , "/mnt/usb/temp/",       "WarmTemp21.cdb"},  
	{ID_WARM_CURVE22_FILE  , "/mnt/usb/temp/",       "WarmTemp22.cdb"},  
	{ID_WARM_CURVE23_FILE  , "/mnt/usb/temp/",       "WarmTemp23.cdb"},  
	{ID_WARM_CURVE24_FILE  , "/mnt/usb/temp/",       "WarmTemp24.cdb"},  
	{ID_WARM_CURVE25_FILE  , "/mnt/usb/temp/",       "WarmTemp25.cdb"},  
	{ID_WARM_CURVE26_FILE  , "/mnt/usb/temp/",       "WarmTemp26.cdb"},  
	{ID_WARM_CURVE27_FILE  , "/mnt/usb/temp/",       "WarmTemp27.cdb"},  
	{ID_WARM_CURVE28_FILE  , "/mnt/usb/temp/",       "WarmTemp28.cdb"},  
	{ID_WARM_CURVE29_FILE  , "/mnt/usb/temp/",       "WarmTemp29.cdb"},  
	{ID_WARM_CURVE30_FILE , "/mnt/usb/temp/",       "WarmTemp30.cdb"}, 
	{ID_WARM_CURVE31_FILE , "/mnt/usb/temp/",       "WarmTemp31.cdb"}, 
	{ID_WARM_CURVE32_FILE , "/mnt/usb/temp/",       "WarmTemp32.cdb"}, 
	{ID_WARM_CURVE33_FILE , "/mnt/usb/temp/",       "WarmTemp33.cdb"}, 
	{ID_WARM_CURVE34_FILE , "/mnt/usb/temp/",       "WarmTemp34.cdb"}, 
	{ID_WARM_CURVE35_FILE , "/mnt/usb/temp/",       "WarmTemp35.cdb"}, 
	{ID_WARM_CURVE36_FILE , "/mnt/usb/temp/",       "WarmTemp36.cdb"}, 
	{ID_WARM_CURVE37_FILE , "/mnt/usb/temp/",       "WarmTemp37.cdb"}, 
	{ID_WARM_CURVE38_FILE , "/mnt/usb/temp/",       "WarmTemp38.cdb"}, 
	{ID_WARM_CURVE39_FILE , "/mnt/usb/temp/",       "WarmTemp39.cdb"}, 
	{ID_WARM_CURVE40_FILE , "/mnt/usb/temp/",       "WarmTemp40.cdb"}, 
	{ID_WARM_CURVE41_FILE  , "/mnt/usb/temp/",       "WarmTemp41.cdb"},  
	{ID_WARM_CURVE42_FILE  , "/mnt/usb/temp/",       "WarmTemp42.cdb"},  
	{ID_WARM_CURVE43_FILE  , "/mnt/usb/temp/",       "WarmTemp43.cdb"},  
	{ID_WARM_CURVE44_FILE  , "/mnt/usb/temp/",       "WarmTemp44.cdb"},  
	{ID_WARM_CURVE45_FILE  , "/mnt/usb/temp/",       "WarmTemp45.cdb"},  
	{ID_WARM_CURVE46_FILE  , "/mnt/usb/temp/",       "WarmTemp46.cdb"},  
	{ID_WARM_CURVE47_FILE  , "/mnt/usb/temp/",       "WarmTemp47.cdb"},  
	{ID_WARM_CURVE48_FILE  , "/mnt/usb/temp/",       "WarmTemp48.cdb"},  
	{ID_WARM_CURVE49_FILE  , "/mnt/usb/temp/",       "WarmTemp49.cdb"},  
	{ID_WARM_CURVE50_FILE , "/mnt/usb/temp/",       "WarmTemp50.cdb"}, 
	{ID_WARM_CURVE51_FILE , "/mnt/usb/temp/",       "WarmTemp51.cdb"}, 
	{ID_WARM_CURVE52_FILE , "/mnt/usb/temp/",       "WarmTemp52.cdb"}, 
	{ID_WARM_CURVE53_FILE , "/mnt/usb/temp/",       "WarmTemp53.cdb"}, 
	{ID_WARM_CURVE54_FILE , "/mnt/usb/temp/",       "WarmTemp54.cdb"}, 
	{ID_WARM_CURVE55_FILE , "/mnt/usb/temp/",       "WarmTemp55.cdb"}, 
	{ID_WARM_CURVE56_FILE , "/mnt/usb/temp/",       "WarmTemp56.cdb"}, 
	{ID_WARM_CURVE57_FILE , "/mnt/usb/temp/",       "WarmTemp57.cdb"}, 
	{ID_WARM_CURVE58_FILE , "/mnt/usb/temp/",       "WarmTemp58.cdb"}, 
	{ID_WARM_CURVE59_FILE , "/mnt/usb/temp/",       "WarmTemp59.cdb"}, 
	{ID_WARM_CURVE60_FILE , "/mnt/usb/temp/",       "WarmTemp60.cdb"}, 
	{ID_WARM_CURVE61_FILE  , "/mnt/usb/temp/",       "WarmTemp61.cdb"},  
	{ID_WARM_CURVE62_FILE  , "/mnt/usb/temp/",       "WarmTemp62.cdb"},  
	{ID_WARM_CURVE63_FILE  , "/mnt/usb/temp/",       "WarmTemp63.cdb"},  
	{ID_WARM_CURVE64_FILE  , "/mnt/usb/temp/",       "WarmTemp64.cdb"}, 
	{ID_WARM_MESSAGE_FILE , "/mnt/usb/temp/",       "WarmMessage.cdb"},
	{ID_TEMP_SET_FILE     , "/mnt/usb/temp/",       "TempSet.cdb"}
	
};
	
tmHMIDiskFile	CtmDiskFile::m_DiskFiles[] = 
{
	{ID_MONI_SRAMFILE,		"/usr/Data/moni/"		,	"moni.cdb"},
	{ID_ERROR_SRAMFILE,		"/usr/Data/error/"		,	"error.cdb"},
	{ID_MOLDSET_SRAMFILE,	"/usr/Data/moldset/"	,	"moldset.cdb"},
	{ID_MACHINE_FILE,		"/usr/Data/machine/"	,	"machine.cdb"},
	{ID_MACHINEB_FILE,		"/usr/Data/machine/"	,	"machineb.cdb"},
	{ID_DATABLE_FILE	,	"/usr/Data/DA/"			,	"da.cdb"},
	{ID_MACHINE_MACH_FILE ,	"/usr/Data/machine/"	,	"machine_mach.cdb" },
	{ID_MACHINEB_MACH_FILE,	"/usr/Data/machine/"	,	"machineb_mach.cdb" },
	{ID_DATABLE_MACH_FILE ,	"/usr/Data/DA/"			,	"da_mach.cdb"},
	{ID_SYSTEMX_FILE	,	"/usr/Data/systemx/"	,	"systemx.cdb"},
	{ID_USERM_FILE		,	"/usr/Data/user/"		,	"userm.cdb"},
	{ID_INSTALL_FILE	,	"/usr/Data/Install/"	,	"Install.cdb"},
	{ID_ZEROSET_FILE,	    "/usr/Data/ZeroSet/" 	,	"zeroset.cdb"},
	{ID_MMI_PROGRAM_FILE,	NULL					,	"mmi"},
	{ID_DB_SRAMFILE	,	 	"/usr/Data/Default/"	,	"default.cdb"}
};

char*   CtmUsbFile::m_pszUsbDev[] = 
{

	#ifdef   D_ARM
 	"/dev/sda1",
    "/dev/sda"
	#else
	"/dev/sdb1",
	"/dev/sdb",
    "/dev/sdc1",
    "/dev/sdc",
 	"/dev/sda1",
    "/dev/sda"
	#endif	
//    "/dev/sda1",
//    "/dev/sda"
//    "/dev/sdb1",
//    "/dev/sdc1",
//    "/dev/sdd1"
};


tmHMIUsbFile	CtmUsbFile::m_UsbFiles[] = 
{
	#ifdef	D_3354
	{ID_MONI_SRAMFILE,		"/media/sda1/monitor/",	"moni.cdb"},
	{ID_ERROR_SRAMFILE,		"/media/sda1/error/",		"error.cdb"},
	{ID_MOLDSET_SRAMFILE,	"/media/sda1/moldset/",	"moldset.cdb"},
	{ID_MMI_PROGRAM_FILE,	"/media/sda1/hmi/",		"mmi"},
	
	{ID_MACHINE_FILE      ,	 "/media/sda1/machine/",	"machine.cdb"   }, //Sunny<20070911><<<<<
	{ID_MACHINEB_FILE      ,	 "/media/sda1/machine/",	"machineb.cdb"   },
	{ID_DATABLE_FILE	,	 "/media/sda1/DA/"		,	"da.cdb"},
	{ID_MACHINE_MACH_FILE ,	 "/media/sda1/machine/",	"machine_mach.cdb" },
	{ID_MACHINEB_MACH_FILE,	 "/media/sda1/machine/",	"machineb_mach.cdb" },
	{ID_DATABLE_MACH_FILE ,	 "/media/sda1/DA/"		,	"da_mach.cdb"},
	{ID_SYSTEMX_FILE	,	 "/media/sda1/systemx/",	"systemx.cdb"},
	{ID_USERM_FILE		,	 "/media/sda1/user/",		"userm.cdb"},
	{ID_DB_SRAMFILE		,	 "/media/sda1/Default/",   "default.cdb"},
	                         
	{ID_TEMP_CURVE1_FILE  , "/media/sda1/temp/",       "Temp1.cdb"},  
	{ID_TEMP_CURVE2_FILE  , "/media/sda1/temp/",       "Temp2.cdb"},  
	{ID_TEMP_CURVE3_FILE  , "/media/sda1/temp/",       "Temp3.cdb"},  
	{ID_TEMP_CURVE4_FILE  , "/media/sda1/temp/",       "Temp4.cdb"},  
	{ID_TEMP_CURVE5_FILE  , "/media/sda1/temp/",       "Temp5.cdb"},  
	{ID_TEMP_CURVE6_FILE  , "/media/sda1/temp/",       "Temp6.cdb"},  
	{ID_TEMP_CURVE7_FILE  , "/media/sda1/temp/",       "Temp7.cdb"},  
	{ID_TEMP_CURVE8_FILE  , "/media/sda1/temp/",       "Temp8.cdb"},  
	{ID_TEMP_CURVE9_FILE  , "/media/sda1/temp/",       "Temp9.cdb"},  
	{ID_TEMP_CURVE10_FILE , "/media/sda1/temp/",       "Temp10.cdb"},  
	{ID_TEMP_CURVE11_FILE , "/media/sda1/temp/",       "Temp11.cdb"},  
	{ID_TEMP_CURVE12_FILE , "/media/sda1/temp/",       "Temp12.cdb"},  
	{ID_TEMP_CURVE13_FILE , "/media/sda1/temp/",       "Temp13.cdb"},  
	{ID_TEMP_CURVE14_FILE , "/media/sda1/temp/",       "Temp14.cdb"},  
	{ID_TEMP_CURVE15_FILE , "/media/sda1/temp/",       "Temp15.cdb"},  
	{ID_TEMP_CURVE16_FILE , "/media/sda1/temp/",       "Temp16.cdb"},  
	{ID_TEMP_CURVE17_FILE , "/media/sda1/temp/",       "Temp17.cdb"},  
	{ID_TEMP_CURVE18_FILE , "/media/sda1/temp/",       "Temp18.cdb"},  
	{ID_TEMP_CURVE19_FILE , "/media/sda1/temp/",       "Temp19.cdb"},  
	{ID_TEMP_CURVE20_FILE , "/media/sda1/temp/",       "Temp20.cdb"},  
	{ID_TEMP_CURVE21_FILE , "/media/sda1/temp/",       "Temp21.cdb"},  
	{ID_TEMP_CURVE22_FILE , "/media/sda1/temp/",       "Temp22.cdb"},  
	{ID_TEMP_CURVE23_FILE , "/media/sda1/temp/",       "Temp23.cdb"},  
	{ID_TEMP_CURVE24_FILE , "/media/sda1/temp/",       "Temp24.cdb"},
	{ID_TEMP_CURVE25_FILE , "/media/sda1/temp/",       "Temp25.cdb"},  
	{ID_TEMP_CURVE26_FILE , "/media/sda1/temp/",       "Temp26.cdb"},  
	{ID_TEMP_CURVE27_FILE , "/media/sda1/temp/",       "Temp27.cdb"},  
	{ID_TEMP_CURVE28_FILE , "/media/sda1/temp/",       "Temp28.cdb"},  
	{ID_TEMP_CURVE29_FILE , "/media/sda1/temp/",       "Temp29.cdb"},  
	{ID_TEMP_CURVE30_FILE , "/media/sda1/temp/",       "Temp30.cdb"},  
	{ID_TEMP_CURVE31_FILE , "/media/sda1/temp/",       "Temp31.cdb"},  
	{ID_TEMP_CURVE32_FILE , "/media/sda1/temp/",       "Temp32.cdb"},  
	{ID_TEMP_CURVE33_FILE , "/media/sda1/temp/",       "Temp33.cdb"},  
	{ID_TEMP_CURVE34_FILE , "/media/sda1/temp/",       "Temp34.cdb"},
	{ID_TEMP_CURVE35_FILE , "/media/sda1/temp/",       "Temp35.cdb"},  
	{ID_TEMP_CURVE36_FILE , "/media/sda1/temp/",       "Temp36.cdb"},  
	{ID_TEMP_CURVE37_FILE , "/media/sda1/temp/",       "Temp37.cdb"},  
	{ID_TEMP_CURVE38_FILE , "/media/sda1/temp/",       "Temp38.cdb"},  
	{ID_TEMP_CURVE39_FILE , "/media/sda1/temp/",       "Temp39.cdb"},  
	{ID_TEMP_CURVE40_FILE , "/media/sda1/temp/",       "Temp40.cdb"},  
	{ID_TEMP_CURVE41_FILE , "/media/sda1/temp/",       "Temp41.cdb"},  
	{ID_TEMP_CURVE42_FILE , "/media/sda1/temp/",       "Temp42.cdb"},  
	{ID_TEMP_CURVE43_FILE , "/media/sda1/temp/",       "Temp43.cdb"},  
	{ID_TEMP_CURVE44_FILE , "/media/sda1/temp/",       "Temp44.cdb"},
	{ID_TEMP_CURVE45_FILE , "/media/sda1/temp/",       "Temp45.cdb"},  
	{ID_TEMP_CURVE46_FILE , "/media/sda1/temp/",       "Temp46.cdb"},  
	{ID_TEMP_CURVE47_FILE , "/media/sda1/temp/",       "Temp47.cdb"},  
	{ID_TEMP_CURVE48_FILE , "/media/sda1/temp/",       "Temp48.cdb"},  
	{ID_TEMP_CURVE49_FILE , "/media/sda1/temp/",       "Temp49.cdb"},  
	{ID_TEMP_CURVE50_FILE , "/media/sda1/temp/",       "Temp50.cdb"},  
	{ID_TEMP_CURVE51_FILE , "/media/sda1/temp/",       "Temp51.cdb"},  
	{ID_TEMP_CURVE52_FILE , "/media/sda1/temp/",       "Temp52.cdb"},  
	{ID_TEMP_CURVE53_FILE , "/media/sda1/temp/",       "Temp53.cdb"},  
	{ID_TEMP_CURVE54_FILE , "/media/sda1/temp/",       "Temp54.cdb"},
	{ID_TEMP_CURVE55_FILE , "/media/sda1/temp/",       "Temp55.cdb"},  
	{ID_TEMP_CURVE56_FILE , "/media/sda1/temp/",       "Temp56.cdb"},  
	{ID_TEMP_CURVE57_FILE , "/media/sda1/temp/",       "Temp57.cdb"},  
	{ID_TEMP_CURVE58_FILE , "/media/sda1/temp/",       "Temp58.cdb"},  
	{ID_TEMP_CURVE59_FILE , "/media/sda1/temp/",       "Temp59.cdb"},  
	{ID_TEMP_CURVE60_FILE , "/media/sda1/temp/",       "Temp60.cdb"},  
	{ID_TEMP_CURVE61_FILE , "/media/sda1/temp/",       "Temp61.cdb"},  
	{ID_TEMP_CURVE62_FILE , "/media/sda1/temp/",       "Temp62.cdb"},  
	{ID_TEMP_CURVE63_FILE , "/media/sda1/temp/",       "Temp63.cdb"},  
	{ID_TEMP_CURVE64_FILE , "/media/sda1/temp/",       "Temp64.cdb"},
	{ID_TEMP_MESSAGE_FILE , "/media/sda1/temp/",       "TempMessage.cdb"},
	{ID_WARM_CURVE1_FILE  , "/media/sda1/temp/",       "WarmTemp1.cdb"},  
	{ID_WARM_CURVE2_FILE  , "/media/sda1/temp/",       "WarmTemp2.cdb"},  
	{ID_WARM_CURVE3_FILE  , "/media/sda1/temp/",       "WarmTemp3.cdb"},  
	{ID_WARM_CURVE4_FILE  , "/media/sda1/temp/",       "WarmTemp4.cdb"},  
	{ID_WARM_CURVE5_FILE  , "/media/sda1/temp/",       "WarmTemp5.cdb"},  
	{ID_WARM_CURVE6_FILE  , "/media/sda1/temp/",       "WarmTemp6.cdb"},  
	{ID_WARM_CURVE7_FILE  , "/media/sda1/temp/",       "WarmTemp7.cdb"},  
	{ID_WARM_CURVE8_FILE  , "/media/sda1/temp/",       "WarmTemp8.cdb"},  
	{ID_WARM_CURVE9_FILE  , "/media/sda1/temp/",       "WarmTemp9.cdb"},  
	{ID_WARM_CURVE10_FILE , "/media/sda1/temp/",       "WarmTemp10.cdb"},  
	{ID_WARM_CURVE11_FILE , "/media/sda1/temp/",       "WarmTemp11.cdb"},  
	{ID_WARM_CURVE12_FILE , "/media/sda1/temp/",       "WarmTemp12.cdb"},  
	{ID_WARM_CURVE13_FILE , "/media/sda1/temp/",       "WarmTemp13.cdb"},  
	{ID_WARM_CURVE14_FILE , "/media/sda1/temp/",       "WarmTemp14.cdb"},  
	{ID_WARM_CURVE15_FILE , "/media/sda1/temp/",       "WarmTemp15.cdb"},  
	{ID_WARM_CURVE16_FILE , "/media/sda1/temp/",       "WarmTemp16.cdb"},  
	{ID_WARM_CURVE17_FILE , "/media/sda1/temp/",       "WarmTemp17.cdb"},  
	{ID_WARM_CURVE18_FILE , "/media/sda1/temp/",       "WarmTemp18.cdb"},  
	{ID_WARM_CURVE19_FILE , "/media/sda1/temp/",       "WarmTemp19.cdb"},  
	{ID_WARM_CURVE20_FILE , "/media/sda1/temp/",       "WarmTemp20.cdb"},  
	{ID_WARM_CURVE21_FILE  , "/media/sda1/temp/",       "WarmTemp21.cdb"},  
	{ID_WARM_CURVE22_FILE  , "/media/sda1/temp/",       "WarmTemp22.cdb"},  
	{ID_WARM_CURVE23_FILE  , "/media/sda1/temp/",       "WarmTemp23.cdb"},  
	{ID_WARM_CURVE24_FILE  , "/media/sda1/temp/",       "WarmTemp24.cdb"},  
	{ID_WARM_CURVE25_FILE  , "/media/sda1/temp/",       "WarmTemp25.cdb"},  
	{ID_WARM_CURVE26_FILE  , "/media/sda1/temp/",       "WarmTemp26.cdb"},  
	{ID_WARM_CURVE27_FILE  , "/media/sda1/temp/",       "WarmTemp27.cdb"},  
	{ID_WARM_CURVE28_FILE  , "/media/sda1/temp/",       "WarmTemp28.cdb"},  
	{ID_WARM_CURVE29_FILE  , "/media/sda1/temp/",       "WarmTemp29.cdb"},  
	{ID_WARM_CURVE30_FILE , "/media/sda1/temp/",       "WarmTemp30.cdb"}, 
	{ID_WARM_CURVE31_FILE , "/media/sda1/temp/",       "WarmTemp31.cdb"}, 
	{ID_WARM_CURVE32_FILE , "/media/sda1/temp/",       "WarmTemp32.cdb"}, 
	{ID_WARM_CURVE33_FILE , "/media/sda1/temp/",       "WarmTemp33.cdb"}, 
	{ID_WARM_CURVE34_FILE , "/media/sda1/temp/",       "WarmTemp34.cdb"}, 
	{ID_WARM_CURVE35_FILE , "/media/sda1/temp/",       "WarmTemp35.cdb"}, 
	{ID_WARM_CURVE36_FILE , "/media/sda1/temp/",       "WarmTemp36.cdb"}, 
	{ID_WARM_CURVE37_FILE , "/media/sda1/temp/",       "WarmTemp37.cdb"}, 
	{ID_WARM_CURVE38_FILE , "/media/sda1/temp/",       "WarmTemp38.cdb"}, 
	{ID_WARM_CURVE39_FILE , "/media/sda1/temp/",       "WarmTemp39.cdb"}, 
	{ID_WARM_CURVE40_FILE , "/media/sda1/temp/",       "WarmTemp40.cdb"}, 
	{ID_WARM_CURVE41_FILE  , "/media/sda1/temp/",       "WarmTemp41.cdb"},  
	{ID_WARM_CURVE42_FILE  , "/media/sda1/temp/",       "WarmTemp42.cdb"},  
	{ID_WARM_CURVE43_FILE  , "/media/sda1/temp/",       "WarmTemp43.cdb"},  
	{ID_WARM_CURVE44_FILE  , "/media/sda1/temp/",       "WarmTemp44.cdb"},  
	{ID_WARM_CURVE45_FILE  , "/media/sda1/temp/",       "WarmTemp45.cdb"},  
	{ID_WARM_CURVE46_FILE  , "/media/sda1/temp/",       "WarmTemp46.cdb"},  
	{ID_WARM_CURVE47_FILE  , "/media/sda1/temp/",       "WarmTemp47.cdb"},  
	{ID_WARM_CURVE48_FILE  , "/media/sda1/temp/",       "WarmTemp48.cdb"},  
	{ID_WARM_CURVE49_FILE  , "/media/sda1/temp/",       "WarmTemp49.cdb"},  
	{ID_WARM_CURVE50_FILE , "/media/sda1/temp/",       "WarmTemp50.cdb"}, 
	{ID_WARM_CURVE51_FILE , "/media/sda1/temp/",       "WarmTemp51.cdb"}, 
	{ID_WARM_CURVE52_FILE , "/media/sda1/temp/",       "WarmTemp52.cdb"}, 
	{ID_WARM_CURVE53_FILE , "/media/sda1/temp/",       "WarmTemp53.cdb"}, 
	{ID_WARM_CURVE54_FILE , "/media/sda1/temp/",       "WarmTemp54.cdb"}, 
	{ID_WARM_CURVE55_FILE , "/media/sda1/temp/",       "WarmTemp55.cdb"}, 
	{ID_WARM_CURVE56_FILE , "/media/sda1/temp/",       "WarmTemp56.cdb"}, 
	{ID_WARM_CURVE57_FILE , "/media/sda1/temp/",       "WarmTemp57.cdb"}, 
	{ID_WARM_CURVE58_FILE , "/media/sda1/temp/",       "WarmTemp58.cdb"}, 
	{ID_WARM_CURVE59_FILE , "/media/sda1/temp/",       "WarmTemp59.cdb"}, 
	{ID_WARM_CURVE60_FILE , "/media/sda1/temp/",       "WarmTemp60.cdb"}, 
	{ID_WARM_CURVE61_FILE  , "/media/sda1/temp/",       "WarmTemp61.cdb"},  
	{ID_WARM_CURVE62_FILE  , "/media/sda1/temp/",       "WarmTemp62.cdb"},  
	{ID_WARM_CURVE63_FILE  , "/media/sda1/temp/",       "WarmTemp63.cdb"},  
	{ID_WARM_CURVE64_FILE  , "/media/sda1/temp/",       "WarmTemp64.cdb"}, 
	{ID_WARM_MESSAGE_FILE , "/media/sda1/temp/",       "WarmMessage.cdb"},
	{ID_TEMP_SET_FILE     , "/media/sda1/temp/",       "TempSet.cdb"}
	#else
	{ID_MONI_SRAMFILE,		"/mnt/usb/monitor/",	"moni.cdb"},
	{ID_ERROR_SRAMFILE,		"/mnt/usb/error/",		"error.cdb"},
	{ID_MOLDSET_SRAMFILE,	"/mnt/usb/moldset/",	"moldset.cdb"},
	{ID_MMI_PROGRAM_FILE,	"/mnt/usb/hmi/",		"mmi"},
	
	{ID_MACHINE_FILE      ,	 "/mnt/usb/machine/",	"machine.cdb"   }, //Sunny<20070911><<<<<
	{ID_MACHINEB_FILE      ,	 "/mnt/usb/machine/",	"machineb.cdb"   },
	{ID_DATABLE_FILE	,	 "/mnt/usb/DA/"		,	"da.cdb"},
	{ID_MACHINE_MACH_FILE ,	 "/mnt/usb/machine/",	"machine_mach.cdb" },
	{ID_MACHINEB_MACH_FILE,	 "/mnt/usb/machine/",	"machineb_mach.cdb" },
	{ID_DATABLE_MACH_FILE ,	 "/mnt/usb/DA/"		,	"da_mach.cdb"},
	{ID_SYSTEMX_FILE	,	 "/mnt/usb/systemx/",	"systemx.cdb"},
	{ID_USERM_FILE		,	 "/mnt/usb/user/",		"userm.cdb"},
	{ID_DB_SRAMFILE		,	 "/mnt/usb/Default/",   "default.cdb"},
	                         
	{ID_TEMP_CURVE1_FILE  , "/mnt/usb/temp/",       "Temp1.cdb"},  
	{ID_TEMP_CURVE2_FILE  , "/mnt/usb/temp/",       "Temp2.cdb"},  
	{ID_TEMP_CURVE3_FILE  , "/mnt/usb/temp/",       "Temp3.cdb"},  
	{ID_TEMP_CURVE4_FILE  , "/mnt/usb/temp/",       "Temp4.cdb"},  
	{ID_TEMP_CURVE5_FILE  , "/mnt/usb/temp/",       "Temp5.cdb"},  
	{ID_TEMP_CURVE6_FILE  , "/mnt/usb/temp/",       "Temp6.cdb"},  
	{ID_TEMP_CURVE7_FILE  , "/mnt/usb/temp/",       "Temp7.cdb"},  
	{ID_TEMP_CURVE8_FILE  , "/mnt/usb/temp/",       "Temp8.cdb"},  
	{ID_TEMP_CURVE9_FILE  , "/mnt/usb/temp/",       "Temp9.cdb"},  
	{ID_TEMP_CURVE10_FILE , "/mnt/usb/temp/",       "Temp10.cdb"},  
	{ID_TEMP_CURVE11_FILE , "/mnt/usb/temp/",       "Temp11.cdb"},  
	{ID_TEMP_CURVE12_FILE , "/mnt/usb/temp/",       "Temp12.cdb"},  
	{ID_TEMP_CURVE13_FILE , "/mnt/usb/temp/",       "Temp13.cdb"},  
	{ID_TEMP_CURVE14_FILE , "/mnt/usb/temp/",       "Temp14.cdb"},  
	{ID_TEMP_CURVE15_FILE , "/mnt/usb/temp/",       "Temp15.cdb"},  
	{ID_TEMP_CURVE16_FILE , "/mnt/usb/temp/",       "Temp16.cdb"},  
	{ID_TEMP_CURVE17_FILE , "/mnt/usb/temp/",       "Temp17.cdb"},  
	{ID_TEMP_CURVE18_FILE , "/mnt/usb/temp/",       "Temp18.cdb"},  
	{ID_TEMP_CURVE19_FILE , "/mnt/usb/temp/",       "Temp19.cdb"},  
	{ID_TEMP_CURVE20_FILE , "/mnt/usb/temp/",       "Temp20.cdb"},  
	{ID_TEMP_CURVE21_FILE , "/mnt/usb/temp/",       "Temp21.cdb"},  
	{ID_TEMP_CURVE22_FILE , "/mnt/usb/temp/",       "Temp22.cdb"},  
	{ID_TEMP_CURVE23_FILE , "/mnt/usb/temp/",       "Temp23.cdb"},  
	{ID_TEMP_CURVE24_FILE , "/mnt/usb/temp/",       "Temp24.cdb"},
	{ID_TEMP_CURVE25_FILE , "/mnt/usb/temp/",       "Temp25.cdb"},  
	{ID_TEMP_CURVE26_FILE , "/mnt/usb/temp/",       "Temp26.cdb"},  
	{ID_TEMP_CURVE27_FILE , "/mnt/usb/temp/",       "Temp27.cdb"},  
	{ID_TEMP_CURVE28_FILE , "/mnt/usb/temp/",       "Temp28.cdb"},  
	{ID_TEMP_CURVE29_FILE , "/mnt/usb/temp/",       "Temp29.cdb"},  
	{ID_TEMP_CURVE30_FILE , "/mnt/usb/temp/",       "Temp30.cdb"},  
	{ID_TEMP_CURVE31_FILE , "/mnt/usb/temp/",       "Temp31.cdb"},  
	{ID_TEMP_CURVE32_FILE , "/mnt/usb/temp/",       "Temp32.cdb"},  
	{ID_TEMP_CURVE33_FILE , "/mnt/usb/temp/",       "Temp33.cdb"},  
	{ID_TEMP_CURVE34_FILE , "/mnt/usb/temp/",       "Temp34.cdb"},
	{ID_TEMP_CURVE35_FILE , "/mnt/usb/temp/",       "Temp35.cdb"},  
	{ID_TEMP_CURVE36_FILE , "/mnt/usb/temp/",       "Temp36.cdb"},  
	{ID_TEMP_CURVE37_FILE , "/mnt/usb/temp/",       "Temp37.cdb"},  
	{ID_TEMP_CURVE38_FILE , "/mnt/usb/temp/",       "Temp38.cdb"},  
	{ID_TEMP_CURVE39_FILE , "/mnt/usb/temp/",       "Temp39.cdb"},  
	{ID_TEMP_CURVE40_FILE , "/mnt/usb/temp/",       "Temp40.cdb"},  
	{ID_TEMP_CURVE41_FILE , "/mnt/usb/temp/",       "Temp41.cdb"},  
	{ID_TEMP_CURVE42_FILE , "/mnt/usb/temp/",       "Temp42.cdb"},  
	{ID_TEMP_CURVE43_FILE , "/mnt/usb/temp/",       "Temp43.cdb"},  
	{ID_TEMP_CURVE44_FILE , "/mnt/usb/temp/",       "Temp44.cdb"},
	{ID_TEMP_CURVE45_FILE , "/mnt/usb/temp/",       "Temp45.cdb"},  
	{ID_TEMP_CURVE46_FILE , "/mnt/usb/temp/",       "Temp46.cdb"},  
	{ID_TEMP_CURVE47_FILE , "/mnt/usb/temp/",       "Temp47.cdb"},  
	{ID_TEMP_CURVE48_FILE , "/mnt/usb/temp/",       "Temp48.cdb"},  
	{ID_TEMP_CURVE49_FILE , "/mnt/usb/temp/",       "Temp49.cdb"},  
	{ID_TEMP_CURVE50_FILE , "/mnt/usb/temp/",       "Temp50.cdb"},  
	{ID_TEMP_CURVE51_FILE , "/mnt/usb/temp/",       "Temp51.cdb"},  
	{ID_TEMP_CURVE52_FILE , "/mnt/usb/temp/",       "Temp52.cdb"},  
	{ID_TEMP_CURVE53_FILE , "/mnt/usb/temp/",       "Temp53.cdb"},  
	{ID_TEMP_CURVE54_FILE , "/mnt/usb/temp/",       "Temp54.cdb"},
	{ID_TEMP_CURVE55_FILE , "/mnt/usb/temp/",       "Temp55.cdb"},  
	{ID_TEMP_CURVE56_FILE , "/mnt/usb/temp/",       "Temp56.cdb"},  
	{ID_TEMP_CURVE57_FILE , "/mnt/usb/temp/",       "Temp57.cdb"},  
	{ID_TEMP_CURVE58_FILE , "/mnt/usb/temp/",       "Temp58.cdb"},  
	{ID_TEMP_CURVE59_FILE , "/mnt/usb/temp/",       "Temp59.cdb"},  
	{ID_TEMP_CURVE60_FILE , "/mnt/usb/temp/",       "Temp60.cdb"},  
	{ID_TEMP_CURVE61_FILE , "/mnt/usb/temp/",       "Temp61.cdb"},  
	{ID_TEMP_CURVE62_FILE , "/mnt/usb/temp/",       "Temp62.cdb"},  
	{ID_TEMP_CURVE63_FILE , "/mnt/usb/temp/",       "Temp63.cdb"},  
	{ID_TEMP_CURVE64_FILE , "/mnt/usb/temp/",       "Temp64.cdb"},
	{ID_TEMP_MESSAGE_FILE , "/mnt/usb/temp/",       "TempMessage.cdb"},
	{ID_WARM_CURVE1_FILE  , "/mnt/usb/temp/",       "WarmTemp1.cdb"},  
	{ID_WARM_CURVE2_FILE  , "/mnt/usb/temp/",       "WarmTemp2.cdb"},  
	{ID_WARM_CURVE3_FILE  , "/mnt/usb/temp/",       "WarmTemp3.cdb"},  
	{ID_WARM_CURVE4_FILE  , "/mnt/usb/temp/",       "WarmTemp4.cdb"},  
	{ID_WARM_CURVE5_FILE  , "/mnt/usb/temp/",       "WarmTemp5.cdb"},  
	{ID_WARM_CURVE6_FILE  , "/mnt/usb/temp/",       "WarmTemp6.cdb"},  
	{ID_WARM_CURVE7_FILE  , "/mnt/usb/temp/",       "WarmTemp7.cdb"},  
	{ID_WARM_CURVE8_FILE  , "/mnt/usb/temp/",       "WarmTemp8.cdb"},  
	{ID_WARM_CURVE9_FILE  , "/mnt/usb/temp/",       "WarmTemp9.cdb"},  
	{ID_WARM_CURVE10_FILE , "/mnt/usb/temp/",       "WarmTemp10.cdb"},  
	{ID_WARM_CURVE11_FILE , "/mnt/usb/temp/",       "WarmTemp11.cdb"},  
	{ID_WARM_CURVE12_FILE , "/mnt/usb/temp/",       "WarmTemp12.cdb"},  
	{ID_WARM_CURVE13_FILE , "/mnt/usb/temp/",       "WarmTemp13.cdb"},  
	{ID_WARM_CURVE14_FILE , "/mnt/usb/temp/",       "WarmTemp14.cdb"},  
	{ID_WARM_CURVE15_FILE , "/mnt/usb/temp/",       "WarmTemp15.cdb"},  
	{ID_WARM_CURVE16_FILE , "/mnt/usb/temp/",       "WarmTemp16.cdb"},  
	{ID_WARM_CURVE17_FILE , "/mnt/usb/temp/",       "WarmTemp17.cdb"},  
	{ID_WARM_CURVE18_FILE , "/mnt/usb/temp/",       "WarmTemp18.cdb"},  
	{ID_WARM_CURVE19_FILE , "/mnt/usb/temp/",       "WarmTemp19.cdb"},  
	{ID_WARM_CURVE20_FILE , "/mnt/usb/temp/",       "WarmTemp20.cdb"},  
	{ID_WARM_CURVE21_FILE  , "/mnt/usb/temp/",       "WarmTemp21.cdb"},  
	{ID_WARM_CURVE22_FILE  , "/mnt/usb/temp/",       "WarmTemp22.cdb"},  
	{ID_WARM_CURVE23_FILE  , "/mnt/usb/temp/",       "WarmTemp23.cdb"},  
	{ID_WARM_CURVE24_FILE  , "/mnt/usb/temp/",       "WarmTemp24.cdb"},  
	{ID_WARM_CURVE25_FILE  , "/mnt/usb/temp/",       "WarmTemp25.cdb"},  
	{ID_WARM_CURVE26_FILE  , "/mnt/usb/temp/",       "WarmTemp26.cdb"},  
	{ID_WARM_CURVE27_FILE  , "/mnt/usb/temp/",       "WarmTemp27.cdb"},  
	{ID_WARM_CURVE28_FILE  , "/mnt/usb/temp/",       "WarmTemp28.cdb"},  
	{ID_WARM_CURVE29_FILE  , "/mnt/usb/temp/",       "WarmTemp29.cdb"},  
	{ID_WARM_CURVE30_FILE , "/mnt/usb/temp/",       "WarmTemp30.cdb"}, 
	{ID_WARM_CURVE31_FILE , "/mnt/usb/temp/",       "WarmTemp31.cdb"}, 
	{ID_WARM_CURVE32_FILE , "/mnt/usb/temp/",       "WarmTemp32.cdb"}, 
	{ID_WARM_CURVE33_FILE , "/mnt/usb/temp/",       "WarmTemp33.cdb"}, 
	{ID_WARM_CURVE34_FILE , "/mnt/usb/temp/",       "WarmTemp34.cdb"}, 
	{ID_WARM_CURVE35_FILE , "/mnt/usb/temp/",       "WarmTemp35.cdb"}, 
	{ID_WARM_CURVE36_FILE , "/mnt/usb/temp/",       "WarmTemp36.cdb"}, 
	{ID_WARM_CURVE37_FILE , "/mnt/usb/temp/",       "WarmTemp37.cdb"}, 
	{ID_WARM_CURVE38_FILE , "/mnt/usb/temp/",       "WarmTemp38.cdb"}, 
	{ID_WARM_CURVE39_FILE , "/mnt/usb/temp/",       "WarmTemp39.cdb"}, 
	{ID_WARM_CURVE40_FILE , "/mnt/usb/temp/",       "WarmTemp40.cdb"}, 
	{ID_WARM_CURVE41_FILE  , "/mnt/usb/temp/",       "WarmTemp41.cdb"},  
	{ID_WARM_CURVE42_FILE  , "/mnt/usb/temp/",       "WarmTemp42.cdb"},  
	{ID_WARM_CURVE43_FILE  , "/mnt/usb/temp/",       "WarmTemp43.cdb"},  
	{ID_WARM_CURVE44_FILE  , "/mnt/usb/temp/",       "WarmTemp44.cdb"},  
	{ID_WARM_CURVE45_FILE  , "/mnt/usb/temp/",       "WarmTemp45.cdb"},  
	{ID_WARM_CURVE46_FILE  , "/mnt/usb/temp/",       "WarmTemp46.cdb"},  
	{ID_WARM_CURVE47_FILE  , "/mnt/usb/temp/",       "WarmTemp47.cdb"},  
	{ID_WARM_CURVE48_FILE  , "/mnt/usb/temp/",       "WarmTemp48.cdb"},  
	{ID_WARM_CURVE49_FILE  , "/mnt/usb/temp/",       "WarmTemp49.cdb"},  
	{ID_WARM_CURVE50_FILE , "/mnt/usb/temp/",       "WarmTemp50.cdb"}, 
	{ID_WARM_CURVE51_FILE , "/mnt/usb/temp/",       "WarmTemp51.cdb"}, 
	{ID_WARM_CURVE52_FILE , "/mnt/usb/temp/",       "WarmTemp52.cdb"}, 
	{ID_WARM_CURVE53_FILE , "/mnt/usb/temp/",       "WarmTemp53.cdb"}, 
	{ID_WARM_CURVE54_FILE , "/mnt/usb/temp/",       "WarmTemp54.cdb"}, 
	{ID_WARM_CURVE55_FILE , "/mnt/usb/temp/",       "WarmTemp55.cdb"}, 
	{ID_WARM_CURVE56_FILE , "/mnt/usb/temp/",       "WarmTemp56.cdb"}, 
	{ID_WARM_CURVE57_FILE , "/mnt/usb/temp/",       "WarmTemp57.cdb"}, 
	{ID_WARM_CURVE58_FILE , "/mnt/usb/temp/",       "WarmTemp58.cdb"}, 
	{ID_WARM_CURVE59_FILE , "/mnt/usb/temp/",       "WarmTemp59.cdb"}, 
	{ID_WARM_CURVE60_FILE , "/mnt/usb/temp/",       "WarmTemp60.cdb"}, 
	{ID_WARM_CURVE61_FILE  , "/mnt/usb/temp/",       "WarmTemp61.cdb"},  
	{ID_WARM_CURVE62_FILE  , "/mnt/usb/temp/",       "WarmTemp62.cdb"},  
	{ID_WARM_CURVE63_FILE  , "/mnt/usb/temp/",       "WarmTemp63.cdb"},  
	{ID_WARM_CURVE64_FILE  , "/mnt/usb/temp/",       "WarmTemp64.cdb"}, 
	{ID_WARM_MESSAGE_FILE , "/mnt/usb/temp/",       "WarmMessage.cdb"},
	{ID_TEMP_SET_FILE     , "/mnt/usb/temp/",       "TempSet.cdb"}
	#endif
};
/*==========================================================================+
|           Class implementation - CtmSram		      	                    |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmSram::CtmSram(): CtmStorageFile()
{
	g_SramWriteFd=
	m_nReadFd=
	m_nWriteFd=-1;
	#ifdef	D_3354
	m_nReadFd = open(tmSRAM_NAME, O_RDWR | O_NOCTTY);
	m_nWriteFd = open(tmSRAM_NAME, O_RDWR | O_NOCTTY);
	//m_nRead2Fd = open("ReplaceSram.rps", O_RDWR | O_CREAT);
	//m_nWrite2Fd = open("ReplaceSram.rps", O_RDWR | O_CREAT);
	if (m_nReadFd <= 0) 
    {
        m_nReadFd = open("/usr/ReplaceSram.rps", O_RDWR | O_CREAT);
        g_SramWriteFd = m_nWriteFd = open("/usr/ReplaceSram.rps", O_RDWR | O_CREAT);
        printf("can't find the Sram, the file replaces the sram\n");
        if (m_nWriteFd > 0)
            ftruncate(m_nWriteFd, CtmConfig::GetInstance()->GetSramSize());
    }
    #elif defined(D_ATOM)
    if (m_nReadFd <= 0) 
    {
        m_nReadFd = open("/usr/ReplaceSram.rps", O_RDWR | O_CREAT);
        g_SramWriteFd = m_nWriteFd = open("/usr/ReplaceSram.rps", O_RDWR | O_CREAT);
        //printf("can't find the Sram, the file replaces the sram\n");
        if (m_nWriteFd > 0)
            ftruncate(m_nWriteFd, CtmConfig::GetInstance()->GetSramSize());
    }
	#else
	m_nReadFd = open(tmSRAM_NAME, O_RDWR | O_NOCTTY);
	m_nWriteFd = open(tmSRAM_NAME, O_RDWR | O_NOCTTY);
	if (m_nReadFd <= 0) 
    {
        m_nReadFd = open("ReplaceSram.rps", O_RDWR | O_CREAT);
        m_nWriteFd = open("ReplaceSram.rps", O_RDWR | O_CREAT);
        //printf("can't find the Sram, the file replaces the sram\n");
        if (m_nWriteFd > 0)
            ftruncate(m_nWriteFd, CtmConfig::GetInstance()->GetSramSize());
    }
	#endif
    
    pthread_mutex_init(&m_ReadMutex, NULL);
	pthread_mutex_init(&m_WriteMutex, NULL);
}

CtmSram::CtmSram(void* pInitData): CtmStorageFile(pInitData)
{
	
}

CtmSram::~CtmSram()
{
	if (m_nReadFd > 0) close(m_nReadFd);
	if (m_nWriteFd > 0) close(m_nWriteFd);
	//if (m_nRead2Fd > 0) close(m_nRead2Fd);
	//if (m_nWrite2Fd > 0) close(m_nWrite2Fd);
	pthread_mutex_destroy(&m_ReadMutex);
	pthread_mutex_destroy(&m_WriteMutex);
}

CtmSram*	CtmSram::GetInstance()
{
	if (m_pInstance == NULL) m_pInstance = new CtmSram();
	return m_pInstance;	
}

CtmSram*	CtmSram::GetInstance(void* pInitData)
{
	if (m_pInstance == NULL) m_pInstance = new CtmSram(pInitData);
	else m_pInstance->ReInit(pInitData);
	
	return m_pInstance;	
}

void	CtmSram::ReleaseInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :  Check()														|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmSram::Check()
{
	return m_nWriteFd;
}

/*--------------------------------------------------------------------------+
|  Function :  Write(unsigned int nPosition, int nSize, BYTE* pcBuffer)		|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSram::Write(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	static	BOOL	bWrite;
	//BOOL	bExit = FALSE;
	const	int	TEST_LEN = 1024;
	
	int	nStatus = 0;
	if (m_nWriteFd > 0)
	{
		pthread_mutex_lock(&m_WriteMutex);
		#if defined(D_3354)||defined(D_ATOM)
		//if((nPosition + nSize) >= SRAM_3354_MAXSIZE)
		//{
		//	if(m_nWrite2Fd > 0)
		//	{
		//		lseek(m_nWrite2Fd, nPosition, SEEK_SET);
		//		nStatus = write(m_nWrite2Fd, (char*)pcBuffer, nSize);
		//	}
		//}
		//else
		g_SramWriteFlag = 1;
		{
			lseek(m_nWriteFd, nPosition, SEEK_SET);
			nStatus = write(m_nWriteFd, (char*)pcBuffer, nSize);
			//printf("CtmSram::Write write size nPosition=%d nSize=%d \n", nPosition, nSize);
		}
		g_SramWriteFlag = 2;
		//fdatasync(m_nWriteFd);
		#else
		lseek(m_nWriteFd, nPosition, SEEK_SET);
		nStatus = write(m_nWriteFd, (char*)pcBuffer, nSize);
		#endif
		//m_nWriteCount--;
		pthread_mutex_unlock(&m_WriteMutex);
	}
	
	
	return nStatus;
}

/*--------------------------------------------------------------------------+
|  Function :  Read(unsigned int nPosition, int nSize, BYTE* pcBuffer)		|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int    CtmSram::Read(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	//printf("Mem Pointer = %X\n", pcBuffer);
	
	
	BYTE	Buffer[512 * 1024];
	int	nStatus = 0;
	if (m_nReadFd > 0)
	{
		pthread_mutex_lock(&m_ReadMutex);
		#ifdef	D_3354
		//if((nPosition + nSize) >= SRAM_3354_MAXSIZE)
		//{
		//	if(m_nRead2Fd > 0)
		//	{
		//		lseek(m_nRead2Fd, nPosition, SEEK_SET);                           
		//		nStatus = read(m_nRead2Fd, (char*)Buffer, nSize);
		//	}
		//}
		//else
		{
			lseek(m_nReadFd, nPosition, SEEK_SET);                           
			nStatus = read(m_nReadFd, (char*)Buffer, nSize); 
			//printf("read size nPosition=%d nSize=%d \n", nPosition, nSize);
		}      
		#else
		lseek(m_nReadFd, nPosition, SEEK_SET);                           
		nStatus = read(m_nReadFd, (char*)Buffer, nSize); 
		#endif
		       
		memcpy(pcBuffer, Buffer, nSize);
		pthread_mutex_unlock(&m_ReadMutex);            
		                             
	}	
	
	return nStatus;
}

/*--------------------------------------------------------------------------+
|  Function :  Delete(int nFlag = -1)										|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSram::Delete(int nFlag )
{
	int	bStatus = 0;	
	if (m_nWriteFd > 0)
	{
		int	nSize = CtmConfig::GetInstance()->GetSramSize();
		char*	pEmpty = new char[nSize];		
		memset(pEmpty, 0, nSize);
		lseek(m_nWriteFd, 0, SEEK_SET);
		bStatus = write(m_nWriteFd, pEmpty, nSize);
		delete []pEmpty;
	}	
	return bStatus;
}



CtmFlashEx::CtmFlashEx(): CtmStorageFile()
{
    //m_nReadFd = open("/dev/mtdblock4", O_RDWR | O_NOCTTY);
    m_FlashEXRecordThreadID = 0;
    m_nWriteFd = open(DEVICE_FLASHEX, O_RDWR | O_NOCTTY | O_SYNC);
	g_tmDebugInfo->PrintDebugInfo("open Write /dev/mtdblock4 %d\n", m_nWriteFd);
    if (m_nWriteFd <= 0) 
    {
        //m_nReadFd = open("ReplaceFlash.rps", O_RDWR | O_CREAT);
        m_nWriteFd = open("ReplaceFlash.rps", O_RDWR | O_CREAT);
        g_tmDebugInfo->PrintDebugInfo("Can't open dev/mtdblock4\n");
        //printf("can't find the Sram, the file replaces the sram\n");
        if (m_nWriteFd > 0)
            ftruncate(m_nWriteFd, m_nFlashSize);
    }
    //pthread_mutex_init(&m_ReadMutex, NULL);
	pthread_mutex_init(&m_WriteMutex, NULL);
}

CtmFlashEx::CtmFlashEx(void* pInitData): CtmStorageFile(pInitData)
{

    
}

CtmFlashEx::~CtmFlashEx()
{
    //if (m_nReadFd > 0) close(m_nReadFd);
	if (m_nWriteFd > 0) close(m_nWriteFd);
	//pthread_mutex_destroy(&m_ReadMutex);
	pthread_mutex_destroy(&m_WriteMutex);
}

CtmFlashEx* CtmFlashEx::GetInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CtmFlashEx();
    }

    return m_pInstance;
}

void    CtmFlashEx::ReleaseInstance()
{
    delete m_pInstance;
    m_pInstance = NULL;
}

int		CtmFlashEx::Check()
{
	return m_nWriteFd;
}

int 	CtmFlashEx::Read(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	BYTE	Buffer[512 * 1024];
	int	nStatus = 0;
	if (m_nWriteFd > 0)
	{
		pthread_mutex_lock(&m_WriteMutex);
		lseek(m_nWriteFd, nPosition, SEEK_SET);
		nStatus = read(m_nWriteFd, (char*)Buffer, nSize);
		memcpy(pcBuffer, Buffer, nSize);
		pthread_mutex_unlock(&m_WriteMutex);            		                             
	}	
	
	return nStatus;
}


int	CtmFlashEx::Write(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	
	int	nStatus = 1;
	m_nPosition = nPosition;
	m_nSize		= nSize;
	//m_pcBuffer  = pcBuffer;
	//pthread_create(&m_FlashEXRecordThreadID, NULL, (PRSRUN)WritePthread, pcBuffer);
	if(!GetFileType(CHECKFILETYPE))	//«OŒ¨mtdblock4¤£³Q‡À§¥
		WritePthread(pcBuffer);
	return nStatus;
}


int	CtmFlashEx::WritePthread(BYTE* pcBuffer)
{
	static	BOOL	bWrite;
	
	int	nStatus = 0;

	if (m_nWriteFd > 0)
	{
		pthread_mutex_lock(&m_WriteMutex);
		
		//char*  pEmpty = new char[m_nSize];		
		//memset(pEmpty, 0, m_nSize);
		//memcpy(pEmpty, pcBuffer, m_nSize);
		lseek(m_nWriteFd, m_nPosition, SEEK_SET);
		//pthread_create(&m_FlashEXRecordThreadID, NULL, (PRSRUN)write(m_nWriteFd, (char*)pcBuffer, nSize), NULL);
		nStatus = write(m_nWriteFd, pcBuffer, m_nSize);
		//delete []pEmpty; 
		//sync();
		//fsync(m_nWriteFd);
		//int	Val = 0;
		//Val = fcntl(m_nWriteFd, F_GETFL, 0);
		//Val |= O_SYNC;
		//fcntl(m_nWriteFd, F_SETFL, Val);
		close(m_nWriteFd);
		m_nWriteFd = open(DEVICE_FLASHEX, O_RDWR | O_NOCTTY | O_SYNC);
		if (m_nWriteFd <= 0) 
	    {
	        m_nWriteFd = open("ReplaceFlash.rps", O_RDWR | O_CREAT);
	        if (m_nWriteFd > 0)
	            ftruncate(m_nWriteFd, m_nFlashSize);
	    }
		pthread_mutex_unlock(&m_WriteMutex);
	}
	
	return nStatus;
}

int	CtmFlashEx::Delete(int nFlag)
{
	int	bStatus = 0;	
	if (m_nWriteFd > 0)
	{
		pthread_mutex_lock(&m_WriteMutex);
		int	nSize = m_nFlashSize;//CtmConfig::GetInstance()->GetSramSize();
		char*	pEmpty = new char[m_nFlashSize];		
		memset(pEmpty, 0, nSize);
		lseek(m_nWriteFd, 0, SEEK_SET);
		g_tmDebugInfo->PrintDebugInfo("FlashEX Delete WireteFd =%d, m_nFlashSize=%d\n", m_nWriteFd, nSize);
		bStatus = write(m_nWriteFd, pEmpty, nSize);
		delete []pEmpty;
		//int	Val = 0;
		//Val = fcntl(m_nWriteFd, F_GETFL, 0);
		//Val |= O_SYNC;
		//fcntl(m_nWriteFd, F_SETFL, Val);
		//fdatasync(m_nWriteFd);
		close(m_nWriteFd);
		m_nWriteFd = open(DEVICE_FLASHEX, O_RDWR | O_NOCTTY | O_SYNC);
		if (m_nWriteFd <= 0) 
	    {
	        m_nWriteFd = open("ReplaceFlash.rps", O_RDWR | O_CREAT);
	        if (m_nWriteFd > 0)
	            ftruncate(m_nWriteFd, m_nFlashSize);
	    }
		pthread_mutex_unlock(&m_WriteMutex);
	}	
	return bStatus;
}


/*==========================================================================+
|           Class implementation - CtmDiskFile	      	                    |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/


CtmDiskFile::CtmDiskFile(): CtmStorageFile()
{
	m_nMountNum = sizeof(m_DiskFiles) / sizeof(m_DiskFiles[0]);
	GetManageFile(m_DiskFiles, m_nMountNum);
	
}

CtmDiskFile::CtmDiskFile(void* pInitData): CtmStorageFile(pInitData)
{
	m_nMountNum = sizeof(m_DiskFiles) / sizeof(m_DiskFiles[0]);
	GetManageFile(m_DiskFiles, m_nMountNum);
	m_nFd = Init(pInitData);
}

CtmDiskFile::~CtmDiskFile()
{
	Close();
	for (int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		m_HMIFiles[i].FileID	= ID_STORAGE_NOFILE;
		if (m_HMIFiles[i].pFileInfo != NULL)
		{
			free(m_HMIFiles[i].pFileInfo);
			m_HMIFiles[i].pFileInfo = NULL;
		}
	}
}

int		CtmDiskFile::Open(char* pszName)
{
	int nResult = -1;
	Close();
	m_File.Open(pszName, O_RDWR | O_CREAT | O_SYNC);
	nResult = m_File.GetFileHandle();
	m_nFd = nResult;
	return nResult;
}

void	CtmDiskFile::Close()
{
	if (m_nFd > 0)
	{
		m_File.Close();
		m_nFd = -1;
	}
}

/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
void CtmDiskFile::GetManageFile(tagHMIMANFILE* pManFile, int nFileNum)
{
	m_nFd = -1;
	for (int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		m_HMIFiles[i].FileID	= ID_STORAGE_NOFILE;
	}
	
	for (unsigned int i = 0; i < nFileNum; i++)
	{
		m_HMIFiles[i].FileID	= pManFile[i].FileID;
		if (pManFile[i].pszFilePath != NULL)
		{
			ExistDir(pManFile[i].pszFilePath);		
			m_HMIFiles[i].pFileInfo = (char*)malloc(strlen(pManFile[i].pszFilePath) + 
										strlen(pManFile[i].pszFileName) + 1);
			sprintf((char*)m_HMIFiles[i].pFileInfo, "%s%s", pManFile[i].pszFilePath, 
				pManFile[i].pszFileName);
		}
		else
		{
			m_HMIFiles[i].pFileInfo = (char*)malloc(strlen(pManFile[i].pszFileName) + 1);
			sprintf((char*)m_HMIFiles[i].pFileInfo, "%s",pManFile[i].pszFileName);
		}
		
	}	
}



/*--------------------------------------------------------------------------+
|  Function :  Check()														|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmDiskFile::Check()
{
	return m_nFd;
}


/*--------------------------------------------------------------------------+
|  Function :  Write(unsigned int nPosition, int nSize, BYTE* pcBuffer)		|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmDiskFile::Write(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	int	nResult = 0;
	if (m_nFd > 0)
	{
		m_File.Seek(nPosition, SEEK_SET);
		nResult = m_File.Write(pcBuffer, nSize);
	}
	return nResult;
}

int CtmDiskFile::Read(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	int	nResult = 0;
	if (m_nFd > 0)
	{
		m_File.Seek(nPosition, SEEK_SET);
		nResult = m_File.Read(pcBuffer, nSize);
	}
	return nResult;
}

int	CtmDiskFile::Delete(int nFlag)
{
	if (m_nFd > 0)
	{
		if (nFlag <= 0) 
			ftruncate(m_nFd, 0);
		else 
			ftruncate(m_nFd, nFlag);
	}
	return m_nFd;
}

int	CtmDiskFile::ReInit(void* pInitData)
{
	if (m_nFd > 0)
	{
		m_File.Close();
	}
	
	m_nFd = Init(pInitData);
	return m_nFd;
}
	

int	CtmDiskFile::Init(void* pInitData)
{
	int	nResult = 0;
	if (pInitData != NULL)
	{
		strncpy(m_szFileName, (char*)pInitData, FILE_NAME_MAX_LEN - 1);
		nResult = Open(m_szFileName);
	}
	return nResult;
}	

int	CtmDiskFile::ReInit(int	nFlag)
{
	int	nResult = -1;
		
	for (int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		if (m_HMIFiles[i].FileID == nFlag)
		{
			nResult = ReInit(m_HMIFiles[i].pFileInfo);
			break;
		}
	}
	
	return nResult;	
}

void	CtmDiskFile::ExistDir(char* pszPath)
{
	DIR*		dir;
	
	dir = opendir(pszPath);
	if (!dir) 
	{
		mkdir(pszPath, 777);
	}
	closedir(dir);
}

int		CtmDiskFile::ReName(int nFileID, char*	pszFileName)
{
	int nResult = -1, nFileLength=0;
	
	//if(nFileID >=0 && nFileID < HMI_SAVE_FILE_MAX)
	for(int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		if(m_DiskFiles[i].FileID == nFileID)
		{
			if (m_DiskFiles[i].pszFilePath != NULL)
			{
				ExistDir(m_DiskFiles[i].pszFilePath);
				if(m_HMIFiles[i].pFileInfo != NULL)
				{
					free(m_HMIFiles[i].pFileInfo);
					nFileLength = strlen(pszFileName);
					if(nFileLength > 0 && nFileLength < 256)
					{
						m_HMIFiles[i].pFileInfo = (char*)malloc(strlen(m_DiskFiles[i].pszFilePath) + nFileLength+ 1);
					}
					else 
						return nResult;
				}
				else
					return nResult;
			}
			else
			{
				m_HMIFiles[i].pFileInfo = (char*)malloc(nFileLength + 1);
			}
			sprintf((char*)m_HMIFiles[i].pFileInfo, "%s%s", m_DiskFiles[i].pszFilePath, pszFileName);	
			//printf("DISK FileInfo=%s \n", m_HMIFiles[i].pFileInfo);
		}
	}
	nResult = 0;
	return 	nResult;
}


int	CtmDiskFile::CheckFileExist(int nFileID)
{
	int nResult = -1, nFd = 0;
	for(int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		if(m_DiskFiles[i].FileID == nFileID)
		{
			if(m_HMIFiles[i].pFileInfo != NULL)
			{
				nFd = open((char*)m_HMIFiles[i].pFileInfo, O_RDONLY, 0);
				if(nFd > 0)
				{
					nResult = 1;
					close(nFd);
				}
				else
					nResult = 0;
				break;	
			}
			else
				break;
		}
	}
	return 	nResult;	
}

/*==========================================================================+
|           Class implementation - CtmMountFile      	                    |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmMountFile::CtmMountFile(): CtmDiskFile() 
{
	pthread_mutex_init(&m_mutex, NULL);
	m_nMountStatus = -1;
}

CtmMountFile::CtmMountFile(void* pInitData): CtmDiskFile(pInitData) 
{
	
}

CtmMountFile::~CtmMountFile()
{
	Close();
	UMount();
	for (int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		m_HMIFiles[i].FileID	= ID_STORAGE_NOFILE;
		if (m_HMIFiles[i].pFileInfo != NULL)
		{
			free(m_HMIFiles[i].pFileInfo);
			m_HMIFiles[i].pFileInfo = NULL;
		}
	}	
	pthread_mutex_destroy(&m_mutex);
}


void CtmMountFile::GetManageFile(tagHMIMANFILE* pManFile, int nFileNum)
{
	CtmDiskFile::GetManageFile(pManFile, nFileNum);
	m_bReplace = FALSE;
}

int CtmMountFile::ReInitMountFile(void* pInitData, tagHMIMANFILE* pMountFile)
{
	int	nResult = -1;
	//if (m_nMountStatus != MOUNT_SUCCESS) Mount();
	
	if (m_nMountStatus == MOUNT_SUCCESS)
	{
		for (int i = 0; i < m_nMountNum; i++)
		{
			//printf( "pMountFile[i].pszFilePath =%s \n", pMountFile[i].pszFilePath );
			//printf( "i=%d FileInfo=%s InitData=%s \n",i, m_HMIFiles[i].pFileInfo, (char*)pInitData );
			if (strcmp((char*)m_HMIFiles[i].pFileInfo, (char*)pInitData) == 0)
			{
				ExistDir(pMountFile[i].pszFilePath);
				GetFileInfo(pMountFile[i].pszFilePath);
				break;
			} 
		}
		nResult = Open((char*)pInitData);
		g_tmDebugInfo->PrintDebugInfo("save data=%s\n", (char*)pInitData);
	}
	
	return nResult;
}

int	CtmMountFile::Write(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{	
	int nResult = -1;
	if (m_nFd > 0)
	{
		if (m_lAvailSize > nSize)
		{
			m_File.Seek(nPosition, SEEK_SET);
			nResult = m_File.Write(pcBuffer, nSize);
			if (nResult < 0)
			{
				
				SendMsg(MSG_OUTSTORAGE_WERROR, nResult, 0, NULL);
				//
				//MsgBox(g_MultiLanguage["VM_HMI_OUTSIDE_WRITE_ERROR_MMC"], tmFT_CODE_TECH);
				UMount();
			}
		}
	}
	//UMount();
	return nResult;
}

int CtmMountFile::Read(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	int nResult = -1;
	if (m_nFd > 0)
	{
		if (m_lAvailSize > nSize)
		{
			m_File.Seek(nPosition, SEEK_SET);
			nResult = m_File.Read(pcBuffer, nSize);
			if (nResult < 0)
			{
				SendMsg(MSG_OUTSTORAGE_RERROR, nResult, 0, NULL);
					
				//MsgBox(g_MultiLanguage["VM_HMI_OUTSIDE_READ_ERROR_MMC"], tmFT_CODE_TECH);
				UMount();
			}
		}
	}
	//UMount();
	return nResult;
}

int	CtmMountFile::Delete(int nFlag)
{
	int	nResult = -1;
	if (m_nFd > 0) 
	{
		m_File.Truncate(0);
		nResult = 0;
	}
	//UMount();
	return nResult;
}

int	CtmMountFile::Open(char* pszName)
{
	int	nResult = 0;
	if (m_nMountStatus == MOUNT_SUCCESS)
	{
//		GetFileInfo(MOUNT_MMCARD_PATH);
		nResult = CtmDiskFile::Open(pszName);
	}
	return nResult;
}


void	CtmMountFile::GetFileInfo(char* pszPath)
{
    if (m_bReplace)
        m_lAvailSize = 0x7FFFFFFF;
    else 
    {
        struct	statfs	StatFS;
	
    	if (pszPath == NULL) return;
    	statfs(pszPath, &StatFS);
    	m_lAvailSize = StatFS.f_bfree * StatFS.f_blocks / 2;
    }
	
}

/*==========================================================================+
|           Class implementation - CtmMMCardFile      	                    |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CtmMMCardFile::CtmMMCardFile(): CtmMountFile()
{
	m_nMountNum = sizeof(m_MMCardFiles) / sizeof(m_MMCardFiles[0]);
	GetManageFile(m_MMCardFiles, m_nMountNum);	
	strcpy(m_szMountPath, MOUNT_MMCARD_PATH);
}


CtmMMCardFile::CtmMMCardFile(void* pInitData): CtmMountFile(pInitData)
{
	m_nMountNum = sizeof(m_MMCardFiles) / sizeof(m_MMCardFiles[0]);
	GetManageFile(m_MMCardFiles, m_nMountNum);
	strcpy(m_szMountPath, MOUNT_MMCARD_PATH);
	ReInit(pInitData);
}


CtmMMCardFile::~CtmMMCardFile()
{
	
	
}


CtmMMCardFile*	CtmMMCardFile::GetInstance()
{
	if (m_pInstance == NULL) m_pInstance = new CtmMMCardFile();
	
	return m_pInstance;		
}

CtmMMCardFile*	CtmMMCardFile::GetInstance(void* pInitData)
{
	if (m_pInstance == NULL) m_pInstance = new CtmMMCardFile(pInitData);
	else m_pInstance->ReInit(pInitData);
	
	return m_pInstance;	
}

void		CtmMMCardFile::ReleaseInstance()
{
	//printf("CtmMMcardFile ReleaseInstance()!!\n");
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
		

BOOL CtmMMCardFile::Mount(BOOL bMsg, BOOL bPromp)
{
	if (m_nMountStatus == MOUNT_SUCCESS)
	{
		g_wMMCStatus=1;	//yuc<080118>
		return TRUE;
	}
	//=============================================================================
	// mount return value:
	//   0      success
    //   1      incorrect invocation or permissions
    //   2      system error (out of memory, cannot fork, no more loop devices)
    //   4      internal mount bug or missing nfs support in mount
    //   8      user interrupt
    //   16     problems writing or locking /etc/mtab
    //   32     mount failure
    //   64     some mount succeeded
    //=============================================================================
	BOOL bResult;
	int nStatus;
	
	pthread_mutex_lock(&m_mutex);
	ExistDir(m_szMountPath);
	
	//	MSG_OUTSTORAGE_MOUNT
	SendMsg(MSG_OUTSTORAGE_MOUNT, 0, 0, NULL);
	//if(bPromp)
	//	Prompt(g_MultiLanguage["VM_HMI_MOUNT_OUTSIDE_MMC"], 1);//Sunny<20080213>
	umount(m_szMountPath);									   //James add 2009/3/20 01:02¤U¤È
	nStatus = mount(DEVICE_MMC, m_szMountPath, FILESYSTEM_TYPE, 0, NULL);

    //if (nStatus != MOUNT_SUCCESS)  
    //{
    //	m_bReplace = TRUE;
    //	nStatus = MOUNT_SUCCESS;
    //}
    //else 
    	m_bReplace = FALSE;
  
    							
    										
    							

	m_nMountStatus = nStatus;
	bResult = (m_nMountStatus == MOUNT_SUCCESS);
	g_wMMCStatus=bResult; //yuc<080118>
	
	if (!bResult)
		SendMsg(MSG_OUTSTORAGE_NODEVICE, m_nMountStatus, STORAGE_TYPE_MMCARD, NULL);
	//if(!bResult && bMsg)	
	//	MsgBox(g_MultiLanguage["VW_MSG_MMCNOTINSINDE"], tmFT_CODE_TECH);
	pthread_mutex_unlock(&m_mutex);
	return	bResult;
}

BOOL CtmMMCardFile::UMount(BOOL bMsg, BOOL bPromp)
{
	BOOL bResult;
	int		nStatus = m_nMountStatus;	
	pthread_mutex_lock(&m_mutex);
	if (m_nMountStatus == MOUNT_SUCCESS)
	{
		Close();
        if (m_bReplace) 
        {
        	m_bReplace = FALSE;
            nStatus = 1;
        }
        else
        {
        	
        	SendMsg(MSG_OUTSTORAGE_UNMOUNT, 0, STORAGE_TYPE_MMCARD, NULL);
        	//if(bPromp)
        	//	Prompt(g_MultiLanguage["VM_HMI_UMOUNT_OUTSIDE_MMC"], 1);//Sunny<20080213>

            nStatus = umount(m_szMountPath);
            //g_tmDebugInfo->PrintDebugInfo("MMCard UMont %d\n", nStatus);
            Prompt("", 2);
        }
       
		m_nMountStatus = nStatus;
		if (m_nMountStatus >= 0) 
		{
			m_nMountStatus = UMOUNT_SUCCESS;
		}
		else 
		{
			m_nMountStatus = UMOUNT_FAIL;
		}
	}
	bResult = (m_nMountStatus == UMOUNT_SUCCESS);
	
	//g_tmDebugInfo->PrintDebugInfo("MMCard UMont Result %d, %d\n", nStatus, bResult);
	g_wMMCStatus=!(bResult); //yuc<080118>
	
	
	if (!bResult)
		SendMsg(MSG_OUTSTORAGE_NODEVICE, m_nMountStatus, STORAGE_TYPE_MMCARD, NULL);

	//if(!bResult && bMsg)	
	//	MsgBox(g_MultiLanguage["VW_MSG_MMCNOTINSINDE"], tmFT_CODE_TECH);
	pthread_mutex_unlock(&m_mutex);
	return bResult;
}

int	CtmMMCardFile::ReInit(void* pInitData)
{
	return ReInitMountFile(pInitData, m_MMCardFiles);	
}



int	CtmMMCardFile::Write(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	int nResult = CtmMountFile::Write(nPosition, nSize, pcBuffer);
	//UMount();
	return nResult;
}		

int CtmMMCardFile::Read(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	int nResult = CtmMountFile::Read(nPosition, nSize, pcBuffer);
	//UMount();
	return nResult;
}

int	CtmMMCardFile::Delete(int nFlag)
{
	int nResult = CtmMountFile::Delete(nFlag);
	//UMount();
	return nResult;
}

int	CtmMMCardFile::ReName(int nFileID, char*	pszFileName)
{
	int nResult = -1, nFileLength=0;
	
	//if(nFileID >=0 && nFileID < HMI_SAVE_FILE_MAX)
	for(int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		if(m_MMCardFiles[i].FileID == nFileID)
		{
			if (m_MMCardFiles[i].pszFilePath != NULL)
			{
				ExistDir(m_MMCardFiles[i].pszFilePath);
				if(m_HMIFiles[i].pFileInfo != NULL)
				{
					free(m_HMIFiles[i].pFileInfo);
					nFileLength = strlen(pszFileName);
					if(nFileLength > 0 && nFileLength < 256)
					{
						m_HMIFiles[i].pFileInfo = (char*)malloc(strlen(m_MMCardFiles[i].pszFilePath) + nFileLength+ 1);
					}
					else 
						return nResult;
				}
				else
					return nResult;
			}
			else
			{
				m_HMIFiles[i].pFileInfo = (char*)malloc(nFileLength + 1);
			}
			sprintf((char*)m_HMIFiles[i].pFileInfo, "%s%s", m_MMCardFiles[i].pszFilePath, pszFileName);	
			//printf("MMC FileInfo=%s \n", m_HMIFiles[i].pFileInfo);
		}
	}
	nResult = 0;
	return 	nResult;
}

int	CtmMMCardFile::CheckFileExist(int nFileID)
{
	int nResult = -1, nFd = 0;
	for(int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		if(m_MMCardFiles[i].FileID == nFileID)
		{
			if(m_HMIFiles[i].pFileInfo != NULL)
			{
				nFd = open((char*)m_HMIFiles[i].pFileInfo, O_RDONLY, 0);
				if(nFd > 0)
				{
					nResult = 1;
					close(nFd);
				}
				else
					nResult = 0;
				break;	
			}
			else
				break;
		}
	}
	return 	nResult;	
}

/*==========================================================================+
|           Class implementation - CtmUsbFile	      	                    |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CtmUsbFile::CtmUsbFile(): CtmMountFile()
{
	m_nMountNum = sizeof(m_UsbFiles) / sizeof(m_UsbFiles[0]);
	m_pSelf	= this;
	pthread_attr_init(&m_ThreadAttr);
	pthread_attr_setdetachstate(&m_ThreadAttr, PTHREAD_CREATE_DETACHED);
	pthread_create(&m_OpenThreadID, NULL, (PRSRUN)ThreadUSBhotplug, NULL);
	

	GetManageFile(m_UsbFiles, m_nMountNum);	
	strcpy(m_szMountPath, MOUNT_USB_PATH);
}

CtmUsbFile::CtmUsbFile(void *pInitData): CtmMountFile(pInitData)
{
	m_nMountNum = sizeof(m_UsbFiles) / sizeof(m_UsbFiles[0]);
    GetManageFile(m_UsbFiles, m_nMountNum);	
	strcpy(m_szMountPath, MOUNT_USB_PATH);
    ReInit(pInitData);
}

CtmUsbFile::~CtmUsbFile()
{
	pthread_cancel(m_OpenThreadID);
}

void	CtmUsbFile::ThreadUSBhotplug(void *pData)
{
	int nStat=0;
	static int nCount = 0, nOldStat = 0;
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		
	}
	while(1)
	{
		if(g_bAutoHotPlug)
		{
			#ifdef	D_PXA270
				if(g_nRTCFd >= 0)
				{
					#ifdef D_3354
						nStat=access("/dev/sda1",F_OK)?0:1;
						if(0==nStat)
						{
							nStat=access("/dev/sda",F_OK)?0:1;	
						}
					#else
						nStat = ioctl(g_nRTCFd, 0x4021, 0);
					#endif	
					if(nOldStat != nStat)
					{
						nCount = 0;
						nOldStat = nStat;
					}
					if(1 == nStat)	//1:ok 0:no usb
					{
						if(!g_wUSBStatus && (nCount == 0 || nCount >=20))
						{
							m_pSelf->Mount(0,0);
							nCount = 0;
						}
					}
					else
					{
						if(g_wUSBStatus && (nCount == 0 || nCount >=20))
						{
							m_pSelf->UMount(0,0);
							nCount = 0;
						}	
					}
				}
			#endif
			#ifdef	D_X86
				int  fd = -1;
				if ((fd = open("/dev/sda1", O_RDONLY, 0)) > 0 )
				{
					close(fd);
					if(!g_wUSBStatus)
					{
						//printf("mount \n");
						m_pSelf->Mount(0,0);
					}
				}
				else
				{
					if(g_wUSBStatus)
					{
						//printf("umount \n");
						m_pSelf->UMount(0,0);
					}	
				}
				//printf("fd=%d \n", fd);
			#endif
		}
		else 
		{
			nCount = 0;	
		}
		//#ifdef	D_PXA255
		//	if(g_wMMCStatus==0)
		//	{	
		//		CtmMMCardFile::GetInstance()->Mount(1);
		//	}
		//	if(g_wMMCStatus == 1)
		//		nStat = g_wMMCStatus;
		//	CtmMMCardFile::GetInstance()->UMount(1);
		//#endif
		nCount++;
		usleep(1000*1000);
	}
}

CtmUsbFile* CtmUsbFile::GetInstance()
{
    if (m_pInstance == NULL)
        m_pInstance = new CtmUsbFile;
    return m_pInstance;
}

CtmUsbFile* CtmUsbFile::GetInstance(void* pInitData)
{
    if (m_pInstance == NULL)
        m_pInstance = new CtmUsbFile;
    m_pInstance->ReInit(pInitData);

    return m_pInstance;
}

void		CtmUsbFile::ReleaseInstance()
{
    delete m_pInstance;
    m_pInstance = NULL;
}

/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
BOOL CtmUsbFile::Mount(BOOL bMsg, BOOL bPromp)
{
	if (m_nMountStatus == MOUNT_SUCCESS)
	{
		g_wUSBStatus  =1;
		//g_wMMCStatus=1;	//yuc<080118>
		return TRUE;
	}
//=============================================================================
	// mount return value:
	//   0      success
    //   1      incorrect invocation or permissions
    //   2      system error (out of memory, cannot fork, no more loop devices)
    //   4      internal mount bug or missing nfs support in mount
    //   8      user interrupt
    //   16     problems writing or locking /etc/mtab
    //   32     mount failure
    //   64     some mount succeeded
    //=============================================================================
    BOOL bResult = FALSE;
    int nStatus = 0;
    pthread_mutex_lock(&m_mutex);
    ExistDir(m_szMountPath);
    //char	szCmd[256];
    
    //	fans add 2009-10-22 9:57:13
    umount(m_szMountPath);
    for (int i = 0; i < sizeof(m_pszUsbDev) / sizeof(m_pszUsbDev[0]); i++)
    {
    	//umount(m_pszUsbDev[i]);		//	fans mark 2009-10-22 9:57:04
        nStatus = mount(m_pszUsbDev[i], m_szMountPath, FILESYSTEM_TYPE, 0, NULL);
        g_tmDebugInfo->PrintDebugInfo("mount m_pszUsbDev[%d] =%s nStatus=%d\n", i, m_pszUsbDev[i], nStatus);
        //printf("mount m_pszUsbDev[%d] =%s nStatus=%d\n", i, m_pszUsbDev[i], nStatus);
        //sprintf(szCmd, "%s%s%s", "mount ", m_pszUsbDev[i], " /mnt/usb/");
        //nStatus = execl("/",szCmd);
        //nStatus = execl("/", "mount"
        m_nMountStatus = nStatus;
//        CtmDebuggerCpp::GetInstance()->GetDebugInfoInterface()->LogDebugInfo(NULL,
//			"Usb m_pszUsbDev[%d]=%s, usb status=%d", i, m_pszUsbDev[i], m_nMountStatus);
        if (MOUNT_SUCCESS == nStatus) 
        {
        	m_bReplace = FALSE;
        	break;
        }
    }
	
	m_nMountStatus = nStatus;
	bResult = (m_nMountStatus == MOUNT_SUCCESS);
	g_wUSBStatus  =bResult; //yuc<080118> 
	//g_wMMCStatus=bResult; //yuc<080118>
	//James mark 2008/4/16 08:37¤U¤È ¸Ñ¨M²Ä¤G¦¸mount§Y¨Ï¨S¦³usbª¬ºA¤]ªð¦^¦³
	/*if (nStatus != MOUNT_SUCCESS)  
    {
    	m_bReplace = TRUE;
    	nStatus = MOUNT_SUCCESS;
    	m_nMountStatus = nStatus;
//    	CtmDebuggerCpp::GetInstance()->GetDebugInfoInterface()->LogDebugInfo(NULL,
//			"Usb can't mount, the usb will be replaced!!");
    }
    else m_bReplace = FALSE;*/
    
    if(!bResult)
    
    	SendMsg(MSG_OUTSTORAGE_NODEVICE, m_nMountStatus, STORAGE_TYPE_USB, NULL);
    	
   	//if(!bResult && bMsg)	
	//	MsgBox(g_MultiLanguage["VW_MSG_MMCNOTINSINDE"], tmFT_CODE_TECH);
    pthread_mutex_unlock(&m_mutex);
    return bResult;
}

BOOL CtmUsbFile::UMount(BOOL bMsg, BOOL bPromp)
{   
	BOOL bResult;
	int		nStatus = m_nMountStatus;
	if (m_nMountStatus == MOUNT_SUCCESS)
	{
		Close();
        if (m_bReplace) 
        {
//        	CtmDebuggerCpp::GetInstance()->GetDebugInfoInterface()->LogDebugInfo(NULL,
//			"not need umount! The USB has been replaced with File\n");
        	m_bReplace = FALSE;
            nStatus = 1;
        }
        else
        {
        	nStatus = -1;
        	SendMsg(MSG_OUTSTORAGE_UNMOUNT, 0, STORAGE_TYPE_USB, NULL);
        	for (int i = 0; i < 3/*10*/; i++)
        	//while (nStatus < 0)
        	{
            	nStatus = umount(m_szMountPath);
            //	printf("umount nStatus=%d m_szMountPath =%s \n", nStatus, m_szMountPath);
            	g_tmDebugInfo->PrintDebugInfo("umount nStatus=%d m_szMountPath =%s \n", nStatus, m_szMountPath);
            	//nStatus = execl("/", "umount", m_szMountPath);
//            	 CtmDebuggerCpp::GetInstance()->GetDebugInfoInterface()->LogDebugInfo(NULL,
//					"umount %s %d", m_szMountPath, nStatus);
            	if (nStatus >= 0)
            		break;
            	else
            	{
//            		 CtmDebuggerCpp::GetInstance()->GetDebugInfoInterface()->LogDebugInfo(NULL,
//					"umount ERROR: %s ", strerror(errno));
            		//usleep(100 * 1000);
            		Delay(100);
            	}
            }
        }
       
		m_nMountStatus = nStatus;
		if (m_nMountStatus >= 0) 
		{
			m_nMountStatus = UMOUNT_SUCCESS;
//			CtmDebuggerCpp::GetInstance()->GetDebugInfoInterface()->LogDebugInfo(NULL,
//			"umount successfully!");
			//printfTM("umount successfully!\n");
		}
		else {

//			CtmDebuggerCpp::GetInstance()->GetDebugInfoInterface()->LogDebugInfo(NULL,
//			"umount not successfully %d!", m_nMountStatus);
			m_nMountStatus = UMOUNT_FAIL;
		}
	}
	bResult = (m_nMountStatus == UMOUNT_SUCCESS);
	//add by J.Wong 2015-04-13 15:58:20
	#ifdef D_3354
		if(!bResult)
		{
			bResult=TRUE; //设备不存在，默认卸载成功
		}
	#else
		if(!bResult)
    		SendMsg(MSG_OUTSTORAGE_NODEVICE, nStatus, STORAGE_TYPE_USB, NULL);
    #endif
	//if(!bResult && bMsg)	
	//	MsgBox(g_MultiLanguage["VW_MSG_MMCNOTINSINDE"], tmFT_CODE_TECH);
	g_wUSBStatus= !bResult;
	//printf("umount g_wUSBStatus=%d \n", g_wUSBStatus);
	//g_wMMCStatus=bResult; //yuc<080118>
	g_tmDebugInfo->PrintDebugInfo("g_wMMCStatus =%d\n", g_wMMCStatus);
	return bResult;
}

int	CtmUsbFile::ReInit(void* pInitData)
{
//	CtmDebuggerCpp::GetInstance()->GetDebugInfoInterface()->LogDebugInfo(NULL,
//			"CtmUsbFile::ReInit %s", pInitData);
	return ReInitMountFile(pInitData, m_UsbFiles);	
}



int	CtmUsbFile::Write(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	//printfTM("CtmUsbFile::Write\n");
	int nResult = CtmMountFile::Write(nPosition, nSize, pcBuffer);
	//UMount();
	return nResult;
}		

int CtmUsbFile::Read(unsigned int nPosition, int nSize, BYTE* pcBuffer)
{
	int nResult = CtmMountFile::Read(nPosition, nSize, pcBuffer);
	//UMount();
	return nResult;
}

int	CtmUsbFile::Delete(int nFlag)
{
	int nResult = CtmMountFile::Delete(nFlag);
	//UMount();
	return nResult;
}

int	CtmUsbFile::ReName(int nFileID, char*	pszFileName)
{
	int nResult = -1, nFileLength=0;
	
	//if(nFileID >=0 && nFileID < HMI_SAVE_FILE_MAX)
	for(int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		if(m_UsbFiles[i].FileID == nFileID)
		{
			if (m_UsbFiles[i].pszFilePath != NULL)
			{
				ExistDir(m_UsbFiles[i].pszFilePath);
				if(m_HMIFiles[i].pFileInfo != NULL)
				{
					free(m_HMIFiles[i].pFileInfo);
					nFileLength = strlen(pszFileName);
					if(nFileLength > 0 && nFileLength < 256)
					{
						m_HMIFiles[i].pFileInfo = (char*)malloc(strlen(m_UsbFiles[i].pszFilePath) + nFileLength+ 1);
					}
					else 
						return nResult;
				}
				else
					return nResult;
			}
			else
			{
				m_HMIFiles[i].pFileInfo = (char*)malloc(nFileLength + 1);
			}
			sprintf((char*)m_HMIFiles[i].pFileInfo, "%s%s", m_UsbFiles[i].pszFilePath, pszFileName);	
			//printf("USB FileInfo=%s nFileID=%d \n", m_HMIFiles[i].pFileInfo, nFileID);
		}
	}
	nResult = 0;
	return 	nResult;
}

int	CtmUsbFile::CheckFileExist(int nFileID)
{
	int nResult = -1, nFd = 0;
	for(int i = 0; i < HMI_SAVE_FILE_MAX; i++)
	{
		if(m_UsbFiles[i].FileID == nFileID)
		{
			//printf("m_HMIFiles[i].pFileInfo=%s \n", m_HMIFiles[i].pFileInfo);
			if(m_HMIFiles[i].pFileInfo != NULL)
			{
				nFd = open((char*)m_HMIFiles[i].pFileInfo, O_RDONLY, 0);
				if(nFd > 0)
				{
					nResult = 1;
					close(nFd);
				}
				else
					nResult = 0;
				break;	
			}
			else
				break;
		}
	}
	return 	nResult;	
}
