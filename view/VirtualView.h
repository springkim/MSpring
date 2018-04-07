#pragma once
#include<vector>
#include<string>
#include<imm.h>
#include<set>
#include<memory>
#include"../control/control.h"
__declspec(selectany) CDC* pDC = nullptr;
__declspec(selectany) CWnd* pOldWnd = nullptr;
__declspec(selectany) CWnd* pNewWnd = nullptr;
__declspec(selectany) UINT nFlags = -1;
__declspec(selectany) CPoint point(-1, -1);
__declspec(selectany) short zDelta = -1;
__declspec(selectany) CPoint pt(-1, -1);
__declspec(selectany) CWnd* pWnd = nullptr;
__declspec(selectany) UINT nHitTest = -1;
__declspec(selectany) UINT message = -1;
__declspec(selectany) UINT nType = -1;
__declspec(selectany) int cx = -1;
__declspec(selectany) int cy = -1;
__declspec(selectany) UINT nChar = -1;
__declspec(selectany) UINT nRepCnt = -1;
__declspec(selectany) UINT_PTR nIDEvent = -1;
__declspec(selectany) WPARAM wParam = -1;
__declspec(selectany) LPARAM lParam = -1;
#define MCALL	do{\
std::string func=__FUNCTION__;\
if(func.find("OnPaint")!=std::string::npos)VirtualView::OnPaint(pDC);\
if(func.find("OnSetFocus")!=std::string::npos)VirtualView::OnSetFocus(pOldWnd);\
if(func.find("OnKillFocus")!=std::string::npos)VirtualView::OnKillFocus(pNewWnd);\
if(func.find("OnLButtonDown")!=std::string::npos)VirtualView::OnLButtonDown(nFlags,point);\
if(func.find("OnLButtonUp")!=std::string::npos)VirtualView::OnLButtonUp(nFlags,point);\
if(func.find("OnLButtonDblClk")!=std::string::npos)VirtualView::OnLButtonDblClk(nFlags,point);\
if(func.find("OnRButtonDown")!=std::string::npos)VirtualView::OnRButtonDown(nFlags,point);\
if(func.find("OnRButtonUp")!=std::string::npos)VirtualView::OnRButtonUp(nFlags,point);\
if(func.find("OnMouseMove")!=std::string::npos)VirtualView::OnMouseMove(nFlags,point);\
if(func.find("OnMouseWheel")!=std::string::npos)VirtualView::OnMouseWheel(nFlags,zDelta,pt);\
if(func.find("OnMouseLeave")!=std::string::npos)VirtualView::OnMouseLeave();\
if(func.find("OnSetCursor")!=std::string::npos)VirtualView::OnSetCursor(pWnd,nHitTest,message);\
if(func.find("OnSize")!=std::string::npos)VirtualView::OnSize(nType,cx,cy);\
if(func.find("OnKeyDown")!=std::string::npos)VirtualView::OnKeyDown(nChar,nRepCnt,nFlags);\
if(func.find("OnKeyUp")!=std::string::npos)VirtualView::OnKeyUp(nChar,nRepCnt,nFlags);\
if(func.find("OnChar")!=std::string::npos)VirtualView::OnChar(nChar,nRepCnt,nFlags);\
if(func.find("OnTimer")!=std::string::npos)VirtualView::OnTimer(nIDEvent);\
if(func.find("OnComposition")!=std::string::npos)VirtualView::OnComposition(wParam,lParam);\
if(func.find("OnDestroy")!=std::string::npos)VirtualView::OnDestroy();\
}while(0);
class VirtualView {
protected:
	std::set<std::shared_ptr<MControlObject>> m_objs;
public:
	void AddControl(const std::shared_ptr<MControlObject>& obj) {
		m_objs.insert(obj);
	}
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
	CClientDC* m_cdc = nullptr;
	
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

	virtual void OnPaint(CDC* pDC){
		for_each(m_objs.begin(), m_objs.end(), [pDC](std::shared_ptr<MControlObject> obj)->void {obj->OnPaint(pDC); });
	}
	virtual void OnSetFocus(CWnd* pOldWnd){}
	virtual void OnKillFocus(CWnd* pNewWnd){}
	
	virtual void OnLButtonDown(UINT nFlags, CPoint point){
		for_each(m_objs.begin(), m_objs.end(), [](std::shared_ptr<MControlObject> obj)->void {obj->OnLButtonDown(); });
	}
	virtual void OnLButtonUp(UINT nFlags, CPoint point){
		for_each(m_objs.begin(), m_objs.end(), [](std::shared_ptr<MControlObject> obj)->void {obj->OnLButtonUp(); });
	}
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point){
		for_each(m_objs.begin(), m_objs.end(), [](std::shared_ptr<MControlObject> obj)->void {obj->OnLButtonDblClk(); });
	}
	virtual void OnRButtonDown(UINT nFlags, CPoint point) {
		for_each(m_objs.begin(), m_objs.end(), [](std::shared_ptr<MControlObject> obj)->void {obj->OnRButtonDown(); });
	}
	virtual void OnRButtonUp(UINT nFlags, CPoint point){
		for_each(m_objs.begin(), m_objs.end(), [](std::shared_ptr<MControlObject> obj)->void {obj->OnRButtonUp(); });
	}
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
		for_each(m_objs.begin(), m_objs.end(), [zDelta](std::shared_ptr<MControlObject> obj)->void {obj->OnMouseWheel(zDelta); });
		return TRUE;
	}
	virtual void OnMouseMove(UINT nFlags, CPoint point){
		for_each(m_objs.begin(), m_objs.end(), [](std::shared_ptr<MControlObject> obj)->void {obj->OnMouseMove(); });
	}
	virtual void OnMouseLeave(){
		for_each(m_objs.begin(), m_objs.end(), [](std::shared_ptr<MControlObject> obj)->void {obj->OnMouseLeave(); });
	}

	virtual BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
		return TRUE;
	}
	virtual void OnSize(UINT nType, int cx, int cy){}
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
		for_each(m_objs.begin(), m_objs.end(), [nChar](std::shared_ptr<MControlObject> obj)->void {obj->OnKeyDown(nChar); });
	}
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
		for_each(m_objs.begin(), m_objs.end(), [nChar](std::shared_ptr<MControlObject> obj)->void {obj->OnKeyUp(nChar); });
	}
	virtual void OnTimer(UINT_PTR nIDEvent){
		for_each(m_objs.begin(), m_objs.end(), [nIDEvent](std::shared_ptr<MControlObject> obj)->void {obj->OnTimer(nIDEvent); });
	}
	virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
		for_each(m_objs.begin(), m_objs.end(), [nChar](std::shared_ptr<MControlObject> obj)->void {obj->OnChar(nChar); });
	}
	//virtual void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual LRESULT OnComposition(WPARAM wParam, LPARAM lParam) {
		for_each(m_objs.begin(), m_objs.end(), [wParam, lParam](std::shared_ptr<MControlObject> obj)->void {obj->OnComposition(wParam, lParam); });
		return 1;
	}
};

