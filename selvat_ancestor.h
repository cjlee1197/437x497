#ifndef     D__TMSELVATANCESTOR
#define     D__TMSELVATANCESTOR

#include	"tmpackclass.h"

extern	BOOL	GetOilTable();
extern	int     GetOilVat(int x);
extern	int     OilToPosi(int y);

class	CtmVat_Ancestor: public CtmPackClass
{

	DECLARE_DYNCREATE(CtmVat_Ancestor)	
	//	friend ��Ƨ������F�ݮe�H�e����Ʊ��f
	friend	BOOL	GetOilTable();			//	CtmVat_Ancestor.GetTable
	friend	int     GetOilVat(int x);		//	CtmVat_Ancestor.GetCrossPos
	friend	int     OilToPosi(int y);		//	CtmVat_Ancestor.GetMoldPos
	public:
		CtmVat_Ancestor();
		~CtmVat_Ancestor();
	public:
		virtual	BOOL       	GetTable();							// ���o��e���ƹ��������O�y��
    	virtual	int         GetCrossPos(int x);					// �ഫ�ҪO��m���o����m
    	virtual	int         GetMoldPos(int y);					// �ഫ�o����m���ҪO��m
    	virtual	BOOL		TransferToCrossPos(BYTE* pBuffer);	// �ഫ�Ҧ������O�y�ѼƬ��o����m
    	virtual	BOOL		SendToHost();						// �ǰe�Ҧ������O�y�Ѽƨ�D��
    	virtual	BOOL		TransferToMoldPos();				// �ഫ�Ҧ������O�y�ѼƬ��ҪO��m
    	virtual	int			GetCrossNum();						// ��o�Q�r�Y������
    	virtual	int			GetMoldNum();						// ��o�ҪO������
   
};

extern	CtmVat_Ancestor*	g_pSelvat;

#endif   
