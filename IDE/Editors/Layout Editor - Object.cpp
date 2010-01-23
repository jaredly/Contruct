// LayoutEditorObject.cpp : implementation of the CLayoutEditor class
//

// File includes
#include "StdAfx.h"
#include "..\Construct.h"
#include "..\Construct Doc.h"
#include "..\Bars\LayerBar.h"
#include "layoutEditor.h"
#include "..\ChildFrame.h"
#include "..\Dialogs\Insert Object Dlg.h"
#include "..\Dialogs\ArrayPasteDlg.h"
#include "..\MainFrm.h"
#include "..\Utilities\QArray.h"
#include "..\Dialogs\Object Info Dlg.h"
#include "..\Dialogs\New Behavior Dlg.h"
#include "..\Dialogs\NewHLSLDlg.h"

extern CLayerBar* g_pLayerBar;

extern CString gInsert;
extern int gInsertAuto;
CString insert_name;
CString insert_folder;

extern BOOL g_bPaste;
extern BOOL g_bPasteClone;
extern CDragObjects *g_dragInfo;
extern CMainFrame* pMainWnd;
extern PropertiesBar* g_PropertyBar;
extern long g_addToGroup;
extern bool g_SuspendFrameEditor;

bool g_bArrayPaste = false;

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Object Creation //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

long CLayoutEditor::GetUnusedID() 
{
	return GetUnusedOID();
}

float CLayoutEditor::RoundGridX(float value, bool isResizing)
{
	if(inPrecisionMode())
		return value;
	if((layout->m_SnapResize && isResizing) || (layout->m_SnapMovements && !isResizing))
		return floor(value / GridWidth()+0.5)*GridWidth();
	return floor(value+0.5);	
	// note : we can return a float value and everything looks fine, however the ide uses pixels, and 
	// maybe in later versions we'll use decimal place pixels. 
}
float CLayoutEditor::RoundGridY(float value, bool isResizing)
{
	if(inPrecisionMode())
		return value;
	if((layout->m_SnapResize && isResizing) || (layout->m_SnapMovements && !isResizing))
		return floor(value / GridHeight()+0.5)*GridHeight();
	return floor(value+0.5);
	
}

long CLayoutEditor::GetUnusedOID() 
{
	long oID = application->m_objID;
	application->m_objID = oID + 1;
	return oID;
}

CString CLayoutEditor::GetUnusedName(CString name)
{
	return application->GetUniqueObjectTypeName(name);
}

CObj* CLayoutEditor::CreateNewClonedObject(const char* objectFile, CPoint point, CObj *fObj)
{
	HICON prvCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	CObjType* pOldType = fObj->GetObjectType(application);

	/* Get OINFO structure */

	OINFO *oInfo;
	POSITION pos = g_dlls->GetStartPosition();
	DWORD nKey;
	while (pos != NULL) { // locate the object DLL to use
		g_dlls->GetNextAssoc(pos, nKey, oInfo);
		if (oInfo->oID == fObj->GetDLLIdentifier())
			break;
	}
	if (oInfo->oID != fObj->GetDLLIdentifier())
		return NULL;

	/* initialize object and object type */

	long oID = GetUnusedOID();
	long ID = GetUnusedID();
	CString name = GetUnusedName(fObj->m_tempName);

	CObjType *oT = new CObjType;
	oT->ObjectIdentifier = oID;
	oT->DLLIndex = fObj->GetDLLIdentifier();

	// Load and add bitmaps
	oT->m_Image.Copy(pOldType->m_Image);
	oT->small_image.Copy(pOldType->small_image);

	// Copy ID.. important
	oT->DLLIndex = pOldType->DLLIndex;

	oT->SetName(name);
	application->object_types[oID] = oT;

	// we now how two types to copy stuff between
	oT->behaviors = pOldType->behaviors;
	oT->effects = pOldType->effects;
	oT->traits = pOldType->traits;
	oT->category = pOldType->category;
	oT->destroyWhen = pOldType->destroyWhen;
	oT->families = pOldType->families;
	oT->m_bGlobal = pOldType->m_bGlobal;
	oT->m_bShowInEventSheetEditor = pOldType->m_bShowInEventSheetEditor;
	oT->m_PrivateValues = pOldType->m_PrivateValues;
	oT->VariableID = pOldType->VariableID;

	CObj *o = new CObj;
	// editObject now points to member variable
	//o->editObject = new editInfo();

	// Set size etc from the old object
	o->SetSize(fObj->GetW(), fObj->GetH());
	o->editObject->objectAngle = fObj->editObject->objectAngle;
	o->editObject->originX = fObj->editObject->originX;
	o->editObject->originY = fObj->editObject->originY;

	// Create
	oInfo->ETCreateObject(o->editObject, layout);

	// Copy edit data over
	bin DataTransfer(g_allocator);
	DataTransfer.loading = false;
	oInfo->ETSerialize(fObj->editObject, DataTransfer);
	bin Loader(g_allocator);
	Loader = DataTransfer;
	Loader.loading = true;
	oInfo->ETSerialize(o->editObject, Loader);

	// These come after we copy data
	o->SetPos(point.x, point.y, FALSE);
	o->editObject->ObjectIdentifier = oID; // finalize the name and IDs
	o->editObject->instanceID = ID;

	o->SetDLLIdentifier(oInfo->oID);

	o->m_values = fObj->m_values;





	if (oInfo->ideFlags & OF_NOSIZE)
		o->m_AllowSize = false;

	if (oInfo->ideFlags & OF_NODRAW)
		o->m_Draw = false;
	else
		o->m_Draw = true;

	if (oInfo->ideFlags & OF_ALLOWANGLES)
		o->m_ShowAngles = true;



	layout->objects[ID] = o;
	layout->current_layer->m_zOrder.AddTail(ID);

	if (!oInfo->ETInitializeObject(o->editObject, layout))
	{
		delete o;
		delete oT;
		SetCursor(prvCursor);
		return NULL;
	}

	layout->SetChanged(true);

	return o;
}

CLayer* CLayoutEditor::GetObjectLayer(long object)
{
	CLayer* lyr;
	CObj* o = GetObject(object); // if object is in a group, get the group
	if (o) {
		while (o->m_bInGroup) {
			o = o->m_group;
		}
	}
	else
		return NULL;

	object = o->GetInstanceID();

	POSITION pos2;
	pos2 = layout->layers.GetTailPosition();

	for (int j=0; j < layout->layers.GetCount(); j++) 
	{
		lyr = layout->layers.GetPrev(pos2);

		if (lyr->m_zOrder.Find(object))
			return lyr;
	}

	return NULL;
}

CObj* CLayoutEditor::CreateClonedObject(const char* objectFile, CPoint point, CObj *fObj, long fOID)
{
	layout->m_LayoutEditor = this;


	HICON prvCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	CString name;

	CObj *o = NULL;
	CObjType *oT = NULL;

	o = fObj;
	name = o->m_tempName;


	/* Get OINFO structure */
	OINFO *oInfo = 0;
	POSITION pos = g_dlls->GetStartPosition();

	if (o->GetDLLIdentifier() == m_PriorClonedObject && m_PriorClonedObject!=0 )
	{
		oInfo = m_PriorClonedOINFO;
	}

	else if(!o->m_bIsGroup)
	{
		//while (pos != NULL) 
		{ 
			// locate the object DLL to use
			DWORD id = o->GetDLLIdentifier();
			g_dlls->Lookup(id,oInfo);
			//g_dlls->GetNextAssoc(pos, nKey, oInfo);
			//if (oInfo->oID == o->GetIdentifier())
			{
				m_PriorClonedObject = id;
				m_PriorClonedOINFO = oInfo;

			//	break;
			}
		}
	}

	/* initialize object and object type */

	BOOL bCallFunction = (fOID == -1);

	long oID = (fOID == -1) ? GetUnusedOID() : fOID;
	long ID = o->GetInstanceID();
	if(layout->objects.Lookup(ID, fObj))
	{
		ID = o->GetInstanceID();
		if(fObj != o)
			ID = GetUnusedID();
	}
	else
	{
		ID = o->GetInstanceID();
	}




	o->SetPos(point.x, point.y, FALSE);
	o->editObject->ObjectIdentifier = oID; // finalize the name and IDs
	o->editObject->instanceID = ID;

	if (!GetObjectLayer(ID))
		if(!o->m_bInGroup)
			layout->current_layer->m_zOrder.AddTail(ID);

	if(oInfo)
	{

		if (oInfo->ideFlags & OF_NOSIZE)
			o->m_AllowSize = false;

		if (oInfo->ideFlags & OF_NODRAW)
			o->m_Draw = false;
		else
			o->m_Draw = true;

		if (oInfo->ideFlags & OF_ALLOWANGLES)
			o->m_ShowAngles = true;
		if (!oInfo->ETInitializeObject(o->editObject, layout))
		{
			SetCursor(prvCursor);
			return NULL;
		}

	}

	if (!application->object_types.Lookup(oID, oT)) 
	{
		// As of 12 Feb 2009, this code is no longer needed
		// As the CDragDrop class itself creates CObjTypes based
		// on what was serialised
		/*
		CObjType *oT = new CObjType;
		oT->SetName(name);
		oT->ObjectIdentifier = oID;
		oT->DLLIndex = o->GetIdentifier();

		// ICON TODO: Somehow maintain custom icons (or generated icons)
		HBITMAP LargeIcon  = oInfo->hLargeIcon;
		HBITMAP SmallIcon = oInfo->hSmallIcon;

		oT->m_Image.CreateFromHBITMAP(LargeIcon);
		oT->small_image.CreateFromHBITMAP(SmallIcon);




		application->object_types[oID] = oT;*/
	}

	layout->objects[ID] = o;

	layout->SetChanged(true);

	return o;
}

