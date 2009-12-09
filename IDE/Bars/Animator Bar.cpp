// Includes
#include "stdafx.h"
#include "Animator Bar.h"
#include "..\MainFrm.h"
#include "..\Dialogs\ImageEditorDlg.h"
#include "..\Dialogs\AngleDlg.h"
#include "..\Dialogs\ImportImagesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern PropertiesBar*	g_PropertyBar;
extern CLayerBar*		g_pLayerBar;
extern CMainFrame*		pMainWnd;

#include "..\ChildFrame.h"
#include "..\Construct.h"

// Image editor include
#include "..\Editors\ImageEditor.h"

class CAnimEdFrame : public CObject
{
public:
	AnimatorBar* m_anim_ed;
	CAnimEdFrame()
	{
		m_anim_ed = 0;
	}
	virtual void Serialize(CArchive& ar)
	{
		m_anim_ed->FrameSerialize(ar);
	}
};

/////////////////////////////////////////////////////////////////////////////
// AnimatorBar & AnimatorBar

AnimatorBar::AnimatorBar()
	: CExtResizableDialog(IDD_ANIMATORBAR, NULL)
{
	application = NULL;
	layout = NULL;
	m_LockChange = false;
}

BEGIN_MESSAGE_MAP(AnimatorBar, CExtResizableDialog)
	//{{AFX_MSG_MAP(AnimatorBar)
	// Dialog notifications
	ON_WM_CREATE()
	ON_WM_SIZE()

	// Tree notifications
	ON_NOTIFY(TVN_SELCHANGED, IDC_ANIMATIONS, OnAnimationChanged)
	ON_NOTIFY(NM_CLICK, IDC_ANIMATIONS, OnAnimationLClick)
	ON_NOTIFY(NM_RCLICK, IDC_ANIMATIONS, OnRClickAnimation)

	// Drag/drop animations
	ON_REGISTERED_MESSAGE(WM_XHTMLTREE_BEGIN_DRAG, OnBeginDrag)
	ON_REGISTERED_MESSAGE(WM_XHTMLTREE_END_DRAG, OnEndDrag)

	// Film strip notifications
	ON_NOTIFY(NM_DBLCLK, IDC_FRAMES, OnDblClickFilmStrip)
	ON_NOTIFY(NM_RCLICK, IDC_FRAMES, OnRClickFilmStrip)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FRAMES, OnClickFilmStrip)
	ON_NOTIFY(LVN_KEYDOWN, IDC_FRAMES, OnKeyDownFilmStrip)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_FRAMES, OnBegindragFrames)

	ON_BN_CLICKED(IDC_PACKAGE, OnBnClickedPackage)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void AnimatorBar::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AnimatorBar)
	DDX_Control(pDX, IDC_ANIMATIONS, animations);
	DDX_Control(pDX, IDC_FRAMES, film_strip);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// AnimatorBar message handlers

////////////////////////////////////////////////////////////////////////////
// Animation functions
////////////////////////////////////////////////////////////////////////////
void AnimatorBar::OnBnClickedPackage()
{
	MessageBox("Sorry this feature is incomplete",0,0);
}

void AnimatorBar::AddAnimation()
{
	HTREEITEM Item = curHTreeItem;
	HTREEITEM Parent = 0;

	if (Item) 
	{
		if (Item == TVI_ROOT)
			Parent = 0;
		else
			Parent = animations.GetParentItem(Item);
	}

	CAnimation* pAnimation;
	
	if(Parent)
		pAnimation = AddAnimation((CAnimation*)animations.GetItemData(Parent));
	else
		pAnimation = AddAnimation(m_pAnimation);	

	UpdateAnimations(NULL, NULL, NULL, -2);

	// Select animation
	animations.SelectItem(pAnimation->m_Tree);
}

CAnimation* AnimatorBar::AddAnimation(CAnimation* pParent)
{
	// Create a satisfactory name
	CString Name;
	Name.Format("%s %d", AB_ANIMATION, pParent->m_SubAnimations.size());

	// Set up CAnimation
	CAnimation Animation;
	Animation.m_FixedID	= application->m_AnimationFixID++;
	Animation.application	= pParent->application;
	Animation.m_Name	= Name;

	// Add a default angle too
	Animation.m_SubAnimations.push_back(CAnimation());

	CAnimation& Angle = Animation.m_SubAnimations.back();

	Angle.m_IsAngle = true;
	Angle.m_Angle = 0;
	Angle.m_FixedID = application->m_AnimationFixID++;
	Angle.application = application;

	// And a frame
	bool			Found  = false;
	CImageResource* pImage = 0;

	int ID = 0;
	ID = layout->AddImage(32, 32);

	Angle.m_Images.push_back(ID);
	Angle.m_FrameTimes.push_back(1.0);

	// Push back
	pParent->m_SubAnimations.push_back(Animation);

	AnimationHasChanged();
	return &pParent->m_SubAnimations.back();
}

void AnimatorBar::AddSubAnimation()
{
	HTREEITEM Item = curHTreeItem;

	if(((CAnimation*)animations.GetItemData(Item))->IsAngle())
		Item = animations.GetParentItem(Item);

	CAnimation* pAnimation;

	if(Item) 
		pAnimation = AddAnimation((CAnimation*)animations.GetItemData(Item));
	else
		pAnimation = AddAnimation(m_pAnimation);

	UpdateAnimations(NULL, NULL,NULL, -2);

	// Select animation
	animations.SelectItem(pAnimation->m_Tree);
}

