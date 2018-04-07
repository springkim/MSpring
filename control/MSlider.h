/*
*  mslider.h
*  MSpring
*
*  Created by KimBomm on 2018. 4. 7...
*  Copyright 2018 KimBomm. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E2_4_7_MSLIDER_HPP_INCLUDED)
#define MSPRING_7E2_4_7_MSLIDER_HPP_INCLUDED
#include"MControlObject.h"
class MSlider_core : public MControlObject {
#define MSLIDER_TIMER		17789
public:
	TString m_text;
	float m_pos = 0.1F;
	bool m_vertical = true;	//이 값이 true이면 수직 슬라이더 입니다.
private:
	bool m_click = false;
	CRect m_thumb;
public:
	MSlider_core(CWnd* parent, MRect base) : MControlObject(parent, base) {

	}
	INT OnPaint(CDC* pDC) {
		CPoint point = this->GetMousePoint();
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);

		if (m_vertical == false) {
			auto DrawHSlider = [](CDC* pDC, CRect rect, COLORREF color)->void {
				CBrush brush;
				brush.CreateSolidBrush(color);
				CPen pen_null;
				pen_null.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
				CPen* old_pen = pDC->SelectObject(&pen_null);
				CBrush* old_brush = pDC->SelectObject(&brush);
				CRect ellipse_left = rect;
				ellipse_left.right = ellipse_left.left + rect.Height();
				CRect ellipse_right = rect;
				ellipse_right.left = ellipse_right.right - rect.Height();
				pDC->Ellipse(ellipse_left);
				if (ellipse_left.Width() <= rect.Width()) {
					pDC->Ellipse(ellipse_right);
				}
				CRect rect_inside = rect;
				rect_inside.left += rect.Height() / 2;
				rect_inside.right -= rect.Height() / 2;
				if (ellipse_left.Width() <= rect.Width()) {
					pDC->Rectangle(rect_inside);
				}
				pDC->SelectObject(old_pen);
				pDC->SelectObject(old_brush);
				pen_null.DeleteObject();
				brush.DeleteObject();
			};
			DrawHSlider(pDC, rect, *m_color_bk);
			CRect rect_activate = rect;
			rect_activate.right = static_cast<LONG>(rect.left + rect.Height() + (m_pos*(rect.Width() - rect.Height())));
			DrawHSlider(pDC, rect_activate, *m_color_fr);

			int base = rect.left + rect.Height() / 2;
			int width = rect.Width() - rect.Height();
			int dst = static_cast<int>(base + m_pos*width);
			m_thumb.left = dst - rect.Height() / 2;
			m_thumb.right = dst + rect.Height() / 2;
			m_thumb.top = rect.top;
			m_thumb.bottom = rect.bottom;
			CBrush brush;
			brush.CreateSolidBrush(*m_color_other);
			CPen pen_null;
			pen_null.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
			CPen* old_pen = pDC->SelectObject(&pen_null);
			CBrush* old_brush = pDC->SelectObject(&brush);
			pDC->Ellipse(m_thumb);
			pDC->SelectObject(old_pen);
			pDC->SelectObject(old_brush);
			pen_null.DeleteObject();
			brush.DeleteObject();
		}

		return 1;
	}
	INT OnLButtonDown() override {
		MControlObject::OnLButtonDown();
		if (m_thumb.PtInRect(this->GetMousePoint()) == TRUE) {
			m_click = s_curr_id == m_id;
		};
		return 1;
	}
	INT OnMouseMove()override {
		MControlObject::OnMouseMove();
		if (s_curr_id != m_id) {
			return 1;
		}
		if (m_click == false) {
			return 1;
		}
		CPoint point = this->GetMousePoint();
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);
		if (m_vertical == true) {
			m_pos = static_cast<float>(rect.bottom - point.y) / rect.Height();
		} else {
			m_pos = static_cast<float>(point.x - (rect.left + rect.Height() / 2)) / (rect.Width() - rect.Height());
		}
		mspring::SetRange(m_pos, 0.0F, 1.0F);
		return 1;
	}
	INT OnLButtonUp() {
		MControlObject::OnLButtonUp();
		m_click = false;
		return 1;
	}
	INT OnMouseLeave() {
		m_click = false;
		return 1;
	}
	INT OnTimer(UINT_PTR nIDEvent) {
		return 1;
	}
};
using MSlider = std::shared_ptr<MSlider_core>;
#endif  //MSPRING_7E2_4_7_MSLIDER_HPP_INCLUDED