// Dialogs\AnimationEditorDlg.cpp : implementation file
// 

#include "stdafx.h"
#include "..\Construct.h"
#include "AnimationEditorDlg.h"
#include "ImageEditorDlg.h"
#include "ImportImagesDlg.h"
#include "AngleDlg.h"
// CAnimationEditorDlg dialog
IMPLEMENT_DYNAMIC(CAnimationEditorDlg, CDialog)

CAnimationEditorDlg::CAnimationEditorDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CAnimationEditorDlg::IDD, pParent)
{
	m_CurrentAnimation = 0;
}

CAnimationEditorDlg::~CAnimationEditorDlg()
{
}
 
void CAnimationEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATIONTREE, m_AnimationTree);
	DDX_Control(pDX, IDC_FILMSTRIP, m_Frames);
	DDX_Control(pDX, IDC_PREVIEW, m_Preview);
	DDX_Control(pDX, IDC_ADDFRAME, m_AddFrame);
	DDX_Control(pDX, IDC_REMOVEFRAME, m_RemoveFrame);
	DDX_Control(pDX, IDC_TIME, m_FrameTime);
	DDX_Control(pDX, IDC_ANIMSPEED, m_AnimSpeedSlider);
	DDX_Control(pDX, IDC_SPEED, m_AnimSpeed);
	DDX_Control(pDX, IDC_REPEAT, m_AnimRepeat);
	DDX_Control(pDX, IDC_BACKTO, m_AnimBackTo);
	DDX_Control(pDX, IDC_ADDANIMATION, m_AddAnim);
	DDX_Control(pDX, IDC_SUBANIMATION, m_SubAnim);
	DDX_Control(pDX, IDC_ADDDIRECTION, m_AddDir);
	DDX_Control(pDX, IDC_ANIMATIONREMOVE, m_RemoveAnim);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_BACKTO_SELECT, m_AnimBackToSelect);

	DDX_Control(pDX, IDC_FOREVER, m_AnimForever);
	DDX_Control(pDX, IDC_PINGPONG, m_AnimPingPong);

	DDX_Control(pDX, ID_COPY, m_Copy);
	DDX_Control(pDX, ID_PASTE, m_Paste);
	DDX_Control(pDX, IDC_IMPORT, m_Import);

	DDX_Control(pDX, IDC_PLAY, m_Play);

	DDX_Control(pDX, IDC_LABEL1, m_Label1);
	DDX_Control(pDX, IDC_LABEL2, m_Label2);
	DDX_Control(pDX, IDC_LABEL3, m_Label3);
	DDX_Control(pDX, IDC_LABEL4, m_Label4);

}

BEGIN_MESSAGE_MAP(CAnimationEditorDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_FILMSTRIP, &CAnimationEditorDlg::OnNMDblclkFilmstrip)
	ON_NOTIFY(NM_CLICK, IDC_FILMSTRIP, &CAnimationEditorDlg::OnClickFilmstrip)
	ON_BN_CLICKED(IDC_ADDFRAME, OnAddFrame)
	ON_BN_CLICKED(IDC_REMOVEFRAME, OnRemoveFrame)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGED, IDC_ANIMATIONTREE, &CAnimationEditorDlg::OnTvnSelchangedAnimationtree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_ANIMATIONTREE, &CAnimationEditorDlg::OnLvnBeginlabelEdit)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_ANIMATIONTREE, &CAnimationEditorDlg::OnLvnEndlabelEdit)
	ON_BN_CLICKED(IDC_ADDANIMATION, &CAnimationEditorDlg::OnBnClickedAddanimation)
	ON_BN_CLICKED(IDC_SUBANIMATION, &CAnimationEditorDlg::OnBnClickedSubanimation)
	ON_BN_CLICKED(IDC_ADDDIRECTION, &CAnimationEditorDlg::OnBnClickedAdddirection)
	ON_BN_CLICKED(IDC_ANIMATIONREMOVE, &CAnimationEditorDlg::OnBnClickedAnimationremove)
	ON_BN_CLICKED(IDC_BACKTO_SELECT, &CAnimationEditorDlg::OnBnClickedBacktoSelect)
	ON_BN_CLICKED(IDC_FOREVER, &CAnimationEditorDlg::OnBnClickedForever)
	ON_BN_CLICKED(IDC_PINGPONG, &CAnimationEditorDlg::OnBnClickedPingpong)
	ON_EN_CHANGE(IDC_REPEAT, &CAnimationEditorDlg::OnEnChangeRepeat)
	ON_EN_CHANGE(IDC_BACKTO, &CAnimationEditorDlg::OnEnChangeBackto)
	ON_EN_CHANGE(IDC_SPEED, &CAnimationEditorDlg::OnEnChangeSpeed)
	ON_EN_CHANGE(IDC_TIME, &CAnimationEditorDlg::OnChangeFrameTime)
	
	ON_NOTIFY(LVN_BEGINDRAG, IDC_FILMSTRIP, OnBegindragFrames)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()

	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_COPY, &CAnimationEditorDlg::OnBnClickedCopy)
	ON_BN_CLICKED(ID_PASTE, &CAnimationEditorDlg::OnBnClickedPaste)
	ON_BN_CLICKED(IDC_IMPORT, &CAnimationEditorDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_PLAY, &CAnimationEditorDlg::OnBnClickedPlay)
	ON_NOTIFY(LVN_KEYDOWN, IDC_FILMSTRIP, &CAnimationEditorDlg::OnKeyDownFilmStrip)

	ON_NOTIFY(NM_RCLICK, IDC_FILMSTRIP, &CAnimationEditorDlg::OnRClickFilmstrip)
END_MESSAGE_MAP()

