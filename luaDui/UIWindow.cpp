#include "StdAfx.h"
#include "UIWindow.h"

#if !defined(UNDER_CE) && defined(_DEBUG)
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


namespace DuiLib
{
	std::map<HWND, CWindowUI*> CWindowUI::m_windows;

	CWindowUI::CWindowUI()
	{

	}

	CWindowUI::~CWindowUI()
	{
		if (m_pIconInfo) {
			::DestroyIcon(m_pIconInfo);
			m_pIconInfo = NULL;
		}
	}

	UINT CWindowUI::GetClassStyle() const
	{
		return CS_DBLCLKS;
	}

	CControlUI* CWindowUI::CreateControl(LPCTSTR pstrClass)
	{
		return NULL;
	}

	void CWindowUI::OnFinalMessage(HWND /*hWnd*/)
	{
		paint_manager_.ReapObjects(paint_manager_.GetRoot());
	}

	void CWindowUI::Init()
	{
		m_windows[*this] = this;// (std::make_pair(*this, this));

		HICON icon = CApplicationUI::SharedInstance()->GetIcon();
		if (icon)
		{
			NotiyIcon(icon);
		}
	}

	LRESULT CWindowUI::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWindowUI::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		m_windows.erase(*this);
		bHandled = FALSE;
		return 0;
	}

#if defined(WIN32) && !defined(UNDER_CE)
	LRESULT CWindowUI::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (::IsIconic(*this)) bHandled = FALSE;
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT CWindowUI::OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT CWindowUI::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT CWindowUI::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		if (!::IsZoomed(*this)) {
			RECT rcSizeBox = paint_manager_.GetSizeBox();
			if (pt.y < rcClient.top + rcSizeBox.top) {
				if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
				if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
				return HTTOP;
			}
			else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
				if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
				if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			}
			if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
		}

		RECT rcCaption = paint_manager_.GetCaptionRect();
		if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(pt));
			if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 &&
				_tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
				_tcscmp(pControl->GetClass(), DUI_CTR_TEXT) != 0)
				return HTCAPTION;
		}

		return HTCLIENT;
	}

	LRESULT CWindowUI::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-rcWork.left, -rcWork.top);

		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMaxPosition.x = rcWork.left;
		lpMMI->ptMaxPosition.y = rcWork.top;
		lpMMI->ptMaxSize.x = rcWork.right;
		lpMMI->ptMaxSize.y = rcWork.bottom;

		bHandled = FALSE;
		return 0;
	}

	LRESULT CWindowUI::OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
#endif


	LRESULT CWindowUI::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWindowUI::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWindowUI::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWindowUI::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWindowUI::OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWindowUI::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWindowUI::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = paint_manager_.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
		if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
			CDuiRect rcWnd;
			::GetWindowRect(*this, &rcWnd);
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(*this, hRgn, TRUE);
			::DeleteObject(hRgn);
		}
#endif
		bHandled = FALSE;
		return 0;
	}


	LRESULT CWindowUI::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == SC_CLOSE) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
#if defined(WIN32) && !defined(UNDER_CE)
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if (::IsZoomed(*this) != bZoomed) {
		}
#else
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
		return lRes;
	}


	LRESULT CWindowUI::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

		paint_manager_.Init(this->GetHWND());

		CDialogBuilder builder;

		CControlUI* pRoot = builder.Create(skin_xml_.GetData(), (UINT)0, this, &paint_manager_);
		if (pRoot)
			paint_manager_.AttachDialog(pRoot);
		else
		{
			ASSERT(FALSE);
			// TODO
			//LOGE("faild to parser xml error:" << builder.GetLastError());
		}
		Init();
		return 0;
	}

	LRESULT CWindowUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LPCTSTR CWindowUI::GetWindowClassName() const
	{
		return _T("LuaDuiWindowClass");
	}

	std::map<HWND, CWindowUI*> CWindowUI::GetWindows()
	{
		return CWindowUI::m_windows;
	}

	LRESULT CWindowUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch (uMsg)
		{
		case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
		case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEWHEEL:		lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
#endif
		case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN:		lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
		case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_SETFOCUS:		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_LBUTTONUP:		lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
		case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEMOVE:		lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
		default:				bHandled = FALSE; break;
		}
		if (bHandled) return lRes;

		lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
		if (bHandled) return lRes;

		if (paint_manager_.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}


	CPaintManagerUI* CWindowUI::GetManager()
	{
		return &paint_manager_;
	}

	void CWindowUI::SetIcon(CDuiString name = nullptr)
	{
		if (m_pIconInfo) {
			::DestroyIcon(m_pIconInfo);
			m_pIconInfo = NULL;
		}

		m_pIconInfo = static_cast<HICON>(::LoadImage(CApplicationUI::GetInstance(), name.GetData(), IMAGE_ICON, 0, 0,
			LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_DEFAULTSIZE));

		NotiyIcon(m_pIconInfo);
	}

	void CWindowUI::NotiyIcon(HICON icon)
	{
		if(icon)
		{
			::PostMessage(this->m_hWnd, WM_SETICON, (WPARAM)FALSE, (LPARAM)icon);
		}
	}










}