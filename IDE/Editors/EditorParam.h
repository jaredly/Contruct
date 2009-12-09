
// A parameter in an event.
class CEditorParam
{
public:
	CEditorParam() : valid(true), m_type(0), m_reserved(""), oid(-1) {}
	~CEditorParam();
	vector<Token> ttokens;
	bool valid;	// is a parameter is invalid, this is false;

	CString CreateParamString(CChronoEventEditor* Ed);
	CString CreateParamString(CExport* Ed);
	CString CreateParamString(CApplication* pApp);

	CString CreateFormattedParamString(EventSheetEditor *pEd, CEditorAction* pAction, CEditorCondition* pCondition);

	int m_type;

	CString m_reserved;
	vector<CRect> displayrects;

	long oid;	// OID of object owning action/condition

	void Serialize( CArchive& ar, CChronoEventEditor* Ed);
	void CreateFromString(CString src,CApplication* pApp, int type, CString reserved, bool exporting = false);

	bool CharPartOfName(char i);

};