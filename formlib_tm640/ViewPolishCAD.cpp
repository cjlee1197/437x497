/*============================================================================+
|  Class    : ViewManualMode library                	                  			|
|  Task     : ViewManualMode library  header file  	  	                 			|
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
#include <dirent.h>
#include	"ViewPolishCAD.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"
#include	"../tmshmsg.h"
#include 	"../taskcnc.h"
#include 	"../gui.h"
#include 	"../tmFileExplorer.h"

/*============================================================================+
|           Constant                                                     			|
+============================================================================*/
#define CAD_FILE_PATH "./Data/PolishCADFile/"


#define POLISHPATHFILE_1 "faucetpath_S"
#define POLISHPATHFILE_2 ".csv"
#define POLISHCONFIGFILE "faucetpath.cfg"

#define POLISHPATHFILE "faucetpath.csv"
#define OUT_FILE	 "Mario.csv"

#define D_INT 0
#define D_MATRIX3 1
#define D_MATRIX4 2

char* Config_Str[]=
{
	"POLISH_MODE",
	"WORK_COORD_SYSTEM",
	"WHEEL_COORD_SYSTEM",
	NULL,
};
/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/

CtmWnd*		u_selecteditObject		= NULL;
CtmWnd*		u_BtnPathCreate		= NULL;
CtmWnd*		u_memoPath		= NULL;
CtmWnd*		u_editTool		= NULL;
CtmWnd*		u_editWork		= NULL;
CtmWnd*		u_editSpeed		= NULL;
CtmWnd*		u_editFileName		= NULL;
CtmWnd*		u_btnCreateProg		= NULL;
CtmWnd*		u_btnProgType		= NULL;

CtmWnd*		u_StaticInfoMark  = NULL;
CtmWnd*		u_StaticAngle		= NULL;
CtmWnd*		u_EditAngle		= NULL;
CtmWnd*		u_ImageWheel		= NULL;
                     

FILENAME* cdf_head=NULL; 
FILENAME* cdf_end=NULL;  
int cdf_Num=0;            
int sel_cdf_index=-1;   
BOOL path_ok=FALSE;          
int Mode=0;
int ProgType = 0;




//============================================起始校正用資料(將座標系平移至工具工件交叉點) 
double O_xyz[3]={0,0,-50};
double P_x[3]={0,0,-42.74};
double P_xy[3]={7.26,0,-50};
double inv_Work_E[4][4];
//============================================砂輪機磨點
double I_uvw[4][4]={{0,0,1,0},{0,1,0,0},{-1,0,0,0},{0,0,0,1}};	//工件向量與砂輪機座標系夾角Ry=90度 (預設 工件向量(工作座標係X+方向)=>砂輪機工作座標係Z-)) 
//============================================
int		total_path;  //必須手動自己設定輸入的路徑數 
char	*dir_path;
int 	total_point=0;
int   ChangeFile=1;
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_selecteditObject 	= pwndSender->FindControlFromName("SelectTargetObject");
	u_BtnPathCreate 		= pwndSender->FindControlFromName("BtnPathPointCreate");
	
	u_StaticInfoMark 	= pwndSender->FindControlFromName("StaticInfoMark");
	u_StaticAngle 		= pwndSender->FindControlFromName("StaticAngle");
	u_EditAngle 			= pwndSender->FindControlFromName("EditAngle");
	u_ImageWheel 			= pwndSender->FindControlFromName("ImageInfo");

	u_memoPath 					= pwndSender->FindControlFromName("PathMemo");
	
	u_editTool 					= pwndSender->FindControlFromName("EditCoordinateWork");
	u_editWork 					= pwndSender->FindControlFromName("EditCoordinateTool");
	u_editSpeed 				= pwndSender->FindControlFromName("EditPathSpeed");
	

	u_editFileName 			= pwndSender->FindControlFromName("EditFileName");
	u_btnProgType 			= pwndSender->FindControlFromName("BtnProgType");
	u_btnCreateProg 		= pwndSender->FindControlFromName("BtnOutputGenerator");
	//FindCADFile();
	return	TRUE;
}  

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp == u_BtnPathCreate)
	{ 
		path_ok=FALSE;  
		((CtmFormView*)pwndSender)->Goto(1);
		int a = CalPath();
		if(a==1)
			MsgBoxCall("msgboxConfirm1.txt","ROBOT_STR_FILE_NO_EXIST");
		if(a==2)
			MsgBoxCall("msgboxConfirm1.txt","ROBOT_STR_FILE_ERROR");
		if(a==0)
			path_ok=TRUE;  
	}
	else if(pwndTemp == u_btnCreateProg)
	{
		if(path_ok)
		{
			int a = CreateProg();	
			if(a==1)
				MsgBoxCall("msgboxConfirm1.txt","ROBOT_STR_SPEED_PARAM_ERROR");
			if(a==2)
				MsgBoxCall("msgboxConfirm1.txt","ROBOT_STR_PLEASE_ENTER_NAME");
			else 
				MsgBoxCall("msgboxConfirm1.txt","ROBOT_BTN_CREATE_PROG_OK");
		}
		else
			MsgBoxCall("msgboxConfirm1.txt","ROBOT_STR_NOT_CREATE_PATH");
			
	}
	else if(pwndTemp == u_btnProgType)
	{
		ProgType++;
		if(ProgType>1)ProgType=0;
		
		if(ProgType==0)
			u_btnProgType->SetPropValueT("captionID","ROBOT_STR_PROG_MAIN");
		else if(ProgType==1)
			u_btnProgType->SetPropValueT("captionID","ROBOT_STR_PROG_SUB");
		u_btnProgType->CreateA();
		u_btnProgType->Update();
	}
	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	return	((CtmFormView*)pwndSender)->OnKey1(wKey);
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	if(pwndTemp == u_selecteditObject)
	{
		sel_cdf_index = ((CtmSelectEdit*)u_selecteditObject)->GetIndex();
		dir_path=GetCDFFileName();
		ReadPolishConfig();
		SetInfoShow(pwndSender);
		((CtmMTextMemo*)u_memoPath)->CreateA();
		((CtmMTextMemo*)u_memoPath)->Update();
		ChangeFile=1;
	}
	return wIDControl;
}
void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{	

}

