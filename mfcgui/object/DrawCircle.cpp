//--------------------------------------------------------------------
// �ļ���:		CDrawCircle.cpp
// ��  ��:		Բ�λ���
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
//--------------------------------------------------------------------

#include "stdafx.h"
#include "DrawObj.h"
//#include "DrawRect.h"
#include "IControl.h"
#include "DrawCircle.h"
#include <assert.h>
#include "../drawdoc.h"
#include "../drawvw.h"
#include "../manager/GUIManager.h"
////////////////////////////////////////////////////////////////////////////
// CDrawRect

CDrawCircle::CDrawCircle()
{
}

CDrawCircle::CDrawCircle(const CRect& position)
	: IControl(position)
{
	assert(this);
}

void CDrawCircle::Draw()
{
	assert(this);

	CDC* cdc = Painter::GetInstancePtr()->GetCDC();
	CClientDC dc(Painter::GetInstancePtr()->GetCWnd());//�ͻ����豸������

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
	///*�޸��������                                                          */
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

	Painter::GetInstancePtr()->DrawCircle(rect);
	//switch (m_nShape)
	//{
	//default:
	//	assert(0);
	//	break;
	//case obj_Shape_circle:
	//	//DrawCircle(cdc, rect);
	//	Painter::GetInstancePtr()->DrawCircle(rect);
	//	//pDC->Circle(rect);
	//	break;
	//}

	cdc->SelectObject(pOldBrush);
	cdc->SelectObject(pOldPen);
}

//void CDrawCircle::Paint(float seconds)
//{
//	return;
//}
void CDrawCircle::InvalObj()
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
	//����
	rect.InflateRect(1, 1); // handles CDrawOleObj objects

	Painter::GetInstancePtr()->InvalidateRect(rect);
	return;
}

//int CDrawCircle::GetHandleCount()
//{
//	assert(this);
//
//	return 	CDrawRect::GetHandleCount();
//}

// returns center of handle in logical coordinates
CPoint CDrawCircle::GetHandle(int nHandle)
{
	assert(this);
	return IControl::GetHandle(nHandle);
}
//��������״
HCURSOR CDrawCircle::GetHandleCursor(int nHandle)
{
	assert(this);
	return IControl::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void CDrawCircle::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	assert(this);
	IControl::MoveHandleTo(nHandle, point, pView);
}

// rect must be in logical coordinates
bool CDrawCircle::Intersects(const CRect& rect)
{
	assert(this);

	CRect rectT = rect;
	rectT.NormalizeRect();

	CRect fixed = m_position;
	///************************************************************************/
	///*�޸��������                                                          */
	///************************************************************************/
	//CDrawView* pwnd = (CDrawView*)Painter::GetInstancePtr()->GetCWnd();
	//if (!pwnd)
	//{
	//	assert(0) ;
	//}
	//pwnd->ClientToDoc(fixed);
	Painter::GetInstancePtr()->ClientToDoc(fixed);
	///************************************************************************/
	///*                                                                      */
	///************************************************************************/
	fixed.NormalizeRect();
	if ((rectT & fixed).IsRectEmpty())
		return FALSE;

	CRgn rgn;
	//switch (m_nShape)
	//{
	//default:
	//	assert(0);
	//	break;
	//case obj_Shape_circle:
		rgn.CreateEllipticRgnIndirect(fixed);
	//	break;
	//}
	return TRUE == rgn.RectInRegion(fixed);
}

CDrawObj* CDrawCircle::Clone()
{
	assert(this);

	CDrawCircle* pClone = new CDrawCircle(m_position);

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
