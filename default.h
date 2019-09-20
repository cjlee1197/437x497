#ifndef 	__D__CTMDEFAULT
#define		__D__CTMDEFAULT
#include	"tmpackclass.h"
#include	"database.h"



class	CtmDefault: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmDefault)
	public:
		CtmDefault();
		~CtmDefault();
	public:		
		virtual	int		SetDefault();
		
	protected:
		
};


extern	CtmDefault*	g_pDefault;
#endif
