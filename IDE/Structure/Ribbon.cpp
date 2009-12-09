#include "stdafx.h"
#include "ribbon.h"
#include "..\structure.h"
#include "..\MainFrm.h"

extern CString gInsert;
extern CMainFrame* g_MainFrame;

CExtRibbonNodeFile* CXRibbonBar::_InitRibbon_FileMenu()
{
	CExtRibbonNodeFile * pRibbonNodeFile = new CExtRibbonNodeFile;

	CExtRibbonNode* pNode =	new CExtRibbonNode(11, 11,
			NULL,__ECTN_BIG_ACCENT_IN_MENU,	NULL,_T(RIBBON_NEW), NULL, 0, NULL, 100, -1, 150);
	pNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE( ID_FB_NEW ));
	pNode->m_iconBig.m_bmpNormal.Make32();
	pNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonNodeFile->InsertNode(NULL, pNode);

		CExtRibbonNode * pSubNode;
		pSubNode = new CExtRibbonNode(ID_FILE_NEWAPP,ID_FILE_NEWAPP,NULL,	__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,NULL,_T(RIBBON_APPLICATION));
		pSubNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE( ID_APPLICATION ));
		pSubNode->m_iconBig.m_bmpNormal.Make32();
		pSubNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
		pNode->InsertNode( NULL, pSubNode );

		pSubNode = new CExtRibbonNode(ID_FILE_NEW,ID_FILE_NEW,NULL,	__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,NULL,_T(RIBBON_DIRECTX));
		pSubNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE( ID_DX ));
		pSubNode->m_iconBig.m_bmpNormal.Make32();
		pSubNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
		pNode->InsertNode( NULL, pSubNode );

		pSubNode = new CExtRibbonNode(ID_FILE_NEW_TEMPLATE,ID_FILE_NEW_TEMPLATE,NULL,	__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,NULL,_T(RIBBON_TEMPLATE));
		pSubNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE( ID_TEMPLATE ));
		pSubNode->m_iconBig.m_bmpNormal.Make32();
		pSubNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
		pNode->InsertNode( NULL, pSubNode );

	pNode =	new CExtRibbonNode(ID_FILE_OPEN, ID_FILE_OPEN,
			NULL,__ECTN_BIG_ACCENT_IN_MENU,	NULL,_T(RIBBON_OPEN));
	pNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE( ID_FB_OPEN ));
	pNode->m_iconBig.m_bmpNormal.Make32();
	pNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonNodeFile->InsertNode(NULL, pNode);

	pNode =	new CExtRibbonNode(ID_FILE_SAVEIT, ID_FILE_SAVEIT,
			NULL,__ECTN_BIG_ACCENT_IN_MENU,	NULL,_T(RIBBON_SAVE));
	pNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE( ID_FB_SAVE ));
	pNode->m_iconBig.m_bmpNormal.Make32();
	pNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonNodeFile->InsertNode(NULL, pNode);

	pNode =	new CExtRibbonNode(ID_FILE_SAVEAS, ID_FILE_SAVEAS,
			NULL,__ECTN_BIG_ACCENT_IN_MENU,	NULL,_T(RIBBON_SAVEAS));
	pNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE( ID_FB_SAVE_AS ));
	pNode->m_iconBig.m_bmpNormal.Make32();
	pNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonNodeFile->InsertNode(NULL, pNode);

	pNode =	new CExtRibbonNode(ID_PREFERENCES,ID_PREFERENCES,
			NULL,__ECTN_BIG_ACCENT_IN_MENU,	NULL,_T(RIBBON_PREFERENCES));
	pNode->m_iconBig.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE(ID_CONFIGURE));
	pNode->m_iconBig.m_bmpNormal.Make32();
	pNode->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pNode->SetTextMenuCaption( _T("&Preferences") );
	pRibbonNodeFile->InsertNode(NULL, pNode);

	return pRibbonNodeFile;
}