void	OnDestroyA(CtmWnd* pwndSender)
{
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	static BOOL StartRunOne = TRUE;
	if(StartRunOne)
	{
		StartRunOne=FALSE;
		FindCADFile();
		dir_path=GetCDFFileName();
		ReadPolishConfig();
		SetInfoShow(pwndSender);
	}
}

/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/

void FindCADFile()
{
	BOOL full=FALSE;
	struct dirent **file;
	int n = scandir(CAD_FILE_PATH, &file, 0, alphasort);
	int i=0;
	//printf("CAD_FILE_PATH = %s\n",CAD_FILE_PATH);
	FILENAME* temp=NULL;
	while(n>0)
	{
		n--;
		if(!full && file[n]->d_type == 4)/* && file[n]->d_type*/
		{
			if(file[n]->d_name[0]=='.') continue;
			if(cdf_Num>=20)
				full=TRUE;
			else
			{
				SetSelectEditSTR(i,file[n]->d_name);
				i++;
				int name_len = strlen(CAD_FILE_PATH)+strlen(file[n]->d_name)+3;
				temp = (FILENAME*)malloc(sizeof(FILENAME));
				temp->next = NULL;
				temp->name = (char*)malloc(name_len);
				memset(temp->name,'\0',sizeof(name_len));
				sprintf(temp->name,"%s\%s\0",CAD_FILE_PATH,file[n]->d_name);
				//printf("1name = %s\n",temp->name);
				//printf("file = %d\n",file[n]->d_type);
				
				if(cdf_end==NULL)
					cdf_head=cdf_end=temp;
				else			
				{
					cdf_end->next = temp;
					cdf_end = cdf_end->next;
				}
				cdf_Num++;
			}
		}
		free(file[n]);
	}
	
	if(i>5)
		u_selecteditObject->SetPropValueT("listnum",5);
	else
		u_selecteditObject->SetPropValueT("listnum",i);
	((CtmSelectEdit*)u_selecteditObject)->SetIndex(0);
	((CtmSelectEdit*)u_selecteditObject)->UpdateAll();
	if(cdf_Num > 0)
		sel_cdf_index = 0;
}

void	SetSelectEditSTR(int no, char* str)
{
	char str1[2*strlen(str)+4];
	char strX[6];
	memset(str1,'\0',2*strlen(str)+4);
	memset(strX,'\0',6);
	
	WORD	*pwDst = (WORD *)str1;
  while(*str != '\0')      
  	*pwDst++ = *str++;
  *pwDst ='\0';  
  
  sprintf(strX,"str%d",no);
  //printf("strX = %s\n",strX);
	u_selecteditObject->SetPropValueT(strX,str1);
}

int CalPath()
{
	if(dir_path==NULL) return 1	;
	if(ChangeFile==0) return 0;
	((CtmMTextMemo*)u_memoPath)->CreateA();
	((CtmMTextMemo*)u_memoPath)->Update();
	ChangeFile=0;
	if(Mode == 1)
	{
		char cdf_file[512];
		int angel;
		u_EditAngle->GetPropValueT("value",&angel,sizeof(angel));
		sprintf(cdf_file,"%s/%s\0",dir_path,POLISHPATHFILE);
		if(pathpointproduct(0,angel/1000.0,cdf_file)==1) return 2;
	}
	else if(Mode == 2)
		if(PolishCal()==1) return 2;
	return 0;
}


