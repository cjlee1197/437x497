/*============================================================================+
|  Class    : ViewMagBox library                                            	|
|  Task     : ViewMagBox library action source file                          	|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

#include	"ViewMsgBoxSetWorkCoordSeat.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

#define SIGN(A)  ( (A) >= 0 ? 1: -1 )
/*========================================================+
|						Global Variable																|
+========================================================*/

//�P�_�O�_���UENTER��
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;

CtmWnd*		u_pwndCoordTable[6]					= {NULL};
int				u_nCoordTableCnt						=	0;
CtmWnd*		u_pwndOriginTable[6]				= {NULL};
int				u_nOriginTableCnt						=	0;

CtmWnd*		u_pwndOffsetTable[6]				= {NULL};
int				u_nOffsetTableCnt						=	0;
CtmWnd*		u_pwndWorkCoordTable[6]			= {NULL};
int				u_nWorkCoordTableCnt				=	0;
CtmWnd*		u_pwndStaticUnit[12]				= {NULL};
int				u_nStaticUnitCnt						=	0;

CtmWnd*		u_pwndEditWorkCoordNO				= NULL;


int			Unitindex							=0;		//��ܥثe���(0=����,1=�^��)�A�w�]0

WORD		u_wDBvalueC_StartNo					=0;		//����Ѽƽs���G�d��y�Ъ�X�y��
WORD		u_wDBvalueOrigin_StartNo		=0;		//����Ѽƽs���G����y�Ъ�X�y��

/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy��
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//��e�y��
	u_nCoordTableCnt		= GetSpecialControlNum(pwndSender, "CTable", 				"CtmEditX1", u_pwndCoordTable);
	u_nOriginTableCnt		= GetSpecialControlNum(pwndSender, "OTable", 				"CtmEditX1", u_pwndOriginTable);
	
	//�w�q�y�Ц�m
	u_nOffsetTableCnt		= GetSpecialControlNum(pwndSender, "EditOffset", 		"CtmEditX1", u_pwndOffsetTable);
	
	//�u�@�y�Э�
	u_nWorkCoordTableCnt= GetSpecialControlNum(pwndSender, "EditWorkValue", "CtmEditX1", u_pwndWorkCoordTable);
	
	//�u�@�y�нs��
	u_pwndEditWorkCoordNO	=	pwndSender->FindControlFromName("EditSetWorkCoordNo");
	
	//���
	u_nStaticUnitCnt= GetSpecialControlNum(pwndSender, "StaticUnit", "CtmStaticX2", u_pwndStaticUnit);
	
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	
	
	u_wDBvalueOrigin_StartNo	= g_pDatabase->GetIndex(MACHINE_COORD_ID);
	u_wDBvalueC_StartNo		   	= g_pDatabase->GetIndex(ABSOLUTE_COORD_ID);
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	for(int i = 0 ; i < u_nOffsetTableCnt ; i++ )
	{
		if(pwndTemp == u_pwndOffsetTable[i])
		{
			ShowWorkCoordValue();
			return wIDControl;
		}
	}
	return wIDControl;
}
//20140624��
WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

	if(pwndTemp == u_pwndBtnCancel)
	{
		Exit();
	}
	else if(pwndTemp == u_pwndBtnOK)
	{
		int u_WorkCoordNO = 0;
		u_pwndEditWorkCoordNO->GetPropValueT("value",&u_WorkCoordNO,sizeof(u_WorkCoordNO));
		if(u_WorkCoordNO > 0 && u_WorkCoordNO < 25)
		{
			SetDB_WorkCoordValue(u_WorkCoordNO);
			Exit();
		}
		else
			;
	}
	return wIDControl;
}




void		OnUpdateA(CtmWnd* pwndSender)
{
	static BOOL FristOne = TRUE;
	
	ShowCurrentCoordinate(); 
	
	if(FristOne)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();		
		u_pwndEditWorkCoordNO->Update();
		FristOne = FALSE;
		SetUnit();
		ShowWorkCoordValue();
		((CtmFormView*)pwndSender)->OnGetFocus();		
	}
}


/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/


