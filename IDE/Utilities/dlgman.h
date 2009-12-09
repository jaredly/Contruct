#ifndef _CDLGMAN_
#define _CDLGMAN_

/*
 *  CDlgMan class
 *
 *  uemakeXp development team
 *  http://www.uemake.com
 *
 *  you may freely use or modify this code
 *  ---------------------------------------------------------
 *  CDlgMan is simple class that save and restore position of
 *  all child windows inside of dialog.
 */

class CDlgMan {
public:
  // constructor
  CDlgMan() {
    m_hWnd = NULL;
    m_szPath = NULL;
    m_bUseNegId = FALSE;
  }
  ~CDlgMan() {
    if (m_szPath) {
      delete m_szPath;
      m_szPath = NULL;
    }
  }

  // set bUseNegId to TRUE, if you want to load/store controls with
  // undefined (-1) ID ("STATIC" windows). in this case is used
  // window text (instead of window ID) as identifier. the best solution
  // is to set for static controls some IDC_ (other than IDC_STATIC==-1)
  // and keep bUseNegId FALSE. this will save space in registry.

  BOOL Load(HWND hWnd, LPCSTR lpRegPath, BOOL bUseNegId=FALSE) {
    RECT r;
    if (!IsWindow(hWnd)) return FALSE;
    if (!lpRegPath) return FALSE;
    if (m_szPath) delete m_szPath;
    // save window handle
    m_hWnd = hWnd;
    // save registry path
    m_szPath = new char [strlen(lpRegPath)+1];
    m_bUseNegId = bUseNegId;
    if (m_szPath) { strcpy(m_szPath, lpRegPath); }
    if (GetKey(HKEY_CURRENT_USER, lpRegPath, NULL, &r, sizeof(r))) {  // restore...
      if (!SetRect(m_hWnd, &r, TRUE)) return FALSE;
      HWND hChildWnd = GetWindow(m_hWnd, GW_CHILD);
      while (hChildWnd) {
        UINT uId = GetWindowLong(hChildWnd, GWL_ID);
        if (uId && uId!=-1L) { // control has ID, so we can use it
          if (GetKey(HKEY_CURRENT_USER, lpRegPath, i2s(uId), &r, sizeof(r))) {
            if (!SetRect(hChildWnd, &r, TRUE)) return FALSE;
          }
        }
        else if (m_bUseNegId) {  // static controls -> we use text as identifier
          char szName[48];
          if (GetWindowText(hChildWnd, szName, sizeof(szName))) {
            if (GetKey(HKEY_CURRENT_USER, m_szPath, szName, &r, sizeof(r))) {
              if (!SetRect(hChildWnd, &r, TRUE)) return FALSE;
            }
          }
        }
        hChildWnd = GetWindow(hChildWnd, GW_HWNDNEXT);
      }
	  return TRUE;
    }
    return FALSE;
  }
  BOOL Save() {
    if (!m_hWnd || !IsWindow(m_hWnd) || !m_szPath)
      return FALSE;
    RECT r;
    if (GetWindowRect(m_hWnd, &r)) {
      if (!SetKey(HKEY_CURRENT_USER, m_szPath, NULL, &r, sizeof(r))) return FALSE;
      HWND hChildWnd = GetWindow(m_hWnd, GW_CHILD);
      while (hChildWnd) {
        UINT uId = GetWindowLong(hChildWnd, GWL_ID);
        if (uId && uId!=-1L) { // control has ID, so we can use it
          if (GetRect(hChildWnd, &r)) {
            if (!SetKey(HKEY_CURRENT_USER, m_szPath, i2s(uId), &r, sizeof(r))) return FALSE;
          }
        }
        else if (m_bUseNegId) {  // static controls -> we use text as identifier
          char szName[48];
          if (GetWindowText(hChildWnd, szName, sizeof(szName))) {
            if (GetRect(hChildWnd, &r)) {
              if (!SetKey(HKEY_CURRENT_USER, m_szPath, szName, &r, sizeof(r))) return FALSE;
            }
          }
        }
        hChildWnd = GetWindow(hChildWnd, GW_HWNDNEXT);
      }
      return TRUE;
    }
    return FALSE;
  }
  // remove settings from registry. use this function if you need to get
  // dialog window to default position/size.
  BOOL Default() {
    if (!m_szPath) return FALSE;
    return (RegDeleteKey(
      HKEY_CURRENT_USER, m_szPath)==ERROR_SUCCESS) ? TRUE : FALSE;
  }
protected:
  // helpers
  BOOL GetRect(HWND hWnd, LPRECT lpRect) {
    if (GetWindowRect(hWnd, lpRect)) {
      if (ScreenToClient(m_hWnd, &((LPPOINT)lpRect)[0]) &&
          ScreenToClient(m_hWnd, &((LPPOINT)lpRect)[1]))
      return TRUE;
    }
    return FALSE;
  }
  __inline BOOL SetRect(HWND hWnd, LPRECT lpRect, BOOL bRedraw) {
    return MoveWindow(hWnd, lpRect->left, lpRect->top, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, bRedraw);
  }
  BOOL GetKey(HKEY hKeyRoot, LPCSTR lpRegPath, LPCSTR lpKey, LPVOID lpData, DWORD dwSize) {
    DWORD dwType = REG_BINARY;
    HKEY  hKey;
    if (RegOpenKeyEx(hKeyRoot, lpRegPath, 0, KEY_READ, &hKey)!=ERROR_SUCCESS) return FALSE;
    if (RegQueryValueEx(hKey, lpKey, NULL, (LPDWORD) &dwType,
      (LPBYTE) lpData, (LPDWORD) &dwSize)!=ERROR_SUCCESS) {
      RegCloseKey(hKey);
      return FALSE;
    }
    RegCloseKey(hKey);
    return TRUE;
  }
  BOOL SetKey(HKEY hKeyRoot, LPCSTR lpRegPath, LPCSTR lpKey, LPVOID lpData, DWORD dwSize) {
    DWORD dwType = REG_BINARY;
    DWORD dwDisposition;
    HKEY  hKey;
    if (RegOpenKeyEx(hKeyRoot, lpRegPath, 0, KEY_WRITE, &hKey)!=ERROR_SUCCESS) {
      if (RegCreateKeyEx(hKeyRoot, lpRegPath, 0, NULL, REG_OPTION_NON_VOLATILE,
        KEY_WRITE, NULL, &hKey, &dwDisposition)!=ERROR_SUCCESS) {
        return FALSE;
      }
    }
    if (RegSetValueEx(hKey, lpKey, 0, dwType, (const BYTE*) lpData, dwSize)!=ERROR_SUCCESS) {
      RegCloseKey(hKey);
      return FALSE;
    }
    RegCloseKey(hKey);
    return TRUE;
  }
  LPCSTR i2s(UINT nNumber) {
    static char szn[34];
    return _itoa(nNumber, szn, 16);
  }
  // variables
  HWND  m_hWnd;
  LPSTR m_szPath;
  BOOL  m_bUseNegId;
};
#endif