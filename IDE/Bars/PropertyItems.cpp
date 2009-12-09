
// Property items: for custom property controls, item insertion and change mapping
#include "stdafx.h"
#include "PropertiesBar.h"
#include "../MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern PropertiesBar* propertyWindow;
extern CImageList* m_treeImages;
extern CMainFrame* pMainWnd;

#include "..\ChildFrame.h"
#include "..\Construct.h"

// Image editor include
#include "..\Editors\ImageEditor.h"

extern CLayerBar* g_pLayerBar;

// Convert double to CString
CString FloatToString(double x)
{
	char strbuf[32];
	_gcvt(x, 7, strbuf);
	
	// Last character is a dot - remove it
	int lastch = strlen(strbuf) - 1;
	if (strbuf[lastch] == '.')
		strbuf[lastch] = '\0';

	return strbuf;
}

// Hyperlink class
CExtGridCellHyperLinkEx::CExtGridCellHyperLinkEx(
    CExtGridDataProvider * pDP // = NULL
    )
    : CExtGridCellHyperLink( pDP )
{
}

CExtGridCellCurrencyEx::CExtGridCellCurrencyEx(
    CExtGridDataProvider * pDP // = NULL
    )
    : CExtGridCellCurrency( pDP )
{
}

CExtGridCellNumberEx::CExtGridCellNumberEx(
    CExtGridDataProvider * pDP // = NULL
    )
    : CExtGridCellNumber( pDP )
{
}

void PropertiesBar::SetGridCellIcon(CExtGridCell* pCell, HBITMAP bmpIcon)
{
	// Don't use icons for multiselected objects.  Really slows down the performance.
	// Can't see how to make work with Combine().
	if (vSelectedObjects.size() > 1)
		return;

	CExtCmdIcon icon;
	icon.m_dwFlags = __EXT_ICON_FLAGS_DEFAULT;
	icon.AssignFromHBITMAP(bmpIcon);

	int nIndex = m_pGrid->pGridCategorized->GridIconInsert(&icon);

	if (nIndex == -1) return;

	pCell->IconIndexSet(nIndex);	
}

CExtPropertyItem* PropertiesBar::InsertCategory(CString label, CString desc, CExtPropertyItem* pRoot)
{
	CExtPropertyItem * pCat = new CExtPropertyCategory(label);
	pCat->DescriptionSet(desc);
	if (pRoot)
		pRoot->ItemInsert(pCat);
	else
		m_pCurrentStore->ItemInsert(pCat);

	// Use label to remember whether this was expanded or not
	if (curExpandSave)
		pCat->ExpandedSet(!(*curExpandSave)[label]);

	return pCat;
}

