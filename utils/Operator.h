/*
*  Operator.hpp
*  MSpring
*
*  Created by kimbom on 2017. 9. 12...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(MSPRING_7E1_9_C_OPERATOR_HPP_INCLUDED)
#define MSPRING_7E1_9_C_OPERATOR_HPP_INCLUDED
#include<afxwin.h>
inline std::ostream& operator<<(std::ostream& os, const CPoint& point) {
	os << "(" << point.x << "," << point.y << ")";
	return os;
}
inline std::ostream& operator<<(std::ostream& os, const CRect& rect) {
	os << "[left : " << rect.left << "],[top : " << rect.top << "],[right : " << rect.right << "],[bottom : " << rect.bottom << "]";
	return os;
}
#define MSPRING_BEGIN_MESSAGE_MAP(theClass, baseClass) \
	PTM_WARNING_DISABLE \
	inline const AFX_MSGMAP* theClass::GetMessageMap() const \
		{ return GetThisMessageMap(); } \
	inline const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() \
	{ \
		typedef theClass ThisClass;						   \
		typedef baseClass TheBaseClass;					   \
		 static const AFX_MSGMAP_ENTRY _messageEntries[] =  \
		{

#define MSPRING_END_MESSAGE_MAP() \
		{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } \
	}; \
		static const AFX_MSGMAP messageMap = \
		{ &TheBaseClass::GetThisMessageMap, &_messageEntries[0] }; \
		return &messageMap; \
	}								  \
	PTM_WARNING_RESTORE

#define MSPRING_IMPLEMENT_DYNAMIC(class_name, base_class_name) \
	MSPRING_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL, NULL)

#define MSPRING_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew, class_init) \
	AFX_COMDAT const CRuntimeClass class_name::class##class_name = { \
		#class_name, sizeof(class class_name), wSchema, pfnNew, \
			RUNTIME_CLASS(base_class_name), NULL, class_init }; \
	inline CRuntimeClass* class_name::GetRuntimeClass() const \
		{ return RUNTIME_CLASS(class_name); }

#endif  //MSPRING_7E1_9_C_OPERATOR_HPP_INCLUDED