// Edittime.cpp - Edittime implementation of object
//

// Include StdAfx and Properties.h
#include "StdAfx.h"

#ifndef RUN_ONLY

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
	if(editObject->objectPtr->m_Add)
	{
		PROPERTY_BUTTON("Cancel", "Bone", "Cancel", CancelObject());
	}
	else if(editObject->objectPtr->m_Edit)
	{
		PROPERTY_BUTTON("Close", "Bones", "Click this button to close the bone editor", CloseEdit());
		if (iMode == MODE_READ)
			if (SLabel == "Bones" )
				return;  // Its vital that after CloseEdit is clicked that this function terminates coz 'this' becomes deleted memory
		PROPERTY_BUTTON("Add", "Bone", "Click this button to add a bone", AddObject());


		map<int, int> idtoindex;
		map<int, int> indextoid;
		int index;

		////////////////////////
		// Animations
		index = 0;
		CString anims;
		
		
		for(map<int, KeyAnimation>::iterator i = editObject->objectPtr->anim.begin(); i != editObject->objectPtr->anim.end(); i++)
		{
			idtoindex[i->first] = index; 
			indextoid[index] = i->first;
			index++;
			anims.Append(i->second.name);
			anims.Append("|");
		}
		indextoid[index] = editObject->objectPtr->unique_id;
		anims.Append("New");


		editObject->objectPtr->comboBoxHack = idtoindex[editObject->objectPtr->curAnim];
		PROPERTY_COMBO(comboBoxHack, "Animation", "Select an animation to edit",anims);
		

		KeyAnimation* pAnim = editObject->objectPtr->GetAnimation(editObject->objectPtr->curAnim);
		editObject->objectPtr->editBoxHack = pAnim->name;
		if(pAnim){
			PROPERTY_EDIT(editBoxHack, "Animation Name", "Name of the animation");
			if (iMode == MODE_READ) {
				if (SLabel == "Animation Name" ) 
				{
					pAnim->name = editObject->objectPtr->editBoxHack; 
					editObject->objectPtr->pEditTime->RefreshProperties();
					return;
				}
			} 
		}
		PROPERTY_BUTTON("Delete Animation", "Delete Animation", "Click this to delete the selected animation", RemoveAnimation());


		PROPERTY_FLOAT(GetAnimation(editObject->objectPtr->curAnim)->animSpeed, "Animation speed","The number of frames shown per second");
		PROPERTY_BOOL(GetAnimation(editObject->objectPtr->curAnim)->animRepeat, "Loop", "Repeat the animation forever");
		PROPERTY_VALUE(GetAnimation(editObject->objectPtr->curAnim)->animRepeatNumber, "Repeat #", "Number of times to repeat animation (if not looping)");
		PROPERTY_VALUE(GetAnimation(editObject->objectPtr->curAnim)->animRepeatTo, "Repeat To", "Frame to repeat to after reaching the final frame");
		PROPERTY_BOOL(GetAnimation(editObject->objectPtr->curAnim)->animPingPong, "Ping-pong", "Alternately reverse the animation");

		if (iMode == MODE_READ && SLabel == "Animation") 
		{
			if (SLabel == "Animation")
			{
				int oldindex = idtoindex[editObject->objectPtr->curAnim];
				map<int, KeyAnimation>::iterator i = editObject->objectPtr->anim.begin();
				advance(i, oldindex);


				editObject->objectPtr->limb.SetFrameToBones(editObject->objectPtr->GetFrame(editObject->objectPtr->curFrame));
				editObject->objectPtr->curAnim = indextoid[editObject->objectPtr->comboBoxHack];
				if(editObject->objectPtr->comboBoxHack + 1 == indextoid.size())
				{
					// NEW ANIMATION
					KeyAnimation* pAnim = &editObject->objectPtr->anim[editObject->objectPtr->unique_id];
					
					pAnim->name.Format("Animation %d", editObject->objectPtr->anim.size());
					editObject->objectPtr->curAnim = editObject->objectPtr->unique_id;
					editObject->objectPtr->unique_id ++;

					KeyFrame* pFrame = &editObject->objectPtr->frame[editObject->objectPtr->unique_id];
					editObject->objectPtr->curFrame = editObject->objectPtr->unique_id;
					pFrame->name = "Frame 1";
					editObject->objectPtr->unique_id ++;

					pAnim->frames.push_back(pFrame);

					editObject->objectPtr->pEditTime->RefreshProperties();
					
				}
				else
				{
					// Existing Animation
					map<int, KeyAnimation>::iterator i = editObject->objectPtr->anim.begin();
					advance(i, editObject->objectPtr->comboBoxHack);

					KeyAnimation* pAnim = editObject->objectPtr->GetAnimation(editObject->objectPtr->curAnim);

					int id;
					for(map<int, KeyFrame>::iterator f = editObject->objectPtr->frame.begin(); f != editObject->objectPtr->frame.end(); f++)
					{
						if(&f->second == pAnim->frames[0])
							id = f->first;		
					}

					editObject->objectPtr->curFrame = id;
					KeyFrame* pFrame = editObject->objectPtr->GetFrame(editObject->objectPtr->curFrame);

					editObject->objectPtr->limb.SetBonesToFrame(pFrame);


					editObject->objectPtr->pEditTime->RefreshProperties();
				}
			}
		}

		indextoid.clear();
		idtoindex.clear();

		pAnim = editObject->objectPtr->GetAnimation(editObject->objectPtr->curAnim);
		
		////////////////////////
		// Frames
		index = 0;
		CString frames;
		int id = 0;
		for(vector<KeyFrame*>::iterator i = pAnim->frames.begin(); i != pAnim->frames.end(); i++)
		{
			
			for(map<int, KeyFrame>::iterator f = editObject->objectPtr->frame.begin(); f != editObject->objectPtr->frame.end(); f++)
			{
				if(&f->second == pAnim->frames[index])
					id = f->first;		
			}
			idtoindex[id] = index; 
			indextoid[index] = id;
			index++;
			frames.Append((*i)->name);
			frames.Append("|");
		}
		indextoid[index] = id;
		frames.Append("New");

		editObject->objectPtr->comboBoxHack = idtoindex[editObject->objectPtr->curFrame];
		PROPERTY_COMBO(comboBoxHack, "Keyframe", "Select a frame to edit",frames);
//		PROPERTY_BUTTON("Rename Frame", "Rename", "Click this to rename the selected animation", RenameFrame());


		
		KeyFrame* pFrame = editObject->objectPtr->GetFrame(editObject->objectPtr->curFrame);
		if(pFrame)
		{
			editObject->objectPtr->editBoxHack = pFrame->name;
			if(pAnim){
				PROPERTY_EDIT(editBoxHack, "Frame Name", "Name of the selected frame");
				if (iMode == MODE_READ) {
					if (SLabel == "Frame Name" ) 
					{
						pFrame->name = editObject->objectPtr->editBoxHack; 
						editObject->objectPtr->pEditTime->RefreshProperties();
					}
				} 
			}
		PROPERTY_BUTTON("Delete Frame", "Delete Frame", "Click this to remove the selected animation", RemoveFrame());
		PROPERTY_FLOAT(GetFrame(editObject->objectPtr->curFrame)->speed, "Frame speed","Speed multiplier. 1 is normal, 0.5 is twice as fast");
		}
	if (iMode == MODE_READ && SLabel == "Keyframe") 
		{
			if (SLabel == "Keyframe")
			{
				// Update old
				editObject->objectPtr->limb.SetFrameToBones(editObject->objectPtr->GetFrame(editObject->objectPtr->curFrame));

				editObject->objectPtr->curFrame = indextoid[editObject->objectPtr->comboBoxHack];
				if(editObject->objectPtr->comboBoxHack + 1 == indextoid.size())
				{
					// NEW FRAME
					KeyFrame* pFrame = &editObject->objectPtr->frame[editObject->objectPtr->unique_id];
					editObject->objectPtr->curFrame = editObject->objectPtr->unique_id;
					
					editObject->objectPtr->unique_id ++;
					pFrame->name.Format("Frame %d", pAnim->frames.size()+1);

					pAnim->frames.push_back(pFrame);
					
					editObject->objectPtr->pEditTime->RefreshProperties();
				}
				else
				{
					KeyFrame* pFrame = editObject->objectPtr->GetFrame(indextoid[editObject->objectPtr->comboBoxHack]);
					editObject->objectPtr->limb.SetBonesToFrame(pFrame);

					editObject->objectPtr->pEditTime->RefreshProperties();
				}
			}
		}




	}
	else
	{
		PROPERTY_BUTTON("Edit", "Bones", "Click this button to edit the bones", Edit());
	}
