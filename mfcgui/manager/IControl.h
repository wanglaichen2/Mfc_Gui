
#ifndef _ICONTROL_H
#define _ICONTROL_H

#include <string>
#include <vector>

// ����ؼ�����

class IControl: public IEntity
{
public:
	// ��Դ����ģʽ
	enum LOAD_MODE_ENUM 
	{
		LOADMODE_DEFAULT,	// �븸�ؼ���ͬ��ͬ������
		LOADMODE_ASYNC,		// �첽����
		LOADMODE_SYNC,		// ͬ������
	};

public:
	IControl();
	virtual ~IControl() = 0;

	virtual bool Init(const IVarList& args);
	virtual bool Shut();
		
	// ���ƿؼ�
	virtual void Paint(float seconds);
	
	// �����������Ϣ
	virtual void PrepareDesignInfo(IDesignInfo* di);

	// �Ƿ�������
	virtual bool GetLoadFinish();
    virtual bool GetLoadFinishValue();
	
	// ����
	virtual void SetText(const wchar_t* val);
	virtual const wchar_t* GetText() const;

    // ��ȡ��ʾ�ı�
	virtual void SetShowText(const wchar_t* val);
	virtual const wchar_t* GetShowText() const;

    // ��ȡ���λ��
    virtual POINT GetCaretPos();

	// ����������
	virtual void SetCaretName(const char* name) {}
	virtual const char* GetCaretName() const { return ""; }
	
    // ����Դ����
    virtual void SetDataSource(const char* source);
    virtual const char* GetDataSource() const;

	// ��С�ߴ�
	virtual int GetMinWidth() const { return 4; }
	virtual int GetMinHeight() const { return 4; }
	
	// �Ƿ���Ҫ����TAB��
	virtual bool GetAllowTab() const { return false; }
	// �Ƿ���Ҫ����س���
	virtual bool GetAllowReturn() const { return false; }

	// �Ƿ�����
	virtual bool IsContainer() const { return false; }
	// �ÿؼ�������
	virtual IControl* FindChild(const char* name) const { return NULL; }
	// ����ӿؼ�
	virtual bool AddChild(IControl* child) { return false; }
	// ��ָ���ؼ�֮�����
	virtual bool InsertAfterChild(IControl* child, 
		IControl* after) { return false; }
	// ��ָ���ؼ�֮ǰ����
	virtual bool InsertBeforeChild(IControl* child, 
		IControl* before) { return false; }
	// �Ƴ��ӿؼ�
	virtual bool RemoveChild(IControl* child) { return false; }
	// ����ӿؼ�
	virtual void ClearChild() {}
	// ɾ�������ӿؼ�
	virtual void DeleteAllChild() {}
	// ���ӿؼ��ŵ����ZOrder��
	virtual bool SendToBack(IControl* child) { return false; }
	// ���ӿؼ��ŵ���ǰ��ZOrder��
	virtual bool BringToFront(IControl* child) { return false; }
	// �Ƿ��ӿؼ�
	virtual bool IsChildControl(IControl* child) const { return false; }
	// ����ӿؼ�����
	virtual size_t GetChildCount() const { return 0; }
	// ���ָ��λ�õ��ӿؼ�
	virtual IControl* GetChildByIndex(size_t index) const { return NULL; }
	// ����ӿؼ�ID�б�
	virtual size_t GetChildList(IVarList& result) const { return 0; }
	// ��Ҫ��ʾ�����ϲ�Ŀؼ�
	virtual bool FindTopLayer(const PERSISTID& id) { return false; }
	virtual bool AddTopLayer(const PERSISTID& id) { return false; }
	virtual bool RemoveTopLayer(const PERSISTID& id) { return false; }
    // ɾ���������ϲ�Ŀؼ�
    virtual void DeleteAllTopLayer() {}
	// �ص�
	// ������
	virtual int OnKeyDown(unsigned int vk);
	// ��̧��
	virtual int OnKeyUp(unsigned int vk);
	// ����������
	virtual int OnLButtonDown(int x, int y);
	// ������̧��
	virtual int OnLButtonUp(int x, int y);
	// ������˫��
	virtual int OnLButtonDoubleClick(int x, int y);
	// ��������������
	virtual int OnLButtonHover(int x, int y);
	// ����Ҽ�����
	virtual int OnRButtonDown(int x, int y);
	// ����Ҽ�̧��
	virtual int OnRButtonUp(int x, int y);
	// ����Ҽ�˫��
	virtual int OnRButtonDoubleClick(int x, int y);
	// ����Ҽ���������
	virtual int OnRButtonHover(int x, int y);
	// ������
	virtual int OnMouseWheel(int x, int y, int delta);
	// ����ƶ�
	virtual int OnMouseMove(int x, int y);
	// �϶�
	virtual int OnDragMove(int x, int y);
	// �϶�����
	virtual int OnDropIn(int x, int y);
	// �����϶�
	virtual int OnDragLeave(int x, int y);
	// �����ַ�
	virtual int OnChar(unsigned int ch);
	// ������뽹��
	virtual int OnGetFocus(const PERSISTID& lost_id);
	// ʧȥ���뽹��
	virtual int OnLostFocus(const PERSISTID& get_id);
	// ����ӿؼ�
	virtual int OnAdd(const PERSISTID& child_id);
	// �Ƴ��ӿؼ�
	virtual int OnRemove(const PERSISTID& child_id);
	// ���븸�ؼ�
	virtual int OnEnter(const PERSISTID& parent_id);
	// �뿪���ؼ�
	virtual int OnLeave(const PERSISTID& parent_id);
	// �����꽹��
	virtual int OnGetCapture(const PERSISTID& lost_id);
	// ʧȥ��꽹��
	virtual int OnLostCapture(const PERSISTID& get_id);
	// �����ؼ��¼�
	virtual int OnSuborEvent(IControl* subor, 
		int event, const IVarList& args);
	// ���Ͻ�����ı�
	virtual int OnLeftChanged(int old);
	virtual int OnTopChanged(int old);
	// �ߴ�ı�
	virtual int OnWidthChanged(int old);
	virtual int OnHeightChanged(int old);
	// ���ø�����ʾ
	virtual int OnSetHint(IControl* hint, int x, int y);
	// ���������Ĳ˵�
	virtual int OnSetContext(IControl* context);
	// ���������״
	virtual int OnSetCursor(int x, int y);
	// �������λ���ĸ��ؼ���
	virtual IControl* OnGetInWhat(int x, int y);
    // �ж�ָ��λ���Ƿ�͸��
    virtual bool TestTransparent(int x, int y);

