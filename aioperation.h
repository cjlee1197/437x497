#ifndef     D_AIOPERATION
#define     D_AIOPERATION
#include	"CObject.h"

#define		AI_OP_TYPE_VISIBLE			0x00010000		//	visible type
#define		AI_OP_TYPE_ENABLE			0x00020000		//	enable
#define		AI_OP_TYPE_MESSAGE			0x00040000

#define		AI_OP_NO					0x00000001
#define     AI_OP_YES			  		0x00000002		
		
#define		AI_DEFAULT					0x7FFFFFFFFFFFFFFFLL

#define		WND_AI_FLAG					"aiop"
#define		WND_AI_LINKWND				"aiwnd"
#define		WND_AI_LINKDB				"aiid"
#define		WND_AI_VALUE				"aivalue"
#define		WND_AI_DISIBLE_DEFAULT		"aidvalue"
#define		WND_AI_VISILBE_DEFAULT		"aisvalue"

#define		WND_AI_MSGID				"aimsgid"
#define		WND_AI_LPARAM				"ailparam"
#define		WND_AI_WPARAM				"aiwparam"



class	CtmWnd;
class CtmAiOperation : public CObject
{
	DECLARE_DYNAMIC(CtmAiOperation)
	public:
		CtmAiOperation();
		CtmAiOperation(CtmWnd*	pwnd);
		~CtmAiOperation();
	public:
		void	SetAiWnd(CtmWnd* pwnd);
		void	GetWndInfo();
		int		AiOp();
		CtmWnd* GetAiWnd();
		void	ExeAiOp(int nAiOp);
		void	ExeAiOp();
		CtmWnd*	GetDbWnd();
		char*	GetDBID() {return m_szDBID;};
		
	
	private:
		void	InitPara();
		
		int		AiOpEdit();
		int		AiOpOption();
		int		AiOpSelectEdit();
		int		AiOpDB();
		
		
		int		Compare(long long llValue);
		void	SetNo(char* pszProperty);
		void	SetYes(char* pszProperty);
	protected:
		CtmWnd*		m_pAiWnd;
		int			m_nOpType;
		char		m_szDBID[128];
		CtmWnd*		m_dbWnd;		
		BOOL		m_bOp;
		
		long long 	m_llValue;
		long long	m_llValues[10];
		
	private:
		bool		m_bFirst;
};

#endif

