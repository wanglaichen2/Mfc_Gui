
#ifndef _ICONTROL_H
#define _ICONTROL_H

#include <string>
#include <vector>

// 界面控件基类

class IControl: public IEntity
{
public:
	// 资源加载模式
	enum LOAD_MODE_ENUM 
	{
		LOADMODE_DEFAULT,	// 与父控件相同或同步加载
		LOADMODE_ASYNC,		// 异步加载
		LOADMODE_SYNC,		// 同步加载
	};

public:
	IControl();
	virtual ~IControl() = 0;

	virtual bool Init(const IVarList& args);
	virtual bool Shut();
		
	// 绘制控件
	virtual void Paint(float seconds);
	
	// 设置设计器信息
	virtual void PrepareDesignInfo(IDesignInfo* di);

	// 是否加载完成
	virtual bool GetLoadFinish();
    virtual bool GetLoadFinishValue();
	
	// 标题
	virtual void SetText(const wchar_t* val);
	virtual const wchar_t* GetText() const;

    // 获取显示文本
	virtual void SetShowText(const wchar_t* val);
	virtual const wchar_t* GetShowText() const;

    // 获取光标位置
    virtual POINT GetCaretPos();

	// 输入光标名称
	virtual void SetCaretName(const char* name) {}
	virtual const char* GetCaretName() const { return ""; }
	
    // 数据源配置
    virtual void SetDataSource(const char* source);
    virtual const char* GetDataSource() const;

	// 最小尺寸
	virtual int GetMinWidth() const { return 4; }
	virtual int GetMinHeight() const { return 4; }
	
	// 是否需要输入TAB键
	virtual bool GetAllowTab() const { return false; }
	// 是否需要输入回车键
	virtual bool GetAllowReturn() const { return false; }

	// 是否容器
	virtual bool IsContainer() const { return false; }
	// 用控件名查找
	virtual IControl* FindChild(const char* name) const { return NULL; }
	// 添加子控件
	virtual bool AddChild(IControl* child) { return false; }
	// 在指定控件之后插入
	virtual bool InsertAfterChild(IControl* child, 
		IControl* after) { return false; }
	// 在指定控件之前插入
	virtual bool InsertBeforeChild(IControl* child, 
		IControl* before) { return false; }
	// 移除子控件
	virtual bool RemoveChild(IControl* child) { return false; }
	// 清空子控件
	virtual void ClearChild() {}
	// 删除所有子控件
	virtual void DeleteAllChild() {}
	// 将子控件放到最后（ZOrder）
	virtual bool SendToBack(IControl* child) { return false; }
	// 将子控件放到最前（ZOrder）
	virtual bool BringToFront(IControl* child) { return false; }
	// 是否子控件
	virtual bool IsChildControl(IControl* child) const { return false; }
	// 获得子控件数量
	virtual size_t GetChildCount() const { return 0; }
	// 获得指定位置的子控件
	virtual IControl* GetChildByIndex(size_t index) const { return NULL; }
	// 获得子控件ID列表
	virtual size_t GetChildList(IVarList& result) const { return 0; }
	// 需要显示在最上层的控件
	virtual bool FindTopLayer(const PERSISTID& id) { return false; }
	virtual bool AddTopLayer(const PERSISTID& id) { return false; }
	virtual bool RemoveTopLayer(const PERSISTID& id) { return false; }
    // 删除所有最上层的控件
    virtual void DeleteAllTopLayer() {}
	// 回调
	// 键按下
	virtual int OnKeyDown(unsigned int vk);
	// 键抬起
	virtual int OnKeyUp(unsigned int vk);
	// 鼠标左键按下
	virtual int OnLButtonDown(int x, int y);
	// 鼠标左键抬起
	virtual int OnLButtonUp(int x, int y);
	// 鼠标左键双击
	virtual int OnLButtonDoubleClick(int x, int y);
	// 鼠标左键持续按下
	virtual int OnLButtonHover(int x, int y);
	// 鼠标右键按下
	virtual int OnRButtonDown(int x, int y);
	// 鼠标右键抬起
	virtual int OnRButtonUp(int x, int y);
	// 鼠标右键双击
	virtual int OnRButtonDoubleClick(int x, int y);
	// 鼠标右键持续按下
	virtual int OnRButtonHover(int x, int y);
	// 鼠标滚轮
	virtual int OnMouseWheel(int x, int y, int delta);
	// 鼠标移动
	virtual int OnMouseMove(int x, int y);
	// 拖动
	virtual int OnDragMove(int x, int y);
	// 拖动放入
	virtual int OnDropIn(int x, int y);
	// 结束拖动
	virtual int OnDragLeave(int x, int y);
	// 输入字符
	virtual int OnChar(unsigned int ch);
	// 获得输入焦点
	virtual int OnGetFocus(const PERSISTID& lost_id);
	// 失去输入焦点
	virtual int OnLostFocus(const PERSISTID& get_id);
	// 添加子控件
	virtual int OnAdd(const PERSISTID& child_id);
	// 移除子控件
	virtual int OnRemove(const PERSISTID& child_id);
	// 进入父控件
	virtual int OnEnter(const PERSISTID& parent_id);
	// 离开父控件
	virtual int OnLeave(const PERSISTID& parent_id);
	// 获得鼠标焦点
	virtual int OnGetCapture(const PERSISTID& lost_id);
	// 失去鼠标焦点
	virtual int OnLostCapture(const PERSISTID& get_id);
	// 附属控件事件
	virtual int OnSuborEvent(IControl* subor, 
		int event, const IVarList& args);
	// 左上角坐标改变
	virtual int OnLeftChanged(int old);
	virtual int OnTopChanged(int old);
	// 尺寸改变
	virtual int OnWidthChanged(int old);
	virtual int OnHeightChanged(int old);
	// 设置浮动提示
	virtual int OnSetHint(IControl* hint, int x, int y);
	// 设置上下文菜单
	virtual int OnSetContext(IControl* context);
	// 设置鼠标形状
	virtual int OnSetCursor(int x, int y);
	// 获得坐标位于哪个控件中
	virtual IControl* OnGetInWhat(int x, int y);
    // 判断指定位置是否透明
    virtual bool TestTransparent(int x, int y);

