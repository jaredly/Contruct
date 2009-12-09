
#include "StdAfx.h"
#include "..\mainfrm.h"
#include "..\Structure\Effect.h"
#include "..\Editors\LayoutEditor.h"
#include "Undo.h"
#include <list>
#include <vector>
using namespace std;
extern CLayerBar* g_pLayerBar;
extern CMainFrame* pMainWnd;
extern PropertiesBar* g_PropertyBar;
extern CLayoutEditor* g_pLayoutView;

//////////////////////////////////////////////
// CUNDO FUNCTIONS

// Constructor
CUndo::CUndo(CLayoutEditor *view)
{
	m_pFrameEditor = view;
	m_pEventEditor = NULL;
}

CUndo::CUndo(EventSheetEditor *view)
{
	m_pEventEditor = view;
	m_pFrameEditor = NULL;
}

// Destructor
CUndo::~CUndo()
{
	ClearRedo();
	ClearUndo();
}

// Properties
BOOL CUndo::CanUndo()
{
	return !m_undo.IsEmpty();
}

BOOL CUndo::CanRedo()
{
	return !m_redo.IsEmpty();
}

// Action
void CUndo::AddAction(CAction *action)
{
	m_undo.AddTail(action);
	ClearRedo();
}

// Undo
void CUndo::Undo()
{
	if (CanUndo())
	{
		BOOL bRepeat = TRUE;
		int count = 0;
		CAction *action, *action2;
		while (bRepeat) {
			action = m_undo.GetTail();
			bRepeat = !(action->m_bGroup);
			if (m_pFrameEditor)
				action2 = m_pFrameEditor->PerformAction(action, (count > 0));
			else if (m_pEventEditor)
				action2 = m_pEventEditor->PerformAction(action, (count > 0));
			if(action2)
				m_redo.AddTail(action2);
			delete action;
			m_undo.RemoveTail();
			count++;
		}
	}
}

// Redo
void CUndo::Redo()
{
	if (CanRedo())
	{
		BOOL bRepeat = TRUE;
		int count = 0;
		CAction *action, *action2;
		while (bRepeat) {
			action = m_redo.GetTail();
			bRepeat = !(action->m_bGroup);
			if (m_pFrameEditor)
				action2 = m_pFrameEditor->PerformAction(action, (count > 0));
			else if (m_pEventEditor)
				action2 = m_pEventEditor->PerformAction(action, (count > 0));
		if(action2)
			m_undo.AddTail(action2);
			delete action;
			m_redo.RemoveTail();
			count++;
		}
	}
}

// Clear
void CUndo::ClearUndo()
{
	POSITION pos = m_undo.GetHeadPosition();
	for (int i = 0; i < m_undo.GetCount(); i++)
		delete m_undo.GetNext(pos);
	m_undo.RemoveAll();
}

void CUndo::ClearRedo()
{
	POSITION pos = m_redo.GetHeadPosition();
	for (int i = 0; i < m_redo.GetCount(); i++)
		delete m_redo.GetNext(pos);
	m_redo.RemoveAll();
}


