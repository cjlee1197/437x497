#include "simplefs.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define MAX_DEV_SIZE	2048*1024
#define MIN_DEV_SIZE	64*1024

#define ACCESS(x)	CtmBufferManage::Seek(m_iDevFd, m_lStartAddr + (x) * TMSIMPLEFS_BLOCKSIZE, 0)
CtmSimpleFS::CtmSimpleFS()
{
	m_pBufferManage = NULL;
}
int CtmSimpleFS::tmInitSimpleFS(int fd, unsigned long start, unsigned long end)
{
	if (m_pBufferManage != NULL)
		delete m_pBufferManage;
	
	int i = 0;
	/* init the opened file list */
	for (i=0; i<TMSIMPLEFS_NR_FILE; i++) 
	{
		m_OpenItemArray[i].inode = NULL;
		strcpy(m_OpenItemArray[i].name, "");
		m_OpenItemArray[i].count = 0;
	}
	
	/* init the handle pool */
	for (i=0; i<TMSIMPLEFS_NR_OPEN; i++) 
	{
		m_HandleArray[i].item = TMSIMPLEFS_NR_FILE;
		m_HandleArray[i].pos = 0;
	}
	return EnterFS(fd, start, end);
}

CtmSimpleFS::CtmSimpleFS(int fd, unsigned long start, unsigned long end)
{	
	m_pBufferManage = NULL;
	tmInitSimpleFS(fd, start, end);
}

CtmSimpleFS::~CtmSimpleFS()
{
	ExitFS();
}

int CtmSimpleFS::EnterFS(int fd, unsigned long start, unsigned long end)
{

	int ret = 0;
	tmSuperBlock s;
	
	m_iDevFd = fd;
	m_lStartAddr=start; 
	m_lEndAddr = end;
	
	if ((m_iDevFd == -1) || (end-start) > MAX_DEV_SIZE || (end-start) < MIN_DEV_SIZE) 
	{
		fprintf(stderr,"EnterFS: can't open %d\n", fd);
		return -EOPEN;
	}
	ACCESS(0);
	if (CtmBufferManage::ReadFromDev(m_iDevFd, &s, sizeof(tmSuperBlock)) <=  0)
		return -E_DEFAULT;			
	if (s.s_magic != TMSIMPLEFS_MAGIC) 
	{
		if ((ret = Format(m_iDevFd, m_lStartAddr, m_lEndAddr)) != 0)
			return ret;
	}
	m_pBufferManage = new CtmBufferManage(m_iDevFd, m_lStartAddr, m_lEndAddr);	
	
	return 0;
}

void CtmSimpleFS::ExitFS()
{
	if (m_pBufferManage != NULL)
		delete m_pBufferManage;
	//close(m_iDevFd);
}

void CtmSimpleFS::SetBit(char *s, int pos)
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

/* format */
int CtmSimpleFS::f_format(void)
{
	int ret = 0;

	ExitFS();
	if ((ret = Format(m_iDevFd, m_lStartAddr, m_lEndAddr)) != 0)
		return ret;

	int i = 0;
	/* init the opened file list */
	for (i=0; i<TMSIMPLEFS_NR_FILE; i++) 
	{
		m_OpenItemArray[i].inode = NULL;
		strcpy(m_OpenItemArray[i].name, "");
		m_OpenItemArray[i].count = 0;
	}
	
	/* init the handle pool */
	for (i=0; i<TMSIMPLEFS_NR_OPEN; i++) 
	{
		m_HandleArray[i].item = TMSIMPLEFS_NR_FILE;
		m_HandleArray[i].pos = 0;
	}

	m_pBufferManage = new CtmBufferManage(m_iDevFd, m_lStartAddr, m_lEndAddr);	

	return 0;
}

