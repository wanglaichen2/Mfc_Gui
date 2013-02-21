//--------------------------------------------------------------------
// 文件名:		DrawVw.h
// 内  容:		MFC视图
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------
#ifndef __DRAWVW_H__
#define __DRAWVW_H__

#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_DRAWOBJ     1
#define HINT_UPDATE_SELECTION   2
#define HINT_DELETE_SELECTION   3
#define HINT_UPDATE_OLE_ITEMS   4

class CDrawItem;

class CDrawView : public CScrollView
{
protected: // create from serialization only
	CDrawView();
	DECLARE_DYNCREATE(CDrawView)

// Attributes
public:
	//BOOL m_bActive;//显示活动
	COLORREF m_gridColor;
	BOOL m_bGrid;  //显示网格

	CDrawObjList m_selection;
	afx_msg LRESULT DemoData(WPARAM wParam, LPARAM lParam);
	CDrawDoc* GetDocument();
	// m_pSelection holds the selection to the current CDrawItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CDrawItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
//	CDrawItem* m_pSelection;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	//virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	//void PasteEmbedded(COleDataObject& dataObject, CPoint point);
	void PasteNative(COleDataObject& dataObject);
	virtual void OnActivateView(BOOL bActivate,CView* pActiveView,CView* pDeactiveView);
	CRect GetInitialPosition();
 	void SetPageSize(CSize size);
	void DrawGrid(CDC* pDC);

	//void Deselect(CDrawObj* pObj);
	//void CloneSelection();
	//void Select(CDrawObj* pObj,bool bAdd = false);
	void DocToClient(CRect& rect);
	void DocToClient(CPoint& point);
	void ClientToDoc(CPoint& point);
	void ClientToDoc(CRect& rect);
	void InvalObj(CDrawObj* pObj);
	virtual ~CDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
static CLIPFORMAT m_cfDraw;
static CLIPFORMAT m_cfObjectDescriptor;

protected:

// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CDrawView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnDrawSelect();
	afx_msg void OnDrawRect();
	//更新ui
	afx_msg void OnUpdateDrawSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawRect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawCircle(CCmdUI* pCmdUI);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawEllipse();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawRoundrect();
	afx_msg void OnEditProperties();
	afx_msg void OnObjectMoveback();
	afx_msg void OnObjectMoveforward();
	afx_msg void OnObjectMovetoback();
	afx_msg void OnObjectMovetofront();
	afx_msg void OnUpdateObjectMoveback(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjectMoveforward(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjectMovetoback(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjectMovetofront(CCmdUI* pCmdUI);
	afx_msg void OnObjectFillcolor();
	afx_msg void OnObjectLinecolor();
	afx_msg void OnUpdateObjectFillcolor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjectLinecolor(CCmdUI* pCmdUI);
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	afx_msg void OnViewShowobjects();
	afx_msg void OnUpdateViewShowobjects(CCmdUI* pCmdUI);
	afx_msg void OnEditClear();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in DrawVw.cpp
inline CDrawDoc* CDrawView::GetDocument()
   { return (CDrawDoc*)m_pDocument; }
#endif

#endif // __DRAWVW_H__
