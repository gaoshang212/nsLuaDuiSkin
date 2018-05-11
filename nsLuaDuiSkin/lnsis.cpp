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

	LBIND_DEFINE_STATIC_FUNC(Nsis, OnProgress)
		CHECK_ARG_FUNC(1);
		LuaFunction param = arg[1];
		auto func = param.getIndex();
		


		Nsis::OnProgress(func);

	LBIND_END_DEFINE_FUNC


}

LBIND_BEGIN_DEFINE_LIB(Nsis)
{
	"install", lbindnsis::Install
},
	{ "uninstall", lbindnsis::Uninstall },
	{ "onProgress", lbindnsis::OnProgress },



		LBIND_END_DEFINE_LIB