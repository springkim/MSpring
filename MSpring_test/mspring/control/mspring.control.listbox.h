/**
* @file		mspring.control.listbox.h
* @author		kimbomm (springnode@gmail.com)
* @date		2017. 11. 17...
* @version	1.0.0
*
*  @brief
*			MSpring ListBox
*	@remark
*			Created by kimbom on 2017. 11. 17...
*			Copyright 2017 kimbom.All rights reserved.
*/
#pragma once
#include<vector>
#include"mspring.control.object.h"

class MListBox : public MControlObject {
public:
	std::vector<std::pair<CString,bool>> m_data;
	UINT m_padding=5;				//체크박스와 텍스트의 패딩 입니다.
	int m_scrollbar_width = 20;	//스크롤바의 두께 입니다.
	int HEIGHT = 20;				//각 리스트의 높이 입니다.
	bool is_check = false;			//체크 박스를 넣을지 말지 결정 합니다.
	int m_select = -1;				//선택된 리스트의 인덱스 입니다.
	float m_scroll_pos = 1.0F;		//(내부적사용)스크롤의 위치를 나타냅니다.(0.0~1.0)
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
			page_jump = static_cast<int>((page_height - view_height)*m_scroll_pos);
		}
		int page_y = 0;		//페이지가 시작될 좌표.
		int page_idx = 0;	//시작될 줄 넘버
		if (page_jump > 0) {
			div_t div = std::div(page_jump, HEIGHT);
			page_y = -div.rem;
			page_idx = div.quot;
		}
		while (page_idx < (int)m_data.size() && page_y < view_height) {
			//pDC->TextOutW(rect.left+m_padding, rect.top+page_y, m_data[page_idx]);
			if (rect.top + page_y <= pt.y && pt.y < rect.top + page_y + HEIGHT) {
				return page_idx;
			}
			page_idx++;
			page_y += HEIGHT;
		}
		return -1;
	}
protected:
	bool m_is_drag = false;			//(내부적사용)현재 드래그 상태인지 나타냅니다.
	CPoint m_drag_point;			//(내부적사용)드래그의 시작점을 나타냅니다.
	CRect m_thumb_rect;			//(내부적사용)스크롤 엄지의 사각좌표 입니다.
	float m_prev_scroll_pos;		//(내부적사용)스크롤의 예전 위치 입니다.
	

	
