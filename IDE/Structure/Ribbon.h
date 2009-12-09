#pragma once

#include "..\res\RibbonIcons.h"

#define ID_RIBBON_PAGE_STANDARD                                       8999
#define ID_RIBBON_PAGE_CLIPBOARD                                      9000
#define ID_RIBBON_PAGE_FONT                                           9001
#define ID_RIBBON_PAGE_PARAGRAPH                                      9002
#define ID_MAILINGS_RULES                                             9003
#define ID_MAILINGS_GOTO_RECORD                                       9004
#define ID_TRANSLATION_TOOL_TIP                                       9005
#define ID_SHOW_HIDE_RULER                                            9006
#define ID_SHOW_HIDE_GRIDLINES                                        9007
#define ID_SHOW_HIDE_PROPERTIES                                       9008
#define ID_SHOW_HIDE_DOCUMENT_MAP                                     9009
#define ID_SHOW_HIDE_THUMBNAILS                                       9010
#define ID_SHOW_HIDE_DOCUMENT_ACTION_BAR                              9011
#define ID_RIBBON_PAGE_STYLE                                          9012
#define ID_RIBBON_PAGE_INSERT_PAGES                                   9013
#define ID_RIBBON_PAGE_INSERT_TABLE                                   9014
#define ID_RIBBON_PAGE_INSERT_ILLUSTRATIONS                           9015
#define ID_RIBBON_PAGE_INSERT_LINKS                                   9016
#define ID_RIBBON_PAGE_INSERT_HEADER_AND_FOOTER                       9017
#define ID_RIBBON_PAGE_INSERT_TEXT                                    9018
#define ID_RIBBON_PAGE_INSERT_SYMBOLS                                 9019
#define ID_RIBBON_PAGE_PL_THEMES                                      9020
#define ID_RIBBON_PAGE_PL_PAGE_SETUP                                  9021
#define ID_RIBBON_PAGE_PL_PAGE_BACKGROUND                             9022
#define ID_RIBBON_PAGE_PL_PARAGRAPH                                   9023
#define ID_RIBBON_PAGE_PL_ARRANGE                                     9024
#define ID_RIBBON_PAGE_REF_TOC                                        9025
#define ID_RIBBON_PAGE_REF_FOOTNOTES                                  9026
#define ID_RIBBON_PAGE_REF_CB                                         9027
#define ID_RIBBON_PAGE_REF_CAPTIONS                                   9028
#define ID_RIBBON_PAGE_REF_INDEX                                      9029
#define ID_RIBBON_PAGE_REF_TOA                                        9030
#define ID_RIBBON_PAGE_MAIL_CREATE                                    9031
#define ID_RIBBON_PAGE_MAIL_SMM                                       9032
#define ID_RIBBON_PAGE_MAIL_WIF                                       9033
#define ID_RIBBON_PAGE_MAIL_PREVIEW_RESULTS                           9034
#define ID_RIBBON_PAGE_MAIL_FINISH                                    9035
#define ID_RIBBON_PAGE_REVIEW_PROOFING                                9036
#define ID_RIBBON_PAGE_REVIEW_COMMENTS                                9037
#define ID_RIBBON_PAGE_REVIEW_TRACKING                                9038
#define ID_RIBBON_PAGE_REVIEW_CHANGES                                 9039
#define ID_RIBBON_PAGE_REVIEW_COMPARE                                 9040
#define ID_RIBBON_PAGE_REVIEW_PROTECT                                 9041
#define ID_RIBBON_PAGE_VIEW_DV                                        9042
#define ID_RIBBON_PAGE_VIEW_SHOW_HIDE                                 9043
#define ID_RIBBON_PAGE_VIEW_ZOOM                                      9044
#define ID_RIBBON_PAGE_VIEW_WINDOW                                    9044
#define ID_FB_FINISH_RESTRICT_PERMISSION_UNRESTRICTED_ACCESS          9044
#define ID_FB_FINISH_RESTRICT_PERMISSION_DO_NOT_DISTRIBUTE            9045
#define ID_FB_FINISH_RESTRICT_PERMISSION_AS                           9046
#define ID_SAVE_SELECTION_AS_NEW_QUICK_STYLE                          9047
#define ID_CLEAR_FORMATTING                                           9048
#define ID_APPLY_STYLES                                               9049
#define ID_FONT_MORE_UNDERLINES                                       9050
#define ID_FONT_CASE_SENTENCE                                         9051
#define ID_FONT_CASE_LOWER                                            9052
#define ID_FONT_CASE_UPPPER                                           9053
#define ID_FONT_CASE_CAPITALIZE_EACH_WORD                             9054
#define ID_FONT_CASE_TOGGLE                                           9055
#define ID_EDIT_PASTE_AS_HYPERLINK                                    9056
#define ID_PARAGRAPH_LINE_SPACING_10                                  9057
#define ID_PARAGRAPH_LINE_SPACING_15                                  9058
#define ID_PARAGRAPH_LINE_SPACING_20                                  9059
#define ID_PARAGRAPH_LINE_SPACING_25                                  9060
#define ID_PARAGRAPH_LINE_SPACING_30                                  9061
#define ID_PARAGRAPH_LINE_SPACING_MORE                                9062
#define ID_PARAGRAPH_LINE_SPACING_SPACE_BEFORE                        9063
#define ID_PARAGRAPH_LINE_SPACING_NO_SPACE_AFTER                      9064
#define ID_DEFINE_NEW_BULLET                                          9065
#define ID_DEFINE_NEW_MULTILEVEL_LIST                                 9066
#define ID_DEFINE_NEW_LIST_STYLE                                      9067
#define ID_SE_FONT_LIST                                               9068
#define ID_SE_SIZE                                                    9069
#define ID_RTL                                                        9070
#define IDI_ICON_FONT_TT                131
#define IDI_ICON_FONT_PIX               132

