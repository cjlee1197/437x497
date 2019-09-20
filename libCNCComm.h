/*==========================================================================+
|  Class    : CNC comm struct                                               |
|  Task     : CNC comm struct define                              			|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James                                                         |
|  Version  : V1.00                                                         |
|  Creation : 2009/12/23                                                    |
|  Revision : V1.00                                                         |
+==========================================================================*/
#ifndef	D_LIBCNCCOMM
#define	D_LIBCNCCOMM

#pragma pack(push)
#pragma pack(1) 
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/ 
#define     MAX_SERVO_NO   	135//138
#define     MAX_TUNE_NO    	70//117
#define     MAX_MAKER_NO    51
#define     MAX_USER_NO     14
#define     MAX_COMMON_NO   11

#define			D_COORD_NUM 6
#define			D_JOINT_NUM 6

#ifdef TCP_HMI_NC
#elif defined	SHM_HMI_NC
#else
#define	SHM_HMI_NC 
#endif

enum Robot_Type {SIXROBOT=1,SCARAROBOT,PALLETROBOT,IMMROBOT};


#define		PITCH_NO_MAX	200
//#define		NUM_OF_OFFSET	100   //2011-09-08
#define		INT_LV_NO		34
#define		INT_GV_NO		101
#define     MAX_DATA        512//30000/*20000*/
//for INT_TABLE1
//data lenghth constants
#define INT_FN_LEN        	12//9    			//filename length
#define INT_NC_WORD_NO   	28     			//no. of NC words than can be used
#define INT_GV_NO       	101     		//no. of global variables
#define INT_LV_NO        	34     			//no. of local variables each layer
#define INT_MACRO_SYS_PARA_NO	400   			//no. of SystemMacroPara.,add for $370~$399	2012/7/23

#define INT_AXES_NO       	4     			//no. of axes
#define INT_TR_AXES_NO    	3     			//no. of axes for transformation
#define INT_GROUP_NO     	20//19     			//no. of total G-code groups
#define INT_MAX_DEPTH     	8     			//maximum program depth
#define INT_MACRO_DEPTH   	4     			//maximum macro depth
                                			
#define INT_MAX_COORD     	24    			//maximum coordinate system 
#define INT_MAX_MLC_DATA 	16     			//maximum macro mlc exchange data

#define NERR_BUSY			-1
#define NERR_EOF			-2
#define NERR_FILENOTFOUND	-3
#define NERR_BLOCKNOTFOUND	-4

//#define NUM_OF_OFFSET      100    //2011-09-08
//#define NUM_OF_TOOL             60   //2011-09-08
#define 	NUM_OF_COOR         25
#define 	NUM_OF_TP						24
#define 	NUM_OF_SET_COORD		100	//NUM_OF_SET_JOINT也一樣是100


//RunStatus
#define   RUN_STATUS_STOP     0
#define   RUN_STATUS_RUN      1
#define   RUN_STATUS_PAUSE    2

//Coordinate Database ID
#define		MACHINE_COORD_ID		"CNC_NC_MACHINE_X"
#define		ABSOLUTE_COORD_ID		"CNC_NC_ABSOLUTE_X"
#define		ANGULAR_COORD_ID		"CNC_NC_ANGULAR_POS_1"
#define		DIS_TO_GO_COORD_ID	"CNC_DIS_TO_GO_X"
#define		MACHINE_ID					"CNC_MACHPOS_X"
#define		NO_ACE_MACHINE_ID		"CNC_NOACEMACHPOS_X"
#define		ISR_MACHINE_ID			"CNC_ISRMACHPOS_X"
/*==========================================================================+
|           TrackCurve                                                      |
+==========================================================================*/
//wCoordinateMode
#define	    TRACK_CURVE_XY			1
#define	    TRACK_CURVE_XZ			2
#define	    TRACK_CURVE_YZ			3

#define	    TRACK_CURVE_NORMAL		0x10
#define	    TRACK_CURVE_ZOOMIN		0x20
#define	    TRACK_CURVE_ZOOMOUT		0x40

///*----------------------------------------------------------------
// *  ON/OFF STATE DEFINITION             
// *---------------------------------------------------------------*/
// MLC I/O state                               
#define OP_IO_ON      (char)0xff            
#define OP_IO_OFF     (char)0x00                  

//OPERATION FLAG state                               
#define OP_ON               1            
#define OP_OFF              0            

//MOTION FLAG state                               
#define MOT_ON              1            
#define MOT_OFF             0            
                              
//INTERPRETER FLAG state                              
#define INT_ON              1            
#define INT_OFF             0            
                              
//MMI FLAG state                              
#define MMI_ON              1            
#define MMI_OFF             0            
/*----------------------------------------------------------------
 *  CNC MESSAGE                                   
 *---------------------------------------------------------------*/                
#define OP_UNSELECT         0                  
#define OP_EDITING          1                  
/*----------------------------------------------------------------
 *  MOTION CONTROL COMMAND DEFINITION             
 *---------------------------------------------------------------*/
#define MOT_NO_CMD          0                 
#define MOT_START           1                 
#define MOT_BUFF_START      2                 
#define MOT_SINGLE          3                 
#define MOT_BUFF_SINGLE     4                 
#define MOT_FEEDHOLD        5                 
#define MOT_STOP            6              
#define MOT_RESET           7              
#define MOT_CLEAR           8              
#define MOT_ISR_FAIL_COUNT  100
#define ISR_FROM_NON        0
#define ISR_FROM_AUTO       1
#define ISR_FROM_JOG        2
#define ISR_FROM_COMMON     3
/*----------------------------------------------------------------
 *  M CODE FUNCTION CONSTANT                 
 *---------------------------------------------------------------*/          