	// 控件名
	void SetName(const char* val);
	const char* GetName() const;
	unsigned int GetHash() const;
	
	// 相对于父容器左边缘的距离
	void SetLeft(int val);
	int GetLeft() const { return m_nLeft; }

	// 相对于父容器上边缘的距离
	void SetTop(int val);
	int GetTop() const { return m_nTop; }
	
	// 控件宽度
	int GetWidth() const;
	void SetWidth(int val);

	// 控件高度
	int GetHeight() const;
	void SetHeight(int val);

	// 考虑锚定后的实际绝对坐标位置
	void SetAbsLeft(int val);
	int GetAbsLeft() const;
	void SetAbsTop(int val);
	int GetAbsTop() const;

	// 前景色,字体色
	void SetForeColor(unsigned int val);
	unsigned int GetForeColor() const;

	// 背景色
	void SetBackColor(unsigned int val);
	unsigned int GetBackColor() const;

	// 线条色
	void SetLineColor(unsigned int val);
	unsigned int GetLineColor() const;

	// 混色
	void SetBlendColor(unsigned int val);
	unsigned int GetBlendColor() const;

	// 字体阴影色/描边色
	void SetShadowColor(unsigned int val);
	unsigned int GetShadowColor() const;

	// 允许/禁止
	void SetEnabled(bool val) { m_bEnabled = val; }
	bool GetEnabled() const { return m_bEnabled; }

	// 是否显示
	void SetVisible(bool val) { m_bVisible = val; }
	bool GetVisible() const { return m_bVisible; }

	// 是否可以接受输入
	void SetCanFocus(bool val) { m_bCanFocus = val; }
	bool GetCanFocus() const { return m_bCanFocus; }

	// 是否可被选择
	void SetCanSelect(bool val) { m_bCanSelect = val; }
	bool GetCanSelect() const { return m_bCanSelect; }
	
	// 当前是否拥有输入焦点
	void SetFocused(bool val) { m_bFocused = val; }
	bool GetFocused() const { return m_bFocused; } 

