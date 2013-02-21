//--------------------------------------------------------------------
// 文件名:		CDrawRect.h
// 内  容:		测试通用模板
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------
#ifndef __CDRAWELLIPSE_H__
#define __CDRAWELLIPSE_H__

#include "DrawDefine.h"

class IControl;
class CDrawEllipse : public IControl
{
protected:
	CDrawEllipse();

public:
	CDrawEllipse(const CRect& position); 
	// Implementation
public:
	/*virtual void Serialize(CArchive& ar);*/
	virtual void Draw();
	//virtual void Paint(float seconds);
	virtual void InvalObj();//刷新矩形区域

	//virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual bool Intersects(const CRect& rect);
	virtual CDrawObj* Clone();
protected:

};

#endif // __CDRAWELLIPSE_H__
