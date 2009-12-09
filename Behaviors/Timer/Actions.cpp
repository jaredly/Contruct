// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aStartTimer(LPVAL params)
{
	CString name = params[0].GetString();
	int length = params[1].GetInt();
	bool repeat = params[2].GetBool();
	bool destroy = params[3].GetBool();

	ObjectTimer new_timer;
	new_timer.name = name;
	new_timer.repeat = repeat;
	new_timer.time = pRuntime->GetLayoutClock(pLayout) + length;
	new_timer.initial_length = length;
	new_timer.destroy = destroy;

	timers.push_back(new_timer);

	return 0;
}

long ExtObject::aStopTimer(LPVAL params)
{
	CString name = params[0].GetString();
	name.MakeLower();
	
	vector<ObjectTimer>::iterator i = timers.begin();

	for ( ; i != timers.end(); ) 
	{
		CString lower = name;
		lower.MakeLower();

		if (lower == name)
		{
			timers.erase(i);
			return 0;
		}
		else
			i++;
	}

	return 0;
}
