//////////////////////////////////////////////
//	Undo.h
//	Copyright (c) Chris Newhouse 2006
//////////////////////////////////////////////

#pragma once

// Action types
enum FRAME_ACTION {
	ACTION_MOVESIZE = 1001,		// long id, CObj o - this actually just serializes the CObj, so it is for rotating too, etc
	ACTION_CREATE,				// long id, layer, prevobjid, CObj o
	ACTION_CREATETYPE,			// long globalID, CObjType oT, long id, layer, prevobjid, CObj o
	ACTION_DELETE,				// long id
	ACTION_CHANGENAME,			// long id, CString name
	ACTION_CHANGEFRAMEPROP,		// long proptype, variable propvalue
	ACTION_CHANGEOBJECTPROP,	// long id, proptype, variable propvalue
	ACTION_CHANGEZORDER,		// a bunch of CLists
	ACTION_SETVISIBLE,			// long id, bool visible
	ACTION_SETLOCK,				// long id, bool locked
	ACTION_GROUP,				// long gid, CList objectIDs
	ACTION_UNGROUP,				// long id
	ACTION_GROUPOPEN,			// long id, bool open
	ACTION_GROUPADD,			// long gid, long oid
	ACTION_GROUPREMOVE,			// long gid, long oid, layer, prevobjid
	ACTION_EVENTEDITOR,
	ACTION_CHANGETYPE,			// long id, CObjType oT
	ACTION_CHANGELAYER,			// long id, CLayer lyr
	ACTION_CHANGELISTS,			// long familycount, familylist, long containercount, containerlist
	ACTION_CHANGEEFFECTS		// double, effectaddr, long effectcount, effectlist
};

class CAction {
public:
	CAction();
	~CAction();

	// Methods
	CArchive* Prepare1();
	void Prepare2();
	CArchive* Unprepare1();
	void Unprepare2();

	// Properties
	FRAME_ACTION	m_type;
	BOOL			m_bGroup;

private:
	CArchive	*m_ar;
	HGLOBAL		m_data;
	CSharedFile	*m_sf;
	CMemFile	*m_mf;
};

class CLayoutEditor;
class CEventEditor;

class CUndo {
public:
	CUndo(CLayoutEditor *view);
	CUndo(EventSheetEditor *view);
	~CUndo();

	// Methods
	void ClearUndo();
	void ClearRedo();
	void Undo();
	void Redo();
	void AddAction(CAction *action); 

	// Properties
	BOOL CanUndo();
	BOOL CanRedo();

private:
	CLayoutEditor *m_pFrameEditor;
	EventSheetEditor *m_pEventEditor;
	CList<CAction*, CAction*&> m_undo;
	CList<CAction*, CAction*&> m_redo;
};