// CAnimationEditorDlg message handlers
BOOL CAnimationEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_iFrame = 0;
	
	m_AnimSpeedSlider.SetRange(0, 100);
	m_AnimSpeedSlider.SetPos(m_Animation->m_Speed);

	m_AddFrame.SetWindowTextA(PROPERTIES_ADD);
	m_RemoveFrame.SetWindowText(PROPERTIES_REMOVE);

	dlgMan.Load(this->m_hWnd, "Software\\Construct\\AnimationEditorDlg");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDOK, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDCANCEL, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_PLAY, ANCHOR_BOTTOM | ANCHOR_RIGHT);
	dlgAnchor.Add(IDC_FILMSTRIP, ANCHOR_TOPLEFT | ANCHOR_RIGHT | ANCHOR_BOTTOM);

	dlgAnchor.Add(IDC_FRAMETIMELABEL, ANCHOR_RIGHT | ANCHOR_TOP);
	dlgAnchor.Add(IDC_TIME, ANCHOR_RIGHT | ANCHOR_TOP);

	dlgAnchor.Add(ID_COPY, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(ID_PASTE, ANCHOR_LEFT | ANCHOR_BOTTOM);

	/////////////////////////////////////////////
	//// Set up initial variables
	m_bDragging = false;
	m_nDragIndex = -1;
	m_nDropIndex = -1;
	m_pDragImage = NULL;

	m_pDDMgr.AddFormat("Construct Frames");
	m_pDDMgr.AddFormat("Construct Animations");

	FirstRun = true;
	UpdateAnimations();
	if(m_AnimationTree.GetCount() == 0)
		m_CurrentAnimation = m_Animation;
	UpdateList();
	
	return TRUE;
}

void CAnimationEditorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	dlgAnchor.OnSize();	
	Invalidate();
}

void CAnimationEditorDlg::OnDestroy() 
{
	CDialog::OnDestroy();	
	dlgMan.Save();
}

void CAnimationEditorDlg::AddAnimationToTree(CAnimation* anim, HTREEITEM parent)
{
	HTREEITEM item;
	if(anim->IsAngle())
	{
		CString AngleName;
		AngleName.Format("Angle: %.3f", anim->m_Angle);

		// Strip trailing zeroes
		if (AngleName.Find('.') != -1) {
			while (AngleName.Right(1) == "0")
				AngleName.Delete(AngleName.GetLength() - 1);
			if (AngleName.Right(1) == ".")
				AngleName.Delete(AngleName.GetLength() - 1);
		}

		AngleName += "°";

		if(anim->m_Angle == 0)
			AngleName += " (right)";
		if(anim->m_Angle == 90)
			AngleName += " (bottom)";
		if(anim->m_Angle == 180)
			AngleName += " (left)";
		if(anim->m_Angle == 270)
			AngleName += " (top)";

		item = m_AnimationTree.InsertItem(AngleName, parent);
	}
	else
	{
		item = m_AnimationTree.InsertItem(anim->m_Name, parent);

	}
	m_AnimationTree.SetItemData(item, (DWORD)anim);

	vector<CAnimation*> angles;
	vector<CAnimation*> subanimations;
	for(list<CAnimation>::iterator i = anim->m_SubAnimations.begin(); i != anim->m_SubAnimations.end(); i++)
	{
		if(i->m_IsAngle)
			angles.push_back(&*i);
		else
			subanimations.push_back(&*i);
	}

	//This is a sort of 'sort on the fly' function
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
	{
		AddAnimationToTree(*s, item);
	}
}

class CExpand
{
public:
	bool expand;
	vector<CExpand> expands;
};

void SaveLoadTreeExpand(CTreeCtrl* tree, HTREEITEM current, CExpand& expand, bool save)
{
	if(current != TVGN_ROOT)
		expand.expand = tree->GetItemState(current, TVIF_STATE) & TVIS_EXPANDED;

	HTREEITEM child = tree->GetNextItem(current, TVGN_ROOT);
	while(child != NULL)
	{
		expand.expands.push_back(CExpand());
		SaveLoadTreeExpand(tree, child, expand.expands.back(), save);
			
		child = tree->GetNextItem(child, TVGN_NEXT);
	}
}

void CAnimationEditorDlg::UpdateAnimations()
{
	m_AnimationTree.SetRedraw(false);
	
	CExpand expanded;
	DWORD selected = -1;
	if(m_AnimationTree.GetSelectedItem())
		selected = m_AnimationTree.GetItemData(m_AnimationTree.GetSelectedItem());
	if(FirstRun)
	{
		CAnimation* anim = m_Animation;
		//topmost animation we need to find a subanimation that isn't an angle
		for(list<CAnimation>::iterator i = anim->m_SubAnimations.begin(); i!= anim->m_SubAnimations.end(); i++)
		{
			if(i->IsAngle())
				continue;
			
			anim = &*i;
			i = anim->m_SubAnimations.end();
		}

		while(anim->m_SubAnimations.size() != 0)
			anim = &anim->m_SubAnimations.front();

		selected = (DWORD)anim;
	}

	// store what has been expanded (note this is a special type of nesting performed without a nested function
	map<int, bool> ToExpand;
	HTREEITEM Iterate = m_AnimationTree.GetNextItem(m_AnimationTree.GetRootItem(), TVGN_ROOT);
	while(Iterate != NULL)
	{
		if (m_AnimationTree.GetItemState(Iterate, TVIF_STATE) & TVIS_EXPANDED)
		{
			ToExpand[m_AnimationTree.GetItemData(Iterate)] = true;
		}

		HTREEITEM Iterate2;

		Iterate2 = m_AnimationTree.GetChildItem(Iterate);
		if(!Iterate2)
		{
			bool loop = true;
			while(loop)
			{
				Iterate2 = m_AnimationTree.GetNextItem(Iterate, TVGN_NEXT);
				if(Iterate2)
					loop = false;
				else
					Iterate2 = m_AnimationTree.GetParentItem(Iterate);
				if(!Iterate2)
					loop = false;
				Iterate = Iterate2;
			}
		}
		Iterate = Iterate2;
	}

	m_AnimationTree.DeleteAllItems();
	m_Frames.DeleteAllItems();
	m_ImageList.DeleteImageList();

	// End store
	for(list<CAnimation>::iterator i = m_Animation->m_SubAnimations.begin(); i!= m_Animation->m_SubAnimations.end(); i++)
	{
		if(i->IsAngle())
			continue;
		AddAnimationToTree(&*i, TVI_ROOT);
	}

	UpdateList();

	// Recover
	// store what has been expanded (note this is a special type of nesting performed without a nested function
	
	Iterate = m_AnimationTree.GetNextItem(m_AnimationTree.GetRootItem(), TVGN_ROOT);
	while(Iterate != NULL)
	{
		if( ToExpand.find( m_AnimationTree.GetItemData(Iterate)) != ToExpand.end() || FirstRun )
		{
			m_AnimationTree.SetItemState(Iterate, TVIS_EXPANDED, TVIS_EXPANDED);
		}
		if(m_AnimationTree.GetItemData(Iterate) == selected)
		{
			m_AnimationTree.SelectItem(Iterate);
		}

		HTREEITEM Iterate2;

		Iterate2 = m_AnimationTree.GetChildItem(Iterate);
		if(!Iterate2)
		{
			bool loop = true;
			while(loop)
			{
				Iterate2 = m_AnimationTree.GetNextItem(Iterate, TVGN_NEXT);
				if(Iterate2)
					loop = false;
				else
					Iterate2 = m_AnimationTree.GetParentItem(Iterate);
				if(!Iterate2)
					loop = false;
				Iterate = Iterate2;
			}
		}
		Iterate = Iterate2;
	}

	FirstRun = false;

	// End Recover
	m_AnimationTree.SetRedraw(true);
	m_AnimationTree.Invalidate();
}

