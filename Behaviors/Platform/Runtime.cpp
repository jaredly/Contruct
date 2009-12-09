// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Only use for class initializer list.  Object initialisation must be done in OnCreate.
// It is not safe to make runtime calls here: do so in OnCreate.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

// This is called just after the constructor when your object has been created.  Construct has set
// up your object here so it is safe to make runtime calls.
void ExtObject::OnCreate()
{
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

//	ar >> acc >> dec >> maxspeed >> jumpstrength >> g >> aircontrol >> jumpSustain;

	
	ar >> floor_acc >> floor_dec >> air_acc >> air_dec >> max_floor_speed >> max_air_speed;
	ar >> max_fall  >> jump_strength >> jump_sustain;
	ar >> grav_fall >> grav_dir;
	grav_jump = grav_fall;
	grav_jump_sustain = 0;
	autochangeangle = false;
	ar >> inv;
	ar >> bunnyhop;	

	if (Version >= 2)
		ar >> CanPressDown;
	if (Version >= 3)
		ar >> player;
	else
		player = 0;
	if (Version >= 4)
	{
		ar >> grav_jump;
		ar >> grav_jump_sustain;
	}
	if(Version >= 5)
	{
		ar >> autochangeangle;
	}

	ar.detach();

	dx = dy = 0.0f;
	lastjump = -9999;


	vk_left = vk_right = vk_jump = vk_down = 0;
//	vk_left = pRuntime->GetControlVK("Move Left", player);
//	vk_right = pRuntime->GetControlVK("Move Right", player);
//	vk_jump = pRuntime->GetControlVK("Jump", player);
//	vk_down = pRuntime->GetControlVK("Move Down", player);

	ignoringInput = false;
	activated = true;
	oldjump = false;

	pStandOnMoving = NULL;

	// Defaults in case user deleted keys
	if (vk_left == -1) vk_left = VK_LEFT;
	if (vk_right == -1) vk_right = VK_RIGHT;
	if (vk_jump == -1) vk_jump = VK_SHIFT;

	// Get the Solid trait for the player to land on
	pObstacles = pRuntime->GetTraitFromName("Solid");
	pPlatforms = pRuntime->GetTraitFromName("Platform");

	standing_on_platform = false;

	// Animations
	CRunAnimation *root = (CRunAnimation*)pLink->GetData(1, NULL);

	if (root)
	{
		stopped = pRuntime->GetPresetAnimation(root, 1);
		walking = pRuntime->GetPresetAnimation(root, 2);
		jumping = pRuntime->GetPresetAnimation(root, 3);
		falling = pRuntime->GetPresetAnimation(root, 4);
	}

	// Setting this to false will disable the moving of an object with a platofrm
	bMove_with_platform = true;
	action_gothroughplatform = false;
	testallexceptinsideplatforms = false;
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
}

// Check if on floor by checking for overlap 1px below
bool ExtObject::IsOnFloor()
{
	if (pObstacles == NULL && pPlatforms == NULL) return false;

	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	pLink->info.x += cosga;
	pLink->info.y += singa;
	pRuntime->UpdateBoundingBox(pLink);

	bool onFloor = IsOverlapping();

	pLink->info.x -= cosga;
	pLink->info.y -= singa;
	pRuntime->UpdateBoundingBox(pLink);

	return onFloor;
}

// Check if our heads are on the roof so we can make the player stick to the ceiling
bool ExtObject::IsOnCeiling()
{
	if (pObstacles == NULL && pPlatforms == NULL) return false;
	
	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	pLink->info.x -= cosga;
	pLink->info.y -= singa;
	pRuntime->UpdateBoundingBox(pLink);

	bool onFloor = IsOverlapping();

	pLink->info.x += cosga;
	pLink->info.y += singa;
	pRuntime->UpdateBoundingBox(pLink);

	return onFloor;
}



// Check if can move left/right by same method as IsOnFloor
bool ExtObject::CanMoveLeft()
{
	if (pObstacles == NULL && pPlatforms == NULL) return true;
	
	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	pLink->info.x -= singa;
	pLink->info.y -= cosga;
	pRuntime->UpdateBoundingBox(pLink);

	//Hack - dont check platforms
	CRunObjType* mem = pPlatforms;
	pPlatforms = NULL;
	bool can = IsOverlapping();
	pPlatforms = mem;

	pLink->info.x += singa;
	pLink->info.y += cosga;
	pRuntime->UpdateBoundingBox(pLink);

	return !can;
}