void CXRibbonBar::OnRibbonGalleryItemSelEndOK(
   CExtRibbonGalleryWnd & wndRG,
   CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
   CExtRibbonButtonGallery * pRibbonGalleryTBB,
   CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI
)
{
	ASSERT_VALID((&wndRG));
	UINT nCmdID = 0;

	if(pRibbonGalleryTBB != NULL)
	{
		ASSERT_VALID(pRibbonGalleryTBB);
		nCmdID = pRibbonGalleryTBB->GetCmdID();
	} 

	else
	{
		ASSERT(pGalleryPopup != NULL);
		CExtCustomizeCmdTreeNode * pNode = pGalleryPopup->_CmdNodeGet();
		ASSERT_VALID(pNode);
		nCmdID = pNode->GetCmdID();
	}

	switch(nCmdID)
	{
		case IDFR_INSERT:
			{
				OINFO* pInfo = GetOINFO(pTBCI->LPARAM_Get());

				char *insert = new char[1024];
				CString fileName(pInfo->extFileName);
				CString file = fileName.Right(fileName.GetLength() - fileName.ReverseFind('\\') - 1);
				strcpy(insert, (LPCTSTR)file);
				gInsert = insert;
			} break;

		default:
			{
				// Recent file; get which it was
				int ID = pTBCI->LPARAM_Get();

				CString Number;
				Number.Format("%d", ID + 1);
				CString File = g_MainFrame->m_INI.GetString("Recent", Number);

				if (File == "None" || File == "") return; 

				// Verify this file exists
				if (!FileExists(File)) return;

				pGalleryPopup->DestroyWindow();
				g_MainFrame->DoLoad(File);
			} break;
	}
}

