// CLayout.cpp
// All CLayout functions

#include "StdAfx.h"
#include "CDragObjects.h"

extern CLayoutEditor* g_pLayoutView;
extern BOOL g_bPaste;
extern BOOL g_bPasteClone;

void nest_add_object(CObj* o, vector<CObj*>& init_later)
{
	init_later.push_back(o);
	if(o->m_bIsGroup)
	{
		POSITION pos = o->m_groupObjects.GetHeadPosition();
		while(pos)
		{
			CObj* o2;
			g_pFrame->objects.Lookup(o->m_groupObjects.GetNext(pos),o2);
			nest_add_object(o2, init_later);
		}
	}


}





void CDragObjects::Serialize(CArchive& ar)
{
	CString ExpectedName = "CDragObjects";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return;
	// Note: application etc are the ones the data came from
	//  the global information: g_pApp are what the data is going to
	//   note note: we should note use global variables for it...we could have easily used
	//   member variables...GOSH!! - davo

	// Changes by davo:
	// 1 - dragging and dropping between frames no longer creates a new type because types are all global
	/*  Theres a new method used for handling application to application transfers

	Basically it works like this:

	We have a special serialize function which is trigger when an object serializes its image.

	If we copy a sprite from one application to another, it is important that we copy over the 
	images as well. However, there is a complication. The image number also needs to be changed.

	We have to account also that there could be lots of sprites all with the same image - and creating a new
	images for every single one is not the desired result. So what we do is in our application we store some
	special data which instructs us where images from applications go.

	The sytem works like this.

	Theres a map of application pointers which direct us to a map of integers -> integers.
	This second map allows us to redirect an image index from one application into an image index in another application. 

	Initially these maps are blank. The system works like this:

	1. you drag and drop 3 sprites from application1 to application2.
	First object - ah its from another application ...ah we are serializin an image...lets see if we have it mapped...no we dont..okay we know its
	image 3 in the source application, lets append it onto our images list...

	second object - ' '   ' ' ' '  ' ' ' ' '' '  lets see if we have it mapped...ah we do...says here that it becomes image 7...lets do that then


	...simple.

	We dont save this information as its just tempory while the user has both applications open and is clearly copying stuff over


	*/

	//important we clear this...going to and from both use it!


	if (g_pApp == NULL)
		g_pApp = application;

	g_pApp->images_to_save.clear();
	g_pApp->animations_to_save.clear();

	if( ar.IsLoading() )
	{
		g_pLayoutView->SetRedraw(false);
		int i;
		long p = 0;
		ar >> clipboard;
		ar >> m_curPt;
		ar >> p;
		m_pSel = (CObjList*)p;
		ar >> p;
		layout = (CLayout*)p;
		ar >> p;
		application = (CApplication*)p;
		ar >> p;
		layout_editor = (CLayoutEditor*)p;

		
		int c;
		ar >> c;
		long n;
		layers.clear();
		for (int j=0; j < c; j++) {
			ar >> n;
			layers.push_back(n);
		}

		// Serialize the object types
		ar >> c;
		map<CObjType*, int> objectTypesToSave;
		bool have_used_types = false;
		for(int ot = 0; ot < c; ot++)
		{
			CObjType* pType = new CObjType;
			pType->Serialize(ar);
			int number;
			ar >> number;

			objectTypesToSave[pType] = number;
		}

		ar >> c;


		CObj* o = NULL;
		if(g_bDuplicate)
			*g_pDrop = DROPEFFECT_COPY;
		if (*g_pDrop == DROPEFFECT_MOVE && layout == g_pFrame) 
		{
			// Simple move
			POSITION pos = m_pSel->GetHeadPosition();
			for (int i=0; i < m_pSel->GetCount(); i++) 
			{
				long object = m_pSel->GetNext(pos);
				if (layout->objects.Lookup(object, o)) 
				{
					// START UNDO INFO //
					CAction *action = new CAction();
					action->m_type = ACTION_MOVESIZE;
					if (i > 0)
						action->m_bGroup = FALSE; // all objects will be moved back in one undo

					CArchive *ar = action->Prepare1();					
					long ID = o->GetInstanceID();
					*ar << ID;
					o->Serialize(*ar);
					action->Prepare2();
					layout_editor->m_undo->AddAction(action);
					// END UNDO INFO //
					

					CPoint ptNew = 
						CPoint(floor(o->GetX() * g_pLayoutView->m_Zoom + (g_pPt->x - m_curPt.x)  )
								,floor(o->GetY()*g_pLayoutView->m_Zoom + (g_pPt->y - m_curPt.y)) )
						 - 
						(CPoint(floor(FRAME_MARGIN_W*g_pLayoutView->m_Zoom), floor(FRAME_MARGIN_H*g_pLayoutView->m_Zoom)) - g_pLayoutView->GetScrollPosition());
					
					o->SetPos(g_pLayoutView->RoundGridX(ptNew.x/g_pLayoutView->m_Zoom), g_pLayoutView->RoundGridY(ptNew.y/g_pLayoutView->m_Zoom));
				}
			}
		}

		//else if (*g_pDrop != DROPEFFECT_NONE) 
		else
		{

			if (*g_pDrop == DROPEFFECT_MOVE) 
			{ 
				// we are dragging, not duplicating, so delete the old objects
				layout_editor->DeleteSelection();
			}

			g_pSel->RemoveAll();
			CObjMap* objMap = &g_pFrame->objects;

			if(g_pApp == application)
			{
				g_pApp->m_drag_app_from = 0;
				g_bDuplicate = FALSE;
			}
			else	//From another application...crap...
			{
				g_pApp->m_drag_app_from = application;
				g_bDuplicate = FALSE;
			}

			vector<CObj*> init_later;
			vector<CObj*> layer_moves;

			for (i=0; i < c; i++) 
			{
				//TODO: add undo information



				o = new CObj();
				o->bSerializeData = TRUE;
				o->DuplicateSerialize = true;
				o->Serialize(ar);
				o->DuplicateSerialize = false;
				o->bSerializeData = FALSE;
				o->m_bInGroup = false;
				

				
				nest_add_object(o, init_later);
		

				layer_moves.push_back(o);
				//
				//	

				


			}
		
			map<int, int> remapObjectTypes;
			//We have finished dragging this. Time to copy over the images/animations/objecttypes
			if(g_pApp->m_drag_app_from)
			{


				// Serialize Animations

				map<int, int>& animfromto = g_pApp->m_animfrom_animto_map[g_pApp->m_drag_app_from];

				int size;
				ar >> size;
				for(int i = 0; i < size; i++)
				{
					int fixed;
					ar >> fixed;
					CAnimation* anim = g_pApp->resources.FindAnimationFromNumber(animfromto[fixed]);
					fixed = anim->m_FixedID; // remember new number

					anim->Serialize(ar);
					anim->SerializeFromOtherApplicationStep(g_pApp);
					anim->m_FixedID = fixed;


					
				}
				


				// Serialize Images

				map<int, int>& imagefromto = g_pApp->m_imagefrom_imageto_map[g_pApp->m_drag_app_from];
				map<int, CObj*>::iterator it;

				ar >> size;
				for(int i = 0; i < size; i++)
				{
					// Change by AG.  Using pointers.
					int p;
					ar >> p;
					CImageResource* loadedImgPtr = (CImageResource*)p;
					CImageResource* img = g_pApp->resources.FindImageResourceFromNumber(imagefromto[loadedImgPtr->m_FixedID]);

					int fixed = img->m_FixedID;

					//if (img != loadedImgPtr) {
						//*img = *loadedImgPtr;
					img->Copy(loadedImgPtr);
					img->m_FixedID = fixed;
					//}
				}
				
				// Also copy over object types
				map<CObjType*, int>::iterator ot = objectTypesToSave.begin();
				for(; ot != objectTypesToSave.end(); ot++)
				{
					long identifier = g_pApp->m_objID++;
					remapObjectTypes[ot->second] =  identifier;
					CObjType* pType = ot->first;
									
					g_pApp->object_types[identifier] = pType;
					pType->ObjectIdentifier = identifier;
				}
				have_used_types = true;
			}		

		
			//now create the objects
			for(int a = init_later.size()-1; a >=0 ; a --)
			{
				o = init_later[a];
				long oldID = o->GetInstanceID();
				long oID = -1;
				if (application->m_appID == g_pApp->m_appID)
					oID = o->GetGlobalID();
	
				CPoint ptNew = 
						CPoint(g_pLayoutView->RoundGridX(o->GetX()* g_pLayoutView->m_Zoom)
								,g_pLayoutView->RoundGridY(o->GetY()* g_pLayoutView->m_Zoom))
						+ (*g_pPt - m_curPt);

				// Due to the system of making an object x,y,width, height change to reflect zooming in,
				// in the event that we are pasting, we need to make the point be in real co-ordinates instead
				// of zoom scaled co-ordinates. Stupid system = stupid code
				if(g_bPaste || g_bPasteClone)
				{
					ptNew = 
						CPoint(g_pLayoutView->RoundGridX(o->GetX())
								,g_pLayoutView->RoundGridY(o->GetY()))
						+ (*g_pPt - m_curPt);
				}

				g_pLayoutView->m_AutoIncrement = true;
				if(a == init_later.size()-1)
					g_pLayoutView->m_AutoIncrement = false;
				
				if(remapObjectTypes.size() > 0)
				{
					oID = remapObjectTypes[o->GetGlobalID()];
					o->editObject->ObjectIdentifier = oID;
				}

				g_pLayoutView->CreateObject(NULL, ptNew, o, oID, g_bDuplicate);


			

				
				long ID = o->GetInstanceID();

				if(o->m_bInGroup && o->m_group)
				{
					CObj* parent = o->m_group;
					
				//	parent->m_groupObjects.SetAt(parent->m_groupObjects.FindIndex(oldID), ID);
				}
				else
				{
					g_pSel->AddTail(ID);	//we dont add to the selection objects inside groups
				}

				g_pLayoutView->UpdateSelection();


			}

			// ctrl-drag restore proper layer
			if (*g_pDrop == DROPEFFECT_COPY && layout_editor->m_bLayersAfterMove
					&& g_pLayoutView->layout == layout_editor->layout) {
				for (int l = 0; l < c; l++) {
					o = layer_moves[l];
					
					// get layer		
					long layid = layers[l];
					POSITION lpos = layout_editor->layout->layers.GetHeadPosition();
					CLayer *layer;
					for (int k = 0; k < layout_editor->layout->layers.GetCount(); k++) {
						layer = layout_editor->layout->layers.GetNext(lpos);
						if (layer->m_layerID == layid)
							break;
					}

					// move object to correct layer
					long id = o->GetInstanceID();
					CLayer *layer2 = layout_editor->GetObjectLayer(id);
					if (layer != layer2) {
						POSITION found = layer2->m_zOrder.Find(id);
						layer2->m_zOrder.RemoveAt(found);
						layer->m_zOrder.AddTail(id);
					}
				}
			}

			g_pApp->m_drag_app_from = 0;	//must be kept null else it will mess up saving

		}
		g_pLayoutView->SetRedraw(true);

		layout_editor->m_bLayersAfterMove = FALSE;
		
		// We didn't use the types, they need freeing		
		if(!have_used_types)
		{	
			map<CObjType*, int>::iterator ot = objectTypesToSave.begin();
			for(; ot != objectTypesToSave.end(); ot++)
			{
				delete ot->first;
			}
		}
	}
	
	// Saving
	else
	{
		long p;
		ar << clipboard;
		ar << m_curPt;
		p = (long)m_pSel;
		ar << p;
		p = (long)layout;
		ar << p;
		p = (long)application;
		ar << p;
		p = (long)layout_editor;
		ar << p;

		int c = layers.size();
		ar << c;
		long n;
		for (int j=0; j < c; j++) {
			n = layers[j];
			ar << n;
		}

		CObj* o;
		application->images_to_save.clear();
		application->animations_to_save.clear();

		// Work out what object types are used
		POSITION pos = m_pSel->GetHeadPosition();
		map<CObjType*, int> objectTypesToSave;
		for (int i=0; i < m_pSel->GetCount(); i++) 
		{
			long object = m_pSel->GetNext(pos);
			if (layout->objects.Lookup(object, o)) 
			{
				CObjType* pType = o->GetObjectType(application);
				objectTypesToSave[pType] = pType->ObjectIdentifier;
			}
		}
		// Serialize the object types
		map<CObjType*, int>::iterator ot = objectTypesToSave.begin();
		ar << objectTypesToSave.size();
		for(; ot != objectTypesToSave.end(); ot++)
		{
			ot->first->Serialize(ar);
			ar << ot->second;
		}

		c = m_pSel->GetCount();
		ar << c;
		pos = m_pSel->GetHeadPosition();
		// Now serialize the instances selected
		for (int i=0; i < m_pSel->GetCount(); i++) 
		{
			long object = m_pSel->GetNext(pos);
			if (layout->objects.Lookup(object, o)) 
			{
				o->bSerializeData = TRUE;
				if (o->GetGlobalID() != -1)
				{
					CObjType* pType = o->GetObjectType(application);
					if(pType)
					{
						o->m_tempName = pType->GetName();
					}
				}
				o->Serialize(ar);
				o->bSerializeData = FALSE;
			}
		}

		
		map<int, CObj*>::iterator it;



		ar << (int)application->animations_to_save.size();
		for(it = application->animations_to_save.begin(); it != application->animations_to_save.end(); it++)
		{
			CAnimation* anim = application->resources.FindAnimationFromNumber(it->first);
			//img->Serialize(ar);
			// Change by AG.  Storing pointer to CImageResource.
			ar << anim->m_FixedID;
			anim->Serialize(ar);
		}


		ar << (int)application->images_to_save.size();
		for(it = application->images_to_save.begin(); it != application->images_to_save.end(); it++)
		{
			CImageResource* img = application->resources.FindImageResourceFromNumber(it->first);
			//img->Serialize(ar);
			// Change by AG.  Storing pointer to CImageResource.
			ar << (int)img;
		}

	}
	
}