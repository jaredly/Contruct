// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2008 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// mailto:support@prof-uis.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXT_CONTROLS_SHELL_H)
#define __EXT_CONTROLS_SHELL_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_CONTROLS_COMMON_H)
	#include <ExtControlsCommon.h>
#endif

#if (!defined __EXT_NC_FRAME_H)
	#include <ExtNcFrame.h>
#endif

#if (!defined __EXT_SPLITTER_WND_H)
	#include <ExtSplitterWnd.h>
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if ( ! defined __EXT_MFC_NO_SHELL )

#define __EXT_MFC_IDO_SHGIOI_SHARE					0x0FFFFFFF
#define __EXT_MFC_IDO_SHGIOI_LINK					0x0FFFFFFE
#define __EXT_MFC_IDO_SHGIOI_SLOWFILE				0x0FFFFFFFD

#define __EXT_MFC_SHGFI_ADDOVERLAYS					0x000000020
#define __EXT_MFC_SHGFI_OVERLAYINDEX				0x000000040

#define __EXT_MFC_DEF_SHELL_ITEM_SCA_HIDDEN			72
#define __EXT_MFC_DEF_SHELL_ITEM_SCA_READ_ONLY		144

// SHGNO enumeration emulation
#define __EXT_MFC_SHGDN_NORMAL						0        // default (display purpose)
#define __EXT_MFC_SHGDN_INFOLDER					1        // displayed under a folder (relative)
#define __EXT_MFC_SHGDN_INCLUDE_NONFILESYS			0x2000   // if not set, display names for shell name space items that are not in the file system will fail.
#define __EXT_MFC_SHGDN_FORADDRESSBAR				0x4000   // for displaying in the address (drives dropdown) bar
#define __EXT_MFC_SHGDN_FORPARSING					0x8000   // for ParseDisplayName or path

#define IDM_FIRST_SHELLMENUID						10000
#define IDM_LAST_SHELLMENUID						( IDM_FIRST_SHELLMENUID + 10000 )
#define IDM_SHELL_RENAME_COMMAND_ID					( IDM_FIRST_SHELLMENUID + 18 )

#define __EXT_MFC_CSIDL_CONNECTIONS					0x0031

#define __EXT_MFC_CMF_EXTENDEDVERBS					0x00000100

#define __EXT_MFC_SFGAO_DEFAULT_ATTRIBUTE_FILTER \
	(	SFGAO_CANCOPY           /* (capability) objects can be copied                  */ \
	|	SFGAO_CANMOVE           /* (capability) objects can be moved                   */ \
	|	SFGAO_CANLINK           /* (capability) objects can be linked                  */ \
	|	SFGAO_CANRENAME         /* (capability) objects can be renamed                 */ \
	|	SFGAO_CANDELETE         /* (capability) objects can be deleted                 */ \
	|	SFGAO_HASPROPSHEET      /* (capability) objects have property sheets           */ \
	|	SFGAO_DROPTARGET        /* (capability) objects are drop target                */ \
	|	SFGAO_LINK              /* (display) shortcut (link)                           */ \
	|	SFGAO_SHARE             /* (display) shared                                    */ \
	|	SFGAO_READONLY          /* (display) read-only                                 */ \
	|	SFGAO_GHOSTED           /* (display) ghosted icon                              */ \
	|	SFGAO_HIDDEN            /* (display) hidden object                             */ \
	|	SFGAO_FILESYSANCESTOR   /* (content) it contains file system folder            */ \
	|	SFGAO_FOLDER            /* (content) it's a folder.                            */ \
	|	SFGAO_FILESYSTEM        /* (content) is a file system thing (file,folder,root) */ \
	|	SFGAO_HASSUBFOLDER      /* (content) expandable in the map pane                */ \
	|	SFGAO_VALIDATE          /* invalidate cached information                       */ \
	|	SFGAO_REMOVABLE         /* is this removable media?                            */ \
	|	SFGAO_COMPRESSED        /* object is compressed (use alt color)                */ \
	|	SFGAO_BROWSABLE         /* is in-place browse-able                             */ \
	|	SFGAO_NONENUMERATED     /* is a non-enumerated object                          */ \
	|	SFGAO_NEWCONTENT        /* should show bold in explorer tree                   */ \
	)

#define __EXT_MFC_SHACF_DEFAULT						0x00000000
#define __EXT_MFC_SHACF_FILESYSTEM					0x00000001
#define __EXT_MFC_SHACF_URLALL						(__EXT_MFC_SHACF_URLHISTORY|__EXT_MFC_SHACF_URLMRU)
#define __EXT_MFC_SHACF_URLHISTORY					0x00000002
#define __EXT_MFC_SHACF_URLMRU						0x00000004
#define __EXT_MFC_SHACF_USETAB						0x00000008
#define __EXT_MFC_SHACF_FILESYS_ONLY				0x00000010

#define __EXT_MFC_SHACF_AUTOSUGGEST_FORCE_ON		0x10000000
#define __EXT_MFC_SHACF_AUTOSUGGEST_FORCE_OFF		0x20000000
#define __EXT_MFC_SHACF_AUTOAPPEND_FORCE_ON			0x40000000
#define __EXT_MFC_SHACF_AUTOAPPEND_FORCE_OFF		0x80000000


// if this preprocessor symbol is commented, then each CExtSWDC object will run its own internal thread
#define __EXT_MFC_SWDC_USES_THREAD_POOL

#if (defined __EXT_MFC_SWDC_USES_THREAD_POOL)

/////////////////////////////////////////////////////////////////////////////
// CExtSWDC_ThreadPool

class CExtSWDC;

#define __EXT_MFC_SWDC_TP_OBJECT_COUNT				64
#define __EXT_MFC_SWDC_TP_OBJECT_INTERNAL_COUNT		1
#define __EXT_MFC_SWDC_TP_OBJECT_WATCH_COUNT		( __EXT_MFC_SWDC_TP_OBJECT_COUNT - __EXT_MFC_SWDC_TP_OBJECT_INTERNAL_COUNT )

class __PROF_UIS_API CExtSWDC_ThreadPool
{
public:
	CExtSWDC_ThreadPool();
	virtual ~CExtSWDC_ThreadPool();

	DWORD m_dwThreadCreateFlags;
	UINT m_nThreadStackSize;
	LPSECURITY_ATTRIBUTES m_pThreadSecurityAttributes;

	class __PROF_UIS_API CExtSWDC_Thread : public CWinThread
	{
	protected:
		CTypedPtrList < CPtrList, CExtSWDC * > m_listSWDC;
		mutable CCriticalSection m_CS;
		CExtSWDC_ThreadPool & m_ThreadPool;
		CEvent m_eventRequest, m_eventResponse;
	public:
		CExtSWDC_Thread(
			CExtSWDC_ThreadPool & _ThreadPool
			);
		virtual ~CExtSWDC_Thread();

		virtual BOOL InitInstance();
		virtual int ExitInstance();

		INT GetBusyCount() const;
		bool CanAdd() const;
		bool Add( CExtSWDC * pSWDC );
		bool Add( CExtSWDC & _SWDC );
		bool Remove( CExtSWDC * pSWDC );
		bool Remove( CExtSWDC & _SWDC );
	public:
		bool IsPresent( const CExtSWDC * pSWDC ) const;
		bool IsPresent( const CExtSWDC & _SWDC ) const;
	}; /// class CExtSWDC_Thread

	class __PROF_UIS_API CExtSWDC_ThreadPool_Ptr
	{
	public:
		CExtSWDC_ThreadPool_Ptr();
		~CExtSWDC_ThreadPool_Ptr();
		CExtSWDC_ThreadPool * GetShellThreadPool();
		CExtSWDC_ThreadPool * operator -> ();
	private:
		CExtSWDC_ThreadPool * m_pThreadPool;
	}; /// class  CExtSWDC_ThreadPool_Ptr

protected:
	mutable CCriticalSection m_CS;
	CMap < CExtSWDC *, CExtSWDC *, CExtSWDC_Thread *, CExtSWDC_Thread * > m_mapWatcherToThread;
	CMap < CExtSWDC_Thread *, CExtSWDC_Thread *, INT, INT > m_mapThreadCounts;
	CTypedPtrList < CPtrList, CExtSWDC_Thread * > m_listThreadReceptors;
public:
	bool Start( CExtSWDC * pSWDC );
	bool Start( CExtSWDC & _SWDC );
	bool Stop( CExtSWDC * pSWDC );
	bool Stop( CExtSWDC & _SWDC );

	friend class CExtSWDC_Thread;
}; /// class CExtSWDC_ThreadPool

extern __PROF_UIS_API CExtSWDC_ThreadPool::CExtSWDC_ThreadPool_Ptr g_ShellThreadPool;

#endif // if (defined __EXT_MFC_SWDC_USES_THREAD_POOL)

/////////////////////////////////////////////////////////////////////////////
// CExtSWDC

class __PROF_UIS_API CExtSWDC
{
public:
	volatile bool	m_bSubFolder:1;
	volatile DWORD	m_dwNotifyFilter; // set of FILE_NOTIFY_CHANGE_*** flags
#if (!defined __EXT_MFC_SWDC_USES_THREAD_POOL)
	volatile HANDLE	m_hThread;
	CEvent			m_eventThreadStop, m_eventThreadComplete;
#endif // if (!defined __EXT_MFC_SWDC_USES_THREAD_POOL)
	CExtSWDC();
	virtual ~CExtSWDC();
#if (!defined __EXT_MFC_SWDC_USES_THREAD_POOL)
protected:
	static UINT stat_WatchDirChanges_ThreadProc( CExtSWDC * pSWDC );
public:
	virtual void WatchDirChanges_ThreadProc();
#endif // if (!defined __EXT_MFC_SWDC_USES_THREAD_POOL)
	virtual bool WatchDirChanges_CanStart() const;
	virtual void WatchDirChanges_Start( bool bForceRestrart, bool bSubFolder = false );
	virtual void WatchDirChanges_Stop();
	virtual CExtSafeString WatchDirChanges_GetWatchPath() const = 0;
	virtual HWND WatchDirChanges_GetSafeHwnd() const = 0;

}; /// class CExtSWDC

