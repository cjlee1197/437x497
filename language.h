/*===========================================================================+
|  Class    : Language                                                       |
|  Task     : Language                                                       |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52 -                                                   |
|  Link     : MSVC V1.52 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/27/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef		D_LANGUAGE
#define		D_LANGUAGE

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/


/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
#define     PA_MAX_FILE         8                   //<<yang 2005/3/18 .........增加
#define     GetPrintList()              GetLanguageList()

enum		_LANGUAGE   {						//<<yang 2005/3/19
					_CHINESETRAD, 			//0		中文繁體	chn       
					_CHINESESIMPLE,         //1		中文簡體	chs       
					_ENGLISH,               //2		英文	eng           
					_JAPANESE,              //3		日文	jpn           
					_KOREA,                 //4		韓文	kor           
                    _THAILAND,              //5		泰文	tal           
                    _VIETNAM,               //6		越南文	vtn           
                    _INDONESIAN,            //7		印尼文	ind           
                    _TURKISH,               //8		土耳其文	tur       
                    _ALBANIAN,              //9		阿爾巴尼亞文	alb   
                    _ARABIC,                //10	阿拉伯文	arb       
                    _IRANIAN,               //11	伊朗文	irn           
                    _HEBRAIC,               //12	希伯萊文	heb       
                    _GREECE,                //13	希臘文	grk           
                    _ITALIAN,               //14	義大利文	itl       
                    _SPANISH,               //15	西班牙文	esp       
                    _PORTUGUESE,            //16	葡萄牙文	prt       
                    _FRENCH,                //17	法文	frn           
                    _GERMAN,                //18	德文	ger           
                    _CZECHOSLOVAK,          //19	捷克文	cez           
                    _SWEDISH,               //20	瑞典文	swd           
                    _MAGYAR,                //21	匈牙利文	hug       
                    _POLISH,                //22	波蘭文	pol           
                    _FINLAND,               //23	芬蘭文	fnn           
                    _HOLAND,                //24	荷蘭文	hol           
                    _RUSSIAN,               //25	俄文	rus           
                    _SYLVIA,                //26	塞爾維亞文	slv       
                    _LATIN,                 //27	拉丁文	ltn           
                    _RESERVED1,             //28	留1	lg1
                    _RESERVED2,             //29	留2	lg2
                    _RESERVED3,             //30	留3	lg3
                    _RESERVED4,             //31	留4	lg4
                    _RESERVED5,  	        //32	留5	lg5
                    _RESERVED6,             //33	留6	lg6
                    _RESERVED7,             //34	留7	lg7
                    _RESERVED8,             //35	留8	lg8	
                    _RESERVED9,             //36	留9	lg9	
                    _RESERVED10,            //37	留10 lg10
					};
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

char**      GetLanguageList();

extern      short       g_nMaxLanguage;     			
extern      char*       pLanguageString[PA_MAX_FILE];	
//extern      WORD		g_wLangCode[PA_MAX_FILE];		
extern      short		g_nLanguageIndex;       		


#endif