#define M00                 0
#define M01                 1
#define M02                 2
#define M30                 30
#define M31                 31
#define M03                 3
#define M04                 4
#define M05                 5
/*----------------------------------------------------------------
 *  JOG COMMAND TYPE                                 
 *---------------------------------------------------------------*/    
                           
#define JOG_X1              1                  
#define JOG_X10             10                 
#define JOG_X100            100            
#define JOG_X1000           1000           
#define LOW                 1                  
#define MED                 2                  
#define HIGH                3                  
#define RAPID               4                  
#define JOG_POSITIVE        1                  
#define JOG_NEGATIVE       	-1                 
#define JOG_NONE            0
#define POSI_EDGE_PDIR      1                  
#define NEGA_EDGE_PDIR     	-1                 
#define POSI_EDGE_NDIR     	-1                  
#define NEGA_EDGE_NDIR      1                 
#define LONGDISP            99999999          
#define LONGDISN           	-99999999          
#define NODIS               0                 
/*----------------------------------------------------------------
 *  SETTING COMMAND TYPE                                 
 *---------------------------------------------------------------*/
#define LOW_SPEED           10000              
#define MED_SPEED           100000             
#define HIGH_SPEED          1000000            
#define RAPID_SPEED         2000000            
#define SET_ON              1                  
#define SET_OFF             0                  
/*----------------------------------------------------------------
 *  MDI BUFFER ON/OFF DEFINITION                     
 *---------------------------------------------------------------*/
#define MDI_BUFFER_FULL		1
#define MDI_BUFFER_EMPTY    0
/*----------------------------------------------------------------
 *  SPINDLE DEFINITION                     
 *---------------------------------------------------------------*/
#define SPINDLE_CW          1
#define SPINDLE_CCW         2
#define SPINDLE_STOP        3
#define SPINDLE_SPEED      	1000

#define INT_FILE_LEN       12		//9
#define INT_BUFFER_SIZE     180 //use g code    //maximum block length
#define COMMAND_STR_SIZE     256 //use robot languth   //maximum block length
//#define NUM_OF_OFFSET       100   //2011-09-08
#define NUM_OF_BLOCK        30

//CNC_status           
#define NOT_READY        0                  
#define M_READY          1                  
#define C_START          2                  
#define F_HOLD           3                  
#define B_STOP           4  

//Ethernet
#define ETHERNET_MAX_LEN		950
#define ETHERNET_PROG_NAME	100
#define ETHERNET_PROG_DATE 850
#define DM5_LENGTH 32


//Ethernet type
#define HMI2NC_UPDATE  					1
#define HMI2NC_GETDATE 					2
#define HMI2NC_FILE		 					3
#define HMI2NC_CONFIRM_FILE			4
#define	HMI2NC_TEACHPOINT				5
#define	HMI2NC_RE_PEOG_END			6
#define HMI2NC_INI_OK	 					9

#define NC2HMI_UPDATE  					11 
#define NC2HMI_GETDATE 					12
#define NC2HMI_FILE_DEMAND 			13
#define NC2HMI_RESET 						14
#define NC2HMI_SERVO_STATUS 		15
#define NC2HMI_IO_SET_OK 				16

#define INI_DATACOMM   101
#define INI_POSCOMM    102
/*----------------------------------------------------------------
 *  for INT1              
 *---------------------------------------------------------------*/
typedef enum { P_NORMAL, P_M_MACRO, P_S_MACRO, P_CANNED_CYCLE } PROG_TYPE;
typedef struct TYPE_SHORT_BITFIELD
{
    unsigned short bit_f : 1;
    unsigned short bit_e : 1;
    unsigned short bit_d : 1;
    unsigned short bit_c : 1;
    unsigned short bit_b : 1;
    unsigned short bit_a : 1;
    unsigned short bit_9 : 1;
    unsigned short bit_8 : 1;
    unsigned short bit_7 : 1;
    unsigned short bit_6 : 1;
    unsigned short bit_5 : 1;
    unsigned short bit_4 : 1;
    unsigned short bit_3 : 1;
    unsigned short bit_2 : 1;
    unsigned short bit_1 : 1;
    unsigned short bit_0 : 1;
} CONTROL_WORD;
/*----------------------------------------------------------------
 *  TRANS, coordinate       
 *---------------------------------------------------------------*/
typedef struct TYPE_COORD2
{
    double x;
    double y;
} INT_COORD2;
typedef struct TYPE_COORD3
{
    double x;
    double y;
    double z;
} INT_COORD3;

typedef struct TYPE_COORD6
{
    double x;
    double y;
    double z;
    double u;
    double v;
    double w;
    
} INT_COORD6;

typedef struct TYPE_COORD4
{
    double x;
    double y;
    double z;
    double c;   //4-th axis
} INT_COORD4;

typedef struct TYPE_COORD5
{
    double x;
    double y;
    double z;
    double c;   //4-th axis
    double a;   //5-th axis
} INT_COORD5;

typedef double TransMatrix[INT_TR_AXES_NO][INT_TR_AXES_NO];
typedef enum { TRANS_OFF, TRANS_ON } TRANS_FLAG;
typedef enum { NOT_UPDATED, UPDATED } UPDATED_FLAG;

