#pragma once
#include "stdafx.h"
#include "LuaObj/LuaBind.h"
#include "MsgDef.h"
#include "api.h"
#include <thread>

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




class Dispatcher : public DuiLib::IRunbaleUI
{
public:

	Dispatcher(int progressfunction, float value) :m_value((int)(value * 100)), m_progressfunction(progressfunction)
	{

	}


	void Run(LuaState* L) override
	{
		LuaObject func = L->getGlobal("onProgress");
		if (func.isFunction()) 
		{
			LuaFunction fun = func;
			fun(m_value);
		}

		LuaTable nsis = L->getGlobal("nsis");

		if (nsis.isTable())
		{
			LuaFunction onProgress = nsis["onProgress"];

			if (onProgress.isValid())
			{
				onProgress(m_value);
			}
		}

		//func(m_value);

		/*if (m_progressfunction)
		{
			 auto function = static_cast<LuaFunction>(LuaFunction::objFromIndex(L, m_progressfunction));
			 function(m_value);
		}*/
	}

	int AddRef() override
	{
		return ++m_count;
	};

	int Release() override
	{
		int count = --m_count;

		if (!count)
		{
			delete this;
		}
		return count;
	};

	int GetRefCt() override
	{
		return  m_count;
	};

protected:

	int m_count = 0;
	int m_value;
	int m_progressfunction;

private:
	//typedef int *callback1(LuaState *L);
	//Func m_func;
};


extern  extra_parameters* g_pluginParms;
class Nsis
{
public:
	LBIND_BASE_CLASS_DEFINE(Nsis);

	static void Install()
	{
		Send(WM_NSIS_INSTALL);

		if (m_extra != nullptr && m_installfucntion > 0)
		{
			//NsisInstall();
			std::thread task = std::thread(NsisInstall);
			task.detach();
		}
	}

	static void Uninstall()
	{
		Send(WM_NSIS_UNINSTALL);
	}

	static void InstallFunction(extra_parameters* extra, INT_PTR installfucntion)
	{
		m_extra = extra;
		m_installfucntion = installfucntion;
	}

	static void OnProgress(int func)
	{
		m_progressfunction = func;
	}

	static void NotifyProgress(float progress)
	{
		Dispatcher* d = new Dispatcher(m_progressfunction, progress);

		DuiLib::CApplicationUI::SharedInstance()->PostRunable(d);
		LOGE("the progress is %f", progress);
	}

	static void CallFunction(const char* fn)
	{
		auto item = m_functions[std::string(fn)];
		if (item > 0)
		{
			m_extra->ExecuteCodeSegment(item, nullptr);
		}
	}

	static void RegisterFunction(const char* fn, int function)
	{
		m_functions[std::string(fn)] = function - 1;
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

			::SendMessage(window->GetHWND(), umsg, NULL, NULL);
			++it;
		}
	}

	static void NsisInstall()
	{
		if (m_extra != nullptr && m_installfucntion > 0)
		{
			m_extra->ExecuteCodeSegment(m_installfucntion, nullptr);
		}
	}

	static extra_parameters* m_extra;
	static INT_PTR m_installfucntion;
	static int m_progressfunction;

	static std::map<std::string, int> m_functions;
};


