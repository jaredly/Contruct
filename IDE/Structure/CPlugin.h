#pragma once

class CPlugin
{
public:
	CPlugin()
	{
		m_Name = "";
		m_Description = "";
		m_Author = "";
		m_WebLink = "http://www.scirra.com";
		m_Version = "1.0"; 
		m_Category = ""; 
		m_FullPath = ""; 
		m_FileName = "";
		m_bMovement = false;
		m_Flags = 0;
		m_pInfo = NULL; // Add
	}

	// Information
	CString m_Name;
	CString m_Description;
	CString m_Author;
	CString m_WebLink;
	CString m_Version;
	CString m_Category;

	// Path
	CString m_FullPath;
	CString m_FileName;

	// Bitmaps
	HBITMAP m_LargeIcon;
	HBITMAP m_SmallIcon;

	// Other information
	bool	m_bMovement;
	int		m_Flags;
	int		m_ID;

	OINFO*	m_pInfo;
};