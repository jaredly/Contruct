
// Represents a condition in an event.
class CEditorCondition
{
public:
	~CEditorCondition();
	int oid;					// object ID
	int mid;					// movement id
	int cndID;					// condition ID	
	bool m_Negate;				// Negated
	bool m_Family;
	CString m_FamilyName;
	ParamVector params;			// Parameters

	CString m_Readable;

	int     m_BehaviorDLLIndex;

	bool    m_visible; // Is false if its not to be shown - this only happens if isValid is false and it prevents serialization!
	
	BOOL    m_select_default;
	ChronoBoolMap m_ED_select;
	BOOL& m_select(CChronoEventEditor* Ed = NULL);


	CRect   m_rect_default; // Display Rectangle in respect to window
	ChronoRectMap m_ED_rect;
	CRect& m_rect(CChronoEventEditor* Ed = NULL);


	CString m_Text;
	ImageVector m_imgs;

	CString m_Object;
	CString m_Script;

	bool ValidToDisplay(CApplication* pApp);
	bool Valid(CApplication* pApp);
	int DLLIndex; // DLL index for object

public:
	void Init();

	bool Serialize(CArchive &ar,CChronoEventEditor* Ed);

	void Render(CRect &Pos, CDC &dc, bool EventSelected,CChronoEventEditor& Ed, bool bBookmarked = false, bool bEnabled = true);

	void Remove(bool Animate, CChronoEventEditor* pEd);
	bool IsTrigger(CApplication* pApp);
	bool IsLoop(CApplication* app);

	CChronoAnimation m_Anim;

	bool m_Valid;
	bool m_bEnabled;
};