/*
*  mlistbox.h
*  MSpring
*
*  Created by KimBomm on 2018. 4. 7...
*  Copyright 2018 KimBomm. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E2_4_7_MLISTBOX_HPP_INCLUDED)
#define MSPRING_7E2_4_7_MLISTBOX_HPP_INCLUDED
#include"MControlObject.h"
class MListBox_core : public MControlObject {
public:
	std::vector<std::pair<TString, bool>> m_data;
	int m_padding = 5;				//체크박스와 텍스트의 패딩 입니다.
	int m_num_padding = 0;			//숫자영역의 넓이 입니다.
	int m_checkbox_padding = 0;		//체크박스의 넓이 입니다.

	int m_scrollbar_weight = 20;	//스크롤바의 두께 입니다.
	int HEIGHT = 20;				//각 리스트의 높이 입니다.
	bool is_check = false;			//체크 박스를 넣을지 말지 결정 합니다.
	bool is_numbering = false;		//순서를 보여줄지 말지 결정 합니다.
	int m_select = -1;				//선택된 리스트의 인덱스 입니다.
	float m_v_scroll_pos = 0.0F;		//(내부적사용)스크롤의 위치를 나타냅니다.(0.0~1.0)
	float m_h_scroll_pos = 0.0F;
public:///Utility Functions
	   //좌표로 클릭한 지점의 인덱스를 가져옵니다.
	int GetElementByPoint(CPoint pt) {
		CRect rect = this->m_rect.GetRect(GetViewRect());
		if (rect.PtInRect(pt) == FALSE) {
			return -1;
		}
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		int page_jump = 0;	//띄워야 하는 좌표
		if (page_height > view_height) {
			page_jump = static_cast<int>((page_height - view_height)*m_v_scroll_pos);
		}
		int page_y = 0;		//페이지가 시작될 좌표.
		int page_idx = 0;	//시작될 줄 넘버
		if (page_jump > 0) {
			div_t div = std::div(page_jump, HEIGHT);
			page_y = -div.rem;
			page_idx = div.quot;
		}
		CDC* pDC = this->m_parent->GetDC();
		int h = mspring::Font::GetRealFontHeight(m_font_str, HEIGHT, pDC);

		CFont font; font.CreatePointFont(h, m_font_str.data());
		CFont* old_font = pDC->SelectObject(&font);
		int ret = -1;
		while (page_idx < (int)m_data.size() && page_y < view_height) {
			//pDC->TextOutW(rect.left+m_padding, rect.top+page_y, m_data[page_idx]);
			if (rect.top + page_y <= pt.y && pt.y < rect.top + page_y + HEIGHT) {
				CSize sz;
				GetTextExtentPoint32(pDC->GetSafeHdc(), m_data[page_idx].first.data(), m_data[page_idx].first.length(), &sz);
				if (rect.left + m_padding + m_checkbox_padding + m_num_padding + sz.cx > pt.x) {	//가로 셀에 있는지 검사
					if (page_height <= view_height || pt.x < rect.right - m_scrollbar_weight) {		//V스크롤이 있으면 그 부분은 제외
						if (m_max_length < rect.Width() || pt.y < rect.bottom - m_scrollbar_weight) {	//H스크롤이 있으면 그 부분은 제외
							ret = page_idx;
						}
					}
				}
				break;
			}
			page_idx++;
			page_y += HEIGHT;
		}
		font.DeleteObject();
		this->m_parent->ReleaseDC(pDC);
		return ret;
	}
protected:
	enum DragStatus {
		NO_DRAG,
		VSCROLL_DRAG,
		HSCROLL_DRAG
	}m_is_drag;	//(내부적사용)현재 드래그 상태인지 나타냅니다.
	CPoint m_drag_point;			//(내부적사용)드래그의 시작점을 나타냅니다.
	CRect m_v_thumb_rect;			//(내부적사용)스크롤 엄지의 사각좌표 입니다.
	CRect m_h_thumb_rect;
	float m_prev_scroll_pos;		//(내부적사용)스크롤의 예전 위치 입니다.
	int m_max_length = -1;				//제일 긴 문자열의 길이입니다.
protected:
	//리스트박스의 높이를 가져 옵니다.
	int GetViewHeight() {
		CRect rect = this->m_rect.GetRect(GetViewRect());
		int r = this->m_rect.GetRect(this->GetViewRect()).Height();
		/*if (m_max_length >= rect.Width()) {
		r -= HEIGHT;
		r = mspring::Max(r, 0);
		}*/
		return r;
	}
	int GetViewWidth() {
		CRect rect = this->m_rect.GetRect(GetViewRect());
		int r = rect.Width();
		if (GetPageHeight() > GetViewHeight()) {
			r -= m_scrollbar_weight;
		}
		return r;
		//return rect.Width() - (m_padding * 2 + m_num_padding + m_checkbox_padding);
	}
	//실제 내용의 높이를 가져 옵니다.
	int GetPageHeight() {
		return static_cast<int>(m_data.size()*HEIGHT);
	}
	int GetPageWidth() {
		return m_max_length - (m_padding * 2 + m_num_padding + m_checkbox_padding);
	}
	//스크롤 엄지의 높이를 가져 옵니다.
	int GetVThumbHeight() {
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		return static_cast<int>(((float)view_height / page_height)*view_height);
	}
	int GetHThumbWidth() {
		int view_width = GetViewWidth();
		int page_width = GetPageWidth();
		return static_cast<int>(((float)view_width / page_width)*view_width);
	}
	//페이지의 인덱스를 가져 옵니다.
	int GetPageIndex() {
		int view_height = GetViewHeight();
		int thumb_height = GetVThumbHeight();
		int page_height = GetPageHeight();
		int page_jump = 0;	//띄워야 하는 좌표
		if (page_height > view_height) {
			page_jump = static_cast<int>((page_height - view_height)*m_v_scroll_pos);
		}
		int page_idx = 0;	//시작될 줄 넘버
		if (page_jump > 0) {
			page_idx = static_cast<int>(std::round((float)page_jump / HEIGHT));
		}
		return page_idx;
	}
	void SetPageIndex(int page_idx) {
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		m_v_scroll_pos = ((float)page_idx*HEIGHT) / (page_height - view_height);
		mspring::SetRange(m_v_scroll_pos, 0.0F, 1.0F);
	}
	void DrawScroll(CDC* pDC) {
		CRect rect = this->m_rect.GetRect(GetViewRect());
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		int thumb_height = GetVThumbHeight();
		thumb_height = mspring::Max(thumb_height, 20);
		CPen pen; pen.CreatePen(PS_NULL, 1, RGB(0, 0, 0));
		CBrush brush; brush.CreateSolidBrush(*m_color_fr);
		CPen* old_pen = pDC->SelectObject(&pen);
		CBrush* old_brush = pDC->SelectObject(&brush);
		CBrush brush_bk; brush_bk.CreateSolidBrush(*m_color_bk);
		if (page_height > view_height) {
			CRect thumb_rect = rect;
			thumb_rect.top += static_cast<decltype(thumb_rect.top)>((view_height - thumb_height)*m_v_scroll_pos);
			thumb_rect.left = thumb_rect.right - m_scrollbar_weight;
			thumb_rect.bottom = thumb_rect.top + thumb_height;

			CRect area = rect;
			area.left = thumb_rect.left;
			pDC->FillRect(&area, &brush_bk);
			pDC->RoundRect(&thumb_rect, CPoint(5, 5));
			m_v_thumb_rect = thumb_rect;
		}
		if (m_max_length >= rect.Width()) {
			CRect thumb_rect = rect;
			int actual_width = m_max_length - (m_padding * 2 + m_num_padding + m_checkbox_padding);

			int view_width = GetViewWidth();
			int page_width = GetPageWidth();
			int thumb_width = GetHThumbWidth();
			thumb_width = mspring::Max(thumb_width, 20);

			thumb_rect.left += static_cast<decltype(thumb_rect.left)>((view_width - thumb_width)*m_h_scroll_pos);
			thumb_rect.top = thumb_rect.bottom - m_scrollbar_weight;
			thumb_rect.right = thumb_rect.left + thumb_width;

			CRect area = rect;
			area.top = thumb_rect.top;
			area.right = rect.right - m_scrollbar_weight;
			pDC->FillRect(&area, &brush_bk);
			pDC->RoundRect(&thumb_rect, CPoint(5, 5));
			m_h_thumb_rect = thumb_rect;
		}
		brush_bk.DeleteObject();
		pDC->SelectObject(old_pen);
		pDC->SelectObject(old_brush);
		brush.DeleteObject();
		pen.DeleteObject();
	}
	void DrawCheckBox(CDC* pDC, int x, int y, bool check) {
		CPen pen_check;
		CBrush brush_check;
		CPen pen_null;
		pen_null.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		pen_check.CreatePen(PS_SOLID, HEIGHT / 10, *m_color_text);
		if (check == true) {
			brush_check.CreateSolidBrush(*m_color_fr);
		} else {
			brush_check.CreateSolidBrush(this->GetDarkColor(this->GetDarkColor(*m_color_bk)));
		}
		CBrush* old_brush = pDC->SelectObject(&brush_check);
		CPen* old_pen = pDC->SelectObject(&pen_null);
		CRect rect = CRect(x, y, x + HEIGHT, y + HEIGHT);
		pDC->RoundRect(&rect, CPoint(5, 5));
		pDC->SelectObject(old_pen);
		old_pen = pDC->SelectObject(&pen_check);

		int divw = HEIGHT / 5;
		pDC->MoveTo(rect.left + divw, static_cast<int>(rect.top + divw*2.5));
		pDC->LineTo(static_cast<int>(rect.left + divw*1.5), rect.top + divw * 4);
		pDC->LineTo(rect.right - divw, rect.top + divw * 1);
		pDC->SelectObject(old_pen);
		pDC->SelectObject(old_brush);


		brush_check.DeleteObject();
		pen_check.DeleteObject();
		pen_null.DeleteObject();
	}