/*----------------------------------------------------------------
 *  PFMANAGE, local variable memory 
 *---------------------------------------------------------------*/
typedef struct TYPE_LOCAL_VAR LOCAL_VAR;
typedef	struct TYPE_LOCAL_VAR
{
    double lv[INT_LV_NO];   			//local variable storage area
    LOCAL_VAR *next;        			//for building a linked list
}LOCAL_VAR;

/*----------------------------------------------------------------
 *  program status structure, used by PSTATUS, MSTATUS and CCSTATUS
 *---------------------------------------------------------------*/
typedef struct TYPE_PSTATUS
{
    char fn[INT_FN_LEN];    			//part program filename
    int ln;                 			//remnained loop no
    long bn;                			//next block no
    PROG_TYPE pt;           			//program pype
    LOCAL_VAR *lvp;         			//local variable pointer
} PSTATUS;
typedef PSTATUS MSTATUS;
typedef PSTATUS CCSTATUS;   			//PSTATUS, MSTATUS and CCSTATUS are the same

/*----------------------------------------------------------------
 *  table1 modal information
 *---------------------------------------------------------------*/
typedef struct TYPE_MODAL
{
    INT_COORD6 work_pos;                //work coordinate
    INT_COORD6 joint_work_pos;         //angular work coordinate
    //INT_COORD4 UV_work_pos;             //for UV
    CONTROL_WORD control_word;          //for compensation, chamfering ....
    unsigned char prog_end;             //program end flag
    unsigned char error;                //no error if error==0
    //unsigned char g41_g42;              //exchange G41 and G42 if nonzero
    unsigned char reserve;				//reserve
    unsigned char group[INT_GROUP_NO];  //modal group g code value
    int b;                              //velocity percentage
    int c;                              //1:Cartesian command for G00, 0:angular command for G00
    int f;                              //feedrate percentage
    int q;                              // posture
    long n;                             //sequence no.
    int t;                              //tool no.
    int s;                              //final point posture
    int p;                              //middle point posture
    char fn[INT_FN_LEN];                //current filename
    long bn;                            //current block no. in the file above
    long read_bn;                       //current read block  for canned cycle or macro program
} INT_MODAL;

/*----------------------------------------------------------------
 *  table1 single information
 *---------------------------------------------------------------*/
typedef struct TYPE_SINGLE
{
    INT_COORD6 work_shift_vector;           //work coordinate shift amount
    INT_COORD6 tool_shift_vector;           //tool coordinate shift amount
    unsigned char useful_flag;              //null or skipped block
    //unsigned char xu_flag;                  //X, U which is first
    //unsigned char yv_flag;                  //Y, V which is first
    //unsigned char zw_flag;                  //Z, W which is first
    unsigned char inch_mm;                  //last block is inch or mm
    unsigned char reserve1;					//reserve
    unsigned char group_flag[INT_GROUP_NO]; //if the G code appears
    unsigned char code_flag[INT_NC_WORD_NO];//if the code appears
    float value[INT_NC_WORD_NO];            //used by MMI, raw material
    double code[INT_NC_WORD_NO];            //used by INT, converted data
} INT_SINGLE;

/*----------------------------------------------------------------
 *  table1 program, subroutine, single macro, modal macro
 *  and canned cycle file information
 *  mainly for re-interpretation
 *---------------------------------------------------------------*/
typedef struct TYPE_PROGRAM
{
    //char cc_set;                    	//canned cycle set
    //char inside_cc;                 	//inside the canned cycle
    int/*char*/ macro_level;               	//modal macro level	//Sunny<20100413>modify
    int/*char*/ prog_level;                	//program level  	//Sunny<20100413>modify
    //CCSTATUS ccs;                   	//canned cycle status
    MSTATUS ms[INT_MACRO_DEPTH];    	//modal macro status stack
    PSTATUS ps[INT_MAX_DEPTH+1];    	//program status stack
} INT_PROGRAM;

/*----------------------------------------------------------------
 *  table1 coordinate and transformation data
 *---------------------------------------------------------------*/
typedef struct TYPE_TRANS
{
    //TRANS_FLAG polar_flag;           	//polar command OFF/ON
    //TRANS_FLAG scaling_flag;         	//scaling mode OFF/ON
    //TRANS_FLAG rotation_flag;        	//rotation mode OFF/ON
    //TRANS_FLAG shift_flag;           	//local shift OFF/ON
    //UPDATED_FLAG x_updated_flag;
    //UPDATED_FLAG y_updated_flag;     	//for coordinate maintanece under scaling
    //UPDATED_FLAG z_updated_flag;
    int work_plane;                  	//current work plane selection
    int cw_coord;                    	//current work coordinate selection
    int ct_coord;                    //current work coordinate selection
    //double polar_radius;             	//current
    //double polar_argument;           	//current, unit=degree
    //double rotation_angle;           	//current, unit=degree
    //INT_COORD4 SV;                   	//total transformation shift vector
    //INT_COORD4 SVs;                  	//scaling shift vector
    //INT_COORD4 SVr;                  	//rotation shift vector
    //INT_COORD4 SVl;                  	//local shift vector
    INT_COORD6 movement;             	//tool moveing amount of one block
    INT_COORD6 work_pos;             	//current work coordinate value
    INT_COORD6 prog_pos;             	//current program coordinate value
    INT_COORD6 work_zofs[NUM_OF_COOR];//current zero point offset values for work coord.
    INT_COORD6 tool_zofs[NUM_OF_COOR];//current zero point offset values for tool coord.
    //INT_COORD4 ZOFS[INT_MAX_COORD+1];	//current zero point offset values
    //TransMatrix Ms;                  	//scaling matrix
    //TransMatrix Mss;                 	//complementary scaling matrix
    //TransMatrix Mr;                  	//rotation matrix
    //TransMatrix Mrs;                 	//complementary rotation matrix
    //TransMatrix Msr;                 	//total transformation matrix,Mr*Ms
    //TransMatrix Msr_inv;             	//Msr^(-1)
} INT_TRANS;

