#pragma once

#include "..\Editors\Scintilla\include\Scintilla.h"
#include "..\Editors\Scintilla\ScintillaWnd.h"

class CParamScintilla
{
public:
	static void OnChar(NMHDR* pNMHDR, LRESULT* pResult,CScintillaWnd& scintWin, CObjTypeMap* objectMap,
		CParamTooltipCtrl& m_Tooltip,
		bool& m_ParameterTooltipIsVisible,
		bool& m_QuotesOpen, CApplication* pApp, bool bInline = false);
};


class CModalPopup : public CDialog
{
public:
	CWnd* child;



};