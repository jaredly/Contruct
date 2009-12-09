#pragma once

class CChildFrame;

enum ProjectBarImage
{
	image_stub = 0,
	image_application = 1,
	image_layout = 2,
	image_event_sheet = 3,
	image_object_folder = 4,
	image_binary_resource = 5,
	image_font_resource = 6,
	image_icon_resource = 7,
	image_menu_resource = 8,
	image_sound_resource = 9,
	image_music_resource = 10,
	image_global_variable = 11
};

class ProjectBar : public CExtControlBar
{
public:

	ProjectBar();

	CXHtmlTree tree;
	CImageList images;
	CFont font;

	CChildFrame* last_opened;

	void AddApplication(CApplication* application);
	void UpdateApplication(CApplication* application);

	virtual void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);

protected:

	bool stop_expand;
	bool dragging_object;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnRightClick(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnSingleClick(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnBeginEditLabel(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnEditLabel(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg LRESULT OnBeginDrag(WPARAM, LPARAM);
	afx_msg LRESULT OnEndDrag(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};