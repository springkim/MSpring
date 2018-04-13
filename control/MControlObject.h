﻿/*
*  mcontrolobject.h
*  MSpring
*
*  Created by KimBomm on 2018. 4. 7...
*  Copyright 2018 KimBomm. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E2_4_7_MCONTROLOBJECT_HPP_INCLUDED)
#define MSPRING_7E2_4_7_MCONTROLOBJECT_HPP_INCLUDED
#include<atomic>
#include<memory>

#include<afxwin.h>

#include"../utils/utils.h"

enum MRectPosition {	//컨트롤의 위치의 상태를 나타냅니다.
	LT, RT, LB, RB, DYNAMIC, L, R, T, B
};
class MRect {
private:
	MRectPosition m_pos;
	int m_xd;		//left-dist
	int m_yd;		//top-dist
	int m_xd2;	//pos가 DYNAMIC이면 right-dist, 그 이외의 경우 width 입니다.
	int m_yd2;	//pos가 DYNAMIC이면 bottom-dist, 그 이외의 경우 height 입니다.
public:
	MRect(MRectPosition pos, int xd, int yd, int xd2, int yd2) {
		m_pos = pos;
		m_xd = xd;
		m_yd = yd;
		m_xd2 = xd2;
		m_yd2 = yd2;
	}
	MRect() {
		m_pos = MRectPosition::LT;
		m_xd = 10;
		m_yd = 10;
		m_xd2 = 100;
		m_yd2 = 100;
	}
	CRect GetRect(CRect view) {
		CRect rect;
		switch (m_pos) {
			case L: {
				rect.left = view.left + m_xd;
				rect.top = view.top + m_yd;
				rect.right = rect.left + m_xd2;
				rect.bottom = view.bottom - m_yd2;
			}break;
			case R: {
				rect.top = view.top + m_yd;
				rect.right = view.right - m_xd2;
				rect.bottom = view.bottom - m_yd2;
				rect.left = rect.right - m_xd;
			}break;
			case T: {
				rect.left = view.left + m_xd;
				rect.top = view.top + m_yd;
				rect.right = view.right - m_xd2;
				rect.bottom = rect.top + m_yd2;
			}break;
			case B: {
				rect.left = view.left + m_xd;
				rect.bottom = view.bottom - m_yd2;
				rect.right = view.right - m_xd2;
				rect.top = rect.bottom - m_yd;
			}break;
			case LT: {
				rect.left = view.left + m_xd;
				rect.top = view.top + m_yd;
				rect.right = rect.left + m_xd2;
				rect.bottom = rect.top + m_yd2;
			}break;
			case RT: {
				rect.right = view.right - m_xd;
				rect.top = view.top + m_yd;
				rect.left = rect.right - m_xd2;
				rect.bottom = rect.top + m_yd2;
			}break;
			case LB: {
				rect.left = view.left + m_xd;
				rect.right = rect.left + m_xd2;
				rect.bottom = view.bottom - m_yd;
				rect.top = rect.bottom - m_yd2;
			}break;
			case RB: {
				rect.right = view.right - m_xd;
				rect.bottom = view.bottom - m_yd;
				rect.left = rect.right - m_xd2;
				rect.top = rect.bottom - m_yd2;
			}break;
			case DYNAMIC: {
				rect.left = view.left + m_xd;
				rect.top = view.top + m_yd;
				rect.right = view.right - m_xd2;
				rect.bottom = view.bottom - m_yd2;
			}break;
			default:break;
		}
		return rect;
	}
};
enum MControlState {
	NORMAL, HOVER, CLICK
};
using DoFunc_VP = UINT(*)(void*);
using DoFunc = UINT(*)();
inline void ThreadEvent(DoFunc_VP func, void* param) {
	CWinThread* pthread = AfxBeginThread(func, param);
}
template<class T>
T CreateControl(CWnd* pWnd, MRect base) {
	return T(new T::element_type(pWnd, base));
}
class MControlObject {
public:
	CWnd* m_parent;			//부모 윈도우에 대한 포인터 입니다. 주로 View를 의미 합니다.
	MControlState m_state;		//컨트롤의 상태를 나타냅니다. 
	MRect m_rect;				//컨트롤의 위치를 나타냅니다.
public:	///색상 및 글꼴
	std::atomic<COLORREF>* m_color_bk;		//배경색을 지정합니다.
	std::atomic<COLORREF>* m_color_fr;		//전경색을 지정합니다.
	std::atomic<COLORREF>* m_color_text;	//글자색을 지정합니다.
	std::atomic<COLORREF>* m_color_other;	//기타색을 지정합니다.
	std::atomic<COLORREF>* m_color_view;	//부모 뷰의 색을 지정합니다.
	TString m_font_str;		//글꼴을 나타냅니다.
protected:
	static std::atomic<COLORREF> s_color_bk;
	static std::atomic<COLORREF> s_color_fr;
	static std::atomic<COLORREF> s_color_text;
	static std::atomic<COLORREF> s_color_other;
	static std::atomic<COLORREF> s_color_view;
	static TCHAR s_font_str[256];
public:
	static int s_id;				//객체의 아이디를 관리 합니다.	
	static int s_curr_id;			//현재 Focused 된 아이디 입니다.	
	int m_id;						//객체의 고유 id 입니다.
public:
	virtual bool isFocused() {
		return m_id == s_curr_id;
	}
	virtual bool isClicked() {
		return false;
	}
	virtual bool isChanged() {
		return false;
	}

	MControlObject(CWnd* parent, MRect rect) {
		m_parent = parent;
		m_rect = rect;
		m_color_bk = &s_color_bk;
		m_color_fr = &s_color_fr;
		m_color_text = &s_color_text;
		m_color_other = &s_color_other;
		m_font_str = s_font_str;
		m_state = MControlState::NORMAL;
		m_id = s_id;
		s_id++;
	}
public:
	CPoint GetMousePoint() {
		CPoint point;
		::GetCursorPos(&point);
		//std::cout << point.x << "," << point.y << std::endl;
		//ScreenToClient(this->m_parent->GetSafeHwnd(), &point);
		CRect rect;
		m_parent->GetWindowRect(&rect);
		point.x -= rect.left;
		point.y -= rect.top;
		return point;
	}
	CRect GetViewRect() {
		CRect rect;
		m_parent->GetClientRect(rect);
		return rect;
	}
	static COLORREF GetBrightColor(COLORREF color) {
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
		r = mspring::Min(r + 10, 255);
		g = mspring::Min(g + 10, 255);
		b = mspring::Min(b + 10, 255);
		return RGB(r, g, b);
	}
	static COLORREF GetDarkColor(COLORREF color) {
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
		r = mspring::Max(r - 10, 0);
		g = mspring::Max(g - 10, 0);
		b = mspring::Max(b - 10, 0);
		return RGB(r, g, b);
	}
public:
	virtual INT OnPaint(CDC* pDC) {
		return 1;
	}
	virtual INT OnLButtonDown() {
		//반드시 이 함수를 자식 클래스에서 호출해야 합니다.
		CPoint point = this->GetMousePoint();
		//std::cout << "(" << point.x << "," << point.y << ")" << std::endl;
		CRect rect;
		m_parent->GetClientRect(&rect);
		CRect crect = m_rect.GetRect(rect);
		//std::cout << "(" << crect.left << "," << crect.top << "," << crect.right << "," << crect.bottom << ")" << std::endl;
		if (crect.PtInRect(point) == TRUE) {
			s_curr_id = m_id;
		} else {
			if (s_curr_id == m_id) {
				s_curr_id = -1;
			}
		}
		return 1;
	}
	virtual INT OnLButtonUp() {
		return 1;
	}
	virtual INT OnRButtonDown() {
		return 1;
	}
	virtual INT OnRButtonUp() {
		return 1;
	}
	virtual INT OnLButtonDblClk() {
		return 1;
	}
	virtual INT OnMouseMove() {
		return 1;
	}
	virtual INT OnMouseWheel(short zDelta) {
		return 1;
	}
	virtual INT OnMouseLeave() {
		return 1;
	}
	virtual INT OnChar(UINT nchar) {
		return 1;
	}
	virtual INT OnComposition(WPARAM wParam, LPARAM lParam) {
		return 1;
	}
	virtual INT OnKeyDown(UINT nChar) {
		return 1;
	}
	virtual INT OnKeyUp(UINT nChar) {
		return 1;
	}
	virtual INT OnTimer(UINT_PTR nIDEvent) {
		return 1;
	}
	//모든 설정을 초기화 합니다.
	virtual INT Reset() {
		return 1;
	}
};
__declspec(selectany) std::atomic<COLORREF> MControlObject::s_color_bk = RGB(199,199,199);
__declspec(selectany) std::atomic<COLORREF> MControlObject::s_color_fr = RGB(38,139,210);
__declspec(selectany) std::atomic<COLORREF> MControlObject::s_color_text = RGB(0,0,0);
__declspec(selectany) std::atomic<COLORREF> MControlObject::s_color_other = RGB(147,161,161);
__declspec(selectany) std::atomic<COLORREF> MControlObject::s_color_view = RGB(255, 255, 255);
__declspec(selectany) TCHAR MControlObject::s_font_str[256] = TEXT("Arial");
__declspec(selectany) int MControlObject::s_id = 0;
__declspec(selectany) int MControlObject::s_curr_id = -1;
#endif  //MSPRING_7E2_4_7_MCONTROLOBJECT_HPP_INCLUDED
