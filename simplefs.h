#ifndef SIMPLEFS_H
#define SIMPLEFS_H

#include <iostream>
#include <string>
#include <unistd.h>

//#define DBUG 0x1
#define	ROOT_BLOCK		5
#define	BLOCKS_PER_FILE		1

#define	TMSIMPLEFS_MAGIC 	0xDA34

#define	TMSIMPLEFS_BLOCKSIZE	512

#define	TMSIMPLEFS_NR_OPEN 	128
#define	TMSIMPLEFS_NR_FILE 	20
#define	TMSIMPLEFS_NAME_LEN	32
#define	TMSIMPLEFS_PATH_LEN	64

#define	E_DEFAULT		1
#define	E_MEMORY		2
#define	E_RDWR			3
#define	E_OPEN			4
#define	E_READ			5
#define	E_WRITE			6
#define	E_DEL			7

#define EDEFAULT		1
#define	EOPEN			2
#define ECLOSE			3
#define	EREAD			4
#define	EWRITE			5
#define	ELSEEK			6
#define	EREMOVE			7

#define	SEEKSET			0
#define	SEEKCUR			1
#define	SEEKEND			2

#define BITS_PER_BYTE		8	// bits per bytes
#define	BLOCKS_SUPER		1	// blocks owned by super block

#define	B_CREATE		1

#define O_CREATE		0100
#define	O_READ			00
#define	O_WRITE			01

typedef struct tagSimpleFsSuperBlock
{
	unsigned short s_ninodes;	
	unsigned short s_nzones;
	unsigned short s_imap_blocks;		
	unsigned short s_zmap_blocks;
	unsigned short s_inode_blocks;
	unsigned short s_firstdatazone;
	unsigned short s_magic;
	unsigned long s_max_size;
} tmSuperBlock;

typedef struct tagSimpleFsInode		// 8
{
	unsigned long  i_size;
	unsigned short i_zone;
} tmInode;

typedef struct tagSimpleFsFat_item	// 2
{
	unsigned short zone;
} tmFatItem;

typedef struct tagSimpleFsOpen_item
{
	tmInode *inode;
	char name[TMSIMPLEFS_NAME_LEN];
	unsigned short count;
	pthread_rwlock_t  rw_lock;
}tmOpenItem;

typedef struct tagSimpleFsHandle
{
	unsigned long pos;
	unsigned short item;
} tmHandle;

class CtmBufferManage
{
	public:
		CtmBufferManage(int fd, unsigned long start, unsigned long end);
		~CtmBufferManage();
	private:
		int m_iDevFd;
		long m_lStartAddr, m_lEndAddr;
		
		int m_iInode_pos, m_iFatPos;		// imap_pos, zmap_pos;
//		int inode_blks, fat_blks, imap_blks, zmap_blks;
		tmSuperBlock *m_pSuper;
		char *m_pImap, *m_pZmap;
		tmInode *m_pInodes;
		int m_iImap_bytes, m_iZmap_bytes; 	// for bit operation
		char *m_pRoot;				// root directory
		unsigned short *m_pFat;
		
		pthread_mutex_t inode_lock;
		pthread_mutex_t block_lock;
	public:
		int ReadBuf(int zone, void *buf, int n);
		int WriteBuf(int zone, const void *buf, int n);

		int GetSuper(tmSuperBlock *super);
		
		char *GetRoot();
		int PutRoot();
		
		tmInode *GetInode(int  );
		int PutInode(void);
		
		unsigned short *GetFat();
		int PutFat();
		
		int NewInode(tmInode **);
		int FreeInode(int );
		
		int NewBlock(void);
		int FreeBlock(int);
		
		int BufferInit(void);
		void BufferExit(void);
		
		/* bit manipulation */
		int FindFirstZero(char *s, int size);
		void SetBit(char *s, int pos);
		void ClearBit(char *s, int pos);
		
		static int WriteToDev(int fd, const void *buf, int count);
		static int ReadFromDev(int fd, void *buf, int count);
		static long Seek(int fd, long offset, int origin);
		
		void ListFS(void);
		int CalBit(char *,  int);		
};

class CtmSimpleFS
{
	public:
		CtmSimpleFS();
		CtmSimpleFS(int dev_fd, unsigned long start, unsigned long end);
		~CtmSimpleFS();
	public:
		int tmInitSimpleFS(int dev_fd, unsigned long start, unsigned long end);
		/* open a file */
		int f_open(const char *, int flag );
		/* remove a file */
		int f_remove(char *);
		/* close an opened file */
		int f_close(int);
		/* read from a file to buf */
		int f_read(int fd, char *buf, int count);
		/* write from buf to file */
		int f_write(int fd, const char *buf, int count);
		/* seek position in a file */
		int f_lseek(int fd, long ofFSet, int origin);
		
		int f_truncate(char *name);
		
		int f_create(const char *name);

		int f_format(void);
		
		/* list the files in filesystem */
		void list(void); 
	private:
		CtmBufferManage *m_pBufferManage;
		long m_lStartAddr, m_lEndAddr;
		int m_iDevFd;
		
		tmOpenItem m_OpenItemArray[TMSIMPLEFS_NR_FILE];
		tmHandle m_HandleArray[TMSIMPLEFS_NR_OPEN];
		
	private:
		
		
		/* enter the FS, if not format the device */
		
		int EnterFS(int fd, unsigned long start, unsigned long end);
	
		/* exit the file system */
		void ExitFS(void);
		
		/* format */
		int Format(int m_iDevFd, unsigned long start, unsigned long end);
		
		/* read && write */
		int w_file(tmInode *, tmHandle*, const char *, int);
		int r_file(tmInode *, tmHandle*, char *, int);
		
		/* change file block No. to disk block No. */
		int CreateBlock(tmInode *inode, int block);
		int Bmap(tmInode *inode, int block);
		int _bmap(tmInode *inode, int block, int create);
		
		void SetBit(char *s, int pos);
		
		int OpenName(const char *name, int flag, tmInode ** res_inode);
		int f_unlink(char *name);
		
};


#endif
		
