//--------------------------------------------------------------------
// 文件名:		IControl.cpp
// 内  容:		
// 说  明:		
// 创建日期:	2007年3月9日
// 创建人:		陆利民
// 版权所有:	苏州蜗牛电子有限公司
//--------------------------------------------------------------------

#include "IControl.h"

// IControl

IControl::IControl()
{
}

IControl::~IControl()
{
}

bool IControl::Init(const IVarList& args)
{
	return true;
}

bool IControl::Shut()
{
    // 为了避免控件被删除后GUI产生的BUG
    if (m_pGui->GetCaptureID() == this->GetID())
    {
        m_pGui->ClearCapture();
    }

	m_BackImage.Release();

	SAFE_RELEASE(m_pData);
	SAFE_RELEASE(m_pPainter);

	return true;
}

IControl* IControl::GetRootControl()
{
	IControl* pControl = this;

	while (pControl->GetParentControl() != NULL)
	{
		pControl = pControl->GetParentControl();
	}

	if (!pControl->GetEntInfo()->IsKindSameSpace(this, "Form"))
	{
		return NULL;
	}

	return pControl;
}

PERSISTID IControl::GetRootID()
{
	IControl* root = GetRootControl();
	
	if (NULL == root)
	{
		return PERSISTID();
	}
	else
	{
		return root->GetID();
	}
}

PERSISTID IControl::GetParentFormID()
{
    IControl* pControl = this;

    while (pControl)
    {
        if (pControl->GetEntInfo()->IsKindSameSpace(this, "Form"))
        {
            return pControl->GetID();
        }

        pControl = pControl->GetParentControl();
    }

    return PERSISTID();

}

PERSISTID IControl::GetParentID() const
{
    if (NULL == m_pParent)
	{
		return PERSISTID();
	}
	else
	{
		return m_pParent->GetID();
	}
}

IControl* IControl::GetMasterControl()
{
	IControl* pControl = this;
	
	while (pControl->GetDelegateControl() != NULL)
	{
		pControl = pControl->GetDelegateControl();
	}
	
	return pControl;
}

PERSISTID IControl::GetMasterID()
{
	return GetMasterControl()->GetID();
}

PERSISTID IControl::GetDelegateID()
{
	if (NULL == m_pDelegate)
	{
		return PERSISTID();
	}
	
	return m_pDelegate->GetID();
}

void IControl::SetName(const char* val)
{
	Assert(val != NULL);

	m_strName = val;
	m_nHash = GetHashValueCase(val);
}

const char* IControl::GetName() const
{
	return m_strName.c_str();
}

unsigned int IControl::GetHash() const
{
	return m_nHash;
}

void IControl::InnerSetLeft(int value)
{
	int old = m_nLeft;

	m_nLeft = value;

    m_bRefreshInnerAbsPos = true;

	OnLeftChanged(old);
}

void IControl::InnerSetTop(int value)
{
	int old = m_nTop;

	m_nTop = value;

    m_bRefreshInnerAbsPos = true;

	OnTopChanged(old);
}

void IControl::InnerSetWidth(int value)
{
	int old = m_nWidth;

	m_nWidth = value;
	
	OnWidthChanged(old);
}

void IControl::InnerSetHeight(int value)
{
	int old = m_nHeight;

	m_nHeight = value;
	
	OnHeightChanged(old);
}

void IControl::SetLeft(int val)
{
	InnerSetLeft(val);
}

void IControl::SetTop(int val)
{
	InnerSetTop(val);
}

void IControl::SetWidth(int val)
{
    if (m_bAutoSize && m_BackImage.IsReady())
    {
        m_nWidth = m_BackImage.GetWidth();
        return;
    }

    // 必须按有符号数比较
    if (val < GetMinWidth())
    {
        val = GetMinWidth();
    }

    InnerSetWidth(val);
}

int IControl::GetWidth() const
{
	return m_nWidth;
}

void IControl::SetHeight(int val)
{
    if (m_bAutoSize && m_BackImage.IsReady())
    {
        m_nHeight = m_BackImage.GetHeight();
        return;
    }

    // 必须按有符号数比较
    if (val < GetMinHeight())
    {
        val = GetMinHeight();
    }

    InnerSetHeight(val);
}

