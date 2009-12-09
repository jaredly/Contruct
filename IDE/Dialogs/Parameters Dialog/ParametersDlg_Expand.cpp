/////////////////////////////////////////////////////////////////////////////
// Parameters dialog
/////////////////////////////////////////////////////////////////////////////

// Expand panel file

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\ParametersDlg.h"
extern CMainFrame* g_MainFrame;

// OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
// Handle right clicking an object and inserting an expression.
void CParametersDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION listpos = m_Expand.GetFirstSelectedItemPosition();
	int item = m_Expand.GetNextSelectedItem(listpos);

	//if(item == -1) return;

	CObjType* pObjType = (CObjType*)m_Expand.GetItemData(item);

	CEventWizardDlg EventWizard;
	EventWizard.m_aceType = EXPRESSION;
	EventWizard.layout = layout;
	EventWizard.application = application;

	if(pObjType){
		EventWizard.m_pSelectedObject = pObjType->ObjectIdentifier;
		EventWizard.m_iStep = 2;
	}
	else
		EventWizard.m_iStep = 1;
	
	EventWizard.SetMap(&application->object_types);

	if(EventWizard.DoModal() != IDOK)
		return;

	// The user can click Back and get a different object's expression, so make sure we're using the current one.
	pObjType = application->FindObjTypeFromNumber(EventWizard.m_pSelectedObject);

	int aceIndex = EventWizard.m_aceIndex;
	int mID = EventWizard.m_childid;

	CObjType tmp;
	CObjType *pInfoType = NULL;
	if(mID == -1)
		pInfoType = pObjType;

	CBehavior* pMov = NULL;
	CEffect* pEffect = NULL; // there are no effect expressions but perhaps in the future...
	if(mID < -1)
	{
		pInfoType = &tmp;
		pEffect = pObjType->GetEffect(-mID);
		tmp.CreateFromEffect(pEffect);
	}
	if(mID > -1)
	{
		pInfoType = &tmp;
		pMov = pObjType->GetBehaviorByUniqueID(mID);
		tmp.CreateFromBehavior(pMov);
	}

	if(!pInfoType)
		return;
	ACESEntry2* pACE = pInfoType->GetACESEntry(EXPRESSION, aceIndex);

	if (pWndFocusParam == NULL) 
		return;

	CString Func;
	if(pObjType->ObjectIdentifier == -1)
	{
		Func.Format("%s",  pACE->aceDisplayText);

		if (pACE->auxStr != "") 
		{
			Func += "(";
			Func += pACE->auxStr;
			Func += ")";
		}
	}
	else 
	{
		if(pMov)
		{
			Func.Format("%s[%s].%s", pObjType->GetName(), pMov->Text, pACE->aceDisplayText);

			if (pACE->params.size() != 0)
			{
				Func += "(";

				for (int i = 0; i < pACE->params.size(); i++)
				{
					Func += pACE->params[i].name;

					if (i != (pACE->params.size() - 1))
						Func += ", ";
				}

				Func += ")";
			}
		}

		else
		{
			// Starts with <:  interpret as shortcut for other expression eg. <MouseX>
			if (pACE->aceDisplayText.Left(1) == "<") {
				Func = pACE->aceDisplayText.Mid(1, pACE->aceDisplayText.GetLength() - 2);
			}
			else {
				Func = pObjType->GetName();

				// Allow expressions to specify object name like 'Counter'
				if (pACE->aceDisplayText != "") {
					// Allow expressions to specify an ACE name like "(x,y,z)" eg. Array(x,y,z)
					if (pACE->aceDisplayText.Left(1) != "(")
						Func += ".";

					Func += pACE->aceDisplayText;
				}

				if (pACE->params.size() != 0)
				{
					Func += "(";
					// okay loop params, add name and ,
					for (int i = 0; i < pACE->params.size(); i++)
					{
						Func += pACE->params[i].name;
						//only add if liek not last 0ne
						if (i != (pACE->params.size() - 1))
							Func += ", ";
					}
					Func += ")";
				}
			}
		}
	}

	pWndFocusParam->SendMessage(SCI_REPLACESEL, 0, (LPARAM)(const char*)Func);

	*pResult = 0;
}