bool ExtObject::CanMoveRight()
{
	if (pObstacles == NULL && pPlatforms == NULL) return true;

	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	pLink->info.x += singa;
	pLink->info.y += cosga;
	pRuntime->UpdateBoundingBox(pLink);

	//Hack - dont check platforms
	CRunObjType* mem = pPlatforms;
	pPlatforms = NULL;
	bool can = IsOverlapping();
	pPlatforms = mem;

	pLink->info.x -= singa;
	pLink->info.y -= cosga;
	pRuntime->UpdateBoundingBox(pLink);

	return !can;
}

bool ExtObject::OverlapTest(CRunObject* pObj)
{
	float fx = 	pLink->info.x;
	float fy = 	pLink->info.y;
	
	int w = 2, h = 2;
	
	if(pLink->info.x == floor(pLink->info.x) )
		w = 1;
	if(pLink->info.y == floor(pLink->info.y) )
		h = 1;
	for( int x = 0; x < w; x ++)
	{
		pLink->info.x = floor(fx) + x;

		for(int y = 0; y < h; y++)
		{
			pLink->info.y = floor(fy) + y;
			pRuntime->UpdateBoundingBox(pLink);
			if(pRuntime->QueryCollision(pLink, pObj))
			{
				pLink->info.x = fx;
				pLink->info.y = fy;
				pRuntime->UpdateBoundingBox(pLink);
				return true;
			}
		}
	}
	pLink->info.x = fx;
	pLink->info.y = fy;
	pRuntime->UpdateBoundingBox(pLink);
	return false;
}


bool ExtObject::IsOverlapping()
{
	float fx = 	pLink->info.x;
	float fy = 	pLink->info.y;


	int w = 2, h = 2;
	
	if(pLink->info.x == floor(pLink->info.x) )
		w = 1;
	if(pLink->info.y == floor(pLink->info.y) )
		h = 1;


	for( int x = 0; x < w; x ++)
	{
		pLink->info.x = floor(fx) + x;

		for(int y = 0; y < h; y++)
		{
			pLink->info.y = floor(fy) + y;
			pRuntime->UpdateBoundingBox(pLink);
			if(pObstacles)
			{
				if(pRuntime->QueryCollision(pLink, pObstacles))
				{
					pRuntime->QueryCollision(pLink, pObstacles);
					pLink->info.x = fx;
					pLink->info.y = fy;
					pRuntime->UpdateBoundingBox(pLink);
					return true;
				}
			}

			// Testing for platforms is a little more complicated, we need to loop each platform
			// individually, and check to see if its inside the platforms inside mask. If its not, then its
			// okay to proceed, otherwise that platform is ignored

			if(pPlatforms){
				if(testallexceptinsideplatforms)
				{
					int count;
					CRunObject **objs;
					pRuntime->GetTypeSelectedInstances(pPlatforms, objs, count);
					for(int i = 0; i < count; i++) 
					{
						CRunObject* platform = objs[i]; // do here	
						if(platforms_inside.find(platform) ==  platforms_inside.end())
						{
							if(pRuntime->QueryCollision(pLink, platform))
							{
								pLink->info.x = fx;
								pLink->info.y = fy;
								pRuntime->UpdateBoundingBox(pLink);
								return true;
							}
						}
					}
				}
				else
				{
					map<CRunObject*, CRunObject*>::iterator i = platforms_getoutof.begin();

					for(; i != platforms_getoutof.end(); i++) 
					{
						CRunObject* platform = i->first;
						if(pRuntime->QueryCollision(pLink, platform))
						{
							pLink->info.x = fx;
							pLink->info.y = fy;
							pRuntime->UpdateBoundingBox(pLink);
							return true;
						}
					}
				}
			}
		}
	}

	pLink->info.x = fx;
	pLink->info.y = fy;

	pRuntime->UpdateBoundingBox(pLink);

	return false;
}

int DirRound(double dir, double val)
{
	if(dir < 0)
		return floor(val);
	if(dir > 0)
		return ceil(val);
	else
		return (int)(val + 0.5);
}
		
