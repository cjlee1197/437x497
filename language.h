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
#define     PA_MAX_FILE         8                   //<<yang 2005/3/18 .........�W�[
#define     GetPrintList()              GetLanguageList()

enum		_LANGUAGE   {						//<<yang 2005/3/19
					_CHINESETRAD, 			//0		�����c��	chn       
					_CHINESESIMPLE,         //1		����²��	chs       
					_ENGLISH,               //2		�^��	eng           
					_JAPANESE,              //3		���	jpn           
					_KOREA,                 //4		����	kor           
                    _THAILAND,              //5		����	tal           
                    _VIETNAM,               //6		�V�n��	vtn           
                    _INDONESIAN,            //7		�L����	ind           
                    _TURKISH,               //8		�g�ը��	tur       
                    _ALBANIAN,              //9		�����ڥ��Ȥ�	alb   
                    _ARABIC,                //10	���ԧB��	arb       
                    _IRANIAN,               //11	��Ԥ�	irn           
                    _HEBRAIC,               //12	�ƧB�ܤ�	heb       
                    _GREECE,                //13	��þ��	grk           
                    _ITALIAN,               //14	�q�j�Q��	itl       
                    _SPANISH,               //15	��Z����	esp       
                    _PORTUGUESE,            //16	�������	prt       
                    _FRENCH,                //17	�k��	frn           
                    _GERMAN,                //18	�w��	ger           
                    _CZECHOSLOVAK,          //19	���J��	cez           
                    _SWEDISH,               //20	����	swd           
                    _MAGYAR,                //21	�I���Q��	hug       
                    _POLISH,                //22	�i����	pol           
                    _FINLAND,               //23	������	fnn           
                    _HOLAND,                //24	������	hol           
                    _RUSSIAN,               //25	�X��	rus           
                    _SYLVIA,                //26	�뺸���Ȥ�	slv       
                    _LATIN,                 //27	�ԤB��	ltn           
                    _RESERVED1,             //28	�d1	lg1
                    _RESERVED2,             //29	�d2	lg2
                    _RESERVED3,             //30	�d3	lg3
                    _RESERVED4,             //31	�d4	lg4
                    _RESERVED5,  	        //32	�d5	lg5
                    _RESERVED6,             //33	�d6	lg6
                    _RESERVED7,             //34	�d7	lg7
                    _RESERVED8,             //35	�d8	lg8	
                    _RESERVED9,             //36	�d9	lg9	
                    _RESERVED10,            //37	�d10 lg10
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


