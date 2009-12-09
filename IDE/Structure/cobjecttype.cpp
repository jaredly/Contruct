#include "StdAfx.h"
#include "CObjectType.h"
#include "SystemObject.h"

#include "..\MainFrm.h"
extern CMainFrame* pMainWnd;

CObjType::~CObjType()
{
}

bool CObjType::Serialize(CArchive& ar) 
{
	CString ExpectedName = "CObjType";
	int     Version      = 4;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	if(ar.IsLoading())
	{
		ar >> m_Name >> ObjectIdentifier;
	
		CString name = "";
		ar >> name;
		m_bIsGroupType = false;

		if(ObjectIdentifier == -1)
		{
			DLLIndex = -1;
		}

		else
		{
			OINFO* o = GetOINFO(name);
			if(o)
				DLLIndex = o->oID;
			else
			{
				DLLIndex = -1;
				m_bIsGroupType = true;
			}
		}

		SerializeBitmap(m_Image, ar);
		SerializeBitmap(small_image, ar);

		behaviors.clear();
		effects.clear();
		m_PrivateValues.clear();
		traits.clear();

		// Load Behaviors
		int count = 0;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			CBehavior blank;
			behaviors.push_back(blank);
			
			if (!behaviors.back().Serialize(ar))
				return false;
		}

		// Load effects
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			effects.push_back(CEffect());
			
			if (!effects.back().Serialize(ar))
				return false;
		}

		// Load Traits
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			CString mystring;
			ar >> mystring;
			traits.push_back(mystring);
		}

		// Load private values
		ar >> count;
		ar >> VariableID;
		for (int i = 0; i < count; i++)
		{
			CString name;
			int type;
			int index;
			ar >> name;
			ar >> type;
			ar >> index;
			m_PrivateValues.push_back(PrivateValue(name, type, index));
		}

		// Global object
		ar >> m_bGlobal;

		int temp;
		ar >> temp;
		destroyWhen = (DisableShaderWhen)temp;

		if (Version >= 2)
			ar >> m_bShowInEventSheetEditor;
		else
			m_bShowInEventSheetEditor = true;

		if(Version >= 3)
		{
			ar >> category;
		}

		if (Version >= 4)
			ar >> folder;
	}

	else
	{
		CString name;
			
		ar << m_Name << ObjectIdentifier;

		OINFO* o = GetOINFO(DLLIndex);
		CString CStr = o->extName;

		ar << CStr;

		SerializeBitmap(m_Image, ar);
		SerializeBitmap(small_image, ar);

		// Save behaviors
		ar << behaviors.size();

		BehaviorIterator i = behaviors.begin();
		BehaviorConstIterator movs_end = behaviors.end();

		for ( ; i != movs_end; i++)
			i->Serialize(ar);

		// Save effects
		ar << effects.size();

		EffectIterator e = effects.begin();
		EffectConstIterator fx_end = effects.end();

		for ( ; e != fx_end; e++)
			e->Serialize(ar);

		// Save Traits
		int count = traits.size();
		ar << count;
		for (int i = 0; i < count; i++)
		{
			CString mystring;
			ar << traits[i];
		}

		// Save private values
		ar << m_PrivateValues.size();
		ar << VariableID;
		for (int i = 0; i < m_PrivateValues.size(); i++)
		{
			ar << m_PrivateValues[i].name;
			ar << m_PrivateValues[i].type;
			ar << m_PrivateValues[i].identifier;
		}

		// Global object
		ar << m_bGlobal;
		ar << (int)destroyWhen;

		// Show in editor
		ar << m_bShowInEventSheetEditor;

		ar << category;

		// object folders
		ar << folder;
	}

	SerializeClassPadding(5, 15, ar);

	return true;
}

CObjType* LookupObjectType(CString objName, CObjTypeMap* objMap)
{
	CObjType* oT;

	objName.MakeLower();

	POSITION pos = objMap->GetStartPosition();
	long mapKey;

	while (pos != NULL) 
	{
		objMap->GetNextAssoc(pos, mapKey, oT);
		CString Name = oT->GetName();
		Name.MakeLower();

		if (Name == objName)
			return oT;
	}

	return NULL;
}

CObjType* LookupObjectType(int oid, CObjTypeMap* objMap)
{
	CObjType* oT;

	POSITION pos = objMap->GetStartPosition();
	long mapKey;

	// Loop through and add the objects
	while (pos != NULL) 
	{
		objMap->GetNextAssoc(pos, mapKey, oT);
		
		if (oT->ObjectIdentifier == oid)
			return oT;
	}

	return NULL;
}

