//--------------------------------------------------------------------
// 文件名:		DrawTool.cpp
// 内  容:		工具类
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"
#include "DrawCli.h"
#include "DrawTool.h"
 
#include "drawdoc.h"
#include "drawvw.h"
#include "object/drawobj.h"
#include "object/DrawRect.h"
#include <assert.h>
 
extern GUIManager* g_pGuiManager;
int g_nDragHandle;

CDrawTool::CDrawTool(eDrawShape drawShape)
{
	m_drawShape = drawShape;
}

//void CDrawTool::OnLButtonDown(CDrawView* pView, UINT nFlags, const CPoint& point)
//{
//	pView->SetCapture();
//	g_pGuiManager->SetHitLast(point);
//	g_pGuiManager->SetHitCurrent(point);
//	g_pGuiManager->SetDownFlages(nFlags);
//}

//void CDrawTool::OnLButtonDblClk(CDrawView* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
//{
//}

//void CDrawTool::OnLButtonUp(CDrawView* /*pView*/, UINT /*nFlags*/, const CPoint& point)
//{
//}

//void CDrawTool::OnMouseMove(CDrawView* /*pView*/, UINT /*nFlags*/, const CPoint& point)
//{
//	g_pGuiManager->SetHitLast(point);
//	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
//}

//void CDrawTool::OnEditProperties(/*CDrawView* pView*/)
//{
//}

void CDrawTool::OnCancel()
{
	g_pGuiManager->SetDrawShape(DRAWSHAPE_TOOL_SELECTION);
	g_pGuiManager->SetSelectMode(SELECTMODE_NONE);
}

////////////////////////////////////////////////////////////////////////////
// CResizeTool


CSelectTool::CSelectTool()
	: CDrawTool(DRAWSHAPE_TOOL_SELECTION)
{
}

/*void CSelectTool::OnLButtonDown(CDrawView* pView, UINT nFlags, const CPoint& point)
{
	
	CPoint local = point;
	pView->ClientToDoc(local);

	CDrawObj* pObj;

	//设置选择模式
	g_pGuiManager->SetSelectMode(SELECTMODE_NONE);

	//已经属于选择状态 Check for resizing (only allowed on single selections)
	if (g_pGuiManager->GetSelectCount() == 1)
	{
		CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
		pObj = *it;

		int DragHandle = pObj->HitTest(local, pView, TRUE);

		g_pGuiManager->SetDragHandle(DragHandle);

		if (g_pGuiManager->GetDragHandle() != 0)
		{
			g_pGuiManager->SetSelectMode(SELECTMODE_SIZE);
		}
	}

	// See if the click was on an object, select and start move if so
	if (g_pGuiManager->GetSelectMode() == SELECTMODE_NONE)
	{
		pObj = g_pGuiManager->ObjectAt(local);

		if (pObj != NULL)
		{
			g_pGuiManager->SetSelectMode(SELECTMODE_MOVE);

			if (!g_pGuiManager->IsSelected(pObj))
				g_pGuiManager->Select(pObj, (nFlags & MK_SHIFT) != 0);

			// Ctrl+Click clones the selection...

			if ((nFlags & MK_CONTROL) != 0)
				g_pGuiManager->CloneSelection();
		}
	}
 
	//选择了背景 Click on background, start a net-selection
	if (g_pGuiManager->GetSelectMode() == SELECTMODE_NONE)
	{
		if ((nFlags & MK_SHIFT) == 0)
			g_pGuiManager->Select(NULL);

		g_pGuiManager->SetSelectMode(SELECTMODE_NETSELECT);

		//以下可以消除选取框
		CClientDC dc(pView);
		CRect rect(point.x, point.y, point.x, point.y);
		rect.NormalizeRect(); //规范化 CRect，以便该高度和宽度是一样的
		dc.DrawFocusRect(rect);
	}

	g_pGuiManager->SetSelectLast(local);

	//CDrawTool::OnLButtonDown(pView, nFlags, point);
}
*/
//void CSelectTool::OnLButtonDblClk(CDrawView* pView, UINT nFlags, const CPoint& point)
//{
//	
//	//if ((nFlags & MK_SHIFT) != 0)
//	//{
//	//	// Shift+DblClk deselects object...
//	//	CPoint local = point;
//	//	pView->ClientToDoc(local);
//	//	CDrawObj* pObj = /*pView->GetDocument()->*/g_pGuiManager->ObjectAt(local);
//	//	if (pObj != NULL)
//	//		g_pGuiManager->Deselect(pObj);
//	//}
//	//else
//	//{
//	//	// "Normal" DblClk opens properties, or OLE server...
//	//	if (g_pGuiManager->m_SelectList.size() == 1)
//	//	{
//	//		CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
//	//		(*it)->OnOpen(pView);
//	//	}
//	//}
//
//	CDrawTool::OnLButtonDblClk(pView, nFlags, point);
//
//}