CObj* CLayoutEditor::CreateObject(const char* objectFile, CPoint point, CObj *fObj, long fOID, BOOL bNewType)
{
	bool bInitNow = false;
	CObjType *oT = NULL;
	OINFO *oInfo;

	point -= CPoint(FRAME_MARGIN_W*m_Zoom, FRAME_MARGIN_H*m_Zoom); // offset for frame border
	point += GetScrollPosition(); // offset for scroll position
	point.x = RoundGridX(point.x / m_Zoom);
	point.y = RoundGridY(point.y / m_Zoom);
	m_bInsert = FALSE;		
	HICON prvCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	// Finished object
	CObj *o = NULL;

	if (bNewType) {
		o = CreateNewClonedObject(objectFile, point, fObj);
		if (o == NULL)
			return NULL;
	}
	else if (fObj != NULL) {
		o = CreateClonedObject(objectFile, point, fObj, fOID);
		if (o == NULL)
			return NULL;
	}

	else {
		DWORD identifier = 0xFFFFFFFF; // set a fake object ID
		CString name = "";

		if (strcmp(objectFile, m_PriorObjectFile) == 0) {
			oInfo = m_PriorOINFO;
			name = oInfo->extName;
		}

		else
		{
			POSITION pos = g_dlls->GetStartPosition();
			DWORD nKey;
			while (pos != NULL)
			{ 
				// locate the object DLL to use
				g_dlls->GetNextAssoc(pos, nKey, oInfo);

				CString extFileName = oInfo->extFileName;
				CString strObjectFile = objectFile;

				extFileName.MakeLower();
				strObjectFile.MakeLower();

				if (extFileName == strObjectFile) 
				{
					name = oInfo->extName;

					m_PriorObjectFile = objectFile;
					m_PriorOINFO      = oInfo;

					break;
				}
			}
		}

		identifier = oInfo->oID;

		o = new CObj();

		long oID = GetUnusedOID();
		long ID = GetUnusedID();

		CString to_name;

		if (insert_name == "")
			to_name = GetUnusedName(name);
		else
		{
			to_name = GetUnusedName(insert_name);
			insert_name = "";
		}

		// Load bitmaps
		o->SetPos(point.x, point.y, FALSE);
		o->editObject->ObjectIdentifier = oID; // finalize the name and IDs
		o->editObject->instanceID = ID;
			
		if (oInfo->ideFlags & OF_NOSIZE)
			o->m_AllowSize = false;

		if (oInfo->ideFlags & OF_NODRAW)
			o->m_Draw = false;
		else
			o->m_Draw = true;

		if (oInfo->ideFlags & OF_ALLOWANGLES)
			o->m_ShowAngles = true;

		oT = new CObjType;
		oT->SetName(to_name);
		oT->ObjectIdentifier = oID;
		oT->DLLIndex = identifier;

		if (insert_folder != "")
		{
			oT->SetFolder(insert_folder);
			insert_folder = "";
		}

		// Set bitmaps
		oT->m_Image.CreateFromHBITMAP(oInfo->hLargeIcon);
		oT->small_image.CreateFromHBITMAP(oInfo->hSmallIcon);

		o->SetSize(32, 32);

		oInfo->ETCreateObject(o->editObject, layout);

		o->SetDLLIdentifier(identifier);
		o->editObject->objectAngle = 0;

		layout->objects[ID] = o;
		application->object_types[oID] = oT;

		bInitNow = true;

		if (bInitNow)
		{
			if (!CreateAndInitialize(oInfo, o->editObject, layout))
			{
				delete o;
				delete oT;
				SetCursor(prvCursor);
				return NULL;
			}
		}

		// At end, insert
		m_sel.RemoveAll();

			m_sel.AddTail(ID);

		int iRoot = -1;
		if (oInfo->ETGetAnimationHandle) {
			oInfo->ETGetAnimationHandle(o->editObject, iRoot);
			CObjType* pType = o->GetObjectType(application);

			CMainFrame* pMainFrame = ((CMainFrame*)(CLayoutEditor::GetParentOwner()));
			pMainFrame->animator.UpdateAnimations(application, layout, pType, iRoot);
		}

		if (oInfo->ideFlags & OF_NODRAW)
			layout->layers.GetHead()->m_zOrder.AddTail(ID);
		else
			if(!o->m_bInGroup)
				layout->current_layer->m_zOrder.AddTail(ID);
	}

	// START UNDO INFO //
	long objID = o->GetInstanceID();
	CAction *action = new CAction();
	action->m_type = ACTION_DELETE;
	action->m_bGroup = !m_AutoIncrement; // array paste
	CArchive *ar = action->Prepare1();
	*ar << objID;
	action->Prepare2();
	m_undo->AddAction(action);
	// END UNDO INFO //

	if (!g_bArrayPaste)
	{
		m_bUpdateFrame = TRUE;
	}

	SetCursor(prvCursor);

	// update project bar
	pMainWnd->project_bar.UpdateApplication(application);

	UpdateSelection();

	layout->SetChanged(true);

	return o;
}
	
BOOL CLayoutEditor::CreateAndInitialize(OINFO *oInfo, editInfo *editObject, CLayout *layout)
{
	oInfo->ETPutObject(editObject);

	if (!oInfo->ETInitializeObject(editObject, layout))
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
//////////////////////////// Object Manipulation ////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CLayoutEditor::CheckSizingHandles(CPoint point)
{
	CObj* o;
	POSITION pos = m_sel.GetHeadPosition();
	int sizeType = 0;
	
	if (m_sel.GetCount() == 1)
	for (int i=0; i < m_sel.GetCount(); i++) {
		long object = m_sel.GetNext(pos);
		if (layout->objects.Lookup(object, o) && o->m_AllowSize) {
			BOOL thisObj = FALSE;
			LPCTSTR sizeCur = IDC_ARROW;
			CObjectRectangle ObjRect = o->GetObjectRect(this);

		//	point.x += (m_Zoom*FRAME_MARGIN_W - scrollX);
		//	point.y += (m_Zoom*FRAME_MARGIN_H - scrollY);


				// Top left
				{
					CPoint pt;
					pt = ObjRect.GetPoint(0,0,8);
					//pt.x *= m_Zoom;
					//pt.y *= m_Zoom;
					CRect sz(pt,pt);
					sz.InflateRect(3,3);
					if (sz.PtInRect(point))
					{
						sizeType = 1;
						sizeCur = IDC_SIZENWSE;
						thisObj = TRUE;
					}
				}

				// Top middle
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(0.5,0,8);
					//pt.x *= m_Zoom;
					//pt.y *= m_Zoom;
					CRect sz(pt,pt);
					sz.InflateRect(2,2);
					if (sz.PtInRect(point))
					{
						sizeType = 2;
						sizeCur = IDC_SIZENS;
						thisObj = TRUE;
					}
				}

				// Top right
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(1,0,8);
					//pt.x *= m_Zoom;
					//pt.y *= m_Zoom;
					CRect sz(pt,pt);
					sz.InflateRect(3,3);
					if (sz.PtInRect(point))
					{
						sizeType = 3;
						sizeCur = IDC_SIZENESW;
						thisObj = TRUE;
					}
				}

				// Middle left
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(0,0.5,8);
					//pt.x *= m_Zoom;
					//pt.y *= m_Zoom;
					CRect sz(pt,pt);
					sz.InflateRect(2,2);
					if (sz.PtInRect(point))
					{
						sizeType = 4;
						sizeCur = IDC_SIZEWE;
						thisObj = TRUE;
					}
				}

				// Middle right
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(1,0.5,8);
					//pt.x *= m_Zoom;
					//pt.y *= m_Zoom;
					CRect sz(pt,pt);
					sz.InflateRect(2,2);
					if (sz.PtInRect(point))
					{
						sizeType = 6;
						sizeCur = IDC_SIZEWE;
						thisObj = TRUE;
					}
				}

				// Bottom left
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(0,1,8);
					//pt.x *= m_Zoom;
					//pt.y *= m_Zoom;
					CRect sz(pt,pt);
					sz.InflateRect(3,3);
					if (sz.PtInRect(point))
					{
						sizeType = 7;
						sizeCur = IDC_SIZENESW;
						thisObj = TRUE;
					}
				}
				// Bottom middle
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(0.5,1,8);
					//pt.x *= m_Zoom;
					//pt.y *= m_Zoom;
					CRect sz(pt,pt);
					sz.InflateRect(2,2);
					if (sz.PtInRect(point))
					{
						sizeType = 8;
						sizeCur = IDC_SIZENS;
						thisObj = TRUE;
					}
				}

				// Bottom right
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(1,1,8);
					//pt.x *= m_Zoom;
					//pt.y *= m_Zoom;
					CRect sz(pt,pt);
					sz.InflateRect(3,3);
					if (sz.PtInRect(point))
					{
						sizeType = 9;
						sizeCur = IDC_SIZENWSE;
						thisObj = TRUE;
					}
				}

				int distance = 20;
				CPoint pt2;

				float sin_a, cos_a;
				cr::sincosf(Rad(ObjRect.a), &sin_a, &cos_a);

				pt2.x = ObjRect.x + cos_a*distance;
				pt2.y = ObjRect.y + sin_a*distance;

				CRect sz(pt2,pt2);
				sz.InflateRect(3,3,4,4);
				if (sz.PtInRect(point) && o->m_ShowAngles)
				{
					sizeType = 10;
					sizeCur = IDC_CROSS;
					thisObj = TRUE;
				}

			if (sizeType > 0 && thisObj) {
				m_objToSize = object;
				m_sizeType = sizeType;
				m_sizeCur = sizeCur;
			}
			else {
				if (sizeType == 0) {
					m_sizeType = sizeType;
					m_sizeCur = sizeCur;
				}
			}
			
			SetCursor(LoadCursor(NULL, m_sizeCur));
		}
	}
}

void CLayoutEditor::SelectItemsInBox(UINT nFlags)
{
	CObj *o;

	if(InputLocked())
		return;

	if (!(nFlags & MK_SHIFT)) 
		m_sel.RemoveAll();

	CRect rcSel(min(m_oldPt.x, m_curPt.x), min(m_oldPt.y, m_curPt.y),
				max(m_oldPt.x, m_curPt.x), max(m_oldPt.y, m_curPt.y));

	CObjMap* objMap = &layout->objects;
	POSITION pos = objMap->GetStartPosition();
	long nKey;

	while (pos != NULL)
	{ 
		// iterate through objects to choose one to select
		objMap->GetNextAssoc(pos, nKey, o);

		// If visible, not locked and drawing and not in a closed group
		CLayer* layer = GetObjectLayer(o->GetInstanceID());
		if (layer && ISLAYERVISIBLE(layer->m_state) && !ISLAYERLOCKED(layer->m_state) &&
			o->GetVisible() && !o->GetLocked() /*&& o->m_Draw*/ && (!o->m_bInGroup || (o->m_bInGroup && o->m_group->m_bGroupOpen)))
		{
			CRect rc = o->GetObjectRect(this,false).GetBoundingRect();
			if (PtInRect(rcSel, rc.TopLeft()) && PtInRect(rcSel, rc.BottomRight())) 
			{
				long instanceID = o->GetInstanceID();
				if (!m_sel.Find(instanceID)) 
					m_sel.AddTail(instanceID);
			}
		}
	}
	
	Invalidate();
	RedrawWindow();

	if (m_sel.IsEmpty())
		g_PropertyBar->Update(this, TYPE_LAYOUT, NULL, layout, NULL, application); // nothing is selected, show frame props
	else
		g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props

	UpdateSelection();
}

