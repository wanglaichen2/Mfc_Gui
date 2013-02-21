//--------------------------------------------------------------------
// �ļ���:		DrawDoc.h
// ��  ��:		MFC�ĵ���
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
//--------------------------------------------------------------------
#ifndef __DRAWDOC_H__
#define __DRAWDOC_H__

#include "object/DrawObj.h"
//class CDrawObj;
 
class CDrawDoc : public COleDocument
{
protected: // create from serialization only
	CSize m_size;
	int m_nMapMode;
	CDrawDoc();
	DECLARE_DYNCREATE(CDrawDoc)

// Attributes
public:
	COLORREF m_paperColor;

// Operations
public:
	void ComputePageSize();
	//void Draw(CDC* pDC,CDrawView* pView);
	const CSize& GetSize() const { return m_size; }
	COLORREF GetPaperColor() const { return m_paperColor; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~CDrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawDoc)
	afx_msg void OnViewPapercolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __DRAWDOC_H__
