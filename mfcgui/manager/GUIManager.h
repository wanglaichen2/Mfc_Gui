//--------------------------------------------------------------------
// 文件名:		GUIManager.h
// 内  容:		gui
// 说  明:		
// 创建日期:	2012年11月5日
// 创建人:		陈望来
// 版权所有:	XX公司
//--------------------------------------------------------------------
#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

//#include "../DrawTool.h"
#include <string>
#include <map>
#include <vector>

enum eDrawShape
{
	DRAWSHAPE_TOOL_SELECTION,
	DrawShape_TOOL_LINE,
	DrawShape_TOOL_RECT,
	DrawShape_TOOL_ROUNDRECT,
	DrawShape_TOOL_ELLIPSE,
	DrawShape_TOOL_CIRCLE,
	DrawShape_TOOL_POLY,
};

enum eSelectMode
{
	SELECTMODE_NONE,
	SELECTMODE_NETSELECT,
	SELECTMODE_MOVE,
	SELECTMODE_SIZE
};

enum eRectMode
{
	RECTMODE_NONE,
	RECTMODE_SIZE,
};

enum eDragHandle
{
	DRAGHANDLE_NONE = 0,

	DRAGHANDLE_RIGHT_BOTTOM,//
	DRAGHANDLE_BOTTOM,
	DRAGHANDLE_LEFT_BOTTOM,
	DRAGHANDLE_LEFT,

	DRAGHANDLE_LEFT_TOP,//
	DRAGHANDLE_TOP,
	DRAGHANDLE_RIGHT_TOP,
	DRAGHANDLE_RIGHT,

	DRAGHANDLE_9,

};

class CRectTool;
class CDrawTool;
class CSelectTool;

class GUIManager 
{
	friend CSelectTool; 
	friend CRectTool;
public:
	GUIManager() ;
	virtual ~GUIManager() ;

	CDrawTool* FindTool(eDrawShape drawShape);
	/*bool AddTool(CDrawTool *);*/

	CPoint GetHitLast(){ return m_HitLast;};
	void SetHitLast(CPoint point){ m_HitLast = point;};

	CPoint GetHitCurrent(){ return m_HitCurrent; };
	void SetHitCurrent(CPoint point){ m_HitCurrent = point;};

	//CPoint GetSelectLast(){ return m_SelectLast; };
	//void SetSelectLast(CPoint point){ m_SelectLast = point;};

	//几种选择模式
	eSelectMode GetSelectMode(){ return m_SelectMode; };
	void SetSelectMode(eSelectMode rec){ m_SelectMode = rec;};

	//几种绘图模式
	eRectMode GetRectMode(){return m_RectMode;};
	void SetRectMode(eRectMode rec){ m_RectMode = rec;};
	
	int GetDragHandle(){ return m_DragHandle; };
	void SetDragHandle(int rec){ m_DragHandle = rec;};


	UINT GetDownFlages(){ return m_nDownFlags;};
	void SetDownFlages(UINT downflages){ m_nDownFlags = downflages;};

	eDrawShape GetDrawShape(){ return m_drawShape;};
	void SetDrawShape(eDrawShape sharp){ m_drawShape = sharp;};

	//获得对象列表
	CDrawObjLists* GetObjLists() { return &m_ObjectList; }

	//获得几大列表的计数：总对象、选中对象、拷贝对象
	int GetObjectCount(){ return m_objectcount;};
	int GetSelectCount(){ return m_selectcount;};
	int GetCopyCount(){ return m_copycount;};




	void Add(CDrawObj* pObj);
	void AddBack(CDrawObj *pObj);//new添加到后面
	void AddFront(CDrawObj* pObj);//new添加到前面


	void Remove(CDrawObj* pObj);
	void Draw();

	bool RefrshAllObjList();//刷新所有的绘图对象

	//选择列表的处理
	void RemoveSelection(CDrawObj* pObj);
	bool IsSelected(const CDrawObj* pDrawObj);
	//void InvalObj(CDrawObj *pObj); //刷新矩形区域
	bool RefrshSelectionList();//刷新矩形区域
	void CloneSelection();//克隆选中的


	bool Release();
	bool init();
	bool shut();

