// ScriptDlg.cpp

#include "stdafx.h"
#include "ScriptDlg.h"
#include "Popup dialogs\Snippets Dlg.h"
#include "..\resource.h"

IMPLEMENT_DYNAMIC(CScriptDlg, CDialog)

CScriptDlg::CScriptDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CScriptDlg::IDD, pParent)
{
	pApp = 0;


	m_Dot = false;
}

CScriptDlg::~CScriptDlg()
{

}

void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_HELPLINE, m_HelpLine);
	DDX_Control(pDX, IDC_FUNCTIONS, m_Functions);
	DDX_Control(pDX, IDC_CLASSES, m_Classes);
	DDX_Control(pDX, IDC_TOOLBAR, m_wndToolBar); 
}

BEGIN_MESSAGE_MAP(CScriptDlg, CExtResizableDialog)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_CLBN_CHKCHANGE(IDC_CLASSES, OnCheckClass)
	ON_LBN_DBLCLK(IDC_FUNCTIONS, OnDblClickFunction)
	ON_LBN_SELCHANGE(IDC_FUNCTIONS, OnFunctionSelChange)
	ON_EN_CHANGE(IDC_HELP, &CScriptDlg::OnEnChangeHelp)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_NEW, OnNew)
	ON_COMMAND(ID_FILE_OPEN, OnLoad)
	ON_COMMAND(ID_FILE_SAVE, OnSave)
	ON_COMMAND(ID_CUT, OnCut)
	ON_COMMAND(ID_COPY, OnCopy)
	ON_COMMAND(ID_PASTE, OnPaste)
	ON_COMMAND(ID_UNDO, OnUndo)
	ON_COMMAND(ID_REDO, OnRedo)
	ON_COMMAND(ID_SNIPPETS, OnSnippets)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

#define KEYWORDS "self new and del from not while as elif global or with assert else if pass yield break except import print class exec in raise continue finally is def for lambda try return True False"

void CScriptDlg::OnNew() 
{
	m_Script.SetText("");
}

void CScriptDlg::OnLoad() 
{
	char openFilters[]=
	"Python Files (*.py)|*.py|All Files (*.*)|*.*||";

	// Load selector
	CFileDialog pdlg (TRUE, "Python Files (*.py)", "*.py", OFN_FILEMUSTEXIST, openFilters);
	if (pdlg.DoModal() == IDOK)
		m_Script.LoadFile(pdlg.GetPathName());
}

void CScriptDlg::OnSave() 
{
	char openFilters[]=
	"Python Files (*.py)|*.py|All Files (*.*)|*.*||";

	// Load selector
	CFileDialog pdlg (FALSE, "Python Files (*.py)", "*.py", OFN_FILEMUSTEXIST, openFilters);
	if (pdlg.DoModal() == IDOK)
		m_Script.SaveFile(pdlg.GetPathName());
}

void CScriptDlg::OnCut() 
{
	m_Script.Cut();
}

void CScriptDlg::OnCopy() 
{
	m_Script.Copy();
}

void CScriptDlg::OnPaste() 
{
	m_Script.Paste();
}

void CScriptDlg::OnUndo()
{
	m_Script.Undo();
}

void CScriptDlg::OnRedo() 
{
	m_Script.Redo();
}

void CScriptDlg::OnSnippets() 
{
	if (m_Snippets.m_hWnd)
		m_Snippets.DestroyWindow();

	CPoint Pt;
	::GetCursorPos(&Pt);

	m_Snippets.m_pParent = this;
	m_Snippets.Create(IDD_SNIPPETS, this);
	m_Snippets.SetWindowPos(NULL, Pt.x, Pt.y, -1, -1, SWP_NOSIZE);
	m_Snippets.ShowWindow(SW_SHOW);
}

void SetPythonStyle(CScintillaWnd& wind)
{
	wind.SetLexer(SCLEX_PYTHON);
	wind.SendMessage(SCI_STYLESETFORE, SCE_P_COMMENTLINE, RGB(0,128,0));
	wind.SendMessage(SCI_STYLESETFORE, SCE_P_STRING, RGB(128,60,0));
	wind.SendMessage(SCI_STYLESETFORE, SCE_P_CLASSNAME, RGB(0,0,0));
	wind.SendMessage(SCI_STYLESETBOLD, SCE_P_CLASSNAME, 1);
	wind.SendMessage(SCI_STYLESETFORE, SCE_P_DEFNAME, RGB(0,0,100));
	wind.SendMessage(SCI_STYLESETBOLD, SCE_P_DEFNAME, 1);

	wind.SendMessage(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
	wind.SendMessage(SCI_SETHSCROLLBAR, NULL, false);

	wind.SetDisplayFolding(TRUE);
	wind.SendMessage(SCI_SETKEYWORDS, 0, 
		(long)_T(KEYWORDS));
}

bool StringStartsWith(CString str, CString with)
{
	if(str.GetLength() >= with.GetLength())
	{
		if(str.Left(with.GetLength()) == with)
			return true;
	}
	return false;
}

void CScriptDlg::OnOk()
{
	m_script = m_Script.GetText();

	CDialog::OnOK();	
}

BOOL CScriptDlg::OnInitDialog() 
{
	CExtResizableDialog::OnInitDialog();

	CRect WindowRect;
	CWnd* pSize = GetDlgItem(IDC_SCRIPT);
	pSize->GetWindowRect(&WindowRect);
	pSize->DestroyWindow();
	ScreenToClient(&WindowRect);

	m_Script.Create("Scintilla", 
					WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|WS_BORDER, 
					WindowRect, this, 10000);
	m_Script.Init();

	SetAStyle(STYLE_DEFAULT, RGB(255, 255, 255), RGB(255, 255, 255), 10, "Courier New" );

	//////////////////////////
	// Setup Scintilla window
	//////////////////////////

	m_Script.SendMessage(SCI_SETCARETLINEBACK, RGB(240,240,255),0 ); 
	m_Script.SendMessage(SCI_SETCARETLINEVISIBLE, (long)true, 0);
	
	m_Script.ID = 0;
	m_Script.ModifyStyleEx(0,WS_EX_WINDOWEDGE,0);
	m_Script.SetFocus();

	SetPythonStyle(m_Script);

	// Remove margins to the left
	m_Help.SendMessage(SCI_SETMARGINWIDTHN, 0, 0);
	m_Help.SendMessage(SCI_SETMARGINWIDTHN, 1, 0);

	InitPythonInfo();

	m_Script.SetText(m_script);
		
	HelpMe();
	UpdateList();
	UpdateClassList();

	// Resizing
	dlgMan.Load(this->m_hWnd, "Software\\Construct\\ScriptDlg3a");
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_HELPLINE, ANCHOR_TOP | ANCHOR_LEFT | ANCHOR_RIGHT);
	dlgAnchor.Add(m_Script, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(m_Help, ANCHOR_LEFT | ANCHOR_BOTTOM | ANCHOR_RIGHT);
	dlgAnchor.Add(IDC_FUNCTIONS, ANCHOR_TOP | ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_CLASSES, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDOK, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDCANCEL, ANCHOR_RIGHT | ANCHOR_BOTTOM);

	// Toolbar
	m_wndToolBar.LoadToolBar(IDR_PYTHONTOOLBAR);

	CWnd::RepositionBars(0,0xFFFF,0);

	return TRUE;
}

void CScriptDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_Snippets.m_hWnd)
		m_Snippets.DestroyWindow();

	CExtNCW<CExtResizableDialog>::OnLButtonUp(nFlags, point);
}

void CScriptDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();

	CWnd::RepositionBars(0,0xFFFF,0);

	Invalidate();
}

void CScriptDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	dlgMan.Save();
}

CScintillaWnd* CScriptDlg::GetSelectedScintilla()
{
	CWnd* pWnd = GetFocus();

	return &m_Script;
}

bool IsPartOf(CString entry, CString main)
{
	// This function is used for intellisense to match up
	// possible strings:
	// eg : if you type 'he' then 'hello' could be an option

	if(main.GetLength() < entry.GetLength())
		return false;
	if(main.Left(entry.GetLength()) == entry)
		return true;
	return false;
}
/*
bool CScriptDlg::UpdateFunctionList()
{

	for(int i = m_Functions.GetCount() - 1; i >= 0 ; i--)
	{
		PyIntellFunctions* item = (PyIntellFunctions*)m_Functions.GetItemData(i);
		CString name = item->name;
		if(IsPartOf(m_CurrentFunction, name))
			m_Functions.SetCurSel(i);

		if(name == m_CurrentFunction)
			return true;	// we found it exactly

	}
	return false;
}*/
/*
void CScriptDlg::UpdateLists()
{
	// First, create a map of functions
	// we need to remember the string, and what classes use it
	// ... this is identical to a Function...as that is what a Function remembers
	PyIntellVarMap& items = m_FunctionItems;
	items.clear();
	m_Functions.SetRedraw(false);
	for( PyIntellClassMap::iterator c = m_Intellisense.m_Intellisense.begin(); c!= m_Intellisense.m_Intellisense.end(); c++)
	{
		// loop every class we have	
		for(PyIntellFunctionMap::iterator f = c->second.functions.begin(); f!= c->second.functions.end(); f++)
		{
			// test that our function name is acceptable
			// loop every function in the class
			if(IsPartOf(m_CurrentFunction, f->second.name))
			{
				PyIntellFunctions& item = items[f->second.name];
				item.name = f->second.name;
				item.classes[c->second.name] = &c->second;
			}
		}
	}

	m_Functions.ResetContent();
	for( PyIntellVarMap::iterator v = items.begin(); v!= items.end(); v++)
	{
		int indx = m_Functions.AddString(v->second.name);
		m_Functions.SetItemData(indx, (DWORD_PTR)&v->second);
	}

	for(int i = m_Functions.GetCount()-1; i >= 0 ; i--)
	{
		PyIntellFunctions* item = (PyIntellFunctions*)m_Functions.GetItemData(i);
		CString name = item->name;
		if(IsPartOf(m_CurrentFunction, name))
			m_Functions.SetCurSel(i);

		if(name == m_CurrentFunction)
			i = 0;	// we found it exactly



	}

	m_Functions.SetRedraw(true);
}
*/

bool KeywordToken(char letter)
{
	// Used if we are typing something like != ,  ==,  =,  +,  etc
	if(letter == '-'
		|| letter == '='
		|| letter == '|'
		|| letter == '!'
		|| letter == '+'
		|| letter == '*'
		|| letter == '/'
		|| letter == '('
		|| letter == ')'
		|| letter == '['
		|| letter == ']'
		)
		return true;
	return false;
}

bool ScriptToken(char letter)
{
	if(isalpha(letter ))
		return true;
	if(isdigit(letter))
		return true;
	if(letter == '.')
		return true;

	return false;
}

bool keywordcmp(char* str1, char* str2)
{
	char* string1 = str1;
	char* string2 = str2;
	int str1_len = strlen(string1);
	int str2_len = strlen(string2);

	if(str2_len >= str1_len)
		return false;

	int len = str2_len;

	for(int a = 0; a < len; a++)
	{
		if(*string1 != *string2)
			return false;

		string1 ++;
		string2 ++;
	}

	if(*string1 == '('
		||  *string1 == ' '
		||  *string1 == '\t'
		||  *string1 == '{'
		||  *string1 == '['
		||  *string1 == '\0'
		||  *string1 == '\n'
		||  *string1 == '\r')
		return true;

	
	return false;

}
void CScriptDlg::OnModify()
{
	CScintillaWnd* pSelected = GetSelectedScintilla();

	long pos = pSelected->GetCurrentPosition()-1;
	if(pos < 0)
		pos = 0;

	CString text = pSelected->GetText();

	// Okay our goal is to work out what the user is typing. 
	// for example:
	// myobject.function

	// so we need to get all the stuff on the left and right

	int start = 0;

	m_BeforeDotText = "";
	m_AfterDotText = "";
	m_OperatorText = "";
	m_Dot = false;
/*
	CString selected = pSelected->GetSelectedText();
	if(selected != "")
	{
		selected.Replace(".","");
		m_BeforeDotText = selected;
		UpdateList();
		return;
	}
*/
	// First case: we have typed something like mykeyword.whatever.
	if(ScriptToken(text.GetAt(pos)))
	{
		for(int c = pos; c >= 0; c--) // loop backwards
		{
			if(!ScriptToken(text.GetAt(c)))
			{
				start = c+1;
				c = -1;
			}
		}
		int length = text.GetLength();
		int end = length;
		for(int c = pos+1; c < length; c++) // loop forwards
		{
			if(!ScriptToken(text.GetAt(c)) || text.GetAt(c) == '.')
			{
				end = c;
				c = length;
			}
		}	

		CString token = text.Mid(start, end-start);
		int dot = token.Find('.');
		CString First = token;
		CString Second = "";

		if(dot != -1)
		{
			Second = First.Mid(dot+1, length - dot-1);
			First = First.Left(dot);
			m_Dot = true;
		}

		m_BeforeDotText = First;
		m_AfterDotText = Second;
	}
	else if(KeywordToken(text.GetAt(pos)))// we have typed something else, such as a symbol like ==
	{
		for(int c = pos-1; c >= 0; c--) // loop backwards
		{
			if(!KeywordToken(text.GetAt(c)))
			{
				start = c+1;
				c = -1;
			}
		}
		int length = text.GetLength();
		int end = length;
		for(int c = pos; c < length; c++) // loop forwards
		{
			if(!KeywordToken(text.GetAt(c)))
			{
				end = c-1;
				c = length;
			}
		}	
		m_OperatorText = text.Mid(start, end-start);
	}

	UpdateList();

}
void CScriptDlg::UpdateClassList()
{
	m_Classes.ResetContent();

	for(map<CString, PyInfoClass>::iterator i = m_Intell.classes.begin(); i!= m_Intell.classes.end(); i++)
	{
		int index = m_Classes.AddString(i->first);
		m_Classes.SetItemData(index, (DWORD)&i->second);
	}

}