void AnimatorBar::AddAngle()
{
	HTREEITEM Item = curHTreeItem;

	if(((CAnimation*)animations.GetItemData(Item))->IsAngle())
		Item = animations.GetParentItem(Item);

	CAnimation* pAngle;

	bool changed = false;

	if(Item)
	{
		CAngleDlg AddAngle;
		if (AddAngle.DoModal() != IDOK)
			return;

		CAnimation* pAnimation = (CAnimation*)animations.GetItemData(Item);

		map<float,int> CurrentAngles;

		for(list<CAnimation>::iterator a = pAnimation->m_SubAnimations.begin(); a!= pAnimation->m_SubAnimations.end(); a++)
			if(a->IsAngle())
				CurrentAngles[a->m_Angle] = 1;

		for(vector<float>::iterator i = AddAngle.m_Angles.begin(); i!= AddAngle.m_Angles.end(); i++)
		{
			if(CurrentAngles.find(*i) == CurrentAngles.end())
			{
				pAnimation->m_SubAnimations.push_back(CAnimation());

				CAnimation& Angle = pAnimation->m_SubAnimations.back();

				Angle.m_IsAngle = true;
				Angle.m_Angle = *i;
				Angle.m_FixedID = application->m_AnimationFixID++;
				CurrentAngles[*i] = 1;
				Angle.application = pAnimation->application;

				// Add a frame
				int ID = 0;
				ID = layout->AddImage(32, 32);

				Angle.m_Images.push_back(ID);
				Angle.m_FrameTimes.push_back(1.0);

				pAngle = &Angle;

				changed = true;
			}
		}
	}

	if (changed)
	{
		AnimationHasChanged();
		UpdateAnimations(NULL, NULL, NULL, -2);

		// Select animation
		animations.SelectItem(pAngle->m_Tree);
	}
}

void AnimatorBar::AddFrame()
{
	bool			Found  = false;
	CImageResource* pImage = 0;

	if (m_pCurrentAnimation->m_Images.size() != 0)
		pImage = application->resources.FindImageResourceFromNumber(m_pCurrentAnimation->m_Images[m_pCurrentAnimation->m_Images.size() - 1]);

	int ID = 0;

	if (pImage)
		ID = layout->AddImage(pImage->bitmap.GetWidth(), pImage->bitmap.GetHeight());
	else
		ID = layout->AddImage(32, 32);

	m_pCurrentAnimation->m_Images.push_back(ID);
	m_pCurrentAnimation->m_FrameTimes.push_back(1.0);

	layout->EditImageInAnimation(m_pCurrentAnimation->m_FixedID, ID);

	UpdateFilmStrip();
}

void AnimatorBar::RemoveFrame()
{
	vector<int> FramesToDelete;

	// verify that not all frames are selected (Fix, 0.96.2)
	if (film_strip.GetSelectedCount() == film_strip.GetItemCount())
	{
		message_bar.Attach(film_strip);
		message_bar.SetHighlightOnMouseOver();
		message_bar.SetWrapText();
		message_bar.SetText(AB_CANNOTDELETEDESC);

		return;
	}

	POSITION Pos = film_strip.GetFirstSelectedItemPosition();

	if(!m_pCurrentAnimation)
		return;

	while(Pos)
	{
		int Item = film_strip.GetNextSelectedItem(Pos);
		FramesToDelete.push_back(Item);
	}

	if (film_strip.GetItemCount() == 1)
	{
		message_bar.Attach(film_strip);
		message_bar.SetHighlightOnMouseOver();
		message_bar.SetWrapText();
		message_bar.SetText(AB_CANNOTDELETEDESC);

		return;
	}

	// Delete backwards otherwise the rest of the frames go out of alignment
	vector<int>& Images = m_pCurrentAnimation->m_Images;
	vector<float>& FrameTime = m_pCurrentAnimation->m_FrameTimes;

	vector<int>::reverse_iterator i = FramesToDelete.rbegin();

	for ( ; i != FramesToDelete.rend(); i++) {
		Images.erase(Images.begin() + *i);
		FrameTime.erase(FrameTime.begin() + *i);
	}

	AnimationHasChanged();
	UpdateFilmStrip();
}

void AnimatorBar::RemoveAnimation()
{
	HTREEITEM Item = animations.GetSelectedItem();
	if(!Item) 
		return;

	HTREEITEM Parent = animations.GetParentItem(Item);

	CAnimation* pChildAnim = (CAnimation*)animations.GetItemData(Item);
	CAnimation* pParentAnim;

	if(Parent)
		pParentAnim = (CAnimation*)animations.GetItemData(Parent);
	else
	{
		pParentAnim = m_pAnimation;

		if (pParentAnim->m_SubAnimations.size() == 1)
		{
			message_bar.Attach(this);
			message_bar.SetHighlightOnMouseOver();
			message_bar.SetWrapText();
			message_bar.SetText(AB_CANNOTDELETEANIMATIONDESC);

			return;
		}
	}

	if (pChildAnim->IsAngle())
	{
		// Do a quick check for other angles
		bool bFound = false;

		for(list<CAnimation>::iterator i = pParentAnim->m_SubAnimations.begin(); i != pParentAnim->m_SubAnimations.end(); i ++)
			if(i->IsAngle() && i->m_FixedID != pChildAnim->m_FixedID)
				bFound = true;

		if (!bFound)
		{
			message_bar.Attach(this);
			message_bar.SetHighlightOnMouseOver();
			message_bar.SetWrapText();
			message_bar.SetText(AB_CANNOTDELETEANGLEDESC);

			return;
		}
	}

	for(list<CAnimation>::iterator i = pParentAnim->m_SubAnimations.begin();
		i != pParentAnim->m_SubAnimations.end();
		i ++)
	{
		if(&(*i) == pChildAnim)
		{
			pParentAnim->m_SubAnimations.erase(i);
			i = pParentAnim->m_SubAnimations.end();
		}
	}

	AnimationHasChanged();
	UpdateFilmStrip();
	UpdateAnimations(NULL, NULL, NULL, -2);
}

