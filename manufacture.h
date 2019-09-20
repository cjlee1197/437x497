/*===========================================================================+
|  Function : iNet                                                           |
|  Task     : Produce manage service routine header file			         |
|----------------------------------------------------------------------------|
|  Compile  : 			                                                     |
|  Link     : 			                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   :                                     		                     |
|  Version  : V1.00                                                          |
|  Creation : 03/04/1997                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef	D_MANUFACTURE
#define	D_MANUFACTURE

#include 	"common.h"
#include	"main.h"
#include	"sram.h"
#include	"storage.h"

// Plan Status
#define		BILLSTAT_FINISHED				1
#define     BILLSTAT_NOCONFIRM          	2
#define     BILLSTAT_COMMAND            	3
#define     BILLSTAT_INVALID            	5
#define		BILLSTAT_WAIT					6
#define		BILLSTAT_PAUSE					7
#define		BILLSTAT_PRODUCTING				8
// Produce Status
#define		MANUFACTSTAT_NOBILL				0
#define		MANUFACTSTAT_PRODUCT			1
#define		MANUFACTSTAT_AMENDMOLD			2
#define		MANUFACTSTAT_ADJUSTCOLOR		3
#define		MANUFACTSTAT_ADJUSTMOLD			4
#define		MANUFACTSTAT_TRANSMATERIAL		5
#define		MANUFACTSTAT_WAITMATERIAL		6
#define		MANUFACTSTAT_WAITMOLD			7
#define		MANUFACTSTAT_BADMACHINE			8
#define		MANUFACTSTAT_ADJUSTMACH			9
#define		MANUFACTSTAT_HALTMACHINE		10
#define		MANUFACTSTAT_OTHERS				11

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
/*===========================================================================+
|           Class declaration - TaskINet                                     |
+===========================================================================*/
class	CtmManufacturePlan: public CTask
{
public:
	static	CtmManufacturePlan*		GetInstance();
	static	void					ReleaseInstance();

private:
	CtmManufacturePlan();
	virtual ~CtmManufacturePlan();
	
public:
	
	void	WndProc(int message, WPARAM wParam, LPARAM lParam);
	
	BOOL	Save(DB_PRODMANAGE* pdbplan, tmStorageType StorageType = STORAGE_TYPE_SRAM);
	BOOL	Delete(char* pszNum, tmStorageType	StorageType = STORAGE_TYPE_SRAM);
	char*	ReadPlan(int nIndex, tmStorageType StorageType = STORAGE_TYPE_SRAM);
	char*	ReadPlan(char* pszNum, tmStorageType StorageType = STORAGE_TYPE_SRAM);
	
	char*	GetCurrentPlan();
	BOOL	SaveCurrentPlan();
	char*	GetPlanInfo();
	BOOL	SavePlanInfo();
	BOOL	SetCurrentPlan(char* pszNum, tmStorageType	StorageType = STORAGE_TYPE_SRAM);
	BOOL	ResetPlanStorage();
	
	int		GetMaxNum();
	int		GetPlanSize();
	int		GetPlanTotalSize();
	
	//	fans add
	int		GetRealIndex(int nLogicIndex);
	int		Find(char* pszNum, tmStorageType StorageType = STORAGE_TYPE_SRAM);
protected:
	int		FindFree();
	BOOL	Delete(int nIndex);
	BOOL	Read(int nIndex, tmStorageType StorageType = STORAGE_TYPE_SRAM);
	
	BOOL	Read(char* pszNum, tmStorageType StorageType = STORAGE_TYPE_SRAM);
	BOOL	Save(DB_PRODMANAGE* pdbplan, int nIndex, tmStorageType StorageType = STORAGE_TYPE_SRAM);
	
private:
	int		CalculatePlanSize();
	
	void	SetProduceStatus(WORD wMachineMode);
	void	SetScheduleStatus(int nIndex, int nStatus);
	void	SetProductCount(int nBadProduct);
	
	void	RecordSort();
	DWORD	DateTimeToValue(DATE date, tmTIME time);
protected:
	CtmSarmRecordFileControl*	m_pPlanFile;
	DB_PRODMANAGE				m_manufactureplancur;
	DB_PRODMANAGE				m_manufactureplanHeadInfo;
	DB_PRODMANAGE				m_manufactureplanTemp;
	int							m_nMaxPlanCount;
	int							m_nPlanSize;
	
private:
	static	CtmManufacturePlan*	m_pInstance;
	BOOL	m_bSortFlag;
};

void    SetPlanScheduleStatus(TNET_PLANSTATUS* pScheduleStatus, DB_PRODMANAGE* pProdManage);

#endif
