//--------------------------------------------------------------------
// 文件名:		CDrawRoundRectangle.cpp
// 内  容:		圆角矩形图元
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"
#include "DrawObj.h"
#include "IControl.h"
#include "DrawRoundRectangle.h"
#include <assert.h>
#include "../drawdoc.h"
#include "../drawvw.h"
#include "../manager/GUIManager.h"
////////////////////////////////////////////////////////////////////////////
// CDrawRect

CDrawRoundRectangle::CDrawRoundRectangle()
{
}

CDrawRoundRectangle::CDrawRoundRectangle(const CRect& position)
	: IControl(position)
{
	assert(this);

	//m_nShape = obj_Shape_roundRectangle;
	m_Roundness.x = 16;
	m_Roundness.y = 16;
}


void CDrawRoundRectangle::Draw()
{
	assert(this);

	CDC* cdc = Painter::GetInstancePtr()->GetCDC();
	CClientDC dc(Painter::GetInstancePtr()->GetCWnd());//客户区设备上下文

	CBrush brush;
	if (!brush.CreateBrushIndirect(&m_logbrush))
		return;
	CPen pen;
	if (!pen.CreatePenIndirect(&m_logpen))
		return;

	CBrush* pOldBrush;
	CPen* pOldPen;

	if (m_bBrush)
		pOldBrush = cdc->SelectObject(&brush);
	else
		pOldBrush = (CBrush*)cdc->SelectStockObject(NULL_BRUSH);

	if (m_bPen)
		pOldPen = cdc->SelectObject(&pen);
	else
		pOldPen = (CPen*)cdc->SelectStockObject(NULL_PEN);

	CRect rect = m_position;

	Painter::GetInstancePtr()->ClientToDoc(rect);
	//rect.NormalizeRect();

	cdc->RoundRect(rect, m_Roundness);
	//switch (m_nShape)
	//{
	//default:
	//	assert(0);
	//	break;
	//case obj_Shape_roundRectangle:
	//	cdc->RoundRect(rect, m_roundness);
	//	break;
	//}

	cdc->SelectObject(pOldBrush);
	cdc->SelectObject(pOldPen);
}

//void CDrawCircle::Paint(float seconds)
//{
//	return;
//}
void CDrawRoundRectangle::InvalObj()
{
	assert(this);

	CRect rect = this->m_position;

	rect.NormalizeRect();

	if (Painter::GetInstancePtr()->GetActive() && g_pGuiManager->IsSelected(this))
	{
		rect.left -= 4;
		rect.top -= 5;
		rect.right += 5;
		rect.bottom += 4;
	}
	//增大
	rect.InflateRect(1, 1); // handles CDrawOleObj objects

	Painter::GetInstancePtr()->InvalidateRect(rect);
	return;
}

int CDrawRoundRectangle::GetHandleCount()
{
	assert(this);

	return IControl::GetHandleCount() + 1;
}

// returns center of handle in logical coordinates
CPoint CDrawRoundRectangle::GetHandle(int nHandle)
{
	assert(this);
	if (nHandle == 9)
	{
		CRect rect = m_position;

		Painter::GetInstancePtr()->ClientToDoc(rect);

		rect.NormalizeRect();
		CPoint point = rect.BottomRight();
		point.x -= m_Roundness.x / 2;
		point.y -= m_Roundness.y / 2;
		return point;
	}
	return IControl::GetHandle(nHandle);
}
//获得鼠标形状
HCURSOR CDrawRoundRectangle::GetHandleCursor(int nHandle)
{
	assert(this);
	if (nHandle == 9)
	{
		return AfxGetApp()->LoadStandardCursor(IDC_SIZEALL); //十字箭头
	}
	return IControl::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void CDrawRoundRectangle::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	assert(this);
	if (nHandle == 9)
	{
		CRect rect = m_position;

		Painter::GetInstancePtr()->ClientToDoc(rect);

		rect.NormalizeRect();
		if (point.x > rect.right - 1)
			point.x = rect.right - 1;
		else if (point.x < rect.left + rect.Width() / 2)
			point.x = rect.left + rect.Width() / 2;
		if (point.y > rect.bottom - 1)
			point.y = rect.bottom - 1;
		else if (point.y < rect.top + rect.Height() / 2)
			point.y = rect.top + rect.Height() / 2;
		m_Roundness.x = 2 * (rect.right - point.x);
		m_Roundness.y = 2 * (rect.bottom - point.y);
		m_pDocument->SetModifiedFlag();
		if (pView == NULL)
			Invalidate();
		else
			InvalObj();
		return;
	}
	IControl::MoveHandleTo(nHandle, point, pView);
}

// rect must be in logical coordinates
bool CDrawRoundRectangle::Intersects(const CRect& rect)
{
	assert(this);

	CRect rectT = rect;
	rectT.NormalizeRect();

	CRect fixed = m_position;

	Painter::GetInstancePtr()->ClientToDoc(fixed);

	fixed.NormalizeRect();
	if ((rectT & fixed).IsRectEmpty())
		return FALSE;

	CRgn rgn;

	rgn.CreateRoundRectRgn(fixed.left, fixed.top, fixed.right, fixed.bottom,
		m_Roundness.x, m_Roundness.y);

	return TRUE == rgn.RectInRegion(fixed);
}

CDrawObj* CDrawRoundRectangle::Clone()
{
	assert(this);

	CDrawRoundRectangle* pClone = new CDrawRoundRectangle(m_position);

	pClone->m_bPen = m_bPen;
	pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	pClone->m_logbrush = m_logbrush;
	//pClone->m_nShape = m_nShape;
	pClone->m_Roundness = m_Roundness;

	pClone->m_nOrder = m_nOrder;
	pClone->m_nType = m_nType;

	pClone->m_pDocument = m_pDocument;

	assert(pClone);

	// 	if (pDoc != NULL)
	// 		pDoc->AddBack(pClone);

	assert(pClone);

	return pClone;
}
