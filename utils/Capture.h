/*
*  Capture.hpp
*  MSpring
*
*  Created by kimbomm on 2018. 12. 11...
*  Copyright 2018 kimbomm. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_CAPTURE_HPP_INCLUDED)
#define MSPRING_7E1_9_C_CAPTURE_HPP_INCLUDED

#include<afxwin.h>
#include<wingdi.h>
#include<atlimage.h>
#include"ErrorMessage.h"
#include"Safe.h"
///Double Buffering
namespace mspring {
	class Capture : public CImage {
	public:
		BOOL CaptureRect(const CRect& rect) {
			// detach and destroy the old bitmap if any attached
			CImage::Destroy();

			// create a screen and a memory device context
			HDC hDCScreen = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
			HDC hDCMem = ::CreateCompatibleDC(hDCScreen);
			// create a compatible bitmap and select it in the memory DC
			HBITMAP hBitmap =
				::CreateCompatibleBitmap(hDCScreen, rect.Width(), rect.Height());
			HBITMAP hBmpOld = (HBITMAP)::SelectObject(hDCMem, hBitmap);

			// bit-blit from screen to memory device context
			// note: CAPTUREBLT flag is required to capture layered windows
			DWORD dwRop = SRCCOPY | CAPTUREBLT;
			BOOL bRet = ::BitBlt(hDCMem, 0, 0, rect.Width(), rect.Height(),
								 hDCScreen,
								 rect.left, rect.top, dwRop);
			// attach bitmap handle to this object
			Attach(hBitmap);

			// restore the memory DC and perform cleanup
			::SelectObject(hDCMem, hBmpOld);
			::DeleteDC(hDCMem);
			::DeleteDC(hDCScreen);

			return bRet;
		}
		BOOL CaptureScreen() {
			CRect rect(0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
			return CaptureRect(rect);
		}
		BOOL CaptureWindow(HWND hWnd) {
			BOOL bRet = FALSE;
			if (::IsWindow(hWnd)) {
				CRect rect;
				::GetWindowRect(hWnd, rect);
				bRet = CaptureRect(rect);
			}
			return bRet;
		}
	};
}
#endif  //MSPRING_7E1_9_C_DOUBLEBUFFERINGDC_HPP_INCLUDED