int IControl::GetHeight() const
{
	return m_nHeight;
}

void IControl::SetAbsLeft(int val)
{
	IControl* parent = m_pParent;
	
	if (NULL == parent)
	{
		SetLeft(val);
		return;
	}
	
	switch (m_nHAnchor)
	{
	case CGuiConst::H_ANCHOR_LEFT:
		SetLeft(val - parent->GetAbsLeft());
		break;
	case CGuiConst::H_ANCHOR_CENTER:
		SetLeft(val - (parent->GetAbsLeft() + parent->GetWidth() / 2));
		break;
	case CGuiConst::H_ANCHOR_RIGHT:
		SetLeft(val - (parent->GetAbsLeft() + parent->GetWidth()));
		break;
	default:
		Assert(0);
		break;
	}
}

int IControl::GetAbsLeft() const
{
	IControl* parent = m_pParent;
	
	if (NULL == parent)
	{
		return m_nLeft;
	}

	switch (m_nHAnchor)
	{
	case CGuiConst::H_ANCHOR_LEFT:
		return parent->GetAbsLeft() + m_nLeft;
	case CGuiConst::H_ANCHOR_CENTER:
		return parent->GetAbsLeft() + int(parent->GetWidth() * 0.5f) + m_nLeft;
	case CGuiConst::H_ANCHOR_RIGHT:
		return parent->GetAbsLeft() + parent->GetWidth() + m_nLeft;
	default:
		Assert(0);
		break;
	}

	return 0;
}

void IControl::SetAbsTop(int val)
{
	IControl* parent = m_pParent;
	
	if (NULL == parent)
	{
		SetTop(val);
		return;
	}
	
	switch (m_nVAnchor)
	{
	case CGuiConst::V_ANCHOR_TOP:
		SetTop(val - parent->GetAbsTop());
		break;
	case CGuiConst::V_ANCHOR_CENTER:
		SetTop(val - (parent->GetAbsTop() + int(parent->GetHeight() / 2)));
		break;
	case CGuiConst::V_ANCHOR_BOTTOM:
		SetTop(val - (parent->GetAbsTop() + int(parent->GetHeight())));
		break;
	default:
		Assert(0);
		break;
	}
}

int IControl::GetAbsTop() const
{
	IControl* parent = m_pParent;
	
	if (NULL == parent)
	{
		return m_nTop;
	}

	switch (m_nVAnchor)
	{
	case CGuiConst::V_ANCHOR_TOP:
		return parent->GetAbsTop() + m_nTop;
	case CGuiConst::V_ANCHOR_CENTER:
		return parent->GetAbsTop() + parent->GetHeight() / 2 + m_nTop;
	case CGuiConst::V_ANCHOR_BOTTOM:
		return parent->GetAbsTop() + parent->GetHeight() + m_nTop;
	default:
		Assert(0);
		break;
	}

	return 0;
}

int IControl::InnerGetAbsLeft() const
{
    return m_nInnerAbsLeft;
}

int IControl::InnerGetAbsTop() const
{
    return m_nInnerAbsTop;
}

void IControl::SetForeColor(unsigned int val)
{
	m_ForeColor.SetValue(val);
}

unsigned int IControl::GetForeColor() const
{
	return m_ForeColor.GetValue();
}

void IControl::SetBackColor(unsigned int val)
{
	m_BackColor.SetValue(val);
}

unsigned int IControl::GetBackColor() const
{
	return m_BackColor.GetValue();
}

void IControl::SetLineColor(unsigned int val)
{
	m_LineColor.SetValue(val);
}

unsigned int IControl::GetLineColor() const
{
	return m_LineColor.GetValue();
}

// 混色
void IControl::SetBlendColor(unsigned int val)
{
	m_BlendColor.SetValue(val);
}

unsigned int IControl::GetBlendColor() const
{
	return m_BlendColor.GetValue();
}

void IControl::SetForeColorString(const char* val)
{
	Assert(val != NULL);

	m_ForeColor.SetName(m_pGui, val);
}

std::string IControl::GetForeColorString() const
{
	return m_ForeColor.GetName();
}

void IControl::SetBackColorString(const char* val)
{
	Assert(val != NULL);

	m_BackColor.SetName(m_pGui, val);
}

