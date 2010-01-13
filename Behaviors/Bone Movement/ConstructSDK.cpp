// ConstructSDK.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ConstructSDK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double lerp(double a, double b, float f)
{
	return (1-f) * a + (f) * b;
}