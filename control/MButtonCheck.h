/*
*  mbuttoncheck.h
*  MSpring
*
*  Created by KimBomm on 2018. 4. 7...
*  Copyright 2018 KimBomm. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E2_4_7_MBUTTONCHECK_HPP_INCLUDED)
#define MSPRING_7E2_4_7_MBUTTONCHECK_HPP_INCLUDED
#include"MControlObject.h"
class MButtonCheck_core : public MControlObject {
public:
	bool check = false;
	bool disable = false;
private:
	bool m_is_changed = false;
public:
	bool isChanged() override{
		return m_is_changed;
	}
	MButtonCheck_core(CWnd* parent, MRect base) : MControlObject(parent, base) {

	}
	INT OnPaint(CDC* pDC) {
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);
		CPen null_pen; null_pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		CBrush brush;
		if (disable == true) {
			brush.CreateSolidBrush(*this->m_color_other);
		} else if (this->m_state == MControlState::CLICK) {
			brush.CreateSolidBrush(GetDarkColor(*this->m_color_fr));
		} else if (this->m_state == MControlState::NORMAL) {
			brush.CreateSolidBrush(*this->m_color_fr);
		} else if (this->m_state == MControlState::HOVER) {
			brush.CreateSolidBrush(GetBrightColor(*this->m_color_fr));
		}
		CPen* old_pen = pDC->SelectObject(&null_pen);
		CBrush* old_brush = pDC->SelectObject(&brush);
		CDrawingManager dm(*pDC);
		dm.DrawShadow(&CRect(rect.left, rect.top + 1, rect.right - 3, rect.bottom - 2), 5);
		pDC->RoundRect(&rect, CPoint(5, 5));
		if (check == true && disable == false) {
			CPen pen;
			pen.CreatePen(PS_ALTERNATE, 3, *m_color_text);
			pDC->SelectObject(&pen);
			int divw = rect.Height() / 5;
			pDC->MoveTo(rect.left + divw, static_cast<int>(rect.top + divw*2.5));
			pDC->LineTo(static_cast<int>(rect.left + divw*1.5), rect.top + divw * 4);
			pDC->LineTo(rect.right - divw, rect.top + divw * 1);
			pen.DeleteObject();
		}
		pDC->SelectObject(old_pen);
		null_pen.DeleteObject();
		brush.DeleteObject();
		return 1;
	}
	INT OnLButtonDown()override {
		m_is_changed = false;
		MControlObject::OnLButtonDown();
		if (disable == true) {
			return 1;
		}
		CPoint point = this->GetMousePoint();
		CRect rect;
		m_parent->GetClientRect(&rect);
		if (m_rect.GetRect(rect).PtInRect(point) == TRUE) {
			void* param = this;
			ThreadEvent([](void* param)->UINT {
				MControlObject* mco = static_cast<MControlObject*>(param);
				mco->m_state = MControlState::CLICK;
				Sleep(100);
				mco->m_state = MControlState::NORMAL;
				mco->m_parent->Invalidate();
				return 1;
			}, param);
			check = !check;
			m_is_changed = true;
			return 1;
		}
		return 1;
	}
	INT OnNcLButtonDblClk() {
		return OnLButtonDown();
	}
	INT OnMouseMove()override {
		if (disable == true) {
			return 1;
		}
		CPoint point = this->GetMousePoint();
		CRect rect;
		m_parent->GetClientRect(&rect);
		if (m_rect.GetRect(rect).PtInRect(point) == TRUE) {
			m_state = MControlState::HOVER;
		} else {
			m_state = MControlState::NORMAL;
		}
		return 1;
	}
};
using MButtonCheck = std::shared_ptr<MButtonCheck_core>;
#endif  //MSPRING_7E2_4_7_MBUTTONCHECK_HPP_INCLUDED