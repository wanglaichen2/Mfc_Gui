//--------------------------------------------------------------------
// �ļ���:		CDrawRect.h
// ��  ��:		����ͨ��ģ��
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
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
	virtual void InvalObj();//ˢ�¾�������

	//virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual bool Intersects(const CRect& rect);
	virtual CDrawObj* Clone();
protected:

};

#endif // __CDRAWELLIPSE_H__