int ReadPolishConfig()
{
	char FileName[512];
	FILE * fp;
	char buf[1024];
	int	 ReadID=-1;
	int  ReadNum=0;
	
	sprintf(FileName,"%s/%s\0",dir_path,POLISHCONFIGFILE);
	fp=fopen(FileName,"r");
	while (fgets(buf, sizeof(buf), fp)) // fgets循取，直到文件最后，才返回NULL
	{
		if(buf[0]=='#') continue;
		if(ReadID==-1)
		{
			for(int i = 0 ; i<3; i++)
			{
				char* ptr = strstr(buf,Config_Str[i]);
				if(NULL == ptr) 
				{
					continue;
				}
				switch(i)
				{
					case 0:
						ReadValue(ptr+strlen(Config_Str[i]),&Mode,0);
					break;
					case 1:
						ReadID=1;
						ReadNum=3;
					break;
					case 2:
						ReadID=2;
						ReadNum=4;
					break;
				}
				break;
			}
			continue;
		}
		if(ReadNum!=0)
		{
			if(ReadID==1)
			{
				if(ReadNum==3)
				{
					ReadValue(buf,O_xyz,1);
				}
				else if(ReadNum==2)
					ReadValue(buf,P_x,1);
				else if(ReadNum==1)
					ReadValue(buf,P_xy,1);
				ReadNum--;
			}
			if(ReadID==2)
			{
				ReadValue(buf,I_uvw[4-ReadNum],2);
				ReadNum--;
			}
		}
		if(ReadNum==0)
			ReadID=-1;
	}
  fclose(fp);
  printf("Mode = %d\n",Mode);
  printf("O_xyz = %f %f %f\n",O_xyz[0],O_xyz[1],O_xyz[2]);
  printf("P_x = %f %f %f\n",P_x[0],P_x[1],P_x[2]);
  printf("P_xy = %f %f %f\n",P_xy[0],P_xy[1],P_xy[2]);
  printf("I_uvw[0] = %f %f %f %f\n",I_uvw[0][0],I_uvw[0][1],I_uvw[0][2],I_uvw[0][3]);
  printf("I_uvw[1] = %f %f %f %f\n",I_uvw[1][0],I_uvw[1][1],I_uvw[1][2],I_uvw[1][3]);
  printf("I_uvw[2] = %f %f %f %f\n",I_uvw[2][0],I_uvw[2][1],I_uvw[2][2],I_uvw[2][3]);
  printf("I_uvw[3] = %f %f %f %f\n",I_uvw[3][0],I_uvw[3][1],I_uvw[3][2],I_uvw[3][3]);
}

void	SetInfoShow(CtmWnd* pwndSender)
{
	printf("SetInfoShow \n");
	static int EditAngelTaborder=-1;
	
	if(EditAngelTaborder == -1)
		u_EditAngle->GetPropValueT("taborder",&EditAngelTaborder,sizeof(EditAngelTaborder));
	
	//u_StaticInfoMark->CreateA();
	u_StaticInfoMark->Update();
	if(Mode == 1)
	{
		printf("in 1\n");
		((CtmStaticX2*)u_StaticAngle)->Update2();
		u_EditAngle->SetPropValueT("taborder",EditAngelTaborder);
		u_EditAngle->Update();
	}
	else if(Mode==2)
	{
		u_EditAngle->SetPropValueT("taborder",-2);
		u_ImageWheel->Update();
	}
	
}

char* GetCDFFileName()
{
	int a=0;
	FILENAME* temp=cdf_head;
	while(a<sel_cdf_index)
	{
		if(temp==NULL) return NULL;
		printf("temp->name = %s\n",temp->name);
		a++;
		temp = temp->next;
	}
	if(temp==NULL) return NULL;
	return temp->name;
}


int CreateProg()
{
	char tmp[512];
	int ProgName=0,w=0,t=0,s=0;
	u_editFileName->GetPropValueT("value",&ProgName, sizeof(ProgName));
	u_editTool->GetPropValueT("value",&w, sizeof(w));
	u_editWork->GetPropValueT("value",&t, sizeof(t));
	u_editSpeed->GetPropValueT("value",&s, sizeof(s));
	if(ProgName<1) return 2;
	char ProgName1[32];
	sprintf(ProgName1,"Prog/O%04d\0",ProgName);
	//printf("s = %d\n",s);
	if(s<1) return 1;
	FILE *ProgTEXT;
	ProgTEXT = fopen(ProgName1,"w");
	if(w>0)
		fprintf(ProgTEXT,"WORKCOORD W%d;\n",w);
	if(t>0)
		fprintf(ProgTEXT,"TOOL T%d;\n",t);
	for(int i=0 ; i<((CtmMTextMemo*)u_memoPath)->GetStrNum()-1 ; i++)
	{
		memset(tmp,0,sizeof(tmp));
		((CtmMTextMemo*)u_memoPath)->GetStr(i, tmp,FALSE);
		printf("tmp = %s\n",tmp);
		fprintf(ProgTEXT,"LINE_ABS (%s) S%d F3;\n",tmp,s);
	}
	if(ProgType==0)
		fprintf(ProgTEXT,"END;\n",w);
	else if(ProgType==1)
		fprintf(ProgTEXT,"RETURN;\n",w);
	fclose(ProgTEXT);
	
	return 0;
}



