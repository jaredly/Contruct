// Dialogs/ImportImagesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportImagesDlg.h"
#include "..\Resource.h"
#include "..\Utilities\FECFileDialog.h"

#include <algorithm>
#include <string>
// CImportImagesDlg dialog

IMPLEMENT_DYNAMIC(CImportImagesDlg, CDialog)

CImportImagesDlg::CImportImagesDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CImportImagesDlg::IDD, pParent)
{
	m_frame = false;
	m_animation = false;
}

CImportImagesDlg::~CImportImagesDlg()
{
}

void CImportImagesDlg::DoDataExchange(CDataExchange* pDX)
{
	CExtNCW<CExtResizableDialog>::DoDataExchange(pDX);
	//Buttons
	DDX_Control(pDX, IDC_HOTSPOT1, m_Hotspot1);
	DDX_Control(pDX, IDC_HOTSPOT2, m_Hotspot2);
	DDX_Control(pDX, IDC_HOTSPOT3, m_Hotspot3);
	DDX_Control(pDX, IDC_HOTSPOT4, m_Hotspot4);
	DDX_Control(pDX, IDC_HOTSPOT5, m_Hotspot5);
	DDX_Control(pDX, IDC_HOTSPOT6, m_Hotspot6);
	DDX_Control(pDX, IDC_HOTSPOT7, m_Hotspot7);
	DDX_Control(pDX, IDC_HOTSPOT8, m_Hotspot8);
	DDX_Control(pDX, IDC_HOTSPOT9, m_Hotspot9);

	DDX_Control(pDX, IDC_COLOUR_MASK, m_ColourMask);
	DDX_Control(pDX, IDC_COLLISION, m_Collision);
	DDX_Control(pDX, IDC_MIRROR, m_Mirror);
	DDX_Control(pDX, IDC_FLIP, m_Flip);
	DDX_Control(pDX, IDC_ROTATE1, m_Rotate1);
	DDX_Control(pDX, IDC_ROTATE2, m_Rotate2);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDC_UP, m_Up);
	DDX_Control(pDX, IDC_DOWN, m_Down);
	DDX_Control(pDX, IDC_CUT_INTO_TILES, m_Tiles);

	DDX_Control(pDX, IDC_IMPORTANIMATION, m_ImportAnimation);
	DDX_Control(pDX, ID_CANCEL2, m_Cancel);

	// Editboxes
	DDX_Control(pDX, IDC_TILESETX, m_TilesetX);
	DDX_Control(pDX, IDC_TILESETY, m_TilesetY);
	DDX_Control(pDX, IDC_TILESETW, m_TilesetW);
	DDX_Control(pDX, IDC_TILESETH, m_TilesetH);

	DDX_Control(pDX, IDC_STATIC_TILEX, m_Static_TileX);
	DDX_Control(pDX, IDC_STATIC_TILEY, m_Static_TileY);
	DDX_Control(pDX, IDC_STATIC_TILEW, m_Static_TileW);
	DDX_Control(pDX, IDC_STATIC_TILEH, m_Static_TileH);

	DDX_Control(pDX, IDC_CHECK1, m_KeepImage);

	DDX_Control(pDX, IDC_LIST, m_ImageList);
	DDX_Control(pDX, IDC_PREVIEW_IMAGE, m_Preview);
}

