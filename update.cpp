/*==========================================================================+
|  Class    : Update                                                        |
|  Task     : Update file and directory					                    |
|---------------------------------------------------------------------------|
|  Compile  : ARM-LINUX-G++ -                                               |
|  Link     : ARM-LINUX-G++ -                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : Leo Zhang                                                     |
|  Version  : V2.00                                                         |
|  Creation : 05/14/2007                                                    |
|  Revision :                                                               |
+==========================================================================*/

#include	<sys/mount.h>
#include 	<pthread.h>
#include    "update.h"
#include	"common.h"
#include    "utils.h"
#include	"timer.h"
#include	"commonaction.h"

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
const	char*	u_pcUpdateFileName = "update/mmiupdate";
const	char*	u_pcBackUpDirectory= "update/backup";
const	char*	u_pcUpdatePath	   = "update/";
const	char*	u_pcTempFileName   = "/usr/tempconfig";
const	DWORD	u_dwFlashBlockSize = 0x40000;
#ifdef	D_ARM
#ifdef	D_PXA270
const	char*	u_pcUpdPath		   = "/mnt/usb";
const	char*	u_pcUpdDevicePath1 = "/dev/sda1";
const	char*	u_pcUpdDeviceFlash = "/dev/mtdblock1";
#else
const	char*	u_pcUpdPath		   = "/mnt/mmc";
const	char*	u_pcUpdDevicePath1 = "/dev/mmcda1";
const	char*	u_pcUpdDeviceFlash = "/dev/mtdblock2";
#endif
#else
const	char*	u_pcUpdPath		   = "/mnt/usb";
const	char*	u_pcUpdDevicePath1 = "/dev/sda1";
const	char*	u_pcUpdDevicePath2 = "/dev/sdb1";
const	char*	u_pcUpdDeviceFlash = "/dev/mtdblock1";
#endif
const	char*	u_pcUpdFileSystem  = "vfat";
const	long	u_lMovieTimer	   = 1024;

#define		PRINTF(x)		;
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//********
//	example:
//	[BEG]
//	[UPF] mmi /usr/i86
//	[UPD] res /usr/i86
//	[DEL]     /usr/i86/mmi
//  [UKN] zImage
//	[DLF] /usr/i86/mmi backup
//  [DLD] /usr/i86/res backup
//	[END]
//********


/* 
==========================================================
yuc add <2008.02.18>
update the whole system in /usr/i86
leave /usr/i86base alone(don't update i86base)
---------------------

1:cp system.tar.gz /usr/test
2:tar zxf /usr/test/system.tar.gz

---------------------
config file will like this
---------------------
	[BEG]
	[UPF]	system.tar.gz /usr/test
	[TXF]	/usr/test/system.tar.gz /usr/i86
	[END]
==========================================================	
	*/
const	tmCMDTYPE	u_acmdtype[] =
	{		
		// <<yuc add 2 command <080218>
		{CMDTYPE_TARPARA, "[TXF]", "tar zxf "},		// tar a whole packge. 
		// >>
		
		{CMDTYPE_TWOPARA, "[UPF]", "cp -f "},		// update one file or some
		{CMDTYPE_TWOPARA, "[UPD]", "cp -Rdf "},		// update directory
		{CMDTYPE_ONEPARA, "[DEL]", "rm -rf "},		// delete file or directory
		#ifdef	D_ARM
		{CMDTYPE_UPKERNEL, "[UKN]", "kernel"},		// update kernel LEO20070808
		#endif
		{CMDTYPE_TWOPARASP, "[DLF]", "cp -f "},		// download file from pxa255 to mmc
		{CMDTYPE_TWOPARASP, "[DLD]", "cp -Rdf "},	// download file from pxa255 to mmc
		
		{CMDTYPE_NONEPARA, "[BEG]", NULL},			// start flag, must exist at first line
		{CMDTYPE_NONEPARA, "[END]", NULL},			// end flag, must exist at last line
		{0, NULL, NULL}
	};

