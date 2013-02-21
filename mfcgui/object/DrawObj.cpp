//--------------------------------------------------------------------
// 文件名:		DrawObj.cpp
// 内  容:		基类
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"
#include "../DrawCli.h"
#include "DrawObj.h"

#include "../drawdoc.h"
#include "../drawvw.h"
#include "../rectdlg.h"
#include <assert.h>
#include "../manager/GUIManager.h"


bool GuiUtil_PointInRect(int x, int y, int x1, int y1, int x2, int y2);
CDrawObj::CDrawObj()
{
}

CDrawObj::~CDrawObj()
{
}

CDrawObj::CDrawObj(const CRect& position)
{
	m_position = position;
	m_pDocument = NULL;

	m_bPen = TRUE;
	m_logpen.lopnStyle = PS_INSIDEFRAME;
	m_logpen.lopnWidth.x = 2;
	m_logpen.lopnWidth.y = 2;
	m_logpen.lopnColor = RGB(0, 0, 0);

	m_bBrush = TRUE;
	m_logbrush.lbStyle = BS_SOLID;
	m_logbrush.lbColor = RGB(192, 192, 192);
	m_logbrush.lbHatch = HS_HORIZONTAL;

	m_nOrder=0;
	m_nType=0;
}

/*
void CDrawObj::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_position;
		ar << (WORD)m_bPen;
		ar.Write(&m_logpen, sizeof(LOGPEN));
		ar << (WORD)m_bBrush;
		ar.Write(&m_logbrush, sizeof(LOGBRUSH));
		ar << m_nOrder;
		ar << m_nType;
	}
	else
	{
		// get the document back pointer from the archive
		m_pDocument = (CDrawDoc*)ar.m_pDocument;

		assert(m_pDocument);
		//ASSERT_VALID(m_pDocument);
		ASSERT_KINDOF(CDrawDoc, m_pDocument);

		WORD wTemp;
		ar >> m_position;
		ar >> wTemp; m_bPen = (BOOL)wTemp;
		ar.Read(&m_logpen,sizeof(LOGPEN));
		ar >> wTemp; m_bBrush = (BOOL)wTemp;
		ar.Read(&m_logbrush, sizeof(LOGBRUSH));
		ar >> m_nOrder;
		ar >> m_nType;
	}
}
*/
void CDrawObj::Remove()
{
	delete this;
}

void CDrawObj::Draw()
{
	return;
}
void CDrawObj::Paint(float seconds)
{
	return;
}
void CDrawObj::InvalObj()
{
	return;
}


void CDrawObj::DrawTracker(CDC* pDC, TrackerState state)
{
	assert(this);

	CClientDC dc(Painter::GetInstancePtr()->GetCWnd());//客户区设备上下文
	CDC * pcdc = Painter::GetInstancePtr()->GetCDC();
	switch (state)
	{
	case normal:
		break;

	case selected:
	case active:
		{
			int nHandleCount = GetHandleCount();
			for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
			{
				CPoint handle = GetHandle(nHandle);
				Painter::GetInstancePtr()->PatBlt(handle);
			}
		}
		break;
	}
}

// position is in logical
void CDrawObj::MoveTo(const CRect& position, CDrawView* pView)
{
	assert(this);

	if (position == m_position)
		return;

	if (pView == NULL)
	{
		Invalidate();
		m_position = position;
		Invalidate();
	}
	else
	{
		InvalObj();
		m_position = position;
		InvalObj();
	}
	m_pDocument->SetModifiedFlag();
}

// Note: if bSelected, hit-codes start at one for the top-left
// and increment clockwise, 0 means no hit.
// If !bSelected, 0 = no hit, 1 = hit (anywhere)

// point is in logical coordinates
int CDrawObj::HitTest(CPoint point, bool bSelected)
{
	assert(this);

	CRect rect = m_position;
	if (bSelected)
	{
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			// GetHandleRect returns in logical coords
			CRect rc;
			CPoint point_temp = GetHandle(nHandle);//GetHandleRect(nHandle);
			rc.SetRect(point_temp.x - 3, point_temp.y - 3, point_temp.x + 3, point_temp.y + 3);

			//TRACE("point is :%d  %d  rc is :%d  %d  %d  %d\n",point.x, point.y,
			//	rc.left, rc.right, rc.top, rc.bottom);

			if (point.x >= rc.left && point.x < rc.right &&
				point.y >= rc.top && point.y < rc.bottom)
				return nHandle;
		}
	}
	else
	{
		if (point.x >= rect.left && point.x < rect.right &&
			point.y <= rect.top && point.y > rect.bottom)
			return DRAGHANDLE_RIGHT_BOTTOM;
	}
	return DRAGHANDLE_NONE;
}

bool GuiUtil_PointInRect(int x, int y, int x1, int y1, int x2, int y2)
{
	return (x >= x1) && (x < x2) && (y >= y1) && (y < y2);
}

// rect must be in logical coordinates
bool CDrawObj::Intersects(const CRect& rect)
{
	assert(this);

	CRect fixed = m_position;

	Painter::GetInstancePtr()->ClientToDoc(fixed);

	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	return !(rectT & rect).IsRectEmpty();
}

int CDrawObj::GetHandleCount()
{
	assert(this);

	return 8;
}

