/*
*  mspring.frame.h
*  MSpring
*
*  Created by kimbom on 2017. 9. 12...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_MSPRINGFRAME_HPP_INCLUDED)
#define MSPRING_7E1_9_C_MSPRINGFRAME_HPP_INCLUDED

#include<afxwin.h>
#include<vector>
#include<iostream>
#include<algorithm>
#include<memory>
#include<string>
#include<array>
#include<map>
#include"../utils/utils.h"
#include"MSpringResource.h"
#include"MSpringResource01.h"
#include"MSpringResource02.h"
#include"MSpringResource03.h"
#include"MSpringColormap.h"

#define RESOURCE_CLOSE -1
#define RESOURCE_MAXIMIZE -2
#define RESOURCE_MINIMIZE -3

inline unsigned char* GetColorMap(DWORD mspcmap) {
	switch (mspcmap) {
		case 16777216:return g_COLORMAP_AUTUMN;
		case 16777217:return g_COLORMAP_BONE;
		case 16777218:return g_COLORMAP_JET;
		case 16777219:return g_COLORMAP_WINTER;
		case 16777220:return g_COLORMAP_RAINBOW;
		case 16777221:return g_COLORMAP_OCEAN;
		case 16777222:return g_COLORMAP_SUMMER;
		case 16777223:return g_COLORMAP_SPRING;
		case 16777224:return g_COLORMAP_COOL;
		case 16777225:return g_COLORMAP_HSV;
		case 16777226:return g_COLORMAP_PINK;
		case 16777227:return g_COLORMAP_HOT;
		case 16777228:return g_COLORMAP_PARULA;
		default:return nullptr;
	}
}
inline void SetWindowBlur(HWND hWnd,COLORREF color,bool disable=false) {
	const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
	if (hModule) {
		struct ACCENTPOLICY {
			int nAccentState;
			int nFlags;
			int nColor;
			int nAnimationId;
		};
		struct WINCOMPATTRDATA {
			int nAttribute;
			PVOID pData;
			ULONG ulDataSize;
		};
		enum AccentState {
			ACCENT_DISABLED = 0,
			ACCENT_ENABLE_GRADIENT = 1,
			ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
			ACCENT_ENABLE_BLURBEHIND = 3,
			ACCENT_ENABLE_ACRYLICBLURBEHIND = 4
		};
		typedef BOOL(WINAPI*pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
		const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hModule, "SetWindowCompositionAttribute");
		if (SetWindowCompositionAttribute) {
			ACCENTPOLICY policy = { 0 };
			if (disable == true) {
				policy.nAccentState = AccentState::ACCENT_DISABLED;
			} else {
				policy.nAccentState = AccentState::ACCENT_ENABLE_ACRYLICBLURBEHIND;
			}
			policy.nFlags = 0x20 | 0x40 | 0x80 | 0x100;
			policy.nColor = (color) | (0xAF << 24);
			WINCOMPATTRDATA data = { 19, &policy, sizeof(ACCENTPOLICY) }; // WCA_ACCENT_POLICY=19
			SetWindowCompositionAttribute(hWnd, &data);
		}
		FreeLibrary(hModule);
	}
}
template<class T>
T CreateFrame(CWnd* pWnd) {
	return T(new T::element_type(pWnd));
}
#define REDRAW_NCAREA this->OnNcPaint()
class MSpringFrameExpansion {
	/*
	*	@class : MSpringFrameExpansion
	*	@comment : 이 클래스는 BMDLFrame에 확장 기능을 부여하기 위해 사용됩니다.
	*				 , 이 클래스를 상속 받아 각 메세지 콜백 함수에 대한 처리기를 만드십시오.
	*	@warning : OnNcPaint의 경우 반환값과 매개변수가 원래의 OnNcPaint와 다릅니다 .유의 하십시오.
	*/

protected:
	CWnd* m_wnd;
public:
	MSpringFrameExpansion(CWnd* wnd) :m_wnd(wnd){}
	virtual bool PtInRect() = 0;
	virtual void OnCreate(LPCREATESTRUCT lpCreateStruct) = 0;
	/*
	*	@method : OnNcPaint
	*	@param
	*		* pDC : 윈도우 캡션에 그림을 그릴수 있는 Device Context 입니다.
	*		* rect : 이 함수가 그릴수 있는(그려야 하는) 영역을 나타냅니다.
	*	@return : 주어진 영역에서 사용한 만큼의 x너비를 반환해야 합니다. 양수의 경우 left-align, 음수의 경우 right-align 입니다.
	*/
	virtual int OnNcPaint(CDC* pDC,CRect rect) = 0;
	virtual void OnSize(UINT nType, int cx, int cy) = 0;
	//아래 마우스 이벤트 함수들의 매개변수 point 는 윈도우에 대한 상대 좌표가 입력 됩니다.
	virtual bool OnNcLButtonDown(UINT nHitTest, CPoint point) = 0;	
	virtual bool OnNcMouseMove(UINT nHitTest, CPoint point) = 0;
	virtual bool OnNcLButtonUp(UINT nHitTest, CPoint point) = 0;
	virtual bool OnNcLButtonDblClk(UINT nHitTest, CPoint point) = 0;
	virtual void OnNcMouseLeave() = 0;
	virtual void OnDestroy() = 0;
};