public:///Message Function
	MListBox_core(CWnd* parent, MRect base) : MControlObject(parent, base) {

	}

	INT OnPaint(CDC* pDC) override {
		CRect rect = this->m_rect.GetRect(GetViewRect());
		CRgn rgn; rgn.CreateRectRgnIndirect(&rect);
		pDC->SelectClipRgn(&rgn);

		CBrush brush_bk; brush_bk.CreateSolidBrush(*m_color_bk);
		pDC->FillRect(&rect, &brush_bk);


		int h = mspring::Font::GetRealFontHeight(m_font_str, HEIGHT, pDC);
		CFont font; font.CreatePointFont(h, m_font_str.data());
		CFont* old_font = pDC->SelectObject(&font);
		int view_height = GetViewHeight();
		int view_width = GetViewWidth();
		int page_height = GetPageHeight();
		int page_width = GetPageWidth();
		int page_jump = 0;	//띄워야 하는 좌표
		if (page_height > view_height) {
			page_jump = static_cast<int>((page_height - view_height)*m_v_scroll_pos);
		}
		int page_y = 0;		//페이지가 시작될 좌표.
		int page_idx = 0;	//시작될 줄 넘버
		if (page_jump > 0) {
			div_t div = std::div(page_jump, HEIGHT);
			page_y = -div.rem;
			page_idx = div.quot;
		}
		CPen pen_null; pen_null.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		CPen* pen_old = pDC->SelectObject(&pen_null);
		pDC->SetTextColor(*m_color_text);

		CBrush brush_highlight; brush_highlight.CreateSolidBrush(*m_color_fr);
		CSize sz; GetTextExtentPoint32(pDC->GetSafeHdc(), TEXT("A"), 1, &sz);


		int actual_width = m_max_length - (m_padding * 2 + m_num_padding + m_checkbox_padding);
		int h_minus = static_cast<int>((actual_width - (view_width - (m_padding * 2 + m_num_padding + m_checkbox_padding))) * m_h_scroll_pos);
		if (m_max_length < rect.Width()) {
			h_minus = 0;
			m_h_scroll_pos = 0.0F;
		}
		if (is_check == true) {
			m_checkbox_padding = HEIGHT + m_padding;
		} else {
			m_checkbox_padding = 0;
		}
		if (is_numbering == true) {
			m_num_padding = sz.cx*static_cast<int>(round(log10(m_data.size())));
		} else {
			m_num_padding = 0;
		}
		while (page_idx < (int)m_data.size() && page_y < view_height) {
			pDC->SetBkMode(TRANSPARENT);
			GetTextExtentPoint32(pDC->GetSafeHdc(), m_data[page_idx].first.data(), m_data[page_idx].first.length(), &sz);
			int width = m_padding + m_num_padding + sz.cx;


			if (page_idx == m_select) {
				CBrush* brush_old = pDC->SelectObject(&brush_highlight);
				pDC->RoundRect(rect.left + m_checkbox_padding - h_minus,
							   rect.top + page_y,
							   rect.left + m_checkbox_padding + width + m_padding - h_minus,
							   rect.top + page_y + HEIGHT, 5, 5);
				pDC->SelectObject(&brush_old);
			}
			pDC->TextOut(rect.left + m_padding + m_checkbox_padding + m_num_padding - h_minus, rect.top + page_y, m_data[page_idx].first.data());

			CRect area = rect;
			area.top += page_y;
			area.bottom = area.top + HEIGHT;
			area.right = rect.left + m_padding + m_checkbox_padding + m_num_padding;
			if (m_h_scroll_pos > 0.0F) {
				COLORREF dark = this->GetDarkColor(*m_color_bk);
				CBrush brush_bk_dark; brush_bk_dark.CreateSolidBrush(dark);
				pDC->FillRect(&area, &brush_bk_dark);
				brush_bk_dark.DeleteObject();
			} else {
				pDC->FillRect(&area, &brush_bk);
			}
			if (is_check == true) {
				DrawCheckBox(pDC, rect.left + m_padding, rect.top + page_y, m_data[page_idx].second);
			}
			if (is_numbering == true) {
				OStringStream oss;
				oss << page_idx;
				pDC->TextOut(rect.left + m_padding + m_checkbox_padding, rect.top + page_y, oss.str().data());
			}


			page_idx++;
			page_y += HEIGHT;
		}
		m_max_length = -1;
		int max_len_idx = -1;
		int max_len = -1;
		for (size_t i = 0; i<m_data.size(); i++) {
			if (static_cast<int>(m_data[i].first.length()) > max_len) {
				max_len = static_cast<int>(m_data[i].first.length());
				max_len_idx = i;
			}
		}
		if (max_len_idx != -1) {
			GetTextExtentPoint32(pDC->GetSafeHdc(), m_data[max_len_idx].first.data(), m_data[max_len_idx].first.length(), &sz);
			m_max_length = mspring::Max(m_max_length, static_cast<int>(m_checkbox_padding + m_num_padding + m_padding * 2 + sz.cx));
		}

		DrawScroll(pDC);
		pDC->SelectObject(&pen_old);
		pen_null.DeleteObject();
		font.DeleteObject();
		brush_bk.DeleteObject();
		pDC->SelectClipRgn(nullptr);
		return 1;
	}
	INT OnLButtonDown() override {
		MControlObject::OnLButtonDown();
		if (s_curr_id != m_id) return 0;
		CPoint point = this->GetMousePoint();
		CRect rect;
		m_parent->GetClientRect(&rect);
		if (m_v_thumb_rect.PtInRect(point) == TRUE) {
			m_is_drag = DragStatus::VSCROLL_DRAG;
			m_prev_scroll_pos = m_v_scroll_pos;
			m_drag_point = point;
		} else if (m_h_thumb_rect.PtInRect(point) == TRUE) {
			m_is_drag = DragStatus::HSCROLL_DRAG;
			m_prev_scroll_pos = m_h_scroll_pos;
			m_drag_point = point;
		} else {
			m_select = this->GetElementByPoint(point);
		}
		return 1;
	}
	INT OnRButtonDown() override {
		MControlObject::OnLButtonDown();
		if (s_curr_id != m_id) return 0;
		CPoint point = this->GetMousePoint();
		CRect rect;
		m_parent->GetClientRect(&rect);
		if (m_v_thumb_rect.PtInRect(point) == FALSE && m_h_thumb_rect.PtInRect(point) == FALSE) {
			m_select = this->GetElementByPoint(point);
		}
		return 1;
	}
	INT OnLButtonUp()override {
		if (m_is_drag != DragStatus::NO_DRAG) {
			m_parent->KillTimer(TIMER_SCROLLMOVE + m_id);
			m_is_drag = DragStatus::NO_DRAG;
		}
		return 1;
	}
	INT OnMouseWheel(short zDelta) override {
		CPoint point = this->GetMousePoint();
		CRect rect = this->m_rect.GetRect(this->GetViewRect());
		if (rect.PtInRect(point) == FALSE) {
			return 0;
		}
		if ((GetKeyState(VK_CONTROL) & 0x8000) == 0x8000) {
			//Ctrl키를 누르고 휠을 돌리면 확대 옵션.
			HEIGHT += (zDelta > 0) ? 1 : -1;
			mspring::SetRange(HEIGHT, 10, 50);
		} else if ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000) {
			m_v_scroll_pos += 0.01F*((zDelta > 0) ? -1 : 1);
			mspring::SetRange(m_v_scroll_pos, 0.0F, 1.0F);
		} else {
			int page_idx = GetPageIndex();
			page_idx += ((zDelta > 0) ? -1 : 1);
			SetPageIndex(page_idx);
		}
		return 1;
	}
	INT OnMouseMove()override {
		CPoint point = this->GetMousePoint();
		if (m_is_drag == DragStatus::VSCROLL_DRAG) {
			int view_height = GetViewHeight();
			int page_height = GetPageHeight();
			int thumb_height = GetVThumbHeight();
			int drag_height = point.y - m_drag_point.y;
			m_v_scroll_pos = m_prev_scroll_pos + ((float)drag_height / (view_height - thumb_height));
			mspring::SetRange(m_v_scroll_pos, 0.0F, 1.0F);
		} else if (m_is_drag == DragStatus::HSCROLL_DRAG) {
			int view_width = GetViewWidth();
			int page_width = GetPageWidth();
			int thumb_width = GetHThumbWidth();
			int drag_width = point.x - m_drag_point.x;
			m_h_scroll_pos = m_prev_scroll_pos + ((float)drag_width / (view_width - thumb_width));
			mspring::SetRange(m_h_scroll_pos, 0.0F, 1.0F);
		}
		return 1;
	}
	INT OnMouseLeave()override {
		if (m_is_drag != DragStatus::NO_DRAG) {
			m_parent->SetTimer(TIMER_SCROLLMOVE + m_id, 10, nullptr);
		}
		return 1;
	}
	INT OnKeyDown(UINT nChar) override {
		if (isFocused() == false)return 0;
		int page_idx = GetPageIndex();
		switch (nChar) {
			case VK_HOME:this->m_v_scroll_pos = 0.0F; break;
			case VK_END:this->m_v_scroll_pos = 1.0F; break;
			case VK_PRIOR:SetPageIndex(page_idx - GetViewHeight() / HEIGHT); break;
			case VK_NEXT:SetPageIndex(page_idx + GetViewHeight() / HEIGHT); break;
			case VK_UP:if (m_select >0) {
				if (m_select < page_idx || m_select >= page_idx + (GetViewHeight() / HEIGHT)) {
					m_select--;
					SetPageIndex(m_select);

				} else {
					m_select--;
					if (m_select < page_idx) {
						SetPageIndex(page_idx - 1);
					}
				}

			}break;
			case VK_DOWN:if (m_select != -1 && m_select<(int)m_data.size() - 1) {
				if (m_select < page_idx || m_select >= page_idx + (GetViewHeight() / HEIGHT)) {
					m_select++;
					SetPageIndex(m_select);

				} else {
					m_select++;
					if (m_select >= page_idx + (GetViewHeight() / HEIGHT)) {
						SetPageIndex(page_idx + 1);
					}
				}

			}break;
			case VK_LEFT:m_h_scroll_pos = 0.0F; break;
			case VK_RIGHT:m_h_scroll_pos = 1.0F; break;
		}
		return 1;
	}
	const UINT_PTR TIMER_SCROLLMOVE = (UINT_PTR)"mspring.control.listbox.h(TIMER_SCROLLMOVE)";
	INT OnTimer(UINT_PTR nIDEvent)override {
		if (nIDEvent == TIMER_SCROLLMOVE + m_id) {
			CPoint point = this->GetMousePoint();
			CRect rect = this->GetViewRect();
			//두 조건은 합쳐질수없음
			if (rect.PtInRect(point) == TRUE) {
				m_parent->KillTimer(TIMER_SCROLLMOVE + m_id);
				return 0;
			}
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) == false) {
				m_is_drag = DragStatus::NO_DRAG;
				m_parent->KillTimer(TIMER_SCROLLMOVE + m_id);
				return 0;
			}
			if (m_is_drag == DragStatus::VSCROLL_DRAG) {
				int view_height = GetViewHeight();
				int page_height = GetPageHeight();
				int thumb_height = GetVThumbHeight();
				int drag_height = point.y - m_drag_point.y;
				m_v_scroll_pos = m_prev_scroll_pos + ((float)drag_height / (view_height - thumb_height));
				mspring::SetRange(m_v_scroll_pos, 0.0F, 1.0F);
			} else {
				int view_width = GetViewWidth();
				int page_width = GetPageWidth();
				int thumb_width = GetHThumbWidth();
				int drag_width = point.x - m_drag_point.x;
				m_h_scroll_pos = m_prev_scroll_pos + ((float)drag_width / (view_width - thumb_width));
				mspring::SetRange(m_h_scroll_pos, 0.0F, 1.0F);
			}
		}
		return 1;
	}
};
using MListBox = std::shared_ptr<MListBox_core>;
#endif  //MSPRING_7E2_4_7_MLISTBOX_HPP_INCLUDED