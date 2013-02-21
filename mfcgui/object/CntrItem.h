//--------------------------------------------------------------------
// 文件名:		CntrItem.h
// 内  容:		控件基类
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------
#ifndef __CDRAWITEM_H__
#define __CDRAWITEM_H__

class CDrawDoc;
class CDrawView;
class CDrawOleObj;
class CDrawItem : public COleClientItem
{
	//DECLARE_SERIAL(CDrawItem)

// Constructors
public:
	BOOL UpdateExtent();
	CDrawOleObj* m_pDrawObj;
//	CDrawItem(CDrawDoc* pContainer = NULL);
	CDrawItem(CDrawDoc* pContainer = NULL, CDrawOleObj* pDrawObj = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CDrawDoc* GetDocument()
		{ return (CDrawDoc*)COleClientItem::GetDocument(); }
	CDrawView* GetActiveView()
		{ return (CDrawView*)COleClientItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CDrawItem();
//#ifdef _DEBUG
//	virtual void AssertValid() const;
//	virtual void Dump(CDumpContext& dc) const;
//#endif
	//virtual void Serialize(CArchive& ar);
};

#endif // __CDRAWITEM_H__