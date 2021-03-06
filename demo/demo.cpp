// demo.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	zsummer::log4z::ILog4zManager::GetInstance()->Start();


	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);


	DuiLib::CApplicationUI app(hInstance);
	try {
		//app.GetLuaState()->require("main");
		app.SetResourcePath(_T("F:\\Source\\nsLuaDuiSkin\\skin\\"));
		app.GetLuaState()->setPath("F:\\Source\\nsLuaDuiSkin\\sample\\");
	
		app.GetLuaState()->require("main");
	}
	catch (LuaException e)
	{
		LOGE(e.what());
	}

	app.MessageLoop();

	FreeConsole();
	return 0;
}

int main()
{
	
}