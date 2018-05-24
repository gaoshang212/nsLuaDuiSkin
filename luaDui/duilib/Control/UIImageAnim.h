#ifndef ImageAnimUI_h__
#define ImageAnimUI_h__

#pragma once
#include "UIlib.h"

namespace DuiLib
{
	class DUILIB_API CImageAnimUI: public CControlUI
	{
	public:
		struct AnimFrame
		{
			CDuiString      m_pathImage;    //图路径
			Gdiplus::Image *m_pImage;       //图
			UINT    m_spantime;             //间隔

		};
		typedef std::vector<AnimFrame> AnimFrameList;

		LBIND_CLASS_DEFINE(CImageAnimUI, CControlUI);

		CImageAnimUI(void);
		~CImageAnimUI(void);

		LPCTSTR GetClass() const;
		LPVOID  GetInterface(LPCTSTR pstrName);
		void    DoInit() override;
		//void    DoPaint(HDC hDC, const RECT& rcPaint) ;
		bool	DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) override;
		void    DoEvent(TEventUI& event) override;
		void    SetVisible(bool bVisible = true) override;
		void    SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		void    SetBkImage(LPCTSTR pStrImage);
		LPCTSTR GetBkImage();
		void    SetAutoPlay(bool bIsAuto = true);
		bool    IsAutoPlay() const;
		void    SetAutoSize(bool bIsAuto = true);
		bool    IsAutoSize() const;
		void    Play();
		void    Pause();
		void    Stop();

	private:
		void    InitGifImage();
		void    Delete();
		void    OnTimer(UINT_PTR idEvent);
		void    DrawFrame(HDC hDC);       // 绘制GIF每帧
		void    LoadAnimXmlFromFile(LPCTSTR pstrGifPath);
		void    initAnimXmlFromMemory(LPVOID pBuf, size_t dwSize);

		Gdiplus::Image* LoadImageFromFile(LPCTSTR pstrGifPath);
	private:

		UINT            m_nFramePosition;           // 当前放到第几帧

		CDuiString      m_aniName;                  //动画名
		CDuiString      m_bkImage;
		bool            m_bIsWebp;
		bool            m_bIsAutoPlay;              // 是否自动播放gif
		bool            m_bIsAutoSize;              // 是否自动根据图片设置大小
		bool            m_bIsPlaying;
		bool			m_bIsLoop;


		AnimFrameList   m_framelist;
	};
}

#endif // ImageAnimUI_h__