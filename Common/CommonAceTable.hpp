
#ifdef RUN_ONLY
#define ADDCND_AT(i, n, c, d, f, s, r)		at->RunAce((ACROUTINE)f, NULL, ACETYPE_CNDFUNC, r, i)
#define ADDACT_AT(i, n, c, d, f, s, r)		at->RunAce((ACROUTINE)f, NULL, ACETYPE_ACTFUNC, r, i)
#define ADDEXP_AT(i, n, c, d, f, r)			at->RunAce((ACROUTINE)f, d, ACETYPE_EXPFUNC, 0, i)
#else
#define ADDCND_AT(i, n, c, d, f, s, r)		at->EditAce(n, c, d, s, r, ACETYPE_CONDITION, i)
#define ADDACT_AT(i, n, c, d, f, s, r)		at->EditAce(n, c, d, s, r, ACETYPE_ACTION, i)
#define ADDEXP_AT(i, n, c, d, f, r)			at->EditAce(n, c, d, d, r, ACETYPE_EXPRESSION, i)
#endif


///////////////////////////
// COMMON ACE TABLE ENTRIES

// COMMONACE_POSITION	: Add position aces such as set X, set Y, compare X etc
// COMMONACE_DIMENSION	: Add dimension aces such as set W, compare H etc
// COMMONACE_ZORDER		: Add Z order aces such as move to front, move to layer
// COMMONACE_ANGLE		: Add angle aces such as compare angle, set angle, get angle etc
// COMMONACE_VISIBILITY : Add is visible/set visible condition and action
// COMMONACE_OPACITY	: Add compare/set/get opacity action, condition and expression, via info.filter
// COMMONACE_FILTER		: Add compare/set/get filter action, condition and expression, via info.filter
// COMMONACE_IDS		: Add compare UID, get OID and UID
// COMMONACE_COUNT_DESTROY : Add a .count expression, and a destroy action

// Next CND ID: 20
// Next ACT ID: 25
// Next EXP ID: 23

#define COMPARISON_COMBO_TEXT "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal"

#ifdef COMMONACE_POSITION
	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", COMPARISON_COMBO_TEXT);
	ADDPARAM(PARAM_VALUE, "X co-ordinate", "X position to compare");
	ADDCND_AT(0, "Compare X position", "Size & Position", "%o %n:  X %0 %1", &ExtObject::cmn_cXCmp, "CompareX", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", COMPARISON_COMBO_TEXT);
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "Y position to compare");
	ADDCND_AT(1, "Compare Y position", "Size & Position", "%o %n:  Y %0 %1", &ExtObject::cmn_cYCmp, "CompareY", 0);

	ADDCND_AT(2, "Object is on-screen?", "Size & Position", "%o %n:  Is on-screen", &ExtObject::cmn_cIsOnscreen, "IsOnscreen", 0);
	ADDCND_AT(3, "Object is outside layout?", "Size & Position", "%o %n:  Is outside layout", &ExtObject::cmn_cOutsideLayout, "IsOutsideLayout", 0);

	ADDPARAM(PARAM_VALUE, "Value 1", "Value 1");
	ADDPARAMCOMBO("Comparison", "Comparison to make", "Equal to|Different to|Lower than|Lower or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value 2", "Value 2");
	ADDCND_AT(14, "Pick by comparison", "Pick", "%o %n: Pick by %0 %1 %2", &ExtObject::cCompare2Values, "PickByCompare", 0);

	ADDPARAM(PARAM_VALUE, "Value", "Value");
	ADDCND_AT(15, "Pick by evaluate", "Pick", " %o %n: Pick by %0", &ExtObject::cEvaluate, "Evaluate", 0);



	ADDPARAM(PARAM_VALUE, "X", "New X co-ordinate in pixels");
	ADDACT_AT(0, "Set X", "Size & Position", "Set X to %0", &ExtObject::cmn_aSetX, "SetX", 0);

	ADDPARAM(PARAM_VALUE, "Y", "New Y co-ordinate in pixels");
	ADDACT_AT(1, "Set Y", "Size & Position", "Set Y to %0", &ExtObject::cmn_aSetY, "SetY", 0);

	ADDPARAM(PARAM_OBJECT, "Object", "Object to set position to.");
	ADDPARAM(PARAM_UNDEFINED, "Image point name or number (optional)", "Leave 0 to use object's position, or enter the image point name or number to position to.");
	ADDACT_AT(2, "Set position to another object", "Size & Position", "Set position to object %0 (image point %1)", &ExtObject::cmn_aSetPositionToObject, "SetPositionToObject", 0);

	ADDPARAM(PARAM_VALUE, "X", "New X co-ordinate in pixels");
	ADDPARAM(PARAM_VALUE, "Y", "New Y co-ordinate in pixels");
	ADDACT_AT(19, "Set position", "Size & Position", "Set position to %0, %1", &ExtObject::cmn_aSetPosition, "SetPosition", 0);

	ADDPARAM(PARAM_VALUE, "Pixel Distance", "Number of pixels to move by");
	ADDPARAM(PARAM_VALUE, "Angle to move at", "0 = right, 90 = down, 180 = left, 270 = up, or use .Angle to move at the object's angle");
	ADDACT_AT(20, "Move at angle", "Size & Position", "Move %0 pixels at %1 degrees", &ExtObject::cmn_aMovePosition, "MovePosition", 0);




	ADDEXP_AT(0, "Get X", "Size & Position", "X", &ExtObject::cmn_eGetX, RETURN_FLOAT);
	ADDEXP_AT(1, "Get Y", "Size & Position", "Y", &ExtObject::cmn_eGetY, RETURN_FLOAT);
	ADDEXP_AT(2, "Get X,Y", "Size & Position", "XY", &ExtObject::cmn_eGetXY, RETURN_ARRAY);

	ADDEXP_AT(12, "Get X position of left edge", "Size & Position", "Left", &ExtObject::cmn_eGetLeftEdge, RETURN_FLOAT);
	ADDEXP_AT(13, "Get Y position of top edge", "Size & Position", "Top", &ExtObject::cmn_eGetTopEdge, RETURN_FLOAT);
	ADDEXP_AT(14, "Get X position of right edge", "Size & Position", "Right", &ExtObject::cmn_eGetRightEdge, RETURN_FLOAT);
	ADDEXP_AT(15, "Get Y position of bottom edge", "Size & Position", "Bottom", &ExtObject::cmn_eGetBottomEdge, RETURN_FLOAT);

