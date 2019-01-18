
#pragma once
#include "MSpring/MSpring.h"
#include<memory>
#include"Theme.h"
class ControlTestView : public VirtualView {
public:
	MListBox m_list;
	MButton m_button;
	MButtonCheck m_check;
	MStatic m_static;
	MSingleEdit m_edit;
	MSingleEdit m_edit2;

	MSlider m_slider;
public:
	ControlTestView(CWnd* wnd) :VirtualView(wnd) {
		m_button = CreateControl<MButton>(wnd, MRect(MRectPosition::RB, 20, 20, 100, 30));
		m_button->m_func = []()->UINT {
			::MessageBoxA(NULL, "test", "test", MB_OK);
			return 1;
		};
		m_button->m_color_fr = new std::atomic<COLORREF>(GetTheme().g_control_foreground[GetTheme().g_idx]);
		m_button->m_color_text= new std::atomic<COLORREF>(GetTheme().g_control_text[GetTheme().g_idx]);
		this->AddControl(m_button);

		m_check = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::RB, 20, 100, 30, 30));
		m_check->m_color_fr = new std::atomic<COLORREF>(GetTheme().g_control_foreground[GetTheme().g_idx]);
		m_check->m_color_text = new std::atomic<COLORREF>(GetTheme().g_control_text[GetTheme().g_idx]);
		this->AddControl(m_check);

		m_list = CreateControl<MListBox>(wnd, MRect(MRectPosition::R, 400, 20, 20, 150));
		m_list->m_color_bk = new std::atomic<COLORREF>(GetTheme().g_color_bk[GetTheme().g_idx]);
		m_list->m_color_text = new std::atomic<COLORREF>(GetTheme().g_control_text[GetTheme().g_idx]);
		m_list->m_color_fr = new std::atomic<COLORREF>(GetTheme().g_control_foreground[GetTheme().g_idx]);
		m_list->is_check = true;
		m_list->is_numbering = true;
		for (int i = 0; i < 50; i++) {
			OStringStream oss;
			oss << TEXT("구구단을 외자, 구구단을 외자!!  ") << i << TEXT("x") << i << TEXT(" is ") << i*i;
			m_list->m_data.push_back(std::make_pair(oss.str(), rand() % 2 == 0));
		}
		this->AddControl(m_list);

		m_static = CreateControl<MStatic>(wnd, MRect(MRectPosition::LB, 20, 20, 400, 30));
		m_edit = CreateControl<MSingleEdit>(wnd, MRect(MRectPosition::LB, 20, 70, 200, 30));
		m_edit2 = CreateControl<MSingleEdit>(wnd, MRect(MRectPosition::LB, 20, 120, 400, 60));
		
		m_static->m_text = TEXT("Test Application 그리고 한글....");
		m_static->m_color_text = new std::atomic<COLORREF>(RGB(128,128,128));
		


		m_slider = CreateControl<MSlider>(wnd, MRect(MRectPosition::B, 400, 20, 20, 60));
		m_slider->m_color_bk = new std::atomic<COLORREF>(RGB(255, 199, 199));
		m_slider->m_vertical = false;

		
		
		this->AddControl(m_static);
		this->AddControl(m_edit);
		this->AddControl(m_edit2);
		
		this->AddControl(m_slider);
	}
	virtual ~ControlTestView() {

	}
	void OnPaint(CDC* pDC) {
		MCALL;
	}
	virtual void OnLButtonDown(UINT nFlags, CPoint point) {
		MCALL;
	}
	virtual void OnLButtonUp(UINT nFlags, CPoint point) {
		MCALL;
	}
	virtual void OnMouseMove(UINT nFlags, CPoint point) {
		MCALL;
	}
	virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
		MCALL;
	}
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
		MCALL;
	}
	virtual LRESULT OnComposition(WPARAM wParam, LPARAM lParam) {
		MCALL;
		return 1;
	}
	virtual void OnTimer(UINT_PTR nIDEvent) {
		MCALL;
	}
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
		MCALL;
		return TRUE;
	}
	virtual void OnMouseLeave() {
		MCALL;
	}
	virtual void OnRButtonUp(UINT nFlags, CPoint point) {
		MCALL;
		int idx = m_list->GetElementByPoint(point);
	}
	virtual void OnDestroy() {
		delete m_slider->m_color_bk;
		MCALL;
	}
};