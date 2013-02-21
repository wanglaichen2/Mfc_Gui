// SplitFrm.h : interface of the CSplitFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLITFRM_H__04ACD9A9_79D7_466B_AF83_5B9FCFC90C60__INCLUDED_)
#define AFX_SPLITFRM_H__04ACD9A9_79D7_466B_AF83_5B9FCFC90C60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSplitFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSplitFrame)
public:
	CSplitFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSplitFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CSplitFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITFRM_H__04ACD9A9_79D7_466B_AF83_5B9FCFC90C60__INCLUDED_)