int PolishCal()
{  	
	total_point = pathplanningfeasibility(&total_path);
	if(total_point<2) return 1;	//不合理
	
	//計算時，會用到的資料為
	//1.兩組路徑中 同位置的兩個點取中點(兩個X)
	//2.除了上述中間點，還需要下一個位置的座標來計畫Y方向(一個O)
	//3.需要下一個點的第二個路徑參考點
	//參考下圖，單引號''中的位置為計算時所需資料
	//          雙引號""則為本次計算姿態的位置
//	
//			 	O        	   O			->第N路徑
//		 "X" -Y方向-> 'X'       ->兩路徑中間位置
//	 'O'     		   O          ->第N+1路徑
//	
	//考慮到方便，讀取資料時將以路徑為單位，而非點。
	
	COORD_MEM Path_1[total_point];	//第一個路徑
	COORD_MEM Path_2[total_point];	//第二個路徑
	
	COORD_MEM TargetPoint;
	COORD_MEM next_TargetPoint;
  int x=0;
	
	
	//FILE * out_fp; 
	//char FileName[512];
	//sprintf(FileName,"%s/%s\0",dir_path,OUT_FILE);
	//printf("out FileName = %s\n",FileName);
	//out_fp=fopen(FileName,"w");
	//if(out_fp==NULL) return 0;
	
	//計算CAD原點 轉至 工件原點的齊次矩陣
	CalInvWorkCoordSystem();
	
	int path_num=0;
	for(path_num=0 ; path_num<total_path ; path_num++)
	{
		//因使用的路徑由[path1 , path2] => [path2 , path3] => [path2 , path3]
		//因此將Path_2直接copy到Path_1
		memcpy(Path_1,Path_2,sizeof(Path_1));
		//重新取得Path_2
		GetPathPoint(Path_2,path_num);
		//轉換CAD坐標系為工件原點
		coordinate_transformation(Path_2);
		if(path_num==0) continue;	//首次計算需要讀取兩條路徑
		
		//PS.計算第一條路徑時 path_num 為 1 非 0 詳見L88
		//因此，所有path_num%2==0的處理會與原本先寫法相反
		int point_num=0,point_no,next_point_no;
		//依序計算每個點的最終TCP座標
		for(point_num=0; point_num<total_point ; point_num++)
		{
			//取得計算此TCP時，所需的座標資料在第幾陣列(有反向問題)
			if(path_num%2==0)
			{
				point_no=total_point-1-point_num;
				next_point_no = point_no-1;
			}
			else
			{
				point_no=point_num;
				next_point_no = point_no+1;
			}
			
			//計算兩個路徑的中間路徑，第一次需計算兩個點(當前位置與下一個位置)，爾後將原第二點複製到第一點，僅重新計算第二點
			//最後一個點不需計算，後面有增加特殊處理(因為沒有下一個位置了)
			if(point_num==0)	//第一個點
			{
				GetTargetPoint(&TargetPoint,Path_1[point_no],Path_2[point_no]);
				GetTargetPoint(&next_TargetPoint,Path_1[next_point_no],Path_2[next_point_no]);
			}
			else if(point_num!=total_point-1)	//非最後一個點
			{
				memcpy(&TargetPoint,&next_TargetPoint,sizeof(TargetPoint));
				GetTargetPoint(&next_TargetPoint,Path_1[next_point_no],Path_2[next_point_no]);
			}
			//printf("Cal Path[%d] Point[%d] TCP \n",path_num,point_num);
			//printf("TargetPoint = %.3f %.3f %.3f , next_TargetPoint = %.3f %.3f %.3f \n",
			//				TargetPoint.coord[0],TargetPoint.coord[1],TargetPoint.coord[2],next_TargetPoint.coord[0],next_TargetPoint.coord[1],next_TargetPoint.coord[2])	;
			/*PS.關於最後一個點*/
			//沿用前一個點的L_point_y
			//但是B需要改用當前點的座標資料重新計算
			//軌跡也是要用當前軌跡，但是因為前一步(L110~L119)最後一個點並沒有計算，所以使用的是上一次計算的紀錄next_TargetPoint
			
			double L_point_x[3],L_point_y[3],L_point_z[3];
			double len_L_point_x,len_L_point_y,len_L_point_z;
			double B[3];
			double E[4][4],inv_E[4][4],out_E[4][4];
			
			L_point_y[0] = next_TargetPoint.coord[0]-TargetPoint.coord[0];
			L_point_y[1] = next_TargetPoint.coord[1]-TargetPoint.coord[1];
			L_point_y[2] = next_TargetPoint.coord[2]-TargetPoint.coord[2];
			
			//反向時增加負號
			if (path_num%2==0)
			{
				L_point_y[0]=-L_point_y[0];
				L_point_y[1]=-L_point_y[1];
				L_point_y[2]=-L_point_y[2];
			}
			
	  	//printf("TargetPoint=[%.6f,%.6f,%.6f]\n",TargetPoint.coord[0],TargetPoint.coord[1],TargetPoint.coord[2]);
	  	//printf("Path_2[%d]=[%.6f,%.6f,%.6f]\n",point_no,Path_2[point_no].coord[0],Path_2[point_no].coord[1],Path_2[point_no].coord[2]);
	  	//最後一個點的特殊處理，邏輯與原本做法相同
	  	if(point_num == total_point-1)
	  	{
				B[0]=Path_2[point_no].coord[0]-next_TargetPoint.coord[0]; 
				B[1]=Path_2[point_no].coord[1]-next_TargetPoint.coord[1]; 
				B[2]=Path_2[point_no].coord[2]-next_TargetPoint.coord[2]; 
			}
			else
	  	{
				B[0]=Path_2[point_no].coord[0]-TargetPoint.coord[0]; 
				B[1]=Path_2[point_no].coord[1]-TargetPoint.coord[1]; 
				B[2]=Path_2[point_no].coord[2]-TargetPoint.coord[2]; 
			}
			//printf("B  %.3f   %.3f   %.3f\n",B[0],B[1],B[2]);
			
			Cross_product(L_point_y,B,L_point_x);  //計算法向量 
	    Cross_product(L_point_x,L_point_y,L_point_z);
	      
	  	//printf("TargetPoint=[%.6f,%.6f,%.6f]\n",TargetPoint.coord[0],TargetPoint.coord[1],TargetPoint.coord[2]);
	  	//printf("TargetPoint[%d]=[%.6f,%.6f,%.6f]\n",j,TargetPoint[j][0],TargetPoint[j][1],TargetPoint[j][2]);
			len_L_point_x=sqrt(pow(L_point_x[0],2)+pow(L_point_x[1],2)+pow(L_point_x[2],2));  
			len_L_point_y=sqrt(pow(L_point_y[0],2)+pow(L_point_y[1],2)+pow(L_point_y[2],2));   
			len_L_point_z=sqrt(pow(L_point_z[0],2)+pow(L_point_z[1],2)+pow(L_point_z[2],2)); 
			//將單位向量轉為旋轉矩陣
	    E[0][0]=L_point_x[0]/len_L_point_x;
	    E[1][0]=L_point_x[1]/len_L_point_x;
	    E[2][0]=L_point_x[2]/len_L_point_x;
	    E[0][1]=L_point_y[0]/len_L_point_y;
	    E[1][1]=L_point_y[1]/len_L_point_y;
	    E[2][1]=L_point_y[2]/len_L_point_y;
	    E[0][2]=L_point_z[0]/len_L_point_z;
	    E[1][2]=L_point_z[1]/len_L_point_z;
	    E[2][2]=L_point_z[2]/len_L_point_z;
	    
	    //將旋轉矩陣加上座標 變為4x4的齊次矩陣
	    //最後一個點的特殊處理，邏輯與原本做法相同
	    if(point_num == total_point-1)
	    {
	    	E[0][3]=next_TargetPoint.coord[0];
	    	E[1][3]=next_TargetPoint.coord[1];
	    	E[2][3]=next_TargetPoint.coord[2];  
	    }
	    else
	    {
	    	E[0][3]=TargetPoint.coord[0];
	    	E[1][3]=TargetPoint.coord[1];
	    	E[2][3]=TargetPoint.coord[2];  
	    }
	    
	    E[3][0]=0;
	    E[3][1]=0;
	    E[3][2]=0;
	    E[3][3]=1;
      
      //E為工件座標原點坐標系 到 待磨點坐標系 的旋轉矩陣
      //==> TCP * E = 待磨點
      //當待磨點與砂輪坐標系重合時
      //==> 待磨點 = 砂輪磨點(I_uvw) = TCP * E
      //==> TCP = TCP * E * E-1 = 待磨點 * E-1 = 砂輪磨點 * E-1
			Inv_matrix_4(E,inv_E);  
			mt_DHmul(I_uvw,inv_E,out_E);
			
			//printf_M(E,"E");    
			//printf_M(inv_E,"inv_E");      
			//printf_M(I_uvw,"I_uvw");       
			//printf_M(out_E,"out_E");  
			 
			double posout[6];
			MToC6_SixAxis(out_E,posout);
			//printf("posout  %.3f   %.3f   %.3f   %.3f   %.3f   %.3f\n",posout[0],posout[1],posout[2],posout[3],posout[4],posout[5]);
			//fprintf(out_fp,"%f,%f,%f,%f,%f,%f,\n",posout[0],posout[1],posout[2],posout[3],posout[4],posout[5]);	
			char coord[512];
			memset(coord,'\0',sizeof(coord));
			sprintf(coord,"X=%.3f Y=%.3f Z=%.3f RX=%.3f RY=%.3f RZ=%.3f",posout[0],posout[1],posout[2],posout[3],posout[4],posout[5]);
			((CtmMTextMemo*)u_memoPath)->AddNewStr(x,coord,FALSE);
			x++;
			((CtmMTextMemo*)u_memoPath)->Update();
		}
	}
	
	return 0;
}