/////////////////////////////////////////////////////////////////////////////
// CExtCIP

template < class _IT > class CExtCIP
{
protected:
	_IT * m_ptr;
public:
	CExtCIP()
		: m_ptr( NULL )
	{
	}
	CExtCIP( _IT * ptr )
		: m_ptr( NULL )
	{
		AssignFrom( ptr );
	}
	CExtCIP( const CExtCIP & other )
		: m_ptr( NULL )
	{
		AssignFrom( other.m_ptr );
	}
	~CExtCIP()
	{
		Empty();
	}
	_IT * operator -> ()
	{
		return m_ptr;
	}
	const _IT * operator -> () const
	{
		return m_ptr;
	}
	operator _IT * ()
	{
		return m_ptr;
	}
	CExtCIP & operator = ( _IT * ptr )
	{
		if( LPVOID(m_ptr) == LPVOID(ptr) )
			return (*this);
		Empty();
		if( ptr != NULL )
			AssignFrom( ptr );
		return (*this);
	}
	CExtCIP & operator = ( const CExtCIP & other )
	{
		if( other.m_ptr == m_ptr || other.m_ptr == NULL )
			return *this;
		Empty();
		AssignFrom( other.m_ptr );
		return *this;
	}
	bool IsEmpty() const
	{
		return ( m_ptr == NULL ) ? true : false;
	}
	void Empty()
	{
		if( m_ptr == NULL )
			return;
		m_ptr->Release();
		m_ptr = NULL;
	}
	void AssignFrom( _IT * ptr )
	{
		if( LPVOID(m_ptr) == LPVOID(ptr) )
			return;
		Empty();
		m_ptr = ptr;
		if( m_ptr != NULL )
			m_ptr->AddRef();
	}
	void Attach( _IT * ptr )
	{
		ASSERT( IsEmpty() );
		m_ptr = ptr;
	}
	_IT * Detach()
	{
		_IT * ptr = m_ptr;
		m_ptr = NULL;
		return ptr;
	}
	_IT * GetPtr()
	{
		return m_ptr;
	}
	const _IT * GetPtr() const
	{
		return m_ptr;
	}
	_IT ** GetPtrPtr()
	{
		return (&m_ptr);
	}
	void ** GetPtrPtrVoid()
	{
		return ((void **)(&m_ptr));
	}
}; /// class CExtCIP

/////////////////////////////////////////////////////////////////////////////
// CExtShellBase

class __PROF_UIS_API CExtShellBase
{
	static int g_nInstanceCounter;
public:
	static const UINT g_nMsgDelayFocusPIDL;
	static const UINT g_nMsgUpdateItemContent;
	static const UINT g_nMsgChangeChildrenState;
	static const UINT g_nMsgShellItemExecute;
	static IMalloc * g_pMalloc;
	static LPITEMIDLIST g_pidlDesktop;
	static IShellFolder * g_pDesktopFolder;
	static CExtImageList g_ilShellBig;
	static CExtImageList g_ilShellSmall;
	static __EXT_MFC_SAFE_LPTSTR g_strTextOfMenuItemInEmptyShellContextMenu;
	static CExtCmdIcon g_iconOverlayShareSmall;
	static CExtCmdIcon g_iconOverlayShareBig;
	static CExtCmdIcon g_iconOverlayLinkSmall;
	static CExtCmdIcon g_iconOverlayLinkBig;
	static CExtCmdIcon g_iconOverlaySlowFileSmall;
	static CExtCmdIcon g_iconOverlaySlowFileBig;
	static bool stat_OverlayIconsInit();
	static void stat_OverlayIconsDone();

protected:
	static void stat_RemoveDateTimeField(
		CExtSafeString & sFormatStr,
		__EXT_MFC_SAFE_TCHAR chField,
		__EXT_MFC_SAFE_LPCTSTR sSeparator
		);
public:
	static CExtSafeString stat_GetDateTimeFieldText(
		const COleDateTime & _time,
		bool bIncludeDate              = true,
		bool bIncludeTime              = true,
		int  nTimeFormat               = 0,    // 0-default, 12-with AM/PM, 24-...
		bool bIncludeFieldYear         = true, // used only if bIncludeDate is !false
		bool bIncludeFieldMonth        = true, // used only if bIncludeDate is !false
		bool bIncludeFieldDay          = true, // used only if bIncludeDate is !false
		bool bIncludeFieldHour         = true, // used only if bIncludeTime is !false
		bool bIncludeFieldMinute       = true, // used only if bIncludeTime is !false
		bool bIncludeFieldSecond       = true, // used only if bIncludeTime is !false
		bool bIncludeFieldDesignator   = true  // used only if bIncludeTime is !false
		);
	static LPITEMIDLIST stat_CreateItem( UINT cbSize );
	static LPITEMIDLIST stat_CopyItem( LPCITEMIDLIST pidlSrc );
	static UINT stat_GetItemCount( LPCITEMIDLIST pidl );
	static UINT stat_GetItemSize( LPCITEMIDLIST pidl );
	static LPITEMIDLIST stat_GetNextItem( LPCITEMIDLIST pidl );
	static INT stat_GetParentItem( LPCITEMIDLIST pidl, LPITEMIDLIST & pidlParent );
	static LPITEMIDLIST stat_GetParentItem( LPCITEMIDLIST pidl );
	static INT stat_ILGetLength( LPCITEMIDLIST pidl );
	static LPCITEMIDLIST stat_ILGetNext( LPCITEMIDLIST pidl );
	static  LPITEMIDLIST stat_ILGetNext(  LPITEMIDLIST pidl );
	static LPCITEMIDLIST stat_ILGetLast( LPCITEMIDLIST pidl );
	static  LPITEMIDLIST stat_ILGetLast(  LPITEMIDLIST pidl );
	static LPITEMIDLIST stat_ILCombine( LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2 );
	static LPITEMIDLIST stat_ILCloneFirst( LPCITEMIDLIST pidl );
	static LPITEMIDLIST stat_ILClone( LPCITEMIDLIST pidl );
	static bool stat_ILExtractName( IShellFolder * pShellFolder, LPITEMIDLIST pItemIDList, DWORD dwFlags, CExtSafeString & strName );
	static LPITEMIDLIST stat_ILFromPath( __EXT_MFC_SAFE_LPCTSTR pszPath, HWND hWndOwner );
	static LPITEMIDLIST stat_ILCloneParent( LPCITEMIDLIST pidl );
	static bool stat_ILIsDesktop( LPCITEMIDLIST pidl );
	static bool stat_ILIsSimple( LPCITEMIDLIST pidl );
	static bool stat_ILRemoveLast( LPITEMIDLIST pidl );
	static bool stat_ILSerialize(
		LPITEMIDLIST * p_pidl,
		CArchive & ar,
		bool bEnableThrowExceptions = false
		);
	static bool stat_ILSerialize(
		LPITEMIDLIST * p_pidl,
		__EXT_MFC_SAFE_LPCTSTR sPIDLName,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		bool bSave,
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bEnableThrowExceptions = false
		);

	static HRESULT stat_SHBindToParent( LPCITEMIDLIST pidl, REFIID riid, LPVOID * ppv, LPCITEMIDLIST * ppidlLast );
	static HRESULT stat_SHGetFolderLocation( HWND hWnd, int csidl, HANDLE hToken, DWORD dwFlags, LPITEMIDLIST * ppidl );
	static HRESULT stat_SHAutoComplete( HWND hWnd, DWORD dwFlags );

	IShellFolder * m_pRootFolder;
	CExtShellBase();
	virtual ~CExtShellBase();

	virtual bool Shell_Init(
		bool bEnableThrowExceptions = false
		);
protected:
	virtual void Shell_DoneImpl();
public:
	virtual void Shell_Done();
	virtual bool Shell_IsInitialized() const;
	virtual HWND Shell_GetHWND();
	static bool stat_ValidPath( __EXT_MFC_SAFE_LPCTSTR strPath );
	static bool stat_ValidPIDL( LPCITEMIDLIST pidlAbsolute );

	static bool stat_CreateFolder( __EXT_MFC_SAFE_LPCTSTR strPath );
	static bool stat_FileExist( __EXT_MFC_SAFE_LPCTSTR strPath );
	static bool stat_IsFileAnywhereOpen( __EXT_MFC_SAFE_LPCTSTR strPath );
	static bool stat_DirExist( __EXT_MFC_SAFE_LPCTSTR strPath );

	struct __PROF_UIS_API NotifyShellItemExecuteData
	{
		HWND m_hWndControl;
		LPARAM m_lParamItemID;
		UINT m_nMessage;
		LPARAM m_lParamReserved;
		bool m_bHandled:1;
		NotifyShellItemExecuteData(
			HWND hWndControl,
			LPARAM lParamItemID,
			UINT nMessage,
			LPARAM lParamReserved = 0L
			);
		operator LPARAM () const;
		static NotifyShellItemExecuteData & FromLPARAM( LPARAM lParam );
		bool Notify();
	};
	static bool stat_NotifyShellItemExecute(
		HWND hWndControl,
		LPARAM lParamItemID,
		UINT nMessage,
		LPARAM lParamReserved = 0L
		);
}; /// class CExtShellBase

/////////////////////////////////////////////////////////////////////////////
// CExtShellLock

class __PROF_UIS_API CExtShellLock : public CExtShellBase
{
public:
	CExtShellLock();
	virtual ~CExtShellLock();
}; /// class CExtShellLock


/////////////////////////////////////////////////////////////////////////////
// CExtCIP_SF

