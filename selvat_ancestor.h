#ifndef     D__TMSELVATANCESTOR
#define     D__TMSELVATANCESTOR

#include	"tmpackclass.h"

extern	BOOL	GetOilTable();
extern	int     GetOilVat(int x);
extern	int     OilToPosi(int y);

class	CtmVat_Ancestor: public CtmPackClass
{

	DECLARE_DYNCREATE(CtmVat_Ancestor)	
	//	friend ㄧ计Ч甧玡ㄧ计钡
	friend	BOOL	GetOilTable();			//	CtmVat_Ancestor.GetTable
	friend	int     GetOilVat(int x);		//	CtmVat_Ancestor.GetCrossPos
	friend	int     OilToPosi(int y);		//	CtmVat_Ancestor.GetMoldPos
	public:
		CtmVat_Ancestor();
		~CtmVat_Ancestor();
	public:
		virtual	BOOL       	GetTable();							// 眔讽玡痉计癸莱崩畒
    	virtual	int         GetCrossPos(int x);					// 锣传家狾竚猳竚
    	virtual	int         GetMoldPos(int y);					// 锣传猳竚家狾竚
    	virtual	BOOL		TransferToCrossPos(BYTE* pBuffer);	// 锣传┮Τ崩畒把计猳竚
    	virtual	BOOL		SendToHost();						// 肚癳┮Τ崩畒把计诀
    	virtual	BOOL		TransferToMoldPos();				// 锣传┮Τ崩畒把计家狾竚
    	virtual	int			GetCrossNum();						// 莉眔繷
    	virtual	int			GetMoldNum();						// 莉眔家狾
   
};

extern	CtmVat_Ancestor*	g_pSelvat;

#endif   
