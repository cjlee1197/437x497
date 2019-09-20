#include "simplefs.h"

class CSimpleFile
{
	public:
	CSimpleFile(int fd, unsigned long start, unsigned long end);
	~CSimpleFile();
	
	private:
		CtmSimpleFS 	*m_pSimpleFs;
		int 		m_fd;
		char		m_name[TMSIMPLEFS_NAME_LEN+1];
	public:
		int 	Open(const char *name, int mode = O_CREATE | O_WRITE);
		
		int 	Read(void *buf, int length);
		int 	Read(int offset, void *buf, int length);
		
		int 	Write(void *buf, int length);
		int 	Write(int offset, void *buf, int length);
		
		int     Seek(int offset, int origin);
		
		int 	Close();
		int		Create(const char *name);
		int		Remove();
		int		Truncate();
		void	List();
		int     Format();
};
