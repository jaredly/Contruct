///////////////////////////
// COMMON ACE DEFINITIONS

// COMMONACE_POSITION	: Add position aces such as set X, set Y, compare X etc
// COMMONACE_DIMENSION	: Add dimension aces such as set W, compare H etc
// COMMONACE_ZORDER		: Add Z order aces such as move to front, move to layer
// COMMONACE_ANGLE		: Add angle aces such as compare angle, set angle, get angle etc
// COMMONACE_VISIBILITY : Add is visible/set visible condition and action
// COMMONACE_OPACITY	: Add compare/set/get opacity action, condition and expression, via info.filter
// COMMONACE_FILTER		: Add compare/set/get filter action, condition and expression, via info.filter
// COMMONACE_IDS		: Add compare UID, get OID and UID
// COMMONACE_COUNT_DESTROY : Add a .count expression, and a destroy action

///////////////////////////
// POSITIONING ACES
#ifdef COMMONACE_POSITION
long ExtObject::cmn_cXCmp(LPVAL params)
{
	float r = params[1].GetFloat();
	return DoComparison(params[0].GetInt(), info.x, r);
}

long ExtObject::cmn_cYCmp(LPVAL params)
{
	float r = params[1].GetFloat();
	return DoComparison(params[0].GetInt(), info.y, r);
}

long ExtObject::cmn_cIsOnscreen(LPVAL params)
{
	return !pRuntime->ObjectOffscreen(this);
}

long ExtObject::cmn_cOutsideLayout(LPVAL params)
{
	return pRuntime->ObjectOutsideFrame(this);
}
long ExtObject::cmn_aSetX(LPVAL params)
{
	info.x = params[0].GetFloat();
	pRuntime->UpdateBoundingBox(this);
	return 0;
}

long ExtObject::cmn_aSetY(LPVAL params)
{
	info.y = params[0].GetFloat();
	pRuntime->UpdateBoundingBox(this);
	return 0;
}
long ExtObject::cmn_aSetPosition(LPVAL params)
{
	info.x = params[0].GetFloat();
	info.y = params[1].GetFloat();
	pRuntime->UpdateBoundingBox(this);
	return 0;
}
long ExtObject::cmn_aMovePosition(LPVAL params)
{
	double distance = params[0].GetFloat();
	double angle_degrees = params[1].GetFloat();

	cr::clamp_angle_degrees(angle_degrees);

	double sin_a, cos_a;
	cr::sincosf(cr::to_radians(angle_degrees), &sin_a, &cos_a);

	info.x += cos_a * distance;
	info.y += sin_a * distance;
	pRuntime->UpdateBoundingBox(this);
	return 0;
}

long ExtObject::cCompare2Values(LPVAL theParams)
{
	// Get operator ID
	int operatorID = theParams[1].GetInt();
	const ExpReturn& l = theParams[0];
	const ExpReturn& r = theParams[2];

	switch (theParams[0].Type()) {
	case EXPTYPE_INTEGER:
		return DoComparison(operatorID, l.GetInt64(), r.GetInt64());
	case EXPTYPE_FLOAT:
		return DoComparison(operatorID, l.GetDouble(), r.GetDouble());
	case EXPTYPE_STRING:
		return DoComparison(operatorID, *(l.GetStringPtr()), r.GetString());
	default:
		return false;
	}
}
long ExtObject::cEvaluate(LPVAL theParams)
{
	switch (theParams[0].Type()) {
	case EXPTYPE_INTEGER:
		return theParams[0].GetInt() != 0;
	case EXPTYPE_FLOAT:
		return theParams[0].GetFloat() != 0.0;
	case EXPTYPE_STRING:
		return theParams[0].GetString() != "";
	default:
		return FALSE;
	}
}

long ExtObject::cmn_aSetPositionToObject(LPVAL params)
{
	CRunObject* pTarget = params[0].GetPairedObjectParam(pRuntime, this);
	if (pTarget == NULL) return 0;

	info.x = pTarget->info.x;
	info.y = pTarget->info.y;

	// Check for action points
	if (params[1].Type() == EXPTYPE_STRING) {
		POINTF pt;

		if (pRuntime->GetImagePoint(pTarget, *(params[1].GetStringPtr()), pt)) {
			info.x = pt.x;
			info.y = pt.y;
		}
	}
	else {
		POINTF pt;

		if (pRuntime->GetImagePoint(pTarget, params[1].GetInt() - 1, pt)) {
			info.x = pt.x;
			info.y = pt.y;
		}
	}

	pRuntime->UpdateBoundingBox(this);

	return 0;
}

