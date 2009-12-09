#include "stdafx.h"
#include "ChronoEventEditor\ChronoEventEditor.h"
#include "Event Sheet Editor\Event Sheet Editor.h"

void CEditorCondition::Init()
{
	cndID = 0;
	oid = -1;
	mid = -1;
	m_Anim.m_bDestroyed = false;
	m_select_default  = false;
	m_rect_default = CRect(0,0,16,16);
	m_BehaviorDLLIndex = -1;
	m_select_default = false;
	m_Negate = false;
	m_bEnabled = true;
}

/*
void  CEditorCondition::Release()
{
	delete this;
}
*/

CEditorCondition::~CEditorCondition()
{
	for(int p = 0; p < params.size(); p++)
		delete params[p];
}
bool CEditorCondition::IsLoop(CApplication* app)
{
	if(m_Text != "")
	{
		return (m_Text.Find("<img>3</img>") != -1);
	}
	else
	{
		if(!app)
			return false;
		CObjType* objectType = app->FindObjTypeFromNumber(oid);
		if(!objectType)
			return false;
		ACESEntry2* acesEntry;

		if(mid == -1) // ace or custom ace
			acesEntry = objectType->GetACESEntry(CONDITION, cndID);
		else				 // movement			
		{
			//MovementIterator mi = objectType->m_Movements.begin();
			//advance(mi, mid);
			CBehavior* pMov = objectType->GetBehaviorByUniqueID(mid);
			if (pMov == NULL) throw;
			acesEntry = GetOINFO(pMov->BehaviorDLLIndex)->GetACEEntry(CONDITION, cndID);
		}

		// effects never have triggers

		if(!acesEntry)
			return false;
		if (acesEntry->aceDisplayText.Find("<img>3</img>") != -1)
			return true;
		return false;
	}



}


bool CEditorCondition::IsTrigger(CApplication* app)
{
	if(m_Text != "")
	{
		return (m_Text.Find("<img>1</img>") != -1);
	}
	else
	{
		if(!app)
			return false;
		CObjType* objectType = app->FindObjTypeFromNumber(oid);
		if(!objectType)
			return false;
		ACESEntry2* acesEntry;

		if(mid == -1) // ace or custom ace
			acesEntry = objectType->GetACESEntry(CONDITION, cndID);
		else				 // movement			
		{
			//MovementIterator mi = objectType->m_Movements.begin();
			//advance(mi, mid);
			CBehavior* pMov = objectType->GetBehaviorByUniqueID(mid);
			if (pMov == NULL) throw;
			acesEntry = GetOINFO(pMov->BehaviorDLLIndex)->GetACEEntry(CONDITION, cndID);
		}

		// effects never have triggers

		if(!acesEntry)
			return false;
		if (acesEntry->retrn & 0x0004 || acesEntry->retrn & 0x0008)
			return true;
		return false;
	}

}

