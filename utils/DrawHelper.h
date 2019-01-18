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
#include"Safe.h"
#include"String.h"
namespace mspring {
	namespace Text {
#define NO_LIGHT 0
#define KEY_LIGHT_T 0x00000001
#define KEY_LIGHT_R 0x00000010
#define KEY_LIGHT_L 0x00000100
#define KEY_LIGHT_B 0x00001000
#define KEY_LIGHT 0x00001111
#define AMBIENT_LIGHT4 0x00010000
#define AMBIENT_LIGHT8 0x00110000
		/**
		*	@brief 그림자를 포함한 텍스트를 출력합니다.
		*	@param pdc 디바이스 컨텍스트 포인터.
		*	@param x 출력될 X 좌표.
		*	@param y 출력될 Y 좌표 .
		*	@param lpString 출력할 문자열.
		*	@param c 문자열의 길이.
		*	@param font_name 폰트명.
		*	@param h 출력할 폰트의 높이, CreatePointFont함수의 첫 번째 인자.
		*	@param option 그림자의 형태. TextOutMSPOption를 참조하십시오.
		*	@param color_bk 출력될 위치의 배경색상.
		*	@param measure 이 인수가 참이면 그리지 않고 크기만 반환 합니다.
		*	@return 출력된 텍스트의 사각크기를 반환합니다.
		*	@details
		*				그림자는 key light와 ambient light 그리고 두 옵션을 합칠 수 있습니다.
		*				그림자의 색상은 배경색상에 의존적 입니다.
		*/
		static CRect TextOutMSP(_In_ CDC* pdc, _In_ int x, _In_ int y, _In_reads_(c) LPCTSTR lpString, _In_ int c, LPCTSTR font_name, int h,
								DWORD option_shadow, COLORREF color_bk=RGB(0,0,0), bool measure = false,int depth=4){
			COLORREF color_text = pdc->GetTextColor();
			int tc = (GetRValue(color_bk) + GetGValue(color_bk) + GetBValue(color_bk)) / 3;
			tc = mspring::Min(tc, 150);
			std::vector<COLORREF> color_shadows = {
				RGB(tc*0.5,tc*0.5,tc*0.5),
				RGB(tc*0.6,tc*0.6,tc*0.6),
				RGB(tc*0.7,tc*0.7,tc*0.7),
				RGB(tc*0.8,tc*0.8,tc*0.8),
				RGB(tc*0.9,tc*0.9,tc*0.9)
			};
			VERIFY(depth >= 0);
			VERIFY(color_shadows.size() >= depth);
			CFont font;
			//font.CreateFont(h/7, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, font_name);
			font.CreatePointFont(h, font_name);
			
			CFont* font_old = pdc->SelectObject(&font);
			CSize size;
			GetTextExtentPoint32(pdc->GetSafeHdc(), lpString, _tcslen(lpString), &size);
			y -= size.cy / 2;
			if (measure == false) {
				if (option_shadow&AMBIENT_LIGHT4) {
					pdc->SetTextColor(color_shadows[0]);
					int xi = 1, yi = 0;
					do {
						pdc->DrawText(lpString, &CRect(x + xi, y + yi, x + size.cx + xi, y + size.cy + yi), DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX);
						xi ^= yi ^= xi ^= yi *= -1;
					} while (xi*yi + xi<1 || ((option_shadow&AMBIENT_LIGHT8) && yi == 0 && (yi = 1)));
				}
				for (int i = depth; i > 0; i--) {
					pdc->SetTextColor(color_shadows[i - 1]);
					if (option_shadow& KEY_LIGHT_T)
						pdc->DrawText(lpString, &CRect(x + 0, y - i, x + size.cx + 0, y + size.cy - i), DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX);
					if (option_shadow& KEY_LIGHT_B)
						pdc->DrawText(lpString, &CRect(x + 0, y + i, x + size.cx + 0, y + size.cy + i), DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX);
					if (option_shadow& KEY_LIGHT_R)
						pdc->DrawText(lpString, &CRect(x + i, y + 0, x + size.cx + i, y + size.cy + 0), DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX);
					if (option_shadow& KEY_LIGHT_L)
						pdc->DrawText(lpString, &CRect(x - i, y + 0, x + size.cx - i, y + size.cy + 0), DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX);
				}
				pdc->SetTextColor(color_text);
				pdc->DrawText(lpString, &CRect(x, y, x + size.cx, y + size.cy), DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX);
			}
			pdc->SelectObject(font_old);
			font.DeleteObject();
			return CRect(x, y, x+size.cx, y+size.cy);
		}
	};
	namespace Font {
		enum CreateType {
			FontType = 0
			, FontIndirectType
			, PointFontType
			, PointFontIndirectType
			//deprecated//, GdiplusPathType
		};
		static int GetRealFontHeight(TString font_str, int size, CDC* pdc, TString str = TEXT("A"), bool height = true, CreateType type = CreateType::PointFontType) {
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

				if (type == CreateType::PointFontType) {
					font.CreatePointFont(M, font_str.data());
				} else if (type == CreateType::FontType) {
					font.CreateFont(M, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
									, NONANTIALIASED_QUALITY
									, DEFAULT_PITCH | FF_SWISS, font_str.data());
				}
				CFont* old_font = pdc->SelectObject(&font);
				GetTextExtentPoint32(pdc->GetSafeHdc(), str.data(), (int)str.length(), &sz);
				pdc->SelectObject(old_font);
				font.DeleteObject();
				

				if (*lp == size) {
					break;
				} else if (*lp > size) {
					R = M - 1;
				} else {
					L = M + 1;
				}

			}
			return (L + R) / 2;
		}
	};
}
#endif  //MSPRING_7E1_9_C_DRAWHELPER_HPP_INCLUDED