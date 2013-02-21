//--------------------------------------------------------------------
// 文件名:		DrawVw.cpp
// 内  容:		MFC视图
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"
#include <afxpriv.h>
#include "DrawCli.h"

#include "DrawDoc.h"
#include "object/CntrItem.h"
#include "DrawVw.h"
#include "resource.h"

#include "object/DrawObj.h"
#include "object/DrawOleObj.h"
#include "drawtool.h"
#include "object/CntrItem.h"
#include "mainfrm.h"
#include "manager/GUIManager.h"
#include "manager/Painter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern Data_Bus  Demo_Data; 

// private clipboard format (list of Draw objects)
CLIPFORMAT CDrawView::m_cfDraw = (CLIPFORMAT)
#ifdef _MAC
	::RegisterClipboardFormat(_T("DCLI"));
#else
	::RegisterClipboardFormat(_T("MFC Draw Sample"));
#endif
CLIPFORMAT CDrawView::m_cfObjectDescriptor = NULL;

/////////////////////////////////////////////////////////////////////////////
// CDrawView

IMPLEMENT_DYNCREATE(CDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CDrawView, CScrollView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CDrawView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)

	//几个选择toolbar
	ON_COMMAND(ID_DRAW_SELECT, OnDrawSelect)
	ON_COMMAND(ID_DRAW_RECT, OnDrawRect)
	ON_COMMAND(ID_DRAW_CIRCLE, OnDrawCircle)
	ON_COMMAND(ID_DRAW_ELLIPSE, OnDrawEllipse)
	ON_COMMAND(ID_DRAW_LINE, OnDrawLine)
	ON_COMMAND(ID_DRAW_ROUNDRECT, OnDrawRoundrect)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)

	//更新toolbar
	ON_UPDATE_COMMAND_UI(ID_DRAW_SELECT, OnUpdateDrawSelect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, OnUpdateDrawRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CIRCLE, OnUpdateDrawCircle)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_OBJECT_MOVEBACK, OnObjectMoveback)
	ON_COMMAND(ID_OBJECT_MOVEFORWARD, OnObjectMoveforward)
	ON_COMMAND(ID_OBJECT_MOVETOBACK, OnObjectMovetoback)
	ON_COMMAND(ID_OBJECT_MOVETOFRONT, OnObjectMovetofront)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEBACK, OnUpdateObjectMoveback)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEFORWARD, OnUpdateObjectMoveforward)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOBACK, OnUpdateObjectMovetoback)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOFRONT, OnUpdateObjectMovetofront)
	ON_COMMAND(ID_OBJECT_FILLCOLOR, OnObjectFillcolor)
	ON_COMMAND(ID_OBJECT_LINECOLOR, OnObjectLinecolor)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_FILLCOLOR, OnUpdateObjectFillcolor)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_LINECOLOR, OnUpdateObjectLinecolor)
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
	ON_COMMAND(ID_VIEW_SHOWOBJECTS, OnViewShowobjects)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWOBJECTS, OnUpdateViewShowobjects)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)

	ON_MESSAGE(WM_DEMO_DATA, DemoData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawView construction/destruction

CDrawView::CDrawView()
{
//	m_pSelection = NULL;
	m_bGrid = FALSE; 
	m_gridColor = RGB(0, 0, 128);
	//m_bActive = FALSE;
	Painter::GetInstancePtr()->SetActive(false);

	if (g_pGuiManager)
	{
	}
	else
	{
		g_pGuiManager = new GUIManager();
	}
}

CDrawView::~CDrawView()
{
}

BOOL CDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView drawing

void CDrawView::OnDraw(CDC* pDC)
{	
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;

	// only paint the rect that needs repainting
	CRect client;
	pDC->GetClipBox(client);				//检取当前裁剪边界的最近限定矩形的大小
	CRect rect = client;
	DocToClient(rect);
	rect.NormalizeRect();

	TRACE("CDrawView::OnDraw\n");

	if (!pDC->IsPrinting())//不加会闪屏
	{
		// draw to offscreen bitmap for fast looking repaints
		if (dc.CreateCompatibleDC(pDC))				//创建一个与指定设备兼容的内存设备上下文环境
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))//创建与指定的设备环境相关的设备兼容的位图
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// offset origin more because bitmap is just piece of the whole drawing
				dc.OffsetViewportOrg(-rect.left, -rect.top);//相对于当前视区起点坐标修正视区起点坐标
				pOldBitmap = dc.SelectObject(&bitmap);		//选择一对象到指定的设备上下文环境中
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);//指定GDI将分配给下画笔应用程序选择到设备上下文的原点

				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);				//创建了一个新的剪切区域，该区域是当前剪切区域和一个特定矩形的交集
			}
		}
	}

	// paint background
	CBrush brush;
	if (!brush.CreateSolidBrush(pDoc->GetPaperColor()))
		return;

	brush.UnrealizeObject();				//重置一个逻辑调色板
	pDrawDC->FillRect(client, &brush);		//用指定的画刷填充矩形


	Painter::GetInstancePtr()->SetCDC(pDrawDC);
	Painter::GetInstancePtr()->SetCWnd(this);

	if (!pDC->IsPrinting() && m_bGrid)
		DrawGrid(pDrawDC);

	g_pGuiManager->Draw();

	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
	}
}