#endif // COMMONACE_POSITION

#ifdef COMMONACE_DIMENSION
	ADDPARAMCOMBO("Comparison", "Select how to compare the width.", COMPARISON_COMBO_TEXT);
	ADDPARAM(PARAM_VALUE, "Width", "The height to compare to.");
	ADDCND_AT(4, "Compare width", "Size & Position", "%o %n:  Width %0 %1", &ExtObject::cmn_cCompareWidth, "CompareWidth", 0);
	ADDPARAMCOMBO("Comparison", "Select how to compare the height.", COMPARISON_COMBO_TEXT);
	ADDPARAM(PARAM_VALUE, "Height", "The height to compare to.");
	ADDCND_AT(5, "Compare height", "Size & Position", "%o %n:  Height %0 %1", &ExtObject::cmn_cCompareHeight, "CompareHeight", 0);

	ADDPARAM(PARAM_VALUE, "Width", "New width in pixels");
	ADDACT_AT(3, "Set width", "Size & Position", "Set width to %0", &ExtObject::cmn_aSetWidth, "SetWidth", 0);
	ADDPARAM(PARAM_VALUE, "Height", "New height in pixels");
	ADDACT_AT(4, "Set height", "Size & Position", "Set height to %0", &ExtObject::cmn_aSetHeight, "SetHeight", 0);

	ADDEXP_AT(3, "Get width", "Size & Position", "Width", &ExtObject::cmn_eGetWidth, RETURN_FLOAT);
	ADDEXP_AT(4, "Get height", "Size & Position", "Height", &ExtObject::cmn_eGetHeight, RETURN_FLOAT);
#endif // COMMONACE_DIMENSION