////////////////////////////////////////////////////////////////////////////
// Film strip
////////////////////////////////////////////////////////////////////////////
void AnimatorBar::OnKeyDownFilmStrip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	// Check modifier
	bool bControlDown = GetKeyState(VK_CONTROL) >> 4;

	LV_KEYDOWN* pKeyDown = (LV_KEYDOWN*)pNMHDR;
	WORD Test = pKeyDown->wVKey;

	// delete, delete
	if (Test == VK_DELETE)
		RemoveFrame();

	// C, copy
	if (Test == 0x43)
	{
		CAnimEdFrame* pFrames = new CAnimEdFrame;
		pFrames->m_anim_ed = this;
		DROPEFFECT de = DROPEFFECT_NONE;
		m_pDDMgr.PrepareDrop(DO_CLIPBOARD, "Construct Frames", pFrames, &de);
	}

	// X, cut
	if (Test == 0x78 && film_strip.GetItemCount() != 1)
	{
		CAnimEdFrame* pFrames = new CAnimEdFrame;
		pFrames->m_anim_ed = this;
		DROPEFFECT de = DROPEFFECT_NONE;
		m_pDDMgr.PrepareDrop(DO_CLIPBOARD, "Construct Frames", pFrames, &de);

		RemoveFrame();
	}

	// V, paste
	if (Test == 0x56)
	{
		if (m_pDDMgr.OkToPaste())
		{
			CAnimEdFrame pFrames;
			pFrames.m_anim_ed = this;
			m_pDDMgr.DoDrop(&pFrames,
							  NULL,
							  "Construct Frames");
		}
	}

	*pResult = 0;
}

void AnimatorBar::OnDblClickFilmStrip(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION Pos = film_strip.GetFirstSelectedItemPosition();
	int Item = film_strip.GetNextSelectedItem(Pos);

	if(Item == -1)
		return;

	layout->EditImageInAnimation(m_pCurrentAnimation->m_FixedID, Item);

	*pResult = 0;

	AnimationHasChanged();
	UpdateFilmStrip();
}