BEGIN_MESSAGE_MAP(CImportImagesDlg, CExtNCW<CExtResizableDialog>)
	ON_BN_CLICKED(IDC_HOTSPOT1, &CImportImagesDlg::OnBnClickedHotspot1)
	ON_BN_CLICKED(IDC_HOTSPOT2, &CImportImagesDlg::OnBnClickedHotspot2)
	ON_BN_CLICKED(IDC_HOTSPOT3, &CImportImagesDlg::OnBnClickedHotspot3)
	ON_BN_CLICKED(IDC_HOTSPOT4, &CImportImagesDlg::OnBnClickedHotspot4)
	ON_BN_CLICKED(IDC_HOTSPOT5, &CImportImagesDlg::OnBnClickedHotspot5)
	ON_BN_CLICKED(IDC_HOTSPOT6, &CImportImagesDlg::OnBnClickedHotspot6)
	ON_BN_CLICKED(IDC_HOTSPOT7, &CImportImagesDlg::OnBnClickedHotspot7)
	ON_BN_CLICKED(IDC_HOTSPOT8, &CImportImagesDlg::OnBnClickedHotspot8)
	ON_BN_CLICKED(IDC_HOTSPOT9, &CImportImagesDlg::OnBnClickedHotspot9)
	ON_BN_CLICKED(IDC_COLOUR_MASK, &CImportImagesDlg::OnBnClickedColourMask)
	ON_BN_CLICKED(IDC_MIRROR, &CImportImagesDlg::OnBnClickedMirror)
	ON_BN_CLICKED(IDC_FLIP, &CImportImagesDlg::OnBnClickedFlip)
	ON_BN_CLICKED(IDC_ROTATE1, &CImportImagesDlg::OnBnClickedRotate1)
	ON_BN_CLICKED(IDC_ROTATE2, &CImportImagesDlg::OnBnClickedRotate2)
	ON_BN_CLICKED(IDC_ADD, &CImportImagesDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, &CImportImagesDlg::OnBnClickedRemove)
	ON_BN_CLICKED(IDC_UP, &CImportImagesDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CImportImagesDlg::OnBnClickedDown)
	ON_BN_CLICKED(IDC_CUT_INTO_TILES, &CImportImagesDlg::OnBnClickedCutIntoTiles)
	ON_BN_CLICKED(IDC_IMPORTFRAME, &CImportImagesDlg::OnBnClickedImportframe)
	ON_BN_CLICKED(IDC_IMPORTANIMATION, &CImportImagesDlg::OnBnClickedImportanimation)
	ON_BN_CLICKED(ID_CANCEL2, &CImportImagesDlg::OnBnClickedCancel2)
	ON_LBN_SELCHANGE(IDC_LIST, &CImportImagesDlg::OnListSelChange)
	ON_BN_CLICKED(IDC_COLLISION, &CImportImagesDlg::OnBnClickedCollision)
END_MESSAGE_MAP()

bool sort_file_path_strings (CString i,CString j) 
{ 
	if(i.GetLength() < j.GetLength())
		return true;
	else if(i.GetLength() > j.GetLength())
		return false;
	else
		return i < j;
}



