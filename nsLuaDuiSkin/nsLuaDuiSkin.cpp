// nsLuaDuiSkin.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginapi.h"
#include "utils.h"

using namespace  DuiLib;

extern HINSTANCE g_hInstance;

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

	TCHAR pluginPath[MAX_PATH] = { 0 };
	char pluginPathW[MAX_PATH] = { 0 };

	extra->RegisterPluginCallback(g_hInstance, PluginCallback);

	popstringn(pluginPath, sizeof(pluginPath));

	g_app = new CApplicationUI(g_hInstance);
	utils::TCharToChar(pluginPath, pluginPathW);

	g_app->SetResourcePath(pluginPath);
	g_app->GetLuaState()->setPath(pluginPathW);
}

NSISEXPORT void AddSearchPath(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();

	TCHAR searchPath[MAX_PATH] = { 0 };
	char searchPathw[MAX_PATH] = { 0 };

	popstringn(searchPath, sizeof(searchPath));
	utils::TCharToChar(searchPath, searchPathw);

	//auto app = CApplicationUI::SharedInstance();

	g_app->GetLuaState()->setPath(searchPathw);
}

NSISEXPORT void Require(HWND hwndParent, int string_size, LPTSTR variables, stack_t **stacktop, extra_parameters *extra, ...)
{
	EXDLL_INIT();

	TCHAR mainlua[MAX_PATH] = { 0 };
	char mainluaw[MAX_PATH] = { 0 };

	popstringn(mainlua, sizeof(mainlua));
	utils::TCharToChar(mainlua, mainluaw);

	auto app = CApplicationUI::SharedInstance();

	try {

		if (strlen(mainluaw) > 0)
		{
			strcpy_s(mainluaw, "main");
		}
		g_app->GetLuaState()->require(mainluaw);
	}
	catch (LuaException e)
	{
		LOGE(e.what());
	}


	g_app->MessageLoop();
}