void		ShowCurrentCoordinate()		//��ܾ�����u�ثe���y�Ц�m
{
	for(int i =0; i<u_nCoordTableCnt; i++)		//�d��y��(Cartesian)		POS�t�~�B�z
	{
		if(u_pwndCoordTable[i] != NULL)
		{
			//���o����Ѽƪ����ID
      char*	pDataID 	= NULL;
      pDataID 	= g_pDatabase->GetString(u_wDBvalueC_StartNo+i);
      //printf("pDataID :  %s %d %d\n", pDataID, u_wDBvalueC_StartNo, i);
      
      //�ھڸ��ID�A���o����Ѽƪ���
      //long	lDBvalue	=0;																							
      double	lDBvalue	=0;																							
      lDBvalue	= GetDBValue(pDataID).lValue;
      
      if(i < 3)																									   
      {																																
				lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
				lDBvalue += 0.5*SIGN(lDBvalue);																								
				u_pwndCoordTable[i]->SetPropValueT("precision",3+Unitindex);		
			}																												
      u_pwndCoordTable[i]->SetPropValueT("value", lDBvalue);
      u_pwndCoordTable[i]->Update();
		}
		if(u_pwndOriginTable[i] != NULL)
		{
			//���o����Ѽƪ����ID
      char*	pDataID 	= NULL;
      pDataID 	= g_pDatabase->GetString(u_wDBvalueOrigin_StartNo+i);
      //printf("pDataID :  %s %d %d\n", pDataID, u_wDBvalueC_StartNo, i);
      
      //�ھڸ��ID�A���o����Ѽƪ���
      //long	lDBvalue	=0;																							
      double	lDBvalue	=0;																							
      lDBvalue	= GetDBValue(pDataID).lValue;
      
      if(i < 3)																									   
      {																																
				lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
				lDBvalue += 0.5*SIGN(lDBvalue);																								
				u_pwndOriginTable[i]->SetPropValueT("precision",3+Unitindex);		
			}																												
      u_pwndOriginTable[i]->SetPropValueT("value", lDBvalue);
      u_pwndOriginTable[i]->Update();
		}	
	}
}

void 		SetUnit()
{
	for(int i = 0 ; i < 3 ; i++)
	{
		if(u_pwndCoordTable[i] != NULL)
		{
			u_pwndCoordTable[i]->SetPropValueT("precision",3+Unitindex);
			u_pwndCoordTable[i]->Update();
		}
		if(u_pwndOriginTable[i] != NULL)
		{
			u_pwndOriginTable[i]->SetPropValueT("precision",3+Unitindex);
			u_pwndOriginTable[i]->Update();
		}
		if(u_pwndOffsetTable[i] != NULL)
		{
			u_pwndOffsetTable[i]->SetPropValueT("precision",3+Unitindex);
			u_pwndOffsetTable[i]->Update();
		}
		if(u_pwndWorkCoordTable[i] != NULL)
		{
			u_pwndWorkCoordTable[i]->SetPropValueT("precision",3+Unitindex);
			u_pwndWorkCoordTable[i]->Update();
		}
	}
	for(int i = 0 ; i < u_nStaticUnitCnt ; i++ )
	{
		if(Unitindex)
			u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_INCH");
		else
			u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_MM");
		u_pwndStaticUnit[i]->Update();
	}
}

void		ShowWorkCoordValue()
{
	int u_EXOFS_StartNo		= g_pDatabase->GetIndex("CNC_WORK_COORD0_X");
	for(int i = 0 ; i < u_nOffsetTableCnt ; i++)
	{
		char*	pDataMachineID	= NULL;
		char*	pDataEXOFSID 		= NULL;
		int 	Machine_Value 	= 0;
		int 	EXOFS_Value 		= 0;
		int 	Offset_Value 		= 0;
		int		temp 						= 0;
		//���o�����y��
		u_pwndOriginTable[i]->GetPropValueT("value",&Machine_Value,sizeof(Machine_Value));
		
		//���oEXOFS��
		pDataEXOFSID 			= g_pDatabase->GetString(u_EXOFS_StartNo+i);
		double lDBvalue 	= 0;
		lDBvalue	=  GetDBValue(pDataEXOFSID).lValue;
		lDBvalue	=	 lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
		lDBvalue	+= 0.5*SIGN(lDBvalue);	
		EXOFS_Value = lDBvalue;
		
		//�����w�q�y�Ц�m
		u_pwndOffsetTable[i]->GetPropValueT("value",&Offset_Value,sizeof(Offset_Value));
		
		//�p��u�@�y�Э�
		temp = Machine_Value - EXOFS_Value - Offset_Value ;
		u_pwndWorkCoordTable[i]->SetPropValueT("value",temp);
		u_pwndWorkCoordTable[i]->Update();
	}
}

void		SetDB_WorkCoordValue(int NO)
{	
	int u_WorkCoord_StartNo = g_pDatabase->GetIndex("CNC_WORK_COORD1_X");
	for(int i = 0 ; i < u_nWorkCoordTableCnt ; i++)
	{
		double lDBvalue 	= 0; 
		int 	temp = 0;
		char*	pDataID	= NULL;
		u_pwndWorkCoordTable[i]->GetPropValueT("value",&temp,sizeof(temp));
		lDBvalue = temp;
		if(Unitindex && i < 3)
			lDBvalue = lDBvalue*g_Metric_Imperial_Unit/10;
		//printf("lDBvalue is %f\n",lDBvalue);
		pDataID = g_pDatabase->GetString(u_WorkCoord_StartNo + (NO-1)*6 + i );
		SetDBValue(pDataID,(int)lDBvalue);
	}
}