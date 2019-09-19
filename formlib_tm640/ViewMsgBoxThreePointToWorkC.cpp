/*============================================================================+
|  Class    : ViewMessageBoxTeachPoint library                               	|
|  Task     : ViewMessageBoxTeachPoint library action source file							|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                    	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewMsgBoxThreePointToWorkC.h"
#include	"../utils.h"
#include	"../commonaction.h"

/*========================================================+
|						Constants																			|
+========================================================*/

#define		INDEX_EDIT_TEACHPOINT_NO		0

#define SIGN(A)  ( (A) >= 0 ? 1: -1 )
//判斷字串顯示
#define		TeachPoint_TYPE_Delete				1
#define		TeachPoint_TYPE_Update				2
#define		TeachPoint_TYPE_Move					3

/*========================================================+
|						Global Variable																|
+========================================================*/

//判斷是否按下ENTER鍵
BOOL			u_bEnterKey							= FALSE;

CtmWnd*		u_pwndOPointTPNo					= NULL;
CtmWnd*		u_pwndOPointValue[3]			= {NULL};
CtmWnd*		u_pwndXPointTPNo					= NULL;
CtmWnd*		u_pwndXPointValue[3]			= {NULL};
CtmWnd*		u_pwndX_YPointTPNo				= NULL;
CtmWnd*		u_pwndX_YPointValue[3]		= {NULL};
CtmWnd*		u_pwndWorkCoordNo					= NULL;
CtmWnd*		u_pwndWorkCoordValue[6]		= {NULL};

CtmWnd*		u_pwndStaticUnit[12]			= {NULL};


CtmWnd*		u_pwndOK									= NULL;
CtmWnd*		u_pwndCancel							= NULL;

int 			u_nOPointValueCnt 		= 0;
int 			u_nXPointValueCnt 		= 0;
int 			u_nX_YPointValueCnt		= 0;
int 			u_nWorkCoordValueCnt 	= 0;
int 			u_nStaticUnitCnt 			= 0;

int				Unitindex							=	0;		//表示目前單位(0=公制,1=英制)，預設0	

