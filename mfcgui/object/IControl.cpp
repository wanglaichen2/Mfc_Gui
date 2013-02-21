//--------------------------------------------------------------------
// 文件名:		IControl.cpp
// 内  容:		图元基类
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"
#include "DrawObj.h"
#include "IControl.h"
#include <assert.h>
#include "../drawdoc.h"
#include "../drawvw.h"
#include "../manager/GUIManager.h"

IControl::IControl()
{
}
IControl::~IControl()
{
}
IControl::IControl(const CRect& position)
	: CDrawObj(position)
{
	assert(this);
}
/*
void CDrawRect::Serialize(CArchive& ar)
{
	assert(this);
	//ASSERT_VALID(this);

	CDrawObj::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << (WORD) m_nShape;
		ar << m_roundness;
	}
	else
	{
		WORD wTemp;
		ar >> wTemp; m_nShape = (Shape)wTemp;
		ar >> m_roundness;
	}
}
*/

void IControl::Draw()
{
	assert(this);
}

void IControl::Paint(float seconds)
{
	assert(this);
	return;
}
void IControl::InvalObj()
{
	assert(this);
	return;
}

int IControl::GetHandleCount()
{
	assert(this);
	return CDrawObj::GetHandleCount();
}

// returns center of handle in logical coordinates
CPoint IControl::GetHandle(int nHandle)
{
	assert(this);
	return CDrawObj::GetHandle(nHandle);
}
//获得鼠标形状
HCURSOR IControl::GetHandleCursor(int nHandle)
{
	assert(this);
	return CDrawObj::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void IControl::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	assert(this);
	CDrawObj::MoveHandleTo(nHandle, point, pView);
}

// rect must be in logical coordinates
bool IControl::Intersects(const CRect& rect)
{
	assert(this);
	return true;
}

CDrawObj* IControl::Clone()
{
	assert(this);

	IControl* pClone = new IControl(m_position);

	pClone->m_bPen = m_bPen;
	pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	pClone->m_logbrush = m_logbrush;

	pClone->m_nOrder = m_nOrder;
	pClone->m_nType = m_nType;

	pClone->m_pDocument = m_pDocument;

	assert(pClone);

// 	if (pDoc != NULL)
// 		pDoc->AddBack(pClone);

	assert(pClone);

	return pClone;
}

