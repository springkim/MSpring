/*
*  msingleedit.hpp
*  MSpring
*
*  Created by KimBomm on 2018. 4. 7...
*  Copyright 2018 KimBomm. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E2_4_7_MSINGLEEDIT_HPP_INCLUDED)
#define MSPRING_7E2_4_7_MSINGLEEDIT_HPP_INCLUDED
#include"MControlObject.h"
class MSingleEdit_core : public MControlObject {
	const UINT_PTR MEDIT_TIMER = (UINT_PTR)"mspring.control.singleedit.h(MEDIT_TIMER)";
public:
	TString m_text;
private:
	bool caret = false;
	int pos = 0;
public:
	MSingleEdit_core(CWnd* parent, MRect base) : MControlObject(parent, base) {

	}
	INT OnPaint(CDC* pDC) {
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);
		CPen* old_pen = nullptr;
		int h = mspring::Font::GetRealFontHeight(m_font_str, rect.Height() - 6, pDC, m_text, true);
		int h2 = mspring::Font::GetRealFontHeight(m_font_str, rect.Width() - 6, pDC, m_text, false);
		h = mspring::Min(h, h2);

		CPen pen;
		pen.CreatePen(PS_SOLID, 1, *m_color_fr);
		old_pen = pDC->SelectObject(&pen);
		pDC->MoveTo(rect.left, rect.bottom);
		pDC->LineTo(rect.right, rect.bottom);
		if (h < 2)return 0;
		CFont font;
		font.CreatePointFont(h, m_font_str.data());
		CFont* old_font = pDC->SelectObject(&font);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(*m_color_text);
		CSize sz;
		::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text.data(), m_text.length(), &sz);

		pDC->TextOut(rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text.data());



		if (m_id == s_curr_id && caret == true) {
			CPen pen;
			pen.CreatePen(PS_SOLID, 2, *m_color_fr);
			pDC->SelectObject(&pen);
			::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text.data(), pos, &sz);
			int x = rect.left + sz.cx;
			int width = mspring::Min(rect.Width() - sz.cx, (long)rect.Height());
			pDC->MoveTo(x, rect.top);
			pDC->LineTo(x, rect.bottom);

			pen.DeleteObject();
		}


		pDC->SelectObject(old_font);
		pDC->SelectObject(old_pen);
		pen.DeleteObject();
		font.DeleteObject();
		return 1;
	}

	INT OnLButtonDown() override {
		MControlObject::OnLButtonDown();
		if (s_curr_id == m_id) {
			m_parent->SetTimer(MEDIT_TIMER + m_id, 500, nullptr);
		} else {
			m_parent->KillTimer(MEDIT_TIMER + m_id);
			caret = false;
		}
		return 1;
	}
	INT OnKeyDown(UINT nChar)override {
		if (s_curr_id != m_id) return 0;
		switch (nChar) {
			case VK_LEFT:pos--; caret = true; break;
			case VK_RIGHT:pos++; caret = true; break;
			case VK_DELETE: {
				if (pos <m_text.length()) {
					m_text.erase(m_text.begin() + pos);
				}
			}break;
			default:break;
		}
		mspring::SetRange(pos, 0, (int)m_text.length());
		return 1;
	}
	INT OnChar(UINT nChar) override {
		if (s_curr_id != m_id) return 0;
		std::cout << nChar << std::endl;
		if (nChar == VK_BACK) {
			if (pos > 0) {
				m_text.erase(m_text.begin() + (pos - 1));
				pos--;
			}
		}else {
			if (nChar == VK_TAB) {
				for (int i = 0; i < 4; i++) {
					m_text.insert(m_text.begin() + pos, TEXT(' '));
					pos++;
				}
			} else {
				m_text.insert(m_text.begin() + pos, nChar);
				pos++;
			}
		}
		caret = true;
		return 1;
	}
	BOOL m_compstr = FALSE;
	INT OnComposition(WPARAM wParam, LPARAM lParam) override {
		if (s_curr_id != m_id) return 0;
		HIMC hImc = ImmGetContext(this->m_parent->GetSafeHwnd());//현재 IME가져오기
		TCHAR* result = NULL;
		TCHAR dest[3];
		BOOL ins = FALSE;
		LONG len = 0;
		if (this->m_compstr == FALSE) {
			ins = TRUE;
		}
		if (lParam & GCS_RESULTSTR) {//완성된 글자
			if (hImc) {//IME상태이면
				len = ImmGetCompositionStringW(hImc, GCS_RESULTSTR, NULL, 0);//완성된 글자의 길이 가져오기
				if (len > 0) {
					result = new TCHAR[len + 1];//길이+1 만큼의 공간 생성(마지막에 \0 넣어야하기때문에)
					ImmGetCompositionString(hImc, GCS_RESULTSTR, result, len);//완성된 글자 가져오기 result의 저장
					result[len] = L'\0';//글자 마무리
					this->m_compstr = FALSE;
				}
				ImmReleaseContext(this->m_parent->GetSafeHwnd(), hImc);
			}
		} else if (lParam & GCS_COMPSTR) { //조합중 글자
			if (hImc) { //IME상태이면
				len = ImmGetCompositionStringW(hImc, GCS_COMPSTR, NULL, 0); //조합중 글자의 길이 가져오기
				if (len > 0) {
					result = new TCHAR[len + 1];
					ImmGetCompositionString(hImc, GCS_COMPSTR, result, len); //조합중 글자 가져오기 result의 저장
					result[len] = L'\0'; //글자 가져오기
					this->m_compstr = TRUE;
				} else {	//한글이 완성이 아닌데 지웠을경우
					m_text.erase(m_text.begin() + (pos - 1));
					pos--;
				}
				ImmReleaseContext(this->m_parent->GetSafeHwnd(), hImc);
			}
		}
		if (result != NULL) {
			_tcscpy_s(dest, result);
			dest[1] = 0;
			delete[] result;
		} else {
			_tcscpy_s(dest, TEXT(""));
			this->m_compstr = FALSE;
		}

		if (len > 0) {
			if (ins) {
				m_text.insert(m_text.begin() + pos, dest[0]);
				pos++;
			} else {
				m_text[pos - 1] = dest[0];
			}
		}
		return 1;
	}
	INT OnTimer(UINT_PTR nIDEvent) {
		if (s_curr_id != m_id) return 0;
		if (nIDEvent >= MEDIT_TIMER && nIDEvent <= MEDIT_TIMER + (UINT_PTR)s_id) {
			caret = !caret;
			m_parent->Invalidate();
		}
		return 1;
	}
	INT Reset() override {
		m_text.clear();
		pos = 0;
		caret = true;
		return 1;
	}
};
using MSingleEdit = std::shared_ptr<MSingleEdit_core>;
#endif  //MSPRING_7E2_4_7_MSINGLEEDIT_HPP_INCLUDED