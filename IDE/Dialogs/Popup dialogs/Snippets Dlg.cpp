#include "stdafx.h"
#include "..\..\Construct.h"
#include "Snippets Dlg.h"
 
// CSnippetsDlg dialog
IMPLEMENT_DYNAMIC(CSnippetsDlg, CDialog)

CSnippetsDlg::CSnippetsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSnippetsDlg::IDD, pParent)
{
}

CSnippetsDlg::~CSnippetsDlg()
{
}

void CSnippetsDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_SNIPPETS, m_Snippets);
	DDX_Control(pDX, IDC_INSERT, m_Insert);
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnippetsDlg, CDialog)
	ON_BN_CLICKED(IDC_INSERT, &CSnippetsDlg::OnBnClickedInsert)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_SNIPPETS, &CSnippetsDlg::OnNMDblclkResults)
END_MESSAGE_MAP()

// CSnippetsDlg message handlers
BOOL CSnippetsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Snippets.InsertColumn(0, "Name", LVCFMT_LEFT, 82);
	m_Snippets.InsertColumn(1, "Description", LVCFMT_LEFT, 152);
			
	CPath Path;
	Path.SetToCurrentDirectory();
	CString Snippets;
	Snippets.Format("%sSnippets\\*.*", Path.GetFullPath());

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(Snippets, &FindFileData);

	DWORD id = 0;

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		CErrorDlg error;
		error.Error("Snippets directory missing", "The snippets directory has been moved.\nConstruct cannot initialise your snippets.");     
		EndDialog(0);
	} 

	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			CString file = FindFileData.cFileName;
			CString ext = file.Right(4);
			ext.MakeLower();

			if (ext == ".txt")
			{
				int item = m_Snippets.InsertItem(m_Snippets.GetItemCount(), file.Left(file.GetLength() - 4), 0);

				CString Open = Snippets;
				Open = Open.Left(Open.GetLength() - 3);
				Open += file;

				CStdioFile File(Open, CFile::modeRead);
				CString Desc;
				CString Snippet;
				CString String;
				int Count = 0;

				while(File.ReadString(String))
				{
					if (Count == 0)
						Desc = String;

					else
					{
						Snippet += String;
						Snippet += "\r\n";
					}

					Count++;
				}

				m_Snippets.SetItemText(item, 1, Desc);
				m_SnippetTexts.push_back(Snippet);
			}
		}
	}	

	m_Snippets.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSnippetsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		
	CRect rect;
	GetClientRect(&rect);

	dc.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(0, 0, 0));
	dc.FillSolidRect(1, 1, rect.Width() - 2, rect.Height() - 2, RGB(255, 255, 255));
}

void CSnippetsDlg::OnBnClickedInsert()
{
	POSITION Pos = m_Snippets.GetFirstSelectedItemPosition();
	int Item = m_Snippets.GetNextSelectedItem(Pos);
	if (Item == -1) return;

	m_pParent->m_Script.SendMessage(SCI_REPLACESEL, 0, (LPARAM)(const char*)m_SnippetTexts[Item]);

	EndDialog(0);
}

void CSnippetsDlg::OnNMDblclkResults(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	POSITION Pos = m_Snippets.GetFirstSelectedItemPosition();
	int Item = m_Snippets.GetNextSelectedItem(Pos);
	if (Item == -1) return;

	m_pParent->m_Script.SendMessage(SCI_REPLACESEL, 0, (LPARAM)(const char*)m_SnippetTexts[Item]);
	EndDialog(0);
}
