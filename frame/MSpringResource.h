#pragma once
#include<afxwin.h>
#include<map>
inline HBITMAP GetHeaderResource(HDC hdc, int w, int h, unsigned int* image) {
	static std::map<unsigned int*, HBITMAP> dp;
	if (dp.find(image) == dp.end()) {
		BITMAPINFOHEADER   bmih;
		memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
		bmih.biWidth = w;
		bmih.biHeight = -h;
		bmih.biBitCount = 32;
		bmih.biCompression = BI_RGB;
		bmih.biSize = sizeof(BITMAPINFOHEADER);
		bmih.biPlanes = 1;
		BITMAPINFO* bmi = (BITMAPINFO*)&bmih;
		HBITMAP hbitmap = CreateDIBitmap(hdc, &bmih, CBM_INIT, image, bmi, DIB_RGB_COLORS);
		dp.insert(std::make_pair(image, hbitmap));
		return hbitmap;
	} else {
		return dp.find(image)->second;
	}
}