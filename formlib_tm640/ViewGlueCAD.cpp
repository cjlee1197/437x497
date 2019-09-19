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
#include	"ViewGlueCAD.h"
#include	"../tmdebug.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"
#include	"../tmshmsg.h"
#include 	"../taskcnc.h"
#include 	"../gui.h"
#include 	"../tmFileExplorer.h"

/*============================================================================+
|           Constant                                                     			|
+============================================================================*/
#define CAD_FILE_PATH "./Data/GlueCADFile/"
#define USE_MATH_DEFINES

/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/

CtmWnd*		u_selecteditObject		= NULL;
CtmWnd*		u_editAngel		= NULL;
CtmWnd*		u_editDistance		= NULL;
CtmWnd*		u_BtnPathCreate		= NULL;
CtmWnd*		u_memoPath		= NULL;
CtmWnd*		u_editTool		= NULL;
CtmWnd*		u_editWork		= NULL;
CtmWnd*		u_editSpeed		= NULL;
CtmWnd*		u_editFileName		= NULL;
CtmWnd*		u_btnCreateProg		= NULL;
CtmWnd*		u_ManageVersion		= NULL;
                     

FILENAME* cdf_head=NULL; 
FILENAME* cdf_end=NULL;  
int cdf_Num=0;            
int sel_cdf_index=-1;   
BOOL path_ok=FALSE;          
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_selecteditObject 	= pwndSender->FindControlFromName("SelectTargetObject");
	u_editAngel					= pwndSender->FindControlFromName("EditSprinklerAngle");
	u_editDistance 			= pwndSender->FindControlFromName("EditSprinklerDistance");
	u_BtnPathCreate 		= pwndSender->FindControlFromName("BtnPathPointCreate");
	
	u_memoPath 					= pwndSender->FindControlFromName("PathMemo");
	
	u_editTool 					= pwndSender->FindControlFromName("EditCoordinateWork");
	u_editWork 					= pwndSender->FindControlFromName("EditCoordinateTool");
	u_editSpeed 				= pwndSender->FindControlFromName("EditPathSpeed");
	
	u_editFileName 			= pwndSender->FindControlFromName("EditFileName");
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
		sel_cdf_index = ((CtmSelectEdit*)u_selecteditObject)->GetIndex();
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
		FindCADFile();
		StartRunOne=FALSE;
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
		char* tmp =strstr(file[n]->d_name,".cdf");
		if(!full && file[n]->d_type == 8 && tmp!=NULL && (*(tmp+4))=='\0')/* && file[n]->d_type*/
		{
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
	int angel,dis;
	u_editAngel->GetPropValueT("value",&angel,sizeof(angel));
	u_editDistance->GetPropValueT("value",&dis,sizeof(dis));
	char* cdf_file;
	cdf_file=GetCDFFileName();
	if(cdf_file==NULL) return 1	;
	if(pathpointproduct(dis/1000,angel/1000,cdf_file)==1) return 2;
	return 0;
	
	/*if(cdf_file!=NULL)
		free(cdf_file);*/
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

int pathpointproduct(double distance_ratio,double theta,char* file)
{
	printf("distance_ratio = %f theta=%f\n",distance_ratio,theta);
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


void Cross_product( double a[],double b[],double c[])
{
    c[0]= a[1]*b[2]-a[2]*b[1];
    c[1]= a[2]*b[0]-a[0]*b[2];
    c[2]= a[0]*b[1]-a[1]*b[0];
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
	fprintf(ProgTEXT,"/*GLUE CAD SUB PROG*/\n\n");
	if(w>0)
		fprintf(ProgTEXT,"WORKCOORD W%d;\n",w);
	if(t>0)
		fprintf(ProgTEXT,"TOOL T%d;\n",t);
	for(int i=0 ; i<((CtmMTextMemo*)u_memoPath)->GetStrNum()-1 ; i++)
	{
		memset(tmp,0,sizeof(tmp));
		((CtmMTextMemo*)u_memoPath)->GetStr(i, tmp,FALSE);
		fprintf(ProgTEXT,"LINE_ABS (%s POS=0) S%d F5;\n",tmp,s);
	}
	fprintf(ProgTEXT,"RETURN\n",w);
	fclose(ProgTEXT);
	
	return 0;
}