#ifdef COMMONACE_ZORDER
	ADDPARAM(PARAM_LAYER, "Layer", "Select a layer to check.");
	ADDCND_AT(6, "Is on layer", "Z Order", "%o %n:  Is on layer %0", &ExtObject::cmn_cIsOnLayer, "IsOnLayer", 0);

	ADDCND_AT(12, "Pick top-most", "Z Order", "%o %n: Pick the top-most", &ExtObject::cmn_cPickTopmost, "PickTopmost", CND_STATIC);
	ADDCND_AT(13, "Pick bottom-most", "Z Order", "%o %n: Pick the bottom-most", &ExtObject::cmn_cPickBottommost, "PickBottommost", CND_STATIC);

	ADDACT_AT(5, "Send to front", "Z Order", "Send to front", &ExtObject::cmn_aZToFront, "ToFront", 0);
	ADDACT_AT(6, "Send to back", "Z Order", "Send to back", &ExtObject::cmn_aZToBack, "ToBack", 0);
	ADDPARAM(PARAM_OBJECT, "Object", "Object to place in front of.");
	ADDACT_AT(7, "Place in front of another object", "Z Order", "Place in front of %0", &ExtObject::cmn_aZInFrontOf, "ToInFront", 0);
	ADDPARAM(PARAM_OBJECT, "Object", "Object to place behind.");
	ADDACT_AT(8, "Place behind another object", "Z Order", "Place behind of %0", &ExtObject::cmn_aZBehindOf, "ToBehind", 0);
	ADDPARAM(PARAM_LAYER, "Layer", "Layer to move object to.");
	ADDACT_AT(9, "Move to layer", "Z Order", "Move to layer %0", &ExtObject::cmn_aMoveToLayer, "ToLayer", 0);

	ADDEXP_AT(5, "Get layer number", "Z Order", "Layer", &ExtObject::cmn_eGetLayer, RETURN_INTEGER);
	ADDEXP_AT(20, "Get Z index", "Z Order", "ZIndex", &ExtObject::cmn_eGetZIndex, RETURN_INTEGER);
#endif

#ifdef COMMONACE_ANGLE
	ADDPARAMCOMBO("Comparison", "Select how to compare the angle.", COMPARISON_COMBO_TEXT);
	ADDPARAM(PARAM_VALUE, "Angle", "The angle to compare to.");
	ADDCND_AT(7, "Compare angle", "Angle", "%o %n:  Angle %0 %1", &ExtObject::cmn_cCompareAngle, "CompareAngle", 0);

	ADDPARAM(PARAM_VALUE, "Angle", "New angle, in degrees 0-360");
	ADDACT_AT(10, "Set angle", "Angle", "Set angle to %0", &ExtObject::cmn_aSetAngle, "SetAngle", 0);
	ADDPARAM(PARAM_OBJECT, "Object", "Select the object to orient towards.");
	ADDACT_AT(11, "Set angle towards an object", "Angle", "Set angle towards %0", &ExtObject::cmn_aAngleToObject, "AngleTowardsObject", 0);
	ADDPARAM(PARAM_VALUE, "X co-ordinate", "X co-ordinate to orient towards, in pixels, eg. MouseX");
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "Y co-ordinate to orient towards, in pixels, eg. MouseY");
	ADDACT_AT(12, "Set angle towards position", "Angle", "Set angle towards (%0, %1)", &ExtObject::cmn_aAngleToPosition, "AngleTowardsPosition", 0);
	ADDPARAM(PARAM_VALUE, "Rotation, in degrees", "Number of degrees to rotate.");
	ADDACT_AT(13, "Rotate clockwise", "Angle", "Rotate %0 degrees clockwise", &ExtObject::cmn_aRotateClockwise, "RotateClockwise", 0);
	ADDPARAM(PARAM_VALUE, "Rotation, in degrees", "Number of degrees to rotate.");
	ADDACT_AT(14, "Rotate counter-clockwise", "Angle", "Rotate %0 degrees counter-clockwise", &ExtObject::cmn_aRotateCounterwise, "RotateCounterclockwise", 0);

	ADDEXP_AT(6, "Get angle", "Angle", "Angle", &ExtObject::cmn_eGetAngle, RETURN_FLOAT);
#endif // COMMONACE_ANGLE

#ifdef COMMONACE_VISIBILITY
	ADDCND_AT(8, "Is visible?", "Appearance", "%o %n:  Is visible", &ExtObject::cmn_cIsVisible, "IsVisible", 0);

	ADDPARAMCOMBO("Visibility", "Either hide or show the object.", "Invisible|Visible");
	ADDACT_AT(15, "Set visible", "Appearance", "Make %0", &ExtObject::cmn_aSetVisible, "SetVisible", 0);
#endif // COMMONACE_VISIBILITY

#ifdef COMMONACE_OPACITY
	ADDPARAMCOMBO("Comparison", "Select how to compare the opacity.", COMPARISON_COMBO_TEXT);
	ADDPARAM(PARAM_VALUE, "Opacity", "Opacity to compare to. (0 = transparent, 100 = opaque)");
	ADDCND_AT(9, "Compare opacity", "Appearance", "%o %n:  Opacity %0 %1", &ExtObject::cmn_cCompareOpacity, "CompareOpacity", 0);

	ADDPARAM(PARAM_VALUE, "Opacity", "New semitransparency value, 0 (invisible) to 100 (opaque)");
	ADDACT_AT(16, "Set opacity", "Appearance", "Set opacity to %0", &ExtObject::cmn_aSetOpacity, "SetOpacity", 0);

	ADDEXP_AT(7, "Get opacity", "Appearance", "Opacity", &ExtObject::cmn_eGetOpacity, RETURN_FLOAT);
