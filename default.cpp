#include	"default.h"


CtmDefault*	g_pDefault = NULL;

IMPLEMENT_DYNCREATE(CtmDefault, CtmPackClass)

CtmDefault::CtmDefault(): CtmPackClass()
{
	
}

CtmDefault::~CtmDefault()
{
	
}


int		CtmDefault::SetDefault()
{
	return 0;
}


