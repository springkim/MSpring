/*
*  MSpringMenuFrame.hpp
*  MSpring
*
*  Created by kimbom on 2017. 9. 12...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_MSPRINGMENUFRAME_HPP_INCLUDED)
#define MSPRING_7E1_9_C_MSPRINGMENUFRAME_HPP_INCLUDED
#include<afxwin.h>

#include"MSpringFrame.h"

#include<deque>
#include<vector>
///MSpringMenu
class MSpringMenu : public CMenu {
	/*
	*	@class : BMDLMenu
	*	@comment : 이 메뉴는 원하는 대로 색상을 바꿀 수 있는 커스텀 메뉴 입니다.
	*/
public:		//constructor,destructor
	MSpringMenu(CWnd* wnd) :CMenu() {
		m_parent_wnd = wnd;
	}
	virtual ~MSpringMenu() {
		for (size_t i = 0; i < deleteItem.size(); i++) {
			delete ((MenuObject*)deleteItem[i]);
		}
		for (size_t i = 0; i < deleteMenu.size(); i++) {
			delete ((MSpringMenu*)deleteMenu[i]);
		}
	}
private:		//Inner class
	struct MenuObject {
		HICON m_hIcon;
		CString m_strCaption;
		BOOL bFirstMenu;
	};
	std::vector<DWORD_PTR> deleteItem;
	std::vector<DWORD_PTR> deleteMenu;
	//std::vector<MenuObject*> m_objects;
protected:
	CWnd* m_parent_wnd;
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)override {
		static CWindowDC dc(m_parent_wnd);
		lpMeasureItemStruct->itemHeight = MENU_HEIGHT;
		CFont font;
		int h = mspring::Font::GetRealFontHeight(m_font_string, lpMeasureItemStruct->itemHeight, &dc);
		font.CreatePointFont(h, m_font_string);
		CFont* old_font = dc.SelectObject(&font);
		CString str = ((MenuObject*)lpMeasureItemStruct->itemData)->m_strCaption;
		CSize sz;
		::GetTextExtentPoint32(dc.GetSafeHdc(), str, str.GetLength(), &sz);

		lpMeasureItemStruct->itemWidth = sz.cx + 10 < 100 ? 100 : sz.cx + 10;
	}
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)override {
		//CMenu::DrawItem(lpDrawItemStruct);
		CRect rect(lpDrawItemStruct->rcItem);
		COLORREF color_bk = MSpringMenu::m_color_bk;

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		::FillRect(pDC->GetSafeHdc(), &rect, CreateSolidBrush(color_bk));
		if (lpDrawItemStruct->itemID != 0) {
			if (((MenuObject*)lpDrawItemStruct->itemData)->bFirstMenu) {
				//일반 상태
				color_bk = MSpringMenu::m_color_bk;
			}
			if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
				(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
				//Hover 상태
				color_bk = MSpringMenu::m_color_hover;
			}
			CFont font;
			int h = mspring::Font::GetRealFontHeight(m_font_string, MENU_HEIGHT, pDC);
			font.CreatePointFont(h, m_font_string);
			CFont* old_font = pDC->SelectObject(&font);
			pDC->SetBkColor(color_bk);
			pDC->SetTextColor(MSpringMenu::m_color_text);
			pDC->TextOut(rect.left + 5, rect.top, ((MenuObject*)lpDrawItemStruct->itemData)->m_strCaption);
		} else {
			//구분선
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, MSpringMenu::m_color_text);
			CPen* old_pen = pDC->SelectObject(&pen);
			pDC->MoveTo(rect.left, (rect.top + rect.bottom) / 2);
			pDC->LineTo(rect.right, (rect.top + rect.bottom) / 2);
			pDC->SelectObject(old_pen);
			pen.DeleteObject();
		}
	}
public:	//static member
	const int MENU_HEIGHT = 16;
	static CString m_font_string;
	static COLORREF m_color_bk;
	static COLORREF m_color_text;
	static COLORREF m_color_hover;
	static COLORREF m_color_border;
