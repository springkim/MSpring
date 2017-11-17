#pragma once
#include "mspring/View.h"
#include"mspring/control.h"
class ControlTestView : public VirtualView {
public:
	MButton* m_button;
	MButtonCheck* m_check;
	MStatic* m_static;
	MEdit* m_edit;
	MEdit* m_edit2;
	MListBox* m_list;
	MListBox* m_list2;
public:
	ControlTestView(CWnd* wnd) :VirtualView(wnd) {
		m_button = new MButton(wnd, MRect(MRectPosition::RB, 20, 20, 100, 30));
		m_button->m_func = []()->UINT {
			::MessageBoxA(NULL, "test", "test", MB_OK);
			return 1;
		};
		m_check = new MButtonCheck(wnd, MRect(MRectPosition::RB, 20, 100, 30, 30));
		m_static = new MStatic(wnd, MRect(MRectPosition::LB, 20, 20, 200, 30));
		m_edit = new MEdit(wnd, MRect(MRectPosition::LB, 20, 70, 200, 30));
		m_edit2 = new MEdit(wnd, MRect(MRectPosition::LB, 20, 120, 200, 30));
		m_list = new MListBox(wnd, MRect(MRectPosition::DYNAMIC, 20, 20, 400, 400));
		m_list->is_check = true;
		for (int i = 0; i < 50; i++) {
			std::ostringstream oss;
			oss << "list element " << i;
			m_list->m_data.push_back(std::make_pair(mspring::String::ToCString(oss.str()),true));
		}
		m_list2 = new MListBox(wnd, MRect(MRectPosition::RT, 20, 20, 300, 300));
		m_list2->m_data.push_back(std::make_pair(TEXT("dog"),false));
		m_list2->m_data.push_back(std::make_pair(TEXT("cat"), false));
		m_list2->m_data.push_back(std::make_pair(TEXT("person"), false));
		m_list2->m_data.push_back(std::make_pair(TEXT("bus"), false));
	}
	virtual ~ControlTestView() {

	}
	void OnPaint(CDC* pDC) {
		CRect rect;
		rect=this->GetViewRect();
		m_button->OnPaint(pDC);
		m_check->OnPaint(pDC);
		m_static->OnPaint(pDC);
		m_edit->OnPaint(pDC);
		m_edit2->OnPaint(pDC);
		m_list->OnPaint(pDC);
		m_list2->OnPaint(pDC);
	}
	virtual void OnLButtonDown(UINT nFlags, CPoint point) {
		m_button->OnLButtonDown();
		m_check->OnLButtonDown();
		m_edit->OnLButtonDown();
		m_edit2->OnLButtonDown();
		m_list->OnLButtonDown();
		m_list2->OnLButtonDown();
	}
	virtual void OnLButtonUp(UINT nFlags, CPoint point) {
		m_list->OnLButtonUp();
		m_list2->OnLButtonUp();
	}
	virtual void OnMouseMove(UINT nFlags, CPoint point) {
		m_button->OnMouseMove();
		m_check->OnMouseMove();
		m_list->OnMouseMove();
		m_list2->OnMouseMove();
	}
	virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
		m_edit->OnChar(nChar);
		m_edit2->OnChar(nChar);
	}
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
		m_edit->OnKeyDown(nChar);
		m_edit2->OnKeyDown(nChar);
	}
	virtual LRESULT OnComposition(WPARAM wParam, LPARAM lParam) {
		m_edit->OnComposition(wParam, lParam);
		m_edit2->OnComposition(wParam, lParam);
		return 1;
	}
	virtual void OnTimer(UINT_PTR nIDEvent) {
		m_edit->OnTimer(nIDEvent);
		m_edit2->OnTimer(nIDEvent);
		m_list->OnTimer(nIDEvent);
		m_list2->OnTimer(nIDEvent);
	}
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
		m_list->OnMouseWheel(zDelta);
		m_list2->OnMouseWheel(zDelta);
		return TRUE;
	}
	virtual void OnMouseLeave() {
		m_list->OnMouseLeave();
		m_list2->OnMouseLeave();
	}
	virtual void OnRButtonUp(UINT nFlags, CPoint point) {
		int idx = m_list->GetElementByPoint(point);
		std::cout << idx << std::endl;
	}
};

