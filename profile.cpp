/*==========================================================================+
|  Class    : Profile                                                       |
|  Task     : Program Profile                                               |
|---------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                  |
|  Link     : MSVC V1.50 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 03/12/1995                                                    |
|  Revision :                                                               |
+==========================================================================*/

#include    <stdlib.h>
#include    <string.h>

#include    "profile.h"

/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
//=======
//      Initialize static class member
//=======

/*==========================================================================+
|           Class implementation - Profile                                  |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
CProfile::CProfile() : CtmTextFile()
{
}

CProfile::CProfile(char* pszName) : CtmTextFile(pszName)
{
	Open(pszName);
}

CProfile::~CProfile()
{
}
    
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function : GetProfileString(char* pszSection,                            |
|                              char* pszEntry,                              |
|                              char* pszDefault,                            |
|                              char* pszBuffer,                             |
|                              int nBufferSize)                             |
|  Task     : Read a string value of specified Section/Entry from profile   |
|             Profile format :                                              |
|               [Section Name]                                              |
|               EntryName = Entry value                                     |
|               EntryName = Entry value                                     |
|               [Section Name]                                              |
|               EntryName = Entry value                                     |
|               EntryName = Entry value                                     |
+---------------------------------------------------------------------------+
|  Call     : GetProfileString(pszSection,                                  |
|                              pszEntry,                                    |
|                              pszDefault,                                  |
|                              pszBuffer,                                   |
|                              nBufferSize)                                 |
|                                                                           |
|  Parameter: pszSection                - Pointer to section name           |
|             pszEntry                  - Pointer to entry name             |
|             pszDefault                - Pointer to default value if the   |
|                                         entry is not found                |
|             pszBuffer                 - Pointer to return buffer          |
|             nBufferSize               - Return buffer size                |
|                                                                           |
|  Return   : *pszBuffer                - Entry value read                  |
|             GetProfileString          - String length of the entry value  |
+--------------------------------------------------------------------------*/
int CProfile::GetProfileString(const char* pszSection, const char* pszEntry, const char* pszDefault, char* pszBuffer, UINT nBufferSize)
{
	int         nIndex;
	UINT        nSize = nBufferSize - 1;                // Extract 1 byte for '\0'
	UINT        nLength = strlen(pszDefault);           // Preset return string length to 0
	char*       pszPointer = (char *)pszDefault;                // Preset return string pointer to default
	int			nLineNo;
	
	if (FindEntry(pszSection, pszEntry, &nLineNo) == ENTRY_FOUND)
    {
	    m_pszPointer = strstr(m_pszPointer, "=");
	    if (m_pszPointer != NULL)
        {
	        nIndex = strspn(++m_pszPointer, " \t");     // Skip the blank and tab
	        m_pszPointer += nIndex;
	        if ((*m_pszPointer == '\'') || (*m_pszPointer == '\"'))
	            m_pszPointer++;
	        nLength = strlen(m_pszPointer);
	        if (nLength > 0)
            	if ((*(m_pszPointer+nLength-1) == '\'') || (*(m_pszPointer+nLength-1) == '\"'))
                {
	                *(m_pszPointer+nLength-1) = '\0';
	                nLength--;
                }
        		pszPointer = m_pszPointer;
        }
    }

	if (nLength > nSize)
    {
	    strncpy(pszBuffer, pszPointer, nSize);
	    *(pszBuffer + nSize) = '\0';
    }
	else
    	strcpy(pszBuffer, pszPointer);

	return nLength;
}

/*--------------------------------------------------------------------------+
|  Function : GetProfileInt(char* pszSection,                               |
|                           char* pszEntry,                                 |
|                           int   pszDefault)                               |
|  Task     : Read an integer value of specified Section/Entry from profile |
+---------------------------------------------------------------------------+
|  Call     : nValue = GetProfileInt(pszSection, pszEntry, nDefault)        |
|                                                                           |
|  Parameter: pszSection                - Pointer to section name           |
|             pszEntry                  - Pointer to entry name             |
|             nDefault                  - Default value if the entry is not |
|                                         found                             |
|                                                                           |
|  Return   : GetProfileInt             - Entry value read                  |
+--------------------------------------------------------------------------*/
int CProfile::GetProfileInt(const char* pszSection, const char* pszEntry, int nDefault)
{
	int         nIndex;
	int         nValue = nDefault;
	int			nLineNo;
	
	if (FindEntry(pszSection, pszEntry, &nLineNo) == ENTRY_FOUND)
    {
	    m_pszPointer = strstr(m_pszPointer, "=");
	    if (m_pszPointer != NULL)
        {
	        nIndex = strspn(++m_pszPointer, " \t");     // Skip the blank and tab
	        m_pszPointer += nIndex;
	        nValue = atoi(m_pszPointer);
        }
    }

	return nValue;
}