std::string IControl::GetBackColorString() const
{
	return m_BackColor.GetName();
}

// 背景色字符串
void IControl::SetBlendColorString(const char* val)
{
	Assert(val != NULL);

	m_BlendColor.SetName(m_pGui, val);
}

std::string IControl::GetBlendColorString() const
{
	return m_BlendColor.GetName();
}

// 字体阴影色/描边色
void IControl::SetShadowColor(unsigned int val)
{
	m_ShadowColor.SetValue(val);
}

unsigned int IControl::GetShadowColor() const
{
	return m_ShadowColor.GetValue();
}

// 字体阴影色/描边色
void IControl::SetShadowColorString(const char* val)
{
	Assert(val != NULL);

	m_ShadowColor.SetName(m_pGui, val);
}

std::string IControl::GetShadowColorString() const
{
	return m_ShadowColor.GetName();
}

// 整个控件的透明度
void IControl::SetAlpha(int val)
{
    if (val > 255)
    {
        val = 255;
    }
    else if (val < 0)
    {
        val = 0;
    }
    else
    {
        m_nAlpha = val;
    }
}

int IControl::GetAlpha() const
{
    return m_nAlpha;
}

void IControl::SetLineColorString(const char* val)
{
	Assert(val != NULL);

	m_LineColor.SetName(m_pGui, val);
}

std::string IControl::GetLineColorString() const
{
	return m_LineColor.GetName();
}

void IControl::InnerSetVAnchor(int value)
{
	if (value == m_nVAnchor)
	{
		return;
	}

	int top = GetAbsTop();

	m_nVAnchor = value;

	SetAbsTop(top);
}

void IControl::InnerSetHAnchor(int value)
{
	if (value == m_nHAnchor)
	{
		return;
	}

	int left = GetAbsLeft();
	
	m_nHAnchor = value;

	SetAbsLeft(left);
}

void IControl::SetVAnchorString(const char* val)
{
	if (stricmp(val, "Top") == 0)
	{
		InnerSetVAnchor(CGuiConst::V_ANCHOR_TOP);
	}
	else if (stricmp(val, "Center") == 0)
	{
		InnerSetVAnchor(CGuiConst::V_ANCHOR_CENTER);
	}
	else if (stricmp(val, "Bottom") == 0)
	{
		InnerSetVAnchor(CGuiConst::V_ANCHOR_BOTTOM);
	}
}

std::string IControl::GetVAnchorString() const
{
	switch (m_nVAnchor)
	{
	case CGuiConst::V_ANCHOR_TOP:
		return std::string("Top");
	case CGuiConst::V_ANCHOR_CENTER:
		return std::string("Center");
	case CGuiConst::V_ANCHOR_BOTTOM:
		return std::string("Bottom");
	default:
		Assert(0);
		break;
	}

	return std::string();
}

void IControl::SetHAnchorString(const char* val)
{
	if (stricmp(val, "Left") == 0)
	{
		InnerSetHAnchor(CGuiConst::H_ANCHOR_LEFT);
	}
	else if (stricmp(val, "Center") == 0)
	{
		InnerSetHAnchor(CGuiConst::H_ANCHOR_CENTER);
	}
	else if (stricmp(val, "Right") == 0)
	{
		InnerSetHAnchor(CGuiConst::H_ANCHOR_RIGHT);
	}
}

std::string IControl::GetHAnchorString() const
{
	switch (m_nHAnchor)
	{
	case CGuiConst::H_ANCHOR_LEFT:
		return std::string("Left");
	case CGuiConst::H_ANCHOR_CENTER:
		return std::string("Center");
	case CGuiConst::H_ANCHOR_RIGHT:
		return std::string("Right");
	default:
		Assert(0);
		break;
	}
	
	return std::string();
}

// 透明测试,以免某些控件透明区域也响应消息
void IControl::SetTestTrans(bool bTest)
{
    m_bTestTrans = bTest;
}

bool IControl::GetTestTrans()
{
    return m_bTestTrans;
}

void IControl::SetText(const wchar_t* val)
{
    Assert(val != NULL);

    m_wsText = val;

    if (val[0] == L'@' && (m_wsText.size() > 1))
    {
		if (m_pGui->ConvertText(val, m_wcShowText))
		{
			return;
		}
    }

    m_wcShowText = val;
}