protected:
	//리스트박스의 높이를 가져 옵니다.
	int GetViewHeight() {
		return this->m_rect.GetRect(this->GetViewRect()).Height();
	}
	//실제 내용의 높이를 가져 옵니다.
	int GetPageHeight() {
		return static_cast<int>(m_data.size()*HEIGHT);
	}
	//스크롤 엄지의 높이를 가져 옵니다.
	int GetThumbHeight() {
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		return static_cast<int>(((float)view_height / page_height)*view_height);
	}
	void DrawScroll(CDC* pDC) {
		CRect rect = this->m_rect.GetRect(GetViewRect());
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		int thumb_height = GetThumbHeight();
		if (page_height > view_height) {
			CRect thumb_rect = rect;
			thumb_rect.top += static_cast<decltype(thumb_rect.top)>((view_height - thumb_height)*m_scroll_pos);
			thumb_rect.left = thumb_rect.right - m_scrollbar_width;
			thumb_rect.bottom = thumb_rect.top + thumb_height;
			CPen pen; pen.CreatePen(PS_NULL, 1, RGB(0, 0, 0));
			CBrush brush; brush.CreateSolidBrush(*m_color_fr);
			CPen* old_pen = pDC->SelectObject(&pen);
			CBrush* old_brush = pDC->SelectObject(&brush);
			pDC->RoundRect(&thumb_rect, CPoint(5, 5));
			pDC->SelectObject(old_pen);
			pDC->SelectObject(old_brush);
			brush.DeleteObject();
			pen.DeleteObject();
			m_thumb_rect = thumb_rect;
		}
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
			brush_check.CreateSolidBrush(this->GetDarkColor(*m_color_bk));
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
	MListBox(CWnd* parent, MRect base) : MControlObject(parent, base) {

	}
	INT OnPaint(CDC* pDC) override{
		CRect rect = this->m_rect.GetRect(GetViewRect());
		CRgn rgn;rgn.CreateRectRgnIndirect(&rect);
		pDC->SelectClipRgn(&rgn);
		{
			CBrush brush_bk; brush_bk.CreateSolidBrush(*m_color_bk);
			pDC->FillRect(&rect, &brush_bk);
			brush_bk.DeleteObject();
		}
		int h = mspring::Font::GetRealFontHeight(m_font_str, HEIGHT, pDC);
		CFont font; font.CreatePointFont(h, m_font_str);
		CFont* old_font = pDC->SelectObject(&font);
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		int page_jump = 0;	//띄워야 하는 좌표
		if (page_height > view_height) {
			page_jump = static_cast<int>((page_height - view_height)*m_scroll_pos);
		}
		int page_y = 0;		//페이지가 시작될 좌표.
		int page_idx = 0;	//시작될 줄 넘버
		if (page_jump > 0) {
			div_t div = std::div(page_jump, HEIGHT);
			page_y = -div.rem;
			page_idx = div.quot;
		}
		CPen pen_null;pen_null.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		while (page_idx < (int)m_data.size() && page_y < view_height) {
			int margin = 0;
			if (is_check == true) {
				margin = HEIGHT + m_padding;
				DrawCheckBox(pDC, rect.left + m_padding, rect.top + page_y, m_data[page_idx].second);
			}
			if (page_idx == m_select) {
				pDC->SetBkMode(OPAQUE);
				pDC->SetBkColor(this->GetBrightColor(*m_color_fr));
			} else {
				pDC->SetBkMode(TRANSPARENT);
			}
			CString str;
			CString format;
			format.Format(TEXT("%%%dd    %%s"), std::to_string(m_data.size()).size());
			str.Format(format, page_idx,m_data[page_idx].first);


			pDC->TextOut(rect.left+m_padding+margin, rect.top+page_y, str);
			page_idx++;
			page_y += HEIGHT;
		}
		DrawScroll(pDC);
		pen_null.DeleteObject();
		font.DeleteObject();
		pDC->SelectClipRgn(nullptr);
		return 1;
	}
	INT OnLButtonDown() override{
		MControlObject::OnLButtonDown();
		if (s_curr_id != m_id) return 0;
		CPoint point = this->GetMousePoint();
		CRect rect;
		m_parent->GetClientRect(&rect);
		if (m_thumb_rect.PtInRect(point) == TRUE) {
			m_is_drag = true;
			m_prev_scroll_pos = m_scroll_pos;
			m_drag_point = point;
		} else {
			m_select = this->GetElementByPoint(point);
		}
		return 1;
	}
	INT OnRButtonDown() override{
		MControlObject::OnLButtonDown();
		if (s_curr_id != m_id) return 0;
		CPoint point = this->GetMousePoint();
		CRect rect;
		m_parent->GetClientRect(&rect);
		if (m_thumb_rect.PtInRect(point) == FALSE) {
			m_select = this->GetElementByPoint(point);
		}
		return 1;
	}
	INT OnLButtonUp()override {
		if (m_is_drag == true) {
			m_parent->KillTimer(TIMER_SCROLLMOVE+m_id);
			m_is_drag = false;
		}
		return 1;
	}
	INT OnMouseWheel(short zDelta) override{
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
			m_scroll_pos += 0.01F*((zDelta > 0) ? -1 : 1);
			mspring::SetRange(m_scroll_pos, 0.0F, 1.0F);
		} else {
			int view_height = GetViewHeight();
			int thumb_height = GetThumbHeight();
			int page_height = GetPageHeight();
			int page_jump = 0;	//띄워야 하는 좌표
			if (page_height > view_height) {
				page_jump = static_cast<int>((page_height - view_height)*m_scroll_pos);
			}
			int page_idx = 0;	//시작될 줄 넘버
			if (page_jump > 0) {
				page_idx = static_cast<int>(std::round((float)page_jump / HEIGHT));
			}
			page_idx += ((zDelta > 0) ? -1 : 1);
			m_scroll_pos = ((float)page_idx*HEIGHT) / (page_height - view_height);
			mspring::SetRange(m_scroll_pos, 0.0F, 1.0F);
		}
		return 1;
	}
	INT OnMouseMove()override {
		if (m_is_drag == true) {
			CPoint point = this->GetMousePoint();
			int view_height = GetViewHeight();
			int page_height = GetPageHeight();
			int thumb_height = GetThumbHeight();
			int drag_height = point.y - m_drag_point.y;
			m_scroll_pos = m_prev_scroll_pos + ((float)drag_height / (view_height - thumb_height));
			mspring::SetRange(m_scroll_pos, 0.0F, 1.0F);
		}
		return 1;
	}
	INT OnMouseLeave()override {
		if (m_is_drag == true) {
			m_parent->SetTimer(TIMER_SCROLLMOVE+m_id, 10, nullptr);
		}
		return 1;
	}
	const UINT_PTR TIMER_SCROLLMOVE = 0x65C187C6;
	INT OnTimer(UINT_PTR nIDEvent)override {
		if (nIDEvent == TIMER_SCROLLMOVE+m_id) {
			CPoint point = this->GetMousePoint();
			CRect rect = this->GetViewRect();
			//두 조건은 합쳐질수없음
			if (rect.PtInRect(point) == TRUE) {
				m_parent->KillTimer(TIMER_SCROLLMOVE + m_id);
				return 0;
			}
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) == false) {
				m_is_drag = false;
				m_parent->KillTimer(TIMER_SCROLLMOVE + m_id);
				return 0;
			}
			int view_height = GetViewHeight();
			int page_height = GetPageHeight();
			int thumb_height = GetThumbHeight();
			int drag_height = point.y - m_drag_point.y;
			m_scroll_pos = m_prev_scroll_pos + ((float)drag_height / (view_height - thumb_height));
			m_scroll_pos = mspring::Max(m_scroll_pos, 0.0F);
			m_scroll_pos = mspring::Min(m_scroll_pos, 1.0F);
		}
		return 1;
	}
};