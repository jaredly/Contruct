// CGroupListCtrl.h


#ifndef CGROUPLISTCTRL
#define CGROUPLISTCTRL

class CGroupListCtrl : public CListCtrl
{
public:
	// Add a group
	// Index: index of group
	// Name : group name
	void AddGroup(int Index, LPCSTR Name);

	// Enable groups. This should be called after all groups have been added
	// bEnable: true by default; to enable group view mode
	void EnableGroups(bool bEnable = true)
	{
		ListView_EnableGroupView(m_hWnd, bEnable);
	}

	// Remove all groups
	void RemoveAllGroups()
	{
		ListView_RemoveAllGroups(m_hWnd);
	}

	// Remove a group
	// Index: group index to remove
	void RemoveGroup(int Index)
	{
		ListView_RemoveGroup(m_hWnd, Index);
	}

	// Insert item with default CListCtrl parameters, but with Group
	// Index: index of new item
	// Text : text of item
	// Image: image index of item
	// Group: group index of item
	void InsertItem(int Index, LPCSTR Text, int Image, int Group)
	{
		LVITEM li;
		li.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE | LVIF_GROUPID;
		li.state = 0;
		li.stateMask = 0;
		li.iItem =Index;
		li.iImage = Image;
		li.iSubItem = 0;
		li.iGroupId = Group;
		li.pszText = (LPTSTR)Text;
		ListView_InsertItem(m_hWnd, &li);

		groups[Index] = Group;
	}

	// Enable full row select
	// bFullRowSelect: true by default; enable full row select
	void EnableFullRowSelect(bool bFullRowSelect = true)
	{
		SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, bFullRowSelect ? LVS_EX_FULLROWSELECT : 0);
	}

	// Get selected item
	// Simply get selected item without hassle
	int GetSelectedItem()
	{
		POSITION Position = GetFirstSelectedItemPosition();
		return GetNextSelectedItem(Position);
	}

	// Get a cell rect
	BOOL GetCellRect(int iRow, int iCol, int nArea, CRect &rect);

	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:

	map<int, int> groups;
	map<int, bool> bold_items;
};

#endif