BOOL CImportImagesDlg::OnInitDialog()
{
	CExtNCW<CExtResizableDialog>::OnInitDialog();

	CFECFileDialog Dialog(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, "All files|*.*");
/*	if (Dialog.DoModal())
	{ 
		m_ImageList.ResetContent();
		POSITION pos= Dialog.GetStartPosition();
		map<int, map<CString, int>> sorted_files;
		while (pos!=NULL)
		{
			CString file = Dialog.GetNextPathName(pos);
			map<CString, int>& sort = sorted_files[file.GetLength()];
			sort[file] = 0;
		}
		for(map<int, map<CString, int>>::iterator v = sorted_files.begin(); v != sorted_files.end();v++)
		{
			map<CString, int>& sort = v->second;
			for(map<CString, int>::iterator i = sort.(); i!= sort.end(); i++)
			{
				CString file = i->first;
				CxImage img;
				img.Load(file);
				if(img.IsValid())
				{	
					file = file.Right(file.GetLength()- file.ReverseFind('\\')-1);
					file = file.Left(file.Find('.'));
					m_ImageList.InsertString(-1, file);
					m_Images.push_back(CImageResource());
					m_Images.back().bitmap = img;
					m_Images.back().m_Name = file;
					m_Images.back().m_Hotspot.x = img.GetWidth()/2;
					m_Images.back().m_Hotspot.y = img.GetHeight()/2;
					m_Images.back().Premultiply();
				}
			}
		}
	}*/
	if (Dialog.DoModal())
	{ 
		m_ImageList.ResetContent();
		POSITION pos= Dialog.GetStartPosition();
		vector<CString> sorted_files;
		while (pos!=NULL)
		{
			CString file = Dialog.GetNextPathName(pos);
			sorted_files.push_back(file);
		}
		sort (sorted_files.begin(), sorted_files.end(), sort_file_path_strings);
		for(vector<CString>::iterator i = sorted_files.begin(); i != sorted_files.end(); ++i)
		{
			CString file = *i;
			CxImage img;
			img.Load(file);
			if(img.IsValid())
			{	
				file = file.Right(file.GetLength()- file.ReverseFind('\\')-1);
				file = file.Left(file.Find('.'));
				m_ImageList.InsertString(-1, file);
				m_Images.push_back(CImageResource());
				m_Images.back().bitmap = img;
				m_Images.back().m_Name = file;
				m_Images.back().m_Hotspot.x = img.GetWidth()/2;
				m_Images.back().m_Hotspot.y = img.GetHeight()/2;
				m_Images.back().Premultiply();
			}
		}
	}

	m_bDragging = false;
	m_nDragIndex = -1;
	m_nDropIndex = -1;
	m_pDragImage = NULL;

	m_TilesetX.SetWindowText("0");
	m_TilesetY.SetWindowText("0");
	m_TilesetW.SetWindowText("32");
	m_TilesetH.SetWindowText("32");

	m_ImageList.SelItemRange(true, 0, m_ImageList.GetCount());

	UpdatePreview(); 

	// Setup buttons
		CExtBitmap Bitmap;
		Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONADD));
		m_Add.SetIcon(Bitmap.CreateHICON());
		m_Add.SetTooltipText(AM_ADD);

		Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_ICONDELETE));
		m_Remove.SetIcon(Bitmap.CreateHICON());
		m_Remove.SetTooltipText(AM_REMOVE);

		Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_UP));
		m_Up.SetIcon(Bitmap.CreateHICON());
		m_Up.SetTooltipText("Up");

		Bitmap.LoadBMP_Resource(MAKEINTRESOURCE(IDB_UP));
		Bitmap.Flip(FALSE, TRUE);
		m_Down.SetIcon(Bitmap.CreateHICON());
		m_Down.SetTooltipText("Down");

	return true;
}



// CImportImagesDlg message handlers

void UncheckHotspots(CImportImagesDlg* dlg)
{
	dlg->m_Hotspot1.SetCheck(false);
	dlg->m_Hotspot2.SetCheck(false);
	dlg->m_Hotspot3.SetCheck(false);
	dlg->m_Hotspot4.SetCheck(false);
	dlg->m_Hotspot5.SetCheck(false);
	dlg->m_Hotspot6.SetCheck(false);
	dlg->m_Hotspot7.SetCheck(false);
	dlg->m_Hotspot8.SetCheck(false);
	dlg->m_Hotspot9.SetCheck(false);
}
void CImportImagesDlg::OnBnClickedHotspot1()
{
	UncheckHotspots(this);
	m_Hotspot1.SetCheck(true);

	UpdateHotspot(0,0);
}

void CImportImagesDlg::OnBnClickedHotspot2()
{
	UncheckHotspots(this);
	m_Hotspot2.SetCheck(true);

	UpdateHotspot(1,0);
}

void CImportImagesDlg::OnBnClickedHotspot3()
{
	UncheckHotspots(this);
	m_Hotspot3.SetCheck(true);

	UpdateHotspot(2,0);
}

void CImportImagesDlg::OnBnClickedHotspot4()
{
	UncheckHotspots(this);
	m_Hotspot4.SetCheck(true);

	UpdateHotspot(0,1);
}

void CImportImagesDlg::OnBnClickedHotspot5()
{
	UncheckHotspots(this);
	m_Hotspot5.SetCheck(true);

	UpdateHotspot(1,1);
}

void CImportImagesDlg::OnBnClickedHotspot6()
{
	UncheckHotspots(this);
	m_Hotspot6.SetCheck(true);

	UpdateHotspot(2,1);
}

