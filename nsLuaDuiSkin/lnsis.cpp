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
}

LBIND_BEGIN_DEFINE_LIB(Nsis)
{ "install", lbindnsis::Install},
{ "uninstall", lbindnsis::Uninstall },



LBIND_END_DEFINE_LIB