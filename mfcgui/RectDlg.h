#if !defined(AFX_RECTDLG_H__21BACE39_882A_4385_8669_B46E530F474C__INCLUDED_)
#define AFX_RECTDLG_H__21BACE39_882A_4385_8669_B46E530F474C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRectDlg dialog

class CRectDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CRectDlg)

// Construction
public:
	COLORREF m_LineColor;
	COLORREF m_FillColor;
    int m_nTyp;
	CRectDlg();
	~CRectDlg();

// Dialog Data
	//{{AFX_DATA(CRectDlg)
	enum { IDD = IDD_PROP_RECT };
	CComboBox	m_cTyp;
	CSpinButtonCtrl	m_SpinCtrl;
	BOOL    m_bNoFill;
	UINT    m_penSize;
	int		m_nOrd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnFillcolorButton();
	afx_msg void OnLinecolorButton();
	afx_msg void OnCloseupComboType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECTDLG_H__21BACE39_882A_4385_8669_B46E530F474C__INCLUDED_)