void CImportImagesDlg::OnBnClickedHotspot7()
{
	UncheckHotspots(this);
	m_Hotspot7.SetCheck(true);

	UpdateHotspot(0,2);
}

void CImportImagesDlg::OnBnClickedHotspot8()
{
	UncheckHotspots(this);
	m_Hotspot8.SetCheck(true);

	UpdateHotspot(1,2);
}

void CImportImagesDlg::OnBnClickedHotspot9()
{
	UncheckHotspots(this);
	m_Hotspot9.SetCheck(true);

	UpdateHotspot(2,2);
}

void CImportImagesDlg::UpdateHotspot(int a, int b)
{
	float x = a / 2.0;
	float y = b / 2.0;

	vector<CImageResource*> imgs;
	GetSelectedImages(imgs);
	for(vector<CImageResource*>::iterator i = imgs.begin(); i!= imgs.end(); i++)
	{
		(*i)->m_Hotspot.x = (*i)->bitmap.GetWidth()*x;
		(*i)->m_Hotspot.y = (*i)->bitmap.GetHeight()*y;
	}
	UpdatePreview();
	

}
void CImportImagesDlg::OnBnClickedColourMask()
{
	CExtColorDlg dlg;
	if(dlg.DoModal() == IDOK)
	{

		vector<CImageResource*> imgs;
		GetSelectedImages(imgs);
		for(vector<CImageResource*>::iterator i = imgs.begin(); i!= imgs.end(); i++)
		{
			(*i)->ColourReplace(dlg.m_clrNew+0xff000000, 0);
		}
	}
	UpdatePreview();
}

void CImportImagesDlg::OnBnClickedMirror()
{
	vector<CImageResource*> imgs;
	GetSelectedImages(imgs);
	for(vector<CImageResource*>::iterator i = imgs.begin(); i!= imgs.end(); i++)
	{
		(*i)->bitmap.Mirror();
	}
	UpdatePreview();
}

void CImportImagesDlg::OnBnClickedFlip()
{
	vector<CImageResource*> imgs;
	GetSelectedImages(imgs);
	for(vector<CImageResource*>::iterator i = imgs.begin(); i!= imgs.end(); i++)
	{
		(*i)->bitmap.Flip();
	}
	UpdatePreview();
}

void CImportImagesDlg::OnBnClickedRotate1()
{
	vector<CImageResource*> imgs;
	GetSelectedImages(imgs);
	for(vector<CImageResource*>::iterator i = imgs.begin(); i!= imgs.end(); i++)
	{
		(*i)->bitmap.RotateLeft();
	}
	UpdatePreview();
}

void CImportImagesDlg::OnBnClickedRotate2()
{
	vector<CImageResource*> imgs;
	GetSelectedImages(imgs);
	for(vector<CImageResource*>::iterator i = imgs.begin(); i!= imgs.end(); i++)
	{
		(*i)->bitmap.RotateRight();
	}
	UpdatePreview();
}

void CImportImagesDlg::OnBnClickedAdd()
{
	CFECFileDialog Dialog(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, "All files|*.*");
	if (Dialog.DoModal() == IDOK)
	{ 
		POSITION pos= Dialog.GetStartPosition();
		while (pos!=NULL)
		{
			CString file = Dialog.GetNextPathName(pos);
			CxImage img;
			img.Load(file);
			if(img.IsValid())
			{	
				file = file.Right(file.GetLength()- file.ReverseFind('\\')-1);
				file = file.Left(file.Find('.'));
				m_ImageList.InsertString(-1, file);
				m_Images.push_back(CImageResource());
				m_Images.back().bitmap = img;
				m_Images.back().m_Hotspot.x = img.GetWidth()/2;
				m_Images.back().m_Hotspot.y = img.GetHeight()/2;
				m_Images.back().Premultiply();
				m_Images.back().m_Name = file;
			}
		}
	}
	m_ImageList.SetSel(0, true);



	UpdatePreview();
}

