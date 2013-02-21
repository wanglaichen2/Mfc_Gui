//--------------------------------------------------------------------
// �ļ���:		CDrawCircle.h
// ��  ��:		Բ�λ���
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
//--------------------------------------------------------------------
#ifndef __CDRAWCIRCLE_H__
#define __CDRAWCIRCLE_H__

#include "DrawDefine.h"

class IControl;
class CDrawCircle : public IControl
{
protected:
	CDrawCircle();

public:
	CDrawCircle(const CRect& position);
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

	CPoint m_roundness; // for roundRect corners

};

#endif // __CDRAWCIRCLE_H__
