#pragma once
#include"mspring.control.object.h"

class MButton : public MControlObject {
public:
	CString m_text = TEXT("Button");
	DoFunc m_func = nullptr;
public:
	MButton(CWnd* parent, MRect base) : MControlObject(parent, base) {

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
		font.CreatePointFont(h, m_font_str);
		CFont* old_font = pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(*m_color_text);
		CSize sz;
		::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text, m_text.GetLength(), &sz);
		pDC->TextOut((rect.Width() - sz.cx) / 2 + rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text);
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
			return M_CLICKED;
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
class MButtonCheck : public MControlObject {
public:
	bool check = false;
	bool disable = false;
public:
	MButtonCheck(CWnd* parent, MRect base) : MControlObject(parent, base) {

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
		pDC->RoundRect(&rect, CPoint(5, 5));
		if (check == true && disable == false) {
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
		return 1;
	}
	INT OnLButtonDown()override {
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
			return M_CLICKED;
		}
		return 1;
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
	INT OnPaint(CDC* pDC) {
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
			pDC->TextOut(rect.right - sz.cx, (rect.Height() - sz.cy) / 2 + rect.top, m_text);
		}
		pDC->SelectObject(old_font);
		font.DeleteObject();
		return 1;
	}
};
class MEdit : public MControlObject {
#define MEDIT_TIMER		17777
public:
	CString m_text;
private:
	bool caret = false;
	int pos = 0;
public:
	MEdit(CWnd* parent, MRect base) : MControlObject(parent, base) {

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
		font.CreatePointFont(h, m_font_str);
		CFont* old_font = pDC->SelectObject(&font);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(*m_color_text);
		CSize sz;
		::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text, m_text.GetLength(), &sz);

		pDC->TextOut(rect.left, (rect.Height() - sz.cy) / 2 + rect.top, m_text);



		if (m_id == s_curr_id && caret == true) {
			CPen pen;
			pen.CreatePen(PS_SOLID, 2, *m_color_fr);
			pDC->SelectObject(&pen);
			::GetTextExtentPoint32(pDC->GetSafeHdc(), m_text, pos, &sz);
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
		switch (nChar) {
			case VK_LEFT:pos--; caret = true; break;
			case VK_RIGHT:pos++; caret = true; break;
			default:break;
		}
		mspring::SetRange(pos, 0, m_text.GetLength());
		return 1;
	}
	INT OnChar(UINT nChar) override {
		if (s_curr_id != m_id) {
			return 0;
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
		return 1;
	}
	BOOL m_compstr = FALSE;
	INT OnComposition(WPARAM wParam, LPARAM lParam) {
		if (s_curr_id != m_id) {
			return 0;
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
				m_text.Insert(pos, dest[0]);
				pos++;
			} else {
				m_text.SetAt(pos - 1, dest[0]);
			}
		}
		return 1;
	}
	INT OnTimer(UINT_PTR nIDEvent) {
		if (s_curr_id != m_id) {
			return 0;
		}
		if (nIDEvent >= MEDIT_TIMER && nIDEvent <= MEDIT_TIMER + (UINT_PTR)s_id) {
			caret = !caret;
			m_parent->Invalidate();
		}
		return 1;
	}
	INT Reset() override {
		m_text.Empty();
		pos = 0;
		caret = true;
		return 1;
	}
};
class MSlider : public MControlObject {
#define MSLIDER_TIMER		17789
public:
	CString m_text;
	float m_pos = 0.1F;
	bool m_vertical = true;	//이 값이 true이면 수직 슬라이더 입니다.
private:
	bool m_click = false;
	CRect m_thumb;
public:
	MSlider(CWnd* parent, MRect base) : MControlObject(parent, base) {

	}
	INT OnPaint(CDC* pDC) {
		CPoint point = this->GetMousePoint();
		CRect view_rect = this->GetViewRect();
		CRect rect = m_rect.GetRect(view_rect);
		
		if (m_vertical == false) {
			auto DrawHSlider = [](CDC* pDC, CRect rect,COLORREF color)->void {
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
			m_pos = static_cast<float>(point.x - (rect.left+rect.Height()/2)) / (rect.Width() - rect.Height());
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