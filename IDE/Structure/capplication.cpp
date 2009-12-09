// CApplication.cpp
// All CApplication functions

#include "StdAfx.h"
#include "..\Structure.h"
#include "..\MainFrm.h"
#include "..\Dialogs\ProgressDlg.h"
#include "..\Dialogs\AddFamilyVarsDlg.h"

#include "GUIPersistence.h"

extern CMainFrame* g_MainFrame;
extern CPluginMap g_Plugins;

CString g_BuiltInTraits[NUM_BUILTIN_TRAITS];

CApplication::CApplication() :
	verifying(false),
	runtime(rt_directx),
	event_sheet_unique_identifier(0)
{
	srand (time(NULL));
	m_appID = rand();
	m_layerID = 0;

	m_frameID = 0; 
	m_objID = 2; // we give it a head start because effect actions use this value as well...and it needs to be greater than 1
	m_varID = 0;;



	reserved3 = 0;
	reserved4 = 0;

	// Add default traits
	int numDefaultTraits = sizeof(g_BuiltInTraits) / sizeof(g_BuiltInTraits[0]);

	g_BuiltInTraits[0] = ATTRIBS_SOLID;
	g_BuiltInTraits[1] = ATTRIBS_CENTERVIEW;
	g_BuiltInTraits[2] = ATTRIBS_DESTROYSTART;
	g_BuiltInTraits[3] = ATTRIBS_DESTROYIFFAR;
	g_BuiltInTraits[4] = ATTRIBS_DESTROYIFNOTONSCREEN;
	g_BuiltInTraits[5] = ATTRIBS_PLATFORM;
	g_BuiltInTraits[6] = ATTRIBS_NOSERIALIZE;

	for (int i = 0; i < NUM_BUILTIN_TRAITS; i++)
		traits.push_back(g_BuiltInTraits[i]);

	file_information.file_path = "";
	m_drag_app_from = 0;

	m_display = NULL;
	m_display_refcount = 0;

	// Effect textures
	iLayerTexture = -2;
	iOriginalTexture = -2;
	iEffectTargets[0] = -2;
	iEffectTargets[1] = -2;
	iDisplayTexture = -2;
	iPicture = -2;
	iThumbnail = -2;

	// Default controls
	m_Controls.push_back(BehaviorControl("Move Left", 21, VK_LEFT));
	m_Controls.push_back(BehaviorControl("Move Right", 22, VK_RIGHT));
	m_Controls.push_back(BehaviorControl("Move Up", 19, VK_UP));
	m_Controls.push_back(BehaviorControl("Move Down", 20, VK_DOWN));
	m_Controls.push_back(BehaviorControl("Jump", 5, VK_SHIFT));
	m_Controls.push_back(BehaviorControl("Steer Left", 21, VK_LEFT));
	m_Controls.push_back(BehaviorControl("Steer Right", 22, VK_RIGHT));
	m_Controls.push_back(BehaviorControl("Accelerate", 19, VK_UP));
	m_Controls.push_back(BehaviorControl("Brake", 20, VK_DOWN));

	warnedAboutShaders = false;	

	// Auto backup
	m_Backup = 1;

	property_expand_state["Advanced"] = 1;	// Close the Advanced group by default
}

bool CApplication::CloseSafely()
{
	int Return = 0;

	// Find changes
	bool bChanged = false;
	if (file_information.modified) bChanged = true;

	POSITION pos = layouts.GetHeadPosition();
	while(pos)
	{
		CLayout* pLayout = layouts.GetNext(pos);
		if(pLayout->GetChanged())
			bChanged = true;
	}

	if (bChanged)
	{
		CString Text;
		Text.Format("%s %s", file_information.name, CONF_CLOSE);
		Return = MessageBox(g_MainFrame->m_hWnd, Text, "Construct", MB_ICONQUESTION | MB_YESNOCANCEL);

		// fix prof-uis draw bug
		g_MainFrame->Invalidate();

		if (Return == IDCANCEL) return false;
	}

	if (Return == IDYES)
	{
		if (!g_MainFrame->OnFileSave(this)) return false;
		file_information.modified = false;
	}

	CPath out;
	out.SetPath(file_information.file_path);
	
	std::string directory = (LPCSTR)out.GetFullPath();
	directory += (LPCSTR)out.GetFileTitle();
	directory += ".persist";
	 
	GUIPersistence persist(g_MainFrame, this);
	persist.SavePersistence(directory);

	return true;
}

/////////////////////////////////////////////////////////////////////
// Information routines                                            //
/////////////////////////////////////////////////////////////////////
void CApplication::ChangeName(CString name_)
{
	file_information.name = name_;

	HWND ClientWindow;
	ClientWindow = ::GetWindow(g_MainFrame->m_hWndMDIClient, GW_CHILD);

	// Now find any layouts within this application
	while (ClientWindow != NULL)
	{
		CChildFrame* pWnd = (CChildFrame*)CWnd::FromHandle(ClientWindow);
		if(pWnd->application == this)
		{
			// rename
			if (pWnd->layout)
			{
				CString Title;
				Title.Format("%s : %s", file_information.name, pWnd->layout->GetName());
				pWnd->SetWindowText(Title);
			}

			else
			{
				CString Title;
				Title.Format("%s : %s", file_information.name, pWnd->event_sheet->GetName());
				pWnd->SetWindowText(Title);
			}
		}

		ClientWindow = :: GetWindow(ClientWindow, GW_HWNDNEXT);
	}

	// Update windows displaying this potentially
	g_MainFrame->tabs.RedrawWindow();
}

bool CApplication::ObjectFolderNameExists(CString name)
{
	name.MakeLower();

	vector<ObjectFolder>::iterator i = object_folders.begin();

	for ( ; i != object_folders.end(); i++)
	{
		CString current_name = i->name;
		current_name.MakeLower();

		if (name == current_name)
			return true;
	}

	return false;
}

CString	CApplication::GetUniqueObjectFolderName(CString start)
{
	if (!ObjectFolderNameExists(start))
		return start;

	CString name = start;
	int count = 2;

	while (ObjectFolderNameExists(name))
	{
		name.Format("%s (%d)", start, count);
		count++;
	}

	return name;
}

CImageResource* CApplication::ResourceInformation::FindImageResourceFromNumber(int fixed)
{
	for (list<CImageResource>::iterator i = images.begin(); i != images.end(); i++)
	{
		if (i->m_FixedID == fixed)
			return &(*i);	// i is an iterator : DONT CHANGE THIS
	}
	return 0;
}

CLayer* CApplication::FindLayerByName(CString name)
{
	POSITION pos = layouts.GetHeadPosition();

	CLayer* result = 0;

	while(pos)
	{
		CLayout* pLayout = layouts.GetNext(pos);

		POSITION pos2 = pLayout->layers.GetHeadPosition();
		while(pos2)
		{
			CLayer* new_layer = pLayout->layers.GetNext(pos2);

			if(new_layer->m_name == name)
			{
				if(!result)
					result = new_layer;
				else if(result->m_layerID > new_layer->m_layerID) 
					result = new_layer;
			}

		}
	}
	return result;
}

CLayer* CApplication::FindLayerFromNumber(int fixed, CLayout** ppLayout)
{
	POSITION pos = layouts.GetHeadPosition();
	while(pos)
	{
		CLayout* pLayout = layouts.GetNext(pos);

		POSITION pos2 = pLayout->layers.GetHeadPosition();
		while(pos2)
		{
			CLayer* new_layer = pLayout->layers.GetNext(pos2);

			
			if(new_layer->m_layerID == fixed)
			{
				if(ppLayout)
					*ppLayout = pLayout;
				return new_layer;
			}
		}
	}
	return NULL;
}