typedef	struct TYPE_TABLE1 INT_TABLE1;
typedef	struct TYPE_TABLE1
{
    INT_MODAL modal;					//modal data
    INT_SINGLE single;                  //single data
    INT_PROGRAM program;                //program information data
    INT_TRANS trans;                    //coordinate data
    //INT_TRANS UV_trans;                 //coordinate data
    INT_TABLE1 *next;                   //for building a linked list
}/* __attribute__((packed, aligned(1))) */INT_TABLE1;

#define MAX_LINE 5
typedef struct tagMOTION
{
    char move_gcode;            		//Indicates G0/1/2/3/33/-1...
    char type;                  		//Indicates type of compensation
    unsigned char line_no;      		//Indicates line no.
    unsigned char reserve1;				//reserve
    INT_COORD6 end_point;       //for G00 or G01
    INT_COORD6 circle_start;    //for G02 or G03
    INT_COORD6 circle_middle;   //for G02 or G03
    INT_COORD6 circle_end;      //for G02 or G03
} INT_MOTION;

typedef struct tagNONMOTION
{
    unsigned char error;    			//Error no.
    char prog_end;          			//Indicates program end.
    //char B_flag;            			//Is B code present?
    //char C_flag;            			//Is C code present?
    char M_flag;            			//Is M code present?
    //char S_flag;            			//Is S code present?
    char T_flag;            			//Is T code present?(For tool no.)
    char x_flag;            			//Is X address present?(For G4/22/27/28/30/53/92)
    char y_flag;            			//Is Y address present?(For G22/27/28/30/53/92)
    char z_flag;            			//Is Z address present?(For G22/27/28/30/53/92)
    //char c_flag;            			//Is 4th address present?(For G53/92)
    //char a_flag;            			//Is 5th address present?(For G53/92) //930128
    char u_flag;            			//Is U address present?(For G28/30/92)
    char v_flag;            			//Is V address present?(For G28/30/92)
    char w_flag;            			//Is V address present?(For G28/30/92)
    char t_flag;            			//Is W address present?(For G28/30/92)
    char i_flag;            			//Is I address present?(For G22)
    char j_flag;            			//Is J address present?(For G22)
    char k_flag;            			//Is K address present?(For G22)
    //char l_flag;            			//Is L address present?
    //char Exact_stop;        			//In-position check
    char GroupSINGLE;       			//Indicates G4/09/10/27/28/29/30/53/92
    char GroupSTROKE;       			//Indicates G22/23
    char GroupCOOR;         			//Indicates G54-G59
    char GroupPLANE;        			//Indicates G17/18/19
    //char GroupFEED;         			//G94/95:Feed per min./rev.,constant/servo feed
    //char GroupMODE;         			//Indicates G61-G64
    //char GroupCSS;          			//Indicates G96/97
    //char GroupSFD;          			//Indicates G25/26
    char last_inch_mm;      			//Indicates mm/inch command of the last block
    char exact_stop;        			//In-position check
    int feedrate;           //feedrate percentage
    int posture_middle;     //G00/G01時:目標位置的姿態, G02/G03時:中間點的姿態
    int posture_final;      //在G02/G03指令中，表示目標位置的姿態
    int M_code;             //M code
    int T_code;             //Tool no.
    int p;                 	//for G10 P***
    char c;                 //for Cartesian coord(c=1) or not
    INT_COORD6 xyz_data;
    INT_COORD3 ijk_data;
    //int B_code;             			//tool back type
    //int C_code;             			//machaning condition
    //int M_code;             			//M code
    //int S_rpm;              			//Spindle speed or Constant Surface Speed
    //int T_code;             			//Tool no.
    //double thread_lead;     			//Lead length of thread cutting.
    //INT_COORD4 xyz_data;
    //INT_COORD4 uvw_data;
    //INT_COORD4 UV_uvw_data; 			//for UV
    //INT_COORD3 ijk_data;
} INT_NONMOTION;

typedef struct tagREINT_DATA
{
    INT_COORD2 plane_vector;          	//Radius compensation vector
    INT_COORD2 len_vector;            	//Length compensation vector
    int        start_up_flag;         	//Radius compensation start up flag
} INT_REINT2;

typedef	struct tagTABLE2 INT_TABLE2;
typedef	struct tagTABLE2
{
    INT_MOTION    xy_plane[MAX_LINE]; 	//Indicates the move amount of XY PLANE
    INT_MOTION    uv_plane[MAX_LINE]; 	//Indicates the move amount of UV PLANE
    INT_NONMOTION nonmotion;          	//Indicates line no.
    INT_TABLE1    *curr_blk_ptr;      	//Indicates the pointer of executing blk.
    INT_TABLE2    *next;
    //INT_REINT2    reint2;             	//Indicates the re-int data for int2
}/*__attribute__((packed, aligned(1))) */INT_TABLE2;

