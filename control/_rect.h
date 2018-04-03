/**
* @file		_rect.h
* @author		kimbomm (springnode@gmail.com)
* @date		2017. 10. 10...
* @version	1.0.0
*
*  @brief
*			MSpring 공용 컨트롤에 사용되는 좌표
*	@remark
*			Created by kimbom on 2017. 10. 10...
*			Copyright 2017 kimbom.All rights reserved.
*/
#pragma once
#if !defined(MSPRING_7E1_A_9__RECT_HPP_INCLUDED)
#define MSPRING_7E1_A_9__RECT_HPP_INCLUDED
#include<afxwin.h>
enum MRectPosition {
	LT, RT, LB, RB, DYNAMIC, L, R, T, B
};
class MRect {
private:
	MRectPosition m_pos;
	int m_xd;		//left-dist
	int m_yd;		//top-dist
	int m_xd2;	//pos가 DYNAMIC이면 right-dist, 그 이외의 경우 width 입니다.
	int m_yd2;	//pos가 DYNAMIC이면 bottom-dist, 그 이외의 경우 height 입니다.
public:
	MRect(MRectPosition pos, int xd, int yd, int xd2, int yd2) {
		m_pos = pos;
		m_xd = xd;
		m_yd = yd;
		m_xd2 = xd2;
		m_yd2 = yd2;
	}
	MRect() {
		m_pos = MRectPosition::LT;
		m_xd = 10;
		m_yd = 10;
		m_xd2 = 100;
		m_yd2 = 100;
	}
	CRect GetRect(CRect view) {
		CRect rect;
		switch (m_pos) {
			case L: {
				rect.left = view.left + m_xd;
				rect.top = view.top + m_yd;
				rect.right = rect.left + m_xd2;
				rect.bottom = view.bottom - m_yd2;
			}break;
			case R: {
				rect.top = view.top + m_yd;
				rect.right = view.right - m_xd2;
				rect.bottom = view.bottom - m_yd2;
				rect.left = rect.right - m_xd;
			}break;
			case T: {
				rect.left = view.left + m_xd;
				rect.top = view.top + m_yd;
				rect.right = view.right - m_xd2;
				rect.bottom = rect.top + m_yd2;
			}break;
			case B: {
				rect.left = view.left + m_xd;
				rect.bottom = view.bottom - m_yd2;
				rect.right = view.right - m_xd2;
				rect.top = rect.bottom - m_yd;
			}break;
			case LT: {
				rect.left = view.left + m_xd;
				rect.top = view.top + m_yd;
				rect.right = rect.left + m_xd2;
				rect.bottom = rect.top + m_yd2;
			}break;
			case RT: {
				rect.right = view.right - m_xd;
				rect.top = view.top + m_yd;
				rect.left = rect.right - m_xd2;
				rect.bottom = rect.top + m_yd2;
			}break;
			case LB: {
				rect.left = view.left + m_xd;
				rect.right = rect.left + m_xd2;
				rect.bottom = view.bottom - m_yd;
				rect.top = rect.bottom - m_yd2;
			}break;
			case RB: {
				rect.right = view.right - m_xd;
				rect.bottom = view.bottom - m_yd;
				rect.left = rect.right - m_xd2;
				rect.top = rect.bottom - m_yd2;
			}break;
			case DYNAMIC: {
				rect.left = view.left + m_xd;
				rect.top = view.top + m_yd;
				rect.right = view.right - m_xd2;
				rect.bottom = view.bottom - m_yd2;
			}break;
			default:break;
		}
		return rect;
	}
};
#endif  //MSPRING_7E1_A_9__RECT_HPP_INCLUDED