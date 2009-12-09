//ddmgr.h - Header file for drag-drop manager class

#ifndef DDMGR
#define DDMGR


//#include "globals.h"
    
#define DO_CLIPBOARD TRUE
#define DO_DRAGDROP  FALSE
#define DD_MAXCHARS  1000  //Maximum size of a CF_TEXT block

//Only one of these per app
static BOOL g_bOleInitCalled = TRUE;

/////////////////////////////////////////
class CDragDropMgr
{
public:

    CDragDropMgr(BOOL bInitOle = TRUE);

    BOOL AddFormat(CString csFormat);

    BOOL OkToPaste();
    
    CString AvailableDataType(COleDataObject* pDataObject);

    BOOL PrepareDrop(BOOL        bToClipboard,
                     LPCTSTR     lpstrFormat,
                     CObject*    pObj,
                     DROPEFFECT* pDropEffect,
					 LPCTSTR     lpstrFormat2 = NULL,
					 CObject*    pObj2 = NULL);

    BOOL DoDrop(CObject*        pT,
                COleDataObject* pDataObject,
                LPCTSTR         lpstrFormat);

    BOOL GetCFText(CStringArray*   pcsText,
                   COleDataObject* pDataObject);

    DROPEFFECT OnDragOver(COleDataObject* pDataObject,
                          DWORD           dwKeyState,
                          CPoint          point);

    BOOL OnDragEnter(COleDataObject* pDataObject);

    void MakeDropTarget(CWnd* pWnd);//View* pView);

protected:

    BOOL m_bOkToDrop;

    COleDropTarget m_DropTarget;

    CStringArray      m_csFormats; //Array of registered formats
    CArray<UINT,UINT> m_nFormats;  //and their integer values

    BOOL InitOle();

};
#endif //DDMGR