typedef	struct def_CHAR_AXIS
{
       char x;
       char y;
       char z;
       char a;
       char b;  						// HSV
       char c;					
} CHAR_AXIS;

typedef	struct def_INT_AXIS
{
       int  x;
       int  y;
       int  z;
       int  a;
       int  b; 	 						// HSV
       int  c; 
} INT_AXIS;

typedef	struct def_LONG_AXIS
{
       long x;
       long y;
       long z;
       long a;
       long b;  						// HSV
       long c;  		
}LONG_AXIS;

typedef struct tagCNC_OFFSET
{
   /*char*/short type;                  			// TOOL TYPE 0 ~ 9
   long Glen1;                 			// GEOMETRY LENGTH 1
   long Glen2;                 			// GEOMETRY LENGTH 2
   long Gradius;               			// GEOMETRY RADIUS
   long Wlen1;                 			// WEAR LENGTH 1
   long Wlen2;                 			// WEAR LENGTH 2
   long Wlen2_wear;            			// 長度磨耗
   long Wradius;               			// WEAR RADIUS
   long Wradius_wear;          			// 半徑磨耗
}CNC_OFFSET;

typedef struct Electric_Circle_probe
{
    double          E_IsrRadius;
    LONG_AXIS       E_center;
} E_circle;

typedef	struct tagLONG_ISR_AXISs {
       long  x;
       long  y;
       long  z;
       long  a;
       long  b;  // HSV
       long  c;
}LONG_DATAS;

typedef struct {
        INT_MODAL  mdl;
        INT_SINGLE  sgl;
} CNC_BLOCK_DATA;

typedef 	struct	tagTrackPoint	//軌跡點
{
	//int				nIndex;
	unsigned char 	G_group1;
    LONG_AXIS		lAxis;
	struct	tagTrackPoint*	pNext;
}TRACKPOINT;

typedef enum { ID_SERVO=1, ID_TUNE, ID_MAKER, ID_USER, ID_COMMON, ID_ALL_PARAM} PARAM_TYPE;
typedef enum { ID_NO_ACTION=0, ID_START, ID_FEEDHOLD, ID_STOP, ID_RESET , ID_CNC_EXIT} ACTION_ID;
//typedef enum { ID_EDIT=1, ID_MANUAL, ID_AUTO, ID_MPG, ID_JOG, ID_RAPID, ID_HOME,ID_DNC} OPER_MODE;
typedef enum { ID_EDIT=1, ID_MANUAL, ID_AUTO, ID_MPG, ID_JOG, ID_RAPID, ID_HOME} OPER_MODE;//20111209 modify the same as statusbar0.cpp
typedef enum { EMPTY_STATUS=0, READ_OK_STATUS, FULL_STATUS, END_OF_FILE } BLOCK_QUEUE_STATUS;
typedef enum { OP_NOT_READY=0,OP_EDIT_MODE,OP_MEMORY_MODE,OP_MDI_MODE,OP_JOG_MODE,OP_INCJOG_MODE, OP_MPG_MODE, OP_HOME_MODE}ROBOT_STATUS;//statusbar->HMI Mode
typedef enum { ID_NO_ACT=0, ID_SET_FULL, ID_BUILD_QUEUE ,ID_BUILD_SUB_QUEUE} BLOCK_QUEUE_REQUEST;
typedef enum { ID_OPER=1,ID_INTER,ID_MOTION,ID_MLC,ID_DRIVERX,ID_DRIVERY,ID_DRIVERZ,ID_DRIVERA,ID_DRIVERB,ID_DRIVERC,ID_DRIVERD,ID_DRIVERE,ID_DSP} ALARM_GROUP;	//20120209 add
typedef enum { TP_NO_REQ=0, TP_ADD, TP_MODIFY, TP_DELETE, TP_FULL_LOAD=9} TP_OPR_MODE;



/*----------------------------------------------------------------
 *  TeachPointTable           
 *---------------------------------------------------------------*/
 
typedef struct def_TP_TABLE
{
	int 				CoordSysMaxTP[NUM_OF_COOR-1];   
	LONG_AXIS 	CoordValue[NUM_OF_COOR-1][NUM_OF_TP]; 
	int					CoordPos[NUM_OF_COOR-1][NUM_OF_TP];
}TP_TABLE;


typedef struct tmPOINTCOORD
{
	int CoordinateMode;
	int DefineMode;
	int	x;
	int y;
	int z;
	int a;
	int b;
	int c;
	int p;
}POINTCOORD;

typedef struct tagNC_Ethernet
{
	WORD	type;
	long	s_arr;
	WORD	len;
	char  data[ETHERNET_MAX_LEN];
}NC_Ethernet;

typedef struct tagUpdate_File_Name Update_File_Name;
typedef struct tagUpdate_File_Name{
	char	filename[50];
	Update_File_Name* next;
}Update_File_Name;

typedef struct tagNC_Ethernet_Prog{
	char	filename[ETHERNET_PROG_NAME];
	char  filedata[ETHERNET_PROG_DATE];
}NC_Ethernet_Prog;

typedef struct tagNC_TeachPoint{
	int 				Coordinate;   
	int					TeachPointNum;
	LONG_AXIS 	TeachPointValue; 
	int					TeachPointPos;
}NC_TeachPoint;