void CDrawView::OnInitialUpdate()
{
//	CScrollView::OnInitialUpdate();

  	CSize size = GetDocument()->GetSize();
	CClientDC dc(NULL);
	size.cx = MulDiv(size.cx, dc.GetDeviceCaps(LOGPIXELSX), 100);
	size.cy = MulDiv(size.cy, dc.GetDeviceCaps(LOGPIXELSY), 100);
	SetScrollSizes(MM_TEXT, size);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView printing

BOOL CDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDrawView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CScrollView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

//BOOL CDrawView::IsSelected(const CObject* pDocItem) const
//{
///*
//	// The implementation below is adequate if your selection consists of
//	//  only CDrawItem objects.  To handle different selection
//	//  mechanisms, the implementation here should be replaced.
//	// TODO: implement this function that tests for a selected OLE client item
//	return pDocItem == m_pSelection;
//*/
//	CDrawObj* pDrawObj = (CDrawObj*)pDocItem;
//	//if (pDocItem->IsKindOf(RUNTIME_CLASS(CDrawItem)))
//	//	pDrawObj = ((CDrawItem*)pDocItem)->m_pDrawObj;
//
//	CDrawObjLists * pselectlist = const_cast<CDrawObjLists*>(&g_pGuiManager->m_SelectList);
//	CDrawObjLists::iterator it = pselectlist->begin();
//	CDrawObjLists::iterator it_end = pselectlist->end();
//	for(; it != it_end; it++)
//	{
//		if (*it  == pDrawObj)
//		{
//			return true;
//		}
//	}
//	return false;
//
//	/*
//	return m_selection.Find(pDrawObj) != NULL;
//	*/
//
//}

void CDrawView::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CDrawItem object.
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	// First create the C++ object
	CDrawOleObj* pObj = new CDrawOleObj(GetInitialPosition());
	ASSERT_VALID(pObj);
	CDrawItem* pItem = new CDrawItem(GetDocument(), pObj);
	ASSERT_VALID(pItem);
	pObj->m_pClientItem = pItem;

	// Now create the OLE object/item
	TRY
	{
		if (!dlg.CreateItem(pObj->m_pClientItem))
			AfxThrowMemoryException();

		// add the object to the document
		/*GetDocument()->*/g_pGuiManager->AddBack(pObj);
		pObj->m_pDocument = GetDocument();
		GetDocument()->SetModifiedFlag();

		// try to get initial presentation data
		pItem->UpdateLink();
		pItem->UpdateExtent();

		// if insert new object -- initially show the object
		if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);
	}
	CATCH_ALL(e)
	{
		// clean up item
		pItem->Delete();
		pObj->m_pClientItem = NULL;
		/*GetDocument()->*/g_pGuiManager->Remove(pObj);
		GetDocument()->SetModifiedFlag(TRUE);
		pObj->Remove();

		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH_ALL

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.
void CDrawView::OnCancelEditCntr()
{
	// Close any in-place active item on this view.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CDrawView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CScrollView::OnSetFocus(pOldWnd);
}

void CDrawView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView diagnostics

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDrawDoc* CDrawView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawDoc)));
	return (CDrawDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawView message handlers

void CDrawView::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(ID_POPUP_MENU));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

void CDrawView::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CDrawView::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);


	TRACE("rect1 is :%d  %d  %d  %d\n",rect.left, rect.right, rect.top, rect.bottom);
	//rect.NormalizeRect();
	TRACE("rect2 is :%d  %d  %d  %d\n",rect.left, rect.right, rect.top, rect.bottom);

	//ASSERT(rect.left <= rect.right);
	//ASSERT(rect.bottom <= rect.top);
}

