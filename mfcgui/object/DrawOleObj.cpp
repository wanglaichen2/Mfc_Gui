//--------------------------------------------------------------------
// 文件名:		DrawOleObj.cpp
// 内  容:		ole控件
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#include "stdafx.h"
#include "DrawObj.h"
#include "DrawOleObj.h"
#include <assert.h>
#include "../drawdoc.h"
#include "../drawvw.h"
#include "cntritem.h"
////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDrawOleObj, CDrawObj, 0)

BOOL CDrawOleObj::c_bShowItems = TRUE;

CDrawOleObj::CDrawOleObj() : m_extent(0,0)
{
	m_pClientItem = NULL;
}

CDrawOleObj::CDrawOleObj(const CRect& position)
: CDrawObj(position), m_extent(0, 0)
{
	m_pClientItem = NULL;
}

CDrawOleObj::~CDrawOleObj()
{
	if (m_pClientItem != NULL)
	{
		m_pClientItem->Release();
		m_pClientItem = NULL;
	}
}

void CDrawOleObj::Remove()
{
	if (m_pClientItem != NULL)
	{
		m_pClientItem->Delete();
		m_pClientItem = NULL;
	}
	CDrawObj::Remove();
}
/*
void CDrawOleObj::Serialize( CArchive& ar )
{
	assert(this);
	//ASSERT_VALID(this);

	CDrawObj::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_extent;
		ar << m_pClientItem;
	}
	else
	{
		ar >> m_extent;
		ar >> m_pClientItem;
		m_pClientItem->m_pDrawObj = this;
	}
}
*/
CDrawObj* CDrawOleObj::Clone(CDrawDoc* pDoc)
{
	assert(this);
	//ASSERT_VALID(this);

	AfxGetApp()->BeginWaitCursor();

	CDrawOleObj* pClone = NULL;
	CDrawItem* pItem = NULL;

	TRY
	{
		// perform a "deep copy" -- need to copy CDrawOleObj and the CDrawItem
		//  that it points to.
		CDrawOleObj* pClone = new CDrawOleObj(m_position);
		CDrawItem* pItem = new CDrawItem(m_pDocument, pClone);
		if (!pItem->CreateCloneFrom(m_pClientItem))
			AfxThrowMemoryException();

		pClone->m_pClientItem = pItem;
		pClone->m_bPen = m_bPen;
		pClone->m_logpen = m_logpen;
		pClone->m_bBrush = m_bBrush;
		pClone->m_logbrush = m_logbrush;

		pClone->m_nOrder = m_nOrder;
		pClone->m_nType = m_nType;

		assert(pClone);
		//ASSERT_VALID(pClone);

		//if (pDoc != NULL)
		//	pDoc->AddBack(pClone);
	}
	CATCH_ALL(e)
	{
		pItem->Delete();
		pClone->m_pClientItem = NULL;
		pClone->Remove();
		AfxGetApp()->EndWaitCursor();

		THROW_LAST();
	}
	END_CATCH_ALL

		AfxGetApp()->EndWaitCursor();

	return pClone;
}

void CDrawOleObj::Draw(CDC* pDC)
{
	assert(this);
	//ASSERT_VALID(this);

	CDrawItem* pItem = m_pClientItem;
	if (pItem != NULL)
	{
		// draw the OLE item itself
		pItem->Draw(pDC, m_position);

		// don't draw tracker in print preview or on printer
		if (!pDC->IsPrinting())
		{
			// use a CRectTracker to draw the standard effects
			CRectTracker tracker;
			tracker.m_rect = m_position;
			pDC->LPtoDP(tracker.m_rect);

			if (c_bShowItems)
			{
				// put correct border depending on item type
				if (pItem->GetType() == OT_LINK)
					tracker.m_nStyle |= CRectTracker::dottedLine;
				else
					tracker.m_nStyle |= CRectTracker::solidLine;
			}

			// put hatching over the item if it is currently open
			if (pItem->GetItemState() == COleClientItem::openState ||
				pItem->GetItemState() == COleClientItem::activeUIState)
			{
				tracker.m_nStyle |= CRectTracker::hatchInside;
			}
			tracker.Draw(pDC);
		}
	}
}

void CDrawOleObj::OnOpen(CDrawView* pView)
{
	AfxGetApp()->BeginWaitCursor();
	m_pClientItem->DoVerb(
#ifndef _MAC		
		GetKeyState(VK_CONTROL) < 0 ? OLEIVERB_OPEN : OLEIVERB_PRIMARY,
#else		
		GetKeyState(VK_OPTION) < 0 ? OLEIVERB_OPEN : OLEIVERB_PRIMARY,
#endif		
		pView);
	AfxGetApp()->EndWaitCursor();
}

void CDrawOleObj::OnEditProperties()
{
	// using COlePropertiesDialog directly means no scaling
	COlePropertiesDialog dlg(m_pClientItem, 100, 100, NULL);

	dlg.DoModal();
}

// position is in logical
void CDrawOleObj::MoveTo(const CRect& position, CDrawView* pView)
{
	assert(this);
	//ASSERT_VALID(this);

	if (position == m_position)
		return;

	// call base class to update position
	CDrawObj::MoveTo(position, pView);

	// update position of in-place editing session on position change
	if (m_pClientItem->IsInPlaceActive())
		m_pClientItem->SetItemRects();
}
////////////////////////////////////////////////////////////////////////////