int				OPointNo;
double 		OValue[3];
int				XPointNo;
double 		XValue[3];
int				X_YPointNo;
double 		X_YValue[3];
double 		u_WorkCoordValue[3];
int			WorkCoordIsOK	=	0;

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndOPointTPNo				= pwndSender->FindControlFromName("EditTP1");
	u_nOPointValueCnt				= GetSpecialControlNum(pwndSender, "EditTP1Value", "CtmEditX1", u_pwndOPointValue);
	
	u_pwndXPointTPNo				= pwndSender->FindControlFromName("EditTP2");
	u_nXPointValueCnt				= GetSpecialControlNum(pwndSender, "EditTP2Value", "CtmEditX1", u_pwndXPointValue);
	
	u_pwndX_YPointTPNo			= pwndSender->FindControlFromName("EditTP3");
	u_nX_YPointValueCnt			= GetSpecialControlNum(pwndSender, "EditTP3Value", "CtmEditX1", u_pwndX_YPointValue);
	
	u_pwndWorkCoordNo				= pwndSender->FindControlFromName("EditSetWorkCoordNo");
	u_nWorkCoordValueCnt		= GetSpecialControlNum(pwndSender, "EditWorkValue", "CtmEditX1", u_pwndWorkCoordValue);
	
	u_nStaticUnitCnt				= GetSpecialControlNum(pwndSender, "btnPlusC", "CtmToolButton", u_pwndStaticUnit);
	
	
	u_pwndOK								= pwndSender->FindControlFromName("btnOK");
	u_pwndCancel						= pwndSender->FindControlFromName("btnCancel");
	
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
	return TRUE;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{	
	CtmWnd*		pwndTemp = NULL; 
	pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
	if(wKey == _ENTER)
	{
		if(u_pwndOPointTPNo == pwndTemp)
		{
			int New_TPNO=0;
			LONG_AXIS TPValue;
			int Pos;
			u_pwndOPointTPNo->GetPropValueT("value", &New_TPNO, sizeof(OPointNo));
			if(GetTechPointValue(&TPValue,&Pos,New_TPNO))
			{
				AbsPos2MachinePos(TPValue,OValue);
				if(XPointNo!=0 && X_YPointNo!=0)  UpdateWorkCoord();
			}
			if(WorkCoordIsOK == -1)
			{
				u_pwndOPointTPNo->SetPropValueT("value",OPointNo);
				u_pwndOPointTPNo->Update();
			}
			else
			{
				OPointNo = New_TPNO;
				if(Unitindex==0)
				{
					u_pwndOPointValue[0]->SetPropValueT("value",OValue[0]);
					u_pwndOPointValue[1]->SetPropValueT("value",OValue[1]);
					u_pwndOPointValue[2]->SetPropValueT("value",OValue[2]);
				}
				else if(Unitindex==1)
				{
					u_pwndOPointValue[0]->SetPropValueT("value",CnangeMMtoIN(OValue[0]));
					u_pwndOPointValue[1]->SetPropValueT("value",CnangeMMtoIN(OValue[1]));
					u_pwndOPointValue[2]->SetPropValueT("value",CnangeMMtoIN(OValue[2]));
				}
				u_pwndOPointValue[0]->Update();
				u_pwndOPointValue[1]->Update();
				u_pwndOPointValue[2]->Update();
			}
		}
		else	if(u_pwndXPointTPNo == pwndTemp)
		{
			int New_TPNO=0;
			LONG_AXIS TPValue;
			int Pos;
			u_pwndXPointTPNo->GetPropValueT("value", &New_TPNO, sizeof(XPointNo));
			if(GetTechPointValue(&TPValue,&Pos,New_TPNO))
			{
				AbsPos2MachinePos(TPValue,XValue);
				if(OPointNo!=0 && X_YPointNo!=0)  UpdateWorkCoord();
			}
			if(WorkCoordIsOK==-1)
			{
				u_pwndXPointTPNo->SetPropValueT("value",XPointNo);
				u_pwndXPointTPNo->Update();
			}
			else
			{
				XPointNo = New_TPNO;
				if(Unitindex==0)
				{
					u_pwndXPointValue[0]->SetPropValueT("value",XValue[0]);
					u_pwndXPointValue[1]->SetPropValueT("value",XValue[1]);
					u_pwndXPointValue[2]->SetPropValueT("value",XValue[2]);
				}
				else if(Unitindex==1)
				{
					u_pwndXPointValue[0]->SetPropValueT("value",CnangeMMtoIN(XValue[0]));
					u_pwndXPointValue[1]->SetPropValueT("value",CnangeMMtoIN(XValue[1]));
					u_pwndXPointValue[2]->SetPropValueT("value",CnangeMMtoIN(XValue[2]));
				}
				u_pwndXPointValue[0]->Update();
				u_pwndXPointValue[1]->Update();
				u_pwndXPointValue[2]->Update();
			}
		}
		else	if(u_pwndX_YPointTPNo == pwndTemp)
		{
			int New_TPNO=0;
			LONG_AXIS TPValue;
			int Pos;
			u_pwndX_YPointTPNo->GetPropValueT("value", &New_TPNO, sizeof(X_YPointNo));
			if(GetTechPointValue(&TPValue,&Pos,New_TPNO))
			{
				AbsPos2MachinePos(TPValue,X_YValue);
				if(XPointNo!=0 && OPointNo!=0)  UpdateWorkCoord();
			}
			if(WorkCoordIsOK==-1)
			{
				u_pwndX_YPointTPNo->SetPropValueT("value",X_YPointNo);
				u_pwndX_YPointTPNo->Update();
			}
			else
			{
				X_YPointNo = New_TPNO;
				if(Unitindex==0)
				{
					u_pwndX_YPointValue[0]->SetPropValueT("value",X_YValue[0]);
					u_pwndX_YPointValue[1]->SetPropValueT("value",X_YValue[1]);
					u_pwndX_YPointValue[2]->SetPropValueT("value",X_YValue[2]);
				}
				else if(Unitindex==1)
				{
					u_pwndX_YPointValue[0]->SetPropValueT("value",CnangeMMtoIN(X_YValue[0]));
					u_pwndX_YPointValue[1]->SetPropValueT("value",CnangeMMtoIN(X_YValue[1]));
					u_pwndX_YPointValue[2]->SetPropValueT("value",CnangeMMtoIN(X_YValue[2]));
				}
				u_pwndX_YPointValue[0]->Update();
				u_pwndX_YPointValue[1]->Update();
				u_pwndX_YPointValue[2]->Update();
			}
		}
	}
}
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	if(u_pwndOK==pwndTemp)
	{
		int u_WorkCoordNO = 0;
		u_pwndWorkCoordNo->GetPropValueT("value",&u_WorkCoordNO,sizeof(u_WorkCoordNO));
		if(u_WorkCoordNO > 0 && u_WorkCoordNO < 25)
		{
			SetDB_WorkCoordValue(u_WorkCoordNO);
			Exit();
		}
		Exit();
	}
	else if(u_pwndCancel==pwndTemp)
	{
		Exit();
	}
	
	return	wIDControl;
}