END_PROPERTY_TABLE  ();

/////////////////////////////////////////////////////////////////////////////////
// Edittime functions
//////////////////////////////////////////////////////////////////////////////////

// Constructor: when your object is created during edittime or application loaded.
// Initialise values here.
EditExt::EditExt(VEditTime* pVEditTime, editInfo* pEInfo)
{
	// Save the edittime and einfo pointers
	pEditTime = pVEditTime;
	pInfo = pEInfo;
	
	pInfo->objectWidth  = 32;
	pInfo->objectHeight = 32;

	m_Edit = false;
	m_Add = 0;

	pMouseOver = NULL;

	iFont = -1;
	iDot = -1;
	drag_handle = NULL;
	drag_point = NULL;

	unique_id = 0;

	KeyFrame* pFrame = &frame[unique_id];
	pFrame->name = "Frame 1";
	unique_id ++;

	KeyAnimation* pAnim = &anim[unique_id];
	pAnim->name = "Animation 1";
	pAnim->frames.push_back(pFrame);
	unique_id ++;

	curFrame = 0;
	curAnim = 1;

	pParentInfo = pEditTime->GetObjectBelongingToThisBehavior();
	limb.instanceID = pParentInfo->instanceID;
	limb.ObjectIdentifier = pParentInfo->ObjectIdentifier;
	limb.globalID = pParentInfo->globalID;

	pSelectedLimb = NULL;

	allLimbs.clear();
	limb.UpdateAllLimbs(allLimbs);
	
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{
	if(iDot != -1)
		pEditTime->DeleteTexture(iDot);
	if(iFont != -1)
		pEditTime->DestroyFont(iFont);
	//
}

void EditExt::DrawDot(float x, float y, D3DCOLOR filter)
{
	pEditTime->SetTexture(iDot);
	pEditTime->Blitrc(x-8, y-8, 16,16, 0, filter);
	pEditTime->SetTexture(-1);
}
// Draw your object in the frame editor.
// Note the co-ordinates are automatically adjusted; draw at pInfo->objectX/Y
void EditExt::Draw()
{
	CPoint mouse2 = pEditTime->GetCursor();
	POINTF mouse = {mouse2.x, mouse2.y};

	pParentInfo = pEditTime->GetObjectBelongingToThisBehavior();
	limb.instanceID = pParentInfo->instanceID;
	limb.ObjectIdentifier = pParentInfo->ObjectIdentifier;
	limb.globalID = pParentInfo->globalID;

	if(GetKeyState(VK_RBUTTON)>>2)
	{
		return;
	}

	pEditTime->SetTexture(-1);
	pEditTime->Fill(CRect(-10000,-10000, 10000,100000), 0x77ffffff);	// theres no way to get the size...
	float ox = pParentInfo->objectX;
	float oy = pParentInfo->objectY;
	float zoom = pEditTime->GetZoom();

	if(m_Add)
	{
		if(m_Add == 2)
		{
			editInfo* Info = m_AttachLimb;
			if(Info)
			{
				double ox = Info->originX * Info->objectWidth;
				double oy = Info->originY * Info->objectHeight;
				double HotSpotAngle = atan2(float(oy), float(ox));
				double HotSpotDist = sqrt(ox * ox + oy * oy);

				float dx = HotSpotDist * cos(Rad(Info->objectAngle) + HotSpotAngle);
				float dy = HotSpotDist * sin(Rad(Info->objectAngle) + HotSpotAngle);
				pEditTime->Blitrc((Info->objectX-dx) * zoom, (Info->objectY-dy)* zoom, Info->objectWidth* zoom, Info->objectHeight* zoom, (float)Info->objectAngle, 0x220000dd);
			}
		}

		if(m_Add == 1 || m_Add == 2)
		{
			editInfo* Info = pMouseOver;
			if(Info)
			{
				double ox = Info->originX * Info->objectWidth;
				double oy = Info->originY * Info->objectHeight;
				double HotSpotAngle = atan2(float(oy), float(ox));
				double HotSpotDist = sqrt(ox * ox + oy * oy);

				float dx = HotSpotDist * cos(Rad(Info->objectAngle) + HotSpotAngle);
				float dy = HotSpotDist * sin(Rad(Info->objectAngle) + HotSpotAngle);
				pEditTime->Blitrc((Info->objectX-dx) * zoom, (Info->objectY-dy)* zoom, Info->objectWidth* zoom, Info->objectHeight* zoom, (float)Info->objectAngle, 0x440000dd);
			}
		}

		if(m_Add == 3 || m_Add == 4)
		{
			DrawDot(mouse.x, mouse.y);
		}

		/////////// Text helper
		CString text;
		if(m_Add == 1)
			text = "Select the limb your attaching";
		if(m_Add == 2)
			text = "Select the limb to attach to";
		if(m_Add == 3)
			text = "Click and hold to place the pivot";
		if(m_Add == 4)
			text = "Drag the limb in place and release";

		CRect rc(0,0,200,20);
		rc.OffsetRect(mouse.x - rc.Width()/2, mouse.y+50);

		pEditTime->Fill(rc, 0xCCFFFFFF);
		pEditTime->Box(rc, 0x33000000);

		rc.OffsetRect(0,3);
		pEditTime->DrawText(iFont, text, rc, 0xFF000000, DT_CENTER);
		//////////

	}
	else
	{
		for(vector<Limb*>::iterator i = allLimbs.begin(); i!= allLimbs.end(); i++)
		{
			D3DCOLOR highlight = 0xffffffff;
			if((*i)->mouseover && !(GetKeyState(VK_RBUTTON) >> 2))
			{
				highlight = 0xff4444ff;
			}

			double x = (*i)->pivot_pixelx;
			double y = (*i)->pivot_pixely;
			DrawDot(x,y, highlight);

			double cosa;// = cos(Rad((*i)->a));
			double sina;// = sin(Rad((*i)->a));
			cr::sincosf(Rad((*i)->a), &sina, &cosa );

			double handlex = x + cosa * HANDLE_DISTANCE;
			double handley = y + sina * HANDLE_DISTANCE;
			pEditTime->Line(x,y,handlex,handley, 0xff000000);
			CRect box(handlex,handley,handlex+1,handley+1);
			box.InflateRect(3,3);
			pEditTime->Fill(box, 0xff000000);
			box.DeflateRect(1,1);
			pEditTime->FillGradient(box, 0x00000000, highlight, DIR_UP);
			pEditTime->FillGradient(box, 0x00000000, highlight, DIR_UP);

		}
	}


	//When a movement draws, it also indicates the end of a draw

	EndDraw();

}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
	//USEMYRESOURCES();

	iFont = pEditTime->CreateFont("Arial", 14, false, false);
	//int meh = pEditTime->AddImageFromResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_DOT));
	//int meh = pEditTime->AddImageFromResource(NULL, MAKEINTRESOURCE(IDR_DOT));
	//iDot = pEditTime->AddImageAsTexture(meh);//AddTextureFromResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_DOT));
	
	char dir[MAX_PATH];
	GetModuleFileName(NULL, dir, MAX_PATH);	

	char drive[_MAX_DRIVE], directory[_MAX_DIR], fn[_MAX_FNAME], ext[_MAX_EXT];
	_splitpath(dir, drive, directory, fn, ext);

	CString path;
	path.Format("%s%sdot.png", drive, directory);
	
	iDot = pEditTime->AddTextureFromFile(path);

	allLimbs.clear();
	limb.UpdateAllLimbs(allLimbs);
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
	// Limit all 0-1000
	//Clamp<float>(maxspeed, 0, 1000);
	//Clamp<float>(acc, 0, 1000);
	//Clamp<float>(dec, 0, 1000);
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
	int Version = 1;
	SerializeVersion(ar, Version);

	limb.Serialize(ar);

	allLimbs.clear();
	limb.UpdateAllLimbs(allLimbs);

	if (ar.loading)
	{
		// Update our allLimbs list
		anim.clear();
		frame.clear();
		
		int size;
		ar >> size;
		frame.clear();
		for(int f = 0; f < size; f++)
		{
			int index;
			ar >> index;
			frame[index].Serialize(ar, allLimbs);
		}

		ar >> size;
		anim.clear();
		for(int a = 0; a < size; a++)
		{
			int index;
			ar >> index;
			anim[index].Serialize(ar, frame);
		}

		
		limb.SetBonesToFrame(&frame[curFrame]);

		ar >> unique_id;
	}
	else
	{
		ar << (int)frame.size();

		//CString debug;
		//debug.Format("%d", frame.size());
		//MessageBox(0, debug, 0,0);
		for(map<int, KeyFrame>::iterator f = frame.begin(); f != frame.end(); f++)
		{
			ar << f->first;
			f->second.Serialize(ar, allLimbs);
		}

		ar << (int)anim.size();
		for(map<int, KeyAnimation>::iterator a = anim.begin(); a != anim.end(); a++)
		{
			ar << a->first;
			a->second.Serialize(ar, frame);
		}
		
		ar << unique_id;

	}
}





