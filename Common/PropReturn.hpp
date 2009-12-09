// A data value in Construct.
#ifndef _PROPRETURN_HPP_
#define _PROPRETURN_HPP_

class PropReturn 
{
public:
	__int64			val64;		// 64 bit int
	int				val;		// 32 bit int
	double			fVal;		// floating point values
	void* 			CSPtr;		// Pointer to a CString for writebacks to Construct
	const char*		chstr;		// Pointer to const char for readings from Construct
};

#endif