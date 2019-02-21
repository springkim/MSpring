/*
*  mstatic.h
*  MSpring
*
*  Created by KimBomm on 2018. 4. 7...
*  Copyright 2018 KimBomm. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E2_4_7_MSTATIC_HPP_INCLUDED)
#define MSPRING_7E2_4_7_MSTATIC_HPP_INCLUDED
#include"MControlObject.h"
class MStatic_core : public MControlObject {
public:
	TString m_text = TEXT("Static Text");
	UINT_PTR align;
	const UINT_PTR MSTATIC_ALIGN_LEFT = (UINT_PTR)"mspring.control.static.h(MSTATIC_ALIGN_LEFT)";
	const UINT_PTR MSTATIC_ALIGN_MIDDLE = (UINT_PTR)"mspring.control.static.h(MSTATIC_ALIGN_MIDDLE)";
	const UINT_PTR MSTATIC_ALIGN_RIGHT = (UINT_PTR)"mspring.control.static.h(MSTATIC_ALIGN_RIGHT)";
public:
	MStatic_core(CWnd* parent, MRect base) : MControlObject(parent, base) {
		align = MSTATIC_ALIGN_LEFT;
	}
	INT OnPaint(CDC* pDC) {
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);

		int h = mspring::Font::GetRealFontHeight(m_font_str, rect.Height() - 6, pDC, m_text, true);
		int h2 = mspring::Font::GetRealFontHeight(m_font_str, rect.Width() - 6, pDC, m_text, false);
		h = mspring::Min(h, h2);
		CFont font;
		font.CreatePointFont(h, m_font_str.data());
		CFont* old_font = pDC->SelectObject(&font);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_color_text);
		

		DWORD option_shadow = 0;
		DWORD option_depth = 0;
		CSize sz;
		CRect rsz = mspring::Text::TextOutMSP(pDC, 0, 0, m_text.data(), static_cast<int>(m_text.length()), m_font_str.data(), h, option_shadow, m_color_bk, true, option_depth);
		sz.cx = rsz.Width();
		sz.cy = rsz.Height();
		if (align == MSTATIC_ALIGN_MIDDLE) {
			mspring::Text::TextOutMSP(pDC, (rect.Width() - sz.cx) / 2 + rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text.data(), static_cast<int>(m_text.length()), m_font_str.data(), h, option_shadow, m_color_bk, false, option_depth);
			//pDC->TextOut((rect.Width() - sz.cx) / 2 + rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text.data());
		} else if (align == MSTATIC_ALIGN_LEFT) {
			mspring::Text::TextOutMSP(pDC, rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text.data(), static_cast<int>(m_text.length()), m_font_str.data(), h, option_shadow, m_color_bk, false, option_depth);
			//pDC->TextOut(rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text.data());
		} else if (align == MSTATIC_ALIGN_RIGHT) {
			mspring::Text::TextOutMSP(pDC, rect.right - sz.cx, (rect.Height() - sz.cy) / 2 + rect.top, m_text.data(), static_cast<int>(m_text.length()), m_font_str.data(), h, option_shadow, m_color_bk, false, option_depth);
			//pDC->TextOut(rect.right - sz.cx, (rect.Height() - sz.cy) / 2 + rect.top, m_text.data());
		}
		pDC->SelectObject(old_font);
		font.DeleteObject();
		return 1;
	}
};
using MStatic = std::shared_ptr<MStatic_core>;
#endif  //MSPRING_7E2_4_7_MSTATIC_HPP_INCLUDED