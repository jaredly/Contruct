// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aVibrate(LPVAL params)
{
	CXBOXController* player = GetPlayer(params);
	if(!player) return 0; // error

	int combovib = params[1].GetInt();
	float strength = params[2].GetFloat();
	float duration = params[3].GetFloat();
	int combomethod = params[4].GetInt();

	if(combovib != 1) // not right
	{
		player->leftVib.time = 0;
		player->leftVib.total_time = duration;
		player->leftVib.method = combomethod;
		player->leftVib.strength = strength;
	}
	if(combovib != 0) // not left
	{
		player->rightVib.time = 0;
		player->rightVib.total_time = duration;
		player->rightVib.method = combomethod;
		player->rightVib.strength = strength;
	}


	// Do nothing
	return 0;
}