static	STORAGEDEV	u_asStorageDevice[] = 
{
	{1, -1, u_pcUpdDevicePath1, u_pcUpdPath},
	#ifndef	D_ARM
	{2, -1, u_pcUpdDevicePath2, u_pcUpdPath},
	#endif
	{-1,-1, NULL,NULL}
};

const	char	u_acCustomName[] = 
	{ '0','1','2','3','4','5','6','7','8','9', 
	  'a','b','c','d','e','f','g','h','i','j',
	  'k','l','m','n','o','p','q','r','s','t',
	  'u','v','w','x','y','z'};
//********
//	update begin flag
//********
static	BOOL 		u_bUpdateBegin;
static	BOOL		u_bThreadStopFlag;
static	BOOL 		u_bUpdateFlag;
static	pthread_t	u_pthreadmovie;
static	pthread_t	u_pthreadupdate;
static	WORD		u_wUpdateWarningFlag  = E_UPDATE_NULL;
static	BOOL		u_UpdateEndFlag = FALSE;
		int 		u_nUpdateActionFlag   = ACTION_NOP;
static	char		u_acUpdateFileName[64];
static	char		u_acBackUpDirectory[64];
static	char		u_acUpdatePath[64];
		CtmWnd*		u_pUpdateWindow = NULL;
		CtmWnd*		u_pUpdateWindow2 = NULL; 	//yuc<080130> add
		CtmWnd*		u_pUpdateWindow3 = NULL;	//yuc<080130> add

static	void*		DealUpdate(void* arg);
static	int			DealUpdateFile(int nState);
static	int			DealString(char *pcCommand, int len, int nState);
static	int			DealKernelUpdate(char *pFileName);
static	int			DealBackupConfig(char* pString, int len);
static	void*		DealMovie(void* arg);
static	int			DealCustomName(long lPassword);
static	void		GetFileNameFromString(char* pSource, char* pDest);
/*==========================================================================+
|           Class implementation - Device                                   |
+==========================================================================*/