KeyFrame* EditExt::GetFrame(int curframe)
{
	if(frame.find(curframe) == frame.end()){
		return NULL;
	}
	return &frame[curframe];
}

KeyAnimation* EditExt::GetAnimation(int curAnim)
{
	return &anim[curAnim];
}

void EditExt::SetBonesToFrame(int curframe)
{
	KeyFrame* pFrame = GetFrame(curframe);
	limb.SetBonesToFrame(pFrame);
}

void EditExt::SetFrameToBones(int curframe)
{
	KeyFrame* pFrame = GetFrame(curframe);
	limb.SetFrameToBones(pFrame);
}

void EditExt::OnMessage(int message)
{
	pParentInfo = pEditTime->GetObjectBelongingToThisBehavior();
	float ox = pParentInfo->objectX;
	float oy = pParentInfo->objectY;
	float zoom = pEditTime->GetZoom();
	CPoint mouse2 = pEditTime->GetCursor();
	POINTF mouse;
	mouse.x = mouse2.x;
	mouse.y = mouse2.y;


	switch (message)
	{
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		{
			HWND FrameWindow = pEditTime->GetFrameHwnd();
			InvalidateRect(FrameWindow, NULL, FALSE);
		}
		break;
		case WM_LBUTTONDOWN:
		{
			HWND FrameWindow = pEditTime->GetFrameHwnd();
			pSelectedLimb = false;
			if(m_Add == 1 && pMouseOver)
			{
				// Dont allow us to use the main body or an already existing limb
				if(pParentInfo != pMouseOver && !GetLimb(pMouseOver))
				{
					m_AttachLimb = pMouseOver;
					m_Add ++;
				}
			}
			else if(m_Add == 2 && pMouseOver)
			{
				if(pMouseOver->ObjectIdentifier != m_AttachLimb->ObjectIdentifier)
				{
					//We must be selecting a limb or the main body
					if(pParentInfo == pMouseOver || GetLimb(pMouseOver))
					{
						m_AttachToLimb = pMouseOver;
						m_Add ++;
					}
				}
			}
			else if(m_Add == 3)
			{
				m_Add ++;
			}
			else if(m_Add == 0)
			{
				for(vector<Limb*>::iterator i = allLimbs.begin(); i!= allLimbs.end(); i++)
				{
					double x = (*i)->pivot_pixelx;
					double y = (*i)->pivot_pixely;

					double cosa = cos(Rad((*i)->a));
					double sina = sin(Rad((*i)->a));

					double handlex = x + cosa * HANDLE_DISTANCE;
					double handley = y + sina * HANDLE_DISTANCE;
					
					// Check if we are clicking a handle
					CRect box(handlex,handley,handlex+1,handley+1);
					box.InflateRect(5,5);

					CPoint mouse2 = CPoint(mouse.x, mouse.y);

					if(box.PtInRect(mouse2))
					{
						drag_handle = (*i);
						drag_point = NULL;
						drag_olda = drag_handle->a;
					}
					else if(drag_handle == NULL) // if we have a handle, then ignore the drag points
					{
						// If not, check if we are clicking the pivot
						box = CRect(x, y, x+1, y+1);
						box.InflateRect(5,5);
						if(box.PtInRect(mouse2))
						{
							drag_point = (*i);
							pSelectedLimb = drag_point;
							drag_oldx = x;
							drag_oldy = y;
							getObjectDragInfo = true; // we need to get a bit more info
													  // for dragging, but its not available atm
						}
					}
				}
			}
			dragstart.x = mouse.x;
			dragstart.y = mouse.y;
			dragoffset.x = 0;
			dragoffset.y = 0;
			InvalidateRect(FrameWindow, NULL, FALSE);
		}
		break;
		case WM_LBUTTONUP:
		{
			HWND FrameWindow = pEditTime->GetFrameHwnd();
			if(m_Add == 0)
			{
				drag_handle = NULL;
				drag_point = NULL;
			}
			else if(m_Add == 4)
			{
				// All complete. Lets add the limb :D
				editInfo* pLimbChildInfo = m_AttachLimb;
				editInfo* pLimbParentInfo = m_AttachToLimb;
				if(pLimbChildInfo && pLimbParentInfo)
				{
					Limb* pLimb = NULL;
					
					Limb* pParent = GetLimb(pLimbParentInfo);
					if(pParent)
					{
						pParent->limbs.push_back(Limb());
						pLimb = &pParent->limbs.back();

						// We need to update all the limbs
						// This should be done better, but current the begin
						// and end draw functions simply prepare the objects
						// for rendering and update the co-ordinates. This is 
						// exactly what we want to do.
						BeginDraw();

						pLimb->a = pLimbChildInfo->objectAngle;
						pLimb->w = 1;
						pLimb->h = 1;
						pLimb->filter = 0xffffff;
						pLimb->opacity = 1;

						double cospa = cos(Rad(pLimbParentInfo->objectAngle));
						double sinpa = sin(Rad(pLimbParentInfo->objectAngle));
						double cosca = cos(Rad(pLimbChildInfo->objectAngle));
						double sinca = sin(Rad(pLimbChildInfo->objectAngle));

						mouse.x /= pEditTime->GetZoom();
						mouse.y /= pEditTime->GetZoom();

						pLimb->cx = cosca * (mouse.x - pLimbChildInfo->objectX) + sinca * (mouse.y - pLimbChildInfo->objectY);
						pLimb->cy = -sinca * (mouse.x - pLimbChildInfo->objectX) + cosca * (mouse.y - pLimbChildInfo->objectY);
						pLimb->cx /= pLimbChildInfo->objectWidth;
						pLimb->cy /= pLimbChildInfo->objectHeight;

						pLimb->px = cospa * (mouse.x - pLimbParentInfo->objectX) + sinpa * (mouse.y - pLimbParentInfo->objectY);
						pLimb->py = -sinpa * (mouse.x - pLimbParentInfo->objectX) + cospa * (mouse.y - pLimbParentInfo->objectY);
						pLimb->px /= pLimbParentInfo->objectWidth;
						pLimb->py /= pLimbParentInfo->objectHeight;

						pLimb->instanceID = pLimbChildInfo->instanceID;
						pLimb->ObjectIdentifier = pLimbChildInfo->ObjectIdentifier;
						pLimb->globalID = pLimbChildInfo->globalID;

						EndDraw();

						allLimbs.clear();
						limb.UpdateAllLimbs(allLimbs);

					}
				}

				m_Add = 0;
				pEditTime->RefreshProperties();

			}
			InvalidateRect(FrameWindow, NULL, FALSE);
		}
		break;

		case WM_MOUSEMOVE:
		{
			HWND FrameWindow = pEditTime->GetFrameHwnd();
			
			// Account for bug in construct where mousemove seems to keep being called.
			if(dragoffset.x == mouse.x - dragstart.x
				&& dragoffset.y == mouse.y - dragstart.y)
				break;


		
			dragoffset.x = mouse.x - dragstart.x;
			dragoffset.y = mouse.y - dragstart.y;

			if(m_Add == 1 || m_Add == 2)
			{
				pMouseOver = pEditTime->GetObjectAt(CPoint(mouse.x, mouse.y));
			}

			if(m_Add == 0)
			{
				if(drag_handle)
				{
					float oldangle = drag_handle->a;
					drag_handle->a = drag_olda;
					double x = drag_handle->pivot_pixelx;
					double y = drag_handle->pivot_pixely;
				
					double cosa = cos(Rad(drag_handle->a));
					double sina = sin(Rad(drag_handle->a));

					double handlex = x + cosa * HANDLE_DISTANCE;
					double handley = y + sina * HANDLE_DISTANCE;

					handlex += dragoffset.x;
					handley += dragoffset.y;


					drag_handle->a = Deg(atan2(handley - y, handlex - x));

					if(GetKeyState(VK_SHIFT)>>2)
					{
						drag_handle->RotateChildren(drag_handle->a - oldangle);
					}
				}
				if(drag_point)
				{
					BeginDraw();
					Limb* pLimb = drag_point;

					editInfo* pLimbChildInfo = pEditTime->FindObjectByAnID(pLimb->instanceID, pLimb->ObjectIdentifier);
					if(!pLimbChildInfo)
						return EndDraw();
			
					Limb* pParent = GetParent(drag_point);
					if(!pParent)
						return EndDraw();
			
					editInfo* pLimbParentInfo = pEditTime->FindObjectByAnID(pParent->instanceID, pParent->ObjectIdentifier);
					if(!pLimbParentInfo)
						return EndDraw();

					if(getObjectDragInfo)
					{
						drag_objectX = pLimbChildInfo->objectX;
						drag_objectY = pLimbChildInfo->objectY;
						getObjectDragInfo = false;
					}



					double cospa = cos(Rad(pLimbParentInfo->objectAngle));
					double sinpa = sin(Rad(pLimbParentInfo->objectAngle));
					double cosca = cos(Rad(pLimbChildInfo->objectAngle));
					double sinca = sin(Rad(pLimbChildInfo->objectAngle));

					mouse.x = drag_oldx + dragoffset.x;
					mouse.y = drag_oldy + dragoffset.y;

					mouse.x /= zoom;
					mouse.y /= zoom;



					if(GetKeyState(VK_CONTROL) >> 2)
					{
						//Todo: Dragging the limb will rotate the parent
					
						//Limb* pParent = pLimb->LookForParent(limb);
					}
					else
					{
						if(GetKeyState(VK_SHIFT) >> 2)
						{
						  pLimb->cx = cosca * (mouse.x - drag_objectX) + sinca * (mouse.y - drag_objectY);
						  pLimb->cy = -sinca * (mouse.x - drag_objectX) + cosca * (mouse.y - drag_objectY);
						  pLimb->cx /= pLimbChildInfo->objectWidth;
						  pLimb->cy /= pLimbChildInfo->objectHeight;
						}
						else
						{
							getObjectDragInfo = true;
						}

						pLimb->px = cospa * (mouse.x - pLimbParentInfo->objectX) + sinpa * (mouse.y - pLimbParentInfo->objectY);
						pLimb->py = -sinpa * (mouse.x - pLimbParentInfo->objectX) + cospa * (mouse.y - pLimbParentInfo->objectY);
						pLimb->px /= pLimbParentInfo->objectWidth;
						pLimb->py /= pLimbParentInfo->objectHeight;
					}

					pLimb->instanceID = pLimbChildInfo->instanceID;
					pLimb->ObjectIdentifier = pLimbChildInfo->ObjectIdentifier;
					pLimb->globalID = pLimbChildInfo->globalID;

					EndDraw();

				}
			}

			if(!(GetKeyState(VK_LBUTTON) >> 2))
			{
				// This code is used so we can determine what happens if the mouse is over a handle or point!
				for(vector<Limb*>::iterator i = allLimbs.begin(); i!= allLimbs.end(); i++)
				{
					(*i)->mouseover = false;
					{
						double x = (*i)->pivot_pixelx;
						double y = (*i)->pivot_pixely;

						double cosa = cos(Rad((*i)->a));
						double sina = sin(Rad((*i)->a));

						double handlex = x + cosa * HANDLE_DISTANCE;
						double handley = y + sina * HANDLE_DISTANCE;
						
						// Check if we are clicking a handle
						CRect box(handlex,handley,handlex+1,handley+1);
						box.InflateRect(5,5);

						if(box.PtInRect(CPoint(mouse.x, mouse.y))) // over handle
						{
							(*i)->mouseover = true;
						}
						else if(drag_handle == NULL) // if we have a handle, then ignore the drag points
						{
							// If not, check if we are clicking the pivot
							box = CRect(x, y, x+1, y+1);
							box.InflateRect(5,5);
							if(box.PtInRect(CPoint(mouse.x, mouse.y)))
							{
								(*i)->mouseover = true;
							}
						}
					}
				}
			}


			InvalidateRect(FrameWindow, NULL, FALSE);
		}
		break;

		case WM_PAINT:
			BeginDraw();
			break;

		case WM_KEYDOWN:
			if(GetKeyState(VK_DELETE) >> 2)
			{
				HWND FrameWindow = pEditTime->GetFrameHwnd();
				limb.RemoveLimb(pSelectedLimb);

				for( map<int, KeyFrame>::iterator i = frame.begin(); i!= frame.end(); i++)
				{
					for(map< Limb*, LimbData>::iterator l = i->second.limb.begin(); l!= i->second.limb.end(); l++)
					{
						if(l->first == pSelectedLimb)
						{
							i->second.limb.erase(l);
							break;
						}
					}
				}

				allLimbs.clear();
				limb.UpdateAllLimbs(allLimbs);
				pSelectedLimb = NULL;

				InvalidateRect(FrameWindow, NULL, FALSE);
			}
			break;

	}

}




