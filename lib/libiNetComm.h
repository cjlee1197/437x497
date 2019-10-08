/*===========================================================================+
|  Class    :  		                                             			 |
|  Task     :  			                         							 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 01/08/2009	                                                 |
|  Revision : 							                                     |
+===========================================================================*/
#ifndef	D_LIBINETCOMM
#define	D_LIBINETCOMM

typedef	struct tagINETDATE
{
	WORD	wYear;
	BYTE	byMonth;
	BYTE	byDay;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	INETDATE;
#else 
#ifdef	D_X86
INETDATE;
#endif
#endif

typedef	struct tagINETTIME
{
	BYTE	byHour;
	BYTE	byMinute;
	WORD	wMilliSecond;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	INETTIME;
#else 
#ifdef	D_X86
INETTIME;
#endif
#endif

typedef	struct tagINETDATETIME
{
	INETDATE	iNetDate;
	INETTIME	iNetTime;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	INETDATETIME;
#else 
#ifdef	D_X86
INETDATETIME;
#endif
#endif


//=========================================================================================
//	the relative inet's id
//=========================================================================================

#define		INET_SEND_STATIC_REQIP						0x01000001
#define		INET_SEND_STATIC_ERROR						0x01000000
#define		INET_SEND_STATIC_REQTIMER					0x01000002
#define		INET_SEND_STATIC_TIMERFLAG					0x01010002
#define		INET_SEND_STATIC_ONLINE						0x01000003
#define		INET_SEND_STATIC_VERSION					0x01000004
#define		INET_SEND_STATIC_UPDATESTART				0x01000005
#define		INET_SEND_STATIC_PROLIST					0x01010005
#define		INET_SEND_STATIC_PROTOCOL					0x01030005
#define		INET_SEND_STATIC_UPDATEEND					0x01040005
#define		INET_SEND_STATIC_READDATA					0x01000006
#define		INET_SEND_STATIC_WRITEDATA					0x01000007
#define		INET_SEND_STATIC_DATADIC					0x01000008
#define		INET_SEND_STATIC_DATAMAP					0x01000009


#endif