enum MSpringThemeStory {
	Flat=0,
	Material,
	Fluent
};
__declspec(selectany) MSpringThemeStory g_theme_story = MSpringThemeStory::Flat;
#define MSPRING_DISABLE_HTTEST	33333
__declspec(selectany) double g_maximized_time = 0.0;			//창이 최대화 상태가 된 시간입니다. (최대화후 몇 ms 간은 MouseMove를 동작하지 않습니다.)
class MSpringFrame : public CFrameWnd {
protected:
	DECLARE_DYNAMIC(MSpringFrame)
public://constructor,destructor
	MSpringFrame() {
		m_color_transparent = RGB(255, 255, 255);
		m_color_bk = RGB(83, 83, 83);
		m_color_border = RGB(0, 216, 255);
	}
	virtual ~MSpringFrame() = default;
private://inner class
	class SystemButton {
		/*
		*	@class : SystemButton
		*	@comment : 시스템 버튼을 나타내는 클래스 입니다.
		*/
	public:
		enum State {Normal,Hover,Click};
		CRect m_rect;					//시스템 버튼이 차지하는 영역입니다.
		State m_state;					//시스템 버튼의 상태입니다.
		int m_bitmap_resource = 0;	//시스템 버튼에 사용되는 리소스의 ID 입니다.
	private:
		void(*Event)(CWnd* frame) = nullptr;	//시스템 버튼을 클릭했을때 실행되는 함수입니다.
	public:
		void Do(CWnd* param = nullptr) {
			if (Event != nullptr) {
				Event(param);
			}
		}
		SystemButton(int r, void(*EventFunction)(CWnd*) = nullptr) {
			m_bitmap_resource = r;
			m_state = State::Normal;
			Event = EventFunction;
		}
	};
	struct AppIcon {
		/*
		*	@class : AppIcon
		*	@comment : Application의 아이콘을 나타냅니다.
		*/
		HICON m_icon = nullptr;
		CRect m_rect;
	};
	std::vector<SystemButton> m_sysbtn;		//시스템 버튼
	void ReplaceSystemButtonState(SystemButton::State curr, SystemButton::State val) {
		for (int i = 0; i < (int)this->m_sysbtn.size(); i++) {
			if (this->m_sysbtn[i].m_state == curr) {
				this->m_sysbtn[i].m_state = val;
			}
		}
	}


protected:
	AppIcon m_icon;								//앱 아이콘
protected:	
	std::array<mspring::DoubleBufferingDC*,4> m_dbb;			//상,하,좌,우 를 각각 그리는 Device Context 입니다.
	bool m_is_maximize = false;				//현재 창이 최대화 상태인지 나타냅니다.
	std::vector<std::shared_ptr<MSpringFrameExpansion>> m_expansion;	//확장 기능 클래스 입니다.
	CRect m_blank_caption;						//OnNcDblClk 함수에서 사용합니다.
protected:	//coord value
	int m_sysbtn_margin = 4;					//시스템 버튼 사이의 간격입니다.
	int m_icon_margin = 4;						//아이콘의 margin 입니다.
	
protected:	//style value
	COLORREF m_color_transparent;			//시스템버튼의 투명색을 지정합니다.
	COLORREF m_color_bk;						//프레임의 배경색입니다.
	COLORREF m_color_border;					//경계색의 색상입니다.
	COLORREF m_color_text;
	COLORREF m_color_title;
	TString m_font_str;
	TString m_title = TEXT("MSpring");
	float m_title_height_ratio = 1.0F;
	bool m_other_task = false;				//탭이나 메뉴 등을 클릭했을때 true가 됩니다.
	bool m_disable_httest = false;
public:
	virtual void SetHtTest(bool b) {
		m_disable_httest = b;
	}
public:		//static method
	static void ButtonEvent_Close(CWnd* wnd) {
		::AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);
	}
	static void ButtonEvent_MaximizeWindow(CWnd* wnd) {
		MSpringFrame* pmainframe = dynamic_cast<MSpringFrame*>(wnd);
		if (pmainframe->m_is_maximize == false) {
			
			
			static CRect rect_window;
			pmainframe->GetWindowRect(rect_window);
#ifdef _M_AMD64
			::SetWindowLongPtr(pmainframe->GetSafeHwnd(), GWLP_USERDATA, (LONG_PTR)&rect_window);
			//If the function fails, the return value is zero. To get extended error information, call GetLastError. 
#else
			::SetWindowLongA(pmainframe->GetSafeHwnd(), GWL_USERDATA, (LONG)&rect_window);
			//If the function fails, the return value is zero. To get extended error information, call GetLastError. 
#endif
			
			MONITORINFOEXA monitor;
			monitor.cbSize = sizeof(MONITORINFOEXA);
			CRect rect;
			wnd->GetWindowRect(&rect);
			HMONITOR hMOnitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
			GetMonitorInfoA(hMOnitor, &monitor);
			g_maximized_time = clock();			
			wnd->MoveWindow(&monitor.rcWork,FALSE);
		} else {
#ifdef _M_AMD64
			CRect* rect_window = (CRect*)::GetWindowLongPtr(pmainframe->GetSafeHwnd(), GWLP_USERDATA);
#else
			CRect* rect_window = (CRect*)::GetWindowLongA(pmainframe->GetSafeHwnd(), GWL_USERDATA);
#endif
			pmainframe->MoveWindow(*rect_window);
		}
		pmainframe->m_is_maximize = !pmainframe->m_is_maximize;
		pmainframe->ReplaceSystemButtonState(SystemButton::State::Hover, SystemButton::State::Normal);
		pmainframe->ReplaceSystemButtonState(SystemButton::State::Click, SystemButton::State::Normal);
	}
	static void ButtonEvent_MinimizeWindow(CWnd* wnd) {
		wnd->SendMessage(WM_SYSCOMMAND, (WPARAM)SC_MINIMIZE);
	}
