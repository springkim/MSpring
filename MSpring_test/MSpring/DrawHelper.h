/*
*  DrawHelper.hpp
*  MSpring
*
*  Created by kimbom on 2017. 9. 12...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_DRAWHELPER_HPP_INCLUDED)
#define MSPRING_7E1_9_C_DRAWHELPER_HPP_INCLUDED
#include<afxwin.h>
#include<wingdi.h>
#include<winnt.h>
namespace mspring {
	class Font {
	public:
		static int GetRealFontHeight(CString font_str, int size, CDC* pdc,CString str=TEXT("A"),bool height=true) {
			int L = 0;
			int R = 1000;
			CSize sz;
			decltype(CSize::cx)* lp = nullptr;
			if (height == true) {
				lp = &sz.cy;
			} else {
				lp = &sz.cx;
			}
			while (L <= R) {
				int M = (L + R) / 2;
				CFont font;
				font.CreatePointFont(M, font_str);
				CFont* old_font = pdc->SelectObject(&font);
				GetTextExtentPoint32(pdc->GetSafeHdc(), str, str.GetLength(), &sz);
				if (*lp == size) {
					break;
				} else if (*lp > size) {
					R = M - 1;
				} else {
					L = M + 1;
				}
				pdc->SelectObject(old_font);
				font.DeleteObject();
			}
			return (L + R) / 2;
		}
	};
}
#endif  //MSPRING_7E1_9_C_DRAWHELPER_HPP_INCLUDED