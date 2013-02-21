// SplitFrm.cpp : implementation of the CSplitFrame class
//

#include "stdafx.h"
#include "DrawCli.h"

#include "SplitFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame

IMPLEMENT_DYNCREATE(CSplitFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CSplitFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSplitFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame construction/destruction

CSplitFrame::CSplitFrame()
{
	// TODO: add member initialization code here
	
}

CSplitFrame::~CSplitFrame()
{
}

BOOL CSplitFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSplitFrame diagnostics

#ifdef _DEBUG
void CSplitFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CSplitFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame message handlers
