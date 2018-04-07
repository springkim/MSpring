/*
*  DoubleBufferingDC.hpp
*  MSpring
*
*  Created by kimbom on 2017. 9. 12...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_DOUBLEBUFFERINGDC_HPP_INCLUDED)
#define MSPRING_7E1_9_C_DOUBLEBUFFERINGDC_HPP_INCLUDED

#include<afxwin.h>
#include<wingdi.h>
#include"ErrorMessage.h"
#include"Safe.h"
///Double Buffering
namespace mspring {
	class DoubleBufferingDC {
	protected:
		CDC* m_pdc = nullptr;	//object exist or not
		CRect m_rect;
		CDC m_dc;
		CBitmap m_cbmp;
		CBitmap* m_cbmp_old = nullptr;
	public:
		CDC& getDC() {
			if (m_pdc == nullptr) {
				MSPRING_ERROR("DoubleBufferingDC is NULL");
			}
			return this->m_dc;
		}
		CDC* getPDC() {
			if (m_pdc == nullptr) {
				MSPRING_ERROR("DoubleBufferingDC is NULL");
			}
			return &this->m_dc;
		}
		DoubleBufferingDC() {
			m_pdc = nullptr;
		}
		DoubleBufferingDC(CDC* pdc, CRect rect) {
			this->Create(pdc, rect);
		}
		void Create(CDC* pdc, CRect rect) {
			this->m_pdc = pdc;
			this->m_rect = rect;
			this->m_dc.CreateCompatibleDC(pdc);
			this->m_cbmp.CreateCompatibleBitmap(pdc, rect.Width(), rect.Height());
			this->m_cbmp_old = (CBitmap*)this->m_dc.SelectObject(&this->m_cbmp);
			this->m_dc.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);
		}
		void Draw() {
			if (m_pdc == nullptr) {
				MSPRING_ERROR("DoubleBufferingDC is NULL");
			}
			this->m_pdc->BitBlt(this->m_rect.left, this->m_rect.top, this->m_rect.Width(), this->m_rect.Height(), &this->m_dc, 0, 0, SRCCOPY);
			this->m_dc.SelectObject(this->m_cbmp_old);
			this->m_dc.DeleteDC();
			this->m_cbmp.DeleteObject();
			m_pdc = nullptr;
		}
		~DoubleBufferingDC() {
			if (m_pdc != nullptr) {
				this->m_pdc->BitBlt(this->m_rect.left, this->m_rect.top, this->m_rect.Width(), this->m_rect.Height(), &this->m_dc, 0, 0, SRCCOPY);
				this->m_dc.SelectObject(this->m_cbmp_old);
				this->m_dc.DeleteDC();
				this->m_cbmp.DeleteObject();
			}
		}
	};
}
#endif  //MSPRING_7E1_9_C_DOUBLEBUFFERINGDC_HPP_INCLUDED