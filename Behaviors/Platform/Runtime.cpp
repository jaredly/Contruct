// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"
#include <float.h>
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

	ignoringInput = false;
	activated = true;
	oldjump = false;

	pStandOnMoving = NULL;

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
	vertical_pixels_moved = 0;
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
}

// Check if on floor by checking for overlap 1px below
bool ExtObject::IsOnFloor( bool use_p_stand_on_moving_too )
{
	if (pObstacles == NULL && pPlatforms == NULL) return false;

	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	pLink->info.x += cosga;
	pLink->info.y += singa;
	pRuntime->UpdateBoundingBox(pLink);

	bool onFloor = IsOverlapping(false);

	pLink->info.x -= cosga;
	pLink->info.y -= singa;
	pRuntime->UpdateBoundingBox(pLink);

	if(use_p_stand_on_moving_too && pStandOnMoving && bMove_with_platform)
		return true; // we are standing on the platform, therefore we are on the floor

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

	bool onFloor = IsOverlapping(false);

	pLink->info.x += cosga;
	pLink->info.y += singa;
	pRuntime->UpdateBoundingBox(pLink);

	return onFloor;
}

// Check if can move left/right by same method as IsOnFloor
bool ExtObject::CanMoveLeft()
{
	if (pObstacles == NULL && pPlatforms == NULL) return true;

	float oldx = pLink->info.x;
	float oldy = pLink->info.y;

	float ga = RADIANS(grav_dir * 90);
	int cosga = -cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	bool can = true;

	// test left
	pLink->info.x -= singa;
	pLink->info.y -= cosga;
	pRuntime->UpdateBoundingBox(pLink);
	if ( IsOverlapping(true) )
	{
		// test up a pixel
		pLink->info.x += cosga;
		pLink->info.y -= singa;
		pRuntime->UpdateBoundingBox(pLink);
	
		if ( IsOverlapping(true) )
		{
			// test up a pixel
			pLink->info.x += cosga;
			pLink->info.y -= singa;
			pRuntime->UpdateBoundingBox(pLink);
		
			if ( IsOverlapping(true) )
			{
				can = false;
			}
		}
	}

	pLink->info.x = oldx;
	pLink->info.y = oldy;
	pRuntime->UpdateBoundingBox(pLink);

	return can;
}

bool ExtObject::CanMoveRight()
{
	if (pObstacles == NULL && pPlatforms == NULL) return true;

	float oldx = pLink->info.x;
	float oldy = pLink->info.y;

	float ga = RADIANS(grav_dir * 90);
	int cosga = -cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;


	bool can = true;

	// test right
	pLink->info.x += singa;
	pLink->info.y += cosga;
	pRuntime->UpdateBoundingBox(pLink);
	if ( IsOverlapping(true) )
	{
		// test up a pixel
		pLink->info.x += cosga;
		pLink->info.y -= singa;
		pRuntime->UpdateBoundingBox(pLink);

		if ( IsOverlapping(true) )
		{
			// test up a pixel
			pLink->info.x += cosga;
			pLink->info.y -= singa;
			pRuntime->UpdateBoundingBox(pLink);

			if ( IsOverlapping(true) )
			{
				can = false;
			}
		}
	}

	pLink->info.x = oldx;
	pLink->info.y = oldy;
	pRuntime->UpdateBoundingBox(pLink);

	return can;
}

