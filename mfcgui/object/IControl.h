//--------------------------------------------------------------------
// 文件名:		IControl.h
// 内  容:		图元基类
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------
#ifndef __ICONTROL_H__
#define __ICONTROL_H__

class CDrawObj;
class IControl : public CDrawObj
{
protected:
	IControl();
	virtual ~IControl();

public:
	IControl(const CRect& position);
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
};

#endif // __ICONTROL_H__
