/*
*  String.hpp
*  MSpring
*
*  Created by kimbom on 2017. 9. 12...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_STRING_HPP_INCLUDED)
#define MSPRING_7E1_9_C_STRING_HPP_INCLUDED
#include<afxwin.h>

using TString = std::basic_string<TCHAR>;

#ifdef _UNICODE
using OStringStream = std::wostringstream;
using IStringStream = std::wistringstream;
#else
using OStringStream = std::ostringstream;
using IStringStream = std::istringstream;
#endif
namespace mspring {
	/**
	* static class : String
	* Convert [CString<->std::string<->std::wstring]
	*/
	class String {
	public:
		static void ToWChar(char* src, wchar_t* dst, size_t dst_len) {
			ZeroMemory(dst, dst_len * sizeof(wchar_t));
			int len = MultiByteToWideChar(CP_ACP, 0, src, static_cast<int>(strlen(src)), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, src, static_cast<int>(strlen(src)), dst, len);
		}
		static void ToChar(wchar_t* src, char* dst, size_t dst_len) {
			ZeroMemory(dst, dst_len * sizeof(char));
			int len = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, src, -1, dst, len, NULL, NULL);
		}
		static std::string ToString(std::wstring str) {
			return std::string(CW2A(str.c_str()));
		}
		__declspec(deprecated("** Don't use CString **")) static std::string ToString(CString str) {
			return std::string(CT2A((LPCTSTR)str));
		}
		static std::wstring ToWString(std::string str) {
			return std::wstring(CA2W(str.c_str()));
		}
		__declspec(deprecated("** Don't use CString **")) static std::wstring ToWString(CString str) {
			return std::wstring((CT2W)(LPCTSTR)str);
		}
		__declspec(deprecated("** Don't use CString **")) static CString ToCString(std::string str) {
			//Has memory leak
			return CString(CA2CT(str.c_str()));
		}
		__declspec(deprecated("** Don't use CString **")) static CString ToCString(std::wstring str) {
			//Has memory leak
			return CString(CW2CT(str.c_str()));
		}
	};
}
#endif  //MSPRING_7E1_9_C_STRING_HPP_INCLUDED