// This function should be used if you move an object downwards
void ExtObject::CheckForPlatformsInside()
{	
	platforms_inside.clear();
	int count;
	CRunObject **objs;
	pRuntime->GetTypeSelectedInstances(pPlatforms, objs, count);
	for(int i = 0; i < count; i++) 
	{
		CRunObject* platform = objs[i]; // do here	
		if(OverlapTest(platform))
		{
			platforms_inside[platform] = platform;
		}
	}
}

void ExtObject::CheckForPlatformsVertically()
{
	platforms_getoutof.clear();
	map<CRunObject*, CRunObject*> newPlatformsInside;

	float oldx = pLink->info.x;
	float oldy = pLink->info.y;

	switch (grav_dir) {
	case GRAV_UP:
		pLink->info.y -= 0.01f;
		break;
	case GRAV_DOWN:
		pLink->info.y += 0.01f;
		break;
	case GRAV_LEFT:
		pLink->info.x -= 0.01f;
		break;
	case GRAV_RIGHT:
		pLink->info.x += 0.01f;
		break;
	}

	// If we aren't going upwards, we loop any platforms that we are in and see if they are inside the map
	// of objects we overlapped last frame. This is a safe way of removing platforms we are no longer inside
	int count;
	CRunObject **objs;
	pRuntime->GetTypeSelectedInstances(pPlatforms, objs, count);
	for(int i = 0; i < count; i++) 
	{
		CRunObject* platform = objs[i]; // do here	
		if(OverlapTest(platform))
		{
			if(platforms_inside.find(platform) ==  platforms_inside.end())
			{
				platforms_getoutof[platform] = platform;
			}
		}
	}
	pLink->info.x = oldx;
	pLink->info.y = oldy;
}

