/*
*  MSpringTaFrame.hpp
*  MSpring
*
*  Created by kimbom on 2017. 9. 12...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_MSPRINGTAFRAME_HPP_INCLUDED)
#define MSPRING_7E1_9_C_MSPRINGTAFRAME_HPP_INCLUDED
#include<afxwin.h>

#include "MSpringFrame.h"
#include<deque>
class MSpringTabFrame : public MSpringFrameExpansion {
	/*
	*	@class : BMDLTabFrame
	*	@comment : 이 클래스는 BMDLFrame에 사용될 확장 클래스 입니다.
	*				 , 커스텀 탭을 제공 합니다.
	*/
protected:		//inner class
	class Tab {
	public:
		CString m_str;
		UINT m_bitmap_resource;	//이 변수는 현재 사용되지 않습니다.
		Tab(CString str, UINT bitmap_resource = 0) {
			m_str = str;
			m_bitmap_resource = bitmap_resource;
		}
	};
protected:
	UINT m_position = 0;				//align 입니다. 0일경우 left-align, 그 이외의 경우 right-align
	std::deque<CRect> m_tab_rect;		//탭의 영역을 나타냅니다.
	std::vector<Tab> m_tab;				//탭을 나타냅니다.
	int m_tab_idx = -1;					//현재 선택된 탭 입니다.	
	int m_tab_hover = -1;				//현재 Hover된 탭 인덱스 입니다.
protected:		//style value
	CString m_font_str;				//탭에 사용될 폰트입니다.
	COLORREF m_color_activate;		//활성화된 탭의 배경색 입니다.
	COLORREF m_color_deactivate;		//비-활성화된 탭의 배경색 입니다.
	COLORREF m_color_bk;				//배경색
	COLORREF m_color_text;			//글꼴 색
	COLORREF m_color_hover;			//hover된 탭의 배경색 입니다. 이 색은 자동으로 지정됩니다.
	COLORREF m_color_bitmap_transparent;	//이 변수는 아직 사용되지 않습니다.
public:
	void SetStyle(CString font, COLORREF color_activate, COLORREF color_deactivate, COLORREF color_bk, COLORREF color_text, COLORREF color_bitmap_transparent = RGB(0, 0, 0)) {
		m_font_str = font;
		m_color_activate = color_activate;
		m_color_deactivate = color_deactivate;
		m_color_bk = color_bk;
		m_color_text = color_text;
		m_color_bitmap_transparent = color_bitmap_transparent;
		int r = ((int)GetRValue(color_activate) + (int)GetRValue(color_deactivate)) / 2;
		int g = ((int)GetGValue(color_activate) + (int)GetGValue(color_deactivate)) / 2;
		int b = ((int)GetBValue(color_activate) + (int)GetBValue(color_deactivate)) / 2;
		m_color_hover = RGB(r, g, b);
	}
	void SetPosition(UINT left_is_zero_right_is_nonzero) {
		m_position = left_is_zero_right_is_nonzero;
	}
	void AddTab(CString str, UINT bitmap_id = 0) {
		m_tab_idx = 0;
		m_tab.push_back(Tab(str, bitmap_id));
	}
	int GetCurrentTab() {
		return m_tab_idx;
	}
public:
	MSpringTabFrame(CWnd* wnd) :MSpringFrameExpansion(wnd) {
	};
	virtual ~MSpringTabFrame() = default;
