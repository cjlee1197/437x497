/*============================================================================+
|  Class    : ViewCodeEdit library                                    	 			|
|  Task     : ViewCodeEdit library  header file                       	 			|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	| 
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#ifndef		D__VIEWCODEEDIT
#define		D__VIEWCODEEDIT

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../libCNCComm.h"
#include	"../_kb.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);


void 		ShowProgram(int nProgramStartNo ,BOOL mode);	//20141007  Mario �[�Jmode�Ӥ����}�ҥؼХH�ζ}�ҼȦs�ɮ�
void 		SaveProgram(BOOL mode);		//20141007	Mario �[�Jmode�Ӥ����x�s�ɮץH�μȦs�ɮ�
void		TeachPointCheck(WORD wStartNo);
void		UpdateTable(WORD wStartNo);
void    CodeChange(char *pDst, char *pSrc);
void		GetNewStartNo();
void		NoUseTablePart(WORD wPartNo);
void		AddNewCommand();
void		ModifyCommand();


void		ChangeFile(CtmWnd* pwndSender);		//�p���q�ɮ׭������}��  �h��s�}���ɮ�
//20140923��
BOOL		FileCmy();													//���ɭY�P�Ȧs�ɬۦP�h�^��TRUE
//BOOL file_exists (char * fileName);					//�P�_�ɮ׬O�_�s�b 1:�s�b 0:���s�b
//20141007
void  SetCommandMode(int Mode);										//�]�w��ܪ����O����
void	CommandListShow	();									//���O�C��
void	SetListUpDown();										//�W�U���s
//20141023
//char* GetCommandStr(int mode , int Num);
void	CallCommandWork(int mode , int temp);		//�I�s���O�u�@�禡�Ϋ��O


//keyborad��������
void	ChangeNum2Sign(CtmWnd* m_pwndSender, int m_KeyMode);
void	OnKeyBorad();
void	OffKeyBorad();


//2015/6/15 �U�� 03:02:11
int		CallModifyCommand(char* f_Command);
void 	SetFileInfo(char* Str);

#endif
