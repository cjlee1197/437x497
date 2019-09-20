#ifndef 	__D__CTMREADDATA
#define		__D__CTMREADDATA
#include	"tmpackclass.h"
#include	"tmvalues.h"
#include	"parsepule.h"


int		ReadDataStr(CtmWnd* pwndSource, char* pszOut[], int nNum = -1);
int		ReadDataInt(CtmWnd* pwndSource, int	nOut[],   int nNum = -1);


class	CtmReadData: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmReadData)
	public:
		CtmReadData();
		~CtmReadData();
	public:
		
		virtual	BOOL	OpenFileStr(char* pszFileName);
		virtual	BOOL	OpenFileInt(char* pszFileName);
		virtual	void	CloseFileStr() {};
		virtual	void	CloseFileInt() {};
		
		
		virtual	int		ReadData(char* pszOut[], int nNum = -1);
		virtual	int		ReadData(int	nOut[],   int nNum = -1);
	
		virtual	CtmWnd* GetControlStr();
		virtual	CtmWnd* GetControlInt();
	protected:
		BOOL			CheckFileExist(char* pszFileName);
		CtmWnd*			OpenFile(CParseObject* pParse, char* pszFileName);
		void			CloseFile() {};
	protected:
		CParseObject m_ParseStr;
   	 	CParseObject m_ParseInt;
    	
  		CtmWnd*		m_pwndStr;
  		CtmWnd*		m_pwndInt;
    	
};


class	CtmReadPuerData: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmReadPuerData)
	
	public:
		CtmReadPuerData();
		~CtmReadPuerData();
	public:
		virtual	int		GetNum();
		
		virtual	int		Read(char*	pszOut[], 	int nNum = -1);
		virtual	int		Read(int	nOut[],		int nNum = -1);
		
		virtual	CtmWnd*	Open(char* pszFileName);
		virtual	void	Close();
			
		int				GetFileType() {return m_nFileType;};
	protected:
		BOOL			CheckFileExist(char* pszFileName);
		int				FileType();
	private:
		CParseObject	m_Parse;
		CtmWnd*			m_pwnd;
		int				m_nFileType;	//	1:int; 2: string values
	
	
};

#endif