void CScriptDlg::UpdateList()
{
	m_Functions.SetRedraw(false);
	m_Functions.ResetContent();
	m_Functions.data.clear();

	if(!m_Dot)
	{
		for(PyInfoMap::iterator i = m_IntellMap.begin(); i!= m_IntellMap.end(); i++)
		{
			if(StringStartsWith(i->first, m_BeforeDotText))
			{
				PyInfoMapItem::iterator item = i->second.find(&m_Intell);
				if(item != i->second.end())
				{
					//Check if the name starts
					
					int list_item = m_Functions.AddString(i->first);

					m_Functions.data.push_back(functiondata());
					functiondata& listdata = m_Functions.data.back();

					listdata.m_IntellMap[item->first] = 1;
					listdata.m_InfoType[item->second] = 1;

					m_Functions.SetItemData(list_item, (DWORD)&listdata);
				}
			}
		}

		if(m_BeforeDotText != "")
			m_Functions.SetCurSel(0);

		m_Functions.SetRedraw(true);
		m_Functions.Invalidate();
	}

	else // first.second
	{
		PyInfoMap::iterator i = m_IntellMap.find(m_BeforeDotText);
		if(i == m_IntellMap.end())
		{
			// Add as new variable if we dont have it listed
			PyInfoVar* pVar = &m_Intell.variables[m_BeforeDotText];
			pVar->pParent = &m_Intell;
			pVar->name = m_BeforeDotText;

			PyInfoMapItem& Item = m_IntellMap[m_BeforeDotText];
			Item[&m_Intell] = PY_VARIABLE;

			i = m_IntellMap.find(m_BeforeDotText); 
		}

		PyInfoClass* pParent =  i->second.begin()->first;

		PyInfoVar* pVar = NULL;
		PyInfoClass* pClass = NULL;

		map<CString, PyInfoVar>::iterator v = pParent->variables.find(m_BeforeDotText);
		if(v != pParent->variables.end())
			pVar = &v->second;

		map<CString, PyInfoClass>::iterator c = pParent->classes.find(m_BeforeDotText);
		if(c != pParent->classes.end())
			pClass = &c->second;
		
		// We loop each string in our intellisense map, and with each string we loop 
		// which type it belongs to. Then we compare it
		if(pVar)
		{
			for(PyInfoMap::iterator i = m_IntellMap.begin(); i!= m_IntellMap.end(); i++)
			{
				//Check if the name starts
				if(StringStartsWith(i->first, m_AfterDotText))
				{	
					if(pVar->classtypes.size() == 0)
					{
						functiondata listdata;

						for(PyInfoMapItem::iterator item = i->second.begin(); item != i->second.end(); item++)
						{
							if(item->first == &m_Intell)
								continue;
							
							listdata.m_IntellMap[item->first] = 1;
							listdata.m_InfoType[item->second] = 1;
						}
						if(listdata.m_InfoType.size() > 0)
						{
							int list_item = m_Functions.AddString(i->first);

							m_Functions.data.push_back(listdata);
							functiondata& listdata = m_Functions.data.back();

							m_Functions.SetItemData(list_item, (DWORD)&listdata);
						}
					}
					else
					{
						functiondata listdata;

						for(map<PyInfoClass*,int>::iterator mi = pVar->classtypes.begin(); mi!= pVar->classtypes.end(); mi++)
						{
							PyInfoMapItem::iterator item = i->second.find(mi->first);
							if(item != i->second.end())
							{
								listdata.m_IntellMap[item->first] = 1;
								listdata.m_InfoType[item->second] = 1;
							}
						}

						if(listdata.m_InfoType.size() > 0)
						{
							int list_item = m_Functions.AddString(i->first);

							m_Functions.data.push_back(listdata);
							functiondata& listdata = m_Functions.data.back();

							m_Functions.SetItemData(list_item, (DWORD)&listdata);
						}
					}

				}
			}
		}
		else if(pClass)
		{
			for(PyInfoMap::iterator i = m_IntellMap.begin(); i!= m_IntellMap.end(); i++)
			{
				//Check if the name starts
				if(StringStartsWith(i->first, m_AfterDotText))
				{
					PyInfoMapItem::iterator item = i->second.find(pClass);
					if(item != i->second.end())
					{
						m_Functions.data.push_back(functiondata());
						functiondata& listdata = m_Functions.data.back();
		
						listdata.m_IntellMap[item->first] = 1;
						listdata.m_InfoType[item->second] = 1;
						
						int list_item = m_Functions.AddString(i->first);

						m_Functions.SetItemData(list_item, (DWORD)&listdata);		
					}
				}
			}
		}
		m_Functions.SetCurSel(0);
		m_Functions.SetRedraw(true);
		m_Functions.Invalidate();
	}
	UpdateHelpLine();
}

