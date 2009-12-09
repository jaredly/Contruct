// File includes
#include "StdAfx.h"
#include "Export.h"

// Generate layout
void CExport::GenerateLayout(CList<CString, CString&>& dllList)
{
	// Any behaviors we are currently editing need to be saved
	for (int i = 0; i < application->layouts.GetSize(); i++)
	{
		CLayout* pLayout = application->layouts.GetAt(application->layouts.FindIndex(i));

		if(pLayout->m_pControlBehaviorExt)
		{
			bin BehaviorData(g_allocator);
			BehaviorData.allocate(1024);
			BehaviorData.loading = false;
			pLayout->m_oControlBehaviorInfo->ETSerialize(pLayout->m_pControlBehaviorExt, BehaviorData);

			*pLayout->m_pControlBehaviorData = (BYTE*)BehaviorData.ptr();
			*pLayout->m_pControlBehaviorDataSize = BehaviorData.size();

			BehaviorData.detach();
		}
	}

	///////////////////
	// Object types
	///////////////////

	// We need to convert
	POSITION pos = application->object_types.GetStartPosition();
	CObjType* oT;
	long nKey = 0;
	int objectnumber = 0;
	while (pos != NULL) 
	{	
		application->object_types.GetNextAssoc(pos, nKey, oT);
		if(oT && !oT->m_bIsGroupType)
		{
			// It is okay to export this object type
			m_ExportTypesNumbers[oT->ObjectIdentifier] =  objectnumber;
			m_ExportTypes[objectnumber] = oT;
			
			objectnumber ++;	// move on to the next number
		}
	}

	// Work out which familys we need
	Families.clear();
	CObjTypeMap* pTypeMap = &application->object_types;
	pos = pTypeMap->GetStartPosition();

	list<Family>::iterator f = application->families.begin();

	for ( ; f != application->families.end(); f++)
	{
		if(f->is_in_use)
		{
			FamilyLookup n;
			n.m_FamilyName = f->name;
			n.m_Family = &*f;
			vector<CObjType*> objects;
			f->GetObjectTypes(objects);
			
			if(objects.size())
				Families.push_back(n);
		}
	}

	while (pos != NULL)
	{
		pTypeMap->GetNextAssoc(pos, nKey, oT);
		for (int i = 0; i < oT->families.size(); i++)
		{
			// Find reference in other vector
			for (int j = 0; j < Families.size(); j++)
				if (Families[j].m_FamilyName == oT->families[i]->name)
					Families[j].m_ObjectTypeID.push_back(ObjTypeIDCnvt(oT->ObjectIdentifier));
		}
	}






	// Number of types to export
	LayoutBlock << m_ExportTypes.size();
	
	// Loop through and export the object types
	for (NumberToTypeMap::iterator i = m_ExportTypes.begin(); i != m_ExportTypes.end(); i++)
	{
		oT = i->second;

		LayoutBlock << (int)ObjTypeIDCnvt(oT->ObjectIdentifier);
		LayoutBlock << oT->GetName();

		// Get ext filename from o identifier
		OINFO* oInfo;
		g_dlls->Lookup(oT->DLLIndex, oInfo);
		CString extFile(oInfo->extFileName);

		MicroDependencyImpl Dep;
		Dep.m_pDependencies = &m_Dependencies;

		// Add dependencies
		ETADDDEPENDENCIES ETAddDependencies = (ETADDDEPENDENCIES)GetProcAddress(oInfo->hInstLib, "ETAddDependencies");

		if (ETAddDependencies != NULL)
			(*ETAddDependencies)(Dep);

		// Add the DLL if it doesn't already exist.
		bool dllAdded = true;
		if (!dllList.Find(extFile))
		{
			dllAdded = false;
			dllList.AddTail(extFile);
		}

		// Get the plugin index in the dll list
		int extIndex = 0;

		POSITION p = dllList.GetHeadPosition();
		// Search dlllist
		while (p != NULL) {	
			if (dllList.GetNext(p) == extFile)
				break;
			extIndex++;
		}

		LayoutBlock<<extIndex;

		LayoutBlock << (char)oT->m_bGlobal;
		LayoutBlock << (int)oT->destroyWhen;

		LayoutBlock << (int)oT->m_PrivateValues.size();
		for (int i = 0 ; i < oT->m_PrivateValues.size() ; i++)
		{
			LayoutBlock << oT->m_PrivateValues[i].name;
			LayoutBlock << oT->m_PrivateValues[i].type;
		}

		// Pad 16 chars
		LayoutBlock<<(int)0;
		LayoutBlock<<(int)0;
		LayoutBlock<<(int)0;
		LayoutBlock<<(int)0;

#ifndef PYTHON
		LayoutBlock<<(BYTE)0;
#else
		if(dllAdded)
		{
			LayoutBlock<<(BYTE)0;	// we have nothing to add
		}
		else
		{
			LayoutBlock<<(BYTE)1;	// we have nothing to add

			LayoutBlock<<(int)oInfo->actions.size();
			for(vector<ACESEntry2>::iterator a = oInfo->actions.begin(); a < oInfo->actions.end(); a++)
			{
				LayoutBlock << a->scriptName;
				LayoutBlock << a->params.size();
			}

			LayoutBlock<<(int)oInfo->conditions.size();
			for(vector<ACESEntry2>::iterator c = oInfo->conditions.begin(); c < oInfo->conditions.end(); c++)
			{
				LayoutBlock << c->scriptName;
				LayoutBlock << c->params.size();
			}

			LayoutBlock<<(int)oInfo->expressions.size();
			for(vector<ACESEntry2>::iterator e = oInfo->expressions.begin(); e < oInfo->expressions.end(); e++)
			{
				LayoutBlock << e->scriptName;
				LayoutBlock << e->params.size();
			}
		}
#endif
		// Add ID
		CString nId;
		nId.Format("%d", nKey);
		exportID.Add(nId);
	}


	int movDllsAdded = 0;

	///////////////////
	// Behaviors
	///////////////////
	BehaviorCount = 0;

	for (NumberToTypeMap::iterator i = m_ExportTypes.begin(); i != m_ExportTypes.end(); i++)
	{
		oT = i->second;
		BehaviorCount += oT->behaviors.size();
	}

	LayoutBlock << BehaviorCount;

	// Wasn't cleared and was causing multiple export problems -Ash
	MvtLookup.clear();
		
	for (NumberToTypeMap::iterator i = m_ExportTypes.begin(); i != m_ExportTypes.end(); i++)
	{
		oT = i->second;

		BehaviorIterator j = oT->behaviors.begin();
		BehaviorConstIterator movs_end = oT->behaviors.end();

		int movindex = 0;
		for ( ; j != movs_end; j++, movindex++)
		{
			LayoutBlock << (int)ObjTypeIDCnvt(oT->ObjectIdentifier);
			
			int NewIndex = -1;

			// Attempt to find a map for the Behavior DLL to a DLL index exported to the runtime.
			// If one doesn't exist, create a map.
			bool New = true;
			for (int h = 0; h < MvtLookup.size(); h++)
				if (MvtLookup[h].PreviousIndex == j->BehaviorDLLIndex)
				{
					New = false;
					NewIndex = MvtLookup[h].NewIndex;
					
					break;
				}

			if (New)
			{
				// Fixed for more than 1 Behavior per frame -Ash
				NewIndex = dllList.GetCount() + movDllsAdded;
				movDllsAdded++;

				MvLookup Lookup;
				Lookup.PreviousIndex = j->BehaviorDLLIndex;
				Lookup.NewIndex = NewIndex;

				OINFO* o = GetOINFO(j->BehaviorDLLIndex);
				Lookup.Name = o->extFileName;

				MvtLookup.push_back(Lookup);
			}

			// Put new dll index into list
			LayoutBlock << NewIndex;

			LayoutBlock << movindex;

			// Behavior name
			LayoutBlock << j->Text;

			// Export the Behavior properties data
			
			LayoutBlock << j->eSize;
			LayoutBlock.append(j->eData, j->eSize);

#ifndef PYTHON
			LayoutBlock<<(BYTE)0;
#else
			if (New)
			{
				OINFO* oInfo = GetOINFO(j->BehaviorDLLIndex);
				LayoutBlock << (BYTE)1;

				LayoutBlock<<(int)oInfo->actions.size();
				for(vector<ACESEntry2>::iterator a = oInfo->actions.begin(); a < oInfo->actions.end(); a++)
				{
					LayoutBlock << a->scriptName;
					LayoutBlock << a->params.size();
				}

				LayoutBlock<<(int)oInfo->conditions.size();
				for(vector<ACESEntry2>::iterator c = oInfo->conditions.begin(); c < oInfo->conditions.end(); c++)
				{
					LayoutBlock << c->scriptName;
					LayoutBlock << c->params.size();
				}

				LayoutBlock<<(int)oInfo->expressions.size();
				for(vector<ACESEntry2>::iterator e = oInfo->expressions.begin(); e < oInfo->expressions.end(); e++)
				{
					LayoutBlock << e->scriptName;
					LayoutBlock << e->params.size();
				}
			}
			else
			{
				LayoutBlock << (BYTE)0;
			}
#endif

		}
	}

	/////////////////////////////////////////
	// Export traits
	vector<TraitExport> Traits;

	CObjTypeMap* objTypeMap = &application->object_types;
	pos = objTypeMap->GetStartPosition();

	// iterate through objects
	for (NumberToTypeMap::iterator i = m_ExportTypes.begin(); i != m_ExportTypes.end(); i++)
	{
		oT = i->second;

		for (int i = 0; i < oT->traits.size(); i++)
		{
			TraitExport t;
			t.OID = ObjTypeIDCnvt(oT->ObjectIdentifier);
			t.TraitName = oT->traits[i];
			Traits.push_back(t);
		}
	}

	// Gather list of traits
	vector<CString> TraitNames;
	for (int i = 0; i < Traits.size(); i++)
	{
		if (find(TraitNames.begin(), TraitNames.end(), Traits[i].TraitName) == TraitNames.end())
			TraitNames.push_back(Traits[i].TraitName);
	}

	// Fixed -Ash
	TraitsCount = TraitNames.size();
	LayoutBlock<<TraitNames.size();

	for (int i = 0; i < TraitNames.size(); i++)
	{
		LayoutBlock<<TraitNames[i];

		int Count = 0;
		for(int j = 0; j < Traits.size(); j++)
			if (Traits[j].TraitName == TraitNames[i])
				Count++;

		LayoutBlock<<Count;
		for (int j = 0; j < Traits.size(); j++)
			if (Traits[j].TraitName == TraitNames[i])
				LayoutBlock<<Traits[j].OID;
	}

	/////////////////////////////////////////
	// Export families
	

	LayoutBlock << Families.size();

	for (int i = 0; i < Families.size(); i++)
	{
		LayoutBlock << Families[i].m_FamilyName;
		LayoutBlock << Families[i].m_ObjectTypeID.size();
		for (int j = 0; j < Families[i].m_ObjectTypeID.size(); j++)
			LayoutBlock << Families[i].m_ObjectTypeID[j];

		Families[i].m_Family->UpdateFamilyData();

		int size = Families[i].m_Family->variables.size();
		int size_check = 0;
		for (int j = 0 ; j < size ; j++)
		{
			PrivateValue* pVal = Families[i].m_Family->GetReferenceVariable(j);
			if(pVal)
				size_check ++;
		}
		LayoutBlock << (int)size_check;
		for (int j = 0 ; j < size ; j++)
		{
			PrivateValue* pVal = Families[i].m_Family->GetReferenceVariable(j);
			if(pVal){
				LayoutBlock << pVal->name;
				LayoutBlock << pVal->type;
			}
		}

	}

	// Containers
	LayoutBlock << application->m_Containers.size();
	list<CContainer>::iterator ctr = application->m_Containers.begin();
	for ( ; ctr != application->m_Containers.end(); ctr++)
	{
		LayoutBlock << ctr->m_IDs.size();

		for (int j = 0; j < ctr->m_IDs.size(); j++)
			LayoutBlock << ObjTypeIDCnvt(ctr->m_IDs[j]);
	}

	// EXPORT LAYOUTS
	int numLayouts = application->layouts.GetSize();

	LayoutBlock << numLayouts;

	int iter = 0;
	for (int i = 0; i < numLayouts; i++)
	{
		// This is filled when objects are serialized
		application->images_to_save.clear();
		application->animations_to_save.clear();
		CLayout* pLayout;
		
		// Exporting all layouts
		pLayout = application->layouts.GetAt(application->layouts.FindIndex(i));
		
		// Common properties
		LayoutBlock << (int)pLayout->GetW();
		LayoutBlock << (int)pLayout->GetH();
		LayoutBlock << pLayout->GetName();
		LayoutBlock << (int)pLayout->GetColor();
		LayoutBlock << (bool)(pLayout->m_unboundedScrolling != 0);
		LayoutBlock << pLayout->application_background;

		// Layout keys
		LayoutBlock << pLayout->layoutKeys.GetSavedKeyCount();
		map<CString, KeyData>::iterator k = pLayout->layoutKeys.keys.begin();

		for ( ; k != pLayout->layoutKeys.keys.end(); k++)
		{
			// Save if necessary
			if (k->second.bSave)
			{
				LayoutBlock << k->first;
				LayoutBlock << k->second.iType;

				if (k->second.iType == 0)
					LayoutBlock << (int)k->second.pData;
				else
					LayoutBlock << k->second.String;
			}
		}
	
		//////////////////
		//  Export Layers
		//////////////////
		LayoutBlock << (int)pLayout->layers.GetCount();

		POSITION rearrange_position = pLayout->layers.GetHeadPosition();

		// make a new layer list so the non-layout layer is last
		CLayerList rearrange;

		// For each layer
		while(rearrange_position)
		{
			CLayer* layer = pLayout->layers.GetNext(rearrange_position);

			rearrange.AddTail(layer);
		}

		rearrange.RemoveHead();
		rearrange.AddTail(pLayout->layers.GetHead());

		POSITION layer_position = rearrange.GetHeadPosition();

		while(layer_position)
		{
			CLayer* layer = rearrange.GetNext(layer_position);

			// Export properties
			LayoutBlock << (int) layer->m_layerID;           // ID
			LayoutBlock << layer->m_name;                    // Name
			LayoutBlock << (BYTE)layer->m_layerType;         // Type
			LayoutBlock << (long)layer->color;               // Filter
			LayoutBlock << (float)layer->opacity;			// Opacity
			LayoutBlock << (float)layer->m_angle;            // Initial angle
			LayoutBlock << (float)layer->scrollXf;           // X Coefficient
			LayoutBlock << (float)layer->scrollYf;           // Y Coefficient
			LayoutBlock << (float)layer->scrollX;			// Scroll X Offset
			LayoutBlock << (float)layer->scrollY;			// Scroll Y Offset

			LayoutBlock << (float)layer->zoomXf;             // X Zoom Offset
			LayoutBlock << (float)layer->zoomYf;             // Y Zoom Offset
			LayoutBlock << (float)layer->zoomX;				// X Zoom Coefficient
			LayoutBlock << (float)layer->zoomY;				// Y Zoom Coefficient

			LayoutBlock << (char)layer->clearBack;			// Clear background colour?
			LayoutBlock << (long)layer->backColor;			// Background colour
			LayoutBlock << (bool)layer->forceOwnTexture;		// Force own texture
			LayoutBlock << (int)layer->sampler;
			LayoutBlock << layer->enable3d;
			
			//Prepare objects
			vector<CObj*> Objects;
			
			if(layer->m_inherrit_layerID)
			{
				CLayout* pLayout = NULL;
				CLayer* pLayer = application->FindLayerFromNumber(layer->m_inherrit_layerID, &pLayout);
				if(pLayout)
				{
					PrepareObjectList(pLayer->m_zOrder, Objects, pLayer, pLayout);
				}
			}

			PrepareObjectList(layer->m_zOrder, Objects, layer, pLayout);
			
			LayoutBlock << (int) Objects.size(); // Object count

			// Export all objects
			for(vector<CObj*>::iterator i = Objects.begin(); i != Objects.end(); i++)
			{
				CObj* o = *i;
				int eSize = GetOINFO(o->editObject->DLLIndex)->eSize;

				LayoutBlock<<(int)nKey;
				LayoutBlock<<(int)o->editObject->objectX;
				LayoutBlock<<(int)o->editObject->objectY;
				LayoutBlock<<(int)o->editObject->objectWidth;
				LayoutBlock<<(int)o->editObject->objectHeight;
				LayoutBlock<<(float)o->editObject->objectAngle;
				LayoutBlock<<(int)o->editObject->filter;

				// Future rethink needed
				// okay guys if u use an object type id u HAVE to convert it...ffs
				LayoutBlock<<(int)ObjTypeIDCnvt(o->editObject->ObjectIdentifier);
				LayoutBlock<<(int)(o->editObject->instanceID);
				LayoutBlock<<(int)ObjTypeIDCnvt(o->editObject->ObjectIdentifier);

				
				//LayoutBlock << (int)o->m_values.size();

				// the order that the variables are exported are determined by
				// the order of variables in the object type, so we need to match them up
				// Note: this could be sped up later with a map or something
				CObjType* pType = o->GetObjectType(application);

				LayoutBlock<<pType->m_PrivateValues.size();	// more reliable
				for( int i = 0; i < pType->m_PrivateValues.size(); i++)
				{
					CString value;
					for(int j = 0; j < o->m_values.size(); j++)
					{
						// Variable matches ID; export initial value
						if(o->m_values[j].ID ==  pType->m_PrivateValues[i].identifier) {
							value = o->m_values[j].value;
							LayoutBlock << value;
							break;
						}
					}
				}

				bin objData(g_allocator);
				objData.loading = false;
				objData.allocate(100);		// allocate some bytes to prevent reallocs early

				GetOINFO(o->editObject->DLLIndex)->ETSerialize(o->editObject, objData);
		
				// Emits eSize followed by binary
				LayoutBlock<<objData;

				iter++;
			} // end of for each object
		}  // end of each layer

		// Serialize all the images
		map<int, CObj*>::iterator at = application->animations_to_save.begin();
		for(; at!= application->animations_to_save.end(); at++)
		{
			CAnimation* anim = application->resources.FindAnimationFromNumber(at->first);
			if(anim)
			{
				anim->GetImagesToSave(application->images_to_save);
			}
		}

		LayoutBlock << (int)application->images_to_save.size();
		map<int, CObj*>::iterator it = application->images_to_save.begin();


		for(; it!= application->images_to_save.end(); it++)
		{
			LayoutBlock << it->first;
		}

		// Serialize layout texture mode
		LayoutBlock << (int)pLayout->texture_loading;

	} // end of each layout

	/////////////////////////////////////////
	// Export animations
	
	ExportAnimations(application->resources.animations);


	/////////////////////////////////////////
	// Now generate HLSL block
	try {
		GenerateHLSL(dllList);
	}
	catch (...) {
		// An error occurred compiling.  Clear the HLSL block and pad with zeroes, disabling effects.
		hlslBlock.clear();

		::MessageBox(NULL, "Errors occurred compiling one of the FX files in your Effects directory.  "
				   "The application will run with effects disabled.  You need to fix "
				   "the FX script, or stop using it, before effects can run correctly in this application.", "HLSL error", MB_OK | MB_ICONHAND); 

		// Pad zeros, no effects will load
		for (int x = 0; x < 100; x++)
			hlslBlock<<0;
	}
}
void CExport::ExportAnimations(list<CAnimation>& anims)
{
	LayoutBlock << anims.size();

	for(list<CAnimation>::iterator i = anims.begin(); i!= anims.end(); i++)
	{
		// Fixed ID
		LayoutBlock << (int)i->m_FixedID;

		LayoutBlock << i->m_Name;
		LayoutBlock << i->tag;

		LayoutBlock << (float)i->m_Speed; // Speed
		LayoutBlock << (BYTE)i->IsAngle();
		LayoutBlock << (float)i->m_Angle; // Angle
		if(i->m_bForever)
			LayoutBlock << (int)-1;
		else
			LayoutBlock << (int)i->m_Repeat;
		LayoutBlock << (int)(i->m_RepeatTo-1);
		LayoutBlock << (int)i->m_bPingPong;

		

		LayoutBlock << (int)i->m_Images.size();

		for (int j = 0 ; j < i->m_Images.size(); j++)
		{
			LayoutBlock << i->m_FrameTimes[j]; // Time
			LayoutBlock << i->m_Images[j];
		}

		ExportAnimations(i->m_SubAnimations);
	}

}

