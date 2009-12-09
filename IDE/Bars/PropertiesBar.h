#pragma once

#include "PropBar.h"

#define PROPTYPE_EDIT		0
#define PROPTYPE_STATIC		1
#define PROPTYPE_BUTTON		2
#define PROPTYPE_VALUE		3
#define PROPTYPE_COMBO		4
#define PROPTYPE_FLOAT		5
#define PROPTYPE_COLOR		6
#define PROPTYPE_CHECKBOX	7
#define PROPTYPE_FONT		8
#define PROPTYPE_PERCENT	9
#define PROPTYPE_SLIDER		10
#define PROPTYPE_FRAME		11

struct HyperLinkClickInfo {
	CExtPropertyItem* pItem;
	CProfStudioPropertyGridCtrl* pGrid;
};

// Overridden OnHyperlinkOpen
class CExtGridCellHyperLinkEx : public CExtGridCellHyperLink 
{
    DECLARE_DYNCREATE(CExtGridCellHyperLinkEx);
    IMPLEMENT_ExtGridCell_Clone( CExtGridCellHyperLinkEx, CExtGridCellHyperLink );
    CExtGridCellHyperLinkEx(
        CExtGridDataProvider * pDP = NULL
        );
public:

    virtual bool OnHyperLinkOpen(
        __EXT_MFC_SAFE_LPCTSTR strURL,
        e_OpenMode_t eOM = __EOM_OPEN,
        bool bAllowErrorAlert = false
        ) const 
    {
		// Trigger the onchange
		HyperLinkClickInfo* info = (HyperLinkClickInfo*)LParamGet();
		info->pGrid->DoItemChange(info->pItem);
        return true;
    }
};

// Overridden for trailing zero removal
class CExtGridCellCurrencyEx : public CExtGridCellCurrency
{
    DECLARE_DYNCREATE(CExtGridCellCurrencyEx);
    IMPLEMENT_ExtGridCell_Clone( CExtGridCellCurrencyEx, CExtGridCellCurrency );
    CExtGridCellCurrencyEx(
        CExtGridDataProvider * pDP = NULL
        );
public:
	void TextGet( CExtSafeString & strCopy ) const;
};

// Overridden for trailing zero removal
class CExtGridCellNumberEx : public CExtGridCellNumber
{
    DECLARE_DYNCREATE(CExtGridCellNumberEx);
    IMPLEMENT_ExtGridCell_Clone( CExtGridCellNumberEx, CExtGridCellNumber );
    CExtGridCellNumberEx(
        CExtGridDataProvider * pDP = NULL
        );
public:
	void TextGet( CExtSafeString & strCopy ) const;
};

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar window

#include "..\Construct Doc.h"
#include "..\Editors\LayoutEditor.h"

class PropertiesBar : public CExtControlBar
{
// Construction
public:
	PropertiesBar();

	// Animation information
	CAnimation* m_pAnimation;
	int			m_iIndex;

	void AddAnimationProperties();
	void AddLayoutProperties(CLayout* layout);
	void AddLayerProperties(CLayer* layer);
	void AddResourceProperties(ApplicationResource* resource, CApplication* application, list<ApplicationResource>* stored);

// Implementation
public:
	virtual ~PropertiesBar();

	void Update(CLayoutEditor* view, long type, CObjList* sel, CLayout* pLayout, CObjMap *objs, CApplication* app = NULL, long expand = 0, CLayer* pLayer = NULL, ApplicationResource* pResource = NULL, list<ApplicationResource>* stored = NULL);
	void Refresh();

	template <typename M, typename C>
	bool SelectionMembersEqual(M C::* member);

	long		m_type;
	CLayoutEditor*	m_view;
	CObjList m_sel;
	CObjMap* m_objects;

	bool multiSelect;

	vector<CObj*> vSelectedObjects;
	vector<CObjType*> vSelectedTypes;
	vector<OINFO*> vSelectedOINFOs;

	map<CString, bool>* curExpandSave;

	CExtPropertyStore* m_pStore;				// Current property store to display
	CExtPropertyStore* m_pCurrentStore;			// Current property store to add elements to
	CProfStudioPropertyGridCtrl* m_pGrid;		// Property grid ctrl
	CProfStudioPropertyGridCtrl m_Grid;		// Property grid ctrl

