//--------------------------------------------------------------------
// 文件名:		GUIManager.cpp
// 内  容:		gui
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"

//#include "../DrawCli.h"
//#include "../drawdoc.h"
//#include "../object/drawobj.h"
//#include "../object/DrawRect.h"

#include "../object/DrawObj.h"
#include "GUIManager.h"
#include <assert.h>
//#include "../drawdoc.h"
//#include "../drawvw.h"
#include "../DrawTool.h"
#include "../drawvw.h"
#include "../DrawDoc.h"
#include "../object/IControl.h"
#include "../object/DrawRect.h"
#include "../object/DrawCircle.h"
#include "../object/DrawLine.h"
#include "../object/DrawEllipse.h"
#include "../object/DrawRoundRectangle.h"


GUIManager* g_pGuiManager;

GUIManager::GUIManager()
{
	m_drawShape = DRAWSHAPE_TOOL_SELECTION;

	m_SelectTools = new CSelectTool();
	m_lineTool = new CRectTool(DrawShape_TOOL_LINE);
	m_rectTool = new CRectTool(DrawShape_TOOL_RECT);
	m_roundRectTool = new CRectTool(DrawShape_TOOL_ROUNDRECT);
	m_ellipseTool = new CRectTool(DrawShape_TOOL_ELLIPSE);
	m_circleTool = new CRectTool(DrawShape_TOOL_CIRCLE);

	m_ToolMap.insert(std::map<int, CDrawTool*>::value_type(DRAWSHAPE_TOOL_SELECTION, m_SelectTools));
	m_ToolMap.insert(std::map<int, CDrawTool*>::value_type(DrawShape_TOOL_LINE, m_lineTool));
	m_ToolMap.insert(std::map<int, CDrawTool*>::value_type(DrawShape_TOOL_RECT, m_rectTool));
	m_ToolMap.insert(std::map<int, CDrawTool*>::value_type(DrawShape_TOOL_ROUNDRECT, m_roundRectTool));
	m_ToolMap.insert(std::map<int, CDrawTool*>::value_type(DrawShape_TOOL_ELLIPSE, m_ellipseTool));
	m_ToolMap.insert(std::map<int, CDrawTool*>::value_type(DrawShape_TOOL_CIRCLE, m_circleTool));


	m_SelectMode = SELECTMODE_NONE; //默认选择模式时为空
	m_DragHandle = 0;				//鼠标默认指示

	Release();
}

bool GUIManager::init()
{
	return true;
}

bool GUIManager::Release()
{
	//m_ToolMap.clear();

	m_ObjectList.clear();//初始化清空对象列表
	m_objectcount = 0;

	m_SelectList.clear();//初始化选择对象列表
	m_selectcount = 0;

	m_CopyList.clear();//初始化拷贝对象列表
	m_copycount = 0;

	return true;
}
bool GUIManager::shut()
{
	Release();

	delete m_SelectTools;
	delete m_lineTool;
	delete m_rectTool;
	delete m_roundRectTool;
	delete m_ellipseTool;
	delete m_circleTool;


	return true;
}

void GUIManager::Add(CDrawObj *pObj)
{
	m_ObjectList.push_back(pObj);
	m_objectcount++;
}
void GUIManager::AddBack(CDrawObj *pObj)
{
	m_ObjectList.push_back(pObj);
	m_objectcount++;
}
void GUIManager::AddFront(CDrawObj *pObj)
{
	m_ObjectList.push_front(pObj);
	m_objectcount++;
}



void GUIManager::Remove(CDrawObj* pObj)
{
	CDrawObjLists::iterator it = m_ObjectList.begin();
	CDrawObjLists::iterator it_end = m_ObjectList.end();
	for (it; it != it_end; )
	{
		CDrawObj* pObj_temp = *it;
		if (pObj_temp == pObj)
		{
			m_ObjectList.erase(it++);
			m_objectcount--;
		}
		else
		{
			it++;
		}
	}
}

