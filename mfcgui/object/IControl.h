//--------------------------------------------------------------------
// �ļ���:		IControl.h
// ��  ��:		ͼԪ����
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
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
	virtual void InvalObj();//ˢ�¾�������

	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual bool Intersects(const CRect& rect);
	virtual CDrawObj* Clone();
};

#endif // __ICONTROL_H__