class __PROF_UIS_API CExtCIP_SF : public CExtCIP < IShellFolder >
{
public:
	CExtCIP_SF();
	CExtCIP_SF( IShellFolder * ptr );
	CExtCIP_SF( IShellFolder * pParentFolder, LPCITEMIDLIST pidlRelative );
	CExtCIP_SF( LPCITEMIDLIST pidlAbsolute );
}; /// class CExtCIP_SF

/////////////////////////////////////////////////////////////////////////////
// CExtCIP_SCM

class __PROF_UIS_API CExtCIP_SCM : public CExtCIP < IContextMenu >
{
public:
	CExtCIP_SCM();
	CExtCIP_SCM(
		IShellFolder * pParentFolder,
		LPCITEMIDLIST pidlRelative,
		HWND hWndOwner // = NULL
		);
	CExtCIP_SCM(
		IShellFolder * pParentFolder,
		LPCITEMIDLIST * pidlArrayRelative,
		INT nCount,
		HWND hWndOwner // = NULL
		);
}; /// class CExtCIP_SCM

/////////////////////////////////////////////////////////////////////////////
// CExtCIP_SCM2

class __PROF_UIS_API CExtCIP_SCM2 : public CExtCIP < IContextMenu2 >
{
public:
	CExtCIP_SCM2( IContextMenu * ptr );
}; /// class CExtCIP_SCM2

/////////////////////////////////////////////////////////////////////////////
// CExtCIP_SCM3

class __PROF_UIS_API CExtCIP_SCM3 : public CExtCIP < IContextMenu3 >
{
public:
	CExtCIP_SCM3( IContextMenu * ptr );
}; /// class CExtCIP_SCM3

/////////////////////////////////////////////////////////////////////////////
// CExtSCM

class __PROF_UIS_API CExtSCM
{
protected:
	CWnd * m_pWndOwner; // can be NULL of no owner
	CExtCIP_SCM m_pSCM;
	UINT m_nCmdFirstID, m_nCmdLastID;
	WNDPROC m_pfnWndProcRestore;
public:
	CExtSCM();
	virtual ~CExtSCM();
	bool Create( IShellFolder * pParentFolder, LPCITEMIDLIST pidl );
	bool Create( IShellFolder * pParentFolder, LPCITEMIDLIST * pidlArrayRelative, INT nCount );
	bool InvokeCommand( UINT nCmdID );
	BOOL FillMenu(
		CMenu * pMenu,
		UINT nStartIndex = 0,
		UINT nCmdFirstID = 1,
		UINT nCmdLastID = 0x7FFF,
		UINT nFlags = 0
		);
	bool GetCommandDescription( UINT nCmdID, CExtSafeString & strDescriptionText );
	void GetCommandRange( UINT & nCmdFirstID, UINT & nCmdLastID ) const;
	bool IsCommandInRange( UINT nCmdID ) const;
	CWnd * GetOwner();
	const CWnd * GetOwner() const;
	void SetOwner( CWnd * pWndOwner );

protected:
	static const __EXT_MFC_SAFE_LPCTSTR g_str_SH_CTXMENU_OBJ;
	void _WatchOwnerWndMessages_Start();
	void _WatchOwnerWndMessages_Stop();
	static LRESULT CALLBACK stat_WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	virtual bool _WatchOwnerWndMessages_WindowProc( UINT message, WPARAM wParam, LPARAM lParam, LRESULT * pResult );
}; /// class CExtSCM

/////////////////////////////////////////////////////////////////////////////
// CExtSO

template < class _OT > class CExtSO
{
protected:
	_OT * m_ptr;
public:
	CExtSO()
		: m_ptr( NULL )
	{
	}
	CExtSO( const CExtSO < _OT > & other )
		: m_ptr( NULL )
	{
		AssignFrom( other );
	}
	~CExtSO()
	{
		Empty();
	}
	operator const _OT * () const
	{
		return m_ptr;
	}
	CExtSO & operator = ( const CExtSO < _OT > & other )
	{
		AssignFrom( other );
		return (*this);
	}
	void AssignFrom( const CExtSO < _OT > & other )
	{
		if( LPVOID(m_ptr) == LPVOID(other.m_ptr) )
			return;
		Empty();
		if( other.m_ptr == NULL )
			return;
		ASSERT( CExtShellBase::g_pMalloc != NULL );
		ULONG nSize = (ULONG)CExtShellBase::g_pMalloc->GetSize( (void*)other.m_ptr );
		ASSERT( nSize > 0 );
		if( other.m_ptr != NULL )
		{
			m_ptr = (_OT*)CExtShellBase::g_pMalloc->Alloc( nSize );
			::memcpy( m_ptr, other.m_ptr, nSize );
		}
	}
	void Empty()
	{
		_OT * ptr = Detach();
		if( ptr == NULL )
			return;
		ASSERT( CExtShellBase::g_pMalloc != NULL );
		CExtShellBase::g_pMalloc->Free( ptr );
	}
	bool IsEmpty() const
	{
		return ( m_ptr != NULL ) ? false : true;
	}
	void Attach( _OT * ptr )
	{
		ASSERT( m_ptr == NULL );
		m_ptr = ptr;
	}
	_OT * Detach()
	{
		_OT * ptr = m_ptr;
		m_ptr = NULL;
		return ptr;
	}
	_OT * GetPtr()
	{
		return m_ptr;
	}
	const _OT * GetPtr() const
	{
		return m_ptr;
	}
	_OT ** GetPtrPtr()
	{
		return (&m_ptr);
	}
	void ** GetPtrPtrVoid()
	{
		return ((void **)(&m_ptr));
	}
}; /// class CExtSO

/////////////////////////////////////////////////////////////////////////////
// CExtPIDL

#define __EXT_DEFAULT_ATTRIBUTES_FOR_CExtPIDL_GetAttributesOf__ \
	(	0                                                                                 \
	/*|	SFGAO_CANCOPY            * (capability) objects can be copied                  */ \
	/*|	SFGAO_CANMOVE            * (capability) objects can be moved                   */ \
	/*|	SFGAO_CANLINK            * (capability) objects can be linked                  */ \
	/*|	SFGAO_CANRENAME          * (capability) objects can be renamed                 */ \
	/*|	SFGAO_CANDELETE          * (capability) objects can be deleted                 */ \
	/*|	SFGAO_HASPROPSHEET       * (capability) objects have property sheets           */ \
	/*|	SFGAO_DROPTARGET        /* (capability) objects are drop target                */ \
	|	SFGAO_LINK              /* (display) shortcut (link)                           */ \
	|	SFGAO_SHARE             /* (display) shared                                    */ \
	|	SFGAO_READONLY          /* (display) read-only                                 */ \
	/*|	SFGAO_GHOSTED            * (display) ghosted icon                              */ \
	|	SFGAO_HIDDEN            /* (display) hidden object                             */ \
	|	SFGAO_FILESYSANCESTOR   /* (content) it contains file system folder            */ \
	|	SFGAO_FOLDER            /* (content) it's a folder.                            */ \
	|	SFGAO_FILESYSTEM        /* (content) is a file system thing (file,folder,root) */ \
	/*|	SFGAO_HASSUBFOLDER       * (content) expandable in the map pane                */ \
	/*|	SFGAO_VALIDATE           * invalidate cached information                       */ \
	|	SFGAO_REMOVABLE         /* is this removable media?                            */ \
	/*|	SFGAO_COMPRESSED         * object is compressed (use alt color)                */ \
	/*|	SFGAO_BROWSABLE          * is in-place browse-able                             */ \
	/*|	SFGAO_NONENUMERATED      * is a non-enumerated object                          */ \
	/*|	SFGAO_NEWCONTENT         * should show bold in explorer tree                   */ \
	)

class __PROF_UIS_API CExtPIDL : public CExtSO < ITEMIDLIST >
{
public:
	CExtPIDL();
	CExtPIDL( LPCITEMIDLIST pidl );
	CExtPIDL( LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidlRelative );
	CExtPIDL( UINT nSpecialFolder, HWND hWndOwner );
	CExtPIDL( __EXT_MFC_SAFE_LPCTSTR pszPath, HWND hWndOwner );
	virtual ~CExtPIDL();
	bool FromFolder( UINT nSpecialFolder, HWND hWndOwner );
	bool FromFolder( __EXT_MFC_SAFE_LPCTSTR pszPath, HWND hWndOwner );
	bool operator == ( const CExtSO < ITEMIDLIST > & other ) const;
	bool operator == ( LPCITEMIDLIST pidl ) const;
	bool IsRoot() const;
	int GetIconIndex(
		UINT uFlags = SHGFI_SMALLICON
		) const;
	void Combine( LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2 );
	void CloneParent( LPCITEMIDLIST pidl );
	void CloneLast( LPCITEMIDLIST pidl );
	void CloneFirst( LPCITEMIDLIST pidl );
	LPCITEMIDLIST GetFirst() const;
	LPITEMIDLIST GetFirst();
	LPCITEMIDLIST GetLast() const;
	LPITEMIDLIST GetLast();
	CExtSafeString GetPath() const;
	void AttachLast( LPITEMIDLIST pidl );
	IShellFolder * GetParentFolder() const;
	DWORD GetAttributesOf(
		DWORD dwAttributesRequested = DWORD(-1L)
		) const;
	bool CheckAttributesOf( DWORD dwAttributesRequested ) const;
	bool IsReadOnly() const;
	bool IsHidden() const;
	bool IsFolder() const;
	bool IsSimplePIDL() const;
	CExtSafeString GetDisplayNameOf(
		IShellFolder * pShellFolder = NULL,
		DWORD _shgdnf = __EXT_MFC_SHGDN_NORMAL
		) const;
	bool GetSpecialFolderLocation(
		HWND hWnd,
		int nFolder
		);
	bool Serialize(
		CArchive & ar,
		bool bEnableThrowExceptions = false
		);
	bool Serialize(
		__EXT_MFC_SAFE_LPCTSTR sPIDLName,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		bool bSave,
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bEnableThrowExceptions = false
		);
	bool _OnQueryWatchFS();
}; // class CExtPIDL