void CScriptDlg::UpdateHelpLine()
{
	int sel = m_Functions.GetCurSel();

	if(sel == -1)
		return;

	m_HelpLine.SetWindowText("");

	functiondata* pData = (functiondata*)m_Functions.GetItemData(sel);
	CString name;
	m_Functions.GetText(sel,name);

	CString help = "";
	PyInfoMapItem* pItem = &m_IntellMap[name];
	for(PyInfoMapItem::iterator i = pItem->begin(); i!= pItem->end(); i++)
	{
		PyInfoClass* pClass = i->first;

		map<CString, PyInfoClass>::iterator c = pClass->classes.find(name);
		if(c!= pClass->classes.end())
		{
			help = c->second.help;
			break;
		}
		map<CString, PyInfoFunc>::iterator f = pClass->functions.find(name);
		if(f!= pClass->functions.end())
		{
			help = f->second.help;
			break;
		}
		map<CString, PyInfoVar>::iterator v = pClass->variables.find(name);
		if(v!= pClass->variables.end())
		{
			help = v->second.help;
			break;
		}
	}
	
	m_HelpLine.SetWindowText(help);
}

void CScriptDlg::OnChar(char ch)
{
	CScintillaWnd* pSelected = GetSelectedScintilla();

	long pos = pSelected->GetCurrentPosition();
	CString text = pSelected->GetText();

	if(pos > 3)
	{
		if(text.GetAt(pos-2) == '=')
		{
			if(text.GetAt(pos-3) != '!'
				&& text.GetAt(pos-3) != '<'
				&& text.GetAt(pos-3) != '>')
			{
				// we have typed something like x = 0
				// check to see if theres an 'if' at the start of the line
				
				char linebuf[1000];
				int curLine  =  pSelected->SendMessage(SCI_LINEFROMPOSITION, pos);
				int LineLength  =  pSelected->SendMessage(SCI_LINELENGTH,  curLine);

				WORD buflen = sizeof(linebuf);
				memcpy(linebuf,  &buflen,  sizeof(buflen));
				pSelected->SendMessage(SCI_GETLINE,  curLine,
				reinterpret_cast<LPARAM>(static_cast<char  *>(linebuf)));
				linebuf[LineLength]  =  '\0';

				char* strpos = &linebuf[0];
				for(int a = 0; a < LineLength; a++)
				{
					if(linebuf[a] == '\t' || linebuf[a] == ' ')
						strpos ++;
					else
						break;
				}

				if(keywordcmp(strpos, "if") || keywordcmp(strpos, "while") )
				{
					//okay the user has type something like: if x=0
					// so go back a char, insert another = so the user gets
					// if x==0
					pSelected->GotoPosition(pos-1);
					char equals[] = "=";
					 
					pSelected->SendMessage(SCI_REPLACESEL,  0, (LPARAM)equals);

					pos ++;
					pSelected->GotoPosition(pos);
				}
			}
		}
	}
	// The following code enables autoindent and autocolon to help out the programmer.

	if  (ch  ==  '\r'  ||  ch  ==  '\n') 
	{
		 char  linebuf[1000];
		 int  curLine  =  pSelected->SendMessage(SCI_LINEFROMPOSITION, pos+1);
		 int  lineLength  =  pSelected->SendMessage(SCI_LINELENGTH,  curLine);

		 if  (curLine  >  0  &&  lineLength  <=  2)  
		 {
			 int  prevLineLength  =  pSelected->SendMessage(SCI_LINELENGTH,  curLine  -  1);
			 if  (prevLineLength  <  sizeof(linebuf))  
			 {
				 WORD  buflen  =  sizeof(linebuf);
				 memcpy(linebuf,  &buflen,  sizeof(buflen));
				 pSelected->SendMessage(SCI_GETLINE,  curLine  -  1,
						reinterpret_cast<LPARAM>(static_cast<char  *>(linebuf)));
				 linebuf[prevLineLength]  =  '\0';
	
			
				 bool indent = false;
				 bool addcolon = false;

				 for  (int  p  =  0;  linebuf[p];  p++)  
				 { 
					 // note: space at the end to prevent errors
					 if (keywordcmp(&linebuf[p], "def")
						|| keywordcmp( &linebuf[p], "class")
						|| keywordcmp( &linebuf[p], "for")	
						|| keywordcmp( &linebuf[p], "while")
						|| keywordcmp( &linebuf[p], "if"))
					 {
						 addcolon = true;
						 indent = true;
					 }
					 if(keywordcmp(&linebuf[p], ":"))
					 {
						 indent = true;
						 addcolon = false;
					 }			 
				 }

				 int bracketdepth = 0;
				 for  (int  p  =  0;  linebuf[p];  p++)  
				 { 
					if (linebuf[p] == '(')
						 bracketdepth ++;
					 if (linebuf[p] == ')')
						 bracketdepth --;
					 if  (linebuf[p]  !=  ' '  &&  linebuf[p]  !=  '\t')
						linebuf[p]  =  '\0';

				 }

				 while(bracketdepth > 0)
				 {
					 pSelected->GotoPosition(pos-1);
					 char colon[] = ")";
					 
					 pSelected->SendMessage(SCI_REPLACESEL,  0, (LPARAM)colon);

					 pos ++;
					 pSelected->GotoPosition(pos);
					 bracketdepth --;
				 }
				 if(addcolon)
				 {
					 pSelected->GotoPosition(pos-1);
					 char colon[] = ":";
					 pSelected->SendMessage(SCI_REPLACESEL,  0, (LPARAM)colon);
					 pSelected->GotoPosition(pos+1);
				 }
				 pSelected->SendMessage(SCI_REPLACESEL,  0, (LPARAM)linebuf);
				 if(indent)
				 {
					 char tab[] = "\t";
					 pSelected->SendMessage(SCI_REPLACESEL,  0, (LPARAM)tab);
				 }
			 }
		}

	}
	if(ch == ')')
	{
		char linebuf[1000];
		int curLine  =  pSelected->SendMessage(SCI_LINEFROMPOSITION, pos);
		int LineLength  =  pSelected->SendMessage(SCI_LINELENGTH,  curLine);

		WORD buflen = sizeof(linebuf);
		memcpy(linebuf,  &buflen,  sizeof(buflen));
		pSelected->SendMessage(SCI_GETLINE,  curLine,
		reinterpret_cast<LPARAM>(static_cast<char  *>(linebuf)));
		linebuf[LineLength]  =  '\0';
		int bracketlevel = 0;
		for( int c = 0; c < LineLength; c++)
		{
			if(linebuf[c] == '(')
				bracketlevel ++;
			if(linebuf[c] == ')')
				bracketlevel --;
		}
		if(bracketlevel < 0)
		{
			pSelected->SendMessage(SCI_SETSEL, pos-1, pos);
			char blank[] = "";
			pSelected->SendMessage(SCI_REPLACESEL,  0, (LPARAM)blank);

		}
	}
	return;
}