CExtPropertyItem* PropertiesBar::InsertAppItem(CString label, CString desc, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CAppPropertyValueMgr(m_pCurrentApplication, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertResourceItem(CString label, CString desc, CApplication* application, list<ApplicationResource>* stored, ApplicationResource* resource, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new ApplicationResourcePropertyValueMgr(resource, label, stored, application);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertControlItem(CString label, CString desc, int player, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CControlPropertyValueMgr(m_pCurrentApplication, label, player);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertFrameItem(CString label, CString desc, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CLayoutPropertyValueMgr(m_CurrentLayout, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertStdObjectItem(CString label, CString desc, CObj* obj, CObjType* pType, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CPluginStdPropertyValueMgr(obj, pType, m_view, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertPrivateVarItem(CString label, CString desc, CObj* obj, CObjType* pType, int valueIndex, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CPluginPrivateVarMgr(obj, pType, valueIndex, m_view);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertObjectItem(CString label, CString desc, CObj* obj, editInfo* data, CObjType* pType, OINFO* oInfo, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CPluginPropertyValueMgr(obj, data, pType, oInfo, m_view, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertEffectItem(CString label, CString desc, CEffect* _pEffect, list<CEffect>& _effects, CObjType* _pType, CLayer* _pLayer, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CEffectPropertyValueMgr(_pEffect, _effects, _pType, _pLayer, m_view, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertBehaviorItem(CString label, CString desc, CBehavior* _pMov, CObjType* _pOwner,
												CLayout* _level, CLayoutEditor* _frame, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new BehaviorPropertyValueMgr(_pMov, _pOwner, _level, _frame, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}
CExtPropertyItem* PropertiesBar::InsertTraitItem(CString label, CString desc, CString _trait, CObjType* _pOwner, CExtPropertyItem* pRoot)
{
	// Memory leak - Davo, use InsertNeutralItem
	//CExtPropertyItem* pItem = InsertStdObjectItem(label, desc, pRoot);

	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CTraitPropertyValueMgr(_trait, _pOwner, m_view, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertFamilyItem(CString label, CString desc, CObjType* pType, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new FamilyPropertyMgr(pType, label, m_view);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertTransitionItem(CString label, CString desc, CTransition* pTransition, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CTransitionPropertyMgr(pTransition, m_pCurrentApplication, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}
CExtPropertyItem* PropertiesBar::InsertEventSheetItem(CString label, CString desc, EventSheet* pEventSheet, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new EventSheetPropertyMgr(pEventSheet, m_pCurrentApplication, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertContainerItem(CString label, CString desc, CContainer* pContainer, long oid, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CContainerPropertyMgr(pContainer, oid, m_view);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertGlobalItem(CString label, CString desc, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CGlobalPropertyMgr(m_pCurrentApplication, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertLayerItem(CString label, CString desc, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CLayerPropertyValueMgr(pLastLayer, m_view, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertAnimationItem(CString label, CString desc, CAnimation* pAnim, int frameIndex, CExtPropertyItem* pRoot)
{
	CExtPropertyItem* pItem = InsertNeutralItem(label, desc, pRoot);

	// Register the property value manager
	m_pGrid->propValueMgrs[pItem] = new CAnimationPropertyValueMgr(pAnim, frameIndex, label);
	m_pGrid->propValueMgrs[pItem]->pItem = pItem;

	return pItem;
}

CExtPropertyItem* PropertiesBar::InsertNeutralItem(CString label, CString desc, CExtPropertyItem* pRoot, bool compound)
{
	CExtPropertyItem* pItem;
	if (compound) {
		pItem = new CExtPropertyValueCompound(label);
		// Use label to remember whether this was expanded or not
		pItem->ExpandedSet(!(*curExpandSave)[label]);
	}
	else 
		pItem = new CExtPropertyValue(label);

	pItem->DescriptionSet(desc);

	if (pRoot)
		pRoot->ItemInsert(pItem);
	else
		m_pCurrentStore->ItemInsert(pItem);
	return pItem;
}

CExtGridCellDropListComboBox* PropertiesBar::GetCombo(CExtPropertyItem* pItem)
{
	CExtGridCellDropListComboBox* pCombo = STATIC_DOWNCAST(CExtGridCellDropListComboBox,
						   pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellDropListComboBox)));

	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pCombo;

	return pCombo;
}

CExtGridCellRadioButton* PropertiesBar::GetRadio(CExtPropertyItem* pItem)
{
	CExtGridCellRadioButton* pRadio = STATIC_DOWNCAST(CExtGridCellRadioButton,
						   pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellRadioButton)));

	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pRadio;

	return pRadio;
}

CExtGridCellFont* PropertiesBar::GetFont(CExtPropertyItem* pItem)
{
	CExtGridCellFont* pFont = STATIC_DOWNCAST(CExtGridCellFont,
						   pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellFont)));

	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pFont;

	return pFont;
}

CExtGridCellString* PropertiesBar::GetEdit(CExtPropertyItem* pItem)
{
	CExtGridCellString* pString = STATIC_DOWNCAST(CExtGridCellString,
						 pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellString)));
	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pString;
	return pString;
}

CExtGridCellSlider* PropertiesBar::GetSlider(CExtPropertyItem* pItem)
{
	CExtGridCellSlider* pSlider = STATIC_DOWNCAST(CExtGridCellSlider,
						 pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellSlider)));
	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pSlider;
	return pSlider;
}

CExtGridCellNumber* PropertiesBar::GetInteger(CExtPropertyItem* pItem)
{
	CExtGridCellNumber* pNumber = STATIC_DOWNCAST(CExtGridCellNumber,
						 pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellNumber)));
	pNumber->ModifyStyle(__EGCS_TA_HORZ_LEFT, __EGCS_TA_HORZ_MASK);
	// No comma separation
	pNumber->SetGrouping("0");
	pNumber->SetThousandSeparator("");
	pNumber->SetAllowTrailingZeroes(false);
	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pNumber;
	return pNumber;
}

CExtGridCellNumberEx* PropertiesBar::GetFloat(CExtPropertyItem* pItem)
{
	CExtGridCellNumberEx* pNumber = STATIC_DOWNCAST(CExtGridCellNumberEx,
						 pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellNumberEx)));
	pNumber->ModifyStyle(__EGCS_TA_HORZ_LEFT, __EGCS_TA_HORZ_MASK);
	pNumber->SetNumDigits(3);
	pNumber->SetAllowTrailingZeroes(false);

	// No comma separation
	pNumber->SetGrouping("0");
	pNumber->SetThousandSeparator("");

	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pNumber;
	return pNumber;
}

CExtGridCellCurrency* PropertiesBar::GetPercent(CExtPropertyItem* pItem)
{
	CExtGridCellCurrencyEx* pCurrency = STATIC_DOWNCAST(CExtGridCellCurrencyEx,
						 pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellCurrencyEx)));
	pCurrency->ModifyStyle(__EGCS_TA_HORZ_LEFT, __EGCS_TA_HORZ_MASK);
	pCurrency->SetNumDigits(3);
	pCurrency->SetCurrencySymbol("%");
	pCurrency->SetPositiveOrder(1);
	pCurrency->SetNegativeOrder(5);
	pCurrency->SetLeadingZero(0);
	pCurrency->SetAllowTrailingZeroes(false);

	// No comma separation
	pCurrency->SetGrouping("0");
	pCurrency->SetThousandSeparator("");

	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pCurrency;
	return pCurrency;
}

CExtGridCellCheckBox* PropertiesBar::GetCheckBox(CExtPropertyItem* pItem)
{
	CExtGridCellCheckBox* pCheck = STATIC_DOWNCAST(CExtGridCellCheckBox,
						 pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellCheckBox)));
	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pCheck;
	pCheck->ModifyStyle(__EGCS_NO_INPLACE_CONTROL);
	return pCheck;
}

CExtGridCellHyperLinkEx* PropertiesBar::GetHyperLink(CExtPropertyItem* pItem)
{
	CExtGridCellHyperLinkEx* pLink = STATIC_DOWNCAST(CExtGridCellHyperLinkEx,
						 pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellHyperLinkEx)));

	pLink->TextColorSet(CExtGridCellHyperLink::__ECS_NORMAL, RGB(0,0,255));
	pLink->TextColorSet(CExtGridCellHyperLink::__ECS_SELECTED, RGB(255,0,0));
	pLink->TextColorSet(CExtGridCellHyperLink::__ECS_HOVERED, RGB(128,128,255));
	pLink->SetAllowVisitedState(1);
	pLink->SetTextColorVisited(RGB(0,0,255));
	pLink->FontUnderlineSet();

	// I hate this workaround, but it is the only damn thing that works.
	HyperLinkClickInfo hci;
	hci.pGrid = m_pGrid;
	hci.pItem = pItem;
	hyperLinks.push_back(hci);
	pLink->LParamSet((LPARAM)&(hyperLinks.back()));

	pLink->SetCursor(LoadCursor(NULL, IDC_HAND));

	// Set the cell pointer
	m_pGrid->propValueMgrs[pItem]->pCell = pLink;
	return pLink;
}

CExtGridCellColor* PropertiesBar::GetColor(CExtPropertyItem* pItem)
{
	CExtGridCellColor* pColor = STATIC_DOWNCAST(CExtGridCellColor,
				pItem->ValueDefaultGetByRTC(RUNTIME_CLASS(CExtGridCellColor)));

	m_pGrid->propValueMgrs[pItem]->pCell = pColor;
	return pColor;
}

CExtGridCellEx* PropertiesBar::AddPropertyItemToTree(CPropItem& item, CExtPropertyItem* pRoot, CString& retStr, PropReturn& Return)
{
	CExtGridCellString* pString;
	CExtGridCellDropListComboBox* pCombo;
	CExtGridCellNumber* pNumber;
	CExtGridCellCurrency* pCurr;
	CExtGridCellCheckBox* pCheck;
	CExtGridCellColor* pColor;
	CExtGridCellHyperLink* pLink;
	CExtGridCellSlider* pSlider;
	CExtGridCellFont* pFont;

	switch (item.Type)
	{
	// Edit
	case 0:
		{
			pString = GetEdit(pRoot);
			pString->TextSet(retStr);
			return pString;
		}

	// Static
	case 1:
		{
			pString = GetEdit(pRoot);
			pString->TextSet(retStr);
			pString->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL);
			return pString;
		}

	// Button
	case 2:
		{
			pLink = GetHyperLink(pRoot);
			pLink->TextSet(item.Text);
			return pLink;
		}

	// Value
	case 3:
		{
			pNumber = GetInteger(pRoot);
			pNumber->_VariantAssign(Return.val);
			return pNumber;
		}

	// Combo
	case 4:
		{
			pCombo = GetCombo(pRoot);

			// Add items
			CString Work = item.Text;
			int count = Work.Replace("|", "");
			count++;

			for (int e = 0; e < count; e++)
			{
				CString tok;
				AfxExtractSubString(tok, item.Text, e, '|');
				pCombo->InsertString(tok);
			}

			pCombo->SetCurSel(Return.val);
			return pCombo;
		}

	// Float
	case 5:
		{
			pNumber = GetFloat(pRoot);
			pNumber->_VariantAssign(Return.fVal);
			return pNumber;
		}

	// Color
	case 6:
		{
			pColor = GetColor(pRoot);
			pColor->SetColor((COLORREF)Return.val);
			return pColor;
		}

	// Checkbox (bool)
	case 7:
		{
			pCheck = GetCheckBox(pRoot);
			pCheck->SetCheck(Return.val);
			return pCheck;
		}

	//	Font
	case 8:
		{
			pFont = GetFont(pRoot);

			return pFont;
		}

		// Percentage
	case 9:
		{
			pCurr = GetPercent(pRoot);
			pCurr->_VariantAssign(Return.fVal * 100.0);
			return pCurr;
		}

		// Slider (same as value)
	case 10:
		{
			pSlider = GetSlider(pRoot);
			pSlider->SetPos(Return.val);
			return pSlider;
		}

	// Frame property (int)
	case 11:
		{
			pCombo = GetCombo(pRoot);

			// Add frames
			for (int e = 0; e < m_pCurrentApplication->layouts.GetSize(); e++)
				pCombo->InsertString(m_pCurrentApplication->layouts.GetAt(m_pCurrentApplication->layouts.FindIndex(e))->GetName());

			pCombo->SetCurSel(Return.val);
			return pCombo;
		}
	}

	// Invalid
	return NULL;
}