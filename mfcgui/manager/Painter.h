//--------------------------------------------------------------------
// 文件名:		Painter.h
// 内  容:		绘图接口
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
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

	//获得鼠标形状
	//virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void SetCapture();
	virtual CWnd* GetCapture();
	virtual void FreeCapture();
	virtual void InvalidateRect(CRect& rect, bool is = false);//这一部分将被重新绘制

	virtual void PatBlt(CPoint point);
	virtual void DrawCircle(CRect rect);

	virtual void DocToClient(CRect& rect);
	virtual void DocToClient(CPoint& point);
	virtual void ClientToDoc(CPoint& point);
	virtual void ClientToDoc(CRect& rect);

	COLORREF GetPaperColor() const { return m_paperColor; }

	//静态成员函数,提供全局访问的接口
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

	bool m_bActive;//显示活动
};

inline Painter::~Painter(){}

extern Painter* g_pPainter;

#endif // __PAINTER_H__
