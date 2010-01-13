// Edittime.cpp - Edittime implementation of object
//

// Include StdAfx and Properties.h
#include "StdAfx.h"


void CollisionShape::Serialize(bin& ar)
{
	int Version = 1;
	SerializeVersion(ar, Version);

	if (ar.loading){
		ar >> m_Shape;
		ar >> width;
		ar >> height;
		ar >> angle;
		ar >> radius;
		
		int count;
		ar >> count;
		for(int i = 0; i < count; i++)
		{
			POINTF pt;
			ar >> pt.x;
			ar >> pt.y;
			m_pts.push_back(pt);
		}
		

	}
	else{
		ar << m_Shape;
		ar << width;
		ar << height;
		ar << angle;
		ar << radius;
		
		int count = m_pts.size();
		ar << count;
		for(int i = 0; i < count; i++)
		{
			POINTF pt = m_pts.at(i);
			ar << pt.x;
			ar << pt.y;
		}
	}

}





#ifndef RUN_ONLY

#define SHIFT_DOWN GetKeyState(VK_SHIFT) < 0
#define CONTROL_UP GetKeyState(VK_CONTROL) >= 0

#define PROPERTY_IF(x) if(editObject && editObject->objectPtr->x){
#define PROPERTY_ENDIF }
#define PROPERTY_ELSE }else{
//////////////////////////////////////////////////////////////////////////////////
// Property table
//////////////////////////////////////////////////////////////////////////////////

// The property table houses settings for your object that you wish to 
// be configurable before runtime
//
// For example, a button object would have settings relating to the text 
// that will be displayed, and what type of button it is
//
// There are several types of properties currently available to use:
//
// PROPERTY_EDIT  , a text input field
// PROPERTY_STATIC, a static display of text
//
// Here is a brief example of them all:
//
// PROPERTY_EDIT  (myVariable,   "Label", "Description); // myVariable is a string
// PROPERTY_STATIC(myVariable,   "Label", "Description); // myVariable is a string
// PROPERTY_VALUE (myVariable,   "Label", "Description); // myVariable is an int
// PROPERTY_BUTTON("ButtonText", "Label", "Description", Function()); // Function is a void with no parameters

BEGIN_PROPERTY_TABLE();
	if(!editObject)
		return;
	if(!editObject->objectPtr)
		return;
	if (iMode == MODE_WRITE) editObject->objectPtr->OnBeginPropertyRead(SLabel);

	PROPERTY_IF(editMode)
		PROPERTY_BUTTON("Delete", "Delete last point", "Click this to remove the last point", ClearEdit());
		PROPERTY_BUTTON("Close", "Custom Collision", "Click this to stop editing the custom collision", CloseEdit());

	PROPERTY_ELSE

		PROPERTY_BOOL(gravity,	"Gravity",	"Add gravity to this object.");
		PROPERTY_BOOL(immovable, "Immovable", "Tick to make object have infinite mass.");
		PROPERTY_BOOL(norotation, "No Rotation", "Enable to prevent the object rotating.");
		PROPERTY_FLOAT(mass, "Mass", "Object mass.");
		PROPERTY_PERCENT(lineardamp, "Linear damping", "A damping value from 0 to 100 for freely moving objects.");
		PROPERTY_PERCENT(angulardamp, "Angular damping", "A damping value from 0 to 100 for freely rotating objects.");
		PROPERTY_PERCENT(contactFriction, "Contact friction", "A friction value from 0 to 100.");
		PROPERTY_PERCENT(contactElasticity, "Contact elasticity", "A elasticity value from 0 to 100.");
		PROPERTY_COMBO(shape, "Collision mask", "Choose the shape of the collision mask.", "Bounding box|Ellipse|None|Custom");
		PROPERTY_BUTTON( "Edit", "Custom Collision","Click this to edit the custom collision", EditCustom());

		//PROPERTY_COMBO(worldSolver, "World Solver", "Adjust the accuracy/speed tradeoff of the physics engine.", "Linear (fastest)|Adaptive (medium)|Exact (slowest)");
		//PROPERTY_COMBO(worldFriction, "World Friction", "Adjust the accuracy/speed tradeoff of the friction model.", "Adaptive (fastest)|Exact (slowest)");
		
		PROPERTY_VALUE(simulation_steps,	"Simulation Steps",	"Number of simulation steps the physics uses");
		PROPERTY_PERCENT(worldXscale, "World X scale", "The ratio that defines how big your world objects are.");
		PROPERTY_PERCENT(worldYscale, "World Y scale", "The ratio that defines how big your world objects are.");
		PROPERTY_FLOAT(worldGravity, "World Gravity", "The force of gravity, 9.8 being earth gravity.");
	PROPERTY_ENDIF

	if (iMode == MODE_READ)
		if (SLabel == "Custom Collision")
			return;
		else// means it does not equal custom colliison
			editObject->objectPtr->OnPropertyUpdated(SLabel);