const wchar_t* IControl::GetText() const
{
    return m_wsText.c_str();
}

void IControl::SetShowText(const wchar_t* val)
{
	Assert(val != NULL);
	m_wcShowText = val;
}

const wchar_t* IControl::GetShowText() const
{
    return m_wcShowText.c_str();
}

POINT IControl::GetCaretPos()
{
	POINT pt = { InnerGetAbsLeft(), InnerGetAbsTop() + GetHeight() };

	return pt;
}

void IControl::SetDataSource(const char* source)
{
    if (source == NULL || source[0] == 0)
    {
        m_strDataSource = "";
    }
    else
    {
        m_strDataSource = source;
    }
}

const char* IControl::GetDataSource() const 
{ 
	return m_strDataSource.c_str();
}

void IControl::SetFontName(const char* val)
{
	Assert(val != NULL);

	m_strFontName = val;
}

const char* IControl::GetFontName() const
{
	return m_strFontName.c_str();
}

void IControl::SetCursorName(const char* val)
{
	Assert(val != NULL);

	m_strCursorName = val;
}

const char* IControl::GetCursorName() const
{
	return m_strCursorName.c_str();
}

void IControl::SetHintText(const wchar_t* value)
{
	Assert(value != NULL);
	
    m_wsHintText = value;

    // 先检查是否是IDNAME
    if (m_pGui)
    {
		if (m_pGui->ConvertText(value, m_wcRealHintText))
		{
			return;
		}
        else
        {
            m_wcRealHintText = value;
        }
    }
}

const wchar_t* IControl::GetHintText() const
{
	return m_wsHintText.c_str();
}

const wchar_t* IControl::GetRealHintText() const
{
    return m_wcRealHintText.c_str();
}

void IControl::SetHintType(const char* value)
{
	Assert(value != NULL);

	m_strHintType = value;
}

const char* IControl::GetHintType() const
{
	return m_strHintType.c_str();
}

void IControl::SetContext(const char* value)
{
	Assert(value != NULL);

	m_strContext = value;
}

const char* IControl::GetContext() const
{
	return m_strContext.c_str();
}

void IControl::SetDataID(const PERSISTID& value)
{
	SAFE_RELEASE(m_pData);
	
	m_pData = GetCore()->GetEntity(value);
}

PERSISTID IControl::GetDataID() const
{
	if (NULL == m_pData)
	{
		return PERSISTID();
	}

	return m_pData->GetID();
}

void IControl::SetAsyncLoad(bool value)
{
	if (value)
	{
		m_nLoadMode = LOADMODE_ASYNC;
	}
	else
	{
		m_nLoadMode = LOADMODE_SYNC;
	}
}

bool IControl::GetAsyncLoad() const
{
	if (LOADMODE_DEFAULT == m_nLoadMode)
	{
		if (m_pParent)
		{
			return m_pParent->GetAsyncLoad();
		}

		return false;
	}

	return (m_nLoadMode == LOADMODE_ASYNC);
}

void IControl::SetAutoSize(bool value)
{
	m_bAutoSize = value;

	if (m_bAutoSize && m_BackImage.IsReady())
	{
		InnerSetWidth(m_BackImage.GetWidth());
		InnerSetHeight(m_BackImage.GetHeight());
	}
}

bool IControl::GetAutoSize() const
{
	return m_bAutoSize;
}

void IControl::SetDrawModeString(const char* value)
{
	Assert(value != NULL);

	if (stricmp(value, "Tile") == 0)
	{
		m_nDrawMode = CGuiConst::DRAWMODE_TILE;
	}
	else if (stricmp(value, "Center") == 0)
	{
		m_nDrawMode = CGuiConst::DRAWMODE_CENTER;
	}
	else if (stricmp(value, "FitWindow") == 0)
	{
		m_nDrawMode = CGuiConst::DRAWMODE_FITWINDOW;
	}
	else if (stricmp(value, "Expand") == 0)
	{
		m_nDrawMode = CGuiConst::DRAWMODE_EXPAND;
	}
	else if (stricmp(value, "ExpandH") == 0)
	{
		m_nDrawMode = CGuiConst::DRAWMODE_EXPANDH;
	}
	else if (stricmp(value, "ExpandV") == 0)
	{
		m_nDrawMode = CGuiConst::DRAWMODE_EXPANDV;
	}
}

