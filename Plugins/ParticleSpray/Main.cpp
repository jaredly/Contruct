// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

// Note: earlier versions no longer use this function as there was no version number in the serialize
void ParticleParameters::Serialize(bin& ar)
{
	int Version = 7;
	SerializeVersion(ar, Version);

	if (ar.loading) {
		ar >> sprayCone >> particleSize >> color >> opacity >> speed >> acc >> destroyMode >> lifeTime
		   >> angleRandomiser;

		if(Version < 7)
		{
			int irate;
			ar >> irate;
			rate = irate;
		}
		else
			ar >> rate;
		
		
		ar  >> xRandom >> yRandom >> speedRandom >> gravity >> colorRandom
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
	pp.rate = params[0].GetFloat();
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
long ExtObject::aSetSpeedSimulationRandom(LPVAL params)
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

//////////////////////////
//
//    NEW ACTIONS
//
/////////////////////////

long ExtObject::aSetOneShot(LPVAL params)
{
	pp.oneShot = params[0].GetBool();
	return 0;
}

long ExtObject::aSetUseTexture(LPVAL params)
{
	useTexture = params[0].GetBool();
	return 0;
}

long ExtObject::aSetRenderAdditive(LPVAL params)
{
	pp.renderAdditive = params[0].GetBool();
	return 0;
}

long ExtObject::aSetDisplayAngle(LPVAL params)
{
	pp.particleDisplayAngle = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetSizeRandomiser(LPVAL params)
{
	pp.sizeRandom = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetDisplayAngleRandomiser(LPVAL params)
{
	pp.daRandom = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetGrowRandomiser(LPVAL params)
{
	pp.growRandom = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetDisplayAngleTurnRandomiser(LPVAL params)
{
	pp.daTurnRandom = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetGravityAngle(LPVAL params)
{
	pp.gravityangle = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetGrow(LPVAL params)
{
	pp.grow = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetDisplayAngleTurnSpeed(LPVAL params)
{
	pp.daTurn = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetOpacityRandomiser(LPVAL params)
{
	pp.opacityRandom = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetOpacity(LPVAL params)
{
	pp.opacity = params[0].GetFloat() / 100.0f;
	return 0;
}

long ExtObject::aSetDestroyMode(LPVAL params)
{
	pp.destroyMode = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetColor(LPVAL params)
{
	pp.color = params[0].GetColorRef();

	col = cr::color(pp.color);

	return 0;
}

long ExtObject::aSetFadeToColor(LPVAL params)
{
	pp.fadeColor = params[0].GetColorRef();
	col_fade = cr::color(pp.fadeColor);

	return 0;
}

long ExtObject::aSetFadeColorTime(LPVAL params)
{
	pp.fadeColorTime = params[0].GetFloat();
	return 0;
}

long ExtObject::aSetSpeedRandom( LPVAL params )
{
	pp.speedRandom = params[0].GetFloat();
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
	return ret = DEGREES(pp.sprayCone);
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

long ExtObject::eSpeedSimulationRandom(LPVAL params, ExpReturn& ret)
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

//////
// NEW EXPRESSIONS
/////


long ExtObject::eOneShot(LPVAL params, ExpReturn& ret)
{
	return ret = pp.oneShot;
}
long ExtObject::eUseTexture(LPVAL params, ExpReturn& ret)
{
	return ret = useTexture;
}
long ExtObject::eRenderAdditive(LPVAL params, ExpReturn& ret)
{
	return ret = pp.renderAdditive;
}
long ExtObject::eDisplayAngle(LPVAL params, ExpReturn& ret)
{
	return ret = pp.particleDisplayAngle;
}
long ExtObject::eOpacity(LPVAL params, ExpReturn& ret)
{
	return ret = pp.opacity * 100;
}

long ExtObject::eSizeRandomiser(LPVAL params, ExpReturn& ret)
{
	return ret = pp.sizeRandom;
}

long ExtObject::eDisplayAngleRandomiser(LPVAL params, ExpReturn& ret)
{
	return ret = pp.daRandom;
}

long ExtObject::eSpeedRandomiser(LPVAL params, ExpReturn& ret)
{
	return ret = pp.speedRandom;
}

long ExtObject::eGrowRandomiser(LPVAL params, ExpReturn& ret)
{
	return ret = pp.growRandom;
}

long ExtObject::eDisplayAngleTurnRandomiser(LPVAL params, ExpReturn& ret)
{
	return ret = pp.growRandom;
}

long ExtObject::eGravityAngle(LPVAL params, ExpReturn& ret)
{
	return ret = pp.gravityangle;
}

long ExtObject::eGrow(LPVAL params, ExpReturn& ret)
{
	return ret = pp.grow;
}

long ExtObject::eDisplayAngleTurn(LPVAL params, ExpReturn& ret)
{
	return ret = pp.daTurn;
}

long ExtObject::eDestroyMode(LPVAL params, ExpReturn& ret)
{
	return ret = pp.destroyMode;
}

long ExtObject::eColor(LPVAL params, ExpReturn& ret)
{
	return ret = (int)pp.color;
}

long ExtObject::eFadeToColor(LPVAL params, ExpReturn& ret)
{
	return ret = (int)pp.fadeColor;

	fadeColorTimef = pp.fadeColorTime;
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
	ADDACT("Set spray cone", "Particle Creation Randomization", "Set spray cone to %0 degrees", &ExtObject::aSetSprayCone, "SetSprayCone", 0);

	ADDPARAM(PARAM_VALUE, "Particle size", "Particle size, in pixels");
	ADDACT("Set particle size", "Particle Creation Settings", "Set particle size to %0", &ExtObject::aSetParticleSize, "SetParticleSize", 0);

	ADDPARAM(PARAM_VALUE, "Particle speed", "Particle speed, in pixels per second");
	ADDACT("Set speed", "Particles", "Set speed to %0", &ExtObject::aSetSpeed, "SetSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Particle acceleration", "Particle acceleration, in pixels per second per second");
	ADDACT("Set acceleration", "Particle Simulation Settings", "Set acceleration to %0", &ExtObject::aSetAcc, "SetAcc", 0);

	ADDPARAM(PARAM_VALUE, "Particle timeout", "Particle timeout, in milliseconds");
	ADDACT("Set timeout", "Particle Life Settings", "Set timeout to %0", &ExtObject::aSetLifetime, "SetTimeout", 0);

	ADDPARAM(PARAM_VALUE, "Angle randomiser", "Angle randomiser, in degrees");
	ADDACT("Set angle randomiser", "Particle Simulation Randomization", "Set angle randomiser to %0", &ExtObject::aSetAngleRandomiser, "SetAngleRandomiser", 0);

	ADDPARAM(PARAM_VALUE, "Spawn rate", "The number of particles to create every second.");
	ADDACT("Set rate", "Particles", "Set rate to %0 particles per second", &ExtObject::aSetRate, "SetRate", 0);

	ADDPARAM(PARAM_VALUE, "X Randomiser", "Particle X co-ordinate randomiser, pixels");
	ADDACT("Set X randomiser", "Particle Creation Randomization", "Set X randomiser to %0", &ExtObject::aSetXRandom, "SetXRandom", 0);

	ADDPARAM(PARAM_VALUE, "Y Randomiser", "Particle Y co-ordinate randomiser, pixels");
	ADDACT("Set Y randomiser", "Particle Creation Randomization", "Set Y randomiser to %0", &ExtObject::aSetYRandom, "SetYRandom", 0);

	ADDPARAM(PARAM_VALUE, "Speed Randomiser", "Particle speed randomiser");
	ADDACT("Set speed simulation randomiser", "Particle Simulation Randomization", "Set speed simulation randomiser to %0", &ExtObject::aSetSpeedSimulationRandom, "SetSpeedSimulationRandom", 0);

	ADDPARAM(PARAM_VALUE, "Gravity", "Downwards acceleration of particles, in pixels per second per second");
	ADDACT("Set gravity", "Particle Simulation Settings", "Set gravity to %0", &ExtObject::aSetGravity, "SetGravity", 0);

	ADDPARAM(PARAM_VALUE, "Fadeout time", "Time for particles to fade to 0 opacity, in milliseconds");
	ADDACT("Set fadeout time", "Particle Life Settings", "Set fadeout time to %0", &ExtObject::aSetFadeoutTime, "SetFadeoutTime", 0);

	ADDPARAMCOMBO("Particle spray enabled?", "Choose whether spray is on or off.", "Off|On");
	ADDACT("Set spray enabled", "Particles", "Set spray %0", &ExtObject::aSetOn, "SetEnabled", 0);

	// NEW ACTIONS

	ADDPARAM(PARAM_VALUE, "Rate", "Number of particles to create per second, or in total for one-shot effects");
	ADDACT("One shot?", "Particles", "Use one shot? %0", &ExtObject::aSetOneShot, "SetOneShot", 0);

	ADDPARAMCOMBO("Use texture?", "Determine whether to use a texture or not", "Yes|No");
	ADDACT("Use texture?", "Particles", "Use texture? %0", &ExtObject::aSetUseTexture, "SetUseTexture", 0);

	ADDPARAMCOMBO("Render additive?", "Determine whether to render each particle in additive mode", "Yes|No");
	ADDACT("Render additive?", "Particles", "Render additive? %0", &ExtObject::aSetRenderAdditive, "SetRenderAdditive", 0);

	ADDPARAM(PARAM_VALUE, "Angle", "Display angle of the new spawned particles, in degrees");
	ADDACT("Set display angle", "Particles", "Set display angle to %0", &ExtObject::aSetDisplayAngle, "SetDisplayAngle", 0);

	ADDPARAM(PARAM_VALUE, "Size", "Number of pixels to randomly add to the particle when they spawn");
	ADDACT("Set size randomiser", "Particle Creation Randomization", "Set size randomiser to %0", &ExtObject::aSetSizeRandomiser, "SetSizeRandomiser", 0);

	ADDPARAM(PARAM_VALUE, "Angle", "Randomisation of the textures angle on creation");
	ADDACT("Set display angle randomiser", "Particle Creation Randomization", "Set display angle randomiser to %0", &ExtObject::aSetDisplayAngleRandomiser, "SetDisplayAngleRandomiser", 0);

	ADDPARAM(PARAM_VALUE, "Grow", "Randomisation of the particle size change, in pixels per second");
	ADDACT("Set grow randomiser", "Particle Creation Randomization", "Set grow randomiser to %0", &ExtObject::aSetGrowRandomiser, "SetGrowRandomiser", 0);

	ADDPARAM(PARAM_VALUE, "Turn Speed", "Randomisation of the textures turning speed on creation");
	ADDACT("Set display angle turn randomiser", "Particle Creation Randomization", "Set display angle turn randomiser to %0", &ExtObject::aSetDisplayAngleTurnRandomiser, "SetDisplayAngleTurnRandomiser", 0);

	ADDPARAM(PARAM_VALUE, "Angle", "Angle of the gravity");
	ADDACT("Set gravity angle", "Particle Simulation Settings", "Set gravity angle to %0", &ExtObject::aSetGravityAngle, "SetGravityAngle", 0);

	ADDPARAM(PARAM_VALUE, "Grow", "Particle size change in pixels per second");
	ADDACT("Set grow", "Particle Simulation Settings", "Set grow to %0", &ExtObject::aSetGrow, "SetGrow", 0);

	ADDPARAM(PARAM_VALUE, "Turn Speed", "Display angle rotation of particles, in degrees per second");
	ADDACT("Set display angle turn speed", "Particle Simulation Settings", "Set display angle turn speed to %0", &ExtObject::aSetDisplayAngleTurnSpeed, "SetDisplayAngleTurnSpeed", 0);

	ADDPARAM(PARAM_VALUE, "Opacity Random", "Maximum opacity adjustment per second");
	ADDACT("Set opacity randomiser", "Particle Life Settings", "Set opacity randomiser to %0", &ExtObject::aSetOpacityRandomiser, "SetOpacityRandomiser", 0);

	ADDPARAMCOMBO("Destroy Mode", "Choose when each particle is destroyed", "Timeout Expired|Particle Stopped|Faded to invisible");
	ADDACT("Set destroy mode", "Particle Life Settings", "Set destroy mode to (%0)", &ExtObject::aSetDestroyMode, "SetDestroyMode", 0);

	ADDPARAM(PARAM_COLOR, "Color", "Initial color of particles");
	ADDACT("Set color", "Particle Life Settings", "Set color to %0", &ExtObject::aSetColor, "SetColor", 0);

	ADDPARAM(PARAM_COLOR, "Color", "Color for particle to fade to over fade colour time");
	ADDACT("Set fade to color", "Particle Life Settings", "Set fade-to color to %0", &ExtObject::aSetFadeToColor, "SetFadeToColor", 0);

	ADDPARAM(PARAM_VALUE, "Opacity", "Percentage");
	ADDACT("Set particle opacity", "Particle Creation Settings", "Set particle opacity to %0", &ExtObject::aSetOpacity, "SetParticleOpacity", 0);

	ADDPARAM(PARAM_VALUE, "Fadeout time", "Time for particles to fade to the second color, in milliseconds");
	ADDACT("Set fade color time", "Particle Life Settings", "Set fade color time to %0", &ExtObject::aSetFadeColorTime, "SetParticleOpacity", 0);

	ADDPARAM(PARAM_VALUE, "Speed", "Randomisation of the particle speed, in pixels per second");
	ADDACT("Set speed randomiser", "Particle Creation Randomization", "Set speed randomiser to %0", &ExtObject::aSetSpeedRandom, "SetSpeedRandomiser", 0);






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

	ADDEXP("Get spray cone", "Particle Creation Randomization", "SprayCone", &ExtObject::eSprayCone, RETURN_VALUE);
	ADDEXP("Get particle size", "Particle Creation Settings", "ParticleSize", &ExtObject::eParticleSize, RETURN_VALUE);
	ADDEXP("Get speed", "Particle Creation Settings", "Speed", &ExtObject::eSpeed, RETURN_VALUE);
	ADDEXP("Get acceleration", "Particle Simulation Settings", "Acceleration", &ExtObject::eAcc, RETURN_VALUE);
	ADDEXP("Get timeout", "Particle Life Settings", "Timeout", &ExtObject::eLifetime, RETURN_VALUE);
	ADDEXP("Get angle randomiser", "Particle Simulation Randomization", "AngleRandomiser", &ExtObject::eAngleRandomiser, RETURN_VALUE);
	ADDEXP("Get rate", "Particles", "Rate", &ExtObject::eRate, RETURN_VALUE);
	ADDEXP("Get X randomiser", "Particle Creation Randomization", "XRandomiser", &ExtObject::eXRandom, RETURN_VALUE);
	ADDEXP("Get Y randomiser", "Particle Creation Randomization", "YRandomiser", &ExtObject::eYRandom, RETURN_VALUE);
	ADDEXP("Get speed simulation randomiser", "Particle Simulation Randomization", "SpeedSimulationRandomiser", &ExtObject::eSpeedSimulationRandom, RETURN_VALUE);
	ADDEXP("Get gravity", "Simulation Settings", "Gravity", &ExtObject::eGravity, RETURN_VALUE);
	ADDEXP("Get opacity randomiser", "Particle Simulation Randomization", "OpacityRandomiser", &ExtObject::eOpacityRandom, RETURN_VALUE);
	ADDEXP("Get fadeout time", "Particle Life Settings", "FadeoutTime", &ExtObject::eFadeoutTime, RETURN_VALUE);
	ADDEXP("Get color fade time", "Particle Life Settings", "FadeColorTime", &ExtObject::eFadeColorTime, RETURN_VALUE);


	ADDEXP("Get is one shot?", "Particles", "OneShot", &ExtObject::eOneShot, RETURN_VALUE);
	ADDEXP("Get is using texture", "Particles", "UseTexture", &ExtObject::eUseTexture, RETURN_VALUE);
	ADDEXP("Get is rendering additive", "Particles", "RenderAdditive", &ExtObject::eRenderAdditive, RETURN_VALUE);
	ADDEXP("Get display angle", "Particle Creation Settings", "DisplayAngle", &ExtObject::eDisplayAngle, RETURN_VALUE);
	ADDEXP("Get particle opacity", "Particle Creation Settings", "ParticleOpacity", &ExtObject::eOpacity, RETURN_VALUE);
	ADDEXP("Get size randomiser", "Particle Creation Randomization", "SizeRandomiser", &ExtObject::eSizeRandomiser, RETURN_VALUE);
	ADDEXP("Get display angle randomiser", "Particle Creation Randomization", "DisplayAngleRandomiser", &ExtObject::eDisplayAngleRandomiser, RETURN_VALUE);
	ADDEXP("Get speed randomiser", "Particle Creation Randomization", "SpeedRandomiser", &ExtObject::eSpeedRandomiser, RETURN_VALUE);
	ADDEXP("Get grow randomiser", "Particle Creation Randomization", "GrowRandomiser", &ExtObject::eGrowRandomiser, RETURN_VALUE);
	ADDEXP("Get display angle turn randomiser", "Particle Creation Randomization", "DisplayAngleTurnRandomiser", &ExtObject::eDisplayAngleTurnRandomiser, RETURN_VALUE);
	ADDEXP("Get gravity angle", "Particle Simulation Settings", "GravityAngle", &ExtObject::eGravityAngle, RETURN_VALUE);
	ADDEXP("Get grow", "Particle Simulation Settings", "Grow", &ExtObject::eGrow, RETURN_VALUE);
	ADDEXP("Get display angle turn", "Particle Simulation Settings", "DisplayAngleTurn", &ExtObject::eDisplayAngleTurn, RETURN_VALUE);
	ADDEXP("Get destroy mode", "Particle Life Settings", "DestroyMode", &ExtObject::eDestroyMode, RETURN_VALUE);
	ADDEXP("Get color", "Particle Life Settings", "Color", &ExtObject::eColor, RETURN_VALUE);
	ADDEXP("Get fade to color", "Particle Life Settings", "FadeToColor", &ExtObject::eFadeToColor, RETURN_VALUE);

#include "..\..\Common\CommonAceTable.hpp"


}
