//--------------------------------------------------------------------
// �ļ���:		CDrawRect.h
// ��  ��:		����ͨ��ģ��
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
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
	virtual void InvalObj();//ˢ�¾�������

	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual bool Intersects(const CRect& rect);
	virtual CDrawObj* Clone();
protected:

};

#endif // __CDRAWLINE_H__