void CAnimationEditorDlg::UpdateList()
{
	m_Frames.SetRedraw(false);
	m_ImageList.DeleteImageList();
	m_Frames.DeleteAllItems();

	if(!m_CurrentAnimation)
		return;
	
	m_ImageList.Create(32, 32, ILC_COLOR24, 3, 3);
	m_Frames.SetImageList(&m_ImageList, LVSIL_NORMAL);
	
	for (int i = 0; i < m_CurrentAnimation->m_Images.size(); i++)
	{
		// Create thumbnail & add to filmstrip
		CImageResource* image = m_pApplication->FindImageResourceFromNumber(m_CurrentAnimation->m_Images[i]);

		if(image)
		{
				
				HBITMAP Bitmap = image->GetHBitmap(true,false);
				ImageList_Add(m_ImageList.m_hImageList, Bitmap, Bitmap);

				CString name;
				name.Format("%d", i+1);
				m_Frames.InsertItem(i, name, i);
		}
	}

	m_Frames.SetRedraw(true);
	m_Frames.Invalidate();
}

void CAnimationEditorDlg::OnOK()
{
	UpdateAnimationSettings();
	CDialog::OnOK();
}

void CAnimationEditorDlg::OnClickFilmstrip(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION listpos = m_Frames.GetFirstSelectedItemPosition();
	int item = m_Frames.GetNextSelectedItem(listpos);

	if(item == -1)
		return;
	// Update editbox
	CString Time;
	Time.Format("%f", m_CurrentAnimation->m_FrameTimes[item]);
	m_FrameTime.SetWindowText(Time);

	*pResult = 0;
}

void CAnimationEditorDlg::OnAddFrame()
{
	bool Found = false;
	CImageResource* Image = 0;

	if (m_CurrentAnimation->m_Images.size() != 0)
	{
		Image = m_pApplication->FindImageResourceFromNumber(m_CurrentAnimation->m_Images[m_CurrentAnimation->m_Images.size() - 1]);
	}

	int ID = 0;
	if (Image)
		ID = m_pFrame->AddImage(Image->m_Bitmap.GetWidth(), Image->m_Bitmap.GetHeight());
	else
		ID = m_pFrame->AddImage(32, 32);

	m_CurrentAnimation->m_Images.push_back(ID);
	m_CurrentAnimation->m_FrameTimes.push_back(1.0);


	UpdateList();

}

void CAnimationEditorDlg::OnRemoveFrame()
{
	vector<int> framesToDelete;

	POSITION pos = m_Frames.GetFirstSelectedItemPosition();
	if(!m_CurrentAnimation)
		return;
	while(pos)
	{
		int item = m_Frames.GetNextSelectedItem(pos);
		framesToDelete.push_back(item);
	}

	// Delete backwards otherwise the rest of the frames go out of alignment
	vector<int>& images = m_CurrentAnimation->m_Images;

	vector<int>::reverse_iterator i = framesToDelete.rbegin();

	for ( ; i != framesToDelete.rend(); i++) {
		images.erase(images.begin() + *i);
	}

	UpdateList();
}


void CAnimationEditorDlg::OnChangeFrameTime()
{
	POSITION listpos = m_Frames.GetFirstSelectedItemPosition();
	int item = m_Frames.GetNextSelectedItem(listpos);

	if(!m_CurrentAnimation)
		return;
	if(item == -1)
		return;
	
	// Update saved time
	CString Time;
	m_FrameTime.GetWindowText(Time);
	m_CurrentAnimation->m_FrameTimes[item] = atof(Time);

}

void CAnimationEditorDlg::OnNMDblclkFilmstrip(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION listpos = m_Frames.GetFirstSelectedItemPosition();
	int item = m_Frames.GetNextSelectedItem(listpos);

	if(item == -1)
		return;

	m_pFrame->EditImageInAnimation(m_CurrentAnimation->m_FixedID, item);

	*pResult = 0;

	UpdateList();
}

void CAnimationEditorDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(500);

	
	
	


	// Show next bitmap
	if(!m_CurrentAnimation)
	{
		m_Preview.SetBitmap(0);
		return;
	}

	if(m_CurrentAnimation->m_RepeatTo < 1)
		m_CurrentAnimation->m_RepeatTo = 1;
	if(m_CurrentAnimation->m_RepeatTo > m_CurrentAnimation->m_Images.size())
		m_CurrentAnimation->m_RepeatTo = m_CurrentAnimation->m_Images.size();

	if(m_CurrentAnimation->m_Images.size() == 0)
	{
		m_Preview.SetBitmap(0);
		return;
	}

	

	if(m_iPlayForward)
		m_iFrame ++;
	else
		m_iFrame --;

	if (m_iFrame >= m_CurrentAnimation->m_Images.size())
	{		
		if(m_CurrentAnimation->m_bPingPong)
		{
			m_iPlayForward = 1 - m_iPlayForward;
			m_iFrame -=2;
		}
		else
		{
			m_iPlayCount ++;
			m_iFrame = m_CurrentAnimation->m_RepeatTo - 1;
		}
	}
	if(m_iFrame < m_CurrentAnimation->m_RepeatTo-1)
	{
		m_iFrame = m_CurrentAnimation->m_Images.size() - 1;
		m_iPlayForward = 1 - m_iPlayForward;	
		m_iFrame = m_CurrentAnimation->m_RepeatTo-1;
		m_iPlayCount ++;
	}
	
	if(m_iPlayCount < m_CurrentAnimation->m_Repeat || m_CurrentAnimation->m_bForever)
	{
		CImageResource* res = m_pApplication->FindImageResourceFromNumber(m_CurrentAnimation->m_Images[m_iFrame]);
		if(m_Preview.GetBitmap() != res->GetHBitmap(true,true))
			m_Preview.SetBitmap(res->GetHBitmap(true,true));
			
		SetTimer(0, ((int)m_CurrentAnimation->m_FrameTimes[m_iFrame]) * m_CurrentAnimation->m_Speed*20, NULL);
	}
	else
		m_Preview.SetBitmap(NULL);
	CDialog::OnTimer(nIDEvent);
}

void CAnimationEditorDlg::OnTvnSelchangedAnimationtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	UpdateAnimationSettings();
	m_CurrentAnimation = (CAnimation*)m_AnimationTree.GetItemData(pNMTreeView->itemNew.hItem);
	
	UpdateList();
	UpdateAnimationUISettings();
}

CAnimation* CAnimationEditorDlg::AddAnimation(CAnimation* parent)
{
	CAnimation anim;
	anim.m_FixedID = m_pApplication->m_AnimationFixID++;
	anim.m_pApp = parent->m_pApp;

	
	parent->m_SubAnimations.push_back(anim);
	return &parent->m_SubAnimations.back();
}

void CAnimationEditorDlg::OnBnClickedAddanimation()
{
	HTREEITEM item = m_AnimationTree.GetSelectedItem();
	HTREEITEM parent = 0;
	if(item) 
		parent = m_AnimationTree.GetParentItem(item);
	if(parent)
		AddAnimation((CAnimation*)m_AnimationTree.GetItemData(parent));
	else
		AddAnimation(m_Animation);	
	
	UpdateAnimations();
}

void CAnimationEditorDlg::UpdateAnimationUISettings()
{
	if(!m_CurrentAnimation)
	{
		m_AnimForever.EnableWindow(false);
		m_AnimPingPong.EnableWindow(false);

		m_AddFrame.EnableWindow(false);
		m_RemoveFrame.EnableWindow(false);
		m_ImportFrame.EnableWindow(false);

		m_AnimBackToSelect.EnableWindow(false);


		m_FrameTime.EnableWindow(false);
		m_AnimSpeed.EnableWindow(false);
		m_AnimRepeat.EnableWindow(false);
		m_AnimBackTo.EnableWindow(false);
		m_AnimSpeedSlider.EnableWindow(false);

		m_SubAnim.EnableWindow(false);
		m_AddDir.EnableWindow(false);

		m_Copy.EnableWindow(false);
		m_Paste.EnableWindow(false);
		m_Import.EnableWindow(false);
		m_Preview.EnableWindow(false);

	}
	else if(m_CurrentAnimation->IsAngle())
	{

		m_AnimForever.EnableWindow();
		m_AnimPingPong.EnableWindow();

		m_AddFrame.EnableWindow();
		m_ImportFrame.EnableWindow();
		m_RemoveFrame.EnableWindow();

		m_AnimBackToSelect.EnableWindow();


		m_FrameTime.EnableWindow();
		m_AnimSpeed.EnableWindow();
		m_AnimRepeat.EnableWindow();
		m_AnimBackTo.EnableWindow();
		m_AnimSpeedSlider.EnableWindow();


		m_AnimForever.SetCheck(m_CurrentAnimation->m_bForever);
		m_AnimPingPong.SetCheck(m_CurrentAnimation->m_bPingPong);

		CString text;
		text.Format("%d", m_CurrentAnimation->m_Repeat);
		m_AnimRepeat.SetWindowText(text);

		text.Format("%d", m_CurrentAnimation->m_RepeatTo);
		m_AnimBackTo.SetWindowText(text);

		text.Format("%d", m_CurrentAnimation->m_Speed);
		m_AnimSpeed.SetWindowText(text);

		m_SubAnim.EnableWindow();
		m_AddDir.EnableWindow();
		m_AddAnim.EnableWindow();

		m_Copy.EnableWindow();
		m_Paste.EnableWindow();
		m_Import.EnableWindow();
		m_Preview.EnableWindow();

	}
	else
	{
		m_AnimForever.EnableWindow(false);
		m_AnimPingPong.EnableWindow(false);

		m_AddFrame.EnableWindow(false);
		m_RemoveFrame.EnableWindow(false);
		m_ImportFrame.EnableWindow(false);

		m_AnimBackToSelect.EnableWindow(false);


		m_FrameTime.EnableWindow(false);
		m_AnimSpeed.EnableWindow(false);
		m_AnimRepeat.EnableWindow(false);
		m_AnimBackTo.EnableWindow(false);
		m_AnimSpeedSlider.EnableWindow(false);

		m_SubAnim.EnableWindow();
		m_AddDir.EnableWindow();
		m_AddAnim.EnableWindow();

		m_Copy.EnableWindow(false);
		m_Paste.EnableWindow(false);
		m_Import.EnableWindow(false);
		m_Preview.EnableWindow(false);
		

	}
}

void CAnimationEditorDlg::OnBnClickedSubanimation()
{
	HTREEITEM item = m_AnimationTree.GetSelectedItem();

	if(((CAnimation*)m_AnimationTree.GetItemData(item))->IsAngle())
		item = m_AnimationTree.GetParentItem(item);
	if(item) 
		AddAnimation((CAnimation*)m_AnimationTree.GetItemData(item));
	else
		OnBnClickedAddanimation();
	
	UpdateAnimations();
}

