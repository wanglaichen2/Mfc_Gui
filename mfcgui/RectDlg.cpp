// RectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawCli.h"
#include "RectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRectDlg property page

IMPLEMENT_DYNCREATE(CRectDlg, CPropertyPage)

CRectDlg::CRectDlg() : CPropertyPage(CRectDlg::IDD)
{
	//{{AFX_DATA_INIT(CRectDlg)
	m_bNoFill = FALSE;
	m_penSize = 0;
	m_nOrd = 0;
	//}}AFX_DATA_INIT
}

CRectDlg::~CRectDlg()
{
}

void CRectDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRectDlg)
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cTyp);
	DDX_Control(pDX, IDC_SPIN1, m_SpinCtrl);
	DDX_Check(pDX, IDC_NOFILL, m_bNoFill);
	DDX_Text(pDX, IDC_WEIGHT, m_penSize);
	DDV_MinMaxUInt(pDX, m_penSize, 0, 100);
	DDX_Text(pDX, IDC_EDIT_ORDER, m_nOrd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRectDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CRectDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_FILLCOLOR_BUTTON, OnFillcolorButton)
	ON_BN_CLICKED(IDC_LINECOLOR_BUTTON, OnLinecolorButton)
	ON_CBN_CLOSEUP(IDC_COMBO_TYPE, OnCloseupComboType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRectDlg message handlers

BOOL CRectDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_SpinCtrl.SetRange(0, 100);
	m_SpinCtrl.SetBase(10);
	m_SpinCtrl.SetPos(1);

 	m_cTyp.SetCurSel(m_nTyp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRectDlg::OnPaint() 
{
   CPaintDC dc(this); // device context for painting
   CBrush brush;
	if (!brush.CreateSolidBrush(m_FillColor))
		return;

   CBrush* pOldBrush;
   CPen pen;
   if (!pen.CreatePen(PS_INSIDEFRAME,4,m_LineColor))
		return;

   CPen* pOldPen;
   CWnd* phwnd;
   CDC*  pdc;
   CRect rect;

   phwnd=GetDlgItem(IDC_LINECOLOR);
   pdc=phwnd->GetDC();
   pOldPen = pdc->SelectObject(&pen);
   phwnd->GetClientRect(&rect);
   pdc->MoveTo(rect.left+2,rect.top+(rect.bottom-rect.top)/2);
   pdc->LineTo(rect.right-3,rect.top+(rect.bottom-rect.top)/2);
   pdc->SelectObject(pOldPen);

   phwnd=GetDlgItem(IDC_FILLCOLOR);
   pdc=phwnd->GetDC();
   pOldBrush = pdc->SelectObject(&brush);
   phwnd->GetClientRect(&rect);
   pdc->Rectangle(rect);
   pdc->SelectObject(pOldBrush);	
}

void CRectDlg::OnFillcolorButton() 
{
	CColorDialog cdlg;
	if (cdlg.DoModal()!=IDOK)
		return;
	m_FillColor = cdlg.GetColor();
	
}

void CRectDlg::OnLinecolorButton() 
{
	CColorDialog cdlg;
	if (cdlg.DoModal()!=IDOK)
		return;
	m_LineColor = cdlg.GetColor();
	
}

void CRectDlg::OnCloseupComboType() 
{
	// TODO: Add your control notification handler code here
	m_nTyp=m_cTyp.GetCurSel();
}