CAnimation* RecursiveFindAnimation( list<CAnimation>& Anims, int fixed, CAnimation** parent)
{
	for (list<CAnimation>::iterator i = Anims.begin(); i != Anims.end(); i++)
	{
		if (i->m_FixedID == fixed)
			return &(*i);	// i is an iterator : DONT CHANGE THIS
		
		CAnimation* child = RecursiveFindAnimation(i->m_SubAnimations, fixed, parent);
		if(child)
		{
			if(parent)
				if(!*parent)
					*parent = &*i;
			return child;
		}
	}
	return NULL;
}

bool RecursiveDeleteAnimation(list<CAnimation>& Anims, int fixed)
{
	for (list<CAnimation>::iterator i = Anims.begin(); i != Anims.end(); i++)
	{
		if (i->m_FixedID == fixed)
		{
			// Delete from current list
			Anims.erase(i);

			return true;
		}

		if (RecursiveDeleteAnimation(i->m_SubAnimations, fixed))
			return true;
	}

	return false;
}

CApplication::GlobalVariable* CApplication::GetGlobalVariable(int fixed)
{
	for(list<CApplication::GlobalVariable>::iterator i = global_variables.begin(); i!= global_variables.end(); i++)
		if(i->identifier == fixed)
			return &*i;
	
	return NULL;
}

bool CApplication::ResourceInformation::DeleteAnimationFromNumber(int fixed)
{
	return RecursiveDeleteAnimation(animations, fixed);
}

void CApplication::ResourceInformation::CopyCollisionMaskToEveryFrameInAnimation(CAnimation* pAnim, CImageResource* pImg)
{
	for(vector<int>::iterator i = pAnim->m_Images.begin(); i != pAnim->m_Images.end(); i++)
	{
		CImageResource* dest = FindImageResourceFromNumber(*i);
		if(dest)
		{
			CPoint hotspotdiff = dest->m_Hotspot - pImg->m_Hotspot;
			hotspotdiff.y = dest->m_Collision.GetHeight() - hotspotdiff.y - 1; //CxImage has its y flipped

			dest->m_Collision = dest->bitmap;
			dest->m_Collision.Clear(); 
			dest->m_Collision.AlphaClear();
			dest->m_Collision.Mix(pImg->m_Collision, CxImage::OpAdd, hotspotdiff.x, hotspotdiff.y, true);
		}
	}
	for(list<CAnimation>::iterator s = pAnim->m_SubAnimations.begin(); s != pAnim->m_SubAnimations.end(); s++)
	{
		CopyCollisionMaskToEveryFrameInAnimation(&*s, pImg);
	}
}

CAnimation* CApplication::ResourceInformation::FindAnimationFromNumber(int fixed, CAnimation** parent)
{
	if(parent)
		*parent = NULL;

	return RecursiveFindAnimation(animations, fixed, parent);
}

Family* CApplication::FindFamilyFromNumber(int fixed)
{
	for (list<Family>::iterator i = families.begin(); i != families.end(); i++)
	{
		if (i->identifier == fixed)
		{
			i->is_in_use = true;
			return &(*i);	// i is an iterator : DONT CHANGE THIS
		}
	}
	return 0;
}

int CApplication::GetLayoutIndexFromID(int id)
{
	POSITION pos = layouts.GetHeadPosition();
	int i = 0;

	while(pos)
	{
		CLayout* pLayout = layouts.GetNext(pos);
		if(pLayout->identifier == id)
			return i;

		i++;
	}

	return 0;
}

EventSheet* CApplication::FindEventSheetFromNumber(int fixed)
{
	for (list<EventSheet*>::iterator i = event_sheets.begin(); i != event_sheets.end(); i++)
	{
		if ((*i)->identifier == fixed)
			return (*i);	// i is an iterator : DONT CHANGE THIS
	}
	return 0;
}


CApplication::~CApplication() 
{
	CLayout *pLayout;
	POSITION pos = layouts.GetHeadPosition();
	int iter = 0;
	int max  = layouts.GetCount();

	while (pos != NULL) 
	{ 
		if (iter == max) break;

		// Iterate through frames and delete each one
		pLayout = layouts.GetNext(pos);
		delete pLayout;
		
		iter++;
	}

	layouts.RemoveAll();

	long nKey = 0;

	CObjType *oT;
	pos = object_types.GetStartPosition();

	while (pos != NULL) 
	{ 
		// iterate through objects to choose one to select
		object_types.GetNextAssoc( pos, nKey, oT );
		delete oT;
	}

	pos = m_DeletedObjectTypes.GetStartPosition();

	while (pos != NULL) 
	{ 
		// iterate through objects to choose one to select
		m_DeletedObjectTypes.GetNextAssoc( pos, nKey, oT );
		delete oT;
	}

	object_types.RemoveAll();
	m_DeletedObjectTypes.RemoveAll();

	// remove textures used
	map<int, int>::iterator i = m_image_to_texture.begin();

	//for ( ; i != m_image_to_texture.end(); i++)
	//	m_display->RemoveTexture(i->second);	

	// Nothing's using this display any more
	if (m_display && m_display_refcount == 0) {
		delete m_display; // calls Close()
		m_display = NULL;
	}
}

void CApplication::Instantiate()
{
	// setup default menu
	MenuResource menu("Default");
	menu.CreateDefault();

	resources.menu_resources.push_back(menu);

	// default object folder
	CApplication::ObjectFolder f;
	f.name = "Default";
	object_folders.push_back(f);

	m_ImageFixID     = 0;
	m_AnimationFixID = 0;

	// do icon
	HBITMAP bitmap = ::LoadBitmap(AfxGetInstanceHandle(),
                              MAKEINTRESOURCE(256));

	IconResource Icon("Default");
	Icon.icon.CreateFromHBITMAP(bitmap);
	resources.icon_resources.push_back(Icon);

	m_System.DLLIndex = -1;
	m_System.ObjectIdentifier = -1;
	m_System.SetName("System");
	m_System.small_image.LoadResource(FindResource(NULL, MAKEINTRESOURCE(IDB_SYSTEMC), RT_BITMAP),0, NULL);
	m_System.m_Image.LoadResource(FindResource(NULL, MAKEINTRESOURCE(IDB_SYSTEM), RT_BITMAP), 0, NULL);
}

CLayout* CApplication::AddLayout()
{
	// Create the Layout and add it to the application
	CLayout* new_layout = new CLayout();
	new_layout->Set(window_information.window_width, window_information.window_height);
	new_layout->SetColor(RGB(255,255,255));
	new_layout->SetOpacity(100.0);
	new_layout->application = this;
	new_layout->identifier = m_frameID++;

	long layout_index = layouts.GetCount();

	CString name_;
	name_.Format("%s %d", LAYOUT, layout_index + 1);
	name_ = GetUniqueLayoutName(name_);
	new_layout->m_WndName = name_;
	new_layout->SetName(name_);

	if (runtime == CApplication::rt_application)
		new_layout->application_background = true;

	// Add event sheet
	EventSheet* event_sheet = new EventSheet(this);

	event_sheet->identifier = event_sheet_unique_identifier++,		
	name_ += " events";
	event_sheet->SetName(GetUniqueEventSheetName(name_));

	event_sheets.push_back(event_sheet);
	new_layout->event_sheet = event_sheets.back();

	// Add layer
	CString layer_name;
	layer_name.Format("%s 1", LAYER);
	CLayer* new_layer = new CLayer(layer_name, LAYER_NORMAL);
	new_layer->m_layerID = m_layerID++;
	new_layout->layers.AddTail(new_layer);

	layer_name.Format("Non-layout");
	CLayer* nonlayout_layer = new CLayer(layer_name, LAYER_NONFRAME);
	nonlayout_layer->m_state = LAYSTATE_HIDDEN;

	nonlayout_layer->m_layerID = m_layerID++;
	new_layout->layers.AddHead(nonlayout_layer);

	new_layout->current_layer = new_layer;

	ChangeModifiedStatus(true);

	layouts.AddTail(new_layout);

	// Project bar
	g_MainFrame->project_bar.UpdateApplication(this);

	return new_layout;
}

