// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

CXBOXController* ExtObject::GetPlayer(LPVAL params)
{
	switch (params[0].GetInt())
	{
		case 1:
			return this->Player[0];
		case 2:
			return this->Player[1];
		case 3:
			return this->Player[2];
		case 4:
			return this->Player[2];
		default:
			return NULL;
	}
}
#endif

void EndProperties(int iMode, CVirtualPropItem* item, MicroEdittime* pm)
{
	if (iMode == MODE_INIT) {
		ASSIGN_CSTRING(item->Description, "");
		ASSIGN_CSTRING(item->Label, "");
		ASSIGN_CSTRING(item->Text, "");
		item->Type = -1;	// end
		pm->Proc(item);		// post end to property vector
	}
}

string operator+ (string& s, int& i)
{
	std::stringstream out;
	out << i;
	s.append(out.str());
	return s;
}

bin& operator<< (bin& b, const Control& c)
{
	CString controlstring = c.control.c_str();
	b << c.c;
	b << controlstring;
	b << c.player;
	return b;
}

bin& operator>> (bin& b, Control& c)
{
	CString controlstring;
	b >> c.c;
	b >> controlstring;
	b >> c.player;

	c.control = controlstring;
	return b;
}