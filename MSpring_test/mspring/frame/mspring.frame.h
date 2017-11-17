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
#include<string>
#include<array>
#include<map>
#include"../utils.h"
#include"mspring.resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
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
	std::vector<MSpringFrameExpansion*> m_expansion;	//확장 기능 클래스 입니다.
	CRect m_blank_caption;						//OnNcDblClk 함수에서 사용합니다.
protected:	//coord value
	int m_sysbtn_margin = 4;					//시스템 버튼 사이의 간격입니다.
	int m_icon_margin = 4;						//아이콘의 margin 입니다.
protected:	//style value
	COLORREF m_color_transparent;			//시스템버튼의 투명색을 지정합니다.
	COLORREF m_color_bk;						//프레임의 배경색입니다.
	COLORREF m_color_border;					//경계색의 색상입니다.
	COLORREF m_color_text;
	CString m_font_str;
	CString m_title = TEXT("MSpring");
public:		//static method
	static void ButtonEvent_Close(CWnd* wnd) {
		EXEC_ALWAYS(::AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT, 0));
	}
	static void ButtonEvent_MaximizeWindow(CWnd* wnd) {
		MSpringFrame* pmainframe = dynamic_cast<MSpringFrame*>(wnd);
		if (pmainframe->m_is_maximize == false) {
			static CRect rect_window;
			pmainframe->GetWindowRect(rect_window);
#ifdef _M_AMD64
			EXEC_ALWAYS(::SetWindowLongPtr(pmainframe->GetSafeHwnd(), GWLP_USERDATA, (LONG_PTR)&rect_window));
			//If the function fails, the return value is zero. To get extended error information, call GetLastError. 
#else
			EXEC_ALWAYS(::SetWindowLongA(pmainframe->GetSafeHwnd(), GWL_USERDATA, (LONG)&rect_window));
			//If the function fails, the return value is zero. To get extended error information, call GetLastError. 
#endif
			
			MONITORINFOEXA monitor;
			monitor.cbSize = sizeof(MONITORINFOEXA);
			CRect rect;
			wnd->GetWindowRect(&rect);
			HMONITOR hMOnitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
			EXEC_ALWAYS(GetMonitorInfoA(hMOnitor, &monitor));

			wnd->MoveWindow(&monitor.rcWork);
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
	void SetStyle(CString font_str,COLORREF color_bk,COLORREF color_text ,COLORREF color_border, COLORREF color_transparent = RGB(255, 255, 255)) {
		m_font_str = font_str;
		m_color_bk = color_bk;
		m_color_text = color_text;
		m_color_border = color_border;
		m_color_transparent = color_transparent;
	}
	void SetTitle(CString title) {
		m_title = title;
	}
	//시스템버튼을 추가합니다.
	void AddSysBtn(int bitmap, void(*EventFunc)(CWnd*)) {
		m_sysbtn.push_back(SystemButton(bitmap, EventFunc));
	}
	//확장 클래스를 추가합니다.
	void AddExpansionClass(MSpringFrameExpansion* _class) {
		m_expansion.push_back(_class);
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
		EXEC_ALWAYS(this->ModifyStyle(WS_SYSMENU, 0));	//시스템 메뉴(종료,최대화,최소화) 버튼을 제거합니다.
		for (auto&e : m_expansion) {
			e->OnCreate(lpCreateStruct);
		}
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
			EXEC_ALWAYS(brush.CreateSolidBrush(m_color_bk));
			dbb_top->getDC().FillRect(Normalize(rect_window_top), &brush);
			dbb_bottom->getDC().FillRect(Normalize(rect_window_bottom), &brush);
			dbb_left->getDC().FillRect(Normalize(rect_window_left), &brush);
			dbb_right->getDC().FillRect(Normalize(rect_window_right), &brush);
			EXEC_ALWAYS(brush.DeleteObject());
		}
		//여기까지가 프레임 그리기.
		{
			CPen pen, *old_pen;
			int thickness = 1;
			EXEC_ALWAYS(pen.CreatePen(PS_SOLID, thickness, m_color_border));
			old_pen = dbb_top->getDC().SelectObject(&pen);
			dbb_top->getDC().MoveTo(Normalize(rect_window_top).left, Normalize(rect_window_top).bottom);
			dbb_top->getDC().LineTo(Normalize(rect_window_top).left, Normalize(rect_window_top).top);
			dbb_top->getDC().LineTo(Normalize(rect_window_top).right - 1 - thickness, Normalize(rect_window_top).top);
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

			EXEC_ALWAYS(pen.DeleteObject());
		}
		//테두리 그리기.
		CSize btn_sz(rect_window_top.Height() - m_sysbtn_margin * 2, rect_window_top.Height() - m_sysbtn_margin * 2);
		CPoint btn_pt(rect_window.right - rect_window_right.Width() - btn_sz.cx,
					  rect_window.top + m_sysbtn_margin);

		CDC cdc;
		EXEC_ALWAYS(cdc.CreateCompatibleDC(ncpaint));
		for (size_t i = 0; i < m_sysbtn.size(); i++) {
			
			BITMAP bmp;
			CBitmap* cbmp=nullptr, *old_cbmp = nullptr;
			if (m_sysbtn[i].m_bitmap_resource > 0) {
				cbmp=new CBitmap;
				EXEC_ALWAYS(cbmp->LoadBitmap(m_sysbtn[i].m_bitmap_resource));
				if (cbmp == nullptr) {
					continue;
				}
				EXEC_ALWAYS(cbmp->GetBitmap(&bmp));
				//메서드가 성공 하면 0이 아닌. 그렇지 않으면 0입니다.
				old_cbmp = cdc.SelectObject(cbmp);
				EXEC_ALWAYS(dbb_top->getDC().TransparentBlt(btn_pt.x, btn_pt.y, btn_sz.cx, btn_sz.cy, &cdc
															, bmp.bmHeight*static_cast<int>(this->m_sysbtn[i].m_state), 0
															, bmp.bmHeight, bmp.bmHeight, m_color_transparent));
				m_sysbtn[i].m_rect = CRect(btn_pt.x, btn_pt.y, btn_pt.x + btn_sz.cx, btn_pt.y + btn_sz.cy);
				btn_pt.x -= btn_sz.cx + m_sysbtn_margin;
				cdc.SelectObject(old_cbmp);
			} else {
				HBITMAP hbmp = nullptr;
				CDC* DC = this->GetDC();
				switch (m_sysbtn[i].m_bitmap_resource) {
					case -1:hbmp = GetHeaderResource(DC->GetSafeHdc(), resource_close_w, resource_close_h, resource_close); break;
					case -2:hbmp=GetHeaderResource(DC->GetSafeHdc(), resource_maximize_w, resource_maximize_h, resource_maximize); break;
					case -3:hbmp = GetHeaderResource(DC->GetSafeHdc(), resource_minimize_w, resource_minimize_h, resource_minimize); break;
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
				EXEC_ALWAYS(cbmp->GetBitmap(&bmp));
				//메서드가 성공 하면 0이 아닌. 그렇지 않으면 0입니다.
				old_cbmp = cdc.SelectObject(cbmp);
				EXEC_ALWAYS(dbb_top->getDC().TransparentBlt(btn_pt.x, btn_pt.y, btn_sz.cx, btn_sz.cy, &cdc
															, bmp.bmHeight*static_cast<int>(this->m_sysbtn[i].m_state), 0
															, bmp.bmHeight, bmp.bmHeight, m_color_transparent));
				m_sysbtn[i].m_rect = CRect(btn_pt.x, btn_pt.y, btn_pt.x + btn_sz.cx, btn_pt.y + btn_sz.cy);
				btn_pt.x -= btn_sz.cx + m_sysbtn_margin;
				cdc.SelectObject(old_cbmp);
				//BITMAP is static, Do Not Delete or Release
				//cbmp->DeleteObject();
				//EXEC_ALWAYS(::DeleteObject(hbmp));
			}
		}
		cdc.DeleteDC();
		if (m_icon.m_icon != nullptr) {
			m_icon.m_rect.left = m_icon_margin + rect_window_left.Width();
			m_icon.m_rect.top = m_icon_margin;
			m_icon.m_rect.right = rect_window_top.Height() - m_icon_margin + 10;
			m_icon.m_rect.bottom = rect_window_top.Height() - m_icon_margin;
			m_icon.m_rect.NormalizeRect();
			EXEC_ALWAYS(::DrawIconEx(dbb_top->getDC().GetSafeHdc()
									 , m_icon.m_rect.left, m_icon.m_rect.top
									 , m_icon.m_icon, m_icon.m_rect.Width(), m_icon.m_rect.Height()
									 , 0, NULL, DI_NORMAL));
		}
		

		int title_h = rect_window_top.Height() - 10;
		int h = mspring::Font::GetRealFontHeight(m_font_str, title_h, dbb_top->getPDC());
		CFont font;
		
		EXEC_ALWAYS(font.CreatePointFont(h, m_font_str));
		CFont* old_font=dbb_top->getDC().SelectObject(&font);
		dbb_top->getDC().SetTextColor(m_color_text);
		dbb_top->getDC().SetBkMode(TRANSPARENT);
		
		CSize title_sz;
		EXEC_ALWAYS(GetTextExtentPoint32(dbb_top->getDC().GetSafeHdc(), m_title, m_title.GetLength(), &title_sz));
		dbb_top->getDC().TextOut(m_icon.m_rect.right + 5, rect_window_top.Height() / 2 - title_sz.cy / 2, m_title);
		dbb_top->getDC().SelectObject(&old_font);
		EXEC_ALWAYS(font.DeleteObject());
		//아이콘 오른쪽 부터
		int begin_point = m_icon.m_rect.right + 10 + title_sz.cx;
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
		rect_window = this->GetWindowNomalizedRect();
		CRgn rgn;
		//윈도우를 깔끔한 사각형으로 변경합니다.
		EXEC_ALWAYS(rgn.CreateRoundRectRgn(0, 0, rect_window.Width(), rect_window.Height(), 0, 0));
		EXEC_ALWAYS(this->SetWindowRgn(static_cast<HRGN>(rgn.GetSafeHandle()), TRUE));
		for (auto&e : m_expansion) {
			e->OnSize(nType, cx, cy);
		}
	}
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point) {
		CPoint apoint = point;
		point = this->GetMousePoint();
		//마우스 추적 활성화
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
		tme.dwHoverTime = 0;
		EXEC_ALWAYS(TrackMouseEvent(&tme));
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
			EXEC_ALWAYS(::GetCursorPos(&abs_point));
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
		if (ret == false) {
			CFrameWnd::OnNcLButtonDown(nHitTest, apoint);
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
		point = this->GetMousePoint();
		//시스템 버튼 클릭시 아무것도 안함.
		for (int i = 0; i < (int)this->m_sysbtn.size(); i++) {
			if (this->m_sysbtn[i].m_rect.PtInRect(point) == TRUE) {
				return HTBORDER;
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
		return TRUE;
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
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCMOUSELEAVE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
MSPRING_END_MESSAGE_MAP()


#endif  //MSPRING_7E1_9_C_MSPRINGFRAME_HPP_INCLUDED