void CApplication::RemoveLayout(CLayout* index)
{
	POSITION pos = layouts.GetHeadPosition();
	POSITION pos2 = 0;

	CLayout* pLayout;
	CLayout* found;

	while(pos)
	{
		POSITION pos3 = pos;
		pLayout = layouts.GetNext(pos);

		if(pLayout->identifier == index->identifier)
		{
			found = pLayout;

			pos2 = pos3;
		}
	}

	HTREEITEM layout_tree;

	if(pos2)
	{
		// prompt to remove event sheet
		int identifier = index->event_sheet->identifier;
		index->event_sheet = NULL;
		RemoveEventSheet(identifier, true);

		CLayout* pLayout = layouts.GetAt(pos2);
		layout_tree = pLayout->tree;

		delete pLayout;
		layouts.RemoveAt(pos2);

		GetVectorOfEmptyObjectTypes counter(this);
		ForEachObjectType(this, counter);

		for(list<CObjType*>::iterator i = counter.objectTypesToRemove.begin(); i!= counter.objectTypesToRemove.end(); i++)
		{
			// Find any containers using this OID, and remove the type from the container
			list<CContainer>::iterator c = m_Containers.begin();

			for ( ; c != m_Containers.end(); ) {

				// See if this ID exists in the container
				vector<int>::iterator t = find(c->m_IDs.begin(), c->m_IDs.end(), (*i)->ObjectIdentifier);

				bool increment_c = true;

				if (t != c->m_IDs.end()) {
					c->m_IDs.erase(t);		// Delete from container types

					// m_IDs is now empty: delete the container
					if (c->m_IDs.empty()) {
						c = m_Containers.erase(c);
						increment_c = false;
					}
					// m_IDs now is a container of one object: this is redundant: erase the container
					else if (c->m_IDs.size() == 1) {
						long lastObjectID = c->m_IDs.front();
						CObjType* pLastType = object_types[lastObjectID];
						pLastType->m_pContainer = NULL;		// remove pointer to container, now containerless

						// Erase container
						c = m_Containers.erase(c);
						increment_c = false;
					}
				}

				if (increment_c)
					c++;
			}

			// Delete the object type from the application
			POSITION Pos = object_types.GetStartPosition();
			CObjType* oT;
			long nKey = 0;

			while (Pos != NULL) 
			{	
				object_types.GetNextAssoc(Pos, nKey, oT);
				if(oT == *i)
				{
					object_types.RemoveKey(nKey);
					m_DeletedObjectTypes.SetAt(nKey, oT);
					Pos = NULL;
				}
			}
		}
	}

	// Changed
	ChangeModifiedStatus(true);

	// project bar
	g_MainFrame->project_bar.UpdateApplication(this);
}

void CApplication::RemoveEventSheet(int identifier, bool prompt)
{
	POSITION position = layouts.GetHeadPosition();
	CLayout* layout;

	int used = 0;

	while(position)
	{
		layout = layouts.GetNext(position);

		if (layout->event_sheet)
			if (layout->event_sheet->identifier == identifier)
				used++;
	}

	if (used == 0)
	{
		// safe to remove
		list<EventSheet*>::iterator sheet;

		for(list<EventSheet*>::iterator i = event_sheets.begin(); i != event_sheets.end(); i++)
		{	
			if ((*i)->identifier == identifier)
			{
				sheet = i;

				break;
			}
		}

		int ID = IDYES;

		if (prompt)
			ID = MessageBox(g_MainFrame->m_hWnd, CONF_REMOVEEVENTSHEET, CONF_REMOVEEVENTSHEETTITLE, MB_YESNO);

		if (ID == IDYES)
		{
			// remove from project tree
			if ((*sheet)->second_tree)
				g_MainFrame->project_bar.tree.DeleteItem((*sheet)->second_tree);

			event_sheets.erase(sheet);
		}
	}

	else
		if (prompt)
			MessageBox(g_MainFrame->m_hWnd, "This event sheet cannot be removed as it is linked to a layout.", "Cannot remove event sheet", NULL);
}

void CApplication::RemoveObjectFolder(CString name)
{
	if (name == "Default")
	{
		MessageBox(g_MainFrame->m_hWnd, "This object folder cannot be removed as it is the default object folder.", "Cannot remove object folder", NULL);
		return;
	}

	// change all objects to default
	vector<CObjType*> types;

	GetTypesInFolder(this, name, &types);

	for (int i = 0; i < types.size(); i++)
		types[i]->SetFolder("Default");

	vector<ObjectFolder>::iterator folder = object_folders.begin();

	for ( ; folder != object_folders.end(); folder++)
	{
		if (folder->name == name)
		{
			g_MainFrame->project_bar.tree.DeleteItem(folder->tree);
			object_folders.erase(folder);

			return;
		}
	}

	ChangeModifiedStatus();
}

void CApplication::RenameObjectFolder(CString old_folder, CString new_folder)
{
	vector<ObjectFolder>::iterator folder = object_folders.begin();

	for ( ; folder != object_folders.end(); folder++)
	{
		if (folder->name == old_folder)
		{
			folder->name = new_folder;
	
			break;
		}
	}

	// change all objects to default
	vector<CObjType*> types;

	GetTypesInFolder(this, old_folder, &types);

	for (int i = 0; i < types.size(); i++)
		types[i]->SetFolder(new_folder);

	ChangeModifiedStatus();
}

bool CApplication::ObjectTypeNameExists(CString name)
{
	CObjType* type;
	long key;

	POSITION position = object_types.GetStartPosition();

	while (position != NULL)
	{
		object_types.GetNextAssoc(position, key, type);

		CString compare = type->GetName();

		if (compare.CompareNoCase(name) == 0)
			return true;
	}

	return false;
}

CString CApplication::GetUniqueObjectTypeName(CString& name)
{
	long ID = 0;
	long num = 1;

	CString finalName;
	bool nextChUpper = false;
	
	// Remove spaces, but make the character after a space capital.
	for (int i = 0; i < name.GetLength(); i++) {
		if (name[i] == ' ') {
			nextChUpper = true;
		}
		else {
			if (nextChUpper) {
				CString ch = name[i];
				ch.MakeUpper();
				finalName += ch;
				nextChUpper = false;
			}
			else
				finalName += name[i];
		}
	}

	name = finalName;
	name.Replace("&", "");

	CObjType *oT;
	CObjTypeMap* objTypeMap = &object_types;
	long nKey;

	POSITION pos = objTypeMap->GetStartPosition();

	while (pos != NULL)
	{
		// iterate through objects to choose one to select
		objTypeMap->GetNextAssoc(pos, nKey, oT);

		CString tempName = oT->GetName();
		if (num == 1)
			finalName.Format("%s", name);
		else
			finalName.Format("%s%d", name, num);

		if (tempName.CompareNoCase(finalName) == 0)
		{
			num++;
			pos = objTypeMap->GetStartPosition();
		}
	}
	if (num == 1)
		finalName.Format("%s", name);
	else
		finalName.Format("%s%d", name, num);
	return finalName;
}

