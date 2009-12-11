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

//-----------------------------------------

class ExportAction
{
public:
	ExportAction(int oid, int mid, int actid, ParamVector* params);

	void Export(CExport* exporter);

private:
	int oid;
	int mid;
	int actid;

	ParamVector* params;
};

//--------------------------

class ExportCondition
{
public:
	ExportCondition(int oid, int mid, int cndid, bool negated, bool trigger, ParamVector* params);

	void Export(CExport* exporter);
	bool isTrigger();
private:
	int oid;
	int mid;
	int cndid;
	bool negated;
	bool trigger;
	ParamVector* params;
};

//--------------------------

class ExportBlock
{
public:
	ExportBlock();
	~ExportBlock();
	ExportBlock(ExportBlock* other, bool children);

	void moveAfter(ExportBlock* block);
	void moveBefore(ExportBlock* block);
	void addChildBack(ExportBlock* newChild); // adds block as a child at the end
	void addChildFront(ExportBlock* newChild); // adds block as a child at the start
	void detach();

	virtual void Export(CExport* exporter) = 0;

	virtual ExportBlock* clone(bool children) = 0;

public:
	ExportBlock* getPrev();
	ExportBlock* getNext();
	ExportBlock* getParent();
	ExportBlock* getFirstChild();
	ExportBlock* getLastChild();

	void setParentForAllSiblings(ExportBlock* newParent);

public:
	virtual void ProcessEventsTriggers(ExportBlock* root, list<ExportBlock*>& append);
	virtual bool ProcessThisEventForTrigger(ExportBlock* root, list<ExportBlock*>& append);
	virtual ExportBlock* createConditionOnlyEvents(ExportBlock* other);

private:
	ExportBlock* next;
	ExportBlock* prev;
	ExportBlock* parent; 
	ExportBlock* first_child;
	ExportBlock* last_child;

};

//--------------------------

class ExportEvent : public ExportBlock
{
public:
	ExportEvent(int LineNumber, int SheetNumber );
	ExportEvent(ExportEvent* other, bool children);

	void Export(CExport* exporter);
	virtual ExportBlock* clone(bool children);

	virtual bool ProcessThisEventForTrigger(ExportBlock* root, list<ExportBlock*>& append);

	void RemoveAllTriggers();
	virtual ExportBlock* createConditionOnlyEvents(ExportBlock* other);

public:
	list<ExportAction> actions;
	list<ExportCondition> conditions;
private:
	int linenumber;
	int sheetnumber;

};

//--------------------------

class ExportGroup : public ExportBlock
{
public:
	ExportGroup(CString Name, BYTE Active);
	ExportGroup(ExportGroup* other, bool children);

	void Export(CExport* exporter);
	virtual ExportBlock* clone(bool children);
private:
	CString name;
	BYTE active;
};

//--------------------------

class ExportScript : public ExportBlock
{
public:
	ExportScript(CString Script);
	ExportScript(ExportScript* other, bool children);

	void Export(CExport* exporter);
	virtual ExportBlock* clone(bool children);

private:
	CString script;
};



//--------------------------

class CExport
{
public:
	int						m_TriggerDepth;
	int						m_EventSheetNumber;

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
	void AddEvent(CEditorEvent*, ExportBlock* root, int sheetid);
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

	void ProcessEventsElse(ExportBlock* root);
	void ProcessEventsTriggers(ExportBlock* root);
	void ExportEvents(ExportBlock* root);

	int ConvertMID(int mid, int oid);
};