public:
	void MakeItemsOwnDraw() {
		//https://stackoverflow.com/questions/30353644/cmenu-border-color-on-mfc-solved
		MENUINFO MenuInfo = { 0 };
		MenuInfo.cbSize = sizeof(MENUINFO);
		GetMenuInfo(&MenuInfo);
		MenuInfo.hbrBack = ::CreateSolidBrush(MSpringMenu::m_color_border);
		MenuInfo.fMask = MIM_BACKGROUND | MIM_STYLE;
		MenuInfo.dwStyle = MIM_APPLYTOSUBMENUS;
		SetMenuInfo(&MenuInfo);

		int iMaxItems = GetMenuItemCount();
		for (int i = 0; i < iMaxItems; i++) {
			CString nameHolder;
			MenuObject* pObject = new MenuObject;
			//m_objects.push_back(pObject);
			deleteItem.push_back((DWORD_PTR)pObject);
			pObject->m_hIcon = NULL;
			pObject->bFirstMenu = TRUE;
			GetMenuString(i, pObject->m_strCaption, MF_BYPOSITION);
			MENUITEMINFO mInfo;
			ZeroMemory(&mInfo, sizeof(MENUITEMINFO));
			//I dont use GetMenuItemID because it doesn't return 0/-1 when it's a Popup (so the MSDN is wrong)
			UINT uID = mInfo.wID;
			uID = GetMenuItemID(i);

			ModifyMenu(i, MF_BYPOSITION | MF_OWNERDRAW,
					   uID, (TCHAR*)pObject);
			if (GetSubMenu(i)) {
				MSpringMenu* pSubMenu = new MSpringMenu(m_parent_wnd);
				deleteMenu.push_back((DWORD_PTR)pSubMenu);
				pSubMenu->Attach(GetSubMenu(i)->GetSafeHmenu());
				pSubMenu->MakeItemsOwnDraw();
			}
		}
	}
	static void SetStyle(CString font_string, COLORREF color_bk, COLORREF color_text, COLORREF color_hover, COLORREF color_border) {
		MSpringMenu::m_font_string = font_string;
		MSpringMenu::m_color_bk = color_bk;
		MSpringMenu::m_color_text = color_text;
		MSpringMenu::m_color_hover = color_hover;
		MSpringMenu::m_color_border = color_border;
	}
};
__declspec(selectany) CString MSpringMenu::m_font_string = TEXT("Arial");
__declspec(selectany) COLORREF MSpringMenu::m_color_bk = RGB(255, 255, 255);
__declspec(selectany) COLORREF MSpringMenu::m_color_text = RGB(0, 0, 0);
__declspec(selectany) COLORREF MSpringMenu::m_color_hover = RGB(199, 199, 199);
__declspec(selectany) COLORREF MSpringMenu::m_color_border = RGB(103, 153, 255);

