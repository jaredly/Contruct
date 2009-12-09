#ifndef _CDLGANCHOR_
#define _CDLGANCHOR_
/*
 *  CDlgAnchor class
 *
 *  uemakeXp development team
 *  http://www.uemake.com
 *
 *  you may freely use or modify this code
 *  -------------------------------------------------------------
 *  Controls can be anchored to one or more edges of their parent
 *  window.  Anchoring  a  control to its parent ensures that the
 *  anchored edges  remain  in  the same position relative to the
 *  edges  of  the  parent container when the parent container is
 *  resized. Controls  can  be  also  docked to one edge of their
 *  parent window or can be docked to all edges and fill it.
 */

#define ANCHOR_TOP          0x0001
#define ANCHOR_LEFT         0x0002
#define ANCHOR_RIGHT        0x0004
#define ANCHOR_BOTTOM       0x0008
#define ANCHOR_TOPLEFT      0x0003
#define ANCHOR_TOPRIGHT     0x0005
#define ANCHOR_BOTTOMLEFT   0x000a
#define ANCHOR_BOTTOMRIGHT  0x000c
#define ANCHOR_ALL          0x000f

#define DOCK_TOP            0x0100
#define DOCK_LEFT           0x0200
#define DOCK_RIGHT          0x0400
#define DOCK_BOTTOM         0x0800
#define DOCK_FILL           0x0f00

typedef struct _CDlgAncItem {
  HWND hWnd;    // hWnd of element
  UINT uFlag;   // anchor
  RECT pRect;   // window rect
  struct _CDlgAncItem *next;  // next item
  _CDlgAncItem() {
    this->hWnd  = NULL;
    this->next  = NULL;
    this->uFlag = 0;
    ZeroMemory(&pRect, sizeof(pRect));
  }
  _CDlgAncItem(HWND hWnd, UINT uFlag, LPRECT lpRect) {
    this->hWnd  = hWnd;
    this->next  = NULL;
    this->uFlag = uFlag;
    memcpy(&pRect, lpRect, sizeof(pRect));
  }
}
CDlgAncItem;

class CDlgAnchor {
public:
  // constructor
  CDlgAnchor() {
    m_hWnd = NULL;
    ZeroMemory(&m_pRect, sizeof(m_pRect));
  }
  // initialize
  BOOL Init(HWND hDlgWindow) {
    if (IsWindow(hDlgWindow)) {
      m_hWnd = hDlgWindow;
      if (GetClientRect(m_hWnd, &m_pRect)) {
        return TRUE;
      }
    }
    return FALSE;
  }
  BOOL Add(HWND hWnd, UINT uFlag) {
    // go to end of list
    CDlgAncItem *item = &m_pControls;
    while (item->next) {
      item = item->next;
    }
    // insert new item to the end
    RECT pRect;
    if (GetWindowRect(hWnd, &pRect)) {
      ScreenToClient(m_hWnd, &((LPPOINT)&pRect)[0]);
      ScreenToClient(m_hWnd, &((LPPOINT)&pRect)[1]);
      item->next = new CDlgAncItem(hWnd, uFlag, &pRect);
    }
    return item->next ? TRUE : FALSE;
  }
  BOOL Add(UINT uID, UINT uFlag) {
    return Add(GetDlgItem(m_hWnd, uID), uFlag);
  }
  BOOL Remove(UINT uID) {
    return Remove(GetDlgItem(m_hWnd, uID));
  }
  BOOL Remove(HWND hWnd) {
    CDlgAncItem *item = &m_pControls;
    while (item->next) {
      if (item->next->hWnd==hWnd) { // remove it
        CDlgAncItem *s = item->next->next;
        delete item->next;
        item->next = s;
        return TRUE;
      }
      item = item->next;
    }
    return FALSE;
  }
  BOOL Update(UINT uID) {
    return Update(GetDlgItem(m_hWnd, uID));
  }
  BOOL Update(HWND hWnd) {
    CDlgAncItem *item = m_pControls.next;
    while (item) {
      if (item->hWnd==hWnd) { // update position
        if (GetWindowRect(hWnd, &item->pRect)) {
          ScreenToClient(m_hWnd, &((LPPOINT)&item->pRect)[0]);
          ScreenToClient(m_hWnd, &((LPPOINT)&item->pRect)[1]);
          return TRUE;
        }
      }
      item = item->next;
    }
    return FALSE;
  }
  void UpdateAll() {
    CDlgAncItem *item = m_pControls.next;
    while (item) {
      if (GetWindowRect(item->hWnd, &item->pRect)) {
        ScreenToClient(m_hWnd, &((LPPOINT)&item->pRect)[0]);
        ScreenToClient(m_hWnd, &((LPPOINT)&item->pRect)[1]);
      }
      item = item->next;
    }
  }
  void RemoveAll() {
    RemoveIt(m_pControls.next);
  }
  ~CDlgAnchor() {
    RemoveAll();
  }
  BOOL OnSize(BOOL bRepaint=TRUE) {
    RECT pRect;
    if (GetClientRect(m_hWnd, &pRect)) {
      CDlgAncItem *item = m_pControls.next;
      while (item) {
        // horizontal
        if ((item->uFlag & ANCHOR_LEFT) && (item->uFlag & ANCHOR_RIGHT)) {
          item->pRect.right += pRect.right - m_pRect.right;
        }
        else if (item->uFlag & ANCHOR_LEFT) {
          // left is default
        }
        else if (item->uFlag & ANCHOR_RIGHT) {
          item->pRect.right += pRect.right - m_pRect.right;
          item->pRect.left += pRect.right - m_pRect.right;
        }
        else {
          // relative move
          LONG sx = ((pRect.right-pRect.left)/2)-((m_pRect.right-m_pRect.left)/2);
          item->pRect.right += sx;
          item->pRect.left += sx;
        }
        // vertical
        if ((item->uFlag & ANCHOR_TOP) && (item->uFlag & ANCHOR_BOTTOM)) {
          item->pRect.bottom += pRect.bottom - m_pRect.bottom;
        }
        else if (item->uFlag & ANCHOR_TOP) {
          // top is default
        }
        else if (item->uFlag & ANCHOR_BOTTOM) {
          item->pRect.bottom += pRect.bottom - m_pRect.bottom;
          item->pRect.top += pRect.bottom - m_pRect.bottom;
        }
        else {
          // relative move
          LONG sy = ((pRect.bottom-pRect.top)/2)-((m_pRect.bottom-m_pRect.top)/2);
          item->pRect.bottom += sy;
          item->pRect.top += sy;
        }
        if (item->uFlag & DOCK_TOP) item->pRect.top = pRect.top;
        if (item->uFlag & DOCK_LEFT) item->pRect.left = pRect.left;
        if (item->uFlag & DOCK_RIGHT) item->pRect.right = pRect.right;
        if (item->uFlag & DOCK_BOTTOM) item->pRect.bottom = pRect.bottom;
        MoveWindow(item->hWnd, item->pRect.left, item->pRect.top, item->pRect.right-item->pRect.left, item->pRect.bottom-item->pRect.top, bRepaint);
        item = item->next;
      }
      memcpy(&m_pRect, &pRect, sizeof(pRect));
      return TRUE;
    }
    return FALSE;
  }
protected:
  void RemoveIt(CDlgAncItem* item) {
    if (item) {
      RemoveIt(item->next);
      delete item;
    }
  }
protected:
  HWND m_hWnd;
  RECT m_pRect;
  CDlgAncItem m_pControls;
};
#endif