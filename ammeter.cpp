/*
 * =====================================================================================
 *
 *       Filename:  ammeter.cpp
 *
 *    Description:  DL-T645-1997
 *
 *        Version:  1.0
 *        Created:  11/21/2009 06:07:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alien Chen (ac), alien@spectrum-sys.cn
 *        Company:  Techmation. Co., Ltd.
 *
 * =====================================================================================
 */

#include	<string.h>
#include	<stdio.h>
#include	<termios.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<errno.h>


#include	"ammeter.h"

#define	AMMETER_FRAME_HEADMARK	   0x68			/*  frame start flag 0x68 */
#define	AMMETER_FRAME_TAILMARK	   0x16			/*  frame end flag 0x16 */
#define	AMMETER_FRAME_WAKEUPMARK   0xFE			/*  wake up flag 0xFE */
#define	AMMETER_FRAME_DATAADD	   0x33  		/*  data added 0x33 */
#define	AMMETER_FRAME_ADDRLENGTH   6			/*  address len 6 */
#define	AMMETER_SENDDATA_MAXLENGTH 50			/*  master send data len <= 50 */
#define	AMMETER_READDATA_MAXLENGTH 200			/*  master read data len <= 200 */

#define	AMMETER_BROADCAST_ADDR	   {0x99, 0x99, \
						0x99, 0x99, 0x99, 0x99}	/*  broadcast addr */


#define AMMETER_COM_READ_ERROR				0		/* error */
#define	AMMETER_COM_READ_READY				1		/* ready */
#define	AMMETER_COM_READ_WAITHEADA			2		/* wait heada */
#define	AMMETER_COM_READ_WAITADDR			3		/* wait addr */
#define	AMMETER_COM_READ_WAITHEADB			4		/* wait headb */
#define	AMMETER_COM_READ_WAITCONTROL		5		/* wait control */
#define	AMMETER_COM_READ_WAITLEN			6		/* wait length */
#define	AMMETER_COM_READ_WAITDATA			7		/* wait data */
#define	AMMETER_COM_READ_WAITCRC			8		/* wait crc */
#define	AMMETER_COM_READ_WAITTAIL			9		/* wait tail */


void *
RecvFunc ( void *pClass);



/*
 *--------------------------------------------------------------------------------------
 *       Class:  CAmmeter
 *      Method:  CAmmeter
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
CAmmeter::CAmmeter ( char *devicename )
{
	m_notifyer				= NULL;
	m_fdcom					= -1;
	m_waitlen				= 0;
	m_readcs				= 0;
	m_errcount				= 0;
	m_readstate				= AMMETER_COM_READ_READY;
	m_sendframe.starta		= AMMETER_FRAME_HEADMARK;
	m_sendframe.startb		= AMMETER_FRAME_HEADMARK;
	m_sendframe.end			= AMMETER_FRAME_TAILMARK;

	memset(m_devicename, 0, sizeof(m_devicename));
	memset(&m_oldtermios, 0, sizeof(termios));	
	memset(m_wakebytes, AMMETER_FRAME_WAKEUPMARK, sizeof(m_wakebytes));
	memset(m_address, 0xAA, sizeof(m_address));

	memcpy(m_sendframe.address, m_address, AMMETER_FRAME_ADDRLENGTH);
	strcpy(m_devicename, devicename);

	sem_init(&send_sem, 0, 0);
	sem_init(&read_sem, 0, 0);
	
	InitDevice(m_devicename);

	if(pthread_create(&m_rxthreadid, NULL, RecvFunc, this) != 0)
	{
		perror("CAmmeter\n");	
	}

	sem_post(&send_sem);
}  /* -----  end of method CAmmeter::CAmmeter  (constructor)  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  CAmmeter
 *      Method:  CAmmeter
 * Description:  destoryer
 *--------------------------------------------------------------------------------------
 */
CAmmeter::~CAmmeter ( )
{
	sem_destroy(&send_sem);
	sem_destroy(&read_sem);
	pthread_cancel(m_rxthreadid);
	CloseDevice();
}  /* -----  end of method CAmmeter::CAmmeter  (destoryer)  ----- */


	int