//計算中間點
void	GetTargetPoint(COORD_MEM* TargetPoint,COORD_MEM Path_1,COORD_MEM Path_2)
{		
	int i;
	for(i=0;i<3;i++)
		TargetPoint->coord[i]=(Path_1.coord[i]+Path_2.coord[i])/2;
}

//檢查檔案數量並且回傳每個檔案的路徑點數 回傳0代表異常
int pathplanningfeasibility(int* path_cnt)
{
	//
	  
	char FileName[512];
	int	 PathPointNum;;
	FILE * fp;
	
	//搜尋路徑數量
	int file_cnt=0;
	while(1)
	{	
		sprintf(FileName,"%s/%s%d%s\0",dir_path,POLISHPATHFILE_1,file_cnt+1,POLISHPATHFILE_2);
		printf("FileName = %s\n",FileName);
		fp=fopen(FileName,"r");
		if(fp==NULL)
		{
			printf("File Num is %d\n",file_cnt);
			break;
		}
		fclose(fp);
		file_cnt++;
	}
	if(file_cnt<2) return 0;	//小於2不合理
	
	//設定路徑數
	*path_cnt = file_cnt;
	
	//檢查路徑中檔案行數(點數)是否相同
	int pathcnt;   
	for(pathcnt=1;pathcnt<=file_cnt;pathcnt++)
  {
		sprintf(FileName,"%s/%s%d%s\0",dir_path,POLISHPATHFILE_1,pathcnt,POLISHPATHFILE_2);
		fp=fopen(FileName,"r");
		char buf[1024];
  	int lineCnt_tmp = 0;
    	
		while (fgets(buf, sizeof(buf), fp)) // fgets循取，直到文件最后，才返回NULL
    	lineCnt_tmp++; // 累計行數
  	fclose(fp);

		printf("lineCnt_tmp = %d\n",lineCnt_tmp);
  	if(pathcnt==1)
  	{
    	PathPointNum=lineCnt_tmp;
  	}
  	else 
  	{
    	if(PathPointNum!=lineCnt_tmp) 
    	{
				printf("PathPointNum = %d\n",0);
    		return 0;
    	}
  	}	
	}
	//回傳行數
	printf("PathPointNum = %d\n",PathPointNum);
	return PathPointNum;
}