void CXRibbonBar::OnRibbonGalleryInitContent(
	CExtRibbonGalleryWnd & wndRG,
	CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
	CExtRibbonButtonGallery * pRibbonGalleryTBB
	)
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT_VALID( (&wndRG) );
	ASSERT( wndRG.GetSafeHwnd() != NULL );

	UINT nCmdID = 0;

	if(pRibbonGalleryTBB != NULL)
	{
		ASSERT_VALID( pRibbonGalleryTBB );
		nCmdID = pRibbonGalleryTBB->GetCmdID();
	} 

	else
	{
		ASSERT( pGalleryPopup != NULL );
		CExtCustomizeCmdTreeNode * pNode = pGalleryPopup->_CmdNodeGet();
		ASSERT_VALID( pNode );
		nCmdID = pNode->GetCmdID();
	}

	switch( nCmdID )
	{
	case IDFR_INSERT:
		{
			// Audio
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pAudio =
				wndRG.ItemInsert( NULL, _T("Audio"), (HICON) NULL );
			// Controls
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pControls =
				wndRG.ItemInsert( NULL, _T("Controls"), (HICON) NULL );
			// Data files
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pData =
				wndRG.ItemInsert( NULL, _T("Data && Files"), (HICON) NULL );
			// Games
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pGames =
				wndRG.ItemInsert( NULL, _T("Games"), (HICON) NULL );
			// Games
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pGraphics =
				wndRG.ItemInsert( NULL, _T("Graphics"), (HICON) NULL );
			// Input
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pInput =
				wndRG.ItemInsert( NULL, _T("Input"), (HICON) NULL );
			// Internet
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pInternet =
				wndRG.ItemInsert( NULL, _T("Internet"), (HICON) NULL );
			// Other
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pOther =
				wndRG.ItemInsert( NULL, _T("Other"), (HICON) NULL );
			// System
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pSystem =
				wndRG.ItemInsert( NULL, _T("System"), (HICON) NULL );

			POSITION pos = g_dlls->GetStartPosition();
			DWORD    nKey;
			OINFO*   oInfo;
		
			while (pos != NULL)
			{ 
				// locate the object DLL to use
				g_dlls->GetNextAssoc(pos, nKey, oInfo);

				if (!(oInfo->ideFlags & OF_MOVEMENTPLUGIN)) 
				{	
					CPath Path;
					Path.SetToCurrentDirectory();
					CString Base;
					Base.Format("%sPlugins\\%s", Path.GetFullPath(), oInfo->extFileName);

					// Load it into a HINSTANCE
					HINSTANCE curPlugin = ::LoadLibrary(Base);
					if(curPlugin) 
					{
						// Extract the name
						char objName[1024];
						LoadString(curPlugin, OBJ_NAME, objName, 1024);
						char scategory[1024];
						LoadString(curPlugin, OBJ_CATEGORY, scategory, 1024);

						CExtCmdIcon icon;
						icon.AssignFromHBITMAP(oInfo->hSmallIcon, false);

						// Add to list
						CString category = scategory;
						category.MakeLower();

						CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pParent = NULL;

						if (category.Find("audio") != -1)
							pParent = pAudio;

						if (category.Find("control") != -1)
							pParent = pControls;
				
						if (category.Find("game") != -1)
							pParent = pGames;
			
						if (category.Find("input") != -1)
							pParent = pInput;

						if (category.Find("graphics") != -1)
							pParent = pGraphics;

						if (category.Find("internet") != -1)
							pParent = pInternet;
				
						if (category.Find("data") != -1)
							pParent = pData;

						if (category.Find("other") != -1)
							pParent = pOther;

						if (category.Find("system") != -1)
							pParent = pSystem;

						CExtToolBoxWnd::TOOLBOX_ITEM_DATA* pItem = wndRG.ItemInsert(pParent, objName, &icon);	
						pItem->LPARAM_Set(oInfo->oID);

						// Free the current plugin
						::FreeLibrary(curPlugin);	
					}
				}
			}

			// Remove empty groups
			if (pAudio->GetChildCount(false) == 0)
				wndRG.ItemRemove(pAudio, false, true);
			if (pControls->GetChildCount(false) == 0)
				wndRG.ItemRemove(pControls, false, true);
			if (pGames->GetChildCount(false) == 0)
				wndRG.ItemRemove(pGames, false, true);
			if (pInput->GetChildCount(false) == 0)
				wndRG.ItemRemove(pInput, false, true);
			if (pInternet->GetChildCount(false) == 0)
				wndRG.ItemRemove(pInternet, false, true);
			if (pOther->GetChildCount(false) == 0)
				wndRG.ItemRemove(pOther, false, true);
			if (pSystem->GetChildCount(false) == 0)
				wndRG.ItemRemove(pSystem, false, true);
			if (pData->GetChildCount(false) == 0)
				wndRG.ItemRemove(pData, false, true);
			if (pGraphics->GetChildCount(false) == 0)
				wndRG.ItemRemove(pGraphics, false, true);
			wndRG.UpdateToolBoxWnd( true );
		}
		break; 

	default:
		if((pGalleryPopup->TrackFlagsGet()&TPMX_RIBBON_FILE_MENU))
		{
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup =
				wndRG.ItemInsert( NULL, _T("Recent Documents"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup != NULL );

			INT nItemIndex, nItemCount = 10;
			for(nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++)
			{
				CString str;

				// Get the file at this position
				CString ID;
				ID.Format("%d", nItemIndex + 1);
				CString File = g_MainFrame->m_INI.GetString("Recent", ID);
				//CPath Path;
				//Path.SetPath(File);

				// Show full path - AG
				TCHAR shortPath[MAX_PATH];
				PathCompactPathEx(shortPath, File, 59, 0);
				//str.Format(" %d  %s", nItemIndex + 1, Path.GetFileTitle());
				str.Format(" %d  %s", nItemIndex + 1, shortPath);

				CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryItem =
					wndRG.ItemInsert( pTBCI_GalleryGroup, str, (HICON)NULL );

				ASSERT(pTBCI_GalleryItem != NULL);
				pTBCI_GalleryItem;

				pTBCI_GalleryItem->LPARAM_Set(nItemIndex);
			} 

			wndRG.ItemExpand( pTBCI_GalleryGroup );
			wndRG.ItemSetActive( pTBCI_GalleryGroup );
			wndRG.UpdateToolBoxWnd( true );
		}
		break; 
	} 
}

