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
	std::vector<COLORREF> g_color_bk = { RGB(49,60,90),RGB(241,241,242) ,RGB(40,40,40) };
	std::vector<COLORREF> g_color_hover = { RGB(66,165,245) ,RGB(243,199,78) ,RGB(251,107,78) };
	std::vector<COLORREF> g_color_text= { RGB(253,254,255), RGB(83,83,83), RGB(250,250,255) };
	std::vector<COLORREF> g_color_border = { MSP_COLORMAP_SPRING,RGB(243,199,78),RGB(211,54,130) };
	std::vector<COLORREF> g_color_deactivate = { RGB(33,63,83) , RGB(216,216,216),RGB(147,161,161) };

	std::vector<COLORREF> g_color_title = { RGB(66,165,245),RGB(243,199,78),RGB(251,107,78) };
	std::vector<TString> g_string_title = {TEXT("Fluent Design App"),TEXT("Material Design App"),TEXT("Flattastic Design App") };

	std::vector<COLORREF> g_control_foreground = { RGB(66,165,245) ,RGB(85,143,242) ,RGB(251,107,78) };
	std::vector<COLORREF> g_control_background = { RGB(49,60,90) ,RGB(216,216,216) ,RGB(147,161,161) };
	std::vector<COLORREF> g_control_text = { RGB(253,254,255) ,RGB(83,83,83) ,RGB(240,240,244) };
	std::vector<COLORREF> g_control_text2 = { RGB(253,254,255) ,RGB(253,254,255) ,RGB(240,240,244) };
	std::vector<bool> g_shadow = { true,true,false };
	
};
__declspec(selectany) Theme* ptheme = nullptr;
void ThemeRelease();
Theme& GetTheme();