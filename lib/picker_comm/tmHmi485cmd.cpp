/*
 * hmiCmd.h
 *
 *  Created on: 2015-9-3
 *      Author: wang.z.w
 */

#include "tmHmi485cmd.h"
#include <stdio.h>
#include <string.h>

/***************** 给画面逻辑用，返回一个结构体HMI_485_CMD指针 ****************/

// cmd : reset 重置
HMI_485_CMD 	hmiSysReset(char state)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("SysReset(): ...\n");
	cmdTempData.hmiCmdType 					    =	HMI_485_SYS_RESET_TYPE;
	cmdTempData.hmiCmdData.hmiSysReset.state 	=	state;
	return cmdTempData;
}

// cmd : stop  停止
HMI_485_CMD 	hmiSysAbort(char state)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("SysAbort(): ...\n");
	cmdTempData.hmiCmdType 					   =   HMI_485_SYS_ABORT_TYPE;
	cmdTempData.hmiCmdData.hmiSysAbort.state   =   state;
	return cmdTempData;
}

// cmd : mode	模式
HMI_485_CMD 	hmiSetMode(int mode)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("SetMode(): mode=%d\n", mode);
	cmdTempData.hmiCmdType 					     =	 HMI_485_SET_MODE_TYPE;
	cmdTempData.hmiCmdData.hmiSetMode.mode   	 =   mode;
	return cmdTempData;
}

// cmd : home  回零
HMI_485_CMD 	hmiSetHome(int axisNum)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("SetHome(): ...\n");
	cmdTempData.hmiCmdType 					    =	HMI_485_SET_HOME_TYPE;
	cmdTempData.hmiCmdData.hmiSetHome.axisNum   =   axisNum;
	return cmdTempData;
}

// cmd : axis pos move 轴正向移动
// para:   axisNum; //移动具体轴号
//		 vel;  //速度等级1(10%),2(30%),3(50%),4(80%),5(100%)
//		 pos;  //位置等级1(0.1),2(1.0),3(10.0),4(50.0),5(100.0)
HMI_485_CMD 	hmiAxisPosMove(int axisNum, int vel, int pos)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("AxisPosMove(): ...\n");
	cmdTempData.hmiCmdType 					        			 =	 HMI_485_AXIS_POS_MOVE_TYPE;
	cmdTempData.hmiCmdData.hmiAxisPosMove.vel   	 =   vel;
	cmdTempData.hmiCmdData.hmiAxisPosMove.pos   	 =   pos;
	cmdTempData.hmiCmdData.hmiAxisPosMove.axisNum  =   axisNum;
	return cmdTempData;
}

// cmd : axis neg move 轴负向移动
// para:   axisNum; //移动具体轴号
//		 vel;  //速度等级1(10%),2(30%),3(50%),4(80%),5(100%)
//		 pos;  //位置等级1(0.1),2(1.0),3(10.0),4(50.0),5(100.0)
HMI_485_CMD 	hmiAxisNegMove(int axisNum, int vel, int pos)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("AxisNegMov(): ...\n");
	cmdTempData.hmiCmdType 					               =   HMI_485_AXIS_NEG_MOVE_TYPE;
	cmdTempData.hmiCmdData.hmiAxisNegMove.vel   	 =   vel;
	cmdTempData.hmiCmdData.hmiAxisNegMove.pos   	 =   pos;
	cmdTempData.hmiCmdData.hmiAxisNegMove.axisNum  =   axisNum;
	return cmdTempData;
}

// cdm : interp run 运行
HMI_485_CMD 	hmiInterpRun(int line)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("InterpRun(): ...\n");
	cmdTempData.hmiCmdType 					    =	HMI_485_INTERP_RUN_TYPE;
	cmdTempData.hmiCmdData.hmiInterpRun.line    =   line;
	return cmdTempData;
}

// cmd : interp pause 暂停
HMI_485_CMD 	hmiInterpPause(char state)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("InterpPause(): ...\n");
	cmdTempData.hmiCmdType 					    =	HMI_485_INTERP_PAUSE_TYPE;
	cmdTempData.hmiCmdData.hmiInterpPause.state	= 	state;
	return cmdTempData;
}

// cmd : interp resume 继续运行
HMI_485_CMD 	hmiInterpResume(char state)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("InterpResume(): ...\n");
	cmdTempData.hmiCmdType 						  =	    HMI_485_INTERP_RESUME_TYPE;
	cmdTempData.hmiCmdData.hmiInterpResume.state  = 	state;
	return cmdTempData;
}

// cmd : interp step 单步执行
HMI_485_CMD 	hmiInterpStep(char state)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	//printf("InterpStep(): ...\n");
	cmdTempData.hmiCmdType 					    =	HMI_485_INTERP_STEP_TYPE;
	cmdTempData.hmiCmdData.hmiInterpStep.state 	= 	state;
	return cmdTempData;	
}

// cmd : set mouldNum 更换模具
HMI_485_CMD 	hmiSetMouldNum(short mouldNum)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	
	//printf("SetMouldNum(): ...\n");
	cmdTempData.hmiCmdType 						  	=	HMI_485_SET_MOULD_NUM_TYPE;
	cmdTempData.hmiCmdData.hmiSetMouldNum.mouldNum  =   mouldNum;
	return cmdTempData;
}

// cmd : set controler 设置控制者标志
HMI_485_CMD  hmiControlRobot(char state)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	
	//printf("hmiControlRobot(): state=%d\n", state);
	cmdTempData.hmiCmdType 						  	       =	HMI_485_CONTROL_ROBOT_TYPE;
	cmdTempData.hmiCmdData.hmiControlRobot.stat  =  state;
	return cmdTempData;
}

//cmd : set chamferFlag 设置倒角
HMI_485_CMD  hmiSetChamferFlag(char state)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	
	//printf("hmiSetChamferFlag(): ...\n");
	cmdTempData.hmiCmdType 						  	         =	HMI_485_SET_CHAMFER_FLAG_TYPE;
	cmdTempData.hmiCmdData.hmiSetChamferFlag.stat  =  state;
	return cmdTempData;
}

// cmd : set fixtureFlag 设置吸具
HMI_485_CMD  hmiSetFixtureFlag(char state)
{
	HMI_485_CMD cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	
	//printf("hmiSetFixtureFlag(): ...state=%d\n", state);
	cmdTempData.hmiCmdType 						  	         =	HMI_485_SET_FIXTURE_FLAG_TYPE;
	cmdTempData.hmiCmdData.hmiSetFixtureFlag.stat  =  state;
	return cmdTempData;
}

//  File cmd: 文件命令
HMI_485_FILE_CMD 	hmiFileCmd(short hmiMouldNum)
{
	HMI_485_FILE_CMD  cmdTempData;
	memset(&cmdTempData, 0, sizeof(cmdTempData));
	
	//printf("hmiFileCmd(): ...\n");
	cmdTempData.hmiCmdType 						  		 =	 HMI_485_FILE_MODULE_NAME_TYPE;
	cmdTempData.hmiCmdData.fileEchoMouldNum.hmiMouldNum  =   hmiMouldNum;
	return cmdTempData;
}
