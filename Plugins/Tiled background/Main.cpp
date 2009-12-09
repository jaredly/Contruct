// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

// If run only
#ifdef RUN_ONLY

#include "..\..\Common\CommonAceDef.hpp"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cValueCmp(LPVAL params)
{
	const ExpStore& l = privateVars[params[0].GetVariableIndex(pRuntime, pType)];
	const ExpReturn& r = params[2];

	return DoComparison(params[1].GetInt(), (const ExpBase&)l, (const ExpBase&)r);
}

// Static condition
long ExtObject::cPickLowestVar(LPVAL params)
{
	// Param 0: Private Variable (Variable name)
	int count;
	CRunObject** instances;
	pRuntime->GetTypeSelectedInstances(pType, instances, count);

	int varIndex = params[0].GetVariableIndex(pRuntime, pType);

	// We know these are sprites
	ExtObject** sprites = (ExtObject**)instances;

	bool anyResults = false;
	double bestVal;
	ExtObject* bestObj;

	// Iterate all looking for the lowest numeric value
	ExtObject** i = sprites;
	ExtObject** end = sprites + count;

	for ( ; i != end; i++) {
		
		// This private var is numerical
		const ExpStore& curVal = (*i)->privateVars[varIndex];

		if (curVal.Type() == EXPTYPE_INTEGER || curVal.Type() == EXPTYPE_FLOAT) {

			// No value yet: use as first
			if (!anyResults) {
				anyResults = true;
				bestVal = curVal.GetDouble();
				bestObj = *i;
				continue;				
			}

			// Else check if this value is better than the best
			if (curVal.GetDouble() < bestVal) {
				bestVal = curVal.GetDouble();
				bestObj = *i;
			}
			
		}
	}

	// No object had a numerical value - fail the event.
	if (!anyResults)
		return false;

	// Otherwise, select just the best object we found.
	pRuntime->SelectAll(pType);
	pRuntime->SelectF(bestObj, pType);
	return true;
}

// Static condition
long ExtObject::cPickHighestVar(LPVAL params)
{
	// Param 0: Private Variable (Variable name)
	int count;
	CRunObject** instances;
	pRuntime->GetTypeSelectedInstances(pType, instances, count);

	int varIndex = params[0].GetVariableIndex(pRuntime, pType);

	// We know these are sprites
	ExtObject** sprites = (ExtObject**)instances;

	bool anyResults = false;
	double bestVal;
	ExtObject* bestObj;

	// Iterate all looking for the lowest numeric value
	ExtObject** i = sprites;
	ExtObject** end = sprites + count;

	for ( ; i != end; i++) {
		
		// This private var is numerical
		const ExpStore& curVal = (*i)->privateVars[varIndex];

		if (curVal.Type() == EXPTYPE_INTEGER || curVal.Type() == EXPTYPE_FLOAT) {

			// No value yet: use as first
			if (!anyResults) {
				anyResults = true;
				bestVal = curVal.GetDouble();
				bestObj = *i;
				continue;				
			}

			// Else check if this value is better than the best
			if (curVal.GetDouble() > bestVal) {
				bestVal = curVal.GetDouble();
				bestObj = *i;
			}
			
		}
	}

	// No object had a numerical value - fail the event.
	if (!anyResults)
		return false;

	// Otherwise, select just the best object we found.
	pRuntime->SelectAll(pType);
	pRuntime->SelectF(bestObj, pType);
	return true;
}

// Static condition
long ExtObject::cPickRandom(LPVAL params)
{
	int count;
	CRunObject** objs;
	pRuntime->GetTypeSelectedInstances(pType, objs, count);

	// Select all first, we're only going to pick one object.
	pRuntime->SelectAll(pType);

	// Generate a random number 0-count
	int index = pRuntime->Random() * count;

	// Select this object
	pRuntime->SelectF(objs[index], pType);

	return true;	// We selected something.
}

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aLoadTextureFromFile(LPVAL params)
{
	filename = params[0].GetString();

	TextureHandle newTex;

	try {
		newTex = renderer->CreateTextureFromFile(filename);
	}
	catch (...) {
		return 0;	// failed
	}

	// Free old texture
	renderer->ReleaseTexture(th);

	// Update to new texture
	info.curTexture = th = newTex;

	has_file_loaded = true;

	return 0;
}