void CImportImagesDlg::OnBnClickedRemove()
{
	bool keepGoing = true;

	while (keepGoing) {
		keepGoing = false;

		list<CImageResource>::iterator i = m_Images.begin();

		for(int a = 0; a < m_ImageList.GetCount(); a++, i++)
		{
			if(m_ImageList.GetSel(a))
			{
				m_ImageList.DeleteString(a);
				m_Images.erase(i);
				keepGoing = true;
				break;
			}
		}
	}

	OnListSelChange();
}

void CImportImagesDlg::OnBnClickedUp()
{
	bool keepGoing = true;
	vector<int> linesToSelect;

	while (keepGoing) {
		keepGoing = false;

		list<CImageResource>::iterator i = m_Images.begin();

		for(int a = 0; a < m_ImageList.GetCount(); a++, i++)
		{
			if(a != 0 && m_ImageList.GetSel(a))
			{
				CString str;
				m_ImageList.GetText(a, str);
				m_ImageList.DeleteString(a);
				m_ImageList.InsertString(a - 1, str);
				linesToSelect.push_back(a - 1);
				m_Images.insert(--i, *i);
				m_Images.erase(++i);
				keepGoing = true;
				break;
			}
		}
	}

	vector<int>::iterator n = linesToSelect.begin();

	for ( ; n != linesToSelect.end(); n++)
		m_ImageList.SetSel(*n, true);
}

void CImportImagesDlg::OnBnClickedDown()
{
	bool keepGoing = true;
	vector<int> linesToSelect;

	while (keepGoing) {
		keepGoing = false;

		list<CImageResource>::iterator i = m_Images.end();
		--i;
		for(int a = m_ImageList.GetCount()-1; a >= 0; a--,--i)
		{
			if(a != m_ImageList.GetCount() - 1 && m_ImageList.GetSel(a))
			{
				CString str;
				m_ImageList.GetText(a, str);
				m_ImageList.DeleteString(a);
				m_ImageList.InsertString(a + 1, str);
				linesToSelect.push_back(a + 1);

				CImageResource temp = *i;
				i = m_Images.erase(i);
				++i;
				m_Images.insert(i, temp);
				--i;
				
			
				keepGoing = true;
				break;
			}
		}
	}

	vector<int>::iterator n = linesToSelect.begin();

	for ( ; n != linesToSelect.end(); n++)
		m_ImageList.SetSel(*n, true);
}

void CImportImagesDlg::OnBnClickedCutIntoTiles()
{
	int xo, yo, w, h;
	CString buffer;
	
	m_TilesetX.GetWindowText(buffer);
	xo = atoi(buffer);

	m_TilesetY.GetWindowText(buffer);
	yo = atoi(buffer);

	m_TilesetW.GetWindowText(buffer);
	w = atoi(buffer);

	m_TilesetH.GetWindowText(buffer);
	h = atoi(buffer);

	// error handling
	if(w <= 0)
		return;
	if(h <= 0)
		return;

	vector<CImageResource*> imgs;
	GetSelectedImages(imgs);
	for(vector<CImageResource*>::iterator i = imgs.begin(); i!= imgs.end(); i++)
	{
		for(int y = xo; y < (*i)->bitmap.GetHeight(); y += h)
		{
			for(int x = xo; x < (*i)->bitmap.GetWidth(); x += w)
			{
				this->m_Images.push_back(CImageResource());
				CImageResource* newimg = &m_Images.back();

				newimg->m_Hotspot.x = w/2;
				newimg->m_Hotspot.y = h/2;

				(*i)->bitmap.Crop( CRect(x, y, x + w, y+h),  &newimg->bitmap);

				newimg->m_Name.Format("%s [%d, %d]",(*i)->m_Name,x,y);
				m_ImageList.AddString(newimg->m_Name);

			}


		}

	}

	// TODO: Add your control notification handler code here
}

void CImportImagesDlg::OnBnClickedImportframe()
{
	m_frame = true;
	EndDialog(IDOK);
}