void CAnimationEditorDlg::OnBnClickedAdddirection()
{
	HTREEITEM item = m_AnimationTree.GetSelectedItem();

	if(((CAnimation*)m_AnimationTree.GetItemData(item))->IsAngle())
		item = m_AnimationTree.GetParentItem(item);

	if(item)
	{
		CAngleDlg dlg;
		dlg.DoModal();

		CAnimation* anim = (CAnimation*)m_AnimationTree.GetItemData(item);
		map<float,int> CurrentAngles;
		for(list<CAnimation>::iterator a = anim->m_SubAnimations.begin(); a!= anim->m_SubAnimations.end();a ++)
		{
			if(a->IsAngle())
				CurrentAngles[a->m_Angle] = 1;
		}
		for(vector<float>::iterator i = dlg.m_Angles.begin(); i!= dlg.m_Angles.end(); i++)
		{
			if(CurrentAngles.find(*i) == CurrentAngles.end() )
			{
				anim->m_SubAnimations.push_back( CAnimation());
				CAnimation& angle = anim->m_SubAnimations.back();
				angle.m_IsAngle = true;
				angle.m_Angle = *i;
				angle.m_FixedID = m_pApplication->m_AnimationFixID++;
				CurrentAngles[*i] = 1;
				angle.m_pApp = anim->m_pApp;
			}
		}
	}
	UpdateAnimations();
}

void CAnimationEditorDlg::OnBnClickedAnimationremove()
{
	HTREEITEM item = m_AnimationTree.GetSelectedItem();
	if(!item) 
		return;

	HTREEITEM parent = m_AnimationTree.GetParentItem(item);


	{
		CAnimation* child_anim = (CAnimation*)m_AnimationTree.GetItemData(item);
		

		CAnimation* parent_anim;
		if(parent)
			parent_anim = (CAnimation*)m_AnimationTree.GetItemData(parent);
		else
			parent_anim = m_Animation;
			

		for(list<CAnimation>::iterator i = parent_anim->m_SubAnimations.begin();
			i != parent_anim->m_SubAnimations.end();
			i ++)
		{
			if(&(*i) == child_anim)
			{
				parent_anim->m_SubAnimations.erase(i);
				i = parent_anim->m_SubAnimations.end();
			}
		}



	}

	UpdateAnimations();
	UpdateList();
	// TODO: Add your control notification handler code here
}

void CAnimationEditorDlg::OnBnClickedBacktoSelect()
{
	POSITION pos = this->m_Frames.GetFirstSelectedItemPosition();
	int index = m_Frames.GetNextSelectedItem(pos);

	CString backto;
	backto.Format("%d", index+1);
	m_AnimBackTo.SetWindowTextA(backto);
	UpdateAnimationSettings();
}

void CAnimationEditorDlg::OnBnClickedForever()
{
	UpdateAnimationSettings();
}

void CAnimationEditorDlg::OnBnClickedPingpong()
{
	UpdateAnimationSettings();
}

void CAnimationEditorDlg::UpdateAnimationSettings()
{
	if(m_CurrentAnimation)
	{
		CString str;
		m_AnimRepeat.GetWindowText(str);
		m_CurrentAnimation->m_Repeat = atoi(str);

		m_AnimBackTo.GetWindowTextA(str);
		m_CurrentAnimation->m_RepeatTo = atoi(str);

		m_AnimSpeed.GetWindowTextA(str);
		m_CurrentAnimation->m_Speed = atoi(str);

		m_CurrentAnimation->m_bForever  = m_AnimForever.GetCheck();
		m_CurrentAnimation->m_bPingPong = m_AnimPingPong.GetCheck();


	}


}
void CAnimationEditorDlg::OnEnChangeRepeat()
{
	if(m_AnimRepeat.GetModify())
		UpdateAnimationSettings();
}

void CAnimationEditorDlg::OnEnChangeBackto()
{
	if(m_AnimBackTo.GetModify())
		UpdateAnimationSettings();
}

void CAnimationEditorDlg::OnEnChangeSpeed()
{
	if(m_AnimSpeed.GetModify())
		UpdateAnimationSettings();
}

//void CAnimationEditorDlg::OnEnChangeTime()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CExtNCW<CExtResizableDialog>::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// TODO:  Add your control notification handler code here
//}

void CAnimationEditorDlg::OnLvnBeginlabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
}

void CAnimationEditorDlg::OnLvnEndlabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here

	CString Name = pDispInfo->item.pszText;
/*
	if (Name == "") {
		*pResult = 0;
		return;
	}

	if (Name != oldName && VariableNameExists(Name))
		*pResult = 0;
	else {
		// Do rename
		*pResult = 1;

		// Update name
		if (globalsMode)
			(*find(pApp->m_Globals.begin(), pApp->m_Globals.end(), oldName)).name = Name;
		else
			(*find(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), oldName)).name = Name;
	}*/
	//TODO check for name conflicts


	*pResult = 1;

	CAnimation* animation = (CAnimation*)m_AnimationTree.GetItemData(pDispInfo->item.hItem);
	animation->m_Name = Name;

}


void CAnimationEditorDlg::OnBegindragFrames(NMHDR* pNMHDR, LRESULT* pResult)
{
	//This routine sets the parameters for a Drag and Drop operation.
	//It sets some variables to track the Drag/Drop as well
	// as creating the drag image to be shown during the drag.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//// Save the index of the item being dragged in m_nDragIndex
	////  This will be used later for retrieving the info dragged
	m_nDragIndex = pNMListView->iItem;

	//// Create a drag image
	POINT pt;
	int nOffset = -10; //offset in pixels for drag image (positive is up and to the left; neg is down and to the right)
	//if(m_Frames.GetSelectedCount() > 1) //more than one item is selected
	pt.x = nOffset;
	pt.y = nOffset;

	m_pDragImage = m_Frames.CreateDragImage(m_nDragIndex, &pt);

	ASSERT(m_pDragImage); //make sure it was created

	m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
	m_pDragImage->DragEnter(NULL, pNMListView->ptAction);
	
	//// Set dragging flag and others
	m_bDragging = TRUE;	//we are in a drag and drop operation
	m_nDropIndex = -1;	//we don't have a drop index yet
	m_pDragList = &m_Frames; //make note of which list we are dragging from
	m_pDropWnd = &m_Frames;	//at present the drag list is the drop list

	//// Capture all mouse messages
	SetCapture ();

	*pResult = 0;
}

void CAnimationEditorDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	//While the mouse is moving, this routine is called.
	//This routine will redraw the drag image at the present
	// mouse location to display the dragging.
	//Also, while over a CListCtrl, this routine will highlight
	// the item we are hovering over.

	//// If we are in a drag/drop procedure (m_bDragging is true)
	if (m_bDragging)
	{
		//// Move the drag image
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		m_pDragImage->DragMove(pt); //move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		//// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd); //make sure we have a window

		//// If we drag outside current window we need to adjust the highlights displayed
		if (pDropWnd != m_pDropWnd)
		{
			if (m_nDropIndex != -1) //If we drag over the CListCtrl header, turn off the hover highlight
			{
				TRACE("m_nDropIndex is -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));
				// redraw item
				VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));
				pList->UpdateWindow ();
				m_nDropIndex = -1;
			}
			else //If we drag out of the CListCtrl altogether
			{
				TRACE("m_nDropIndex is not -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				int i = 0;
				int nCount = pList->GetItemCount();
				for(i = 0; i < nCount; i++)
				{
					pList->SetItemState(i, 0, LVIS_DROPHILITED);
				}
				pList->RedrawItems(0, nCount);
				pList->UpdateWindow();
			}
		}

		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;

		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);

		//If we are hovering over a CListCtrl we need to adjust the highlights
		if(pDropWnd->IsKindOf(RUNTIME_CLASS (CListCtrl)))
		{			
			//Note that we can drop here
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			UINT uFlags;
			CListCtrl* pList = (CListCtrl*)pDropWnd;

			// Turn off hilight for previous drop target
			pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
			// Redraw previous item
			pList->RedrawItems (m_nDropIndex, m_nDropIndex);
			
			// Get the item that is below cursor
			m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);
			// Highlight it
			if(m_nDropIndex >= 0)
				pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
			// Redraw item
			pList->RedrawItems(m_nDropIndex, m_nDropIndex);
			pList->UpdateWindow();
		}
		else
		{
			//If we are not hovering over a CListCtrl, change the cursor
			// to note that we cannot drop here
			SetCursor(LoadCursor(NULL, IDC_NO));
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}
		
	CDialog::OnMouseMove(nFlags, point);
}

void CAnimationEditorDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//This routine is the end of the drag/drop operation.
	//When the button is released, we are to drop the item.
	//There are a few things we need to do to clean up and
	// finalize the drop:
	//	1) Release the mouse capture
	//	2) Set m_bDragging to false to signify we are not dragging
	//	3) Actually drop the item (we call a separate function to do that)

	//If we are in a drag and drop operation (otherwise we don't do anything)
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();
		delete m_pDragImage; //must delete it because it was created at the beginning of the drag

		CPoint pt (point); //Get current mouse coordinates
		ClientToScreen (&pt); //Convert to screen coordinates
		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd); //make sure we have a window pointer
		// If window is CListCtrl, we perform the drop
		if(m_nDropIndex != -1)	//prevent tusers from having their frames go to the end of the anim if they miss the drop target
			if (pDropWnd->IsKindOf (RUNTIME_CLASS (CListCtrl)))
			{
				m_pDropList = (CListCtrl*)pDropWnd; //Set pointer to the list we are dropping on
				DropItemOnList(m_pDragList, m_pDropList); //Call routine to perform the actual drop
			}
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}
typedef struct {
	LVITEM* plvi;
	CString sCol2;
} lvItem, *plvItem;
void CAnimationEditorDlg::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)
{

	vector<int> framesToDelete;

	POSITION pos = m_Frames.GetFirstSelectedItemPosition();
	if(!m_CurrentAnimation)
		return;
	while(pos)
	{
		int item = m_Frames.GetNextSelectedItem(pos);
		framesToDelete.push_back(item);
	}

	vector<int> newimages;
	// Delete backwards otherwise the rest of the frames go out of alignment
	vector<int>& images = m_CurrentAnimation->m_Images;

	vector<int>::reverse_iterator i = framesToDelete.rbegin();

	//m_nDropIndex--;
	for ( ; i != framesToDelete.rend(); i++)
	{
		newimages.push_back(*(images.begin() + *i));
		images.erase(images.begin() + *i);
		//if(m_nDropIndex <= *i)
		//	m_nDropIndex++;
	}

	vector<int>::iterator v = newimages.begin();
	if(m_nDropIndex > images.size()+1)
		m_nDropIndex = images.size()+1;
	for ( ; v != newimages.end(); v++)
	{
		// The images are inserted in reverse order
		if(m_nDropIndex > m_CurrentAnimation->m_Images.size())
		{
			m_CurrentAnimation->m_Images.push_back(*v);
			m_nDropIndex--;
		}
		else
			m_CurrentAnimation->m_Images.insert(images.begin() + m_nDropIndex, *v);


	}

	UpdateList();
	for(int a = m_nDropIndex; a < m_nDropIndex + newimages.size(); a++)
	{
		pDropList->SetItemState (a, LVIS_SELECTED, LVIS_SELECTED);
	}


/*
	if(pDragList->GetSelectedCount() == 1)
	{
		for(int a = pDragList->GetItemCount(); a > 0; a++)
		{
			if(pDragList->GetItemState
			pDragList->DeleteItem(a);


		}
		
		int image = m_CurrentAnimation->m_Images[m_nDragIndex];
		m_CurrentAnimation->m_Images.erase(m_CurrentAnimation->m_Images.begin() + m_nDragIndex);


		if(m_nDropIndex > m_CurrentAnimation->m_Images.size())
			m_CurrentAnimation->m_Images.push_back(image);
		else
			m_CurrentAnimation->m_Images.insert(m_CurrentAnimation->m_Images.begin() + m_nDropIndex, image);
	
		// Select the new item we just inserted

		UpdateList();
		pDropList->SetItemState (m_nDropIndex, LVIS_SELECTED, LVIS_SELECTED);


	}*/
//	else //more than 1 item is being dropped
	//{
	/*	//We have to parse through all of the selected items from the DragList
		//1) Retrieve the info for the items and store them in memory
		//2) If we are reordering, delete the items from the list
		//3) Insert the items into the list (either same list or different list)

		CList<lvItem*, lvItem*> listItems;
		POSITION listPos;
			
		//Retrieve the selected items
		POSITION pos = pDragList->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
		while(pos) //so long as we have a valid POSITION, we keep iterating
		{
			plvitem = new LVITEM;
			ZeroMemory(plvitem, sizeof(LVITEM));
			pItem = new lvItem;
			//ZeroMemory(pItem, sizeof(lvItem)); //If you use ZeroMemory on the lvItem struct, it creates an error when you try to set sCol2
			pItem->plvi = plvitem;
			pItem->plvi->iItem = m_nDragIndex;
			pItem->plvi->mask = LVIF_TEXT;
			pItem->plvi->pszText = new char; //since this is a pointer to the string, we need a new pointer to a new string on the heap
			pItem->plvi->cchTextMax = 255;

			m_nDragIndex = pDragList->GetNextSelectedItem(pos);

			//Get the item
			pItem->plvi->iItem = m_nDragIndex; //set the index in the drag list to the selected item
			pDragList->GetItem(pItem->plvi); //retrieve the information
			pItem->sCol2 = pDragList->GetItemText(pItem->plvi->iItem, 1);

			//Save the pointer to the new item in our CList
			listItems.AddTail(pItem);
		} //EO while(pos) -- at this point we have deleted the moving items and stored them in memory

		if(pDragList == pDropList) //we are reordering the list (moving)
		{
			//Delete the selected items
			pos = pDragList->GetFirstSelectedItemPosition();
			while(pos)
			{
				pos = pDragList->GetFirstSelectedItemPosition();
				m_nDragIndex = pDragList->GetNextSelectedItem(pos);

				pDragList->DeleteItem(m_nDragIndex); //since we are MOVING, delete the item
				if(m_nDragIndex < m_nDropIndex) m_nDropIndex--; //must decrement the drop index to account
																//for the deleted items
			} //EO while(pos)
		} //EO if(pDragList...

		//Iterate through the items stored in memory and add them back into the CListCtrl at the drop index
		listPos = listItems.GetHeadPosition();
		while(listPos)
		{
			pItem = listItems.GetNext(listPos);

			m_nDropIndex = (m_nDropIndex == -1) ? pDropList->GetItemCount() : m_nDropIndex;
			pItem->plvi->iItem = m_nDropIndex;
			pDropList->InsertItem(pItem->plvi); //add the item
			pDropList->SetItemText(pItem->plvi->iItem, 1, pItem->sCol2);

			pDropList->SetItemState(pItem->plvi->iItem, LVIS_SELECTED, LVIS_SELECTED); //highlight/select the item we just added

			m_nDropIndex++; //increment the index we are dropping at to keep the dropped items in the same order they were in in the Drag List
			//If we dont' increment this, the items are added in reverse order
		} //EO while(listPos)
*/
	//}
}