typedef struct tagNC_FileCheck{
	int 				isProg;
	char 				Full_File_Name[ETHERNET_PROG_NAME];   
	char				MD5_Code[DM5_LENGTH];				
}NC_FileCheck;

typedef struct tagRobot_Config{
	int 				Machine;
	int					jn;
	int					cn;
	char* 			j_s[10];  
	char* 			j_sid[10];   
	char* 			j_unit_sid[10];   
	char* 			c_s[10];   
	char* 			c_sid[10];    
	char* 			c_unit_sid[10];  			
}Robot_Config;




typedef struct tagCNC_TXSHMDATA
{
	int param_update_id;					
	struct tagParam								//此結構資料需要及時告知給host
	{
	    long       	servo_param[MAX_SERVO_NO];
	    long       	tune_param[MAX_TUNE_NO];
	    long				maker_param[MAX_MAKER_NO];
	    long       	user_param[MAX_USER_NO];
	    long       	common_param[MAX_COMMON_NO];
	}PARAM;
  char       		CNC_mem_part[INT_FILE_LEN];
  ACTION_ID  		CNC_action_id;
  OPER_MODE  		CNC_oper_mode;
  long       		CNC_start_blk_no;
  char      		CNC_block_buffer[INT_BUFFER_SIZE];
  int        		CNC_read_block_result; //Sunny<20100519>從TO_HMI移過來此處!
    //CNC_OFFSET CNC_offset[NUM_OF_OFFSET]; //20110908
	LONG_AXIS			CNC_work_coord[NUM_OF_COOR];
	LONG_AXIS			CNC_tool_coord[NUM_OF_COOR];
	CHAR_AXIS  		CNC_absolute_reset;
	LONG_AXIS  		CNC_absolute_preset;
  LONG_AXIS			CNC_work_shift;
  char       		CNC_single_block;
	char       		CNC_work_shift_flag; 
	char       		CNC_read_finish_flag;	//Sunny<20100526>add
	//macro SYSTEM parameter 2012/7/23
	int						macro_system_var_set_no;
	double				macro_system_var[INT_MACRO_SYS_PARA_NO];	//for $370~$399
	//Don't save behind here
  LONG_AXIS  		CNC_machine;   //Sunny<20100813>
  LONG_AXIS  		CNC_absolute;  //Sunny<20100813>
  int						CNC_posture;  
	LONG_AXIS  		CNC_angular_pos;
  int       		CNC_work_coord_no;  //1:00, 2:G54, 3:G55, 4:G56..., 8:G154, 9:G155   //Sunny<20100813>
	int						CNC_tool_coord_no;	
	int						CNC_feed_over_cmd;  //2015-03-11		//自動模式速度(百分比)
	int						CNC_jog_over_cmd; 	//2015-03-11		//手動模式速度(百分比)																				add by mario 2015/3/2 上午 11:27:44
 	char     			CNC_jog_mode;   			//手動模式下的移動模式(0=連續 / 1=吋動)											add by mario 2015/3/2 上午 11:27:44
 	int      			CNC_jog_inc;//CNC_jog_length;  			//吋動量(1~10) XYZ的單位是mm  RX,RY,RZ,J1~J6的單位還未定		add by mario 2015/3/2 上午 11:27:44
 	//char      		CNC_auto_mode;  			//自動模式下的執行方式(0=連續 / 1=單步)											add by mario 2015/3/2 上午 11:27:44
	char      		CNC_cycle_status;  			//自動模式下的循環(0=關 / 1=開)															add by mario 2015/3/2 上午 11:27:44
	char					CNC_option_stop;					//自動模式下，選擇性暫停(0=關 / 1=開) 2015/3/9 上午 10:53:12
	char					CNC_block_skip;				//自動模式下，遇到'/'是否執行(0=關 / 1=開) 2015/3/9 上午 10:53:13
//以上與資料庫順序相同
 	//char      	CNC_jog_return_home;        //手動模式回原點(1=回原點)
  int       		CNC_goto_tp_no;                   //手動模式移至教導點時，教導點的編號
  char      		CNC_jog_status;                   //手動模式下的程式運行控制(0=停止 / 1=執行 / 2=暫停)
  char      		CNC_auto_status;         //自動模式下的程式運行控制(0=停止 / 1=執行 / 2=暫停)
 	//add by Mario   2015/3/3 上午 09:35:03  原點設定相關控制
	char   				CNC_pos_zero;           //多圈歸零，預設0，1~6分別代表J1~J6
 	char   				CNC_pos_calibrate;        //校正，預設0，1~6分別代表J1~J6
 	int   				CNC_brake_release;        //剎車，預設0，1~6分別代表J1~J6
 	//Mario:--------首頁&手動模式控制手臂控制區 2015/1/29 下午 02:44:55
 	char   				CNC_jog_coor;           //0表示沒有控制手臂，1為直角座標，2為關節座標
 	int    				CNC_jog_axis;         //[0]~[5]分別為 直角座標{X,Y,Z,A,B,C}與 關節座標{J1,J2,J3,J4,J5,J6}──依 CNC_jog_coord決定。 1為(+)  -1為(-)
 	//Mario：-------教導點設定相關變數---------2015/4/1 
 	TP_OPR_MODE		CNC_tp_opr_mode;       //參照 TP_OPR_MODE;  
 	int    				CNC_tp_coord_sys;         //此教點所屬坐標系 1~24 
 	int    				CNC_tp_num;            //此教點編號 1~24
 	LONG_AXIS 		CNC_tp_coord;           //此教點座標值 
 	int       		CNC_tp_posture;         //0~7
  char      		CNC_servo_on; //0:OFF  1:ON
  char					CNC_machine_lock;
 	int       		CNC_mode_lgt;
  char					CNC_queue1[NUM_OF_BLOCK][INT_BUFFER_SIZE]; //2010.12.16 added
  char					CNC_queue2[NUM_OF_BLOCK][INT_BUFFER_SIZE]; //2010.12.16 added
  int						CNC_queue1_status;
  int						CNC_queue2_status;
  int						CNC_queue1_total_block;
  int						CNC_queue2_total_block;
	//20110510 addMDI queue >>>>> 
	double     		CNC_macro_local_var[INT_LV_NO];//34		//20110715 add
	long  				lReadCurveIndex;	//目前HMI已讀取到的曲線資料序號20111110 add
 	long  				lReadCurveTimes;    //目前HMI已讀取曲線迴圈次數20111110 add
	long  				lCurveSampleTime;	//PC申請的曲線取樣時間20111110 add
  char      		CNC_stop_action_OK;  //HMI通知NC已經切換按鍵至"停止"(畫面上的暫停按鈕)
  int						CNC_update_wt_coord; //工作/工具座標值更新  預設0  工作座標更新1~25    工具座標更新101~125
  int						CNC_jaws_control;		//第一夾爪OFF +=1  第二夾爪OFF +=2 第三夾爪OFF +=4 依此類推
  long					CNC_start_encoder_value[6];
  char					iNet_Server_IP[17];
  char					iNet_LinkState; // 0:關閉 1:連線 2:重新連線
}CNC_TXSHMDATA;