void CLayoutEditor::ResizeObject()
{
	CObj *o;
	if (layout->objects.Lookup(m_sel.GetTail(), o)) {

		
		CObjectRectangle rc = GetTempChangedRect(o);

		// START UNDO INFO //
		CAction *action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		CArchive *ar = action->Prepare1();
		long ID = o->GetInstanceID();
		*ar << ID;
		o->Serialize(*ar);
		action->Prepare2();
		m_undo->AddAction(action);
		// END UNDO INFO //

		o->SetPos(
			RoundGridX(rc.x/m_Zoom),
			RoundGridY(rc.y/m_Zoom));
		o->SetSize(
			RoundGridX(rc.w/m_Zoom, true),
			RoundGridY(rc.h/m_Zoom, true));
		o->SetAngle(rc.a);

		// NOTE: it is assumed the angle and hotspots wont change...but if they do they too will need to be set.

		if (o->m_bInGroup)
			o->UpdateGroupInfo();


		// Get oInfo pointer
		OINFO *oInfo;
		DWORD oid = o->GetDLLIdentifier();

		g_dlls->Lookup(oid, oInfo);
		
		if(oInfo->ETOnNotify)
			oInfo->ETOnNotify((o)->editObject, 4);

		SetCursor(LoadCursor(NULL, m_sizeCur));
		g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props
		m_bUpdateFrame = TRUE;
		m_sizeType = 0;
	}

	layout->SetChanged(true);
}

