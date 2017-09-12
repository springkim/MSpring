#pragma once
#include<vector>
#include<string>
class VirtualView {
public:
	CPoint GetMousePoint() {
		CPoint point;
		CRect rect;
		m_parent->GetWindowRect(&rect);
		::GetCursorPos(&point);
		point.x -= rect.left;
		point.y -= rect.top;
		return point;
	}
public:
	VirtualView(CWnd* wnd) {
		this->m_parent = wnd;
	}
	virtual ~VirtualView() = default;
public:
	CWnd* m_parent;
	CClientDC* m_cdc;
	CRect GetViewRect() {
		CRect rect;
		m_parent->GetClientRect(&rect);
		return rect;
	}
	virtual int OnCreate() {
		this->m_cdc = new CClientDC(this->m_parent);
		return 0;
	}
	virtual void OnDestroy() {
		if (this->m_cdc != nullptr) {
			delete this->m_cdc;
		}
	}

	virtual void OnPaint(CDC* pDC){}

	virtual void OnSetFocus(CWnd* pOldWnd){}
	virtual void OnKillFocus(CWnd* pNewWnd){}
	
	virtual void OnLButtonDown(UINT nFlags, CPoint point){}
	virtual void OnLButtonUp(UINT nFlags, CPoint point){}
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point){}
	virtual void OnRButtonUp(UINT nFlags, CPoint point){}
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
		return TRUE;
	}
	virtual void OnMouseMove(UINT nFlags, CPoint point){}
	virtual void OnMouseLeave(){}

	virtual BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
		return TRUE;
	}
	virtual void OnSize(UINT nType, int cx, int cy){}
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){}
	
	virtual void OnTimer(UINT_PTR nIDEvent){}
	//virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//virtual void OnContextMenu(CWnd* pWnd, CPoint point);
	//virtual LONG OnComposition(UINT wParam, LONG lParam);
};