public:
	void OnCreate(LPCREATESTRUCT lpCreateStruct)override {

	}
	int OnNcPaint(CDC* pDC, CRect rect)override {
		auto CutText = [](CString str, CDC* pDC, int w)->CString {
			CSize sz;
			::GetTextExtentPoint32(pDC->GetSafeHdc(), str, str.GetLength(), &sz);
			while (sz.cx > w) {
				str.Delete(str.GetLength() - 1);
				::GetTextExtentPoint32(pDC->GetSafeHdc(), str, str.GetLength(), &sz);
			}
			return str;
		};
		auto MakeGradation = [](COLORREF src, COLORREF dst, int n)->std::vector<COLORREF> {
			int r1 = GetRValue(src);
			int g1 = GetGValue(src);
			int b1 = GetBValue(src);
			int r2 = GetRValue(dst);
			int g2 = GetGValue(dst);
			int b2 = GetBValue(dst);
			n--;
			std::vector<COLORREF> ret;
			for (int i = 0; i <= n; i++) {
				int r = r1 + ((r2 - r1) / n)*i;
				int g = g1 + ((g2 - g1) / n)*i;
				int b = b1 + ((b2 - b1) / n)*i;
				ret.push_back(RGB(r, g, b));
			}
			return ret;
		};
		const int margin = 3;
		const int padding = 6;
		rect.top += margin;
		//rect.bottom -= margin;
		int default_width = 150;
		if (default_width*(int)m_tab.size() > rect.Width()) {
			default_width = rect.Width() / (int)m_tab.size();
		}
		int ret = 0;
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, m_color_activate);
		CPen pen_bk;
		pen_bk.CreatePen(PS_SOLID, 1, m_color_bk);
		CFont font;
		int h = mspring::Font::GetRealFontHeight(m_font_str, rect.Height() - padding * 2, pDC);
		font.CreatePointFont(h, m_font_str, pDC);
		m_tab_rect.clear();
		CBrush brush;
		brush.CreateSolidBrush(m_color_bk);
		pDC->FillRect(&rect, &brush);
		if (m_position == 0) {	//left align
			int posx = rect.left;
			for (int i = 0; i < (int)m_tab.size(); i++) {
				std::vector<COLORREF> gra;
				CBrush brush_tab;
				m_tab_rect.push_back(CRect(posx, rect.top, posx + default_width, rect.bottom));
				CRect rect_draw = m_tab_rect.back();
				if (i == m_tab_hover) {
					brush_tab.CreateSolidBrush(m_color_hover);
				} else if (i == m_tab_idx) {
					brush_tab.CreateSolidBrush(m_color_activate);
				} else {
					brush_tab.CreateSolidBrush(m_color_deactivate);
				}

				pDC->FillRect(&rect_draw, &brush_tab);
				CPen* old_pen = nullptr;
				old_pen = pDC->SelectObject(&pen_bk);
				if (i != 0) {
					pDC->MoveTo(CPoint(rect_draw.left, rect_draw.top));
					pDC->LineTo(CPoint(rect_draw.left, rect_draw.bottom));
				}
				if (i != m_tab.size() - 1) {
					pDC->MoveTo(CPoint(rect_draw.right, rect_draw.top));
					pDC->LineTo(CPoint(rect_draw.right, rect_draw.bottom));
				}
				pDC->SelectObject(old_pen);
				old_pen = pDC->SelectObject(&pen);
				for (int j = 0; j < 3; j++) {
					pDC->MoveTo(CPoint(rect_draw.left, rect_draw.bottom - 1 - j));
					pDC->LineTo(CPoint(rect_draw.right, rect_draw.bottom - 1 - j));
				}
				pDC->SelectObject(old_pen);
				CString str = CutText(m_tab[i].m_str, pDC, default_width);
				CSize sz;
				::GetTextExtentPoint32(pDC->GetSafeHdc(), str, str.GetLength(), &sz);
				pDC->SetTextColor(m_color_text);
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOut(posx + (default_width - sz.cx) / 2, margin + padding, str);



				posx += default_width;
				brush_tab.DeleteObject();
			}
			ret = posx - rect.left;
		} else {	//right align
			int posx = rect.right - default_width;
			for (int i = (int)m_tab.size() - 1; i >= 0; i--) {
				std::vector<COLORREF> gra;
				CBrush brush_tab;
				m_tab_rect.push_front(CRect(posx, rect.top, posx + default_width, rect.bottom));
				CRect rect_draw = m_tab_rect.front();
				if (i == m_tab_hover) {
					brush_tab.CreateSolidBrush(m_color_hover);
				} else if (i == m_tab_idx) {
					brush_tab.CreateSolidBrush(m_color_activate);
				} else {
					brush_tab.CreateSolidBrush(m_color_deactivate);
				}

				pDC->FillRect(&rect_draw, &brush_tab);
				CPen* old_pen = nullptr;
				old_pen = pDC->SelectObject(&pen_bk);
				if (i != 0) {
					pDC->MoveTo(CPoint(rect_draw.left, rect_draw.top));
					pDC->LineTo(CPoint(rect_draw.left, rect_draw.bottom));
				}
				if (i != m_tab.size() - 1) {
					pDC->MoveTo(CPoint(rect_draw.right, rect_draw.top));
					pDC->LineTo(CPoint(rect_draw.right, rect_draw.bottom));
				}
				pDC->SelectObject(old_pen);
				old_pen = pDC->SelectObject(&pen);
				for (int j = 0; j < 3; j++) {
					pDC->MoveTo(CPoint(rect_draw.left, rect_draw.bottom - 1 - j));
					pDC->LineTo(CPoint(rect_draw.right, rect_draw.bottom - 1 - j));
				}
				pDC->SelectObject(old_pen);
				CString str = CutText(m_tab[i].m_str, pDC, default_width);
				CSize sz;
				::GetTextExtentPoint32(pDC->GetSafeHdc(), str, str.GetLength(), &sz);
				pDC->SetTextColor(m_color_text);
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOut(posx + (default_width - sz.cx) / 2, margin + padding, str);



				posx -= default_width;
				brush_tab.DeleteObject();
			}
			ret = -(rect.right - posx - default_width);
		}

		pen_bk.DeleteObject();
		pen.DeleteObject();
		font.DeleteObject();
		brush.DeleteObject();
		return ret;
	}
	void OnSize(UINT nType, int cx, int cy)override {

	}
	bool OnNcLButtonDown(UINT nHitTest, CPoint point) override {
		bool ret = false;
		for (int i = 0; i < (int)m_tab_rect.size(); i++) {
			if (m_tab_rect[i].PtInRect(point) == TRUE) {
				m_tab_idx = i;
				ret = true;
				break;
			}
		}
		return ret;
	}
	bool OnNcMouseMove(UINT nHitTest, CPoint point) override {
		m_tab_hover = -1;
		bool ret = false;
		for (int i = 0; i < (int)m_tab_rect.size(); i++) {
			if (m_tab_rect[i].PtInRect(point) == TRUE) {
				m_tab_hover = i;
				ret = true;
				break;
			}
		}
		return ret;
	}
	bool OnNcLButtonUp(UINT nHitTest, CPoint point)override {
		return false;
	}
	bool OnNcLButtonDblClk(UINT nHitTest, CPoint point)override {
		return false;
	}
	void OnDestroy()override {

	}
	void OnNcMouseLeave() override {
		m_tab_hover = -1;
	}
};

#endif  //MSPRING_7E1_9_C_MSPRINGTAFRAME_HPP_INCLUDED