/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :  UpdateMMIFile                                                 |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		UpdateMMIFile(CtmWnd* pWnd, int nState, long lPassword)
{
	if(nState == ACTION_NOP)	return;
	u_nUpdateActionFlag = nState;
	if(DealCustomName(lPassword) == -1)						
	{
		u_wUpdateWarningFlag = E_UPDATE_PASSWD;
		return;
	}
	u_bThreadStopFlag   = FALSE;
	u_bUpdateFlag		= TRUE;
	if(pWnd != NULL)
	{
		u_pUpdateWindow = pWnd->FindControlFromName("StaticUpdataMMI");
		u_pUpdateWindow2 = pWnd->FindControlFromName("StaticUpdataMMI2");
		u_pUpdateWindow3 = pWnd->FindControlFromName("StaticUpdataMMI3");
	}
	// create thread of movie
	if(pthread_create(&u_pthreadmovie, NULL, DealMovie, NULL) != 0)
	{
		PRINTF(printf("Create Thread Error:%s\n",strerror(errno)));
		return;
	}
	// create thread of update
	if(pthread_create(&u_pthreadupdate, NULL, DealUpdate, NULL) != 0)
	{
		PRINTF(printf("Create Thread Error:%s\n",strerror(errno)));
		pthread_exit(&u_pthreadmovie);
		return;
	}
	
	if((pthread_join(u_pthreadmovie, NULL) != 0) ||
							(pthread_join(u_pthreadupdate, NULL) != 0))
	{
		PRINTF(printf("Thread Join Error:%s\n", strerror(errno)));
	}
	// if update ok, exec dead cycle
	if(u_UpdateEndFlag)
	{
		if(!vfork())
		{
			 MsgBox(g_MultiLanguage["VW_UPDATE_END"],3);	//yuc<080130> add
			 exit(1);
		}
	}
	while(u_bUpdateFlag && u_bUpdateBegin)	//if update end sucessesful
		sleep(2);

	return;
}
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :  DealUpdate                                                    |
|  Task     :  thread of update                                              |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
static	void*			DealUpdate(void* arg)
{
	int			nMountStatus;
	int			nUpdateFile = -2;
	char		msz[80];
	char 		mszMul[160];
	int			i, j;
	struct stat dirbuf;
	int			dirstatus;

	dirstatus = stat(u_pcUpdPath, &dirbuf);
	if(dirstatus == -1 || S_ISDIR(dirbuf.st_mode) == 0)
	{
		if(mkdir(u_pcUpdPath, 0777) < 0)
			PRINTF(printf("Create directroy Error:%s\n", strerror(errno)));
	}
	
	// mount update file device
	i = 0;
	//printf("in DealUpdate(),begin mount\n");
	while(u_asStorageDevice[i].nDeviceNO != -1)
	{
		//printf("mount suorce=%s, dest=%s\n",u_asStorageDevice[i].pDevicePath,u_pcUpdPath, u_pcUpdFileSystem);
		#ifdef	D_ARM
		if(g_wMMCStatus==0)	{// not mounted
			CtmMMCardFile::GetInstance()->Mount(1);
			if(g_wMMCStatus==1)
				nMountStatus = 0;
		}
		else 
			nMountStatus =0;
		#else
			CtmMMCardFile::GetInstance()->Mount(0);
			//nMountStatus = mount(u_asStorageDevice[i].pDevicePath, 
							// u_pcUpdPath, u_pcUpdFileSystem, 0, NULL);
			if(g_wMMCStatus==1)
				nMountStatus = 0;
		#endif
		//printf("nMountStatus == %d\n",nMountStatus);
		if(nMountStatus == 0)
		{
			#ifdef D_ARM
			g_wMMCStatus=1;
			#endif
			//printf("nMountStatus == 0\n");
			//printf("in DealUpdate(), mount done!\n");
			u_bUpdateBegin = FALSE;
			if(u_nUpdateActionFlag == ACTION_RESTORE)	
					nUpdateFile = DealUpdateFile(1);	//ACTION_RESTORE
			else	nUpdateFile = DealUpdateFile(0);	//update
			j = 0;	
			nMountStatus = -1;
			// wait for umount
			while(nMountStatus < 0 && j < 3)
			{
				sleep(2);
				nMountStatus = umount(u_pcUpdPath);
				j++;
			}
		}
		else	u_wUpdateWarningFlag = E_UPDATE_MOUNT;
		if(nUpdateFile != -1)	break;
		i++;
	}

	u_bThreadStopFlag = TRUE;
	sleep(1);
	if((nUpdateFile != -1) && u_bUpdateBegin)
	{
		u_UpdateEndFlag = TRUE;
		//sprintf(msz, "Update end, please restart mmi !!!");
		if(u_pUpdateWindow != NULL)
		{
			//CodeChange(mszMul, msz);
			//u_pUpdateWindow->SetPropValueT("text", mszMul);
			u_pUpdateWindow->SetPropValueT("textID", "VW_UPDATE_END");
			u_pUpdateWindow->Update();
			//MsgBox(g_MultiLanguage["VW_UPDATE_END"],3);	//yuc<080130> add
		}
	}
	else
	{
		u_bUpdateFlag = FALSE;
		//printf("u_bUpdateFlag = FALSE\n");
		sprintf(msz, " ");
	}

	// exit thread of update
	pthread_exit(&u_pthreadupdate);
}

