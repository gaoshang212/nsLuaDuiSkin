#include "UIImageAnim.h"
#include "stdafx.h"
#include "UIImageAnim.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime, ctime, mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{
	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime, ctime, mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z, unsigned int len, DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);
///////////////////////////////////////////////////////////////////////////////////////

namespace DuiLib
{

	CImageAnimUI::CImageAnimUI(void)
	{
		m_nFramePosition = 0;
		m_bIsWebp = false;
		m_bIsAutoPlay = false;
		m_bIsAutoSize = false;
		m_bIsPlaying = false;
		m_bIsLoop = false;
	}


	CImageAnimUI::~CImageAnimUI(void)
	{
		Delete();
		m_pManager->KillTimer(this, EVENT_TIEM_ID);

	}

	LPCTSTR CImageAnimUI::GetClass() const
	{
		return _T("CImageAnimUI");
	}

	LPVOID CImageAnimUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_IMAGEANIM) == 0) return static_cast<CImageAnimUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CImageAnimUI::DoInit()
	{
		InitGifImage();
	}

	bool CImageAnimUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)  //CImageAnimUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return false;
		if (m_framelist.empty())
		{
			InitGifImage();
		}
		DrawFrame(hDC);

		return true;
	}

	void CImageAnimUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_TIMER)
			OnTimer((UINT_PTR)event.wParam);
	}

	void CImageAnimUI::SetVisible(bool bVisible /* = true */)
	{
		CControlUI::SetVisible(bVisible);
		if (bVisible)
			Play();
		else
			Stop();
	}

	void CImageAnimUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("bkimage")) == 0) SetBkImage(pstrValue);
		else if (_tcscmp(pstrName, _T("autoplay")) == 0) {
			SetAutoPlay(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if (_tcscmp(pstrName, _T("autosize")) == 0) {
			SetAutoSize(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else
			CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CImageAnimUI::SetBkImage(LPCTSTR pStrImage)
	{
		if (m_bkImage == pStrImage || NULL == pStrImage) return;

		m_bkImage = pStrImage;

		Stop();
		Delete();

		Invalidate();

	}

	LPCTSTR CImageAnimUI::GetBkImage()
	{
		return m_bkImage.GetData();
	}

	void CImageAnimUI::SetAutoPlay(bool bIsAuto)
	{
		m_bIsAutoPlay = bIsAuto;
	}

	bool CImageAnimUI::IsAutoPlay() const
	{
		return m_bIsAutoPlay;
	}

	void CImageAnimUI::SetAutoSize(bool bIsAuto)
	{
		m_bIsAutoSize = bIsAuto;
	}

	bool CImageAnimUI::IsAutoSize() const
	{
		return m_bIsAutoSize;
	}

	void CImageAnimUI::Play()
	{

		if (!m_bIsPlaying && m_framelist.size() > m_nFramePosition)
		{
			long lPause = m_framelist[m_nFramePosition].m_spantime;
			if (lPause == 0) lPause = 100;
			m_pManager->SetTimer(this, EVENT_TIEM_ID, lPause);

			m_bIsPlaying = true;
		}
	}

	void CImageAnimUI::Pause()
	{
		if (!m_bIsPlaying)
		{
			return;
		}

		m_pManager->KillTimer(this, EVENT_TIEM_ID);
		//this->Invalidate();
		m_bIsPlaying = false;
	}

	void CImageAnimUI::Stop()
	{
		if (!m_bIsPlaying)
		{
			return;
		}

		m_pManager->KillTimer(this, EVENT_TIEM_ID);
		m_nFramePosition = 0;
		this->Invalidate();
		m_bIsPlaying = false;
	}

	void CImageAnimUI::InitGifImage()
	{
		LoadAnimXmlFromFile(GetBkImage());


		if (m_bIsAutoPlay)
		{
			Play();
		}
	}

	void CImageAnimUI::Delete()
	{
		for (auto& it : m_framelist)
		{
			if (it.m_pImage)
			{
				delete it.m_pImage;
				it.m_pImage = nullptr;
			}
		}
		m_framelist.clear();
		m_nFramePosition = 0;
	}

	void CImageAnimUI::OnTimer(UINT_PTR idEvent)
	{
		if (idEvent != EVENT_TIEM_ID)
			return;
		m_pManager->KillTimer(this, EVENT_TIEM_ID);

		m_nFramePosition++;
		if (m_bIsLoop)
		{
			m_nFramePosition = m_nFramePosition % m_framelist.size();
		}

		if (m_nFramePosition >= m_framelist.size())
		{
			Pause();
			DoLuaEvent("completed");
			return;
		}

		this->Invalidate();

		long lPause = m_framelist[m_nFramePosition].m_spantime;
		if (lPause == 0) lPause = 100;
		m_pManager->SetTimer(this, EVENT_TIEM_ID, lPause);
	}

	void CImageAnimUI::DrawFrame(HDC hDC)
	{
		if (NULL == hDC) return;

		if (m_framelist.size() > m_nFramePosition && m_framelist[m_nFramePosition].m_pImage)
		{
			Gdiplus::Graphics graphics(hDC);
			graphics.DrawImage(m_framelist[m_nFramePosition].m_pImage, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
		}
	}

	void CImageAnimUI::LoadAnimXmlFromFile(LPCTSTR pstrGifPath)
	{
		LPBYTE pData = NULL;
		DWORD dwSize = 0;

		do
		{
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			if (CPaintManagerUI::GetResourceZip().IsEmpty()) {
				sFile += pstrGifPath;
				HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
					FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE) break;
				dwSize = ::GetFileSize(hFile, NULL);
				if (dwSize == 0) break;

				DWORD dwRead = 0;
				pData = new BYTE[dwSize];
				::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
				::CloseHandle(hFile);

				if (dwRead != dwSize) {
					delete[] pData;
					pData = NULL;
					break;
				}
			}
			else {
				sFile += CPaintManagerUI::GetResourceZip();
				HZIP hz = NULL;
				if (CPaintManagerUI::IsCachedResourceZip()) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
				else hz = OpenZip((void*)sFile.GetData(), 0, 2);
				if (hz == NULL) break;
				ZIPENTRY ze;
				int i;
				if (FindZipItem(hz, pstrGifPath, true, &i, &ze) != 0) break;
				dwSize = ze.unc_size;
				if (dwSize == 0) break;
				pData = new BYTE[dwSize];
				int res = UnzipItem(hz, i, pData, dwSize, 3);
				if (res != 0x00000000 && res != 0x00000600) {
					delete[] pData;
					pData = NULL;
					if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
					break;
				}
				if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
			}

		} while (0);

		//if (CPaintManagerUI::GetResourceZip().IsEmpty())
		//{
		//	pData = ThirdParty::LoadFromFile(pstrGifPath, dwSize);
		//}
		//else
		//{
		//	pData = ThirdParty::LoadFromZip(pstrGifPath, dwSize);
		//}

		if (!pData)
		{
			return;
		}

		initAnimXmlFromMemory(pData, dwSize);
		delete pData;

	}

	void CImageAnimUI::initAnimXmlFromMemory(LPVOID pBuf, size_t dwSize)
	{
		CMarkup markup;
		markup.LoadFromMem((BYTE*)pBuf, dwSize);

		SIZE_T cchMax = 128;
		TCHAR pstrMessage[128]; // = new LPTSTR[cchMax];
		markup.GetLastErrorMessage(pstrMessage, 128);
		markup.GetLastErrorLocation(pstrMessage, 128);


		LPCTSTR pstrClass = NULL;
		int nAttributes = 0;
		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		LPTSTR pstr = NULL;
		CMarkupNode root = markup.GetRoot();

		nAttributes = root.GetAttributeCount();
		for (int i = 0; i < nAttributes; i++) {
			pstrName = root.GetAttributeName(i);
			pstrValue = root.GetAttributeValue(i);
			if (_tcscmp(pstrName, _T("name")) == 0) {
				m_aniName = pstrValue;
			}
			else if (_tcscmp(pstrName, _T("loop")) == 0) {
				m_bIsLoop = (_tcscmp(pstrValue, _T("true")) == 0 || _tcscmp(pstrValue, _T("True")) == 0 || _tcscmp(pstrValue, _T("TRUE")) == 0);
			}
			/*	else if (_tcscmp(pstrName, _T("iswebp")) == 0) {
					m_bIsWebp = (_tcscmp(pstrValue, _T("true")) == 0 || _tcscmp(pstrValue, _T("True")) == 0 || _tcscmp(pstrValue, _T("TRUE")) == 0);
				}*/
		}


		for (CMarkupNode node = root.GetChild(); node.IsValid(); node = node.GetSibling()) {
			pstrClass = node.GetName();
			if (_tcscmp(pstrClass, _T("frame")) == 0) {
				AnimFrame frame;
				nAttributes = node.GetAttributeCount();
				LPCTSTR pPathImage = NULL;
				DWORD time = 0;
				for (int i = 0; i < nAttributes; i++) {
					pstrName = node.GetAttributeName(i);
					pstrValue = node.GetAttributeValue(i);
					if (_tcscmp(pstrName, _T("image")) == 0) {
						pPathImage = pstrValue;
					}
					else if (_tcscmp(pstrName, _T("time")) == 0) {
						time = _tcstol(pstrValue, &pstr, 10);
					}
				}
				if (pPathImage)
				{
					frame.m_pathImage = pPathImage;
					frame.m_spantime = time;
					frame.m_pImage = LoadImageFromFile(pPathImage);
					m_framelist.push_back(frame);
				}
			}
		}

		return;
	}

	Gdiplus::Image* CImageAnimUI::LoadImageFromFile(LPCTSTR pstrGifPath)
	{
		LPBYTE pData = NULL;
		DWORD dwSize = 0;

		do
		{
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			if (CPaintManagerUI::GetResourceZip().IsEmpty()) {
				sFile += pstrGifPath;
				HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
					FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE) break;
				dwSize = ::GetFileSize(hFile, NULL);
				if (dwSize == 0) break;

				DWORD dwRead = 0;
				pData = new BYTE[dwSize];
				::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
				::CloseHandle(hFile);

				if (dwRead != dwSize) {
					delete[] pData;
					pData = NULL;
					break;
				}
			}
			else {
				sFile += CPaintManagerUI::GetResourceZip();
				HZIP hz = NULL;
				if (CPaintManagerUI::IsCachedResourceZip()) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
				else hz = OpenZip((void*)sFile.GetData(), 0, 2);
				if (hz == NULL) break;
				ZIPENTRY ze;
				int i;
				if (FindZipItem(hz, pstrGifPath, true, &i, &ze) != 0) break;
				dwSize = ze.unc_size;
				if (dwSize == 0) break;
				pData = new BYTE[dwSize];
				int res = UnzipItem(hz, i, pData, dwSize, 3);
				if (res != 0x00000000 && res != 0x00000600) {
					delete[] pData;
					pData = NULL;
					if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
					break;
				}
				if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
			}

		} while (0);

		//if (CPaintManagerUI::GetResourceZip().IsEmpty())
		//{
		//	pData = ThirdParty::LoadFromFile(pstrGifPath, dwSize);
		//}
		//else
		//{
		//	pData = ThirdParty::LoadFromZip(pstrGifPath, dwSize);
		//}

		if (!pData)
		{
			return nullptr;
		}

		Gdiplus::Image *pImg = nullptr;

		HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, dwSize);
		BYTE* pMem = (BYTE*)::GlobalLock(hMem);

		memcpy(pMem, pData, dwSize);
		std::cout << "loadmemory size:" << dwSize << std::endl;

		IStream* pStm = NULL;
		::CreateStreamOnHGlobal(hMem, TRUE, &pStm);
		pImg = Gdiplus::Image::FromStream(pStm);
		if (!pImg || pImg->GetLastStatus() != Gdiplus::Ok)
		{
			pStm->Release();
			::GlobalUnlock(hMem);
			return nullptr;
		}

		delete pData;
		return pImg;
	}
}