//讀取指定路徑的座標
void	GetPathPoint(COORD_MEM Path[],int path_num)
{	
	FILE *fp_contourpath;    
	char FileName[128];

	sprintf(FileName,"%s/%s%d%s\0",dir_path,POLISHPATHFILE_1,path_num+1,POLISHPATHFILE_2);
	printf("GetPathPoint FileName = %s\n",FileName);
	fp_contourpath=fopen(FileName,"r");
	int i;
	float pos[3];
	for(i=0;i<total_point;i++)
	{
		fscanf(fp_contourpath,"%f,%f,%f,",&(pos[0]),&(pos[1]),&(pos[2]));
		//fscanf(fp_contourpath,"%f,%f,%f,",&(Path[i].coord[0],&(Path[i].coord[1]),&(Path[i].coord[2]));
		Path[i].coord[0]=pos[0];
		Path[i].coord[1]=pos[1];
		Path[i].coord[2]=pos[2];
		//printf("Path[%d]=[%.3f,%.3f,%.3f]\n",i,Path[i].coord[0],Path[i].coord[1],Path[i].coord[2]) ;           
	} 
	fclose(fp_contourpath); 
}

//計算CAD原點 轉至 工件原點的齊次矩陣
void	CalInvWorkCoordSystem()	
{
	double L_tmp[3];
	double L_point_x[3]; 
	double L_point_y[3]; 
	double L_point_z[3];
	double len_L_point_x,len_L_point_y,len_L_point_z;
	double E[4][4];
	//----------------------------------------------------------------------
	int i;
	for(i=0;i<3;i++)
	{
		L_point_x[i]	=P_x[i] 	- O_xyz[i];  
	  L_tmp[i]			=P_xy[i] - O_xyz[i];  
	}  
	Cross_product(L_point_x,L_tmp,L_point_z);  //計算法向量 
	Cross_product(L_point_z,L_point_x,L_point_y);
	
	//-----------------------------------------------計算向量長度		
	len_L_point_x=sqrt(pow(L_point_x[0],2)+pow(L_point_x[1],2)+pow(L_point_x[2],2));  
	len_L_point_y=sqrt(pow(L_point_y[0],2)+pow(L_point_y[1],2)+pow(L_point_y[2],2));   
	len_L_point_z=sqrt(pow(L_point_z[0],2)+pow(L_point_z[1],2)+pow(L_point_z[2],2));
	            	 	
	//求CAD原點 至 工件原點的4x4齊次矩陣
	E[0][0]=L_point_x[0]/len_L_point_x;
	E[1][0]=L_point_x[1]/len_L_point_x;
	E[2][0]=L_point_x[2]/len_L_point_x;
	E[0][1]=L_point_y[0]/len_L_point_y;
	E[1][1]=L_point_y[1]/len_L_point_y;
	E[2][1]=L_point_y[2]/len_L_point_y;
	E[0][2]=L_point_z[0]/len_L_point_z;
	E[1][2]=L_point_z[1]/len_L_point_z;
	E[2][2]=L_point_z[2]/len_L_point_z;
	    	
	E[0][3]=O_xyz[0];
	E[1][3]=O_xyz[1];
	E[2][3]=O_xyz[2];  
	    	
	E[3][0]=0;
	E[3][1]=0;
	E[3][2]=0;
	E[3][3]=1;
	
	Inv_matrix_4(E,inv_Work_E);
	
}
//將該路徑的所有座標由CAD坐標系轉為工件原點坐標系
void coordinate_transformation(COORD_MEM Path[])
{
	double pos[3];
	//printf("coordinate_transformation\n");
	
	//原本的待磨點是CAD坐標系的座標
	//     CAD坐標系  * CAD座標 = 待磨點
	//     工件座標系 * 轉換後待磨點座標(pos) = 待磨點
	
	//     工件座標系 = CAD坐標系 * E
	// ==> CAD坐標系 * E * 轉換後待磨點座標(pos) = 待磨點
	
	//     CAD坐標系 * E * 轉換後待磨點座標(pos) = CAD坐標系  * CAD座標
	// ==> E * 轉換後待磨點座標(pos) = CAD座標
	// ==> E-1 * E *轉換後待磨點座標(pos) = E-1 * CAD座標
	// ==> 轉換後待磨點座標(pos) = E-1 * CAD座標
	
	//計算每個點相對於工件原點的座標
	//標準情況是將CAD座標轉為矩陣在去相乘，但我需要的資料只有座標，因此簡化僅將座標有關的部分取出計算
	//可參考mt_DHmul 在j=3情況即是計算座標的位置
	int  point_cnt;
	for(point_cnt=0 ; point_cnt<total_point ; point_cnt++)
	{
		pos[0]=inv_Work_E[0][0]*Path[point_cnt].coord[0]+inv_Work_E[0][1]*Path[point_cnt].coord[1]+inv_Work_E[0][2]*Path[point_cnt].coord[2]+inv_Work_E[0][3];
		pos[1]=inv_Work_E[1][0]*Path[point_cnt].coord[0]+inv_Work_E[1][1]*Path[point_cnt].coord[1]+inv_Work_E[1][2]*Path[point_cnt].coord[2]+inv_Work_E[1][3];
		pos[2]=inv_Work_E[2][0]*Path[point_cnt].coord[0]+inv_Work_E[2][1]*Path[point_cnt].coord[1]+inv_Work_E[2][2]*Path[point_cnt].coord[2]+inv_Work_E[2][3];
		
		Path[point_cnt].coord[0]=pos[0];
		Path[point_cnt].coord[1]=pos[1];
		Path[point_cnt].coord[2]=pos[2];
		//printf("Point[%d] = %.3f %.3f %.3f\n",point_cnt,Path[point_cnt].coord[0],Path[point_cnt].coord[1],Path[point_cnt].coord[2]);
	}
	
 
}

