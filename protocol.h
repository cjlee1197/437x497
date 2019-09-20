#ifndef 	__D_CTMPROTOCOL
#define		__D_CTMPROTOCOL
#include	"common.h"
#include	"database.h"


//	各種數據的協議祖類
class	CtmProtocol
{
	public:
		CtmProtocol();
		virtual	~CtmProtocol();
	public:
		virtual	int	ComposeReadProtocal(void* pOutData, void* pInData,  int nInputLen, void* pEx);
		virtual	int	ComposeWriteProtocal(void* pOutData, void* pInData,  int nInputLen, void* pEx);
		virtual	int	ParseProtocol(void* pInData, int nInputLen);
		
		virtual	int	ComposeReadData(void* pOutdata, int nElements, int pIndexs[], void* pEx);
		virtual	int ComposeWriteData(void* pOutdata, int nElements, int pIndexs[], void* pEx);
		
};

class	CtmProtocol_Dsp28Data: public CtmProtocol
{
	public:
		CtmProtocol_Dsp28Data();
		~CtmProtocol_Dsp28Data();
	public:
		int		ComposeReadProtocal(void* pOutData, void* pInData,  int nInputLen, void* pEx);
		int		ComposeWriteProtocal(void* pOutData, void* pInData,  int nInputLen, void* pEx);
		
		
		int		ComposeReadData(void* pOutdata, int nElements, int pIndexs[], void* pEx);
		int 	ComposeWriteData(void* pOutdata, int nElements, int pIndexs[], void* pEx);
		
		int		ParseProtocol(void* pInData, int nInputLen);
		
		
		void	SetReadType(int nType);
		void	SetWriteType(int nType);
	protected:
		virtual	int	ComposeReqHead(int nType, int nLen, int* pPackageIndex, void* pOutData, void* pEx);
		virtual	int	ComposeReqFoot(int nType, int nLen, int* pPackageIndex, void* pOutData, void* pEx);
		
		//virtual	int GetReadData(int nElements, int pIndex[], void* pOutData, void* pEx);
		
		virtual	int	SetDataToDataBase(void* pData, int nCount, BOOL bSave = FALSE);
		
		
		
	protected:
		int	m_nReadReqPackage;
		int	m_nWriteReqPackage;
		int	m_nReadReqType;
		int	m_nWriteReqType;
	private:
		
};


class	CtmProtocol_Dsp28Data_Actual: public CtmProtocol_Dsp28Data
{
	public:
		CtmProtocol_Dsp28Data_Actual();
		~CtmProtocol_Dsp28Data_Actual();
	protected:
		int	SetDataToDataBase(void* pData, int nCount, BOOL bSave = FALSE);

};

class	CtmProtocol_Dsp28Data_OnLine: public CtmProtocol_Dsp28Data
{
	public:
		CtmProtocol_Dsp28Data_OnLine();
		~CtmProtocol_Dsp28Data_OnLine();
	protected:
		int		ComposeReadData(void* pOutdata, int nElements, int pIndexs[], void* pEx);

};

typedef	CtmProtocol_Dsp28Data_OnLine CtmProtocol_Dsp28Data_DataStatus;



class	CtmProtocol_Dsp28Data_Motor: public CtmProtocol_Dsp28Data
{
	public:
		CtmProtocol_Dsp28Data_Motor();
		~CtmProtocol_Dsp28Data_Motor();
	public:
		int	ParseProtocol(void* pInData, int nInputLen);				//	virtual	function
	protected:
		int	ComposeReqHead(int nType, int nLen, int* pPackageIndex, void* pOutData, void* pEx);
		int	SetDataToDataBase(void* pData, int nCount, BOOL bSave = FALSE);
		//int	ParseProtocol(void* pInData, void* pOutData, int nInputLen);				//	virtual	function
};


typedef	CtmProtocol_Dsp28Data_Motor CtmProtocol_Dsp28Data_Mld;

class	CtmProtocol_Dsp28Data_Alarm: public CtmProtocol_Dsp28Data
{
	public:
		CtmProtocol_Dsp28Data_Alarm();
		~CtmProtocol_Dsp28Data_Alarm();
	public:
		int	ParseProtocol(void* pInData, int nInputLen);
	private:
		int		m_nErrorID;
		int 	m_nErrorBegin;
		int		m_nErrorStrBeginIndex;
		
};



//	主機協議的祖類
//	從本質來講，會使用數據的協議
class	CtmHostProtocol
{
	public:
		CtmHostProtocol();
		virtual	~CtmHostProtocol();
	public:
		virtual	int	ComposeProtocol(void* pOutData, int nType, int nElements, int pIndexs[], void* pEx);	//	virtual	function		
		virtual	int	ParseProtocol(void* pInData, int nLen);		//	virtual	function
		virtual	int	GetDataType(void* pInData, int nLen);
};

class	CtmDsp28Protocol: public CtmHostProtocol
{
	public:
		CtmDsp28Protocol();
		~CtmDsp28Protocol();
	public:
		
		int	ComposeProtocol(void* pOutData, int nType, int nElements, int pIndexs[], void* pEx);	//	virtual	function		
		int	ParseProtocol(void* pInData, int nLen);									//	virtual	function
		int	GetDataType(void* pInData, int nLen);
	private:
		CtmProtocol_Dsp28Data*	m_pRealProtocol;
		CtmProtocol_Dsp28Data*	m_pMotorProtocol;
		CtmProtocol_Dsp28Data*	m_pAlarmProtocol;
		CtmProtocol_Dsp28Data*	m_pCurveProtocol;
		CtmProtocol_Dsp28Data*	m_pOnLineProtocol;
		CtmProtocol_Dsp28Data*	m_pDataStatusProtocol;
		CtmProtocol_Dsp28Data*	m_pMldProtocol;
};

#endif
