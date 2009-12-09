// Dialogs\ExchangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "ExchangeDlg.h"
#include "..\Utilities\WebGrab.h"

class CDCallback : public IBindStatusCallback  
{
public:
    CExchangeDlg* pDialog;

    STDMETHOD(OnStartBinding) (DWORD dwReserved, IBinding __RPC_FAR *pib) { return E_NOTIMPL; }
    STDMETHOD(GetPriority) (LONG __RPC_FAR *pnPriority) { return E_NOTIMPL; }
    STDMETHOD(OnLowResource) (DWORD reserved) { return E_NOTIMPL; }
    STDMETHOD(OnProgress) (ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText)
	{
		pDialog->m_Progress.SetRange(0, (static_cast<int>(ulProgressMax) / 1024));
		pDialog->m_Progress.SetPos((static_cast<int>(ulProgress) / 1024));
		return S_OK;
	}

    STDMETHOD(OnStopBinding) (HRESULT hresult, LPCWSTR szError) { return E_NOTIMPL; }
    STDMETHOD(GetBindInfo)(DWORD __RPC_FAR *grfBINDF, BINDINFO __RPC_FAR *pbindinfo) { return E_NOTIMPL; }
    STDMETHOD(OnDataAvailable)(DWORD grfBSCF, DWORD dwSize, FORMATETC __RPC_FAR *pformatetc, STGMEDIUM __RPC_FAR *pstgmed) { return E_NOTIMPL; }
    STDMETHOD(OnObjectAvailable)( REFIID riid, IUnknown __RPC_FAR *punk) { return E_NOTIMPL; }
    STDMETHOD_(ULONG,AddRef) () { return 0; }
    STDMETHOD_(ULONG,Release) () { return 0; }
    STDMETHOD(QueryInterface) (REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject) { return E_NOTIMPL; }
};

void DownloadThread(void* pData)
{
	CExchangeDlg* pDialog = (CExchangeDlg*)pData;
	CDCallback Callback;
	Callback.pDialog = pDialog;

	MessageBox(NULL, pDialog->m_LocalFile, NULL, NULL);
	HRESULT Result = URLDownloadToFile(NULL, pDialog->m_RemoteURL, pDialog->m_LocalFile, 0, &Callback);
	if (SUCCEEDED(Result))
	{
		//
	}

	else
	{
		CErrorDlg Dlg;
		Dlg.Error("Download error", "An error occured when downloading a plugin.\r\n\r\nPlease check your internet and firewall settings.");
	}
}

// CExchangeDlg dialog
IMPLEMENT_DYNAMIC(CExchangeDlg, CDialog)

CExchangeDlg::CExchangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExchangeDlg::IDD, pParent)
{

}

CExchangeDlg::~CExchangeDlg()
{
}

void CExchangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_DOWNLOAD, m_Download);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDOK, m_OK);
}

BEGIN_MESSAGE_MAP(CExchangeDlg, CDialog)
	ON_BN_CLICKED(IDC_DOWNLOAD, &CExchangeDlg::OnBnClickedDownload)
END_MESSAGE_MAP()

// CExchangeDlg message handlers
BOOL CExchangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifdef CONSTRUCT_PUBLICBETA
	MessageBox("This feature is disabled in the public beta.", "Disabled feature");
	EndDialog(0);
#else
	m_List.InsertColumn(0, "Plugin name", 0, 120);
	m_List.InsertColumn(0, "Author", 0, 80);
	m_List.InsertColumn(0, "Version", 0, 60);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CWebGrab WG;
	WG.SetForceReload(true);
	CString Plugins;
	WG.GetFile("http://herbdx.spatang.com/scirra/construct_integration.php", Plugins);
	
	// Parse
	if (Plugins != "")
	{
		Plugins.TrimRight();

		CToken Token(Plugins);
		Token.SetToken("\r\n");
		while (Token.MoreTokens())
		{
			CString Line = Token.GetNextToken();

			// Get name, author, version, description, URL
			CString Name, Author, Version, Description, URL;
			AfxExtractSubString(Name, Line, 0, '|');
			AfxExtractSubString(Author, Line, 1, '|');
			AfxExtractSubString(Version, Line, 2, '|');
			AfxExtractSubString(Description, Line, 3, '|');
			AfxExtractSubString(URL, Line, 4, '|');

			int Item = m_List.InsertItem(m_List.GetItemCount(), Name);
			m_List.SetItemText(Item, 1, Author);
			m_List.SetItemText(Item, 2, Version);

			m_URLs.push_back(URL);
		}
	}

	m_Download.SetWindowText(GetString("Exchange", "2"));
	CString Title;
	Title.Format("Construct : %s", GetString("Exchange", "1"));
	SetWindowText(Title);

	return TRUE;
#endif
}
void CExchangeDlg::OnBnClickedDownload()
{
	POSITION listpos = m_List.GetFirstSelectedItemPosition();
	int item = m_List.GetNextSelectedItem(listpos);

	if (item == -1) return;

	m_RemoteURL = m_URLs.at(item);
	m_LocalFile = "C:\\test.txt";

	_beginthread(DownloadThread, 0, (void*)this);
}