void CExport::GenerateHLSL(CList<CString, CString&>& dllList)
{
	// First, count how many objects with HLSL there are, whilst creating a map of HLSL names to indices
	int hlslObjectCount = 0;
	CObjType* oT;
	int i;
	CPath path;
	path.SetToCurrentDirectory();

	vector<CString> hlslMap;

	// Find the HLSL used by objects
	for (NumberToTypeMap::iterator i = m_ExportTypes.begin(); i != m_ExportTypes.end(); i++)
	{
		oT = i->second;

		if (!oT->effects.empty()) {
			hlslObjectCount++;
			
			EffectIterator e = oT->effects.begin();
			EffectConstIterator fx_end = oT->effects.end();

			// For each effect, ensure is in the hlsl map
			for ( ; e != fx_end; e++) {

				push_back_unique(hlslMap, CString("Effects\\") + e->effectFile);

			}//for
		}//if
	}

	int hlslLayerCount = 0;

	int numLayouts = application->layouts.GetCount();

	// for each frame
	for (i = 0; i < numLayouts; i++) {

		CLayout* pLayout = application->layouts.GetAt(application->layouts.FindIndex(i));

		POSITION layerpos = pLayout->layers.GetHeadPosition();

		// Find the HLSL used by layers
		while(layerpos)
		{
			CLayer* layer = pLayout->layers.GetNext(layerpos);

			if (!layer->effects.empty()) {
				hlslLayerCount++;

				EffectIterator e = layer->effects.begin();
				EffectConstIterator fx_end = layer->effects.end();

				// For each effect, ensure is in the hlsl map
				for ( ; e != fx_end; e++) {

					push_back_unique(hlslMap, CString("Effects\\") + e->effectFile);

				}
			}
		}
	}

	// Add transitions
	list<CTransition>::iterator tr = application->m_Transitions.begin();

	for ( ; tr != application->m_Transitions.end(); tr++) {

		push_back_unique(hlslMap, CString("Transitions\\") + tr->m_FileName);

	}

	// Run without PS option
	hlslBlock << application->game_information.run_without_ps;

	// Export HLSL list
	hlslBlock << hlslMap.size();

	for (i = 0; i < hlslMap.size(); i++) {
		// Compile HLSL; emit name, size & data
		hlslBlock << hlslMap[i];
		CString curFile;
		curFile.Format("%s%s", path.GetFullPath(), hlslMap[i]);

		// Load file for inspection
		CFile f(curFile, CFile::modeRead);
		char* strbuf = new char[f.GetLength()+1];
		f.Read(strbuf, f.GetLength());
		strbuf[f.GetLength()] = '\0';

		// Get version strings (third line, eg // PS 2.0)
		CString verStr;
		AfxExtractSubString(verStr, strbuf, 2);		// Third line
		verStr.Replace("/", "");
		verStr.Replace(" ", "");
		verStr.Replace("PS", "");
		
		CString majorVer;
		CString minorVer;
		AfxExtractSubString(majorVer, verStr, 0, '.');
		AfxExtractSubString(minorVer, verStr, 1, '.');

		// No version specified by FX file writer.  Default to 2.0
		if (majorVer == "") {
			majorVer = "2";
			minorVer = "0";
		}

		// Emit major then minor version
		hlslBlock << atoi(majorVer);
		hlslBlock << atoi(minorVer);

		// Emit flags
		hlslBlock << (bool)(strstr(strbuf, "//#CROSS-SAMPLING") != NULL);
		hlslBlock << (bool)(strstr(strbuf, "//#BORDER-MODE") != NULL);
		hlslBlock << (bool)(strstr(strbuf, "//#FULLSCREEN-MODE") != NULL);

		delete[] strbuf;
		f.Close();

		// Compile the effect.
		ID3DXEffectCompiler* hlslCompiler;
		ID3DXBuffer* errors;
		D3DXCreateEffectCompilerFromFile(curFile, NULL, NULL, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, &hlslCompiler, &errors);

		if (errors) {
			::MessageBox(NULL, (const char*)errors->GetBufferPointer(), NULL, NULL);
			errors->Release();
			throw;
		}
		
		ID3DXBuffer* hlslData;
		hlslCompiler->CompileEffect(0, &hlslData, &errors);

		if (errors) {
			::MessageBox(NULL, (const char*)errors->GetBufferPointer(), NULL, NULL);
			errors->Release();
			throw;
		}

		// Write effect data
		hlslBlock << hlslData->GetBufferSize();
		hlslBlock.append(hlslData->GetBufferPointer(), hlslData->GetBufferSize());

		if (hlslData) hlslData->Release();
		if (hlslCompiler) hlslCompiler->Release();
	}

	hlslBlock << hlslObjectCount;

	// Follow HLSL data with objects with effects list
	for (NumberToTypeMap::iterator i = m_ExportTypes.begin(); i != m_ExportTypes.end(); i++)
	{
		oT = i->second;

		if (!oT->effects.empty()) {
			hlslBlock << ObjTypeIDCnvt(oT->ObjectIdentifier);
			hlslBlock << oT->effects.size();

			EffectIterator e = oT->effects.begin();
			EffectConstIterator fx_end = oT->effects.end();

			// For each of the object's effects, emit the effect index
			for ( ; e != fx_end; e++) {
				hlslBlock << find_index(hlslMap.begin(), hlslMap.end(), CString("Effects\\") + e->effectFile);

				// Name of the effect
				hlslBlock << e->Text;

				// Disable mode
				hlslBlock << (int)e->disableMode;

				// Emit each of the effect's parameters
				hlslBlock << e->params.size();

				for (int j = 0; j < e->params.size(); j++) {
					hlslBlock << e->params[j].type;
					hlslBlock << e->params[j].name;
					hlslBlock << e->params[j].variableName;
					hlslBlock << e->params[j].value;
				}
			}
			
		}//if
	}

	hlslBlock << hlslLayerCount;

	// Emit layers and their effects
	for (i = 0; i < numLayouts; i++) {

		CLayout* pLayout = application->layouts.GetAt(application->layouts.FindIndex(i));

		POSITION layerpos = pLayout->layers.GetHeadPosition();
		int index = 0;

		// Find the HLSL used by layers
		while(layerpos)
		{
			CLayer* layer = pLayout->layers.GetNext(layerpos);

			if (!layer->effects.empty()) {

				hlslBlock << i;	// frame number
				hlslBlock << index - 1; // layer index: compensate for non-layout layer at bottom - the runtime
										// sees the nonlayout layer at the top

				hlslBlock << layer->effects.size();

				EffectIterator e = layer->effects.begin();
				EffectConstIterator fx_end = layer->effects.end();

				// For each of the layer's effects, emit the effect index
				for ( ; e != fx_end; e++) {
					hlslBlock << find_index(hlslMap.begin(), hlslMap.end(), CString("Effects\\") + e->effectFile);

					// Name of the effect
					hlslBlock << e->Text;

					// Disable mode
					hlslBlock << (int)e->disableMode;

					// Emit each of the effect's parameters
					hlslBlock << e->params.size();

					for (int j = 0; j < e->params.size(); j++) {
						hlslBlock << e->params[j].type;
						hlslBlock << e->params[j].name;
						hlslBlock << e->params[j].variableName;
						hlslBlock << e->params[j].value;
					}
				}
			}

			index++;
		}
	}//for each frame

	hlslBlock << application->m_Transitions.size();

	tr = application->m_Transitions.begin();

	for ( ; tr != application->m_Transitions.end(); tr++)
	{
		// Export name
		hlslBlock << tr->m_Name;

		// Export index
		hlslBlock << find_index(hlslMap.begin(), hlslMap.end(), CString("Transitions\\") + tr->m_FileName);

		// Name of the effect
		hlslBlock << tr->m_Name;

		hlslBlock << 0;//reserved for disableMode

		// Emit each of the effect's parameters
		hlslBlock << tr->params.size();

		for (int j = 0; j < tr->params.size(); j++) {
			hlslBlock << tr->params[j].type;
			hlslBlock << tr->params[j].name;
			hlslBlock << tr->params[j].variableName;
			hlslBlock << tr->params[j].value;
		}

	}
}


void CExport::PrepareObjectList(CObjList& objlist, vector<CObj*>& objects, CLayer* layer, CLayout* pLayout)
{


	POSITION pos = objlist.GetHeadPosition();
	while(pos)
	{
		CObj* o;
		long id = layer->m_zOrder.GetNext(pos);		
		pLayout->objects.Lookup(id, o);

		if(o->m_bIsGroup)
		{
			PrepareObjectList(o->m_groupObjects, objects, layer, pLayout);
		}
		else
			objects.push_back(o);

	}

}