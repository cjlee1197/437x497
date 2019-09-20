/*
 * =====================================================================================
 *
 *       Filename:  ammeter.h
 *
 *    Description:  DL-T645-1997
 *
 *        Version:  1.0
 *        Created:  11/18/2009 08:28:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alien Chen (ac), alien@spectrum-sys.cn
 *        Company:  Techmation. Co., Ltd.
 *
 * =====================================================================================
 */


#ifndef  AMMETER_INC_INC
#define  AMMETER_INC_INC

#include	<pthread.h>
#include	<termios.h>
#include	<semaphore.h>

#define AMMETER_NOTIFY_TOTALPOWER  0x01

#define	AMMETER_FRAME_ADDRLENGTH   6			/*  address len 6 */
#define	AMMETER_SENDDATA_MAXLENGTH 50			/*  master send data len <= 50 */
#define	AMMETER_READDATA_MAXLENGTH 200			/*  master read data len <= 200 */

typedef  void (*NOTIFYERFUNC)(int , void *, int);

typedef struct tagAMMETERFRAME
{
	unsigned char starta;
	unsigned char address[AMMETER_FRAME_ADDRLENGTH];
	unsigned char startb;
	unsigned char control;
	unsigned char len;
	unsigned char data[AMMETER_READDATA_MAXLENGTH];
	unsigned char cs;
	unsigned char end;
} AMMETERFRAME;

/*
 * =====================================================================================
 *        Class:  CAmmeter
 *  Description:  DL-T645-1997
 * =====================================================================================
 */
class CAmmeter
{
	friend void * RecvFunc(void *pClass);
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CAmmeter (char *devicename);                             /* constructor */
		virtual ~CAmmeter ();					                 /* destoryer */
		/* ====================  ACCESSORS     ======================================= */
		int SetNotifyer( NOTIFYERFUNC notifyer);

		/* ====================  MUTATORS      ======================================= */
		/* ====================  OPERATORS     ======================================= */
		int GetTotalPower();
		/* ====================  DATA MEMBERS  ======================================= */
	protected:
		int SetSlaveAddr(unsigned char *address = 0);

	private:
		AMMETERFRAME m_sendframe;
		AMMETERFRAME m_readframe;

		int m_errcount;
		int m_fdcom;
		char m_devicename[64];
		struct termios m_oldtermios;
		
		pthread_t m_rxthreadid;
		NOTIFYERFUNC m_notifyer;
		
		unsigned char m_wakebytes[4];
		unsigned char m_address[AMMETER_FRAME_ADDRLENGTH];
    	unsigned char m_readstate;
		unsigned char m_readcs;
		int m_waitlen;

		sem_t send_sem;
		sem_t read_sem;
	private:
		int InitDevice(char *devicename);		
		int CloseDevice();
		void TransStart();
		void TransOver();
		void SendFrame(AMMETERFRAME *frame);
		void RecvFrame(AMMETERFRAME *frame);
		void ReadData(unsigned char *pData, int len);

}; /* -----  end of class CAmmeter  ----- */

#endif   /* ----- #ifndef AMMETER_INC_INC  ----- */