/////////////////////////////////////////////////////////////////////////////
// CExtArrayOfPIDLs

class __PROF_UIS_API CExtArrayOfPIDLs : private CTypedPtrArray < CPtrArray, CExtPIDL * >
{
public:
	CExtArrayOfPIDLs();
	virtual ~CExtArrayOfPIDLs();
	INT GetSize() const;
	INT GetCount() const;
	CExtPIDL * GetObjAt( INT nIndex );
	const CExtPIDL * GetObjAt( INT nIndex ) const;
	LPITEMIDLIST GetAt( INT nIndex );
	LPCITEMIDLIST GetAt( INT nIndex ) const;
	LPITEMIDLIST operator [] ( INT nIndex );
	LPCITEMIDLIST operator [] ( INT nIndex ) const;
	bool SetAt( INT nIndex, LPCITEMIDLIST pidl );
	bool SetAt( INT nIndex, const CExtPIDL & pidl );
	void RemoveAll();
	bool InsertAt( INT nIndex, LPCITEMIDLIST pidl );
	bool InsertAt( INT nIndex, const CExtPIDL & pidl );
	INT RemoveAt(
		INT nIndex,
		INT nCountToRemove = 1
		); // returns removed count
	INT RemoveFromUpToTail( INT nIndex );
	bool Add( LPCITEMIDLIST pidl );
	bool AddHead( LPCITEMIDLIST pidl );
	bool AddTail( LPCITEMIDLIST pidl );
	bool RemoveHead();
	bool RemoveTail();
	bool IsEmpty() const;
	void Empty();
	bool InsertAt( INT nStartIndex, const CExtArrayOfPIDLs & _arr );
	bool InsertAt( INT nStartIndex , const CExtArrayOfPIDLs * pArray );
	bool Append( const CExtArrayOfPIDLs & _arr );
	bool Append( const CExtArrayOfPIDLs * pArray );
	bool Copy( const CExtArrayOfPIDLs & _arr );
	bool Copy( const CExtArrayOfPIDLs * pArray );
	INT Find(
		LPCITEMIDLIST pidl,
		INT nStartSearchIndex // = -1
		) const;
	INT Find(
		const CExtPIDL & pidl,
		INT nStartSearchIndex // = -1
		) const;
	INT MakeUnique(); // returns count of removed items
}; // class CExtArrayOfPIDLs

/////////////////////////////////////////////////////////////////////////////
// CExtShellItemData

class __PROF_UIS_API CExtShellItemData
{
public:
	enum e_ShellItemType
	{
		__ESIT_NORMAL            = 0,
		__ESIT_JUMP_TO_ROOT      = 1,
		__ESIT_JUMP_ONE_LEVEL_UP = 2,
	};
	e_ShellItemType m_eSIT;
	CExtPIDL m_pidlAbsolute;
	CExtSWDC * m_pSWDC;
	mutable DWORD m_dwCachedAttributes;
	CExtShellItemData();
	virtual ~CExtShellItemData();
	virtual bool IsValid() const;
	bool UpdateCachedAttributes() const;
}; // class  CExtShellItemData

/////////////////////////////////////////////////////////////////////////////
// CExtShellContentNode

class __PROF_UIS_API CExtShellContentNode
{
public:
	CExtPIDL m_pidlRelative;
	CTypedPtrList < CPtrList, CExtShellContentNode * > m_listChildren;
	CExtShellContentNode * m_pParent;
	CExtShellContentNode(
		CExtShellContentNode * pParent = NULL
		);
	virtual ~CExtShellContentNode();
	void RemoveChildren();
	void Empty();
	CExtPIDL GetAbsPIDL() const;
	IShellFolder * GetSF() const;
	IShellFolder * GetParentSF() const;
	CExtSafeString GetDisplayNameOf(
		DWORD _shgdnf = __EXT_MFC_SHGDN_NORMAL
		) const;
protected:
	void _ExpandChildren(
		HWND hWnd,
		bool bIncludeHidden,
		INT nIndentLimit,
		INT nCurrentIndent,
		bool bAddToTail,
		IShellFolder * pSF
		);
	void _BuildContent_Impl(
		HWND hWnd,
		bool bIncludeHidden,
		INT nIndentLimit,
		CExtShellContentNode * pParent,
		LPITEMIDLIST pidlRelative,
		INT nCurrentIndent,
		bool bAddToTail
		);
public:
	void ExpandChildren(
		HWND hWnd,
		bool bIncludeHidden,
		INT nIndentLimit = 1,
		bool bAddToTail = true
		);
	void ExpandPIDL(
		LPCITEMIDLIST pidlToExpand
		);
	void BuildContent(
		HWND hWnd,
		bool bIncludeHidden,
		INT nIndentLimit = 1,
		CExtShellContentNode * pParent = NULL,
		LPITEMIDLIST pidlRelative = NULL,
		bool bAddToTail = true
		);
	CExtShellContentNode * Find(
		LPITEMIDLIST pidlAbsolute
		);
	CExtShellContentNode * FindChild(
		LPCITEMIDLIST pidlRelative
		);
protected:
	void _FillLinearList_Impl(
		CTypedPtrList < CPtrList, CExtShellContentNode * > & _list,
		bool bAddToTail
		);
public:
	void FillLinearList(
		CTypedPtrList < CPtrList, CExtShellContentNode * > & _list,
		bool bAddToTail = true
		);
	void FillLinearArray(
		CTypedPtrArray < CPtrArray, CExtShellContentNode * > & _arr,
		bool bAddToTail = true
		);
	INT GetIndent() const;
	bool GetIcon(
		CExtBitmap & _bmpIcon,
		bool bBigIcon = false
		) const;
}; /// class CExtShellContentNode

/////////////////////////////////////////////////////////////////////////////

#endif // ( ! defined __EXT_MFC_NO_SHELL )

#if ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )

/////////////////////////////////////////////////////////////////////////////
// CExtShellComboBox window

class __PROF_UIS_API CExtShellComboBox
	: public CExtComboBox
	, public CExtShellBase
{
public:
	CExtShellComboBox();
	virtual ~CExtShellComboBox();
	virtual HWND Shell_GetHWND();

	virtual void UpdateContent();
	virtual void OnFocusedItemChanged();
	void DelayFocusPIDL(
		LPCITEMIDLIST pidlAbsolute = NULL
		);
	bool FocusPIDL(
		LPCITEMIDLIST pidlAbsolute = NULL // absolute folder's PIDL
		);
	bool FocusPath(
		__EXT_MFC_SAFE_LPCTSTR strPath
		);

	static const UINT g_nMsgShellLocationChanged;
	INT m_nDrawTextFlags, m_nListBoxIndentPxAt96DPI, m_nListBoxItemHeightPxAt96DPI, m_nIconToTextMargin;
	bool m_bIncludeHiddenFolders:1, m_bFullRowSelection:1, m_bDisplayFullPathIfAvailable:1;

	bool m_bHelperEnableShellAutoComplete:1;
	DWORD m_dwHelperFlagsSHACF;
protected:
	CExtPIDL m_pidlDelayedFocus;
	CExtPIDL m_pidlFolder; // absolute folder's PIDL
	CExtShellContentNode m_node;
	CTypedPtrArray < CPtrArray, CExtShellContentNode * > m_arrLinear;
public:
	LPITEMIDLIST GetCurrentFolderPIDL();
	LPCITEMIDLIST GetCurrentFolderPIDL() const;

	//{{AFX_VIRTUAL(CExtShellComboBox)
	public:
	virtual void MeasureItem( LPMEASUREITEMSTRUCT pMIS );
	virtual void DrawItem( LPDRAWITEMSTRUCT pDIS );
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExtShellComboBox)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual bool OnHookSpyPreTranslateMessage(
		MSG * pMSG
		);
}; /// CExtShellComboBox

#endif //  ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )

#if ( ! defined __EXT_MFC_NO_SHELL_EXTENSIONS_COMBO_BOX )

/////////////////////////////////////////////////////////////////////////////
// CExtShellExtensionsComboBox window

class __PROF_UIS_API CExtShellExtensionsComboBox
#if ( _MFC_VER == 0x700 )
		: public CExtComboBox
#else
		: public CExtNCSB < CExtComboBox >