#endif // COMMONACE_OPACITY

#ifdef COMMONACE_FILTER
	ADDPARAMCOMBO("Comparison", "Select how to compare the opacity.", "Is|Is not");
	ADDPARAM(PARAM_COLOR, "Filter", "Filter to compare to.");
	ADDCND_AT(10, "Compare colour filter", "Appearance", "%o %n:  Filter %0 %1", &ExtObject::cmn_cCompareFilter, "CompareFilter", 0);

	ADDPARAM(PARAM_COLOR, "Filter", "New filter for the object.");
	ADDACT_AT(17, "Set colour filter", "Appearance", "Set filter to %0", &ExtObject::cmn_aSetFilter, "SetFilter", 0);

	ADDEXP_AT(8, "Get colour filter", "Appearance", "Filter", &ExtObject::cmn_eGetFilter, RETURN_INTEGER);
#endif // COMMONACE_FILTER

#ifdef COMMONACE_IDS
	ADDPARAM(PARAM_VALUE, "Unique ID", "Unique ID to compare.");
	ADDCND_AT(11, "Compare unique ID", "Misc", "%o %n:  Unique ID is %0", &ExtObject::cmn_cCompareUID, "CompareUID", 0);

	ADDEXP_AT(9, "Get Object ID", "Misc", "OID", &ExtObject::cmn_eGetOID, RETURN_VALUE);
	ADDEXP_AT(10, "Get Unique ID", "Misc", "UID", &ExtObject::cmn_eGetUID, RETURN_VALUE);
#endif

#ifdef COMMONACE_COUNT_DESTROY
	ADDACT_AT(18, "Destroy", "Misc", "Destroy", &ExtObject::cmn_aDestroy, "Destroy", 0);

	ADDEXP_AT(19, "Get number of objects", "Misc", "Count", &ExtObject::cmn_eGetCount, RETURN_VALUE);
#endif

#ifdef COMMONACE_Z_ELEVATION
	ADDPARAMCOMBO("Comparison", "Select how to compare the Z elevation.", COMPARISON_COMBO_TEXT);
	ADDPARAM(PARAM_VALUE, "Z elevation", "The elevation to compare to.");
	ADDCND_AT(16, "Compare Z elevation", "Z Order", "%o %n: Z elevation %0 %1", &ExtObject::cmn_cCompareZElevation, "CompareZElevation", 0);

	ADDPARAM(PARAM_VALUE, "Z elevation", "The Z elevation to set, from 100 (at camera) to -1000 (in the distance).  Default is 0.");
	ADDACT_AT(21, "Set Z elevation", "Z Order", "Set Z elevation to %0", &ExtObject::cmn_aSetZElevation, "SetZElevation", 0);

	ADDEXP_AT(21, "Get Z elevation", "Z Order", "ZElevation", &ExtObject::cmn_eGetZElevation, RETURN_VALUE);
#endif

#ifdef COMMONACE_PRIVATE_VARIABLES
	// PRIVATE VARIABLES
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to test.");
	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value", "Value to compare to");
	ADDCND_AT(17, "*Compare a private variable", "Private variables", "%o %n:  Value %0 %1 %2", &ExtObject::cValueCmp, "CompareValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Variable name", "Variable to find lowest value in.");
	ADDCND_AT(18, "Pick object with lowest variable", "Private variables", "%o %n: Pick with lowest '%0'", &ExtObject::cPickLowestVar, "PickLowestVar", CND_STATIC);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Variable name.", "Variable to find highest value in.");
	ADDCND_AT(19, "Pick object with highest variable", "Private variables", "%o %n: Pick with highest '%0'", &ExtObject::cPickHighestVar, "PickHighestVar", CND_STATIC);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to store");
	ADDACT_AT(22, "*Set value", "Private variables", "Set %0 to %1", &ExtObject::aSetValue, "SetValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to add.");
	ADDACT_AT(23, "Add to value", "Private variables", "Add %1 to %0", &ExtObject::aAddValue, "AddValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to subtract.");
	ADDACT_AT(24, "Subtract from value", "Private variables", "Subtract %1 from %0", &ExtObject::aSubValue, "SubValue", 0);


	ADDPARAM(PARAM_PRIVATEVARIABLE, "'Variable name'", "Name of value.");
	ADDEXP_AT(22, "*Get private variable", "Private variables", "Value", &ExtObject::eGetValue, 0);


#endif