//====================================================================================================================

//============================================================================================================其餘函數 

//3. 外積 
void Cross_product( double *a,double *b,double *c)
{
    c[0]= a[1]*b[2]-a[2]*b[1];
    c[1]= a[2]*b[0]-a[0]*b[2];
    c[2]= a[0]*b[1]-a[1]*b[0];
}

//齊次矩陣相乘
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

//倒置矩陣
//正交矩陣是一個方塊矩陣，其元素為實數，而且行與列皆為正交的單位向量，使得該矩陣的轉置矩陣為其逆矩陣
void Inv_matrix_4( double Mtrix_A[4][4], double Inv_Mtrix_A[4][4])
{
	Inv_Mtrix_A[0][0]=Mtrix_A[0][0];
	Inv_Mtrix_A[0][1]=Mtrix_A[1][0];
	Inv_Mtrix_A[0][2]=Mtrix_A[2][0];

	Inv_Mtrix_A[1][0]=Mtrix_A[0][1];
	Inv_Mtrix_A[1][1]=Mtrix_A[1][1];
	Inv_Mtrix_A[1][2]=Mtrix_A[2][1];

	Inv_Mtrix_A[2][0]=Mtrix_A[0][2];
	Inv_Mtrix_A[2][1]=Mtrix_A[1][2];
	Inv_Mtrix_A[2][2]=Mtrix_A[2][2];

	Inv_Mtrix_A[0][3]=-(Inv_Mtrix_A[0][0]*Mtrix_A[0][3] + Inv_Mtrix_A[0][1]*Mtrix_A[1][3] + Inv_Mtrix_A[0][2]*Mtrix_A[2][3] );//-RTT
	Inv_Mtrix_A[1][3]=-(Inv_Mtrix_A[1][0]*Mtrix_A[0][3] + Inv_Mtrix_A[1][1]*Mtrix_A[1][3] + Inv_Mtrix_A[1][2]*Mtrix_A[2][3] );//-RTT
	Inv_Mtrix_A[2][3]=-(Inv_Mtrix_A[2][0]*Mtrix_A[0][3] + Inv_Mtrix_A[2][1]*Mtrix_A[1][3] + Inv_Mtrix_A[2][2]*Mtrix_A[2][3] );//-RTT

	Inv_Mtrix_A[3][0]=0;
	Inv_Mtrix_A[3][1]=0;
	Inv_Mtrix_A[3][2]=0;
	Inv_Mtrix_A[3][3]=1;
}

void	printf_M(double a[4][4],char *s)
{
	printf("%s:\n",s);
	printf("[%3.7f  %3.7f  %3.7f  %3.8f]\n",a[0][0],a[0][1],a[0][2],a[0][3]);
	printf("[%3.7f  %3.7f  %3.7f  %3.8f]\n",a[1][0],a[1][1],a[1][2],a[1][3]);
	printf("[%3.7f  %3.7f  %3.7f  %3.8f]\n",a[2][0],a[2][1],a[2][2],a[2][3]);
	printf("[%3.7f  %3.7f  %3.7f  %3.8f]\n",a[3][0],a[3][1],a[3][2],a[3][3]);
}

void  MToC6_SixAxis(double E[4][4], double Pos[6])
{
	Pos[0]=E[0][3];
	Pos[1]=E[1][3];
	Pos[2]=E[2][3];
	Pos[4]=atan2(E[0][2],sqrt(pow(E[1][2],2)+pow(E[2][2],2)));
	if(fabs(fabs(Pos[4])-M_PI/2)<0.001) //Cos(y)!=0 無法求出其他兩角度
	{
		Pos[3]=999;
		Pos[4]= Pos[4]/M_PI*180.0;
		Pos[5]=999;
	}
	else  
	{
		Pos[3]=atan2(-E[1][2]/cos(Pos[4]),E[2][2]/cos(Pos[4]));
		Pos[5]=atan2(-E[0][1]/cos(Pos[4]),E[0][0]/cos(Pos[4]));
		
		Pos[3]= Pos[3]/M_PI*180.0;
		Pos[4]= Pos[4]/M_PI*180.0;
		Pos[5]= Pos[5]/M_PI*180.0;
	}
}

