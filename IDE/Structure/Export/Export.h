#pragma once

#include "..\..\Dialogs\ProgressDlg.h"
#include "..\..\MainFrm.h"

class MicroDependencyImpl : public MicroDependency {
public:
    virtual void Add(const char* dll)
	{
		m_pDependencies->Add(dll);
	}

	CStringArray* m_pDependencies;
};

class TraitExport
{
public:
	int OID;
	CString TraitName;
};

class MvLookup
{
public:
	int PreviousIndex;
	int NewIndex;

	CString Name;
};

class CExport
{
public:
	int						m_Type;
	int						m_TriggerDepth;
	int						m_EventSheetNumber;

	CEditorEvent*			m_triggerless;	// topmost triggerless
	CEditorEvent*			m_curTriggerlessEvent; //current one to modify
	list<CEditorEvent>		m_export_later; // list of events to append at the end
	list<CEditorEvent>		m_event_memory; // instead of making new events we use this...it'll be cleaned once export done

	CApplication*			application;
	CMainFrame*				main_frame;

	CProgressDlg			ProgressDlg;	// For displaying progress

	void Preview			(CApplication* pApplication, int layoutid = -1);
	void PreviewDebug		(CApplication* pApplication, int layoutid = -1);
	void ExportToEXE		(CApplication* pApplication);
	void ExportToZIP		(CApplication* pApplication);
	void PrepareObjectList	(CObjList& objlist, vector<CObj*>& objects, CLayer* layer, CLayout* pFrame);
	void DoExport			(bool previewMode, const CString& pathName, bool bApp, int layoutid = -1);
	CStringArray exportID;

	void ReallocateTriggerlessEvent(CEditorEvent* cutoff);
	bool ReallocateEvent(CEditorEvent* pOldEvent, CEditorEvent* cutoff);

	// Data blocks
	bin		LayoutBlock;
	bin		eventBlock;
	bin		appBlock;
	bin		imageBlock;
	bin		hlslBlock;
	bin		menuBlock;

	BYTE*   images;
	int     imsize;
	int		m_depth;

	vector<MvLookup> MvtLookup;

	HANDLE m_UpdateHandle;

	// Operations
	// Application
		void GenerateApplicationData(int layoutid);

	// Auxiliary
		int ObjTypeIDCnvt(int obj);
		int ObjIDCnvt(int obj);

	// Python
		void GeneratePython();

	// Layout
		void GenerateLayout(CList<CString, CString&>& dllList);
		void GenerateHLSL(CList<CString, CString&>& dllList);

	// Events
		void AddEvent(CEditorEvent*, int);
		void ExportParameter(CEditorParam& param, int runtimeOid, int edittimeOid);
		void GenerateEvents();

	// Images
		void CreateCollisionMask(CxImage& Image, BYTE*& Bits, int& Pitch);
		void GenerateImages();
		void ExportAnimations(list<CAnimation>& anims);

	// Resources
		void ExportResources(bool previewing);
		void ExportMenuItems(MenuResourceItem Item);

	// Dependencies
		CStringArray m_Dependencies;

	// Installer
		bool	m_bInstaller;
		CString	m_License;
		CString	m_InstallPath;
		int		m_OS;
		CString m_Out;
		CString m_InstallerOut;
		void	CreateInstaller();

	// Screensaver
		bool	m_bScreensaver;

	typedef map<int,CObjType*> NumberToTypeMap;
	typedef map<int,int> NumberToNumberMap;
	typedef map<CObjType*, int> TypeToNumber;

	NumberToTypeMap m_ExportTypes;
	NumberToNumberMap m_ExportTypesNumbers;
	map<int,int> m_EventSheetConvert;

	int EventSheetIndex(int id);

	int BehaviorCount;
	int TraitsCount;

	vector<FamilyLookup> Families;
};



