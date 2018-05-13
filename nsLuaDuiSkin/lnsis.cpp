#include "stdafx.h"
#include "LuaObj/LuaBind.h"
#include "DuiLib/UIString.h"
#include "MsgDef.h"
#include "nsis.h"


namespace lbindnsis
{
	LBIND_DEFINE_STATIC_FUNC(Nsis, Install)
		Nsis::Install();
	LBIND_END_DEFINE_FUNC;

	LBIND_DEFINE_STATIC_FUNC(Nsis, Uninstall)
		Nsis::Uninstall();
	LBIND_END_DEFINE_FUNC

	LBIND_DEFINE_STATIC_FUNC(Nsis, Call)
		CHECK_ARG_STRING(1);
		
		LuaObject fn = arg[1];
		if (fn.isString()) {
			Nsis::CallFunction(fn.toString());
		}

	LBIND_END_DEFINE_FUNC


}

LBIND_BEGIN_DEFINE_LIB(Nsis)
{
	"install", lbindnsis::Install
},
	{ "uninstall", lbindnsis::Uninstall },
	{ "call", lbindnsis::Call},


LBIND_END_DEFINE_LIB