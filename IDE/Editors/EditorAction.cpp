#include "stdafx.h"
#include "ChronoEventEditor\ChronoEventEditor.h"
#include "Event Sheet Editor\Event Sheet Editor.h"

void CEditorAction::Init()
{
	actID = 0;
	oid = -1;
	mid = -1;
	m_Anim.m_bDestroyed = false;
	m_select_default  = false;
	m_rect_default = CRect(0,0,16,16);
	m_MovementDLLIndex = -1;
	m_select_default = false;
	m_bEnabled = true;
}

CEditorAction::~CEditorAction()
{
	for(int p = 0; p < params.size(); p++)
		delete params[p];
}

bool CEditorAction::Valid(CApplication* pApp)
{
	// if the user removes an object sometimes the conditions / actions can still refer to the removed object
	// and this causes a headache

	// uberhack: set global filter removed
	if (oid == -1 && actID == 7)
		return false;

	CObjType* objecttype = pApp->FindObjTypeFromNumber(oid);
	if(objecttype )
	{
		if(mid < -1)
		{
			//check that the effect still exists
			if(!objecttype->GetEffect(mid * -1))
				return false;
		}
		if(mid > -1)
		{
			if(!objecttype->GetBehaviorByUniqueID(mid))
				return false;
		}
		
		// loop each param;
		for(int p = 0; p < params.size(); p++)
		{
			CEditorParam* param = params.at(p);
			if(param->m_type == 14) //variable
			{
				// lookup the variable
				if(!objecttype->GetPrivateVariable(atoi(param->CreateParamString(pApp))))
					return false;
			}
			if(param->m_type == 13) //global
			{
				if(!pApp->GetGlobalVariable(atoi(param->CreateParamString(pApp))))
					return false;
			}
			if(param->m_type == EDITORPARAM_OBJECT)
			{
				CString stparam = param->CreateParamString(pApp);
				int o = atoi(stparam);
				if(o == 0 && stparam != "0")
					return true; //attribute
				
				if(!pApp->FindObjTypeFromNumber( o))
					return false;

			}
			if(pApp)
				param->CreateParamString(pApp); //Calling this function updates valid...terrible design but meh
			if(param->valid == false)
				return false;
		}
		return true;
	
	}
	return false;
}

bool CEditorAction::ValidToDisplay(CApplication* pApp)
{
	if(pApp)
	{
		return m_Valid = Valid(pApp); 
	}
	else
		return m_Valid;
}