END_PROPERTY_TABLE  ();

/////////////////////////////////////////////////////////////////////////////////
// Edittime functions
//////////////////////////////////////////////////////////////////////////////////
float MakePtrFloat(void* ptr)
{
	int data = (int)ptr;
	return *((float*)&data);
}
void* MakeFloatPtr(float x)
{
	void* ret;
	*((float*)&ret) = x;
	return ret;
}
// Constructor: when your object is created during edittime or application loaded.
// Initialise values here.
EditExt::EditExt(VEditTime* pVEditTime, editInfo* pEInfo)
{
	// Save the edittime and einfo pointers
	pEditTime = pVEditTime;
	pInfo = pEInfo;

	pInfo->objectWidth  = 32;
	pInfo->objectHeight = 32;

	gravity = false;
	mass = 1.0f;
	immovable = false;
	lineardamp = 0.1f;
	angulardamp = 0.1f;
	shape = 0;
	norotation = false;
	worldGravity = 9.8f;

	// Default world scale is 2%:
	worldXscale = 0.02f;
	worldYscale = 0.02f;

	//worldSolver = 0;
	//worldFriction = 0;

	simulation_steps = 10;

	contactFriction = 0.8f;
	contactElasticity = 0.4f;

	if (pEditTime->LayoutKeyExists("worldGravity"))
		worldGravity = MakePtrFloat(pEditTime->GetLayoutKey("worldGravity"));
	else
		pEditTime->SetLayoutKey("worldGravity", MakeFloatPtr(worldGravity));

	if (pEditTime->LayoutKeyExists("worldXscale"))
		worldXscale = MakePtrFloat(pEditTime->GetLayoutKey("worldXscale"));
	else
		pEditTime->SetLayoutKey("worldXscale", MakeFloatPtr(worldXscale));

	if (pEditTime->LayoutKeyExists("worldYscale"))
		worldYscale = MakePtrFloat(pEditTime->GetLayoutKey("worldYscale"));
	else
		pEditTime->SetLayoutKey("worldYscale", MakeFloatPtr(worldYscale));

/*	if (pEditTime->LayoutKeyExists("worldSolver"))
		worldSolver = (int)pEditTime->GetLayoutKey("worldSolver");
	else
		pEditTime->SetLayoutKey("worldSolver", (void*)worldSolver);*/

/*	if (pEditTime->LayoutKeyExists("worldFriction"))
		worldFriction = (int)pEditTime->GetLayoutKey("worldFriction");
	else
		pEditTime->SetLayoutKey("worldFriction", (void*)worldFriction);*/

	if (pEditTime->LayoutKeyExists("simulationSteps"))
		simulation_steps = (int)pEditTime->GetLayoutKey("simulationSteps");
	else
		pEditTime->SetLayoutKey("simulationSteps", (void*)simulation_steps);



	editMode = false;
	boolAddingPoints = false;
	AddingPointsIndex = 0;
	AddingPointsCollision = 0;
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{
	//
}

inline void Rotate(float& x, float& y, float radians)
{
	cr::point pt(x,y);
	pt.rotate(radians);
	x = pt.x;
	y = pt.y;
}


// Draw your object in the frame editor.
// Note the co-ordinates are automatically adjusted; draw at pInfo->objectX/Y
void EditExt::Draw()
{
	pEditTime->SetTexture(-1);
	pEditTime->Fill(CRect(-10000,-10000, 10000,100000), 0x77ffffff);	// theres no way to get the size...

	CPoint mouse = pEditTime->GetCursor();
	editInfo*  pParentInfo = pEditTime->GetObjectBelongingToThisBehavior();
	float w = pParentInfo->objectWidth * pEditTime->GetZoom();
	float h = pParentInfo->objectHeight * pEditTime->GetZoom();
	float ox = pParentInfo->objectX * pEditTime->GetZoom();
	float oy = pParentInfo->objectY * pEditTime->GetZoom();
	float oa = pParentInfo->objectAngle;

	for(vector<CollisionShape>::iterator i = m_CompoundCollisions.begin(); i!= m_CompoundCollisions.end(); i++)
	{
		switch(i->m_Shape)
		{
			case shape_polygon:
			{
				float lastx;
				float lasty;
				float firstx;
				float firsty;
				int index = 0;
				for(int repeat = 0; repeat < 2; repeat ++)
				{
					for(vector<POINTF>::iterator p = i->m_pts.begin(); p!= i->m_pts.end(); p++)
					{
						float x = p->x * w;
						float y = p->y * h;
						Rotate(x,y, RADIANS(oa));
						x += ox; y += oy;

						if(index == 0)
						{
							firstx = x;
							firsty = y;
						}

						if(repeat == 1)
						{
							CRect point(x-4,y-4,x+5,y+5);

							pEditTime->Fill(point, 0xff000000);

							point.DeflateRect(1,1);
							pEditTime->Fill(point, 0xffffffff);

							// Draw a red minus symbol inside the first point
							if (p == i->m_pts.begin())
							{
								pEditTime->Line(x - 2, y , x + 3, y, 0xffff0000);
							}
							// Draw a blue plus symbol inside the last point
							else if(p == i->m_pts.end() - 1)
							{
								// Horizontal section
								pEditTime->Line(x - 2, y , x + 3, y, 0xff0000ff);

								// Vertical section
								pEditTime->Line(x, y - 2, x, y + 3, 0xff0000ff);
							}
						}



						if(repeat == 0)
						{
							if(index != 0)
							{
								// Draw a line between the current point and the previous point
								pEditTime->Line(x, y, lastx, lasty, 0xff000000);
							}

							if(index == i->m_pts.size()-1)
							{
								// Draw a line between the last point and the first point
								pEditTime->Line(x, y ,firstx, firsty, SHIFT_DOWN ? 0x40000000 : 0xff000000);

								// While shift is down, draw lines showing where new collision lines will be if a new point is placed
								if (SHIFT_DOWN)
								{
									pEditTime->Line(x, y, mouse.x, mouse.y, 0xff000000);
									pEditTime->Line(firstx, firsty, mouse.x, mouse.y, 0xff000000);
								}
							}
						}

						lastx = x;
						lasty = y;

						index++;
						
					}
				}

			}
		}
	}
	//
}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
	// 
}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
}