bool CEditorCondition::Valid(CApplication* pApp)
{
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

bool CEditorCondition::ValidToDisplay(CApplication* pApp)
{
	if(pApp)
	{
		return m_Valid = Valid(pApp); 
	}
	else
		return m_Valid;
}

void CEditorCondition::Render(CRect &Pos, CDC &dc, bool EventSelected, CChronoEventEditor& Ed, bool bBookmarked, bool bEnabled)
{
	COLORREF BG;
	BG = m_select(&Ed)? CONDITION_BG_SEL : (EventSelected? CONDITION_BG_EVSEL : CONDITION_BG);
	if(IsTrigger(NULL))
	{
		BG = m_select(&Ed)? CONDITION_BG_SEL : (EventSelected? CONDITION_BG_EVSEL : EVENT_BG);	
	}

	// Animating
	if(m_Anim.m_bDestroyed)
	{
		CRect Calc = Pos;
		Pos.OffsetRect(0,m_Anim.Space);
		Calc.bottom = Pos.bottom;
		dc.FillSolidRect(Calc,BG);
		return;
	}

	CRect Box = Pos;

	Box.bottom+= m_rect(&Ed).Height();

	CRect Calc = Pos;
	Calc.bottom = Box.bottom;

	CHTMLFont f;
	f.SetBold(false);
	f.SetSize(8);
	f.SetName("Arial");
	f.SetImage(false);

	// Is it enabled?
	if (!bEnabled)
		f.SetStrike(true);

	CString Out;
	Out.Format("%s", m_Text);
	g_HTMLDraw.DrawText(&dc, Out, f, Calc, 0, &Ed, &m_imgs,&params);
	m_Readable.Format("%s", Out);

	dc.FillSolidRect(Calc,BG);
	{
		CRect Calc2 = Calc;
		CRect Calc = Pos;
		Calc.bottom = Box.bottom;

		//We may be hovering over something.
		CPoint mouse;
		GetCursorPos(&mouse);
		Ed.ScreenToClient(&mouse);

		if(Calc2.PtInRect(mouse) && m_select(&Ed))
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
		CString Out;

		bool show_quotes = true;
		if (m_Text.Find("#noquotes") != -1) show_quotes = false;

		CString display_text = m_Text;
		display_text.Replace("#noquotes", "");
		if (!show_quotes) display_text.Replace("\"", "");

		Out.Format("%s", display_text);
		g_HTMLDraw.DrawText(&dc, Out, f, Calc, 0, &Ed, &m_imgs,&params);
		m_Readable.Format("%s", Out);

		if(m_select(&Ed))
			dc.Draw3dRect(Calc2,CONDITION_BG_SEL2,CONDITION_BG_SEL2);
	}


	m_rect(&Ed) = Calc;
	
	Pos.OffsetRect(CPoint(0,Calc.Height()));
	m_Anim.Space = Calc.Height();

	if(IsLoop(NULL))
	{
		Pos.left += 10;
	}
}

bool CEditorCondition::Serialize(CArchive &ar, CChronoEventEditor* Ed)
{
	CString ExpectedName = "CEditorCondition";
	int     Version      = 2;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	if( ar.IsLoading())
	{
		m_visible = true;
		m_Valid = true;
		ar >> mid;
		ar >> cndID;
		ar >> oid;
		if(Ed)	Ed->RegisterObjectID(oid, ar);
		ar >> m_Negate;
		ar >> m_Family >> m_FamilyName;

		ar >> m_rect_default;
		ar >> m_select_default;
		ar >> m_select_default;
		ar >> m_Anim.Space;
		ar // //m_Text - note: must call EventSheetEditor::ReformatCondition 
			>> m_Object >> m_Script;
		m_Anim.m_bDestroyed = false;

		m_Text = "";

		int size = 0;
		ar >> size;
		for (int i = 0; i < size; i++)
		{
			CEditorParam* Param = new CEditorParam;
			Param->Serialize(ar, Ed);
 			params.push_back(Param);
		}

		if (oid != -1)
		{
			CString name = "";
			ar >> name;
			OINFO* o = GetOINFO(name);
			if (!o)
			{
				CErrorDlg Dlg;
				Dlg.Error("Error in condition.", "An invalid object in a condition was referenced.");

				return false;
			}

			DLLIndex = o->oID;
		}

		if(Version >= 2)
			ar >> m_bEnabled;
		else
			m_bEnabled = true;

		if(Ed)
			((EventSheetEditor*)Ed)->ReformatCondition(this);
	}
	else
	{
		ar << mid;
		ar << cndID;
		ar << oid;
		if(Ed)	Ed->RegisterObjectID(oid, ar);
		ar << m_Negate;
		ar << m_Family << m_FamilyName;

		ar << m_rect_default;
		ar << m_select_default;
		ar << m_select(Ed);
		ar << m_Anim.Space;
		ar //<< m_Text
			<< m_Object << m_Script;

		int size = params.size();
		ar << size;

		for (int i = 0; i < params.size(); i++)
			params[i]->Serialize(ar, Ed);
			
		if (oid != -1)
		{
			OINFO* o = GetOINFO(DLLIndex);
			CString CStr = o->extName;

			ar << CStr;
		}
		ar << m_bEnabled;
	}

	return true;
}

void CEditorCondition::Remove(bool Animate, CChronoEventEditor* pEd)
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

BOOL& CEditorCondition::m_select(CChronoEventEditor* Ed){
	return GetMapItemFunction_bool(& m_ED_select, m_select_default, Ed);   	
}
CRect& CEditorCondition::m_rect(CChronoEventEditor* Ed){	
	return GetMapItemFunction_CRect(& m_ED_rect, m_rect_default, Ed);   	
}