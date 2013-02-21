//--------------------------------------------------------------------
// 文件名:		Painter.cpp
// 内  容:		绘图接口
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"

#include "../object/DrawObj.h"
#include "Painter.h"
#include "../DrawVw.h"
#include <assert.h>
Painter* g_pPainter;

Painter* Painter::_instance = 0;
Painter::Painter()
{
	//m_selectcount = 0;
	//m_SelectList.clear();

	//m_drawShape = TOOL_SELECTION;

	//m_selectTool = new CSelectTool();
	//m_lineTool = new CRectTool(TOOL_LINE);
	//m_rectTool = new CRectTool(TOOL_RECT);
	//m_roundRectTool = new CRectTool(TOOL_ROUNDRECT);
	//m_ellipseTool = new CRectTool(TOOL_ELLIPSE);
	//m_circleTool = new CRectTool(TOOL_CIRCLE);

	//m_toolmap.insert(std::map<int, CDrawTool*>::value_type(TOOL_SELECTION, m_selectTool));
	//m_toolmap.insert(std::map<int, CDrawTool*>::value_type(TOOL_LINE, m_lineTool));
	//m_toolmap.insert(std::map<int, CDrawTool*>::value_type(TOOL_RECT, m_rectTool));
	//m_toolmap.insert(std::map<int, CDrawTool*>::value_type(TOOL_ROUNDRECT, m_roundRectTool));
	//m_toolmap.insert(std::map<int, CDrawTool*>::value_type(TOOL_ELLIPSE, m_ellipseTool));
	//m_toolmap.insert(std::map<int, CDrawTool*>::value_type(TOOL_CIRCLE, m_circleTool));

	m_paperColor = RGB(255, 255, 255);
	m_bActive = false;

}

bool Painter::shut()
{

	return true;
}
bool Painter::init()
{
	return true;
}
void Painter::SetCapture()
{
	assert(m_pCwnd);
	m_pCwnd->SetCapture();
}
CWnd* Painter::GetCapture()
{
	assert(m_pCwnd);
	return m_pCwnd->GetCapture();
}
void Painter::FreeCapture()
{
	ReleaseCapture();
}

void Painter::InvalidateRect(CRect& rect, bool is)
{
	assert(m_pCwnd);
	m_pCwnd->InvalidateRect(rect, is);//这一部分将被重新绘制
}
void Painter::PatBlt(CPoint point)
{
	assert(m_pCwnd);
	assert(m_pCdc);

	CDrawView* pwnd = (CDrawView*)m_pCwnd;
	
	pwnd->ClientToDoc(point);
	m_pCdc->PatBlt(point.x - 3, point.y - 3, 7, 7, DSTINVERT);
}
//
//HCURSOR Painter::GetHandleCursor(int nHandle)
//{
//	assert(this);
//
//	LPCTSTR id;
//	switch (nHandle)
//	{
//	default:
//		ASSERT(FALSE);
//
//	case 1:
//	case 5:
//		id = IDC_SIZENWSE;
//		break;
//
//	case 2:
//	case 6:
//		id = IDC_SIZENS;
//		break;
//
//	case 3:
//	case 7:
//		id = IDC_SIZENESW;
//		break;
//
//	case 4:
//	case 8:
//		id = IDC_SIZEWE;
//		break;
//	}
//
//	return AfxGetApp()->LoadStandardCursor(id);
//}
//
void Painter::ClientToDoc(CPoint& point)
{
	assert(m_pCwnd);
	//CClientDC dc(m_pCwnd);
	////OnPrepareDC(&dc, NULL);
	//dc.DPtoLP(&point);

	CDrawView* pwnd = (CDrawView*)m_pCwnd;
	pwnd->ClientToDoc(point);
}

void Painter::ClientToDoc(CRect& rect)
{
	assert(m_pCwnd);
	//CClientDC dc(m_pCwnd);
	////OnPrepareDC(&dc, NULL);
	//dc.DPtoLP(rect);


	////TRACE("rect1 is :%d  %d  %d  %d\n",rect.left, rect.right, rect.top, rect.bottom);
	//////rect.NormalizeRect();
	////TRACE("rect2 is :%d  %d  %d  %d\n",rect.left, rect.right, rect.top, rect.bottom);

	////ASSERT(rect.left <= rect.right);
	////ASSERT(rect.bottom <= rect.top);


	CDrawView* pwnd = (CDrawView*)m_pCwnd;
	pwnd->ClientToDoc(rect);
}

void Painter::DocToClient(CPoint& point)
{
	assert(m_pCwnd);
	//CClientDC dc(m_pCwnd);
	////OnPrepareDC(&dc, NULL);
	//dc.LPtoDP(&point);

	CDrawView* pwnd = (CDrawView*)m_pCwnd;
	pwnd->DocToClient(point);
}

void Painter::DocToClient(CRect& rect)
{
	assert(m_pCwnd);
	//CClientDC dc(m_pCwnd);
	////OnPrepareDC(&dc, NULL);
	//dc.LPtoDP(rect);
	//rect.NormalizeRect();

	CDrawView* pwnd = (CDrawView*)m_pCwnd;
	pwnd->DocToClient(rect);
}


void Painter::DrawCircle(CRect rect)
{
	assert(this);
	assert(m_pCdc);

	int cx,cy;
	if (rect.right < rect.left)
	{
		cx = rect.left;
		rect.left = rect.right;
		rect.right = cx;
	}
	if (rect.bottom < rect.top)
	{
		cy = rect.top;
		rect.top = rect.bottom;
		rect.bottom = cy;
	}
	cx = rect.right - rect.left;
	cy = rect.bottom - rect.top;
	if (cx > cy)
	{
		rect.left += (cx-cy) / 2;
		rect.right = rect.left + cy;
	}
	else
	{
		rect.top += (cy-cx) / 2;
		rect.bottom = rect.top + cx;
	}
	//m_pCdc->TextOut(rect.left,rect.bottom,"123",3);
	m_pCdc->Ellipse(rect);
}