std::string IControl::GetDrawModeString() const
{
	switch (m_nDrawMode)
	{
	case CGuiConst::DRAWMODE_TILE:
		return std::string("Tile");
	case CGuiConst::DRAWMODE_CENTER:
		return std::string("Center");
	case CGuiConst::DRAWMODE_FITWINDOW:
		return std::string("FitWindow");
	case CGuiConst::DRAWMODE_EXPAND:
		return std::string("Expand");
	case CGuiConst::DRAWMODE_EXPANDH:
		return std::string("ExpandH");
	case CGuiConst::DRAWMODE_EXPANDV:
		return std::string("ExpandV");
	default:
		Assert(0);
		break;
	}

	return std::string();
}

int IControl::GetDrawMode() const
{
	return m_nDrawMode;
}

void IControl::SetBackImage(const char* value)
{
	Assert(value != NULL);

    if (value[0] == 0)
    {
        //m_BackImage.Release();
		RelaseBackImage();
        return;
    }

	m_BackImage.SetName(value);
	m_BackImage.Create(m_pPainter, m_pGui, GetAsyncLoad());
	
	if (m_bAutoSize && m_BackImage.IsReady())
	{
		InnerSetWidth(m_BackImage.GetWidth());
		InnerSetHeight(m_BackImage.GetHeight());
	}
}

std::string IControl::GetBackImage() const
{
	return m_BackImage.GetName();
}

bool IControl::HasBackImage()
{
	return m_BackImage.IsReady();
}

void IControl::RelaseBackImage()
{
	m_BackImage.Release();
	m_BackImage.SetName("");
}

bool IControl::DrawBackImage(float seconds, int x1, int y1, int x2, int y2)
{
    if (m_BackImage.IsReady())
    {
	    return m_BackImage.Draw(m_nDrawMode, x1, y1, x2, y2, GetBlendColor());
    }

    return false;
}

IControl* IControl::CreateSuborControl(IControl* parent, const char* name)
{
	IControl* pControl = (IControl*)GetCore()->CreateEntitySameSpace(
		this, name, CVarList() << m_pGui->GetPainterName() << m_pGui->GetID());

	if (!pControl->GetEntInfo()->IsKindOf("IControl"))
	{
		pControl->Release();
		return NULL;
	}

	pControl->SetParentControl(parent);
	pControl->SetDelegateControl(parent);

	m_Subors.push_back(pControl);
	
	return pControl;
}

bool IControl::RemoveSuborControl(IControl* pControl)
{
	std::vector<IControl*>::iterator it = m_Subors.begin();

    for(; it != m_Subors.end(); ++it)
    {
        if ((*it) == pControl)
        {
            m_Subors.erase(it);
            return true;
        }
    }

    return false;
}

void IControl::Paint(float seconds)
{
}

