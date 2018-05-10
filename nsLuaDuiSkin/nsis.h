#pragma once
#include "stdafx.h"
#include "LuaObj/LuaBind.h"
#include "MsgDef.h"

class NsisMessageFilter :public DuiLib::IMessageFilterUI
{
public:
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override
	{
		LRESULT result;
		switch (uMsg)
		{
		case WM_NSIS_INSTALL:
			result = TRUE;
		case WM_NSIS_UNINSTALL:
			result = TRUE;
		}

		return result;
	}
};

class Nsis
{
public:
	LBIND_BASE_CLASS_DEFINE(Nsis);

	static void Install()
	{
		Send(WM_NSIS_INSTALL);
	}

	static void Uninstall()
	{
		Send(WM_NSIS_UNINSTALL);
	}
	static NsisMessageFilter n_filter;

private:
	static void Send(UINT umsg)
	{
		auto map = DuiLib::CWindowUI::GetWindows();

		auto it = map.begin();

		while (it != map.end())
		{
			auto window = it->second;
			//window.GetManager()->AddMessageFilter(&n_filter);

			::SendMessage(window->GetHWND(), umsg, NULL, NULL);
			++it;
		}
	}


};