void CImportImagesDlg::OnBnClickedImportanimation()
{
	m_animation = true;
	EndDialog(IDOK);
}

void CImportImagesDlg::OnBnClickedCancel2()
{
	EndDialog(IDCANCEL);
}

void CImportImagesDlg::GetSelectedImages(vector<CImageResource*>& imgs)
{
	list<CImageResource>::iterator i = m_Images.begin();
	for( int a = 0; a < m_ImageList.GetCount(); a++, i++)
		if (m_ImageList.GetSel(a))
			imgs.push_back(&*i);
}

void CImportImagesDlg::UpdatePreview()
{
	vector<CImageResource*> imgs;
	GetSelectedImages(imgs);
	int index = m_ImageList.GetCurSel();

	
	if(imgs.size() == 0)
		m_Preview.SetBitmap(0);
	else
	{	
		HBITMAP bitmap = m_Preview.GetBitmap();
		if(bitmap)
			DeleteObject(bitmap);
		

		bitmap = imgs.back()->bitmap.MakeBitmap(m_Preview.GetDC()->m_hDC);
		m_Preview.SetBitmap(bitmap);
	}

}

void CImportImagesDlg::OnListSelChange()
{
	UpdatePreview();

	bool enable = false;
	if(m_ImageList.GetSelCount())
		enable = true;

	
	m_Hotspot1.EnableWindow(enable);
	m_Hotspot2.EnableWindow(enable);
	m_Hotspot3.EnableWindow(enable);
	m_Hotspot4.EnableWindow(enable);
	m_Hotspot5.EnableWindow(enable);
	m_Hotspot6.EnableWindow(enable);
	m_Hotspot7.EnableWindow(enable);
	m_Hotspot8.EnableWindow(enable);
	m_Hotspot9.EnableWindow(enable);		

	m_ColourMask.EnableWindow(enable);
	m_Mirror.EnableWindow(enable);
	m_Flip.EnableWindow(enable);
	m_Rotate1.EnableWindow(enable);
	m_Rotate2.EnableWindow(enable);
	m_Remove.EnableWindow(enable);
	m_Up.EnableWindow(enable);
	m_Down.EnableWindow(enable);
	m_Tiles.EnableWindow(enable);
	m_Collision.EnableWindow(enable);

	m_HotspotX.EnableWindow(enable);
	m_HotspotY.EnableWindow(enable);
	m_TilesetX.EnableWindow(enable);
	m_TilesetY.EnableWindow(enable);
	m_TilesetW.EnableWindow(enable);
	m_TilesetH.EnableWindow(enable);

	m_KeepImage.EnableWindow(enable);




}

/////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CListBoxMulti, CListBox)



BEGIN_MESSAGE_MAP(CListBoxMulti, CListBox)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


CListBoxMulti::CListBoxMulti()
{
	lastclicked = 0;
}
void CListBoxMulti::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDown(nFlags, point);


	BOOL outside;
	int item = this->ItemFromPoint(point, outside );
	

	if(!(GetKeyState(VK_CONTROL) >> 4))
	{
		for(int a = 0;  a < GetCount(); a ++)
			SetSel(a, false);
	}

	if(item == -1)
		return;
	if((GetKeyState(VK_SHIFT) >> 4))
		for(int a = min(item, lastclicked); a <= max(item, lastclicked); a++)
			SetSel(a, true);




	SetSel(item, true);

	lastclicked = item;

}
void CImportImagesDlg::OnBnClickedCollision()
{
	vector<CImageResource*> imgs;
	GetSelectedImages(imgs);

	int count = imgs.size();
	if(count % 2 == 0)
	{
		count /= 2;
		vector<CImageResource*>::iterator img = imgs.begin();
		vector<CImageResource*>::iterator col = imgs.begin();
		advance(col, count);
		vector<CImageResource*>::iterator imgEnd = col;

		for( ; img!=imgEnd ; img++, col++)
		{
			(*img)->m_Collision = (*col)->bitmap;
		}

	}


	// TODO: Add your control notification handler code here
}