CAmmeter::InitDevice ( char *devicename )
{
	struct termios newtermios;
	int ret = -1;
	memset(&newtermios, 0, sizeof(termios));

	m_fdcom = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(m_fdcom == -1)
	{
		printf("CAmmeter::InitDevice open %s:%s\n", devicename, strerror(errno));
		return -1;
	}
	ret = fcntl(m_fdcom, F_SETFL, 0/*FASYNC*/);
	if ( ret != 0 ) {
		printf("CAmmeter::InitDevice fcntl %s\n", strerror(errno));
		return ret;
	}
	ret = isatty(m_fdcom);
	if( ret == 0 ) {
		printf("CAmmeter::InitDevice isatty %d %s\n", m_fdcom, strerror(errno));
		return ret;
	}
	
	ret = tcgetattr(m_fdcom, &m_oldtermios);
	if ( ret != 0) {
		printf("CAmmeter::InitDevice tcgetattr %s\n", strerror(errno));
		return ret;
	}
	
	newtermios.c_cflag 		= CLOCAL | CREAD;

	newtermios.c_cflag      &= ~CSIZE;
	newtermios.c_cflag      |= CS8;
	
	newtermios.c_cflag      &= ~CSTOPB;

	newtermios.c_iflag 		= INPCK; 
	newtermios.c_cflag      |= PARENB;
	newtermios.c_cflag      &= ~PARODD;

	
	newtermios.c_cc[VTIME] 	= 5;	//500ms
	newtermios.c_cc[VMIN] 	= 0;

	cfsetispeed(&newtermios, B1200);
	cfsetospeed(&newtermios, B1200);
	tcflush(m_fdcom, TCIOFLUSH);
    ret = tcsetattr(m_fdcom, TCSANOW, &newtermios);
	if ( ret != 0) {
		printf("CAmmeter::InitDevice tcsetattr %s\n", strerror(errno));
		return ret;
	}
	
	return 0;
}		/* -----  end of method CAmmeter::InitDevice  ----- */

	void
CAmmeter::TransStart ( )
{
	m_readstate = AMMETER_COM_READ_WAITHEADA;
	m_waitlen = 1;
	m_readcs = 0;
	sem_post(&read_sem);
	return ;
}		/* -----  end of method CAmmeter::TransStart  ----- */

	void
CAmmeter::TransOver ( )
{
	m_readstate = AMMETER_COM_READ_READY;
	m_waitlen = 0;
	m_readcs = 0;
	sem_post(&send_sem);
	sem_wait(&read_sem);
	return ;
}		/* -----  end of method CAmmeter::TransOver  ----- */

	int
CAmmeter::SetNotifyer ( NOTIFYERFUNC notifyer )
{
	m_notifyer = notifyer;
	return 0;
}		/* -----  end of method CAmmeter::SetNotifyer  ----- */

	int
CAmmeter::CloseDevice ( )
{
	if ( m_fdcom == -1 ) {
		return 0;
	}
	tcsetattr(m_fdcom, TCSANOW,&m_oldtermios);
	
	if ( close(m_fdcom) == -1 ) {
		perror("CloseDevice \n");
		return -1;
	}
	m_fdcom = -1;
	return 0;
}		/* -----  end of method CAmmeter::CloseDevice  ----- */

	int
CAmmeter::SetSlaveAddr ( unsigned char *address )
{
	unsigned char broadcastaddr[AMMETER_FRAME_ADDRLENGTH] = AMMETER_BROADCAST_ADDR;
	
	memcpy(m_sendframe.address, broadcastaddr, AMMETER_FRAME_ADDRLENGTH);

	m_sendframe.control = 0x0A;
	
	if ( address != NULL) {
		memcpy(m_address, address, AMMETER_FRAME_ADDRLENGTH);
	}
	memcpy(m_sendframe.data, m_address, AMMETER_FRAME_ADDRLENGTH);
	m_sendframe.len = AMMETER_FRAME_ADDRLENGTH;

	SendFrame(&m_sendframe);

	TransStart();

	return 0;
}		/* -----  end of method CAmmeter::SetSlaveAddr  ----- */

	int
CAmmeter::GetTotalPower ()
{
	sem_wait(&send_sem);
	
	m_sendframe.control = 0x01;
	m_sendframe.data[0] = 0x10;
	m_sendframe.data[1] = 0x90;
	m_sendframe.len = 2;

	SendFrame(&m_sendframe);
	
	TransStart();
	
	return 0;
}		/* -----  end of method CAmmeter::GetTotalPower  ----- */

	void
