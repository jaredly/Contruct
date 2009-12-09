#if !defined(AFX_FECFILEDIALOG_H__F15965B0_B05A_11D4_B625_A1459D96AB20__INCLUDED_)
#define AFX_FECFILEDIALOG_H__F15965B0_B05A_11D4_B625_A1459D96AB20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFECFileDialog : public CFileDialog
{
    DECLARE_DYNAMIC(CFECFileDialog)

public:
    CFECFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
        LPCTSTR lpszDefExt = NULL,
        LPCTSTR lpszFileName = NULL,
        DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        LPCTSTR lpszFilter = NULL,
        CWnd* pParentWnd = NULL);

    virtual ~CFECFileDialog();// { if (Files) { delete[] Files; delete[] Folder; } }

    virtual int DoModal();
	CString GetNextPathName(POSITION &pos) const;
	POSITION GetStartPosition();

protected:
	BOOL bParsed;
	TCHAR *Folder;
	TCHAR *Files;
	virtual void OnFileNameChange();
    DECLARE_MESSAGE_MAP()
};

#endif
