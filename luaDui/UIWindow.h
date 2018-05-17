#ifndef UIWINDOW_H
#define UIWINDOW_H
#include <map>


namespace DuiLib
{
	class CWindowUI : public CWindowWnd, public IDialogBuilderCallback
	{
	public:
		LBIND_BASE_CLASS_DEFINE(CWindowUI);

		CWindowUI();
		virtual ~CWindowUI();

		virtual void OnFinalMessage(HWND hWnd);

		virtual UINT GetClassStyle() const;

		virtual void Init();

		virtual CControlUI* CreateControl(LPCTSTR pstrClass);

		virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

#if defined(WIN32) && !defined(UNDER_CE)
		virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif

		virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LPCTSTR GetWindowClassName() const;

		static std::map<HWND, CWindowUI*> GetWindows();

		void SetSkinXml(LPCTSTR xml) { if (xml)skin_xml_ = xml; }

		CControlUI* FindControl(LPCTSTR name) { return paint_manager_.FindControl(name); }
		CPaintManagerUI* GetManager();

		void SetIcon(CDuiString name);

	protected:
		CPaintManagerUI paint_manager_;
		CDuiString skin_xml_;

		static std::map<HWND, CWindowUI*> m_windows;
		HICON m_pIconInfo = nullptr;

		HICON CreateIcon(HBITMAP hBitmap);
	};


}
#endif // BaseDialog_H