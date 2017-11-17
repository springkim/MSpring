#pragma once
#include"mspring.control.object.h"

class MButton : public MControlObject{
public:
	CString m_text = TEXT("Button");
	DoFunc m_func = nullptr;
public:
	MButton(CWnd* parent, MRect base): MControlObject(parent,base){
		
	}
	void OnPaint(CDC* pDC)override {
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);
		CPen null_pen;null_pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		CBrush brush;
		if (this->m_state == MControlState::CLICK) {
			brush.CreateSolidBrush(GetDarkColor(*this->m_color_fr));
		}else if (this->m_state == MControlState::NORMAL) {
			brush.CreateSolidBrush(*this->m_color_fr);
		} else if(this->m_state == MControlState::HOVER){
			brush.CreateSolidBrush(GetBrightColor(*this->m_color_fr));
		}
		CPen* old_pen= pDC->SelectObject(&null_pen);
		CBrush* old_brush = pDC->SelectObject(&brush);
		pDC->RoundRect(&rect, CPoint(5, 5));
		int h = mspring::Font::GetRealFontHeight(m_font_str, rect.Height()-6, pDC, m_text, true);
		int h2= mspring::Font::GetRealFontHeight(m_font_str, rect.Width()-6, pDC, m_text,false);
		h = mspring::Min(h, h2);
		CFont font;
		font.CreatePointFont(h, m_font_str);
		CFont* old_font=pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(*m_color_text);
		CSize sz;
		::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text, m_text.GetLength(), &sz);
		pDC->TextOut((rect.Width() - sz.cx) / 2+rect.left, (rect.Height() - sz.cy) / 2+rect.top, m_text);
		pDC->SelectObject(old_font);
		pDC->SelectObject(old_pen);
		pDC->SelectObject(old_brush);
		font.DeleteObject();
		brush.DeleteObject();
		null_pen.DeleteObject();
	}
	void OnLButtonDown()override {
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
		}
	}
	void OnMouseMove()override {
		CPoint point = this->GetMousePoint();
		CRect rect;
		m_parent->GetClientRect(&rect);
		if (m_rect.GetRect(rect).PtInRect(point) == TRUE) {
			m_state = MControlState::HOVER;
		} else {
			m_state = MControlState::NORMAL;
		}
	}
};
class MButtonCheck : public MControlObject {
public:
	bool check = false;
public:
	MButtonCheck(CWnd* parent, MRect base) : MControlObject(parent, base) {
		
	}
	void OnPaint(CDC* pDC) {
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);
		CPen null_pen;null_pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		CBrush brush;
		if (this->m_state == MControlState::CLICK) {
			brush.CreateSolidBrush(GetDarkColor(*this->m_color_fr));
		} else if (this->m_state == MControlState::NORMAL) {
			brush.CreateSolidBrush(*this->m_color_fr);
		} else if (this->m_state == MControlState::HOVER) {
			brush.CreateSolidBrush(GetBrightColor(*this->m_color_fr));
		}
		CPen* old_pen=pDC->SelectObject(&null_pen);
		CBrush* old_brush = pDC->SelectObject(&brush);
		pDC->RoundRect(&rect, CPoint(5, 5));
		if (check == true) {
			CPen pen;
			pen.CreatePen(PS_SOLID, 3, *m_color_text);
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
	}
	void OnLButtonDown()override {
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
			check = !check;
		}
	}
	void OnMouseMove()override {
		CPoint point = this->GetMousePoint();
		CRect rect;
		m_parent->GetClientRect(&rect);
		if (m_rect.GetRect(rect).PtInRect(point) == TRUE) {
			m_state = MControlState::HOVER;
		} else {
			m_state = MControlState::NORMAL;
		}
	}
};
//class MButtonRadio : public MControlObject {};
class MStatic : public MControlObject {
public:
	CString m_text = TEXT("Static Text");
	DWORD align = 1;
#define MSTATIC_ALIGN_LEFT 1
#define MSTATIC_ALIGN_MIDDLE 2
#define MSTATIC_ALIGN_RIGHT 3
public:
	MStatic(CWnd* parent, MRect base) : MControlObject(parent, base) {

	}
	void OnPaint(CDC* pDC) {
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);

