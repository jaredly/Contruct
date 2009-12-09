#include "StdAfx.h"
#include "ParamScintilla.h"
#include "..\Structure\SystemObject.h"
#include "..\Utilities\Helper.h"

struct SysExpIntellisenseEntry {
	CString expname;
	CString params;		// comma delimited
};

SysExpIntellisenseEntry SysExpIntellisenseTable[] = {

	{"LoopIndex", "Loop name [string]"},
	{"LoopIndex", ""},
	{"LoopLength", "Loop name [string]"},
	{"LoopLength", ""},
	{"Max", "Value 1 [number], Value 2 [number] ..."},
	{"Min", "Value 1 [number], Value 2 [number] ..."},
	{"Global", "Variable name eg. 'MyVariable'"},
	{"FormatDecimal", "Value [number], Number of decimal places [number]"},
	{"ZeroPad",	"Value [number], Number of digits [number]"},
	{"Left", "String, Number of characters"},
	{"Right", "String, Number of characters"},
	{"Mid", "String, Start position, Number of characters"},
	{"Uppercase", "String"},
	{"Lowercase", "String"},
	{"Find", "String, String to find"},
	{"Find", "String, String to find, Start position [number]"},
	{"Remove", "String, String to remove"},
	{"Replace", "String, String to find, String to replace"},
	{"Trim",	"String"},
	{"Reverse",	"String"},
	{"NumTokens", "String, Separator [string]"},
	{"NumTokens", "String"},
	{"GetToken", "String, Index [number], Seperator [string]"},
	{"GetToken", "String, Index [number]"},
	{"Distance", "x1 [number], y1 [number], x2 [number], y2 [number]"},
	{"Distance", "xy1 [array], xy2 [array]"},
	{"Angle", "x1 [number], y1 [number], x2 [number], y2 [number]"},
	{"Angle", "xy1 [array], xy2 [array]"},
	{"CountMatching", "Object name [string]"},
	{"CountMatching", "Object OID [number]"},
	{"RGB", "Red value [number], Green value [number], Blue value [number]"},
	{"GetRed", "RGB value [number]"},
	{"GetGreen", "RGB value [number]"},
	{"GetBlue", "RGB value [number]"},
	{"Python", "Python script [string]"},
	{"lerp", "a [number], b [number], x [number]"},
	{"str", "Number to convert to string"},
	{"int",	"String or float to convert"},
	{"float", "String or int to convert"},
	{"random", "Integer or float maximum value"},
	{"len",	"String to get length of"},
	{"round", "Number"},
	{"LayerZoomX", "Layer name or number"},
	{"LayerZoomY", "Layer name or number"},
	{"LayerScrollRateX", "Layer name or number"},
	{"LayerScrollRateY", "Layer name or number"},
	{"LayerOpacity", "Layer name or number"},
	{"LayerName", "Layer number"},
	{"Clamp", "Value, Lower bound, Upper bound"},
	{"LayerAngle", "Layer name or number"},
	{"LinearAim", "Bullet X, Bullet Y, Bullet speed, Target X, Target Y, Target Speed, Target Angle"},
	{"Cosp", "a [number], b [number], x [number]"},
	{"AngleDiff", "First angle, second angle"},
	{"RotateAngle",	"start angle, end angle, step"},
	{"NormalRandom", "mean, sigma"},
	{"Cubic", "a, b, c, d, x"}
};