void AnimatorBar::OnRClickFilmStrip(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pCurrentAnimation == NULL)
		return;

	POINT MousePosition;
	GetCursorPos(&MousePosition);

	CExtPopupMenuWnd * popup = new CExtPopupMenuWnd;
	popup->LoadMenu(m_hWnd, IDR_BLANK, true, false);
	popup->ItemRemove(0);

	UINT ChosenItem = 0;

	// Add frame
	popup->ItemInsertCommand(1, -1, "Add frame", NULL, NULL);

	bool bIsSel = false;

	POSITION Pos = film_strip.GetFirstSelectedItemPosition();

	if(!m_pCurrentAnimation)
		return;

	if(!m_pCurrentAnimation->IsAngle())
		return;

	while(Pos)
	{
		int Item = film_strip.GetNextSelectedItem(Pos);
		if (Item != -1)
			bIsSel = true;
	}

	// Import
	popup->ItemInsertCommand(3, -1, AB_IMPORTFRAME, NULL, NULL);
	popup->ItemBoldGet(0);

	// Copy/paste
	popup->ItemInsertCommand();

	// Remove frame(s)
	if (bIsSel)
		popup->ItemInsertCommand(2, -1, AB_REMOVEFRAME, NULL, NULL);

	if (film_strip.GetItemCount() != 1)
		popup->ItemInsertCommand(7, -1, AB_CUTFRAMES, NULL, NULL);

	popup->ItemInsertCommand(4, -1, AB_COPYFRAMES, NULL, NULL);
	popup->ItemInsertCommand(5, -1, AB_PASTEFRAMES, NULL, NULL);
	popup->ItemInsertCommand(10, -1, AB_DUPLICATEFRAMES, NULL, NULL);

	popup->ItemInsertCommand();

	// flip vertical/horizontal
	popup->ItemInsertCommand(8, -1, AB_MIRRORFRAMES, NULL, NULL);
	popup->ItemInsertCommand(9, -1, AB_FLIPFRAMES, NULL, NULL);

	// Explore
	popup->ItemInsertCommand();
	popup->ItemInsertCommand(6, -1, AB_LAUNCHEXPLORER, NULL, NULL);

	// Explore
	popup->ItemInsertCommand();
	popup->ItemInsertCommand(11, -1, AB_COPYCOLLISIONMASKTOEACHFRAME, NULL, NULL);


	// Show menu
	popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND | TPMX_NO_CMD_UI, MousePosition.x, MousePosition.y, NULL, NULL, NULL, &ChosenItem);
		
	if (ChosenItem == 1)
		AddFrame();

	else if (ChosenItem == 2)
		RemoveFrame();

	else if (ChosenItem == 3)
	{
		CImportImagesDlg ImportDlg;
		if (ImportDlg.DoModal() == IDOK)
		{			
			for(list<CImageResource>::iterator i = ImportDlg.m_Images.begin(); i!= ImportDlg.m_Images.end(); i++)
			{
				int id = layout->AddImage(i->bitmap.GetWidth(), i->bitmap.GetHeight());

				m_pCurrentAnimation->m_Images.push_back(id);
				m_pCurrentAnimation->m_FrameTimes.push_back(1.0);

				CImageResource* image = application->resources.FindImageResourceFromNumber(id);
				image->bitmap.Copy(i->bitmap);

				image->m_Action = i->m_Action;
				image->m_Hotspot = i->m_Hotspot;
				image->m_Collision = i->m_Collision;
				
			}
			
			UpdateFilmStrip();
		}
	}

	// copy
	else if (ChosenItem == 4)
	{	
		CAnimEdFrame* pFrames = new CAnimEdFrame;
		pFrames->m_anim_ed = this;
		DROPEFFECT de = DROPEFFECT_NONE;
		m_pDDMgr.PrepareDrop(DO_CLIPBOARD, "Construct Frames", pFrames, &de);
	}

	// cut
	else if (ChosenItem == 7)
	{
		CAnimEdFrame* pFrames = new CAnimEdFrame;
		pFrames->m_anim_ed = this;
		DROPEFFECT de = DROPEFFECT_NONE;
		m_pDDMgr.PrepareDrop(DO_CLIPBOARD, "Construct Frames", pFrames, &de);

		RemoveFrame();
	}

	// paste
	else if (ChosenItem == 5)
	{
		if (m_pDDMgr.OkToPaste())
		{
			CAnimEdFrame pFrames;
			pFrames.m_anim_ed = this;
			m_pDDMgr.DoDrop(&pFrames,
							  NULL,
							  "Construct Frames");
		}
	}

	// duplicate
	else if (ChosenItem == 10)
	{	
		CAnimEdFrame* pFrames = new CAnimEdFrame;
		pFrames->m_anim_ed = this;
		DROPEFFECT de = DROPEFFECT_NONE;
		m_pDDMgr.PrepareDrop(DO_CLIPBOARD, "Construct Frames", pFrames, &de);

		if (m_pDDMgr.OkToPaste())
		{
			CAnimEdFrame pFrames;
			pFrames.m_anim_ed = this;
			m_pDDMgr.DoDrop(&pFrames,
							  NULL,
							  "Construct Frames");
		}
	}
	// Copy collision mask to every other frame
	else if (ChosenItem == 11)
	{
		// Get the frame we right clicked on
		POSITION pos = film_strip.GetFirstSelectedItemPosition();
		if(pos)
		{
			int nItem = film_strip.GetNextSelectedItem(pos);
			int handle = m_pCurrentAnimation->m_Images.at(nItem);
		
			int result = MessageBox("Click 'OK' to confirm you want to copy the collision mask of this selected frame to every other frame in the animation package", "Construct", MB_OKCANCEL);

			if(result == IDOK){
				CImageResource* res = application->resources.FindImageResourceFromNumber(handle);
				if(!res)
					MessageBox("Error. Image resource could not be found for this animation frame");
				else
				{
					if(!res->m_Collision.IsValid())
						res->m_Collision = res->bitmap; // copy
					
					application->resources.CopyCollisionMaskToEveryFrameInAnimation(m_pAnimation, res); 

				}
			
				
		}
		}

	}

	// Launch explorer
	else if (ChosenItem == 6)
	{
		CPath Path;
		Path.SetToCurrentDirectory();
		
		CString InfoPath;
		InfoPath.Format("%sImages\\", Path.GetFullPath());

		// make sure directory exists
		::CreateDirectory(InfoPath, NULL);

		// now open file
		InfoPath += "frames.txt";
		CFile File(InfoPath, CFile::modeWrite | CFile::modeCreate);
	

		int index = 0;
		for(vector<int>::iterator i = m_pCurrentAnimation->m_Images.begin(); i!= m_pCurrentAnimation->m_Images.end(); i++)
		{
			CImageResource* img = application->resources.FindImageResourceFromNumber(*i);
			if(img)
			{
				CString nl;
				nl.Format("%c%c", 13,10);
				index ++;
				CString SavePath;
				SavePath.Format("%sImages\\Frame %d.png", Path.GetFullPath(), index);
				CxImage backup = img->bitmap;
				img->Predivide();
				img->bitmap.Save(SavePath,CXIMAGE_FORMAT_PNG);
				img->bitmap = backup;

				if(img->m_Collision.IsValid())
				{
					SavePath.Format("%sImages\\Mask %d.png", Path.GetFullPath(), index);
					img->m_Collision.Save(SavePath,CXIMAGE_FORMAT_PNG);
				}
				else
				{
					SavePath.Format("%sImages\\Mask %d.png", Path.GetFullPath(), index);
					img->bitmap.Save(SavePath,CXIMAGE_FORMAT_PNG);
				}


				CString Info;
				Info.Format("Frame %d%sPivotX%s%d%sPivotY%s%d%s", index, nl,nl, img->m_Hotspot.x, nl,nl, img->m_Hotspot.y, nl);
				for(map<CString, CPoint>::iterator a = img->m_Action.begin(); a!= img->m_Action.end(); a++)
				{
					Info.Format("%s%s%s%d%s%d%s", Info, nl, a->first, nl, a->second.x, nl, a->second.y, nl);
				}
				Info += nl;
				File.Write((const char*)Info, Info.GetLength() * sizeof(char));


			}
		}
		File.Close();

		CString LaunchPath;
		LaunchPath.Format("%sImages\\", Path.GetFullPath());

		ShellExecute(NULL,  "open", LaunchPath, NULL, NULL, SW_SHOW);

		int result = MessageBox("Click 'OK' to reimport the images that have been exported to Explorer.", "Construct", MB_OKCANCEL);

		if(result == IDOK)
		{
			int result2 = MessageBox("Import collision mask?", "Construct", MB_YESNOCANCEL);
			int index = 0;
			for(vector<int>::iterator i = m_pCurrentAnimation->m_Images.begin(); i!= m_pCurrentAnimation->m_Images.end(); i++)
			{
				index ++;
				CImageResource* img = application->resources.FindImageResourceFromNumber(*i);
			
				CString SavePath;
				SavePath.Format("%sImages\\Frame %d.png", Path.GetFullPath(), index);			
				
				img->bitmap.Load(SavePath,CXIMAGE_FORMAT_PNG);
				img->Premultiply();

				if(result2 == IDYES)
				{
				SavePath.Format("%sImages\\Mask %d.png", Path.GetFullPath(), index);			
				img->m_Collision.Load(SavePath,CXIMAGE_FORMAT_PNG);
				}
				if(result2 == IDNO)
				{
					if(img->m_Collision.IsValid())
						img->m_Collision.Destroy();
				}

				application->m_image_to_texture.erase(img->m_FixedID);

				img->small_thumbnail.Destroy();
				img->large_thumbnail.Destroy();
			}

			application->resources.images_changed = true;

			AnimationHasChanged();
			UpdateFilmStrip();
		}
	}

	else if (ChosenItem == 8)
	{
		// mirror
		POSITION position = film_strip.GetFirstSelectedItemPosition();

		if(!m_pCurrentAnimation)
			return;

		vector<int>& images = m_pCurrentAnimation->m_Images;

		while(position)
		{
			int item = film_strip.GetNextSelectedItem(position);

			if (item == -1) break;

			CImageResource* image = application->resources.FindImageResourceFromNumber(images[item]);
			image->Mirror();

			application->resources.images_changed = true;
		}

		AnimationHasChanged();
		UpdateFilmStrip();
	}

	else if (ChosenItem == 9)
	{
		// mirror
		POSITION position = film_strip.GetFirstSelectedItemPosition();

		if(!m_pCurrentAnimation)
			return;

		vector<int>& images = m_pCurrentAnimation->m_Images;

		while(position)
		{
			int item = film_strip.GetNextSelectedItem(position);

			if (item == -1) break;

			CImageResource* image = application->resources.FindImageResourceFromNumber(images[item]);
			image->Flip();

			application->resources.images_changed = true;
		}

		AnimationHasChanged();
		UpdateFilmStrip();
	}
}