protected:		//Mouse, Window-size method
	//현재 마우스의 상대적 위치를 가져옵니다.
	CPoint GetMousePoint() {
		CPoint point;
		CRect rect;
		this->GetWindowRect(&rect);
		::GetCursorPos(&point);
		point.x -= rect.left;
		point.y -= rect.top;
		return point;
	}
	//윈도우 캡션의 상대적 영역을 가져옵니다.
	CRect GetWindowCaptionRect() {
		CRect rect_caption = this->GetWindowNomalizedRect();
		rect_caption.top = GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYFRAME);
		rect_caption.bottom = rect_caption.top + GetSystemMetrics(SM_CYCAPTION);
		return rect_caption;
	}
	//윈도우의 상대적 영역을 가져옵니다.
	CRect GetWindowNomalizedRect() {
		CRect rect;
		this->GetWindowRect(&rect);
		rect.NormalizeRect();
		rect.OffsetRect(-rect.left, -rect.top);
		return rect;
	}
public:	
	//시스템버튼의 마진을 지정합니다.
	void SetSysBtnMargin(int margin) {
		m_sysbtn_margin = margin;
	}
	//아이콘의 마진을 지정합니다.
	void SetIconMargin(int margin) {
		m_icon_margin = margin;
	}
	//아이콘을 지정합니다.
	void SetIcon(UINT id) {
		m_icon.m_icon = ::AfxGetApp()->LoadIcon(id);
	}
	//스타일을 설정합니다.
	void SetStyle(TString font_str,COLORREF color_bk,COLORREF color_text ,COLORREF color_border, COLORREF color_transparent = RGB(255, 255, 255)) {
		m_font_str = font_str;
		m_color_bk = color_bk;
		m_color_text = color_text;
		m_color_border = color_border;
		m_color_transparent = color_transparent;
	}
	void SetTitle(TString title,float title_height_ratio=1.0F) {
		VERIFY(title_height_ratio > 0.0F);
		VERIFY(title_height_ratio <= 1.0F);
		m_title = title;
		m_title_height_ratio = title_height_ratio;
	}
	void SetTitleColor(COLORREF title_color) {
		m_color_title = title_color;
	}
	//시스템버튼을 추가합니다.
	void AddSysBtn(int bitmap, void(*EventFunc)(CWnd*)) {
		m_sysbtn.push_back(SystemButton(bitmap, EventFunc));
	}
	void ClearSysBtn() {
		m_sysbtn.clear();
	}
	//확장 클래스를 추가합니다.
	void AddExpansionClass(std::shared_ptr<MSpringFrameExpansion> _class) {
		m_expansion.push_back(_class);
	}
public:	///About Theme Story
	void SetAcrylicThemeAsBackgroundColor() {
		g_theme_story = MSpringThemeStory::Fluent;
		::SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, ::GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
		this->SetLayeredWindowAttributes(m_color_bk, 255, LWA_COLORKEY);
		
		
		SetWindowBlur(this->GetSafeHwnd(), m_color_bk);
	}
	void SetAcrylicTheme(COLORREF color) {
		g_theme_story = MSpringThemeStory::Fluent;
		::SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, ::GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
		this->SetLayeredWindowAttributes(color, 255, LWA_COLORKEY);
		SetWindowBlur(this->GetSafeHwnd(), color);
	}
	void SetMaterialTheme() {
		g_theme_story = MSpringThemeStory::Material;
		this->SetLayeredWindowAttributes(-1, 255, LWA_COLORKEY);
		SetWindowBlur(this->GetSafeHwnd(), -1,true);
	}
	void SetFlatTheme() {
		g_theme_story = MSpringThemeStory::Flat;
		this->SetLayeredWindowAttributes(-1, 255, LWA_COLORKEY);
		SetWindowBlur(this->GetSafeHwnd(), -1, true);
	}
