//--------------------------------------------------------------------
// 文件名:		CDrawRect.cpp
// 内  容:		测试通用模板
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"
#include "DrawObj.h"
#include "IControl.h"
#include "DrawEllipse.h"
#include <assert.h>
#include "../drawdoc.h"
#include "../drawvw.h"
#include "../manager/GUIManager.h"
////////////////////////////////////////////////////////////////////////////
// CDrawRect

CDrawEllipse::CDrawEllipse()
{
}

CDrawEllipse::CDrawEllipse(const CRect& position)
	: IControl(position)
{
	assert(this);
}

void CDrawEllipse::Draw()
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


	cdc->Ellipse(rect);

	cdc->SelectObject(pOldBrush);
	cdc->SelectObject(pOldPen);
}

//void CDrawCircle::Paint(float seconds)
//{
//	return;
//}
void CDrawEllipse::InvalObj()
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
// returns center of handle in logical coordinates
CPoint CDrawEllipse::GetHandle(int nHandle)
{
	assert(this);

	return IControl::GetHandle(nHandle);
}
//获得鼠标形状
HCURSOR CDrawEllipse::GetHandleCursor(int nHandle)
{
	assert(this);
	return IControl::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void CDrawEllipse::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	assert(this);
	IControl::MoveHandleTo(nHandle, point, pView);
}

// rect must be in logical coordinates
bool CDrawEllipse::Intersects(const CRect& rect)
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
	
	rgn.CreateEllipticRgnIndirect(fixed);

	return TRUE == rgn.RectInRegion(fixed);
}

CDrawObj* CDrawEllipse::Clone()
{
	assert(this);

	CDrawEllipse* pClone = new CDrawEllipse(m_position);

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