void		OnUpdateA(CtmWnd* pwndSender)
{
	static	BOOL FristOne = TRUE;
	if(FristOne)
	{	
		u_pwndWorkCoordNo->Update();
		FristOne = FALSE;
	}
}
/*========================================================+
|						Helper																				|
+========================================================*/

void	UpdateWorkCoord()
{
	double E[3][3];
	double X[3],X_Y[3],Y[3],Z[3];
	double X_len,Y_len,Z_len;
	
	X[0]=(XValue[0]-OValue[0])/1000;
	X[1]=(XValue[1]-OValue[1])/1000;
	X[2]=(XValue[2]-OValue[2])/1000;
	X_len = sqrt(X[0]*X[0]+X[1]*X[1]+X[2]*X[2]);
	//printf("X=[%.2f  %.2f  %.2f]\n",X[0],X[1],X[2]);
	X_Y[0]=(X_YValue[0]-OValue[0])/1000;
	X_Y[1]=(X_YValue[1]-OValue[1])/1000;
	X_Y[2]=(X_YValue[2]-OValue[2])/1000;
	//printf("X_Y=[%.2f  %.2f  %.2f]\n",X_Y[0],X_Y[1],X_Y[2]);
	
	Z[0]=X[1]*X_Y[2]-X[2]*X_Y[1];
	Z[1]=X[2]*X_Y[0]-X[0]*X_Y[2];
	Z[2]=X[0]*X_Y[1]-X[1]*X_Y[0];
	Z_len = sqrt(Z[0]*Z[0]+Z[1]*Z[1]+Z[2]*Z[2]);
	//printf("Z=[%.2f  %.2f  %.2f]\n",Z[0],Z[1],Z[2]);
	
	Y[0]=Z[1]*X[2]-Z[2]*X[1];
	Y[1]=Z[2]*X[0]-Z[0]*X[2];
	Y[2]=Z[0]*X[1]-Z[1]*X[0];
	Y_len = sqrt(Y[0]*Y[0]+Y[1]*Y[1]+Y[2]*Y[2]);
	//printf("Y=[%.2f  %.2f  %.2f]\n",Y[0],Y[1],Y[2]);
	
	E[0][0]=X[0]/X_len;
	E[1][0]=X[1]/X_len;
	E[2][0]=X[2]/X_len;
	E[0][1]=Y[0]/Y_len;
	E[1][1]=Y[1]/Y_len;
	E[2][1]=Y[2]/Y_len;
	E[0][2]=Z[0]/Z_len;
	E[1][2]=Z[1]/Z_len;
	E[2][2]=Z[2]/Z_len;
	//printf("E=\n[%.2f  %.2f  %.2f]\n[%.2f  %.2f  %.2f]\n[%.2f  %.2f  %.2f]\n",E[0][0],E[0][1],E[0][2],E[1][0],E[1][1],E[1][2],E[2][0],E[2][1],E[2][2]);
	double f_WorkCoordValue[6];
	f_WorkCoordValue[4]=atan2(E[0][2],sqrt(pow2(E[1][2])+pow2(E[2][2])));
	if(fabs(fabs(f_WorkCoordValue[4])-M_PI)<0.001) 
	{
		WorkCoordIsOK=-1;
		return;
	}
	else  
	{
		WorkCoordIsOK = 1;
		f_WorkCoordValue[3]=atan2(-E[1][2]/cos(f_WorkCoordValue[4]),E[2][2]/cos(f_WorkCoordValue[4]));
		f_WorkCoordValue[5]=atan2(-E[0][1]/cos(f_WorkCoordValue[4]),E[0][0]/cos(f_WorkCoordValue[4]));
		
		u_WorkCoordValue[3]= f_WorkCoordValue[3]/M_PI*180.0*1000;
		u_WorkCoordValue[4]= f_WorkCoordValue[4]/M_PI*180.0*1000;
		u_WorkCoordValue[5]= f_WorkCoordValue[5]/M_PI*180.0*1000;
	}
	u_WorkCoordValue[0]=OValue[0];
	u_WorkCoordValue[1]=OValue[1];
	u_WorkCoordValue[2]=OValue[2];
	
	if(Unitindex==0)
	{
		u_pwndWorkCoordValue[0]->SetPropValueT("value",u_WorkCoordValue[0]);
		u_pwndWorkCoordValue[1]->SetPropValueT("value",u_WorkCoordValue[1]);
		u_pwndWorkCoordValue[2]->SetPropValueT("value",u_WorkCoordValue[2]);
	}
	else if(Unitindex==1)
	{
		u_pwndWorkCoordValue[0]->SetPropValueT("value",CnangeMMtoIN(u_WorkCoordValue[0]));
		u_pwndWorkCoordValue[1]->SetPropValueT("value",CnangeMMtoIN(u_WorkCoordValue[1]));
		u_pwndWorkCoordValue[2]->SetPropValueT("value",CnangeMMtoIN(u_WorkCoordValue[2]));
	}
	u_pwndWorkCoordValue[3]->SetPropValueT("value",u_WorkCoordValue[3]);
	u_pwndWorkCoordValue[4]->SetPropValueT("value",u_WorkCoordValue[4]);
	u_pwndWorkCoordValue[5]->SetPropValueT("value",u_WorkCoordValue[5]);
	
	u_pwndWorkCoordValue[0]->Update();
	u_pwndWorkCoordValue[1]->Update();
	u_pwndWorkCoordValue[2]->Update();
	u_pwndWorkCoordValue[3]->Update();
	u_pwndWorkCoordValue[4]->Update();
	u_pwndWorkCoordValue[5]->Update();
				
}

