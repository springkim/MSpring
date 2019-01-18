#include"Theme.h"
void ThemeRelease() {
	if (ptheme) {
		delete ptheme;
		ptheme = nullptr;
	}
}
Theme& GetTheme() {
	if (ptheme == nullptr) {
		ptheme = new Theme();
		atexit(ThemeRelease);
	}
	return *ptheme;
}