/*--------------------------------------------------------------------------+
|  Function : WriteProfileString(char* pszSection,                          |
|                                char* pszEntry,                            |
|                                char* pszValue)                            |
|  Task     : Write a string value of specified Section/Entry to profile    |
+---------------------------------------------------------------------------+
|  Call     : WriteProfileInt(pszSection, pszEntry, pszValue)               |
|                                                                           |
|  Parameter: pszSection                - Pointer to section name           |
|             pszEntry                  - Pointer to entry name             |
|             pszValue                  - Pointer to string to be written   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int CProfile::WriteProfileString(const char* pszSection, const char* pszEntry, char* pszValue)
{
	char        pszLine[tmMAX_LINE_BUFFER];
	int			nLineNo;
	int			nBlankLine	= -1;
	
	switch (FindEntry(pszSection, pszEntry, &nLineNo))
    {
    case ENTRY_FOUND:                   // Pointer is on the entry line if entry is found
        m_pszPointer = strstr(m_pszPointer, "=");
        strcpy(m_pszPointer, "= ");
        strcat(m_pszPointer, pszValue);
        WriteLine(-1, m_pszLine);       // Write to current line
		break;
    case SECTION_FOUND:                 // Pointer is on the next section or EOF if section is found but entry not    	
        do
        {                              // Read back to find blank lines
            ReadLine(-1, pszLine, tmMAX_LINE_BUFFER);
            nBlankLine	++;
        }
        while (strlen(pszLine) == 0);
        ReadLine(0, pszLine, tmMAX_LINE_BUFFER);
        strcpy(m_pszLine, pszEntry);
        strcat(m_pszLine, " = ");
        strcat(m_pszLine, pszValue);
        InsertLine(0, m_pszLine);       	// Insert before current line
        break;
    default:                            // Pointer is on the EOF if both section and entry is not found
        strcpy(m_pszLine, "[");
        strcat(m_pszLine, pszSection);
        strcat(m_pszLine, "]");
        AppendLine(m_pszLine);
        strcpy(m_pszLine, pszEntry);
        strcat(m_pszLine, " = ");
        strcat(m_pszLine, pszValue);
        AppendLine(m_pszLine);
        break;
    }
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function : WriteProfileInt(char* pszSection,                             |
|                             char* pszEntry,                               |
|                             int   nValue)                                 |
|  Task     : Write an integer value of specified Section/Entry to profile  |
+---------------------------------------------------------------------------+
|  Call     : WriteProfileInt(pszSection, pszEntry, nValue)                 |
|                                                                           |
|  Parameter: pszSection                - Pointer to section name           |
|             pszEntry                  - Pointer to entry name             |
|             nValue                    - Integer to be written             |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int CProfile::WriteProfileInt(const char* pszSection, const char* pszEntry, int nValue)
{
	char        pszValue[tmMAX_LINE_BUFFER];
	
	sprintf(pszValue, "%d", nValue);
	return WriteProfileString(pszSection, pszEntry, pszValue);
}

/*--------------------------------------------------------------------------+
|           Helpers                                                         |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function : FindEntry(char* pszSection,                                   |
|                       char* pszEntry,                                     |
|  Task     : Find the specified Section/Entry                              |
+---------------------------------------------------------------------------+
|  Call     : FindEntry(pszSection, pszEntry)                               |
|                                                                           |
|  Parameter: pszSection                - Pointer to section name           |
|             pszEntry                  - Pointer to entry name             |
|                                                                           |
|  Return   : FindEntry                 - Status                            |
|                                         0 : Section not exist             |
|                                         1 : Section exist but entry not   |
|                                         2 : Entry exist                   |
|             m_pszLine                 - Buffer of the entry               |
|             m_pszPointer              - Pointer within the entry line     |
+--------------------------------------------------------------------------*/
int CProfile::FindEntry(const char* pszSection, const char* pszEntry, int* pnLineNo)
{
	char        pszSect[tmMAX_LINE_BUFFER];
	int         nFound = 0;
	char*       psz;
	
	strcpy(pszSect, "[");
	strcat(pszSect, pszSection);
	strcat(pszSect, "]");

	//=======
	//      Find the section
	//=======
	Rewind();                                           // Go to the first line
	*pnLineNo	= 0;
	while (ReadLine(0, m_pszLine, tmMAX_LINE_BUFFER) == tmFILE_GOOD)
    {
    	if (*m_pszLine != COMMENT_DELIMITER)
        {
        	m_pszPointer = strstr(m_pszLine, pszSect);
        	if (m_pszPointer != NULL)
            {
	            *pnLineNo		= m_nCurLine;
	            nFound++;                               // nFound = 1 : Find section
	            break;
            }
        }
    }

	//=======
	//      Find the entry
	//=======
	if (nFound == 1)                                    // Section is found
    {
    	while (ReadLine(0, m_pszLine, tmMAX_LINE_BUFFER) == tmFILE_GOOD)
        {
        	if (*m_pszLine != COMMENT_DELIMITER)
            {
	            m_pszPointer = strstr(m_pszLine, pszEntry);
	            if (m_pszPointer != NULL)
                {
	                psz = m_pszPointer + strlen(pszEntry);
	                if ((*psz == ' ')  ||               // Check if there is a legal separater after the entry
	                    (*psz == '\t') ||               // Otherwise FontS16Flag will be confused with FontS16 ... 04/30/96
	                    (*psz == '='))
                    {
	                    *pnLineNo	= m_nCurLine;
	                    nFound++;                       // nFound = 2 : Find entry
	                    break;
                    }
                }
	            m_pszPointer = strstr(m_pszLine, "[");  // Check if there is a '['
	            if (m_pszPointer != NULL)               // Next section occurred
	            {
	                *pnLineNo	= m_nCurLine;
	                break;
	            }
            }
        }
    }
	return nFound;
}
