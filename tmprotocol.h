#ifndef 	__D__CTMPROTOCOL
#define		__D__CTMPROTOCOL
#include	"tmpackclass.h"
#include	"taskcomm.h"

/*!	\class CtmPartProtocol 
*	\brief 數據非內容部分的協議
*	\author fans.
*	\note	
*	
*/
class	CtmPartProtocol: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmPartProtocol)
	
	public:
		CtmPartProtocol();
		virtual	~CtmPartProtocol();
	public:
		virtual	int		SetPartValue(int nPos, void* pValue, int nSize);
		virtual	int		GetPartValue(int nPos, void* pValue, int nSize);
		virtual	void*	GetBuffer();
		//virtual	BOOL	CheckStand(void* pBuffer);
		
		virtual	int		GetSize();
		virtual	void	SetSize(int nSize);
		virtual	int		GetMaxSize();
		virtual	void	SetMaxSize(int nSize);
		
};

class	CtmCommonPartProtocol: public CtmPartProtocol
{
	DECLARE_DYNCREATE(CtmCommonPartProtocol)
	public:
		CtmCommonPartProtocol();
		~CtmCommonPartProtocol();
	public:
		//=================
		//	override the virtual functions 
		//=================
		int		CreateSelf();									//	CtmPackClass::CreateSelf
		int		FreeSelf();
		
		int		SetPartValue(int nPos, void* pValue, int nSize);
		int		GetPartValue(int nPos, void* pValue, int nSize);
		void*	GetBuffer();
		   
		int		GetSize();
		void	SetSize(int nSize);
		int		GetMaxSize();
		void	SetMaxSize(int nSize);
		
		
	protected:
		int		m_nMaxSize;
		int		m_nSize;
		BYTE*	m_pBuffer;

};




//	各種數據的協議祖類
class	CtmProtocol : public CTaskComm, public CtmPackClass
{
	DECLARE_DYNCREATE(CtmProtocol)
	public:
		CtmProtocol();
		virtual	~CtmProtocol();	
	public:		
		virtual	int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		virtual	int		ComposeProtocol	(int nCmd, void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		virtual	int		ParseProtocol	(void* pInData,		int nInputLen);
		virtual	int		GetType			();
		virtual	int		RevType			();
		
		virtual	void	SetSendType(int nType);
		virtual	void	SetRevType(int nType);
		        void    SetDeviceID(int nID);
	protected:
		virtual	int		InitSend();
		virtual	int		InitRev();
		virtual	int		SetDataToDataBase(void* pData, int nCount, BOOL bSend);
	protected:
		int		m_nSendType;
		int		m_nRevType;
		int		m_nCounter;
		int		m_nDeviceID;
		CtmCommonPartProtocol	m_SendHeader;
		CtmCommonPartProtocol	m_SendFooter;
		
		CtmCommonPartProtocol	m_RevHeader;
		CtmCommonPartProtocol	m_RevFooter;
};

class	CtmHostProtocol: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmHostProtocol)
	
	static	const	int	CNT_INT_PROTOCOL_MAXNUM = 64;
	public:
		struct	tmProtocolList
		{
			tmProtocolList* pPre;
			CtmProtocol* 	pProtocol;
			tmProtocolList* pNext;
		};
		
		
		CtmHostProtocol();
		~CtmHostProtocol();
		
	public:
		virtual	int		Compose(int nType, 	void* pOutData, void* pInData, int nInputLen, void* pEx = NULL);
		virtual	int		Parse(void* pInData,int nInputLen);
		virtual	int		GetType(void* pInData, int nInputLen);
		        int     SetDeviceID(int nID);
	protected:
		
		virtual	BOOL	AddProtocol(CtmProtocol* pProtocol);
		virtual	BOOL	DelProtocol(CtmProtocol* pProtocol);
		
		CtmProtocol*	GetSendProtocol(int nType);
		CtmProtocol*	GetRevProtocol(int nType);
	protected:
		tmProtocolList	m_List;
		int				m_nCount;
		CtmProtocol*	m_pProtocols[CNT_INT_PROTOCOL_MAXNUM];
};


#endif
