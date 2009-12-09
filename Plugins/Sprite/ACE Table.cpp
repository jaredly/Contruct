// Include StdAfx
#include "StdAfx.h"

// Include any used common ACE definitions.
#ifdef RUN_ONLY
#include "..\..\Common\CommonAceDef.hpp"
#endif

//////////////////////////////////////////////////////////////////////////////////
// ACE table
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	/////////////////////////////
	// Conditions

	// COLLISIONS
	ADDPARAM(PARAM_OBJECT, "Object name", "Name of the object");
	ADDCND("Is overlapping another object", "Collisions", "%n %o overlaps %0", &ExtObject::cOverlap, "Overlaps", SOL_MODIFIER | CND_STATIC);

	ADDPARAM(PARAM_OBJECT, "Object name", "Name of the object");
	ADDCND("*On collision with another object", "Collisions", "On collision between %n %o and %0", &ExtObject::cOnCollide, "OnCollide", SOL_MODIFIER);

	// PRIVATE VARIABLES
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to test.");
	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value", "Value to compare to");
	ADDCND("*Compare a private variable", "Private variables", "%o %n:  Value %0 %1 %2", &ExtObject::cValueCmp, "CompareValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Variable name", "Variable to find lowest value in.");
	ADDCND("Pick object with lowest variable", "Private variables", "%o %n: Pick with lowest '%0'", &ExtObject::cPickLowestVar, "PickLowestVar", CND_STATIC);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Variable name.", "Variable to find highest value in.");
	ADDCND("Pick object with highest variable", "Private variables", "%o %n: Pick with highest '%0'", &ExtObject::cPickHighestVar, "PickHighestVar", CND_STATIC);

	ADDCND("Pick a random object", "Pick", "%o %n: Pick one at random", &ExtObject::cPickRandom, "PickRandom", CND_STATIC);

	// index = 6
	ADDPARAM(PARAM_STRING, "Animation name", "Enter the name of the animation.");
	ADDCND("On animation finished", "Animations", "%o %n: Animation %0 finished", &ExtObject::cOnAnimationFinish, "AnimFinished", CND_TRIGGERED);

	// index = 7
	ADDCND("On any animation finished", "Animations", "%o %n: Any animation finished", &ExtObject::cTrigger, "AnyAnimFinished", CND_TRIGGERED);

	ADDPARAM(PARAM_STRING, "Animation name", "Enter the name of the animation.");
	ADDCND("An animation is playing", "Animations", "%o %n: Animation %0 is playing", &ExtObject::cAnimationPlaying, "AnimPlaying", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAMDEF(PARAM_VALUE, "Animation frame", "Animation frame to compare to", "1");
	ADDCND("Compare animation frame", "Animations", "%o %n: Animation frame %0 %1", &ExtObject::cCompareAnimFrame, "CompareAnimFrame", 0);

	ADDPARAM(PARAM_OBJECT, "Object name", "Name of the object");
	ADDPARAM(PARAM_VALUE, "X Offset", "The number of pixels on the x axis to advance this object by");
	ADDPARAM(PARAM_VALUE, "Y Offset", "The number of pixels on the y axis to advance this object by");
	ADDCND("Is overlapping at offset", "Collisions", "%o %n: overlaps %0 : offset (%1,%2)", &ExtObject::cOverlapAdvanced, "OverlapsOffset", SOL_MODIFIER);

	ADDPARAM(PARAM_VALUE, "X position", "The X position to find closest for");
	ADDPARAM(PARAM_VALUE, "Y position", "The Y position to find closest for");
	ADDCND("Pick closest", "Pick", "%o %n: Pick closest to: %0, %1", &ExtObject::cPickClosest, "PickClosest", CND_STATIC);

	ADDPARAM(PARAM_VALUE, "X position", "The X position to find farthest for");
	ADDPARAM(PARAM_VALUE, "Y position", "The Y position to find farthest for");
	ADDCND("Pick farthest", "Pick", "%o %n: Pick farthest from: %0, %1", &ExtObject::cPickFarthest, "PickFarthest", CND_STATIC);

	ADDCND("On animation frame change", "Animations", "%o %n: On animation frame change", &ExtObject::cOnFrameChange, "OnAnimFrameChange", CND_TRIGGERED);

	// index = 14
	ADDCND("On flash finished", "Appearance", "%o %n: On flash finished", &ExtObject::cTrigger, "OnFlashFinished", CND_TRIGGERED);

	/////////////////////////////
	// Actions
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to store");
	ADDACT("*Set value", "Private variables", "Set %0 to %1", &ExtObject::aSetValue, "SetValue", 0);

	ADDPARAM(PARAM_OBJECT,"Object","Select the object to fire out.");
	ADDPARAMDEF(PARAM_VALUE, "Layer", "Layer name or number, to create the object on.", "1");
	ADDPARAM(PARAM_VALUE, "Image point name or number (optional)", "Leave 0 to use object's position, or enter an image point's name or number.");
	ADDACT("Spawn another object", "Creation", "Spawn object %0 on layer %1 (image point %2)", &ExtObject::aShootObject, "SpawnObject", SOL_MODIFIER);

	ADDPARAM(PARAM_OBJECT, "Object", "Select the object to rotate towards.");
	ADDPARAM(PARAM_VALUE, "Rotation amount", "Number of degrees to rotate towards the object.");
	ADDACT("Rotate towards another object", "Angle", "Rotate %1 degrees toward %0", &ExtObject::aRotateToObject, "RotateToObject", 0);

	ADDPARAM(PARAM_VALUE, "X co-ordinate", "X co-ordinate to rotate towards, in pixels, eg. MouseX");
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "Y co-ordinate to rotate towards, in pixels, eg. MouseY");
	ADDPARAM(PARAM_VALUE, "Rotation amount", "Number of degrees to rotate towards the position.");
	ADDACT("Rotate towards a position", "Angle", "Rotate %2 degrees toward (%0, %1)", &ExtObject::aRotateToPosition, "RotateToPosition", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to add.");
	ADDACT("Add to value", "Private variables", "Add %1 to %0", &ExtObject::aAddValue, "AddValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to subtract.");
	ADDACT("Subtract from value", "Private variables", "Subtract %1 from %0", &ExtObject::aSubValue, "SubValue", 0);

	ADDPARAMDEF(PARAM_VALUE, "Interval", "Flash interval in seconds - the time between toggling visibility.", "0.1");
	ADDPARAMDEF(PARAM_VALUE, "Duration", "Duration of flash in seconds.", "2");
	ADDACT("Flash", "Appearance", "Flash for %1 seconds with %0 seconds interval", &ExtObject::aFlash, "Flash", 0);

	ADDACT("Play animation", "Animation", "Play current animation", &ExtObject::aPlayAnimation, "PlayAnim", 0);
	ADDACT("Pause animation", "Animation", "Pause current animation", &ExtObject::aPauseAnimation, "PauseAnim", 0);

	ADDPARAM(PARAM_STRING, "Name", "Enter the name of the animation or sub animation");
	ADDACT("Set animation", "Animation", "Set animation to %0", &ExtObject::aSetAnimation, "SetAnim", 0);

	ADDPARAMCOMBO("Collision mode", "Select the collision algorithm, with speed/accuracy tradeoffs.", "None|Point|Bounding box|Per Pixel");
	ADDACT("Set collision mode", "Collisions", "Set collision mode to %0", &ExtObject::aSetCollMode, "SetCollisionMode", 0);

	ADDPARAM(PARAM_STRING, "File name", "Enter the path to the image file to load.");
	ADDACT("Load animation frame from file", "Animation", "Load frame from %0", &ExtObject::aLoadFrameFromFile, "LoadFrameFromFile", 0);

	ADDPARAM(PARAM_VALUE, "X co-ordinate", "X co-ordinate of the new pivot point");
	ADDPARAM(PARAM_VALUE, "Y co-ordinate", "Y co-ordinate of the new pivot point");
	ADDACT("Set current frame pivot point", "Animation", "Set current frame pivot point to (%0, %1)", &ExtObject::aSetFramePivotPoint, "SetPivotPoint", 0);

	ADDPARAM(PARAM_VALUE, "Frame number", "Animation frame number to change to, starting at 1.");
	ADDACT("Set animation frame", "Animation", "Set animation frame to %0", &ExtObject::aSetAnimFrame, "SetAnimFrame", 0);

	ADDPARAMCOMBO("Corner", "Select the corner of the image", "Top-left|Top-right|Bottom-left|Bottom-right");
	ADDPARAMDEF(PARAM_COLOR, "Colour", "Colour", "16777215");
	ADDPARAMDEF(PARAM_VALUE, "Opacity", "0 - 100", "100");
	ADDACT("Set corner filter", "Appearance", "Set corner %0 colour to %1 (%2%)", &ExtObject::aSetVertex, "SetCornerFilter", 0);

	ADDPARAM(PARAM_VALUE, "Horizontal skew", "Horizontal skew, in pixels");
	ADDACT("Set horizontal skew", "Appearance", "Set horizontal skew to %0", &ExtObject::aSetSkewX, "SetSkewX", 0);

	ADDPARAM(PARAM_VALUE, "Vertical skew", "Vertical skew, in pixels");
	ADDACT("Set vertical skew", "Appearance", "Set vertical skew to %0", &ExtObject::aSetSkewY, "SetSkewY", 0);

	ADDPARAM(PARAM_VALUE, "Speed", "Enter the speed of the animation (frames per second)");
	ADDACT("Set animation speed", "Animation", "Set animation speed to %0", &ExtObject::aSetAnimationSpeed, "SetAnimSpeed", 0);

	ADDPARAMDEF(PARAM_VALUE, "Columns", "The number of columns in the distort map.", "1");
	ADDPARAMDEF(PARAM_VALUE, "Rows", "The number of rows in the distort map", "1");
	ADDACT("Set distort map size", "Distort Map", "Set distort map size to %0, %1", &ExtObject::aSetDistortMapSize, "SetDistortMapSize", 0);

	ADDPARAMDEF(PARAM_VALUE, "Column", "Column number", 0);
	ADDPARAMDEF(PARAM_VALUE, "Row", "Row number", 0);
	ADDPARAMDEF(PARAM_VALUE, "X displace", "X displacement", 0);
	ADDPARAMDEF(PARAM_VALUE, "Y displace", "Y displacement", 0);
	ADDPARAMDEF(PARAM_VALUE, "U displace", "U displacement", 0);
	ADDPARAMDEF(PARAM_VALUE, "V displace", "V displacement", 0);
	ADDPARAMDEF(PARAM_COLOR, "Filter", "The colour", "16777215");
	ADDPARAMDEF(PARAM_VALUE, "Opacity", "The opacity", "100");
	ADDPARAMDEF(PARAM_VALUE, "Z position", "Z position", 0);
	ADDACT("Set relative displacement at", "Distort Map", "Set relative displacement at (%0, %1) to XYZ(%2, %3, %8) UV(%4, %5) %7 % %6", &ExtObject::aSetDisplacementAt, "SetDisplacementAt", 0);

	ADDACT("Resume animation", "Animation", "Resume current animation", &ExtObject::aResumeAnimation, "ResumeAnim", 0);

	ADDPARAMDEF(PARAM_VALUE, "Column", "Column number", 0);
	ADDPARAMDEF(PARAM_VALUE, "Row", "Row number", 0);
	ADDPARAMDEF(PARAM_VALUE, "X position", "X position", 0);
	ADDPARAMDEF(PARAM_VALUE, "Y positon", "Y position", 0);
	ADDPARAMDEF(PARAM_VALUE, "U displace", "U position", 0);
	ADDPARAMDEF(PARAM_VALUE, "V displace", "V position", 0);
	ADDPARAMDEF(PARAM_COLOR, "Filter", "The colour", "16777215");
	ADDPARAMDEF(PARAM_VALUE, "Opacity", "The opacity", "100");
	ADDPARAMDEF(PARAM_VALUE, "Z position", "Z position", 0);
	ADDACT("Set absolute displacement at", "Distort Map", "Set absolute displacement at (%0, %1) to XYZ(%2, %3, %8) UV(%4, %5) %7 % %6", &ExtObject::aSetDisplacementRealAt, "SetDisplacementRealAt", 0);

	ADDPARAM(PARAM_VALUE, "Animation angle", "Animation angle to set.  This will change the object angle unless 'Lock animation angles' is disabled.");
	ADDACT("Set animation angle", "Animation", "Set animation angle to %0", &ExtObject::aSetAnimationAngle, "SetAnimAngle", 0);

	ADDPARAM(PARAM_STRING, "Filename to save to", "Enter the full path to a file to store the mesh in.");
	ADDACT("Save mesh to file", "Distort Map", "Save mesh to %0", &ExtObject::aSaveMesh, "SaveMesh", 0);

	ADDPARAM(PARAM_STRING, "Filename to load from", "Enter the full path to a file to load a mesh from.");
	ADDACT("Load mesh from file", "Distort Map", "Load mesh from %0", &ExtObject::aLoadMesh, "LoadMesh", 0);

	/////////////////////////////
	// Expressions

	// ADDEXP(menuID, routine, expressionName, displaytext, flags)
	// Same format as conditions, except with the expression editor text.
	// expressionName = the name in the expression editor, e.g "Width" would
	// associate this expression when the user types YourObject.Width
	// Flags:  RETURN_VALUE, RETURN_STRING, RETURN_UNDEFINED
	ADDPARAM(PARAM_PRIVATEVARIABLE, "'Variable name'", "Name of value.");
	ADDEXP("*Get private variable", "Private variables", "Value", &ExtObject::eGetValue, 0);

	ADDEXP("Get original width", "Size & Position", "OriginalWidth", &ExtObject::eOriginalWidth, RETURN_VALUE);
	ADDEXP("Get original height", "Size & Position", "OriginalHeight", &ExtObject::eOriginalHeight, RETURN_VALUE);

	ADDPARAM(PARAM_UNDEFINED, "Imagepoint", "Image point name or number.");
	ADDEXP("Get image point X", "Misc", "ImagePointX", &ExtObject::eGetImagePointX, RETURN_VALUE);
	ADDPARAM(PARAM_UNDEFINED, "Imagepoint", "Image point name or number.");
	ADDEXP("Get image point Y", "Misc", "ImagePointY", &ExtObject::eGetImagePointY, RETURN_VALUE);

	ADDEXP("Get private variable count", "Private variables", "NumVars", &ExtObject::eNumPVs, RETURN_VALUE);

	ADDEXP("Get horizontal skew", "Appearance", "SkewX", &ExtObject::eSkewX, RETURN_VALUE);
	ADDEXP("Get vertical skew", "Appearance", "SkewY", &ExtObject::eSkewY, RETURN_VALUE);

	ADDEXP("Get animation frame", "Animation", "AnimFrame", &ExtObject::eGetAnimFrame, RETURN_VALUE);
	ADDEXP("Get animation name", "Animation", "AnimName", &ExtObject::eGetAnimName, RETURN_STRING);
	ADDEXP("Get animation frame count", "Animation", "GetAnimationFrameCount", &ExtObject::eGetFrameCount, RETURN_VALUE);

	ADDEXP("Get animation speed", "Animation", "AnimSpeed", &ExtObject::eGetAnimSpeed, RETURN_VALUE);

	ADDEXP("Get mesh columns", "Distort Map", "MeshCols", &ExtObject::eGetMeshCols, RETURN_VALUE);
	ADDEXP("Get mesh rows", "Distort Map", "MeshRows", &ExtObject::eGetMeshRows, RETURN_VALUE);

	ADDPARAM(PARAM_VALUE, "Column", "Column to retrieve from");
	ADDPARAM(PARAM_VALUE, "Row", "Row to retrieve from");
	ADDEXP("Vertex X", "Distort Map", "VertexX", &ExtObject::eGetMeshX, RETURN_VALUE);

	ADDPARAM(PARAM_VALUE, "Column", "Column to retrieve from");
	ADDPARAM(PARAM_VALUE, "Row", "Row to retrieve from");
	ADDEXP("Vertex Y", "Distort Map", "VertexY", &ExtObject::eGetMeshY, RETURN_VALUE);

	ADDPARAM(PARAM_VALUE, "Column", "Column to retrieve from");
	ADDPARAM(PARAM_VALUE, "Row", "Row to retrieve from");
	ADDEXP("Vertex Z", "Distort Map", "VertexZ", &ExtObject::eGetMeshZ, RETURN_VALUE);

	ADDPARAM(PARAM_VALUE, "Column", "Column to retrieve from");
	ADDPARAM(PARAM_VALUE, "Row", "Row to retrieve from");
	ADDEXP("Vertex U", "Distort Map", "VertexU", &ExtObject::eGetMeshU, RETURN_VALUE);

	ADDPARAM(PARAM_VALUE, "Column", "Column to retrieve from");
	ADDPARAM(PARAM_VALUE, "Row", "Row to retrieve from");
	ADDEXP("Vertex V", "Distort Map", "VertexV", &ExtObject::eGetMeshV, RETURN_VALUE);

	ADDPARAM(PARAM_VALUE, "Column", "Column to retrieve from");
	ADDPARAM(PARAM_VALUE, "Row", "Row to retrieve from");
	ADDEXP("Vertex filter", "Distort Map", "VertexFilter", &ExtObject::eGetMeshFilter, RETURN_VALUE);

	ADDPARAM(PARAM_VALUE, "Column", "Column to retrieve from");
	ADDPARAM(PARAM_VALUE, "Row", "Row to retrieve from");
	ADDEXP("Vertex opacity", "Distort Map", "VertexOpacity", &ExtObject::eGetMeshOpacity, RETURN_VALUE);

#include "..\..\Common\CommonAceTable.hpp"


}