// When the object is removed from the frame editor by the user.
void EditExt::OnRemoved()
{
}

// Get any icons needed to show this ACEs menu
int EditExt::GetMenuIcons(int ACEMenu)
{
	// Todo: return the parameter number of a bitmap carrying an imagelist
	//       to show in the menu indicated by ACEMenu

	return -1; // -1 indicates no icons will be used
}

// Properties updated
void EditExt::OnPropertiesUpdate()
{
	Clamp(angulardamp, 0.0f, 1.0f);

	// 'No rotations' incompatible with 'Immovable'
	if (immovable && norotation)
		norotation = false;
}

void EditExt::OnPropertyUpdated(CString label)
{
	if (label == "World Gravity")		pEditTime->SetLayoutKey("worldGravity", MakeFloatPtr(worldGravity));
	if (label == "World X scale")		pEditTime->SetLayoutKey("worldXscale", MakeFloatPtr(worldXscale));
	if (label == "World Y scale")		pEditTime->SetLayoutKey("worldYscale", MakeFloatPtr(worldYscale));
	//if (label == "World Solver")		pEditTime->SetLayoutKey("worldSolver", (void*)worldSolver);
	//if (label == "World Friction")		pEditTime->SetLayoutKey("worldFriction", (void*)worldFriction);
	if (label == "Simulation Steps")	pEditTime->SetLayoutKey("simulationSteps", (void*)simulation_steps);
}

void EditExt::OnBeginPropertyRead(CString label)
{
	worldGravity = MakePtrFloat(pEditTime->GetLayoutKey("worldGravity"));
	worldXscale = MakePtrFloat(pEditTime->GetLayoutKey("worldXscale"));
	worldYscale = MakePtrFloat(pEditTime->GetLayoutKey("worldYscale"));
	//worldSolver = (int)pEditTime->GetLayoutKey("worldSolver");
	//worldFriction = (int)pEditTime->GetLayoutKey("worldFriction");
	simulation_steps = (int)pEditTime->GetLayoutKey("simulationSteps");
}

int EditExt::OnShowACEMenu(TABLE ACEType)
{
	// You can return the resource number indicating the menu to show for the
	//       ACE type indicated in ACEType
	// This overrides the default menu

	return -1;	// Return -1 for standard menu to be loaded
}

// On object resized
BOOL EditExt::OnSizeObject()
{
	// TRUE = allow sizing to occur, FALSE = disallow
	return TRUE;
}