void CScriptDlg::HelpMe()
{
	// When we double click on scintilla, it highlights a word. We use this word to get help information
	CScintillaWnd* pSelected = GetSelectedScintilla();
	CString selected = pSelected->GetSelectedText();

	selected.MakeLower();

	CPath Path;
	Path.SetToCurrentDirectory();

	CString Base;
	Base.Format("%sPyAutoC\\Help\\", Path.GetFullPath());

	TRY
	{
		CFile File(Base + selected + ".py", CFile::modeRead);

		char* ReadIn = new char[File.GetLength() + 1];
		File.Read(ReadIn, File.GetLength());
		ReadIn[File.GetLength()] = '\0';

		// Store details
		int Length = File.GetLength();
		CString Info;
		Info.Format("%s", ReadIn);

		// Delete memory
		delete[] ReadIn;

		m_Help.SetText(Info);
		m_Help.SendMessage(SCI_SETSEL,0,0);
	}
	CATCH(CFileException, e)
	{
		TRY
		{
			CFile File(Base + "default.txt", CFile::modeRead);

			char* ReadIn = new char[File.GetLength() + 1];
			File.Read(ReadIn, File.GetLength());
			ReadIn[File.GetLength()] = '\0';

			// Store details
			int Length = File.GetLength();
			CString Info;
			Info.Format("%s", ReadIn);

			// Delete memory
			delete[] ReadIn;

			m_Help.SetText(Info);
		}
		CATCH(CFileException, e)
		{
			// no help and no default file! do nothing
			return;
		}
		END_CATCH

		// no help for this line
		return;
	}
	END_CATCH
}
/*
void PyIntell::FillIntellisense()
{
	CPath Path;
	Path.SetToCurrentDirectory();

	CString Base;
	Base.Format("%sIntellisense\\", Path.GetFullPath());

	CString Folder;
	Folder = Base + "*";

	WIN32_FIND_DATA FindFileData;

	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(Folder, &FindFileData);

	DWORD id = 0;

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		CErrorDlg error;
		error.Error("Intellisense Directory Missing", "No intellisense folder was found in your install directory. Intellisense will be unavailable to assist you.");     
	} 

	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			CString name = FindFileData.cFileName;
			name = name.Left(name.GetLength()-4);
			if(name == "")
				continue;
			
			// Add class 
			PyIntellClass& Class = m_Intellisense[name];
			Class.name = name;

			// Add the functions in the class
			CString FilePath = Base + FindFileData.cFileName;
			
			CFile File(FilePath, CFile::modeRead);
			char* ReadIn = new char[File.GetLength() + 1];
			File.Read(ReadIn, File.GetLength());
			ReadIn[File.GetLength()] = '\0';

			// Store details
			int Length = File.GetLength();
			CString Functions;
			Functions.Format("%s", ReadIn);

			// Delete memory
			delete[] ReadIn;

			// Read in functions
			int Start = 0;
			int End = 0;

			while(End = Functions.Find('\n', Start))
			{
				if(End == -1)
					End = Length+1;

				CString Function = Functions.Mid(Start, End - Start - 1);
				Start = End + 1;

				// Add function
				PyIntellFunction& Funct = Class.functions[Function];
				Funct.name = Function;

				if(End == Length+1)
					break;
			}

			// Make sure to close file
			File.Close();
		}
	}
}
*/
BOOL CScriptDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		if (pMsg->wParam == VK_RETURN)
		{
		/*	if(m_CurrentFunction == "")
			{
				int sel = m_Functions.GetCurSel();
				if(sel != -1)
				{
					CString item = ((PyIntellFunctions*)m_Functions.GetItemData(sel))->name;
					if(item != m_CurrentFunction)
					{
						OnEnterFunction();
						return true;
					}
				}
			}
			else
			{
				// Insert function that's highlighted
				int sel = m_Functions.GetCurSel();
				if(sel != -1)
				{
					CString item = ((PyIntellFunctions*)m_Functions.GetItemData(sel))->name;
					if(item != m_CurrentFunction)
					{
						OnEnterFunction();
						return true;
					}
				}
			}
			*/
		}
	}
	return CExtResizableDialog::PreTranslateMessage(pMsg);
}



void CScriptDlg::OnEnterFunction()
{
	CScintillaWnd* pSelected = GetSelectedScintilla();

/*	long pos = pSelected->GetCurrentPosition();
	CString text = pSelected->GetText();

	// Okay our goal is to work out what the user is typing. All we care about is stuff like
	// myobject.function()

	// so we need to get all the stuff on the left and right

	int start = 0;
	for(int c = pos-1; c >= 0; c--) // loop backwards
	{
		if(!ScriptToken(text.GetAt(c)))
		{
			start = c+1;
			c = -1;
		}
	}
	int length = text.GetLength();
	int end = length;
	for(int c = pos; c < length; c++) // loop forwards
	{
		if(!ScriptToken(text.GetAt(c)))
		{
			end = c-1;
			c = length;
		}
	}	
	CString token = text.Mid(start, end-start);
	int dot = text.Find('.');

	if(dot == -1)
		return; //wtf :P


	int sel = m_Functions.GetCurSel();
	if(sel == -1)
		return;

	CString item = ((PyIntellFunctions*)m_Functions.GetItemData(sel))->name;

	text = text.Left(dot+start+1) + item + text.Mid(end, length-end);
	pSelected->SetText(text);
	pSelected->GotoPosition(dot+start+item.GetLength()+1);
*/
}