///MSpringMenuFrame
class MSpringMenuFrame : public MSpringFrameExpansion {
	/*
	*	@class : BMDLMenuFrame
	*	@comment : 이 클래스는 BMDLFrame에 사용될 확장 클래스 입니다.
	*				 , 커스텀 메뉴를 제공 합니다.
	*/
protected:
	// 이 클래스는 메뉴를 한번에 띄우지 않고 최상위 메뉴를 Caption에 그린뒤 알맞은 위치에 PopupMenu를 띄우는 방식으로 
	//구성 했기에, CString과 CMenu를 각각 가지고 있습니다.
	std::vector<std::pair<CString, CMenu*>> m_menu;
	//메뉴에 대한 영역 입니다.
	std::deque<CRect> m_menu_rect;		
	//hover된 메뉴의 인덱스 입니다.
	int m_menu_hover = -1;				
	CMenu* m_bmdl_menu = nullptr;	
	//align 입니다. 0일경우 left-align, 그 이외의 경우 right-align
	UINT m_position = 0;					
protected:		//style value
	//메뉴에 사용될 폰트입니다.
	CString m_font_str;
	//글꼴의 색상입니다.
	COLORREF m_color_text;	
	//Hover 된 메뉴의 배경 색 입니다.
	COLORREF m_color_hover;
	//메뉴의 배경색 입니다.
	COLORREF m_color_bk;					
public:
	void SetPosition(UINT left_is_zero_right_is_nonzero) {
		m_position = left_is_zero_right_is_nonzero;
	}
	void SetStyle(CString font, COLORREF bk, COLORREF text, COLORREF hover, COLORREF border) {
		m_font_str = font;
		m_color_bk = bk;
		m_color_text = text;
		m_color_hover = hover;
		MSpringMenu::SetStyle(font, bk, text, hover, border);
		if (m_bmdl_menu != nullptr) {
			((MSpringMenu*)m_bmdl_menu)->MakeItemsOwnDraw();
		}
	}
	void SetMenu(UINT menu_id) {
		if (m_bmdl_menu == nullptr) {
			m_bmdl_menu = new MSpringMenu(m_wnd);
			m_bmdl_menu->LoadMenu(menu_id);
			((MSpringMenu*)m_bmdl_menu)->MakeItemsOwnDraw();
			int c = m_bmdl_menu->GetMenuItemCount();
			for (int i = 0; i < c; i++) {
				CString str;
				m_bmdl_menu->GetMenuString(i, str, MF_BYPOSITION);
				m_bmdl_menu->ModifyMenu(i, MF_BYPOSITION | MF_OWNERDRAW, i, (LPCTSTR)m_bmdl_menu->m_hMenu);
				m_menu.push_back(std::make_pair(str, m_bmdl_menu));
			}
		}
	}
public:			//constructor,destructor
	MSpringMenuFrame(CWnd* wnd) :MSpringFrameExpansion(wnd){
		m_font_str = TEXT("Arial");
		m_color_text = RGB(255, 255, 255);
		m_color_hover = RGB(255, 199, 199);
		m_color_bk = RGB(255, 255, 255);
	}
	virtual ~MSpringMenuFrame() = default;
public:			//messageevent method
	void OnCreate(LPCREATESTRUCT lpCreateStruct)override{ }
	int OnNcPaint(CDC* pDC, CRect rect)override {
		const int margin = 7;
		rect.top += margin;
		rect.bottom -= margin;
		CBrush brush;
		brush.CreateSolidBrush(m_color_bk);
		pDC->FillRect(rect, &brush);
		int h = mspring::Font::GetRealFontHeight(m_font_str, rect.Height(), pDC);
		CFont font;
		font.CreatePointFont(h, m_font_str);
		CFont* old_font = pDC->SelectObject(&font);
		pDC->SetBkColor(m_color_bk);
		pDC->SetTextColor(m_color_text);
		m_menu_rect.clear();
		int posx = 0;
		int ret = 0;
		if (m_position == 0) {	//set left
			posx = rect.left;
			for (int i = 0; i < (int)m_menu.size(); i++) {
				CString str = m_menu[i].first;
				CSize sz;
				::GetTextExtentPoint32(pDC->GetSafeHdc(), str, str.GetLength(), &sz);
				m_menu_rect.push_back(CRect(posx, rect.top, posx + sz.cx + margin, rect.bottom));
				if (i == m_menu_hover) {
					CBrush brush;
					brush.CreateSolidBrush(m_color_hover);
					pDC->FillRect(m_menu_rect.back(), &brush);
					brush.DeleteObject();
				}
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOut(posx + margin / 2, margin, str, str.GetLength());
				posx += sz.cx + margin;
				if (posx > rect.right) {
					break;
				}
			}
			ret = posx - rect.left;
		} else {
			posx = rect.right;
			for (int i = (int)m_menu.size() - 1; i >= 0; i--) {
				CString str = m_menu[i].first;
				CSize sz;
				::GetTextExtentPoint32(pDC->GetSafeHdc(), str, str.GetLength(), &sz);
				m_menu_rect.push_front(CRect(posx - sz.cx - margin, rect.top, posx, rect.bottom));
				if (i == m_menu_hover) {
					CBrush brush;
					brush.CreateSolidBrush(m_color_hover);
					pDC->FillRect(m_menu_rect.back(), &brush);
					brush.DeleteObject();
				}
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOut(posx - sz.cx - margin, margin, str, str.GetLength());
				posx -= sz.cx + margin;
				if (posx < rect.left) {
					break;
				}
			}
			ret = -(rect.right - posx);
		}
		pDC->SelectObject(old_font);
		brush.DeleteObject();
		font.DeleteObject();
		return ret;
	}
	void OnSize(UINT nType, int cx, int cy)override {}
	bool OnNcLButtonDown(UINT nHitTest, CPoint point) override {
		CPoint apoint;
		::GetCursorPos(&apoint);
		bool ret = false;
		for (int i = 0; i < (int)m_menu_rect.size(); i++) {
			if (m_menu_rect[i].PtInRect(point) == TRUE) {
				int rVal = (int)m_menu[i].second->GetSubMenu(i)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD
																				, m_menu_rect[i].left + apoint.x - point.x
																				, m_menu_rect[i].bottom + apoint.y - point.y
																				, m_wnd);
				if (rVal != 0) {
					m_wnd->PostMessage(WM_COMMAND, rVal, 0);
					ret = true;
				}
			}
		}
		return ret;
	}
	bool OnNcMouseMove(UINT nHitTest, CPoint point) override {
		m_menu_hover = -1;
		bool ret = false;
		for (int i = 0; i < (int)m_menu_rect.size(); i++) {
			if (m_menu_rect[i].PtInRect(point) == TRUE) {
				m_menu_hover = i;
				ret = true;
				break;
			}
		}
		return ret;
	}
	bool OnNcLButtonUp(UINT nHitTest, CPoint point)override {
		return false;
	}
	bool OnNcLButtonDblClk(UINT nHitTest, CPoint point)override {
		return false;
	}
	void OnNcMouseLeave() override {
		m_menu_hover = -1;
	}
	void OnDestroy() override {
		delete m_bmdl_menu;
	}
};
#endif  //MSPRING_7E1_9_C_MSPRINGMENUFRAME_HPP_INCLUDED