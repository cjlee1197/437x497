#include "simplefs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define  E_DEFAULT 1
CtmBufferManage::CtmBufferManage(int fd, unsigned long start, unsigned long end)
{	
	m_iDevFd = fd;
	m_lStartAddr = start; m_lEndAddr = end;
	
	m_pImap = NULL;
	m_pZmap = NULL;
	m_pInodes = NULL;
	m_pRoot = NULL;			// root directory
	m_pFat = NULL;
	m_pSuper = NULL;
	
	BufferInit();
	pthread_mutex_init(&inode_lock, NULL);	
	pthread_mutex_init(&block_lock, NULL);
}
CtmBufferManage::~CtmBufferManage()
{
	pthread_mutex_destroy(&block_lock);
	pthread_mutex_destroy(&inode_lock);
	
	BufferExit();
}

/* bit manipulation */
void CtmBufferManage::SetBit(char *s, int pos)
{
	int i = 0, j = 0;

	if ((!s) || (pos <= 0))
		return;
	
	i = pos / 8;
	j = pos % 8;
	if (j > 0)
		s[i] = s[i] | (0x1 << (8-j));
	else
		s[i-1] = s[i-1] | 0x1;
}

void  CtmBufferManage::ClearBit(char *s, int pos)
{
	int i = 0, j = 0;

	if ((!s) || (pos <= 0))
		return;

	i = pos / 8;
	j = pos % 8;
	if (j > 0)
		s[i] = s[i] & ~(0x1 << (8-j));
	else
		s[i-1] = s[i-1] & ~0x1;
}

int CtmBufferManage::FindFirstZero(char *s, int n)
{
	int i = 0, k = 0, j = 0;
	
	if ((!s) || (n <= 0))
		return -E_DEFAULT;

	for (i = 0; i < n; i++) {
		for(k=0; k<8; k++) {
			j++;
			if (((s[i] >> (7-k)) & 0x1) == 0)
				return j;
		}
	}
	return 0;
}

int CtmBufferManage::ReadBuf(int zone, void *buf, int n)
{
	long pos = 0;
	int fd = m_iDevFd;
	
	pos = zone * TMSIMPLEFS_BLOCKSIZE + m_lStartAddr;
	if (Seek(fd, pos, 0) < 0)
		return -E_DEFAULT;
		
	return ReadFromDev(fd, buf, n );

}

int CtmBufferManage::WriteBuf(int zone, const void *buf, int n)
{	
	int fd = m_iDevFd;
	long pos = 0;
	
	pos = zone * TMSIMPLEFS_BLOCKSIZE + m_lStartAddr;
	if (Seek(fd, pos, 0) < 0)
		return -E_DEFAULT;
		
	return WriteToDev(fd, buf, n );

}

int CtmBufferManage::GetSuper(tmSuperBlock *m_pSuper)
{
	return ReadBuf(0, m_pSuper, sizeof(tmSuperBlock));
}

char *CtmBufferManage::GetRoot()
{
	return m_pRoot;
}

int CtmBufferManage::PutRoot()
{
	return WriteBuf(m_pSuper->s_firstdatazone, m_pRoot, TMSIMPLEFS_BLOCKSIZE * ROOT_BLOCK);
}

int CtmBufferManage::NewBlock(void)
{
	int n = 0;
	pthread_mutex_lock(&block_lock);
	n = FindFirstZero(m_pZmap, m_iZmap_bytes);
	
	SetBit(m_pZmap, n);
	pthread_mutex_unlock(&block_lock);
	
	WriteBuf(BLOCKS_SUPER+1, m_pZmap, m_iZmap_bytes);
	return n;
}
int CtmBufferManage::FreeBlock(int n)
{
	ClearBit(m_pZmap, n);
	WriteBuf(BLOCKS_SUPER+1, m_pZmap, m_iZmap_bytes);
	return n;
}

int CtmBufferManage::NewInode(tmInode **s)
{
	int n = 0;
	pthread_mutex_lock(&inode_lock);
	n = FindFirstZero(m_pImap, m_iImap_bytes);

	SetBit(m_pImap, n);
	pthread_mutex_unlock(&inode_lock);
	WriteBuf(BLOCKS_SUPER , m_pImap, m_iImap_bytes);
	*s = m_pInodes + n - 1;
	memset(*s, 0, sizeof(tmInode));
	return n;
}
int CtmBufferManage::FreeInode(int n)
{
	tmInode *p = NULL;
	
	ClearBit(m_pImap, n);
	WriteBuf(BLOCKS_SUPER , m_pImap, m_iImap_bytes);
	p = m_pInodes+n-1;
	p->i_size = 0;
	p->i_zone = 0;
	
	return 0;
}

/* inode operations */	
tmInode * CtmBufferManage::GetInode(int n)
{
	return (n < 1 || n > m_pSuper->s_ninodes || !m_pInodes) ? NULL : (m_pInodes + n - 1);
}
	
int CtmBufferManage::PutInode(void)
{
	return WriteBuf(m_iInode_pos, m_pInodes, m_pSuper->s_ninodes * sizeof(tmInode));
}