#define __SO_FONT_SIZE_MIN	4.0f
#define __SO_FONT_SIZE_MAX	200.f

class CXRibbonPage
{
public:
	CXRibbonPage(CExtRibbonNodeTabPage* pPage)
	{
		m_GroupCount = 1; 
		m_pPage      = pPage;
	}

	int AddGroup(LPCSTR Name)
	{
		CExtRibbonNodeGroup* pRibbonGroup = new CExtRibbonNodeGroup(m_GroupCount);
		pRibbonGroup->SetTextInToolbar(Name);

		m_GroupCount++;

		m_pPage->InsertNode(NULL, pRibbonGroup);

		m_Groups.push_back(pRibbonGroup);
		return m_Groups.size() - 1;
	}

	void AddSeparatorToGroup(int GroupID)
	{
		m_Groups.at(GroupID)->InsertNode(NULL, new CExtRibbonNode);
	}

	CExtRibbonNode* AddCheckBoxToGroup(int GroupID, int ID, LPCSTR Name)
	{
		CExtRibbonNode* pNode = new CExtRibbonNode(ID, 0, NULL, 0, Name);
		pNode->RibbonILE_RuleArrayGet().RemoveAll();
		pNode->ModifyFlags( __ECTN_TBB_CHECK_BOX );
		m_Groups.at(GroupID)->InsertNode(NULL, pNode);

		return pNode;
	}

	CExtRibbonNodeLabel* AddLabelToGroup(int GroupID, LPCSTR Text)
	{
		CExtRibbonNodeLabel* pNode = new CExtRibbonNodeLabel(123, NULL, NULL, Text);
		m_Groups.at(GroupID)->InsertNode(NULL, pNode);

		return pNode;
	}

	CExtRibbonNode* AddItemToGroup(int GroupID, int ID, LPCSTR Name, int ResourceID, bool bSmall = false, bool bIcon = false)
	{
		CExtRibbonNode* pNode = new CExtRibbonNode(ID, 0, NULL, 0, Name);
		pNode->RibbonILE_RuleArrayGet().RemoveAll();

		if (bSmall)
		{
			if (bIcon)
			{
				HICON hIcon=(HICON)::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(ResourceID),IMAGE_ICON,16,16,0);
				pNode->m_iconSmall.AssignFromHICON(hIcon,false);
			}
			else
				pNode->m_iconSmall.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE(ResourceID));

