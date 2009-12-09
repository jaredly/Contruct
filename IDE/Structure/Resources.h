#pragma once

enum ResourcesType
{
	resource_binary = 1,
	resource_icon = 2,
	resource_font = 3,
	resource_menu = 4
};

class ApplicationResource
{
public:

	ApplicationResource() :
		tree(NULL),
		id(-1)
	{
	}

	bool operator == (const ApplicationResource& b)
	{
		return (name == b.name);
	}

	int id;

	CString GetName();
	void SetName(LPCSTR);

	ResourcesType GetType();

	void Serialize(CArchive&);

	HTREEITEM tree;

protected:

	CString name;
	ResourcesType type;
};

class BinaryResource : public ApplicationResource
{
public:

	BinaryResource() 
	{
		type = resource_binary;
	}

	CString path;

	void Preview(); // applies if it's a music/sound file
	bool Serialize(CArchive& ar);
};

class FontResource : public ApplicationResource
{
public:

	FontResource()
	{
		type = resource_font;
	}

	// not yet written

	bool Serialize(CArchive& ar);
};

class IconResource : public ApplicationResource
{
public:

	IconResource()
	{
		type = resource_icon;
	}

	IconResource(LPCSTR name_)
	{
		name = name_;
		type = resource_icon;
	}

	CxImage icon;

	bool Serialize(CArchive& ar);
};

class MenuResourceItem
{
public:

	MenuResourceItem() :
		id(-1),
		checked(false),
		disabled(false)
	{
	}

	CString text;
	int     id;
	bool    checked;
	bool    disabled;

	vector<MenuResourceItem> items;

	bool Serialize(CArchive& ar);
};

class MenuResource : public ApplicationResource
{
public:

	MenuResource()
	{
		type = resource_menu;
	}

	MenuResource(LPCSTR name_)
	{
		name = name_;
		type = resource_menu;
	}

	vector<MenuResourceItem> items;

	HMENU CreateMenu();
	void CreateDefault();

	bool Serialize(CArchive& ar);
};