/*
 * hmiCmd.h
 *
 *  Created on: 2015-8-7
 *      Author: admin
 */

#ifndef HMI485CMD_H_
#define HMI485CMD_H_

#define    HMI_MODE_AUTO       2       //�Զ�ģʽ
#define    HMI_MODE_MDI        3       //�ֶ�ģʽ

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


//�������ݷ���
#define FRAME_HEAD 					0x7f      //����֡��ͷ
#define FRAME_TAIL					0x7e      //����֡��β

//��������
#define IDLE_CMD      			0x01      //����֡��������
#define MOT_CMD       			0x02      //����֡�˶���������
#define FILE_CMD      			0x03      //����֡�ļ�������
#define RESET_CMD     			0x04      //����֡��λ����

//��������
#define RBOT_SYS_STAT				0x01     //״̬֡��е�ֿ�������
#define RBOT_FILE_NAME  		0X02     //״̬֡�ļ���״̬��
#define RBOT_COM_ERR				0x03     //״̬֡������

/**********************************************************************************/
/**************************    ���������״̬����      ****************************/
/**********************************************************************************/

//��е�ֿ�����״̬���ݽṹ
typedef struct
{
	//��е���˶�λ��
	int   x;                          //X��λ��
	int   y;                          //Y��λ��
	int   z;                          //Z��λ��
	int   a;                          //A��λ��
	int   b;                          //B��λ��
	int   c;                          //C��λ��
	//����Ȩ��־   
	unsigned char sys_control;         //0 Ĭ��Ϊ��е��  1 �ܻ�
	//����������״̬
	unsigned char sys_error;           //����������״̬
	//ϵͳ״̬��ģʽ
	unsigned char sys_paused;         //ϵͳ��ͣ��־λ
	unsigned char sys_mode;           //ϵͳģʽ
	//ϵͳ�˶���״̬
	unsigned char m_axis_status[BUFF_AXIS_STAT];     //������״̬
	//ϵͳ���״̬
	unsigned char io_m_output[BUFF_IO_M_OUT];        //�����ź�IO���״̬
	unsigned char euromap_output[BUFF_EUROMAP_OUT];  //ŷ�����IO���״̬
	//ϵͳ����״̬
	unsigned char io_t_input[BUFF_IO_T_IN];          //�����ź�IO����״̬
	unsigned char io_sys_input[BUFF_IO_SYS_IN];      //������λ������ź�
	unsigned char euromap_input[BUFF_EUROMAP_IN];    //ŷ�����IO����״̬
	unsigned char safe_region[BUFF_SAFE_REGION];     //�˶��ᰲȫ��IO����״̬
}SYS_STAT_FOR_485;

//������״̬��ȡ���գ���������
//typedef struct
//{
//	unsigned char state;   //��ʵ������
//
//}HMI_485_SYS_IDLE;

//��������λ��������
typedef struct
{
	unsigned char state;   //��ʵ������

}HMI_485_SYS_RESET;

//����������ֹͣ��������
typedef struct
{
	unsigned char state;  //��ʵ������

}HMI_485_SYS_ABORT;


//������ģʽ������������
typedef struct
{
	int mode;             //1���� 2�Զ�  3�ֶ�

}HMI_485_SET_MODE;


//������������������
typedef struct
{
	int axisNum;          //���������

}HMI_485_SET_HOME;

//���������ƶ���������
typedef struct
{
	unsigned int axisNum; //�ƶ��������
	int vel;  //�ٶȵȼ�1(10%),2(30%),3(50%),4(80%),5(100%)
	int pos;  //λ�õȼ�1(0.1),2(1.0),3(10.0),4(50.0),5(100.0)

}HMI_485_POS_AXIS_MOVE;

//�Ḻ�����ƶ���������
typedef struct
{
	unsigned int axisNum; //�ƶ��������
	int vel;  //�ٶȵȼ�1(10%),2(30%),3(50%),4(80%),5(100%)
	int pos;  //λ�õȼ�1(0.1),2(1.0),3(10.0),4(50.0),5(100.0)

}HMI_485_NEG_AXIS_MOVE;

//�������Զ�������������
typedef struct
{
	unsigned int line;   //������������ Ĭ��0

}HMI_485_INTERP_RUN;

//��������ͣ������������
typedef struct
{
	unsigned char state;  //��ʵ������

}HMI_485_INTERP_PAUSE;

//����������������������
typedef struct
{
	unsigned char state;   //��ʵ������

}HMI_485_INTERP_RESUME;

//����������������������
typedef struct
{
	unsigned char state;   //��ʵ������

}HMI_485_INTERP_STEP;

//����������ģ����������
typedef struct
{
	unsigned short mouldNum;  //ģ�ߺ�

}HMI_485_SET_MOULD_NUM;

//����Ȩ��������
typedef struct
{
	unsigned char stat;    // 0��е�ֿ��� 1�ܻ�����	
	
}HMI_485_CONTROL_ROBOT;

