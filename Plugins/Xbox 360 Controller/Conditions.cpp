// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cButtonDown(LPVAL params)
{
	CXBOXController* player = GetPlayer(params);
	if(!player) return 0; // error

	return player->ButtonState((XBoxControl::Control)params[1].GetInt()) > 0.5;
}