double	CnangeMMtoIN(double f_value)		//公轉英 且精度+1
{
	f_value	=	f_value/g_Metric_Imperial_Unit*10;	
	f_value += 0.5*SIGN(f_value);
	return f_value;
}

double	pow2(double a)
{
	return a * a ;
}


void		SetDB_WorkCoordValue(int NO)
{	
	int u_WorkCoord_StartNo = g_pDatabase->GetIndex("CNC_WORK_COORD1_X");
	for(int i = 0 ; i < u_nWorkCoordValueCnt ; i++)
	{
		double lDBvalue 	= 0; 
		int 	temp = 0;
		char*	pDataID	= NULL;
		u_pwndWorkCoordValue[i]->GetPropValueT("value",&temp,sizeof(temp));
		lDBvalue = temp;
		if(Unitindex && i < 3)
			lDBvalue = lDBvalue*g_Metric_Imperial_Unit/10;
		//printf("lDBvalue is %f\n",lDBvalue);
		pDataID = g_pDatabase->GetString(u_WorkCoord_StartNo + (NO-1)*6 + i );
		SetDBValue(pDataID,(int)lDBvalue);
	}
}

void	AbsPos2MachinePos(LONG_AXIS u_AbsPos,double outMachinePos[3])	//-1為目前使用坐標系
{
	double AbsPos[6];
	double WorkCoord[6];
	double MachineMatrix[4][4];
	double AbsMatrix[4][4];
	double WorkMatrix[4][4];
	
	LONG_AXIS2double(g_TxCNCData.shmCNCData.CNC_work_coord[g_TxCNCData.shmCNCData.CNC_work_coord_no],WorkCoord,1000.0);
	LONG_AXIS2double(u_AbsPos,AbsPos,1000.0);
	
	CartesianToMatrix(AbsPos,AbsMatrix);
	
	CartesianToMatrix(WorkCoord,WorkMatrix);
	mt_DHmul(WorkMatrix,AbsMatrix,MachineMatrix);
	
	outMachinePos[0] = MachineMatrix[0][3] + g_TxCNCData.shmCNCData.CNC_work_coord[0].x/1000.0;
	outMachinePos[1] = MachineMatrix[1][3] + g_TxCNCData.shmCNCData.CNC_work_coord[0].y/1000.0;
	outMachinePos[2] = MachineMatrix[2][3] + g_TxCNCData.shmCNCData.CNC_work_coord[0].z/1000.0;
	
	outMachinePos[0]*=1000;
	outMachinePos[1]*=1000;
	outMachinePos[2]*=1000;
	return;
}