void CDrawView::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CDrawView::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	//rect.NormalizeRect();
}

void CDrawView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!Painter::GetInstancePtr()->GetActive())
		return;

	int itest = 1;
	g_pGuiManager->MsgLButtonDown(NULL, NULL, NULL, itest, nFlags, point);
}

void CDrawView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!Painter::GetInstancePtr()->GetActive())
		return;

	int itest = 1;
	g_pGuiManager->MsgLButtonUp(NULL, NULL, NULL, itest, nFlags, point);
}

void CDrawView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!Painter::GetInstancePtr()->GetActive())
		return;

	int itest = 1;
	g_pGuiManager->MsgMouseMove(NULL, NULL, NULL, itest, nFlags, point);

//  m_wndStatusBar.SetPaneText(1,LPCTSTR("abcdefgh"),TRUE);
}

void CDrawView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (!Painter::GetInstancePtr()->GetActive())
		return;

	int itest = 1;
	g_pGuiManager->MsgLButtonDblclk(NULL, NULL, NULL, itest, nFlags, point);
}
void CDrawView::DrawGrid(CDC* pDC)
{
   	CDrawDoc* pDoc = GetDocument();

	COLORREF oldBkColor = pDC->SetBkColor(pDoc->GetPaperColor());

	CRect rect;
	rect.left = -pDoc->GetSize().cx / 2;
	rect.top = -pDoc->GetSize().cy / 2;
	rect.right = rect.left + pDoc->GetSize().cx;
	rect.bottom = rect.top + pDoc->GetSize().cy;

	// Center lines
	CPen penDash;
	penDash.CreatePen(PS_DASH, 1, m_gridColor);
	CPen* pOldPen = pDC->SelectObject(&penDash);

	pDC->MoveTo(0, rect.top);
	pDC->LineTo(0, rect.bottom);
	pDC->MoveTo(rect.left, 0);
	pDC->LineTo(rect.right, 0);

	// Major unit lines
	CPen penDot;
	penDot.CreatePen(PS_DOT, 1, m_gridColor);
	pDC->SelectObject(&penDot);

	//for (int x = rect.left / 100 * 100; x < rect.right; x += 100)
	for (int x = rect.left / 50 * 50; x < rect.right; x += 50)
	{
		if (x != 0)
		{
			pDC->MoveTo(x, rect.top);
			pDC->LineTo(x, rect.bottom);
		}
	}

	//for (int y = rect.top / 100 * 100; y < rect.bottom; y += 100)
	for (int y = rect.top / 50 * 50; y < rect.bottom; y += 50)
	{
		if (y != 0)
		{
			pDC->MoveTo(rect.left, y);
			pDC->LineTo(rect.right, y);
		}
	}

	// Outlines
	CPen penSolid;
	penSolid.CreatePen(PS_SOLID, 1, m_gridColor);
	pDC->SelectObject(&penSolid);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.left, rect.top);

	pDC->SelectObject(pOldPen);
	pDC->SetBkColor(oldBkColor);

}
void CDrawView::SetPageSize(CSize size)
{
   	CClientDC dc(NULL);
	size.cx = MulDiv(size.cx, dc.GetDeviceCaps(LOGPIXELSX), 100);
	size.cy = MulDiv(size.cy, dc.GetDeviceCaps(LOGPIXELSY), 100);
	SetScrollSizes(MM_TEXT, size);
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_WINDOW, NULL);

}


void CDrawView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
 	CScrollView::OnPrepareDC(pDC, pInfo);
	return;

	// mapping mode is MM_ANISOTROPIC
	// these extents setup a mode similar to MM_LOENGLISH
	// MM_LOENGLISH is in .01 physical inches
	// these extents provide .01 logical inches
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),
	pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(100, -100);

	// set the origin of the coordinate system to the center of the page
	CPoint ptOrg;
	ptOrg.x = GetDocument()->GetSize().cx / 2;
	ptOrg.y = GetDocument()->GetSize().cy / 2;

	// ptOrg is in logical coordinates
	pDC->OffsetWindowOrg(-ptOrg.x,ptOrg.y);
}