void CApplication::AddGlobalVariable(GlobalVariable& variable)
{
	global_variables.push_back(variable);

	g_MainFrame->project_bar.UpdateApplication(this);
}

bool CApplication::GlobalVariable::Serialize(CArchive& ar)
{
	if (ar.IsLoading()) 
		ar >> name >> identifier >> type >> value;
	else 
		ar << name << identifier << type << value;

	return true;
}

void CApplication::ChangeModifiedStatus(bool modified_)
{
	file_information.modified = modified_;

	g_MainFrame->tabs.Invalidate();
}

bool CApplication::GetModifiedStatus()
{
	bool changed = file_information.modified;

	// Check layouts
	CLayout *pLayout;
	POSITION pos = layouts.GetHeadPosition();

	for (int i = 0; i < layouts.GetCount(); i++) 
	{ 
		// Iterate through frames and serialize all
		pLayout = layouts.GetNext(pos);
		if (pLayout->GetChanged())
			changed = true;
	}

	// Check event sheets
	list<EventSheet*>::iterator es = event_sheets.begin();
	for ( ; es != event_sheets.end(); es++)
		if ((*es)->GetChanged())
			changed = true;

	return changed;
}

bool CApplication::Serialize(CArchive& ar) 
{
	CString ExpectedName = "CApplication";
	int     Version      = 17;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	else if(ar.IsLoading())
	{
		CProgressDlg ProgressDlg(verifying ? "Verifying" : "Loading application");
		ProgressDlg.Start();
		ProgressDlg.SetProgress(0);

		int temp;

		bool unused = false;
		CString unused_string;

		ar >> file_information.name;
		ar >> file_information.creator;
		ar >> unused_string;
		ar >> m_objID >> m_appID >> m_layerID >> m_ImageFixID >> m_AnimationFixID;
		ar >> event_sheet_unique_identifier >> m_frameID;

		ar >> file_information.file_path >> (int&)game_information.sampler;
		
		ar >> window_information.window_width;
		ar >> window_information.window_height;
		
		ar >> game_information.run_without_ps;

		ar >> window_information.disable_windows_key;

		ar >> (int&)game_information.fps_mode >> game_information.fps;

		ar >> game_information.fullscreen;
		ar >> window_information.show_menu;

		ar >> unused;
		
		ar >> window_information.hide_maximize;
		ar >> window_information.hide_minimize;
		
		ar >> unused >> temp >> unused
		   >> unused >> unused;
		
		ar >> game_information.fps_in_caption;
		
		BOOL unused_bool = 0;

		ar >> unused_bool;
		ar >> game_information.use_python >> reserved4;

		ar >> m_varID;


		ar >> (int&)runtime;
		ar >> (int&)game_information.simulate_shaders;

		if (Version >= 4)
			ar >> game_information.use_motion_blur >> game_information.motion_blur_steps;

		if (Version >= 6)
			ar >> (int&)game_information.text_rendering_mode;

		// V7: timedelta overrides
		if (Version >= 7)
			ar >> game_information.override_timedelta >> game_information.time_delta_override;

		// V8: CONSTRUCT_VERSION_ID saved
		if (Version >= 8) {
			ar >> file_information.created_with;

			//if (file_information.created_with > CONSTRUCT_VERSION_ID)
			//	throw runtime_error("This file was saved in a newer version of Construct and cannot be loaded by this version.  Please visit www.scirra.com to look for the latest update, or click 'Check for updates' on the ribbon bar.");
		}

		// load plugins used
		if (Version >= 9) 
		{
			int plugins_used;
			ar >> plugins_used;

			CString missing_plugins;
			bool is_missing_plugins = false;

			for (int i = 0; i < plugins_used; i++)
			{
				CString name, filename;

				ar >> name >> filename;

				// Find this plugin
				CPluginIterator p = g_Plugins.begin();

				bool found = false;

				for ( ; p != g_Plugins.end(); p++)
				{
					CString first = filename;
					CString second = p->second.m_FileName;

					first.MakeLower();
					second.MakeLower();

					if (first == second)
					{
						found = true;
						break;
					}
				}//for

				// Plugin wasn't found
				if (!found) 
				{
					is_missing_plugins = true;

					CString first = filename;
					first.MakeLower();

					if (first == "directsound.csx")
						missing_plugins += name + " (" + filename + ") - obsolete and removed in 1.0 - use XAudio2 instead\n";
					else
						missing_plugins += name + " (" + filename + ")\n";
				}
			}

			// plugins are missing
			if (is_missing_plugins)
			{
				CString msg;
				msg.Format("The following plugins could not be found.\n\n%s\nPlease install these plugins and try again.  You may be able to find the plugins at www.scirra.com.",
					missing_plugins);
				throw runtime_error((const char*)msg);
			}
		}

		//V10: EXTRA WINDOW INFO STUFF
		if (Version >= 10) {
			ar >> window_information.caption
			   >> window_information.minimize_box
			   >> temp;
			window_information.resizable = (WindowInformation::ResizeSetting)temp;
		}
		if (Version >= 11)
			ar >> window_information.maximize_box;
		// V12: min fps
		if (Version >= 12)
			ar >> game_information.minimum_fps;

		if (Version >= 15)
			ar >> game_information.multisamples;

		// V17: VRAM controls
		if (Version >= 17) {
			ar >> temp;
			game_information.texture_loading = (GameInformation::TextureLoading)temp;
		}
		else
			// Maintain backwards compatibility for old .caps: load all on start like before, but default for new caps is
			// load on start of each layout
			game_information.texture_loading = GameInformation::tl_load_on_app_start;

		resources.images.clear();
		resources.images_changed = true;

		layouts.RemoveAll();
		long pLayoutCnt;
		ar >> pLayoutCnt;

		CLayout *pLayout;
		int i;
		for (i = 0; i < pLayoutCnt; i++) 
		{
			pLayout = new CLayout();
			pLayout->application = this;

			if (!pLayout->Serialize(ar))
				return false;

			layouts.AddTail(pLayout);
		}

		ProgressDlg.SetProgress(5);

		// Load animations
		int Count = 0;
		ar >> Count;

		for (i = 0; i < Count; i++)
		{
			CAnimation Animation;
			Animation.application = this;
			
			if (!Animation.Serialize(ar))
				return false;

			resources.animations.push_back(Animation);
		}

		ProgressDlg.SetProgress(10);

		// image bank
		long imgCnt;
		ar >> imgCnt;

		// Loading image bank takes a long time, use 10-70%
		for(int img = 0; img < imgCnt; img ++)
		{
			resources.images.push_back(CImageResource());
			CImageResource& intimg = resources.images.back();
			
			if (!intimg.Serialize(ar))
				return false;

			ProgressDlg.SetProgress(10 + (60 * img) / imgCnt);
		}

		// load binary resources, old style
		if (Version <= 13)
		{
			Count = 0;
			ar >> Count;

			for (i = 0; i < Count; i++)
			{
				BinaryResource resource;
				
				if (!resource.Serialize(ar))
					return false;

				resource.id = resources.resource_id++;

				// music
				if (resource.path.Right(4) == ".mp3" || resource.path.Right(4) == ".wma" || resource.path.Right(4) == ".ogg")
					resources.music_resources.push_back(resource);

				// sound effects
				else if (resource.path.Right(4) == ".wav")
					resources.sound_resources.push_back(resource);

				else
					resources.file_resources.push_back(resource);

				// Use 70-80%
				ProgressDlg.SetProgress(70 + (10 * i) / Count);
			}
		}

		else
		{
			// now we have files, sounds and music
			int files = 0;
			ar >> files;

			for (int i = 0; i < files; i++)
			{
				resources.file_resources.push_back(BinaryResource());
				BinaryResource& resource = resources.file_resources.back();
				
				if (!resource.Serialize(ar))
					return false;
			}

			int sounds = 0;
			ar >> sounds;

			for (int i = 0; i < sounds; i++)
			{
				resources.sound_resources.push_back(BinaryResource());
				BinaryResource& resource = resources.sound_resources.back();
				
				if (!resource.Serialize(ar))
					return false;
			}

			int music = 0;
			ar >> music;

			for (int i = 0; i < music; i++)
			{
				resources.music_resources.push_back(BinaryResource());
				BinaryResource& resource = resources.music_resources.back();
				
				if (!resource.Serialize(ar))
					return false;
			}

			ProgressDlg.SetProgress(80);
		}

		// Load icons
		Count = 0;
		ar >> Count;

		for (i = 0; i < Count; i++)
		{
			IconResource Resource;

			if (!Resource.Serialize(ar))
				return false;

			resources.icon_resources.push_back(Resource);
		}

		// Load menus
		Count = 0;
		ar >> Count;

		for (i = 0; i < Count; i++)
		{
			MenuResource Resource;

			if (!Resource.Serialize(ar))
				return false;

			resources.menu_resources.push_back(Resource);
		}

		ProgressDlg.SetProgress(85);
	
		///////////////////////////////////////////////////
		// OBJECT TYPES
		int objTypeCnt = 0;
		ar >> objTypeCnt;
		CObjType *oT;

		for (i = 0; i < objTypeCnt; i++)
		{
			long nKey;
			ar >> nKey;
			oT = new CObjType();
			
			if (!oT->Serialize(ar))
				return false;

			object_types.SetAt(nKey, oT);
		}

		// Load traits
		ar >> Count;
		traits.clear();

		for (i = 0; i < Count; i++)
		{
			CString traitName;
			ar >> traitName;
			traits.push_back(traitName);
		}

		// Load families
		ar >> Count;
		families.clear();

		for (i = 0; i < Count; i++)
		{
			families.push_back(Family());
			Family& Family = families.back();
			Family.application = this;
			Family.is_in_use = true;
			
			if (!Family.Serialize(ar))
				return false;
			
			// Backup incase we have empty families - now changed so family manager can remove family
		/*	vector<CObjType*> Objects;
			Family.GetObjectTypes(Objects);
			if(Objects.size() == 0)
				Family.is_in_use = false;*/



		}

		ar >> Count;
		global_variables.clear();

		for (i = 0; i < Count; i++)
		{
			CApplication::GlobalVariable g;
			
			if (!g.Serialize(ar))
				return false;

			global_variables.push_back(g);
		}

		// Load containers
		ar >> Count;
		m_Containers.clear();

		for (i = 0; i < Count; i++)
		{
			m_Containers.push_back(CContainer());
			CContainer& Container = m_Containers.back();

			Container.application = this;
			
			if (!Container.Serialize(ar))
				return false;
		}

		ProgressDlg.SetProgress(90);

		// System
		if (!m_System.Serialize(ar))
			return false;

		// Load transitions
		m_Transitions.resize(0);
		ar >> Count;

		for (i = 0; i < Count; i++) 
		{
			m_Transitions.push_back(CTransition());
			CTransition& tr = m_Transitions.back();

			if (!tr.Serialize(ar))
				return false;
		}

		// Load Events
		Count = 0;
		ar >> Count;
		event_sheets.clear();

		for (i = 0; i < Count; i++)
		{
			EventSheet* sheet = new EventSheet(this);
			if (!sheet->Serialize(ar))
				return false;

			event_sheets.push_back(sheet);
		}

		// set event sheets to layouts
		POSITION pos = layouts.GetHeadPosition();

		for (int i = 0; i < layouts.GetCount(); i++)
		{ 
			// Iterate through frames and serialize all
			pLayout = layouts.GetNext(pos);
			pLayout->event_sheet = FindEventSheetFromNumber(pLayout->temporary_event_sheet_id);
		}

		ProgressDlg.SetProgress(95);

		// Load controls
		ar >> Count;
		m_Controls.resize(0);

		for (i = 0; i < Count; i++) 
		{
			m_Controls.push_back(BehaviorControl());
			BehaviorControl& control = m_Controls.back();

			ar >> control.name >> control.comboIndex >> control.vk;

			// V16: player controller
			if (Version >= 16)
				ar >> control.player;
		}

		int Tree_;
		ar >> Tree_;

		// KeyCollection changed for v5
		if (Version == 5)
			globalKeys.Serialize(ar, true);
		else
			globalKeys.Serialize(ar, false);

		// Version 3 and onwards has python settings
		if(Version >= 3)
		{
			int size;
			ar >> size;
			CString str;
			for(int a = 0; a < size; a++)
			{
				ar >> str;
				m_PythonDistrib[str] = 0;
			}
		}

		if (Version >= 13)
		{
			int count;
			ar >> count;

			for (int i = 0; i < count; i++)
			{
				CApplication::ObjectFolder folder;
				ar >> folder.name;

				object_folders.push_back(folder);
			}
		}

		if (object_folders.size() == 0)
		{
			// add default object folder
			CApplication::ObjectFolder folder;
			folder.name = "Default";

			object_folders.push_back(folder);
		}

		ProgressDlg.SetProgress(100);
		ProgressDlg.Finish();
	}

	// Saving
	else
	{
		CProgressDlg ProgressDlg("Saving");
		ProgressDlg.Start();
		ProgressDlg.SetProgress(0);
		int progindex = 0;

		ar << file_information.name;
		ar << file_information.creator;
		ar << file_information.creator;
		ar << m_objID << m_appID << m_layerID << m_ImageFixID << m_AnimationFixID;
		ar << event_sheet_unique_identifier << m_frameID;

		ar << file_information.file_path << (int)game_information.sampler;
		
		ar << window_information.window_width;
		ar << window_information.window_height;
		
		ar << game_information.run_without_ps;

		ar << window_information.disable_windows_key;

		ar << (int)game_information.fps_mode << game_information.fps;

		ar << game_information.fullscreen;
		ar << window_information.show_menu;

		ar << (bool)false;
		
		ar << window_information.hide_maximize;
		ar << window_information.hide_minimize;
		
		ar << (bool)false <<(int)0 << (bool)false
		   << (bool)false << (bool)false;
		
		ar << game_information.fps_in_caption;
		
		ar << (BOOL)FALSE;
		ar << game_information.use_python << reserved4;

		ar << m_varID;


		ar << (int)runtime;
		ar << (int)game_information.simulate_shaders;

		ar << game_information.use_motion_blur << game_information.motion_blur_steps; // 4

		ar << (int)game_information.text_rendering_mode; // 6 

		ar << game_information.override_timedelta << game_information.time_delta_override; // 7

		ar << file_information.created_with; // 8

		// Version 9 /////
		// Save a list of all the used plugins in this application so it can be checked on startup
		CObjType* oT;

		POSITION pos = object_types.GetStartPosition();
		long mapKey;

		vector<int> DLLIndices;

		// Loop through and add the objects
		while (pos != NULL) 
		{
			object_types.GetNextAssoc(pos, mapKey, oT);

			if (!exists_element(DLLIndices.begin(), DLLIndices.end(), oT->DLLIndex))
				DLLIndices.push_back(oT->DLLIndex);
		}

		ar << DLLIndices.size();

		vector<int>::iterator p = DLLIndices.begin();

		for ( ; p != DLLIndices.end(); p++) {
			const CPlugin& plugin = GetPlugin(*p);

			// Save name & filename
			ar << plugin.m_Name << plugin.m_FileName;
		}
		///////////////////////
		// Version 10: extra window info stuff
		//V10: EXTRA WINDOW INFO STUFF
		ar << window_information.caption
		   << window_information.minimize_box
		   << (int)window_information.resizable
		   << window_information.maximize_box;
		// v12: min fps
		ar << game_information.minimum_fps;

		// v15: multisamples
		ar << game_information.multisamples;

		// V17: VRAM controls
		ar << (int)game_information.texture_loading;

		images_to_save.clear();
		animations_to_save.clear();
		
		// pLayouts
		ar << layouts.GetCount();
			
		CLayout *pLayout;
		pos = layouts.GetHeadPosition();

		for (int i = 0; i < layouts.GetCount(); i++)
		{ 
			// Iterate through frames and serialize all
			pLayout = layouts.GetNext(pos);
			pLayout->Serialize(ar);
		}

		ProgressDlg.SetProgress(5);

		// Animations
		ar << animations_to_save.size();
		for(map<int, CObj*>::iterator i = animations_to_save.begin(); i!= animations_to_save.end(); i++)
		{
			CAnimation* anim = resources.FindAnimationFromNumber(i->first);	
			anim->Serialize(ar);
		}

		ProgressDlg.SetProgress(10);

		// image bank
		ar << images_to_save.size();
	
		progindex = 0;

		for(map<int, CObj*>::iterator i = images_to_save.begin(); i!= images_to_save.end(); i++, progindex++)
		{
			CImageResource* img = resources.FindImageResourceFromNumber(i->first);	
			img->Serialize(ar);

			// 10-70%
			ProgressDlg.SetProgress(10 + (60 * progindex) / images_to_save.size());
		}

		// Save binary files
		ar << (int)resources.file_resources.size();

		progindex = 0;
		for (list<BinaryResource>::iterator i = resources.file_resources.begin(); i!= resources.file_resources.end(); i++) 
			i->Serialize(ar);

		// new for 0.99
		ar << (int)resources.sound_resources.size();

		for (list<BinaryResource>::iterator i = resources.sound_resources.begin(); i!= resources.sound_resources.end(); i++) 
			i->Serialize(ar);

		ar << (int)resources.music_resources.size();

		for (list<BinaryResource>::iterator i = resources.music_resources.begin(); i!= resources.music_resources.end(); i++) 
			i->Serialize(ar);

		ProgressDlg.SetProgress(80);

		// Save icons
		ar << resources.icon_resources.size();

		for (list<IconResource>::iterator i = resources.icon_resources.begin(); i!= resources.icon_resources.end(); i++)
			i->Serialize(ar);

		// Save menus
		ar << resources.menu_resources.size();

		for (list<MenuResource>::iterator i = resources.menu_resources.begin(); i!= resources.menu_resources.end(); i++)
			i->Serialize(ar);

		ProgressDlg.SetProgress(85);

		// Object Types
		ar << object_types.GetCount();			

		pos = object_types.GetStartPosition();
		long nKey;
		for (int i = 0; i < object_types.GetCount(); i++)
		{
			// iterate through objects to choose one to select
			object_types.GetNextAssoc(pos, nKey, oT);
			ar << nKey;
			oT->Serialize(ar);
		}

		// Save traits
		ar << traits.size();

		for (int i = 0; i < traits.size(); i++)
			ar << traits[i];

		// Save families
		int familySize = 0;
		for (list<Family>::iterator f = families.begin(); f != families.end(); f++)
			if(f->is_in_use)
				familySize ++;

		ar << familySize;

		for (list<Family>::iterator f = families.begin(); f != families.end(); f++)
			if(f->is_in_use)
				f->Serialize(ar);

		// Save globals
		ar << global_variables.size();

		for (list<CApplication::GlobalVariable>::iterator i = global_variables.begin(); i != global_variables.end(); i++)
			i->Serialize(ar);

		// Save containers
		ar << m_Containers.size();

		list<CContainer>::iterator i = m_Containers.begin();
		for ( ; i != m_Containers.end(); i++)
			i->Serialize(ar);

		ProgressDlg.SetProgress(90);

		// System
		m_System.Serialize(ar);
		
		// Save transitions
		ar << m_Transitions.size();

		list<CTransition>::iterator tr = m_Transitions.begin();
		for ( ; tr != m_Transitions.end(); tr++)
			tr->Serialize(ar);

		ProgressDlg.SetProgress(95);

		// Save event sheet
		list<EventSheet*>::iterator es = event_sheets.begin();
		ar << event_sheets.size();
		for ( ; es != event_sheets.end(); es++)
			(*es)->Serialize(ar);

		// Save controls
		vector<BehaviorControl>::iterator mc = m_Controls.begin();

		ar << m_Controls.size();
		for ( ; mc != m_Controls.end(); mc++)
			ar << mc->name << mc->comboIndex << mc->vk << mc->player;

		ar << (int)0;

		globalKeys.Serialize(ar, true);

		int size = m_PythonDistrib.size();
		ar << size;
		CString str;
		for(map<CString, int>::iterator i = m_PythonDistrib.begin(); i!= m_PythonDistrib.end(); i++)
		{
			ar << i->first;
		}

		// save object folders
		ar << (int)object_folders.size();
		for (int i = 0; i < object_folders.size(); i++)
			ar << object_folders[i].name;

		ProgressDlg.SetProgress(100);
		ProgressDlg.Finish();
	}

	SerializeClassPadding(30, 20, ar);

	return true;
}