CXRibbonBar::CXRibbonBar()
{
	::GetLocaleInfo(
		::GetSystemDefaultLCID(),
		LOCALE_SDECIMAL,
		m_sDecimalSeparator.GetBuffer( 64 ),
		63
		);
	m_sDecimalSeparator.ReleaseBuffer();
	m_sDecimalSeparator.TrimLeft( _T(" \r\n\t") );
	m_sDecimalSeparator.TrimRight( _T(" \r\n\t") );
	if( m_sDecimalSeparator.GetLength() == 0 )
		m_sDecimalSeparator = _T('.');

	m_hIconFontTT = AfxGetApp()->LoadIcon(IDI_ICON_FONT_TT);
	ASSERT( m_hIconFontTT != NULL );
	m_hIconFontPix = AfxGetApp()->LoadIcon(IDI_ICON_FONT_PIX);
	ASSERT( m_hIconFontPix != NULL );

	m_sCurrentFontFaceName = _T("Arial");
	m_lfCurrentFontSize = 12.0f;

	//m_clrFontColor = RGB(255,0,0);
	//m_clrTextHighlightColor = RGB(255,255,0);
}

CXRibbonBar::~CXRibbonBar()
{
	::DestroyIcon(m_hIconFontTT);
	::DestroyIcon(m_hIconFontPix);
}

