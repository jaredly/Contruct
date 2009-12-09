
// Represents an action in the event sheet editor.
class CEditorAction
{
public:

	~CEditorAction();
	void Init();
	bool Serialize(CArchive &ar,CChronoEventEditor* Ed);
	void Render(CRect &Pos, CDC &dc, bool EventSelected, CChronoEventEditor& Ed, int renderType, bool bEnabled = true);

	void Remove(bool Animate, CChronoEventEditor* pEd);

	BOOL			m_select_default;
	ChronoBoolMap	m_ED_select;
	BOOL&			m_select(CChronoEventEditor* Ed = NULL);
	bool			m_Family;
	CString			m_FamilyName;

	CRect			m_rect_default; // Display Rectangle in respect to window
	ChronoRectMap	m_ED_rect;
	CRect&			m_rect(CChronoEventEditor* Ed = NULL);

	int oid;					// object ID
	int mid;					// movement ID
	int actID;					// action ID

	CString m_Readable;

	// Used where applicable
	int DLLIndex;
	int m_MovementDLLIndex;

	CString m_Text;
	ImageVector m_imgs;

	CString m_Object;
	CString m_Script;
	
	ParamVector params;	// Parameters
	CChronoAnimation m_Anim;
	bool m_visible; // Is false if its not physically displayed - not sure if this is right though

	bool ValidToDisplay(CApplication* pApp);
	bool Valid(CApplication* pApp);
	bool m_Valid;
	bool m_bEnabled;
};