//������������
typedef struct
{
	unsigned char stat;   // 0 ���Ǵ�ֱ 1 ����ˮƽ
	
}HMI_485_SET_CHAMFER_FLAG;

//������������
typedef struct
{
	unsigned char stat;   // 0 ���߹ر� 1 ���ߴ�
	
}HMI_485_SET_FIXTURE_FLAG;

//��е�ֿ����������������ݽṹ
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

//��е�ֿ����������������ݽṹ
typedef enum
{
	HMI_485_SYS_IDLE_TYPE,               //�����״̬��ȡ���
	HMI_485_SYS_RESET_TYPE,              //��������λ����
	HMI_485_SYS_ABORT_TYPE,              //����������ֹͣ����
	HMI_485_SET_MODE_TYPE,               //������ģʽ��������
	HMI_485_SET_HOME_TYPE,               //��������������
	HMI_485_AXIS_POS_MOVE_TYPE,          //�������ƶ�����
	HMI_485_AXIS_NEG_MOVE_TYPE,          //�Ḻ���ƶ�����
	HMI_485_INTERP_RUN_TYPE,             //�������Զ���������
	HMI_485_INTERP_PAUSE_TYPE,           //��������ͣ��������
	HMI_485_INTERP_RESUME_TYPE,          //������������������
	HMI_485_INTERP_STEP_TYPE,            //������������������
	HMI_485_SET_MOULD_NUM_TYPE,          //����������ģ������
	HMI_485_CONTROL_ROBOT_TYPE,          //ע�ܻ����ƻ�е������
	HMI_485_SET_CHAMFER_FLAG_TYPE,       //���������ǲ���ָ��
	HMI_485_SET_FIXTURE_FLAG_TYPE,       //���������߲���ָ��

}HMI_485_CMD_TYPE;

//��е�ֿ������������ݽṹ
typedef struct
{
	unsigned int  hmiCmdType;
	HMI_485_CMD_DATA hmiCmdData;

}HMI_485_CMD;

/**********************************************************************************/
/**************************    �ļ������״̬����      ****************************/
/**********************************************************************************/

//��󪬺A�ƾڵ��c
typedef struct
{
	unsigned short echoMouldNum;   //��Ӧģ�ߺ�
	char programFileName[64];      //ģ�ߺ�Ӧ��������
	char cReserved[2];

}FILE_NAME_FOR_485;

//ģ�ߺ�����
typedef struct
{
	unsigned short hmiMouldNum;   //ģ�ߺ�

}HMI_485_FILE_ECHO_MOULD_NUM;

//�ļ�����������
typedef union
{
	//��ö�Ӧģ�ߺŻ�Ӧֵ���ļ�����������
	HMI_485_FILE_ECHO_MOULD_NUM       fileEchoMouldNum;

}HMI_485_FILE_CMD_DATA;

//�ļ�����������
typedef enum
{
	HMI_485_FILE_MODULE_NAME_TYPE    //��ö�Ӧģ�ߺŻ�Ӧֵ���ļ���

}HMI_485_FILE_CMD_TYPE;

//�ļ����������ݽṹ
typedef struct
{
	unsigned int  hmiCmdType;            //��������
	HMI_485_FILE_CMD_DATA hmiCmdData;    //��������

}HMI_485_FILE_CMD;

/********* �������߼��ã�����һ���ṹ��HMI_485_CMD ************/
//  ��������
HMI_485_CMD  hmiSysReset(char state);    //����
HMI_485_CMD  hmiSysAbort(char state);    //ֹͣ
HMI_485_CMD  hmiSetMode(int mode);       //����ģʽ 1���� 2�ֶ� 3�Զ�
HMI_485_CMD  hmiSetHome(int axisNum);    //����
HMI_485_CMD  hmiAxisPosMove(int axisNum, int vel, int pos);//�������ƶ�
HMI_485_CMD  hmiAxisNegMove(int axisNum, int vel, int pos);//�Ḻ���ƶ�
HMI_485_CMD  hmiInterpRun(int line);     //�Զ�����
HMI_485_CMD  hmiInterpPause(char state); //��ͣ
HMI_485_CMD  hmiInterpResume(char state);//����
HMI_485_CMD  hmiInterpStep(char state);  //����
HMI_485_CMD  hmiSetMouldNum(short mouldNum);//����
HMI_485_CMD  hmiControlRobot(char state);//����Ȩ 0��е�� 1�ܻ�
HMI_485_CMD  hmiSetChamferFlag(char state);//���� 0��ֱ  1ˮƽ
HMI_485_CMD  hmiSetFixtureFlag(char state);//���� 0�ر�  1��

//  �ļ�����
HMI_485_FILE_CMD 	hmiFileCmd(short hmiMouldNum);

#endif /* HMICMD_H_ */