void GUIManager::RemoveSelection(CDrawObj* pObj)
{
	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObjLists::iterator it_end = m_SelectList.end();
	for (it; it != it_end; )
	{
		CDrawObj* pObj_temp = *it;
		if (pObj_temp == pObj)
		{
			m_SelectList.erase(it++);
		}
		else
		{
			it++;
		}
	}
	m_selectcount = 0;
}
void GUIManager::CloneSelection()
{
	CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	if (!pwnd)
	{
		return;
	}

	CDrawObjLists::iterator it = m_SelectList.begin();

	CDrawObj* pObj = *it;
	assert(pObj);

	CDrawObj* temp = pObj->Clone();
	if (NULL != temp)
	{
		CDrawDoc* pDoc = pwnd->GetDocument();
		assert(pDoc);
		AddFront(temp);
		//temp->m_pDocument = pDoc;
		//pDoc->SetModifiedFlag();

		//temp->m_pDocument = pDoc;
		pDoc->SetModifiedFlag();
	}

}
bool GUIManager::IsSelected(const CDrawObj* pDrawObj)
{	
	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObjLists::iterator it_end = m_SelectList.end();
	for(; it != it_end; it++)
	{
		if (*it  == pDrawObj)
		{
			return true;
		}
	}
	return false;
}
/*
void GUIManager::InvalObj(CDrawObj *pObj)
{
	CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	if (!pwnd)
	{
		return;
	}

	CRect rect = pObj->m_position;
	pwnd->DocToClient(rect);
	if (pwnd->m_bActive && IsSelected(pObj))
	{
		rect.left -= 4;
		rect.top -= 5;
		rect.right += 5;
		rect.bottom += 4;
	}
	//增大
	rect.InflateRect(1, 1); // handles CDrawOleObj objects

	pwnd->InvalidateRect(rect, false);//这一部分将被重新绘制
}*/

bool GUIManager::RefrshSelectionList()
{
	//选中之后实时刷新
	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObjLists::iterator it_end = m_SelectList.end();
	for (; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		if (NULL != pObj)
		{
			pObj->InvalObj();
		}
	}
	return true;
}

bool GUIManager::RefrshAllObjList()
{
	//选中之后实时刷新
	CDrawObjLists::iterator it = m_ObjectList.begin();
	CDrawObjLists::iterator it_end = m_ObjectList.end();
	for (; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		if (NULL != pObj)
		{
			pObj->InvalObj();
		}
	}
	return true;
}
void GUIManager::Draw()
{
	CDC * pcdc = Painter::GetInstancePtr()->GetCDC();
	if (!pcdc)
	{
		return;
	}

	CDrawObjLists::iterator it = m_ObjectList.begin();
	CDrawObjLists::iterator it_end = m_ObjectList.end();
	for (it; it != it_end; it++)
	{
		CDrawObj* pObj = *it;

		pObj->Draw();

		if (Painter::GetInstancePtr()->GetActive() && !pcdc->IsPrinting() && IsSelected(pObj))
		{
			pObj->DrawTracker(pcdc, CDrawObj::selected);
		}
	}

}
CDrawTool* GUIManager::FindTool(eDrawShape drawShape)
{
	std::map<int, CDrawTool*>::iterator it_find = m_ToolMap.find(drawShape);
	if (it_find != m_ToolMap.end())
	{
		return it_find->second;
	}

	return NULL;
}

/*
bool GUIManager::AddTool(CDrawTool *drawtool)
{
	m_tools.AddTail(drawtool);
	return true;
}
*/