//void CSelectTool::OnEditProperties(/*CDrawView* pView*/)
//{
//	if (g_pGuiManager->m_SelectList.size() == 1)
//	{
//		CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
//		(*it)->OnEditProperties();
//	}
//}

//void CSelectTool::OnLButtonUp(CDrawView* pView, UINT nFlags, const CPoint& point)
//{
//	////鼠标提起，删除画刷框
//	//if (pView->GetCapture() == pView)
//	//{
//	//	if (g_pGuiManager->GetSelectMode() == SELECTMODE_NETSELECT)
//	//	{
//	//		CClientDC dc(pView);
//	//		CRect rect(g_pGuiManager->GetHitCurrent().x, g_pGuiManager->GetHitCurrent().y, g_pGuiManager->GetHitLast().x, g_pGuiManager->GetHitLast().y);
//	//		rect.NormalizeRect();
//	//		dc.DrawFocusRect(rect);
//
//	//		pView->ClientToDoc(rect);
//	//		g_pGuiManager->SelectWithinRect(rect, TRUE);
//	//	}
//	//	else if (g_pGuiManager->GetSelectMode() != SELECTMODE_NONE)
//	//	{
//	//		pView->GetDocument()->UpdateAllViews(pView);
//	//	}
//	//}
//
//	//CDrawTool::OnLButtonUp(pView, nFlags, point);
//	Painter::GetInstancePtr()->FreeCapture();
//
//	if (point == g_pGuiManager->GetHitCurrent())
//		g_pGuiManager->SetDrawShape(DRAWSHAPE_TOOL_SELECTION);
//	
//}

//void CSelectTool::OnMouseMove(CDrawView* pView, UINT nFlags, const CPoint& point)
//{
//	DrawShape drawshape = g_pGuiManager->GetDrawShape();
//
//	if (pView->GetCapture() != pView)
//	{
//		if (drawshape == DRAWSHAPE_TOOL_SELECTION && g_pGuiManager->m_SelectList.size() == 1)
//		{
//			//鼠标到边缘
//			CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
//			CDrawObj* pObj = (*it);
//
//			CPoint local = point;
//			pView->ClientToDoc(local);
//			int nDragHandle = pObj->HitTest(local, pView, TRUE);
//			if (nDragHandle != 0)
//			{
//				SetCursor(pObj->GetHandleCursor(nDragHandle));
//				return; // bypass CDrawTool
//			}
//		}
//
//		//
//		if (drawshape == DRAWSHAPE_TOOL_SELECTION)
//		{
//			g_pGuiManager->SetHitLast(point);
//			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
//		}
//
//		return;
//	}
//
//	if (g_pGuiManager->GetSelectMode() == SELECTMODE_NETSELECT)
//	{
//		//网络选择，绘制刷新网络选择框
//		CClientDC dc(pView);
//		CRect rect(g_pGuiManager->GetHitLast().x, g_pGuiManager->GetHitLast().y, g_pGuiManager->GetHitCurrent().x, g_pGuiManager->GetHitCurrent().y);
//		rect.NormalizeRect();
//		dc.DrawFocusRect(rect);
//		rect.SetRect(g_pGuiManager->GetHitCurrent().x, g_pGuiManager->GetHitCurrent().y, point.x, point.y);
//		rect.NormalizeRect();
//		dc.DrawFocusRect(rect);//在窗口中绘制鼠标拖曳框
//
//
//		g_pGuiManager->SetHitLast(point);
//		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
//		return;
//	}
//
//	CPoint local = point;
//	pView->ClientToDoc(local);
//	CPoint delta = (CPoint)(local - g_pGuiManager->GetSelectLast());
//
//	CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
//	CDrawObjLists::iterator it_end = g_pGuiManager->m_SelectList.end();
//
//	for (; it != it_end; it++ )
//	{
//		CDrawObj* pObj = *it;
//		CRect position = pObj->m_position;
//
//		if (g_pGuiManager->GetSelectMode() == SELECTMODE_MOVE)
//		{
//			position += delta;
//			pObj->MoveTo(position, pView);
//		}
//		else if (g_pGuiManager->GetDragHandle() != 0)
//		{
//			pObj->MoveHandleTo(g_pGuiManager->GetDragHandle(), local, pView);
//		}
//	}
//
//	g_pGuiManager->SetSelectLast(local);
//
//	if (g_pGuiManager->GetSelectMode() == SELECTMODE_SIZE && drawshape == DRAWSHAPE_TOOL_SELECTION)
//	{
//		g_pGuiManager->SetHitLast(point);
//
//		it = g_pGuiManager->m_SelectList.begin();
//		SetCursor((*it)->GetHandleCursor(g_pGuiManager->GetDragHandle()));
//		return; // bypass CDrawTool
//	}
//
//	g_pGuiManager->SetHitLast(point);
//
//	if (g_pGuiManager->GetDrawShape() == DRAWSHAPE_TOOL_SELECTION)
//		CDrawTool::OnMouseMove(pView, nFlags, point);
//}