long ExtObject::cmn_eGetX(LPVAL params, ExpReturn& ret)
{
	return ret = info.x;
}

long ExtObject::cmn_eGetY(LPVAL params, ExpReturn& ret)
{
	return ret = info.y;
}

long ExtObject::cmn_eGetXY(LPVAL params, ExpReturn &ret)
{
	// Store and return this instance's X and Y in the array {x, y}
	cmn_xyRet[0] = info.x;
	cmn_xyRet[1] = info.y;

	return ret.ReturnArray(cmn_xyRet, 2);
}

long ExtObject::cmn_eGetLeftEdge(LPVAL params, ExpReturn& ret)
{
	return ret = info.box.left;
}

long ExtObject::cmn_eGetTopEdge(LPVAL params, ExpReturn& ret)
{
	return ret = info.box.top;
}

long ExtObject::cmn_eGetRightEdge(LPVAL params, ExpReturn& ret)
{
	return ret = info.box.right;
}

long ExtObject::cmn_eGetBottomEdge(LPVAL params, ExpReturn& ret)
{
	return ret = info.box.bottom;
}


#endif //COMMONACE_POSITION

#ifdef COMMONACE_DIMENSION
long ExtObject::cmn_cCompareWidth(LPVAL params)
{
	// Param 0: Combo (Comparison)
	// Param 1: Number (Width)
	return DoComparison(params[0].GetInt(), info.w* (info.isMirrored?-1:1), params[1].GetFloat());
}

long ExtObject::cmn_cCompareHeight(LPVAL params)
{
	// Param 0: Combo (Comparison)
	// Param 1: Number (Height)
	return DoComparison(params[0].GetInt(), info.h * (info.isFlipped?-1:1), params[1].GetFloat());
}

long ExtObject::cmn_aSetWidth(LPVAL params)
{
	info.w = params[0].GetFloat() * (info.isMirrored?-1:1);
	//if (info.w < 1) info.w = 1;

	pRuntime->UpdateBoundingBox(this);
	return 0;
}

long ExtObject::cmn_aSetHeight(LPVAL params)
{
	info.h = params[0].GetFloat() * (info.isFlipped?-1:1);
	//if (info.h < 1) info.h = 1;

	pRuntime->UpdateBoundingBox(this);
	return 0;
}

long ExtObject::cmn_eGetWidth(LPVAL params, ExpReturn& ret)
{
	return ret = info.w * (info.isMirrored?-1:1);
}

long ExtObject::cmn_eGetHeight(LPVAL params, ExpReturn& ret)
{
	return ret = info.h * (info.isFlipped?-1:1);
}
#endif // COMMONACE_DIMENSION

#ifdef COMMONACE_ZORDER
long ExtObject::cmn_cIsOnLayer(LPVAL params)
{
	CRunLayer* pLayer = params[0].GetLayerParam(pRuntime, pLayout);

	if (pLayer == NULL) return false;

	return pLayer->number == info.layer;
}

long ExtObject::cmn_cPickTopmost(LPVAL params)
{
	// Param 0: Private Variable (Variable name)
	int count;
	CRunObject** instances;
	pRuntime->GetTypeSelectedInstances(pType, instances, count);


	ExtObject** extinstances = (ExtObject**)instances;

	bool anyResults = false;
	int bestVal;
	ExtObject* bestObj;

	// Iterate all looking for the lowest numeric value
	ExtObject** i = extinstances;
	ExtObject** end = extinstances + count;

	for ( ; i != end; i++) {	

		int curVal = pRuntime->GetZIndex(*i);

		if (!anyResults || curVal > bestVal) {
			anyResults = true;
			bestVal = curVal;
			bestObj = *i;
		}
	}

	// No object had a numerical value - fail the event.
	if (!anyResults)
		return false;

	// Otherwise, select just the best object we found.
	pRuntime->SelectAll(pType);
	pRuntime->SelectF(bestObj, pType);
	//pRuntime->Select(bestObj);
	return true;
}

