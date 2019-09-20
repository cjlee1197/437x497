
#ifndef	_QUALITY_CONTROL
#define	_QUALITY_CONTROL
#include	"tmconfig.h"
#include	"tmcontrol.h"
#include	"curve.h"
#include	"storage.h"
#include	"oil54struct.h"


typedef	struct	tagtmMoniCurve
{
	double dX;
	double dY;
}tmMoniCurve;
/** @defgroup group1	品質類
 *  現有兩種類型:油壓 全電
 *  @{
 */
/** @brief 品質類基類,是純虛類 CtmQuality in group 1 */
class	CtmQuality
{
	public:
		virtual	int			GetMoniCount(int nDataType = -1)						 = 0;
		virtual	double 		SetData(int nDataType, int nFunctionType)				 = 0;
		virtual	void	 	Clear() 												 = 0;
		virtual	int			GetData(int nDataType, int nFunctionType, double* pData);
		virtual	double 		GetDataS(int nDataType, int nFuctionType, int nIndex);
	protected:
		virtual	void		GetDataBasicFunction(int nFunctionType, void* pMachineDataType, 
											 	double* pSingleOutData) 			= 0;
		virtual	void*		GetMachineDataType(int nDataType, int nIndex) 			= 0;
};


class CtmOilQuality: public CtmQuality
{
	public:
		CtmOilQuality();
		virtual	~CtmOilQuality();
	public:
		virtual int			GetMoniCount(int nDataType = -1)							;
		virtual double		SetData(int nDataType, int nFunctionType) 					;
		virtual void		Clear()														;
	protected:
		virtual void		GetDataBasicFunction(int nFunctionType, void* pMachineDataType, 
									double* pSingleOutData);
		virtual void*		GetMachineDataType(int nDataType, int nIndex);
};
	
class	CtmSimQuality: public CtmQuality
{
	public:
		CtmSimQuality();
		virtual	~CtmSimQuality();
	public:
		int			GetMoniCount(int nDataType = -1) {return 0;};      
		double 		SetData(int nDataType, int nFunctionType) {return 0;};     
		void	 	Clear() {return;};
	protected:
		void	GetDataBasicFunction(int nFunctionType, void* pMachineDataType, 
									 double* pSingleOutData){};
		void*	GetMachineDataType(int nDataType, int nIndex){return NULL;};          
		
};
/** @} */ // end of group1


/** @defgroup group2	對外品質類
 *  對外的只有這一種
 *  @{
 */
 
/** @brief 類 CtmQuality in group 2 */
class	CtmQualityControl
{
	public:
		static	CtmQualityControl*	GetInstance();
		static	void		ReleaseInstance();
	protected:
		virtual ~CtmQualityControl();
		CtmQualityControl();
	public:
		int			GetDataCurve(int nDataType, int nFunctionType, void* pData);		
		int			GetMoniCount(int nDataType = -1)						;
		int			GetData(int nDataType, int nFunctionType, double* pData);
		double 		GetDataS(int nDataType, int nFuctionType, int nIndex);
		double 		SetData(int nDataType, int nFunctionType);		
		char* 		GetYName();
		char*		GetXName();		
		void	 	Clear();
		BOOL		SaveAs(tmStorageType StorageType);
		BOOL		ReadAs(tmStorageType StorageTypeSrc, tmStorageType StorageTypeDst =STORAGE_TYPE_SRAM);
	private:
		static	CtmQualityControl*	m_pInstance;
		static	CtmQuality*			m_pQuality;
};
/** @} */ // end of group2

#endif
