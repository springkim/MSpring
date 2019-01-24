/*
*  ColorTable.h
*  MSpring
*
*  Created by kimbomm on 2019. 1. 22...
*  Copyright 2019 kimbomm. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E3_1_16_COLORTABLE_H_INCLUDED)
#define MSPRING_7E3_1_16_COLORTABLE_H_INCLUDED
#include<afxwin.h>
#include<atomic>
#include<map>
#include"String.h"


#define SetThemeColor(name,color)	GetThemeColor(name,color,true)
inline COLORREF GetThemeColor(TString name,COLORREF color=RGB(0,0,0), bool insert=false) {
	static std::map<TString, COLORREF> color_map;
	if (insert) {
		VERIFY(color_map.find(name) == color_map.end());
		color_map.insert(std::make_pair(name, color));
		return color;
	} else {
		auto it=color_map.find(name);
		VERIFY(it != color_map.end());
		return (it->second);
	}
}



#endif  //MSPRING_7E3_1_16_COLORTABLE_H_INCLUDED