	void	SetGridCellIcon(CExtGridCell* pCell, HBITMAP bmpIcon);

	 //For multi selections
	vector<CExtPropertyStore*> m_pMultiStore;

	CLayer* pLastLayer;

	long        curType;

	CObjType* m_pSelectedObjType; //unsafe, only use for movement properties
	CObj* m_pSelectedObj; //unsafe, only use for movement properties
    
    // Allow usage as a pointer    
	PropertiesBar* operator->() { return this; }

	CApplication*	m_pCurrentApplication;
	CLayout*		m_CurrentLayout;

	// A totally stupid hack for working around Prof-UIS hyper link classes being reset
	list<HyperLinkClickInfo> hyperLinks;

     // Allow usage as a pointer    
	operator PropertiesBar*() { return this; }

	CFont	m_font;

	// Property grid helpers
	CExtPropertyItem*		InsertCategory(CString label, CString desc, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertAppItem(CString label, CString desc, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertResourceItem(CString label, CString desc, CApplication* application, list<ApplicationResource>* stored, ApplicationResource* resource, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertControlItem(CString label, CString desc, int player, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertFrameItem(CString label, CString desc, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertStdObjectItem(CString label, CString desc, CObj* obj, CObjType* pType, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertPrivateVarItem(CString label, CString desc, CObj* obj, CObjType* pType, int valueIndex, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertObjectItem(CString label, CString desc, CObj* obj, editInfo* data, CObjType* pType, OINFO* oInfo, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertEffectItem(CString label, CString desc, CEffect* _pEffect, list<CEffect>& _effects, CObjType* _pType, CLayer* _pLayer, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertBehaviorItem(CString label, CString desc, CBehavior* _pMov, CObjType* _pOwner, CLayout* _level, CLayoutEditor* _frame, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertTraitItem(CString label, CString desc, CString _trait, CObjType* _pOwner, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertFamilyItem(CString label, CString desc, CObjType* pType, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertGlobalItem(CString label, CString desc, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertContainerItem(CString label, CString desc, CContainer* pContainer, long oid, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertTransitionItem(CString label, CString desc, CTransition* pTransition, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertEventSheetItem(CString label, CString desc, EventSheet* pEventSheet, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*		InsertAnimationItem(CString label, CString desc, CAnimation* pAnim, int frameIndex, CExtPropertyItem* pRoot = NULL);

	CExtPropertyItem*				InsertLayerItem(CString label, CString desc, CExtPropertyItem* pRoot = NULL);
	CExtPropertyItem*				InsertNeutralItem(CString label, CString desc, CExtPropertyItem* pRoot = NULL, bool compound = false);
	CExtGridCellDropListComboBox*	GetCombo(CExtPropertyItem* pItem);
	CExtGridCellString*				GetEdit(CExtPropertyItem* pItem);
	CExtGridCellNumber*				GetInteger(CExtPropertyItem* pItem);
	CExtGridCellNumberEx*			GetFloat(CExtPropertyItem* pItem);
	CExtGridCellCurrency*			GetPercent(CExtPropertyItem* pItem);
	CExtGridCellCheckBox*			GetCheckBox(CExtPropertyItem* pItem);
	CExtGridCellColor*				GetColor(CExtPropertyItem* pItem);
	CExtGridCellHyperLinkEx*		GetHyperLink(CExtPropertyItem* pItem);
	CExtGridCellSlider*				GetSlider(CExtPropertyItem* pItem);
	CExtGridCellRadioButton*		GetRadio(CExtPropertyItem* pItem);
	CExtGridCellFont*				GetFont(CExtPropertyItem* pItem);

	CExtGridCellEx* AddPropertyItemToTree(CPropItem& item, CExtPropertyItem* pRoot, CString& retStr, PropReturn& Return);

	void							DoAppProperties();

	// tips
	CCtrlMessageBar					tips;
	void							ShowTip(std::string const);

	static void Update_OINFO_Properties(editInfo* info);
	// Generated message map functions

	//{{AFX_MSG(CPropertiesBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};