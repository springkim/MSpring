
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MSpring_test.h"

#include "MainFrm.h"


#include"mspring/MSpring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, MSpringFrame)

BEGIN_MESSAGE_MAP(CMainFrame, MSpringFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()
	ON_COMMAND(ID_32775, &CMainFrame::On32775)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{

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

	


	this->SetStyle(g_font[0],g_color_bk[0], g_color_text[0],g_color_border[0]);

	m_menu_frame = new MSpringMenuFrame(this);
	m_menu_frame->SetStyle(g_font[0], g_color_bk[0], g_color_text[0], g_color_hover[0], g_color_bk[0]);
	m_menu_frame->SetMenu(IDR_MAINFRAME);
	m_menu_frame->SetPosition(0);
	this->AddExpansionClass(m_menu_frame);

	m_tab_frame = new MSpringTabFrame(this);
	m_tab_frame->SetStyle(g_font[0], g_color_hover[0], g_color_deactivate[0], g_color_bk[0], g_color_text[0], RGB(0, 0, 0));
	m_tab_frame->SetPosition(0);
	m_tab_frame->AddTab(TEXT("Dark"));
	m_tab_frame->AddTab(TEXT("Light"));
	m_tab_frame->AddTab(TEXT("solarized light"));
	m_tab_frame->AddTab(TEXT("solarized dark"));
	this->AddExpansionClass(m_tab_frame);
	m_wndView.SetStyle(g_color_bk[0]);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !MSpringFrame::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

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

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	
	MSpringFrame::OnLButtonDown(nFlags, point);
}


void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	MSpringFrame::OnNcLButtonDown(nHitTest, point);
	int idx = m_tab_frame->GetCurrentTab();
	g_idx = idx;
	this->SetStyle(g_font[idx], g_color_bk[idx], g_color_text[idx], g_color_border[idx]);
	m_menu_frame->SetStyle(g_font[idx], g_color_bk[idx], g_color_text[idx], g_color_hover[idx], g_color_bk[idx]);
	m_tab_frame->SetStyle(g_font[idx], g_color_hover[idx], g_color_deactivate[idx], g_color_bk[idx], g_color_text[idx], RGB(idx, idx, idx));
	
	m_wndView.SetStyle(g_color_bk[idx]);
	m_wndView.Invalidate();
	this->Invalidate();
}


void CMainFrame::On32775() {
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CMainFrame::OnDestroy() {
	MSpringFrame::OnDestroy();
	delete m_menu_frame;
	delete m_tab_frame;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
