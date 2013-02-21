//--------------------------------------------------------------------
// �ļ���:		DrawOleObj.h
// ��  ��:		ole�ؼ�
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
//--------------------------------------------------------------------
#ifndef __CDRAWOLEOBJ_H__
#define __CDRAWOLEOBJ_H__

class CDrawView;
class CDrawDoc;

class CDrawItem;
class CDrawObj;
class CDrawOleObj : public CDrawObj
{
protected:
	DECLARE_SERIAL(CDrawOleObj);
	CDrawOleObj();

public:
	CDrawOleObj(const CRect& position);

	// Implementation
public:
	/*virtual void Serialize(CArchive& ar);*/
	virtual void Draw(CDC* pDC);
	virtual CDrawObj* Clone(CDrawDoc* pDoc);
	virtual void OnOpen(CDrawView* pView);
	virtual void MoveTo(const CRect& positon, CDrawView* pView = NULL);
	virtual void OnEditProperties();
	virtual void Remove();
	virtual ~CDrawOleObj();

	static BOOL c_bShowItems;

	CDrawItem* m_pClientItem;
	CSize m_extent; // current extent is tracked separate from scaled position
};

#endif // __CDRAWOLEOBJ_H__
