#pragma once

class CAnimation
{
public:
	CAnimation();

	bool operator==(const CAnimation& other) const
	{
		if (m_FixedID == other.m_FixedID) return true;
		return false;
	}

	void operator=(const CAnimation &other) 
	{
		// Check for self assignment
		if (other.m_FixedID == m_FixedID)
			return;

		application		= other.application;
		m_FixedID		= other.m_FixedID;

		m_Name			= other.m_Name;
		m_Description	= other.m_Description;

		m_Repeat		= other.m_Repeat;
		m_RepeatTo		= other.m_RepeatTo;
		m_bForever		= other.m_bForever;
		m_bPingPong		= other.m_bPingPong;
		m_Speed			= other.m_Speed;
		m_Expanded		= other.m_Expanded;
		m_IsAngle		= other.m_IsAngle;
		m_Angle			= other.m_Angle;

		m_AllowRemove			= other.m_AllowRemove;
		m_AllowSubanimations	= other.m_AllowSubanimations;
		m_AllowAddRemoveFrame	= other.m_AllowAddRemoveFrame;
		m_AllowRenameFrame		= other.m_AllowRenameFrame;
		m_AllowAutoButton		= other.m_AllowAutoButton;

		tag = other.tag;

		// Copy frame times
		m_FrameTimes.clear();

		for (int i = 0; i < other.m_FrameTimes.size(); i++)
			m_FrameTimes.push_back(other.m_FrameTimes[i]);

		// Copy images
		m_Images.clear();

		for (int i = 0; i < other.m_Images.size(); i++)
			m_Images.push_back(other.m_Images[i]);
	}
	void SerializeFromOtherApplicationStep(CApplication* pApp);

	CApplication*	application;
	int				m_FixedID;

	CString			m_Name;
	CString			m_Description;

	int				m_Repeat;
	int				m_RepeatTo;
	BOOL			m_bForever;
	BOOL			m_bPingPong;

	float				m_Speed;

	bool m_Expanded; // Used by the IDE's animation bar

	int tag;

	bool m_IsAngle; // Okay worst type of object orientated programming ever :(
	float m_Angle;
	bool IsAngle() { return m_IsAngle; }

	vector<int> m_Images;
	vector<float> m_FrameTimes;
	
	list<CAnimation> m_SubAnimations;

	CAnimation* GetAngle(int angle);

	// IDE settings
	bool m_AllowRemove;
	bool m_AllowSubanimations;
	bool m_AllowAddRemoveFrame;
	bool m_AllowRenameFrame;
	bool m_AllowAutoButton;

	bool Serialize(CArchive& ar);
	void GetImagesToSave(map<int, CObj*>& images);

	bool supportsFrames();

	HTREEITEM m_Tree;
};