/*===========================================================================+
|  Class    : CtmControl_User	                                        	 |
|  Task     : Mold Service Function	    	                     			 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 05/11/2008	                                                 |
|  Revision : 05/11/2008  				                                     |
+===========================================================================*/
#include	"tmControl_User.h"
#include	"../../utils.h"
#include	"../../tmdebug.h"

IMPLEMENT_DYNCREATE(CtmControl_User, CtmControl)

/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/

char*			g_apszUserID[] = {
	"USER_HMI_NULL_NULL_USERMASTERID",
	"USER_HMI_NULL_NULL_USER1ID",
	"USER_HMI_NULL_NULL_USER2ID",
	"USER_HMI_NULL_NULL_USER3ID",
	"USER_HMI_NULL_NULL_USER4ID",
	"USER_HMI_NULL_NULL_USER5ID",
	"USER_HMI_NULL_NULL_USER6ID",
	"USER_HMI_NULL_NULL_USER7ID",
	"USER_HMI_NULL_NULL_USER8ID",
	"USER_HMI_NULL_NULL_USER9ID",
	"USER_HMI_NULL_NULL_USER10ID"
};

char*			g_apszPasswdID[] = {
	"USER_HMI_NULL_NULL_USERMASTERPASSWORD",
	"USER_HMI_NULL_NULL_USER1PASSWORD",
	"USER_HMI_NULL_NULL_USER2PASSWORD",
	"USER_HMI_NULL_NULL_USER3PASSWORD",
	"USER_HMI_NULL_NULL_USER4PASSWORD",
	"USER_HMI_NULL_NULL_USER5PASSWORD",
	"USER_HMI_NULL_NULL_USER6PASSWORD",
	"USER_HMI_NULL_NULL_USER7PASSWORD",
	"USER_HMI_NULL_NULL_USER8PASSWORD",
	"USER_HMI_NULL_NULL_USER9PASSWORD",
	"USER_HMI_NULL_NULL_USER10PASSWORD"
};

char*			g_apszPrivilegeID[] = {
	"USER_HMI_NULL_NULL_USERMASTERPRIVILEGE",
	"USER_HMI_NULL_NULL_USER1PRIVILEGE",
	"USER_HMI_NULL_NULL_USER2PRIVILEGE",
	"USER_HMI_NULL_NULL_USER3PRIVILEGE",
	"USER_HMI_NULL_NULL_USER4PRIVILEGE",
	"USER_HMI_NULL_NULL_USER5PRIVILEGE",
	"USER_HMI_NULL_NULL_USER6PRIVILEGE",
	"USER_HMI_NULL_NULL_USER7PRIVILEGE",
	"USER_HMI_NULL_NULL_USER8PRIVILEGE",
	"USER_HMI_NULL_NULL_USER9PRIVILEGE",
	"USER_HMI_NULL_NULL_USER10PRIVILEGE"
};