void CApplication::DeleteContainer(CContainer* ctnr)
{
	list<CContainer>::iterator i = m_Containers.begin();

	for ( ; i != m_Containers.end(); i++) {
		// Match address
		if (&(*i) == ctnr) {

			// Reset all object type's container pointers to NULL
			vector<int>::iterator j = ctnr->m_IDs.begin();

			for ( ; j != ctnr->m_IDs.end(); j++) {
				long id = *j;
				CObjType* pType;
				object_types.Lookup(id, pType);

				pType->m_pContainer = NULL;
			}

			// Erase this container
			m_Containers.erase(i);

			return;
		}
	}
}


CObjType* CApplication::FindObjTypeFromNumber(int oid)
{
	Family* it = FindFamilyFromNumber(oid);
	if(it)
	{
		it->UpdateFamilyData();

		vector<CObjType*> objects;
		it->GetObjectTypes(objects);
		
		if(objects.size() == 0)
			return 0;
		
		CObjType* pType = &it->m_FamilyTemp;

		//CHECKME
		pType->tree = NULL;

		pType->m_Image = it->image;
		pType->small_image = it->small_image;

		
		pType->SetName(it->name);
		pType->ObjectIdentifier = it->identifier;
		pType->DLLIndex  = it->DLLIndex;

		pType->behaviors.clear();
		pType->m_PrivateValues.clear();
		pType->effects.clear();

		for(int m = 0; m < it->behaviors.size(); m++)
		{
			FamilyBehavior* pFamMov = &it->behaviors.at(m);
			if(!pFamMov->valid)
				continue;
		
			pType->behaviors.push_back(CBehavior(*it->GetReferenceBehavior(m)));
			CBehavior* pMov = &pType->behaviors.back();
			pMov->id = pFamMov->unique_id;
		}

		for(int e = 0; e < it->effects.size(); e++)
		{
			FamilyEffect* pFamEff = &it->effects.at(e);
			if(!pFamEff->valid)
				continue;
			pType->effects.push_back(CEffect());
			CEffect* pEff = &pType->effects.back();
			*pEff = *it->GetReferenceEffect(e);
			pEff->m_effectID = pFamEff->unique_id;

			// Families don't use their run effect - if this is not NULL it gets double released
			pEff->runEffect.pEffect = NULL;
		}
		for(int v = 0; v < it->variables.size(); v++)
		{
			FamilyVariable* pFamVar = &it->variables.at(v);
			if(!pFamVar->valid)
				continue;
			pType->m_PrivateValues.push_back(PrivateValue());
			PrivateValue* pVar = &pType->m_PrivateValues.back();
			*pVar = *it->GetReferenceVariable(v);
			pVar->identifier = pFamVar->unique_id;
		}
		
	
		// At this point, we would loop through and add movements and effects and variables etc. However, 
		// thats a TODO (remember to clear them though, as its tempory and could still have info stored)



		return &it->m_FamilyTemp;
		
	}
	else
	{
		// STEP 1: Work out the object type
		CObjType* objectType = 0;
		int objid = oid;

		if(objid == -1)
			objectType = &m_System;
		else
		{
			CObjTypeMap* objectMap = &object_types;
			
			long objectKey;
			POSITION startPosition = objectMap->GetStartPosition();
			while (startPosition != NULL) 
			{
				objectMap->GetNextAssoc(startPosition, objectKey, objectType);

				if (objectType->ObjectIdentifier == oid)
				{
					startPosition = objectMap->GetStartPosition();
					break;
				}
			}
			if(startPosition == NULL)
				return 0; // error...object u selected wasn't found...should throw something
		}


		return objectType;
	}
}

