
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#include<iostream>
#include<MSpring/MSpring.h>
#include"ControlTestView.h"
#include"ImageTestView.h"
#include"Theme.h"


class CMainFrame : public MSpringFrame{
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	MSpringMenuFrame m_menu_frame;
	MSpringTabFrame m_tab_frame;

	MSpringView    m_wndView;
	ControlTestView* m_ctl_test_view;
	ImageTestView* m_img_test_view;
	void SetTheme();
// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void On32771();
	afx_msg void On32772();
	afx_msg void On32773();
};