CAmmeter::ReadData ( unsigned char *pData, int len )
{
	if ( len < 1 ) {
		//printf("Cann't get data from device !\n");
		m_errcount++;
		if(m_errcount > 4){
			CloseDevice();
			InitDevice(m_devicename);
			m_errcount = 0;
		}
		TransOver();
		return;
	}
	m_errcount = 0;
/*
	printf("recv : %02X \n", *pData);
*/
	while( len > 0 ) {
		switch ( m_readstate ) {
			case AMMETER_COM_READ_WAITHEADA:	
				if( *pData == AMMETER_FRAME_HEADMARK ) {
					m_readframe.starta = *pData;
					m_readstate = AMMETER_COM_READ_WAITADDR;
					m_waitlen = AMMETER_FRAME_ADDRLENGTH;
					m_readcs += *pData;
				}
				else {
					if( *pData != AMMETER_FRAME_WAKEUPMARK) {
						TransOver();
					}
				}
				break;
	
			case AMMETER_COM_READ_WAITADDR:	
				
				if ( *pData <= 0xAA ) {
					m_readframe.address[AMMETER_FRAME_ADDRLENGTH-m_waitlen] = *pData;
					m_waitlen--;
					m_readcs += *pData;
					if ( m_waitlen == 0 ) {
						m_readstate = AMMETER_COM_READ_WAITHEADB;
						m_waitlen = 1;	
					}
				}
				else {
					TransOver();
				}
				break;
	
			case AMMETER_COM_READ_WAITHEADB:	
				if ( *pData == AMMETER_FRAME_HEADMARK ) {
					m_readframe.startb = *pData;
					m_readstate = AMMETER_COM_READ_WAITCONTROL;
					m_waitlen = 1;
					m_readcs += *pData;
				}
				else {
					TransOver();
				}

				break;
	
			case AMMETER_COM_READ_WAITCONTROL:	
				m_readframe.control = *pData;
				m_readstate = AMMETER_COM_READ_WAITLEN;
				m_waitlen = 1;
				m_readcs += *pData;

				break;

			case AMMETER_COM_READ_WAITLEN:	
				if ( *pData <= AMMETER_READDATA_MAXLENGTH) {
					m_readframe.len = *pData;
					if ( m_readframe.len == 0 ) {
						m_readstate = AMMETER_COM_READ_WAITCRC;
						m_waitlen = 1;
					}
					else {
						m_readstate = AMMETER_COM_READ_WAITDATA;
						m_waitlen = m_readframe.len;
					}
					m_readcs += *pData;
				}
				else {
					TransOver();
				}
				break;
	
			case AMMETER_COM_READ_WAITDATA:	
				m_readcs += *pData;
				m_readframe.data[m_readframe.len-m_waitlen] = *pData-AMMETER_FRAME_DATAADD;
				m_waitlen--;
				if ( m_waitlen == 0 ) {
					m_readstate = AMMETER_COM_READ_WAITCRC;
					m_waitlen = 1;
				}
				break;

			case AMMETER_COM_READ_WAITCRC:	
				if ( m_readcs == *pData ) {
					m_readframe.cs = *pData;
					m_readstate = AMMETER_COM_READ_WAITTAIL;
					m_waitlen = 1;
				}
				else {
					TransOver();
				}
				break;

			case AMMETER_COM_READ_WAITTAIL:	
				if ( *pData == AMMETER_FRAME_TAILMARK ) {
					m_readframe.end = *pData;
					RecvFrame(&m_readframe);
				}
				TransOver();
				break;

			default:	
				break;
		}				/* -----  end switch  ----- */
		pData++;
		len--;
	}
	return ;
}		/* -----  end of method CAmmeter::ReadData  ----- */

	void
CAmmeter::RecvFrame ( AMMETERFRAME *readframe )
{
	switch ( readframe->control ) {
		case 0x8A:	
			printf("Set Ammeter Address Success .\n");
			memcpy(m_sendframe.address, m_address, AMMETER_FRAME_ADDRLENGTH);
			break;

		case 0x81:	
			if( readframe->data[0] == 0x10 && 
					readframe->data[1] == 0x90) {
				double power = 0;
				double temp = 0.01;
				for ( int i=0; i<4; i++) {
					power += (readframe->data[i+2]&0xF)*temp;
					temp *= 10;
					power += ((readframe->data[i+2]>>4)&0xF)*temp;
					temp *= 10;
				}
				m_notifyer(AMMETER_NOTIFY_TOTALPOWER, &power, sizeof(double));
			}
			break;

		case 3:	
			break;

		case 4:	
			break;

		default:	
			break;
	}				/* -----  end switch  ----- */
	return ;
}		/* -----  end of method CAmmeter::ReadFrame  ----- */


	void
CAmmeter::SendFrame ( AMMETERFRAME *sendframe )
{
	unsigned char *pData = (unsigned char *)sendframe;
	unsigned char CS = 0;
	int sendlen = 1 + AMMETER_FRAME_ADDRLENGTH + 1 + 1 + 1 + sendframe->len + 1 + 1;

	for ( int i=0; i<sendframe->len; i++) {
		*(pData+4+AMMETER_FRAME_ADDRLENGTH+i) = *(pData+4+AMMETER_FRAME_ADDRLENGTH+i) + AMMETER_FRAME_DATAADD;
	}
	
	for ( int i=0; i<sendlen-2; i++) {
		CS += *pData++;
	}
	*pData++ = CS;
	*pData = sendframe->end;
	
	write(m_fdcom, m_wakebytes, 3);
	write(m_fdcom, (unsigned char *)sendframe, sendlen);
/*
    printf("send : ");
    printf("%02X %02X %02X ", m_wakebytes[0], m_wakebytes[1], m_wakebytes[2]);
    for(int i=0; i<sendlen; i++)
    {
        printf("%02X ", *((unsigned char *)sendframe+i));
    }
    printf("\n");
*/
	return ;
}		/* -----  end of method CAmmeter::SendFrame  ----- */

	void *
RecvFunc ( void *pClass)
{
	unsigned char buff;
	int len = 0;
	
	pthread_detach(pthread_self());
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0) {
		perror("RecvFunc setcanceltype");
	}

	while ( 1 ) {
		len = read(((CAmmeter *)pClass)->m_fdcom, &buff, 1);
		((CAmmeter *)pClass)->ReadData(&buff, len);
	}

	return NULL;
}		/* -----  end of function RecvFunc  ----- */