long ExtObject::aSetImageOffset(LPVAL theParams)
{
	iOffset.x = theParams[0].GetFloat();
	iOffset.y = theParams[1].GetFloat();
	return 0;
}
long ExtObject::aSetImageScale(LPVAL theParams)
{
	iScale.x = theParams[0].GetFloat();
	iScale.y = theParams[1].GetFloat();
	return 0;
}
long ExtObject::aSetImageAngle(LPVAL theParams)
{
	iAngle = theParams[0].GetFloat();
	return 0;
}

long ExtObject::aSetValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] = params[1];

	return 0;
}

long ExtObject::aAddValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] += params[1];

	return 0;
}

long ExtObject::aSubValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] -= params[1];

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::eGetValue(LPVAL params, ExpReturn& ret)
{
	return ret.ReturnCustom(pRuntime, privateVars[params[0].GetVariableIndex(pRuntime, pType)]);
}

long ExtObject::eNumPVs(LPVAL params, ExpReturn& ret)
{
	return ret = privateVars.size();
}

#endif // #ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// ACE tables
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to test.");
	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value", "Value to compare to");
	ADDCND("*Compare a private variable", "Private variables", "%o Value %0 %1 %2", &ExtObject::cValueCmp, "CompareValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Variable name", "Variable to find lowest value in.");
	ADDCND("Pick object with lowest variable", "Private variables", "Pick %o with lowest '%0'", &ExtObject::cPickLowestVar, "PickLowestVar", CND_STATIC);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Variable name.", "Variable to find highest value in.");
	ADDCND("Pick object with highest variable", "Private variables", "Pick %o with highest '%0'", &ExtObject::cPickHighestVar, "PickHighestVar", CND_STATIC);

	ADDCND("Pick a random object", "Pick", "Pick a random %o", &ExtObject::cPickRandom, "PickRandom", CND_STATIC);


	ADDPARAM(PARAM_STRING, "Filename", "Enter an image file to load");
	ADDACT("Load texture from file", "Files", "Load texture from file %0", &ExtObject::aLoadTextureFromFile, "LoadFromFile", 0);

	ADDPARAM(PARAM_VALUE, "Offset X", "Set the X Offset" );
	ADDPARAM(PARAM_VALUE, "Offset Y", "Set the Y Offset" );
	ADDACT("Set image offset", "Power-of-two textures only", "Set image offset to %0, %1", &ExtObject::aSetImageOffset, "SetImageOffset", 0);

	ADDPARAMDEF(PARAM_VALUE, "Set the X Scale (1.0 is normal, 2.0 is twice the size)", "Scale X", "1");
	ADDPARAMDEF(PARAM_VALUE, "Set the Y Scale (1.0 is normal, 2.0 is twice the size)", "Scale Y", "1");
	ADDACT("Set image scale", "Power-of-two textures only", "Set image scale to %0, %1", &ExtObject::aSetImageScale, "SetImageScale", 0);

	ADDPARAM(PARAM_VALUE, "Set the angle (clockwise)", "Angle");
	ADDACT("Set image angle", "Power-of-two textures only", "Set image angle to %0", &ExtObject::aSetImageAngle, "SetImageAngle", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to store");
	ADDACT("*Set value", "Private variables", "Set %0 to %1", &ExtObject::aSetValue, "SetValue", 0);


	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to add.");
	ADDACT("Add to value", "Private variables", "Add %1 to %0", &ExtObject::aAddValue, "AddValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to subtract.");
	ADDACT("Subtract from value", "Private variables", "Subtract %1 from %0", &ExtObject::aSubValue, "SubValue", 0);


	ADDPARAM(PARAM_PRIVATEVARIABLE, "'Variable name'", "Name of value.");
	ADDEXP("*Get private variable", "Private variables", "Value", &ExtObject::eGetValue, 0);
	ADDEXP("Get private variable count", "Private variables", "NumVars", &ExtObject::eNumPVs, RETURN_VALUE);

#include "..\..\Common\CommonAceTable.hpp"



}
