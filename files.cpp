/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Files                                                          |
|  Task     : File Stream derived class                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 03/19/1995                                                     |
|  Revision : 05/07/1995                                                     |
|  Revision : V2.00                                                          |
|             05/07/1995 by C.C. Chen                                        |
|             - Add the CRecordFile class                                    |
+===========================================================================*/
#include	"files.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
char        g_pszCRLF[3] = {'\r', '\n', '\0'};

/*==========================================================================+
|           Class implementation - CtmFile                                 	|
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                     	|
+--------------------------------------------------------------------------*/
CtmFile::CtmFile()
{
	memset(m_cszLastName, 0, sizeof(m_cszLastName));
	m_fd = 0;
}

CtmFile::CtmFile(char *pszName, int flags)
{
	Open(pszName, flags);
}

CtmFile::~CtmFile()
{
	if(m_fd != 0)
		Close();
}
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*==========================================================================+
|	function:	Open(const char *pszName, int flags)						|
|	remark	:	Open file													|
|---------------------------------------------------------------------------|
|	call	:	Open(pszName, flags)										|
|	parameter:	pszName		:	File Name									|
|				flags		: 	open file flags								|
|																			|
|	return	:	0			:  	success										|
|				¨䥦		: 	error											|
+==========================================================================*/			
int		CtmFile::Open(const char *pszName, int flags)
{
	if(strcmp(m_cszLastName, pszName) != 0)
	{
		strncpy(m_cszLastName, pszName, 254);
		if(m_fd > 0)	Close();
		//printf("Close()");
	}
	
	if(m_fd <= 0)
	{
		m_fd	= open(pszName, flags);
		//printf("Open fd=%d %s\n", m_fd, pszName);
		
		if(m_fd < 0)
		{
			//printf("Open %s : %s\n", pszName, strerror(errno));
			m_fd	= 0;		
			return tmFILE_FAIL;
		}
		fchmod(m_fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
	return tmFILE_GOOD;
}

/*==========================================================================+
|	function:	Close()														|
|	remark	:	Close file													|
|---------------------------------------------------------------------------|
|	call	:	Close()														|
|																			|
|	return	:	0			:  	success										|
|				¨䥦		: 	error											|
+==========================================================================*/
int		CtmFile::Close()
{
	int		nResult;
	
	if(m_fd <= 0)	return tmFILE_FAIL;	
	#ifdef	D_3354
	fdatasync(m_fd);
	#endif
	nResult	= close(m_fd);	
	m_fd	= 0;
	
	return nResult;
}

/*==========================================================================+
|	function:	Read(const void *buf, long count)							|
|	remark	:	Read file													|
|---------------------------------------------------------------------------|
|	call	:	Read(buf, count)											|
|	para	:	Buf			:	read buf									|
|				count		: 	Max read count								|
|																			|
|	return	:	0			:  	eof											|
|				>0			:	read count									|
|				-1			: 	error										|
+==========================================================================*/
long	CtmFile::Read(void *buf, long count)
{
	if(m_fd <=0)	return tmFILE_FAIL;
	return	read(m_fd, buf, count);
}

/*==========================================================================+
|	function:	Write(const void *buf, long count)							|
|	remark	:	Write file													|
|---------------------------------------------------------------------------|
|	call	:	Write(buf, count)											|
|	para	:	Buf			:	Write buf									|
|				count		: 	Max Write count								|
|																			|
|	return	:	>0			:	Write count									|
|				-1			: 	error										|
+==========================================================================*/
long	CtmFile::Write(const void *buf, long count)
{
	if(m_fd <=0)	return tmFILE_FAIL;
	return write(m_fd, buf, count);
}

/*==========================================================================+
|	function:	Seek(long Offset, int whence)								|
|	remark	:	move file point to offset									|
|---------------------------------------------------------------------------|
|	call	:	Seek(Offset, whence)										|
|	para	:	Offset		:	Offset										|
|				whence		: 	SEEK_SET		:							|
|							:	SEEK_CUR		:							|
|							:	SEEK_END		:							|
|																			|
|	return	:	>0			:	file point position							|
|				-1			: 	error										|
+==========================================================================*/
long	CtmFile::Seek(long Offset, int whence)
{
	if(m_fd <=0)	return tmFILE_FAIL;
	return lseek(m_fd, Offset, whence);
}

/*==========================================================================+
|	function:	Truncate(long Length)										|
|	remark	:	Change file Length											|
|---------------------------------------------------------------------------|
|	call	:	Truncate(Length)											|
|	para	:	Length		:	New Length									|
|																			|
|	return	:	0			:	success										|
|				-1			: 	error										|
+==========================================================================*/
int		CtmFile::Truncate(long Length)
{
	if(m_fd <=0)	return tmFILE_FAIL;
	return ftruncate(m_fd, Length);
}

/*===========================================================================+
|           Class implementation - TextFile                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmTextFile::CtmTextFile() : CtmFile()
{
	m_pFirst	= NULL;
	m_pLast		= NULL;
	m_nCurLine	= 0;
}

CtmTextFile::CtmTextFile(char* pszName) : CtmFile()
{
	m_pFirst	= NULL;
	m_pLast		= NULL;
	m_nCurLine	= 0;
	Open(pszName);
}

CtmTextFile::~CtmTextFile()
{
	LINEPOS	*pCur;
	
	pCur	= m_pFirst;
	while(pCur	!= NULL)
	{
		pCur		= m_pFirst->pNext;
		free(m_pFirst);
		m_pFirst	= pCur;
	}
	
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void	CtmTextFile::Clear()
{
	LINEPOS	*pCur;
	
	pCur	= m_pFirst;
	while(pCur	!= NULL)
	{
		pCur		= m_pFirst->pNext;
		free(m_pFirst);
		m_pFirst	= pCur;
	}
	
	Truncate(0);
	m_nCurLine	= 0;
}

void        CtmTextFile::Rewind()
{
	m_nCurLine = 0;                       // Preset current line position
}

/*---------------------------------------------------------------------------+
|  Function : int ReadLine(int nLineNo, char* pszString, int nCount)         |
|  Task     : Read a specified line from file                                |
+----------------------------------------------------------------------------+
|  Call     : nStatus = ReadLine(nLineNo, pszString, nCount)                 |
|                                                                            |
|  Parameter: nLineNo                   - The line number to be read         |
|                                         -1 : Read the previous line???     |
|                                          0 : Read the next line            |
|                                          1 : Read the first line           |
|                                          n : Read the specified line       |
|             pszString                 - Pointer to string buffer           |
|             nBufferSize               - String buffer size                 |
|                                                                            |
|  Return   : *pszString                - Line read                          |
|             nStatus                   - IO status                          |
+---------------------------------------------------------------------------*/
int		CtmTextFile::ReadLine(int nLineNo, char* pszString, int nBufferSize)
{
	int			nReadLineNo;
	LINEPOS	*pCur;

	if(m_pFirst == NULL && GetAllLineInfo() <= 0) goto error;
	
	if(nLineNo < 0)							//µ{§Ǳµ¤f¤ӶáI¡I
	{
		if(m_nCurLine < 2)
		{
			nReadLineNo	= 0;
		}
		else
			nReadLineNo	= m_nCurLine - 1;
	}
	else if(nLineNo == 0)
	{
		nReadLineNo		= m_nCurLine;
	}
	else
		nReadLineNo		= nLineNo - 1;
	
	if(nReadLineNo > m_pLast->nLineNo)	goto error;
	
	//=======
	// §䩬¹𐀳ªº¦渹
	//=======
	if(nReadLineNo > m_pLast->nLineNo - nReadLineNo)
	{
		pCur	= m_pLast;
		while(pCur->nLineNo != nReadLineNo)
		{
			pCur	= pCur->pPre;
		}
	}
	else
	{
		pCur	= m_pFirst;
		
		while(pCur->nLineNo != nReadLineNo)
		{
			pCur	= pCur->pNext;
		}
	}	
	if(pCur->nLength >= nBufferSize)	goto error;
	
	
	//=======
	// Ū¨�
	//=======	
	Seek(pCur->lOffset, SEEK_SET);
	Read(pszString, pCur->nLength);
	m_nCurLine	= pCur->nLineNo + 1;
	
	//=======
	// ¥h°£³̫᪺´«¦泅¡A¥[¤W¦r¦굲§�	//=======
	if(pszString[pCur->nLength -2] == '\r')
		pszString[pCur->nLength -2] 	= 0;
		
	if(pszString[pCur->nLength -1]	== '\n')
	{	
		pszString[pCur->nLength -1]		= 0;		
	}
	else
	{
		pszString[pCur->nLength]	= 0;
	}
	
	return tmFILE_GOOD;
	
error:
	*pszString	= 0;
	return -1;

}

/*---------------------------------------------------------------------------+
|  Function : int WriteLine(int nLineNo, char* pszString)                    |
|  Task     : Write a specified line to file                                 |
+----------------------------------------------------------------------------+
|  Call     : nStatus = WriteLine(nLineNo, pszString)                        |
|                                                                            |
|  Parameter: nLineNo                   - The line number to be written      |
|                                         -1 : Write the current line        |
|                                          0 : Write the next line           |
|                                          1 : Write the first line          |
|                                          n : Write the specified line      |
|             pszString                 - Pointer to string buffer           |
|                                                                            |
|  Return   : nStatus                   - IO status                          |
+---------------------------------------------------------------------------*/
int         CtmTextFile::WriteLine(int nLineNo, char* pszString)
{
	int         nStrLength = strlen(pszString)+2;  // New string length + CR + LF
//	int         nDelta;                         // Delta between original and new string
	char        pszBuffer1[tmTMP_BUFFER];
//	char        pszBuffer2[tmTMP_BUFFER];
	int			nLength;
	int			nWriteLineNo;
	int			nFileLength;
	char		*Buffer;
	char		pszBuffer2[2];
	LINEPOS	*pCur;
	
	if(m_pFirst == NULL && GetAllLineInfo() <= 0) return -1;
	
	//=======
	//      Find the specified line
	//=======
	if(nLineNo < 0)
	{
		nWriteLineNo	= m_nCurLine - 1;
	}
	else if(nLineNo == 0)
		nWriteLineNo	= m_nCurLine;
	else
		nWriteLineNo	= nLineNo - 1;
		
	//=======
	// ¦pªG¤妳¬°ªšA«h¥�@ªŦ䈉//=======
	if(m_pFirst == NULL)
	{
		Seek(0, SEEK_SET);
		Write(g_pszCRLF, 2);
		
		pCur	= (LINEPOS *)malloc(sizeof(LINEPOS));
		
		pCur->nLineNo	= 0;
		pCur->lOffset	= 0;
		pCur->nLength	= 2;
		pCur->pPre		= NULL;
		pCur->pNext		= NULL;
		
		m_pFirst		=
		m_pLast			= pCur;		
	}
		
	//=======
	// ¦pªG¦渹¶W¹L¤妳³̤j¦渹¡A«hK¬d³̫ᣀ¦榳µL´«¦泅
	//=======
	if(m_pLast->nLineNo < nWriteLineNo)
	{
		Seek(0, SEEK_END);
		Seek(-2, SEEK_CUR);
		Read(pszBuffer2, 2);
		
		if(pszBuffer2[1] != '\n')
		{
			Write(g_pszCRLF, 2);
			
			//=======
			// ­קﺯ3¸`ªº¸ꯆ
			//=======			
			m_pLast->nLength   += 2;
		}
	}
	
	while(m_pLast->nLineNo < nWriteLineNo)
	{
		Seek(0, SEEK_END);
		Write(g_pszCRLF, 2);
		
		pCur	= (LINEPOS *)malloc(sizeof(LINEPOS));
		
		pCur->nLineNo	= m_pLast->nLineNo + 1;
		pCur->lOffset	= m_pLast->lOffset + 2;
		pCur->nLength	= 2;
		pCur->pPre		= m_pLast;
		pCur->pNext		= NULL;
		
		m_pLast->pNext	= pCur;
		m_pLast			= pCur;
	}
		
	//=======
	// §䩬¹𐀳ªº¦渹
	//=======
	if(nWriteLineNo > m_pLast->nLineNo - nWriteLineNo)
	{
		pCur	= m_pLast;
		while(pCur->nLineNo != nWriteLineNo)
		{
			pCur	= pCur->pPre;
		}
	}
	else
	{
		pCur	= m_pFirst;
		
		while(pCur->nLineNo != nWriteLineNo)
		{
			pCur	= pCur->pNext;
		}
	}	
		
	strcpy(pszBuffer1, pszString);
	strcat(pszBuffer1, g_pszCRLF);	
	
	//=======
	// «O¦s³o¦次­±ªº¤妳¤º®e
	//=======
	nFileLength	= Seek(0, SEEK_END);
	Buffer	= (char *)malloc(nFileLength - pCur->lOffset - pCur->nLength);
	if(Buffer == NULL)	return -1;
	
	Seek(pCur->lOffset + pCur->nLength, SEEK_SET);
	Read(Buffer, nFileLength - pCur->lOffset - pCur->nLength);
	
	m_nCurLine	= pCur->nLineNo;
	Seek(pCur->lOffset, SEEK_SET);
	Write(pszBuffer1, strlen(pszString) + 2);
	Write(Buffer, nFileLength - pCur->lOffset - pCur->nLength);	
	m_nCurLine	= pCur->nLineNo + 1;
	free(Buffer);	
	
	Truncate(nFileLength + nStrLength - pCur->nLength);			// ­ק便¥󫸫Պ	nLength				= pCur->nLength;
	pCur->nLength		= nStrLength;							// ­ק惡¸`ªº¤妳«H®§
	
	//=======
	// ­ק𐄬ª���®§
	//=======
	pCur	= pCur->pNext;
	while(pCur != NULL)
	{
		pCur->lOffset  += nStrLength - nLength;
		pCur			= pCur->pNext;
	}	
	
	return tmFILE_GOOD;
}

/*---------------------------------------------------------------------------+
|  Function : int InsertLine(int nLineNo, char* pszString)                   |
|  Task     : Insert a line before the specified line number                 |
+----------------------------------------------------------------------------+
|  Call     : nStatus = InsertLine(nLineNo, pszString)                       |
|                                                                            |
|  Parameter: nLineNo                   - The line number to be inserted     |
|                                         0 : Insert before current line     |
|                                         1 : Insert before the first line   |
|                                         n : Insert before specified line   |
|             pszString                 - Pointer to string buffer           |
|                                                                            |
|  Return   : nStatus                   - IO status                          |
+---------------------------------------------------------------------------*/
int         CtmTextFile::InsertLine(int nLineNo, char* pszString)
{
	int         nStrLength = strlen(pszString)+2;  // New string length + CR + LF
	char        pszBuffer1[tmTMP_BUFFER];
	int			nWriteLineNo;
	int			nFileLength;
	char		*Buffer;
	LINEPOS	*pCur, *pNew;
	
	if(m_pFirst == NULL && GetAllLineInfo() <= 0) return -1;
	
	//=======
	//      Find the specified line
	//=======
	if(nLineNo < 0)	return -1;
	
	if(nLineNo == 0)
		nWriteLineNo	= m_nCurLine;
	else
		nWriteLineNo	= nLineNo - 1;	
		

	strcpy(pszBuffer1, pszString);
	strcat(pszBuffer1, g_pszCRLF);
	
	nFileLength	= Seek(0, SEEK_END);
	
	//=======
	// ¦pªG´¡¤J¦췭¶W¹L¤妳¥½,«h¤£¦b´¡¤J¡Aª𥜊	//=======
	if(m_pLast->nLineNo + 1 < nWriteLineNo)		return -1;
	
	//=======
	// ¦pªG´¡¤J¦췭¬O¤妳¥½¡A«h°l¥[
	//=======
	if(m_pLast->nLineNo + 1 == nWriteLineNo)
		return AppendLine(pszString);
	
	//=======
	// ¬d§䭛3ªº¦渹
	//=======
	if(nWriteLineNo > m_pLast->nLineNo - nWriteLineNo)
	{
		pCur	= m_pLast;
		while(pCur->nLineNo != nWriteLineNo)
		{
			pCur	= pCur->pPre;
		}
	}
	else
	{
		pCur	= m_pFirst;
		
		while(pCur->nLineNo != nWriteLineNo)
		{
			pCur	= pCur->pNext;
		}
	}
	
	//=======
	// «O¦s³o¦次­±ªº¤妳¤º®e
	//=======
	Buffer	= (char *)malloc(nFileLength - pCur->lOffset);
	if(Buffer == NULL)	return -1;
	
	pNew	= (LINEPOS *)malloc(sizeof(LINEPOS));
	if(pNew == NULL)	return -1;
	
	
	Seek(pCur->lOffset, SEEK_SET);
	Read(Buffer, nFileLength - pCur->lOffset);
	
	
	Seek(pCur->lOffset, SEEK_SET);
	Write(pszBuffer1, nStrLength);
	Write(Buffer, nFileLength - pCur->lOffset);	
	m_nCurLine	= pCur->nLineNo + 1;
	free(Buffer);
	
	//=======
	// ­ק￣­ԃ쫭«H®§
	//=======
	pNew->pNext		= pCur;
	pNew->pPre		= pCur->pPre;
	pNew->nLineNo	= pCur->nLineNo;
	pNew->lOffset	= pCur->lOffset;
	pNew->nLength	= nStrLength;
	
	pNew->pPre->pNext	= pNew;
	pCur->pPre			= pNew;
	
	while(pCur != NULL)
	{
		pCur->lOffset   += nStrLength;
		pCur->nLineNo  ++;
		pCur			= pCur->pNext;
	}
	
	return tmFILE_GOOD;
}
/*---------------------------------------------------------------------------+
|  Function : int DeleteLine(int nLineNo)                                    |
|  Task     : Delete a specified line from file                              |
+----------------------------------------------------------------------------+
|  Call     : nStatus = DeleteLine(nLineNo)                                  |
|                                                                            |
|  Parameter: nLineNo                   - The line number to be deleteed     |
|                                         0 : Delete the current line        |
|                                         1 : Delete the first line          |
|                                         n : Delete the specified line      |
|                                                                            |
|  Return   : nStatus                   - IO status                          |
+---------------------------------------------------------------------------*/
int         CtmTextFile::DeleteLine(int nLineNo)
{
	int			nDeleteLineNo;
	int			nLength;
	char		*pBuffer;
	LINEPOS	*pCur, *pCur1;
	long		nFileLength;	
	
	if(m_pFirst == NULL && GetAllLineInfo() <= 0) return -1;
	
	if(nLineNo	< 0)	return -1;
	
	if(nLineNo	== 0)	nDeleteLineNo	= m_nCurLine;
	else				nDeleteLineNo	= nLineNo	- 1;
	
	//=======
	// ¬d§亯3¦檺«H®§
	//=======
	if(nDeleteLineNo > m_pLast->nLineNo - nDeleteLineNo)
	{
		pCur	= m_pLast;
		while(pCur->nLineNo != nDeleteLineNo)
		{
			pCur	= pCur->pPre;
		}
	}
	else
	{
		pCur	= m_pFirst;
		
		while(pCur->nLineNo != nDeleteLineNo)
		{
			pCur	= pCur->pNext;
		}
	}
	
	nFileLength	= Seek(0, SEEK_END);
	pBuffer	= (char *)malloc(nFileLength - pCur->lOffset - pCur->nLength);
	if(pBuffer == NULL)	return -1;
	
	Seek(pCur->lOffset + pCur->nLength, SEEK_SET);
	Read(pBuffer, nFileLength - pCur->lOffset - pCur->nLength);
	
	Seek(pCur->lOffset, SEEK_SET);
	Write(pBuffer, nFileLength - pCur->lOffset - pCur->nLength);
	free(pBuffer);
	Truncate(nFileLength - pCur->nLength);
	
	//=======
	// §R°£Ĭª���¦h¥X¨Ӫº¤@¸`¡A¨íק朗¦³«⃾ªºªº¸`
	//=======
	if(pCur->pPre == NULL && pCur->pNext == NULL)
	{	
		free(pCur);
		return tmFILE_GOOD;
	}
	
	if(pCur->pPre != NULL)
		pCur->pPre->pNext	= pCur->pNext;
	else
		m_pFirst	= pCur->pNext;
	
	if(pCur->pNext != NULL)
		pCur->pNext->pPre	= pCur->pPre;
	else
		m_pLast		= pCur->pPre;
	
	pCur1	= pCur->pNext;
	nLength	= pCur->nLength;
	
	free(pCur);
	
	while(pCur1 != NULL)
	{
		pCur1->nLineNo	--;
		pCur1->lOffset -= nLength; 
		pCur1	= pCur1->pNext;
	}
	
	return tmFILE_GOOD;
	
}
/*---------------------------------------------------------------------------+
|  Function : AppendLine(char* pszString)                                    |
|  Task     : Append a line to the end of file                               |
+----------------------------------------------------------------------------+
|  Call     : AppendLine(char* pszString)                                    |
|                                                                            |
|  Parameter: pszString                 - Pointer to string buffer           |
|                                                                            |
|  Return   : None                      -                                    |
+---------------------------------------------------------------------------*/
int         CtmTextFile::AppendLine(char* pszString)
{
	
	int         nLength = strlen(pszString)+2;
	char        pszBuffer1[tmTMP_BUFFER];
	int 		nWriteState;
	LINEPOS	*pCur;
	
	if(m_pFirst != NULL)
	{
		//=======
		// K¬d­즽ªº¦櫏§_¬O´«¦泅µ²§�		//=======
		Seek(0,  SEEK_END);
		Seek(-1, SEEK_CUR);
		Read(pszBuffer1, 1);
		if(pszBuffer1[0] != '\n')
		{
			Write(g_pszCRLF, 2);		
			m_pLast->nLength   += 2;
		}
	}
	
	strcpy(pszBuffer1, pszString);
	strcat(pszBuffer1, g_pszCRLF);
	
	nWriteState 	= Write(pszBuffer1, nLength);
	if(nWriteState < 0)	return -1;
	
	//======
	// ¼W¥[·sªº¸`
	//======
	pCur	= (LINEPOS *)malloc(sizeof(LINEPOS));
	if(pCur ==  NULL)	return -1;
	
	if(m_pFirst != NULL)
	{
		pCur->lOffset	= m_pLast->lOffset + m_pLast->nLength;
		pCur->nLength	= nLength;
		pCur->nLineNo	= m_pLast->nLineNo + 1;
		pCur->pNext		= NULL;
		pCur->pPre		= m_pLast;
		
		m_pLast->pNext	= pCur;
		m_pLast			= pCur;
	}
	else
	{
		pCur->nLineNo	= 0;
		pCur->lOffset	= 0;
		pCur->nLength	= nLength;
		pCur->pPre		= NULL;
		pCur->pNext		= NULL;
		
		m_pFirst		=
		m_pLast			= pCur;
	}
	
	m_nCurLine		= pCur->nLineNo + 1;
	
	return tmFILE_GOOD;
}

BOOL	CtmTextFile::IsEOF()
{
	int		nCurPos;
	BOOL	bEOF;
	
	nCurPos	= Seek(0, SEEK_CUR);
	if(nCurPos == Seek(0, SEEK_END))
		bEOF	= TRUE;
	else
		bEOF	= FALSE;
	
	Seek(nCurPos, SEEK_SET);
	
	return bEOF;
}

int		CtmTextFile::GetLineInfo()
{
	int		i 		= 0;
	int		nLineSize;
	BOOL	bEOF	= TRUE;
	char	Buffer[tmTMP_BUFFER];
	
	LINEPOS	*pCur;
	
	
	//=======
	// ¦pªG¤ª���ªšA«h¬O²Ĥ@¦¸Ū¨�=======
	if(m_pFirst == NULL)
	{
		Seek(0, SEEK_SET);
	}
	else
	{
		Seek(m_pLast->lOffset + m_pLast->nLength, SEEK_SET);
	}
		
	//Ū¦r²šA¤@ª½Ū¨쥥¥󥽧)Ϊͅª¨촫¦泅
	while(Read(Buffer + i, 1) > 0)
	{
		if(*(Buffer + i) == '\n')
		{
			bEOF	= FALSE;
			break;
		}
		
		i ++;
		if(i >= tmTMP_BUFFER)
			return	-1;
	}
	
	//¦pªGŪ¨쥥¥󵲧�h¥[¤W¦r¦굲§�ڊ	if(bEOF && i == 0)		return 0;			// µL¤º®e¡A«h¤妳µ²§�	nLineSize		= i + 1;

	
	//=======
	// ºc«ف䫭
	//======		
	pCur	= (LINEPOS *)malloc(sizeof(LINEPOS));
	
	if(pCur == NULL)	return -1;
	
	
	pCur->nLength	= nLineSize;
	pCur->pNext		= NULL;
	
	if(m_pFirst == NULL)
	{
		pCur->lOffset		= 0;
		pCur->nLineNo		= 0;
		m_pFirst			= pCur;
		m_pFirst->pPre		= NULL;
		m_pLast				= m_pFirst;
	}
	else
	{
		pCur->lOffset	= m_pLast->lOffset + m_pLast->nLength;
		pCur->pPre		= m_pLast;
		pCur->nLineNo	= m_pLast->nLineNo + 1;		
		m_pLast->pNext	= pCur;		
		m_pLast			= pCur;
	}
	
	return nLineSize;
}

int		CtmTextFile::GetAllLineInfo()
{
	int		nLineNum	 = 0;				// ¤妳 ªº¦潆
	long	nFileLength;					// ¤妳 ª�
	char	*pFileBuf;
	char	*pLine, *pLineStart;
	int		nLineLength, nLength;
	
	LINEPOS	*pCur;
	
	//=======
	// ¦pªGĬª���¬°ªšA«h²Mªƃ쫭
	//=======
	if(m_pFirst != NULL)
	{
		pCur	= m_pFirst;
		
		while(pCur != NULL)
		{
			pCur		= m_pFirst->pNext;
			free(m_pFirst);
			m_pFirst	= pCur;
		}
		
		m_pFirst 	= NULL;
		m_pLast		= NULL;
	}
	
	//=======
	// Ū¨�¤妳
	//=======
	nFileLength		= Seek(0, SEEK_END);
	if(nFileLength == 0)	return 0;
	
	pFileBuf	= (char *)malloc(nFileLength);
	if(pFileBuf	== NULL)	return -1;
	Seek(0, SEEK_SET);
	Read(pFileBuf, nFileLength);
	
	pLineStart	= pFileBuf;
	nLength		= nFileLength;
	
	pLine	= strstr(pLineStart, "\n");
	while(pLine != NULL || nLength > 0)
	{
		//=======
		// ­pº⫸«פΤ妳³ѧE¦r¸`
		//=======
		if(pLine != NULL)
		{
			nLineLength	= pLine - pLineStart + 1;
			nLength	   -= nLineLength;
		}
		else
		{
			nLineLength	= 0;
			nLength = 0;
		}
			
		
		
		//=======
		// ¼W¥[·sªº¸`I
		//=======
		pCur	= (LINEPOS *)malloc(sizeof(LINEPOS));
		if(pCur == NULL)	return -1;
		
		nLineNum ++;
		if(m_pFirst == NULL)
		{
			pCur->nLineNo	= 0;
			pCur->lOffset	= 0;
			pCur->nLength	= nLineLength;
			pCur->pPre		= NULL;
			pCur->pNext		= NULL;
			
			m_pFirst		=
			m_pLast			= pCur;
		}
		else
		{
			pCur->lOffset	= m_pLast->lOffset + m_pLast->nLength;
			pCur->nLength	= nLineLength;
			pCur->nLineNo	= m_pLast->nLineNo + 1;
			pCur->pNext		= NULL;
			pCur->pPre		= m_pLast;
			
			m_pLast->pNext	= pCur;
			m_pLast			= pCur;
		}
		
		
		if(nLength <= 0)	break;		// ¦pªG¤妳ª�¹s¡A ¨º»򵲧�∉	
		//=======
		// ¬d§䣕¤@¦檈®§
		//=======
		pLineStart	= pLine + 1;
				
		pLine		= (char*)strstr((const char *)pLineStart, "\n");		
	}
	
	free(pFileBuf);	
	return nLineNum;
}

/*===========================================================================+
|           Class implementation - RecordFile                                |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmRecordFile::CtmRecordFile()
{
}

CtmRecordFile::CtmRecordFile(char* pszName, UINT nRecordSize)
{
	Open(pszName, nRecordSize);
}

CtmRecordFile::~CtmRecordFile()
{
	close(m_fd);
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CtmRecordFile::Open()
{
/*	int 			i;
	tmRAMFILEINFO	fileinfo;
	
	for(i = 0; i < tmRAMFILE_MAX; i ++)
	{
		if(strncmp(g_RamFileList[i].pszName, m_pszName, tmMAX_FILENAME) == 0)
			break;
	}
	if(i == tmRAMFILE_MAX)	
	{	
		m_fd	= -1;
		return;
	}
	
	if((m_fd = open(tmSRAM_NAME, O_RDWR | O_NOCTTY)) > 0)
	{
		fileinfo.wVersion	= 0;
		fileinfo.lOffset	= g_RamFileList[i].lOffset;
		fileinfo.lLength	= g_RamFileList[i].lSize;									//<<yang 2005/12/5 
		strncpy(fileinfo.szName, m_pszName, tmMAX_FILENAME);
		//printf("%d, %08x, %s\n", fileinfo.wVersion, fileinfo.lOffset, fileinfo.szName);
		lseek(m_fd, sizeof(tmRAMFILEINFO) * i, SEEK_SET);
		write(m_fd, &fileinfo, sizeof(tmRAMFILEINFO));
		lseek(m_fd, fileinfo.lOffset, SEEK_SET);
		
		m_bInSram	= TRUE;
		m_lOffset	= fileinfo.lOffset;
		m_lSize		= g_RamFileList[i].lSize;
	}
	else*/
//	{	
		m_fd = open(m_pszName, m_nMode);
		fchmod(m_fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		m_bInSram	= FALSE;
//	}
	m_posCurRecord = 0;                     // Preset current Record position
	m_posNextRecord = 0;                    // Preset next Record position
	m_bRenew = TRUE;
}

void        CtmRecordFile::Open(char* pszName, UINT nRecordSize)
{
	m_nMode = O_RDWR | O_CREAT;
	//m_nProt = filebuf::sh_read | filebuf::sh_write;
	strcpy(m_pszName, pszName);             // Save file pathname
	m_nRecordSize  = nRecordSize;
	m_bBypassWrite = FALSE; 
	Open();
}

void        CtmRecordFile::OpenReadOnly(char* pszName, UINT nRecordSize)   //St030101 for INET
{
	m_nMode = O_RDONLY | O_CREAT;
	//m_nProt = filebuf::sh_read | filebuf::sh_write;
	strcpy(m_pszName, pszName);             // Save file pathname
	m_nRecordSize  = nRecordSize;
	m_bBypassWrite = TRUE; 
	Open();
}

void        CtmRecordFile::Clear()
{
	if (m_bBypassWrite) return;             //St030101 for INET
	if(m_bInSram == FALSE)
		ftruncate(m_fd, 0);
	m_posCurRecord = 0;                     // Preset current Record position
	m_posNextRecord = 0;                    // Preset next Record position
}

void        CtmRecordFile::Erase()        //St041701 for complete clear data 
{
	if (m_bBypassWrite) return;             //St030101 for INET
	if(m_bInSram == TRUE)
	{
		Open();
	}
	ftruncate(m_fd, 0);
	close(m_fd);                                // 
	m_fd	= open(m_pszName, O_RDWR | O_CREAT | O_TRUNC);
	close(m_fd);                               // 
	Open();
}

void        CtmRecordFile::Rewind()
{
	m_posCurRecord = 0;                     // Preset current Record position
	m_posNextRecord = 0;                    // Preset next Record position
}

/*---------------------------------------------------------------------------+
|  Function : ReadRecord(int nRecordNo, void* pRecord)                       |
|  Task     : Read a specified Record from file                              |
+----------------------------------------------------------------------------+
|  Call     : nStatus = ReadRecord(nRecordNo, pRecord)                       |
|                                                                            |
|  Parameter: nRecordNo                 - The Record number to be read       |
|                                         -1 : Read the previous Record      |
|                                          0 : Read the next Record          |
|                                          1 : Read the first Record         |
|                                          n : Read the specified Record     |
|             pRecord                   - Pointer to record buffer           |
|                                                                            |
|  Return   : *pRecord                  - Record read                        |
|             nStatus(ReadRecord)       - IO status                          |
+---------------------------------------------------------------------------*/
int         CtmRecordFile::ReadRecord(int nRecordNo, void* pRecord)
{
	return ReadRecord(nRecordNo, pRecord, m_nRecordSize, 0);
}

int         CtmRecordFile::ReadRecord(int nRecordNo, void* pRecord, int nLength)
{
	return ReadRecord(nRecordNo, pRecord, nLength, 0);
}

int         CtmRecordFile::ReadRecord(int nRecordNo, void* pRecord, int nLength, int nOffset)
{
	int		iReadState;
	if (nRecordNo < 0)
	{
		m_posNextRecord = m_posCurRecord;
		m_posCurRecord -= m_nRecordSize;
		if (m_posCurRecord < 0) m_posCurRecord = 0;
	}
	else if (nRecordNo == 0)
	{
		m_posCurRecord = m_posNextRecord;
		m_posNextRecord += m_nRecordSize;
	}
	else
	{
		m_posCurRecord = (nRecordNo - 1) * m_nRecordSize;
		m_posNextRecord = m_posCurRecord + m_nRecordSize;
	}	
	
	//clear();
	if(m_bInSram == TRUE)
		lseek(m_fd, m_posCurRecord+nOffset+m_lOffset,SEEK_SET);
	else	
		lseek(m_fd, m_posCurRecord+nOffset,SEEK_SET);
		
	iReadState = read(m_fd, (char*)pRecord, nLength);
	m_nRecordNo   = nRecordNo;              //St1109'01 add for µ£¦t´­
	m_wWriteState = tmSTATE_READ;
	if (iReadState>0) return 	tmFILE_GOOD;
	else				return	tmFILE_BAD;
}

/*---------------------------------------------------------------------------+
|  Function : WriteRecord(int nRecordNo, void* pRecord)                      |
|  Task     : Write a specified Record to file                               |
+----------------------------------------------------------------------------+
|  Call     : WriteRecord(nRecordNo, pRecord)                                |
|                                                                            |
|  Parameter: nRecordNo                 - The Record number to be written    |
|                                         -1 : Write the current Record      |
|                                          0 : Write the next Record         |
|                                          1 : Write the first Record        |
|                                          n : Write the specified Record    |
|             pRecord                   - Pointer to record buffer           |
|                                                                            |
|  Return   : nStatus(WriteRecord)      - IO status                          |
+---------------------------------------------------------------------------*/
int         CtmRecordFile::WriteRecord(int nRecordNo, void* pRecord)
{
	return WriteRecord(nRecordNo, pRecord, m_nRecordSize, 0);
}

int         CtmRecordFile::WriteRecord(int nRecordNo, void* pRecord, int nLength)
{
	return WriteRecord(nRecordNo, pRecord, nLength, 0);
}

int         CtmRecordFile::WriteRecord(int nRecordNo, void* pRecord, int nLength, int nOffset)
{
	int		iWriteState;
	if (m_bBypassWrite) return 0;              //St030101 for INET
	if (nRecordNo == 0)
	{
		m_posCurRecord = m_posNextRecord;
		m_posNextRecord += m_nRecordSize;
	}
	else if (nRecordNo > 0)
	{
		m_posCurRecord = (nRecordNo - 1) * m_nRecordSize;
		m_posNextRecord = m_posCurRecord + m_nRecordSize;
	}
	if(m_bInSram == TRUE)
		lseek(m_fd, m_posCurRecord+nOffset+m_lOffset,SEEK_SET);
	else	
		lseek(m_fd, m_posCurRecord+nOffset,SEEK_SET);

	iWriteState = write(m_fd,(char*)pRecord, nLength);
	fsync(m_fd);          // Not actually write into the device, at the operating system's discretion
	m_bRenew      = TRUE;
	m_nRecordNo   = nRecordNo;                //St1109'01 add for netdigger
	m_wWriteState = tmSTATE_WRITE;
	if (iWriteState >= 0) return 	tmFILE_GOOD;
	else				return	tmFILE_BAD;
}

/*---------------------------------------------------------------------------+
|  Function : InsertRecord(int nRecordNo, void* pRecord)                     |
|  Task     : Insert a Record before the specified Record number             |
+----------------------------------------------------------------------------+
|  Call     : nStatus = InsertRecord(nRecordNo, pRecord)                     |
|                                                                            |
|  Parameter: nRecordNo                 - The Record number to be inserted   |
|                                         0 : Insert before current Record   |
|                                         1 : Insert before the first Record |
|                                         n : Insert before specified Record |
|             pRecord                   - Pointer to record buffer           |
|                                                                            |
|  Return   : nStatus(InsertRecord)     - IO status                          |
+---------------------------------------------------------------------------*/
int         CtmRecordFile::InsertRecord(int nRecordNo, void* pRecord)
{
	int		iWriteState;
	//=======
	//      Find the specified Record
	//=======
	if(m_bInSram == TRUE)	return 0;
	
	if (nRecordNo > 0)
	{
		m_posCurRecord = (nRecordNo - 1) * m_nRecordSize;
		m_posNextRecord = m_posCurRecord + m_nRecordSize;
	}
	
	//=======
	//      Move data from the current Record to the next Record
	//=======
	char*       pszBuffer = new char[m_nRecordSize];
	int   		posGet;
	long        lDelta;
	
	posGet = lseek(m_fd, 0, SEEK_END);    // Find the EOF
	lDelta = posGet % m_nRecordSize;        // Check if the record is balance
	if (lDelta == 0)
		posGet -= m_nRecordSize;
	else
		posGet -= lDelta;
	
	do
	{
		lseek(m_fd, posGet, SEEK_SET);
			
		if(read(m_fd, pszBuffer, m_nRecordSize) == -1) return -1;
			
		lseek(m_fd, posGet + m_nRecordSize, SEEK_SET);
		if(write(m_fd, pszBuffer, m_nRecordSize) == -1) return -1;
			
		posGet -= m_nRecordSize;
	}
	while (posGet >= m_posCurRecord);
	
	//=======
	//      Insert the Record and then exit
	//=======
	lseek(m_fd, m_posCurRecord, SEEK_SET);
	iWriteState = write(m_fd, (char*)pRecord, m_nRecordSize);
	fsync(m_fd);
	
	delete []pszBuffer;
	return iWriteState;
}

/*---------------------------------------------------------------------------+
|  Function : DeleteRecord(int nRecordNo)                                    |
|  Task     : Delete a specified Record from file                            |
+----------------------------------------------------------------------------+
|  Call     : nStatus = DeleteRecord(nRecordNo)                              |
|                                                                            |
|  Parameter: nRecordNo                 - The Record number to be deleteed   |
|                                         0 : Delete the current Record      |
|                                         1 : Delete the first Record        |
|                                         n : Delete the specified Record    |
|                                                                            |
|  Return   : nStatus(DeleteRecord)     - IO status                          |
+---------------------------------------------------------------------------*/
int         CtmRecordFile::DeleteRecord(int nRecordNo)
{
	if(m_bInSram == TRUE)	return 0;
	//=======
	//      Find the specified Record
	//=======
	if (nRecordNo > 0)
	{
		m_posCurRecord = (nRecordNo - 1)*m_nRecordSize;
		m_posNextRecord = m_posCurRecord + m_nRecordSize;
	}
	
	//=======
	//      Move data from the next Record to the current Record
	//=======
	char*       pszBuffer = new char[m_nRecordSize];
	int		   	posGet = m_posNextRecord;
	BOOL        bEOF = FALSE;
	
	do
	{
		lseek(m_fd, posGet, SEEK_SET);
		if (read(m_fd, pszBuffer, m_nRecordSize) == 0) bEOF = TRUE;
		lseek(m_fd, posGet - m_nRecordSize, SEEK_SET);
		if(write(m_fd, pszBuffer, m_nRecordSize) == -1) return -1;
		posGet += m_nRecordSize;
	}
	while (!bEOF);
	fsync(m_fd);                               // Must flush the buffer???
	
	//=======
	//      Change file size
	//=======
	//_chsize(m_fd, (lseek(m_fd,0,SEEK_END) - m_nRecordSize));
	delete []pszBuffer;
	return	ftruncate(m_fd, (lseek(m_fd,0,SEEK_END) - m_nRecordSize));
}

/*---------------------------------------------------------------------------+
|  Function : AppendRecord(void* pRecord)                                    |
|  Task     : Append a Record to the end of file                             |
+----------------------------------------------------------------------------+
|  Call     : AppendRecord(char* pRecord)                                    |
|                                                                            |
|  Parameter: pRecord                   - Pointer to record buffer           |
|                                                                            |
|  Return   : nStatus(AppendRecord)     - IO status                          |
+---------------------------------------------------------------------------*/
int         CtmRecordFile::AppendRecord(void* pRecord)
{
	int		iWriteState;

	if(m_bInSram == TRUE)	return 0;
	m_posCurRecord = lseek(m_fd, 0, SEEK_END);                     // Find the EOF
	m_posNextRecord = m_posCurRecord + m_nRecordSize;
	iWriteState = write(m_fd,(char*)pRecord, m_nRecordSize);
	fsync(m_fd);
	return iWriteState;
}

/*---------------------------------------------------------------------------+
|  Function : CopyFile(fstream fsFile)                                       |
|  Task     : Copy whole data to console or specified file                   |
+----------------------------------------------------------------------------+
|  Call     : CopyFile(fstream fsFile)                                       |
|                                                                            |
|  Parameter: fsFile                    - Output file stream                 |
|                                                                            |
|  Return   : None                      -                                    |
+---------------------------------------------------------------------------*/
/*
void        CtmRecordFile::CopyFile(fstream fsFile)          //St032101
{
	
	char        pszBuffer[tmTMP_BUFFER];
	
	seekg(0);
	do
	{
		read(pszBuffer, tmTMP_BUFFER);
		fsFile.write(pszBuffer, gcount());
		fsFile.flush();
	}
	while (!eof());
	clear();

}  
*/

/*---------------------------------------------------------------------------+
|  Function : LoadFile(fstream fsFile)                                       |
|  Task     : Load whole data from console or specified file                 |
+----------------------------------------------------------------------------+
|  Call     : LoadFile(fstream fsFile)                                       |
|                                                                            |
|  Parameter: fsFile                    - Input file stream                  |
|                                                                            |
|  Return   : None                      -                                    |
+---------------------------------------------------------------------------*/
/*
void        CtmRecordFile::LoadFile(fstream fsFile)          //St040901
{
	
	char        pszBuffer[tmTMP_BUFFER];
	
	seekp(0);
	do
		{
		fsFile.read(pszBuffer, tmTMP_BUFFER);
		write(pszBuffer, fsFile.gcount());
		flush();
		}
	while (!fsFile.eof());
	clear();
} 
*/

/*---------------------------------------------------------------------------+
|  Function : IsEOF()                                                        |
|  Task     : Is it end of file                                              |
+----------------------------------------------------------------------------+
|  Call     : bEOF = IsEOF()                                                 |
|                                                                            |
|  Parameter: None                      -                                    |
|                                                                            |
|  Return   : IsEOF                     - End of file flag                   |
+---------------------------------------------------------------------------*/
BOOL    CtmRecordFile::IsEOF()
{
	if(m_bInSram == TRUE)
		return (m_posCurRecord >= m_lSize);
	else
		return (m_posCurRecord >= lseek(m_fd, 0, SEEK_END));
};

/*---------------------------------------------------------------------------+
|  Function :                                                         |
|  Task     :                                               |
+----------------------------------------------------------------------------+
|  Call     :                                                  |
|                                                                            |
|  Parameter:                       -                                    |
|                                                                            |
|  Return   :                      -                    |
+---------------------------------------------------------------------------*/
