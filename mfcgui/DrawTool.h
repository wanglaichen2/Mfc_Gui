//--------------------------------------------------------------------
// 文件名:		DrawTool.h
// 内  容:		工具类
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------

#ifndef __DRAWTOOL_H__
#define __DRAWTOOL_H__

#include "object/drawobj.h"
#include "manager/GUIManager.h"



class CDrawView;
class CDrawTool  
{
// Constructors
public:
	CDrawTool(eDrawShape nDrawShape);

// Overridables
	//virtual void OnLButtonDown(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnLButtonDblClk(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnLButtonUp(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnMouseMove(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnEditProperties(/*CDrawView* pView*/);
	virtual void OnCancel();

// Attributes
	eDrawShape m_drawShape;
};

class CSelectTool : public CDrawTool
{
// Constructors
public:
	CSelectTool();

// Implementation
	//virtual void OnLButtonDown(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnLButtonDblClk(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnLButtonUp(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnMouseMove(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnEditProperties();
};

class CRectTool : public CDrawTool
{
// Constructors
public:
	CRectTool(eDrawShape drawShape);

// Implementation
	//virtual void OnLButtonDown(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnLButtonDblClk(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnLButtonUp(CDrawView* pView, UINT nFlags, const CPoint& point);
	//virtual void OnMouseMove(CDrawView* pView, UINT nFlags, const CPoint& point);
};

#endif // __DRAWTOOL_H__
