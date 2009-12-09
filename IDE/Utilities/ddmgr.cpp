// DDMgr.cpp : Drag/drop manager
//

/////////////////////////////////////////////////////////////////////////////
// Export
//
// Last revision: 1.1
// Chris Newhouse, 11th October 2006
// 22nd April 2006
//
// Changes:
// - Try/catch round some crash prone items
// - Allowing PrepareDrop to add two clipboard formats if necessary instead of one
//
/////////////////////////////////////////////////////////////////////////////

// File includes
#include "stdafx.h"
#include <afxole.h>
#include <afxodlgs.h>
#include <afxdisp.h>
#include <afxpriv.h>
#include "ddmgr.h"

//////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////
CDragDropMgr::CDragDropMgr(BOOL bInitOle)
{
    m_bOkToDrop = FALSE;

    if (bInitOle)
      InitOle();
}

//////////////////////////////////////////////////
// Adds an entry to the list of registered
// clipboatd formats for the manager,
// and gets the integer value associated
// with the string format name.
//////////////////////////////////////////////////
BOOL CDragDropMgr::AddFormat(CString csFormat)
{
    if (csFormat.IsEmpty())
        return FALSE;

    UINT nFormat = CF_TEXT;

    if (csFormat != "CF_TEXT") //Special case - do not register
        nFormat = ::RegisterClipboardFormat((LPCTSTR)csFormat);

    if (nFormat == 0) 
        return FALSE;

    TRY
    {
        m_csFormats.Add(csFormat);
        m_nFormats. Add(nFormat);
    }

    CATCH(CMemoryException,cm)
    {
        return FALSE;
    }
    END_CATCH
   
    return TRUE;
}

//////////////////////////////////////////////////
// Static function to initialize OLE.
// Requires global.h.
//
// You should call this from the OnCreate()
// method of your view class, before
// registering the view class as a drop target.
//
// This function uses a static variable to make
// sure that AfxOleInit() is only called once.
//////////////////////////////////////////////////
BOOL CDragDropMgr::InitOle()
{
    if (!g_bOleInitCalled)
        g_bOleInitCalled = AfxOleInit();

    return g_bOleInitCalled;
}

///////////////////////////////////////////////////
// Verify that data you can handle is on the clipboard.
///////////////////////////////////////////////////
BOOL CDragDropMgr::OkToPaste()
{
    COleDataObject obj;

    BOOL bOk = FALSE;

    if (!obj.AttachClipboard())
        return FALSE;
    
    for (int i=0;i < m_nFormats.GetSize() && !bOk;i++)
    {
        UINT nF = m_nFormats.GetAt(i);
        bOk = obj.IsDataAvailable(m_nFormats.GetAt(i));
    }

    return bOk;
}

///////////////////////////////////////////////////
// Verify that data you can handle is on the clipboard
// or in the drop buffer data object.
//
// If checking the clipboard, set pDataObject to NULL.
///////////////////////////////////////////////////
CString CDragDropMgr::AvailableDataType(COleDataObject* pDataObject)
{
    COleDataObject obj;

    //Assume caller is checking the clipboard
    //because no OLE data object was passed in
    if (pDataObject == NULL)
    {
        pDataObject = &obj;

        if (!obj.AttachClipboard())
            return "";
    }
    
    CString csFormat = "";
    
    for (int i=0;i < m_nFormats.GetSize() && csFormat.IsEmpty();i++)
    {
        if (pDataObject->IsDataAvailable(m_nFormats.GetAt(i)))
            csFormat = m_csFormats.GetAt(i);
    }

    return csFormat;
}

