//--------------------------------------------------------------------
// �ļ���:		Painter.h
// ��  ��:		��ͼ�ӿ�
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
//--------------------------------------------------------------------
#ifndef __PAINTER_H__
#define __PAINTER_H__

//#include "../DrawTool.h"
#include <string>
#include <map>
#include <vector>


class Painter 
{
public:
	Painter() ;
	virtual ~Painter() ;
	bool init();
	bool shut();

	void SetCDC(CDC * cdc){ m_pCdc = cdc;};
	CDC * GetCDC(){ return m_pCdc;};

	void SetCWnd(CWnd* wnd){m_pCwnd = wnd;};
	CWnd* GetCWnd(){return m_pCwnd;};

	void SetActive(bool active){m_bActive = active;};
	bool GetActive(){return m_bActive;};

	//��������״
	//virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void SetCapture();
	virtual CWnd* GetCapture();
	virtual void FreeCapture();
	virtual void InvalidateRect(CRect& rect, bool is = false);//��һ���ֽ������»���

	virtual void PatBlt(CPoint point);
	virtual void DrawCircle(CRect rect);

	virtual void DocToClient(CRect& rect);
	virtual void DocToClient(CPoint& point);
	virtual void ClientToDoc(CPoint& point);
	virtual void ClientToDoc(CRect& rect);

	COLORREF GetPaperColor() const { return m_paperColor; }

	//��̬��Ա����,�ṩȫ�ַ��ʵĽӿ�
	static Painter* GetInstancePtr()
	{
		if (_instance == 0)
		{
			_instance = new Painter();
		}
		return _instance;
	};
	static Painter  GetInstance()
	{	
		return *GetInstancePtr();
	};

private:
	static Painter * _instance;
	CDC * m_pCdc;
	CWnd* m_pCwnd;


	COLORREF m_paperColor;

	bool m_bActive;//��ʾ�
};

inline Painter::~Painter(){}

extern Painter* g_pPainter;

#endif // __PAINTER_H__