void EditExt::Serialize(bin& ar)
{
	int Version = 3;
	SerializeVersion(ar, Version);

	if (ar.loading)
	{
		ar >> gravity >> mass >> immovable >> lineardamp >> shape >> angulardamp
			>> norotation;
		if(Version >= 2)
		{
			ar >> editMode
			 >> boolAddingPoints
			 >> AddingPointsIndex
			 >> AddingPointsCollision;
			int size = m_CompoundCollisions.size();
			ar >> size;
			for(int i = 0; i < size; i++)
			{
				CollisionShape myshape;
				myshape.Serialize(ar);
				m_CompoundCollisions.push_back(myshape);
			}
		}
		if(Version >= 3){
			ar >> contactFriction
				>> contactElasticity;

		}
	
	}
	else
	{
		ar << gravity << mass << immovable << lineardamp << shape << angulardamp
			<< norotation 
			<< editMode
			<< boolAddingPoints
			<< AddingPointsIndex
			<< AddingPointsCollision;
		int size = m_CompoundCollisions.size();
		ar << size;
		for(int i = 0; i < size; i++)
		{
			m_CompoundCollisions[i].Serialize(ar);
		}
		ar << contactFriction
			<< contactElasticity;
	}
}

void EditExt::EditCustom()
{
	// Enable edit mode
	editMode = true;
	shape = 3;

	// Have the window focus set at the start
	setWindowFocus = true;

	// Nullify any existing pointer to a dragging point
	dragPoint = NULL;

	// Capture the frame editor
	pEditTime->CaptureFrameEditor(pInfo->instanceID);
}

void EditExt::CloseEdit()
{
	editMode = false;
	pEditTime->ReleaseFrameEditor(pInfo->instanceID);
}

void EditExt::ClearEdit()
{
	//m_CompoundCollisions.clear();
	//boolAddingPoints = false;
	if(AddingPointsIndex > 0)
	{
		AddingPointsIndex --;
		CollisionShape* pShape = &m_CompoundCollisions[AddingPointsCollision];
		pShape->m_pts.pop_back();
	}

	//AddingPointsCollision = 0;

	CWnd* FrameWindow = CWnd::FromHandle(pEditTime->GetFrameHwnd());
	FrameWindow->Invalidate();
}

void EditExt::OnMessage(int message)
{
	// Bool for keeping track of whether or not the window should be invalidated
	bool invalidate = false;

	CWnd* FrameWindow = CWnd::FromHandle(pEditTime->GetFrameHwnd());

	// Set the focus to the frame editor at the start of editing
	// This is to allow for immediate use of the shift key collision line preview
	if (setWindowFocus)
	{
		FrameWindow->SetFocus();
		setWindowFocus = false;
	}

	// Get the cursor position
	CPoint mouse = pEditTime->GetCursor();

	// Get edit object information
	editInfo*  pParentInfo = pEditTime->GetObjectBelongingToThisBehavior();
	float w = pParentInfo->objectWidth;
	float h = pParentInfo->objectHeight;
	float ox = pParentInfo->objectX;
	float oy = pParentInfo->objectY;
	float oa = pParentInfo->objectAngle;

	// Get the editor zoom level
	float zoom = pEditTime->GetZoom();

	// Adjust the current point for object position, size, angle, and editor zoom level
	POINTF pt = { mouse.x / zoom - ox, mouse.y / zoom - oy};
	Rotate(pt.x, pt.y, RADIANS(-oa));
	pt.x /= w;
	pt.y /= h;

	POINTF pt2 = dragPointOffset;
	pt2.x += mouse.x / zoom - ox;
	pt2.y += mouse.y / zoom - oy;
	Rotate(pt2.x, pt2.y, RADIANS(-oa));
	pt2.x /= w;
	pt2.y /= h;

	switch (message)
	{
		case WM_LBUTTONDOWN:
		{
			// Add a new point as long as the mouse isn't over an existing one
			if (!MouseOverPoint(false))
			{
				if(!boolAddingPoints)
				{
					boolAddingPoints = true;
					CollisionShape newshape;
					newshape.m_Shape = shape_polygon;
					m_CompoundCollisions.push_back(newshape);
					AddingPointsCollision = m_CompoundCollisions.size() - 1;
					AddingPointsIndex = 0;
				}

				// Add the new collision point
				CollisionShape* pShape = &m_CompoundCollisions[AddingPointsCollision];
				pShape->m_pts.insert(pShape->m_pts.begin() + AddingPointsIndex, pt);

				// Set the new point to be dragged immediately unless the control key is held
				if (CONTROL_UP)
				{
					dragPoint = &pShape->m_pts.back();
					dragPointOffset.x = 0;
					dragPointOffset.y = 0;
				}

				AddingPointsIndex ++;

				// Have the frame window invalidated
				invalidate = true;
			}

			break;
		}
		case WM_LBUTTONUP:
		{
			// Nullify the dragPoint pointer
			dragPoint = NULL;

			break;
		}
		case WM_RBUTTONDOWN:
		{
			// Delete the point that is currently under the cursor
			MouseOverPoint(true);

			// Have the frame window invalidated
			invalidate = true;

			break;
		}
	}

	// If there is a point to drag, move it
	if (dragPoint != NULL)
	{
		dragPoint->x = pt2.x;
		dragPoint->y = pt2.y;
		invalidate = true;
	}

	// Shift key is down
	if (SHIFT_DOWN)
	{
		// Have the frame window invalidated
		invalidate = true;
	}

	// Invalidate the frame window if previously specified
	if (invalidate)
	{
		FrameWindow->Invalidate();
	}
}


