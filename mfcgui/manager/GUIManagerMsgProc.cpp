//--------------------------------------------------------------------
// 文件名:		GUIManagerMsgProc.cpp
// 内  容:		gui消息
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"

#include "../object/DrawObj.h"
#include "GUIManager.h"
#include <assert.h>
#include "../DrawTool.h"
#include "../drawvw.h"
#include "../DrawDoc.h"
#include "../MainFrm.h"



extern GUIManager* g_pGuiManager;

bool GUIManager::MsgObjectMoveback()
{	
	CDrawObjLists::iterator it = m_SelectList.begin();

	CDrawObj* pObj = *it;

	if (InnerObjectMoveback(pObj))
	{
		pObj->InvalObj();
	}

	return true;
}
bool GUIManager::MsgObjectMoveforward()
{
	CDrawObjLists::iterator it = m_SelectList.begin();

	CDrawObj* pObj = *it;

	if (InnerObjectMoveforward(pObj))
	{
		pObj->InvalObj();
	}
	return true;
}
bool GUIManager::MsgObjectMovetoback()
{	
	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObj* pObj = *it;

	if (InnerObjectMovetoback(pObj))
	{
		pObj->InvalObj();
	}	
	return true;
}
bool GUIManager::MsgObjectMovetofront()
{
	CDrawObjLists::iterator it = m_SelectList.begin();

	CDrawObj* pObj = *it;
	if (InnerObjectMovetofront(pObj))
	{
		pObj->InvalObj();
	}
	return true;
}
bool GUIManager::InnerObjectMovetoback(CDrawObj *pObj)
{
	bool success = false;
	CDrawObjLists::iterator it = m_ObjectList.begin();
	CDrawObjLists::iterator it_end = m_ObjectList.end();
	for (it; it != it_end;)
	{
		CDrawObj* pObj_temp = *it;
		if (pObj_temp == pObj)
		{
			m_ObjectList.erase(it++);
			success = true;
		}
		else
		{
			it++;
		}
	}
	if (success)
	{
		m_ObjectList.push_front(pObj);
	}
	return success;

}
bool GUIManager::InnerObjectMovetofront(CDrawObj *pObj)
{	
	bool success = false;
	CDrawObjLists::iterator it = m_ObjectList.begin();
	CDrawObjLists::iterator it_end = m_ObjectList.end();
	for (it; it != it_end;)
	{
		CDrawObj* pObj_temp = *it;
		if (pObj_temp == pObj)
		{
			m_ObjectList.erase(it++);
			success = true;
		}
		else
		{
			it++;
		}
	}
	if (success)
	{
		m_ObjectList.push_back(pObj);
	}
	return success;
}

bool GUIManager::InnerObjectMoveforward(CDrawObj *pObj)
{	
	bool success = false;
	CDrawObjLists::iterator it = m_ObjectList.begin();
	CDrawObjLists::iterator it_end = m_ObjectList.end();
	for (it; it != it_end; )
	{
		CDrawObj* pObj_temp = *it;
		if (pObj_temp == pObj )
		{
			if (++it != m_ObjectList.end())
			{
				++it;
				m_ObjectList.insert(it, pObj_temp);//在当前查找的前面插入
				--it;
				--it;
				--it;
				m_ObjectList.erase(it);			//删除当前
				success = true;
			}
			break;
		}
		else
		{
			it++;
		}
	}
	return success;
}
bool GUIManager::InnerObjectMoveback(CDrawObj *pObj)
{	
	bool success = false;
	CDrawObjLists::iterator it = m_ObjectList.begin();
	CDrawObjLists::iterator it_end = m_ObjectList.end();
	for (it; it != it_end; )
	{
		CDrawObj* pObj_temp = *it;
		if (pObj_temp == pObj )
		{
			if (it != m_ObjectList.begin())
			{
				m_ObjectList.insert(--it, pObj_temp);//在当前查找的前面插入
				m_ObjectList.erase(++it);			//删除当前
				success = true;
			}
			break;
		}
		else
		{
			it++;
		}
	}
	return success;
}


