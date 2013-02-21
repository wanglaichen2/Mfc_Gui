//--------------------------------------------------------------------
// �ļ���:		CDrawRoundRectangle.h
// ��  ��:		Բ�Ǿ���ͼԪ
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
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
	virtual void InvalObj();//ˢ�¾�������

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