long ExtObject::cmn_cPickBottommost(LPVAL params)
{
	// Param 0: Private Variable (Variable name)
	int count;
	CRunObject** instances;
	pRuntime->GetTypeSelectedInstances(pType, instances, count);


	ExtObject** extinstances = (ExtObject**)instances;

	bool anyResults = false;
	int bestVal;
	ExtObject* bestObj;

	// Iterate all looking for the lowest numeric value
	ExtObject** i = extinstances;
	ExtObject** end = extinstances + count;

	for ( ; i != end; i++) {	

		int curVal = pRuntime->GetZIndex(*i);

		if (!anyResults || curVal < bestVal) {
			anyResults = true;
			bestVal = curVal;
			bestObj = *i;
		}
	}

	// No object had a numerical value - fail the event.
	if (!anyResults)
		return false;

	// Otherwise, select just the best object we found.
	pRuntime->SelectAll(pType);
	pRuntime->SelectF(bestObj, pType);
	//pRuntime->Select(bestObj);
	return true;
}

long ExtObject::cmn_aZToFront(LPVAL params)
{
	pRuntime->ToFront(this);
	return 0;
}

long ExtObject::cmn_aZToBack(LPVAL params)
{
	pRuntime->ToBack(this);
	return 0;
}

long ExtObject::cmn_aZInFrontOf(LPVAL params)
{
	CRunObject* obj = params[0].GetPairedObjectParam(pRuntime, this);
	pRuntime->InFront(this, obj);
	return 0;
}

long ExtObject::cmn_aZBehindOf(LPVAL params)
{
	CRunObject* obj = params[0].GetPairedObjectParam(pRuntime, this);
	pRuntime->ToBehind(this, obj);
	return 0;
}

long ExtObject::cmn_aMoveToLayer(LPVAL params)
{
	CRunLayer* pLayer = params[0].GetLayerParam(pRuntime, pLayout);

	if (pLayer)
		pRuntime->ToLayer(this, pLayer->number);

	return 0;
}

long ExtObject::cmn_eGetLayer(LPVAL params, ExpReturn& ret)
{
	return ret = info.layer + 1;
}

long ExtObject::cmn_eGetZIndex(LPVAL params, ExpReturn& ret)
{
	return ret = pRuntime->GetZIndex(this);
}


#endif // COMMONACE_ZORDER

#ifdef COMMONACE_ANGLE
long ExtObject::cmn_cCompareAngle(LPVAL params)
{
	// Param 0: Combo (Comparison)
	// Param 1: Number (Angle)
	float r = params[1].GetFloat();

	return DoComparison(params[0].GetInt(), info.angle, r);
}

long ExtObject::cmn_aSetAngle(LPVAL params)
{
	info.angle = GetFloatParam(params, 0);

	if (info.angle < 0.0f)
		info.angle += 360.0f;
	if (info.angle >= 360.0f)
		info.angle -= 360.0f;

	pRuntime->UpdateBoundingBox(this);
	return 0;
}

long ExtObject::cmn_aRotateClockwise(LPVAL params)
{
	info.angle += params[0].GetFloat();

	if (info.angle >= 360.0f)
		info.angle -= 360.0f;

	pRuntime->UpdateBoundingBox(this);
	return 0;
}

long ExtObject::cmn_aRotateCounterwise(LPVAL params)
{
	info.angle -= params[0].GetFloat();

	if (info.angle < 0.0f)
		info.angle += 360.0f;

	pRuntime->UpdateBoundingBox(this);
	return 0;
}

long ExtObject::cmn_aAngleToObject(LPVAL params)
{
	CRunObject* pTarget = params[0].GetPairedObjectParam(pRuntime, this);
	if (pTarget == NULL) return 0;

	float x1 = info.x;
	float y1 = info.y;
	float x2 = pTarget->info.x;
	float y2 = pTarget->info.y;

	info.angle = DEGREES(atan2(y2 - y1, x2 - x1));
	pRuntime->UpdateBoundingBox(this);

	return 0;
}

long ExtObject::cmn_aAngleToPosition(LPVAL params)
{
	float x1 = info.x;
	float y1 = info.y;
	float x2 = params[0].GetFloat();
	float y2 = params[1].GetFloat();

	info.angle = DEGREES(atan2(y2 - y1, x2 - x1));
	pRuntime->UpdateBoundingBox(this);

	return 0;
}

long ExtObject::cmn_eGetAngle(LPVAL params, ExpReturn& ret)
{
	return ret = info.angle;
}
#endif // COMMONACE_ANGLE

