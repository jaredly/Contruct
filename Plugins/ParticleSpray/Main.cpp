// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

// Note: earlier versions no longer use this function as there was no version number in the serialize
void ParticleParameters::Serialize(bin& ar)
{
	int Version = 6;
	SerializeVersion(ar, Version);

	if (ar.loading) {
		ar >> sprayCone >> particleSize >> color >> opacity >> speed >> acc >> destroyMode >> lifeTime
			>> angleRandomiser >> rate >> xRandom >> yRandom >> speedRandom >> gravity >> colorRandom
			>> opacityRandom >> fadeoutTime >> fadeColor >> fadeColorTime;

		if(Version >= 2)
			ar >> particleDisplayAngle >> daRandom >> daTurnRandom >> sizeRandom >> gravityangle >> daTurn >> grow >> growRandom;
		if (Version >= 3)
			ar >> oneShot;
		if (Version >= 5)
			ar >> initialSpeedRandom;
		if(Version >= 6)
			ar >> renderAdditive;

		gravdx = cos(RADIANS(gravityangle)) * gravity;
		gravdy = sin(RADIANS(gravityangle)) * gravity;
	}
	else {
		ar << sprayCone << particleSize << color << opacity << speed << acc << destroyMode << lifeTime
			<< angleRandomiser << rate << xRandom << yRandom << speedRandom << gravity << colorRandom
			<< opacityRandom << fadeoutTime << fadeColor << fadeColorTime

			<< particleDisplayAngle << daRandom << daTurnRandom << sizeRandom << gravityangle << daTurn << grow << growRandom
			<< oneShot
			<< initialSpeedRandom
			<< renderAdditive;
	}
}

// If run only
#ifdef RUN_ONLY

#include "..\..\Common\CommonAceDef.hpp"


//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cTrigger(LPVAL params)
{
	return true;
}

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
	pRuntime->Select(bestObj);
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
	pRuntime->Select(bestObj);
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
	pRuntime->Select(objs[index]);

	return true;	// We selected something.
}

long ExtObject::cOn(LPVAL params)
{
	return on;
}

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aSetValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] = params[1];

	return 0;
}

float RotateTowards(float start, float end, float step)
{
   float cs = cos(RADIANS(start));
   float ce = cos(RADIANS(end));
   float ss = sin(RADIANS(start));
   float se = sin(RADIANS(end));

   float z = cs*se - ss*ce;
   float dot = cs*ce + ss*se;
   float angle = DEGREES(acos(dot));
  
   if(angle > step)
      if(z > 0)
         return start + step;
      else
         return start - step;
   else
      return end;
}

long ExtObject::aRotateToObject(LPVAL params)
{
	CRunObject* pTarget = params[0].GetPairedObjectParam(pRuntime, this);
	if (pTarget == NULL) return 0;

	float x1 = info.x;
	float y1 = info.y;
	float x2 = pTarget->info.x;
	float y2 = pTarget->info.y;

	float targetAngle = DEGREES(atan2(y2 - y1, x2 - x1));
	
	info.angle = RotateTowards(info.angle, targetAngle, params[1].GetFloat());
	pRuntime->UpdateBoundingBox(this);

	return 0;
}

