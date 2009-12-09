#include "StdAfx.h"
#include "CFamily.h"
#include "..\Utilities\Helper.h"

class TypesUsingFamilyExporter : public ObjectTypeFunctor {
public:
	CString family;
	CArchive& ar;

	TypesUsingFamilyExporter(CString _family, CArchive& _ar) : family(_family), ar(_ar) { family.MakeLower(); }

	void operator()(CObjType* pType) {
		vector<Family*>::iterator i = pType->families.begin();

		for ( ; i != pType->families.end(); i++) {
			CString curName = (*i)->name;
			curName.MakeLower();
			if (curName == family)
				ar << pType->ObjectIdentifier;
		}
	}
};

bool Family::Serialize(CArchive& ar)
{
	CString ExpectedName = "CFamily";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	if(ar.IsLoading())
	{
		ar >> identifier;
		ar >> name;

		CString name;
		ar >> name;

		OINFO* o = GetOINFO(name);
		if(o)
			DLLIndex = o->oID;
		else
			DLLIndex = -1;

		SerializeBitmap(image, ar);	
		SerializeBitmap(small_image, ar);

		///////////////////////////////////////////////////
		// OBJECT TYPES
		int objTypeCnt = 0;
		ar >> objTypeCnt;
		CObjType *oT;
		long nKey;

		for (int  i = 0; i < objTypeCnt; i++)
		{
			ar >> nKey;
			application->object_types.Lookup(nKey, oT);
			if(oT)
			{
				oT->families.push_back(this);
			}
		}

		// Now our special map
		int size;
		ar >> size;
		for(int i = 0; i < size; i++)
		{
			behaviors.push_back(FamilyBehavior());
			
			if (!behaviors.at(i).Serialize(ar))
				return false;
		}

		ar >> size;
		for(int i = 0; i < size; i++)
		{
			variables.push_back(FamilyVariable());
			
			if (!variables.at(i).Serialize(ar))
				return false;
		}

		ar >> size;
		for(int i = 0; i < size; i++)
		{
			effects.push_back(FamilyEffect());
			
			if (!effects.at(i).Serialize(ar))
				return false;
		}
	}

	else
	{
		ar << identifier;
		ar << name;
	
		OINFO* o = GetOINFO(DLLIndex);
		CString CStr = o->extName;

		ar << CStr;

		SerializeBitmap(image,ar);	
		SerializeBitmap(small_image, ar);

		// Write down all the index's of the object types which use this
		TypesUsingFamilyCounter counter(name);
		ForEachObjectType(application, counter);
		ar << counter.count;

		TypesUsingFamilyExporter exporter(name, ar);
		ForEachObjectType(application, exporter);

		// Serialize behaviors
		int size = 0;
		size = behaviors.size();
		ar << size;
		for(int i = 0; i < size; i++)
			behaviors.at(i).Serialize(ar);

		// Serialize variables
		size = variables.size();
		ar << size;
		for(int i = 0; i < size; i++)
			variables.at(i).Serialize(ar);

		// Serialize effects
		size = effects.size();
		ar << size;
		for(int i = 0; i < size; i++)
			effects.at(i).Serialize(ar);
	}

	return true;
}

bool FamilyShare::Serialize(CArchive& ar)
{
	CString ExpectedName = "CFamilyShare";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	if(ar.IsLoading())
	{
		ar >> unique_id ;
		ar >> name;
		int size = 0;
		ar >> size;

		reference_ids.clear();

		for(int a = 0; a < size; a++)
		{
			reference_ids.push_back(FamilyAssociate());
			ar >> reference_ids.at(a).item_id;
			ar >> reference_ids.at(a).object;
		}
	}

	else
	{
		ar << unique_id;
		ar << name;
		int size = reference_ids.size();
		ar << size;
		for(int a = 0; a < size; a++)
		{
			ar << reference_ids.at(a).item_id;
			ar << reference_ids.at(a).object;
		}
	}

	return true;
}

void Family::GetObjectTypes(vector<CObjType*>& objects)
{
	objects.clear();
	CObjTypeMap* pTypeMap = &application->object_types;
	CObjType* oT = 0;
	long nKey = 0;

	POSITION pos = pTypeMap->GetStartPosition();

	while (pos != NULL)
	{
		pTypeMap->GetNextAssoc(pos, nKey, oT);
		for (int i = 0; i < oT->families.size(); i++)
		{
			if(oT->families[i] == this)
			{
				objects.push_back(oT);
			}
		}
	}
}

void Family::GetObjectTypesKeys(vector<long>& objects)
{
	objects.clear();
	CObjTypeMap* pTypeMap = &application->object_types;
	CObjType* oT = 0;
	long nKey = 0;

	POSITION pos = pTypeMap->GetStartPosition();

	while (pos != NULL)
	{
		pTypeMap->GetNextAssoc(pos, nKey, oT);
		for (int i = 0; i < oT->families.size(); i++)
		{
			if(oT->families[i] == this)
			{
				objects.push_back(nKey);
			}
		}
	}
}


