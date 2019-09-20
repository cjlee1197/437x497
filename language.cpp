/*===========================================================================+
|  Class    : Language                                                       |
|  Task     : Language                                                       |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52 -                                                   |
|  Link     : MSVC V1.52 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/27/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"language.h"
#include	"stable.h"

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/


/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
short       g_nMaxLanguage = 2;     			
char*       pLanguageString[PA_MAX_FILE];	
//WORD		g_wLangCode[PA_MAX_FILE];		
short		g_nLanguageIndex = 0;       	

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
char**      GetLanguageList()
{ 
short		i;

//for (i=0; i<g_nMaxLanguage; i++) 

	pLanguageString[0] = g_MultiLanguage["DLG_SYSTEM_CETS_CHINESE"];
	pLanguageString[1] = g_MultiLanguage["DLG_SYSTEM_CETS_ENGLISH"];

return	pLanguageString;
};