int CtmSimpleFS::Format(const int fd, const unsigned long start, const unsigned long end)
{
#ifdef DBUG
	printf("start Format\n");
#endif
	unsigned long lDiskSize = 0;
	int iDiskBlocks = 0, iInodeBlocks = 0;
	int iInodes = 0, iInodesPerBlock = 0;
	int iFatBlocks = 0;
	int i = 0;
	tmSuperBlock superBlock;

	m_lStartAddr = start;          
	lDiskSize = end - start;
	if ((lDiskSize > MAX_DEV_SIZE) || lDiskSize < MIN_DEV_SIZE) 
	{
		fprintf(stderr,"Format: disk size %ld is too large or to small!\n", lDiskSize);
		return -E_DEFAULT;
	}
	iDiskBlocks = lDiskSize / TMSIMPLEFS_BLOCKSIZE;	// size of whole disk
	iInodes = iDiskBlocks/BLOCKS_PER_FILE;		// The number of iInodes the disk need
	iInodesPerBlock = TMSIMPLEFS_BLOCKSIZE / sizeof(tmInode);
	iInodeBlocks = ((iInodes % iInodesPerBlock) > 0) ? ((iInodes / iInodesPerBlock) + 1) :
		(iInodes / iInodesPerBlock);		// the blocks owned by iInodes

	
	iFatBlocks = iDiskBlocks / (TMSIMPLEFS_BLOCKSIZE / sizeof(unsigned short));
	if (iDiskBlocks % (TMSIMPLEFS_BLOCKSIZE / sizeof(unsigned short)) != 0)
		iFatBlocks++;				// the blocks owned by fat table

	/* write m_pImap */
	char buf[TMSIMPLEFS_BLOCKSIZE];

	bzero(buf, TMSIMPLEFS_BLOCKSIZE);
	SetBit(buf,1);
	ACCESS(1);
	if (TMSIMPLEFS_BLOCKSIZE != CtmBufferManage::WriteToDev(m_iDevFd, buf, TMSIMPLEFS_BLOCKSIZE))
	{
		return -E_RDWR;
	}
	
	/* write zmap */
	bzero(buf, TMSIMPLEFS_BLOCKSIZE);
	for (i=1; i<=iInodeBlocks + 3 + iFatBlocks + ROOT_BLOCK; i++)	
		SetBit(buf, i);
	ACCESS(2);
	if (CtmBufferManage::WriteToDev(m_iDevFd, buf, TMSIMPLEFS_BLOCKSIZE) != TMSIMPLEFS_BLOCKSIZE)
	{
		return -E_RDWR;
	}

	/* root directory */
	char buf_tmp[TMSIMPLEFS_BLOCKSIZE * ROOT_BLOCK];
	bzero(buf_tmp, TMSIMPLEFS_BLOCKSIZE * ROOT_BLOCK);
	ACCESS(iFatBlocks + iInodeBlocks + 3);
	if (CtmBufferManage::WriteToDev(m_iDevFd, buf_tmp, TMSIMPLEFS_BLOCKSIZE * ROOT_BLOCK) != TMSIMPLEFS_BLOCKSIZE * ROOT_BLOCK)
	{
		return -E_RDWR;
	}
		
	/* fat table */
	char buf1[iFatBlocks * TMSIMPLEFS_BLOCKSIZE];
	bzero(buf1, sizeof(buf1));
	if (!buf)
		return -E_MEMORY;
	ACCESS(iInodeBlocks+3);
	if (CtmBufferManage::WriteToDev(m_iDevFd, buf1, iFatBlocks * TMSIMPLEFS_BLOCKSIZE) != iFatBlocks * TMSIMPLEFS_BLOCKSIZE)
	{
		return -E_RDWR;
	}
	
	/* iInodes */
	tmInode inode;
	inode.i_size = TMSIMPLEFS_BLOCKSIZE;
	inode.i_zone = iInodeBlocks + 3 + iFatBlocks;
	ACCESS(3);
	if (CtmBufferManage::WriteToDev(m_iDevFd, &inode, sizeof(inode)) != sizeof(inode))
		return -E_RDWR;

	superBlock.s_ninodes = iInodes;
	superBlock.s_nzones = iDiskBlocks;
	superBlock.s_imap_blocks = 0x1;
	superBlock.s_zmap_blocks = 0x1;
	superBlock.s_inode_blocks = iInodeBlocks;
	superBlock.s_firstdatazone = iInodeBlocks + 3 + iFatBlocks;
	superBlock.s_magic = TMSIMPLEFS_MAGIC;
	superBlock.s_max_size = 0;
	ACCESS(0);
	if (CtmBufferManage::WriteToDev(m_iDevFd, &superBlock, sizeof(superBlock)) != sizeof(superBlock))
		return -E_RDWR;
#ifdef DBUG
	printf("\n%d inodes\n", superBlock.s_ninodes);
	printf("%d blocks\n", superBlock.s_nzones);
	printf ("%d inode blocks\n", superBlock.s_inode_blocks);
	printf("Fisrtdatazone=%d\n", superBlock.s_firstdatazone);
	printf("Zonesize=%d\nzmap_blocks=%d\n", TMSIMPLEFS_BLOCKSIZE, 0x1);
	printf("Maxsize=%ld\n\n", superBlock.s_max_size);

	printf("end Format\n");	
#endif

	return 0;
}