		int h = mspring::Font::GetRealFontHeight(m_font_str, rect.Height() - 6, pDC, m_text, true);
		int h2 = mspring::Font::GetRealFontHeight(m_font_str, rect.Width() - 6, pDC, m_text, false);
		h = mspring::Min(h, h2);
		CFont font;
		font.CreatePointFont(h, m_font_str);
		CFont* old_font = pDC->SelectObject(&font);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(*m_color_text);
		CSize sz;
		::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text, m_text.GetLength(), &sz);
		if (align == MSTATIC_ALIGN_MIDDLE) {
			pDC->TextOut((rect.Width() - sz.cx) / 2 + rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text);
		} else if (align == MSTATIC_ALIGN_LEFT) {
			pDC->TextOut(rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text);
		} else if (align == MSTATIC_ALIGN_RIGHT) {
			pDC->TextOut(rect.right-sz.cx, (rect.Height() - sz.cy) / 2 + rect.top, m_text);
		}
		pDC->SelectObject(old_font);
		font.DeleteObject();
	}
};
class MEdit : public MControlObject{
#define MEDIT_TIMER		17777
	CString m_text;
private:
	bool caret = false;
	int pos = 0;
public:
	MEdit(CWnd* parent, MRect base) : MControlObject(parent, base) {
	
	}
	void OnPaint(CDC* pDC) {
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
		if (h < 2)return;
		CFont font;
		font.CreatePointFont(h, m_font_str);
		CFont* old_font = pDC->SelectObject(&font);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(*m_color_text);
		CSize sz;
		::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text, m_text.GetLength(), &sz);
	
		pDC->TextOut(rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text);
		
		
		
		if (m_id == s_curr_id && caret==true) {
			CPen pen;
			pen.CreatePen(PS_SOLID, 2, *m_color_fr);
			pDC->SelectObject(&pen);
			::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text, pos, &sz);
			int x = rect.left + sz.cx;
			int width = mspring::Min(rect.Width() - sz.cx, (long)rect.Height());
			pDC->MoveTo(x,rect.top);
			pDC->LineTo(x,rect.bottom);

			pen.DeleteObject();
		}


		pDC->SelectObject(old_font);
		pDC->SelectObject(old_pen);

		font.DeleteObject();
	}

	void OnLButtonDown() override{
		MControlObject::OnLButtonDown();
		if (s_curr_id == m_id) {
			m_parent->SetTimer(MEDIT_TIMER + m_id, 500, nullptr);
		} else {
			m_parent->KillTimer(MEDIT_TIMER + m_id);
			caret = false;
		}
	}
	void OnKeyDown(UINT nChar)override {
		puts("OnKeyDown");
		switch (nChar) {
			case VK_LEFT:pos--; caret = true; break;
			case VK_RIGHT:pos++; caret = true; break;
			default:break;
		}
		mspring::SetRange(pos,0, m_text.GetLength());
	}
	void OnChar(UINT nChar) override{
		if (s_curr_id != m_id) {
			return;
		}
		if (nChar == VK_BACK) {
			if (pos > 0) {
				m_text.Delete(pos - 1);
				pos--;
			}
		} else {
			if (nChar == VK_TAB) {
				for (int i = 0; i < 4; i++) {
					m_text.Insert(pos, TEXT(' '));
					pos++;
				}
			} else {
				m_text.Insert(pos, nChar);
				pos++;
			}
		}
		caret = true;
	}
	BOOL m_compstr = FALSE;
	void OnComposition(WPARAM wParam, LPARAM lParam) {
		if (s_curr_id != m_id) {
			return;
		}
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
					m_text.Delete(pos - 1);
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
				m_text.Insert(pos,dest[0]);
				pos++;
			} else {
				m_text.SetAt(pos - 1, dest[0]);
			}
		}
	}
	void OnTimer(UINT_PTR nIDEvent) {
		if (s_curr_id != m_id) {
			return;
		}
		if (nIDEvent >= MEDIT_TIMER && nIDEvent <= MEDIT_TIMER + (UINT_PTR)s_id) {
			caret = !caret;
			m_parent->Invalidate();
		}
	}
};