void	ReadValue(char* str, void* out , int type)
{
	switch(type)
	{
		case D_INT:
			*((int*)out) = atoi(str);
		break;
		case D_MATRIX3:
			while(*str=='{'||*str==','||*str==' '||*str=='}') str++;
			((double*)out)[0] = atof(str);
			while(*str!=',' && *str!='}') str++;
			while(*str=='{'||*str==','||*str==' '||*str=='}') str++;
			((double*)out)[1] = atof(str);
			while(*str!=',' && *str!='}') str++;
			while(*str=='{'||*str==','||*str==' '||*str=='}') str++;
			((double*)out)[2] = atof(str);
		break;
		case D_MATRIX4:
			while(*str=='{'||*str==','||*str==' '||*str=='}') str++;
			((double*)out)[0] = atof(str);
			while(*str!=',' && *str!='}') str++;
			while(*str=='{'||*str==','||*str==' '||*str=='}') str++;
			((double*)out)[1] = atof(str);
			while(*str!=',' && *str!='}') str++;
			while(*str=='{'||*str==','||*str==' '||*str=='}') str++;
			((double*)out)[2] = atof(str);
			while(*str!=',' && *str!='}') str++;
			while(*str=='{'||*str==','||*str==' '||*str=='}') str++;
			((double*)out)[3] = atof(str);
		break;
	}
}



int pathpointproduct(double distance_ratio,double theta,char* file)
{
  FILE *fp_contourpath;
  float point[3];
  float point1[3];
  float vector[3];
  float vector_1[3];

  FILE *fp_planningpath;
  float normal_vector[3];
  float Path_point[3];
  float rx;
  float ry;
  float rz;
  double len_normal_vector;       
  double A[3]={0};                                  
  double B[3]={0,0,1};   
  double C[3]={0};
  int i,j;               //迴圈變數用 
  //double theta=-pi/4;
  
   
  //------------------------------------------------------------------------抓取檔案行數(輪廓點數) 
  //printf("file = %s\n",file);
  fp_contourpath = fopen(file,"r");
  fp_planningpath = fopen("./test4145.csv","w");          
  int num = 0;
  int SumPoint=0;
  int x=0;
 
	((CtmMTextMemo*)u_memoPath)->LoadFromFile("Data/dummy", 0,TRUE);
	((CtmMTextMemo*)u_memoPath)->Update();
  while (1) // fgets循取，直到文件最后，才返回NULL
 	{
 		memcpy(vector_1,vector,sizeof(vector_1));
 		memcpy(point,point1,sizeof(point));
 		num=fscanf(fp_contourpath,"%f,%f,%f,",&point1[0],&point1[1],&point1[2]);
 		//printf("num = %d %f %f %f\n",num,point1[0],point1[1],point1[2]);
		if(num<3) 
		{
			if(SumPoint<2 )
			{
  			fclose(fp_contourpath);
  			fclose(fp_planningpath);
				return 1;
			}
		}
		else	
		{
			SumPoint++;
			if(SumPoint<2) continue;
			for(j=0;j<2;j++)
				A[j]=point1[j]-point[j];
			Cross_product(B,A,C);
			normal_vector[0]=C[0];
			normal_vector[1]=C[1];
			normal_vector[2]=sqrt(pow(normal_vector[0],2)+pow(normal_vector[1],2))*tan(theta);
			len_normal_vector=sqrt(pow(normal_vector[0],2)+pow(normal_vector[1],2)+pow(normal_vector[2],2));
			for(j=0;j<3;j++)
				vector[j]=normal_vector[j]/len_normal_vector;
			
		}
		for(j=0;j<3;j++)
			Path_point[j]=point[j]+vector[j]*distance_ratio;
		ry=(atan2(vector[0],sqrt( pow(vector[1],2)+pow(vector[2],2)) ))/M_PI*180;   
		rx= atan2(-vector[1]/cos(ry/180*M_PI) , vector[2]/cos(ry/180*M_PI))/M_PI*180 ;     
		rz=0.0;      
		//printf("Path = [%.4f,%.4f,%.4f,%.4f,%.4f,%.4f]\n",Path_point[0],Path_point[1],Path_point[2],rx,ry,rz) ;
		fprintf(fp_planningpath,"%f,%f,%f,%f,%f,%f,\n",Path_point[0],Path_point[1],Path_point[2],rx,ry,rz);
		char coord[512];
		memset(coord,'\0',sizeof(coord));
		sprintf(coord,"X=%.3f Y=%.3f Z=%.3f RX=%.3f RY=%.3f RZ=%.3f",Path_point[0],Path_point[1],Path_point[2],rx,ry,rz);
		((CtmMTextMemo*)u_memoPath)->AddNewStr(x,coord,FALSE);
		((CtmMTextMemo*)u_memoPath)->Update();
		x++;
		if(num<3) break;
	}
  fclose(fp_contourpath);
  fclose(fp_planningpath);
  //printf("file = %s\n",file);
  return 0;
}