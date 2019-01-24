
// ChildView.h : MSpringView 클래스의 인터페이스
//


#pragma once

#include<iostream>
#include<vector>
#include<set>
#include"../utils/utils.h"
#include"VirtualView.h"
#define SAFETY_CALL(VIEW,METHOD,...)	if(VIEW){(VIEW)->METHOD(__VA_ARGS__);}

class MSpringView : public CWnd {
public:
	MSpringView() {
		m_view = nullptr;
	}
	virtual ~MSpringView() = default;
	VirtualView* m_view = nullptr;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) {
		if (!CWnd::PreCreateWindow(cs))
			return FALSE; 
		cs.dwExStyle |= WS_EX_CLIENTEDGE;
		cs.style &= ~WS_BORDER;
		cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
										   ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);
		return TRUE;
	}
	virtual void OnUpdate(CView* view, LPARAM lParam, CObject* co) {
		this->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
		
	}
	COLORREF m_color_bk = RGB(255, 255, 255);
public:
	void SetStyle(COLORREF bk) {
		m_color_bk = bk;
	}
protected:
	afx_msg void OnPaint() {
		CPaintDC dc(this);
		CRect rect;
		this->GetClientRect(&rect);
		mspring::DoubleBufferingDC dbb(&dc, rect);

		CBrush bk_brush;
		bk_brush.CreateSolidBrush(m_color_bk);
		dbb.getDC().FillRect(rect, &bk_brush);
		bk_brush.DeleteObject();

		//auto DrawBitmap = [](CDC* pdc,CRect rect)->void {
		//	CDC memdc;
		//	BITMAP bmpinfo;
		//	memdc.CreateCompatibleDC(pdc);
		//	CBitmap bmp, *oldbmp;
		//	HANDLE hbitmap = ::LoadImage(0, TEXT("blur.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//	//bmp.LoadBitmapW(TEXT("blur.bmp"));
		//	bmp.Attach((HBITMAP)hbitmap);
		//	bmp.GetBitmap(&bmpinfo);
		//	oldbmp = memdc.SelectObject(&bmp);
		//	pdc->SetStretchBltMode(HALFTONE);
		//	pdc->StretchBlt(0, 0, rect.Width(),rect.Height(), &memdc, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
		//	memdc.SelectObject(oldbmp);
		//};
		//DrawBitmap(dbb.getPDC(),rect);

		SAFETY_CALL(m_view, OnPaint, dbb.getPDC());
		dbb.Draw();
	}
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcPaint() {
		//사각영역만 칠해야함.
		CWindowDC ncpaint(this);
		CRect rect_window;
		this->GetWindowRect(&rect_window);
		rect_window.NormalizeRect();
		rect_window.OffsetRect(-rect_window.left, -rect_window.top);
		CRect rect_top = rect_window;
		rect_top.bottom = rect_top.top + 3;
		CRect rect_bottom = rect_window;
		rect_bottom.top = rect_bottom.bottom - 3;
		CRect rect_left = rect_window;
		rect_left.right = rect_left.left + 3;
		CRect rect_right = rect_window;
		rect_right.left = rect_right.right - 3;
		//Fixed value
		CBrush brush;
		brush.CreateSolidBrush(m_color_bk);
		ncpaint.FillRect(rect_top, &brush);
		ncpaint.FillRect(rect_bottom, &brush);
		ncpaint.FillRect(rect_left, &brush);
		ncpaint.FillRect(rect_right, &brush);
		brush.DeleteObject();
		ReleaseDC(&ncpaint);
	}
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct) {
		
		if (CWnd::OnCreate(lpCreateStruct) == -1) {
			return -1;
		}
		
		SAFETY_CALL(m_view, OnCreate);
		
		return 0;
	}
	afx_msg void OnSetFocus(CWnd* pOldWnd) {
		CWnd::OnSetFocus(pOldWnd);
		SAFETY_CALL(m_view, OnSetFocus, pOldWnd);
	}
	afx_msg void OnKillFocus(CWnd* pNewWnd) {
		CWnd::OnKillFocus(pNewWnd);
		SAFETY_CALL(m_view, OnKillFocus, pNewWnd);
	}
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point) {
		SAFETY_CALL(m_view, OnLButtonDown, nFlags, point);
		CWnd::OnLButtonDown(nFlags, point);
		this->Invalidate();
	}
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point) {
		SAFETY_CALL(m_view, OnLButtonUp, nFlags, point);
		CWnd::OnLButtonUp(nFlags, point);
		this->Invalidate();
	}
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point) {
		SAFETY_CALL(m_view, OnLButtonDblClk, nFlags, point);
		CWnd::OnLButtonDblClk(nFlags, point);
	}
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
		SAFETY_CALL(m_view, OnSetCursor, pWnd, nHitTest, message);
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}
	afx_msg void OnSize(UINT nType, int cx, int cy) {
		CWnd::OnSize(nType, cx, cy);
		this->OnPaint();
		SAFETY_CALL(m_view, OnSize, nType, cx, cy);
	}
	afx_msg void OnMouseMove(UINT nFlags, CPoint point) {
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 0;
		TrackMouseEvent(&tme);
		SAFETY_CALL(m_view, OnMouseMove, nFlags, point);
		CWnd::OnMouseMove(nFlags, point);
		this->Invalidate();
	}
	afx_msg void OnMouseLeave() {
		SAFETY_CALL(m_view, OnMouseLeave);
		CWnd::OnMouseLeave();
		this->Invalidate();
	}
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
		SAFETY_CALL(m_view, OnMouseWheel, nFlags, zDelta, pt);
		this->Invalidate();
		return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	}
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
		SAFETY_CALL(m_view, OnKeyDown, nChar, nRepCnt, nFlags);
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		this->Invalidate();
	}
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
		SAFETY_CALL(m_view, OnKeyUp, nChar, nRepCnt, nFlags);
		CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
		this->Invalidate();
	}
	afx_msg void OnDestroy() {
		CWnd::OnDestroy();
		//SAFETY_CALL(m_view, OnDestroy);
		
	}
	afx_msg void OnTimer(UINT_PTR nIDEvent) {
		SAFETY_CALL(m_view, OnTimer, nIDEvent);
		CWnd::OnTimer(nIDEvent);
		this->Invalidate();
	}
	afx_msg BOOL OnNcActivate(BOOL bActive) {
		this->OnNcPaint();
		return TRUE;
	}
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point) {
		SAFETY_CALL(m_view, OnRButtonDown, nFlags, point);
		CWnd::OnRButtonDown(nFlags, point);
		this->Invalidate();
	}
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point) {
		SAFETY_CALL(m_view, OnRButtonUp, nFlags, point);
		CWnd::OnRButtonUp(nFlags, point);
		this->Invalidate();
	}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) {
		return FALSE;
	}
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
		SAFETY_CALL(m_view, OnChar, nChar, nRepCnt,nFlags);
		CWnd::OnChar(nChar, nRepCnt, nFlags);
		this->Invalidate();
	}
	afx_msg  LRESULT OnComposition(WPARAM wParam, LPARAM lParam) {
		SAFETY_CALL(m_view, OnComposition, wParam, lParam);
		this->Invalidate();
		return 1;
	}
};

MSPRING_BEGIN_MESSAGE_MAP(MSpringView, CWnd)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_NCACTIVATE()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_CHAR()
	ON_MESSAGE(WM_IME_COMPOSITION, MSpringView::OnComposition)
MSPRING_END_MESSAGE_MAP()