void CParamScintilla::OnChar(NMHDR* pNMHDR, LRESULT* pResult,CScintillaWnd& scintWin, CObjTypeMap* objectMap,
		CParamTooltipCtrl& m_Tooltip,
		bool& m_ParameterTooltipIsVisible,
		bool& m_QuotesOpen,
		CApplication* pApp,
		bool bInline)
{
	// This function is only for CScintilla...
	SCNotification *scn = (SCNotification*)pNMHDR;

	CString text;
	scintWin.GetWindowText(text);

	CString theChar = "a";
	theChar.SetAt(0, scn->ch);

	::SendMessage(scintWin.m_hWnd, SCI_AUTOCSETSEPARATOR, '|', '|');

	int carretPosition = scintWin.GetCurrentPosition();
	text = text.Left(carretPosition); // everything to the right is irrelavant

	// Get last char
	if (scn->ch=='.')
	{
		// Get name of object
		// First we need to get the current char.

		text = text.Left(scintWin.GetCurrentPosition());

		int pos = 0;
		char finds[] = " ()+-/*{},";
		for(int a = 0; a < strlen(finds); a++)
			pos = max(pos, text.ReverseFind(finds[a]));
		
		CString name = text.Right(text.GetLength() - pos);
		name = name.Left(name.GetLength() - 1);
		if(pos!=0)
			name = name.Right(name.GetLength()-1);
		
		bool exists = true;
		{		
			pApp->m_sort.RemoveAll();
			CString intel;

			if (name != "System")
			{
				// Display intellisense
				// Loop m_TypeCheckerressions
				CObjType* oT;

		
				//POSITION pos = objectMap->GetStartPosition();
				//long nKey;

				//while (pos != NULL) 
				vector<CObjType*> objects;
				pApp->GetObjectTypes(objects);
				pApp->GetFamilyObjectTypes(objects);
				vector<CObjType*>::iterator i = objects.begin();
				for(; i != objects.end(); i++)
				{
					oT = *i;
					//objectMap->GetNextAssoc(pos, nKey, oT);

					CString l = oT->GetName();
					CString r = name;
					l.MakeLower();
					r.MakeLower();
	
					if (l == r)
					{	
						// Enum m_TypeCheckerressions
						for (int i = 0; i < oT->GetTableCount(2); i++)
						{
							ACESEntry2* acesEntry = oT->GetACESEntry(2, i);
							if(acesEntry->aceDisplayText != "")
								pApp->m_sort.Add(acesEntry->aceDisplayText);
						}

						break;
					}
				}	
			}

			else // System
			{
				for (int i = 0; i < GetSystemTableCount(2); i++)
				{
					ACESEntry2* ACE;
					GetSystemExpression(i, ACE);
					pApp->m_sort.Add(ACE->aceDisplayText);
				}
			}
							
			pApp->m_sort.Sort();

			for (int i = 0; i < pApp->m_sort.GetSize(); i++)
			{
				intel += pApp->m_sort.GetAt(i);				
				intel += "|";
			}
			
			intel = intel.Left(intel.GetLength() - 1);
			
			if (intel != "")
				::SendMessage(scintWin.m_hWnd, SCI_AUTOCSHOW, 0, (long)(const char*)intel);
		}
	}

	// Add: Check for inline editing, don't handle this if we are. 0.96.2
	else if (scn->ch == '(' && !bInline)
	{
		if (m_ParameterTooltipIsVisible)
		{
			m_Tooltip.ShowWindow(SW_HIDE);
			m_ParameterTooltipIsVisible = false;
		}

		CString m_TypeCheckerressionName, ObjectName;

		// Get name of m_TypeCheckerression
		int pos = text.ReverseFind('.');

		CString name = text.Right(text.GetLength() - pos - 1);
		int start = (text.GetLength() - pos) - 1;
		m_TypeCheckerressionName = name.Left(name.GetLength() - 1);

		// Get name of object
		CString object = text.Left(text.GetLength() - start);
		pos = object.ReverseFind(' ');

		name = object.Right(object.GetLength() - pos);
		ObjectName = name.Left(name.GetLength() - 1);
		ObjectName.Trim();

		bool exists = true;
		bool alsoexists = false;

		if (ObjectName == "")
			ObjectName = "System";

		if (exists)
		{
			vector<CString> setParams;
			m_Tooltip.FlushMethods();

			if (ObjectName != "System")
			{
				CObjType* oT;
		
				POSITION pos = objectMap->GetStartPosition();
				long nKey;

				while (pos != NULL) 
				{
					objectMap->GetNextAssoc(pos, nKey, oT);

					CString l = oT->GetName();
					CString r = ObjectName;
					l.MakeLower();
					r.MakeLower();
	
					if (l == r)
					{	
						// Enum m_TypeCheckerressions
						for (int i = 0; i < oT->GetTableCount(2); i++)
						{
							ACESEntry2* acesEntry = oT->GetACESEntry(2, i);
							if (acesEntry->aceDisplayText == m_TypeCheckerressionName)
							{
								alsoexists = true;

								for (int x = 0; x < acesEntry->params.size();x++)
								{
									CString addParameter;

									// Check type
									if (acesEntry->params[x].type == 1)
										addParameter.Format("%s [number]", acesEntry->params[x].name);

									if (acesEntry->params[x].type == 2)
										addParameter.Format("%s [string]", acesEntry->params[x].name);

									setParams.push_back(addParameter);
								}
							}	
						}

						break;
					}
				}	
			}

			/*else
			{
				int numTableEntries = sizeof(SysExpIntellisenseTable) / sizeof(SysExpIntellisenseTable[0]);
				setParams.resize(0);

				for (int i = 0; i < numTableEntries; i++) {

					CString l = m_TypeCheckerressionName;
					CString r = SysExpIntellisenseTable[i].expname;

					l.MakeLower();
					r.MakeLower();

					if (l == r) {
						alsoexists = true;
						
						CString curParam;
						int paramNum = 0;

						do {
							AfxExtractSubString(curParam, SysExpIntellisenseTable[i].params, paramNum, ',');
							curParam.Trim();
							if (curParam != "")
								setParams.push_back(curParam);
							paramNum++;
						} while (curParam != "");

						m_Tooltip.AddMethod(m_TypeCheckerressionName, setParams);
						setParams.resize(0);
					}
				}
			}*/

			if (alsoexists && !m_ParameterTooltipIsVisible)
			{
				// Get position of caret
				int currentPosition = scintWin.SendMessage(SCI_GETCURRENTPOS, 0, 0);

				int caretX = scintWin.SendMessage(SCI_POINTXFROMPOSITION, 0, 0);
				int caretY = scintWin.SendMessage(SCI_POINTXFROMPOSITION, 0, 0);

				if (ObjectName != "System")
				{
					CString sMethod;
					m_Tooltip.AddMethod(m_TypeCheckerressionName, setParams);
				}

				m_Tooltip.SetCurMethod(0);
				m_Tooltip.SetCurParam(0);
				m_Tooltip.ShowTooltip(CPoint(caretX, caretY));

				m_ParameterTooltipIsVisible = true;
				m_QuotesOpen = false;
			}
		}
	}

	// Increment parameter count
	else if (scn->ch == ',' && !bInline)
	{
		//scintWin.SendMessage(SCI_AUTOCCOMPLETE, 0, 0);
		if (m_ParameterTooltipIsVisible && !m_QuotesOpen)
			m_Tooltip.ShowNextParam();
	}

	// Hide parameter tooltip
	else if (scn->ch == ')' && !bInline)
	{
		//scintWin.SendMessage(SCI_AUTOCCOMPLETE, 0, 0);
		if (m_ParameterTooltipIsVisible)
		{
			m_Tooltip.ShowWindow(SW_HIDE);
			m_ParameterTooltipIsVisible = false;
		}
	}
	// Hide parameter tooltip
	else if (scn->ch == '"')
	{
		if (m_QuotesOpen)
			m_QuotesOpen = false;
		else
			m_QuotesOpen = true;
	}

	// Prompt variable names
	else if (scn->ch == '\'')
	{
		// Trim the right two (' chars eg. Sprite('
		text = text.Left(text.GetLength() - 2);

		CString temp = text;
		temp.MakeReverse();
		int index = temp.FindOneOf(" {}()+-*/");

		if (index > -1)
			text = text.Right(index);

		// Now find if there's a dot to get the object name
		index = text.Find('.');

		if (index > -1)
			text = text.Left(index);

		CObjType* pType = GetTypeFromName(pApp, text);

		if (pType != NULL) {

			CString varnames;

			vector<PrivateValue>::iterator i = pType->m_PrivateValues.begin();
			vector<PrivateValue>::iterator end = pType->m_PrivateValues.end();

			for ( ; i != end; i++)
				varnames += i->name + "|";

			varnames = varnames.Left(varnames.GetLength() - 1);

			if (varnames != "")
				::SendMessage(scintWin.m_hWnd, SCI_AUTOCSHOW, 0, (long)(const char*)varnames);
		}
		else{
			if(text == "global")
			{

			CString varnames;

			list<CApplication::GlobalVariable>::iterator i = pApp->global_variables.begin();
			list<CApplication::GlobalVariable>::iterator end = pApp->global_variables.end();

			for ( ; i != end; i++)
				varnames += i->name + "|";

			varnames = varnames.Left(varnames.GetLength() - 1);

			if (varnames != "")
				::SendMessage(scintWin.m_hWnd, SCI_AUTOCSHOW, 0, (long)(const char*)varnames);

			}

		}
	}
	/*else
	{
		
		if(text.GetLength() == 2)
		{
		// Any other type of letter, display intellisense for system expressions and object names
		CString suggestions;

		int numTableEntries = sizeof(SysExpIntellisenseTable) / sizeof(SysExpIntellisenseTable[0]);
		for (int i = 0; i < numTableEntries; i++) {
			CString expression = SysExpIntellisenseTable[i].expname;
			suggestions += expression + "|";
		}
		suggestions = suggestions.Left(suggestions.GetLength() - 1);
		if (suggestions != "")
			::SendMessage(scintWin.m_hWnd, SCI_AUTOCSHOW, 0, (long)(const char*)suggestions);
		}


	}*/
}