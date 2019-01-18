#pragma once
#include<vector>
#include"stdafx.h"
#include"MSpring/utils/String.h"
#include"MSpring\MSpring.h"
class Theme {
public:
	Theme() {

	}
public:
	int g_idx = 0;
	std::vector<TString> g_font = { TEXT("Arial") ,  TEXT("Arial"), TEXT("Arial") };
	std::vector<COLORREF> g_color_bk = { RGB(49,60,90),RGB(240,240,244) ,RGB(253,246,227) };
	std::vector<COLORREF> g_color_hover = { RGB(66,165,245) ,RGB(121,171,255) ,RGB(38,139,210) };
	std::vector<COLORREF> g_color_text= { RGB(253,254,255), RGB(33, 33, 33), RGB(7, 54, 66) };
	std::vector<COLORREF> g_color_border = { MSP_COLORMAP_SPRING,RGB(107,102,255),RGB(211,54,130) };
	std::vector<COLORREF> g_color_deactivate = { RGB(33,63,83) , RGB(233,233,233),RGB(147,161,161) };

	std::vector<COLORREF> g_color_title = { RGB(0,225,255),RGB(64,129,236),RGB(64,129,236) };
	std::vector<TString> g_string_title = {TEXT("Fluent Design App"),TEXT("Material Design App"),TEXT("Flattastic Design App") };

	std::vector<COLORREF> g_control_foreground = { RGB(66,165,245) ,RGB(121,171,255) ,RGB(38,139,210) };
	std::vector<COLORREF> g_control_text = { RGB(253,254,255) ,RGB(121,171,255) ,RGB(38,139,210) };
	
};
__declspec(selectany) Theme* ptheme = nullptr;
void ThemeRelease();
Theme& GetTheme();