BOOL CScriptDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	


	NMHDR *phDR;
	phDR = (NMHDR*)lParam;

	// Does notification come from scintilla window?
	if (phDR !=  NULL)
	{
		CScintillaWnd* m_wndScintilla = NULL;
		if(phDR->hwndFrom == m_Script.m_hWnd)
			m_wndScintilla = &m_Script;
		if(phDR->hwndFrom == m_Help.m_hWnd)
			m_wndScintilla = &m_Help;

		SCNotification *pMsg = (SCNotification*)lParam;
		switch(phDR->code)       
		{ 
			case SCN_SAVEPOINTLEFT:
				break;

			case SCN_UPDATEUI:
				m_wndScintilla->UpdateUI();
				OnModify();
				break; 

			case SCN_MARGINCLICK:
				 m_wndScintilla->DoDefaultFolding(pMsg->margin,pMsg->position);
				break;

			case SCN_DOUBLECLICK:
				HelpMe();
				break;

			case SCN_CHARADDED:
			{
				SCNotification *scn = (SCNotification*)phDR;
				OnChar(scn->ch);
				OnModify();
				break;
			}

			case SCN_MODIFIED:
				OnModify();
				break;

		}
		return TRUE; 
   }

   return CWnd::OnNotify(wParam, lParam, pResult);
}
void CScriptDlg::OnEnChangeScript()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CExtNCW<CExtResizableDialog>::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}



void CFunctionListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	 //CRect members to store the position of the items
	CRect rItem;
	CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);

	if ((int)lpDrawItemStruct->itemID < 0)
	{
		// If there are no elements in the CListBox
		// based on whether the list box has Focus  or not
		// draw the Focus Rect or Erase it,
		if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			dc->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			dc->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		   return;
	}


	// String to store the text
	CString strText;

	// Get the item text.
	GetText(lpDrawItemStruct->itemID, strText);

	//Initialize the CListBox Item's row size
	rItem = lpDrawItemStruct->rcItem;

	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
	nFormat |= DT_EXPANDTABS;


	// If CListBox item selected, draw the highlight rectangle.
	// Or if CListBox item deselected, draw the rectangle using the window color.
	bool ItemSelected = (lpDrawItemStruct->itemState & ODS_SELECTED);

	COLORREF crBG = RGB(255,255,255);
	COLORREF crText = RGB(0,0,0);

	functiondata* pData = (functiondata*)lpDrawItemStruct->itemData;
	if(pData)
	{
		if(pData->m_InfoType.size() == 1)
		{
			infoType type = pData->m_InfoType.begin()->first;
			if(type == PY_CLASS)
			{
				crText = RGB(0,0,128);
				crBG = RGB(240,240,255);
			}
			if(type == PY_VARIABLE)
			{
				crText = RGB(128,0,0);
				crBG = RGB(255,240,240);
			}
			if(type == PY_KEYWORD)
			{
				crText = RGB(0,0,255);
				crBG = RGB(255,255,255);
			}
			if(type == PY_OPERATOR)
			{
				crText = RGB(128,128,128);
				crBG = RGB(240,240,240);
			}
		}
	}
	if (ItemSelected)
	{
		crBG = crText;
		crText = 0xffffff;	
	}

	CBrush br(crBG);
	dc->FillRect(&rItem, &br);


	// If the CListBox item has focus, draw the focus rect.
	// If the item does not have focus, erase the focus rect.
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		dc->DrawFocusRect(&rItem);
	}
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		dc->DrawFocusRect(&rItem);
	}

	// To draw the Text in the CListBox set the background mode to Transparent.
	int iBkMode = dc->SetBkMode(TRANSPARENT);

	CFont font;
	font.CreatePointFont(100,"Courier New");

	dc->SetTextColor(crText);
	dc->SelectObject(&font);

	//Draw the Text
	dc->TextOut(rItem.left,rItem.top,strText);

}

void CClassListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	 //CRect members to store the position of the items
	CRect rItem;
	CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);

	if ((int)lpDrawItemStruct->itemID < 0)
	{
		// If there are no elements in the CListBox
		// based on whether the list box has Focus  or not
		// draw the Focus Rect or Erase it,
		if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			dc->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			dc->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		   return;
	}


	// String to store the text
	CString strText;

	// Get the item text.
	GetText(lpDrawItemStruct->itemID, strText);

	//Initialize the CListBox Item's row size
	rItem = lpDrawItemStruct->rcItem;

	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
	nFormat |= DT_EXPANDTABS;


	// If CListBox item selected, draw the highlight rectangle.
	// Or if CListBox item deselected, draw the rectangle using the window color.
	bool ItemSelected = (lpDrawItemStruct->itemState & ODS_SELECTED);
	bool ItemChecked = (lpDrawItemStruct->itemState & ODS_CHECKED);

	COLORREF crBG = RGB(255,255,255);
	COLORREF crText = RGB(0,0,255);

	if (ItemSelected)
	{
		crBG = crText;
		crText = 0xffffff;	
	}

	CBrush br(crBG);
	dc->FillRect(&rItem, &br);

	


	// If the CListBox item has focus, draw the focus rect.
	// If the item does not have focus, erase the focus rect.
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		dc->DrawFocusRect(&rItem);
	}
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		dc->DrawFocusRect(&rItem);
	}

	// To draw the Text in the CListBox set the background mode to Transparent.
	int iBkMode = dc->SetBkMode(TRANSPARENT);

	CFont font;
	font.CreatePointFont(100,"Courier New");

	dc->SetTextColor(crText);
	dc->SelectObject(&font);

	//Draw the Text
	dc->TextOut(rItem.left,rItem.top,strText);

}




bool KeywordLetter(char c)
{
	if(c >= 'a' && c <= 'z')
		return true;
	if(c >= 'A' && c <= 'Z')
		return true;
	if(c == '_')
		return true;
	if(c >= '1' && c <= '9')
		return true;
	if(c == '0')
		return true;
	if(c == '_')
		return true;

	return false;
}

CString ExtractKeyword(CString line) // "  function(..)     " will return "function"
{
	int length = line.GetLength();
	int start = -1;
	int end = length;

	for(int a = 0; a < length; a++)
	{
		if(KeywordLetter(line.GetAt(a)))
		{
			start = a;
			a = length;
		}
	}

	if(start == -1)
		return CString("");

	for(int a = start; a < length; a++)
	{
		if(!KeywordLetter(line.GetAt(a)))
		{
			end = a;
			a = length;
		}
	}
	return line.Mid(start, end-start);
}

