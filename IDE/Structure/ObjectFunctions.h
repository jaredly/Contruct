// ObjectFunctions.h
// Useful common functions

#ifndef OBJFUNCTIONS_H
#define OBJFUNCTIONS_H

typedef int (__cdecl *GENERICCOMPAREFN)(const void * elem1, const void * elem2);
typedef int (__cdecl *STRINGCOMPAREFN)(const CString * elem1, const CString * elem2);

class CSortStringArray : public CStringArray
{
public:
	void Sort(STRINGCOMPAREFN pfnCompare = Compare);

protected:
	static int __cdecl Compare(const CString * pstr1, const CString * pstr2);

};
#include "structure.h"

#endif