	// �ؼ���
	void SetName(const char* val);
	const char* GetName() const;
	unsigned int GetHash() const;
	
	// ����ڸ��������Ե�ľ���
	void SetLeft(int val);
	int GetLeft() const { return m_nLeft; }

	// ����ڸ������ϱ�Ե�ľ���
	void SetTop(int val);
	int GetTop() const { return m_nTop; }
	
	// �ؼ����
	int GetWidth() const;
	void SetWidth(int val);

	// �ؼ��߶�
	int GetHeight() const;
	void SetHeight(int val);

	// ����ê�����ʵ�ʾ�������λ��
	void SetAbsLeft(int val);
	int GetAbsLeft() const;
	void SetAbsTop(int val);
	int GetAbsTop() const;

	// ǰ��ɫ,����ɫ
	void SetForeColor(unsigned int val);
	unsigned int GetForeColor() const;

	// ����ɫ
	void SetBackColor(unsigned int val);
	unsigned int GetBackColor() const;

	// ����ɫ
	void SetLineColor(unsigned int val);
	unsigned int GetLineColor() const;

	// ��ɫ
	void SetBlendColor(unsigned int val);
	unsigned int GetBlendColor() const;

	// ������Ӱɫ/���ɫ
	void SetShadowColor(unsigned int val);
	unsigned int GetShadowColor() const;

	// ����/��ֹ
	void SetEnabled(bool val) { m_bEnabled = val; }
	bool GetEnabled() const { return m_bEnabled; }

	// �Ƿ���ʾ
	void SetVisible(bool val) { m_bVisible = val; }
	bool GetVisible() const { return m_bVisible; }

	// �Ƿ���Խ�������
	void SetCanFocus(bool val) { m_bCanFocus = val; }
	bool GetCanFocus() const { return m_bCanFocus; }

	// �Ƿ�ɱ�ѡ��
	void SetCanSelect(bool val) { m_bCanSelect = val; }
	bool GetCanSelect() const { return m_bCanSelect; }
	
	// ��ǰ�Ƿ�ӵ�����뽹��
	void SetFocused(bool val) { m_bFocused = val; }
	bool GetFocused() const { return m_bFocused; } 

	// �Ƿ��Ѳ������
	void SetCapture(bool val) { m_bCapture = val; }
	bool GetCapture() const { return m_bCapture; }

	// ������
	virtual void SetFontName(const char* val);
	const char* GetFontName() const;

	// �����
	void SetCursorName(const char* val);
	const char* GetCursorName() const;
	
	// ������ʾ����
	void SetHintText(const wchar_t* value);
	const wchar_t* GetHintText() const;
    // ��ȡת�����,��ΪHintText��������Ǹ�IDNAME
    const wchar_t* GetRealHintText() const;
	
	// ������ʾ����
	void SetHintType(const char* value);
	const char* GetHintType() const;

	// �����Ĳ˵�
	void SetContext(const char* value);
	const char* GetContext() const;

	// �������ݶ���
	void SetDataID(const PERSISTID& value);
	PERSISTID GetDataID() const;

	// �Ƿ��첽����
	void SetAsyncLoad(bool value);
	bool GetAsyncLoad() const;

	// TAB���ƶ���˳��
	void SetTabIndex(int value) { m_nTabIndex = value; }
	int GetTabIndex() const { return m_nTabIndex; }
	