typedef struct tagCNC_TXDATA
{
	long	lOffset;
	long	lLength;
  CNC_TXSHMDATA shmCNCData;    
}CNC_TXDATA;

#ifdef	D_TEST
typedef	struct tagCNC_RXDATA
{
	char       s_mlc_i_bit[512]; 		/* I bit buffer address */
  char       s_mlc_o_bit[512]; 		/* O bit buffer address */
  char       s_mlc_c_bit[512]; 		/* C bit buffer address */
  char       s_mlc_s_bit[512]; 		/* S bit buffer address */
  char       s_mlc_a_bit[512]; 		/* A bit buffer address */
  char       s_mlc_cnt_stus[512]; 	/* COUNTER STATUS BIT */
  char       s_mlc_tmr_stus[512]; 	/* TIMER STATUS BIT */
  char       s_mlc_tmr1[512];
	int        s_mlc_cnt[512];
	int        s_mlc_reg[512];
	int        s_mlc_breg[512];
	int        s_mlc_store[100];
	long       maker_param[MAX_MAKER_NO];
	long       user_param[MAX_USER_NO];
}/*__attribute__((packed, aligned(1)))	*/CNC_RXDATA;
#else
typedef	struct tagCNC_RXDATA
{
		char       s_mlc_i_bit[512]; 			/* I bit buffer address */
    char       s_mlc_o_bit[512]; 			/* O bit buffer address */
    char       s_mlc_c_bit[512]; 			/* C bit buffer address */
    char       s_mlc_s_bit[512]; 			/* S bit buffer address */
    char       s_mlc_a_bit[512]; 			/* A bit buffer address */
    char       s_mlc_cnt_stus[512]; 	/* COUNTER STATUS BIT */
    char       s_mlc_tmr_stus[512]; 	/* TIMER STATUS BIT */
    char       s_mlc_tmr1[512];			
    char       CNC_WaitMST;
    char       CNC_InPosCheckFlag;
    char       CNC_mot_cont_cmd;    	// motion control command
    char       CNC_mot_isr_state;   	// isr state
    char       CNC_mot_residue;     	// isr residule huffer state
    char       CNC_RemberPosFlag;
    char	   	 reserve1;				//add reserve1
		int        s_mlc_cnt[512];
		int        s_mlc_reg[512];				//Mario add 2015/4/1
    int        CNC_table2_ptr;
    int        CNC_table2_next_ptr; 
    int        CNC_table2_next_next_ptr; 
    int        CNC_curr_blk_ptr; 		//Diag-2818 Line0
    int        CNC_next_blk_ptr; 
    int        CNC_next_next_blk_ptr;
    int        CNC_status;
    int        CNC_alarm_group; 		//OPER,INTER,MOTION,MLC 
    int        CNC_alarm_id;
    int        CNC_reset_flag;
    int        CNC_isr_switch;      	// isr basic command switch
    long       CNC_part_count;          // Part count No. for display     
    long       CNC_cycle_time;          // CNC cycle time
    long       CNC_run_time;            // CNC run time
    long       CNC_Isr_timer;       	// System Timer
    long       CNC_feedrate;        	// feedrate
    long       CNC_JogSpeed;        	// feedrate
    long       CNC_IndexCounter;
    double     CNC_feed_over_cmd;   	// auto & mdi of feedrate
    //double     CNC_rpfeed_over_cmd; 	// feedrate override command
    struct tagCNC_Macro_LocalVar		// need save sram and start send once
    {
    	double     CNC_macro_local_var[INT_LV_NO];//34
    	double     CNC_macro_global_var[INT_GV_NO];//101
    }CNC_MACRO_LOCALVAR;
    INT_AXIS   CNC_servo_lag;       	// servo lag
    LONG_AXIS  CNC_NoAceMachPos;    	// Interpolation of Machine Position
    LONG_AXIS  CNC_MachProPos;
    LONG_AXIS CNC_IsrMachPos;
    LONG_AXIS  CNC_machine;         	// machine position
    LONG_AXIS  CNC_absolute;        	// absolute position
		int        CNC_posture; 
    LONG_AXIS  CNC_angular_pos;
    LONG_AXIS  CNC_dis_to_go;       	// cnc distance to go
   	LONG_AXIS  total_pulse; 			//=>motion module送給DSP28的total pulse for one block
    INT_AXIS   pulse[MAX_DATA];     //=> motion module送給DSP28的 pulse for one cycle time//20111110 modify yangpeng pulse[]=====>*pulse
    LONG_AXIS  abspos[MAX_DATA];     //=>absolute counter value from DSP28
    LONG_AXIS  error[MAX_DATA];     //=>error counter value from DSP28    //end modify
    char       CNC_read_block_flag;		//=>此值為1時HMI必須
    long       CNC_read_block_no;
    char       CNC_read_block_filename[INT_FN_LEN];
    //char       CNC_read_block_skip_flag;  Mario mark  移到 to cnc 2015/3/9 上午 10:53:00
    int        CNC_mcode_cmd;
    //int		   	CNC_scode_cmd;
    int		   	CNC_tcode_cmd;
    char       CNC_prog_end;
		INT_TABLE2 CNC_table2;
    INT_TABLE2 CNC_table2_next; 
    INT_TABLE2 CNC_table2_next_next; 
    INT_TABLE1 CNC_curr_blk; 
    INT_TABLE1 CNC_next_blk; 
    INT_TABLE1 CNC_next_next_blk;
    //Sunny<20100812><<<<<
    int       	CNC_set_work_shift;
    char       	CNC_set_work_shift_flag;
    int       	CNC_set_work_coord;
    //CHAR_AXIS  	CNC_set_absolute_reset; //20110908 mark
    //CHAR_AXIS  	CNC_set_relative_reset; //20110908 mark
    CHAR_AXIS  	CNC_set_absolute_preset;
    LONG_AXIS  	CNC_work_shift;
    char       	CNC_work_shift_flag; 
    //CNC_OFFSET CNC_offset[NUM_OF_OFFSET]; //20110908
    LONG_AXIS  	CNC_work_coord[NUM_OF_COOR];
		LONG_AXIS		CNC_tool_coord[NUM_OF_COOR];
    CHAR_AXIS  	CNC_absolute_reset;
    LONG_AXIS  	CNC_absolute_preset;
    int         CNC_request_queue1;  //2010.12.16 added
    int         CNC_request_queue2;  //2010.12.16 added
    int         CNC_rebuild_all_queue;  //2010.12.21 added
	//20110510 addMDI queue <<<<<
	//20110510 addMDI queue >>>>>
	long  		lWriteCurveIndex;		//目前NC寫入的曲線資料序號20111110 add
  long  		lWriteCurveTimes;    	//目前NC寫入曲線迴圈次數20111110 add
  LONG_AXIS	lnGridDis;				//20120216 add 回原點柵格量調整，並且在系統畫面中顯示出來，單位為um
    //macro SYSTEM parameter 2012/7/23
	int				macro_system_var_set_no;
	double		macro_system_var[INT_MACRO_SYS_PARA_NO];
	char			CNC_nc_version[16]; //2013-02-27, wilson Lu
	char  			CNC_plc_version[16]; //2013-02-27, wilson Lu
	char      CNC_prog_protect;
	long			CNC_encoder_value[6];
	int				CNC_barke_status;
	int				CNC_return_tp_coord_sys;		//Mario add  2015/4/1
	int				CNC_return_tp_num;		//Mario add  2015/4/1
	char      CNC_pos_return_zero; //CNC_pos_zero_OK
	char      CNC_pos_return_calibrate; //CNC_pos_calibrate_OK
  char      CNC_inform_stop; //在自動模式通知HMI切換按鍵至"停止"
  char			CNC_update_wt_coord_OK; //工作/工具座標值更新  預設0  更新1 
  char      CNC_servo_status;	//0:關  1:開  
  char			CNC_move_end;
}/*__attribute__((packed, aligned(1)))	*/CNC_RXDATA;
//未強行一位對齊，長度為1226706，值對不上；強行一位對齊，長度為1226704，值對上ok
#endif
/*===================================================================================
note:
1.CNC_read_block_no是目前要讀取的第幾個單節號碼。例如，當解譯器要讀取加工程式中的第5個單節時，解譯器就會設定
  CNC_read_block_no=5，則HMI從檔案中讀取第5個單節的內容。
2.HMI必須隨時檢查CNC_read_block_flag的值，當此變數的值為1時，HMI必須做讀取加工程式單節內容的動作，並且設定
  CNC_read_block_flag=2。
3.HMI依照解譯器設定的CNC_read_block_filename內容去開啟該檔案並且做讀檔的動作。
4.當HMI讀取單節時，若讀取成功時令CNC_read_block_result=讀取到的block的字元數
  否則，當開檔失敗請設定CNC_read_block_result=NERR_FILENOTFOUND
  當讀到檔尾還沒有讀到想要的單節時令CNC_read_block_result=NERR_BLOCKNOTFOUND
6.CNC_curr_blk.modal.bn為目前的單節編號(block number)，HMI可用此來設定目前螢幕上游標的位置。
=====================================================================================*/
#pragma pack(pop)
#endif