CBehavior* LookupBehavior(CObjType* pType, CString naem)
{
	if (pType == NULL) return NULL;

	naem.MakeLower();
	list<CBehavior>::iterator i = pType->behaviors.begin();

	for ( ; i != pType->behaviors.end(); i++) {
		CString curName = i->Text;
		curName.MakeLower();

		if (naem == curName)
			return &(*i);
	}

	return NULL;
}



PrivateValue* CObjType::GetPrivateVariable(int id)
{
	for(vector<PrivateValue>::iterator i = m_PrivateValues.begin(); i!= m_PrivateValues.end(); i++)
	{
		if(i->identifier == id)
			return &*i;
	}
	return NULL;
}

PrivateValue* CObjType::GetPrivateVariable(CString varName)
{
	varName.MakeLower();

	for(vector<PrivateValue>::iterator i = m_PrivateValues.begin(); i!= m_PrivateValues.end(); i++)
	{
		CString otherName = i->name;
		otherName.MakeLower();

		if (varName == otherName)
			return &(*i);
	}
	return NULL;
}

bool CObjType::PrivateVariableExists(CString varName)
{
	return exists_element(m_PrivateValues.begin(), m_PrivateValues.end(), varName);
}


CBehavior* CObjType::GetBehaviorByIndex(int id)
{
	list<CBehavior>::iterator i = behaviors.begin();
	if(id >= behaviors.size())
		return false;

	advance(i, id);
	return &*i;
}

CBehavior* CObjType::GetBehaviorByUniqueID(int id)
{
	list<CBehavior>::iterator i = behaviors.begin();
	list<CBehavior>::iterator end = behaviors.end();

	for ( ; i != end; i++) {
		if (i->id == id)
			return &*i;
	}

	return NULL;
}

int CObjType::GetBehaviorIndexFromUniqueID(int id)
{
	list<CBehavior>::iterator i = behaviors.begin();
	list<CBehavior>::iterator end = behaviors.end();

	int index = 0;
	for ( ; i != end; i++) 
	{
		if (i->id == id)
			return index;
		index ++;
	}

	return 0;
}
CBehavior* CObjType::GetBehaviorByName(CString name)
{
	list<CBehavior>::iterator i = behaviors.begin();
	list<CBehavior>::iterator end = behaviors.end();

	name.MakeLower();

	for ( ; i != end; i++) {

		CString curName = i->Text;
		curName.MakeLower();

		if (curName == name)
			return &*i;
	}

	return NULL;
}

CEffect* CObjType::GetEffect(int id)
{
	for(list<CEffect>::iterator i = effects.begin(); i!= effects.end(); i++)
	{
		if(i->m_effectID == id)
			return &*i;
	}
	return NULL;
}

CEffect* CObjType::GetEffectByIndex(int index)
{
	list<CEffect>::iterator i = effects.begin();
	advance(i, index);
	return &*i;
}

void CObjType::CreateFromBehavior(CBehavior* pMov)
{
	SetName(pMov->Text);
	ObjectIdentifier = pMov->id;
	DLLIndex  = pMov->BehaviorDLLIndex;
	m_pBehavior = pMov;
}

void CObjType::CreateFromEffect(CEffect* pEff)
{
	SetName(pEff->Text);
	ObjectIdentifier = pEff->m_effectID;
	DLLIndex  = -2;
	m_pEffect = pEff;
}

ACESEntry* CEffect::GetACESEntry(int type, int index)
{
	temp.params.clear();
	temp.resourceID = index;
	temp.retrn = 0;

	if(index == 0)
	{
		temp.aceDisplayText.Format("Activate %s", Text);	
		temp.aceListName.Format("Activate %s", Text);
		temp.aceCategory.Format("Enabled");
	}
	else if(index ==1)
	{
		temp.aceDisplayText.Format("Deactivate %s", Text);	
		temp.aceListName.Format("Deactivate %s", Text);
		temp.aceCategory.Format("Enabled");
	}
	else
	{
		if(index - 2 < 0 || index - 2 >= params.size())
			return 0;

		CString pName = params[index-2].name;
		temp.aceListName.Format("Set %s", pName);
		temp.aceDisplayText.Format("%s : Set %s to %%0", Text, pName);

		PARAM2 param2;
		param2.desc = "Enter value";
		param2.name = "Enter value";
		param2.type = 1;

		temp.params.push_back(param2);
		temp.aceCategory.Format("Properties");
	}
	return &temp;
}

