// DrawCli.h : main header file for the DRAWCLI application
//

#if !defined(AFX_DRAWCLI_H__D8C91276_DF05_4257_A506_C1232A8E8595__INCLUDED_)
#define AFX_DRAWCLI_H__D8C91276_DF05_4257_A506_C1232A8E8595__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <afxtempl.h>       // drawcli uses collection class templates

/////////////////////////////////////////////////////////////////////////////
// CDrawApp:
// See DrawCli.cpp for the implementation of this class
//

#if !defined(MAX_AI_AO)
#define MAX_AI_AO 1024
#endif

#if !defined(MAX_DI_DO)
#define MAX_DI_DO 1024
#endif

class Data_Bus{
public:
	float  AI_AO[MAX_AI_AO];
    BOOL    DI_DO[MAX_DI_DO];
};

#define WM_DEMO_DATA WM_USER+1

class CDrawApp : public CWinApp
{
public:
	CDrawApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDrawApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWCLI_H__D8C91276_DF05_4257_A506_C1232A8E8595__INCLUDED_)