void AnimatorBar::UpdateFilmStrip()
{
	film_strip.SetRedraw(false);
	m_ImageList.DeleteImageList();
	film_strip.DeleteAllItems();

	if(!m_pCurrentAnimation)
		return;
	
	m_ImageList.Create(32, 32, ILC_COLOR24, 3, 3);
	film_strip.SetImageList(&m_ImageList, LVSIL_NORMAL);
	
	for (int i = 0; i < m_pCurrentAnimation->m_Images.size(); i++)
	{
		// Create thumbnail & add to filmstrip
		CImageResource* pImage = application->resources.FindImageResourceFromNumber(m_pCurrentAnimation->m_Images[i]);

		if(pImage)
		{
			HBITMAP Bitmap = pImage->GetHBitmap(true,false);
			ImageList_Add(m_ImageList.m_hImageList, Bitmap, Bitmap);


			CString Name;
			Name.Format("%d", i + 1);
			film_strip.InsertItem(i, Name, i);
		}
	}

	film_strip.SetRedraw(true);
	film_strip.Invalidate();
}

void AnimatorBar::OnClickFilmStrip(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION Pos = film_strip.GetFirstSelectedItemPosition();
	int Item = film_strip.GetNextSelectedItem(Pos);

	if(Item == -1)
		return;

	if(layout)
	{
		if(layout->m_ObjectFrameIsLockedTo != 0) // InputLocked()
		{
			return;
		}
	}

	// Show properties in property bar
	g_PropertyBar->m_pAnimation = m_pCurrentAnimation;
	g_PropertyBar->m_iIndex = Item;
	g_PropertyBar->Update(NULL, TYPE_ANIMATION, NULL, NULL, NULL, application);

	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////
// Animations drag/drop
////////////////////////////////////////////////////////////////////////////
LRESULT AnimatorBar::OnBeginDrag(WPARAM wParam, LPARAM lParam)
{
	XHTMLTREEMSGDATA *pMsg = (XHTMLTREEMSGDATA *) wParam;
	ASSERT(pMsg);

	XHTMLTREEDRAGMSGDATA *pData = (XHTMLTREEDRAGMSGDATA *) lParam;

	LRESULT lResult = 0;

	if (pMsg && pData)
	{
		// If it's an angle, and there's no other angles..
		CAnimation* pAnimation = (CAnimation*)animations.GetItemData(pData->hItem);

		if (pAnimation->IsAngle())
		{
			// Get parent
			CAnimation* pParent = (CAnimation*)animations.GetItemData(animations.GetParentItem(pData->hItem));

			if (pParent->m_SubAnimations.size() == 1)
				lResult = 1;
		}
	}

	return lResult;	// return 0 to allow drag
}

LRESULT AnimatorBar::OnEndDrag(WPARAM wParam, LPARAM lParam)
{
	XHTMLTREEMSGDATA *pMsg = (XHTMLTREEMSGDATA *) wParam;
	ASSERT(pMsg);

	XHTMLTREEDRAGMSGDATA *pData = (XHTMLTREEDRAGMSGDATA *) lParam;

	LRESULT lResult = 0;

	if (pMsg)
	{
		if (pData)
		{
			// Get the animation to be dropped
			CAnimation* pAnimation = (CAnimation*)animations.GetItemData(pData->hItem);
			CAnimation New = *pAnimation;

			// Find it and remove it from wherever it is
			application->resources.DeleteAnimationFromNumber(pAnimation->m_FixedID);

			// Now find a parent
			HTREEITEM Parent = pData->hNewParent;
			CAnimation* pParent = m_pAnimation;

			if (Parent != TVI_ROOT)
				pParent = (CAnimation*)animations.GetItemData(pData->hNewParent);

			// Now find where under this parent we need to insert
			if (pData->hAfter == TVI_FIRST)
				pParent->m_SubAnimations.push_front(New);
				
			else if (pData->hAfter == TVI_LAST)
				pParent->m_SubAnimations.push_back(New);

			else
			{
				// Custom location somewhere
				CAnimation* pAfter = (CAnimation*)animations.GetItemData(pData->hAfter);

				if (find(pParent->m_SubAnimations.begin(), pParent->m_SubAnimations.end(), *pAfter) != pParent->m_SubAnimations.end())
					pParent->m_SubAnimations.insert(++find(pParent->m_SubAnimations.begin(), pParent->m_SubAnimations.end(), *pAfter), New);
				else
					pParent->m_SubAnimations.push_back(New);
			}

			if (Parent != TVI_ROOT)
				animations.Expand(Parent, TVE_EXPAND);

			AnimationHasChanged();
		}
	}

	return lResult;	// return 0 to allow drop
}

////////////////////////////////////////////////////////////////////////////
// Animations tree
////////////////////////////////////////////////////////////////////////////
void AnimatorBar::OnAnimationChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	if(layout)
	{
		if(layout->m_ObjectFrameIsLockedTo != 0) // InputLocked()
		{
			return;
		}
	}

	// Set current animation
	m_pCurrentAnimation = (CAnimation*)animations.GetItemData(pNMTreeView->itemNew.hItem);

	if(!m_LockChange)
	{
		// Show properties in property bar
		g_PropertyBar->m_pAnimation = m_pCurrentAnimation;
		g_PropertyBar->m_iIndex = -1;
		g_PropertyBar->Update(NULL, TYPE_ANIMATION, NULL, NULL, NULL, application);
	}

	UpdateFilmStrip();
}

