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

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////


long ExtObject::aSetValue(LPVAL params)
{
	// ExpReturn operator=
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

long ExtObject::aClearTransparent(LPVAL theParams)
{
	drawQueue.push_back(new DrawClear(cr::transparent_black));
	return 0;
}

long ExtObject::aClearColour(LPVAL theParams)
{
	COLORREF cr = theParams[0].GetInt();
	drawQueue.push_back(new DrawClear(cr::cr_colorref_opacity(cr, theParams[1].GetFloat() / 100.0)));
	return 0;
}

long ExtObject::aFill(LPVAL theParams)
{
	COLORREF cr = theParams[0].GetInt();
	float a = theParams[1].GetFloat() / 100.0;
	CRect rc(theParams[2].GetInt(), theParams[3].GetInt(), theParams[4].GetInt(), theParams[5].GetInt());
	drawQueue.push_back(new DrawFill(cr::cr_colorref_opacity(cr, a), rc));
	return 0;
}

long ExtObject::aPoint(LPVAL params)
{
	COLORREF cr = params[2].GetInt();
	float a = params[3].GetFloat() / 100.0;

	drawQueue.push_back(new DrawPoint(params[0].GetFloat(), params[1].GetFloat(), cr::cr_colorref_opacity(cr, a)));
	return 0;
}

long ExtObject::aLine(LPVAL params)
{
	COLORREF cr = params[4].GetInt();
	float a = params[5].GetFloat() / 100.0;

	drawQueue.push_back(new DrawLine(params[0].GetFloat(), params[1].GetFloat(), params[2].GetFloat(), params[3].GetFloat(), cr::cr_colorref_opacity(cr, a)));
	return 0;
}

long ExtObject::aSetDebuggingPf(LPVAL params)
{
	CRunObject* pObj = params[0].GetPairedObjectParam(pRuntime, this);

	int count;
	CRunObject** siblings;
	pRuntime->GetObjectSiblings(pObj, siblings, count);

	// get rts
	pObj = siblings[0];

	// get data
	pfMap = (unsigned short*)pObj->GetData(1337, NULL);
	pWaypoints = (list<CPoint>*)pObj->GetData(1338, NULL);
	pfcellW = pObj->GetData(1339, NULL);
	pfcellH = pObj->GetData(1340, NULL);

	pfdebug = true;

	return 0;
}

long ExtObject::aPasteObject(LPVAL params)
{
	PerformDrawingQueue();

	CRunObjType* pType = params[0].GetObjectParam(pRuntime);
	int count;
	CRunObject** objs;
	pRuntime->GetTypeSelectedInstances(pType, objs, count);


	for(int i = 0; i < count; i++)
	{
		CRunObject* pObj = objs[i];

		OBJHEADER restore = pObj->info;
		pObj->info.x -= info.x - info.HotSpotX;
		pObj->info.y -= info.y - info.HotSpotY;
		pObj->UpdateBoundingBox();

		renderer->SetRenderTarget(info.curTexture);
		pRuntime->DrawObject(pObj);

		pObj->info = restore;
		pObj->UpdateBoundingBox();
	}

	textureChanged = true;

	return 0;
}

long ExtObject::aGenerateMask(LPVAL params)
{
	generateMask = true;

	return 0;
}

long ExtObject::aLoadFromFile(LPVAL params)
{
	//TODO
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::eGetValue(LPVAL params, ExpReturn& ret)
{
	return ret.ReturnCustom(pRuntime, privateVars[params[0].GetVariableIndex(pRuntime, pType)]);
}

void ExtObject::PerformDrawingQueue()
{
	if (firstFrame) {
		firstFrame = 0;
		renderer->SetRenderTarget(info.curTexture);
		renderer->ClearRenderTarget();
		renderer->SetTexture(startTexture);
		renderer->SetScreenTranslation(true);		// disable scroll offset, has to draw to 0,0
		renderer->Quad_xy(0, 0);
		renderer->SetScreenTranslation(false);		// enable scroll offset
		renderer->RestoreRenderTarget();
	}

	if (!drawQueue.empty()) {
		renderer->SetRenderTarget(info.curTexture);

		vector<DrawCommand*>::iterator i = drawQueue.begin();

		for ( ; i != drawQueue.end(); i++) {
			(*i)->Do(this);
			delete (*i);
		}

		drawQueue.resize(0);

		renderer->RestoreRenderTarget();

		textureChanged = true;
	}
}

void ExtObject::GenerateCollision()
{
	if (textureChanged && generateMask) {
		textureChanged = false;
		generateMask = false;

		pRuntime->GenerateCollisionMaskFromTexture(this, info.curTexture);
	}
}
#endif // #ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// ACE tables
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{

	// Define your ACE tables here.
	// Use ADDPARAM before each ADDACT/ADDCND/ADDEXP to add formal parameters.
	
	// Format:
	// ADDPARAM(type, title, description)
	//
	// type = the type of parameter.  See the PARAMS enum.
	// title = title of parameter, in expression editor
	// description = text to go in the text box describing this parameter.  Try to give
	//				 a helpful description.

	/////////////////////////////
	// Conditions

	// Format:
	// ADDCND(menuID, routine, scriptname, displaytext, flags)
	//
	// menuID = ID in the menu resource, must start at 2000
	// routine = address of the function associated
	// scriptname = reserved for future script use, e.g. "Set X position" script name could be "SetX"
	// displaytext = text to display in event editor, %0 is first param, %1 second etc
	// flags: can be
	// 0: ordinary evaluated event
	// CND_TRIGGERED: ordinary event trigger
	// CND_FASTTRIGGERED: fast trigger
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to test.");
	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value", "Value to compare to");
	ADDCND("Compare value", "Private variables", "%o Value %0 %1 %2", &ExtObject::cValueCmp, "CompareValue", 0);

	/////////////////////////////
	// Actions

	// ADDACT(menuID, routine, scriptname, displaytext)
	// Same format as ADDCND, but actions have no flags.

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to store");
	ADDACT("Set value", "Private variables", "Set %0 to %1", &ExtObject::aSetValue, "SetValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to add.");
	ADDACT("Add to value", "Private variables", "Add %1 to %0", &ExtObject::aAddValue, "AddToValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to subtract.");
	ADDACT("Subtract from value", "Private variables", "Subtract %1 from %0", &ExtObject::aSubValue, "SubtractFromValue", 0);

	ADDACT("Clear to transparent", "Canvas", "Clear to transparent", &ExtObject::aClearTransparent, "ClearTransparent", 0);

	ADDPARAM(PARAM_COLOR, "Colour", "Colour to clear the surface with.");
	ADDPARAMDEF(PARAM_VALUE, "Opacity", "Opacity to clear surface to.  100 = solid, 0 = transparent.", "100");
	ADDACT("Clear to colour", "Canvas", "Clear %0 opacity %1", &ExtObject::aClearColour, "ClearColour", 0);

	ADDPARAM(PARAM_COLOR, "Colour", "Colour to clear surface with.");
	ADDPARAMDEF(PARAM_VALUE, "Opacity", "Opacity to clear surface to.  100 = solid, 0 = transparent.", "100");
	ADDPARAM(PARAM_VALUE, "Left", "X co-ordinate of left of fill area.");
	ADDPARAM(PARAM_VALUE, "Top", "Y co-ordinate of top of fill area.");
	ADDPARAM(PARAM_VALUE, "Right", "X co-ordinate of right of fill area.");
	ADDPARAM(PARAM_VALUE, "Bottom", "Y co-ordinate of bottom of fill area.");
	ADDACT("Fill rectangle", "Canvas", "Fill area (%2, %3, %4, %5) with %0 opacity %1", &ExtObject::aFill, "Fill", 0);

	ADDPARAM(PARAM_VALUE, "X", "X co-ordinate of point to draw");
	ADDPARAM(PARAM_VALUE, "Y", "Y co-ordinate of point to draw");
	ADDPARAM(PARAM_COLOR, "Colour", "Colour of point to draw.");
	ADDPARAMDEF(PARAM_VALUE, "Opacity", "Opacity of dot.  100 = solid, 0 = transparent.", "100");
	ADDACT("Draw point", "Canvas", "Draw %2 point at (%0, %1) opacity %3", &ExtObject::aPoint, "Point", 0);

	ADDPARAM(PARAM_VALUE, "X1", "X co-ordinate of start of line");
	ADDPARAM(PARAM_VALUE, "Y1", "Y co-ordinate of start of line");
	ADDPARAM(PARAM_VALUE, "X2", "X co-ordinate of end of line");
	ADDPARAM(PARAM_VALUE, "Y2", "Y co-ordinate of end of line");
	ADDPARAM(PARAM_COLOR, "Colour", "Colour of point to draw.");
	ADDPARAMDEF(PARAM_VALUE, "Opacity", "Opacity of line.  100 = solid, 0 = transparent.", "100");
	ADDACT("Draw line", "Canvas", "Draw %4 line opacity %5 from (%0, %1) to (%2, %3)", &ExtObject::aLine, "Line", 0);

	ADDPARAM(PARAM_OBJECT, "Object", "Object to draw");
	ADDACT("Paste Object", "Canvas", "Paste %0 into canvas", &ExtObject::aPasteObject, "PasteObject", 0);

	ADDACT("Update collision mask", "Canvas", "Update collision mask", &ExtObject::aGenerateMask, "UpdateCollisions", 0);

	
	//ADDPARAM(PARAM_OBJECT, "Object", "DEBUG");
	//ADDACT("Set pathfinding object", "DEBUG", "Set debugging pathfinding from RTS of %0", &ExtObject::aSetDebuggingPf, "SetDebug", 0);
	
	/////////////////////////////
	// Expressions

	// ADDEXP(menuID, routine, expressionName, displaytext, flags)
	// Same format as conditions, except with the expression editor text.
	// expressionName = the name in the expression editor, e.g "Width" would
	// associate this expression when the user types YourObject.Width
	// Flags:  RETURN_VALUE, RETURN_STRING, RETURN_UNDEFINED

	ADDPARAM(PARAM_PRIVATEVARIABLE, "'Variable name'", "Name of value.");
	ADDEXP("Get private variable", "Private variables", "Value", &ExtObject::eGetValue, 0);

#include "..\..\Common\CommonAceTable.hpp"
}