	// 是否已捕获鼠标
	void SetCapture(bool val) { m_bCapture = val; }
	bool GetCapture() const { return m_bCapture; }

	// 字体名
	virtual void SetFontName(const char* val);
	const char* GetFontName() const;

	// 光标名
	void SetCursorName(const char* val);
	const char* GetCursorName() const;
	
	// 浮动提示文字
	void SetHintText(const wchar_t* value);
	const wchar_t* GetHintText() const;
    // 获取转换后的,因为HintText本身可能是个IDNAME
    const wchar_t* GetRealHintText() const;
	
	// 浮动提示类型
	void SetHintType(const char* value);
	const char* GetHintType() const;

	// 上下文菜单
	void SetContext(const char* value);
	const char* GetContext() const;

	// 关联数据对象
	void SetDataID(const PERSISTID& value);
	PERSISTID GetDataID() const;

	// 是否异步加载
	void SetAsyncLoad(bool value);
	bool GetAsyncLoad() const;

	// TAB键移动的顺序
	void SetTabIndex(int value) { m_nTabIndex = value; }
	int GetTabIndex() const { return m_nTabIndex; }
	
	// 是否可用TAB将输入焦点移动到此控件
	void SetTabStop(bool value) { m_bTabStop = value; }
	bool GetTabStop() const { return m_bTabStop; }

	// 是否处于设计状态
	void SetDesignMode(bool value) { m_bDesignMode = value; }
	bool GetDesignMode() const { return m_bDesignMode; }

	// 是否有边框
	void SetNoFrame(bool value) { m_bNoFrame = value; }
	bool GetNoFrame() const { return m_bNoFrame; }
	
	// 是否自动调整大小
	void SetAutoSize(bool value);
	bool GetAutoSize() const;
	
	// 背景图片渲染模式
	void SetDrawModeString(const char* value);
	std::string GetDrawModeString() const;
	int GetDrawMode() const;
	
	// 背景图片
	void SetBackImage(const char* value);
	std::string GetBackImage() const;

	// 前景色字符串
	void SetForeColorString(const char* val);
	std::string GetForeColorString() const;

	// 背景色字符串
	void SetBackColorString(const char* val);
	std::string GetBackColorString() const;

	// 背景色字符串
	void SetBlendColorString(const char* val);
	std::string GetBlendColorString() const;

	// 字体阴影色/描边色
	void SetShadowColorString(const char* val);
	std::string GetShadowColorString() const;

    // 整个控件的透明度
    void SetAlpha(int val);
    int GetAlpha() const;

	// 线景色字符串
	void SetLineColorString(const char* val);
	std::string GetLineColorString() const;

	// 垂直锚定方式
	void SetVAnchorString(const char* val);
	std::string GetVAnchorString() const;

	// 水平锚定方式
	void SetHAnchorString(const char* val);
	std::string GetHAnchorString() const;

    // 透明测试,以免某些控件透明区域也响应消息
    void SetTestTrans(bool bTest);
    bool GetTestTrans();

	// 界面
	void SetGui(Gui* value) { m_pGui = value; }
	Gui* GetGui() const { return m_pGui; }
	
	// 父级容器
	void SetParentControl(IControl* value) { m_pParent = value; }
	IControl* GetParentControl() const { return m_pParent;	}
	
	// 窗口消息委托容器
	void SetDelegateControl(IControl* value) { m_pDelegate = value; }
	IControl* GetDelegateControl() const {	return m_pDelegate;	}

    // 鼠标移入的声音
    void SetInSound(const char* value);
    const char* GetInSound() const;

    // 鼠标移出的声音
    void SetOutSound(const char* value);
    const char* GetOutSound() const;

    // 鼠标点击的声音
    void SetClickSound(const char* value);
    const char* GetClickSound() const;

	// 加载控件的xml文件名
	void SetXmlFileName(const char* value);
	const char* GetXmlFileName() const;

	// 加载控件的xml文件的路径
	void SetXmlPathName(const char* value);
	const char* GetXmlPathName() const;

