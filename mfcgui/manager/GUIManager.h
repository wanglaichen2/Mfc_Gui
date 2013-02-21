//--------------------------------------------------------------------
// �ļ���:		GUIManager.h
// ��  ��:		gui
// ˵  ��:		
// ��������:	2012��11��5��
// ������:		������
// ��Ȩ����:	XX��˾
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

	//����ѡ��ģʽ
	eSelectMode GetSelectMode(){ return m_SelectMode; };
	void SetSelectMode(eSelectMode rec){ m_SelectMode = rec;};

	//���ֻ�ͼģʽ
	eRectMode GetRectMode(){return m_RectMode;};
	void SetRectMode(eRectMode rec){ m_RectMode = rec;};
	
	int GetDragHandle(){ return m_DragHandle; };
	void SetDragHandle(int rec){ m_DragHandle = rec;};


	UINT GetDownFlages(){ return m_nDownFlags;};
	void SetDownFlages(UINT downflages){ m_nDownFlags = downflages;};

	eDrawShape GetDrawShape(){ return m_drawShape;};
	void SetDrawShape(eDrawShape sharp){ m_drawShape = sharp;};

	//��ö����б�
	CDrawObjLists* GetObjLists() { return &m_ObjectList; }

	//��ü����б�ļ������ܶ���ѡ�ж��󡢿�������
	int GetObjectCount(){ return m_objectcount;};
	int GetSelectCount(){ return m_selectcount;};
	int GetCopyCount(){ return m_copycount;};




	void Add(CDrawObj* pObj);
	void AddBack(CDrawObj *pObj);//new��ӵ�����
	void AddFront(CDrawObj* pObj);//new��ӵ�ǰ��


	void Remove(CDrawObj* pObj);
	void Draw();

	bool RefrshAllObjList();//ˢ�����еĻ�ͼ����

	//ѡ���б�Ĵ���
	void RemoveSelection(CDrawObj* pObj);
	bool IsSelected(const CDrawObj* pDrawObj);
	//void InvalObj(CDrawObj *pObj); //ˢ�¾�������
	bool RefrshSelectionList();//ˢ�¾�������
	void CloneSelection();//��¡ѡ�е�


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
	//ͼԪǰ���ƶ�
	bool MsgObjectMoveback();
	bool MsgObjectMoveforward();
	bool MsgObjectMovetoback();
	bool MsgObjectMovetofront();

	bool DemoData(WPARAM wParam, LPARAM lParam);

	bool MsgSelectAll();							//ѡ������
	void MsgCopySelect();							//����ѡ�ж���
	void MsgClearSelect();							//���ѡ�ж���
	void MsgEditPaste();							//ճ������
	void MsgEditProperties();						//�༭ѡ�ж���ǰ֧�ֱ༭һ������
	void MsgObjectFillcolor(COLORREF color);        //���������ɫ
	void MsgObjectLinecolor(COLORREF color);		//���������ɫ
private:
	bool InnerObjectMovetofront(CDrawObj *pObj);	//ǰ�ö���
	bool InnerObjectMovetoback(CDrawObj *pObj);		//���ö���
	bool InnerObjectMoveforward(CDrawObj *pObj);	//ǰ��һλ����
	bool InnerObjectMoveback(CDrawObj *pObj);		//����һλ


	bool Select(CDrawObj* pObj, bool bAdd = false);	//ѡ��ĳ������ˢ������
	void Deselect(CDrawObj* pObj);					//��ѡ��ĳ������
	CDrawObj* ObjectAt(const CPoint& point);		//����ĳ��ѡ�ж���
	void SelectWithinRect(CRect rect,bool bAdd = false);//ѡ��������ͼԪ����
private:
	CDrawObjLists m_ObjectList;
	int m_objectcount;

	CDrawObjLists m_SelectList;
	int m_selectcount;

	CDrawObjLists m_CopyList;
	int m_copycount;

	CSelectTool* m_SelectTools;//����

	CRectTool* m_lineTool;
	CRectTool* m_rectTool;
	CRectTool* m_roundRectTool;
	CRectTool* m_ellipseTool;
	CRectTool* m_circleTool;

	CPoint m_HitLast;		//��ʷ����
	CPoint m_HitCurrent;	//��ǰ����
	UINT m_nDownFlags;
	eDrawShape m_drawShape;

	std::map<int, CDrawTool*> m_ToolMap;//����洢���ֹ���


	//ѡ��ʱ����
	CPoint m_SelectLast;	//��ʷ����
	CPoint m_SelectCurrent;	//��ǰ����

	eSelectMode m_SelectMode;//ѡ��ģʽ
	eRectMode m_RectMode;//��ͼģʽ

	int m_DragHandle;

};

inline GUIManager::~GUIManager(){}

extern GUIManager* g_pGuiManager;

#endif // __GUIMANAGER_H__
