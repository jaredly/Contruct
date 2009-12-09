
#ifndef __FileDialogEx_H__
#define __FileDialogEx_H__

enum SHVIEW_ListViewModes 
{
	SHVIEW_Default	= 0 ,
	SHVIEW_ICON		= 0x7029 ,
	SHVIEW_LIST		= 0x702B ,
	SHVIEW_REPORT	= 0x702C ,
	SHVIEW_THUMBNAIL= 0x702D ,
	SHVIEW_TILE		= 0x702E
} ;

class CFileDialogEx : public CFileDialog
{
	SHVIEW_ListViewModes m_mode ;
public:
	CFileDialogEx(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pWndParent = NULL ,
		SHVIEW_ListViewModes mode=SHVIEW_Default)
		: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pWndParent)
		, m_mode( mode )
	{ }


	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
	{
		if( m_mode != SHVIEW_Default ){
			CWnd* hWndParent = GetParent() ;
			HWND hwndLv = ::FindWindowEx(hWndParent->m_hWnd, NULL, "SHELLDLL_DefView", NULL) ;
			if( hwndLv ){
				::SendMessage(hwndLv, WM_COMMAND, m_mode,0) ;
				// once found we don't need to change it again
				m_mode = SHVIEW_Default ; 
			}
		}

		return CFileDialog::DefWindowProc(message, wParam, lParam);
	}

};

#endif //__FileDialogEx_H__