void EditExt::BeginDraw()
{
	// We are about to begin drawing. This function is only called when
	// the layout editor is locked. Therefore, we can assume
	// that m_Edit is true.

	pParentInfo = pEditTime->GetObjectBelongingToThisBehavior();
	
	//m_Backup.push_back(ObjectBackup(pParentInfo));

	//pParentInfo->objectX = 32;
	//pParentInfo->objectY = 32;


	limb.Update(pParentInfo, this);

	if(m_Add == 4)
	{
		editInfo* Info = m_AttachLimb;
		this->m_Backup.push_back(ObjectBackup(Info));
		Info->objectX += dragoffset.x / pEditTime->GetZoom();
		Info->objectY += dragoffset.y / pEditTime->GetZoom();
	}

}


void EditExt::EndDraw()
{
	while(m_Backup.size() > 0)
	{
		m_Backup.back().Restore();
		m_Backup.pop_back();
	}
}

void EditExt::Edit()
{
	m_Edit = true;
	pEditTime->CaptureFrameEditor(pInfo->instanceID);

	pParentInfo = pEditTime->GetObjectBelongingToThisBehavior();

	limb.instanceID = pParentInfo->instanceID;
	limb.ObjectIdentifier = pParentInfo->ObjectIdentifier;
	limb.globalID = pParentInfo->globalID;

	Initialize();
}
void EditExt::CloseEdit()
{
	limb.SetFrameToBones(&frame[curFrame]);
	m_Edit = false;
	pEditTime->ReleaseFrameEditor(pInfo->instanceID);
}
void EditExt::CancelObject()
{
	m_Add = 0;
	pEditTime->RefreshProperties();
}
void EditExt::AddObject()
{
	m_Add = 1;
	pEditTime->RefreshProperties();
}
Limb* EditExt::GetParent(Limb* child)
{
	return limb.LookForParent(child);
}




