﻿
// ChildView.cpp : CChildView 클래스의 구현
//

#include "stdafx.h"
//#include "MSpring_test.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"mspring/MSpring.h"
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString text = TEXT("Hello, 김봄");
	int h=mspring::Font::GetRealFontHeight(TEXT("Arial"), 50, &dc,text,false);
	CFont font;
	font.CreatePointFont(h, TEXT("Arial"));
	dc.SelectObject(&font);
	dc.TextOutA(0, 0, text);
	dc.MoveTo(0, 20);
	dc.LineTo(50, 20);
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}