// On left click, update properties again.
void AnimatorBar::OnAnimationLClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	if(layout)
	{
		if(layout->m_ObjectFrameIsLockedTo != 0) // InputLocked()
		{
			return;
		}
	}

	HTREEITEM hItem = animations.GetSelectedItem();

	if (hItem == NULL) return;

	m_pCurrentAnimation = (CAnimation*)animations.GetItemData(hItem);

	if (m_pCurrentAnimation == NULL) return;

	if(!m_LockChange)
	{
		// Show properties in property bar
		g_PropertyBar->m_pAnimation = m_pCurrentAnimation;
		g_PropertyBar->m_iIndex = -1;
		g_PropertyBar->Update(NULL, TYPE_ANIMATION, NULL, NULL, NULL, application);
	}

	UpdateFilmStrip();
}

void AnimatorBar::OnRClickAnimation(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (application == NULL)
		return;

	CPoint Pt;
	GetCursorPos(&Pt);
	animations.ScreenToClient(&Pt);
	if (animations.HitTest(Pt))
		animations.SelectItem(animations.HitTest(Pt));

	curHTreeItem = animations.GetSelectedItem();

	// Check for this
	if (!animations.HitTest(Pt))
	{
		curHTreeItem = TVI_ROOT;
		m_pCurrentAnimation = m_pAnimation;
	}

	else
		m_pCurrentAnimation = (CAnimation*)animations.GetItemData(curHTreeItem);

	POINT MousePosition;
	GetCursorPos(&MousePosition);

	CExtPopupMenuWnd * popup = new CExtPopupMenuWnd;
	popup->LoadMenu(m_hWnd, IDR_BLANK, true, false);
	popup->ItemRemove(0);

	UINT ChosenItem = 0;

	// Add menu items
	popup->ItemInsertCommand(1, -1, AB_ADDNEWANIMATION, NULL, NULL);
	
	if (curHTreeItem != TVI_ROOT)
		popup->ItemInsertCommand(2, -1, AB_ADDSUBANIMATION, NULL, NULL);

	if (curHTreeItem != TVI_ROOT)
	{
		popup->ItemInsert();
		popup->ItemInsertCommand(3, -1, AB_ADDNEWANGLE, NULL, NULL);
	}

	if (curHTreeItem != TVI_ROOT)
	{
		popup->ItemInsert();
		popup->ItemInsertCommand(4, -1, REMOVE, NULL, NULL);
	}

	popup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND | TPMX_NO_CMD_UI, MousePosition.x, MousePosition.y, NULL, NULL, NULL, &ChosenItem);
		
	if (ChosenItem == 1)
		AddAnimation();

	else if (ChosenItem == 2)
		AddSubAnimation();

	else if (ChosenItem == 3)
		AddAngle();

	else if (ChosenItem == 4)
		RemoveAnimation();
}

void AnimatorBar::AddAnimationToTree(CAnimation* pAnimation, HTREEITEM parent)
{
	HTREEITEM item;
	if(pAnimation->IsAngle())
	{
		CString AngleName;
		AngleName.Format("%s: %.3f", AB_ANGLE, pAnimation->m_Angle);

		// Strip trailing zeroes
		if (AngleName.Find('.') != -1) {
			while (AngleName.Right(1) == "0")
				AngleName.Delete(AngleName.GetLength() - 1);
			if (AngleName.Right(1) == ".")
				AngleName.Delete(AngleName.GetLength() - 1);
		}

		AngleName += "°";

		if(pAnimation->m_Angle == 0)
			AngleName += " (" + AB_RIGHT + ")";
		if(pAnimation->m_Angle == 90)
			AngleName += " (" + AB_BOTTOM + ")";
		if(pAnimation->m_Angle == 180)
			AngleName += " (" + AB_LEFT + ")";
		if(pAnimation->m_Angle == 270)
			AngleName += " (" + AB_TOP + ")";

		item = animations.InsertItem(AngleName, parent);
	}

	else
	{
		if (parent != TVI_ROOT)
			item = animations.InsertItem(pAnimation->m_Name, 2, 2, parent);
		else
			item = animations.InsertItem(pAnimation->m_Name, 1, 1, parent);
	}

	animations.SetItemData(item, (DWORD)pAnimation);

	vector<CAnimation*> angles;
	vector<CAnimation*> subanimations;
	for(list<CAnimation>::iterator i = pAnimation->m_SubAnimations.begin(); i != pAnimation->m_SubAnimations.end(); i++)
	{
		if(i->m_IsAngle)
			angles.push_back(&*i);
		else
			subanimations.push_back(&*i);
	}

	while(angles.size() > 0)
	{
		vector<CAnimation*>::iterator smallest = angles.begin();

		for(vector<CAnimation*>::iterator a = angles.begin(); a != angles.end(); a++)
		{
			if((*smallest)->m_Angle > (*a)->m_Angle)
				smallest = a;
		}

		AddAnimationToTree(*smallest, item);
		angles.erase(smallest);
	}

	// Now simply add subanimations below
	for(vector<CAnimation*>::iterator s = subanimations.begin(); s != subanimations.end(); s++)
		AddAnimationToTree(*s, item);

	pAnimation->m_Tree = item;
}