////////////////////////////////////////////////////////////////////////////
// CRectTool (does rectangles, round-rectangles, and ellipses)

CRectTool::CRectTool(eDrawShape drawShape)
	: CDrawTool(drawShape)
{
}

//void CRectTool::OnLButtonDown(CDrawView* pView, UINT nFlags, const CPoint& point)
//{
//
//	//CPoint local = point;
//	//pView->ClientToDoc(local);
//
//	//CDrawRect* pObj = new CDrawRect(CRect(local, CSize(0, 0)));
//	//switch (m_drawShape)
//	//{
//	//default:
//	//	assert(false); // unsuported shape!
//
//	//case DrawShape_TOOL_RECT:
//	//	pObj->m_nShape = CDrawRect::rectangle;
//	//	break;
//
//	//case DrawShape_TOOL_ROUNDRECT:
//	//	pObj->m_nShape = CDrawRect::roundRectangle;
//	//	break;
//
//	//case DrawShape_TOOL_ELLIPSE:
//	//	pObj->m_nShape = CDrawRect::ellipse;
//	//	break;
//
//	//case DrawShape_TOOL_LINE:
//	//	pObj->m_nShape = CDrawRect::line;
//	//	break;
//	//case DrawShape_TOOL_CIRCLE:
//	//	pObj->m_nShape = CDrawRect::circle;
//	//	break;
//	//}
//	//g_pGuiManager->AddBack(pObj);
//	//pObj->m_pDocument = pView->GetDocument();
//	//pView->GetDocument()->SetModifiedFlag();
//
//	//g_pGuiManager->Select(pObj);
//
//	////g_pGuiManager->SetSelectMode(SELECTMODE_SIZE);
//	//g_pGuiManager->SetRectMode(RECTMODE_SIZE);
//
//	//g_pGuiManager->SetDragHandle(DRAGHANDLE_RIGHT_BOTTOM);
//
//	//g_pGuiManager->SetSelectLast(local);
//
//	//CDrawTool::OnLButtonDown(pView, nFlags, point);
//}

//void CRectTool::OnLButtonDblClk(CDrawView* pView, UINT nFlags, const CPoint& point)
//{
//	CDrawTool::OnLButtonDblClk(pView, nFlags, point);
//}
//
//void CRectTool::OnLButtonUp(CDrawView* pView, UINT nFlags, const CPoint& point)
//{  
//	//CSelectTool * pSelectTool = (CSelectTool *)g_pGuiManager->FindTool(DRAWSHAPE_TOOL_SELECTION);
//	//if (point == g_pGuiManager->GetHitCurrent())
//	//{
//	//	// Don't create empty objects...
//	//	CDrawObjLists::iterator it = g_pGuiManager->m_SelectList.begin();
//	//	CDrawObj *pObj = *it;
//	//	g_pGuiManager->Remove(pObj);
//	//	pView->GetDocument()->SetModifiedFlag();
//
//	//	//每个视图都删除
//	//	//POSITION pos = pView->GetDocument()->GetFirstViewPosition();
//	//	//while (pos != NULL)
//	//	//	((CDrawView*)pView->GetDocument()->GetNextView(pos))->Remove(pObj);
//
//	//	g_pGuiManager->RemoveSelection(pObj);
//
//	//	pObj->Remove();
//	//	pSelectTool->OnLButtonDown(pView, nFlags, point); // try a select!
//	//	pView->SetCapture();
//	//}
//
//	//pSelectTool->OnLButtonUp(pView, nFlags, point);
//}
//
//void CRectTool::OnMouseMove(CDrawView* pView, UINT nFlags, const CPoint& point)
//{
//
//	//CSelectTool * pSelectTool = (CSelectTool *)g_pGuiManager->FindTool(DRAWSHAPE_TOOL_SELECTION);
//	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
//	int itest = 1;
//	g_pGuiManager->innerSelectMouseMove(NULL, NULL, NULL, itest, nFlags, point);
//	//pSelectTool->OnMouseMove(pView, nFlags, point);
//}
