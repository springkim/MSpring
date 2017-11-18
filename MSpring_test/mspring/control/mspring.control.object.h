/**
* @file		mspring.control.object.h
* @author		kimbomm (springnode@gmail.com)
* @date		2017. 10. 10...
* @version	1.0.0
*
*  @brief
*			MSpring 공용 컨트롤
*	@remark
*			Created by kimbom on 2017. 10. 10...
*			Copyright 2017 kimbom.All rights reserved.
*/
#pragma once
#if !defined(MSPRING_7E1_A_9__CONTROLS_HPP_INCLUDED) 
#define MSPRING_7E1_A_9__CONTROLS_HPP_INCLUDED
#include<afxwin.h>
#include<atomic>
#include"_rect.h"
#include"../utils.h"
#define M_CLICKED 255
enum MControlState {
	NORMAL, HOVER, CLICK
};
using DoFunc_VP = UINT(*)(void*);
using DoFunc = UINT(*)();
inline void ThreadEvent(DoFunc_VP func, void* param) {
	CWinThread* pthread = AfxBeginThread(func, param);
	if (pthread != NULL) {
		CloseHandle(pthread);
	}
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
	CString m_font_str;		//글꼴을 나타냅니다.
protected:
	static std::atomic<COLORREF> s_color_bk;
	static std::atomic<COLORREF> s_color_fr;
	static std::atomic<COLORREF> s_color_text;
	static std::atomic<COLORREF> s_color_other;
	static std::atomic<COLORREF> s_color_view;
	static CString s_font_str;
protected:
	static int s_id;				//객체의 아이디를 관리 합니다.	
	static int s_curr_id;			//현재 Focused 된 아이디 입니다.	
	int m_id;						//객체의 고유 id 입니다.
public:
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
	COLORREF GetBrightColor(COLORREF color) {
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
		r = mspring::Min(r + 10, 255);
		g = mspring::Min(g + 10, 255);
		b = mspring::Min(b + 10, 255);
		return RGB(r, g, b);
	}
	COLORREF GetDarkColor(COLORREF color) {
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
		CRect rect;
		m_parent->GetClientRect(&rect);
		if (m_rect.GetRect(rect).PtInRect(point) == TRUE) {
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
	virtual INT OnKeyDown(UINT nChar) {
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
__declspec(selectany) std::atomic<COLORREF> MControlObject::s_color_other = RGB(220,50,47);
__declspec(selectany) std::atomic<COLORREF> MControlObject::s_color_view = RGB(255, 255, 255);
__declspec(selectany) CString MControlObject::s_font_str = TEXT("Arial");
__declspec(selectany) int MControlObject::s_id = 0;
__declspec(selectany) int MControlObject::s_curr_id = -1;
#endif  //MSPRING_7E1_A_9__CONTROLS_HPP_INCLUDED
