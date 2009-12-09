#pragma once

class CBehavior
{
public:

	int     BehaviorDLLIndex;
	CString Text;		// Name

	int id;			// Unique ID

	OINFO* oInfo;


	// Serialized buffer for this movement
	BYTE* eData;
	int eSize;

	// Ctors
	CBehavior() : eData(NULL), eSize(0) {}
	~CBehavior() { if (eData) { delete[] eData; eData = NULL; } }

	// Creates a default movement eData
	void CreateDefault(CLayout* level);

	// Copy ctor (for vector reallocs)
	CBehavior(const CBehavior& m) {
		BehaviorDLLIndex = m.BehaviorDLLIndex;
		Text = m.Text;
		oInfo = m.oInfo;
		eSize = m.eSize;
		// Alloc and copy eData
		if (m.eData) {
			eData = new BYTE[eSize];
			memcpy(eData, m.eData, eSize);
		}
		else
			eData = NULL;
	}

	bool Serialize(CArchive& ar)
	{
		if (ar.IsLoading())
		{
			CString name;
			ar >>  name >> id;
			OINFO* o = GetOINFO(name);
			if(!o)
			{
				CErrorDlg Error;
				CString Missing;
				Missing.Format("You require the behavior '%s' to open this file.", name);
				Error.Error("Unable to find behavior", Missing);

				return false;

				BehaviorDLLIndex = 0;
			}

			else
				BehaviorDLLIndex = o->oID;

			ar >>  Text >> eSize;

			if (eData) delete[] eData;

			eData = new BYTE[eSize];

			ar.CArchive::Read(eData, eSize);

			// Update oInfo
			oInfo = GetOINFO(BehaviorDLLIndex);
		}
		else
		{
			OINFO* o = GetOINFO(BehaviorDLLIndex);
			CString CStr = o->extName;

			ar << CStr << id << Text << eSize;
			ar.Write(eData, eSize);
		}

		return true;
	}

	// If compared to string, use Text member
	bool operator==(const CString other) const {
		CString l = Text;
		CString r = other;

		l.MakeLower();
		r.MakeLower();

		return l == r;
	}
};