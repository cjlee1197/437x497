#include	"selvat_ancestor.h"
#include	"utils.h"

IMPLEMENT_DYNCREATE(CtmVat_Ancestor, CtmPackClass)

CtmVat_Ancestor*	g_pSelvat = NULL;


/*===========================================================================+
|           Class implementation - CtmVat_Ancestor                           |
+===========================================================================*/

CtmVat_Ancestor::CtmVat_Ancestor(): CtmPackClass()
{
	
}

CtmVat_Ancestor::~CtmVat_Ancestor()
{
	
}

BOOL       	CtmVat_Ancestor::GetTable()
{
	
	return FALSE;
}



int         CtmVat_Ancestor::GetCrossPos(int x)
{
	
	return x;
	
}

int         CtmVat_Ancestor::GetMoldPos(int y)
{
	return y;
}

BOOL		CtmVat_Ancestor::TransferToCrossPos(BYTE* pBuffer)
{
	return FALSE;
}

BOOL		CtmVat_Ancestor::SendToHost()
{
	return FALSE;	
}

BOOL		CtmVat_Ancestor::TransferToMoldPos()
{
	return FALSE;
}


int			CtmVat_Ancestor::GetCrossNum()						// 獲得十字頭表的長度
{
	return 0;
}

int			CtmVat_Ancestor::GetMoldNum()						// 獲得模板表的長度
{
	return 0;
}

/*===========================================================================+
|           friend implementation - CtmVat_Ancestor                          |
+===========================================================================*/
BOOL	GetOilTable()
{
	if(u_tmSelvatParam.bUsed)
	{
		//printf("bUsed =%d\n",u_tmSelvatParam.bUsed);
		return u_tmSelvatParam.bUsed;
	}
	if (g_pSelvat != NULL)
	{
		return g_pSelvat->GetTable();
	}
	else return FALSE;
}

int     GetOilVat(int x)
{
	if(u_tmSelvatParam.bUsed)
	{
		//printf("use new method %d GetOilVat %d\n",x,TMSELVATFUNC::GetOilVat(x*0.1)*10);
		if(x>=65535)	return x;
		return TMSELVATFUNC::GetOilVat(x*0.1)*10;
	}
	if (g_pSelvat != NULL)
		return g_pSelvat->GetCrossPos(x);
	else return x;
			
}

int     OilToPosi(int y)
{
	if(u_tmSelvatParam.bUsed)
	{
		//printf("use new method %d oilToPosi %d\n",y,TMSELVATFUNC::OilToPosi(y*0.1)*10);
		if(y>=65535)	return y;
		return TMSELVATFUNC::OilToPosi(y*0.1)*10;
	}
	if (g_pSelvat != NULL)
		return g_pSelvat->GetMoldPos(y);
	else return y;
}





	