// Called every frame, before the events and after drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
BOOL ExtObject::OnFrame()
{
	// Get keystates

	vk_left = pRuntime->GetControlState("Move Left", player);
	vk_right = pRuntime->GetControlState("Move Right", player);
	vk_jump = pRuntime->GetControlState("Jump", player);
	vk_down = pRuntime->GetControlState("Move Down", player);

	if(pStandOnMoving && bMove_with_platform)
	{
		pLink->info.x += floor(pStandOnMoving->info.x) - moving_oldx;
		pLink->info.y += floor(pStandOnMoving->info.y) - moving_oldy;
		moving_oldx = floor(pStandOnMoving->info.x);
		moving_oldy = floor(pStandOnMoving->info.y);
		pRuntime->UpdateBoundingBox(pLink);
	}
	else
	{
		//MessageBox(0,0,0,0);
	}

	bool please_dont_jump = false;

	CheckForPlatformsInside();
	testallexceptinsideplatforms = false;
	CheckForPlatformsVertically();

	float ga = RADIANS(grav_dir * 90);

	float timedelta = pRuntime->GetTimeDelta();

	// we step horizontally so as to apply sloping
	bool collidedx = false;
	bool collidedy = false;

	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	// Limit fall speed
	switch (grav_dir)
	{
		case GRAV_RIGHT:
			if(dx > max_fall)
				dx = max_fall;
			break;
		case GRAV_DOWN:
			if(dy > max_fall)
				dy = max_fall;
			break;
		case GRAV_LEFT:
			if(dx < -max_fall)
				dx = -max_fall;
			break;
		case GRAV_UP:
			if(dy < -max_fall)
				dy = -max_fall;
			break;
	}

	float speedv = dx  * cosga + dy * singa;


	speedv *= timedelta;

	//////////////////////////////////
	// Push out of collisions

	int count = 1;
	int dir = 0;
	float startX = pLink->info.x;
	float startY = pLink->info.y;

	while (IsOverlapping()){
		pLink->info.x = startX;
		pLink->info.y = startY;

		switch (dir) {
		case 0:		// up
			pLink->info.y = startY - count;
			break;
		case 1:		// left
			pLink->info.x = startX - count;
			break;
		case 2:		// right
			pLink->info.x = startX + count;
			break;
		case 3:		// down
			pLink->info.y = startY + count;
			break;

		}

		pLink->UpdateBoundingBox();

		dir++;

		if (dir == 4) {
			dir = 0;
			count++;
		}
	}

	// Platform code - remember anything we are inside
	//if(ignoreallplatforms)

	//////////////////////////////////
	// Move vertically
	//if (dy != 0) 
	{
		float xstepdirection = speedv * cosga;
		float ystepdirection = speedv * singa;

		pRuntime->UpdateBoundingBox(pLink);

		pLink->info.x += xstepdirection;
		pLink->info.y += ystepdirection;
		
		if(xstepdirection < 0) xstepdirection = -1;
		if(xstepdirection > 0) xstepdirection = 1;
		if(ystepdirection < 0) ystepdirection = -1;
		if(ystepdirection > 0) ystepdirection = 1;


		// Move the object
		pRuntime->UpdateBoundingBox(pLink);


		if(speedv >= 0)
		{
			CheckForPlatformsVertically();
		}
		// check we are on top of a platform (hack...should be made into a new function)
		CRunObjType* pHack = pObstacles;
		pObstacles = NULL;
		if(IsOnFloor())
		{
			if(!ignoringInput && vk_down && vk_jump && CanPressDown || action_gothroughplatform)
			{
				platforms_inside.clear();
				platforms_getoutof.clear();
				please_dont_jump = true;
			}
		}
		pObstacles = pHack;
		float x = pLink->info.x;
		float y = pLink->info.y;
	

		// Step back until not overlapping obstacle
		if (pObstacles != NULL || pPlatforms != NULL) 
		{
			if(IsOverlapping())
			{
				pRuntime->UpdateBoundingBox(pLink);
/*				if(!pRuntime->QueryCollision(pLink, pPlatforms))
				{
					gothroughplatform = false;
				}*/
				// If the x and y steps are zero, we are not moving and hence can't move back out from the direction
				// we came.  So just move out to the nearest free space.
				if (xstepdirection == 0.0f && ystepdirection == 0.0f) {

					// This should no longer EVER happen
				}
				else {
				
					int count = 0;
					do
					{
						count ++;
						
						if(singa)
							pLink->info.y = DirRound(ystepdirection, pLink->info.y - ystepdirection);
						if(cosga)
							pLink->info.x = DirRound(xstepdirection, pLink->info.x - xstepdirection);
				
						pRuntime->UpdateBoundingBox(pLink);	
					} while (IsOverlapping());

					if(singa)
						dy = 0 * singa;
					if(cosga)
						dx = 0 * cosga;
				}
			}	
		}	
	}

	bool onFloor = IsOnFloor();

	CheckForPlatformsInside();
	//testallexceptinsideplatforms = true;

	//platforms_getoutof.clear();
	//ignoreallplatforms = true;

	float speedh = dy * cosga + dx * singa;
	speedh *= timedelta;
	//if(IsOnFloor()) // we only check for slope if we were already on the floor
	{
		int invt = speedh > 0 ? 1 : -1;
		float xstepdirection = speedh * singa;
		float ystepdirection = speedh * cosga;
		
		if(xstepdirection < 0) xstepdirection = -1;
		if(xstepdirection > 0) xstepdirection = 1;
		if(ystepdirection < 0) ystepdirection = -1;
		if(ystepdirection > 0) ystepdirection = 1;


		//float speedx = invt * dx * timedelta;

		////////////
		//  HORIZONTAL

		// invt allows the move right code to work just like move left
		float steps_left = speedh * invt;

		
		while(steps_left>0)
		{
			//if(!please_dont_jump)
			//testallexceptinsideplatforms = true;
			bool wasonfloor = IsOnFloor();
			

			float ddx = steps_left * invt;
			Clamp(ddx, -1.0f, 1.0f);

			pLink->info.x += ddx * singa;
			pLink->info.y += ddx * cosga;
			steps_left --;

		/*	if(IsOverlapping())
			{
				// deadeye bug: If we are going sideways into a platform, we dont want to be bumped upwards
				//   so we check to see if it could be a wall:
				pLink->info.y -= 3 * singa;
				pLink->info.x -= 3 * cosga;

				if(IsOverlapping())		
					testallexceptinsideplatforms = false;
				pLink->info.y += 3 * singa;
				pLink->info.x += 3 * cosga;
			}*/
			
			if( IsOverlapping())
			{
				// try to push up
				pLink->info.y -= singa;
				pLink->info.x -= cosga;
				if( IsOverlapping())
				{
					pLink->info.y -= singa;
					pLink->info.x -= cosga;
					if( IsOverlapping())
					{					
						//damn...still overlapping...no good...okay so we arn't going up a slope...perhaps our head is
						// in a ceiling...lets try sliding down instead
						pLink->info.y += 3 * singa;
						pLink->info.x += 3 * cosga;
						CheckForPlatformsVertically();
						if( IsOverlapping())
						{
							pLink->info.y += singa;
							pLink->info.x += cosga;
							CheckForPlatformsVertically();
							if( IsOverlapping())
							{	
								
								
								// okay quite clearly we have wacked into a wall or platform then...
								// make u
								pLink->info.y -= 2 * singa;
								

								CheckForPlatformsInside();
								testallexceptinsideplatforms = false;

								// okay its a wall
								if( IsOverlapping())
								{			
									pLink->info.x -= 2 * cosga;
									if(cosga)
									{
										pLink->info.y = DirRound(ystepdirection, pLink->info.y - ystepdirection);
										if( IsOverlapping())
											pLink->info.y = DirRound(ystepdirection, pLink->info.y - ystepdirection);
										dy = 0;
									}
									if(singa)
									{
										pLink->info.x = DirRound(xstepdirection, pLink->info.x - xstepdirection);
										if( IsOverlapping())
											pLink->info.x = DirRound(xstepdirection, pLink->info.x - xstepdirection);
										dx = 0;
									}
									CheckForPlatformsInside();
									steps_left = 0;
								}
								if(!please_dont_jump)
									testallexceptinsideplatforms = true;
							}
							else if(timedelta != 0)
							{
								dy += 1 / timedelta * singa;
								dx += 1 / timedelta * cosga;
							}
						}
						else if(timedelta != 0)
						{
							dy += 1 / timedelta * singa;
							dx += 1 / timedelta * cosga;
						}		
					}
				}
			}
			else if(wasonfloor)
			{
				// okay we are't overlapping, but the previous step we were on the floor so lets see if we can go down
				pLink->info.y += singa;
				pLink->info.x += cosga;


				if( IsOverlapping())
				{
					//ah overlapping nm!
					pLink->info.y -= singa;
					pLink->info.x -= cosga;
				}
				else
				{
					CheckForPlatformsInside();
					pLink->info.y += singa;
					pLink->info.x += cosga;
					CheckForPlatformsVertically();
					if( IsOverlapping())
					{
						//ah overlapping nm!
						if(timedelta != 0)
						{
							//dy += 1 / timedelta * singa;
							//dx -= 1 / timedelta * cosga;
						}
						pLink->info.y -= singa;
						pLink->info.x -= cosga;
					}
					else
					{
						pLink->info.y += singa;
						pLink->info.x += cosga;
						CheckForPlatformsVertically();
						if(IsOverlapping())
						{
							//ah overlapping nm!
							if(timedelta != 0)
							{
								//dy += 2 / timedelta * singa;
								//dx -= 2 / timedelta * cosga;
							}
							pLink->info.y -= singa;
							pLink->info.x -= cosga;
						}
						else
						{
							//yikes we still arn't overlapping!? abort abort
							pLink->info.y -= 3 * singa;
							pLink->info.x -= 3 * cosga;
						}
					}
				}
				// else all is good keep going
			}  // end if overlap

			pRuntime->UpdateBoundingBox(pLink);

		}// end while loop
	}

	///////////////////
	// Control

	// First modify object
	bool left = false, right = false, jump = false, curjump = false;

	if (!ignoringInput) {
		left = vk_left > 0.5;
		right = vk_right > 0.5;

		if(inv)
			swap(left,right);

		// Prevent bunnyhop-jumping unless bunnyhop bool is true
		curjump = vk_jump > 0.5;
		jump = curjump & !oldjump;
		oldjump = curjump;

		if(bunnyhop)
			jump = curjump;

		if(please_dont_jump)
			jump = false;
		

		// Cannot hold both keys
		if (right && left) {
			left = false;
			right = false;
		}
	}

	bool canMoveLeft = false;
	bool canMoveRight = false;

	if (GetFocus() != pRuntime->GetFrameHwnd(pLink->pLayout)) {
		left = right = jump = false;
	}





	if (speedh <= 0)
		canMoveLeft = CanMoveLeft();
	if (speedh >= 0)
		canMoveRight = CanMoveRight();





	// Apply gravity if nothing beneath me
	float maxspeed = max_floor_speed;
	float acc = floor_acc;
	float dec = floor_dec;
	acc += dec;
	

	if (!onFloor)
	{
		maxspeed = max_air_speed;
		acc = air_acc;
		dec = air_dec;
		acc += dec;

		float gravity = 0;
		
		if(dy < 0) // jumping
		{
			if(jump_sustain > 0 && pRuntime->GetLayoutClock(pLayout) - lastjump <= jump_sustain)
				gravity = grav_jump_sustain;
			else
				gravity = grav_jump;
		}
		else
		{
			gravity = grav_fall;
		}

		dy += gravity * timedelta * singa;
		dx += gravity * timedelta * cosga;
	}

	if(speedh < -maxspeed)
		left = false;
	if(speedh > maxspeed)
		right = false;



	// Handle left/right movement
	
		if (left) 
		{
			if(autochangeangle)
				pLink->info.angle = 180;

			if (acc < 0){
				if(singa)
					dx = -maxspeed * singa;
				if(cosga)
					dy = maxspeed * cosga;
			}
			else {
				dx -= acc * timedelta * singa;
				dy += acc * timedelta * cosga;

				if(singa && dx * singa < -maxspeed)
					dx = -maxspeed * singa;
				if(cosga && dy * cosga > maxspeed)
					dy = maxspeed * cosga;				
					
			}
		}
		if (right) {
			if(autochangeangle)
				pLink->info.angle = 0;

			if (acc < 0){
				if(singa)
					dx = maxspeed * singa;
				if(cosga)
					dy = -maxspeed * cosga;
			}
			else {
				dx += acc * timedelta * singa;
				dy -= acc * timedelta * cosga;

				if(singa && dx * singa > maxspeed)
					dx = maxspeed * singa;
				if(cosga && dy * cosga < -maxspeed)
					dy = -maxspeed * cosga;						
			}
		}

		// decelerate
		if(singa > 0)
		{
			if (dx > 0 && !right) {
				if (dec < 0)
					dx = 0;
				else
				{
					dx -= dec * timedelta;
					if (dx < 0) 
						dx = 0;
				}
			}
			else if (dx < 0 && !left) {
				if (dec < 0)
					dx = 0;
				else {
					dx += dec * timedelta;
					if (dx > 0) 
						dx = 0;
				}
			}
		}
		if(singa < 0)
		{
			if (dx > 0 && !left) {
				if (dec < 0)
					dx = 0;
				else
				{
					dx -= dec * timedelta;
					if (dx < 0) 
						dx = 0;
				}
			}
			else if (dx < 0 && !right) {
				if (dec < 0)
					dx = 0;
				else {
					dx += dec * timedelta;
					if (dx > 0) 
						dx = 0;
				}
			}
		}
		if(cosga < 0)
		{
			if (dy > 0 && !right) {
				if (dec < 0)
					dy = 0;
				else
				{
					dy -= dec * timedelta;
					if (dy < 0) 
						dy = 0;
				}
			}
			else if (dy < 0 && !left) {
				if (dec < 0)
					dy = 0;
				else {
					dy += dec * timedelta;
					if (dy > 0) 
						dy = 0;
				}
			}
		}
		if(cosga > 0)
		{
			if (dy > 0 && !left) {
				if (dec < 0)
					dy = 0;
				else
				{
					dy -= dec * timedelta;
					if (dy < 0) 
						dy = 0;
				}
			}
			else if (dy < 0 && !right) {
				if (dec < 0)
					dy = 0;
				else {
					dy += dec * timedelta;
					if (dy > 0) 
						dy = 0;
				}
			}
		}



	// Jump
	if (jump && onFloor) {
		if(singa)
			dy = -jump_strength * singa;
		if(cosga)
			dx = -jump_strength * cosga;

		lastjump = pRuntime->GetLayoutClock(pLayout);

	}
	// Code removed below, jump sustain is now done by affecting gravity
	/*if (curjump && jump_sustain > 0 && pRuntime->GetLayoutClock(pLayout) - lastjump <= jump_sustain) {
		if(singa)
			dy = -jump_strength * singa;
		if(cosga)
			dx = -jump_strength * cosga;


	}*/
	if (!curjump)
		lastjump = -9999;

	testallexceptinsideplatforms = true;

	// Moving platform code for detecting if we are standing on one
	ga = RADIANS(grav_dir * 90);
	cosga = cos(ga)*2.5f;
	singa = sin(ga)*2.5f;

	pLink->info.x += cosga;
	pLink->info.y += singa;
	pRuntime->UpdateBoundingBox(pLink);



	CRunObject** pObjectList;
	pRuntime->GetTypeInstances(pObstacles, pObjectList, count);

	pStandOnMoving = NULL;
	standing_on_platform = false;
	int i;
	for(i = 0; i < count; i++)
	{
		if(pRuntime->QueryCollision(pLink, pObjectList[i]))
		{
			pStandOnMoving = pObjectList[i];
			moving_oldx = pStandOnMoving->info.x;
			moving_oldy = pStandOnMoving->info.y;
			break;
		}
	}

	pRuntime->GetTypeInstances(pPlatforms, pObjectList, count);
	for(i = 0; i < count; i++)
	{
		if(platforms_inside.find(pObjectList[i]) == platforms_inside.end())
		{
			if(pRuntime->QueryCollision(pLink, pObjectList[i]))
			{
				pStandOnMoving = pObjectList[i];
				moving_oldx = floor(pStandOnMoving->info.x);
				moving_oldy = floor(pStandOnMoving->info.y);
				standing_on_platform = true;
				break;
			}
		}
	}
	
	

	pLink->info.x -= cosga;
	pLink->info.y -= singa;
	pRuntime->UpdateBoundingBox(pLink);

	//Added below to fix the bug:
	//https://sourceforge.net/tracker/?func=detail&aid=2804484&group_id=207820&atid=1003219
	CheckForPlatformsInside();
	testallexceptinsideplatforms = true;

	

	// Update animations
	if(cIsOnGround(NULL))
	{
		if(this->cIsMoving(NULL))
		{
			SetAnimation(walking);
		}
		else
		{
			SetAnimation(stopped);
		}
	}
	else
	{
		if(this->cIsFalling(NULL))
		{
			SetAnimation(falling);
		}
		else if(this->cIsJumping(NULL))
		{
			SetAnimation(jumping);
		}
	}
	return !activated;
}