class CAnimEdFrame : public CObject
{
public:
	CAnimationEditorDlg* m_anim_ed;
	CAnimEdFrame()
	{
		m_anim_ed = 0;
	}
	virtual void Serialize(CArchive& ar)
	{
		m_anim_ed->FrameSerialize(ar);
	}
};

void CAnimationEditorDlg::OnToolbarCopy()
{
	CAnimEdFrame* frames = new CAnimEdFrame;
	frames->m_anim_ed = this;
	DROPEFFECT de = DROPEFFECT_NONE;
	m_pDDMgr.PrepareDrop(DO_CLIPBOARD, "Construct Frames", frames, &de);
}

void CAnimationEditorDlg::OnToolbarPaste()
{
	if (m_pDDMgr.OkToPaste())
	{
		CAnimEdFrame frames;
		frames.m_anim_ed = this;
		m_pDDMgr.DoDrop(&frames,
                          NULL,
                          "Construct Frames");
	}
}

void CAnimationEditorDlg::FrameSerialize(CArchive& ar)
{
	// This function is only called for copying and pasting frames

	if(ar.IsLoading())
	{
		POSITION pos = m_Frames.GetFirstSelectedItemPosition();
		int count;
		ar >> count;
		vector<int> ToSelect;
		for(int a = 0; a < count; a ++)
		{
			m_pApplication->m_imageBank.push_back(CImageResource());
			m_pApplication->m_imageBankHasChanged = true;
			CImageResource* image = &m_pApplication->m_imageBank.back();
			image->Serialize(ar);
			image->m_FixedID = m_pApplication->m_ImageFixID++;

			POSITION pos = m_Frames.GetFirstSelectedItemPosition();
			if(pos)
			{
				int nItem = m_Frames.GetNextSelectedItem(pos);
				m_CurrentAnimation->m_Images.insert( m_CurrentAnimation->m_Images.begin() + nItem + a, image->m_FixedID);
				ToSelect.push_back(nItem + a);
			}
			else
			{
				m_CurrentAnimation->m_Images.push_back( image->m_FixedID);
				ToSelect.push_back(m_CurrentAnimation->m_Images.size());
			}			

			int iFrameTime = 0;
			ar >> iFrameTime;
			m_CurrentAnimation->m_FrameTimes.push_back(iFrameTime);
		}

		UpdateList();
		for(int c = 0 ; c < ToSelect.size(); c++)
			m_Frames.SetItemState (ToSelect[c], LVIS_SELECTED, LVIS_SELECTED);

	}
	else
	{
		POSITION pos = m_Frames.GetFirstSelectedItemPosition();
		ar << (int) m_Frames.GetSelectedCount();
		while (pos)
		{
			int nItem = m_Frames.GetNextSelectedItem(pos);
			CImageResource* image = m_pApplication->FindImageResourceFromNumber(m_CurrentAnimation->m_Images.at(nItem));
			image->Serialize(ar);
			ar << (int)m_CurrentAnimation->m_FrameTimes[nItem];
		}
	}
}

