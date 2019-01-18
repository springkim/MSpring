
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MSpring_Test.h"

#include "MainFrm.h"
#include<dwmapi.h>
#pragma comment(lib,"dwmapi.lib")



// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, MSpringFrame)

BEGIN_MESSAGE_MAP(CMainFrame, MSpringFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MSpringFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}
	
	
	
	
#ifdef _DEBUG
	if (::AllocConsole() == TRUE) {
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio(false);
	}
#endif
	this->AddSysBtn(RESOURCE_CLOSE, MSpringFrame::ButtonEvent_Close);
	this->AddSysBtn(RESOURCE_MAXIMIZE, MSpringFrame::ButtonEvent_MaximizeWindow);
	this->AddSysBtn(RESOURCE_MINIMIZE, MSpringFrame::ButtonEvent_MinimizeWindow);
	this->SetIcon(IDR_MAINFRAME);
	this->SetStyle(GetTheme().g_font[0], GetTheme().g_color_bk[0], GetTheme().g_color_text[0], GetTheme().g_color_border[0]);
	this->SetTitle(GetTheme().g_string_title[GetTheme().g_idx]);
	this->SetTitleColor(GetTheme().g_color_title[GetTheme().g_idx]);
	this->SetAcrylicThemeAsBackgroundColor();
	m_menu_frame = CreateFrame<MSpringMenuFrame>(this);
	
	m_menu_frame->SetStyle(GetTheme().g_font[0], GetTheme().g_color_bk[0], GetTheme().g_color_text[0], GetTheme().g_color_hover[0], GetTheme().g_color_bk[0]);
	m_menu_frame->SetMenu(IDR_MAINFRAME);
	m_menu_frame->SetPosition(0);
	this->AddExpansionClass(m_menu_frame);

	m_tab_frame = CreateFrame<MSpringTabFrame>(this);
	m_tab_frame->SetStyle(GetTheme().g_font[0], GetTheme().g_color_hover[0], GetTheme().g_color_deactivate[0], GetTheme().g_color_bk[0], GetTheme().g_color_text[0], RGB(0, 0, 0));
	m_tab_frame->SetPosition(0);
	m_tab_frame->SetActivateBorder(GetTheme().g_color_hover[0]);
	
	m_tab_frame->AddTab(TEXT("Fluent"));
	m_tab_frame->AddTab(TEXT("Material"));
	m_tab_frame->AddTab(TEXT("Flattastic"));
	this->AddExpansionClass(m_tab_frame);


	m_wndView.SetStyle(GetTheme().g_color_bk[0]);
	static ControlTestView ctl_test_view(&m_wndView);
	static ImageTestView img_test_view(&m_wndView);
	m_ctl_test_view = &ctl_test_view;
	m_img_test_view =&img_test_view;
	m_wndView.m_view = m_ctl_test_view;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !MSpringFrame::PreCreateWindow(cs) )
		return FALSE;
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	MSpringFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	MSpringFrame::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return MSpringFrame::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	MSpringFrame::OnNcLButtonDown(nHitTest, point);
	int idx = m_tab_frame->GetCurrentTab();
	GetTheme().g_idx = idx;
	this->SetStyle(GetTheme().g_font[idx], GetTheme().g_color_bk[idx], GetTheme().g_color_text[idx], GetTheme().g_color_border[idx]);
	m_menu_frame->SetStyle(GetTheme().g_font[idx], GetTheme().g_color_bk[idx], GetTheme().g_color_text[idx], GetTheme().g_color_hover[idx], GetTheme().g_color_bk[idx]);
	m_tab_frame->SetStyle(GetTheme().g_font[idx], GetTheme().g_color_hover[idx], GetTheme().g_color_deactivate[idx], GetTheme().g_color_bk[idx], GetTheme().g_color_text[idx], RGB(idx, idx, idx));

	m_wndView.SetStyle(GetTheme().g_color_bk[idx]);

	if (idx == 0) {
		m_wndView.m_view = this->m_ctl_test_view;
		this->SetAcrylicThemeAsBackgroundColor();
	} else if (idx == 1) {
		m_wndView.m_view = this->m_ctl_test_view;
		this->SetMaterialTheme();
	} else {
		m_wndView.m_view = this->m_img_test_view;
		this->SetFlatTheme();
	}
	
	m_wndView.Invalidate();
	this->Invalidate();
}


void CMainFrame::OnDestroy() {
	MSpringFrame::OnDestroy();
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) {
	return MSpringFrame::OnEraseBkgnd(pDC);
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) {
	MSpringFrame::OnActivate(nState, pWndOther, bMinimized);
	

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