void LONG_AXIS2double(LONG_AXIS Pos , double outPos[6] , double unit)
{
	double n_unit = unit;
	if( n_unit == 0 )n_unit = 1.0;
	outPos[0] = (double)Pos.x/n_unit;
	outPos[1] = (double)Pos.y/n_unit;
	outPos[2] = (double)Pos.z/n_unit;
	outPos[3] = (double)Pos.a/n_unit;
	outPos[4] = (double)Pos.b/n_unit;
	outPos[5] = (double)Pos.c/n_unit;
}

void  CartesianToMatrix(double pos[6] , double E[4][4]) //pos[6]單位: mm & deg 
{
	double Angle2Theta[6];
	Angle2Theta[0] = pos[0];
	Angle2Theta[1] = pos[1];
	Angle2Theta[2] = pos[2];
	Angle2Theta[3] = pos[3]/180*M_PI;
	Angle2Theta[4] = pos[4]/180*M_PI;
	Angle2Theta[5] = pos[5]/180*M_PI;
	E[0][0] = cos(Angle2Theta[4])*cos(Angle2Theta[5]);
	E[0][1] = -cos(Angle2Theta[4])*sin(Angle2Theta[5]);
	E[0][2] = sin(Angle2Theta[4]);
	E[0][3] = Angle2Theta[0];
	E[1][0] = sin(Angle2Theta[3])*sin(Angle2Theta[4])*cos(Angle2Theta[5])+cos(Angle2Theta[3])*sin(Angle2Theta[5]);
	E[1][1] = -sin(Angle2Theta[3])*sin(Angle2Theta[4])*sin(Angle2Theta[5])+cos(Angle2Theta[3])*cos(Angle2Theta[5]);
	E[1][2] = -sin(Angle2Theta[3])*cos(Angle2Theta[4]);
	E[1][3] = Angle2Theta[1];
	E[2][0] = -cos(Angle2Theta[3])*sin(Angle2Theta[4])*cos(Angle2Theta[5])+sin(Angle2Theta[3])*sin(Angle2Theta[5]);
	E[2][1] = cos(Angle2Theta[3])*sin(Angle2Theta[4])*sin(Angle2Theta[5])+sin(Angle2Theta[3])*cos(Angle2Theta[5]);
	E[2][2] = cos(Angle2Theta[3])*cos(Angle2Theta[4]);
	E[2][3] = Angle2Theta[2];
	E[3][0] = 0;
	E[3][1] = 0;
	E[3][2] = 0;
	E[3][3] = 1;
}

void mt_DHmul(double A[4][4], double B[4][4], double output[4][4])
{
	double DHbuf[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,1}};
	double Abuf[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,1}};
	double Bbuf[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,1}};
	unsigned char i=0,j=0;

	for (i=0;i<4;i++){
		for (j=0;j<4;j++){
			Abuf[i][j]=A[i][j];
			Bbuf[i][j]=B[i][j];
		}
	}
	for (i=0;i<4;i++){
		for (j=0;j<4;j++){
			DHbuf[i][j]=Abuf[i][0]*Bbuf[0][j]
				+Abuf[i][1]*Bbuf[1][j]
				+Abuf[i][2]*Bbuf[2][j]
				+Abuf[i][3]*Bbuf[3][j];
			output[i][j]=DHbuf[i][j];
		}
	}
}