void WINAPI ETOnMessage(editInfo* editObject, int message)
{
	editObject->objectPtr->OnMessage(message);

}

// This function checks each point to see if the mouse is over it
bool EditExt::MouseOverPoint(bool deletePoint)
{
	// Proceed to check each point if the control key isn't held
	if (CONTROL_UP)
	{
		// Get the cursor position
		CPoint cmouse = pEditTime->GetCursor();
		POINTF mouse; 
		mouse.x = cmouse.x; 
		mouse.y = cmouse.y;
		// Get edit object information
		editInfo*  pParentInfo = pEditTime->GetObjectBelongingToThisBehavior();
		float w = pParentInfo->objectWidth;
		float h = pParentInfo->objectHeight;
		float ox = pParentInfo->objectX;
		float oy = pParentInfo->objectY;
		float oa = pParentInfo->objectAngle;

		// Get the editor zoom level
		float zoom = pEditTime->GetZoom();

		// Adjust the mouse coordinates to account for zoom
		mouse.x /= zoom;
		mouse.y /= zoom;

		for(vector<CollisionShape>::iterator i = m_CompoundCollisions.begin(); i!= m_CompoundCollisions.end(); i++)
		{
			switch(i->m_Shape)
			{
				case shape_polygon:
				{
					for(vector<POINTF>::iterator p = i->m_pts.begin(); p != i->m_pts.end(); p++)
					{
						// Adjust the point coordinates to account for object size, rotation, and position
						float x = p->x * w;
						float y = p->y * h;
						Rotate(x, y, RADIANS(oa));
						x += ox; 
						y += oy;

						// Rectangle representing the current point
						CRect point(x - 4/zoom, y - 4/zoom, x + 5/zoom, y + 5/zoom);

						// Check if the mouse is hovering over the current point
						if (mouse.x >= point.left && mouse.x <= point.right && mouse.y >= point.top && mouse.y <= point.bottom)
						{
							if (!deletePoint)
							{
								// We are going to shuffle the vector so that the point we have selected becomes the last point.
								vector<POINTF> temp;
								temp.insert(temp.begin(), p+1, i->m_pts.end());
								i->m_pts.erase(p+1, i->m_pts.end());
								i->m_pts.insert(i->m_pts.begin(), temp.begin(), temp.end());
								// Enable dragging of this point
								dragPoint = &i->m_pts.back();//&(*p);
								dragPointOffset.x = x - mouse.x;
								dragPointOffset.y = y - mouse.y;
							}
							else
							{
								// Delete the selected point
								AddingPointsIndex--;
								i->m_pts.erase(p);
							}

							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

#else //ifndef RUN_ONLY

BOOL WINAPI ETCreateObject(editInfo* editObject) {return FALSE;}
BOOL WINAPI ETInitializeObject(editInfo* editObject, VEditTime* pVEditTime) {return FALSE;}
BOOL WINAPI ETDestroyObject(editInfo* editObject) {return FALSE;}
int WINAPI ETDrawObject(editInfo *editObject) {return 0;}
void ETOnPropertyChanged(editInfo* editObject) {}
long LinkPropTable() {return 0;}
void WINAPI ETSerialize(editInfo* editObject, CArchive& ar) {}
void WINAPI ETPutObject(editInfo* editObject) {}
void WINAPI ETRemoveObject(editInfo* editObject) {}
void WINAPI GetInfo(LPOINFO objectInfo) {}
int WINAPI ETGetMenuIcons(editInfo* editObject, int ACEMenu) {return 0;}
int WINAPI ETShowACEMenu(editInfo* editObject, TABLE ACEType)  {return 0;}
BOOL WINAPI ETSizeObject(editInfo* editObject)  {return FALSE;}

#endif // RUN_ONLY