//--------------------------------------------------------------------
// �ļ���:		DrawObj.h
// ��  ��:		����
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
//--------------------------------------------------------------------
#ifndef __DRAWOBJ_H__
#define __DRAWOBJ_H__

#include <map>
#include <string>
#include <vector>
#include <list>

#include "../manager/Painter.h"

class CDrawView;
class CDrawDoc;

class CDrawObj : public CObject  
{
protected:
	CDrawObj();
public:
	virtual ~CDrawObj();

// Constructors
public:
	CDrawObj(const CRect& position);

// Attributes
	CRect m_position;
	CDrawDoc* m_pDocument; //�������ϰ󶨵��ĵ���ָ��

	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	//CPoint GetHandleRect(int nHandleID);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void SetLineColor(COLORREF color);
	virtual void SetFillColor(COLORREF color);

	// ��û�����
	Painter* GetPainter() const
	{
		return Painter::GetInstancePtr();
	}


	// ���ƿؼ�
	virtual void Draw();
	virtual void Paint(float seconds);
	virtual void InvalObj();//ˢ�¾�������


	enum TrackerState { normal, selected, active };
	virtual void DrawTracker(CDC* pDC, TrackerState state);
	virtual void MoveTo(const CRect& positon, CDrawView* pView = NULL);
	virtual int HitTest(CPoint point, bool bSelected);
	virtual bool Intersects(const CRect& rect);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual void OnOpen(CDrawView* pView);
	virtual void OnEditProperties();
	virtual CDrawObj* Clone();
	virtual void Remove();
	void Invalidate();
	/*virtual void Serialize(CArchive& ar);*/

protected:
	BOOL m_bPen;
	LOGPEN m_logpen;
	BOOL m_bBrush;
	LOGBRUSH m_logbrush;
private:

public:
	int   m_nOrder;
	int   m_nType;
};

// special 'list' class for this application (requires afxtempl.h)
typedef CTypedPtrList<CObList, CDrawObj*> CDrawObjList;

//typedef std::map<int, CDrawObj*> CDrawObjMap;
typedef std::list<CDrawObj*> CDrawObjLists;


#endif // __DRAWOBJ_H__
