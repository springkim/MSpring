
// MSpring_Test.h : MSpring_Test ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMSpring_TestApp:
// �� Ŭ������ ������ ���ؼ��� MSpring_Test.cpp�� �����Ͻʽÿ�.
//

class CMSpring_TestApp : public CWinApp
{
public:
	CMSpring_TestApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMSpring_TestApp theApp;