/*---------------------------------------------------------------------------+
|  Function :  DealUpdateFile                                                |
|  Task     :  deal update config file                                       |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
static		int		DealUpdateFile(int	nState)
{
	struct stat	fupdate;
	int			fhandle, i, j, k;
	char*		pcbuffer;
	char		acString[256];
	char		acFileName[256];
	char		msz[80];
	char		mszMul[160];
	DIR			*pDir;
	char		acDirectory[256];

	//printf("in DealUpdateFile(),tmpfile=%s\n",u_pcTempFileName);
	if(nState == 1)	strcpy(acFileName, u_pcTempFileName);
	else
	{
		// create tempconfig file
		if(stat(u_pcTempFileName, &fupdate) == -1)
		{
			if((fhandle = open(u_pcTempFileName ,O_CREAT, 0777)) < 0)	
			{
				PRINTF(printf("Create directroy %s Error:%s\n", 
										u_pcTempFileName, strerror(errno)));
				u_wUpdateWarningFlag  = E_UPDATE_FCREATE;		
				return	-1;
			}
		}
		else
		{
			if((fhandle = open(u_pcTempFileName ,O_TRUNC, 0777)) < 0)	
			{
				PRINTF(printf("Reset directroy %s Error:%s\n", 
										u_pcTempFileName, strerror(errno)));
				u_wUpdateWarningFlag  = E_UPDATE_FOPEN;
				return	-1;
			}
		}
		close(fhandle);
		strcpy(acFileName, u_pcUpdPath);
		strcat(acFileName, u_acUpdateFileName);
	}
	if(stat(acFileName, &fupdate) == -1)	 		return	-1;
	// open update config file
	if((fhandle = open(acFileName, O_RDONLY)) < 0) 	return	-1;
	if((pcbuffer = (char *)malloc(fupdate.st_size)) == NULL)
	{
		close(fhandle);
		u_wUpdateWarningFlag  = E_UPDATE_MEMALLOC;
		return	-1;
	}
	if(read(fhandle, pcbuffer, fupdate.st_size) != fupdate.st_size)
	{
		free(pcbuffer);
		close(fhandle);
		return	-1;
	}
	if(u_pUpdateWindow != NULL)
	{
		//sprintf(msz, "Begin update, please wait ......");
		//CodeChange(mszMul, msz);
		//u_pUpdateWindow->SetPropValueT("text", mszMul);
		u_pUpdateWindow->SetPropValueT("textID","VW_DA_DATAUPDATE");
		u_pUpdateWindow->Update();

	}
	strcpy(acDirectory, u_pcUpdPath);
	strcat(acDirectory, u_acBackUpDirectory);
	//printf("dir=%s\n",acDirectory);
	// open backup directory
	if((pDir = opendir(acDirectory)) == NULL)
	{
		if(mkdir(acDirectory, 0777) < 0) 
		{
			PRINTF(printf("Directory create error: %s\n", strerror(errno)));
			free(pcbuffer);
			close(fhandle);
			u_wUpdateWarningFlag  = E_UPDATE_DCREATE;
			return	-1;
		}
	}
	else	closedir(pDir);
	i = j = 0;
	// deal string of each line
	while(i<fupdate.st_size)
	{
		if(*(pcbuffer+i) == '\\')	acString[j] = '/';
		else						acString[j] = *(pcbuffer+i);
		j++;
		i++;
		if((acString[j-1] == 0x0A) && (j > 1))
		{
			if(acString[j-2] == 0x0D)
			{
			    k = j - 2;
				acString[k] = '\0';
			}
			else
			{
			    k = j - 1;
		        acString[k] = '\0';
		    }
			if(DealString(acString, k, nState) == 1)	break;
			memset(acString, 0, 128);
			j = 0;
		}
	}
	free(pcbuffer);
	close(fhandle);
	return	0;
}

/*---------------------------------------------------------------------------+
|  Function :  DealString                                                    |
|  Task     :  deal string command                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
static		int		DealString(char *pcCommand, int len, int nState)
{
	char	acCommand[256], acTempx[128];
	char	acTemp[256], acFileName[128];
	int		i,j,k;
	char	apacStrings[5][64];
	
	// decompose parameters
	for(i=0; i<5; i++)	apacStrings[i][0] = '\0';
	i = 0;
	j = 0;
	k = 0;
	while(pcCommand != NULL && i < len)
	{
		if(*(pcCommand+i) != ' ' && *(pcCommand+i) != 0x09)
		{
			apacStrings[k][j++] = *(pcCommand+i);
			if(j >= 64)	
			{
				apacStrings[k][j-1] = '\0';
				break;
			}
			else	apacStrings[k][j] = '\0';
		}
		else
		{
			if(j > 0)	
			{
				k++;
				if(k >= 5)	break;
				j = 0;
			}
		}
		i++;
	}

	if(memcmp(apacStrings[0], "[BEG]", 5) == 0)
	{
		if(nState == 0)	DealBackupConfig(apacStrings[0], 5);
		u_bUpdateBegin = TRUE;
		return	0;
	}
	if(memcmp(apacStrings[0], "[END]", 5) == 0)	
	{
		if(nState == 0)	DealBackupConfig(apacStrings[0], 5);
		return	1;
	}
	if(!u_bUpdateBegin)							
	{
		u_wUpdateWarningFlag  = E_UPDATE_NOBEGIN;
		return	-1;
	}
	i = 0;
	while(u_acmdtype[i].pcRealCmd != NULL)
	{
		if(memcmp(apacStrings[0], u_acmdtype[i].pcVirtualCmd,
						strlen(u_acmdtype[i].pcVirtualCmd)) == 0)
		{
			strcpy(acCommand, u_acmdtype[i].pcRealCmd);
			break;
		}
		else	i++;
	}
	if(u_acmdtype[i].pcRealCmd == NULL)			return	-1;
	if(u_acmdtype[i].wCmdType == CMDTYPE_TWOPARA || 
					u_acmdtype[i].wCmdType == CMDTYPE_ONEPARA)
	{
		// save restore config file and save old file
		if(nState == 0)	
		{
			strcpy(acTemp, "[UPD]");
			strcat(acTemp, " ");
			strcat(acTemp, u_pcUpdPath);
			strcat(acTemp, u_acBackUpDirectory);
			if(apacStrings[1][0] != '/') strcat(acTemp, "/");
			GetFileNameFromString(apacStrings[1], acTempx);
			strcat(acTemp, acTempx);//apacStrings[1]);	
			strcat(acTemp, " ");
			strcat(acTemp, apacStrings[2]);
			DealBackupConfig(acTemp, strlen(acTemp));
			// backup old file and directory
			strcpy(acTemp, "cp -Rdf ");
			strcpy(acFileName, apacStrings[2]);						//  strcat(acTemp, apacStrings[2]);
			if(apacStrings[2][strlen(apacStrings[2])-1] == '/')		//	if(apacStrings[2][strlen(apacStrings[2])-1] == '/')
				acFileName[strlen(acFileName)-1] = '\0';			//		acTemp[strlen(acTemp)-1] = '\0';
			if(apacStrings[1][0] != '/') strcat(acFileName, "/");	//	if(apacStrings[1][0] != '/') strcat(acTemp, "/");
			GetFileNameFromString(apacStrings[1], acTempx);
			strcat(acFileName, acTempx);						//	strcat(acTemp, apacStrings[1]);
			strcat(acTemp, acFileName);
			strcat(acTemp, " ");
			strcat(acTemp, u_pcUpdPath);
			strcat(acTemp, u_acBackUpDirectory);
			system(acTemp);
		}
	}
	if(u_acmdtype[i].wCmdType == CMDTYPE_UPKERNEL)
	{
		// update pxa255 kernel
		strcpy(acCommand, u_pcUpdPath);
		strcat(acCommand, u_acUpdatePath);
		strcat(acCommand, apacStrings[1]);
		#ifdef	D_ARM
		return	DealKernelUpdate(acCommand);
		#endif
	}
	else
	{
		//-------------CMDTYPE_TWOPARA----------------------
		if(nState == 0 && u_acmdtype[i].wCmdType == CMDTYPE_TWOPARA)
		{
			strcat(acCommand, u_pcUpdPath);
			strcat(acCommand, u_acUpdatePath);
			if(apacStrings[1][0] == '/')	
				acCommand[strlen(acCommand)-1] = '\0';
		}
		strcat(acCommand, apacStrings[1]);		
		//-------------CMDTYPE_ONEPARA----------------------
		if(u_acmdtype[i].wCmdType != CMDTYPE_ONEPARA) 
			strcat(acCommand, " ");
		//-------------CMDTYPE_TWOPARASP----------------------
		if(u_acmdtype[i].wCmdType == CMDTYPE_TWOPARASP)
		{
			strcat(acCommand, u_pcUpdPath);
			strcat(acCommand, u_acUpdatePath);
			if(apacStrings[2][0] == '/')	acCommand[strlen(acCommand)-1] = '\0';
		}
		//-------------CMDTYPE_TARPARA----------------------
		if(u_acmdtype[i].wCmdType == CMDTYPE_TARPARA)	//for command: tar only
		{
			strcat(acCommand, " -C ");
		}
		if(u_acmdtype[i].wCmdType != CMDTYPE_ONEPARA) 
			strcat(acCommand, apacStrings[2]);
		

		// call system command
		//printf("acCommand=%s",acCommand);
		system(acCommand);
	}
		
	return	0;
}

/*---------------------------------------------------------------------------+
|  Function :  DealBackupConfig                                              |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
static	int			DealBackupConfig(char* pString, int len)
{
	char	cByte 		= 0x0A;
	int		nFileHandle = -1;
	
	if((nFileHandle  = open(u_pcTempFileName, O_WRONLY|O_APPEND)) >= 0)	
	{
		lseek(nFileHandle, 0, SEEK_END);
		write(nFileHandle, pString, len);
		write(nFileHandle, &cByte, 1);
		close(nFileHandle);
	}
	
	return	0;
}

/*---------------------------------------------------------------------------+
|  Function :  DealKernelUpdate                                              |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
static	int			DealKernelUpdate(char *pFileName)
{
	int		nCount, nRemain, nOpSize;
	int		nFlashHandle = -1, nFileHandle = -1;
	char	*pcKernel;
	struct 	stat 	filestat;
	int		j;
	
	if((nFlashHandle = open(u_pcUpdDeviceFlash, O_RDWR)) < 0)		
	{
		u_wUpdateWarningFlag  = E_UPDATE_FOPEN;
		return -1;
	}
	if((nFileHandle  = open(pFileName, O_RDONLY)) < 0)			
	{
		close(nFlashHandle);
		u_wUpdateWarningFlag  = E_UPDATE_FOPEN;
		return -1;
	}
	fstat(nFileHandle, &filestat);
	if(filestat.st_size > (int)(u_dwFlashBlockSize*4) || 
		filestat.st_size < (int)(u_dwFlashBlockSize*2))
	{
		close(nFileHandle);
		close(nFlashHandle);
		u_wUpdateWarningFlag  = E_UPDATE_FSIZE;
		return -1;
	}
	nCount = filestat.st_size/u_dwFlashBlockSize;
	if((nRemain = filestat.st_size%u_dwFlashBlockSize) > 0)	nCount++;
	if((pcKernel = (char *)malloc(u_dwFlashBlockSize)) == NULL)	
	{
		close(nFileHandle);
		close(nFlashHandle);
		u_wUpdateWarningFlag  = E_UPDATE_MEMALLOC;
		return -1;
	}
	for(j=0; j<nCount; j++)
	{
		if(nRemain > 0 && j == (nCount - 1))	nOpSize = nRemain;
		else	nOpSize = u_dwFlashBlockSize;								
		lseek(nFileHandle, j*u_dwFlashBlockSize, 0);
		read(nFileHandle, pcKernel, nOpSize);
		lseek(nFlashHandle, j*u_dwFlashBlockSize, 0);
		write(nFlashHandle, pcKernel, nOpSize);
	}
	free(pcKernel);
	close(nFileHandle);
	close(nFlashHandle);
	
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function :  DealMovie                                                     |
|  Task     :  thread of movie                                               |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
static	void*		DealMovie(void* arg)
{
	WORD	wCount = 0;
	char	msz[80];
	char	mszMul[160];
	int     nSleepCount;
    unsigned    char   bUpdateLED = 0;
	
	// add counter for movie
	while(!u_bThreadStopFlag)
	{
	    if((u_pUpdateWindow != NULL) && u_bUpdateBegin)
		{
		    //sprintf(msz, "Update time %3dsec, please wait ......", ++wCount);
		    sprintf(msz, " %3d ", ++wCount);
			CodeChange(mszMul, msz);
			u_pUpdateWindow->SetPropValueT("textID", "VW_UPDATE_UPDATETIME");
			u_pUpdateWindow2->SetPropValueT("text", mszMul);
			u_pUpdateWindow3->SetPropValueT("textID", "VW_UPDATE_WAIT");
			u_pUpdateWindow->Update();
			u_pUpdateWindow2->Update();
			u_pUpdateWindow3->Update();
		}
		for(nSleepCount=1; nSleepCount<=100; nSleepCount++)
        {
            if(nSleepCount%4 == 0)
            {
                if(bUpdateLED)    _SetLED(11);
                else                        _SetLED(12);
                bUpdateLED = !bUpdateLED;
            }
            else   usleep(500);
            usleep(8000);
        }      
	}
	pthread_exit(&u_pthreadmovie);
}

/*---------------------------------------------------------------------------+
|  Function :  GetUpdateWarningFlag                                          |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD		GetUpdateWarningFlag()
{
	return	u_wUpdateWarningFlag;
}

/*---------------------------------------------------------------------------+
|  Function :  DealCustomName                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
static	int		DealCustomName(long lPassword)
{
	long	lTemp[3];
	char	acTemp[10];
	
	memset(acTemp, 0, 10);
	lTemp[0] = (lPassword / 10000) % 10;
	lTemp[1] = (lPassword / 100  ) % 100;
	lTemp[2] = lPassword % 100;
	if(lTemp[0] >= 36 || lTemp[1] >= 36 || lTemp[2] >= 36)	return -1;
	acTemp[0] = '/';
	for(int i=1; i<4; i++)	acTemp[i] = u_acCustomName[lTemp[i-1]];
	strcpy(u_acUpdateFileName , acTemp);
	strcpy(u_acBackUpDirectory, acTemp);
	strcpy(u_acUpdatePath	  , acTemp);
	strcat(u_acUpdateFileName , u_pcUpdateFileName );
	strcat(u_acBackUpDirectory, u_pcBackUpDirectory);
	strcat(u_acUpdatePath	  , u_pcUpdatePath     );
	if(u_nUpdateActionFlag == ACTION_UPDATEAPP)	
		strcat(u_acUpdateFileName , "1");
	else if(u_nUpdateActionFlag == ACTION_UPDATEKERNEL)	
		strcat(u_acUpdateFileName , "2");
	
	return	0;
} 

/*---------------------------------------------------------------------------+
|  Function :  GetFileNameFromString                                         |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
static	void	GetFileNameFromString(char* pSource, char* pDest)
{
	short	len = 0;
	short	j = 0, i = 0;
	if(pSource == NULL || pDest == NULL)	return;
	while(*(pSource+j) != 0 && j < 128)	j++;
	if(j >= 128)	return;
	len = j;
	while(j && *(pSource+j-1) != '/')
	{
		j--;
		i++;
	}
	if(*(pSource+len) != '/' && *(pSource+len) != 0)	i++;
	j = i;
	while(i)
	{
		*(pDest + i - 1) = *(pSource + len - 1);
		i--;
		len--;
	}	
	*(pDest+j) = 0;
}