void IControl::PrepareDesignInfo(IDesignInfo* di)
{
	Assert(di != NULL);

	di->AddPropInfo("Left", IDesignInfo::TYPE_INT, true, false);
	di->AddPropInfo("Top", IDesignInfo::TYPE_INT, true, false);
	di->AddPropInfo("Width", IDesignInfo::TYPE_INT, true, false);
	di->AddPropInfo("Height", IDesignInfo::TYPE_INT, true, false);
	
	di->AddPropInfo("ForeColor", IDesignInfo::TYPE_COLOR, true, true, 
		&CVar(VTYPE_STRING, GuiUtil_IntToColor(
		CGuiConst::DEF_FORE_COLOR).c_str()));
	di->AddPropInfo("BackColor", IDesignInfo::TYPE_COLOR, true, true, 
		&CVar(VTYPE_STRING, GuiUtil_IntToColor(
		CGuiConst::DEF_BACK_COLOR).c_str()));
	di->AddPropInfo("LineColor", IDesignInfo::TYPE_COLOR, true, true, 
		&CVar(VTYPE_STRING, GuiUtil_IntToColor(
		CGuiConst::DEF_LINE_COLOR).c_str()));
	di->AddPropInfo("BlendColor", IDesignInfo::TYPE_COLOR, true, true, 
		&CVar(VTYPE_STRING, GuiUtil_IntToColor(
		CGuiConst::WHITE_COLOR).c_str()));
	di->AddPropInfo("ShadowColor", IDesignInfo::TYPE_COLOR, true, true, 
		&CVar(VTYPE_STRING, GuiUtil_IntToColor(
		CGuiConst::BLACK_COLOR).c_str()));

	di->AddPropInfo("VAnchor", IDesignInfo::TYPE_STRING, true, false, 
		&CVar(VTYPE_STRING, "Top") );
	di->AddPropInfo("HAnchor", IDesignInfo::TYPE_STRING, true, false, 
		&CVar(VTYPE_STRING, "Left") );
	di->AddPropInfo("Enabled", IDesignInfo::TYPE_BOOL, true, false, 
		&CVar(VTYPE_BOOL, true) );
    di->AddPropInfo("TestTrans", IDesignInfo::TYPE_BOOL, true, false);
	
	di->AddPropInfo("Text", IDesignInfo::TYPE_WIDESTR, true, true);
    di->AddPropInfo("DataSource", IDesignInfo::TYPE_STRING, true, true, 
		&CVar(VTYPE_STRING, ""));
	di->AddPropInfo("Font", IDesignInfo::TYPE_FONT, true, true);
	di->AddPropInfo("Cursor", IDesignInfo::TYPE_CURSOR, true, true);
	di->AddPropInfo("HintText", IDesignInfo::TYPE_WIDESTR, true, true);
	di->AddPropInfo("HintType", IDesignInfo::TYPE_STRING, true, true);
	di->AddPropInfo("Context", IDesignInfo::TYPE_STRING, true, true);
	
	di->AddPropInfo("NoFrame", IDesignInfo::TYPE_BOOL, true, true);
	di->AddPropInfo("AutoSize", IDesignInfo::TYPE_BOOL, true, true);
	di->AddPropInfo("DrawMode", IDesignInfo::TYPE_STRING, true, true, 
		&CVar(VTYPE_STRING, "Tile") );
	di->AddPropInfo("BackImage", IDesignInfo::TYPE_IMAGE, true, true);

    //音效配置
    di->AddPropInfo("InSound", IDesignInfo::TYPE_SOUND, true, true);
    di->AddPropInfo("OutSound", IDesignInfo::TYPE_SOUND, true, true);
    di->AddPropInfo("ClickSound", IDesignInfo::TYPE_SOUND, true, true);

	di->AddEventInfo("on_set_hint");
	di->AddEventInfo("on_set_context");
	di->AddEventInfo("on_get_capture");
    di->AddEventInfo("on_lost_capture");
    di->AddEventInfo("on_right_click");
    di->AddEventInfo("on_left_double_click");
}

bool IControl::GetLoadFinish()
{
    if (m_bLoadFinish)
    {
        return true;
    }

	if (!m_BackImage.IsLoadCompelte())
	{
		return false;
	}
	
	if (m_bAutoSize && m_BackImage.IsReady())
	{
		InnerSetWidth(m_BackImage.GetWidth());
		InnerSetHeight(m_BackImage.GetHeight());
	}

    m_bLoadFinish = true;

	return true;
}

bool IControl::GetLoadFinishValue()
{
    return m_bLoadFinish;
}

bool IControl::IsAncestorOf(IControl* pControl)
{
	IControl* parent = pControl->GetParentControl();

	while (parent)
	{
		if (parent == this)
		{
			return true;
		}

		parent = parent->GetParentControl();
	}

	return false;
}

bool IControl::InRectangle(int x, int y) const
{
	int x1 = InnerGetAbsLeft();
	int y1 = InnerGetAbsTop();
	int x2 = x1 + GetWidth();
	int y2 = y1 + GetHeight();
	
	return (x >= x1) && (x < x2) && (y >= y1) && (y < y2);
}

IControl* IControl::GetControl(const PERSISTID& id) const
{
	IEntity* pEntity = GetCore()->GetEntity(id);
	
	if (NULL == pEntity)
	{
		return NULL;
	}
	
	if (!pEntity->GetEntInfo()->IsKindOf("IControl"))
	{
		return NULL;
	}
	
	return (IControl*)pEntity;
}

