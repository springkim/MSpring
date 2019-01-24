
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
	void SetTheme() {
		int i = GetTheme().g_idx;

		/*m_button->m_color_fr = GetTheme().g_control_foreground[i];
		m_button->m_color_text = GetTheme().g_control_text2[i];

		m_check->m_color_fr = GetTheme().g_control_foreground[i];
		m_check->m_color_text = GetTheme().g_control_text2[i];*/

		m_list->m_color_bk = GetTheme().g_color_bk[i];
		m_list->m_color_text = GetTheme().g_control_text[i];
		m_list->m_color_other = GetTheme().g_control_text2[i];
		m_list->m_color_fr = GetTheme().g_control_foreground[i];

		/*m_edit->m_color_text = GetTheme().g_control_text[i];
		m_edit->m_color_fr = GetTheme().g_control_foreground[i];

		m_static->m_color_text = GetTheme().g_control_text[i];
		

		m_slider->m_color_bk = GetTheme().g_control_background[i];
		m_slider->m_color_fr= GetTheme().g_control_foreground[i];
		m_slider->m_color_other = GetTheme().g_control_text2[i];*/

		for (auto&g : m_objs) {
			g->m_shadow = GetTheme().g_shadow[i];
		}
	}
public:
	ControlTestView(CWnd* wnd) :VirtualView(wnd) {
		/*m_button = CreateControl<MButton>(wnd, MRect(MRectPosition::RB, 20, 20, 100, 30));
		m_button->m_func = []()->UINT {
			::MessageBoxA(NULL, "test", "test", MB_OK);
			return 1;
		};
		this->AddControl(m_button);

		m_check = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::RB, 20, 100, 30, 30));
		this->AddControl(m_check);

		

		m_slider = CreateControl<MSlider>(wnd, MRect(MRectPosition::B, 400, 20, 20, 60));
		m_slider->m_vertical = false;
		this->AddControl(m_slider);

		m_static = CreateControl<MStatic>(wnd, MRect(MRectPosition::LB, 20, 20, 400, 30));
		this->AddControl(m_static);

		m_edit = CreateControl<MSingleEdit>(wnd, MRect(MRectPosition::LB, 20, 70, 200, 30));
		this->AddControl(m_edit);


		m_static->m_text = TEXT("Test Application 그리고 한글....");*/
		
		m_list = CreateControl<MListBox>(wnd, MRect(MRectPosition::DYNAMIC, 50, 100, 150, 200));
		m_list->is_check = true;
		m_list->is_numbering = true;
		for (int i = 1; i < 200; i++) {
			OStringStream oss;

			LPTSTR messageBuffer = nullptr;
			size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
										 NULL, i, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);
			TString str = TEXT("Invalid Error");
			if (messageBuffer) {
				str = messageBuffer;
				LocalFree(messageBuffer);
			} 
			m_list->m_data.push_back(std::make_pair(str, rand() % 2 == 0));
			
			
		}
		this->AddControl(m_list);

		

		SetTheme();
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
		MCALL;
	}
};