	bool MsgMouseMove(void* hwnd, unsigned int param1, unsigned int param2, 
		int& result,
		UINT nFlags, CPoint point);
	bool innerSelectMouseMove(UINT nFlags, CPoint point);
	bool innerSelectMouseMoveNetSelect(UINT nFlags, CPoint point);
	bool innerSelectMouseMoveMove(UINT nFlags, CPoint point);
	bool innerMouseMoveSize(UINT nFlags, CPoint point);

	//bool MsgSetCursor(void* hwnd, unsigned int param1, unsigned int param2, 
	//	int& result);
	bool MsgLButtonDown(void* hwnd, unsigned int param1, unsigned int param2, 
		int& result,
		UINT nFlags, CPoint point);
	bool innerLButtonDown(UINT nFlags, CPoint point);

	bool MsgLButtonUp(void* hwnd, unsigned int param1, unsigned int param2, 
		int& result,
		UINT nFlags, CPoint point);
	bool innerLButtonUp(UINT nFlags, CPoint point);

	bool MsgLButtonDblclk(void* hwnd, unsigned int param1, unsigned int param2, 
		int& result,
		UINT nFlags, CPoint point);
	/*bool MsgRButtonDown(void* hwnd, unsigned int param1, unsigned int param2, 
		int& result);
	bool MsgRButtonUp(void* hwnd, unsigned int param1, unsigned int param2, 
		int& result);
	bool MsgRButtonDblclk(void* hwnd, unsigned int param1, unsigned int param2, 
		int& result);
	bool MsgMouseWheel(void* hwnd, unsigned int param1, unsigned int param2, 
		int& result);
	bool MsgMouseHover(void* hwnd, unsigned int param1, unsigned int param2, 
		int& result);*/
	//图元前后移动
	bool MsgObjectMoveback();
	bool MsgObjectMoveforward();
	bool MsgObjectMovetoback();
	bool MsgObjectMovetofront();

	bool DemoData(WPARAM wParam, LPARAM lParam);

	bool MsgSelectAll();							//选择所有
	void MsgCopySelect();							//复制选中对象
	void MsgClearSelect();							//清除选中对象
	void MsgEditPaste();							//粘贴对象
	void MsgEditProperties();						//编辑选中对象当前支持编辑一个对象
	void MsgObjectFillcolor(COLORREF color);        //设置填充颜色
	void MsgObjectLinecolor(COLORREF color);		//设置填充颜色
private:
	bool InnerObjectMovetofront(CDrawObj *pObj);	//前置对象
	bool InnerObjectMovetoback(CDrawObj *pObj);		//后置对象
	bool InnerObjectMoveforward(CDrawObj *pObj);	//前置一位对象
	bool InnerObjectMoveback(CDrawObj *pObj);		//后置一位


	bool Select(CDrawObj* pObj, bool bAdd = false);	//选择某个对象并刷新区域
	void Deselect(CDrawObj* pObj);					//不选择某个对象
	CDrawObj* ObjectAt(const CPoint& point);		//查找某点选中对象
	void SelectWithinRect(CRect rect,bool bAdd = false);//选择区域内图元对象
private:
	CDrawObjLists m_ObjectList;
	int m_objectcount;

	CDrawObjLists m_SelectList;
	int m_selectcount;

	CDrawObjLists m_CopyList;
	int m_copycount;

	CSelectTool* m_SelectTools;//工具

	CRectTool* m_lineTool;
	CRectTool* m_rectTool;
	CRectTool* m_roundRectTool;
	CRectTool* m_ellipseTool;
	CRectTool* m_circleTool;

	CPoint m_HitLast;		//历史焦点
	CPoint m_HitCurrent;	//当前焦点
	UINT m_nDownFlags;
	eDrawShape m_drawShape;

	std::map<int, CDrawTool*> m_ToolMap;//父类存储各种工具


	//选择时候用
	CPoint m_SelectLast;	//历史焦点
	CPoint m_SelectCurrent;	//当前焦点

	eSelectMode m_SelectMode;//选择模式
	eRectMode m_RectMode;//绘图模式

	int m_DragHandle;

};

inline GUIManager::~GUIManager(){}

extern GUIManager* g_pGuiManager;

#endif // __GUIMANAGER_H__