IControl* IControl::GetInWhat(int x, int y)
{
	if (!GetVisible())
	{
		return NULL;
	}
	
    // 按照windows的处理，禁用时是参于消息处理的，仅是不能使用而已
    /*if (!GetEnabled() && !m_bDesignMode)
    {
        // 禁用时,仅用于显示,不参于消息处理
        return NULL;
    }*/

	// 先判断是否在本控件范围内
	if (!InRectangle(x, y))
	{
		return NULL;
	}
	
	return OnGetInWhat(x, y);
}

int IControl::OnKeyDown(unsigned int vk) 
{ 
	return 0; 
}

int IControl::OnKeyUp(unsigned int vk) 
{ 
	return 0; 
}

int IControl::OnLButtonDown(int x, int y) 
{ 
	return 0; 
}

int IControl::OnLButtonUp(int x, int y) 
{ 
	return 0; 
}

int IControl::OnLButtonDoubleClick(int x, int y) 
{
    return GuiUtil_RunCallback(this, "on_left_double_click",  
		CVarList() << x << y);
}

int IControl::OnLButtonHover(int x, int y)
{
	return 0;
}

int IControl::OnRButtonDown(int x, int y) 
{ 
	return 0; 
}

int IControl::OnRButtonUp(int x, int y) 
{ 
	if (m_pDelegate != NULL)
	{
		m_pDelegate->OnSuborEvent(this, CGuiConst::EVENT_RIGHT_CLICK, 
			CVarList() << x << y);
	}

    return GuiUtil_RunCallback(this, "on_right_click", CVarList() << x << y);
}

int IControl::OnRButtonDoubleClick(int x, int y) 
{ 
	return 0; 
}

int IControl::OnRButtonHover(int x, int y)
{
	return 0;
}

int IControl::OnMouseWheel(int x, int y, int delta)
{
	return 0;
}

int IControl::OnMouseMove(int x, int y) 
{ 
	return 0; 
}

int IControl::OnDragMove(int x, int y) 
{ 
	return 0; 
}

int IControl::OnDropIn(int x, int y) 
{ 
	return 0; 
}

int IControl::OnDragLeave(int x, int y) 
{ 
	return 0; 
}

int IControl::OnChar(unsigned int ch)	
{ 
	return 0; 
}

int IControl::OnGetFocus(const PERSISTID& lost_id) 
{ 
	if (!m_bCanFocus)
	{
		return 0;
	}
	
	if (m_pDelegate != NULL)
	{
		return m_pDelegate->OnSuborEvent(this, 
			CGuiConst::EVENT_GET_FOCUS, CVarList() << lost_id);
	}
	
	return 1; 
}

int IControl::OnLostFocus(const PERSISTID& get_id) 
{ 
	if (!m_bCanFocus)
	{
		return 0;
	}
	
	if (m_pDelegate != NULL)
	{
		return m_pDelegate->OnSuborEvent(this, 
			CGuiConst::EVENT_LOST_FOCUS, CVarList() << get_id);
	}
	
	return 1; 
}

int IControl::OnAdd(const PERSISTID& child_id) 
{ 
	return 0; 
}

int IControl::OnRemove(const PERSISTID& child_id) 
{ 
	return 0; 
}

int IControl::OnEnter(const PERSISTID& parent_id)
{
	return 0;
}

int IControl::OnLeave(const PERSISTID& parent_id)
{
	return 0;
}

int IControl::OnGetCapture(const PERSISTID& lost_id) 
{ 
    if (!m_strInSound.empty())
    {
        // 播放音效
        m_pGui->PlayingSound(m_strInSound.c_str());
    }

    return GuiUtil_RunCallback(this, "on_get_capture", 
		CVarList() << lost_id);
}

int IControl::OnLostCapture(const PERSISTID& get_id) 
{
    if (!m_strOutSound.empty())
    {
        // 播放音效
        m_pGui->PlayingSound(m_strOutSound.c_str());
    }

    return GuiUtil_RunCallback(this, "on_lost_capture", 
        CVarList() << get_id);
}

int IControl::OnSuborEvent(IControl* subor, int event, 
	const IVarList& args) 
{ 
	return 0; 
}

int IControl::OnLeftChanged(int old)
{
	return 0; 
}