public:	//messageevent method
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) {
		if (!CFrameWnd::PreCreateWindow(cs)) {
			return FALSE;
		}
		cs.dwExStyle &= ~WS_EX_CLIENTEDGE;	//테두리를 제거합니다.
		cs.lpszClass = AfxRegisterWndClass(0);
		if (cs.hMenu != NULL) {						//메뉴를 제거합니다.
			::DestroyMenu(cs.hMenu);
			cs.hMenu = NULL;
		}
		return TRUE;
	}
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct) {
		if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
			return -1;
		}
		this->ModifyStyle(WS_SYSMENU, 0);	//시스템 메뉴(종료,최대화,최소화) 버튼을 제거합니다.
		for (auto&e : m_expansion) {
			e->OnCreate(lpCreateStruct);
		}
		//WNDCLASSEXA wcex;
		//ZeroMemory(&wcex, sizeof(wcex));
		//wcex.cbSize = sizeof(WNDCLASSEXA);
		//wcex.style = CS_HREDRAW | CS_VREDRAW;
		//wcex.lpfnWndProc = ::DefWindowProc;
		//wcex.cbClsExtra = 0;
		//wcex.cbWndExtra = 0;
		//wcex.hInstance = ::GetModuleHandle(NULL);
		//wcex.hIcon = NULL;
		//wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		//wcex.lpszMenuName = NULL;
		//wcex.lpszClassName = m_strFakeWndClassName.c_str();
		//wcex.hIconSm = NULL;

		//RegisterClassExA(&wcex);
		//CRect rect;
		//this->GetWindowRect(&rect);
		//m_hFakeWnd = ::CreateWindowExA(WS_EX_NOACTIVATE | WS_EX_LEFT
		////m_hFakeWnd=::CreateWindowExA(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_LEFT
		//							, m_strFakeWndClassName.c_str()
		//							, NULL
		//							, WS_VISIBLE | WS_OVERLAPPED
		//							, rect.left
		//							, rect.top
		//							, rect.Width()
		//							, rect.Height()
		//							, GetSafeHwnd()
		//							, NULL
		//							, ::GetModuleHandle(NULL)
		//							, NULL
		//);
		//SetWindowBlur(m_hFakeWnd);

		return 0;
	}
	afx_msg void OnNcPaint() {
		
		WINDOWPLACEMENT place;
		place.length = (UINT)sizeof(WINDOWPLACEMENT);
		if (GetWindowPlacement(&place) && place.showCmd==SW_SHOWMINIMIZED) {
			return;
		}
		auto Normalize = [](CRect r)->CRect {
			r.OffsetRect(-r.left, -r.top);
			r.NormalizeRect();
			return r;
		};
		CWindowDC _ncpaint(this);
		CWindowDC* ncpaint = &_ncpaint;

		CRect rect_window = this->GetWindowNomalizedRect();
		
		CRect rect_window_top = {rect_window.left,rect_window.top,rect_window.right
			,GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYSIZEFRAME)
		};
		CRect rect_window_bottom = {rect_window.left
			,rect_window.bottom - GetSystemMetrics(SM_CYFRAME) - GetSystemMetrics(SM_CYSIZEFRAME)
			,rect_window.right,rect_window.bottom
		};
		CRect rect_window_left = {rect_window.left,rect_window_top.bottom
			,GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXSIZEFRAME)
			,rect_window_bottom.top
		};
		CRect rect_window_right = {
			rect_window.right - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXSIZEFRAME)
			,rect_window_top.bottom,rect_window.right,rect_window_bottom.top
		};
		mspring::DoubleBufferingDC* dbb_top = new mspring::DoubleBufferingDC(ncpaint, rect_window_top);
		mspring::DoubleBufferingDC* dbb_bottom = new mspring::DoubleBufferingDC(ncpaint, rect_window_bottom);
		mspring::DoubleBufferingDC* dbb_left = new mspring::DoubleBufferingDC(ncpaint, rect_window_left);
		mspring::DoubleBufferingDC* dbb_right = new mspring::DoubleBufferingDC(ncpaint, rect_window_right);
		{
			CBrush brush;
			brush.CreateSolidBrush(m_color_bk);

			dbb_top->getDC().FillRect(Normalize(rect_window_top), &brush);
			dbb_bottom->getDC().FillRect(Normalize(rect_window_bottom), &brush);
			dbb_left->getDC().FillRect(Normalize(rect_window_left), &brush);
			dbb_right->getDC().FillRect(Normalize(rect_window_right), &brush);
			

			brush.DeleteObject();
		}
		//여기까지가 프레임 그리기.
		if ((m_color_border&MSP_COLOR_MAP) == MSP_COLOR_MAP) {
				auto GradientLine = [&Normalize](mspring::DoubleBufferingDC* dbb, DWORD color_border, CPoint point_beg, CPoint point_end)->void {
					int inc = 1;
					int beg = 0;
					int end = 256;
					if (color_border >= 16777229) {
						color_border -= 13;
						inc = -1;
						beg = 255;
						end = -1;
					}
					unsigned char* colormap = GetColorMap(color_border);
					dbb->getDC().MoveTo(point_beg);
					float X = static_cast<float>(point_beg.x);
					float Y = static_cast<float>(point_beg.y);
					for (int i = beg; i != end; i += inc) {
						CPen pen, *old_pen;
						int _B = colormap[i * 3 + 0];
						int _G = colormap[i * 3 + 1];
						int _R = colormap[i * 3 + 2];
						pen.CreatePen(PS_SOLID, 2, RGB(_R, _G, _B));
						old_pen = dbb->getDC().SelectObject(&pen);
						CPoint diff = point_end - point_beg;
						float rtox = ((diff.y == 0) ? diff.x : 0) / 256.0F;
						float rtoy = ((diff.x == 0) ? diff.y : 0) / 256.0F;
						X += rtox;
						Y += rtoy;
						dbb->getDC().LineTo(CPoint(static_cast<decltype(CPoint::x)>(X), static_cast<decltype(CPoint::y)>(Y)));
						pen.DeleteObject();
					}
				};
				DWORD line_first, line_second;
				if (m_color_border == MSP_COLORMAP_AUTUMN) {
					line_first = MSP_COLORMAP_AUTUMN;
					line_second = MSP_COLORMAP_AUTUMN_R;
				}
				if (m_color_border == MSP_COLORMAP_BONE) {
					line_first = MSP_COLORMAP_BONE;
					line_second = MSP_COLORMAP_BONE_R;
				}
				if (m_color_border == MSP_COLORMAP_COOL || m_color_border == MSP_COLORMAP_SPRING) {
					line_first = MSP_COLORMAP_COOL;
					line_second = MSP_COLORMAP_SPRING;
				}
				if (m_color_border == MSP_COLORMAP_HOT) {
					line_first = MSP_COLORMAP_HOT;
					line_second = MSP_COLORMAP_HOT_R;
				}
				if (m_color_border == MSP_COLORMAP_HSV) {
					line_first = MSP_COLORMAP_HSV;
					line_second = MSP_COLORMAP_HSV_R;
				}
				if (m_color_border == MSP_COLORMAP_JET) {
					line_first = MSP_COLORMAP_JET;
					line_second = MSP_COLORMAP_JET_R;
				}
				if (m_color_border == MSP_COLORMAP_OCEAN) {
					line_first = MSP_COLORMAP_OCEAN;
					line_second = MSP_COLORMAP_OCEAN_R;
				}
				if (m_color_border == MSP_COLORMAP_PINK) {
					line_first = MSP_COLORMAP_PINK;
					line_second = MSP_COLORMAP_PINK_R;
				}
				if (m_color_border == MSP_COLORMAP_RAINBOW) {
					line_first = MSP_COLORMAP_RAINBOW;
					line_second = MSP_COLORMAP_RAINBOW_R;
				}
				if (m_color_border == MSP_COLORMAP_SUMMER) {
					line_first = MSP_COLORMAP_SUMMER;
					line_second = MSP_COLORMAP_SUMMER_R;
				}
				if (m_color_border == MSP_COLORMAP_WINTER) {
					line_first = MSP_COLORMAP_WINTER;
					line_second = MSP_COLORMAP_WINTER_R;
				}
				if (m_color_border == MSP_COLORMAP_PARULA) {
					line_first = MSP_COLORMAP_PARULA;
					line_second = MSP_COLORMAP_PARULA_R;
				}
				//top-side
				GradientLine(dbb_top, line_first
							 , CPoint(Normalize(rect_window_top).left, Normalize(rect_window_top).top + 1)
							 , CPoint(Normalize(rect_window_top).right, Normalize(rect_window_top).top + 1));
				GradientLine(dbb_top, line_second
							 , CPoint(Normalize(rect_window_top).right - 2, Normalize(rect_window_right).top)
							 , CPoint(Normalize(rect_window_top).right - 2, Normalize(rect_window_right).bottom));
				GradientLine(dbb_top, line_first
							 , CPoint(Normalize(rect_window_top).left + 1, Normalize(rect_window_left).top)
							 , CPoint(Normalize(rect_window_top).left + 1, Normalize(rect_window_left).bottom - 1));
				//LR-side
				GradientLine(dbb_left, line_first
							 , CPoint(Normalize(rect_window_left).left + 1, Normalize(rect_window_left).top)
							 , CPoint(Normalize(rect_window_left).left + 1, Normalize(rect_window_left).bottom));
				GradientLine(dbb_right, line_second
							 , CPoint(Normalize(rect_window_right).right - 2, Normalize(rect_window_right).top)
							 , CPoint(Normalize(rect_window_right).right - 2, Normalize(rect_window_right).bottom));
				//bottom-side
				GradientLine(dbb_bottom, line_second
							 , CPoint(Normalize(rect_window_bottom).left, Normalize(rect_window_bottom).bottom - 2)
							 , CPoint(Normalize(rect_window_bottom).right, Normalize(rect_window_bottom).bottom - 2));
				GradientLine(dbb_bottom, line_second
							 , CPoint(Normalize(rect_window_bottom).right - 2, Normalize(rect_window_right).bottom)
							 , CPoint(Normalize(rect_window_bottom).right - 2, Normalize(rect_window_right).top));
				GradientLine(dbb_bottom, line_first
							 , CPoint(Normalize(rect_window_bottom).left + 1, Normalize(rect_window_left).bottom)
							 , CPoint(Normalize(rect_window_bottom).left + 1, Normalize(rect_window_left).top));
			
		}else{
			CPen pen, *old_pen;
			int thickness = 1;
			pen.CreatePen(PS_SOLID, thickness, m_color_border);
			old_pen = dbb_top->getDC().SelectObject(&pen);
			
			dbb_top->getDC().MoveTo(Normalize(rect_window_top).left, Normalize(rect_window_top).bottom);
			dbb_top->getDC().LineTo(Normalize(rect_window_top).left, Normalize(rect_window_top).top);
			if ((m_color_border&MSP_COLOR_LINE) == MSP_COLOR_LINE) {
				CPen pen, *old_pen;
				pen.CreatePen(PS_SOLID, 6, m_color_border);
				old_pen = dbb_top->getDC().SelectObject(&pen);
				dbb_top->getDC().LineTo(Normalize(rect_window_top).right - 1 - thickness, Normalize(rect_window_top).top);
				dbb_top->getDC().SelectObject(old_pen);
				pen.DeleteObject();
			} else {
				dbb_top->getDC().LineTo(Normalize(rect_window_top).right - 1 - thickness, Normalize(rect_window_top).top);
			}
			dbb_top->getDC().LineTo(Normalize(rect_window_top).right - 1 - thickness, Normalize(rect_window_top).bottom);
			dbb_top->getDC().SelectObject(old_pen);

			old_pen = dbb_bottom->getDC().SelectObject(&pen);
			//bottom-bottom
			dbb_bottom->getDC().MoveTo(Normalize(rect_window_bottom).left, Normalize(rect_window_bottom).top);
			dbb_bottom->getDC().LineTo(Normalize(rect_window_bottom).left, Normalize(rect_window_bottom).bottom - 1 - thickness);
			dbb_bottom->getDC().LineTo(Normalize(rect_window_bottom).right - 1 - thickness, Normalize(rect_window_bottom).bottom - 1 - thickness);
			dbb_bottom->getDC().LineTo(Normalize(rect_window_bottom).right - 1 - thickness, Normalize(rect_window_bottom).top - 1);
			dbb_bottom->getDC().SelectObject(old_pen);

			old_pen = dbb_left->getDC().SelectObject(&pen);
			dbb_left->getDC().MoveTo(Normalize(rect_window_left).left, Normalize(rect_window_left).top);
			dbb_left->getDC().LineTo(Normalize(rect_window_left).left, Normalize(rect_window_left).bottom);
			dbb_left->getDC().SelectObject(old_pen);

			old_pen = dbb_right->getDC().SelectObject(&pen);
			dbb_right->getDC().MoveTo(Normalize(rect_window_right).right - 1 - thickness, Normalize(rect_window_right).top);
			dbb_right->getDC().LineTo(Normalize(rect_window_right).right - 1 - thickness, Normalize(rect_window_right).bottom);
			dbb_right->getDC().SelectObject(old_pen);

			pen.DeleteObject();
		}
		//테두리 그리기.
		CSize btn_sz(rect_window_top.Height() - m_sysbtn_margin * 2, rect_window_top.Height() - m_sysbtn_margin * 2);
		CPoint btn_pt(rect_window.right - rect_window_right.Width() - btn_sz.cx,
					  rect_window.top + m_sysbtn_margin);

		CDC cdc;
		cdc.CreateCompatibleDC(ncpaint);
		for (size_t i = 0; i < m_sysbtn.size(); i++) {
			
			BITMAP bmp;
			CBitmap* cbmp=nullptr, *old_cbmp = nullptr;
			if (m_sysbtn[i].m_bitmap_resource > 0) {
				cbmp=new CBitmap;
				cbmp->LoadBitmap(m_sysbtn[i].m_bitmap_resource);
				if (cbmp == nullptr) {
					continue;
				}
				cbmp->GetBitmap(&bmp);
				//메서드가 성공 하면 0이 아닌. 그렇지 않으면 0입니다.
				old_cbmp = cdc.SelectObject(cbmp);
				dbb_top->getDC().SetStretchBltMode(COLORONCOLOR);
				dbb_top->getDC().TransparentBlt(btn_pt.x, btn_pt.y, btn_sz.cx, btn_sz.cy, &cdc
															, bmp.bmHeight*static_cast<int>(this->m_sysbtn[i].m_state), 0
															, bmp.bmHeight, bmp.bmHeight, m_color_transparent);
				m_sysbtn[i].m_rect = CRect(btn_pt.x, btn_pt.y, btn_pt.x + btn_sz.cx, btn_pt.y + btn_sz.cy);
				btn_pt.x -= btn_sz.cx + m_sysbtn_margin;
				cdc.SelectObject(old_cbmp);
				delete cbmp;
				cbmp = nullptr;
			} else {
				std::array<unsigned int,3> w, h;
				std::array<unsigned int*,3> image;
				switch (g_theme_story) {
					case MSpringThemeStory::Fluent: {
						w = { sysbtn_fluent_close_width ,sysbtn_fluent_maximize_width,sysbtn_fluent_minimize_width };
						h = { sysbtn_fluent_close_height ,sysbtn_fluent_maximize_height,sysbtn_fluent_minimize_height };
						image = { sysbtn_fluent_close ,sysbtn_fluent_maximize ,sysbtn_fluent_minimize };
					}break;
					case MSpringThemeStory::Material: {
						w = { sysbtn_material_close_width ,sysbtn_material_maximize_width,sysbtn_material_minimize_width };
						h = { sysbtn_material_close_height ,sysbtn_material_maximize_height,sysbtn_material_minimize_height };
						image = { sysbtn_material_close ,sysbtn_material_maximize ,sysbtn_material_minimize };
					}break;
					case MSpringThemeStory::Flat: {
						w = { sysbtn_flat_close_width ,sysbtn_flat_maximize_width,sysbtn_flat_minimize_width };
						h = { sysbtn_flat_close_height ,sysbtn_flat_maximize_height,sysbtn_flat_minimize_height };
						image = { sysbtn_flat_close ,sysbtn_flat_maximize ,sysbtn_flat_minimize };
					}break;
				}

				HBITMAP hbmp = nullptr;
				CDC* DC = this->GetDC();
				switch (m_sysbtn[i].m_bitmap_resource) {
					case -1:hbmp = GetHeaderResource(DC->GetSafeHdc(), w[0], h[0], image[0]); break;
					case -2:hbmp=GetHeaderResource(DC->GetSafeHdc(), w[1], h[1], image[1]); break;
					case -3:hbmp = GetHeaderResource(DC->GetSafeHdc(), w[2], h[2], image[2]); break;
					default:break;
				}
				ReleaseDC(DC);
				if (hbmp == nullptr) {
					continue;
				}
				cbmp = CBitmap::FromHandle(hbmp);
				if (cbmp == nullptr) {
					continue;
				}
				cbmp->GetBitmap(&bmp);
				//메서드가 성공 하면 0이 아닌. 그렇지 않으면 0입니다.
				old_cbmp = cdc.SelectObject(cbmp);
				dbb_top->getDC().SetStretchBltMode(HALFTONE);
				dbb_top->getDC().TransparentBlt(btn_pt.x, btn_pt.y, btn_sz.cx, btn_sz.cy, &cdc
															, bmp.bmHeight*static_cast<int>(this->m_sysbtn[i].m_state), 0
															, bmp.bmHeight, bmp.bmHeight, m_color_transparent);
				m_sysbtn[i].m_rect = CRect(btn_pt.x, btn_pt.y, btn_pt.x + btn_sz.cx, btn_pt.y + btn_sz.cy);
				btn_pt.x -= btn_sz.cx + m_sysbtn_margin;
				cdc.SelectObject(old_cbmp);
				//BITMAP is static, Do Not Delete or Release
				//cbmp->DeleteObject();
			}
		}
		cdc.DeleteDC();
		if (m_icon.m_icon != nullptr) {
			m_icon.m_rect.left = m_icon_margin + rect_window_left.Width();
			m_icon.m_rect.top = m_icon_margin;
			m_icon.m_rect.right = rect_window_top.Height() - m_icon_margin + 10;
			m_icon.m_rect.bottom = rect_window_top.Height() - m_icon_margin;
			m_icon.m_rect.NormalizeRect();
			::DrawIconEx(dbb_top->getDC().GetSafeHdc()
									 , m_icon.m_rect.left, m_icon.m_rect.top
									 , m_icon.m_icon, m_icon.m_rect.Width(), m_icon.m_rect.Height()
									 , 0, NULL, DI_NORMAL);
		}
		

		int title_h = rect_window_top.Height() - 10;


		
		
		dbb_top->getDC().SetTextColor(m_color_title);
		dbb_top->getDC().SetBkMode(TRANSPARENT);
		CRect title_sz;
		int h = mspring::Font::GetRealFontHeight(m_font_str, title_h, dbb_top->getPDC(), m_title.data(), true, mspring::Font::PointFontType);
		
		h = static_cast<decltype(h)>(h * m_title_height_ratio);
		DWORD option_shadow = 0;
		DWORD option_depth = 0;
		switch (g_theme_story) {
			case MSpringThemeStory::Fluent: {
				option_shadow = KEY_LIGHT_B | AMBIENT_LIGHT4;
				option_depth = 4;
			}break;
			case MSpringThemeStory::Material: {
				option_shadow = KEY_LIGHT_R | KEY_LIGHT_B;
				option_depth = 1;
			}break;
			case MSpringThemeStory::Flat: {
				option_shadow = 0;
				option_depth = 0;
			}break;
		}
		title_sz = mspring::Text::TextOutMSP(dbb_top->getPDC(),
												  static_cast<int>(m_icon.m_rect.right + 9),
												  static_cast<int>((rect_window_top.top + rect_window_top.bottom) / 2),
												  m_title.data(),
												  static_cast<int>(m_title.length()),
												  m_font_str.data(), h,option_shadow, this->m_color_bk, false,option_depth);
		//아이콘 오른쪽 부터 (Icon margin + Title margin)
		int begin_point = m_icon.m_rect.right + 10 + title_sz.Width() + 10;
		//시스템 버튼의 왼쪽 까지
		int end_point = m_sysbtn.size()>0 ? m_sysbtn.back().m_rect.left - 10 : rect_window.right;

		CRect iRect = this->GetWindowNomalizedRect();
		iRect.left = begin_point;
		iRect.right = end_point;
		iRect.bottom = iRect.top + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYSIZEFRAME);
		m_dbb[0] = dbb_top;
		m_dbb[1] = dbb_bottom;
		m_dbb[2] = dbb_left;
		m_dbb[3] = dbb_right;
		for (auto&e : m_expansion) {
			int last = e->OnNcPaint(m_dbb[0]->getPDC(), iRect);
			if (last >= 0) {
				iRect.left += last + 20;
			} else {
				iRect.right -= std::abs(last) + 20;
			}
		}
		m_blank_caption = iRect;

		//이제 모든 그림을 출력합니다.
		m_dbb[2]->Draw();	//left
		m_dbb[3]->Draw();	//right
		m_dbb[0]->Draw();	//top
		m_dbb[1]->Draw();	//bottom
		for (auto& e : m_dbb) {
			delete e;
			e = nullptr;
		}
		ReleaseDC(ncpaint);
	}
	afx_msg BOOL OnNcActivate(BOOL bActive) {
		REDRAW_NCAREA;
		return TRUE;
	}
	afx_msg void OnSize(UINT nType, int cx, int cy) {
		CFrameWnd::OnSize(nType, cx, cy);
		CRect rect_window;
		this->GetWindowRect(&rect_window);
		CRgn rgn;
		//윈도우를 깔끔한 사각형으로 변경합니다.
		rgn.CreateRoundRectRgn(0, 0, rect_window.Width(), rect_window.Height(), 0,0);
		this->SetWindowRgn(static_cast<HRGN>(rgn.GetSafeHandle()), TRUE);
		for (auto&e : m_expansion) {
			e->OnSize(nType, cx, cy);
		}
		
	}
	afx_msg void OnMove(int cx, int cy) {
		CFrameWnd::OnMove(cx, cy);
		CRect rect_window;
		this->GetWindowRect(&rect_window);
		for (auto&e : m_expansion) {
			//e->OnSize(nType, cx, cy);
		}
	}
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point) {
		if (clock() - g_maximized_time < 100) {
			return;
		}
		CPoint apoint = point;
		point = this->GetMousePoint();
		//마우스 추적 활성화
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
		tme.dwHoverTime = 0;
		TrackMouseEvent(&tme);
		this->ReplaceSystemButtonState(SystemButton::State::Hover, SystemButton::State::Normal);
		// 버튼 Hover
		bool is_btnclk = false;
		for (int i = 0; i < (int)this->m_sysbtn.size(); i++) {
			if (this->m_sysbtn[i].m_rect.PtInRect(point) == TRUE) {
				this->m_sysbtn[i].m_state = SystemButton::State::Hover;
				is_btnclk = true;
				break;
			}
		}
		CRect rect_caption = this->GetWindowCaptionRect();
		//버튼을 누른게 아니고 최대화 상태에서, 캡션을 잡고, 마우스 왼쪽 버튼으로 끌었을때
		//->크기가 원래대로 돌아가고 창 움직이는상태로.
		if (is_btnclk == false && m_is_maximize == true && rect_caption.PtInRect(point) == TRUE && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
			CPoint abs_point;
			::GetCursorPos(&abs_point);
			CRect rect;
			this->GetWindowRect(rect);
#ifdef _M_AMD64
			CRect* rect_window = (CRect*)::GetWindowLongPtr(this->GetSafeHwnd(), GWLP_USERDATA);
#else
			CRect* rect_window = (CRect*)::GetWindowLongA(this->GetSafeHwnd(), GWL_USERDATA);
#endif
			CRect relative_window = *rect_window;
			int w = relative_window.Width();
			int h = relative_window.Height();
			float xm = (float)point.x / rect.Width();
			float ym = (float)point.y / rect.Height();
			relative_window.left = static_cast<LONG>(abs_point.x - xm*w);
			relative_window.top = static_cast<LONG>(abs_point.y - ym*h);
			relative_window.right = relative_window.left + w;
			relative_window.bottom = relative_window.top + h;
			this->MoveWindow(relative_window);
			m_is_maximize = false;
			::DefWindowProcA(this->GetSafeHwnd(), WM_SYSCOMMAND, SC_MOVE + 1, MAKELPARAM(point.x, point.y));
		}
		for (auto&e : m_expansion) {
			e->OnNcMouseMove(nHitTest, point);
		}
		REDRAW_NCAREA;
		CFrameWnd::OnNcMouseMove(nHitTest, apoint);	
	}
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point) {
		CPoint apoint = point;
		point = this->GetMousePoint();
		this->ReplaceSystemButtonState(SystemButton::State::Hover, SystemButton::State::Normal);
		for (int i = 0; i < (int)this->m_sysbtn.size(); i++) {
			if (this->m_sysbtn[i].m_rect.PtInRect(point) == TRUE) {
				this->m_sysbtn[i].m_state = SystemButton::State::Click;
				break;
			}
		}
		bool ret = false;
		for (auto&e : m_expansion) {
			ret|=e->OnNcLButtonDown(nHitTest, point);
		}
		REDRAW_NCAREA;
		m_other_task = false;
		if (ret == false) {
			CFrameWnd::OnNcLButtonDown(nHitTest, apoint);
		} else {
			m_other_task = true;
		}
	}
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point) {
		CPoint apoint = point;
		point=this->GetMousePoint();
		this->ReplaceSystemButtonState(SystemButton::State::Hover, SystemButton::State::Normal);
		this->ReplaceSystemButtonState(SystemButton::State::Click, SystemButton::State::Normal);
		bool clicked = false;
		for (int i = 0; i < (int)this->m_sysbtn.size(); i++) {
			if (this->m_sysbtn[i].m_rect.PtInRect(point) == TRUE) {
				this->m_sysbtn[i].Do(this);
				clicked = true;
				break;
			}
		}
		if (clicked == false) {
			for (auto&e : m_expansion) {
				e->OnNcLButtonUp(nHitTest, this->GetMousePoint());
			}
		}
		REDRAW_NCAREA;
		CFrameWnd::OnNcLButtonUp(nHitTest, apoint);
	}
	afx_msg LRESULT OnNcHitTest(CPoint point) {
		if (m_disable_httest == true) {
			return HTNOWHERE;
		}
		CPoint apoint = point;
		point = this->GetMousePoint();
		//시스템 버튼 클릭시 아무것도 안함.
		for (int i = 0; i < (int)this->m_sysbtn.size(); i++) {
			if (this->m_sysbtn[i].m_rect.PtInRect(point) == TRUE) {
				return HTBORDER;
			}
		}
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
			for (auto&e : m_expansion) {
				if (e->PtInRect() == true) {
					POINTS ps;
					ps.x = static_cast<SHORT>(apoint.x);
					ps.y = static_cast<SHORT>(apoint.y);
					this->SendMessage(WM_NCLBUTTONDOWN, HTBORDER, (WPARAM)&ps);
					return HTNOWHERE;
				}
			}
		}
			
		//최대화 상태일때 아무것도 안함.
		//캡션바 더블클릭 , 캡션바 잡고끌기 만 허용.
		if (m_is_maximize) {
			//최대화 상태일때 크기 변경을 하지 못하도록 합니다.
			return HTBORDER;
		}
		return CFrameWnd::OnNcHitTest(point);
	}
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point) {
		
		if (m_blank_caption.PtInRect(this->GetMousePoint()) == TRUE) {
			this->ButtonEvent_MaximizeWindow(this);
		}
		for (auto&e : m_expansion) {
			e->OnNcLButtonDblClk(nHitTest, point);
		}
	}
	afx_msg void OnNcMouseLeave() {
		this->ReplaceSystemButtonState(SystemButton::State::Click, SystemButton::State::Normal);
		this->ReplaceSystemButtonState(SystemButton::State::Hover, SystemButton::State::Normal);
		CFrameWnd::OnNcMouseLeave();
		for (auto&e : m_expansion) {
			e->OnNcMouseLeave();
		}
		REDRAW_NCAREA;
	}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) {
		return FALSE;
	}
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus) {
		CFrameWnd::OnShowWindow(bShow, nStatus);
	}
	afx_msg void OnDestroy() {
		for (auto&e : m_expansion) {
			e->OnDestroy();
		}
		CFrameWnd::OnDestroy();
	}
};

MSPRING_IMPLEMENT_DYNAMIC(MSpringFrame, CFrameWnd)

MSPRING_BEGIN_MESSAGE_MAP(MSpringFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCMOUSELEAVE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
MSPRING_END_MESSAGE_MAP()


#endif  //MSPRING_7E1_9_C_MSPRINGFRAME_HPP_INCLUDED