long ExtObject::aRotateToPosition(LPVAL params)
{
	float x1 = info.x;
	float y1 = info.y;
	float x2 = params[0].GetFloat();
	float y2 = params[1].GetFloat();

	float targetAngle = DEGREES(atan2(y2 - y1, x2 - x1));
	
	info.angle = RotateTowards(info.angle, targetAngle, params[2].GetFloat());
	pRuntime->UpdateBoundingBox(this);

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

long ExtObject::aSetSprayCone(LPVAL params)
{
	pp.sprayCone = RADIANS(params[0].GetFloat());
	return 0;
}

long ExtObject::aSetParticleSize(LPVAL params)
{
	pp.particleSize = params[0].GetFloat();
	return 0;
}	

long ExtObject::aSetSpeed(LPVAL params)
{
	pp.speed = params[0].GetFloat();
	return 0;
}
long ExtObject::aSetAcc(LPVAL params)
{
	pp.acc = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetLifetime(LPVAL params)
{
	pp.lifeTime = params[0].GetInt();
	return 0;
}
long ExtObject::aSetAngleRandomiser(LPVAL params)
{
	pp.angleRandomiser = params[0].GetFloat();
	return 0;
}
long ExtObject::aSetRate(LPVAL params)
{
	pp.rate = params[0].GetInt();
	return 0;
}
long ExtObject::aSetXRandom(LPVAL params)
{
	pp.xRandom = params[0].GetFloat();
	return 0;
}
long ExtObject::aSetYRandom(LPVAL params)
{
	pp.yRandom = params[0].GetFloat();
	return 0;
}
long ExtObject::aSetSpeedRandom(LPVAL params)
{
	pp.speedRandom = params[0].GetFloat();
	return 0;
}
long ExtObject::aSetGravity(LPVAL params)
{
	pp.gravity = params[0].GetFloat();
	pp.gravdx = cos(RADIANS(pp.gravityangle))* pp.gravity;
	pp.gravdy = sin(RADIANS(pp.gravityangle))* pp.gravity;
	return 0;
}
long ExtObject::aSetFadeoutTime(LPVAL params)
{
	pp.fadeoutTime = params[0].GetInt();
	fadeTimef = pp.fadeoutTime;
	return 0;
}
long ExtObject::aSetOn(LPVAL params)
{
	on = params[0].GetBool();
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

long ExtObject::eNumParticles(LPVAL params, ExpReturn& ret)
{
	return ret = particles.size();
}

long ExtObject::eSprayCone(LPVAL params, ExpReturn& ret)
{
	return ret = pp.sprayCone;
}

long ExtObject::eParticleSize(LPVAL params, ExpReturn& ret)
{
	return ret = pp.particleSize;
}

long ExtObject::eSpeed(LPVAL params, ExpReturn& ret)
{
	return ret = pp.speed;
}

long ExtObject::eAcc(LPVAL params, ExpReturn& ret)
{
	return ret = pp.acc;
}

long ExtObject::eLifetime(LPVAL params, ExpReturn& ret)
{
	return ret = pp.lifeTime;
}

long ExtObject::eAngleRandomiser(LPVAL params, ExpReturn& ret)
{
	return ret = pp.angleRandomiser;
}

long ExtObject::eRate(LPVAL params, ExpReturn& ret)
{
	return ret = pp.rate;
}

long ExtObject::eXRandom(LPVAL params, ExpReturn& ret)
{
	return ret = pp.xRandom;
}

long ExtObject::eYRandom(LPVAL params, ExpReturn& ret)
{
	return ret = pp.yRandom;
}

long ExtObject::eSpeedRandom(LPVAL params, ExpReturn& ret)
{
	return ret = pp.speedRandom;
}

long ExtObject::eGravity(LPVAL params, ExpReturn& ret)
{
	return ret = pp.gravity;
}

long ExtObject::eOpacityRandom(LPVAL params, ExpReturn& ret)
{
	return ret = pp.opacityRandom;
}

long ExtObject::eFadeoutTime(LPVAL params, ExpReturn& ret)
{
	return ret = pp.fadeoutTime;
}

long ExtObject::eFadeColorTime(LPVAL params, ExpReturn& ret)
{
	return ret = pp.fadeColorTime;
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



	// PRIVATE VARIABLES
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to test.");
	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value", "Value to compare to");
	ADDCND("*Compare a private variable", "Private variables", "%o Value %0 %1 %2", &ExtObject::cValueCmp, "CompareValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Variable name", "Variable to find lowest value in.");
	ADDCND("Pick object with lowest variable", "Private variables", "Pick %o with lowest '%0'", &ExtObject::cPickLowestVar, "PickLowestVar", CND_STATIC);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Variable name.", "Variable to find highest value in.");
	ADDCND("Pick object with highest variable", "Private variables", "Pick %o with highest '%0'", &ExtObject::cPickHighestVar, "PickHighestVar", CND_STATIC);

	ADDCND("Pick a random object", "Pick", "Pick a random %o", &ExtObject::cPickRandom, "PickRandom", CND_STATIC);

	ADDCND("Particle spray is enabled", "Particles", "%o Particle spray enabled", &ExtObject::cOn, "On", 0);

	/////////////////////////////
	// Actions
	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to store");
	ADDACT("*Set value", "Private variables", "Set %0 to %1", &ExtObject::aSetValue, "SetValue", 0);

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

	ADDPARAM(PARAM_VALUE, "Spray cone", "The cone of spray, in degrees");
	ADDACT("Set spray cone", "Particles", "Set spray cone to %0 degrees", &ExtObject::aSetSprayCone, "SetSprayCone", 0);

	ADDPARAM(PARAM_VALUE, "Particle size", "Particle size, in pixels");
	ADDACT("Set particle size", "Particles", "Set particle size to %0", &ExtObject::aSetParticleSize, "SetParticleSize", 0);

	ADDPARAM(PARAM_VALUE, "Particle speed", "Particle speed, in pixels per second");
	ADDACT("Set speed", "Particles", "Set speed to %0", &ExtObject::aSetSpeed, "SetSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Particle acceleration", "Particle acceleration, in pixels per second per second");
	ADDACT("Set acceleration", "Particles", "Set acceleration to %0", &ExtObject::aSetAcc, "SetAcc", 0);

	ADDPARAM(PARAM_VALUE, "Particle timeout", "Particle timeout, in milliseconds");
	ADDACT("Set timeout", "Particles", "Set timeout to %0", &ExtObject::aSetLifetime, "SetTimeout", 0);

	ADDPARAM(PARAM_VALUE, "Angle randomiser", "Angle randomiser, in degrees");
	ADDACT("Set angle randomiser", "Particles", "Set angle randomiser to %0", &ExtObject::aSetAngleRandomiser, "SetAngleRandomiser", 0);

	ADDPARAM(PARAM_VALUE, "Spawn rate", "The number of particles to create every second.");
	ADDACT("Set rate", "Particles", "Set rate to %0 particles per second", &ExtObject::aSetRate, "SetRate", 0);

	ADDPARAM(PARAM_VALUE, "X Randomiser", "Particle X co-ordinate randomiser, pixels");
	ADDACT("Set X randomiser", "Particles", "Set X randomiser to %0", &ExtObject::aSetXRandom, "SetXRandom", 0);

	ADDPARAM(PARAM_VALUE, "Y Randomiser", "Particle Y co-ordinate randomiser, pixels");
	ADDACT("Set Y randomiser", "Particles", "Set Y randomiser to %0", &ExtObject::aSetYRandom, "SetYRandom", 0);

	ADDPARAM(PARAM_VALUE, "Speed Randomiser", "Particle speed randomiser");
	ADDACT("Set speed randomiser", "Particles", "Set speed randomiser to %0", &ExtObject::aSetSpeedRandom, "SetSpeedRandom", 0);

	ADDPARAM(PARAM_VALUE, "Gravity", "Downwards acceleration of particles, in pixels per second per second");
	ADDACT("Set gravity", "Particles", "Set gravity to %0", &ExtObject::aSetGravity, "SetGravity", 0);

	ADDPARAM(PARAM_VALUE, "Fadeout time", "Time for particles to fade to 0 opacity, in milliseconds");
	ADDACT("Set fadeout time", "Particles", "Set fadeout time to %0", &ExtObject::aSetFadeoutTime, "SetFadeoutTime", 0);

	ADDPARAMCOMBO("Particle spray enabled?", "Choose whether spray is on or off.", "Off|On");
	ADDACT("Set spray enabled", "Particles", "Set spray %0", &ExtObject::aSetOn, "SetEnabled", 0);

	/////////////////////////////
	// Expressions

	// ADDEXP(menuID, routine, expressionName, displaytext, flags)
	// Same format as conditions, except with the expression editor text.
	// expressionName = the name in the expression editor, e.g "Width" would
	// associate this expression when the user types YourObject.Width
	// Flags:  RETURN_VALUE, RETURN_STRING, RETURN_UNDEFINED
	ADDPARAM(PARAM_PRIVATEVARIABLE, "'Variable name'", "Name of value.");
	ADDEXP("*Get private variable", "Private variables", "Value", &ExtObject::eGetValue, 0);

	ADDEXP("Get private variable count", "Private variables", "NumVars", &ExtObject::eNumPVs, RETURN_VALUE);

	ADDEXP("Get particle count", "Particles", "ParticleCount", &ExtObject::eNumParticles, RETURN_VALUE);

	ADDEXP("Get spray cone", "Particles", "SprayCone", &ExtObject::eSprayCone, RETURN_VALUE);
	ADDEXP("Get particle size", "Particles", "ParticleSize", &ExtObject::eParticleSize, RETURN_VALUE);
	ADDEXP("Get speed", "Particles", "Speed", &ExtObject::eSpeed, RETURN_VALUE);
	ADDEXP("Get acceleration", "Particles", "Acc", &ExtObject::eAcc, RETURN_VALUE);
	ADDEXP("Get timeout", "Particles", "Timeout", &ExtObject::eLifetime, RETURN_VALUE);
	ADDEXP("Get angle randomiser", "Particles", "AngleRandomiser", &ExtObject::eAngleRandomiser, RETURN_VALUE);
	ADDEXP("Get rate", "Particles", "Rate", &ExtObject::eRate, RETURN_VALUE);
	ADDEXP("Get X randomiser", "Particles", "XRandomiser", &ExtObject::eXRandom, RETURN_VALUE);
	ADDEXP("Get Y randomiser", "Particles", "YRandomiser", &ExtObject::eYRandom, RETURN_VALUE);
	ADDEXP("Get speed randomiser", "Particles", "SpeedRandomiser", &ExtObject::eSpeedRandom, RETURN_VALUE);
	ADDEXP("Get gravity", "Particles", "Gravity", &ExtObject::eGravity, RETURN_VALUE);
	ADDEXP("Get opacity randomiser", "Particles", "OpacityRandomiser", &ExtObject::eOpacityRandom, RETURN_VALUE);
	ADDEXP("Get fadeout time", "Particles", "FadeoutTime", &ExtObject::eFadeoutTime, RETURN_VALUE);
	ADDEXP("Get color fade time", "Particles", "FadeColorTime", &ExtObject::eFadeColorTime, RETURN_VALUE);


#include "..\..\Common\CommonAceTable.hpp"


}
