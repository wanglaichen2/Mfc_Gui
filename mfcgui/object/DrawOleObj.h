//--------------------------------------------------------------------
// 文件名:		DrawOleObj.h
// 内  容:		ole控件
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
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
