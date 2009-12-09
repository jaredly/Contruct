// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

/*long ExtObject::cMyCondition(LPVAL params)
{
	// Always run
	return true;
}*/


long ExtObject::cOnStep(LPVAL theParams)
{
	return true;
}
long ExtObject::cOnHorizontalStep(LPVAL theParams)
{
	return true;
}
long ExtObject::cOnVerticalStep(LPVAL theParams)
{
	return true;
}