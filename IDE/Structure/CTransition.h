#pragma once

// CLayer class
class CTransition 
{
public:
	CString m_Name;
	CString m_FileName;

	// Effect parameter info
	vector<EffectParameter> params;

	void Init() {

		// Load file for inspection
		CPath path;
		path.SetToCurrentDirectory();
		CString curFile;
		curFile.Format("%sTransitions\\%s", path.GetFullPath(), m_FileName);
		CFile f(curFile, CFile::modeRead);
		char* strbuf = new char[f.GetLength()+1];
		f.Read(strbuf, f.GetLength());
		strbuf[f.GetLength()] = '\0';
		f.Close();

		// Find and parse parameters
		// Example: //#PARAM float amplitude 7 : Wave Amplitude : Description here
		char* paramPtr = strbuf;

		if (params.empty()) do {
			// Find a //#PARAM and advance to the beginning of param definition
			paramPtr = strstr(paramPtr, "//#PARAM");
			if (paramPtr == NULL) break;
			paramPtr += strlen("//#PARAM");

			params.push_back(EffectParameter());
			EffectParameter& p = params.back();

			// Extract this line
			CString curLine;
			AfxExtractSubString(curLine, paramPtr, 0, '\n');

			CString token;

			// Extract type part (eg 'float')
			AfxExtractSubString(token, curLine, 1, ' ');

			if (token == "float")
				p.type = 5;
			else if (token == "percent" || token == "percentage")
				p.type = 9;

			// Extract script name part (eg 'amplitude')
			AfxExtractSubString(p.variableName, curLine, 2, ' ');

			// Extract default value part (eg '7')
			AfxExtractSubString(p.value, curLine, 3, ' ');

			// Extract display name (eg Wave Amplitude)
			AfxExtractSubString(p.name, curLine, 1, ':');
			p.name.Trim();

			// Extract description
			AfxExtractSubString(p.description, curLine, 2, ':');

			//CString msg = "Parsed parameter from '";
			//msg += curLine + "':\ntype='" + token + "'\nvar='" + p.variableName + "'\ndefault='" + p.value + "'\nname='" + p.name + "'";
			//MessageBox(NULL, msg, "Param", MB_OK);
		}
		while (paramPtr != NULL);

		delete[] strbuf;

	}

	// Compare to CString by m_Name
	bool operator==(const CString& other) const {
		CString l = m_Name;
		CString r = other;

		l.MakeLower();
		r.MakeLower();

		return l == r;
	}

	bool Serialize(CArchive& ar) 
	{
		if (ar.IsLoading())
		{
			ar >> m_Name >> m_FileName;

			int count;
			ar >> count;
			params.resize(0);

			for (int i = 0; i < count; i++) {
				EffectParameter p;
				ar >> p.type >> p.variableName >> p.value >> p.name >> p.description;
				params.push_back(p);
			}
		}

		else 
		{
			ar << m_Name << m_FileName;

			ar << params.size();

			vector<EffectParameter>::iterator i = params.begin();

			for ( ; i != params.end(); i++)
				ar << i->type << i->variableName << i->value << i->name << i->description;
		}

		return true;
	}
};