class Counter
{
public:
	int count;

	Counter()
	{
		count = 0;
	}
};

// Check a family private variable exists.  Only check valid (existing) variables.
bool Family::VariableNameExists(CString varName)
{
	varName.MakeLower();

	vector<FamilyVariable>::iterator i = variables.begin();

	for ( ; i != variables.end(); i++) {
		if (i->valid) {	// Must be valid (current)
			CString lowerName = i->name;
			lowerName.MakeLower();

			if (varName == lowerName)
				return true;
		}
	}

	return false;
}

void Family::UpdateFamilyData()
{
	vector<CObjType*> objects;

	GetObjectTypes(objects);

	map<CString, vector<FamilyAssociate> > variable_map;
	map<CString, vector<FamilyAssociate> > movement_map;
	map<CString, vector<FamilyAssociate> > effect_map;

	///////////////////////////////////////////////
	// Step 1: map it out so we can do this easier
	for(int o = 0; o < objects.size(); o++)
	{
		CObjType* pType = objects.at(o);
		for(int v = 0; v < pType->m_PrivateValues.size(); v++)
		{
			PrivateValue* pVal = &pType->m_PrivateValues.at(v);
			variable_map[pVal->name].push_back( FamilyAssociate(pType->ObjectIdentifier, pVal->identifier));
		}

		for(int m = 0; m < pType->behaviors.size(); m++)
		{
			CBehavior* pMov = pType->GetBehaviorByIndex(m);
			movement_map[pMov->Text].push_back( FamilyAssociate(pType->ObjectIdentifier, pMov->id));
		}

		for(int e = 0; e < pType->effects.size(); e++)
		{
			CEffect* pEff = pType->GetEffectByIndex(e);
			effect_map[pEff->Text].push_back( FamilyAssociate(pType->ObjectIdentifier, pEff->m_effectID));
		}
	}

	///////////////////////////////////////////////
	// Step 2: set valid to false
	for(int v = 0; v < variables.size(); v++)
		variables.at(v).valid = false;

	for(int m = 0; m < behaviors.size(); m++)
		behaviors.at(m).valid = false;

	for(int e = 0; e < effects.size(); e++)
		effects.at(e).valid = false;

	///////////////////////////////////////////////
	// step 3: We loop our current variables and update the name member incase the user renamed the variable
	for(int v = 0; v < variables.size(); v++)
	{
		FamilyVariable* var = &variables.at(v);
		map<CString, Counter> hitcount;

		for(int i = 0; i < var->reference_ids.size(); i++)
		{
			FamilyAssociate* fam = &var->reference_ids.at(i);
			if(CObjType* pType = application->FindObjTypeFromNumber(fam->object))
				if(PrivateValue* pVal = pType->GetPrivateVariable(fam->item_id))
					hitcount[pVal->name].count ++;
		}
		// Now work out the name that was most common
		CString name;
		int max = -1;
		for(map<CString, Counter>::iterator i = hitcount.begin(); i!= hitcount.end(); i++)
		{
			if(max < i->second.count)
			{
				max = i->second.count;
				name = i->first;
			}
		}
		// If we found one, reset the name
		if(max != -1)
			var->name = name;
	}


	for(int m = 0; m < behaviors.size(); m++)
	{
		FamilyBehavior* mov = &behaviors.at(m);
		map<CString, Counter> hitcount;

		for(int i = 0; i < mov->reference_ids.size(); i++)
		{
			FamilyAssociate* fam = &mov->reference_ids.at(i);
			if(CObjType* pType = application->FindObjTypeFromNumber(fam->object))
				if(CBehavior* pMov = pType->GetBehaviorByUniqueID(fam->item_id))
					hitcount[pMov->Text].count ++;
		}
		// Now work out the name that was most common
		CString name;
		int max = -1;
		for(map<CString, Counter>::iterator i = hitcount.begin(); i!= hitcount.end(); i++)
		{
			if(max < i->second.count)
			{
				max = i->second.count;
				name = i->first;
			}
		}
		// If we found one, reset the name
		if(max != -1)
			mov->name = name;
	}


	for(int e = 0; e < effects.size(); e++)
	{
		FamilyEffect* eff = &effects.at(e);
		map<CString, Counter> hitcount;

		for(int i = 0; i < eff->reference_ids.size(); i++)
		{
			FamilyAssociate* fam = &eff->reference_ids.at(i);
			if(CObjType* pType = application->FindObjTypeFromNumber(fam->object))
				if(CEffect* pEff = pType->GetEffect(fam->item_id))
					hitcount[pEff->Text].count ++;
		}
		// Now work out the name that was most common
		CString name;
		int max = -1;
		for(map<CString, Counter>::iterator i = hitcount.begin(); i!= hitcount.end(); i++)
		{
			if(max < i->second.count)
			{
				max = i->second.count;
				name = i->first;
			}
		}
		// If we found one, reset the name
		if(max != -1)
			eff->name = name;
	}


	///////////////////////////////////////////////
	// Step 4: Loop through and add any items
	for(map<CString, vector<FamilyAssociate> >::iterator i = variable_map.begin(); i != variable_map.end(); i++)
	{
		// we only add items where every object's item shares the same name
		if(i->second.size() != objects.size())
			continue;
		CString name = i->first;

		bool found = false;
		// first see if there are any items that already have this name
		for(int v = 0; v < variables.size(); v++)
		{
			FamilyVariable* pVal = &variables.at(v);
			if(pVal->name == name)
			{
				pVal->reference_ids = i->second; //copy entire vector of reference
				pVal->valid = true;
		
				found = true; // escape
				v = variables.size();
			}
		}
		if(found)
			continue;
		
		// If we reached here, we couldn't find a variable, so we need to add a new one
		variables.push_back(FamilyVariable());
		FamilyVariable* pVal = &variables.back();
		pVal->name = i->first;
		pVal->reference_ids = i->second;
		pVal->valid = true;
		pVal->unique_id = application->m_varID++;
	}
	for(map<CString, vector<FamilyAssociate> >::iterator i = movement_map.begin(); i != movement_map.end(); i++)
	{
		// we only add items where every object's item shares the same name
		if(i->second.size() != objects.size())
			continue;
		CString name = i->first;

		bool found = false;

		// First see if there are any items that already have this name
		for(int m = 0; m < behaviors.size(); m++)
		{
			FamilyBehavior* pMov = &behaviors.at(m);
			if(pMov->name == name)
			{
				pMov->reference_ids = i->second; //copy entire vector of reference
				pMov->valid = true;
		
				found = true; // escape
				m = behaviors.size();
			}
		}
		if(found)
			continue;
		
		// If we reached here, we couldn't find a variable, so we need to add a new one
		behaviors.push_back(FamilyBehavior());
		FamilyBehavior* pMov = &behaviors.back();
		pMov->name = i->first;
		pMov->reference_ids = i->second;
		pMov->valid = true;
		pMov->unique_id = application->m_objID++;
	}	

	for(map<CString, vector<FamilyAssociate> >::iterator i = effect_map.begin(); i != effect_map.end(); i++)
	{
		// we only add items where every object's item shares the same name
		if(i->second.size() != objects.size())
			continue;
		CString name = i->first;

		bool found = false;
		// first see if there are any items that already have this name
		for(int e = 0; e < effects.size(); e++)
		{
			FamilyEffect* pEff = &effects.at(e);
			if(pEff->name == name)
			{
				pEff->reference_ids = i->second; //copy entire vector of reference
				pEff->valid = true;
		
				found = true; // escape
				e = effects.size();
			}
		}
		if(found)
			continue;
		
		// If we reached here, we couldn't find a variable, so we need to add a new one
		effects.push_back(FamilyEffect());
		FamilyEffect* pEff = &effects.back();
		pEff->name = i->first;
		pEff->reference_ids = i->second;
		pEff->valid = true;
		pEff->unique_id = application->m_objID++;
	}

}