void AnimatorBar::UpdateAnimations(CApplication* pApp, CLayout* pFrame,  CObjType* pType, int iHandle)
{
	m_LockChange = true;

	m_pObjType = pType; //dont worry what value it is

	if (iHandle != -2 && iHandle != -1)
	{
		application = pApp;
		layout = pFrame;
	}

	bool bDoSave = false;

	if(pApp)
	{
		CAnimation* pAnimation = pApp->resources.FindAnimationFromNumber(iHandle);
		if (pAnimation == m_pAnimation)
			bDoSave = true;
	}

	if(animations.GetSelectedItem())
		m_SelectedItem[m_iHandle] = (CAnimation*)animations.GetItemData(animations.GetSelectedItem());

	m_iHandle = iHandle;

	if(m_SelectedItem.find(iHandle) == m_SelectedItem.end())
		m_SelectedItem[iHandle] = NULL;

	if(application)
	{
		HTREEITEM Iterate = animations.GetNextItem(animations.GetRootItem(), TVGN_ROOT);
		while(Iterate != NULL)
		{
			CAnimation* pAnim = (CAnimation*)animations.GetItemData(Iterate);
			if(pAnim)
			{
				if (animations.GetItemState(Iterate, TVIF_STATE) & TVIS_EXPANDED)
					pAnim->m_Expanded = true;
				else
					pAnim->m_Expanded = false;
			}
		

			HTREEITEM IterateChild;

			IterateChild = animations.GetChildItem(Iterate);
			if(!IterateChild)
			{
				bool Loop = true;
				while(Loop)
				{
					IterateChild = animations.GetNextItem(Iterate, TVGN_NEXT);
					if(IterateChild)
						Loop = false;
					else
						IterateChild = animations.GetParentItem(Iterate);
					if(!IterateChild)
						Loop = false;
					Iterate = IterateChild;
				}
			}

			Iterate = IterateChild;
		}
	}

	animations.DeleteAllItems();
	film_strip.DeleteAllItems();

	// Do we have a valid handle?
	if (application)
	{
		if (iHandle >= 0)
			m_pAnimation = application->resources.FindAnimationFromNumber(iHandle);
		else if (iHandle == -1)
			return;

		if(!m_pAnimation->m_AllowSubanimations)
			AddAnimationToTree(m_pAnimation, TVI_ROOT);

		for(list<CAnimation>::iterator i = m_pAnimation->m_SubAnimations.begin(); i!= m_pAnimation->m_SubAnimations.end(); i++)
		{
			if(i->IsAngle())
				continue;
			
			AddAnimationToTree(&*i, TVI_ROOT);
		}
	}

	// Restore if necessary
	//if (bDoSave)
	{
		HTREEITEM Iterate = animations.GetNextItem(animations.GetRootItem(), TVGN_ROOT);
		while(Iterate != NULL)
		{

			CAnimation* pAnim = (CAnimation*)animations.GetItemData(Iterate);

			if(pAnim)
			{		
				if( pAnim->m_Expanded)
					animations.Expand(Iterate, TVE_EXPAND);
			
				if(m_SelectedItem[iHandle])
				{
					if(pAnim == m_SelectedItem[iHandle])
						animations.SelectItem(Iterate);
				}
				else
				{
					if(pAnim->m_SubAnimations.size() == 0)
						if(pAnim->m_Images.size())
						{
							m_SelectedItem[iHandle] = pAnim;
							animations.SelectItem(Iterate);
						}
				}

			}
			HTREEITEM IterateChild;

			IterateChild = animations.GetChildItem(Iterate);
			if(!IterateChild)
			{
				bool Loop = true;
				while(Loop)
				{
					IterateChild = animations.GetNextItem(Iterate, TVGN_NEXT);
					if(IterateChild)
						Loop = false;
					else
						IterateChild = animations.GetParentItem(Iterate);
					if(!IterateChild)
						Loop = false;
					Iterate = IterateChild;
				}
			}

			Iterate = IterateChild;
		}
	
	}

	m_LockChange = false;
}

////////////////////////////////////////////////////////////////////////////
// Dialog handlers
////////////////////////////////////////////////////////////////////////////
void AnimatorBar::OnSize(UINT nType, int cx, int cy) 
{
	// Get rect
	CRect Rect;
	GetClientRect(&Rect);

	if (Rect.Height() < 250)
	{
		animations.MoveWindow(0, 0, Rect.Width(), (Rect.Height() / 2) - 2);
		film_strip.MoveWindow(0, Rect.Height() / 2, Rect.Width(), Rect.Height() / 2);
	}

	else
	{
		animations.MoveWindow(0, 0, Rect.Width(), 250);
		film_strip.MoveWindow(0, 252, Rect.Width(), Rect.Height() - 252);
	}
}

int AnimatorBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CExtResizableDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pDDMgr.AddFormat("Construct Frames");
	m_pDDMgr.AddFormat("Construct Animations");

	animations.ModifyStyleEx(0, WS_EX_STATICEDGE);
	film_strip.ModifyStyleEx(0, WS_EX_STATICEDGE);

	m_font.CreateFont(14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	animations.SetFont(&m_font);

	m_bDragging = false;
	m_nDragIndex = -1;
	m_nDropIndex = -1;
	m_pDragImage = NULL;

	return 0;
}

BOOL AnimatorBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Images.Create(16, 16, ILC_COLOR24|ILC_MASK, 0, 0);
	CBitmap bm;
	bm.LoadBitmap(IDB_ANIMICONS);
	m_Images.Add(&bm, RGB(0, 0, 0));

	TreeView_SetImageList(animations, m_Images, TVSIL_NORMAL);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
// Bar handlers
////////////////////////////////////////////////////////////////////////////

void AnimatorBar::AnimationHasChanged()
{
	if(m_pObjType)
	{
		OINFO* info = GetOINFO(m_pObjType->DLLIndex);

		vector<CObj*> objs;
		CObj* o;

		POSITION pos = application->layouts.GetHeadPosition();
		while(pos)
		{
			CLayout* layout = application->layouts.GetNext(pos);
			
			POSITION pos2 = layout->objects.GetStartPosition();

			for (int i = 0; i < layout->objects.GetCount(); i++) 
			{
				long ID;
				layout->objects.GetNextAssoc(pos2, ID, o);

				if (o->GetGlobalID() == m_pObjType->ObjectIdentifier)
						objs.push_back(o);

			}
		}

		// Now loop each of the objects and call the end animation function.
		for(vector<CObj*>::iterator i = objs.begin(); i!= objs.end(); i++)
		{
			if(info->ETOnNotify)
				info->ETOnNotify((*i)->editObject, 2);
		}
		if(objs.size() > 0)
			if(info->ETOnNotify)
				info->ETOnNotify((*objs.begin())->editObject, 3);
	}
}

