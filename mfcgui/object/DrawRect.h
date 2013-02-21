//--------------------------------------------------------------------
// �ļ���:		CDrawRect.h
// ��  ��:		����ͼԪ
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
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
	virtual void InvalObj();//ˢ�¾�������

	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual bool Intersects(const CRect& rect);
	virtual CDrawObj* Clone();
protected:
};

#endif // __CDRAWRECT_H__
