#pragma once
#include "MSpring/MSpring.h"
#include<memory>
#include<dwmapi.h>
#include"Theme.h"
#pragma comment(lib,"dwmapi.lib")
class ImageTestView : public VirtualView {
public:
	ImageTestView(CWnd* wnd) :VirtualView(wnd) {

	}
	void OnPaint(CDC* pDC) {
		MCALL;

		TString img_name[3] = { L"view_fluent.bmp", L"view_material.bmp", L"view_flat.bmp"};

		HBITMAP hBmp = (HBITMAP)::LoadImage(
			NULL,
			img_name[GetTheme().g_idx].data(),
			IMAGE_BITMAP,
			0,
			0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION
		);
		CBitmap cbmp;
		cbmp.Attach(hBmp);


		CDC cdc;
		cdc.CreateCompatibleDC(pDC);
		cdc.SelectObject(&cbmp);
		BITMAP bmp;
		cbmp.GetBitmap(&bmp);
	

		CRect rect_view=this->GetViewRect();
		CRect rect_img(0, 0, bmp.bmWidth, bmp.bmHeight);
		CRect rect_new;
		if (rect_view.Width() != rect_img.Width() || rect_view.Height() != rect_img.Height()) {
			rect_new.right = rect_img.Width()*rect_view.Height() / rect_img.Height();
			rect_new.bottom = rect_view.Height();
			if (rect_new.right > rect_view.Width()) {
				rect_new.right = rect_view.Width();
				rect_new.bottom= rect_img.Height()*rect_view.Width() / rect_img.Width();
			}
		}
		//pDC->TransparentBlt((rect_view.Width()-rect_new.Width())/2, (rect_view.Height() - rect_new.Height()) / 2, rect_new.Width(), rect_new.Height(), &cdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255,255,255));
	}
};