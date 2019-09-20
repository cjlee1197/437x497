/*==========================================================================+
|  Function : 						                                        |
|  Task     :  																|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : zholy                                                         |
|  Version  : V1.00                                                         |
|  Creation : 2007/07/23                                                    |
|  Revision : 1.0                                                           |
+==========================================================================*/

#ifndef D__DES
#define	D__DES

#include "common.h"


/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/




/*==========================================================================+
|           External					                                    |
+==========================================================================*/
void     DES3_Encrypt_BurnAll(LONG *plData,WORD	wLength);
void     DES3_DisEncrypt_BurnAll(LONG *plData,WORD	wLength);
char*	 Revstr(char *str, size_t len)
{   
	char    *start = str;    
	char    *end = str + len - 1;    
	char    ch;    
	if (str != NULL)    
	{       
		while (start < end)        
		{            
			ch = *start;            
			*start++ = *end;            
			*end-- = ch;        
		}    
	}    
	return str;
}

#endif