void CEditorAction::Render(CRect &Pos, CDC& dc,bool EventSelected, CChronoEventEditor& Ed, int renderType, bool bEnabled)
{
	COLORREF ActionBG	=m_select(&Ed)? ACTION_BG_SEL : (EventSelected? ACTION_BG_EVSEL : ACTION_BG);
	// Animating
	if(m_Anim.m_bDestroyed)
	{
		if(renderType == 3)
		{
			Pos.OffsetRect(0,m_Anim.Space);
		}
		if(renderType == 1)
		{
			Pos.OffsetRect(-30 + m_Anim.Space, 0);
		}
		//dc.FillSolidRect(Calc,BG);
		return;
	}

	

	switch(renderType)
	{
/*		case 0:
		{
			if(m_select(&Ed))
			{
				dc.FillSolidRect(Pos,ActionBG);
				dc.Draw3dRect(Pos,ACTION_BG_SEL2,ACTION_BG_SEL2);
			}

			HBITMAP load = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_DOT));
			HDC hdcScreenCompat = CreateCompatibleDC(dc.m_hDC);
			HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcScreenCompat, load);
			BitBlt(dc.m_hDC, Pos.left + 1, Pos.top + 1, 16, 16, hdcScreenCompat, 0, 0, SRCCOPY);
			SelectObject(hdcScreenCompat, hbmpOld);
			DeleteDC(hdcScreenCompat);
			DeleteObject(load);
		}
		break;*/

		case 1:
		{
			if(m_select(&Ed))
			{
				Pos.InflateRect(3,3);
				dc.FillSolidRect(Pos,ActionBG);
				dc.Draw3dRect(Pos,ACTION_BG_SEL2,ACTION_BG_SEL2);
				Pos.DeflateRect(3,3);
			}
			int icon = 0;
			if(m_Family)
				((EventSheetEditor*)(&Ed))->m_familyhbitmaps.Lookup(oid, icon);
			else
				((EventSheetEditor*)(&Ed))->m_objecthbitmaps.Lookup(oid, icon);

			static HDC dcMem = CreateCompatibleDC(dc.m_hDC);
						HBITMAP old = (HBITMAP)SelectObject(dcMem, Ed.GetIcon(icon));
						BitBlt(dc.m_hDC,Pos.left, Pos.top
							,16
							,16,dcMem,0,0,SRCCOPY);
			SelectObject(dcMem, old);
			//DeleteDC(dcMem); - its static so we cant delete it

			m_Anim.Space = 30;
		}
		break;

		case 2:
		{
			CHTMLFont f;
			f.SetBold(false);
			f.SetSize(8);
			f.SetName("Arial");
			f.SetImage(false);

			if (!bEnabled)
				f.SetStrike(true);

			CRect Calc = Pos;
			Calc.bottom  += m_rect(&Ed).Height();

			// Draw the action
			CString Out = m_Text;
			g_HTMLDraw.DrawText(&dc, Out, f, Calc, 0, &Ed, &m_imgs,&params);
			m_Readable = Out;

			Pos.bottom = Pos.top + Calc.Height();
			Calc = Pos;
			dc.FillSolidRect(Pos,ActionBG);

			// We may be hovering over something.
			CPoint mouse;
			GetCursorPos(&mouse);
			Ed.ScreenToClient(&mouse);

			if(Pos.PtInRect(mouse) && m_select(&Ed))
			{
				for(int p = 0; p < params.size(); p++)
					for(int d = 0; d < params[p]->displayrects.size(); d++)
						if(params[p]->displayrects[d].PtInRect(mouse))
						{
							for(int a = 0; a < params[p]->displayrects.size(); a++)
								dc.FillSolidRect(params[p]->displayrects[a], RGB(100,255,100));
							continue;
						}
			}

			// End hover
			Out = m_Text;
			g_HTMLDraw.DrawText(&dc, Out, f, Calc, 0, &Ed, &m_imgs,&params);
			m_Readable = Out;

			if(m_select(&Ed))
				dc.Draw3dRect(Calc,ACTION_BG_SEL2,ACTION_BG_SEL2);
		}
		break;

		case 3:
		{
			CHTMLFont f;
			f.SetBold(false);
			f.SetSize(8);
			f.SetName("Arial");
			f.SetImage(false);

			if (!bEnabled)
				f.SetStrike(true);

			CRect Calc = Pos;
			Calc.bottom  += m_rect(&Ed).Height();

			CString newtext;			
			CObjType* objectType =	((EventSheetEditor*)&Ed)->application->FindObjTypeFromNumber(oid);

			int icon = 0;

			if(m_Family)
				((EventSheetEditor*)(&Ed))->m_familyhbitmaps.Lookup(oid, icon);
			else
				((EventSheetEditor*)(&Ed))->m_objecthbitmaps.Lookup(oid, icon);

			bool show_name = true;
			bool show_icon = true;
			bool show_quotes = true;

			if (m_Text.Find("#noname") != -1) show_name = false;
			if (m_Text.Find("#noicon") != -1) show_icon = false;
			if (m_Text.Find("#noquotes") != -1) show_quotes = false;

			CString display_text = m_Text;

			display_text.Replace("#noname", "");
			display_text.Replace("#noicon", "");
			display_text.Replace("#noquotes", "");

			if (!show_quotes) display_text.Replace("\"", "");

			if (show_name && show_icon)
				newtext.Format("<img>%d</img> %s: %s",  icon, objectType->GetName(), display_text);
			else if (show_name && !show_icon)
				newtext.Format("%s: %s", objectType->GetName(), display_text);
			else if (!show_name && show_icon)
				newtext.Format("<img>%d</img> %s", icon, display_text);
			else
				newtext.Format("%s", display_text);

			CString Out = newtext;
			g_HTMLDraw.DrawText(&dc, Out, f, Calc, 0, &Ed, &m_imgs,&params);

			dc.FillSolidRect(Calc,ActionBG);

			//We may be hovering over something.
			CPoint mouse;
			GetCursorPos(&mouse);
			Ed.ScreenToClient(&mouse);

			if(Calc.PtInRect(mouse) && m_select(&Ed))
			{
				for(int p = 0; p < params.size(); p++)
					for(int d = 0; d < params[p]->displayrects.size(); d++)
						if(params[p]->displayrects[d].PtInRect(mouse))
						{
							for(int a = 0; a < params[p]->displayrects.size(); a++)
								dc.FillSolidRect(params[p]->displayrects[a], RGB(100,255,100));
							continue;
						}
			}


			// End hover
			Calc = Pos;
			Calc.bottom  += m_rect(&Ed).Height();

			// Get readable text
			g_HTMLDraw.DrawText(&dc, newtext, f, Calc, 0, &Ed, &m_imgs,&params);
			m_Readable = newtext.Right(newtext.GetLength() - newtext.ReverseFind(':'));

			Pos.bottom = Pos.top + Calc.Height();
		
			if(m_select(&Ed))
				dc.Draw3dRect(Calc,ACTION_BG_SEL2,ACTION_BG_SEL2);

			m_Anim.Space = Calc.Height();
		}

		break;
	}

	m_visible = true;

	m_rect(&Ed) = Pos;
	if(renderType == 1)
	{
		Pos.InflateRect(3,3);
		m_rect(&Ed) = Pos;
		Pos.DeflateRect(3,3);
	}

	

}