CRect CDrawView::GetInitialPosition()
{
	CRect rect(10, 10, 10, 10);
	ClientToDoc(rect);
	return rect;
}
//当视图被激活为活动视图,或由活动转为非活动时,调用该函数通知视图。基类的实现只是设置该视图为焦点。
//用来显示所画图形
void CDrawView::OnActivateView(BOOL bActivate,CView* pActiveView,CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActiveView, pDeactiveView);

	// invalidate selections when active status changes
	if ((BOOL)Painter::GetInstancePtr()->GetActive() != bActivate)
	{
		if (bActivate)  // if becoming active update as if active
			//m_bActive = bActivate;
			Painter::GetInstancePtr()->SetActive(bActivate == TRUE);

		if (g_pGuiManager->GetSelectCount() > 0)
			//OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
			g_pGuiManager->RefrshSelectionList();
		//m_bActive = bActivate;
		Painter::GetInstancePtr()->SetActive(bActivate == TRUE);
	}
}

void CDrawView::OnEditProperties() 
{
	g_pGuiManager->MsgEditProperties();
	//if (g_pGuiManager->GetSelectCount() == 1 && g_pGuiManager->GetDrawShape() == DrawShape_TOOL_SELECTION)
	//{
	//	CDrawTool* pTool = g_pGuiManager->FindTool(g_pGuiManager->GetDrawShape());
	//	ASSERT(pTool != NULL);
	//	pTool->OnEditProperties(this);
	//}
}

void CDrawView::OnObjectMoveback() 
{
	g_pGuiManager->MsgObjectMoveback();
}

void CDrawView::OnObjectMoveforward() 
{
	g_pGuiManager->MsgObjectMoveforward();
}

void CDrawView::OnObjectMovetoback() 
{
	g_pGuiManager->MsgObjectMovetoback();
}

void CDrawView::OnObjectMovetofront() 
{
	g_pGuiManager->MsgObjectMovetofront();
}

void CDrawView::OnDrawSelect() 
{
	g_pGuiManager->SetDrawShape(DRAWSHAPE_TOOL_SELECTION);
	g_pGuiManager->SetSelectMode(SELECTMODE_NONE);
}
void CDrawView::OnDrawRect() 
{
	g_pGuiManager->SetDrawShape(DrawShape_TOOL_RECT);
}
void CDrawView::OnDrawCircle() 
{
	g_pGuiManager->SetDrawShape(DrawShape_TOOL_CIRCLE);	
}
void CDrawView::OnDrawEllipse() 
{
	g_pGuiManager->SetDrawShape(DrawShape_TOOL_ELLIPSE);
}
void CDrawView::OnDrawLine() 
{
	g_pGuiManager->SetDrawShape(DrawShape_TOOL_LINE);
}
void CDrawView::OnDrawRoundrect() 
{
	g_pGuiManager->SetDrawShape(DrawShape_TOOL_ROUNDRECT);
}

//更新toolbar按钮状态
void CDrawView::OnUpdateDrawSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(g_pGuiManager->GetDrawShape() == DRAWSHAPE_TOOL_SELECTION);
}
void CDrawView::OnUpdateDrawRect(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(g_pGuiManager->GetDrawShape() == DrawShape_TOOL_RECT);
}
void CDrawView::OnUpdateDrawCircle(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(g_pGuiManager->GetDrawShape() == DrawShape_TOOL_CIRCLE);
}


void CDrawView::OnUpdateObjectMoveback(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetSelectCount() == 1);
}
void CDrawView::OnUpdateObjectMoveforward(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetSelectCount() == 1);
}
void CDrawView::OnUpdateObjectMovetoback(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetSelectCount() == 1);
}
void CDrawView::OnUpdateObjectMovetofront(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetSelectCount() == 1);
}
void CDrawView::OnUpdateObjectFillcolor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetSelectCount() >= 1);
}
void CDrawView::OnUpdateObjectLinecolor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetSelectCount() >= 1);
}

void CDrawView::OnObjectFillcolor() 
{
	CColorDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	COLORREF color = dlg.GetColor();

	g_pGuiManager->MsgObjectFillcolor(color);
}

void CDrawView::OnObjectLinecolor() 
{
	CColorDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	COLORREF color = dlg.GetColor();

	g_pGuiManager->MsgObjectLinecolor(color);
}

void CDrawView::OnViewGrid() 
{
	m_bGrid = !m_bGrid;
	Invalidate(FALSE);
}

void CDrawView::OnUpdateViewGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bGrid);
	
}

