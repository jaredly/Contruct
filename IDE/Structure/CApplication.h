#pragma once

#define NUM_BUILTIN_TRAITS 7
extern CString g_BuiltInTraits[NUM_BUILTIN_TRAITS];

class BehaviorControl {
public:
	CString name;
	int comboIndex;
	int vk;
	int player;		// 0-9

	BehaviorControl() : player(0) {}
	BehaviorControl(const char* _name, int _comboIndex, int _vk)
		: comboIndex(_comboIndex),
		  vk(_vk),
		  player(0)	// default player 1
	{
		name = _name;
	}

	bool operator==(const BehaviorControl& other)
	{
		CString l = name;
		CString r = other.name;

		l.MakeLower();
		r.MakeLower();

		return (l == r) && (player == other.player);
	}
};

// Used to store info about an object cross-application
class ObjectTypeDetail
{
public:
	class NameDllInfo
	{
	public:
		NameDllInfo(){};
		NameDllInfo(CString init_name, DWORD init_dll) : name(init_name), dll(init_dll){}
		CString name;
		DWORD dll;
	};

	void CopyDetails(class CObjType* type);
	bool Match(CObjType* type);

	list<NameDllInfo> behaviors;
	list<CString> effects;
	list<NameDllInfo> private_variables;

	int id;
	NameDllInfo info;
};

class CApplication
{
public:

	CApplication();
	~CApplication();

	void Instantiate();
	bool CloseSafely();

    bool Serialize(CArchive&);

	void ChangeName(CString);
	void ChangeModifiedStatus(bool modified_ = true);
	bool GetModifiedStatus();

	// layouts
	CLayout* AddLayout();
	void RemoveLayout(CLayout*);
	bool LayoutNameExists(CString);
	CString	GetUniqueLayoutName(CString);

	CList<CLayout*, CLayout*&> layouts;

	// file information
	class FileInformation
	{
	public:

		FileInformation() :
			created_with(CONSTRUCT_VERSION_ID),
			modified(false)
		{
		}

		CString name;
		CString creator;

		CString file_path;

		bool modified;

		int created_with; // construct version id this file was saved with

	} file_information;

	// game settings, not relevant to applications
	class GameInformation
	{
	public:

		GameInformation() :
			fullscreen(false),
			run_without_ps(true),
			override_timedelta(false),
			use_python(false),
			use_motion_blur(false),
			fps_in_caption(false),
			fps_mode(fps_vsync),
			sampler(sample_linear),
			fps(60),
			time_delta_override(60.0),
			motion_blur_steps(0),
			minimum_fps(0),
			text_rendering_mode(trm_antialiased),
			simulate_shaders(ss_no_simulation),
			multisamples(0),
			texture_loading(tl_load_on_layout_start)
		{
		}

		bool fullscreen;
		int run_without_ps; // games run if pixel shaders aren't used
		bool override_timedelta;
		int use_python;
		bool use_motion_blur;
		int fps_in_caption;

		enum
		{
			fps_vsync = 0,
			fps_unlimited,
			fps_fixed
		} fps_mode;

		enum
		{
			sample_point = 0,
			sample_linear
		} sampler;

		enum TextRenderingMode
		{
			trm_aliased = 0,
			trm_antialiased,
			trm_cleartype
		} text_rendering_mode;

		enum SimulateShaders
		{
			ss_no_simulation = 0,
			ss_ps14,
			ss_ps11,
			ss_ps00
		} simulate_shaders;

		int fps; // if fps_mode is fixed
		float time_delta_override;
		int motion_blur_steps;

		float minimum_fps;

		UINT multisamples;

		enum TextureLoading {
			tl_load_on_app_start,
			tl_load_on_layout_start
		} texture_loading;

	} game_information;

	// window information
	class WindowInformation
	{
	public:

		WindowInformation() :
			show_menu(false),
			hide_minimize(false),
			hide_maximize(false),
			window_width(640),
			window_height(480),
			eye_distance(500),
			caption(true),
			minimize_box(false),
			maximize_box(false),
			disable_windows_key(false),
			resizable(resize_disabled)
		{
		}

		int show_menu;
		bool hide_minimize;
		bool hide_maximize;
		int window_width, window_height;
		float eye_distance;

		bool caption;
		bool minimize_box;
		bool maximize_box;

		bool disable_windows_key;

		enum ResizeSetting 
		{
			resize_disabled,
			resize_showmore,
			resize_stretch
		} resizable;

	} window_information;

	// families
	Family*	FindFamilyFromNumber(int fixed);

	list<Family> families;

	// event sheets
	void RemoveEventSheet(int, bool prompt = false);
	EventSheet* FindEventSheetFromNumber(int);

	long event_sheet_unique_identifier;

	list<EventSheet*> event_sheets;

	// object folders
	class ObjectFolder
	{
	public:

		ObjectFolder() :
			tree(NULL)
		{
		}

		CString name;
		HTREEITEM tree;
	};

	CString	GetUniqueObjectFolderName(CString);
	bool ObjectFolderNameExists(CString);
	void RemoveObjectFolder(CString);
	void RenameObjectFolder(CString, CString);

	vector<ObjectFolder> object_folders;

	// resources
	class ResourceInformation
	{
	public:

		ResourceInformation() :
			images_changed(true),
			resource_id(-1)
		{
		}

		CImageResource* FindImageResourceFromNumber(int);
		CAnimation* FindAnimationFromNumber(int, CAnimation** parent = NULL);
		bool DeleteAnimationFromNumber(int);
		bool ResourceNameExists(list<ApplicationResource>*, CString);
		CString GetUniqueResourceName(list<ApplicationResource>*, CString);

