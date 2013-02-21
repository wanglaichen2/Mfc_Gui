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
#include "DrawLine.h"
#include <assert.h>
#include "../drawdoc.h"
#include "../drawvw.h"
#include "../manager/GUIManager.h"
////////////////////////////////////////////////////////////////////////////
// CDrawRect

CDrawLine::CDrawLine()
{
}

CDrawLine::CDrawLine(const CRect& position)
	: IControl(position)
{
	assert(this);
}

void CDrawLine::Draw()
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
	///************************************************************************/
	///*修改坐标测试                                                          */
	///************************************************************************/
		//CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
		//if (!pwnd)
		//{
		//	return ;
		//}
		//pwnd->ClientToDoc(rect);

		Painter::GetInstancePtr()->ClientToDoc(rect);
		//rect.NormalizeRect();
	///************************************************************************/
	///*                                                                      */
	///************************************************************************/

	//switch (m_nShape)
	//{
	//default:
	//	assert(0);
	//	break;
	//case obj_Shape_line:
		if (rect.top > rect.bottom)
		{
			rect.top -= m_logpen.lopnWidth.y / 2;
			rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
		}
		else
		{
			rect.top += (m_logpen.lopnWidth.y + 1) / 2;
			rect.bottom -= m_logpen.lopnWidth.y / 2;
		}

		if (rect.left > rect.right)
		{
			rect.left -= m_logpen.lopnWidth.x / 2;
			rect.right += (m_logpen.lopnWidth.x + 1) / 2;
		}
		else
		{
			rect.left += (m_logpen.lopnWidth.x + 1) / 2;
			rect.right -= m_logpen.lopnWidth.x / 2;
		}

		cdc->MoveTo(rect.TopLeft());
		cdc->LineTo(rect.BottomRight());
	//	break;
	//}

	cdc->SelectObject(pOldBrush);
	cdc->SelectObject(pOldPen);
}

//void CDrawCircle::Paint(float seconds)
//{
//	return;
//}
void CDrawLine::InvalObj()
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

int CDrawLine::GetHandleCount()
{
	assert(this);
	return 2;
}

// returns center of handle in logical coordinates
CPoint CDrawLine::GetHandle(int nHandle)
{
	assert(this);

	if (nHandle == 2)
		nHandle = 5;

	return IControl::GetHandle(nHandle);
}
//获得鼠标形状
HCURSOR CDrawLine::GetHandleCursor(int nHandle)
{
	assert(this);

	//线段和圆角矩形特殊处理
	if (nHandle == 2)
	{
		nHandle = 5;
	}

	return IControl::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void CDrawLine::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	assert(this);
	//直线和圆角矩形特殊处理
	if (nHandle == 2)
		nHandle = 5;
	IControl::MoveHandleTo(nHandle, point, pView);
}

// rect must be in logical coordinates
bool CDrawLine::Intersects(const CRect& rect)
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
	//switch (m_nShape)
	//{
	//default:
	//	assert(0);
	//	break;
	//case obj_Shape_line:
	//	{
			int x = (m_logpen.lopnWidth.x + 5) / 2;
			int y = (m_logpen.lopnWidth.y + 5) / 2;
			POINT points[4];
			points[0].x = fixed.left;
			points[0].y = fixed.top;
			points[1].x = fixed.left;
			points[1].y = fixed.top;
			points[2].x = fixed.right;
			points[2].y = fixed.bottom;
			points[3].x = fixed.right;
			points[3].y = fixed.bottom;

			if (fixed.left < fixed.right)
			{
				points[0].x -= x;
				points[1].x += x;
				points[2].x += x;
				points[3].x -= x;
			}
			else
			{
				points[0].x += x;
				points[1].x -= x;
				points[2].x -= x;
				points[3].x += x;
			}

			if (fixed.top < fixed.bottom)
			{
				points[0].y -= y;
				points[1].y += y;
				points[2].y += y;
				points[3].y -= y;
			}
			else
			{
				points[0].y += y;
				points[1].y -= y;
				points[2].y -= y;
				points[3].y += y;
			}
			rgn.CreatePolygonRgn(points, 4, ALTERNATE);
	//	}
	//	break;
	//}
	return TRUE == rgn.RectInRegion(fixed);
}

CDrawObj* CDrawLine::Clone()
{
	assert(this);

	CDrawLine* pClone = new CDrawLine(m_position);

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
