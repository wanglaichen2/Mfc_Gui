//--------------------------------------------------------------------
// 文件名:		CDrawRect.h
// 内  容:		测试通用模板
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------
#ifndef __CDRAWLINE_H__
#define __CDRAWLINE_H__

class CDrawObj;
class IControl;
class CDrawLine : public IControl
{
protected:
	CDrawLine();

public:
	CDrawLine(const CRect& position);

	// Implementation
public:
	/*virtual void Serialize(CArchive& ar);*/
	virtual void Draw();
	virtual void InvalObj();//刷新矩形区域

	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual bool Intersects(const CRect& rect);
	virtual CDrawObj* Clone();
protected:

};

#endif // __CDRAWLINE_H__
