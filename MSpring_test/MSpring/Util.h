/*
*  Util.hpp
*  MSpring
*
*  Created by kimbom on 2017. 9. 12...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_UTIL_HPP_INCLUDED)
#define MSPRING_7E1_9_C_UTIL_HPP_INCLUDED
#include<afxwin.h>
namespace mspring {
	template<typename T>
	T Min(const T& a, const T& b) {
		return a < b ? a : b;
	}
	template<typename T>
	T Max(const T& a, const T& b) {
		return a > b ? a : b;
	}
	template<typename T>
	void SetRange(T& val, const T& min_val, const T& max_val) {
		val = Min(val, max_val);
		val = Max(val, min_val);
	}
}
#endif  //MSPRING_7E1_9_C_UTIL_HPP_INCLUDED