int CtmSimpleFS::OpenName(const char *name, int flag, tmInode ** res_inode)
{
	char *p = NULL, *tmp = NULL,*root =NULL;
	int  len = 0;
	unsigned short  inode_no = 0;
	
	if (m_iDevFd == -1)
	{
		fprintf(stderr, "device fd error!\n");
		return -E_OPEN;	
	}
	p = root = m_pBufferManage->GetRoot();
	len = strlen(name)+1;
	if (!strlen(name) || strlen(name) > TMSIMPLEFS_NAME_LEN || !strcmp(name,"") || !p)
	{	
		fprintf(stderr, "openname: %s error or root dir err\n ", name);
		return -E_OPEN;
	}

	while (*p != 0)
	{
		tmp = p++;

		if (!memcmp(p, name, (len > *tmp) ? len : *tmp))
		{
			p += *tmp;
			
			memcpy(&inode_no, p, sizeof(inode_no));

			if (inode_no != 0)
				*res_inode = m_pBufferManage->GetInode(inode_no);
			break;
		}
		else 
			p += (*tmp + 2);	
	}
	
	if ((flag & O_CREATE) && (inode_no == 0) &&
		(( TMSIMPLEFS_BLOCKSIZE*ROOT_BLOCK - (p - root)) > (int)(strlen(name)+3)))
	{
		*p++ = (char )strlen(name)+1; 

		memcpy(p, name, strlen(name));

		p += strlen(name);
		*p++ = '\0';
		inode_no = m_pBufferManage->NewInode(res_inode);

		memcpy(p, &inode_no, sizeof(inode_no));
		if (inode_no == 0)
			fprintf(stderr, "have no more inodes\n");	
	}
	m_pBufferManage->PutRoot();
	return (inode_no == 0) ? -E_OPEN : inode_no;
}

int CtmSimpleFS::f_open(const char *name, int flag)
{
	tmInode *inode = NULL;
	tmOpenItem *open_table = NULL;
	//char name[32];
	int i = 0, fd = 0, err = 0;
	

	if (m_iDevFd == -1)
		return -EOPEN;
		
	if ((i = OpenName(name, flag  & O_CREATE, &inode)) < 0)
	{
		fprintf(stderr, "open: geti  %d err!\n", i);
		return i;
	}

	for (fd = 0; fd < TMSIMPLEFS_NR_OPEN; fd++)
		if (m_HandleArray[fd].item == TMSIMPLEFS_NR_FILE)
			break;
	if (fd >= TMSIMPLEFS_NR_OPEN)
	{	
		fprintf(stderr, "open:  fd = %d is wrong\n", fd);
		return -EOPEN;
	}
	open_table = m_OpenItemArray;
	for (i=0; i<TMSIMPLEFS_NR_FILE; i++)
	{
		if (!(strcmp(m_OpenItemArray[i].name, name)))
		{
			open_table->count++;
			m_HandleArray[fd].item = i;
			return fd;
		}
	}
	
	for (i=0; i<TMSIMPLEFS_NR_FILE; i++,open_table++)
		if (!open_table->count)
			break;
			
	if (i >= TMSIMPLEFS_NR_FILE)
	{	
		fprintf(stderr, "open: file No = %d error!\n", i);
		return -EOPEN;
	}
	open_table->inode = inode;
	strcpy(open_table->name, name);
	open_table->count++;
	m_HandleArray[fd].item = i;
	
	err = pthread_rwlock_init(&m_OpenItemArray[i].rw_lock, NULL);

	if (err != 0)
		return err;
	if (flag & O_WRITE)
		pthread_rwlock_wrlock(&m_OpenItemArray[m_HandleArray[fd].item].rw_lock);	
	else
		pthread_rwlock_rdlock(&m_OpenItemArray[m_HandleArray[fd].item].rw_lock);

	return fd;
}

int CtmSimpleFS::f_create(const char *name)
{
	return f_open(name, O_WRITE | O_CREATE);
}

