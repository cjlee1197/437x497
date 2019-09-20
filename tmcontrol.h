/*===========================================================================+
|  Function : TmControl                                                      |
|  Task     : Machine Control Utilities header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 05/09/2008                                                     |
|  Revision : 05/09/2008                                                     |
+===========================================================================*/

#ifndef     D_TMCONTROL
#define     D_TMCONTROL

#include	"main.h"
#include	"tmpackclass.h"
#include	"wnd.h"
#include	"taskhost.h"
#include	"oil54struct.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - CtmControl		                             |
|Task: 此類提供最基礎的control操作接口										 |
+===========================================================================*/
class	CtmControl: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmControl)
	
	public:
		CtmControl();
		virtual ~CtmControl();
		
		virtual BOOL	AddControl(void *pParas[])						{return FALSE;}
		virtual BOOL	ModifyControl(void *pParas[]) 					{return FALSE;}
		virtual BOOL	FindControl(void *pParas[], void *pFindPara)	{return FALSE;}
		virtual BOOL	DeleteControl(void *pDeletePara = NULL)			{return FALSE;}
		virtual BOOL	FixControl(void *pFixPara)						{return FALSE;}
		virtual void	InitControl()									{}
	public:
		//2019/7/4 下午 02:31:35 Mario Add Oper Log (CtmControl_Oper_Log)
		virtual	BOOL    		AddOperLog(DWORD DBIndex , DWORD OldValue, DWORD NewValue){return FALSE;}
		virtual	BOOL    		ClearOperLog(){return FALSE;}
		virtual	void*				GetOperLog(int nIndex){return NULL;}
		virtual	WORD     		GetOperLogCount(){return 0;}
		virtual	void				Init_OperLog(){}		
		
		
		virtual	BOOL    Robot_AddErrorLog(WORD wID , WORD wGroup)							{return FALSE;}
		virtual	BOOL    AddErrorLog(DWORD dwID, WORD	wSource = tmOIL_ERRORFLAG)		{return FALSE;}
		virtual	BOOL	AddMoni2()										{return FALSE;}
		virtual	BOOL    AddUser(char* pszID, char* pszPassword, DWORD dwPrivilege) {return FALSE;}
		virtual	BOOL    ClearErrorLog()									{return FALSE;}
		virtual	BOOL    ClearMoniLog()									{return FALSE;}
		virtual	void	Destroy_ErrorLog()								{}
		virtual	void	Destroy_MoniLog()								{}
		virtual	BOOL    DeleteUser(char* pszID)							{return FALSE;}
		virtual	void    Error_Control()									{}
		virtual	short   FindFreeUser()									{return 0;}
		virtual	short	FindUser(char* pszID)							{return 0;}
		virtual	BOOL    FixErrorLog(DWORD dwID, WORD	wSource = tmOIL_ERRORFLAG)	{return 0;}
		virtual	int		ParseErrorID(char* pszErrorStr, DWORD dwErrorID){return 0;}	//Caozz 2011/4/14 上午 11:08:53
		virtual	void /*tmDB_ERROR*/*	GetErrorIndex()								{return NULL;}
		virtual	void /*tmDB_ERROR*/*	GetErrorLog(int nIndex)						{return NULL;}
		virtual	WORD        GetErrorLogCount()							{return 0;}
		virtual	void /*tmDB_MONI2*/*	GetMoniIndex()								{return NULL;}
		virtual	void /*tmDB_MONI2*/*	GetMoniLog(int nIndex)						{return NULL;}
		virtual	WORD        GetMoniLogCount()							{return 0;}
		virtual	void		Init_ErrorLog()								{}
		virtual	void		BackupErrorLog()							{}
		virtual	void		RevertErrorLog()							{}
		virtual	void		Init_MoniLog()								{}
		virtual	void		InitCurrentUser()							{}
		virtual	WORD        LoginLocal(char* pszID, char* pszPassword)	{return 0;}
		virtual	WORD        GetPrivilefe(char* pszID)	{return 0;}
		virtual	BOOL        ModifyUser(char* pszID, char* pszPassword, DWORD dwPrivilege){return FALSE;}
		virtual	BOOL		SaveCurrentUser(char* pszName, WORD wPrivilege)	{return FALSE;}
		virtual	BOOL		SaveMoni2(void*/*I32_MONI2**/ pI32Moni2)				{return FALSE;}
		virtual	WORD        Encode(char* psz)							{return 0;};
		virtual void   	IniUser() {return 0;};
		virtual WORD   	GetUserNum() {return 0;};
	public:
		//quality and moni control
		virtual int		SetData(int nType, double* dX, double* dY, int nNum)	    {return 0;}
		virtual int		SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)		    {return 0;}
		virtual int		SetData(int	nType, void* pInputData, int nNum)			    {return 0;}
		virtual int		GetData(int nType, void* pOutData,int TempDataIndex = 0)	{return 0;}
		virtual void	SetComplete(int nType)										{}
		virtual int		SetState(int nType, int nState)								{return 0;}
		virtual int		GetState(int nType)											{return 0;}
		virtual void    ClearBuffer(int nType = -1)                                 {}
		
		virtual  int		GetData2(int 	nType, void* pOutData,int TempDataIndex = 0)	{return 0;}
		virtual  int		SetData2(int	nType, void* pInputData, int nNum)				{return 0;}
		virtual  void		SetComplete2(int nType)											{}
		virtual  int		SetState2(int 	nType, int nState)								{return 0;}
		virtual  int		GetState2(int 	nType)											{return 0;}
		virtual  void    	ClearBuffer2(int nType = -1)                                 	{}
		
		
		virtual	int			AddPoint(int nType, void* pData, int nCurveNum){ return 0;}
		virtual int			GetCurveState(int nType) {return 0;}
		virtual int			GetCurveData(int nType, void *pData, int nPoint, int TempDataIndex = 0) {return 0;}
		virtual	int			GetCurveLastIndex(int nType) {return 0;}
		virtual BOOL		CurveUpdate(int nType) {return FALSE;}
		virtual void		Clear(int nType) {}
		
		virtual int			GetMoniCount(int nDataType = -1)						{return 0;}
		virtual double		SetData(int nDataType, int nFunctionType) 				{return 0;}
		virtual void		Clear()													{}
		virtual void		GetDataBasicFunction(int nFunctionType, void* pMachineDataType, 
									double* pSingleOutData)							{}
		virtual void*		GetMachineDataType(int nDataType, int nIndex)			{return NULL;}
		
		virtual int			GetDataCurve(int nDataType, int nFunctionType, void* pData)	{return 0;}
		virtual BOOL		SaveAs(tmStorageType StorageType)							{return FALSE;}
		virtual BOOL		ReadAs(tmStorageType StorageTypeSrc, tmStorageType StorageTypeDst =STORAGE_TYPE_SRAM) {return FALSE;}
	public:
		//mold control
		virtual BOOL	Save(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	{return FALSE;}
		virtual BOOL	Save(int nIndex,tmStorageType StorageType=STORAGE_TYPE_SRAM)				{return FALSE;}
		virtual int		Find(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	{return 0;}
		virtual BOOL	Read(int nIndex, tmStorageType StorageType = STORAGE_TYPE_SRAM)				{return FALSE;}
		virtual BOOL	Read(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	{return FALSE;}
		virtual BOOL	Delete(DB_MOLDHDR MoldSetHead, tmStorageType StorageType=STORAGE_TYPE_SRAM)	{return FALSE;}
		virtual BOOL	Delete(int nIndex,tmStorageType StorageType=STORAGE_TYPE_SRAM)				{return FALSE;}
        virtual BOOL    SaveToCurrentMold(DB_MOLDHDR MoldSetHead, tmStorageType StorageType = STORAGE_TYPE_SRAM)	{return FALSE;}
        virtual BOOL    SaveToCurrentMold()															{return FALSE;}
        #ifdef	D_NOWRITE_FALSH
        virtual BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType = STORAGE_TYPE_FLASHEX)		{return FALSE;}
        #else 
        	#ifdef D_X86
        virtual BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType = STORAGE_TYPE_FLASH)		{return FALSE;}
        	#else 
        virtual BOOL	SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType = STORAGE_TYPE_SRAM)			{return FALSE;}
        	#endif
        #endif
        virtual char*	GetCurrentMold()															{return NULL;}
		virtual DB_MOLDHDR*	GetMoldSetHead()														{return NULL;}
		virtual DB_MOLDHDR*	GetMoldSetHead(int nStart, int Num, tmStorageType StorageType = STORAGE_TYPE_SRAM)	{return NULL;}
        virtual DB_MOLDHDR* GetMoldSetHeadAsynch()																{return NULL;}
		virtual BYTE*	GetMoldSetContent()																		{return NULL;}
		virtual int		GetMaxMoldSetNum()																		{return 0;}
		virtual int		GetMoldSetNum(tmStorageType	StorageType = STORAGE_TYPE_SRAM)							{return 0;}
		virtual int		GetMoldSize()																			{return 0;}
		virtual	int		SendControlMoldSet()														{return 0;}
		
		virtual	char*	GetMoldSetPath()															{return NULL;}
		virtual	char*	SetMoldSetPath(char*	pszPath)											{return NULL;}
		
		virtual void SetBlockIndex(int nBlockIndex)													{}
   	 	virtual BOOL Delete(tmStorageType StorageType)												{return FALSE;}
    	virtual BOOL Read(tmStorageType StorageType)												{return FALSE;}
		virtual BOOL SaveAs(tmStorageType SourceStorage, tmStorageType DestStorage)					{return FALSE;}
    	//virtual BOOL SaveAs(tmStorageType StorageType)											{return FALSE;}
		virtual int	 GetSize(int nBlockIndex)														{return 0;}
		virtual int	 GetSize(tmFileIDType FileID)													{return 0;}
	public:
		friend  void		OnTimeHeater();
		friend  void		CountZero();
		friend  void		ClearZeroOil();
		
		friend  void    	GetInitPasswd_Install(char* pcMachineID, char* pcPassword, 
											tmDATE dtMachineDateOut);
		friend  int       	GetStage_Install(char* pcPassword, char* pcMachineID, 
											tmDATE dtStartStage);
		friend  BOOL        CheckTimeOut_Install();
		friend  WORD        StartUp_Install(char* pcPassword);
		friend  WORD		JudgeFactoryOnWork_Install();
		friend  void		SaveOperation();
		friend  void		DealOperation(OPERATION* pOperation, int nWRflag);
		friend  int			ReadOperationFromSRAM(OPERATION* pOperation);
		friend  int			WriteOperationToSRAM(OPERATION* pOperation);
		friend  void		DealOperationStartMachine();
		friend  WORD		CountOperationCRC();
		friend  int			GetBlockValueState(int nBlockID, tmStorageType StorageType);
		friend  WORD		VER2HEX();
		
		friend  void        SetCoreEffectPN(BOOL bSendDSP);
		friend  void        SetEjectEffectPN(BOOL bSendDSP);
		friend  int         GetMoniStructSize();
		//Function Control
		virtual	void		_OnTimeHeater()															{}
		virtual	void		_CountZero()															{}
		virtual	void		_ClearZeroOil()															{}
		
		virtual	void    	_GetInitPasswd_Install(char* pcMachineID, char* pcPassword, 
											tmDATE dtMachineDateOut)								{}
		virtual	int       	_GetStage_Install(char* pcPassword, char* pcMachineID, 
											tmDATE dtStartStage)									{return 0;}
		virtual	BOOL        _CheckTimeOut_Install()													{return FALSE;}
		virtual WORD        _StartUp_Install(char* pcPassword)										{return 0;}
		virtual	WORD		_JudgeFactoryOnWork_Install()											{return 0;}
		virtual	void		_SaveOperation()														{}
		virtual	void		_DealOperation(OPERATION* pOperation, int nWRflag)						{}
		virtual	int			_ReadOperationFromSRAM(OPERATION* pOperation)							{return 0;}
		virtual	int			_WriteOperationToSRAM(OPERATION* pOperation)							{return 0;}
		virtual	void		_DealOperationStartMachine()											{}
		virtual	WORD		_CountOperationCRC()													{return 0;}
		virtual	int			_GetBlockValueState(int nBlockID, tmStorageType StorageType)			{return 0;}
		virtual	WORD		_VER2HEX()																{return 0;}
		
		virtual	void        _SetCoreEffectPN(BOOL bSendDSP)											{}
		virtual	void        _SetEjectEffectPN(BOOL bSendDSP)										{}
		virtual	int			_GetMoniStructSize()															{return 0;}
	
	public:
		virtual	void	Craft_SetTable(void* tmTest, WORD	wCount)	{}
		virtual	BOOL	Craft_WriteData(char* cszpName[], int nCount, char* cszDataPath) {return FALSE;}
		virtual	BOOL	Craft_ReadData(char* cszDataPath)	{return FALSE;}
		virtual BOOL	Craft_SetPro(WORD wDataSet, WORD wSetNum = 1) {return FALSE;}
		virtual BOOL	Craft_ReadHead(char* cszFileName, void* tmCraHeadRead) {return FALSE;}
		virtual	BOOL	Craft_SetDirPath(const char* cszDirPath) {return FALSE;}
};

