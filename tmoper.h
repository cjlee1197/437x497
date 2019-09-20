
#ifndef     D_TMPOINTOPER
#define     D_TMPOINTOPER

#include	"wnd.h"
#include	"formview.h"
//	the first character '_', that mean the macro 2009-12-25 17:49:19

#define	EXIT(result) return result
//#define	EXIT	return

#define	AUTOCHECK_CLASS(class_point) 	\
	if (class_point != NULL)	\
		class_point

#define	AUTOCHECK_CLASSF(class_point, result) 	\
	if (class_point != NULL)	\
		result = class_point

#define	GET_WND_VALUE(pwnd, property_name, value) \
	AUTOCHECK_CLASS(pwnd)->GetPropValueT(#property_name, &vlaue, sizeof(vlaue))
#define	SET_WND_VALUE(pwnd, property_name, value) \
	AUTOCHECK_CLASS(pwnd)->SetPropValueT(#property_name, vlaue)

//********************************************************************************************//
//************************************* Form Event Macro**************************************//
//********************************************************************************************//

#define	FORMLIB_START(form_name)\
	CtmWnd* ____u_pTemp 		= NULL;\
	CtmWnd* ____u_pCurrentWnd 	= NULL;\
	WORD	____u_wKey 		 	= 0;   \
	WORD	____u_wID			= 0;   
	
#define	FORMLIB_END 

#define	DEFINE_SELF_VAR(var_type, var_name) 	var_type var_name
#define	DEFINE_SELF_VARP(var_type, var_name) 	var_type var_name
#define	DEFINE_SELF_WND(wnd_name)				DEFINE_SELF_VARP(CtmWnd*, wnd_name)

#define	GET_SELF_VAR(var_type, var_name)	var_name
#define	GET_SELF_VARP(var_type, var_name)	GET_SELF_VAR(var_type, var_name)
#define	GET_SELF_WND(wnd_name)				GET_SELF_VAR(CtmWnd*, wnd_name)

#define	_GET_FORM_CURRENT_WND(view_form) \
	view_form->FindControlFromTab(((CtmFormView*)view_form)->Get_TabStop())
#define _GET_FORM_WND(view_form, wnd_name) \
	view_form->FindControlFromName(#wnd_name)
#define	GET_FORM_WND(wnd_name)				\
	_GET_FORM_WND(pwndSender, wnd_name)
#define	GET_FORM_CURRENT_WND()	\
	_GET_FORM_CURRENT_WND(pwndSender)

#define	DECLARE_FORM_CREATE		extern	"C"	BOOL	OnCreateA(CtmWnd* pwndSender);
#define	DECLARE_FORM_ONCHANGE	extern 	"C" WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
#define	DECLARE_FORM_ONDESTROY	extern	"C" void	OnDestroyA(CtmWnd* pwndSender);
#define	DECLARE_FORM_ONKEY		extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
//=======================================================================//
//	define the OnCreateA
//=======================================================================//
#define	DEFINE_FORM_CREATE_START	\
	BOOL	OnCreateA(CtmWnd* pwndSender)	\
	{										\

#define	DEFINE_FORM_CREATE_END	\
		EXIT(TRUE); 			\
	}
//=======================================================================//
//	define the OnchangeA
//=======================================================================//
#define	DEFINE_FORM_ONCHANGE_START 									\
	WORD OnChangeA(CtmWnd* pwndSender, WORD wIDControl)				\
	{																\
		____u_pTemp = pwndSender->FindControlFromTab(wIDControl);	\
		____u_wID 	= wIDControl;
						
#define	DEFINE_FORM_ONCHANGE_END \
		EXIT(wIDControl);		\
	}

#define	ONCHANGE_FORM_WND2E(pwnd1, pwnd2)	\
	if (pwnd1 == pwnd2 && pwnd1 != NULL)

#define	ONCHANGE_FORM_WNDE(pwnd)	\
	ONCHANGE_FORM_WND2E(pwnd, ____u_pTemp)
		
#define	ONCHANGE_FORM_WND(wnd_name)	\
	CtmWnd* wnd_name = pwndSender->FindControlFromName(#wnd_name);	\
	ONCHANGE_FORM_WNDE(wnd_name)


		
//=======================================================================//
//	OnDestroyA
//=======================================================================//
#define	DEFINE_FORM_DESTROY_START \
	void  OnDestroyA(CtmWnd* pwndSender)	\
	{													\
		
#define	DEFINE_FORM_DESTROY_END	\
		return ;				\
	}
	
//=======================================================================//
//	OnkeyA
//=======================================================================//
#define	DEFINE_FORM_ONKEY_START								\
	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)			\
	{														\
		____u_pCurrentWnd = GET_FORM_CURRENT_WND();			\
		____u_wKey 	= wKey;

#define	DEFINE_FORM_ONKEY_END								\
		return ((CtmFormView*)pwndSender)->OnKey1(wKey);	\
	}

#define	ONKEY_FORM_KEY(key)									\
	if (key == wKey)

#define	ONKEY_FORM_KEY_WND2E(key, pwnd1, pwnd2)				\
	if (key == wKey && pwnd1 == pwnd2 && pwnd1 != NULL)

#define	ONKEY_FORM_KEY_WNDE(key, pwnd)			\
	ONKEY_FORM_KEY_WND2E(key, pwnd, ____u_pCurrentWnd)
				
//	
#define ONKEY_FORM_KEY_WND(key, wnd_name)				\
	CtmWnd*	wnd_name = _GET_FORM_WND(pwndSender, wnd_name);	\
	ONKEY_FORM_KEY_WNDE(key, wnd_name)
		


#endif
