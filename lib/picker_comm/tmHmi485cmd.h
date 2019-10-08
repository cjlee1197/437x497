/*
 * hmiCmd.h
 *
 *  Created on: 2015-8-7
 *      Author: admin
 */

#ifndef HMI485CMD_H_
#define HMI485CMD_H_

#define    HMI_MODE_AUTO       2       //自动模式
#define    HMI_MODE_MDI        3       //手动模式

#define    BUFF_AXIS_STAT	   	 8
#define    BUFF_IO_M_OUT       8
#define    BUFF_IO_T_IN        8
#define    BUFF_EUROMAP_OUT    4
#define    BUFF_EUROMAP_IN     4
#define    BUFF_IO_SYS_IN      4
#define    BUFF_SAFE_REGION    4

#define    SIZE_INT    	    sizeof(int)
#define    SIZE_CHAR   	    sizeof(char)
#define    SIZE_FLOAT  	    sizeof(float) 
#define    SIZE_DOUBLE 	    sizeof(double)

#define    HMI_FILE_NAME_DEFAULT       "rob.ngc"
#define    HMI_VEL_SCALE               0.3


//接收数据分析
#define FRAME_HEAD 					0x7f      //数据帧包头
#define FRAME_TAIL					0x7e      //数据帧包尾

//接受数据
#define IDLE_CMD      			0x01      //数据帧空闲命令
#define MOT_CMD       			0x02      //数据帧运动控制命令
#define FILE_CMD      			0x03      //数据帧文件名命令
#define RESET_CMD     			0x04      //数据帧复位命令

//发送数据
#define RBOT_SYS_STAT				0x01     //状态帧机械手控制器码
#define RBOT_FILE_NAME  		0X02     //状态帧文件名状态码
#define RBOT_COM_ERR				0x03     //状态帧错误码

/**********************************************************************************/
/**************************    控制命令和状态数据      ****************************/
/**********************************************************************************/

//机械手控制器状态数据结构
typedef struct
{
	//机械手运动位置
	int   x;                          //X轴位置
	int   y;                          //Y轴位置
	int   z;                          //Z轴位置
	int   a;                          //A轴位置
	int   b;                          //B轴位置
	int   c;                          //C轴位置
	//控制权标志   
	unsigned char sys_control;         //0 默认为机械手  1 塑机
	//控制器出错状态
	unsigned char sys_error;           //控制器错误状态
	//系统状态及模式
	unsigned char sys_paused;         //系统暂停标志位
	unsigned char sys_mode;           //系统模式
	//系统运动轴状态
	unsigned char m_axis_status[BUFF_AXIS_STAT];     //轴运行状态
	//系统输出状态
	unsigned char io_m_output[BUFF_IO_M_OUT];        //气动信号IO输出状态
	unsigned char euromap_output[BUFF_EUROMAP_OUT];  //欧规相关IO输出状态
	//系统输入状态
	unsigned char io_t_input[BUFF_IO_T_IN];          //气动信号IO输入状态
	unsigned char io_sys_input[BUFF_IO_SYS_IN];      //正负限位和零点信号
	unsigned char euromap_input[BUFF_EUROMAP_IN];    //欧规相关IO输入状态
	unsigned char safe_region[BUFF_SAFE_REGION];     //运动轴安全区IO输入状态
}SYS_STAT_FOR_485;

//控制器状态读取（空）命令内容
//typedef struct
//{
//	unsigned char state;   //无实际意义
//
//}HMI_485_SYS_IDLE;

//控制器复位命令内容
typedef struct
{
	unsigned char state;   //无实际意义

}HMI_485_SYS_RESET;

//控制器运行停止命令内容
typedef struct
{
	unsigned char state;  //无实际意义

}HMI_485_SYS_ABORT;


//控制器模式设置命令内容
typedef struct
{
	int mode;             //1自由 2自动  3手动

}HMI_485_SET_MODE;


//控制器回零命令内容
typedef struct
{
	int axisNum;          //回零具体轴

}HMI_485_SET_HOME;

//轴正方向移动命令内容
typedef struct
{
	unsigned int axisNum; //移动具体轴号
	int vel;  //速度等级1(10%),2(30%),3(50%),4(80%),5(100%)
	int pos;  //位置等级1(0.1),2(1.0),3(10.0),4(50.0),5(100.0)

}HMI_485_POS_AXIS_MOVE;

//轴负方向移动命令内容
typedef struct
{
	unsigned int axisNum; //移动具体轴号
	int vel;  //速度等级1(10%),2(30%),3(50%),4(80%),5(100%)
	int pos;  //位置等级1(0.1),2(1.0),3(10.0),4(50.0),5(100.0)

}HMI_485_NEG_AXIS_MOVE;

//控制器自动运行命令内容
typedef struct
{
	unsigned int line;   //程序启动行数 默认0

}HMI_485_INTERP_RUN;

//控制器暂停运行命令内容
typedef struct
{
	unsigned char state;  //无实际意义

}HMI_485_INTERP_PAUSE;

//控制器继续运行命令内容
typedef struct
{
	unsigned char state;   //无实际意义

}HMI_485_INTERP_RESUME;

//控制器单步运行命令内容
typedef struct
{
	unsigned char state;   //无实际意义

}HMI_485_INTERP_STEP;

//控制器更换模具命令内容
typedef struct
{
	unsigned short mouldNum;  //模具号

}HMI_485_SET_MOULD_NUM;

//控制权命令内容
typedef struct
{
	unsigned char stat;    // 0机械手控制 1塑机控制	
	
}HMI_485_CONTROL_ROBOT;