bool GUIManager::MsgLButtonDblclk(void* hwnd, unsigned int param1, unsigned int param2, 
								  int& result, UINT nFlags, CPoint point)
{
	//CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	//if (!pwnd)
	//{
	//	return false;
	//}

	eDrawShape drawshape = g_pGuiManager->GetDrawShape();
	if (drawshape == DRAWSHAPE_TOOL_SELECTION)
	{
		//双击取消选择
		if ((nFlags & MK_SHIFT) != 0)
		{
			if (GetSelectCount() < 1)
			{
				return false;
			}

			// Shift+DblClk 取消选择对象
			CPoint local = point;
			//pwnd->ClientToDoc(local);
			Painter::GetInstancePtr()->ClientToDoc(local);
			CDrawObj* pObj = ObjectAt(local);
			if (pObj != NULL)
			{
				Deselect(pObj);
			}
		}
		//双击打开属性
		else
		{
			// "Normal" DblClk opens properties, or OLE server...
			if (GetSelectCount() == 1)
			{
				CDrawObjLists::iterator it = m_SelectList.begin();
				CDrawObj *pObj = *it;
				assert(pObj);
				pObj->OnEditProperties();
			}
		}
	}

	return true;
}
bool GUIManager::innerLButtonDown(UINT nFlags, CPoint point)
{	
	Painter::GetInstancePtr()->SetCapture();

	SetHitLast(point);
	SetHitCurrent(point);

	//SetDownFlages(nFlags);//未有效
	return true;
}
bool GUIManager::MsgLButtonDown(void* hwnd, unsigned int param1, unsigned int param2, 
								int& result, UINT nFlags, CPoint point)
{
	CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	if (!pwnd)
	{
		return false;
	}

	CPoint local = point;
	//pwnd->ClientToDoc(local);

	Painter::GetInstancePtr()->ClientToDoc(local);

	if (m_drawShape == DRAWSHAPE_TOOL_SELECTION)
	{
		//设置选择模式
		SetSelectMode(SELECTMODE_NONE);

		CDrawObj* pObj;

		//已经属于选择状态 Check for resizing (only allowed on single selections)
		if (GetSelectCount() == 1)
		{
			CDrawObjLists::iterator it = m_SelectList.begin();
			pObj = *it;
			assert(pObj);

			int nDragHandle = pObj->HitTest(point/* local 修改坐标测试*/, true);

			SetDragHandle(nDragHandle);//设置鼠标形状

			if (GetDragHandle() != DRAGHANDLE_NONE)
			{
				SetSelectMode(SELECTMODE_SIZE);
			}
		}

		// 选择一个目标，移动
		if (GetSelectMode() == SELECTMODE_NONE)
		{
			pObj = ObjectAt(local);

			if (pObj != NULL)
			{
				SetSelectMode(SELECTMODE_MOVE);

				if (!IsSelected(pObj))
					Select(pObj, (nFlags & MK_SHIFT) != 0);

				// Ctrl+Click clones the selection...

				if ((nFlags & MK_CONTROL) != 0)
					CloneSelection();
			}
		}

		//选择了背景 
		if (GetSelectMode() == SELECTMODE_NONE)
		{
			if ((nFlags & MK_SHIFT) == 0)
				Select(NULL);

			SetSelectMode(SELECTMODE_NETSELECT);

			//以下可以消除选取框
			CClientDC clientD(pwnd);
			CRect rect(point.x, point.y, point.x, point.y);
			rect.NormalizeRect(); //规范化 CRect，以便该高度和宽度是一样的
			clientD.DrawFocusRect(rect);
		}
	}
	else
	{
		//设置绘图模式
		SetRectMode(RECTMODE_NONE); 

		IControl* pObj ;//= new CDrawRect(CRect(point/* local 修改坐标测试*/, CSize(0, 0)));
		switch (m_drawShape)
		{
		default:
			assert(false); // unsuported shape!
		case DrawShape_TOOL_RECT:
			pObj = new CDrawRect(CRect(point/* local 修改坐标测试*/, CSize(0, 0)));
			break;
		case DrawShape_TOOL_ROUNDRECT:
			pObj = new CDrawRoundRectangle(CRect(point/* local 修改坐标测试*/, CSize(0, 0)));
			break;
		case DrawShape_TOOL_ELLIPSE:
			pObj = new CDrawEllipse(CRect(point/* local 修改坐标测试*/, CSize(0, 0)));
			break;
		case DrawShape_TOOL_LINE:
			pObj = new CDrawLine(CRect(point/* local 修改坐标测试*/, CSize(0, 0)));
			break;
		case DrawShape_TOOL_CIRCLE:
			pObj = new CDrawCircle(CRect(point, CSize(0, 0)));
			break;
		}
		AddBack(pObj);

		pObj->m_pDocument = pwnd->GetDocument();
		pwnd->GetDocument()->SetModifiedFlag();

		Select(pObj);

		//SetSelectMode(SELECTMODE_SIZE);
		SetRectMode(RECTMODE_SIZE);

		SetDragHandle(DRAGHANDLE_RIGHT_BOTTOM);

	}

	//SetSelectLast(local);
	innerLButtonDown(nFlags, point);

	return true;
}

