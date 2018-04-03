/*
*  ErrorMessage.hpp
*  MSpring
*
*  Created by kimbom on 2017. 9. 12...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_ERRORMESSAGE_HPP_INCLUDED)
#define MSPRING_7E1_9_C_ERRORMESSAGE_HPP_INCLUDED
#include<WinUser.h>
#include<string>
#include<sstream>
#define MSPRING_ERROR(MSG)	__MSpring_Error(MSG,__FILE__,__FUNCTION__,__LINE__)
inline void __MSpring_Error(std::string msg, char* __file__, char* __function__, int __line__) {
	std::ostringstream oss;
	oss << "MSpring Error : (" << msg << ") in " << __function__ << " , " << __file__ << " , line" << __line__ << std::endl;
	if (MessageBoxA(nullptr, oss.str().c_str(), "SCVL Error", MB_RETRYCANCEL) == IDCANCEL) {
		exit(1);
	}
}
#endif  //MSPRING_7E1_9_C_ERRORMESSAGE_HPP_INCLUDED