void CAnimationEditorDlg::OnBnClickedCopy()
{
	OnToolbarCopy();
	// TODO: Add your control notification handler code here
}

void CAnimationEditorDlg::OnBnClickedPaste()
{
	OnToolbarPaste();
	// TODO: Add your control notification handler code here
}

BOOL CAnimationEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	/*if(m_haccel)
	{
		if (::TranslateAccelerator(m_hWnd, m_haccel, pMsg)) 
			return(TRUE);
	
    }	*/
	if (pMsg->message == WM_KEYDOWN) {
	//	CPoint& hs = m_pXDVView->m_PicEd.GetHotspot();
	//	int w = m_pXDVView->m_PicEd.m_Width;
	//	int h = m_pXDVView->m_PicEd.m_Height;
	//	bool interestingKey = false;

		switch (pMsg->wParam) {
	/*	case VK_NUMPAD1:
			hs.x = 0;
			hs.y = h;
			interestingKey = true;
			break;
		case VK_NUMPAD2:
			hs.x = w / 2;
			hs.y = h;
			interestingKey = true;
			break;
		case VK_NUMPAD3:
			hs.x = w;
			hs.y = h;
			interestingKey = true;
			break;
		case VK_NUMPAD4:
			hs.x = 0;
			hs.y = h / 2;
			interestingKey = true;
			break;
		case VK_NUMPAD5:
			hs.x = w / 2;
			hs.y = h / 2;
			interestingKey = true;
			break;
		case VK_NUMPAD6:
			hs.x = w;
			hs.y = h / 2;
			interestingKey = true;
			break;
		case VK_NUMPAD7:
			hs.x = 0;
			hs.y = 0;
			interestingKey = true;
			break;
		case VK_NUMPAD8:
			hs.x = w / 2;
			hs.y = 0;
			interestingKey = true;
			break;
		case VK_NUMPAD9:
			hs.x = w;
			hs.y = 0;
			interestingKey = true;
			break;*/
		case VK_ESCAPE:
		case VK_RETURN:
			{
				


				return TRUE;
			}
		}
	}


	return CDialog::PreTranslateMessage(pMsg);
}

void CAnimationEditorDlg::OnBnClickedImport()
{
	CImportImagesDlg dlg;
	dlg.DoModal();

	if(dlg.m_animation || dlg.m_frame)
	{

		if(dlg.m_animation)
		{
			m_CurrentAnimation->m_Images.clear();
		}
		
		for(list<CImageResource>::iterator i = dlg.m_Images.begin(); i!= dlg.m_Images.end(); i++)
		{
			int id = m_pFrame->AddImage(32, 32);

			m_CurrentAnimation->m_Images.push_back(id);
			m_CurrentAnimation->m_FrameTimes.push_back(1.0);

				
			CImageResource* image = m_pApplication->FindImageResourceFromNumber(id);
			image->m_Bitmap = i->m_Bitmap;
			image->m_Action = i->m_Action;
			image->m_Hotspot = i->m_Hotspot;
		}
		UpdateList();
	}


}
void CAnimationEditorDlg::OnBnClickedPlay()
{
	m_iFrame = -1;
	m_iPlayCount = 0;
	m_iPlayForward = true;


	SetTimer(0, 10, NULL);
	// TODO: Add your control notification handler code here
}

void CAnimationEditorDlg::OnRClickFilmstrip(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT curPos, htPos;
	GetCursorPos(&curPos);
	GetCursorPos(&htPos);
	m_Frames.ScreenToClient(&htPos);

	CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	pPopup->LoadMenu(m_hWnd,IDR_APPPROPS, true, false);
	pPopup->ItemRemove(0);
	pPopup->ItemRemove(0);
	pPopup->ItemRemove(0);
	pPopup->ItemRemove(0);

	UINT ChosenItem = 0;

	pPopup->ItemInsertCommand(1, -1, "Add frame", NULL, NULL);

	bool bIsSel = false;

	POSITION pos = m_Frames.GetFirstSelectedItemPosition();
	if(!m_CurrentAnimation)
		return;
	while(pos)
	{
		int item = m_Frames.GetNextSelectedItem(pos);
		if (item != -1)
			bIsSel = true;
	}

	if (bIsSel)
		pPopup->ItemInsertCommand(2, -1, "Remove frames", NULL, NULL);

	pPopup->TrackPopupMenu(TPMX_DO_MESSAGE_LOOP|TPMX_NO_WM_COMMAND|TPMX_NO_CMD_UI, curPos.x, curPos.y, NULL, NULL, NULL, &ChosenItem);
		
	if (ChosenItem == 1)
	{
		OnAddFrame();
	}

	else if (ChosenItem == 2)
	{
		OnRemoveFrame();
	}	
}

void CAnimationEditorDlg::OnKeyDownFilmStrip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	// Check modifier
	bool ControlDown = GetKeyState(VK_CONTROL) >> 4;

	LV_KEYDOWN* pKeyDown = (LV_KEYDOWN*)pNMHDR;
	WORD Test = pKeyDown->wVKey;

	// Check key

	// Delete, delete
	if (Test == VK_DELETE)
	{
		OnRemoveFrame();
	}

	// C, Copy
	if (Test == 0x43 && ControlDown)
	{
		CAnimEdFrame* frames = new CAnimEdFrame;
		frames->m_anim_ed = this;
		DROPEFFECT de = DROPEFFECT_NONE;
		m_pDDMgr.PrepareDrop(DO_CLIPBOARD, "Construct Frames", frames, &de);
	}

	// V, paste
	if (Test == 0x56 && ControlDown)
	{
		if (m_pDDMgr.OkToPaste())
		{
			CAnimEdFrame frames;
			frames.m_anim_ed = this;
			m_pDDMgr.DoDrop(&frames,
							  NULL,
							  "Construct Frames");
		}
	}

	*pResult = 0;
}
