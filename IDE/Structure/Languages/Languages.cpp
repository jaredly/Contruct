// Languages.cpp : Multi-lingual functions

// File includes
#include "StdAfx.h"
#include "..\..\Utilities\Miscfunc.h"
#include "Languages.h"

extern CString g_Language;

map<CString, CString> m_Languages;

bool LoadLanguageFile(LPCSTR Filename, LPCSTR Type)
{
	if (!FileExists(Filename))
	{
		CErrorDlg Dlg;
		Dlg.Error("Language file not found", "A language file cannot be found for the specified language. Please check your language files.");

		return false;
	}

	CStdioFile File;
	File.Open(Filename, CFile::modeRead);
	CString Line;
	CString Group;
	while (File.ReadString(Line))
	{
		if (Line == "") continue;

		CString Item, Text;
		Item = Type;
		Item += "|";
		Item += Line.Left(Line.Find('='));
		Text = Line.Right(Line.GetLength() - Line.Find('=') - 1);
		m_Languages[Item] = Text;
	}

	return true;
}

bool LoadLanguage(LPCSTR Language)
{
	CPath path;
	path.SetToCurrentDirectory();

	CString File;
	File.Format("%sLanguages\\%s\\General.txt", path.GetFullPath(), Language);

	// Check file exists
	if (!FileExists(File))
	{
		CErrorDlg Dlg;
		Dlg.Error("Language not found", "Your language file cannot be found. Please reinstall Construct.");

		return false;
	}

	// General texts
	File.Format("%sLanguages\\%s\\general.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "general")) return false;

	// about dialog
	File.Format("%sLanguages\\%s\\about-dialog.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "about-dialog")) return false;

	// animator bar
	File.Format("%sLanguages\\%s\\animator-bar.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "animator-bar")) return false;

	// Attributes
	File.Format("%sLanguages\\%s\\Attributes.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "Attributes")) return false;

	// Attribute manager dialog
	File.Format("%sLanguages\\%s\\AttributeManagerDialog.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "AttributeManagerDialog")) return false;

	// Bars
	File.Format("%sLanguages\\%s\\bars.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "bars")) return false;

	// Comment dialog
	File.Format("%sLanguages\\%s\\CommentDialog.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "CommentDialog")) return false;

	// Confirmations
	File.Format("%sLanguages\\%s\\Confirmations.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "Confirmations")) return false;

	// Error dialog
	File.Format("%sLanguages\\%s\\ErrorDialog.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "ErrorDialog")) return false;

	// Event group dialog
	File.Format("%sLanguages\\%s\\EventGroupDialog.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "EventGroupDialog")) return false;

	// Event sheet editor
	File.Format("%sLanguages\\%s\\EventSheetEditor.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "EventSheetEditor")) return false;

	// Exchange dialog
	File.Format("%sLanguages\\%s\\ExchangeDialog.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "ExchangeDialog")) return false;

	// Layout editor
	File.Format("%sLanguages\\%s\\LayoutEditor.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "LayoutEditor")) return false;

	// insert object dialog
	File.Format("%sLanguages\\%s\\insert-object-dialog.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "insert-object-dialog")) return false;

	// Menu editor dialog
	File.Format("%sLanguages\\%s\\MenuEditorDialog.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "MenuEditorDialog")) return false;

	// Object categories
	File.Format("%sLanguages\\%s\\ObjectCategories.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "ObjectCategories")) return false;

	// Object information dialog
	File.Format("%sLanguages\\%s\\object-information-dialog.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "object-information-dialog")) return false;

	// project bar
	File.Format("%sLanguages\\%s\\project-bar.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "project-bar")) return false;

	// resource bar
	File.Format("%sLanguages\\%s\\resource-bar.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "resource-bar")) return false;

	// Ribbon
	File.Format("%sLanguages\\%s\\Ribbon.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "Ribbon")) return false;

	// System object
	File.Format("%sLanguages\\%s\\SystemObject.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "SystemObject")) return false;

	// Tabs
	File.Format("%sLanguages\\%s\\tabs.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "tabs")) return false;

	/////////////////////////////////////////////////////////////////////////////
	// PROPERTIES
	/////////////////////////////////////////////////////////////////////////////

	// Application properties
	File.Format("%sLanguages\\%s\\ApplicationProperties.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "ApplicationProperties")) return false;

	// layer properties
	File.Format("%sLanguages\\%s\\layer-properties.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "layer-properties")) return false;

	// layout properties
	File.Format("%sLanguages\\%s\\layout-properties.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "layout-properties")) return false;

	// Other properties
	File.Format("%sLanguages\\%s\\OtherProperties.txt", path.GetFullPath(), Language);
	if (!LoadLanguageFile(File, "Properties")) return false;

	return true;
}

CString GetString(LPCSTR Group, LPCSTR ID)
{
	CString ToGet;
	ToGet = Group;
	ToGet += "|";
	ToGet += ID;

	CString Out = m_Languages[ToGet];

	return Out;
}