	// 是否指定对象的祖先
	bool IsAncestorOf(IControl* pControl);
	
	// 点是否在控件范围内
    bool InRectangle(int x, int y) const;
	
	// 获得点在哪个控件中
	IControl* GetInWhat(int x, int y);
	
	// 获得顶层容器（FORM）
	IControl* GetRootControl();
	// 顶级容器对象（FORM）
	PERSISTID GetRootID();

    // 获取此控件的直接父界面窗口Form
    PERSISTID GetParentFormID();

	// 父级容器对象
	PERSISTID GetParentID() const;
	// 获得顶级宿主控件
	IControl* GetMasterControl();
	// 获得顶级宿主对象
	PERSISTID GetMasterID();
	// 获得宿主对象
	PERSISTID GetDelegateID();
	// 根据对象号获得控件
	IControl* GetControl(const PERSISTID& id) const;
	
	// 背景贴图是否有效
	bool HasBackImage();
	// 释放背景贴图
	void RelaseBackImage();

	// 创建附属控件
	IControl* CreateSuborControl(IControl* parent, const char* name);
    // 删除附属控件
    bool RemoveSuborControl(IControl* pControl);

    // 重取m_nInnerAbsLeft,m_nInnerAbsTop
    void SetRefreshInnerAbsPos(bool value){ m_bRefreshInnerAbsPos = value; }
    bool GetRefreshInnerAbsPos(){ return m_bRefreshInnerAbsPos; }
protected:
    int InnerGetAbsLeft() const;
    int InnerGetAbsTop() const;

	// 获得绘制器
	IPainter* GetPainter() const
	{
		return m_pPainter;
	}
	
	// 设置初始尺寸
	void SetDesignSize(int width, int height)
	{
        m_nWidth = width;
        m_nHeight = height;
	}

	void InnerSetLeft(int value);
	void InnerSetTop(int value);
	void InnerSetWidth(int value);
	void InnerSetHeight(int value);

	void InnerSetVAnchor(int value);
	void InnerSetHAnchor(int value);

	CImagePlayer& InnerGetBackImage() { return m_BackImage; }

	// 画背景贴图
	bool DrawBackImage(float seconds, int x1, int y1, int x2, int y2);

    virtual bool RefreshInnerAbsPos();
private:
    bool m_bRefreshInnerAbsPos;
protected:
	// 是否透明
	bool m_bTransparent;
    int m_nInnerAbsLeft;
    int m_nInnerAbsTop;
private:
	Gui* m_pGui;
	IPainter* m_pPainter;
	IControl* m_pParent;
	IControl* m_pDelegate;
	IEntity* m_pData;
	
	CColorData m_ForeColor;
	CColorData m_BackColor;
	CColorData m_LineColor;
	CColorData m_BlendColor;
	CColorData m_ShadowColor;

	std::string m_strName;
	unsigned int m_nHash;
	
	int m_nTop;
	int m_nLeft;
	int m_nWidth;
	int m_nHeight;
    int m_nAlpha;
	int m_nLoadMode;

	bool m_bDesignMode;
	bool m_bEnabled;
	bool m_bVisible;
	bool m_bCanFocus;
	bool m_bCanSelect;
	bool m_bFocused;
	bool m_bCapture;
    bool m_bLoadFinish;

	int m_nVAnchor;
	int m_nHAnchor;

	std::wstring m_wsText;
    std::wstring m_wcShowText;
	std::string m_strFontName;
	std::string m_strCursorName;
	std::wstring m_wsHintText;
    std::wstring m_wcRealHintText;
	std::string m_strHintType;
    std::string m_strContext;
    std::string m_strDataSource;

    std::string m_strInSound;
    std::string m_strOutSound;
    std::string m_strClickSound;

	std::string m_strXmlPathName;
	std::string m_strXmlFileName;

	int m_nTabIndex;
	bool m_bTabStop;
	
	bool m_bNoFrame;
	bool m_bAutoSize;
    bool m_bTestTrans;
	int m_nDrawMode;
	CImagePlayer m_BackImage;

	std::vector<IControl*> m_Subors;
};

#endif // _ICONTROL_H