bool ExtObject::OverlapTest(CRunObject* pObj)
{
	float fx = 	pLink->info.x;
	float fy = 	pLink->info.y;


	int w = 2, h = 2;

	float offx1 = -0.5f + 512 * FLT_EPSILON;
	float offx2 = 0.5f -  512 * FLT_EPSILON;
	float offy1 = -0.5f + 512 * FLT_EPSILON;
	float offy2 = 0.5f - 512 * FLT_EPSILON;

	if(pLink->info.x == floor(pLink->info.x) )
	{
		w = 1;
		offx1 = offx2 = 0;
	}
	if(pLink->info.y == floor(pLink->info.y) )
	{
		h = 1;
		offy1 = offy2 = 0;
	}

	for( int x = 0; x < w; x ++)
	{
		pLink->info.x = floor(fx) + ((x == 0) ? offx1 : offx2);

		for(int y = 0; y < h; y++)
		{
			pLink->info.y = floor(fy) + ((y == 0) ? offy1 : offy2);
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



bool ExtObject::IsOverlapping( bool solids_only )
{
	float fx = 	pLink->info.x;
	float fy = 	pLink->info.y;


	int w = 2, h = 2;
	
	float offx1 = -0.2f + 2048 * FLT_EPSILON;
	float offx2 = 0.2f -  2048 * FLT_EPSILON;
	float offy1 = -0.2f + 2048 * FLT_EPSILON;
	float offy2 = 0.2f - 2048 * FLT_EPSILON;

	if(pLink->info.x == floor(pLink->info.x) )
	{
		w = 1;
		offx1 = offx2 = 0;
	}
	if(pLink->info.y == floor(pLink->info.y) )
	{
		h = 1;
		offy1 = offy2 = 0;
	}

	for( int x = 0; x < w; x ++)
	{
		pLink->info.x = floor(fx) + ((x == 0) ? offx1 : offx2);

		for(int y = 0; y < h; y++)
		{
			pLink->info.y = floor(fy) + ((y == 0) ? offy1 : offy2);
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

			if(pPlatforms && !solids_only)
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

bool sameSign(float a, float b)
{
	if(a < 0 && b < 0)
		return true;
	if(b >= 0 && b >= 0)
		return true;
	return false;
}

int signOf(float a)
{
	if(a < 0)
		return -1;
	return 1;
}

void ExtObject::PushOutOfSolids()
{
	//////////////////////////////////
	// Push out of collisions

	int count = 1;
	int dir = 0;
	float startX = pLink->info.x;
	float startY = pLink->info.y;

	while (IsOverlapping(true)){
		pLink->info.x = startX;
		pLink->info.y = startY;

		switch (dir) {
		case 0:		// up
			pLink->info.y = floor(startY - count);
			break;
		case 1:		// left
			pLink->info.x = floor(startX - count);
			break;
		case 2:		// right
			pLink->info.x = ceil(startX + count);
			break;
		case 3:		// down
			pLink->info.y = ceil(startY + count);
			break;

		}

		pLink->UpdateBoundingBox();

		dir++;

		if (dir == 4) {
			dir = 0;
			count++;
		}
	}
}

void ExtObject::PushOutOfPlatformsUpwards()
{
	bool push_out_success = false;
	float startX = pLink->info.x;
	float startY = pLink->info.y;
	for(int r = 0; r < 6; r++)
	{
		if (IsOverlapping(false))
		{
			switch (grav_dir)
			{
			case GRAV_LEFT:
				pLink->info.x = ceil(pLink->info.x) + 1;
				break;
			case GRAV_RIGHT:
				pLink->info.x = floor(pLink->info.x) - 1;
				break;
			case GRAV_UP:
				pLink->info.y = ceil(pLink->info.y) + 1;
				break;
			case GRAV_DOWN:
				pLink->info.y = floor(pLink->info.y) - 1;
				break;
			}
		}
		else
		{
			r = 6;
			push_out_success = true;
		}
	}

	if(!push_out_success)
	{
		pLink->info.x = startX;
		pLink->info.y = startY;
	}
}

void ExtObject::MovePlayerVertically()
{
	float timedelta = pRuntime->GetTimeDelta();

	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;
	float speedv = dx  * cosga + dy * singa;
	speedv *= timedelta;


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


	bool ignore_platforms = false;
	if(speedv <= 0)
		ignore_platforms = true;

	float x = pLink->info.x;
	float y = pLink->info.y;


	// Step back until not overlapping obstacle
	if (pObstacles != NULL || pPlatforms != NULL) 
	{
		if(IsOverlapping(ignore_platforms))
		{
			pRuntime->UpdateBoundingBox(pLink);

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
				} while (IsOverlapping(ignore_platforms));

				if(singa)
					dy = 0 * singa;
				if(cosga)
					dx = 0 * cosga;
			}
		}	
	}	
}


bool ExtObject::IsOnPlatformFloor()
{
	bool ret = false;
	CRunObjType* pHack = pObstacles;
	pObstacles = NULL;
	if(IsOnFloor(false))
		ret = true;
	pObstacles = pHack;
	return ret;
}

bool ExtObject::IsJumpingDown()
{
	return (!ignoringInput && vk_down && vk_jump && CanPressDown || action_gothroughplatform);
}

void ExtObject::MoveWithMovingPlatform()
{
	if(pStandOnMoving && bMove_with_platform)
	{
		float movex = round_x_up(pStandOnMoving->info.x) - moving_oldx;
		float movey = round_y_up(pStandOnMoving->info.y) - moving_oldy; 

		switch (grav_dir)
		{
		case GRAV_DOWN:
			vertical_pixels_moved += movey;
			break;
		case GRAV_UP:
			vertical_pixels_moved -= movey;
			break;
		case GRAV_RIGHT:
			vertical_pixels_moved += movex;
			break;
		case GRAV_LEFT:
			vertical_pixels_moved -= movex;
			break;
		}

		pLink->info.x += movex;
		pLink->info.y += movey;
		moving_oldx = round_x_up(pStandOnMoving->info.x);
		moving_oldy = round_y_up(pStandOnMoving->info.y);

		pRuntime->UpdateBoundingBox(pLink);

	}
}
void ExtObject::MovePlayerHorizontally()
{
	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	float timedelta = pRuntime->GetTimeDelta();

	float speedh = dy * cosga + dx * singa;
	speedh *= timedelta;

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

			bool wasonfloor = IsOnFloor(true);


			float ddx = steps_left * invt;
			Clamp(ddx, -1.0f, 1.0f);

			pLink->info.x += ddx * singa;
			pLink->info.y += ddx * cosga;
			steps_left --;

			if( IsOverlapping(false))
			{
				// try to push up
				pLink->info.y -= singa;
				pLink->info.x -= cosga;
				if( IsOverlapping(false))
				{
					pLink->info.y -= singa;
					pLink->info.x -= cosga;
					if( IsOverlapping(false))
					{					
						//damn...still overlapping...no good...okay so we arn't going up a slope...perhaps our head is
						// in a ceiling...lets try sliding down instead
						pLink->info.y += 3 * singa;
						pLink->info.x += 3 * cosga;

						if( IsOverlapping(false))
						{
							pLink->info.y += singa;
							pLink->info.x += cosga;

							if( IsOverlapping(false))
							{	
								// okay quite clearly we have wacked into a wall or platform then...
								// make u
								pLink->info.y -= 2 * singa;
								pLink->info.x -= 2 * cosga;

								CheckForPlatformsInside();

								// okay its a wall...now we need to determine if the wall is solid or platform
								if( IsOverlapping(true))
								{			
									if(cosga)
									{
										pLink->info.y = DirRound(ystepdirection, pLink->info.y - ystepdirection);
										if( IsOverlapping(true))
											pLink->info.y = DirRound(ystepdirection, pLink->info.y - ystepdirection);
										dy = 0;
									}
									if(singa)
									{
										pLink->info.x = DirRound(xstepdirection, pLink->info.x - xstepdirection);
										if( IsOverlapping(true))
											pLink->info.x = DirRound(xstepdirection, pLink->info.x - xstepdirection);
										dx = 0;
									}
									CheckForPlatformsInside();
									steps_left = 0;
								}
								else
								{
									//MessageBox(0,0,0,0);
									//it was a platform
								}
							}
						}

					}
					else
						vertical_pixels_moved -= 2;
				}
				else
					vertical_pixels_moved -= 1;
			}
			else if(wasonfloor)
			{
				// okay we are't overlapping, but the previous step we were on the floor so lets see if we can go down the slope
				pLink->info.y += singa;
				pLink->info.x += cosga;


				if( IsOverlapping(false))
				{
					//ah the ground is below us...so never mind all is good!
					pLink->info.y -= singa;
					pLink->info.x -= cosga;
				}
				else
				{
					CheckForPlatformsInside();
					pLink->info.y += singa;
					pLink->info.x += cosga;

					if( IsOverlapping(false))
					{
						//ah overlapping nm!
						vertical_pixels_moved += 1;
						pLink->info.y -= singa;
						pLink->info.x -= cosga;
					}
					else
					{
						pLink->info.y += singa;
						pLink->info.x += cosga;

						if(IsOverlapping(false))
						{
							//ah overlapping nm!
							vertical_pixels_moved += 2;
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
}

void ExtObject::LimitFallSpeed()
{
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

	// Defaults in case user deleted keys
	if (vk_left == -1) vk_left = VK_LEFT;
	if (vk_right == -1) vk_right = VK_RIGHT;
	if (vk_jump == -1) vk_jump = VK_SHIFT;
	if (vk_down == -1) vk_down = VK_DOWN;

	MoveWithMovingPlatform();
	PushOutOfSolids();
	PushOutOfPlatformsUpwards();


	bool please_dont_jump = false;

	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	float timedelta = pRuntime->GetTimeDelta();


	LimitFallSpeed();


	CheckForPlatformsInside();

	// check we are on top of a platform
	if(IsOnPlatformFloor())
	{
		if( IsJumpingDown() )
		{
			pLink->info.x += cosga;
			pLink->info.y += singa;

			CheckForPlatformsInside();
			please_dont_jump = true;

		}
	}

	MovePlayerHorizontally();
	ApplyMovingPlatformSpeed();
	MovePlayerVertically();
	

	//Added below to fix the bug:
	//https://sourceforge.net/tracker/?func=detail&aid=2804484&group_id=207820&atid=1003219
	CheckForPlatformsInside();




	///////////////////
	// Control

	// First modify object
	bool jump = false, curjump = false;
	float leftright = 0;

	if (!ignoringInput) {
		leftright = vk_right - vk_left;

		if(inv)
			leftright *= -1;

		// Prevent bunnyhop-jumping unless bunnyhop bool is true
		curjump = vk_jump > 0.5;
		jump = curjump & !oldjump;
		oldjump = curjump;

		if(bunnyhop)
			jump = curjump;

		if(please_dont_jump)
			jump = false;
		
	}

	if (GetFocus() != pRuntime->GetFrameHwnd(pLink->pLayout)) {
		leftright = 0;
		jump = false;
	}



	// Apply gravity if nothing beneath me
	float maxspeed = max_floor_speed;
	float acc = floor_acc;
	float dec = floor_dec;

	bool onFloor = IsOnFloor(true);

	if (!onFloor)
	{
		maxspeed = max_air_speed;
		acc = air_acc;
		dec = air_dec;

		float gravity = 0;
		float speedv = dx  * cosga + dy * singa;
		if(speedv < 0) // jumping
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


	//-----------------------------------------------------------------------
	//     WORK OUT HOW WE ARE GOING TO ACCELERATE
	//-----------------------------------------------------------------------
	// Okay at this point acc and dec represent acceleration and deceleration
	float speedh = - dy * cosga + dx * singa;
	
	if (speedh <= 0)
		if(!CanMoveLeft())
			leftright = max(leftright, 0);
	if (speedh >= 0)
		if(!CanMoveRight())
			leftright = min(leftright, 0);

	bool sameDirection = sameSign(speedh, leftright);

	if(sameDirection) // this stops the player going faster then their maximum speed. If they are using a gamepad it controls the speed as well
	{
		float speedratio = 0;
		if(maxspeed)
			speedratio = abs(speedh / maxspeed);

		if( abs(leftright) <= speedratio) // speed ratio will be bigger than 1 if its over the maxspeed
			leftright = 0;
	}

	bool isAccelerating = (leftright != 0);

	// Okay heres the tricky part... we need to multiply dec and acc by timedelta.
	dec *= timedelta;
	acc *= timedelta;

	if(dec < 0) // lower than 0 means instant so we do it after timedelta!
		dec = abs(maxspeed);
	if(acc < 0) // lower than 0 means instant so we do it after timedelta!
		acc = abs(maxspeed);

	// deceleration can never be more then the speed
	if( abs(dec) > abs(speedh) )
	{
		dec = abs(speedh);
	}

	// Handle left/right movement
	if( sameDirection && isAccelerating)
	{
		if( leftright < 0) // left
		{
			acc *= -1; //accelerate to the left
		}
	}
	else
	{
		// We are going in the opposite direction
		if( isAccelerating )
		{
			acc = max(acc, dec);
		}
		else
			acc = dec;

		// now make the acceleration be in the opposite direction
		if(speedh >= 0) // right
			acc *= -1; //accelerate opposite		
	}

	//-----------------------------------------------------------------------
	//     Okay awesome
	//-----------------------------------------------------------------------
	if(autochangeangle)
	{
		if(leftright < 0)
			pLink->info.angle = 180;
		else if(leftright > 0)
			pLink->info.angle = 0;
	}
	
	// FINALLY - Now apply the acceleration
		
	dx += acc * singa;
	dy -= acc * cosga;

					
	//---------------------------------------------------------------------------------



	// Jump
	if (jump && onFloor) {
		if(singa)
			dy = -jump_strength * singa;
		if(cosga)
			dx = -jump_strength * cosga;

		lastjump = pRuntime->GetLayoutClock(pLayout);

	}

	if (!curjump)
		lastjump = -9999;

	// Moving platform code for detecting if we are standing on one

	LookForMovingPlatformBelow();	

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

void ExtObject::ApplyMovingPlatformSpeed()
{
	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;
	float timedelta = pRuntime->GetTimeDelta();

	if(vertical_pixels_moved > 0 && timedelta > 0)
	{
		// We only apply the speed if its greater than 0...this means if we up up a slope 
		// we wont 'hop' at the end due to gained vertical momentium. Although this would
		// be realistic physics, it would suck for gameplay. Basically this section of code
		// will make the player keep falling down if he falls off a platform or a slope.
		dy += vertical_pixels_moved / timedelta * singa;
		dx += vertical_pixels_moved / timedelta * cosga;
	}
	vertical_pixels_moved = 0;
}


void ExtObject::LookForMovingPlatformBelow()
{
	float ga = RADIANS(grav_dir * 90);
	int cosga = cos(ga)*1.5f;
	int singa = sin(ga)*1.5f;

	pLink->info.x += cosga;
	pLink->info.y += singa;
	pRuntime->UpdateBoundingBox(pLink);


	int count;
	CRunObject** pObjectList;
	pRuntime->GetTypeInstances(pObstacles, pObjectList, count);

	pStandOnMoving = NULL;
	standing_on_platform = false;
	int i;
	for(i = 0; i < count; i++)
	{
		if( OverlapTest( pObjectList[i]) )
		{
			pStandOnMoving = pObjectList[i];
			moving_oldx = round_x_up(pStandOnMoving->info.x);
			moving_oldy = round_y_up(pStandOnMoving->info.y);
			break;
		}
	}

	pRuntime->GetTypeInstances(pPlatforms, pObjectList, count);
	for(i = 0; i < count; i++)
	{
		if(platforms_inside.find(pObjectList[i]) == platforms_inside.end())
		{
			if( OverlapTest( pObjectList[i]) )
			{
				pStandOnMoving = pObjectList[i];
				moving_oldx = round_x_up(pStandOnMoving->info.x);
				moving_oldy = round_y_up(pStandOnMoving->info.y);
				standing_on_platform = true;
				break;
			}
		}
	}

	pLink->info.x -= cosga;
	pLink->info.y -= singa;
	pRuntime->UpdateBoundingBox(pLink);
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

float ExtObject::round_x_up(float x)
{
	switch (grav_dir)
	{
	case GRAV_RIGHT:
	case GRAV_LEFT:
		//return floor(x - 0.499);
		return ceil(x - 0.5);
	}
	return x;
}

float ExtObject::round_y_up(float y)
{
	switch (grav_dir)
	{
	case GRAV_DOWN:	
	case GRAV_UP:
		return ceil(y - 0.5);
	}
	return y;
}
// Called every frame, after the events and before drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
// It is not safe to destroy objects in OnFrame2().  If you have to do this, use OnFrame().
// If you are storing any pointers to CRunObjects, check the info.destroying flag here.  If it is true,
// you must reset the pointer to NULL as it is no longer valid.
BOOL ExtObject::OnFrame2()
{
	MoveWithMovingPlatform();
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