void CXRibbonBar::Init()
{
	m_pRibbonNode = new CExtRibbonNode;
	CExtRibbonNodeTabPageCollection* pRibbonNodeTabPageCollection = new CExtRibbonNodeTabPageCollection;
	m_pRibbonNode->InsertNode( NULL, pRibbonNodeTabPageCollection );

	CExtRibbonNodeFile * pRibbonNodeFile = _InitRibbon_FileMenu();
	m_pRibbonNode->InsertNode( NULL, pRibbonNodeFile );

	// Construct page
	CXRibbonPage Construct(AddPage(RIBBON_HOME));

	Construct.AddGroup(RIBBON_CLIPBOARD);
	Construct.AddItemToGroup(0, ID_EDIT_UNDO , RIBBON_UNDO , ID_QA_EDIT_UNDO   , true, false );
	Construct.AddItemToGroup(0, ID_EDIT_REDO , RIBBON_REDO , ID_QA_EDIT_REDO   , true, false );
	Construct.AddSeparatorToGroup(0);
	Construct.AddItemToGroup(0, ID_EDIT_PASTE, RIBBON_PASTE, ID_EDIT_PASTE_BIG , false, false );
	Construct.AddSeparatorToGroup(0);
	Construct.AddItemToGroup(0, ID_EDIT_COPY , RIBBON_COPY , ID_EDIT_COPY_SMALL , true, false);
	Construct.AddItemToGroup(0, ID_EDIT_CUT  , RIBBON_CUT  , ID_EDIT_CUT_SMALL, true, false);
	Construct.AddItemToGroup(0, ID_EDIT_DELETE , RIBBON_DELETE  , ID_RIBBON_DELETE, true, false);

	Construct.AddGroup(RIBBON_BARS);
	Construct.AddItemToGroup(1, ID_VIEW_LAYERBAR, BAR_LAYERS, ID_LAYERBAR, true, false );
	Construct.AddItemToGroup(1, ID_VIEW_SIDEBAR, BAR_PROJECT, ID_PROJECTBAR, true, false );
	Construct.AddItemToGroup(1, ID_VIEW_PROPERTIESBAR, BAR_PROPERTIES, ID_PROPERTYBAR, true, false );
	Construct.AddSeparatorToGroup(1);
	//Construct.AddItemToGroup(1, ID_VIEW_RESOURCEBAR, BAR_RESOURCES, ID_RESOURCEBAR, true, false );
	Construct.AddItemToGroup(1, ID_VIEW_ANIMBAR, BAR_ANIMATOR, ID_RESOURCEBAR, true, false );

	Construct.AddGroup(RIBBON_ONLINE);
	Construct.AddItemToGroup(2, ID_WEB_SCIRRACOM, "Scirra.com", ID_NEWOBJECT, true, false );
	Construct.AddItemToGroup(2, ID_WEB_FORUMS, RIBBON_FORUMS, ID_NEWOBJECT, true, false );
	Construct.AddItemToGroup(2, ID_WEB_REPORTBUG, RIBBON_REPORTBUG, ID_NEWOBJECT, true, false );
	Construct.AddSeparatorToGroup(2);
	Construct.AddItemToGroup(2, ID_WEB_HELP, RIBBON_HELP, ID_APP_ABOUT_16x16, true, false );
	//Construct.AddItemToGroup(2, ID_EXCHANGE, EE_TITLE, ID_RIBBON_CONNECT, true, false);
	Construct.AddItemToGroup(2, ID_WEB_UPDATE, RIBBON_CHECKFORUPDATES, ID_DEFAULTGRID, true, false );

	pRibbonNodeTabPageCollection->InsertNode(NULL, Construct.m_pPage);

	// Project page
	CXRibbonPage Project(AddPage(RIBBON_PROJECT));

	Project.AddGroup(RIBBON_RUN);
	Project.AddItemToGroup(0, ID_PREVIEW, RIBBON_RUNALL, ID_PREVIEW_BIG, false, false);
	Project.AddItemToGroup(0, ID_PREVIEWLAYOUT, RIBBON_RUNLAYOUT, ID_PREVIEW_BIG, false, false);
	Project.AddItemToGroup(0, ID_DEBUG, RIBBON_DEBUGALL, ID_DEBUG_BIG, false, false);
	Project.AddItemToGroup(0, ID_DEBUGLAYOUT, RIBBON_DEBUGLAYOUT, ID_DEBUG_BIG, false, false);
	Project.AddSeparatorToGroup(0);
	Project.AddItemToGroup(0, ID_EXPORT_EXE, RIBBON_EXPORTTOEXE, ID_EXPORT_EXE_BIG, false, false);

	Project.AddGroup(RIBBON_TRANSITIONS);
	Project.AddItemToGroup(1, ID_TRANSITIONS, RIBBON_ADDTRANSITION, ID_ADDTRANSITION, false, false);

	Project.AddGroup(RIBBON_FAMILIES);
	Project.AddItemToGroup(2, ID_MANAGEFAMILIES, RIBBON_MANAGEFAMILIES, ID_MANAGEFAMILIES, false, false);

	pRibbonNodeTabPageCollection->InsertNode(NULL, Project.m_pPage);

	// Insert page
	CXRibbonPage Insert(AddPage(LAYOUT));

	Insert.AddGroup(OBJECTS);
	Insert.AddItemToGroup(0, IDFR_ARRAYPASTE, RIBBON_ARRAYPASTE, ID_ARRAYPASTE, true);
	Insert.AddItemToGroup(0, IDFR_INSERT, RIBBON_INSERTOBJECT, ID_NEWOBJECT, true);

	Insert.AddGroup(RIBBON_TAGS);
	Insert.AddItemToGroup(1, IDFR_INSERTTAG, RIBBON_ADDTAG, ID_TAG, false, false);

	Insert.AddGroup(RIBBON_GRID);
	Insert.AddItemToGroup(2, ID_TOGGLEGRID, RIBBON_TOGGLEGRID, ID_GRID, false, false);
	Insert.AddSeparatorToGroup(2);
	Insert.AddItemToGroup(2, ID_EDITGRID, RIBBON_EDITGRID, ID_EDIT_SMALL, true);
	Insert.AddCheckBoxToGroup(2, ID_GRID_SNAPMOVEMENT, RIBBON_SNAPTOMOVING);
	Insert.AddCheckBoxToGroup(2, ID_GRID_SNAPRESIZE, RIBBON_SNAPTORESIZING);

	Insert.AddGroup(RIBBON_SELECTION);
	Insert.AddItemToGroup(3, ID_EDIT_SELECTALL, RIBBON_SELECTALL, ID_SELECTALL, true);
	Insert.AddItemToGroup(3, ID_EDIT_UNSELECT, RIBBON_UNSELECTALL, ID_UNSELECTALL, true);
	Insert.AddItemToGroup(3, ID_EDIT_INVERTSELECTION, RIBBON_INVERTSELECTION, ID_RIBBON_UNGROUP, true);

	pRibbonNodeTabPageCollection->InsertNode(NULL, Insert.m_pPage);

	// Objects page
	CXRibbonPage Objects(AddPage(OBJECTS));

	Objects.AddGroup(RIBBON_ORDER);
	Objects.AddItemToGroup(0, IDFR_ORDERFORWARD, RIBBON_MOVEFORWARD, ID_RIBBON_MOVEFORWARD, true);
	Objects.AddItemToGroup(0, IDFR_ORDERBACKWARD, RIBBON_MOVEBACKWARD, ID_RIBBON_MOVEBACKWARD, true);
	Objects.AddSeparatorToGroup(0);
	Objects.AddItemToGroup(0, IDFR_ORDERFRONT, RIBBON_BRINGTOFRONT, ID_ARRANGE_BTF_LARGE);
	Objects.AddItemToGroup(0, IDFR_ORDERBACK , RIBBON_SENDTOBACK, ID_ARRANGE_STB_LARGE);

	Objects.AddGroup("Behaviors");
	Objects.AddItemToGroup(1, IDFR_NEWMOVEMENT, RIBBON_ADDBEHAVIOR, ID_ADDFRAME, true);
	Objects.AddItemToGroup(1, IDFR_REMOVEALLMOVEMENTS, RIBBON_REMOVEALLBEHAVIORS, ID_DELETE_SMALL, true);

	Objects.AddGroup("Effects");
	Objects.AddItemToGroup(2, IDFR_NEWEFFECT, RIBBON_ADDEFFECT, ID_ADDFRAME, true);
	Objects.AddItemToGroup(2, IDFR_REMOVEALLEFFECTS, RIBBON_REMOVEALLEFFECTS, ID_DELETE_SMALL, true);

	Objects.AddGroup(RIBBON_INFORMATION);
	Objects.AddItemToGroup(3, IDFR_OBJECTINFORMATION, RIBBON_INFORMATION, ID_OBJECTINFO, false, false);

	pRibbonNodeTabPageCollection->InsertNode(NULL, Objects.m_pPage);

	// Events page
	CXRibbonPage Events(AddPage(RIBBON_EVENTS));

	Events.AddGroup(RIBBON_STRUCTURE);
	Events.AddItemToGroup(0, ID_RIBBON_ADDEVENT, RIBBON_ADDEVENT, ID_RIBBON_ADDEVENT, true);
	Events.AddItemToGroup(0, ID_RIBBON_ADDSUBEVENT, RIBBON_ADDSUBEVENT, ID_RIBBON_ADDSUBEVENT, true);
	Events.AddItemToGroup(0, ID_RIBBON_ADDCONDITION, RIBBON_ADDCONDITION, ID_RIBBON_ADDCONDITION, true);
	Events.AddSeparatorToGroup(0);
	Events.AddItemToGroup(0, ID_RIBBON_ADDGROUP, RIBBON_ADDGROUP, ID_RIBBON_ADDGROUP, false, false);
	Events.AddSeparatorToGroup(0);
	Events.AddItemToGroup(0, ID_RIBBON_TOGGLEEVENT, RIBBON_TOGGLEEVENT, ID_RIBBON_TOGGLEEVENT, false, false);
	Events.AddItemToGroup(0, ID_RIBBON_FINDREPLACE, RIBBON_FINDANDREPLACE, ID_ZOOM_LARGE, false, false);

	Events.AddGroup(RIBBON_COMMENTS);
	Events.AddItemToGroup(1, ID_EXPAND, RIBBON_ADDCOMMENT, ID_COMMENT_NEW_LARGE);

	Events.AddGroup(RIBBON_VIEW);
	Events.AddItemToGroup(2, ID_CHANGEALLTO_CHRONOLOGICAL, RIBBON_CHRONOLOGICAL, ID_CHRONO_LARGE, false);
	Events.AddItemToGroup(2, ID_CHANGEALLTO_LIST, RIBBON_LIST, ID_LIST_LARGE, false);

	Events.AddGroup(RIBBON_BOOKMARKS);
	Events.AddItemToGroup(3, ID_BOOKMARK, RIBBON_BOOKMARK, ID_BOOKMARK_LARGE, false);
	Events.AddItemToGroup(3, ID_REMOVEBOOKMARKS, RIBBON_REMOVEBOOKMARKS, ID_DELETE_SMALL, true);
	Events.AddItemToGroup(3, ID_NEXTBOOKMARK, RIBBON_NEXTBOOKMARK, ID_DOWN_SMALL, true);
	Events.AddItemToGroup(3, ID_PREVIOUSBOOKMARK, RIBBON_PREVIOUSBOOKMARK, ID_UP_SMALL, true);

	pRibbonNodeTabPageCollection->InsertNode(NULL, Events.m_pPage);
	
	CExtRibbonNodeRightButtonsCollection * pRibbonNodeRightButtonsCollection = new CExtRibbonNodeRightButtonsCollection;
	CExtRibbonNode * pNodeAppAbout =new CExtRibbonNode(ID_APP_ABOUT,ID_APP_ABOUT,NULL,0L,_T(A_TITLE));
	pNodeAppAbout->m_iconSmall.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE( ID_APP_ABOUT_16x16 ));
	pNodeAppAbout->m_iconSmall.m_bmpNormal.PreMultiplyRGBChannels( false );
	pRibbonNodeRightButtonsCollection->InsertNode(NULL,pNodeAppAbout);

	CExtRibbonNodeMdiRightButtons * pNodeMDI =new CExtRibbonNodeMdiRightButtons;
	pRibbonNodeRightButtonsCollection->InsertNode(NULL,pNodeMDI);
	m_pRibbonNode->InsertNode( NULL, pRibbonNodeRightButtonsCollection );

	CExtRibbonNodeQuickAccessButtonsCollection * pRibbonNodeQuickAccessButtonsCollection =new CExtRibbonNodeQuickAccessButtonsCollection;
	CExtRibbonNode* pNew = new CExtRibbonNode(ID_FILE_SAVEIT, ID_FILE_SAVEIT,	NULL,0L,_T(RIBBON_SAVE));
	pNew->m_iconSmall.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE(ID_QA_FILE_SAVE));
	pNew->m_iconSmall.m_bmpNormal.Make32();
	pNew->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonNodeQuickAccessButtonsCollection->InsertNode(NULL,pNew);

 	pNew = new CExtRibbonNode(ID_PREVIEW, ID_PREVIEW,	NULL,0L,_T(RIBBON_RUN));
	pNew->m_iconSmall.m_bmpNormal.LoadBMP_Resource(MAKEINTRESOURCE(ID_PREVIEWSMALL));
	pNew->m_iconSmall.m_bmpNormal.Make32();
	pNew->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonNodeQuickAccessButtonsCollection->InsertNode(NULL,pNew);
	m_pRibbonNode->InsertNode( NULL, pRibbonNodeQuickAccessButtonsCollection );

	// Save to ribbon
	SetButtons(m_pRibbonNode);
	MenuInfoGetDefault()->AccelTableLoad(IDR_MAINFRAME);

	Ribbon_FileButtonGet()->m_iconForLargeMode.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE( IDB_CONSTRUCTICON ) );
}