char*			g_apszCurrentAccount[] = {
	"SYS_HMI_NULL_NULL_USERCURRENTID",
	"SYS_HMI_NULL_NULL_USERCURRENTPRIVILEGE"
};
/*===========================================================================+
|           Type	                                                         |
+===========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/	
/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::CtmControl_User()				             |
|  Task     :  CtmControl_User構造函數                                       |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_User::CtmControl_User(): CtmControl()
{
	
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::~CtmControl_User()				             |
|  Task     :  CtmControl_User析構函數                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_User::~CtmControl_User()
{
		
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::CreateSelf()				             	 |
|  Task     :  Initialize Error Service                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_User::CreateSelf()
{
	//InitControl();
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::FreeSelf()				             	     |
|  Task     :  Relase Error Service                                          |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_User::FreeSelf()
{
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::AddUser(char* pszID, char* pszPassword, DWORD dwPrivilege) |
|  Task     :  Add User									                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL    CtmControl_User::AddUser(char* pszID, char* pszPassword, DWORD dwPrivilege)
{
	short       nRecord = FindUser(pszID);
	DWORD		dwPasswd;

	if (nRecord < tmMAX_USER)
    {
	    //::MsgBox(g_MultiLanguage["MSG_USER_EXIST"],3);
	    MsgBoxCall("msgboxconfirm1.txt","MSG_USER_EXIST");
	    return FALSE;
    }
	else
    {
	    nRecord = FindFreeUser();
	    if (nRecord == tmMAX_USER)
        {
	        //::MsgBox(g_MultiLanguage["MSG_USER_FULL"],3);
	   			 MsgBoxCall("msgboxconfirm1.txt","MSG_USER_FULL");
	        return FALSE;
        }
    	else
        {
	        g_pDatabase->Write(g_apszUserID[nRecord], pszID);
	        dwPasswd	= Encode(pszPassword);
	        g_pDatabase->Write(g_apszPasswdID[nRecord], &dwPasswd);
	        g_pDatabase->Write(g_apszPrivilegeID[nRecord], &dwPrivilege);
	        return TRUE;
        }
    }	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::CtmControl_User::DeleteUser(char* pszID)		 |
|  Task     :  Delete User									                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL    CtmControl_User::CtmControl_User::DeleteUser(char* pszID)
{
	short       nRecord = FindUser(pszID);
	char		szName[256];

	if (nRecord == tmMAX_USER)
    {
		//::MsgBox(g_MultiLanguage["MSG_USER_NOTEXIST"],3);
	    MsgBoxCall("msgboxconfirm1.txt","MSG_USER_NOTEXIST");
	    return FALSE;
    }
	else
    {
	    memset(szName, 0, sizeof(szName));
	    g_pDatabase->Write(g_apszUserID[nRecord], szName);
	    g_pDatabase->Write(g_apszPrivilegeID[nRecord], 0);
	    return TRUE;
    }	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::FindFreeUser()                       		 |
|  Task     :  FindFree User									             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
short   CtmControl_User::FindFreeUser()
{
	short	i = 0;

	for (i=0; i<tmMAX_USER; i++)
    	if (g_pDatabase->Read(g_apszUserID[i]).pcData == NULL || g_pDatabase->Read(g_apszUserID[i]).pcData[0] == '\0') break;
	return i;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::FindUser()                       		 	 |
|  Task     :  Find   User									                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
short	CtmControl_User::FindUser(char* pszID)
{
	short	i = 0;
	
	for (i=0; i<tmMAX_USER; i++)
	{
    	if (g_pDatabase->Read(g_apszUserID[i]).pcData != NULL && strcmp(g_pDatabase->Read(g_apszUserID[i]).pcData, pszID) == 0) break;
  }
	return i;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::InitCurrentUser()                       		 |
|  Task     :  Init   User									                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControl_User::InitCurrentUser()
{
	g_pDatabase->Write(g_apszCurrentAccount[0], NULL); 
	SetDBValue(g_apszCurrentAccount[1], 0);	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::LoginLocal(char* pszID, char* pszPassword)    |
|  Task     :  LoginLocal   User									         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD    CtmControl_User::LoginLocal(char* pszID, char* pszPassword)
{
	printf("LoginLocal\n");
	printf("pszID=%s\n",pszID);
	short       i = 0;;
	if(Encode("TECHEDO") == Encode(pszPassword))	//root不判斷ｕｓｅｒ
		return 16;	
	if(Encode(pszID)==Encode("OP") && Encode(pszPassword)==Encode("1111"))// 預設使用者1
		return 1;
	if(Encode(pszID)==Encode("MANAGE") && Encode(pszPassword)==Encode("1234"))// 預設使用者2
		return 2;
	if(Encode(pszID)==Encode("ENG") && Encode(pszPassword)==Encode("123456"))// 預設使用者3
		return 3;
	if ((i = FindUser(pszID)) == tmMAX_USER)
	    return LOGIN_INVALIDID; 
	if (GetDBValue(g_apszPasswdID[i]).lValue != Encode(pszPassword))
	{
	    return LOGIN_INVALIDPASSWORD;
	}
	userindex = i;
	return GetDBValue(g_apszPrivilegeID[i]).lValue;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::LoginLocal(char* pszID, char* pszPassword)    |
|  Task     :  LoginLocal   User									         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           - 		                   			 |
|                                                                            |
|  Return   :                           -                                    | 
+---------------------------------------------------------------------------*/
WORD    CtmControl_User::GetPrivilefe(char* pszID)
{
	short       i = 0;;
		
	if ((i = FindUser(pszID)) == tmMAX_USER)
	{
		//::MsgBox(g_MultiLanguage["MSG_USER_NOTEXIST"],3);
	    MsgBoxCall("msgboxconfirm1.txt","MSG_USER_NOTEXIST");
	  return 999; 
	}
	return GetDBValue(g_apszPrivilegeID[i]).lValue;	
}



