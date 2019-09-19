/*============================================================================+
|  Class    : ViewOver library                                  	 						|
|  Task     : ViewOver library  header file                     	 						|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                      |
|  Link     :G++(GCC)3.2                                                      |
|  Call     :                                                                 |
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                        |
|  Version  : V1.00                                                           |
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Roy	rewrite
#ifndef		D__VIEWFILEMANAGE
#define		D__VIEWFILEMANAGE

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../libCNCComm.h"
#include	"../_kb.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);

void		OpenFile(CtmWnd* pwndSender, int nFileIndex);
void		CopyFile();
void		DelFile();
void		RenameFile();
void		CreateFile();

void		UpdateFileList();
void		UpdateFileList(char* FileStr);
void		ReLoadDirectoryAndFile();
BOOL		DirectoryLock();
void		GetNewFileName();
BOOL		FocusLock(CtmWnd* pwndSender);

void		PasteFile();
//20140912 ¡ô by mario

//void		ShowCurrentName(CtmWnd* pwndFileExplorer);
//void		ShowCurrentPath(CtmWnd* pwndFileExplorer);
void		ShowCurrentFileName(CtmWnd* pwndFileExplorer, int nIndex);
//void		ShowCurrentDirCount();
//void		ShowCurrentFileCount();
//void		ShowCurrentFocusIndex(CtmWnd* pwndFileExplorer);
//void		ShowFileList();

#endif
