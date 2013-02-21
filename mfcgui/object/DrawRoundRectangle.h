//--------------------------------------------------------------------
// 文件名:		CDrawRoundRectangle.h
// 内  容:		圆角矩形图元
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------
#ifndef __CDRAWROUNDRECTANGLE_H__
#define __CDRAWROUNDRECTANGLE_H__

class IControl;
class CDrawRoundRectangle : public IControl
{
protected:
	CDrawRoundRectangle();

public:
	CDrawRoundRectangle(const CRect& position);

public:
	/*virtual void Serialize(CArchive& ar);*/
	virtual void Draw();
	//virtual void Paint(float seconds);
	virtual void InvalObj();//刷新矩形区域

	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual bool Intersects(const CRect& rect);
	virtual CDrawObj* Clone();
protected:
	CPoint m_Roundness; // for roundRect corners
};

#endif // __CDRAWROUNDRECTANGLE_H__