int IControl::OnTopChanged(int old)
{
	return 0; 
}

int IControl::OnWidthChanged(int old)
{
	return 0; 
}

int IControl::OnHeightChanged(int old)
{
	return 0; 
}

int IControl::OnSetHint(IControl* hint, int x, int y)
{
	if (m_wsHintText.empty())
	{
		return 0;
	}

	if (m_strHintType.empty())
	{
		return 0;
	}

	return GuiUtil_RunCallback(this, "on_set_hint", 
		CVarList() << hint->GetID() << x << y);
}

int IControl::OnSetContext(IControl* context)
{
	return GuiUtil_RunCallback(this, "on_set_context", 
		CVarList() << context->GetID());
}

int IControl::OnSetCursor(int x, int y)
{
	if (!m_pGui->SetCurrentCursor(GetCursorName()))
	{
		return 0;
	}

	return 1;
}

IControl* IControl::OnGetInWhat(int x, int y)
{
    // 先判断子控件，在子控件上则返回子控件
	if (!m_Subors.empty())
	{
		size_t subor_num = m_Subors.size();
		IControl** subors = &m_Subors[0];

		for (size_t i = 0; i < subor_num; ++i)
		{
			IControl* pControl = subors[i]->GetInWhat(x, y);

			if (pControl != NULL)
			{
				return pControl;
			}
		}
	}
	/*
	std::vector<IControl*>::const_iterator it = m_Subors.begin();
	
	for (; it != m_Subors.end(); ++it)
	{
		IControl* pControl = (*it)->GetInWhat(x, y);
		
		if (pControl != NULL)
		{
			return pControl;
		}
	}
	*/
	
    if (m_bDesignMode)
    {
        // 设计状态
        return this;
    }

    // 不在子控件上判断是否在自已上
    if (!TestTransparent(x, y))
    {
        // 此点不透明才返回
        return this;
    }

	return NULL;
}

// 判断指定位置是否透明
bool IControl::TestTransparent(int x, int y)
{
    if (!m_bTestTrans)
    {
        // 不进行透明测试，则返回不透明
        return false;
    }

    // 默认仅测试背景图，当然具体控件具体分析，
    // 特殊的需要重载TestTransparent函数
    if (!HasBackImage())
    {
        // 如果无图片，则认为不透明
        return false;
    }

    int x1 = InnerGetAbsLeft();
	int y1 = InnerGetAbsTop();
    int x2 = x1 + GetWidth();
    int y2 = y1 + GetHeight();

    // 获取相对于控件左上的未放缩的偏移
	int offset_x = x - x1;
	int offset_y = y - y1;

    int alpha;

    if (!InnerGetBackImage().GetAlpha(GetDrawMode(), 0, 0, 
		GetWidth(), GetHeight(), offset_x, offset_y, alpha))
    {
        return true;
    }

    return (alpha == 0);
}

void IControl::SetInSound(const char* value)
{
    Assert(value != NULL);

    m_strInSound = value;
}

const char* IControl::GetInSound() const
{
    return m_strInSound.c_str();
}

void IControl::SetOutSound(const char* value)
{
    Assert(value != NULL);

    m_strOutSound = value;
}

const char* IControl::GetOutSound() const
{
    return m_strOutSound.c_str();
}

void IControl::SetClickSound(const char* value)
{
    Assert(value != NULL);

    m_strClickSound = value;
}

const char* IControl::GetClickSound() const
{
    return m_strClickSound.c_str();
}

void IControl::SetXmlFileName(const char* value)
{
	Assert(value != NULL);

	m_strXmlFileName = value;
}

const char* IControl::GetXmlFileName() const
{
	return m_strXmlFileName.c_str();
}

void IControl::SetXmlPathName(const char* value)
{
	Assert(value != NULL);

	m_strXmlPathName = value;
}

const char* IControl::GetXmlPathName() const
{
	return m_strXmlPathName.c_str();
}

bool IControl::RefreshInnerAbsPos()
{
    if (m_bRefreshInnerAbsPos)
    {
        m_nInnerAbsLeft = GetAbsLeft();
        m_nInnerAbsTop = GetAbsTop();

        m_bRefreshInnerAbsPos = false;
    }

    return true;
}