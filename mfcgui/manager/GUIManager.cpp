//--------------------------------------------------------------------
// �ļ���:		GUIManager.cpp
// ��  ��:		gui
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
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


	m_SelectMode = SELECTMODE_NONE; //Ĭ��ѡ��ģʽʱΪ��
	m_DragHandle = 0;				//���Ĭ��ָʾ

	Release();
}

bool GUIManager::init()
{
	return true;
}

bool GUIManager::Release()
{
	//m_ToolMap.clear();

	m_ObjectList.clear();//��ʼ����ն����б�
	m_objectcount = 0;

	m_SelectList.clear();//��ʼ��ѡ������б�
	m_selectcount = 0;

	m_CopyList.clear();//��ʼ�����������б�
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
	//����
	rect.InflateRect(1, 1); // handles CDrawOleObj objects

	pwnd->InvalidateRect(rect, false);//��һ���ֽ������»���
}*/

bool GUIManager::RefrshSelectionList()
{
	//ѡ��֮��ʵʱˢ��
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
	//ѡ��֮��ʵʱˢ��
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
		//˫��ȡ��ѡ��
		if ((nFlags & MK_SHIFT) != 0)
		{
			if (GetSelectCount() < 1)
			{
				return false;
			}

			// Shift+DblClk ȡ��ѡ�����
			CPoint local = point;
			//pwnd->ClientToDoc(local);
			Painter::GetInstancePtr()->ClientToDoc(local);
			CDrawObj* pObj = ObjectAt(local);
			if (pObj != NULL)
			{
				Deselect(pObj);
			}
		}
		//˫��������
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

	//SetDownFlages(nFlags);//δ��Ч
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
		//����ѡ��ģʽ
		SetSelectMode(SELECTMODE_NONE);

		CDrawObj* pObj;

		//�Ѿ�����ѡ��״̬ Check for resizing (only allowed on single selections)
		if (GetSelectCount() == 1)
		{
			CDrawObjLists::iterator it = m_SelectList.begin();
			pObj = *it;
			assert(pObj);

			int nDragHandle = pObj->HitTest(point/* local �޸��������*/, true);

			SetDragHandle(nDragHandle);//���������״

			if (GetDragHandle() != DRAGHANDLE_NONE)
			{
				SetSelectMode(SELECTMODE_SIZE);
			}
		}

		// ѡ��һ��Ŀ�꣬�ƶ�
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

		//ѡ���˱��� 
		if (GetSelectMode() == SELECTMODE_NONE)
		{
			if ((nFlags & MK_SHIFT) == 0)
				Select(NULL);

			SetSelectMode(SELECTMODE_NETSELECT);

			//���¿�������ѡȡ��
			CClientDC clientD(pwnd);
			CRect rect(point.x, point.y, point.x, point.y);
			rect.NormalizeRect(); //�淶�� CRect���Ա�ø߶ȺͿ����һ����
			clientD.DrawFocusRect(rect);
		}
	}
	else
	{
		//���û�ͼģʽ
		SetRectMode(RECTMODE_NONE); 

		IControl* pObj ;//= new CDrawRect(CRect(point/* local �޸��������*/, CSize(0, 0)));
		switch (m_drawShape)
		{
		default:
			assert(false); // unsuported shape!
		case DrawShape_TOOL_RECT:
			pObj = new CDrawRect(CRect(point/* local �޸��������*/, CSize(0, 0)));
			break;
		case DrawShape_TOOL_ROUNDRECT:
			pObj = new CDrawRoundRectangle(CRect(point/* local �޸��������*/, CSize(0, 0)));
			break;
		case DrawShape_TOOL_ELLIPSE:
			pObj = new CDrawEllipse(CRect(point/* local �޸��������*/, CSize(0, 0)));
			break;
		case DrawShape_TOOL_LINE:
			pObj = new CDrawLine(CRect(point/* local �޸��������*/, CSize(0, 0)));
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

	//��ѡ�����򲻱�ʱ���л�ģʽ
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

	//ѡ��ģʽ
	if(DRAWSHAPE_TOOL_SELECTION == m_drawShape)
	{
		//�������ɾ����ˢ��
		if (Painter::GetInstancePtr()->GetCapture() == Painter::GetInstancePtr()->GetCWnd())
		{
			//����ѡ��ģʽ
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
			// �������ն���
			CDrawObjLists::iterator it = m_SelectList.begin();
			CDrawObj *pObj = *it;
			assert(pObj);

			RemoveSelection(pObj);
			Remove(pObj);

			//ÿ����ͼ��ɾ��
			//POSITION pos = pView->GetDocument()->GetFirstViewPosition();
			//while (pos != NULL)
			//	((CDrawView*)pView->GetDocument()->GetNextView(pos))->Remove(pObj);

			pObj->Remove();

			pwnd->GetDocument()->SetModifiedFlag();

			innerLButtonDown(nFlags, point);
		}

		//������ͼģʽ��һ�в���
		SetRectMode(RECTMODE_NONE);
	}

	innerLButtonUp(nFlags, point);

	return true;
}


bool GUIManager::MsgMouseMove(void* hwnd, unsigned int param1, unsigned int param2, 
				  int& result, UINT nFlags, CPoint point)
{
	//ѡ��ģʽ
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

	//����ѡ�񣬻���ˢ������ѡ���
	CClientDC clientD(pwnd);
	CRect rect(GetHitLast().x, GetHitLast().y, GetHitCurrent().x, GetHitCurrent().y);
	rect.NormalizeRect();
	clientD.DrawFocusRect(rect);
	rect.SetRect(GetHitCurrent().x, GetHitCurrent().y, point.x, point.y);
	rect.NormalizeRect();
	clientD.DrawFocusRect(rect);//�ڴ����л��������ҷ��

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
			//SetCursor(pObj->GetHandleCursor(GetDragHandle()));//�ı����
			pObj->MoveHandleTo(GetDragHandle(), point/* local �޸��������*/, pwnd);
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
			
			//��굽��Ե
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
			int nDragHandle = pObj->HitTest(point/* local �޸��������*/, TRUE);
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

	//ѡ��ģʽ��ͼģʽ�����Ա���
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