#endif
{
public:
	DECLARE_DYNCREATE( CExtShellExtensionsComboBox );
	CExtShellExtensionsComboBox();
	virtual ~CExtShellExtensionsComboBox();
protected:
	CExtSafeStringList m_listCurrentExtensions;
	CExtSafeStringList m_listTitleFilter;
	mutable CExtSafeMapStringToPtr m_mapExtensions;
	void _ClearExtensionsInfo();
public:
	bool GetExtensionsByTitle(
		__EXT_MFC_SAFE_LPCTSTR strTitle, // if NULL or empty - returns currently selected extensions
		CExtSafeStringList & listOfExtensions
		);
	void AddFilter(
		__EXT_MFC_SAFE_LPCTSTR strFilterTitle,
		const CExtSafeStringList & listFilterExtensions
		);
	void SetFilter( __EXT_MFC_SAFE_LPCTSTR strFilter );
	INT FindFilterIndexByExtension( __EXT_MFC_SAFE_LPCTSTR strExtension ) const;
	CExtSafeString GetFilterHash() const;

	//{{AFX_VIRTUAL(CExtShellExtensionsComboBox)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExtShellExtensionsComboBox)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //  ( ! defined __EXT_MFC_NO_SHELL_EXTENSIONS_COMBO_BOX )

#if ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CExtShellListCtrl window

class __PROF_UIS_API CExtShellListCtrl
	: public CExtListCtrl
	, public CExtShellLock
	, public CExtSWDC
{
public:
	CExtShellListCtrl();
	virtual ~CExtShellListCtrl();
protected:
	CExtShellListCtrl( UINT, CWnd * ) { } // for CExtNCSB template class compilation only
	CExtShellListCtrl( __EXT_MFC_SAFE_LPCTSTR, CWnd * ) { } // for CExtNCSB template class compilation only
public:
	static const UINT g_nMsgShellLocationChanged;

	virtual HWND Shell_GetHWND();

	//{{AFX_VIRTUAL(CExtShellListCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	CExtPIDL m_pidlDelayedFocus;
	bool m_bContentUpdating:1, m_bHelperShellCtxMenuTracking:1;
public:
	virtual INT CompareItems(
		const CExtListCtrlDataSortOrder & _sortOrder,
		INT nItemIndex1,
		INT nItemIndex2
		) const;
	virtual void _DeleteSelectedItems();
	virtual void _DeleteSpecifiedItems( CList < INT, INT > & _listSelectedItems );
	virtual bool _GetContextMenu( INT nItemIndex, CExtSCM & shellContextMenu );
	virtual bool _GetContextMenu( CList < INT, INT > & _listSelectedItems, CExtSCM & shellContextMenu );
	virtual bool _SpecificItem_CheckStatus( CList < INT, INT > & _listSelectedItems );
	virtual bool _SpecificItem_CheckStatus( int _indexItem );
	virtual int _SpecificItems_GetCount( CList < INT, INT > & _listSelectedItems );
	void DelayFocusPIDL(
		LPCITEMIDLIST pidlAbsolute = NULL
		);
	bool FocusPIDL(
		LPCITEMIDLIST pidlAbsolute = NULL
		);
	bool FocusPath(
		__EXT_MFC_SAFE_LPCTSTR strPath
		);

	virtual bool UpdateListViewContent(
		CExtShellItemData * lptvid,
		IShellFolder * pShellFolder
		);
	virtual bool UpdateContent( 
		LPITEMIDLIST pidlParent, 
		bool bCompleteRescan,
		CExtShellItemData * pShellItemData = NULL 
		);

	bool m_bDateTimeFieldIncludeDate:1;
	bool m_bDateTimeFieldIncludeTime:1;
	int  m_nDateTimeFieldFormat;    // 0-default, 12-with AM/PM, 24-...
	bool m_bDateTimeFieldIncludeFieldYear:1;
	bool m_bDateTimeFieldIncludeFieldMonth:1;
	bool m_bDateTimeFieldIncludeFieldDay:1;
	bool m_bDateTimeFieldIncludeFieldHour:1;
	bool m_bDateTimeFieldIncludeFieldMinute:1;
	bool m_bDateTimeFieldIncludeFieldSecond:1;
	bool m_bDateTimeFieldIncludeFieldDesignator:1;

	enum e_shell_list_column_type_t
	{
		__ESLCT_NAME = -1,
		__ESLCT_SIZE = -2,
		__ESLCT_TYPE = -3,
		__ESLCT_DATE_MODIFED = -4,
		__ESLCT_DATE_CREATED = -5,
		__ESLCT_DATE_ACCESSED = -6,
		__ESLCT_ATTRIBUTES = -7,
		__ESLCT_CUSTOM_TYPE = 0,
	};
	CArray < INT, INT > m_arrColumnTypes;

	bool _IsFileExtensionPresentInExtensionsList( const CExtPIDL & pidlItem ) const;

	virtual CExtSafeString OnShellListQueryColumnTypeText(
		e_shell_list_column_type_t eSLCT
		);
	virtual void OnShellListInitializeColumns();
	virtual void OnShellListInitializeItem(
		INT nIndexItem,
		CExtShellItemData * pShellItemData,
		__EXT_MFC_SAFE_LPCTSTR strPath,
		CFileFind & _ff
		);
	virtual void OnShellListInitializeSubItem(
		INT nIndexItem,
		INT nIndexSubItem,
		CExtShellItemData * pShellItemData,
		__EXT_MFC_SAFE_LPCTSTR strPath,
		CFileFind & _ff
		);

	virtual CExtSafeString OnShellListGetDateTimeFieldText(
		const COleDateTime & _time,
		bool bIncludeDate              = true,
		bool bIncludeTime              = true,
		int  nTimeFormat               = 0,    // 0-default, 12-with AM/PM, 24-...
		bool bIncludeFieldYear         = true, // used only if bIncludeDate is !false
		bool bIncludeFieldMonth        = true, // used only if bIncludeDate is !false
		bool bIncludeFieldDay          = true, // used only if bIncludeDate is !false
		bool bIncludeFieldHour         = true, // used only if bIncludeTime is !false
		bool bIncludeFieldMinute       = true, // used only if bIncludeTime is !false
		bool bIncludeFieldSecond       = true, // used only if bIncludeTime is !false
		bool bIncludeFieldDesignator   = true  // used only if bIncludeTime is !false
		) const;

	bool ShowParentFolderItemGet() const;
	void ShowParentFolderItemSet( bool bShowParentFolderItem );

	bool ShowRootFolderItemGet();
	void ShowRootFolderItemSet( bool bShowRootFolderItem );

	INT FindItemByPIDL(
		LPITEMIDLIST pidl,
		bool bRelative,
		INT nStartIndex = -1
		);

	INT FindItemByName(
		__EXT_MFC_SAFE_LPCTSTR strName,
		INT nStartIndex = -1,
		bool bCompareNoCase = true
		);

	virtual void _DeleteNotExistingItems();
	void RescanContent();

	virtual void ExtensionsListGet( CExtSafeStringList & listExtensions ) const;
	virtual void ExtensionsListSet( const CExtSafeStringList & listExtensions );

protected:
	CExtSafeStringList m_listExtensions;

	virtual bool _FillItem(
		LVITEM & _lvi,
		LPCITEMIDLIST pidl,
		IShellFolder * pParentFolder,
		LPCITEMIDLIST pidlRelative,
		CExtSafeString * strNameBuffer = NULL
		);
	virtual bool _InsertItemsFromIDLIst(
		IEnumIDList * pEnumIDList,
		CExtCIP_SF pParentFolderOfItem,
		IShellFolder * pParentFolder,
		int	& iCtr,
		bool bSpecialFolder,
		bool bCheckExisting,
		bool bSkipFolders,
		bool bSkipFiles
		);
	void _InsertParentFolderItem( LPITEMIDLIST pidl, bool bRoot = false );
	void _InsertRootFolderItem();
	void _UpdateWalkingFocusAndSelection();
	void _ItemStateFromShellFlags(
		INT nItemIndex,
		DWORD dwAttributesBits,
		DWORD dwAttributesMask
		);
	virtual void _SetAttributes(
		int iItem,
		DWORD dwAttributes
		);
	virtual bool _OnQueryWatchFS();
public:
	void Refresh();

protected:
	//{{AFX_MSG(CExtShellListCtrl)
	afx_msg void OnLvnEndLabelEdit( NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg void OnDeleteItem( NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg void OnDeleteAllItems( NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint point );
	afx_msg void OnBeginLabelEdit( NMHDR * pNMHDR, LRESULT * pResult );
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateItemContent( WPARAM wParam, LPARAM lParam );
	afx_msg void OnShellCommand( UINT nID );
	DECLARE_MESSAGE_MAP()

public:
	void _EnterFolderItem( LPITEMIDLIST pidl );
	
	virtual bool ShellItemExecute(
		INT nItemIndex, // -1 - use all selected items
		UINT nMessage
		);

protected:
	UINT			m_uFlags; // Flags defining which items to include in the enumeration.
	INT				m_nNameColumnWidth; // Size in pixels for the name column width.
	CExtPIDL		m_pidlFolder, m_pidlPrevSel;
	CExtCIP_SF	m_SF;
	CMap < CExtShellItemData *, CExtShellItemData * &, bool, bool > m_mapItemData;
	CExtSCM			m_myShellContextMenu;
	bool			m_bShowParentFolderItem:1;
	bool			m_bShowRootFolderItem:1;
	bool			m_bFoldersBeforeOthers:1;
	bool			m_bIncludeFiles:1;
	bool			m_bWatchFileSystem:1;
//	bool			m_bDragEnabled:1, m_bDropEnabled:1;
	bool			m_bShowExtensions:1;
	bool			m_bRunItems:1;
	bool			m_bShowShellContextMenus:1;
	
public:
	DWORD m_dwAttributeFilterAny, m_dwAttributeFilterAllPresent, m_dwAttributeFilterAllAbsent;
	bool m_bHelperWinExplorerLikeFixOfMyComputerAndMyNetworkPlaces:1;

//	bool DragEnabledGet() const;
//	void DragEnabledSet( bool bDragEnabled );
//	bool DropEnabledGet() const;
//	void DropEnagledSet( bool bDropEnabled );
	bool RunItemsGet() const;
	void RunItemsSet( bool bRunItems );
	bool ShowShellContextMenusGet() const;
	void ShowShellContextMenusSet( bool bShowShellContextMenus );

	void IncludeFilesSet(
		bool bIncludeFiles,
		bool bRescanNow = true
		);
	bool IncludeFilesGet() const;

	void ShowExtensionsSet(
		bool bShowExtensions,
		bool bRescanNow = true
		);
	bool ShowExtensionsGet() const;

	CExtSafeString GetCurrentFolderPath() const;
	LPITEMIDLIST GetCurrentFolderPIDL();
	LPCITEMIDLIST GetCurrentFolderPIDL() const;
	IShellFolder * GetCurrentFolder();

	bool FoldersBeforeOthersGet() const;
	void FoldersBeforeOthersSet( bool bFoldersBeforeOthers );

	bool WatchFileSystemGet() const;
	void WatchFileSystemSet( bool bWatchFileSystem );
	
	virtual CExtSafeString WatchDirChanges_GetWatchPath() const;
	virtual HWND WatchDirChanges_GetSafeHwnd() const;

}; /// class CExtShellListCtrl

#if ( _MFC_VER == 0x700 ) && (! defined __EXT_PROFUIS_STATIC_LINK)
	template class __PROF_UIS_API CExtNCSB_Impl < CExtShellListCtrl >;
#endif

template < > class
#if ( _MFC_VER != 0x700 )
	__PROF_UIS_API
#endif
	CExtNCSB < CExtShellListCtrl > : public CExtNCSB_Impl < CExtShellListCtrl >
{
public:
	CExtNCSB(
		bool bNcsbDelayedInitialization = false,
		bool bNcsbForceMiddleContainerMode = false
		)
		: CExtNCSB_Impl < CExtShellListCtrl > (
			bNcsbDelayedInitialization,
			bNcsbForceMiddleContainerMode
			)
	{
	}
	virtual ~CExtNCSB()
	{
	}
protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch( message )
		{
		case WM_HSCROLL:
		{
			UINT nSBCode = UINT(LOWORD(DWORD(wParam))), nPos = UINT(HIWORD(DWORD(wParam)));
			//TRACE2( "WM_HSCROLL, nSBCode = %d, nPos = %d\r\n", nSBCode, nPos );
			INT nItemExtent = 1;
			DWORD dwStyle = GetStyle();
			DWORD dwListCtrlType = dwStyle&LVS_TYPEMASK;
			switch( dwListCtrlType )
			{
			case LVS_ICON:
			case LVS_SMALLICON:
			case LVS_REPORT:
			break;
			case LVS_LIST:
			{
				CRect rcItem( 0, 0, 0, 0 );
				INT nTopIndex = GetTopIndex();
				if(		nTopIndex >= 0
					&&	CExtShellListCtrl::GetItemRect( nTopIndex, &rcItem, LVIR_BOUNDS )
					)
					nItemExtent = rcItem.Width();
			}
			break;
			} // switch( dwListCtrlType )
			if( nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION )
			{
				INT nPosOld = GetScrollPos( SB_HORZ );
				INT nPosShift = nPos*nItemExtent - nPosOld*nItemExtent;
				Scroll( CSize( nPosShift, 0 ) );
				return 0L;
			}
			if(		nSBCode == SB_LINELEFT
				||	nSBCode == SB_LINERIGHT
				||	nSBCode == SB_PAGELEFT
				||	nSBCode == SB_PAGERIGHT
				||	nSBCode == SB_LEFT
				||	nSBCode == SB_RIGHT
				||	nSBCode == SB_ENDSCROLL
				)
				return 0L;
		} // case WM_HSCROLL:
		break; // case WM_VSCROLL
		case WM_VSCROLL:
		{
			UINT nSBCode = UINT(LOWORD(DWORD(wParam))), nPos = UINT(HIWORD(DWORD(wParam)));
			//TRACE2( "WM_VSCROLL, nSBCode = %d, nPos = %d\r\n", nSBCode, nPos );
			INT nItemExtent = 1;
			DWORD dwStyle = GetStyle();
			DWORD dwListCtrlType = dwStyle&LVS_TYPEMASK;
			switch( dwListCtrlType )
			{
			case LVS_ICON:
			case LVS_SMALLICON:
			case LVS_LIST:
			break;
			case LVS_REPORT:
			{
				CRect rcItem( 0, 0, 0, 0 );
				INT nTopIndex = GetTopIndex();
				if(		nTopIndex >= 0
					&&	CExtShellListCtrl::GetItemRect( nTopIndex, &rcItem, LVIR_BOUNDS )
					)
					nItemExtent = rcItem.Height();
				else
				{
					TEXTMETRIC _tm;
					::memset( &_tm, 0, sizeof(TEXTMETRIC) );
					CClientDC dc( this );
					CFont * pFont = GetFont();
					int nSave = dc.SaveDC();
					dc.SelectObject( pFont );
					dc.GetTextMetrics( &_tm );
					nItemExtent = _tm.tmHeight + _tm.tmExternalLeading + 1;
					dc.RestoreDC( nSave );
				}
			}
			break;
			} // switch( dwListCtrlType )
			if( nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION )
			{
				INT nPosOld = GetScrollPos( SB_VERT );
				INT nPosShift = nPos*nItemExtent - nPosOld*nItemExtent;
				Scroll( CSize( 0, nPosShift ) );
				return 0L;
			}
			if(		nSBCode == SB_LINEUP
				||	nSBCode == SB_LINEDOWN
				||	nSBCode == SB_PAGEUP
				||	nSBCode == SB_PAGEDOWN
				||	nSBCode == SB_TOP
				||	nSBCode == SB_BOTTOM
				||	nSBCode == SB_ENDSCROLL
				)
				return 0L;
		}
		break; // case WM_VSCROLL
		} // switch( message )
		LRESULT lResult = CExtNCSB_Impl < CExtShellListCtrl > :: WindowProc( message, wParam, lParam );
		return lResult;
	}
}; // template < > class CExtNCSB < CExtShellListCtrl >

#if ( _MFC_VER == 0x700 ) && (! defined __EXT_PROFUIS_STATIC_LINK)
	template class __PROF_UIS_API CExtNCSB < CExtShellListCtrl >;
#endif

#endif //  ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )

#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CExtShellTreeCtrl

#if ( _MFC_VER == 0x700 ) && (! defined __EXT_PROFUIS_STATIC_LINK)
	template class __PROF_UIS_API CExtWTC < CExtTreeCtrl >;
#endif

class __PROF_UIS_API CExtShellTreeCtrl
	: public CExtWTC < CExtTreeCtrl >
	, public CExtShellLock
{
public:
	class __PROF_UIS_API CExtSWDCTI : public CExtSWDC
	{
	public:
		HWND m_hWndNotify;
		HTREEITEM m_hti;
		CExtSafeString m_strFullPath;
		CExtSWDCTI( HWND hWndNotify, HTREEITEM hti, __EXT_MFC_SAFE_LPCTSTR strFullPath );
		virtual ~CExtSWDCTI();
		virtual CExtSafeString WatchDirChanges_GetWatchPath() const;
		virtual HWND WatchDirChanges_GetSafeHwnd() const;
	}; // class CExtSWDCTI
protected:
	HTREEITEM m_htiLastCtxMenu;
	bool m_bIncludeFiles:1, m_bShowExtensions:1,
		m_bWatchFileSystem:1,
//		m_bDragEnabled:1, m_bDropEnable:1,
		m_bDelayedIconRescanning:1,
		m_bRunItems:1, m_bShowShellContextMenus:1,
		m_bHelperShellCtxMenuTracking:1;
	CExtSCM m_myShellContextMenu;
	virtual void _InitShellTree( bool bDelayedIconRescanning );
	virtual void _InitShellTreeIcons();
	virtual void _DoDelayedTreeIconInit();
	CExtPIDL m_pidlRoot, m_pidlDelayedFocus;
public:
	DWORD m_dwAttributeFilterAny, m_dwAttributeFilterAllPresent, m_dwAttributeFilterAllAbsent;
	BYTE m_nSCA_Hidden, m_nSCA_ReadOnly;

	virtual void OnPaintTreeItemIcon(
		HTREEITEM hti,
		CDC & dc,
		const CExtCmdIcon & _icon,
		CRect rcTreeIconArea,
		BYTE nSCA = BYTE(255)
		);

//	bool DragEnabledGet() const;
//	void DragEnabledSet( bool bDragEnabled );
//	bool DropEnabledGet() const;
//	void DropEnagledSet( bool bDropEnabled );
	bool RunItemsGet() const;
	void RunItemsSet( bool bRunItems );
	bool ShowShellContextMenusGet() const;
	void ShowShellContextMenusSet( bool bShowShellContextMenus );

	DECLARE_DYNCREATE( CExtShellTreeCtrl );
	CExtShellTreeCtrl();
	virtual ~CExtShellTreeCtrl();

	virtual HTREEITEM _CreateFolder(
		__EXT_MFC_SAFE_LPCTSTR strNewFolderName,
		HTREEITEM htiParent = NULL // NULL - get from focus
		);
	virtual BOOL Expand( HTREEITEM hti, UINT nCode );
	void RescanContent();
	virtual bool UpdateContent( HTREEITEM hti, bool bCompleteRescan );
	void RefreshShellRoot(
		LPCITEMIDLIST pidlRoot = NULL
		);
	void DelayFocusPIDL(
		LPCITEMIDLIST pidlAbsolute = NULL
		);
	HTREEITEM FocusPIDL(
		LPCITEMIDLIST pidlAbsolute,
		bool bFocusItem = true,
		bool bSelectItem = true,
		bool bUnselectOtherItems = true,
		bool bRedraw = true
		);
	HTREEITEM FocusPath(
		__EXT_MFC_SAFE_LPCTSTR strPathToSelect,
		bool bFocusItem = true,
		bool bSelectItem = true,
		bool bUnselectOtherItems = true,
		bool bRedraw = true
		);

	void IncludeFilesSet(
		bool bIncludeFiles,
		bool bRescanNow = true
		);
	bool IncludeFilesGet() const;

	bool WatchFileSystemGet() const;
	void WatchFileSystemSet( bool bWatchFileSystem );

	virtual HTREEITEM _GetItemToSelectAfterDeletion(
		HTREEITEM htiToDelete = NULL // NULL - get focused item
		);

	void ShowExtensionsSet(
		bool bShowExtensions,
		bool bRescanNow = true
		);
	bool ShowExtensionsGet() const;

	HTREEITEM FindItemByPIDL( HTREEITEM htiParent, LPCITEMIDLIST pidlAbsolute );
	virtual void OnInplaceControlComplete(
		__EXT_MFC_SAFE_LPCTSTR strEditedText,
		bool bEditingLabel
		);
	bool _GetContextMenu( HTREEITEM hItem, CExtSCM & shellContextMenu );
	bool _GetContextMenu( CList < HTREEITEM, HTREEITEM > & _listSelectedItems, CExtSCM & shellContextMenu );
protected:
	virtual void _CleanupUpNeededChildren( HTREEITEM hParent );
	virtual void _DeleteNotExistingItems( HTREEITEM htiParent );
	virtual bool _FillItem(
		TVITEM & _tvi,
		LPCITEMIDLIST pidl,
		IShellFolder * pParentFolder,
		LPCITEMIDLIST pidlRelative,
		CExtSafeString * strNameBuffer = NULL
		);
	struct __PROF_UIS_API SortLParamData
	{
		IShellFolder * m_pSF;
		CExtShellTreeCtrl & m_wndShellTree;
		SortLParamData(
			IShellFolder * pSF,
			CExtShellTreeCtrl & wndShellTree
			);
		operator LPARAM () const;
		static SortLParamData & FromLPARAM( LPARAM lParam );
	};
	virtual void _InsertItemFromIDList(
		HTREEITEM hParent = NULL,
		LPITEMIDLIST pidlParent = NULL,
		LPITEMIDLIST pidl = NULL,
		IShellFolder * pParentFolder = NULL,
		bool bRoot = false,
		bool bCheckExisting = false
		);
	virtual HTREEITEM _InsertRootItem( LPITEMIDLIST pidlRoot = NULL );
	virtual void _SWDC_SubFolders_Start( HTREEITEM htiParent );
	virtual void _SWDC_SubFolders_Stop( HTREEITEM htiParent );
	CExtSWDCTI * _SWDC_Get( HTREEITEM hti );
public:
	virtual bool OnHookSpyKeyMsg(
		MSG * pMSG
		);
	virtual bool ShellItemExecute(
		HTREEITEM hti, // NULL means use all selected items
		UINT nMessage
		);
	void Refresh( HTREEITEM hti = TVI_ROOT );

protected:
	virtual void _UnregisterItemsFromMap(
		HTREEITEM hti = NULL
		);
	virtual bool _OnQueryWatchFS( HTREEITEM hti );
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtShellTreeCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CExtShellTreeCtrl)
	//}}AFX_MSG
	afx_msg void OnShellCommand( UINT nID );
	afx_msg LRESULT OnUpdateItemContent( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnChangeChildrenStateItem( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	virtual HWND Shell_GetHWND();
	virtual void OnTreeItemUnRegisterData(
		TREEITEMINFO_t & _TII
		);
	virtual void OnTreeItemContextMenu(
		HTREEITEM hti,
		CPoint point, // client coordinates
		DWORD dwHitTestFlags
		);
	virtual bool OnTreeMouseClick(
		HTREEITEM hti,
		DWORD dwHitTestFlags,
		int nMouseButton,
		int nClick,
		UINT nMouseEventFlags,
		CPoint point
		);
}; /// class CExtShellTreeCtrl

#endif //  ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )

/////////////////////////////////////////////////////////////////////////////
// CExtShellDialogBrowseFor dialog

class __PROF_UIS_API CExtShellDialogBrowseFor
	: public
#if ( _MFC_VER == 0x700 )
		CExtResizableDialog
#else
		CExtNCW < CExtResizableDialog >
#endif

	, public CExtShellLock
{
public:
	CExtShellDialogBrowseFor(
		CWnd * pParent = NULL
		);

	// enum { IDD = IDD_EXT_DIALOG_BROWSE_FOR };

	//{{AFX_DATA(CExtShellDialogBrowseFor)
	CExtLabel	m_labelAtTop;
	CExtLabel	m_labelAtBottom;
	CExtEdit	m_edit;
	CExtButton	m_buttonMakeNewFolder;
	CExtButton	m_buttonCancel;
	CExtButton	m_buttonOK;
	//}}AFX_DATA
	CExtNCSB < CExtShellTreeCtrl > m_wndShellTree;

	CExtSafeString m_strCustomDialogCaption;
	CExtSafeString m_strCustomOkButtonCaption;
	CExtSafeString m_strCustomCancelButtonCaption;
	CExtSafeString m_strCustomNewFolderButtonCaption;
	CExtSafeString m_strCustomEditLabelCaption;
	CExtSafeString m_strLabelAtTop;
	bool m_bShowFolderEdit:1, m_bShowMakeNewFolderButton:1, m_bShowFullPath:1,
		m_bCurrentDirectorySetInitially:1,
		m_bCurrentDirectorySetOnWalk:1,
		m_bCurrentDirectoryRestoreFinallyOnOK:1,
		m_bCurrentDirectoryRestoreFinallyOnCancel:1,
		m_bSaveRestoreShellLocation:1, m_bSaveRestoreWindowPosition:1;
	CExtSafeString m_strSectionSettingsEntry, m_strSectionNameCompany, m_strSectionNameProduct;

	CExtPIDL m_pidlRoot;
	CExtPIDL m_pidlResult; // also used to focus tree item initially
	CExtSafeString m_strDisplayNameResult, m_strRetValPath;

protected:
	CExtSafeString m_strCurrentDirectoryToRestore;
	virtual void _GetSettingsLocationStrings(
		CExtSafeString & strSectionSettingsEntry,
		CExtSafeString & strSectionNameCompany,
		CExtSafeString & strSectionNameProduct
		) const;

	//{{AFX_VIRTUAL(CExtShellDialogBrowseFor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExtShellDialogBrowseFor)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeExtBffEdit();
	afx_msg void OnExtBffMakeNewFolder();
	afx_msg void OnSelChangedExtTreeShell(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnShellItemExecute( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShellTreeDelayedItemFocus( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; // class CExtShellDialogBrowseFor

#endif //  ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

/////////////////////////////////////////////////////////////////////////////
// CExtShellDialogCreateFolder dialog

class
#if ( _MFC_VER != 0x700 )
	__PROF_UIS_API
#endif
	CExtShellDialogCreateFolder
		: public
#if ( _MFC_VER == 0x700 )
				CExtResizableDialog
#else
				CExtNCW < CExtResizableDialog >
#endif
{
public:
	CExtShellDialogCreateFolder(
		CWnd * pParent = NULL
		);

	// enum { IDD = IDD_EXT_DIALOG_SHELL_CREATE_FOLDER };

	//{{AFX_DATA(CExtShellDialogCreateFolder)
	CExtButton	m_buttonOk;
	CExtButton	m_buttonCancel;
	CExtLabel	m_staticNameFolder;
	CExtEdit	m_editNameFolder;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CExtShellDialogCreateFolder)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	__EXT_MFC_SAFE_LPCTSTR GetNameFolder();

protected:
	CExtSafeString m_strNameFolder;
	//{{AFX_MSG(CExtShellDialogCreateFolder)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtShellDialogCreateFolder

#if ( _MFC_VER == 0x700 ) && (! defined __EXT_PROFUIS_STATIC_LINK)
	class __PROF_UIS_API CExtShellDialogCreateFolder;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtShellDialogFile dialog

class __PROF_UIS_API CExtShellDialogFile
	: public
#if ( _MFC_VER == 0x700 )
		CExtResizableDialog
#else
		CExtNCW < CExtResizableDialog >
#endif
	, public CExtShellLock
{
public:
	class __PROF_UIS_API SplitterWndForTreeAndList : public CExtSplitterWnd
	{
	protected:
		UINT m_nSavedShellTreeDlgCtrlID, m_nSavedShellListDlgCtrlID;
	public:
		SplitterWndForTreeAndList();
		virtual ~SplitterWndForTreeAndList();
		virtual bool Create(
			CExtShellTreeCtrl & wndShellTree,
			CExtShellListCtrl & wndShellList,
			CWnd & wndParent,
			bool bShowTree,
			DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_TABSTOP,
			UINT nID = UINT(__EXT_MFC_IDC_STATIC)
			);
	protected:
		virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	}; /// class SplitterWndForTreeAndList

	enum e_file_dialog_type_t
	{
	__EFDT_OPEN_SINGLE		= 0,
	__EFDT_OPEN_MULTIPLE	= 1,
	__EFDT_SAVE				= 2,
	__EFDT_SAVE_AS			= 3,
	__EFDT_COUNT			= 4,
	__EFDT_MIN_VALUE		= 0,
	__EFDT_MAX_VALUE		= 3,
	};
protected:
	e_file_dialog_type_t m_eFDT;
public:
	CExtSafeString m_strCustomDialogCaption;
	CExtSafeString m_strCustomOpenSaveButtonCaption;
	CExtSafeString m_strCustomCancelButtonCaption;
	CExtSafeString m_strCustomFileNameLabelCaption;
	CExtSafeString m_strCustomFileTypeLabelCaption;

	CExtSafeString m_strEditorTextInitial;

	CExtShellDialogFile(
		CWnd * pParent = NULL,
		e_file_dialog_type_t _eFDT = __EFDT_OPEN_SINGLE
		);
	virtual ~CExtShellDialogFile();

	// enum { IDD = IDD_EXT_DIALOG_FILE };

	//{{AFX_DATA(CExtShellDialogFile)
	CExtIconButton		m_buttonCollapse;
	CExtIconButton		m_buttonExpand;
	CExtIconButton		m_buttonRefresh;
	CExtIconButton		m_buttonForward;
	CExtIconButton		m_buttonBack;
	CExtIconButton		m_buttonViewMenu;
	CExtIconButton		m_buttonCreateNewFolder;
	CExtIconButton		m_buttonDelete;
	CExtIconButton		m_buttonUpOneLevel;
	CExtLabel			m_labelFileName;
	CExtLabel			m_labelFilesOfType;
	CExtLabel			m_labelLookIn;
	CExtButton			m_buttonCancel;
	CExtButton			m_buttonOK;
	CExtEdit			m_editFileName;
	//}}AFX_DATA
	SplitterWndForTreeAndList m_wndSplitter;
	CExtNCSB < CExtShellListCtrl > m_wndShellList;
	CExtNCSB < CExtShellTreeCtrl > m_wndShellTree;
	CExtNCSB < CExtShellComboBox > m_comboLookIn;
	CExtShellExtensionsComboBox m_comboFileTypes;
	//bool m_bCheckSelectedFilesExist:1;

	//{{AFX_VIRTUAL(CExtShellDialogFile)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

public:
	CExtSafeString m_strRetValNames, m_strRetValFolder;
	CExtPIDL m_pidlRetValFolder;
	CExtArrayOfPIDLs m_arrRetVal;
	CExtSafeStringArray m_arrRetValNames;
	CExtSafeString m_strCommandProfileName;
	INT m_nInitialListViewMode; // LVS_SMALLICON, LVS_ICON, LVS_LIST or LVS_REPORT
	INT m_nInitialFileTypeIndex;
	bool m_bHelperEnableShellAutoCompleteInFileNameEditor:1;
	DWORD m_dwHelperFlagsSHACFforFileNameEditor;

	bool
		m_bEnableCollapsing:1,
		m_bCollapseInitially:1,
		m_bShowFileNamesEdit:1,
		m_bReadOnlyFileNamesEdit:1,
		m_bShowFileTypesComboBox:1,
		m_bPathMustExist:1,
		m_bFilesMustExist:1,
		m_bCheckReadAccess:1,
		m_bCheckWriteAccess:1,
		m_bPromptOverwrite:1, // used in save dialogs only
		m_bResolveLinks:1,
		m_bEnableCollapsingAnimation:1,
		m_bUseSplitter:1,
		m_bShowTree:1,
		m_bCurrentDirectorySetInitially:1,
		m_bCurrentDirectorySetOnWalk:1,
		m_bCurrentDirectoryRestoreFinallyOnOK:1,
		m_bCurrentDirectoryRestoreFinallyOnCancel:1,
		m_bSaveRestoreInterriorOptions:1,
		m_bSaveRestoreShellLocation:1,
		m_bSaveRestoreWindowPosition:1;

	CExtSafeString m_strSectionSettingsEntry, m_strSectionNameCompany, m_strSectionNameProduct;

protected:
	CExtSafeString m_strCurrentDirectoryToRestore;
	bool m_bHaldingSelectionInTree:1, m_bHaldingSelectionInList:1, m_bHaldingSelectionInCombo:1, m_bHaldingHistory:1,
		m_bLockChangingsValidation:1;
	CExtArrayOfPIDLs m_arrayHistory;
	int m_nCurrentHistoryPosition;
	UINT m_nValidateNamesTimerID, m_nValidateNamesTimerPeriod;
	UINT m_nValidateListViewSelectionTimerID, m_nValidateListViewSelectionTimerPeriod;
	bool m_bHelperValidatingEditor:1, m_bHelperValidatingListViewSelection:1;
	CList < HWND, HWND > m_listCollapsedHWNDs, m_listMovedHWNDs;
	INT m_nSavedCollapsedHeight, m_nCollapsedWindowHeight;
	virtual bool _ValidateEditor( bool bAllowChangingListView );
	virtual void _ValidateListViewSelection();
	virtual void _HistoryBack();
	virtual void _HistoryForward();
	virtual void _ShellListExtensionSet();
	virtual void _ResolveLinksInResult();
	virtual void _SwitchTwoButtons( CWnd * pButtonDisappear, CWnd * pButtonAppear );
	virtual void _CleanUpOnClose();
	virtual void _ReAnchor();
	virtual bool _IfFolderThenOpen();
	virtual bool _CanPorcessWndCollapsing(
		bool bCollapsingDialog, // true - collapsing dialog, false - expanding dialog
		bool bHidingOrShowingWnd, // true - hiding/showing hWnd, false - moving hWnd
		HWND hWnd
		);
	bool m_bHelperCollapsignAnimation:1;
	virtual bool _AnimateCollapsing(
		bool bCollapsingDialog, // true - collapsing dialog, false - expanding dialog
		CRect rcFrom, // initial window rectangle
		CRect rcTo // final window rectangle
		);
	virtual void ArrangeLayout(
		int cx = -1,
		int cy = -1
		);
#if ( _MFC_VER != 0x700 )
	virtual bool NcFrameImpl_GetMinMaxInfo(
		LPMINMAXINFO pMMI
		) const;
#endif
	virtual void _GetSettingsLocationStrings(
		CExtSafeString & strSectionSettingsEntry,
		CExtSafeString & strSectionNameCompany,
		CExtSafeString & strSectionNameProduct
		) const;
	bool _InterriorOptionsSerialize(
		__EXT_MFC_SAFE_LPCTSTR sUniqueName,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		bool bSave,
		HKEY hKeyRoot = HKEY_CURRENT_USER,
		bool bEnableThrowExceptions = false
		);
	virtual bool _InterriorOptionsSerialize(
		CArchive & ar,
		bool bEnableThrowExceptions = false
		);

	//{{AFX_MSG(CExtShellDialogFile)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelendokComboFileTypes();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonCollapse();
	afx_msg void OnButtonExpand();
	afx_msg void OnButtonBack();
	afx_msg void OnButtonForward();
	afx_msg void OnButtonUpOneLevel();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonCreateNewFolder();
	afx_msg void OnChangeEditFileName();
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	//}}AFX_MSG
	afx_msg void OnItemChangedShellList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnShellItemExecute( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShellTreeDelayedItemFocus( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShellListLocationChanged( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShellComboLocationChanged( WPARAM wParam, LPARAM lParam );
	afx_msg void OnListViewMode_Tiles();
	afx_msg void OnListViewMode_UpdateTiles( CCmdUI * pCmdUI );
	afx_msg void OnListViewMode_Icons();
	afx_msg void OnListViewMode_UpdateIcons( CCmdUI * pCmdUI );
	afx_msg void OnListViewMode_List();
	afx_msg void OnListViewMode_UpdateList( CCmdUI * pCmdUI );
	afx_msg void OnListViewMode_Details();
	afx_msg void OnListViewMode_UpdateDetails( CCmdUI * pCmdUI );
	DECLARE_MESSAGE_MAP()
};  /// class CExtShellDialogFile

/////////////////////////////////////////////////////////////////////////////
// CExtDMFP template decorator class for MFC's CDocManager
// (document manager's file prompt, makes CDocManager using CExtShellDialogFile)

template < class _BDM > class CExtDMFP : public _BDM
{
public:
	CExtDMFP()
	{
	}
	virtual ~CExtDMFP()
	{
	}
	static void stat_AppendFilterSuffix(
		CExtSafeString & strComposedFilters,
		CDocTemplate * pTemplate
		)
	{
		ASSERT_VALID( pTemplate );
		ASSERT_KINDOF( CDocTemplate, pTemplate );
		CString strFilterExt, strFilterName;
		if(		pTemplate->GetDocString( strFilterExt, CDocTemplate::filterExt )
			&&	(! strFilterExt.IsEmpty() )
			&&	pTemplate->GetDocString( strFilterName, CDocTemplate::filterName )
			&&	(! strFilterName.IsEmpty() )
			)
		{
			ASSERT( strFilterExt[0] == '.' );
			strComposedFilters += strFilterName;
			ASSERT( ! strComposedFilters.IsEmpty() );
			strComposedFilters += _T('|');
			strComposedFilters += (TCHAR)'*';
			strComposedFilters += strFilterExt;
			strComposedFilters += _T('|');
		}
	}
	virtual BOOL DoPromptFileName(
		CString & strFileName,
		UINT nIDSTitle,
		DWORD lFlags,
		BOOL bOpenFileDialog,
		CDocTemplate * pTemplate
		)
	{
		ASSERT_VALID( this );
		ASSERT_KINDOF( CDocManager, this );
		CExtShellDialogFile dlgShellFile(
			NULL,
			bOpenFileDialog
				?	(	( ( lFlags & OFN_ALLOWMULTISELECT ) != 0 )
						? CExtShellDialogFile::__EFDT_OPEN_MULTIPLE
						: CExtShellDialogFile::__EFDT_OPEN_SINGLE
					)
				:	CExtShellDialogFile::__EFDT_SAVE
			);
		dlgShellFile.m_bFilesMustExist = ( ( lFlags & OFN_FILEMUSTEXIST ) != 0 ) ? true : false;
		dlgShellFile.m_bPathMustExist = ( ( lFlags & OFN_PATHMUSTEXIST ) != 0 ) ? true : false;
		dlgShellFile.m_bPromptOverwrite = ( ( lFlags & OFN_OVERWRITEPROMPT ) != 0 ) ? true : false;
		dlgShellFile.m_strEditorTextInitial = LPCTSTR( strFileName );
		VERIFY(  g_ResourceManager->LoadString( dlgShellFile.m_strCustomDialogCaption, nIDSTitle ) );

		CExtSafeString strAllFilesFilterItem;
		VERIFY(  g_ResourceManager->LoadString( strAllFilesFilterItem, AFX_IDS_ALLFILTER ) );
		strAllFilesFilterItem += _T('|');
		strAllFilesFilterItem += _T("*.*");
		strAllFilesFilterItem += _T('|');

		CExtSafeString strComposedFilters;
		if( pTemplate != NULL )
			stat_AppendFilterSuffix( strComposedFilters, pTemplate );
		else
		{
			for( POSITION pos = m_templateList.GetHeadPosition(); pos != NULL; )
				stat_AppendFilterSuffix( strComposedFilters, (CDocTemplate*)m_templateList.GetNext( pos ) );
		}
		strComposedFilters += strAllFilesFilterItem;

		dlgShellFile.m_comboFileTypes.SetFilter( LPCTSTR( strComposedFilters ) );
		dlgShellFile.m_nInitialFileTypeIndex = 0;

		int nResult = int( dlgShellFile.DoModal() );
		strFileName = LPCTSTR( dlgShellFile.m_strRetValNames );
		return ( nResult == IDOK ) ? TRUE : FALSE;
	}
}; /// template class CExtDMFP

#endif //  ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // (!defined __EXT_CONTROLS_SHELL_H)