// Load params vector by parsing the HLSL script
bool CEffect::Init(CLayoutEditor* pFrame) {

	// Load file for inspection
	CPath path;
	path.SetToCurrentDirectory();
	CString curFile;
	curFile.Format("%sEffects\\%s", path.GetFullPath(), effectFile);

	if (!FileExists(curFile))
	{
		CString Error;
		Error.Format("The effect file '%s' is required to open this file. Please locate it and put it in your \\Effects\\ directory under your Construct installation.", effectFile);

		CErrorDlg Dlg;
		Dlg.Error("Effect missing", Error);
		return false;
	}

	CFile f(curFile, CFile::modeRead);
	char* strbuf = new char[f.GetLength()+1];
	f.Read(strbuf, f.GetLength());
	strbuf[f.GetLength()] = '\0';
	f.Close();

	// Find and parse parameters
	// Example: //#PARAM float amplitude 7 : Wave Amplitude
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

	// Initialise the runtime effect
	ID3DXBuffer* errors;

	(*pFrame->m_display)->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 
	(*pFrame->m_display)->EndBatch();
	HRESULT hr = D3DXCreateEffectFromFile((*(pFrame->m_display))->GetD3DDevice(), curFile, NULL, NULL, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, NULL, &(runEffect.pEffect), &errors);

	if (FAILED(hr) && errors)
		throw runtime_error((const char*)errors->GetBufferPointer());

	runEffect.name = effectFile;
	runEffect.crossSampling = strstr(strbuf, "//#CROSS-SAMPLING") != NULL;
	runEffect.borderMode = strstr(strbuf, "//#BORDER-MODE") != NULL;

	ID3DXEffect* pEffect = runEffect.pEffect;

	D3DXHANDLE technique = pEffect->GetTechniqueByName("ConstructEffect");

	if (technique == NULL)
		throw runtime_error("No valid effect technique detected.  Your computer does not appear to support this effect.");
	pEffect->SetTechnique(technique);
	runEffect.vForegroundTexture = pEffect->GetParameterByName(NULL, "ForegroundTexture");
	runEffect.vBackgroundTexture = pEffect->GetParameterByName(NULL, "BackgroundTexture");
	runEffect.vSourceTexture = pEffect->GetParameterByName(NULL, "SourceTexture");
	runEffect.vPreviousTexture = pEffect->GetParameterByName(NULL, "PreviousTexture");
	runEffect.vFrameCounter = pEffect->GetParameterByName(NULL, "frameCounter");
	runEffect.vBoxLeft = pEffect->GetParameterByName(NULL, "boxLeft");
	runEffect.vBoxTop = pEffect->GetParameterByName(NULL, "boxTop");
	runEffect.vBoxRight = pEffect->GetParameterByName(NULL, "boxRight");
	runEffect.vBoxBottom = pEffect->GetParameterByName(NULL, "boxBottom");
	runEffect.vBoxWidth = pEffect->GetParameterByName(NULL, "boxWidth");
	runEffect.vBoxHeight = pEffect->GetParameterByName(NULL, "boxHeight");
	runEffect.vHotspotX = pEffect->GetParameterByName(NULL, "hotspotX");
	runEffect.vHotspotY = pEffect->GetParameterByName(NULL, "hotspotY");
	runEffect.vPixelWidth = pEffect->GetParameterByName(NULL, "pixelWidth");
	runEffect.vPixelHeight = pEffect->GetParameterByName(NULL, "pixelHeight");
	runEffect.vBgStart = pEffect->GetParameterByName(NULL, "bgStart");
	runEffect.vBgEnd = pEffect->GetParameterByName(NULL, "bgEnd");

	// Add runtime effect parameters if none added yet
	if (runEffect.params.empty()) {
		vector<EffectParameter>::iterator i = params.begin();
		vector<EffectParameter>::const_iterator params_end = params.end();
		int index = 0;

		for ( ; i != params_end; i++, index++) {
			runEffect.params.push_back(CRunEffectParameter());
			CRunEffectParameter& runParam = runEffect.params.back();

			runParam.handle = pEffect->GetParameterByName(NULL, params[index].variableName);

			switch(params[index].type) {
				// Float
				case 5:
				// Percent
				case 9:
					pEffect->SetFloat(runParam.handle, atof(params[index].value));
					break;
			}
		}
	}

	// Tidy
	delete[] strbuf;

	(*pFrame->m_display)->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); 
	initialised = true;
	return true;
}

vector<ACESEntry2>* CObjType::GetTable(int table)
{
	OINFO *oInfo;
	POSITION pos = g_dlls->GetStartPosition();
	DWORD nKey;

	while (pos != NULL) 
	{
		g_dlls->GetNextAssoc(pos, nKey, oInfo);

		// Got it
		if (oInfo->oID == DLLIndex)
		{
			return oInfo->GetTable(table);
		}
	}

	return 0;
}

