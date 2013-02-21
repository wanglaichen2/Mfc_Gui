//--------------------------------------------------------------------
// 文件名:		CDrawRect.h
// 内  容:		矩形图元
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------
#ifndef __CDRAWRECT_H__
#define __CDRAWRECT_H__


#include "IControl.h"
class IControl;
class CDrawRect : public IControl
{
protected:
	CDrawRect();

public:
	CDrawRect(const CRect& position);
public:
	/*virtual void Serialize(CArchive& ar);*/
	virtual void Draw();
	virtual void Paint(float seconds);
	virtual void InvalObj();//刷新矩形区域

	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual bool Intersects(const CRect& rect);
	virtual CDrawObj* Clone();
protected:
};

#endif // __CDRAWRECT_H__