void CScriptDlg::InitPythonInfo()
{
	// First add some standard keywords into our intellisense
	CString keywords = KEYWORDS; // 
	keywords += " ";

	CString _keyword = "";
	for(int i = 0; i < keywords.GetLength(); i++)
	{
		char letter = keywords.GetAt(i);
		if(letter == ' ')
		{
			// Add our keyword
			PyInfoKeyword* key = &m_Intell.keywords[_keyword];
			key->name = _keyword;
			key->pParent = &m_Intell;

			m_IntellMap[_keyword][&m_Intell] = PY_KEYWORD;
			
			_keyword = "";
		}
		else 
			_keyword += letter;
	}
	
	// Now add operators
/*	m_IntellMap["+"][&m_Intell] = PY_OPERATOR;	
	m_IntellMap["*"][&m_Intell] = PY_OPERATOR;		
	m_IntellMap["/"][&m_Intell] = PY_OPERATOR;
	m_IntellMap["-"][&m_Intell] = PY_OPERATOR;
	m_IntellMap["="][&m_Intell] = PY_OPERATOR;
	m_IntellMap["=="][&m_Intell] = PY_OPERATOR;
	m_IntellMap["!="][&m_Intell] = PY_OPERATOR;
	m_IntellMap["<"][&m_Intell] = PY_OPERATOR;
	m_IntellMap[">"][&m_Intell] = PY_OPERATOR;
	m_IntellMap["<="][&m_Intell] = PY_OPERATOR;
	m_IntellMap[">="][&m_Intell] = PY_OPERATOR;
	m_IntellMap["("][&m_Intell] = PY_OPERATOR;
	m_IntellMap[")"][&m_Intell] = PY_OPERATOR;
	m_IntellMap["["][&m_Intell] = PY_OPERATOR;
	m_IntellMap["]"][&m_Intell] = PY_OPERATOR;
	m_IntellMap[":"][&m_Intell] = PY_OPERATOR;
	m_IntellMap["+="][&m_Intell] = PY_OPERATOR;
	m_IntellMap["-="][&m_Intell] = PY_OPERATOR;
	m_IntellMap["*="][&m_Intell] = PY_OPERATOR;
	m_IntellMap["/="][&m_Intell] = PY_OPERATOR;*/
	// Load info from file

	CPath Path;
	Path.SetToCurrentDirectory();
	
	CString FilePath;
	FilePath.Format("%sPyAutoC\\info.txt", Path.GetFullPath());
	
	CFile File(FilePath, CFile::modeRead);
	char* ReadIn = new char[File.GetLength() + 1];
	File.Read(ReadIn, File.GetLength());
	ReadIn[File.GetLength()] = '\0';	
	
	// Store details
	int Length = File.GetLength();
	CString Functions;
	Functions.Format("%s", ReadIn);	


	// Read in functions
	int Start = 0;
	int End = 0;

	list<CString> infoList;

	while(End = Functions.Find('\n', Start))
	{
		if(End == -1)
			End = Length+1;

		CString Function = Functions.Mid(Start, End - Start - 1);
		Start = End + 1;

		infoList.push_back(Function);

		if(End == Length+1)
			break;
	}

	// Make sure to close file
	File.Close();


/////////////////////////////////////////////

	list<CString>::iterator i;
	i = infoList.begin();


	for(;i!= infoList.end(); i++)
	{
		if(StringStartsWith(*i, "    class "))
		{
			CString className = ExtractKeyword( (*i).Mid(10) );
			
			if(!className.IsEmpty())
			{
				// Add a new class
				PyInfoClass* pClass = &m_Intell.classes[className];
				pClass->pParent = &m_Intell;
				pClass->name = className;

				m_IntellMap[className][&m_Intell] = PY_CLASS;

				i++;
				if(i != infoList.end()) 
				{
					if(StringStartsWith(*i, "     |  "))
					{
						pClass->help = (*i).Mid(8);
					}
			
					bool addFunctions = false;
					bool addVariables = false;
					for(i++; i!= infoList.end(); i++)
					{
						if(StringStartsWith(*i, "     |  Methods"))
						{
							addFunctions = true;
							addVariables = false;
						}
						else if(StringStartsWith(*i, "     |  Data"))
						{
							addFunctions = false;
							addVariables = true;
						}
						else if(StringStartsWith(*i, "     |  ----"))
						{
							addFunctions = false;
							addVariables = false;
						}
						else if(!StringStartsWith(*i, "     |"))
						{
							addFunctions = false;
							addVariables = false;
							break; // break out of our for loop - ie: finish processing this class
						}
						else if(!StringStartsWith(*i, "     |      "))
						{
							CString name = ExtractKeyword(*i);
							if(!name.IsEmpty())
							{
								if(addFunctions)
								{
									// Add a function to the class
									PyInfoFunc* pFunc = &pClass->functions[name];
									pFunc->pParent = pClass;
									pFunc->name = name;

									m_IntellMap[name][pClass] = PY_FUNCTION;
									i++;
									if(i != infoList.end()) 
									{
										pFunc->help = (*i).Mid(12);
									}
								}
								if(addVariables)
								{
									// Add a variable to the class
									PyInfoVar* pVar = &pClass->variables[name];
									pVar->pParent = pClass;
									pVar->name = name;

									m_IntellMap[name][pClass] = PY_VARIABLE;
									i++;
									if(i != infoList.end()) 
									{
										pVar->help = (*i).Mid(12);
									}
								}
							}
						}
					}
				}
			}
		} // if class
		
		// Main functions
		else if(StringStartsWith(*i, "    ") && !StringStartsWith(*i, "        "))
		{
			CString name = ExtractKeyword(*i);
			if(!name.IsEmpty())
			{
				PyInfoFunc* pFunc = &m_Intell.functions[name];
				pFunc->pParent = &m_Intell;
				pFunc->name = name;

				m_IntellMap[name][&m_Intell] = PY_FUNCTION;
				i++;
				if(i != infoList.end())
				{
					pFunc->help = (*i).Mid(8); 
				}
			}
		}
	}
	//////////////////////////////
	// Parsing of txt file complete

	InitPythonForObjects();

}