	// �Ƿ����TAB�����뽹���ƶ����˿ؼ�
	void SetTabStop(bool value) { m_bTabStop = value; }
	bool GetTabStop() const { return m_bTabStop; }

	// �Ƿ������״̬
	void SetDesignMode(bool value) { m_bDesignMode = value; }
	bool GetDesignMode() const { return m_bDesignMode; }

	// �Ƿ��б߿�
	void SetNoFrame(bool value) { m_bNoFrame = value; }
	bool GetNoFrame() const { return m_bNoFrame; }
	
	// �Ƿ��Զ�������С
	void SetAutoSize(bool value);
	bool GetAutoSize() const;
	
	// ����ͼƬ��Ⱦģʽ
	void SetDrawModeString(const char* value);
	std::string GetDrawModeString() const;
	int GetDrawMode() const;
	
	// ����ͼƬ
	void SetBackImage(const char* value);
	std::string GetBackImage() const;

	// ǰ��ɫ�ַ���
	void SetForeColorString(const char* val);
	std::string GetForeColorString() const;

	// ����ɫ�ַ���
	void SetBackColorString(const char* val);
	std::string GetBackColorString() const;

	// ����ɫ�ַ���
	void SetBlendColorString(const char* val);
	std::string GetBlendColorString() const;

	// ������Ӱɫ/���ɫ
	void SetShadowColorString(const char* val);
	std::string GetShadowColorString() const;

    // �����ؼ���͸����
    void SetAlpha(int val);
    int GetAlpha() const;

	// �߾�ɫ�ַ���
	void SetLineColorString(const char* val);
	std::string GetLineColorString() const;

	// ��ֱê����ʽ
	void SetVAnchorString(const char* val);
	std::string GetVAnchorString() const;

	// ˮƽê����ʽ
	void SetHAnchorString(const char* val);
	std::string GetHAnchorString() const;

    // ͸������,����ĳЩ�ؼ�͸������Ҳ��Ӧ��Ϣ
    void SetTestTrans(bool bTest);
    bool GetTestTrans();

	// ����
	void SetGui(Gui* value) { m_pGui = value; }
	Gui* GetGui() const { return m_pGui; }
	
	// ��������
	void SetParentControl(IControl* value) { m_pParent = value; }
	IControl* GetParentControl() const { return m_pParent;	}
	
	// ������Ϣί������
	void SetDelegateControl(IControl* value) { m_pDelegate = value; }
	IControl* GetDelegateControl() const {	return m_pDelegate;	}

    // ������������
    void SetInSound(const char* value);
    const char* GetInSound() const;

    // ����Ƴ�������
    void SetOutSound(const char* value);
    const char* GetOutSound() const;

    // �����������
    void SetClickSound(const char* value);
    const char* GetClickSound() const;

	// ���ؿؼ���xml�ļ���
	void SetXmlFileName(const char* value);
	const char* GetXmlFileName() const;

	// ���ؿؼ���xml�ļ���·��
	void SetXmlPathName(const char* value);
	const char* GetXmlPathName() const;

	// �Ƿ�ָ�����������
	bool IsAncestorOf(IControl* pControl);
	
	// ���Ƿ��ڿؼ���Χ��
    bool InRectangle(int x, int y) const;
	
	// ��õ����ĸ��ؼ���
	IControl* GetInWhat(int x, int y);
	
	// ��ö���������FORM��
	IControl* GetRootControl();
	// ������������FORM��
	PERSISTID GetRootID();

    // ��ȡ�˿ؼ���ֱ�Ӹ����洰��Form
    PERSISTID GetParentFormID();

	// ������������
	PERSISTID GetParentID() const;
	// ��ö��������ؼ�
	IControl* GetMasterControl();
	// ��ö�����������
	PERSISTID GetMasterID();
	// �����������
	PERSISTID GetDelegateID();
	// ���ݶ���Ż�ÿؼ�
	IControl* GetControl(const PERSISTID& id) const;
	
	// ������ͼ�Ƿ���Ч
	bool HasBackImage();
	// �ͷű�����ͼ
	void RelaseBackImage();

	// ���������ؼ�
	IControl* CreateSuborControl(IControl* parent, const char* name);
    // ɾ�������ؼ�
    bool RemoveSuborControl(IControl* pControl);

    // ��ȡm_nInnerAbsLeft,m_nInnerAbsTop
    void SetRefreshInnerAbsPos(bool value){ m_bRefreshInnerAbsPos = value; }
    bool GetRefreshInnerAbsPos(){ return m_bRefreshInnerAbsPos; }
protected:
    int InnerGetAbsLeft() const;
    int InnerGetAbsTop() const;

	// ��û�����
	IPainter* GetPainter() const
	{
		return m_pPainter;
	}
	
	// ���ó�ʼ�ߴ�
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

	// ��������ͼ
	bool DrawBackImage(float seconds, int x1, int y1, int x2, int y2);

    virtual bool RefreshInnerAbsPos();
private:
    bool m_bRefreshInnerAbsPos;
protected:
	// �Ƿ�͸��
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