CAction* CLayoutEditor::PerformAction(CAction *action, BOOL bRepeat)
{
	// variables
	long id, gid, prevObj, objLayer;
	long i, oid;
	CLayer *lyr;
	CObj *o, *o2;
	CObjType *oT;
	CObjList objs;
	CString text;
	BOOL b, tb;
	BOOL bMoreInstances;
	POSITION pos, pos2;

	g_pLayoutView = this;
	g_pFrame = layout;
	g_pApp = application;

	long tid, tprevObj, tobjLayer, tglobalID;

	// do stuff
	CArchive *ar = action->Unprepare1();
	CAction *actionNew = new CAction();
	if (bRepeat)
		actionNew->m_bGroup = FALSE;
	CArchive *arNew = actionNew->Prepare1();

	switch (action->m_type) 
	{
	case ACTION_MOVESIZE:
		*ar >> id;
		o = GetObject(id);
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_MOVESIZE;
		tid = o->GetInstanceID();
		*arNew << tid;
		o->Serialize(*arNew);
		// END UNDO INFO //

		o->Serialize(*ar);

		InitializeObject(o);
		g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props
		break;

	case ACTION_CHANGETYPE:
		*ar >> id;
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_CHANGETYPE;
		*arNew << id;
		application->object_types[id]->Serialize(*arNew);
		// END UNDO INFO //
		
		application->object_types[id]->Serialize(*ar);

		g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props
		break;

	case ACTION_CHANGELAYER:
		*ar >> id;
		
		pos = layout->layers.GetHeadPosition();
		for (i = 0; i < layout->layers.GetCount(); i++)
		{
			lyr = layout->layers.GetPrev(pos);
			if( lyr->m_layerID == id)
				break;
		}
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_CHANGELAYER;
		*arNew << id;
		lyr->Serialize(*arNew);
		// END UNDO INFO //

		lyr->Serialize(*ar);

		g_PropertyBar->Update(this, TYPE_LAYER, &m_sel, layout, &layout->objects, application, 0, lyr); // show object props
		break;
		
	case ACTION_SETVISIBLE:
		*ar >> id >> b;
		o = GetObject(id);
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_SETVISIBLE;
		tid = o->GetInstanceID();
		tb = !b;
		*arNew << tid << tb;
		// END UNDO INFO //

		o->SetVisible(b);
		break;
		
	case ACTION_SETLOCK:
		*ar >> id >> b;
		o = GetObject(id);
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_SETLOCK;
		tid = o->GetInstanceID();
		tb = !b;
		*arNew << tid << tb;
		// END UNDO INFO //

		o->SetVisible(b);
		break;
		
	case ACTION_CHANGEZORDER:

		// START UNDO INFO //
		actionNew->m_type = ACTION_CHANGEZORDER;
		this->SerializeAllZLists(arNew);
		// END UNDO INFO //

		SerializeAllZLists(ar);

		g_pLayerBar->m_layerDlg.RefreshLayers();

		// we need to tell the layer dlg to refresh or something!
		break;

	case ACTION_CREATE:
		{
			*ar >> id >> objLayer >> prevObj;
			o = new CObj();
			o->Serialize(*ar);
			
			if(objLayer!= -1 && prevObj != -1)	//group items dont go into the zorder
			{
				if(objLayer >= 0)
				{
					pos = layout->layers.GetHeadPosition();
					for (i = 0; i < layout->layers.GetCount(); i++) {
						lyr = layout->layers.GetNext(pos);
						if (lyr->m_layerID == objLayer) {
							lyr->m_zOrder.InsertAfter(lyr->m_zOrder.FindIndex(prevObj), id);
							break;
						}
					}
				}
				else
				{
					CObj* group = layout->GetObj(-objLayer); // layer is negative for object groups
					group->m_groupObjects.InsertAfter(group->m_groupObjects.FindIndex(prevObj), id);
					break;	
				}			
			}
		
			InitializeObject(o);

			layout->objects[id] = o;

			// START UNDO INFO //
			actionNew->m_type = ACTION_DELETE;
			tid = o->GetInstanceID();
			*arNew << tid;
			// END UNDO INFO //

			pMainWnd->ObjectTypesChanged();

			g_PropertyBar->Update(this, TYPE_LAYOUT, NULL, layout, NULL, application); 
		}

		g_pLayerBar->m_layerDlg.RefreshLayers();
		break;

	case ACTION_CREATETYPE:// long ObjectIdentifier, CObjType oT, long id, layer, prevobjid, CObj o
		{
			*ar >> gid;
			oT = new CObjType();
			oT->Serialize(*ar);
			application->object_types[gid] = oT;

			*ar >> id >> objLayer >> prevObj;
			o = new CObj();
			o->Serialize(*ar);
			if(objLayer!= -1 && prevObj != -1)	//group items dont go into the zorder
			{
				if(objLayer >= 0)
				{
					pos = layout->layers.GetHeadPosition();
					for (i = 0; i < layout->layers.GetCount(); i++)
					{
						lyr = layout->layers.GetNext(pos);
						if (lyr->m_layerID == objLayer) {
							lyr->m_zOrder.InsertAfter(lyr->m_zOrder.FindIndex(prevObj), id);
							break;
						}
					}
				}
				else
				{
					CObj* group = layout->GetObj(-objLayer); // layer is negative for object groups
					group->m_groupObjects.InsertAfter(group->m_groupObjects.FindIndex(prevObj), id);
					break;	
				}
			}
			layout->objects[id] = o;


			// We need to call initialize!
			InitializeObject(o);

			// START UNDO INFO //
			actionNew->m_type = ACTION_DELETE;
			tid = o->GetInstanceID();
			*arNew << tid;
			// END UNDO INFO //

			g_PropertyBar->Update(this, TYPE_LAYOUT, NULL, layout, NULL, application); 
		}

		pMainWnd->ObjectTypesChanged();
		g_pLayerBar->m_layerDlg.RefreshLayers();
		break;

	case ACTION_DELETE:
		{
			*ar >> id;
			o = GetObject(id);
			if(!o)
				return 0; // this actually happens in groups sometimes but is now accounted for

			tglobalID = o->GetGlobalID();

			// remove from zorder
			lyr = GetObjectLayer(id);
			if(lyr)
			{
				 pos2 = lyr->m_zOrder.Find(id);
				 tprevObj = lyr->m_zOrder.GetPrev(pos2);
				 tobjLayer = lyr->m_layerID;
				lyr->m_zOrder.RemoveAt(lyr->m_zOrder.Find(id));
			}
			else if(o->m_bInGroup)
			{
				 pos2 = o->m_group->m_groupObjects.Find(id);
				 tprevObj = o->m_group->m_groupObjects.GetPrev(pos2);
				 tobjLayer = 0 - o->m_group->GetInstanceID(); // we use negative values of the instance id of objects
				 o->m_group->m_groupObjects.RemoveAt(o->m_group->m_groupObjects.Find(id));
			}	

			bMoreInstances = FALSE; // check if any more instances exist
			pos = layout->objects.GetStartPosition();
			long nKey;
			while (pos != NULL) 
			{
				layout->objects.GetNextAssoc(pos, nKey, o2);

				if (o2->GetGlobalID() == tglobalID && o2->GetInstanceID() != id) 
				{
					bMoreInstances = TRUE;
					break;
				}
			}

			if (bMoreInstances == FALSE) // no more exist, delete object type
			{
				CObjType* oT = application->object_types[tglobalID];

				// START UNDO INFO //
				actionNew->m_type = ACTION_CREATETYPE;

				*arNew << tglobalID; //object type
				oT->Serialize(*arNew);

				long tid = o->GetInstanceID(); //object
				*arNew << tid << tobjLayer << tprevObj;
				o->Serialize(*arNew);
				// END UNDO INFO //

				//delete oT;
				application->m_DeletedObjectTypes[tglobalID] = application->object_types[tglobalID];
				application->object_types.RemoveKey(tglobalID);

				if (oT->tree)
				{
					pMainWnd->project_bar.tree.DeleteItem(oT->tree);
					oT->tree = NULL;
				}
			}
			else
			{
				// START UNDO INFO //
				actionNew->m_type = ACTION_CREATE;

				long tid = o->GetInstanceID();
				*arNew << tid << tobjLayer << tprevObj;
				o->Serialize(*arNew);
				// END UNDO INFO //
			}

			// okay time to recursively kill !
			pos = o->m_groupObjects.GetHeadPosition();
			//CObj *o2;
			for (int k = 0; k < o->m_groupObjects.GetCount(); k++) 
			{
				long groupitemid = o->m_groupObjects.GetNext(pos);
				DeleteObjectRecursive(groupitemid);

				// basically the serialization function automatically saves all the children items. We want to 
				// delete them but not back them up...thats where the DeleteOBjectRecursive function is handy :)
			}	

			delete o;
			layout->objects.RemoveKey(id);

			pMainWnd->ObjectTypesChanged();

			g_pLayerBar->m_layerDlg.RefreshLayers();

			g_PropertyBar->Update(this, TYPE_LAYOUT, NULL, layout, NULL, application); 
		}

		break;
		
	case ACTION_GROUP:
		*ar >> gid;
		objs.Serialize(*ar);
		m_sel.RemoveAll();
		pos = objs.GetHeadPosition();
		for (i = 0; i < objs.GetCount(); i++) {
			id = objs.GetNext(pos);
			m_sel.AddTail(id);
		}

		// taken from GroupSel(), penis removed //
		o2 = GetObject(gid);
		pos = m_sel.GetHeadPosition();
		for (i = 0; i < m_sel.GetCount(); i++) { // loop through selected objects
			o = GetObject(m_sel.GetNext(pos));
			if (o->m_bInGroup == FALSE) { // selected object is not in another [open] group
				o->m_bInGroup = TRUE;
				o->m_group = o2;
				lyr = GetObjectLayer(o->GetInstanceID());
				
				tid = o->GetInstanceID();
				if(lyr)
				{
					lyr->m_zOrder.RemoveAt(lyr->m_zOrder.Find(tid)); // move object from its layer to the group
				}
				o2->m_groupObjects.AddTail(tid);
				
			}
		}
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_UNGROUP;
		*arNew << gid;
		// END UNDO INFO //
		g_pLayerBar->m_layerDlg.RefreshLayers();
		break;
		
	case ACTION_UNGROUP:
		*ar >> gid;
		o = GetObject(gid);
		
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_GROUP;
		*arNew << gid;
		o->m_groupObjects.Serialize(*arNew);
		// END UNDO INFO //


		//BLOCK: UNGROUP
		// remove from zorder
//		CObj *o2;
		//POSITION pos2;
		CLayer *lyr;
		if (!o->m_bInGroup) {
			lyr = GetObjectLayer(gid);
			if(!lyr)
			{
				CErrorDlg	Error;
				Error.Error("Error in undo", "In ACTION_UNGROUP we were unable to work out what layer the group was on. Please report this to scirra");
				return 0;
			}
			pos2 = lyr->m_zOrder.Find(gid); // get location in layer to put group objects
		}

		pos = o->m_groupObjects.GetHeadPosition();
		for (int j = 0; j < o->m_groupObjects.GetCount(); j++) {
			o2 = GetObject(o->m_groupObjects.GetNext(pos));
			if (o->m_bInGroup)
				o2->m_group = o->m_group;
			else {
				o2->m_bInGroup = FALSE;
				o2->m_group = NULL;
				id = o2->GetInstanceID();
				lyr->m_zOrder.InsertAfter(pos2, id);
			}
		}
		o->m_groupObjects.RemoveAll();


	










/*
		// taken from Ungroup(), penis removed //
		if (!o->m_bInGroup) {
			lyr = GetObjectLayer(gid);
			if(!lyr)
			{
			CErrorDlg	Error;
			Error.Error("Error in undo", "In ACTION_UNGROUP we were unable to work out what layer the group was on. Please report this to scirra");
				return 0;
			}
			pos2 = lyr->m_zOrder.Find(gid); // get location in layer to put group objects
		}
		pos = o->m_groupObjects.GetHeadPosition();
		for (i = 0; i < o->m_groupObjects.GetCount(); i++) {
			o2 = GetObject(o->m_groupObjects.GetNext(pos));
			if (o->m_bInGroup)
				o2->m_group = o->m_group;
			else {
				o2->m_bInGroup = FALSE;
				o2->m_group = NULL;
				id = o2->GetInstanceID();
				lyr->m_zOrder.InsertAfter(pos2, id);
			}
		}
		o->m_groupObjects.RemoveAll();

		{	// this allows us to remove the group type and the group object - davo
	//	CUndo* temp = m_undo;
	//	m_undo = 0;
		//DeleteObject(gid);
	//	m_undo = temp; 
		}*/



		g_pLayerBar->m_layerDlg.RefreshLayers();


		break;
		
	case ACTION_GROUPOPEN:
		*ar >> id >> b;
		o = GetObject(id);
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_GROUPOPEN;
		tid = o->GetInstanceID();
		tb = !b;
		*arNew << tid << tb;
		// END UNDO INFO //

		o->m_bGroupOpen = b;
		g_pLayerBar->m_layerDlg.RefreshLayers();
		break;
		
	case ACTION_GROUPREMOVE:
		*ar >> gid >> oid >> objLayer >> prevObj;
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_GROUPADD;
		*arNew << gid << oid;
		// END UNDO INFO //

		o = GetObject(oid);
		o2 = GetObject(gid);
		o->m_groupObjects.RemoveAt(o->m_groupObjects.Find(oid));
		o->UpdateGroupInfo(FALSE);
		o2->m_bInGroup = FALSE;
		o2->m_group = NULL;
		for (i = 0; i < layout->layers.GetCount(); i++) {
			lyr = layout->layers.GetNext(pos);
			if (lyr->m_layerID == objLayer) {
				lyr->m_zOrder.InsertAfter(lyr->m_zOrder.FindIndex(prevObj), oid);
				break;
			}
		}
		g_pLayerBar->m_layerDlg.RefreshLayers();
		break;
		
	case ACTION_GROUPADD:
		*ar >> gid >> oid;

		o = GetObject(oid);
		o2 = GetObject(gid);
		o->m_bInGroup = TRUE;
		o->m_group = o2;
		lyr = GetObjectLayer(oid);
		if(!lyr)
		{
			CErrorDlg Error;
			Error.Error("Error", "Unable to work out what layer the object is on in ACTION_GROUPADD, please report this to scirra");
			return 0;
		}
		pos = lyr->m_zOrder.Find(oid);
		prevObj = lyr->m_zOrder.GetPrev(pos);
		
		// START UNDO INFO //
		actionNew->m_type = ACTION_GROUPREMOVE;
		*arNew << gid << oid << lyr->m_layerID << prevObj;
		// END UNDO INFO //

		lyr->m_zOrder.RemoveAt(lyr->m_zOrder.Find(oid));
		o->m_group->m_groupObjects.AddTail(oid);
		g_pLayerBar->m_layerDlg.RefreshLayers();
		break;
	}

	action->Unprepare2();
	actionNew->Prepare2();

	if(action == 0)
	{
		int a = 0;
		a --;
	}
	if(actionNew == 0)
	{
		int a = 0;
		a--;
	}
	return actionNew;
}