#ifdef COMMONACE_VISIBILITY
long ExtObject::cmn_cIsVisible(LPVAL params)
{
	return info.visible;
}
long ExtObject::cmn_aSetVisible(LPVAL params)
{
	info.visible = params[0].GetInt()!=0;
	return 0;
}
#endif // COMMONACE_VISIBILITY

#ifdef COMMONACE_OPACITY
long ExtObject::cmn_cCompareOpacity(LPVAL params)
{
	// Param 0: Combo (Comparison)
	// Param 1: Number (Opacity)
	double r = params[1].GetDouble();

	return DoComparison(params[0].GetInt(), info.pInfo->filter.a * 100.0, r);
}

long ExtObject::cmn_aSetOpacity(LPVAL params)
{
	float p = params[0].GetFloat();

	Clamp(p, 0.0f, 100.0f);

	// User entering a percentage, eg. 50 not 0.5.
	info.pInfo->filter.a = p / 100.0f;

	return 0;
}

long ExtObject::cmn_eGetOpacity(LPVAL params, ExpReturn& ret)
{ 
	return ret = info.pInfo->filter.a * 100.0f;
}
#endif // COMMONACE_OPACITY

#ifdef COMMONACE_FILTER
long ExtObject::cmn_cCompareFilter(LPVAL params)
{
	COLORREF r = params[1].GetInt();

	return DoComparison(params[0].GetInt(), info.pInfo->filter.getCOLORREF(), r);
}

long ExtObject::cmn_aSetFilter(LPVAL params)
{
	COLORREF cr = params[0].GetInt();
	cr::color rgb(cr);
	info.pInfo->filter.set_rgb(rgb.get_rgb());

	return 0;
}

long ExtObject::cmn_eGetFilter(LPVAL params, ExpReturn& ret)
{ 
	return ret = __int64(info.pInfo->filter.getCOLORREF());
}
#endif // COMMONACE_FILTER

#ifdef COMMONACE_IDS
long ExtObject::cmn_cCompareUID(LPVAL params)
{
	return params[0].GetInt() == info.uid;
}
long ExtObject::cmn_eGetOID(LPVAL params, ExpReturn &ret)
{
	return ret = info.oid;
}
long ExtObject::cmn_eGetUID(LPVAL params, ExpReturn &ret)
{
	return ret = info.uid;
}
#endif

#ifdef COMMONACE_COUNT_DESTROY
long ExtObject::cmn_aDestroy(LPVAL params)
{
	pRuntime->DestroyObject(this);
	return 0;
}
long ExtObject::cmn_eGetCount(LPVAL params, ExpReturn& ret)
{
	CRunObject** unused;
	int count;
	pRuntime->GetTypeInstances(pType, unused, count);
	return ret = count;
}
#endif

#ifdef COMMONACE_Z_ELEVATION
long ExtObject::cmn_cCompareZElevation(LPVAL params)
{
	float cmpZ = params[1].GetFloat();
	
	return DoComparison(params[0].GetInt(), info.pInfo->z_elevation, cmpZ);
}

long ExtObject::cmn_aSetZElevation(LPVAL params)
{
	info.pInfo->z_elevation = params[0].GetFloat();

	if (info.pInfo->z_elevation == 0.0f)
		info.pDisplayBoxOverride = NULL;		// Use ordinary bounding box

	// Need custom bounding box calculator - runtime works this out for us
	else
		info.pDisplayBoxOverride = &cmn_custombox;

	return 0;
}

long ExtObject::cmn_eGetZElevation(LPVAL params, ExpReturn& ret)
{
	return ret = info.pInfo->z_elevation;
}
#endif

#ifdef COMMONACE_PRIVATE_VARIABLES

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

long ExtObject::cValueCmp(LPVAL params)
{
	const ExpStore& l = privateVars[params[0].GetVariableIndex(pRuntime, pType)];
	const ExpReturn& r = params[2];

	return DoComparison(params[1].GetInt(), (const ExpBase&)l, (const ExpBase&)r);
}

long ExtObject::eGetValue(LPVAL params, ExpReturn& ret)
{
	return ret.ReturnCustom(pRuntime, privateVars[params[0].GetVariableIndex(pRuntime, pType)]);
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
long ExtObject::aSetValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] = params[1];

	return 0;
}


#endif