/*===========================================================================+
|           Class declaration - CtmControlServer		                     |
|Task: 此類提供服務集接口													 |
+===========================================================================*/
class 	CtmControlServer: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmControlServer)
	
	public:
		CtmControlServer();
		~CtmControlServer();
	protected:
		enum TmControlIndex
		{
			ERRORINDEX = 0,
			MONIINDEX,
			MOLDINDEX,
			QUALITYINDEX,
			USERINDEX,
			CURVEDATA,
			FUNCTION,
			OPERLOG,
			OTHER,
			CURVE2DATA
		};
	public:
		virtual CtmControl*	ErrorControl()		{ return m_pControl[ERRORINDEX]; };
		virtual	CtmControl*	MoniControl()		{ return m_pControl[MONIINDEX]; };
		virtual	CtmControl* MoldControl()		{ return m_pControl[MOLDINDEX]; };
		virtual CtmControl*	QualityControl()	{ return m_pControl[QUALITYINDEX]; };
		virtual	CtmControl*	UserControl()		{ return m_pControl[USERINDEX]; };
		virtual CtmControl* CurveDataControl()  { return m_pControl[CURVEDATA];};
		virtual CtmControl* Curve2DataControl()  { return m_pControl[CURVE2DATA];};
		virtual CtmControl* Oper_Log()   { return m_pControl[OPERLOG];};
		virtual CtmControl* FunctionControl()   { return m_pControl[FUNCTION];};
		virtual CtmControl* StdCraftControl()   { return NULL;};
		virtual	CtmControl*	OtherControl(int nIndex)		{ return m_pControl[nIndex]; };
		
	protected:
		virtual	void	CreateControl();
		virtual	void	DestroyControl();
	protected:
		CtmControl*		m_pControl[CNT_INT_MOTION_CONTROL_NUM];
		
		
};
void		OnTimeHeater();
void		CountZero();
void		ClearZeroOil();
void    	GetInitPasswd_Install(char* pcMachineID, char* pcPassword, 
											tmDATE dtMachineDateOut);
int       	GetStage_Install(char* pcPassword, char* pcMachineID, 
											tmDATE dtStartStage);
BOOL        CheckTimeOut_Install();
WORD        StartUp_Install(char* pcPassword);
WORD		JudgeFactoryOnWork_Install();
void		SaveOperation();
void		DealOperation(OPERATION* pOperation, int nWRflag);
int			ReadOperationFromSRAM(OPERATION* pOperation);
int			WriteOperationToSRAM(OPERATION* pOperation);
void		DealOperationStartMachine();
WORD		CountOperationCRC();
int			GetBlockValueState(int nBlockID, tmStorageType StorageType);
WORD		VER2HEX();
void        SetCoreEffectPN(BOOL bSendDSP);
void        SetEjectEffectPN(BOOL bSendDSP);
int			GetMoniStructSize();
extern 	CtmControlServer*	g_ptmControlServer;
#endif