///////////////////////////////////////////////////
//Copy this object out to the clipboard or drag/drop
//buffer based on Keith Rule's
//serialization algorithm
//from the MFC Programmer's Sourcebook website,
//and Chapter 1 of "the Essence of OLE with Active X"
//by David S. Platt.
//
//pDropEffect is only set if doing a drag-drop.
//
//You usually call this function from:
//
//1) Your view class's OnLButtonDown() method
//   when starting a drag-drop.
//
//2) Your document class when copying data
//   to the clipboard.
///////////////////////////////////////////////////
BOOL CDragDropMgr::PrepareDrop(
                   BOOL        bToClipboard,
                   LPCTSTR     lpstrFormat,
                   CObject*    pObj,
                   DROPEFFECT* pDropEffect,
                   LPCTSTR     lpstrFormat2,
                   CObject*    pObj2)
{
    if (pObj == NULL)
        return FALSE;

    CSharedFile sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
    CSharedFile sf2(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

    UINT format = ::RegisterClipboardFormat(lpstrFormat);

    if (format == 0)
        return FALSE;

    TRY
    {
        CArchive ar(&sf, CArchive::store);

        pObj->Serialize(ar);

        ar.Close();

        HGLOBAL hMem = sf.Detach();

        if (hMem == NULL)
            return FALSE;

        COleDataSource* pSrc = new COleDataSource();

        if (pSrc == NULL)
            return FALSE;
    
        pSrc->CacheGlobalData(format,hMem);

		if (pObj2) { // special case for when the second data type is a filelist
			UINT format2 = (strcmp(lpstrFormat2, "CF_HDROP") ? ::RegisterClipboardFormat(lpstrFormat2) : CF_HDROP);
			if (format2 != CF_HDROP) {
				CArchive ar2(&sf2, CArchive::store);

				pObj2->Serialize(ar2);
		
				ar2.Close();

				HGLOBAL hMem2 = sf2.Detach();

				if (hMem2 == NULL)
					return FALSE;

				pSrc->CacheGlobalData(format2,hMem2);
			}
			else { // fake file list
				DROPFILES *pDrop = (DROPFILES*)pObj2;
				HGLOBAL hgDrop = GlobalAlloc ( GHND | GMEM_SHARE, sizeof(DROPFILES)+1 );
				DROPFILES *pDrop2 = (DROPFILES*) GlobalLock ( hgDrop );
				memcpy((void*)pDrop2, (void*)pDrop, sizeof(DROPFILES)+1);
				GlobalUnlock(hgDrop);
				pSrc->CacheGlobalData(format2, hgDrop);
			}
		}
    
        //Pasting to the clipboard:
        //do not free the data source
        //(OLE will free it when no longer needed)
        if (bToClipboard)
            pSrc->SetClipboard();
    
        //Starting a drag-drop:
        //Set the type of drag-drop effect, and
        //free the data source object.
        //OLE has created a data source object in
        //the drag-drop global cache; it's
        //not our problem anymore...
        else if (pDropEffect != NULL)
        {
            *pDropEffect = pSrc->DoDragDrop();

            delete pSrc;
        }

        return TRUE;
    }   //catch

    CATCH_ALL(ce)
    {
        return FALSE;
    }
    END_CATCH_ALL
}

////////////////////////////////////////////////////////
//This function loads data from the clipboard
//or drag/drop into a local object of the proper
//type for use.
//
//If pasting from clipboard, pDataObject should be NULL.
//
//You usually call this function from:
//
//1) Your view class's OnDrop() method when a drag-
//   drop operation ends, to load the data for use.
//
//2) Your view (or document) class's OnEditPaste()
//   method when doing a paste from the clipboard.
////////////////////////////////////////////////////////
BOOL CDragDropMgr::DoDrop(
                   CObject*        pO,
                   COleDataObject* pDataObject,
                   LPCTSTR         lpstrFormat)
{
//	try
	{
		UINT format = ::RegisterClipboardFormat(lpstrFormat);

		COleDataObject* pObj = NULL;

		// If from clipboard, then there is no
		// data object passed in, so use a local copy
		if (pDataObject == NULL)
			pObj = new COleDataObject();

 		else
			pObj = pDataObject;

		// Make sure basic data is OK
    	if (pObj == NULL)
    	    return FALSE;

		else if (pDataObject == NULL &&
 			!pObj->AttachClipboard())
			return FALSE;

		else if (format == 0 || pO == NULL)
			return FALSE;

		else if (!pObj->IsDataAvailable(format))
			return FALSE;

		FORMATETC fe;

		fe.cfFormat = format;
		fe.tymed    = TYMED_HGLOBAL;
		fe.lindex   = -1;
		fe.dwAspect = DVASPECT_CONTENT;
		fe.ptd      = NULL;

		// Fetch the data from the OLE data object
		HGLOBAL hMem = pObj->GetGlobalData(format,&fe);

		if (hMem == NULL)
			return FALSE;

	//	TRY
		{
			CMemFile mf((BYTE*)::GlobalLock(hMem), ::GlobalSize(hMem));

			CArchive ar(&mf, CArchive::load);
			pO->Serialize(ar); 
			 ar.Close();

			::GlobalUnlock(hMem);

			pObj->Release();

			if (pDataObject == NULL)
				delete pObj;

			return TRUE;
		}

	/*	CATCH_ALL(ce)
		{
			pObj->Release();

			if (pDataObject == NULL)
				delete pObj;

		return FALSE;
		}

		END_CATCH_ALL*/
	}

/*	catch(...)
	{
		CErrorDlg error;
		error.Error("Drag/drop error", "A serious error occured while dropping objects. Please report this to the Construct team.");
		return FALSE;
	}*/

	return TRUE;
}

////////////////////////////////////////////////////////
//This function loads CF_TEXT data from the clipboard
//or drag/drop into a CStringArray, one line of text
//per array entry. A null in the input text
//denotes a new line.
//
//If pasting from clipboard, pDataObject should be NULL.
//
//You usually call this function from:
//
//1) Your view class's OnDrop() method when a drag-
//   drop operation ends, to load the data for use.
//
//2) Your view (or document) class's OnEditPaste()
//   method when doing a paste from the clipboard.
////////////////////////////////////////////////////////
BOOL CDragDropMgr::GetCFText(
                   CStringArray*   pcsText,
                   COleDataObject* pDataObject)
{
    BOOL bOk = FALSE;

    CString csLine = "";

    COleDataObject* pObj = NULL;

    //If from clipboard, then there is no
    //data object passed in, so use a local copy
    if (pDataObject == NULL)
        pObj = new COleDataObject();

    else
        pObj = pDataObject;

    //Make sure basic data is OK
    if (pObj == NULL)
        return FALSE;

    else if ((pDataObject == NULL &&
              !pObj->AttachClipboard())       ||
              pcsText== NULL                  ||
              !pObj->IsDataAvailable(CF_TEXT))
    {
        pObj->Release();

        if (pDataObject == NULL)
            delete pObj;

        return FALSE;
    }

    //The format etc struct tells how the data
    //should be transferred.
    //Do NOT use the MFC defaults here - they are no good
    FORMATETC fe;

    fe.cfFormat = CF_TEXT;
    fe.tymed    = TYMED_HGLOBAL;
    fe.lindex   = -1;
    fe.dwAspect = DVASPECT_CONTENT;
    fe.ptd      = NULL;

    //Fetch the data from the OLE data object
    HGLOBAL hMem = pObj->GetGlobalData(CF_TEXT,&fe);

    if (hMem != NULL)
    {
        char* pchT = (char*)::GlobalLock(hMem);

        if (pchT != NULL)
        {
            DWORD dwSize = ::GlobalSize(hMem);

            int  i     = 0;
            BOOL bDone = FALSE;

            while (!bDone)
            {
               //Hit a crlf: Add current line
               if (pchT[i+1] == '\n' && pchT[i] == '\r')
               {
                  pcsText->Add(csLine);
                  csLine = "";
                  i += 2;
               }

               //End of the block: Add final line
               else if (pchT[i] == '\0' || i >= DD_MAXCHARS)
               {
                  pcsText->Add(csLine);
                  bDone = TRUE;;
               }

               //Normal character: Add to line being built
               else
               {
                  csLine += pchT[i];
                  i++;
               }
            }

            ::GlobalUnlock(hMem);

            bOk = TRUE;
        }
    }   //hmem not null

    pObj->Release();

    if (pDataObject == NULL)
        delete pObj;

    return bOk;
}

////////////////////////////////////////////////////////
//Call this function from your view
//class's OnDragOver method.
//
//It returns move or copy depending on whether the
//ctrl key is up or down.
////////////////////////////////////////////////////////
DROPEFFECT CDragDropMgr::OnDragOver(
                         COleDataObject* pDataObject,
                         DWORD           dwKeyState,
                         CPoint          point)
{
    if (!m_bOkToDrop)
        return DROPEFFECT_NONE;

    else if (!(dwKeyState & MK_CONTROL))
        return DROPEFFECT_MOVE;

    else
        return DROPEFFECT_COPY;
}

////////////////////////////////////////////////////////
//Call this function from your view
//class's OnDragEnter method, to set the
//manager to note if
//there is data that is OK to drop into your view.
////////////////////////////////////////////////////////
BOOL CDragDropMgr::OnDragEnter(COleDataObject* pDataObject)
{
    m_bOkToDrop = FALSE;

    if (pDataObject == NULL)
        return FALSE;

    for (int i=0;i < m_nFormats.GetSize() && !m_bOkToDrop;i++)
    {
        m_bOkToDrop =
        pDataObject->IsDataAvailable(m_nFormats.GetAt(i));
    }

    return m_bOkToDrop;
}

////////////////////////////////////////////////////////
//Call this function from your view
//class's OnCreate method, to set it up
//as a drop target, and make sure OLE is started.
////////////////////////////////////////////////////////
void CDragDropMgr::MakeDropTarget(CWnd* pWnd)//CView* pView)
{
    m_DropTarget.Register(pWnd);//pView);
}
