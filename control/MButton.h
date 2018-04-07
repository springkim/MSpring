/*
*  mbutton.h
*  MSpring
*
*  Created by KimBomm on 2018. 4. 7...
*  Copyright 2018 KimBomm. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E2_4_7_MBUTTON_HPP_INCLUDED)
#define MSPRING_7E2_4_7_MBUTTON_HPP_INCLUDED
#include"MControlObject.h"
class MButton_core : public MControlObject {
public:
	TString m_text = TEXT("Button");
	DoFunc m_func = nullptr;
public:
	MButton_core(CWnd* parent, MRect base) : MControlObject(parent, base) {

	}
	INT OnPaint(CDC* pDC)override {
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);
		CPen null_pen; null_pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		CBrush brush;
		if (this->m_state == MControlState::CLICK) {
			brush.CreateSolidBrush(GetDarkColor(*this->m_color_fr));
		} else if (this->m_state == MControlState::NORMAL) {
			brush.CreateSolidBrush(*this->m_color_fr);
		} else if (this->m_state == MControlState::HOVER) {
			brush.CreateSolidBrush(GetBrightColor(*this->m_color_fr));
		}
		CPen* old_pen = pDC->SelectObject(&null_pen);
		CBrush* old_brush = pDC->SelectObject(&brush);
		pDC->RoundRect(&rect, CPoint(5, 5));
		int h = mspring::Font::GetRealFontHeight(m_font_str, rect.Height() - 6, pDC, m_text, true);
		int h2 = mspring::Font::GetRealFontHeight(m_font_str, rect.Width() - 6, pDC, m_text, false);
		h = mspring::Min(h, h2);
		CFont font;
		font.CreatePointFont(h, m_font_str.data());
		CFont* old_font = pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(*m_color_text);
		CSize sz;
		::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text.data(), m_text.length(), &sz);
		pDC->TextOut((rect.Width() - sz.cx) / 2 + rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text.data());
		pDC->SelectObject(old_font);
		pDC->SelectObject(old_pen);
		pDC->SelectObject(old_brush);
		font.DeleteObject();
		brush.DeleteObject();
		null_pen.DeleteObject();
		return 1;
	}
	INT OnLButtonDown()override {
		MControlObject::OnLButtonDown();
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
			if (m_func != nullptr) {
				m_func();
			}
			return 1;
		}
		return 1;
	}
	INT OnMouseMove()override {
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
using MButton = std::shared_ptr<MButton_core>;
#endif  //MSPRING_7E2_4_7_MBUTTON_HPP_INCLUDED