void CDrawView::OnViewShowobjects() 
{
	CDrawOleObj::c_bShowItems = !CDrawOleObj::c_bShowItems;
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_OLE_ITEMS, NULL);
}

void CDrawView::OnUpdateViewShowobjects(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CDrawOleObj::c_bShowItems);
}

void CDrawView::OnEditClear() 
{
	g_pGuiManager->MsgClearSelect();

	//GetDocument()->UpdateAllViews(NULL, HINT_DELETE_SELECTION, &m_selection);
	////OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	//g_pGuiManager->RefrshSelectionList();

	//// now remove the selection from the document
	//CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
	//CDrawObjLists::iterator it_end = g_pGuiManager->m_SelectList.end();
	//for (; it != it_end; )
	//{
	//	CDrawObj* pObj = *it;
	//	g_pGuiManager->m_SelectList.erase(it++);
	//	g_pGuiManager->Remove(pObj);
	//	GetDocument()->SetModifiedFlag(true);
	//	pObj->Remove();
	//}
	//g_pGuiManager->m_SelectList.clear();
}

void CDrawView::OnEditCopy() 
{
	g_pGuiManager->MsgCopySelect();

	//ASSERT_VALID(this);
	//ASSERT(m_cfDraw != NULL);

	//// Create a shared file and associate a CArchive with it
	//CSharedFile file;
	//CArchive ar(&file, CArchive::store);

	//// Serialize selected objects to the archive
	//m_selection.Serialize(ar);
	//ar.Close();

	//COleDataSource* pDataSource = NULL;
	//TRY
	//{
	//	pDataSource = new COleDataSource;
	//	// put on local format instead of or in addation to
	//	pDataSource->CacheGlobalData(m_cfDraw, file.Detach());

	//	// if only one item and it is a COleClientItem then also
	//	// paste in that format

	//	CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
	//	CDrawObj* pDrawObj = *it;
	//	if (g_pGuiManager->m_SelectList.size() == 1 &&
	//		pDrawObj->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
	//	{
	//		CDrawOleObj* pDrawOle = (CDrawOleObj*)pDrawObj;
	//		pDrawOle->m_pClientItem->GetClipboardData(pDataSource, FALSE);
	//	}
	//	pDataSource->SetClipboard();
	//}
	//CATCH_ALL(e)
	//{
	//	delete pDataSource;
	//	THROW_LAST();
	//}
	//END_CATCH_ALL
	
}

void CDrawView::OnEditCut() 
{
	g_pGuiManager->MsgCopySelect();
	g_pGuiManager->MsgClearSelect();

	//OnEditCopy();
	//OnEditClear();
}

void CDrawView::OnEditPaste() 
{
	//COleDataObject dataObject;
	//dataObject.AttachClipboard();

	// invalidate current selection since it will be deselected
	//OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	//g_pGuiManager->RefrshSelectionList();
	//g_pGuiManager->m_SelectList.clear();

	g_pGuiManager->MsgEditPaste();
	//if (dataObject.IsDataAvailable(m_cfDraw))
	//{
	//	PasteNative(dataObject);

	//	// now add all items in m_selection to document

	//	CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
	//	CDrawObjLists::iterator it_end = g_pGuiManager->m_SelectList.end();
	//	for (; it != it_end; it++)
	//	{
	//		CDrawObj* pObj = *it;
	//		if (NULL != pObj)
	//		{
	//			g_pGuiManager->AddBack(pObj);
	//			pObj->m_pDocument = GetDocument();
	//			GetDocument()->SetModifiedFlag();
	//		}
	//	}
	//}
	//else
	//	PasteEmbedded(dataObject, GetInitialPosition().TopLeft() );

	GetDocument()->SetModifiedFlag();

	// invalidate new pasted stuff
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_SELECTION, &m_selection);
	
}
//菜单选择所有
void CDrawView::OnEditSelectAll() 
{
	g_pGuiManager->MsgSelectAll();
}