void CLayoutEditor::InitializeSelect(CPoint point)
{
	CRect rc(m_oldPt, CSize(1,1));
	rc.InflateRect(1,1);
	if (!rc.PtInRect(point)) {
		m_bDraggingRect = TRUE;

		RedrawWindow();
		SetCapture();
	}
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////// More Object Manipulation //////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CLayoutEditor::DeleteSelection(bool omit_prompt)
{
	CMainFrame* pMainFrame = ((CMainFrame*)(CLayoutEditor::GetParentOwner()));

	bool do_delete = false;

	CExtMsgBox msg(NULL, CONF_DELETE, CONFIRMATION, __EXT_MB_YESNO | __EXT_MB_ICONINFORMATION | __EXT_MB_DO_NOT_ASK_AGAIN, 0, "DeleteObject", __EXT_MB_EX_CHECK_BOX_IS_NOT_CHECKED_INTIALLY);
	int result = 0;

	if (!omit_prompt)
		result = msg.DoModal();

	if (result == IDYES)
		do_delete = true;

	else
	{
		msg.ResetMsgBox();
		return;
	}

	// START UNDO INFO //
	CAction *action = new CAction();
	action->m_type = ACTION_CHANGEZORDER;
	action->m_bGroup = FALSE;
	CArchive *ar = action->Prepare1();
	this->SerializeAllZLists(ar);
	action->Prepare2();
	m_undo->AddAction(action);
	// END UNDO INFO //

	POSITION pos = m_sel.GetHeadPosition();

	for (int i = 0; i < m_sel.GetCount(); i++)
		DeleteObject(m_sel.GetNext(pos), (i > 0));

	m_sel.RemoveAll();

	layout->SetChanged(true);

	CChildFrame* pCF = (CChildFrame*)GetParentFrame();
	pCF->object_bar.Refresh();

	g_PropertyBar->Update(this, TYPE_LAYOUT, NULL, layout, NULL); 
}

void CLayoutEditor::DeleteObjectRecursive(long object)
{
	CObj *o, *o2;

	if (layout->objects.Lookup(object, o)) 
	{
		long ObjectIdentifier = o->GetGlobalID();
		POSITION pos;

		long prevObj = -1;
		long objLayer = -1;

		// remove from zorder
		CLayer *lyr = GetObjectLayer(object);
		if(lyr)
		{
			POSITION pos2 = lyr->m_zOrder.Find(object);
			 prevObj = lyr->m_zOrder.GetPrev(pos2);
			 objLayer = lyr->m_layerID;
			lyr->m_zOrder.RemoveAt(lyr->m_zOrder.Find(object));
		}
		else if(o->m_bInGroup)
		{
			POSITION pos2 = o->m_group->m_groupObjects.Find(object);
			prevObj = o->m_group->m_groupObjects.GetPrev(pos2);
			objLayer = 0 - o->m_group->GetInstanceID(); // we use negative values of the instance id of objects
			o->m_group->m_groupObjects.RemoveAt(o->m_group->m_groupObjects.Find(object));
		}
		BOOL bMoreInstances = FALSE; // check if any more instances exist
		
		long nKey;

		pos = application->layouts.GetHeadPosition();

		while(pos)
		{
			CLayout* layout = application->layouts.GetNext(pos);
			POSITION pos2 = layout->objects.GetStartPosition();
			while (pos != NULL) 
			{
				layout->objects.GetNextAssoc(pos2, nKey, o2);

				if (o2->GetGlobalID() == ObjectIdentifier && o2->GetInstanceID() != object) 
				{
					bMoreInstances = TRUE;
					break;
				}
			}
		}

		if (bMoreInstances == FALSE) // no more exist, delete object type
		{

			CObjType* oT = application->object_types[ObjectIdentifier];

			// If in a container, remove my reference
			if (oT->m_pContainer) {
				vector<int>& ids = oT->m_pContainer->m_IDs;
				
				vector<int>::iterator i = find(ids.begin(), ids.end(), oT->ObjectIdentifier);

				if (i != ids.end())
					ids.erase(i);

				if (ids.size() == 1)
					application->DeleteContainer(oT->m_pContainer);
			}

			// If last in any families delete those families
			vector<Family*>::iterator f = oT->families.begin();
			vector<Family*>::iterator end = oT->families.end();

			for ( ; f != end; f++) {
				if (application->PromptIfTypeLastInFamily(oT, *f, false) == CApplication::is_last_type_in_family)
					application->DeleteFamily(*f);
			}
		}
		
		pos = o->m_groupObjects.GetHeadPosition();
		for (int k = 0; k < o->m_groupObjects.GetCount(); k++) 
		{
			long groupitemid = o->m_groupObjects.GetNext(pos);
			DeleteObjectRecursive(groupitemid);

			// basically the serialization function automatically saves all the children items. We want to 
			// delete them but not back them up
		}		

		delete o;
		layout->objects.RemoveKey(object);
	}
	

	m_bUpdateFrame = TRUE;

	layout->SetChanged(true);
}

void CLayoutEditor::DeleteObject(long object, BOOL bGroupUndo)
{
	CObj *o, *o2;
	CAction *action;

	if (layout->objects.Lookup(object, o)) 
	{
		long ObjectIdentifier = o->GetGlobalID();
		POSITION pos;

		long prevObj = -1;
		long objLayer = -1;

		// remove from zorder
		CLayer *lyr = GetObjectLayer(object);
		if(lyr)
		{
			POSITION pos2 = lyr->m_zOrder.Find(object);
			 prevObj = lyr->m_zOrder.GetPrev(pos2);
			 objLayer = lyr->m_layerID;
			lyr->m_zOrder.RemoveAt(lyr->m_zOrder.Find(object));
		}
		else if(o->m_bInGroup)
		{
			POSITION pos2 = o->m_group->m_groupObjects.Find(object);
			 prevObj = o->m_group->m_groupObjects.GetPrev(pos2);
			 objLayer = 0 - o->m_group->GetInstanceID(); // we use negative values of the instance id of objects
			 o->m_group->m_groupObjects.RemoveAt(o->m_group->m_groupObjects.Find(object));
		}
		BOOL bMoreInstances = FALSE; // check if any more instances exist in any of the layouts
		
		POSITION pos3 = application->layouts.GetHeadPosition();
		while(pos3 != NULL)
		{
			CLayout* pLayout = application->layouts.GetNext(pos3);
			pos = pLayout->objects.GetStartPosition();
			long nKey;
			while (pos != NULL) 
			{
				pLayout->objects.GetNextAssoc(pos, nKey, o2);

				if (o2->GetGlobalID() == ObjectIdentifier && (pLayout != layout || o2->GetInstanceID() != object)) 
				{
					bMoreInstances = TRUE;
					break;
				}
			}
		}

		if (bMoreInstances == FALSE) // no more exist, delete object type
		{
			CObjType* oT = application->object_types[ObjectIdentifier];

			// If in a container, remove my reference
			if (oT->m_pContainer) {
				vector<int>& ids = oT->m_pContainer->m_IDs;
				
				vector<int>::iterator i = find(ids.begin(), ids.end(), oT->ObjectIdentifier);

				if (i != ids.end())
					ids.erase(i);

				if (ids.size() == 1)
					application->DeleteContainer(oT->m_pContainer);
			}

			// If last in any families delete those families
			vector<Family*>::iterator f = oT->families.begin();
			vector<Family*>::iterator end = oT->families.end();

			for ( ; f != end; f++) {
				if (application->PromptIfTypeLastInFamily(oT, *f, false) == CApplication::is_last_type_in_family)
					application->DeleteFamily(*f);
			}

			if(m_undo)
			{
				/*
				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_CREATETYPE;
				if (bGroupUndo)
					action->m_bGroup = FALSE;
				CArchive *ar = action->Prepare1();

				*ar << ObjectIdentifier; //object type
				oT->Serialize(*ar);

				long ID = o->GetInstanceID(); //object
				*ar << ID << objLayer << prevObj;
				o->Serialize(*ar);

				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //*/

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_CREATE;
				if (bGroupUndo)
					action->m_bGroup = FALSE;
				CArchive *ar = action->Prepare1();

				long ID = o->GetInstanceID();
				*ar << ID << objLayer << prevObj;
				o->Serialize(*ar);

				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				//delete oT;
				application->m_DeletedObjectTypes[ObjectIdentifier] = oT;
				application->object_types.RemoveKey(ObjectIdentifier);

				if (oT->tree)
				{
					pMainWnd->project_bar.tree.DeleteItem(oT->tree);
					oT->tree = NULL;
				}
			}
		}
		else
		{
			if(m_undo)
			{
			// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_CREATE;
				if (bGroupUndo)
					action->m_bGroup = FALSE;
				CArchive *ar = action->Prepare1();

				long ID = o->GetInstanceID();
				*ar << ID << objLayer << prevObj;
				o->Serialize(*ar);

				action->Prepare2();
				m_undo->AddAction(action);
			// END UNDO INFO //
			}
		}
		
		pos = o->m_groupObjects.GetHeadPosition();
		for (int k = 0; k < o->m_groupObjects.GetCount(); k++) 
		{
			long groupitemid = o->m_groupObjects.GetNext(pos);
			DeleteObjectRecursive(groupitemid);

			// basically the serialization function automatically saves all the children items. We want to 
			// delete them but not back them up...thats where the DeleteOBjectRecursive function is handy :)
		}		

		delete o;
		layout->objects.RemoveKey(object);
	}
	

	m_bUpdateFrame = TRUE;

	pMainWnd->ObjectTypesChanged();

	layout->SetChanged(true);
}

void CLayoutEditor::InitializeObject(CObj* o)
{
	// We need to call initialize!
	if(!o->m_bIsGroup)
	{
		OINFO *oInfo;
		DWORD key = o->GetDLLIdentifier();
		g_dlls->Lookup(key, oInfo);
	
		oInfo->ETInitializeObject(o->editObject, layout);
	}

	POSITION pos = o->m_groupObjects.GetHeadPosition();
	CObj *o2;
	for (int k = 0; k < o->m_groupObjects.GetCount(); k++) {
		long groupitemid = o->m_groupObjects.GetNext(pos);
		o2 = GetObject(groupitemid);
					
		InitializeObject(o2);
	}
}

CRect CLayoutEditor::GetSelRect()
{
	CRect rcSel;
	BOOL bSetRect = FALSE;
	CObj *o;
	POSITION pos = m_sel.GetHeadPosition();
	int i;
	for (i = 0; i < m_sel.GetCount(); i++) 
	{

		if(o = GetObject(m_sel.GetNext(pos)))
		{
			if (!bSetRect) 
			{
				rcSel = CRect(o->GetX(), o->GetY(), o->GetX() + o->GetW(), o->GetY() + o->GetH());
				bSetRect = TRUE;
			}

			else 
			{
				rcSel.left = min(rcSel.left, o->GetX());
				rcSel.top = min(rcSel.top, o->GetY());
				rcSel.right = min(rcSel.right, o->GetX() + o->GetW());
				rcSel.bottom = min(rcSel.bottom, o->GetY() + o->GetH());
			}
		}
	}
	return rcSel;
}
extern CLayoutEditor* g_pLayoutView;
void CLayoutEditor::Paste(CPoint point, BOOL bClone)
{
	DROPEFFECT drop = DROPEFFECT_NONE;
	CPoint p = point;
	g_pLayoutView = this;
	g_pFrame = layout;
	g_pSel = &m_sel;
	g_pApp = application;
	g_pPt = &p;
	g_pDrop = &drop;
	g_bDuplicate = FALSE;

	CDragObjects *dragInfo = new CDragObjects;
	if (bClone) {
		dragInfo->clone = true;
	}
	if (!ddm.DoDrop(dragInfo, NULL, DRAGDROP_OBJECTS)) 
	{
		CErrorDlg error;
		error.Error("Drag/drop error", "An error occured whilst dropping objects.");
	}

	delete dragInfo;
	m_bUpdateFrame = TRUE;

	if (bClone) {
		CObj *o;
	
		POSITION pos = m_sel.GetHeadPosition();
		int c = m_sel.GetCount();
		for (int i=0; i < c; i++) {
			o = GetObject(m_sel.GetNext(pos));

			CObjType* pOldType = o->GetObjectType(application);
			OINFO* pInfo = GetOINFO(pOldType->DLLIndex);

			CPoint point( o->GetX(), o->GetY());
			point.x *= m_Zoom;
			point.y *= m_Zoom;

			point += CPoint(FRAME_MARGIN_W*m_Zoom, FRAME_MARGIN_H*m_Zoom); // offset for frame border
			point -= GetScrollPosition(); // offset for scroll position

	
			CObj * new_o = CreateObject(pInfo->extFileName, point, o, o->GetInstanceID(), TRUE);
			DeleteObject(o->GetInstanceID());
			o = new_o;

			// Moved into the CreateNewClonedObject function

		}

		m_sel.RemoveAll();
	}

	layout->SetChanged(true);
}

void CLayoutEditor::RotateSelection(long amt)
{
	CObj *o;
	long id;
	CAction *action;
	CArchive *ar;
	if (m_sel.GetCount() == 1) {
		o = GetObject(m_sel.GetHead());

		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		ar = action->Prepare1();
		id = o->GetInstanceID();
		*ar << id;
		o->Serialize(*ar);
		action->Prepare2();
		m_undo->AddAction(action);
		// END UNDO INFO //

		o->editObject->objectAngle = o->editObject->objectAngle + amt;
	}
	else if (m_sel.GetCount() > 1) {
		//
	}

	layout->SetChanged(true);
}

void CLayoutEditor::ArrayPaste(CArrayPasteDlg *apDlg)
{
	g_pApp = application;

	layout->m_LayoutEditor = this;

	m_bSuspendDrawing = TRUE;
	m_bCachedID = TRUE;
	m_cachedID = 0;

	/* GET RECT OF CURRENT SELECTION */

	CRect rcSel = GetSelRect();

	/* COPY CURRENT SELECTION */

	CPoint initPt = rcSel.TopLeft();
	DROPEFFECT drop = DROPEFFECT_NONE;
	CDragObjects *dragInfo = NULL;
	g_pLayoutView = g_dragInfo->layout_editor = this;
	g_pFrame = g_dragInfo->layout = layout;
	g_pSel = g_dragInfo->m_pSel = &m_sel;
	g_pApp = g_dragInfo->application = application;
	g_dragInfo->m_curPt = initPt;
	ddm.PrepareDrop(DO_CLIPBOARD, DRAGDROP_OBJECTS, g_dragInfo, &drop);

	/* PREPARE INFORMATION */

	initPt += CPoint(FRAME_MARGIN_W*m_Zoom, FRAME_MARGIN_H*m_Zoom); // reverse offset for frame border
	initPt -= GetScrollPosition(); // reverse offset for scroll position
	initPt.x /= m_Zoom;
	initPt.y /= m_Zoom;
	
	CPoint curPt = initPt;
	CPoint rowPt = curPt;

	dragInfo = new CDragObjects;
	g_pLayoutView = this;
	g_pFrame = layout;
	g_pSel = &m_sel;
	g_pApp = application;
	g_pPt = &curPt;
	g_pDrop = &drop;
	g_bDuplicate = apDlg->bDuplicate;

	if (!apDlg->b2D)
		apDlg->count2 = 1;

	float spaceX = apDlg->bMoveTotal ? ((float)apDlg->moveX) / ((float)apDlg->count1 - 1) : apDlg->moveX;
	float spaceY = apDlg->bMoveTotal ? ((float)apDlg->moveY) / ((float)apDlg->count1 - 1) : apDlg->moveY;
	float spaceX2 = apDlg->bMoveTotal2 ? ((float)apDlg->moveX2) / ((float)apDlg->count2 - 1) : apDlg->moveX2;
	float spaceY2 = apDlg->bMoveTotal2 ? ((float)apDlg->moveY2) / ((float)apDlg->count2 - 1) : apDlg->moveY2;
	float sizeX = apDlg->bScaleTotal ? ((float)apDlg->scaleX) / ((float)apDlg->count1 - 1) : apDlg->scaleX;
	float sizeY = apDlg->bScaleTotal ? ((float)apDlg->scaleY) / ((float)apDlg->count1 - 1) : apDlg->scaleY;
	float rotate = apDlg->bRotateTotal ? ((float)apDlg->rotateX) / ((float)apDlg->count1 - 1) : apDlg->rotateX;

	m_LoopIteration = 1;

	g_bArrayPaste = true;

LARGE_INTEGER Frequency;

QueryPerformanceFrequency(&Frequency);
  LARGE_INTEGER liStart;
  LARGE_INTEGER liStop;

  // Start time measurement
  ::QueryPerformanceCounter(&liStart);

	// Create copies
	for (int j = 0; j < apDlg->count2; j++) 
	{
		rowPt = CPoint(initPt.x + (long)(spaceX2*j), initPt.y + (long)(spaceY2*j));
		for (int i = 0; i < apDlg->count1; i++) 
		{
			curPt = CPoint(rowPt.x + (long)(spaceX*i), rowPt.y + (long)(spaceY*i));
			if (i != 0 || j != 0) 
			{
				if (!ddm.DoDrop(dragInfo, NULL, DRAGDROP_OBJECTS)) 
				{
					AfxMessageBox("Drop failed");
				}
				m_AutoIncrement = true;

				RotateSelection((long)(rotate*i));

				//ScaleSelection((long)(sizeX*i), (long)(sizeY*i));
			}
		}
	}

  // Stop time measurement
  QueryPerformanceCounter(&liStop);

  LONGLONG llTimeDiff = liStop.QuadPart - liStart.QuadPart;

  // To get duration in milliseconds
  double dftDuration = (double) llTimeDiff * 1000.0 / (double) Frequency.QuadPart;

	g_bArrayPaste = false;

	// No longer auto increment, and now refresh layer bar
	m_AutoIncrement = false;
	CChildFrame* pCF = (CChildFrame*)GetParentFrame();
	pCF->object_bar.Refresh();

	m_bSuspendDrawing = FALSE;
	m_bCachedID = FALSE;
	g_bDuplicate = FALSE;
	delete dragInfo;
	m_bUpdateFrame = TRUE;

	layout->SetChanged(true);
}

void CLayoutEditor::OnFrameAction(UINT idCommand)
{
	CArrayPasteDlg apDlg;
	CObj *o, *o2;
	POSITION pos = m_sel.GetHeadPosition();
	POSITION pos3;
	POSITION pos4;
	long value = 0, value2 = 0, value3 = 0, value4 = 0;
	CRect rc;
	BOOL bValueSet = FALSE;
	int i, j, c;
	POSITION pos2 = layout->objects.GetStartPosition();
	long nKey;
	long id;
	CQArray <twoint, twoint&> twointArr;
	CQArray <intstr, intstr&> intstrArr;
	DROPEFFECT drop = DROPEFFECT_NONE;
	CDragObjects *dragInfo = NULL;
	CPoint p(0,0);
	CRect rcSel = GetSelRect();
	CLayer *lyr;
	CArchive *ar;
	CAction *action;
	CString name;

	switch (idCommand) 
	{

	case IDFR_CUT:
		OnToolbarCut();
		break;

	case IDFR_COPY:
		OnToolbarCopy();
		break;

	case IDFR_PASTE:
		OnToolbarPaste();
		break;

	case IDFR_PASTECLONE:
		if (ddm.OkToPaste())
		{
			application->remapImages.clear();
			application->remapAnimations.clear();
			g_bPasteClone = TRUE;
		}
		break;

	case IDFR_DELETE:
		DeleteSelection();
		break;

	case IDFR_ARRAYPASTE:
		if (m_sel.GetCount() >= 1) 
		{
			apDlg.layout_editor = this;
			apDlg.objCount = m_sel.GetCount();
			apDlg.DoModal();
		}
		break;

	case IDFR_ORDERX:

		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGEZORDER;
		ar = action->Prepare1();
		this->SerializeAllZLists(ar);
		action->Prepare2();
		m_undo->AddAction(action);
		// END UNDO INFO //

		pos = layout->layers.GetHeadPosition();
		for (j=0; j < layout->layers.GetCount(); j++) {
			twointArr.RemoveAll();
			lyr = layout->layers.GetNext(pos);

			// regular objects
			pos3 = lyr->m_zOrder.GetHeadPosition();
			pos4 = lyr->m_zOrder.GetTailPosition();
			c = lyr->m_zOrder.GetCount();
			for (i=0; i < c; i++) { // pos4 = item to be moved, pos3 = next item to be looked at
				pos4 = pos3;
				value = lyr->m_zOrder.GetNext(pos3);
				if (m_sel.Find(value)) {
					twoint n;
					n.id = lyr->m_zOrder.GetAt(pos4);
					n.variable = GetObject(n.id)->GetX();
					twointArr.Add(n);
					lyr->m_zOrder.RemoveAt(pos4);
				}
			}

			twointArr.QuickSort();
			for (i = 0; i < twointArr.GetCount(); i++) {
				id = twointArr.GetAt(i).id;
				lyr->m_zOrder.AddTail(id);
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_ORDERY:

		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGEZORDER;
		ar = action->Prepare1();
		this->SerializeAllZLists(ar);
		action->Prepare2();
		m_undo->AddAction(action);
		// END UNDO INFO //

		pos = layout->layers.GetHeadPosition();
		for (j=0; j < layout->layers.GetCount(); j++) {
			twointArr.RemoveAll();
			lyr = layout->layers.GetNext(pos);

			// regular objects
			pos3 = lyr->m_zOrder.GetHeadPosition();
			pos4 = lyr->m_zOrder.GetTailPosition();
			c = lyr->m_zOrder.GetCount();
			for (i=0; i < c; i++) { // pos4 = item to be moved, pos3 = next item to be looked at
				pos4 = pos3;
				value = lyr->m_zOrder.GetNext(pos3);
				if (m_sel.Find(value)) {
					twoint n;
					n.id = lyr->m_zOrder.GetAt(pos4);
					n.variable = GetObject(n.id)->GetY();
					twointArr.Add(n);
					lyr->m_zOrder.RemoveAt(pos4);
				}
			}

			twointArr.QuickSort();
			for (i = 0; i < twointArr.GetCount(); i++) {
				id = twointArr.GetAt(i).id;
				lyr->m_zOrder.AddTail(id);
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_ORDERFRONT:

		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGEZORDER;
		ar = action->Prepare1();
		this->SerializeAllZLists(ar);
		action->Prepare2();
		m_undo->AddAction(action);
		// END UNDO INFO //

		pos = layout->layers.GetHeadPosition();
		for (j=0; j < layout->layers.GetCount(); j++) {
			lyr = layout->layers.GetNext(pos);
			pos3 = lyr->m_zOrder.GetHeadPosition();
			pos4 = lyr->m_zOrder.GetTailPosition();
			for (i=0; i < lyr->m_zOrder.GetCount(); i++) { // pos4 = item to be moved, pos3 = next item to be looked at
				pos4 = pos3;
				value = lyr->m_zOrder.GetNext(pos3);
				if (m_sel.Find(value)) {
					lyr->m_zOrder.RemoveAt(pos4);
					lyr->m_zOrder.AddTail(value);
				}
			}
				// Check the group objects as well
			CObjList groups;
			GetListOfAllGroups(&groups, &lyr->m_zOrder);
			POSITION gpos = groups.GetHeadPosition();
			for(int g = 0; g < groups.GetCount(); g++)
			{
				long group_id = groups.GetNext(gpos);
				CObj* o = layout->GetObj(group_id);

				POSITION pos3 = o->m_groupObjects.GetHeadPosition();
				POSITION pos4 = o->m_groupObjects.GetTailPosition();

				for(int z = 0; z < o->m_groupObjects.GetCount(); z++)
				{
					pos4 = pos3;
					value = o->m_groupObjects.GetNext(pos3);
					if (m_sel.Find(value))
					{
						o->m_groupObjects.RemoveAt(pos4);
						o->m_groupObjects.AddTail(value);
					}
				}
			}	
		}
		layout->SetChanged(true);
		break;
	case IDFR_ORDERBACK:

		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGEZORDER;
		ar = action->Prepare1();
		this->SerializeAllZLists(ar);
		action->Prepare2();
		m_undo->AddAction(action);
		// END UNDO INFO //

		pos = layout->layers.GetHeadPosition();

		for (j=0; j < layout->layers.GetCount(); j++) {
			lyr = layout->layers.GetNext(pos);
			pos3 = lyr->m_zOrder.GetHeadPosition();
			pos4 = lyr->m_zOrder.GetTailPosition();
			for (i=0; i < lyr->m_zOrder.GetCount(); i++) { // pos3 = item to be moved, pos4 = next item to be looked at
				pos3 = pos4;
				value = lyr->m_zOrder.GetPrev(pos4);
				if (m_sel.Find(value)) {
					lyr->m_zOrder.RemoveAt(pos3);
					lyr->m_zOrder.AddHead(value);
				}
			}

			// Check the group objects as well
			CObjList groups;
			GetListOfAllGroups(&groups, &lyr->m_zOrder);
			POSITION gpos = groups.GetHeadPosition();
			for(int g = 0; g < groups.GetCount(); g++)
			{
				long group_id = groups.GetNext(gpos);
				CObj* o = layout->GetObj(group_id);
				
				pos3 = o->m_groupObjects.GetHeadPosition();
				pos4 = o->m_groupObjects.GetTailPosition();
				for(int z = 0; z < o->m_groupObjects.GetCount(); z++)
				{
					pos3 = pos4;
					value = o->m_groupObjects.GetPrev(pos4);
					if (m_sel.Find(value))
					{
						o->m_groupObjects.RemoveAt(pos3);
						o->m_groupObjects.AddHead(value);
					}
				}
			}
		}

		layout->SetChanged(true);		
		break;

	case IDFR_ORDERFORWARD:

		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGEZORDER;
		ar = action->Prepare1();
		this->SerializeAllZLists(ar);
		action->Prepare2();
		m_undo->AddAction(action);
		// END UNDO INFO //

		pos = layout->layers.GetHeadPosition();
		for (j=0; j < layout->layers.GetCount(); j++) {
			lyr = layout->layers.GetNext(pos);
			pos4 = lyr->m_zOrder.GetTailPosition();
			pos3 = NULL;
			for (i=0; i < lyr->m_zOrder.GetCount(); i++) { // pos3 = item to be moved, pos4 = next item to be looked at
				pos2 = pos3;						  // pos2 = item moved after (previous item)
				pos3 = pos4;
				value = lyr->m_zOrder.GetPrev(pos4);
				if (m_sel.Find(value) && pos2 != NULL) {
					lyr->m_zOrder.RemoveAt(pos3);
					lyr->m_zOrder.InsertAfter(pos2, value);
				}
			}
		

			// Check the group objects as well
			CObjList groups;
			GetListOfAllGroups(&groups, &lyr->m_zOrder);
			POSITION gpos = groups.GetHeadPosition();
			for(int g = 0; g < groups.GetCount(); g++)
			{
				long group_id = groups.GetNext(gpos);
				CObj* o = layout->GetObj(group_id);

				pos4 = o->m_groupObjects.GetTailPosition();
				pos3 = NULL;
				for(int z = 0; z < o->m_groupObjects.GetCount(); z++)
				{
					pos2 = pos3;
					pos3 = pos4;
					value = o->m_groupObjects.GetPrev(pos4);
					if (m_sel.Find(value) && pos2 != NULL)
					{
						o->m_groupObjects.RemoveAt(pos3);
						o->m_groupObjects.InsertAfter(pos2, value);
					}
				}
			}
		}

		layout->SetChanged(true);
		break;

	case IDFR_ORDERBACKWARD:

		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGEZORDER;
		ar = action->Prepare1();
		this->SerializeAllZLists(ar);
		action->Prepare2();
		m_undo->AddAction(action);
		// END UNDO INFO //

		pos = layout->layers.GetHeadPosition();
		for (j=0; j < layout->layers.GetCount(); j++) {
			lyr = layout->layers.GetNext(pos);
			pos3 = lyr->m_zOrder.GetHeadPosition();
			pos4 = NULL;
			for (i=0; i < lyr->m_zOrder.GetCount(); i++) { // pos4 = item to be moved, pos3 = next item to be looked at
				pos2 = pos4;						  // pos2 = item moved after (previous item)
				pos4 = pos3;
				value = lyr->m_zOrder.GetNext(pos3);
				if (m_sel.Find(value) && pos2 != NULL) {
					lyr->m_zOrder.RemoveAt(pos4);
					lyr->m_zOrder.InsertBefore(pos2, value);
				}
			}

			// Check the group objects as well
			CObjList groups;
			GetListOfAllGroups(&groups, &lyr->m_zOrder);
			POSITION gpos = groups.GetHeadPosition();
			for(int g = 0; g < groups.GetCount(); g++)
			{
				long group_id = groups.GetNext(gpos);
				CObj* o = layout->GetObj(group_id);

				pos3 = o->m_groupObjects.GetHeadPosition();
				pos4 = NULL;
				for(int z = 0; z < o->m_groupObjects.GetCount(); z++)
				{
					pos2 = pos4;
					pos4 = pos3;
					value = o->m_groupObjects.GetNext(pos3);
					if (m_sel.Find(value) && pos2 != NULL)
					{
						o->m_groupObjects.RemoveAt(pos4);
						o->m_groupObjects.InsertBefore(pos2, value);
					}
				}
			}
		}

		layout->SetChanged(true);
		break;

	case IDFR_ALIGNLEFT:
		if (m_sel.GetCount() >= 2) 
		{
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));

				if (!bValueSet)
				{
					value = o->GetX();
					bValueSet = TRUE;
				}

				else
					value = min(value, o->GetX());
			}

			pos = m_sel.GetHeadPosition();

			for (i=0; i < m_sel.GetCount(); i++)
			{
				o = GetObject(m_sel.GetNext(pos));

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_MOVESIZE;
				if (i > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				o->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				o->SetPos(value, o->GetY());
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_ALIGNCENTER:
		if (m_sel.GetCount() >= 2) 
		{
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));

				if (!bValueSet) {
					value2 = o->GetX();
					value = o->GetX() + o->GetW();
					bValueSet = TRUE;
				}

				else 
				{
					value2 = min(value2, o->GetX());
					value = max(value, o->GetX() + o->GetW());
				}
			}

			pos = m_sel.GetHeadPosition();
			value = (value + value2) / 2;
			for (i=0; i < m_sel.GetCount(); i++)
			{
				o = GetObject(m_sel.GetNext(pos));

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_MOVESIZE;
				if (i > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				o->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				o->SetPos(value - o->GetW()/2, o->GetY());
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_ALIGNRIGHT:
		if (m_sel.GetCount() >= 2) 
		{
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));
				if (!bValueSet) {
					value = o->GetX() + o->GetW();
					bValueSet = TRUE;
				}
				else
					value = max(value, o->GetX() + o->GetW());
			}
			pos = m_sel.GetHeadPosition();
			for (i=0; i < m_sel.GetCount(); i++)
			{
				o = GetObject(m_sel.GetNext(pos));

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_MOVESIZE;
				if (i > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				o->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				o->SetPos(value - o->GetW(), o->GetY());
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_ALIGNTOP:
		if (m_sel.GetCount() >= 2) {
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));
				if (!bValueSet) {
					value = o->GetY();
					bValueSet = TRUE;
				}
				else
					value = min(value, o->GetY());
			}

			pos = m_sel.GetHeadPosition();

			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_MOVESIZE;
				if (i > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				o->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				o->SetPos(o->GetX(), value);
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_ALIGNMIDDLE:
		if (m_sel.GetCount() >= 2) 
		{
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));

				if (!bValueSet)
				{
					value2 = o->GetY();
					value = o->GetY() + o->GetH();
					bValueSet = TRUE;
				}

				else 
				{
					value2 = min(value2, o->GetY());
					value = max(value, o->GetY() + o->GetH());
				}
			}

			pos = m_sel.GetHeadPosition();
			value = (value + value2) / 2;

			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_MOVESIZE;
				if (i > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				o->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				o->SetPos(o->GetX(), value - o->GetH()/2);
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_ALIGNBOTTOM:
		if (m_sel.GetCount() >= 2) 
		{
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));
				if (!bValueSet) {
					value = o->GetY() + o->GetH();
					bValueSet = TRUE;
				}
				else
					value = max(value, o->GetY() + o->GetH());
			}

			pos = m_sel.GetHeadPosition();
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_MOVESIZE;
				if (i > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				o->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				o->SetPos(o->GetX(), value - o->GetH());
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_FRAME_ALIGNLEFT:
		for (i=0; i < m_sel.GetCount(); i++) {
			o = GetObject(m_sel.GetNext(pos));

			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_MOVESIZE;
			if (i > 0)
				action->m_bGroup = FALSE;
			ar = action->Prepare1();
			id = o->GetInstanceID();
			*ar << id;
			o->Serialize(*ar);
			action->Prepare2();
			m_undo->AddAction(action);
			// END UNDO INFO //

			o->SetPos(0, o->GetY());
		}
		layout->SetChanged(true);
		break;

	case IDFR_FRAME_ALIGNCENTER:
		for (i=0; i < m_sel.GetCount(); i++) 
		{
			o = GetObject(m_sel.GetNext(pos));

			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_MOVESIZE;
			if (i > 0)
				action->m_bGroup = FALSE;
			ar = action->Prepare1();
			id = o->GetInstanceID();
			*ar << id;
			o->Serialize(*ar);
			action->Prepare2();
			m_undo->AddAction(action);
			// END UNDO INFO //

			o->SetPos(layout->GetW()/2 - o->GetW()/2, o->GetY());
		}
		layout->SetChanged(true);
		break;

	case IDFR_FRAME_ALIGNRIGHT:
		for (i=0; i < m_sel.GetCount(); i++) 
		{
			o = GetObject(m_sel.GetNext(pos));

			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_MOVESIZE;
			if (i > 0)
				action->m_bGroup = FALSE;
			ar = action->Prepare1();
			id = o->GetInstanceID();
			*ar << id;
			o->Serialize(*ar);
			action->Prepare2();
			m_undo->AddAction(action);
			// END UNDO INFO //

			o->SetPos(layout->GetW() - o->GetW(), o->GetY());
		}
		layout->SetChanged(true);
		break;

	case IDFR_FRAME_ALIGNTOP:
		for (i=0; i < m_sel.GetCount(); i++) 
		{
			o = GetObject(m_sel.GetNext(pos));

			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_MOVESIZE;
			if (i > 0)
				action->m_bGroup = FALSE;
			ar = action->Prepare1();
			id = o->GetInstanceID();
			*ar << id;
			o->Serialize(*ar);
			action->Prepare2();
			m_undo->AddAction(action);
			// END UNDO INFO //

			o->SetPos(o->GetX(), 0);
		}
		layout->SetChanged(true);
		break;

	case IDFR_FRAME_ALIGNMIDDLE:
		for (i=0; i < m_sel.GetCount(); i++) 
		{
			o = GetObject(m_sel.GetNext(pos));

			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_MOVESIZE;
			if (i > 0)
				action->m_bGroup = FALSE;
			ar = action->Prepare1();
			id = o->GetInstanceID();
			*ar << id;
			o->Serialize(*ar);
			action->Prepare2();
			m_undo->AddAction(action);
			// END UNDO INFO //

			o->SetPos(o->GetX(), layout->GetH()/2 - o->GetH()/2);
		}
		layout->SetChanged(true);
		break;

	case IDFR_FRAME_ALIGNBOTTOM:
		for (i=0; i < m_sel.GetCount(); i++) 
		{
			o = GetObject(m_sel.GetNext(pos));

			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_MOVESIZE;
			if (i > 0)
				action->m_bGroup = FALSE;
			ar = action->Prepare1();
			id = o->GetInstanceID();
			*ar << id;
			o->Serialize(*ar);
			action->Prepare2();
			m_undo->AddAction(action);
			// END UNDO INFO //

			o->SetPos(o->GetX(), layout->GetH() - o->GetH());
		}
		layout->SetChanged(true);
		break;

	case IDFR_DISTRIBUTEH:
		if (m_sel.GetCount() >= 3) {
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));
				if (!bValueSet) {
					value2 = o->GetX();					// leftmost point
					value = o->GetX() + o->GetW();		// rightmost point
					value3 = o->GetW();					// total width of objects
					bValueSet = TRUE;
				}

				else 
				{
					value2 = min(value2, o->GetX());
					value = max(value, o->GetX() + o->GetW());
					value3 += o->GetW();
				}

				twoint values;
				values.variable = o->GetX();
				values.id = o->GetInstanceID();
				twointArr.Add(values);
			}

			twointArr.QuickSort();
			float space = ((float)(value - value2 - value3)) / ((float)m_sel.GetCount()-1); // space btw objects
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(twointArr[i].id);

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_MOVESIZE;
				if (i > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				o->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				o->SetPos(value2 + value4 + ((long)(space * ((float)i))), o->GetY());
				value4 += o->GetW();
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_DISTRIBUTEV:
		if (m_sel.GetCount() >= 3) 
		{
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));
				if (!bValueSet) {
					value2 = o->GetY();					// uppermost point
					value = o->GetY() + o->GetH();		// lowermost point
					value3 = o->GetH();					// total height of objects
					bValueSet = TRUE;
				}
				else {
					value2 = min(value2, o->GetY());
					value = max(value, o->GetY() + o->GetH());
					value3 += o->GetH();
				}
				twoint values;
				values.variable = o->GetY();
				values.id = o->GetInstanceID();
				twointArr.Add(values);
			}
			twointArr.QuickSort();
			float space = ((float)(value - value2 - value3)) / ((float)m_sel.GetCount()-1); // space btw objects
			for (i=0; i < m_sel.GetCount(); i++) {
				o = GetObject(twointArr[i].id);

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_MOVESIZE;
				if (i > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				o->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				o->SetPos(o->GetX(), value2 + value4 + ((long)(space * ((float)i))));
				value4 += o->GetH();
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_SAMEWIDTH:
		if (m_sel.GetCount() >= 2) {
			for (i=0; i < m_sel.GetCount(); i++) {
				o = GetObject(m_sel.GetNext(pos));
				if (!bValueSet) {
					value = o->GetW();
					bValueSet = TRUE;
				}
				else {
					// START UNDO INFO //
					action = new CAction();
					action->m_type = ACTION_MOVESIZE;
					if (i > 1)
						action->m_bGroup = FALSE;
					ar = action->Prepare1();
					id = o->GetInstanceID();
					*ar << id;
					o->Serialize(*ar);
					action->Prepare2();
					m_undo->AddAction(action);
					// END UNDO INFO //

					o->SetSize(value, o->GetH());
				}
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_SAMEHEIGHT:
		if (m_sel.GetCount() >= 2) 
		{
			for (i=0; i < m_sel.GetCount(); i++) 
			{
				o = GetObject(m_sel.GetNext(pos));
				if (!bValueSet) {
					value = o->GetH();
					bValueSet = TRUE;
				}
				else {
					// START UNDO INFO //
					action = new CAction();
					action->m_type = ACTION_MOVESIZE;
					if (i > 1)
						action->m_bGroup = FALSE;
					ar = action->Prepare1();
					id = o->GetInstanceID();
					*ar << id;
					o->Serialize(*ar);
					action->Prepare2();
					m_undo->AddAction(action);
					// END UNDO INFO //

					o->SetSize(o->GetW(), value);
				}
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_SAMESIZE:
		if (m_sel.GetCount() >= 2) {
			for (i=0; i < m_sel.GetCount(); i++)
			{
				o = GetObject(m_sel.GetNext(pos));
				if (!bValueSet) {
					value = o->GetW();
					value2 = o->GetH();
					bValueSet = TRUE;
				}
				else {
					// START UNDO INFO //
					action = new CAction();
					action->m_type = ACTION_MOVESIZE;
					if (i > 1)
						action->m_bGroup = FALSE;
					ar = action->Prepare1();
					id = o->GetInstanceID();
					*ar << id;
					o->Serialize(*ar);
					action->Prepare2();
					m_undo->AddAction(action);
					// END UNDO INFO //

					o->SetSize(value, value2);
				}
			}
		}
		layout->SetChanged(true);
		break;

	case IDFR_CENTERFRAME:
		GetClientRect(&rc);
		ScrollToPosition(CPoint(layout->GetW()/2, layout->GetH()/2) + CPoint(-(rc.right / 2), -(rc.bottom / 2)) + CPoint(FRAME_MARGIN_W*m_Zoom, FRAME_MARGIN_H*m_Zoom));
		break;

	case IDFR_OBJECTINFORMATION:
		{
			if (m_sel.GetCount() == 0) return;
			o = GetObject(m_sel.GetNext(pos));
			OINFO* oi;

			// Check not a group
			if (o->GetObjectType(application)->DLLIndex != -1)
			{
				oi = GetOINFO(o->GetObjectType(application)->DLLIndex);

				CObjectInfoDlg objInfo;
				objInfo.objname = oi->extName;
				objInfo.DoModal();
			}
		}
		break;

	case IDFR_REMOVEALLMOVEMENTS:
		{
			for (int x=0; x < m_sel.GetCount(); x++) 
			{
				CObj*     obj = GetObject(m_sel.GetNext(pos));
				CObjType*  oT = obj->GetObjectType(application);
				
				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_CHANGETYPE;
				if (x > 0) // all together now
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = oT->ObjectIdentifier;
				*ar << id;
				oT->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				oT->behaviors.clear();
				layout->SetChanged(true);
			}
		}
		break;

	case IDFR_REMOVEALLEFFECTS:
		{
			for (int x=0; x < m_sel.GetCount(); x++) 
			{
				CObj*     obj = GetObject(m_sel.GetNext(pos));
				CObjType*  oT = obj->GetObjectType(application);
				
				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_CHANGETYPE;
				if (x > 0) // all together now
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = oT->ObjectIdentifier;
				*ar << id;
				oT->Serialize(*ar);
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				oT->effects.clear();
				layout->SetChanged(true);
			}
		}
		break;

	case IDFR_NEWMOVEMENT:
		{
			CNewBehaviorDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				// Add this Behavior to the selected objects
				CStringArray Names;
		
				for (int x=0; x < m_sel.GetCount(); x++) 
				{
					CObj*     obj = GetObject(m_sel.GetNext(pos));
					CObjType*  oT = obj->GetObjectType(application);
				
					// START UNDO INFO //
					action = new CAction();
					action->m_type = ACTION_CHANGETYPE;
					if (x > 0) // all together now
						action->m_bGroup = FALSE;
					ar = action->Prepare1();
					id = oT->ObjectIdentifier;
					*ar << id;
					oT->Serialize(*ar);
					action->Prepare2();
					m_undo->AddAction(action);
					// END UNDO INFO //

					bool Found = false;
					for (int y = 0; y < Names.GetSize(); y++)
						if (Names.GetAt(y) == oT->GetName()) Found = true;

					if (!Found)
					{
						CString behaviorName = oT->GetUniqueBehaviorName(dlg.Text);
						oT->behaviors.push_back(CBehavior());
						CBehavior& Mv = oT->behaviors.back();
						Mv.BehaviorDLLIndex = dlg.ID;
						Mv.Text = behaviorName;
						Mv.CreateDefault(layout);
						Names.Add(oT->GetName());
						Mv.id = application->m_objID++;
						layout->SetChanged(true);
					}
				}
				break;
			}
		}
		break;

	case IDFR_NEWEFFECT:
		{

			CObj*     obj = GetObject(m_sel.GetNext(pos));
			CObjType*  oT = obj->GetObjectType(application);

			// Check there isn't a PS0.0 effect already at the end
			if (!oT->effects.empty() && oT->effects.back().version == 0.0f) {
				CString msg;
				msg.Format("There is a non-pixel shader effect as the last effect (%s).  Non-pixel shader effects must be the last effect.  Remove this effect to add other effects.",
					oT->effects.back().Text);
				MessageBox(msg, "Add effect", MB_OK | MB_ICONEXCLAMATION);
			}

			CNewHLSLDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				// Add this Behavior to the selected objects
				CStringArray Names;
		
				for (int x=0; x < m_sel.GetCount(); x++) 
				{				
					// START UNDO INFO //
					action = new CAction();
					action->m_type = ACTION_CHANGETYPE;
					if (x > 0) // all together now
						action->m_bGroup = FALSE;
					ar = action->Prepare1();
					id = oT->ObjectIdentifier;
					*ar << id;
					oT->Serialize(*ar);
					action->Prepare2();
					m_undo->AddAction(action);
					// END UNDO INFO //

					bool Found = false;
					for (int y = 0; y < Names.GetSize(); y++)
						if (Names.GetAt(y) == oT->GetName()) Found = true;

					if (!Found)
					{
						dlg.m_Text = oT->GetUniqueEffectName(dlg.m_Text);
						oT->effects.push_back(CEffect());
						CEffect& Effect = oT->effects.back();
						Effect.m_effectID = application->m_objID++;
						Effect.effectFile = dlg.m_Filename;
						Effect.Text       = dlg.m_Text;
						Effect.version	= dlg.m_Version;
						Effect.Init(this);
						
						layout->SetChanged(true);
					}
				}
				break;
			}
		}
		break;

	case IDFR_CENTERFRAMEOVERSELECTION:
		for (i=0; i < m_sel.GetCount(); i++)
		{
			o = GetObject(m_sel.GetNext(pos));
			if (!bValueSet) {
				value2 = o->GetX();					// leftmost point
				value = o->GetX() + o->GetW();		// rightmost point
				value4 = o->GetY();					// uppermost point
				value3 = o->GetY() + o->GetH();		// lowermost point
				bValueSet = TRUE;
			}

			else 
			{
				value2 = min(value2, o->GetX());
				value = max(value, o->GetX() + o->GetW());
				value4 = min(value4, o->GetY());
				value3 = max(value3, o->GetY() + o->GetH());
			}
		}
		value = (value + value2) / 2;				// center x
		value3 = (value3 + value4) / 2;				// center y
		GetClientRect(&rc);
		ScrollToPosition(CPoint(value, value3) + CPoint(-(rc.right / 2), -(rc.bottom / 2)) + CPoint(FRAME_MARGIN_W*m_Zoom, FRAME_MARGIN_H*m_Zoom));
		break;

	case IDFR_GROUP:

		id = GroupSel();

		break;
	case IDFR_UNGROUP:

		for (i = 0; i < m_sel.GetCount(); i++)
		{
			o = GetObject(m_sel.GetNext(pos));
			if (o->m_bIsGroup) {
				Ungroup(o);
			}
		}
		m_sel.RemoveAll();
		break;
	case IDFR_OPENGROUP:
		j = 0;
		for (i = 0; i < m_sel.GetCount(); i++)
		{
			o = GetObject(m_sel.GetNext(pos));
			if (o->m_bIsGroup && !o->m_bGroupOpen) {
				o->m_bGroupOpen = TRUE;

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_GROUPOPEN;
				if (j > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				id = 0; // 0 to close
				*ar << id;
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //
				
				j++;
			}
		}
		break;
	case IDFR_CLOSEGROUP:
		j = 0;
		for (i = 0; i < m_sel.GetCount(); i++)
		{
			o = GetObject(m_sel.GetNext(pos));
			if (o->m_bIsGroup && o->m_bGroupOpen) {
				o->m_bGroupOpen = FALSE;

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_GROUPOPEN;
				if (j > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				id = 1; // 1 to open
				*ar << id;
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //
				
				j++;
			}
			else if (o->m_bInGroup) {
				o->m_group->m_bGroupOpen = FALSE;

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_GROUPOPEN;
				if (j > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o->GetInstanceID();
				*ar << id;
				id = 1; // 1 to open
				*ar << id;
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //
				
				j++;
			}
		}
		break;
	case IDFR_ADDTOGROUP:
		g_addToGroup = TRUE;
		break;
	case IDFR_REMFROMGROUP:
		j = 0;
		for (i = 0; i < m_sel.GetCount(); i++) {
			o2 = GetObject(m_sel.GetNext(pos));
			if (o2->m_bInGroup) {
				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_GROUPADD;
				if (j > 0)
					action->m_bGroup = FALSE;
				ar = action->Prepare1();
				id = o2->m_group->GetInstanceID();
				*ar << id;
				id = o2->GetInstanceID();
				*ar << id;
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //

				RemoveFromGroup(o2);
				
				j++;
			}
		}
		break;

	case IDFR_LOCKSELECTED:
		for (i=0; i < m_sel.GetCount(); i++)
		{
			BOOL bLock = FALSE;
			o = GetObject(m_sel.GetNext(pos));
			o->SetLocked(!bLock);
			id = o->GetInstanceID();

			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_SETLOCK;
			if (i != 0)
				action->m_bGroup = FALSE;
			ar = action->Prepare1();
			*ar << id << bLock;
			action->Prepare2();
			m_undo->AddAction(action);
			// END UNDO INFO //
		}
		m_sel.RemoveAll();
		break;

	case IDFR_LOCKUNSELECTED:
		i = 0;
		while (pos2 != NULL) {
			layout->objects.GetNextAssoc( pos2, nKey, o );
			value = o->GetInstanceID();
			if (!m_sel.Find(value)) 
			{
				if (!o->GetLocked()) {
					BOOL bLock = FALSE;
					o->SetLocked(!bLock);
					id = o->GetInstanceID();

					// START UNDO INFO //
					action = new CAction();
					action->m_type = ACTION_SETLOCK;
					if (i != 0)
						action->m_bGroup = FALSE;
					ar = action->Prepare1();
					*ar << id << bLock;
					action->Prepare2();
					m_undo->AddAction(action);
					// END UNDO INFO //
				}
			}
			i++;
		}
		break;

	case IDFR_UNLOCKALL:
		i = 0;
		while (pos2 != NULL) 
		{
			layout->objects.GetNextAssoc( pos2, nKey, o );
			if (o->GetLocked()) {
				BOOL bLock = TRUE;
				o->SetLocked(!bLock);
				id = o->GetInstanceID();

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_SETLOCK;
					if (i != 0)
						action->m_bGroup = FALSE;
				ar = action->Prepare1();
				*ar << id << bLock;
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //
			}
			i++;
		}
		break;

	case IDFR_HIDESELECTED:
		for (i=0; i < m_sel.GetCount(); i++) 
		{
			BOOL bHide = TRUE;
			o = GetObject(m_sel.GetNext(pos));
			o->SetVisible(!bHide);
			id = o->GetInstanceID();

			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_SETVISIBLE;
			if (i != 0)
				action->m_bGroup = FALSE;
			ar = action->Prepare1();
			*ar << id << bHide;
			action->Prepare2();
			m_undo->AddAction(action);
			// END UNDO INFO //
		}
		m_sel.RemoveAll();
		break;

	case IDFR_HIDEUNSELECTED:
		i = 0;
		while (pos2 != NULL) 
		{
			layout->objects.GetNextAssoc( pos2, nKey, o );
			value = o->GetInstanceID();
			if (!m_sel.Find(value)) {
				if (o->GetVisible()) {
					BOOL bHide = TRUE;
					o->SetVisible(!bHide);
					id = o->GetInstanceID();

					// START UNDO INFO //
					action = new CAction();
					action->m_type = ACTION_SETVISIBLE;
					if (i != 0)
						action->m_bGroup = FALSE;
					ar = action->Prepare1();
					*ar << id << bHide;
					action->Prepare2();
					m_undo->AddAction(action);
					// END UNDO INFO //
				}
			}
			i++;
		}
		break;

	case IDFR_UNHIDEALL:
		i = 0;
		while (pos2 != NULL) 
		{
			layout->objects.GetNextAssoc( pos2, nKey, o );

			if (!o->GetVisible()) {
				BOOL bHide = FALSE;
				o->SetVisible(!bHide);
				id = o->GetInstanceID();

				// START UNDO INFO //
				action = new CAction();
				action->m_type = ACTION_SETVISIBLE;
					if (i != 0)
						action->m_bGroup = FALSE;
				ar = action->Prepare1();
				*ar << id << bHide;
				action->Prepare2();
				m_undo->AddAction(action);
				// END UNDO INFO //
			}
			i++;
		}
		break;

	case IDFR_INSERT:
		{
			InsertObjectDialog dialog(*application);
			dialog.DoModal();

			if (gInsert != "")
			{
				if (gInsertAuto == 1)
				{
					m_AddedObject = gInsert;
					CreateObject(gInsert, CPoint(-20000, -20000));
					g_SuspendFrameEditor = false;
					RedrawWindow();
					m_bPanel = true;
					SetTimer(500, 2000, NULL);

					gInsert = "";

					gInsertAuto = 0;

					CChildFrame* pCF = (CChildFrame*)GetParentFrame();
					pCF->object_bar.Refresh();

					layout->SetChanged(true);
				}
			}
		} break;
	}

	m_UpdatePreview = true;

	CChildFrame* pCF = (CChildFrame*)GetParentFrame();
	pCF->object_bar.Refresh();

	if (m_sel.GetCount() > 0)
		g_PropertyBar->Update(this, TYPE_OBJECT, &m_sel, layout, &layout->objects, application); // show object props
	else
		g_PropertyBar->Update(this, TYPE_LAYOUT, NULL, layout, &layout->objects, application);

	m_bUpdateFrame = TRUE;
	RedrawWindow();
}

void CLayoutEditor::AddToGroup(CObj *obj, CObj *group)
{
	obj->m_bInGroup = TRUE;
	obj->m_group = group;
	CLayer *lyr = GetObjectLayer(obj->GetInstanceID());
	if(!lyr) return;
	long id = obj->GetInstanceID();
	POSITION pos = lyr->m_zOrder.Find(id);
	long prevObj = lyr->m_zOrder.GetPrev(pos);
	lyr->m_zOrder.RemoveAt(lyr->m_zOrder.Find(id));
	obj->m_group->m_groupObjects.AddTail(id);
	long gid = group->GetInstanceID();

	group->UpdateGroupInfo();

	// START UNDO INFO //
	CAction *action = new CAction();
	action->m_type = ACTION_GROUPREMOVE;
	CArchive *ar = action->Prepare1();
	*ar << gid << id << lyr->m_layerID << prevObj;
	action->Prepare2();
	m_undo->AddAction(action);
	// END UNDO INFO //
}

void CLayoutEditor::RemoveFromGroup(CObj *object)
{
	long id = object->GetInstanceID();
	long gid = id;
	CObj *group = object->m_group;
	group->m_groupObjects.RemoveAt(group->m_groupObjects.Find(id));
	group->UpdateGroupInfo(FALSE);
	while (group->m_bInGroup) { // find root in case of nested groups
		group = group->m_group;
	}
	object->m_bInGroup = FALSE;
	object->m_group = NULL;
	id = group->GetInstanceID();
	CLayer *lyr = GetObjectLayer(id);
	if(!lyr) return;
	POSITION pos2 = lyr->m_zOrder.Find(id);
	id = object->GetInstanceID();
	lyr->m_zOrder.InsertAfter(pos2, id);

	group->UpdateGroupInfo();

	// START UNDO INFO //
	CAction *action = new CAction();
	action->m_type = ACTION_GROUPADD;
	CArchive *ar = action->Prepare1();
	*ar << gid << id;
	action->Prepare2();
	m_undo->AddAction(action);
	// END UNDO INFO //
}

void CLayoutEditor::Ungroup(CObj *o)
{
	if (!o->m_bIsGroup)
		return;
	
	long id = o->GetInstanceID();

	// START UNDO INFO //
	CAction *action = new CAction();
	action->m_type = ACTION_GROUP;
	CArchive *ar = action->Prepare1();
	*ar << id;
	o->m_groupObjects.Serialize(*ar);
	action->Prepare2();
	m_undo->AddAction(action);
	// END UNDO INFO //

	// remove from zorder
	CObj *o2;
	POSITION pos2;
	CLayer *lyr;
	if (!o->m_bInGroup) {
		lyr = GetObjectLayer(id);
		if(!lyr) return;
		pos2 = lyr->m_zOrder.Find(id); // get location in layer to put group objects
	}

	POSITION pos3 = o->m_groupObjects.GetHeadPosition();
	for (int j = 0; j < o->m_groupObjects.GetCount(); j++) {
		o2 = GetObject(o->m_groupObjects.GetNext(pos3));
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

	DeleteObject(o->GetInstanceID(), TRUE);
}

long CLayoutEditor::GroupSel()
{
	g_pLayoutView = this;
	CArchive *ar;
	// START UNDO INFO PART 1 //
	CAction *action3 = new CAction();
	action3->m_bGroup = TRUE;
	action3->m_type = ACTION_CHANGEZORDER;
	ar = action3->Prepare1();
	this->SerializeAllZLists(ar);
	action3->Prepare2();
	m_undo->AddAction(action3);
	// END UNDO INFO PART 1 //

	CObj *o2 = new CObj(); // group CObj
	CObj *o; // group members CObj
	CLayer *nlyr = NULL;
	o2->m_bIsGroup = TRUE;
	POSITION pos = m_sel.GetHeadPosition();
	for (int i = 0; i < m_sel.GetCount(); i++) { // loop through selected objects
		o = GetObject(m_sel.GetNext(pos));
		if (o->m_bInGroup == FALSE) { // selected object is not in another [open] group
			CLayer *lyr = GetObjectLayer(o->GetInstanceID()); // get layer to put the group in
			o->m_bInGroup = TRUE;
			o->m_group = o2;
			if (!nlyr)
				nlyr = lyr;
			long id = o->GetInstanceID();
			lyr->m_zOrder.RemoveAt(lyr->m_zOrder.Find(id)); // move object from its layer to the group
			o2->m_groupObjects.AddTail(id);
		}
	}
	
	// set up group CObj
	long id = GetUnusedID();
	CString name = GetUnusedName("Group");
	o2->m_tempName = name;

	// create and set up group CObjType
	CObjType *ot = new CObjType();
	ot->SetName(name);
	ot->m_bIsGroupType = TRUE;
	long oID = GetUnusedOID();
	ot->ObjectIdentifier = oID;
	ot->DLLIndex = -1; //CHECKCHECK: what should this be for a group?
	HBITMAP icon  = NULL; //oInfo->largeIcon; //CHECKCHECK: we should probably make some default icon for groups
	HBITMAP sicon = NULL; //oInfo->smallIcon;
	ot->m_Image.CreateFromHBITMAP(icon);
	ot->small_image.CreateFromHBITMAP(sicon);
	
	// create and set up group editInfo
	//o2->editObject = new editInfo();		// editObject now points to member variable
	o2->editObject->instanceID = id;
	o2->editObject->ObjectIdentifier = oID;
	o2->SetDLLIdentifier(-1);
	o2->editObject->objectAngle = 0;

	// add group to layer
	nlyr->m_zOrder.AddTail(id);

	// name database

	// add CObj and CObjType to application
	layout->objects[id] = o2;
	application->object_types[oID] = ot;

	//update selection
	m_sel.RemoveAll();
	m_sel.AddTail(id);

	// set position and size of group container
	o2->SetPos(0, 0, FALSE);
	o2->UpdateGroupInfo(FALSE);
	
	// START UNDO INFO PART 2 //
	CAction *action = new CAction();
	action->m_type = ACTION_DELETE;
	action->m_bGroup = FALSE;
	ar = action->Prepare1();
	*ar << id;
	action->Prepare2();
	m_undo->AddAction(action);

	CAction *action2 = new CAction();
	action2->m_type = ACTION_UNGROUP;
	action2->m_bGroup = FALSE;
	ar = action2->Prepare1();
	*ar << id;
	action2->Prepare2();
	m_undo->AddAction(action2);
	// END UNDO INFO PART 2 //

	return o2->GetInstanceID();
}