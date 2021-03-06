// nsLuaDuiSkin.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginapi.h"
#include "utils.h"
#include "nsis.h"

using namespace  DuiLib;

extern HINSTANCE g_hInstance;
extra_parameters* g_pluginParms;

CApplicationUI* g_app = NULL;

static UINT_PTR PluginCallback(enum NSPIM msg)
{
	return 0;
}


NSISEXPORT void myFunction(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();


	// note if you want parameters from the stack, pop them off in order.
	// i.e. if you are called via exdll::myFunction file.dat read.txt
	// calling popstring() the first time would give you file.dat,
	// and the second time would give you read.txt. 
	// you should empty the stack of your parameters, and ONLY your
	// parameters.

	// do your stuff here
	{
		LPTSTR msgbuf = (LPTSTR)GlobalAlloc(GPTR, (3 + string_size + 1) * sizeof(*msgbuf));
		if (msgbuf)
		{
			wsprintf(msgbuf, TEXT("$0=%s"), getuservariable(INST_0));
			MessageBox(hwndParent, msgbuf, TEXT("Message from example plugin"), MB_OK);
			GlobalFree(msgbuf);
		}
	}
}

NSISEXPORT void Initialize(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();

	g_pluginParms = extra;

	extra->RegisterPluginCallback(g_hInstance, PluginCallback);

	nstring pluginPath(MAX_PATH);

	popstringn(pluginPath.gettdata(), pluginPath.size());

	g_app = new CApplicationUI(g_hInstance);

	g_app->SetResourcePath(pluginPath.gettdata());
	g_app->GetLuaState()->setPath(pluginPath.getdata());

	TCHAR icon[MAX_PATH] = { 0 };
	popstringn(icon, MAX_PATH);

	g_app->SetIcon(icon);

	LBIND_REGISTER_CLASS(Nsis, g_app->GetLuaState());
}

NSISEXPORT void AddSearchPath(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();

	nstring searchPath(MAX_PATH);

	popstringn(searchPath.gettdata(), searchPath.size());

	g_app->GetLuaState()->setPath(searchPath.getdata());
}

NSISEXPORT void Require(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();

	nstring mainlua(MAX_PATH);

	popstringn(mainlua.gettdata(), mainlua.size());

	auto app = CApplicationUI::SharedInstance();

	try {

		if (mainlua.size() == 0)
		{
			mainlua = _T("main");
		}
		app->GetLuaState()->require(mainlua.getdata());
	}
	catch (LuaException e)
	{
		LOGE(e.what());
	}

	app->MessageLoop();
}

NSISEXPORT void InstallFunction(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();

	const auto func = popint();
	Nsis::InstallFunction(extra, func - 1);
}

NSISEXPORT void ProgressCallBack(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();

	const auto completedSize = popint();
	const auto totalSize = popint();

	double progress = completedSize * 1.0 / totalSize * 1.0 ;
	Nsis::NotifyProgress(progress);
}

NSISEXPORT void RegisterFunction(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();

	nstring name(MAX_PATH);
	popstringn(name.gettdata(), name.size());
	const auto func = popint();

	const char* fn = name.CloneTo();

	Nsis::RegisterFunction(fn, func);
}

NSISEXPORT void CallLua(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();

	nstring name(MAX_PATH);
	nstring fun(MAX_PATH);
	popstringn(name.gettdata(), name.size());
	popstringn(fun.gettdata(), fun.size());

	auto L = DuiLib::CApplicationUI::SharedInstance()->GetLuaState();

	name.getdata();
	fun.getdata();

	LuaObject obj(L);
	if (name.size() > 0)
	{
		obj = L->getGlobal(name.getdata());
	}

	LuaFunction function(L);
	if (name.size() > 0 && (obj).isTable())
	{
		function = static_cast<LuaTable>(obj)[fun.getdata()];
	}
	else
	{
		function = L->getGlobal(fun.getdata());
	}

	if (function.isValid())
	{
		auto result = function();
		if (!result.isNil())
		{
			pushint(result.toInt());
		}
	}

	pushint(0);
}

NSISEXPORT void SetIcon(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{

}