		void CopyCollisionMaskToEveryFrameInAnimation(CAnimation* pAnim, CImageResource* pImg);

		list<CAnimation> animations;
		list<CImageResource> images;
		bool images_changed; // used so image data can be cached from build to build

		int resource_id;

		list<BinaryResource> file_resources;
		list<BinaryResource> sound_resources;
		list<BinaryResource> music_resources;
		list<IconResource> icon_resources;
		list<MenuResource> menu_resources;
		list<FontResource> font_resources;

	} resources;

	int CloneAnimation(int, list<CAnimation>* animation_list = NULL);
	int CloneAnimation(CAnimation*, list<CAnimation>* animation_list = NULL);
	int CloneImage(int);
	int CloneImage(CImageResource*);

	// global variables
	class GlobalVariable : public NameComparableCIS 
	{
	public:

		CString value;
		int type;
		int identifier;

		HTREEITEM tree;

		GlobalVariable() : 
			type(0),
			tree(NULL)
		{
		}

		GlobalVariable(CString _name, int _type, CString _value, int _identifier) :
			NameComparableCIS(_name), 
			type(_type), 
			value(_value), 
			identifier(_identifier),
			tree(NULL)
		{
		}

		bool Serialize(CArchive& ar);
	};

	void AddGlobalVariable(GlobalVariable&);

	list<GlobalVariable> global_variables;

	// runtime used
	enum
	{
		rt_application = 0,
		rt_directx
	} runtime;

	// tree items used in the project bar
	class TreeItems
	{
	public:

		HTREEITEM application;

		HTREEITEM layouts;
		HTREEITEM event_sheets;

		HTREEITEM folders;

		HTREEITEM global_variables;

		HTREEITEM files;
		HTREEITEM sounds;
		HTREEITEM music;
		HTREEITEM fonts;
		HTREEITEM icons;
		HTREEITEM menus;

	} tree_items;

	// object types
	bool ObjectTypeNameExists(CString);
	CString GetUniqueObjectTypeName(CString&);

	// other properties
	bool verifying;




	////////////////////////////////
	// to be deprecated/replaced



	int		reserved1, reserved2, reserved3, reserved4;

	// These ID's are used to create unique identifiers
	int m_ImageFixID;
	int m_AnimationFixID;
	long m_objID;
	long m_appID;
	long m_layerID;

	long m_frameID;

	long m_varID;

	CEditorHeader m_default_header;


	//CLayout*		FindFrameFromNumber(int fixed);

	int				GetLayoutIndexFromID(int id);

	CLayer*			FindLayerFromNumber(int fixed, CLayout** ppLayout);
	CLayer*			FindLayerByName(CString string);

	bool			EventSheetNameExists(CString name);
	CString			GetUniqueEventSheetName(CString startWithThisName);



	GlobalVariable* GetGlobalVariable(int fixed);

	// Add object types to families using this function
	bool			AddObjectTypeToFamily(CObjType* pType, Family* pFamily);

	// Use this when removing a type from a family, or deleting a type, to check if the family is now empty
	// If its the last type in a family, it will prompt, reminding the family will be deleted.
	// The return depends on what they then click on this dialog.
	enum ObjTypeFamilyStatus {
		not_last_type_in_family,			// Not last type in family, dialog wasn't shown
		user_clicked_yes_delete_family,		// Is last type in family, user said yes, delete all references to family
		user_clicked_no,					// Is last type in family, user said no, please dont delete family
		is_last_type_in_family				// Used when prompt = false
	};
	ObjTypeFamilyStatus		PromptIfTypeLastInFamily(CObjType* pType, Family* pFamily, bool prompt = true);

	// Removes a family and all its references from an application - the family MUST be empty first
	// (or CObjType's families vector will still hold pointers to it)
	void					DeleteFamily(Family* pFamily);

	CObjTypeMap				object_types;
	CObjTypeMap				m_DeletedObjectTypes;
	CObjType*				FindObjTypeFromNumber(int oid);	// Will return a tempory ObjectType for families
	void					GetObjectTypes(vector<CObjType*>& types);
	void					GetFamilyObjectTypes(vector<CObjType*>& types);

	CObjType*				FindObjTypeFromDetails(ObjectTypeDetail& details);

	// Containers
	void DeleteContainer(CContainer* ctnr);
	list<CContainer>			m_Containers;

	// Traits used by this application
	vector<CString>				traits;


	// Transitions
	list<CTransition>			m_Transitions;

	// Controls
	vector<BehaviorControl>	m_Controls;

	map<CString, int> m_PythonDistrib;

	CObjType m_System;

	// Saving
	int		m_Backup;

	// Frame stuff
	CD3DDisplay*	m_display;
	int m_display_refcount;
	map<int, int> m_image_to_texture;

	// Texture for layer rendering
	
	// Effect textures
	int iOriginalTexture;
	int iEffectTargets[2];
	int iDisplayTexture;
	int iPicture;
	int iThumbnail;
	int iFont;
	int iLayerTexture;

	//Used for moving objects between applications
	map<CApplication*,   map<int, int> >	m_imagefrom_imageto_map;
	map<CApplication*,   map<int, int> >	m_animfrom_animto_map;
	CApplication*							m_drag_app_from;

	map<int, CObj*> images_to_save;
	map<int, CObj*> animations_to_save;
	KeyCollection globalKeys;		// Edittime application global keys
	bool warnedAboutShaders;		// Flag so warning about unsupported shaders only given once
	
	// temporary, used for param dialog, no need to save it
	CSortStringArray m_sort;

	// To save which properties are expanded
	map<CString, bool> property_expand_state;

	// Used to clone images
	map<int, int> remapImages;
	// Used to clone animations
	map<int, int> remapAnimations;
};