void AnimatorBar::FrameSerialize(CArchive& ar)
{
	if(ar.IsLoading())
	{
		POSITION Pos = film_strip.GetFirstSelectedItemPosition();
		
		int Count;
		ar >> Count;
		vector<int> ToSelect;

		for(int a = 0; a < Count; a ++)
		{
			application->resources.images.push_back(CImageResource());
			application->resources.images_changed = true;
			CImageResource* image = &application->resources.images.back();
			image->Serialize(ar);
			image->m_FixedID = application->m_ImageFixID++;

			POSITION pos = film_strip.GetFirstSelectedItemPosition();
		
			if(pos)
			{
				int nItem = film_strip.GetNextSelectedItem(pos);
				m_pCurrentAnimation->m_Images.insert( m_pCurrentAnimation->m_Images.begin() + nItem + a, image->m_FixedID);
				ToSelect.push_back(nItem + a);
			}

			else
			{
				m_pCurrentAnimation->m_Images.push_back( image->m_FixedID);
				ToSelect.push_back(m_pCurrentAnimation->m_Images.size());
			}			

			int iFrameTime = 0;
			ar >> iFrameTime;
			m_pCurrentAnimation->m_FrameTimes.push_back(iFrameTime);
		}

		UpdateFilmStrip();
		for(int c = 0 ; c < ToSelect.size(); c++)
			film_strip.SetItemState (ToSelect[c], LVIS_SELECTED, LVIS_SELECTED);
	}

	else
	{
		POSITION pos = film_strip.GetFirstSelectedItemPosition();
		ar << (int) film_strip.GetSelectedCount();
		while (pos)
		{
			int nItem = film_strip.GetNextSelectedItem(pos);
			CImageResource* image = application->resources.FindImageResourceFromNumber(m_pCurrentAnimation->m_Images.at(nItem));
			image->Serialize(ar);
			ar << (int)m_pCurrentAnimation->m_FrameTimes[nItem];
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// Drag/drop handlers
////////////////////////////////////////////////////////////////////////////
void AnimatorBar::OnBegindragFrames(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nDragIndex = pNMListView->iItem;

	//// Create a drag image
	POINT pt;
	int nOffset = -10;
	pt.x = nOffset;
	pt.y = nOffset;

	// Set flags
	m_bDragging = TRUE;	
	m_nDropIndex = -1;	
	m_pDragList = &film_strip; 
	m_pDropWnd = &film_strip;	

	SetCapture ();

	*pResult = 0;
}

void AnimatorBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		// Move the drag image
		CPoint pt(point);	
		ClientToScreen(&pt); 
		//m_pDragImage->DragMove(pt);

		//m_pDragImage->DragShowNolock(false);

		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd); //make sure we have a window

		if (pDropWnd != m_pDropWnd)
		{
			if (m_nDropIndex != -1)
			{
				TRACE("m_nDropIndex is -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));

				VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));
				pList->UpdateWindow ();
				m_nDropIndex = -1;
			}

			else 
			{
				TRACE("m_nDropIndex is not -1\n");

				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				int i = 0;
				int nCount = pList->GetItemCount();

				for(i = 0; i < nCount; i++)
					pList->SetItemState(i, 0, LVIS_DROPHILITED);

				pList->RedrawItems(0, nCount);
				pList->UpdateWindow();
			}
		}

		m_pDropWnd = pDropWnd;
		pDropWnd->ScreenToClient(&pt);

		if(pDropWnd->IsKindOf(RUNTIME_CLASS (CListCtrl)))
		{			
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			UINT uFlags;
			CListCtrl* pList = (CListCtrl*)pDropWnd;

			pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
			pList->RedrawItems (m_nDropIndex, m_nDropIndex);
			
			// Get the item that is below cursor
			m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);

			if(m_nDropIndex >= 0)
				pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);

			pList->RedrawItems(m_nDropIndex, m_nDropIndex);
			pList->UpdateWindow();
		}

		else
			SetCursor(LoadCursor(NULL, IDC_NO));

		//m_pDragImage->DragShowNolock(true);
	}
		
	CDialog::OnMouseMove(nFlags, point);
}

void AnimatorBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		//m_pDragImage->DragLeave (GetDesktopWindow ());
		//m_pDragImage->EndDrag ();
		//delete m_pDragImage;

		CPoint pt (point); 
		ClientToScreen (&pt); 

		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd); 

		if(m_nDropIndex != -1)	
			if (pDropWnd->IsKindOf (RUNTIME_CLASS (CListCtrl)))
			{
				m_pDropList = (CListCtrl*)pDropWnd; 
				DropItemOnList(m_pDragList, m_pDropList); 
			}
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

typedef struct {
	LVITEM* plvi;
	CString sCol2;
} lvItem, *plvItem;

void AnimatorBar::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)
{
	vector<int> framesToDelete;

	POSITION pos = film_strip.GetFirstSelectedItemPosition();

	if(!m_pCurrentAnimation)
		return;

	while(pos)
	{
		int item = film_strip.GetNextSelectedItem(pos);
		framesToDelete.push_back(item);
	}

	
	vector<int> newimages;
	vector<float> newframetimes;

	vector<int>& images = m_pCurrentAnimation->m_Images;
	vector<float>& frametimes = m_pCurrentAnimation->m_FrameTimes;

	vector<int>::reverse_iterator i = framesToDelete.rbegin();


	for (; i != framesToDelete.rend(); i++)
	{
		newimages.push_back(*(images.begin() + *i));
		newframetimes.push_back(*(frametimes.begin() + *i));

		images.erase(images.begin() + *i);
		frametimes.erase(frametimes.begin() + *i);
	}

	vector<int>::iterator v = newimages.begin();
	vector<float>::iterator vt = newframetimes.begin();

	if(m_nDropIndex > images.size()+1)
		m_nDropIndex = images.size()+1;

	for ( ; v != newimages.end(); v++,vt++)
	{
		if(m_nDropIndex > m_pCurrentAnimation->m_Images.size())
		{
			m_pCurrentAnimation->m_Images.push_back(*v);
			m_pCurrentAnimation->m_FrameTimes.push_back(*vt);
			m_nDropIndex--;
		}
		else
		{
			m_pCurrentAnimation->m_Images.insert(images.begin() + m_nDropIndex, *v);
			m_pCurrentAnimation->m_FrameTimes.insert(frametimes.begin() + m_nDropIndex, *vt);
		}
	}

	UpdateFilmStrip();

	for(int a = m_nDropIndex; a < m_nDropIndex + newimages.size(); a++)
		pDropList->SetItemState (a, LVIS_SELECTED, LVIS_SELECTED);
}