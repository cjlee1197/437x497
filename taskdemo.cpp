/*===========================================================================+
|  Class    : TaskDemo                                                       |
|  Task     : Demonstration Service Routine                                  |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 03/23/1998                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include    "utils.h"
#include    "taskdemo.h"
#include	"main.h"
/*===========================================================================+
|           External                                                         |
+===========================================================================*/
extern      MACROKEY    g_amacrokey[];

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
MACROKEY    g_amacrokey[] = {
///* Pxa255 bak
//			{_F1            , 100},    //cls
			{_F10            , 2000},
			{_F9             , 4000},
/*             {_F2            , 100},    //cls
            {_F2            , 100},   
           {_F3            , 100},  
            {_F2            , 100},
            {_F10           , 100},
            {_F4            , 100},
            {_F10           , 100},    //ext

			{_F3            , 100},	//ejt
			{_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},  
            {_F5            , 100},
            {_F6            , 100},   
            {_F10           , 100},    //ext

            {_F4            , 100},    //inj
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
//          {_F5            , 100},
//          {_F6            , 100},
            {_F10           , 100},    //ext

            {_F5            , 100},    //chg
            {_F2            , 100},   
            {_F3            , 100},     
            {_F4            , 100},
            {_F10           , 100},    //ext

            {_F6            , 100},    //inju
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F5            , 100},
            {_F10           , 100},    //ext

            {_F7            , 100},    //tmp
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F10           , 100},    //ext
            
            {_F8            , 100},    //tmp
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F10           , 100},    //ext
            
            
            {_F9            , 100},    //fast
            {_F2            , 100},    
            {_F10           , 100},    //ext
            
            {_F10           , 100},    //alarm
            
            {_F2            , 100},    //moni
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F5            , 100},
            {_F10           , 100},
            
            {_F3            , 100},	//zero
            
            {_F4            , 100},	//IO
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F8            , 100},
            {_F5            , 100},
            {_KEY_CLEAR     , 100},
            {_KEY_CLEAR     , 100},
            {_F10           , 100},
            
            {_F5            , 100},	//mold
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F5            , 100},
            {_F10           , 100},
            
            {_F6            , 100},	//version
            
            {_F7            , 100},	//system
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F5            , 100},
            {_F6            , 100},
            {_F7            , 100},
            {_F10           , 100},
            
            {_F8            , 100},	//other
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F5            , 100},
            {_F6            , 100},
            {_F7            , 100},
            {_F10           , 100},
            {_F10           , 100},    //overview*/
//*/
/*   p3���F bak
            {_F2            , 100},    //clamp
            {_F2            , 100},
            {_F10           , 100},    //ext

			{_F3            , 100},	//ejt
			{_F2            , 100},    
            {_F10           , 100},    //ext

			{_F4            , 100},	//inj.
			{_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F10           , 100},    //ext

			{_F5            , 100},	//chrg
			{_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F10           , 100},    //ext

            {_F6            , 100},    //Core
            {_F2            , 100},
            {_F10           , 100},    //ext

            {_F7            , 100},    //Nozl
            {_F2            , 100},
            {_F10           , 100},    //ext
            
            {_F8            , 100},    //tmp
            {_F2            , 100},    
            {_F3            , 100},
            {_F10           , 100},    //ext
            
            {_F9            , 100},    //Fast
            {_F2            , 100},    
            {_F10           , 100},    //ext
            
            {_F10           , 100},    //alarm
            
            {_F2            , 100},    //moni
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F10           , 100},    //ext
            
            {_F3            , 100},	//zero
            
            {_F4            , 100},	//IO
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F5            , 100},
            {_KEY_CLEAR     , 100},
            {_KEY_CLEAR     , 100},
            {_F6            , 100},
            {_F7            , 100},
            {_F8            , 100},
            {_F10           , 100},
            
            {_F5            , 100},	//mold
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F5            , 100},
            {_F6            , 100},
            {_F10           , 100},
            
            {_F6            , 100},	//version
            
            {_F7            , 100},	//system
            {_F2            , 100},    
            {_F3            , 100},   
            {_F4            , 100},
            {_F5            , 100},
            {_F6            , 100},
            {_F7            , 100},
            {_F10           , 100},
            {_F10           , 100},    //overview
*/          
            {_NULL_KEY      ,    0}
};
/*===========================================================================+
|           Class implementation - TaskDemo                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskDemo::CTaskDemo() : CTask()
{
	m_nIndex   = 0;
	m_lCounter = 1000;
	AddCounter(&m_lCounter, m_idMe);
	RegisterMessage(MSG_DEMO, m_idMe);
}

CTaskDemo::~CTaskDemo()
{
	UnRegisterAllMessage(m_idMe);
	DeleteCounter(&m_lCounter, m_idMe);
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CTaskDemo::Run()
{
	if (m_lCounter < 0)
    {
    	PutKeyTail(g_amacrokey[m_nIndex].wKey);
    	m_lCounter = g_amacrokey[m_nIndex].nDelay;
    	if (g_amacrokey[++m_nIndex].wKey == _NULL_KEY) m_nIndex = 0;
    }
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