CBehavior* Family::GetReferenceBehavior(int index)
{
	if(index >= behaviors.size())
		return 0;
	if(index < 0)
		return 0;
	
	FamilyBehavior* pFamMov = &behaviors.at(index);
	for(int r = 0; r < pFamMov->reference_ids.size(); r++)
	{
		FamilyAssociate* assoc = &pFamMov->reference_ids.at(r);
		CObjType* pType = application->FindObjTypeFromNumber(assoc->object);
		if(pType)
		{
			CBehavior* mov = pType->GetBehaviorByUniqueID(assoc->item_id);
			if(mov)
				return mov;
		}
	}

	return 0;
}
CEffect* Family::GetReferenceEffect(int index)
{
	if(index >= effects.size())
		return 0;
	if(index < 0)
		return 0;

	FamilyEffect* pFamEff = &effects.at(index);
	for(int r = 0; r < pFamEff->reference_ids.size(); r++)
	{
		FamilyAssociate* assoc = &pFamEff->reference_ids.at(r);
		CObjType* pType = application->FindObjTypeFromNumber(assoc->object);
		if(pType)
		{
			CEffect* eff = pType->GetEffect(assoc->item_id);
			if(eff)
				return eff;
		}
	}
	return 0;
}
PrivateValue* Family::GetReferenceVariable(int index)
{
	if(index >= variables.size())
		return 0;
	if(index < 0)
		return 0;

	FamilyVariable* pFamVar = &variables.at(index);
	for(int r = 0; r < pFamVar->reference_ids.size(); r++)
	{
		FamilyAssociate* assoc = &pFamVar->reference_ids.at(r);
		CObjType* pType = application->FindObjTypeFromNumber(assoc->object);
		if(pType)
		{
			PrivateValue* var = pType->GetPrivateVariable(assoc->item_id);
			if(var)
				return var;
		}
	}
	return 0;
}