unsigned short * CtmBufferManage::GetFat()
{	
	return m_pFat;
}
int CtmBufferManage::PutFat()
{
	return WriteBuf(m_iFatPos, m_pFat, m_pSuper->s_nzones * sizeof(unsigned short));
}

int CtmBufferManage::BufferInit(void)
{
	
	if (!(m_pSuper = new tmSuperBlock))
		return -E_MEMORY;
	if (GetSuper(m_pSuper) <= 0)
	{	
		delete m_pSuper;
		return -E_DEFAULT;	
	}
	
	m_iInode_pos = BLOCKS_SUPER + m_pSuper->s_imap_blocks + m_pSuper->s_zmap_blocks;
	m_iFatPos = BLOCKS_SUPER + m_pSuper->s_imap_blocks + 
		m_pSuper->s_zmap_blocks  + m_pSuper->s_inode_blocks;
	m_iImap_bytes =  m_pSuper->s_ninodes / BITS_PER_BYTE+1;  
	m_iZmap_bytes= m_pSuper->s_nzones / BITS_PER_BYTE;
	
	m_pInodes = new tmInode[m_pSuper->s_ninodes * sizeof(tmInode)];
	m_pImap = new char[m_pSuper->s_ninodes / BITS_PER_BYTE];		
	m_pZmap = new char[m_pSuper->s_nzones / BITS_PER_BYTE];		
	m_pRoot = new char[TMSIMPLEFS_BLOCKSIZE*ROOT_BLOCK];
	m_pFat = new unsigned short[m_pSuper->s_nzones];
	
	if (!m_pFat || !m_pRoot || !m_pZmap || !m_pImap || !m_pInodes ||
		(ReadBuf(m_iInode_pos,m_pInodes, m_pSuper->s_ninodes * sizeof(tmInode)) <= 0) ||
		(ReadBuf(1, m_pImap, m_iImap_bytes) <= 0) ||
		(ReadBuf(BLOCKS_SUPER + m_pSuper->s_imap_blocks, m_pZmap,  m_iZmap_bytes) <= 0) ||
		(ReadBuf(m_pSuper->s_firstdatazone, m_pRoot, TMSIMPLEFS_BLOCKSIZE * ROOT_BLOCK) <= 0) ||
		(ReadBuf(m_iFatPos, m_pFat, m_pSuper->s_nzones * sizeof(unsigned short)) <= 0) )
	{
		BufferExit();
		return -E_MEMORY;
	}
				
	return 0;
}
void CtmBufferManage::BufferExit(void)
{	
		delete m_pSuper;  	m_pSuper = NULL;
		delete [ ]m_pInodes; 	m_pInodes = NULL;
		delete [ ]m_pImap;	m_pImap = NULL;	
		delete [ ]m_pZmap; 	m_pZmap = NULL;
		delete [ ]m_pRoot;	m_pRoot = NULL;
		delete [ ]m_pFat;	m_pFat = NULL;
}

int CtmBufferManage::WriteToDev(int fd, const void *buf, int count)
{
	return write(fd, buf, count);
}		
int CtmBufferManage::ReadFromDev(int fd, void *buf, int count)
{
	return read(fd, buf, count);
}
long CtmBufferManage::Seek(int fd, long offset, int origin)
{
	return lseek(fd, offset, origin);
}

void CtmBufferManage::ListFS(void)
{
	char *root = NULL, *p = NULL, name[33];
	unsigned short *fat = NULL;
	int len = 0,  n = 0;
	unsigned short node_no = 0;
	unsigned short *tmp = NULL;
	tmInode *inode = NULL;
	tmSuperBlock super;
	fat = GetFat();
	GetSuper(&super);
	p = root = GetRoot();
	printf("name\t\t\tnode_no\tsize\tzone\n");
	while((*p != 0) && ((p-root) <= TMSIMPLEFS_BLOCKSIZE*ROOT_BLOCK))
	{
		len = *p++;
		printf("len, %d\n",len);	
		strncpy(name, p, 33);
		name[33] = '\0';
		p += len;
	
		memcpy(&node_no, p, 2);
		
		inode = GetInode(node_no);
		
		printf("%-16s\t %d\t%ld\t", name, node_no, inode->i_size);
	
	 	n = inode->i_zone;
 	
 		while (n != 0)
 		{	
 			printf("%d->", n);
 			tmp = fat+n;
 			n = *tmp;
 		}
 		printf("\n");
		p += 2;
	}
	n = CalBit(m_pZmap, m_iZmap_bytes);
	printf("FS used: %.1fk\tfile used: %.1fk\tavailable: %.1fk\n", 
		(float)(super.s_firstdatazone+1) / 2, (float)(n - super.s_firstdatazone-1)/ 2 ,(float) (m_iZmap_bytes * 8 - n)/2);
}
int CtmBufferManage::CalBit(char *s, int bytes)
{	
	int k = 0, i = 0, j = 0;
	
	for (i=0; i<bytes; i++)
	{
		for (j=0; j<8; j++)
		{
			if((s[i] >> j) & 0x1)
			k++;
		}
	}			
	return k;
}
