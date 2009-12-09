#pragma once

class CApplication;
class CLayer;

class InitValue
{
public:
	CString value;
	int ID;
	InitValue(CString _value, int _ID)  : value(_value), ID(_ID) {}
};

class CObj
{
public:
    // Constructor/Destructor
    CObj();
	~CObj();

    // Attributes
    void SetPos(double x, double y, BOOL bNotFirst = TRUE);
    void SetSize(double w, double h);
	void SetAngle(double a);
	void SetOrigin(double x, double y);
    void SetDLLIdentifier(DWORD id);
    float GetX(); 
    float GetY();
    float GetW();
    float GetH();
	float GetA();
	float GetOriginX();
	float GetOriginY();
    DWORD GetDLLIdentifier();
    long GetInstanceID();
    long GetGlobalID();
	void SetLocked(BOOL bLocked);
	BOOL GetLocked();
	void SetVisible(BOOL bVisible);
	BOOL GetVisible();
	void UpdateGroupInfo(BOOL bUpdateGroupPos = TRUE, float zoom = 1.0);
	CPoint GetRealPos(CPoint pt);

	CObjectRectangle GetObjectRect(CLayoutEditor* ed, bool useoinfo = false);
	// Get object type in a level
    CObjType* GetObjectType(CApplication* pApplication);

	// Map to store which properties are expanded
	map<CString, bool> expandSave;

	vector<InitValue> m_values;	// Private value initial values (corresponding to CObjType m_PrivateValues)

    // Serialize data
	bool Serialize(CArchive& ar);

	void WriteToLeveLObjects(CLayout* frame);

public:
	// Edit data
	editInfo * const editObject; // always points to objectInfo, since we got rid of xtra's dubious new/deletes
	editInfo objectInfo;

	// Frame settings
	BOOL m_bLocked, m_bVisible, m_AllowSize, m_Draw;

	// Angles
	BOOL m_ShowAngles;

	BOOL bSerializeData;

	CString m_tempName;

	// Grouping
	BOOL m_bInGroup, m_bIsGroup, m_bGroupOpen;
	CObj *m_group;
	CObjList m_groupObjects;

	float   zoomed_objectX,
		    zoomed_objectY,
		    zoomed_objectWidth,
		    zoomed_objectHeight,
			zoomed_angle,
			zoomed_originx,
			zoomed_originy;

	bool DuplicateSerialize;
};