bool CEditorAction::Serialize(CArchive &ar, CChronoEventEditor* Ed )
{
	CString ExpectedName = "CEditorAction";
	int     Version      = 2;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	if(ar.IsLoading())
	{
		m_visible = true;
		m_Valid = true;
		ar >> m_Family >> m_FamilyName;	
		ar >> actID;
		ar >> oid;

		if(Ed) Ed->RegisterObjectID(oid, ar);
		ar >> m_rect_default;
		ar >> m_select_default;
		ar >> m_Anim.Space;
		m_Anim.m_bDestroyed = false;
		ar //>> m_Text - note: must call EventSheetEditor::ReformatAction 
			>> m_Object >> m_Script;
		ar >> mid;

		m_Text = "";

		int size = 0;
		ar >> size;
		for (int i = 0; i < size; i++)
		{
			CEditorParam* Param = new CEditorParam;
			Param->Serialize(ar, Ed);
			params.push_back(Param);
		}

		CString name = "";
		ar >> name;
		OINFO* o = GetOINFO(name);

		if(o)
			DLLIndex = o->oID;
		else
			DLLIndex = -1;

		if(Version >= 2)
			ar >> m_bEnabled;
		else
			m_bEnabled = true;
		if(Ed)
			((EventSheetEditor*)Ed)->ReformatAction(this);
	}

	else
	{
		ar << m_Family << m_FamilyName;
		ar << actID;
		ar << oid;
		if(Ed) Ed->RegisterObjectID(oid, ar);
		ar << m_rect(Ed);
		ar << m_select(Ed);
		ar << m_Anim.Space;
		ar //<< //m_Text
			<< m_Object << m_Script;	
		ar << mid;

		int size = params.size();
		ar << size;
		for (int i = 0; i < params.size(); i++)
			params[i]->Serialize(ar, Ed);

		OINFO* o = GetOINFO(DLLIndex);
		CString CStr = o->extName;

		ar << CStr;

		ar << m_bEnabled;
	}

	return true;
}

void CEditorAction::Remove(bool Animate, CChronoEventEditor* pEd)
{
	if(Animate)
		m_Anim.Start(pEd->m_Ellapsed);
	else
	{
		m_Anim.Space = 0;
		m_Anim.StartSpace=0;
		m_Anim.m_bDestroyed = true;
		m_select(pEd) = false;
	}
}

BOOL& CEditorAction::m_select(CChronoEventEditor* Ed){		
	return GetMapItemFunction_bool(& m_ED_select, m_select_default, Ed);   
}
CRect& CEditorAction::m_rect(CChronoEventEditor* Ed){		
	return GetMapItemFunction_CRect(& m_ED_rect, m_rect_default, Ed);   
}