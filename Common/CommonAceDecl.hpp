///////////////////////////
// COMMON ACE DECLERATIONS

// COMMONACE_POSITION	: Add position aces such as set X, set Y, compare X etc
// COMMONACE_Z_ELEVATION: Add set/get/compare Z elevation ACEs
// COMMONACE_DIMENSION	: Add dimension aces such as set W, compare H etc
// COMMONACE_ZORDER		: Add Z order aces such as move to front, move to layer
// COMMONACE_ANGLE		: Add angle aces such as compare angle, set angle, get angle etc
// COMMONACE_VISIBILITY : Add is visible/set visible condition and action
// COMMONACE_OPACITY	: Add compare/set/get opacity action, condition and expression, via info.filter
// COMMONACE_FILTER		: Add compare/set/get filter action, condition and expression, via info.filter
// COMMONACE_IDS		: Add compare UID, get OID and UID
// COMMONACE_COUNT_DESTROY : Add a .count expression, and a destroy action
// COMMONACE_PRIVATE_VARIABLES : Adds set, addto, subtract, compare, get actions for private variables

///////////////////////////
// POSITIONING ACES

#ifdef COMMONACE_PRIVATE_VARIABLES
vector<ExpStore> privateVars;
// function declarations come later...
#endif

#ifdef COMMONACE_POSITION
long cmn_cXCmp(LPVAL params);
long cmn_cYCmp(LPVAL params);
long cmn_cIsOnscreen(LPVAL params);
long cmn_cOutsideLayout(LPVAL params);
long cmn_aSetX(LPVAL params);
long cmn_aSetY(LPVAL params);
long cmn_aSetPosition(LPVAL params);
long cmn_aSetPositionToObject(LPVAL params);
long cmn_aMovePosition(LPVAL params);
long cmn_eGetX(LPVAL params, ExpReturn& ret);
long cmn_eGetY(LPVAL params, ExpReturn& ret);

ExpStore cmn_xyRet[2];	// For .xy returns

long cmn_eGetXY(LPVAL params, ExpReturn &ret);
long cmn_eGetLeftEdge(LPVAL params, ExpReturn &ret);
long cmn_eGetTopEdge(LPVAL params, ExpReturn &ret);
long cmn_eGetRightEdge(LPVAL params, ExpReturn &ret);
long cmn_eGetBottomEdge(LPVAL params, ExpReturn &ret);

long cCompare2Values(LPVAL theParams);
long cEvaluate(LPVAL theParams);
#endif //COMMONACE_POSITION

#ifdef COMMONACE_DIMENSION
long cmn_cCompareWidth(LPVAL params);
long cmn_cCompareHeight(LPVAL params);
long cmn_aSetWidth(LPVAL params);
long cmn_aSetHeight(LPVAL params);
long cmn_eGetWidth(LPVAL params, ExpReturn& ret);
long cmn_eGetHeight(LPVAL params, ExpReturn& ret);
#endif // COMMONACE_DIMENSION

#ifdef COMMONACE_ZORDER
long cmn_cIsOnLayer(LPVAL params);
long cmn_cPickTopmost(LPVAL params);
long cmn_cPickBottommost(LPVAL params);
long cmn_aZToFront(LPVAL params);
long cmn_aZToBack(LPVAL params);
long cmn_aZInFrontOf(LPVAL params);
long cmn_aZBehindOf(LPVAL params);
long cmn_aMoveToLayer(LPVAL params);
long cmn_eGetLayer(LPVAL params, ExpReturn& ret);
long cmn_eGetZIndex(LPVAL params, ExpReturn& ret);
#endif // COMMONACE_ZORDER

#ifdef COMMONACE_ANGLE
long cmn_cCompareAngle(LPVAL params);
long cmn_aSetAngle(LPVAL params);
long cmn_aRotateClockwise(LPVAL params);
long cmn_aRotateCounterwise(LPVAL params);
long cmn_aAngleToObject(LPVAL params);
long cmn_aAngleToPosition(LPVAL params);
long cmn_eGetAngle(LPVAL params, ExpReturn& ret);
#endif // COMMONACE_ANGLE

#ifdef COMMONACE_VISIBILITY
long cmn_cIsVisible(LPVAL params);
long cmn_aSetVisible(LPVAL params);
#endif // COMMONACE_VISIBILITY

#ifdef COMMONACE_OPACITY
long cmn_cCompareOpacity(LPVAL params);
long cmn_aSetOpacity(LPVAL params);
long cmn_eGetOpacity(LPVAL params, ExpReturn& ret);
#endif // COMMONACE_OPACITY

#ifdef COMMONACE_FILTER
long cmn_cCompareFilter(LPVAL params);
long cmn_aSetFilter(LPVAL params);
long cmn_eGetFilter(LPVAL params, ExpReturn& ret);
#endif // COMMONACE_FILTER

#ifdef COMMONACE_IDS
long cmn_cCompareUID(LPVAL params);
long cmn_eGetOID(LPVAL params, ExpReturn &ret);
long cmn_eGetUID(LPVAL params, ExpReturn &ret);
#endif

#ifdef COMMONACE_COUNT_DESTROY
long cmn_aDestroy(LPVAL params);
long cmn_eGetCount(LPVAL params, ExpReturn& ret);
#endif

#ifdef COMMONACE_Z_ELEVATION
long cmn_cCompareZElevation(LPVAL params);
long cmn_aSetZElevation(LPVAL params);
long cmn_eGetZElevation(LPVAL params, ExpReturn& ret);

RECTF cmn_custombox;	// Custom bounding box
#endif

#ifdef COMMONACE_PRIVATE_VARIABLES

long cPickLowestVar(LPVAL params);
long cPickHighestVar(LPVAL params);
long cValueCmp(LPVAL params);

long eGetValue(LPVAL params, ExpReturn& ret);

long aAddValue(LPVAL params);
long aSubValue(LPVAL params);
long aSetValue(LPVAL params);


#endif