CObjType* CApplication::FindObjTypeFromDetails(ObjectTypeDetail& details)
{
	/*Family* it = FindFamilyFromNumber(oid);
	if(it)
	{
		it->UpdateFamilyData();

		vector<CObjType*> objects;
		it->GetObjectTypes(objects);
		
		if(objects.size() == 0)
			return 0;
		
		CObjType* pType = &it->m_FamilyTemp;

		//CHECKME
		pType->tree = NULL;

		pType->m_Image = it->image;
		pType->small_image = it->small_image;

		
		pType->SetName(it->name);
		pType->ObjectIdentifier = it->identifier;
		pType->DLLIndex  = it->DLLIndex;

		pType->behaviors.clear();
		pType->m_PrivateValues.clear();
		pType->effects.clear();

		for(int m = 0; m < it->behaviors.size(); m++)
		{
			FamilyBehavior* pFamMov = &it->behaviors.at(m);
			if(!pFamMov->valid)
				continue;
		
			pType->behaviors.push_back(CBehavior(*it->GetReferenceBehavior(m)));
			CBehavior* pMov = &pType->behaviors.back();
			pMov->id = pFamMov->unique_id;
		}

		for(int e = 0; e < it->effects.size(); e++)
		{
			FamilyEffect* pFamEff = &it->effects.at(e);
			if(!pFamEff->valid)
				continue;
			pType->effects.push_back(CEffect());
			CEffect* pEff = &pType->effects.back();
			*pEff = *it->GetReferenceEffect(e);
			pEff->m_effectID = pFamEff->unique_id;

			// Families don't use their run effect - if this is not NULL it gets double released
			pEff->runEffect.pEffect = NULL;
		}
		for(int v = 0; v < it->variables.size(); v++)
		{
			FamilyVariable* pFamVar = &it->variables.at(v);
			if(!pFamVar->valid)
				continue;
			pType->m_PrivateValues.push_back(PrivateValue());
			PrivateValue* pVar = &pType->m_PrivateValues.back();
			*pVar = *it->GetReferenceVariable(v);
			pVar->identifier = pFamVar->unique_id;
		}
		
	
		// At this point, we would loop through and add movements and effects and variables etc. However, 
		// thats a TODO (remember to clear them though, as its tempory and could still have info stored)



		return &it->m_FamilyTemp;
		
	}
	else
	{
		// STEP 1: Work out the object type
		CObjType* objectType = 0;
		int objid = oid;

		if(objid == -1)
			objectType = &m_System;
		else
		{
			CObjTypeMap* objectMap = &object_types;
			
			long objectKey;
			POSITION startPosition = objectMap->GetStartPosition();
			while (startPosition != NULL) 
			{
				objectMap->GetNextAssoc(startPosition, objectKey, objectType);

				if (objectType->ObjectIdentifier == oid)
				{
					startPosition = objectMap->GetStartPosition();
					break;
				}
			}
			if(startPosition == NULL)
				return 0; // error...object u selected wasn't found...should throw something
		}


		return objectType;
	}*/
return 0;
}