bool GUIManager::innerLButtonUp(UINT nFlags, CPoint point)
{
	Painter::GetInstancePtr()->FreeCapture();

	//当选择区域不变时，切换模式
	if (point == GetHitCurrent())
	{
		SetDrawShape(DRAWSHAPE_TOOL_SELECTION);
		SetSelectMode(SELECTMODE_NONE);
	}

	return true;
}
bool GUIManager::MsgLButtonUp(void* hwnd, unsigned int param1, unsigned int param2, 
	int& result, UINT nFlags, CPoint point)
{
	CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	if (!pwnd)
	{
		return false;
	}

	//选择模式
	if(DRAWSHAPE_TOOL_SELECTION == m_drawShape)
	{
		//鼠标提起，删除画刷框
		if (Painter::GetInstancePtr()->GetCapture() == Painter::GetInstancePtr()->GetCWnd())
		{
			//网络选择模式
			if (GetSelectMode() == SELECTMODE_NETSELECT)
			{
				CClientDC clientD(Painter::GetInstancePtr()->GetCWnd());
				CRect rect(GetHitCurrent().x, GetHitCurrent().y, GetHitLast().x, GetHitLast().y);
				rect.NormalizeRect();
				clientD.DrawFocusRect(rect);

				//pwnd->ClientToDoc(rect);
				Painter::GetInstancePtr()->ClientToDoc(rect);

				SelectWithinRect(rect, true);
			}
			else if (GetSelectMode() != SELECTMODE_NONE)
			{
				pwnd->GetDocument()->UpdateAllViews(pwnd);
			}
		}
	}
	else
	{
		if (point == GetHitCurrent())
		{
			// 不创建空对象
			CDrawObjLists::iterator it = m_SelectList.begin();
			CDrawObj *pObj = *it;
			assert(pObj);

			RemoveSelection(pObj);
			Remove(pObj);

			//每个视图都删除
			//POSITION pos = pView->GetDocument()->GetFirstViewPosition();
			//while (pos != NULL)
			//	((CDrawView*)pView->GetDocument()->GetNextView(pos))->Remove(pObj);

			pObj->Remove();

			pwnd->GetDocument()->SetModifiedFlag();

			innerLButtonDown(nFlags, point);
		}

		//结束绘图模式的一切操作
		SetRectMode(RECTMODE_NONE);
	}

	innerLButtonUp(nFlags, point);

	return true;
}


bool GUIManager::MsgMouseMove(void* hwnd, unsigned int param1, unsigned int param2, 
				  int& result, UINT nFlags, CPoint point)
{
	//选择模式
	if(DRAWSHAPE_TOOL_SELECTION != m_drawShape)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	}

	innerSelectMouseMove(nFlags, point);

	return true;
}
bool GUIManager::innerSelectMouseMoveNetSelect(UINT nFlags, CPoint point)
{
	CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	if (!pwnd)
	{
		return false;
	}

	//网络选择，绘制刷新网络选择框
	CClientDC clientD(pwnd);
	CRect rect(GetHitLast().x, GetHitLast().y, GetHitCurrent().x, GetHitCurrent().y);
	rect.NormalizeRect();
	clientD.DrawFocusRect(rect);
	rect.SetRect(GetHitCurrent().x, GetHitCurrent().y, point.x, point.y);
	rect.NormalizeRect();
	clientD.DrawFocusRect(rect);//在窗口中绘制鼠标拖曳框

	SetHitLast(point);
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return true;
}