			pNode->m_iconSmall.m_bmpNormal.Make32();
			pNode->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pNode->RibbonILE_RuleArrayGet().Add(__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(__EXT_RIBBON_ILE_MAX,__EXT_RIBBON_ILV_SIMPLE_SMALL,true));
		}

		else
		{
			if (bIcon)
			{
				HICON hIcon=(HICON)::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(ResourceID),IMAGE_ICON,32,32,0);
				pNode->m_iconBig.AssignFromHICON(hIcon,false);
			}
			else
				pNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE(ResourceID));

			pNode->m_iconBig.m_bmpNormal.Make32();
			pNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
		}

		m_Groups.at(GroupID)->InsertNode(NULL, pNode);

		return pNode;
	}

	void AddDropdownItem(int GroupID, int ID, LPCSTR Name, int ResourceID, bool bSmall = false, bool bIcon = false)
	{
		CExtRibbonNodeGallery* pNode = new CExtRibbonNodeGallery(ID, 0, 0, Name, Name, Name);
		pNode->RibbonILE_RuleArrayGet().RemoveAll();

		if (bSmall)
		{
			if (bIcon)
			{
				HICON hIcon=(HICON)::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(ResourceID),IMAGE_ICON,16,16,0);
				pNode->m_iconSmall.AssignFromHICON(hIcon,false);
			}
			else
				pNode->m_iconSmall.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE(ResourceID));

			pNode->m_iconSmall.m_bmpNormal.Make32();
			pNode->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pNode->RibbonILE_RuleArrayGet().Add(__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(__EXT_RIBBON_ILE_MIN,__EXT_RIBBON_ILV_SIMPLE_NORMAL,true));
		}

		else
		{
			if (bIcon)
			{
				HICON hIcon=(HICON)::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(ResourceID),IMAGE_ICON,32,32,0);
				pNode->m_iconBig.AssignFromHICON(hIcon,false);
			}
			else
				pNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE(ResourceID));

			pNode->m_iconBig.m_bmpNormal.Make32();
			pNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
		}

		m_Groups.at(GroupID)->InsertNode(NULL, pNode);

		CArray < DWORD, DWORD > &  Rules = pNode->RibbonILE_RuleArrayGet();
		Rules.RemoveAll();

		//if (bSmall)
			Rules.Add(__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(__EXT_RIBBON_ILE_MAX, __EXT_RIBBON_ILV_SIMPLE_SMALL, false));

	//	pNode->ModifyFlags(__ECTN_TBB_SEPARATED_DROPDOWN);
		pNode->m_nTpmxAdditionalFlags |= TPMX_RIBBON_RESIZING;
		pNode->m_sizePopupGalleryControl.cx = g_PaintManager.GetPM()->UiScalingDo( 310, CExtPaintManager::__EUIST_X );
		pNode->m_sizePopupGalleryControl.cy = g_PaintManager.GetPM()->UiScalingDo( 275, CExtPaintManager::__EUIST_Y );
		pNode->m_sizePopupGalleryControlMin.cx = g_PaintManager.GetPM()->UiScalingDo( 120, CExtPaintManager::__EUIST_X );
		pNode->m_sizePopupGalleryControlMin.cy = g_PaintManager.GetPM()->UiScalingDo( 120, CExtPaintManager::__EUIST_Y );
	}

	int                    m_GroupCount;
	CExtRibbonNodeTabPage* m_pPage;

	vector<CExtRibbonNodeGroup*> m_Groups;
};

class CXRibbonBar : public CExtRibbonBar
{
public:
	HICON m_hIconPara, m_hIconFontTT, m_hIconFontPix;
	CString m_sDecimalSeparator, m_sCurrentFontFaceName;
	float m_lfCurrentFontSize;

	void Init();

	CExtRibbonNode * m_pRibbonNode;

	CXRibbonBar();
	virtual ~CXRibbonBar();

	virtual void OnRibbonGalleryInitContent(CExtRibbonGalleryWnd & wndRG,CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,	CExtRibbonButtonGallery * pRibbonGalleryTBB	);
	virtual void OnRibbonGalleryItemSelEndOK(CExtRibbonGalleryWnd & wndRG,CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,CExtRibbonButtonGallery * pRibbonGalleryTBB,CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI);
	virtual CExtRibbonNodeFile* _InitRibbon_FileMenu();
	
	CExtRibbonNodeTabPage* AddPage (LPCSTR Name)
	{
		CExtRibbonNodeTabPage * pRibbonNodeTabPage = new CExtRibbonNodeTabPage(0L, NULL, Name);
		return pRibbonNodeTabPage;
	}
};
