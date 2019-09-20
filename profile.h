/*===========================================================================+
|  Class    : Profile                                                        |
|  Task     : Program Profile header file                                    |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : netdigger                                                      |
|  Version  : V1.00                                                          |
|  Creation : 03/12/1995                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_PROFILE
#define     D_PROFILE

#include    "files.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     SECTION_FOUND       1
#define     ENTRY_FOUND         2
#define     COMMENT_DELIMITER   ';'

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - Profile                                      |
+===========================================================================*/
class CProfile : public CtmTextFile
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CProfile();
    CProfile(char* pszName);
    ~CProfile();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    char    m_pszLine[tmMAX_LINE_BUFFER]; // Line buffer
    char*   m_pszPointer;               // Pointer within the line buffer

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    int     GetProfileString(const char* pszSection, const char* pszEntry, const char* pszDefault, char* pszBuffer, UINT nBufferSize);
    int     GetProfileInt(const char* pszSection, const char* pszEntry, int nDefault);
    int     WriteProfileString(const char* pszSection, const char* pszEntry, char* pszValue);
    int     WriteProfileInt(const char* pszSection, const char* pszEntry, int nValue);

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
    int     FindEntry(const char* pszSection, const char* pszEntry, int* pnLineNo);
};

#endif