//倒角命令内容
typedef struct
{
	unsigned char stat;   // 0 倒角垂直 1 倒角水平
	
}HMI_485_SET_CHAMFER_FLAG;

//吸具命令内容
typedef struct
{
	unsigned char stat;   // 0 吸具关闭 1 吸具打开
	
}HMI_485_SET_FIXTURE_FLAG;

//机械手控制器命令内容数据结构
typedef union
{
	//HMI_485_SYS_IDLE         hmiSysIdle;
	HMI_485_SYS_RESET        hmiSysReset;
	HMI_485_SYS_ABORT        hmiSysAbort;
	HMI_485_SET_MODE         hmiSetMode;
	HMI_485_SET_HOME         hmiSetHome;
	HMI_485_POS_AXIS_MOVE    hmiAxisPosMove;
	HMI_485_NEG_AXIS_MOVE    hmiAxisNegMove;
	HMI_485_INTERP_RUN       hmiInterpRun;
	HMI_485_INTERP_PAUSE     hmiInterpPause;
	HMI_485_INTERP_RESUME    hmiInterpResume;
	HMI_485_INTERP_STEP      hmiInterpStep;
	HMI_485_SET_MOULD_NUM    hmiSetMouldNum;
	HMI_485_CONTROL_ROBOT    hmiControlRobot;
	HMI_485_SET_CHAMFER_FLAG hmiSetChamferFlag;
	HMI_485_SET_FIXTURE_FLAG hmiSetFixtureFlag;

}HMI_485_CMD_DATA;

//机械手控制器命令类型数据结构
typedef enum
{
	HMI_485_SYS_IDLE_TYPE,               //空命令（状态读取命令）
	HMI_485_SYS_RESET_TYPE,              //控制器复位命令
	HMI_485_SYS_ABORT_TYPE,              //控制器运行停止命令
	HMI_485_SET_MODE_TYPE,               //控制器模式设置命令
	HMI_485_SET_HOME_TYPE,               //控制器回零命令
	HMI_485_AXIS_POS_MOVE_TYPE,          //轴正向移动命令
	HMI_485_AXIS_NEG_MOVE_TYPE,          //轴负向移动命令
	HMI_485_INTERP_RUN_TYPE,             //控制器自动运行命令
	HMI_485_INTERP_PAUSE_TYPE,           //控制器暂停运行命令
	HMI_485_INTERP_RESUME_TYPE,          //控制器继续运行命令
	HMI_485_INTERP_STEP_TYPE,            //控制器单步运行命令
	HMI_485_SET_MOULD_NUM_TYPE,          //控制器更换模具命令
	HMI_485_CONTROL_ROBOT_TYPE,          //注塑机控制机械手命令
	HMI_485_SET_CHAMFER_FLAG_TYPE,       //控制器倒角操作指令
	HMI_485_SET_FIXTURE_FLAG_TYPE,       //控制器吸具操作指令

}HMI_485_CMD_TYPE;

//机械手控制器命令数据结构
typedef struct
{
	unsigned int  hmiCmdType;
	HMI_485_CMD_DATA hmiCmdData;

}HMI_485_CMD;

/**********************************************************************************/
/**************************    文件命令和状态数据      ****************************/
/**********************************************************************************/

//ゅン篈计沮挡篶
typedef struct
{
	unsigned short echoMouldNum;   //回应模具号
	char programFileName[64];      //模具号应程序名称
	char cReserved[2];

}FILE_NAME_FOR_485;

//模具号命令
typedef struct
{
	unsigned short hmiMouldNum;   //模具号

}HMI_485_FILE_ECHO_MOULD_NUM;

//文件名命令内容
typedef union
{
	//获得对应模具号回应值及文件名命令内容
	HMI_485_FILE_ECHO_MOULD_NUM       fileEchoMouldNum;

}HMI_485_FILE_CMD_DATA;

//文件名命令类型
typedef enum
{
	HMI_485_FILE_MODULE_NAME_TYPE    //获得对应模具号回应值及文件名

}HMI_485_FILE_CMD_TYPE;

//文件名命令数据结构
typedef struct
{
	unsigned int  hmiCmdType;            //命令类型
	HMI_485_FILE_CMD_DATA hmiCmdData;    //命令内容

}HMI_485_FILE_CMD;

/********* 给画面逻辑用，返回一个结构体HMI_485_CMD ************/
//  控制命令
HMI_485_CMD  hmiSysReset(char state);    //重置
HMI_485_CMD  hmiSysAbort(char state);    //停止
HMI_485_CMD  hmiSetMode(int mode);       //设置模式 1自由 2手动 3自动
HMI_485_CMD  hmiSetHome(int axisNum);    //回零
HMI_485_CMD  hmiAxisPosMove(int axisNum, int vel, int pos);//轴正向移动
HMI_485_CMD  hmiAxisNegMove(int axisNum, int vel, int pos);//轴负向移动
HMI_485_CMD  hmiInterpRun(int line);     //自动运行
HMI_485_CMD  hmiInterpPause(char state); //暂停
HMI_485_CMD  hmiInterpResume(char state);//继续
HMI_485_CMD  hmiInterpStep(char state);  //单步
HMI_485_CMD  hmiSetMouldNum(short mouldNum);//换摸
HMI_485_CMD  hmiControlRobot(char state);//控制权 0机械手 1塑机
HMI_485_CMD  hmiSetChamferFlag(char state);//倒角 0垂直  1水平
HMI_485_CMD  hmiSetFixtureFlag(char state);//吸具 0关闭  1打开

//  文件命令
HMI_485_FILE_CMD 	hmiFileCmd(short hmiMouldNum);

#endif /* HMICMD_H_ */
