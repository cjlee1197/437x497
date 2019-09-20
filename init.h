/*==========================================================================+
|  Class    : Init                                             				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 担喘                                                        |
|  Version  : V1.00                                                         |
|  Creation : 2004/05/11                                                    |
|  Remark	: ﹍て贺戈方			                                    |
+==========================================================================*/
#ifndef		INIT_H
#define		INIT_H

#include	"tmpackclass.h"
#include	"cfont.h"
#include	"timer.h"
#include	"cbitmap.h"
#include	"files.h"
#include	"stable.h"
#include	"multilanguage.h"
#include	"inifile.h"
#include	"cblock.h"
#include	"main.h"
#include	"cgif.h"
#include 	<ext/hash_map>
//using namespace __gnu_cxx;
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
#define 	IMAGE_MAXNUM	200

typedef	struct	tagtmINIBMP
{
	char		cBMPPath[256];
	CtmBitMap*	pIniBmp;	
}	tmINIBMP;

typedef	struct	tagtmINIGif
{
	char		cGifPath[256];
	CtmGif*		pIniGif;	
}	tmINIGIF;

enum	INIBMPID {
	BMP_OVERVIEWID,	
	BMP_BACKGROUNDID, 
	PROMPT_BARID,	
	PROMPT_BARIDR,			//	fans add 2007/8/7 07:46と
	STATE_BARID,	
	TOOLBAR_BACKGROUNDID,
	BMP_TITLEID,
	BMP_ACTUALBGID,
	BMP_ACTUALBGWID,
	SUBTOOLBAR_BACKGROUNDID
};

class	CInit: public	CtmPackClass
{
	DECLARE_DYNCREATE(CInit)
	public:
		CInit();
		~CInit();
	
	public:
		virtual	int		Init()										{return 0;}
		virtual	void	Init_System()								{}
		virtual	int		Init_Bmp()									{return 0;}
		virtual	int		Init_DB()									{return 0;}
		virtual	int		Init_Block()								{return 0;}
		virtual	void	Init_AxialConfig(int nDealRamp)				{}
		virtual	void	Init_Mold()									{}
		virtual void	Init_FlashEX()								{}
		virtual void	Init_SramFromBack()							{}
		virtual	int		Destroy()									{return 0;}
		virtual int 	Destroy_System()							{return 0;}
		virtual int 	Destroy_Font()								{return 0;}
		virtual int 	Destroy_Bmp()								{return 0;}
		virtual int 	Destroy_Block()								{return 0;}
		
		virtual	void	Init_CloseMoldConfig(int nDealRamp=0)		{}
		virtual	void	Init_OpenMoldConfig(int nDealRamp=0)		{}
		virtual	void	Init_InjectConfig(int nDealRamp=0)			{}
		virtual	void	Init_HoldConfig(int nDealRamp=0)			{}
		virtual	void	Init_ChargeConfig(int nDealRamp=0)			{}
		virtual	void	Init_EjtADVConfig(int nDealRamp=0)			{}
		virtual	void	Init_EjtRETConfig(int nDealRamp=0)			{}
		virtual	void	Init_AirConfig()							{}
		virtual	void	Init_CoreConfig()							{}
		virtual	void	Init_CloseInject()							{}
		virtual void 	Init_InjectBConfig(int nDealRamp=0)   		{}
  		virtual void 	Init_HoldBConfig(int nDealRamp=0)   		{}
  		virtual void 	Init_ChargeBConfig(int nDealRamp=0)  	 	{}
 	 	virtual void 	Init_EjtBADVConfig(int nDealRamp=0)   		{}
  		virtual void 	Init_EjtBRETConfig(int nDealRamp=0)   		{}
		virtual	void	InitData(char*	pszRank, char***	pszArys, int wData){}
		
	protected:		
};

extern		CInit*	g_pInit;


extern	CtmBlock*			g_pBlock	  	;
extern	tmINIBMP			g_aIniBmp[]		;
extern	tmINIGIF			g_aIniGif[]		;

extern 	const	int		STRINGNUM;						//	﹃计程琌1024,猔種ぃ琌才﹃い才计
extern	char*	CHINESET;		//	羉砰いゅ
extern	char*	ENGLISH;		//	璣ゅ
extern	char*	STRDIR;
//extern	hash_map<const char*, char*, hash<const char*>, eqstr> strMap;		// 甧竟
#endif