bool GUIManager::innerSelectMouseMoveMove(UINT nFlags, CPoint point)
{	
	CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	if (!pwnd)
	{
		return false;
	}
	
	CPoint local = point;
	CPoint selectlast = GetHitLast();//GetSelectLast();

	CPoint delta = (CPoint)(local - selectlast);

	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObjLists::iterator it_end = m_SelectList.end();

	for (; it != it_end; it++ )
	{
		CDrawObj* pObj = *it;
		CRect rect = pObj->m_position;
		if (GetSelectMode() == SELECTMODE_MOVE)
		{
			rect += delta;
			pObj->MoveTo(rect, pwnd);
		}
	}

	//SetSelectLast(local);
	SetHitLast(point);

	if (GetDrawShape() == DRAWSHAPE_TOOL_SELECTION)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	return true;
}
bool GUIManager::innerMouseMoveSize(UINT nFlags, CPoint point)
{
	CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	if (!pwnd)
	{
		return false;
	}

	if (GetDragHandle() != DRAGHANDLE_NONE)
	{
		CDrawObjLists::iterator it = m_SelectList.begin();
		CDrawObjLists::iterator it_end = m_SelectList.end();

		for (; it != it_end; it++ )
		{
			CDrawObj* pObj = *it;
			//CRect position = pObj->m_position;
			//SetCursor(pObj->GetHandleCursor(GetDragHandle()));//改变鼠标
			pObj->MoveHandleTo(GetDragHandle(), point/* local 修改坐标测试*/, pwnd);
		}
	}

	//SetSelectLast(local);
	SetHitLast(point);

	if (m_drawShape == DRAWSHAPE_TOOL_SELECTION)
	{
		CDrawObjLists::iterator it = m_SelectList.begin();
		SetCursor((*it)->GetHandleCursor(GetDragHandle()));
		return true; // bypass CDrawTool
	}

	return true;
}
bool GUIManager::innerSelectMouseMove(UINT nFlags, CPoint point)
{
	if (Painter::GetInstancePtr()->GetCapture() != Painter::GetInstancePtr()->GetCWnd())
	{
		if (m_drawShape == DRAWSHAPE_TOOL_SELECTION && m_selectcount == 1)
		{
			
			//鼠标到边缘
			CDrawObjLists::iterator it = m_SelectList.begin();
			CDrawObj* pObj = (*it);


			//CPoint local = point;
			//pwnd->ClientToDoc(local);

			//pObj = ObjectAt(local);
			//if (pObj == NULL)
			//{
			//	return false;
			//}

			//TRACE("point is :%d--%d\n",point.x, point.y);
			int nDragHandle = pObj->HitTest(point/* local 修改坐标测试*/, TRUE);
			if (nDragHandle != 0)
			{
				SetCursor(pObj->GetHandleCursor(nDragHandle));
				return true; // bypass CDrawTool
			}
		}

		//
		if (m_drawShape == DRAWSHAPE_TOOL_SELECTION)
		{
			SetHitLast(point);
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}

		return true;
	}

	if (GetSelectMode() == SELECTMODE_NETSELECT)
	{
		innerSelectMouseMoveNetSelect(nFlags, point);
	}

	//选择模式绘图模式均可以变形
	if ( GetSelectMode() == SELECTMODE_SIZE
		|| GetRectMode() == RECTMODE_SIZE)
	{
		innerMouseMoveSize(nFlags, point);
	}

	if (GetSelectMode() == SELECTMODE_MOVE)
	{
		innerSelectMouseMoveMove(nFlags, point);
	}

	return true;
}