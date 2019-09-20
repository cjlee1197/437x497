/*Created by J.Wong 2016/11/22 12:43:18*/
#pragma once
#include    "task.h"
#include	"tmpackclass.h"
#include 	"xml-tools.h"
#include 	<map>
#include 	<string>
using namespace __gnu_cxx;

struct str_hash{      //hash函数
    size_t operator()(const string& str) const
    {
        unsigned long __h = 0;
        for (size_t i = 0 ; i < str.size() ; i ++)
        {
            __h = 107*__h + str[i];
        }
        return size_t(__h);
    }
};

struct str_equal{      //string 判断相等函数
	bool operator()(const string& s1,const string& s2) const
	{
     	return s1==s2;
	}
};

/*register id: SDO index(2bytes)+subindex(1byte)+len(1byte,bits-count)*/
/* (id-5w+2000H)<<16+(00(subindex)<<8)+len*/

typedef  long long INT64;
typedef struct tagAxisItem{
	DWORD id;
	
	DWORD dataType :8;
	DWORD prec	:4;
	DWORD rw	:2;
	DWORD dirty :1;
		
	INT64	maxValue;
	INT64	minValue;	  
	union{
		int lValue;
		float fValue;
	};
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	AxisItem;
#else 
#ifdef	D_X86
AxisItem;
#endif
#endif

/*
	射出1
	射出2
	储料1
	储料2
	模座1
	模座2
	脱模1
	脱模2
	座台1
	座台2
*/

#define  AXISIDTOID28(ID,BYTES) \
		 ((((ID)-50000)+0x2000)<<16)+(sizeof((BYTES))*8)
		 
#define  ID28TOAXISID(ID) \
		 ((((ID)>>16)-0x2000)+50000)	 

/*
*	CHANNEL: CAN 通道
*   NODEID: 从站节点ID
*   TYPE:  读写类型 1--ETHERCAT 读写 2-- CAN读写 89--28内部诊断	
*/

#define 	COMPOSEWATCHTYPE(CHANNEL,NODEID,TYPE) \
			(((CHANNEL)<<16)|((NODEID)<<8)|(TYPE))
			
			
class CtmAxisPrivateData;
		
class CTaskAxis:public CTask,public CtmPackClass
{
	DECLARE_DYNCREATE(CTaskAxis)
	public:
		enum AXIS{
			AXIS_MOLD1_DRV1,
			AXIS_MOLD1_DRV2,
			AXIS_EJE1_DRV1,
			AXIS_EJE1_DRV2,
			AXIS_INJE1_DRV1,
			AXIS_INJE1_DRV2,
			AXIS_CHRG1_DRV1,
			AXIS_CHRG1_DRV2,
			AXIS_NOZL1_DRV1,
			AXIS_NOZL1_DRV2,
			AXIS_NOZL2_DRV1,
			AXIS_NOZL2_DRV2,
			AXIS_EJE2_DRV1,
			AXIS_EJE2_DRV2,
			AXIS_INJE2_DRV1,
			AXIS_INJE2_DRV2,
			AXIS_CHRG2_DRV1,
			AXIS_CHRG2_DRV2,
			AXIS_MOLD2_DRV1,
			AXIS_MOLD2_DRV2,
			AXIS_ALL
		};
		CTaskAxis();
		~CTaskAxis();
		
		int			CreateSelf();
		int			FreeSelf();
		
		virtual	int			GetAxisValue(int id)const ;
		virtual float		GetAxisValueFloat(int id)const;
		virtual AxisItem	GetAxisItem(int id)const;
		
		virtual int			SetAxisValue(int id, int value, int dirty=true);
		virtual int 		SetAxisValue(int id, float value, int dirty= true);
		virtual int 		GetAxisDataType(int id)const;
		virtual	int			SetDirty(int id, int dirty=true);
		/*手动读取一次所有资料*/
		virtual int			ReadAxisSync();
		/*手动下一次可写资料*/
		virtual int			DownloadAxisSync();
		virtual int 		DownloadDirtyData();
		virtual bool		CheckWriteRegister(DWORD reg);
		virtual bool		CheckRegister(DWORD reg);
		virtual	bool		CheckStateRegister(DWORD reg);
		virtual void		SetResponeState(int state);
		virtual int			GetResponeState()const;
		virtual int			Import(char* pszFileName, CTaskAxis::AXIS axis,bool dirty=false);
		virtual int 		Export(char* pszFileName, CTaskAxis::AXIS axis);
			
		static 		CTaskAxis* 		GetInstance(char* cls);
		static		int				CreateInstance(char* cls, char* lib);
		
	protected:
		int			Lock();
		int 		UnLock();
		CtmAxisPrivateData*	m_data;
};

typedef DWORD	REG;
typedef DWORD	ID;

class CtmAxisPrivateData{
	public:
		pthread_mutex_t 	m_lock;
		map<ID,AxisItem>	m_item_write[CTaskAxis::AXIS_ALL];
		map<ID,AxisItem>	m_item_read[CTaskAxis::AXIS_ALL];
		CXmlNode			m_root;
		CTaskAxis::AXIS		m_axis;
		int					m_rsp;
	public:
		CtmAxisPrivateData():m_rsp(0),m_axis(CTaskAxis::AXIS_ALL){}
		~CtmAxisPrivateData(){}
		
			
};