/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::ModifyUser(char* pszID, char* pszPassword, DWORD dwPrivilege)|
|  Task     :  Modify   User									             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL    CtmControl_User::ModifyUser(char* pszID, char* pszPassword, DWORD dwPrivilege)
{
	short       nRecord = FindUser(pszID);
	DWORD		dwPasswd;

	if (nRecord == tmMAX_USER)
    {
	    //::MsgBox(g_MultiLanguage["MSG_USER_NOTEXIST"],3);
	    MsgBoxCall("msgboxconfirm1.txt","MSG_USER_NOTEXIST");
	    return FALSE;
    }
	else
    {
	    if(pszPassword!=NULL)
	    {
	    	dwPasswd	= Encode(pszPassword);
	    	g_pDatabase->Write(g_apszPasswdID[nRecord], &dwPasswd);
	    }
	    if(dwPrivilege>0)
	    	g_pDatabase->Write(g_apszPrivilegeID[nRecord], &dwPrivilege);
	    return TRUE;
    }	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_User::SaveCurrentUser(char* pszName, WORD wPrivilege)|
|  Task     :  SaveCurrent   User									         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_User::SaveCurrentUser(char* pszName, WORD wPrivilege)
{
	DWORD	dwState;
	short	nRecord = FindUser(pszName);
	
	if (pszName == NULL || pszName[0] == '\0') return FALSE; //|| nRecord >= tmMAX_USER) return FALSE;
		
	dwState = g_pDatabase->Write(g_apszCurrentAccount[0], pszName).dwState; 
	dwState = dwState || SetDBValue(g_apszCurrentAccount[1], wPrivilege);
	return (dwState == DB_SUCCESS);	
}

WORD    CtmControl_User::Encode(char* psz)
{
	short       i;
	short       nLength;
	WORD        w = 168;
	
	if (psz == NULL || psz[0] == '\0')
		return w;
	nLength = strlen(psz);
	for (i=0; i<nLength; i++)
		w += (*psz++)*(i+1);
	printf("w = %d\n",w);
	return w;	
}

void    CtmControl_User::IniUser()
{

		//權限初始化
		g_pDatabase->Write(g_apszCurrentAccount[0], NULL); 
		SetDBValue(g_apszCurrentAccount[1], 0);
		//user初始化
		char* root_pass_str="TECHEDO"; 
		int	root_pass_word	= Encode(root_pass_str);
		int	root_priv_word	= 16;
		
		g_pDatabase->Write(g_apszUserID[0],"TECHROOT");
		g_pDatabase->Write(g_apszPasswdID[0], &root_pass_word);
		g_pDatabase->Write(g_apszPrivilegeID[0], &root_priv_word);
		
		
		char* idiot_pass_str="TECHMATION"; 
		int	idiot_pass_word	= Encode(idiot_pass_str);
		int	idiot_priv_word	= 15;
		g_pDatabase->Write(g_apszUserID[1],"ROOT");
		g_pDatabase->Write(g_apszPasswdID[1], &idiot_pass_word);
		g_pDatabase->Write(g_apszPrivilegeID[1], &idiot_priv_word);
		
		int Zero=0;
		char		szName[256];
	  memset(szName, 0, sizeof(szName));
		for(int i = 2 ; i<11 ; i++)
		{
			g_pDatabase->Write(g_apszUserID[i],szName);
			g_pDatabase->Write(g_apszPasswdID[i], &Zero);
			g_pDatabase->Write(g_apszPrivilegeID[i], &Zero);
		}
}

WORD    CtmControl_User::GetUserNum()
{
	return userindex;
}

#ifndef	D_BEOBJECT_CTMCONTROL_USER
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
	if (pResult != NULL)
	{
		(static_cast<CtmPackClass*>(pResult))->CreateSelf();
	}
	return pResult;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmPackClass*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}

#endif
