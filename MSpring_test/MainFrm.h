
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#include "ChildView.h"

#include"mspring/frame/MSpringFrame.h"
#include"mspring/frame/MSpringMenuFrame.h"
#include"mspring/frame/MSpringTabFrame.h"
#include"mspring/view/MSpringView.h"


__declspec(selectany) std::vector<CString> g_font = { TEXT("Arial") , TEXT("Arial"), TEXT("Arial"),TEXT("Arial") };
__declspec(selectany) std::vector<COLORREF> g_color_bk = { RGB(55,55,55),RGB(255,255,255) ,RGB(253,246,227),RGB(7,54,66) };
__declspec(selectany) std::vector<COLORREF> g_color_hover = { RGB(242,150,97) ,RGB(121,171,255) ,RGB(38,139,210),RGB(211,54,130) };
__declspec(selectany) std::vector<COLORREF> g_color_text = { RGB(255,255,255) ,RGB(33,33,33) ,RGB(7,54,66),RGB(238,232,213) };
__declspec(selectany) std::vector<COLORREF> g_color_border = { RGB(255,0,0),RGB(107,102,255) ,RGB(133,153,0),RGB(42,161,152) };
__declspec(selectany) std::vector<COLORREF> g_color_deactivate = { RGB(255,193,158) , RGB(233,233,233),RGB(147,161,161) ,RGB(108,113,196) };

__declspec(selectany) int g_idx = 0;

class CMainFrame : public MSpringFrame
{
	
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

	MSpringView    m_wndView;
	MSpringMenuFrame* m_menu_frame = nullptr;
	MSpringTabFrame* m_tab_frame = nullptr;

	
// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void On32775();
};