//////////////////////////////////////////////
// CACTION FUNCTIONS

// Constructor
CAction::CAction() {
	m_ar = NULL;
	m_sf = NULL;
	m_mf = NULL;
	m_data = NULL;
	m_bGroup = TRUE;
}

// Destructor
CAction::~CAction() {
	if (m_ar != NULL) {
		m_ar->Close();
		delete m_ar;
	}
	if (m_mf != NULL) {
//		::GlobalUnlock(m_data);
		delete m_mf;
	}
	if (m_sf != NULL) {
		m_data = m_sf->Detach();
		delete m_sf;
	}
	if (m_data != NULL) {
		::GlobalFree(m_data);
	}
}

// Creating a CAction
CArchive* CAction::Prepare1() {
	m_sf = new CSharedFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
    m_ar = new CArchive(m_sf, CArchive::store);
	return m_ar;
}

void CAction::Prepare2() {
    m_ar->Close();
    m_data = m_sf->Detach();
	delete m_ar;
	delete m_sf;
	m_ar = NULL;
	m_sf = NULL;
}

// Performing a CAction
CArchive* CAction::Unprepare1() {
	m_mf = new CMemFile((BYTE*)::GlobalLock(m_data), ::GlobalSize(m_data));
	m_ar = new CArchive(m_mf, CArchive::load);
	return m_ar;
}

void CAction::Unprepare2() {
	m_ar->Close();
	::GlobalUnlock(m_data);
	::GlobalFree(m_data);
	m_data = NULL;
	delete m_ar;
	delete m_mf;
	m_ar = NULL;
	m_mf = NULL;
}