int CtmSimpleFS::f_remove(char *name)
{
	int ret = 0;
	ret = f_truncate(name);
	if (ret != 0)
		return ret;
	return f_unlink(name);
}

int CtmSimpleFS::f_truncate(char *name)
{
	tmInode *inode = NULL;
	unsigned short *fat = NULL;
	int ret = 0, n = 0;
	
	ret = OpenName(name, 0, &inode);
	if (ret < 0)
		return ret;	
	fat = m_pBufferManage->GetFat();


	n = inode->i_zone;
	inode->i_zone = 0;
	inode->i_size = 0;
	m_pBufferManage->FreeBlock(n);
	
	while(fat[n] != 0)
	{	
		ret = fat[n];
		fat[n] = 0;
		n = ret;
		
		m_pBufferManage->FreeBlock(n);
	}
	m_pBufferManage->PutInode();
	m_pBufferManage->PutFat();
	return 0;
}

int CtmSimpleFS::f_unlink(char *name)
{
	char *root = NULL, *p = NULL, *tmp = NULL;
	int i_len = 0, i = 0;
	
	p = root = m_pBufferManage->GetRoot();
		
	for (i=0; i<TMSIMPLEFS_NR_FILE; i++)
		if (!strcmp(name, m_OpenItemArray[i].name))
			return -EREMOVE;
	
	if (!strlen(name) || strlen(name) > TMSIMPLEFS_NAME_LEN|| !strcmp(name,"") || !p)
		return -E_DEL;
	while (*p != 0)
	{
		tmp = p++;
		if (!strncmp(p, name, *tmp))
		{
			p += *tmp;
			break;
		}
		else 
			p += (*tmp + 2);	
	}
	
	if ((*p == 0) || (p-root) >= TMSIMPLEFS_BLOCKSIZE*ROOT_BLOCK)
	{
		return -E_DEL;
	}
	i_len = *tmp +1 + 2;
	m_pBufferManage->FreeInode(*p);
	p += 2;
	memcpy(tmp, p, TMSIMPLEFS_BLOCKSIZE*ROOT_BLOCK - (p -root));
	memset(root+(TMSIMPLEFS_BLOCKSIZE*ROOT_BLOCK - i_len), 0, i_len);
	m_pBufferManage->PutRoot();
	
	return 0;
}
int CtmSimpleFS::f_close(int fd)
{
	int i = 0;
	
	if (fd >= TMSIMPLEFS_NR_OPEN || fd  < 0)
		return -ECLOSE;
	i = m_HandleArray[fd].item;
	if (!strcmp(m_OpenItemArray[i].name, "" ))
		return -ECLOSE;
	
	if (--m_OpenItemArray[i].count == 0)
	{
		strcpy(m_OpenItemArray[i].name, "");
		m_OpenItemArray[i].inode = NULL;
	}
	m_HandleArray[fd].pos = 0;
	m_HandleArray[fd].item = TMSIMPLEFS_NR_FILE;
	
	pthread_rwlock_unlock(&m_OpenItemArray[i].rw_lock);	
	pthread_rwlock_destroy(&m_OpenItemArray[i].rw_lock);
	
	return 0;
}

int CtmSimpleFS::r_file(tmInode * inode,  tmHandle *hand, char *buf, int count)
{
	int left = 0, chars = 0, block = 0, c = 0;
	long pos = 0;
	
	pos = hand->pos;
	if ((left = count) <= 0)
		return 0;
	while (left) 
	{
		block = Bmap(inode, pos / TMSIMPLEFS_BLOCKSIZE);
		
		if (block <= 0)
			return -E_READ;
		c = pos % 	TMSIMPLEFS_BLOCKSIZE;
		chars = ((TMSIMPLEFS_BLOCKSIZE-c) < left) ? (TMSIMPLEFS_BLOCKSIZE - c) : left;
		pos += chars;
		left -= chars;
		
		CtmBufferManage::Seek(m_iDevFd, m_lStartAddr + (block-1)*TMSIMPLEFS_BLOCKSIZE + c, 0);
		CtmBufferManage::ReadFromDev(m_iDevFd, buf, chars);
		buf += chars;
	}
	
	hand->pos = pos;
	return count;		
}