void WINAPI ETOnMessage(editInfo* editObject, int message)
{
	editObject->objectPtr->OnMessage(message);
}

ObjectBackup::ObjectBackup(editInfo* _pInfo)
{
	pInfo = _pInfo;
	x = pInfo->objectX;
	y = pInfo->objectY;
	w = pInfo->objectWidth;
	h = pInfo->objectHeight;
	a = pInfo->objectAngle; 
	filter = pInfo->filter;

}

void ObjectBackup::Restore()
{
	pInfo->objectX = x;
	pInfo->objectY = y;
	pInfo->objectWidth = w;
	pInfo->objectHeight = h;
	pInfo->objectAngle = a; 
	pInfo->filter = filter;
}

Limb* EditExt::GetLimb(editInfo* pInfo)
{
	for(vector<Limb*>::iterator i = allLimbs.begin(); i!= allLimbs.end(); i++)
	{
		if((*i)->ObjectIdentifier == pInfo->ObjectIdentifier)
			return *i;


	}
	return NULL;
}

void Limb::RotateChildren(double dAngle)
{
	for(list<Limb>::iterator i = limbs.begin(); i!= limbs.end(); i++)
	{
		i->a += dAngle;
		i->RotateChildren(dAngle);
	}
}

void EditExt::RemoveAnimation()
{
	if(anim.size() == 1)
	{
		MessageBox(0,"Sorry you must have at least 1 animation", "Bone Movement",0);
		return;
	}

	if(IDOK == MessageBox(NULL, "Are you sure you want to remove this animation", "Bone Movement", MB_OKCANCEL))
	{
		map<int, KeyAnimation>::iterator it = anim.find(curAnim);
		if(it != anim.end())
		{
			anim.erase(it);
			curAnim = anim.begin()->first;
		}
		pEditTime->RefreshProperties();
	}
}
void EditExt::RemoveFrame()
{

	KeyAnimation* pAnim = this->GetAnimation(curAnim);
	if(pAnim){
		if(pAnim->frames.size() == 1)
		{
			MessageBox(0,"Sorry you must have at least 1 frame", "Bone Movement",0);
			return;			
		}
		if(IDOK == MessageBox(NULL, "Are you sure you want to remove this frame", "Bone Movement", MB_OKCANCEL))
		{
			map<int, KeyFrame>::iterator i = frame.find(curFrame);

			pAnim->RemoveAllFramesThatUseKeyframe(&i->second);

			frame.erase(i);

			for( i = frame.begin(); i!= frame.end(); i++)
			{
				if(pAnim->frames[0] == &i->second)
				{
					curFrame = i->first;
					limb.SetBonesToFrame(&i->second);
				}
			}
			pEditTime->RefreshProperties();
		}	

	}
	pEditTime->RefreshProperties();
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
void WINAPI ETOnMessage(editInfo* editObject, int message)  {}
#endif // RUN_ONLY