// returns logical coords of center of handle
CPoint CDrawObj::GetHandle(int nHandle)
{
	assert(this);

	int x, y, xCenter, yCenter;

	CRect rect = m_position;
	///************************************************************************/
	///*修改坐标测试                                                          */
	///************************************************************************/
	//CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	//if (!pwnd)
	//{
	//	assert(0);
	//}
	//pwnd->ClientToDoc(rect);
	///************************************************************************/
	///*                                                                      */
	///************************************************************************/


	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = rect.left + rect.Width() / 2;
	yCenter = rect.top + rect.Height() / 2;

	switch (nHandle)
	{
	default:
		assert(false);

	case 1:
		x = rect.left;
		y = rect.top;
		break;

	case 2:
		x = xCenter;
		y = rect.top;
		break;

	case 3:
		x = rect.right;
		y = rect.top;
		break;

	case 4:
		x = rect.right;
		y = yCenter;
		break;

	case 5:
		x = rect.right;
		y = rect.bottom;
		break;

	case 6:
		x = xCenter;
		y = rect.bottom;
		break;

	case 7:
		x = rect.left;
		y = rect.bottom;
		break;

	case 8:
		x = rect.left;
		y = yCenter;
		break;
	}

	return CPoint(x, y);
}
//
//// return rectange of handle in logical coords
//CPoint CDrawObj::GetHandleRect(int nHandleID)
//{
//	assert(this);
//	CRect rect;
//	// get the center of the handle in logical coords
//	CPoint point = GetHandle(nHandleID);
//	return point;
//}

HCURSOR CDrawObj::GetHandleCursor(int nHandle)
{
	assert(this);

	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case DRAGHANDLE_RIGHT_BOTTOM:
	case DRAGHANDLE_LEFT_TOP:
		id = IDC_SIZENWSE;
		break;

	case DRAGHANDLE_BOTTOM:
	case DRAGHANDLE_TOP:
		id = IDC_SIZENS;
		break;

	case DRAGHANDLE_RIGHT_TOP:
	case DRAGHANDLE_LEFT_BOTTOM:
		id = IDC_SIZENESW;
		break;

	case DRAGHANDLE_LEFT:
	case DRAGHANDLE_RIGHT:
		id = IDC_SIZEWE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

// point must be in logical
void CDrawObj::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	assert(this);

	CRect rect = m_position;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		rect.left = point.x;
		rect.top = point.y;
		break;

	case 2:
		rect.top = point.y;
		break;

	case 3:
		rect.right = point.x;
		rect.top = point.y;
		break;

	case 4:
		rect.right = point.x;
		break;

	case 5:
		rect.right = point.x;
		rect.bottom = point.y;
		break;

	case 6:
		rect.bottom = point.y;
		break;

	case 7:
		rect.left = point.x;
		rect.bottom = point.y;
		break;

	case 8:
		rect.left = point.x;
		break;
	}

	MoveTo(rect, pView);
}

void CDrawObj::Invalidate()
{
	assert(this);
	m_pDocument->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, this);
}

CDrawObj* CDrawObj::Clone()
{
	assert(this);
 
	CDrawObj* pClone = new CDrawObj(m_position);

	pClone->m_bPen = m_bPen;
	pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	pClone->m_logbrush = m_logbrush;
	pClone->m_nOrder = m_nOrder;
	pClone->m_nType = m_nType;

	pClone->m_pDocument = m_pDocument;

	pClone->m_pDocument = m_pDocument;

	assert(pClone);

	////放到调用的上层去添加了
	//if (pDoc != NULL)
	//	pDoc->AddBack(pClone);

	return pClone;

}

void CDrawObj::OnEditProperties()
{
	assert(this);
   
	CPropertySheet sheet( _T("对象显示属性") );
	CRectDlg dlg;
	dlg.m_bNoFill = !m_bBrush;
	dlg.m_penSize = m_bPen ? m_logpen.lopnWidth.x : 0;
	dlg.m_LineColor = m_logpen.lopnColor;	  
    dlg.m_FillColor = m_logbrush.lbColor;	  
    dlg.m_nOrd=m_nOrder;
    dlg.m_nTyp=m_nType;

	sheet.AddPage( &dlg );

	if (sheet.DoModal() != IDOK)
		return;
  

	m_bBrush = !dlg.m_bNoFill;
	m_bPen = dlg.m_penSize > 0;
	m_logbrush.lbColor = dlg.m_FillColor;   
	if (m_bPen)
	{
	    m_logpen.lopnWidth.x = dlg.m_penSize;
		m_logpen.lopnWidth.y = dlg.m_penSize;
		m_logpen.lopnColor = dlg.m_LineColor;   

 	}

    m_nOrder=dlg.m_nOrd;
    m_nType=dlg.m_nTyp;

	Invalidate();
	m_pDocument->SetModifiedFlag(); 
}

void CDrawObj::OnOpen(CDrawView* /*pView*/ )
{
}

void CDrawObj::SetLineColor(COLORREF color)
{
	assert(this);

	m_logpen.lopnColor = color;
	Invalidate();
	m_pDocument->SetModifiedFlag();
}

void CDrawObj::SetFillColor(COLORREF color)
{
	assert(this);

	m_logbrush.lbColor = color;
	Invalidate();
	m_pDocument->SetModifiedFlag();
}

//#ifdef _DEBUG
//void CDrawObj::AssertValid()
//{
//	ASSERT(m_position.left <= m_position.right);
//	ASSERT(m_position.bottom <= m_position.top);
//}
//#endif
//
//////////////////////////////////////////////////////////////////////////////