int CtmSimpleFS::f_read(int fd, char *buf, int count)
{
	tmInode *inode = NULL;
	tmHandle *hd = NULL;
	int ret = 0;
	
	if (fd < 0 || fd >= TMSIMPLEFS_NR_OPEN || count < 0 || m_iDevFd == -1)
		return -EREAD;
	hd = &m_HandleArray[fd];
	inode = m_OpenItemArray[m_HandleArray[fd].item].inode;
	if ((count + m_HandleArray[fd].pos) > inode->i_size)
			count = inode->i_size - m_HandleArray[fd].pos;
	
	ret = r_file(inode, hd, buf, count);

	
	return ret;
}
int CtmSimpleFS::w_file(tmInode *inode, tmHandle *hand, const char *buf, int count)
{
	long pos = 0;
	int block = 0, c = 0, p = 0, i = 0;

	pos = hand->pos;

	while (i<count) 
	{
		if ((block = CreateBlock(inode, pos/TMSIMPLEFS_BLOCKSIZE)) <= 0)
			return -1;
			
		c = pos % TMSIMPLEFS_BLOCKSIZE;
		p = c + (block-1)*TMSIMPLEFS_BLOCKSIZE;
		c = TMSIMPLEFS_BLOCKSIZE - c;
		if (c > count - i) 
			c = count - i;
		pos += c;
		if (pos >(long) inode->i_size) {
			inode->i_size = pos;
		}
		i += c;
		CtmBufferManage::Seek(m_iDevFd, m_lStartAddr + p, 0);
		CtmBufferManage::WriteToDev(m_iDevFd, buf, c);
		buf += c;
	}
	
	hand->pos = pos;
	m_pBufferManage->PutInode();
	return count;
}
int CtmSimpleFS::f_write(int fd, const char *buf, int count)
{
	tmInode *inode = NULL;
	tmHandle *hd = NULL;
	int ret = 0;
	if (fd <0 || fd >= TMSIMPLEFS_NR_OPEN || count < 0 || m_iDevFd == -1)
		return -EWRITE;
	hd = &m_HandleArray[fd];	
	inode = m_OpenItemArray[m_HandleArray[fd].item].inode;

	ret = w_file(inode, hd, buf, count);
	
	return ret;
}

int  CtmSimpleFS::f_lseek(int fd, long offset, int origin)
{
	tmHandle *hd = NULL;
	unsigned long file_size = 0;
	
	if (fd >= TMSIMPLEFS_NR_OPEN || fd < 0)
		return -ELSEEK;
	hd = &m_HandleArray[fd];	
	
	file_size = m_OpenItemArray[hd->item].inode->i_size;
	
	switch (origin) 
	{
		case SEEKSET:
			hd->pos = (file_size <= (unsigned long)offset) ? file_size : offset;
			break;
		case SEEKCUR:
			hd->pos = (file_size <= (unsigned long)(hd->pos + offset)) ? file_size : (hd->pos + offset);
			break;
		case SEEKEND:
			if ((file_size + offset) < 0)
				hd->pos = 0;
			else if(offset > 0)
				hd->pos = file_size;
			else
				hd->pos = file_size + offset;
			break;
		default:
			return -ELSEEK;
	}
	
	return hd->pos;
}

 int  CtmSimpleFS::_bmap(tmInode *inode, int block, int flag)
 {
 	unsigned short *fat = NULL, *tmp = NULL;
 	int i = 0, n = 0;
 	
 	fat = m_pBufferManage->GetFat();
 	
 	n = inode->i_zone;
 	
 	while (++i <= block)
 	{	
 		tmp = fat+n;
 		n = *tmp;
 		
 	}
 	if ((n == 0) && (flag & B_CREATE))
 	{	
 		n  = m_pBufferManage->NewBlock();
 		if (n != 0) 					// if (n == 0) , have no more blocks
 		{							
 			if (i == 1)
 				inode->i_zone = n;
 			else
 				*tmp = n;
		}
 	}

 	m_pBufferManage->PutFat(); 	
 	return n;
}
int CtmSimpleFS::Bmap(tmInode *inode, int block)
{
	return _bmap(inode, block, 0);
}
int CtmSimpleFS::CreateBlock(tmInode *inode, int block)
{
	return _bmap(inode, block, B_CREATE);
}

/* only for debug */
void  CtmSimpleFS::list(void)
{
	m_pBufferManage->ListFS();
}


		
