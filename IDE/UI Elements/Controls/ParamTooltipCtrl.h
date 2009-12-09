#pragma once

#include <vector>
using namespace std;

// CParamTooltipCtrl

class CParamTooltipCtrl : public CWnd
{
	DECLARE_DYNAMIC(CParamTooltipCtrl)

	struct METHOD {
		CString			m_sName;
		vector<CString>	m_setParams;

		METHOD()
		{}

		METHOD(const CString& sName, const vector<CString>& setParams)
			: m_sName(sName)
			, m_setParams(setParams)
		{}

		METHOD(const METHOD& other)
		{
			*this = other;
		}

		const METHOD& operator=(const METHOD& other)
		{
			m_sName = other.m_sName;
			m_setParams = other.m_setParams;
			return *this;
		}
	};

	typedef vector<METHOD> CSetMethod;

	COLORREF		m_cBackground;
	COLORREF		m_cText;
	CSetMethod		m_setMethods;
	int				m_nCurParam;
	int				m_nCurMethod;
	CPoint			m_ptOldClient;
public:
	/// <summary>
	///		Construct the object with default colors (extracted from the system colors)
	/// </summary>
	CParamTooltipCtrl();

	/// <summary>
	///		Construct the object with colors defined with the user
	/// </summary>
	/// <param name="cBackground">Background color</param>
	/// <param name="cText">Text color</param>
	CParamTooltipCtrl(COLORREF cBackground, COLORREF cText);

	/// <summary>
	///		Create the control
	/// </summary>
	/// <param name="pParentWnd">Parent Window for the control</param>
	BOOL Create(CWnd* pParentWnd);


	/// <summary>
	///		Add a new method description to the control
	/// </summary>
	/// <param name="sMethod">Name of the method</param>
	/// <param name="setParams">Vector of CStrings with the name of all the params for the method sMethod</param>
	void AddMethod(const CString& sMethod, const vector<CString>& setParams);

	/// <summary>
	///		Flush all the methods and it parameters
	/// </summary>
	void FlushMethods();

	/// <summary>
	///		Set the current method to show the next call to the method ShowTooltip
	/// </summary>
	///	<remarks>
	///		Call to this method reset the current parameter to 0
	///	</remarks>
	/// <param name="nCurMethod">Index of the method to show</param>
	void SetCurMethod(UINT nCurMethod);

	/// <summary>
	///		Set the current parameter to show the next call to the method ShowTooltip
	/// </summary>
	/// <param name="nCurParam">Index of the parameter to show</param>
	void SetCurParam(UINT nCurParam);

	/// <summary>
	///		Show the tooltip (displaying the method and parameter specified with call to SetCurMethod and SetCurParam) in a
	///		specific coordinates.
	/// </summary>
	/// <param name="ptClient">Client coordinates (of the parent window) where to show the tooltip.</param>
	void ShowTooltip(const CPoint& ptClient);

	/// <summary>
	///		Show the previous method.
	/// </summary>
	void ShowPrevMethod();

	/// <summary>
	///		Show the next method.
	/// </summary>
	void ShowNextMethod();

	/// <summary>
	///		Emphasize the previous parameter.
	/// </summary>
	void ShowPrevParam();

	/// <summary>
	///		Emphasize the next parameter.
	/// </summary>
	void ShowNextParam();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	CSize Draw( CDC& dc, bool bDraw );
};