PyInfoClass::PyInfoClass()
{
	pParent = NULL;
}
PyInfoVar::PyInfoVar()
{
	pParent = NULL;
}
PyInfoFunc::PyInfoFunc()
{
	pParent = NULL;
}
PyInfoKeyword::PyInfoKeyword()
{	
	pParent = NULL;
}

void CScriptDlg::OnCheckClass()
{
	CString name;
	m_Functions.GetText(m_Functions.GetCurSel(), name);
	functiondata* pData = (functiondata*)m_Functions.GetItemData(m_Functions.GetCurSel());
	PyInfoClass* pClass = pData->m_IntellMap.begin()->first;
	map<CString, PyInfoVar>::iterator v = pClass->variables.find(name);
	if(v==pClass->variables.end())
		return; // something went wrong
	PyInfoVar* pVar = &v->second;
	pVar->classtypes.clear();

	for(int a = 0; a < m_Classes.GetCount(); a++)
	{
		if(m_Classes.GetCheck(a))
		{
			pVar->classtypes[(PyInfoClass*)m_Classes.GetItemData(a)] = 1;
		}
	}

}

void CScriptDlg::OnDblClickFunction()
{
	CScintillaWnd* pSelected = GetSelectedScintilla();

	int item = m_Functions.GetCurSel();
	if(item != -1)
	{
		CString name;
		m_Functions.GetText(item, name);
		if(m_Dot)
			name = name.Mid(m_AfterDotText.GetLength());
		else
			name = name.Mid(m_BeforeDotText.GetLength());

		pSelected->SendMessage(SCI_REPLACESEL,  0, (LPARAM)name.GetBuffer());
		pSelected->SetFocus();
	}


}

void CScriptDlg::OnFunctionSelChange()
{
	UpdateHelpLine();
}


void CScriptDlg::InitPythonForObjects()
{
	if(pApp == NULL)
		return;


	POSITION pos = pApp->object_types.GetStartPosition();
	CObjType* oT;
	long nKey = 0;
	int objectnumber = 0;
	while (pos != NULL) 
	{	
		pApp->object_types.GetNextAssoc(pos, nKey, oT);
		if(oT && !oT->m_bIsGroupType)
		{
			CString name = oT->GetName();
			name.Replace("&", "");
			PyInfoClass* pClass = &m_Intell.classes[name];
			InitPythonForObject(oT, pClass);

			PyInfoMapItem& item = m_IntellMap[name];
			item[&m_Intell] = PY_CLASS;
		}
	}

	// System
	oT = &pApp->m_System;
	PyInfoClass* pClass = &m_Intell.classes[oT->GetName()];
	InitPythonForObject(oT, pClass);

	PyInfoMapItem& item = m_IntellMap[oT->GetName()];
	item[&m_Intell] = PY_CLASS;
}

void CScriptDlg::InitPythonForObject(CObjType* pType, PyInfoClass* pClass)
{
	OINFO* oInfo = GetOINFO(pType->DLLIndex);
	
	map<CString, int> possibleAttribute;

	int count = pType->GetTableCount(ACTION);
	for(int i = 0; i < count; i++)
	{
		ACESEntry2* a = pType->GetACESEntry(ACTION, i);


		if(a->scriptName == "")
			continue;

		if(a->params.size() == 1)
		{
			if(a->scriptName.GetLength() > 3)
			{
				if(a->scriptName.Left(3) == "Set")
				{
					possibleAttribute[a->scriptName.Mid(3)] = 1;
				}
			}
		}
		if(a->scriptName.Left(1) == "(" || a->scriptName.Left(1) == "<")
		{
			continue;
		}
		PyInfoFunc* pFunct = &pClass->functions[a->scriptName];
		pFunct->name = a->scriptName;
		pFunct->pParent = pClass;


		CString help = CString("Action: ") + pFunct->name;
		help += "(";
		for(int p = 0; p < a->params.size(); p++)
		{
			if(p>0)
				help += ", ";
			help += a->params.at(p).name;
		}
		help += ")";
		pFunct->help = help;



		PyInfoMapItem& item = m_IntellMap[a->scriptName];
		item[pClass] = PY_FUNCTION;
	}

	count = pType->GetTableCount(CONDITION);
	for(int i = 0; i < count; i++)
	{
		ACESEntry2* c = pType->GetACESEntry(CONDITION, i);

		if(c->scriptName == "")
			continue;

		if(c->scriptName.Left(1) == "("|| c->scriptName.Left(1) == "<")
			continue;

		PyInfoFunc* pFunct = &pClass->functions[c->scriptName];
		pFunct->name = c->scriptName;
		pFunct->pParent = pClass;

		CString help = CString("Condition: ") + pFunct->name;
		help += "(";
		for(int p = 0; p < c->params.size(); p++)
		{
			if(p>0)
				help += ", ";
			help += c->params.at(p).name;
		}
		help += ")";
		pFunct->help = help;

		PyInfoMapItem& item = m_IntellMap[c->scriptName];
		item[pClass] = PY_FUNCTION;
	}

	count = pType->GetTableCount(EXPRESSION);
	for(int i = 0; i < count; i++)
	{
		ACESEntry2* e = pType->GetACESEntry(EXPRESSION, i);
		if(e->scriptName == "")
			continue;
		if(e->scriptName.Left(1) == "(" || e->scriptName.Left(1) == "<")
			continue;


		if(e->params.size() == 0)
		{
			map<CString, int>::iterator find = possibleAttribute.find(e->scriptName);
			//if(find != possibleAttribute.end())
			{
				PyInfoVar* pVar = &pClass->variables[e->scriptName];
				pVar->name = e->scriptName;
				pVar->pParent = pClass;

				PyInfoMapItem& item = m_IntellMap[e->scriptName];
				item[pClass] = PY_VARIABLE;

				continue;
			}
		}
		PyInfoFunc* pFunct = &pClass->functions[e->scriptName];
		pFunct->name = e->scriptName;
		pFunct->pParent = pClass;

		CString help = CString("Expression: ") + pFunct->name;
		help += "(";
		for(int p = 0; p < e->params.size(); p++)
		{
			if(p>0)
				help += ", ";
			help += e->params.at(p).name;
		}
		help += ")";
		pFunct->help = help;

		PyInfoMapItem& item = m_IntellMap[e->scriptName];
		item[pClass] = PY_FUNCTION;
	}

}
void CScriptDlg::OnEnChangeHelp()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CExtNCW<CExtResizableDialog>::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