void ExtObject::SetAnimation(CRunAnimation* pAnim)
{ 
	if (pAnim == NULL) return;

	int handle = pRuntime->GetAnimationByNearestAngle(pAnim->handle, pLink->info.angle);
	// check current animation
	if(pLink->info.curAnim->pAnim->handle == handle)
		return;
	else
	{
		pRuntime->SetAnimation(handle, pLink);

		pLink->info.curAnim->playing = true;
		pLink->info.curAnim->counter = 0;
		pLink->info.curAnim->updated = true;
		pRuntime->SetAnimationFrame(pLink, 0);
	}
}

// Called every frame, after the events and before drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
// It is not safe to destroy objects in OnFrame2().  If you have to do this, use OnFrame().
// If you are storing any pointers to CRunObjects, check the info.destroying flag here.  If it is true,
// you must reset the pointer to NULL as it is no longer valid.
BOOL ExtObject::OnFrame2()
{
	if(pStandOnMoving && bMove_with_platform)
	{
		pLink->info.x += floor(pStandOnMoving->info.x) - moving_oldx;
		pLink->info.y += floor(pStandOnMoving->info.y) - moving_oldy;
		moving_oldx = floor(pStandOnMoving->info.x);
		moving_oldy = floor(pStandOnMoving->info.y);
		pRuntime->UpdateBoundingBox(pLink);
	}
	return !activated;
}

// WindowProc:  called when a window message, or WM_COMMAND specifying your window,
// is sent to Construct.  You must call RegisterWindow() before Construct will send
// you messages for the given window.
LRESULT ExtObject::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

BOOL ExtObject::PreTranslateMessage(MSG* msg)
{
	return 0;
}

// For global objects
void ExtObject::OnFrameChange(int oldFrame, int newFrame)
{
	// Do anything your global object needs when the frame changes
	// oldFrame is -1 on start of app, newFrame is -1 on end of app
}

// User defined functions
long ExtObject::GetData(int id, void* param)
{
	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	return 0;
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
}

#endif