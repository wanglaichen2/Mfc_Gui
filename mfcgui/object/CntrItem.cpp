//--------------------------------------------------------------------
// 文件名:		CntrItem.cpp
// 内  容:		控件基类
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------
#include "stdafx.h"
#include "../DrawCli.h"

#include "../DrawDoc.h"
#include "../DrawVw.h"
#include "DrawObj.h"
#include "CntrItem.h"
#include "DrawOleObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawItem implementation

//IMPLEMENT_SERIAL(CDrawItem, COleClientItem, 0)
/*
CDrawItem::CDrawItem(CDrawDoc* pContainer)
	: COleClientItem(pContainer)
{
	// TODO: add one-time construction code here
}
*/
CDrawItem::CDrawItem(CDrawDoc* pContainer, CDrawOleObj* pDrawObj)
	: COleClientItem(pContainer)
{
	m_pDrawObj = pDrawObj;
}

CDrawItem::~CDrawItem()
{
	// TODO: add cleanup code here
	
}

void CDrawItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// When an item is being edited (either in-place or fully open)
	//  it sends OnChange notifications for changes in the state of the
	//  item or visual appearance of its content.

	// TODO: invalidate the item by calling UpdateAllViews
	//  (with hints appropriate to your application)

	GetDocument()->UpdateAllViews(NULL);
		// for now just update ALL views/no hints
}

BOOL CDrawItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// During in-place activation CDrawItem::OnChangeItemPosition
	//  is called by the server to change the position of the in-place
	//  window.  Usually, this is a result of the data in the server
	//  document changing such that the extent has changed or as a result
	//  of in-place resizing.
	//
	// The default here is to call the base class, which will call
	//  COleClientItem::SetItemRects to move the item
	//  to the new position.

	if (!COleClientItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// TODO: update any cache you may have of the item's rectangle/extent

	return TRUE;
}

void CDrawItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// During in-place activation, CDrawItem::OnGetItemPosition
	//  will be called to determine the location of this item.  The default
	//  implementation created from AppWizard simply returns a hard-coded
	//  rectangle.  Usually, this rectangle would reflect the current
	//  position of the item relative to the view used for activation.
	//  You can obtain the view by calling CDrawItem::GetActiveView.

	// TODO: return correct rectangle (in pixels) in rPosition

	rPosition.SetRect(10, 10, 210, 210);
}

void CDrawItem::OnActivate()
{
    // Allow only one inplace activate item per frame
    CDrawView* pView = GetActiveView();
    ASSERT_VALID(pView);
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != NULL && pItem != this)
        pItem->Close();
    
    COleClientItem::OnActivate();
}

void CDrawItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

    // Hide the object if it is not an outside-in object
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}
/*
void CDrawItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// Call base class first to read in COleClientItem data.
	// Since this sets up the m_pDocument pointer returned from
	//  CDrawItem::GetDocument, it is a good idea to call
	//  the base class Serialize first.
	COleClientItem::Serialize(ar);

	// now store/retrieve data specific to CDrawItem
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
*/
/////////////////////////////////////////////////////////////////////////////
// CDrawItem diagnostics

//#ifdef _DEBUG
//void CDrawItem::AssertValid() const
//{
//	COleClientItem::AssertValid();
//}
//
//void CDrawItem::Dump(CDumpContext& dc) const
//{
//	COleClientItem::Dump(dc);
//}
//#endif

/////////////////////////////////////////////////////////////////////////////
BOOL CDrawItem::UpdateExtent()
{
	CSize size;
	if (!GetExtent(&size) || size == m_pDrawObj->m_extent)
		return FALSE;       // blank

	// if new object (i.e. m_extent is empty) setup position
	if (m_pDrawObj->m_extent == CSize(0, 0))
	{
		m_pDrawObj->m_position.right =
			m_pDrawObj->m_position.left + MulDiv(size.cx, 10, 254);
		m_pDrawObj->m_position.bottom =
			m_pDrawObj->m_position.top - MulDiv(size.cy, 10, 254);
	}
	// else data changed so scale up rect as well
	else if (!IsInPlaceActive() && size != m_pDrawObj->m_extent)
	{
		m_pDrawObj->m_position.right = m_pDrawObj->m_position.left +
			MulDiv(m_pDrawObj->m_position.Width(), size.cx, m_pDrawObj->m_extent.cx);
		m_pDrawObj->m_position.bottom = m_pDrawObj->m_position.top +
			MulDiv(m_pDrawObj->m_position.Height(), size.cy, m_pDrawObj->m_extent.cy);
	}

	m_pDrawObj->m_extent = size;
	m_pDrawObj->Invalidate();   // redraw to the new size/position
	return TRUE;
}
