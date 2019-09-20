#include "simplefile.h"
#include "string.h"
#include <stdlib.h>
#include <string.h>


CSimpleFile::CSimpleFile(int fd, unsigned long start, unsigned long end)
{
	m_pSimpleFs = new CtmSimpleFS(fd, start, end);
	memset(m_name, 0, TMSIMPLEFS_NAME_LEN+1);
	m_fd = -1;
}

CSimpleFile::~CSimpleFile()
{
	if(m_fd > -1)
		Close();
	delete m_pSimpleFs;
}

int CSimpleFile::Create(const char *name)
{
	strncpy(m_name, name, TMSIMPLEFS_NAME_LEN);
	m_name[TMSIMPLEFS_NAME_LEN] = '\0';
	m_fd = m_pSimpleFs->f_create(m_name);
	return m_fd;
}

int CSimpleFile::Open(const char *name, int mode)
{
	strncpy(m_name, name, TMSIMPLEFS_NAME_LEN);
	m_name[TMSIMPLEFS_NAME_LEN] = '\0';
	m_fd = m_pSimpleFs->f_open(m_name, mode);
	return m_fd;
}

int CSimpleFile::Read(int offset, void *buf, int length)
{
	m_pSimpleFs->f_lseek(m_fd, offset, SEEKSET);
	int i = m_pSimpleFs->f_read(m_fd, (char *)buf, length);
	return i;
}

int CSimpleFile::Read(void *buf, int length)
{
	return m_pSimpleFs->f_read(m_fd, (char *)buf, length);
}

int CSimpleFile::Write(int offset, void *buf, int length)
{
    int nDiff = offset - m_pSimpleFs->f_lseek(m_fd, offset, SEEKSET);
    char *cDiff = (char *)malloc(nDiff);
    memset(cDiff, 0, nDiff);
    m_pSimpleFs->f_write(m_fd, cDiff, nDiff);
	int ret = m_pSimpleFs->f_write(m_fd, (char *)buf, length);
	free(cDiff);
	return ret;
}

int CSimpleFile::Write(void *buf, int length)
{
	return m_pSimpleFs->f_write(m_fd, (char *)buf, length);
}

int CSimpleFile::Seek(int offset, int origin)
{
	return m_pSimpleFs->f_lseek(m_fd, offset, origin);
}

int CSimpleFile::Truncate()
{
	return m_pSimpleFs->f_truncate(m_name);
}

int CSimpleFile::Remove()
{
	char name[TMSIMPLEFS_NAME_LEN+1];
	strncpy(name, m_name, TMSIMPLEFS_NAME_LEN);
	Close();
	return m_pSimpleFs->f_remove(name);
}

int CSimpleFile::Close()
{
	int ret = 0;
	memset(m_name, 0, TMSIMPLEFS_NAME_LEN+1);
	ret = m_pSimpleFs->f_close(m_fd);
	m_fd = -1;
	return ret;
}

void CSimpleFile::List()
{
	m_pSimpleFs->list();
}

int  CSimpleFile::Format()
{
    Close();
    return m_pSimpleFs->f_format();
}