bool GUIManager::Select( CDrawObj* pObj, bool bAdd )
{
	if (!bAdd)//没有选中
	{
		RefrshSelectionList();
		//((CDrawView*)Painter::GetInstancePtr()->GetCWnd())->OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_SelectList.clear();
		m_selectcount = 0;
	}

	if (pObj == NULL || IsSelected(pObj))
	{
		return false;
	}

	m_SelectList.push_back(pObj);
	m_selectcount++;

	pObj->InvalObj();

	return true;
}
CDrawObj* GUIManager::ObjectAt(const CPoint& point)
{
	CRect rect(point, CSize(1, 1));

	CDrawObjLists::iterator it = m_ObjectList.begin();
	CDrawObjLists::iterator it_end = m_ObjectList.end();
	for (it; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		assert(pObj);

		if (pObj->Intersects(rect))
		{
			return pObj;
		}
	}

	return NULL;
}
void GUIManager::SelectWithinRect(CRect rect,bool bAdd)
{
	if (!bAdd)
	{
		g_pGuiManager->Select(NULL);
	}

	CDrawObjLists* pDrawObjLists = GetObjLists();
	CDrawObjLists::iterator it = pDrawObjLists->begin();
	CDrawObjLists::iterator it_end = pDrawObjLists->end();
	for (it; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		assert(pObj);

		if (pObj->Intersects(rect))
		{
			Select(pObj, true);
		}
	}

}
void GUIManager::Deselect(CDrawObj* pObj)
{
	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObjLists::iterator it_end = m_SelectList.end();
	for (it; it != it_end; )
	{
		CDrawObj* pObj_temp = *it;
		if (pObj == pObj_temp)
		{
			pObj_temp->InvalObj();

			m_SelectList.erase(it++);
			m_selectcount--;
			return;
		}
		else
		{
			it++;
		}
	}
}
bool GUIManager::MsgSelectAll()
{
	CDrawObjLists* pObjectList = (CDrawObjLists*)GetObjLists();//获得对象列表
	CDrawObjLists::iterator it = pObjectList->begin();
	CDrawObjLists::iterator it_end = pObjectList->end();
	for (it; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		assert(pObj);
		Select(pObj, true);

	}
	return true;
}
void GUIManager::MsgCopySelect()
{
	m_CopyList.clear();
	m_copycount = 0;

	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObjLists::iterator it_end = m_SelectList.end();

	for (it; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		assert(pObj);

		CDrawObj* temp = pObj->Clone();
		assert(temp);

		m_CopyList.push_back(temp);
		m_copycount++;
	}
	
	return ;
}
void GUIManager::MsgClearSelect()
{
	//((CDrawView*)Painter::GetInstancePtr()->GetCWnd())->OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	RefrshSelectionList();
	//RefrshAllObjList();

	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObjLists::iterator it_end = m_SelectList.end();

	for (it; it != it_end; )
	{
		CDrawObj* pObj = *it;
		assert(pObj);

		m_SelectList.erase(it++);

		Remove(pObj);

		//pObj->m_pDocument->SetModifiedFlag(true);

		pObj->Remove();
	}

	m_selectcount = 0;
	return ;
}
void GUIManager::MsgEditPaste()
{
	RefrshSelectionList();

	m_SelectList.clear();
	m_selectcount = 0;

	CDrawObjLists::iterator it = m_CopyList.begin();
	CDrawObjLists::iterator it_end = m_CopyList.end();

	for (it; it != it_end; )
	{
		CDrawObj* pObj = *it;
		assert(pObj);

		m_CopyList.erase(it++);
		m_copycount--;

		AddBack(pObj);

		m_SelectList.push_back(pObj);
		m_selectcount++;
	}

	RefrshAllObjList();

	return;
}
void GUIManager::MsgEditProperties()
{
	//必须选中的是一个对象
	if (GetSelectCount() != 1)
	{
		return;
	}
	
	if (GetDrawShape() != DRAWSHAPE_TOOL_SELECTION)
	{
		return;
	}

	CDrawObjLists::iterator it = m_SelectList.begin();
	(*it)->OnEditProperties();


	return;
}
void GUIManager::MsgObjectFillcolor(COLORREF color)
{
	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObjLists::iterator it_end = m_SelectList.end();
	for (; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		assert(pObj);
		pObj->SetFillColor(color);
	}
	return;
}

void GUIManager::MsgObjectLinecolor(COLORREF color)
{
	CDrawObjLists::iterator it = m_SelectList.begin();
	CDrawObjLists::iterator it_end = m_SelectList.end();
	for (; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		assert(pObj);
		pObj->SetLineColor(color);
	}

	return;
}
bool GUIManager::DemoData(WPARAM wParam, LPARAM lParam)
{
	COLORREF   color;

	CDrawObjLists* pDrawObjLists = g_pGuiManager->GetObjLists();
	CDrawObjLists::iterator it = pDrawObjLists->begin();
	CDrawObjLists::iterator it_end = pDrawObjLists->end();
	for (it; it != it_end; it++)
	{
		CDrawObj* pObj = *it;
		assert(pObj);

		if (pObj->m_nType == 1) // DI/DO状态量
		{
			//color= (Demo_Data.DI_DO[pObj->m_nOrder]==0)?RGB(0,255,0):RGB(255,0,0);
			//pObj->SetFillColor(color);
		}

	}
	return true;
}