void CDrawView::PasteNative(COleDataObject& dataObject)
{
	// get file refering to clipboard data
	CFile* pFile = dataObject.GetFileData(m_cfDraw);
	if (pFile == NULL)
		return;

	// connect the file to the archive
	CArchive ar(pFile, CArchive::load);
	TRY
	{
		ar.m_pDocument = GetDocument(); // set back-pointer in archive

		// read the selection
		m_selection.Serialize(ar);
	}
	CATCH_ALL(e)
	{
		ar.Close();
		delete pFile;
		THROW_LAST();
	}
	END_CATCH_ALL

	ar.Close();
	delete pFile;
}
/*
void CDrawView::PasteEmbedded(COleDataObject& dataObject, CPoint point)
{
	BeginWaitCursor();

	// paste embedded
	CDrawOleObj* pObj = new CDrawOleObj(GetInitialPosition());
	ASSERT_VALID(pObj);
	CDrawItem* pItem = new CDrawItem(GetDocument(), pObj);
	ASSERT_VALID(pItem);
	pObj->m_pClientItem = pItem;

	TRY
	{
		if (!pItem->CreateFromData(&dataObject) &&
			!pItem->CreateStaticFromData(&dataObject))
		{
			AfxThrowMemoryException();      // any exception will do
		}

		// add the object to the document
		g_pGuiManager->AddBack(pObj);
		pObj->m_pDocument = GetDocument();
		GetDocument()->SetModifiedFlag();

		g_pGuiManager->m_SelectList.push_front(pObj);
		g_pGuiManager->m_selectcount++;

		ClientToDoc( point );
		pObj->MoveTo( CRect( point, pObj->m_extent ), this );

		// try to get initial presentation data
		pItem->UpdateLink();
		pItem->UpdateExtent();
	}
	CATCH_ALL(e)
	{
		// clean up item
		pItem->Delete();
		pObj->m_pClientItem = NULL;
		g_pGuiManager->Remove(pObj);
		GetDocument()->SetModifiedFlag(TRUE);
		pObj->Remove();

		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH_ALL

	EndWaitCursor();
}
*/
void CDrawView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetSelectCount() > 0);
}

void CDrawView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetSelectCount() > 0);
}

void CDrawView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetSelectCount() > 0);
}

void CDrawView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetCopyCount() > 0);
}

void CDrawView::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_pGuiManager->GetObjectCount() != 0);
}
/*
void CDrawView::OnUpdate(CView* , LPARAM lHint, CObject* pHint)
{

	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // redraw entire window
		Invalidate(FALSE);
		break;

	case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		g_pGuiManager->InvalObj((CDrawObj*)pHint);
		break;

	case HINT_UPDATE_SELECTION: // an entire selection has changed
		{
			//选中之后实时刷新
			g_pGuiManager->RefrshSelectionList();
		}
		break;

	case HINT_DELETE_SELECTION: // an entire selection has been removed
		if (pHint != &m_selection)
		{

			CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
			CDrawObjLists::iterator it_end = g_pGuiManager->m_SelectList.end();
			for (; it != it_end; it++)
			{
				CDrawObj* pObj = *it;
				if (NULL != pObj)
				{
					g_pGuiManager->InvalObj(pObj);
					g_pGuiManager->RemoveSelection(pObj); // remove it from this view's selection
				}
			}
		}
		break;

	case HINT_UPDATE_OLE_ITEMS:
		{
			CDrawObjLists* pDrawObjLists = g_pGuiManager->GetObjLists();
			CDrawObjLists::iterator it = pDrawObjLists->begin();
			CDrawObjLists::iterator it_end = pDrawObjLists->end();
			for (it; it != it_end; it++)
			{
				CDrawObj* pObj_temp = *it;
				if (pObj_temp != NULL)
				{
					if (pObj_temp->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
						g_pGuiManager->InvalObj(pObj_temp);
				}
			}
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}
}
*/
LRESULT CDrawView::DemoData(WPARAM wParam, LPARAM lParam)
{
	Data_Bus  *pDemo_Data  = (Data_Bus*)lParam; 
	COLORREF   color;

	CDrawObjLists* pDrawObjLists = g_pGuiManager->GetObjLists();
	CDrawObjLists::iterator it = pDrawObjLists->begin();
	CDrawObjLists::iterator it_end = pDrawObjLists->end();
	for (it; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		if (pObj != NULL)
		{
			if (pObj->m_nType == 1) // DI/DO状态量
			{
				color= (pDemo_Data->DI_DO[pObj->m_nOrder]==0)?RGB(0,255,0):RGB(255,0,0);
				pObj->SetFillColor(color);
			}
		}
	}

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->SetModifiedFlag(FALSE);

	return 0;
}

int CDrawView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->vHwnd=this->m_hWnd;
	return 0;
}