// Returns true if object was added to family
bool CApplication::AddObjectTypeToFamily(CObjType* pType, Family* pFamily)
{
	vector<CObjType*> objects;
	pFamily->GetObjectTypes(objects);

	// Type is already in family: exit
	if (exists_element(objects.begin(), objects.end(), pType))
		return false;

	// Family is already in this type: exit
	if (exists_element(pType->families.begin(), pType->families.end(), pFamily))
		return false;

	// Check pType's plugin type is not different to the family's (if empty it definitely can be added!)
	if (!objects.empty()) {
		int familyDLLIndex = objects.front()->DLLIndex;
		int typeDLLIndex = pType->DLLIndex;

		if (typeDLLIndex != familyDLLIndex) {
			const CPlugin& typePlugin = GetPlugin(typeDLLIndex);
			const CPlugin& familyPlugin = GetPlugin(familyDLLIndex);

			CString msg;
			msg.Format("This object cannot be added to the family.  The family '%s' is a family of '%s' objects.  The object '%s' is a '%s' object.  Families have to be all the same kind of object.",
				pFamily->name, familyPlugin.m_Name, pType->GetName(), typePlugin.m_Name);
			MessageBox(NULL, msg, "Add object to family", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
	}

	// Check family private variables are not going to be nuked by adding this object to the family
	pFamily->UpdateFamilyData();
	bool missingVariables = false;

	vector<FamilyVariable>::iterator i = pFamily->variables.begin();

	for ( ; i != pFamily->variables.end(); i++) {
		if (i->valid && !pType->PrivateVariableExists(i->name)) {
			missingVariables = true;
			break;
		}
	}

	// This object is missing some of the family variables
	if (missingVariables) {
		CAddFamilyVarsDlg Dlg;
		Dlg.pType = pType;
		Dlg.pFamily = pFamily;

		if (Dlg.DoModal() == IDOK) {

			// Add the variables in varsToAdd to this object type
			vector<CString>::iterator v = Dlg.varsToAdd.begin();

			for ( ; v != Dlg.varsToAdd.end(); v++) {

				// Find the type of this variable by grabbing it from the first type
				int typeID = PRIVATEVARTYPE_NUMBER;

				vector<CObjType*> familyTypes;
				pFamily->GetObjectTypes(familyTypes);

				if (!familyTypes.empty()) {
					CObjType* pType = familyTypes.front();
					PrivateValue* pv = pType->GetPrivateVariable(*v);

					if (pv != NULL)
						typeID = pv->type;
				}

				// Add the new variable to the object type
				int id = pType->VariableID++;
				pType->m_PrivateValues.push_back(PrivateValue(*v, typeID, id));

				// Add the new variable to the corresponding instance values
				PrivateVariableInstanceAdder updater(id, typeID == PRIVATEVARTYPE_NUMBER ? "0" : "");
				ForEachObjectInstance(pType, this, updater);	
			}

			pType->families.push_back(pFamily);
			return true;
		}
		// Cancelled: don't add to family
		else
			return false;
	}
	// No variables missing: go ahead and add
	else {
		pType->families.push_back(pFamily);
		return true;
	}
}

CApplication::ObjTypeFamilyStatus CApplication::PromptIfTypeLastInFamily(CObjType* pType, Family* pFamily, bool prompt)
{
	// check if the last
	TypesUsingFamilyCounter counter(pFamily->name);
	ForEachObjectType(this, counter);

	// I am the last one using this family
	if (counter.count == 1)
	{
		if (!prompt) return is_last_type_in_family;

		CString msg;
		msg.Format("This is the last object using the family '%s'. Removing it will remove all actions, conditions and expressions relating to this family. Are you sure you wish to continue?",
				   pFamily->name);

		if (CExtMsgBox::DoMsgBox(NULL, msg, "Removing Family", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			return user_clicked_yes_delete_family;	// Family to be deleted, call DeleteFamily() now
		else
			return user_clicked_no;
	}

	// not last type
	else
		return not_last_type_in_family;
}

void CApplication::DeleteFamily(Family* pFamily)
{
	// Remove all references to the family from the application
	EraseFamilyNameReferences(this, pFamily->name);

	pFamily->is_in_use = false;
}

bool CApplication::EventSheetNameExists(CString name)
{
	name.MakeLower();

	list<EventSheet*>::iterator i = event_sheets.begin();

	for ( ; i != event_sheets.end(); i++) {
		CString curName = (*i)->GetName();
		curName.MakeLower();

		if (name == curName)
			return true;
	}

	return false;
}

bool CApplication::ResourceInformation::ResourceNameExists(list<ApplicationResource>* resources, CString start)
{
	bool exists = false;

	list<ApplicationResource>::iterator resource = resources->begin();

	for ( ; resource != resources->end(); resource++)
		if (resource->GetName() == start)
			exists = true;

	return exists;
}

CString CApplication::ResourceInformation::GetUniqueResourceName(list<ApplicationResource>* resources, CString start)
{
	if (!ResourceNameExists(resources, start)) return start;
	
	CString name = start;
	int count = 2;

	while (ResourceNameExists(resources, name))
	{
		name.Format("%s (%d)", start, count);
		count++;
	}

	return name;
}

CString CApplication::GetUniqueEventSheetName(CString startWithThisName)
{
	if (!EventSheetNameExists(startWithThisName))
		return startWithThisName;

	CString retName = startWithThisName;
	int count = 2;

	while (EventSheetNameExists(retName)) {
		retName.Format("%s (%d)", startWithThisName, count);
		count++;
	}

	return retName;
}

bool CApplication::LayoutNameExists(CString name)
{
	FrameNameTester tester;
	tester.name = name;
	ForEachFrame(this, tester);
	return tester.exists;
}

CString CApplication::GetUniqueLayoutName(CString startWithThisName)
{
	if (!LayoutNameExists(startWithThisName))
		return startWithThisName;

	CString retName = startWithThisName;
	int count = 2;

	while (LayoutNameExists(retName)) {
		retName.Format("%s (%d)", startWithThisName, count);
		count++;
	}

	return retName;
}

void CApplication::GetObjectTypes(vector<CObjType*>& types)
{
	POSITION pos = object_types.GetStartPosition();
	long nKey;
	CObjType* oT;

	while (pos != NULL) {
		object_types.GetNextAssoc(pos, nKey, oT);

		if (!oT->m_bIsGroupType)
			types.push_back(oT);
	}
}
void CApplication::GetFamilyObjectTypes(vector<CObjType*>& types)
{
	CObjType* oT;

	list<Family>::iterator f = families.begin();
	for(;f != families.end(); f++)
	{
		if(f->is_in_use)
		{
			oT = FindObjTypeFromNumber(f->identifier);
			if(oT)
			{
				types.push_back(oT);
			}
		}
	}
}
int CApplication::CloneAnimation(int handle, list<CAnimation>* animlist)
{
	if(!animlist)
		animlist = &resources.animations;

	map<int,int>::iterator find = remapAnimations.find(handle);
	if(find == remapAnimations.end())
	{
		CAnimation* pAnim = resources.FindAnimationFromNumber(handle);
		if(pAnim)
			return CloneAnimation(pAnim, animlist);
		else
			return handle; //couldn't clone it for some reason
	}
	else
		return find->second;
}

int CApplication::CloneAnimation(CAnimation* anim, list<CAnimation>* animlist)
{
	if(!animlist)
		animlist = &resources.animations;

	animlist->push_back(*anim);
	CAnimation* pAnim = &animlist->back();
	pAnim->m_FixedID	= m_AnimationFixID++;
	
	for(int i = 0; i < pAnim->m_Images.size(); i++)
		pAnim->m_Images[i] = CloneImage(pAnim->m_Images[i]);

	pAnim->m_SubAnimations.clear();
	for(list<CAnimation>::iterator a = anim->m_SubAnimations.begin(); a != anim->m_SubAnimations.end(); a++)
		CloneAnimation(&*a, &pAnim->m_SubAnimations); // this function will add sub anims

	return pAnim->m_FixedID;
}

int CApplication::CloneImage(int handle)
{
	map<int,int>::iterator find = remapImages.find(handle);
	if(find == remapImages.end())
	{
		CImageResource* pImgRes = resources.FindImageResourceFromNumber(handle);
		if(pImgRes)
			return CloneImage(pImgRes);
		else
			return handle; //couldn't clone it for some reason
	}
	else
		return find->second;
}
int CApplication::CloneImage(CImageResource* img)
{
	resources.images.push_back(*img);
	CImageResource* pImgRes = &resources.images.back();
	pImgRes->m_FixedID = m_ImageFixID++;
	
	return pImgRes->m_FixedID;
}

void ObjectTypeDetail::CopyDetails(CObjType* type)
{
	info.name = type->GetName();
	info.dll = type->DLLIndex;
	id = type->ObjectIdentifier;

	for (list<CBehavior>::iterator b = type->behaviors.begin(); b != type->behaviors.end(); b++)
		behaviors.push_back(NameDllInfo(b->Text, b->BehaviorDLLIndex));

	for (list<CEffect>::iterator e = type->effects.begin(); e != type->effects.end(); e++)
		effects.push_back(e->effectFile);

	for (vector<PrivateValue>::iterator v = type->m_PrivateValues.begin(); v != type->m_PrivateValues.end(); v++)
		private_variables.push_back(NameDllInfo(v->name, v->identifier));
}

bool ObjectTypeDetail::Match(CObjType* type)
{
	if(info.name != type->GetName())
		return false;
	if(info.dll != type->DLLIndex)
		return false;

	return true;
	//todo: loop behaviors and effects and private variables
}

