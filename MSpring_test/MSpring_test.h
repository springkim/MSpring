
// MSpring_test.h : MSpring_test ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMSpring_testApp:
// �� Ŭ������ ������ ���ؼ��� MSpring_test.cpp�� �����Ͻʽÿ�.
//

class CMSpring_testApp : public CWinApp
{
public:
	CMSpring_testApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMSpring_testApp theApp;