// Get a table entry
ACESEntry2* CObjType::GetACESEntry(int table, long index)
{
	if(index >= 0)
	{
		OINFO *oInfo;
		POSITION pos = g_dlls->GetStartPosition();
		DWORD nKey;

		// Find the DLL
		if(DLLIndex == -1)
		{
			ACESEntry2* TemporySystemAce;
			switch( table)
			{
			case ACTION:	
				GetSystemAction(index, TemporySystemAce);
				return TemporySystemAce;
			case CONDITION:
				GetSystemCondition(index, TemporySystemAce);
				return TemporySystemAce;
			case EXPRESSION:
				GetSystemExpression(index, TemporySystemAce);
				return TemporySystemAce;
			}

			return 0;
		}
		else if(DLLIndex == -2)
		{
			if(m_pEffect)
				return m_pEffect->GetACESEntry(table, index);
		}
		else
		{
			while (pos != NULL) 
			{
				g_dlls->GetNextAssoc(pos, nKey, oInfo);

				// Got it
				if (oInfo->oID == DLLIndex)
				{
					return oInfo->GetACEEntry(table, index);
				}
			}
			return 0;
		}
	}

	return NULL;
}

ACESEntry2* CObjType::GetACESEntry(int table, MenuMapItem& ChosenItem, int& i)
{
	CObjType* objectType = this;

	// Get number of actions
	int numberOfItems = 0;

	if(ChosenItem.type ==0) // ace
		numberOfItems = objectType->GetTableCount(table);
	if(ChosenItem.type == 1) // Behavior
	{
		CBehavior* mi = objectType->GetBehaviorByUniqueID(ChosenItem.child);
		
		CObjType mov;
		mov.DLLIndex = mi->BehaviorDLLIndex;

		numberOfItems  = mov.GetTableCount(table);
	}
	if(ChosenItem.type == 2) // effect
	{
		for( EffectIterator ei = objectType->effects.begin(); ei!= objectType->effects.end(); ei++)
		{
			if(ei->m_effectID == ChosenItem.child)
			{
				numberOfItems = 2 + ei->params.size();
			}
		}	
	}
	// Loop them
	for (i = 0; i < numberOfItems; i++)
	{
		// Get the entry
		ACESEntry2* acesEntry = 0;
		if(ChosenItem.type ==0) // ace
			acesEntry = objectType->GetACESEntry(table, i);
		if(ChosenItem.type == 1) // Behavior
		{
			CBehavior* mi = objectType->GetBehaviorByUniqueID(ChosenItem.child);
			acesEntry = GetOINFO(mi->BehaviorDLLIndex)->GetACEEntry(table,i);
		}
		if(ChosenItem.type == 2) // effect
		{
			for( EffectIterator ei = objectType->effects.begin(); ei!= objectType->effects.end(); ei++)
			{
				if(ei->m_effectID == ChosenItem.child)
				{
					acesEntry = ei->GetACESEntry(table, i);
				}
			}	
		}
		if (acesEntry->resourceID == ChosenItem.menuid)
		{
			return acesEntry;
		}
	}
	return 0;
}

CString CObjType::GetUniqueEffectName(CString startName)
{
	if (!exists_element(effects.begin(), effects.end(), startName))
		return startName;

	CString altName;
	int count = 2;

	do {
		altName.Format("%s%d", startName, count);
	} while (exists_element(effects.begin(), effects.end(), altName));

	return altName;
}

CString CObjType::GetUniqueBehaviorName(CString startName)
{
	CString filteredName;

	for (int i = 0; i < startName.GetLength(); i++) {
		// Allowed characters: a-z, A-Z, 0-9, _
		char c = startName[i];

		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')
			filteredName += startName[i];
	}

	startName = filteredName;

	if (startName == "")
		startName = "MyBehavior";

	if (!exists_element(behaviors.begin(), behaviors.end(), startName))
		return startName;

	CString altName;
	int count = 1;

	do {
		count ++;
		altName.Format("%s%d", startName, count);
	} while (exists_element(behaviors.begin(), behaviors.end(), altName));

	return altName;
}

CString CObjType::GetFolder()
{
	return folder;
}

void CObjType::SetFolder(LPCSTR folder_)
{
	// clear my tree item and update
	folder = folder_;

	if (tree)
	{
		HTREEITEM locate = pMainWnd->project_bar.tree.GetParentItem(tree);
		locate = pMainWnd->project_bar.tree.GetParentItem(locate);
		locate = pMainWnd->project_bar.tree.GetParentItem(locate);

		CApplication* application = (CApplication*)pMainWnd->project_bar.tree.GetItemData(locate);

		pMainWnd->project_bar.tree.DeleteItem(tree);
		tree = NULL;

		pMainWnd->project_bar.UpdateApplication(application);
	}
}

void CObjType::SetName(LPCSTR name_)
{
	m_Name = name_;

	pMainWnd->project_bar.tree.SetItemText(tree, m_Name);
}