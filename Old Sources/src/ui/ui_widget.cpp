#include "ui_widget.h"

#include "ui_controller.h"
#include "util\time_utils.h"
#include "render_system\render_system.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UIWidget);

UIWidget::UIWidget(const layout& layout, const String& id/* = ""*/):
mId(id), mLayout(layout), mGeometry(NULL), mVisible(true), mFocused(false), mBasicTransparency(1.0f), mParent(NULL),
mVisibleState(NULL), mUpdatedAtFrame(0), mProcessedInputAtFrame(0), mDrawedAtFrame(0), mCursorInside(false),
mChildsLayout(layout::both()), mAbsMinSize(0, 0), mResizeByChilds(false)
{
	mLayout = layout;
	mCheckParentTransparency = callback<UIWidget>(this, &UIWidget::UpdateResTransparency);
	InitializeProperties();
	UpdateLayout();
}

UIWidget::UIWidget(const UIWidget& widget):
mUpdatedAtFrame(0), mProcessedInputAtFrame(0), mDrawedAtFrame(0), mCursorInside(false), mVisibleState(NULL)
{
	InitializeProperties();

	mCheckParentTransparency = callback<UIWidget>(this, &UIWidget::UpdateResTransparency);

	mId = widget.mId;
	mLayout = widget.mLayout;
	mChildsLayout = widget.mChildsLayout;
	mParent = NULL;
	mChildsOffset = widget.mChildsOffset;
	mVisible = widget.mVisible;
	mFocused = false;
	mBasicTransparency = widget.mBasicTransparency;
	mFixedMinSize = widget.mFixedMinSize;
	mAbsMinSize = widget.mAbsMinSize;
	mResizeByChilds = widget.mResizeByChilds;

	FOREACH_CONST(WidgetsVec, widget.mChildWidgets, it)
		AddChild((*it)->Clone());

	FOREACH_CONST(StatesMap, widget.mStates, state)
		AddState(state->second->Clone());

	UpdateLayout();
}

UIWidget::~UIWidget()
{
	RemoveAllChilds();

	FOREACH(StatesMap, mStates, state)
		SafeRelease(state->second);

	SafeRelease(mCheckParentTransparency);
}

void UIWidget::Draw()
{
	if ( !mVisible || mDrawedAtFrame == AppTimer()->GetCurrentFrame() )
		return;

	mDrawedAtFrame = AppTimer()->GetCurrentFrame();

	LocalDraw();
	if ( UI_DEBUG_FRAMES )
		DrawDebugFrame();

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->Draw();
}

void UIWidget::Update(float dt)
{
	if ( !mVisible || mUpdatedAtFrame == AppTimer()->GetCurrentFrame() )
		return;

	mUpdatedAtFrame = AppTimer()->GetCurrentFrame();

	UpdateStates(dt);
	LocalUpdate(dt);

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->Update(dt);

	mCursorInside = false;
}

void UIWidget::UpdateStates(float dt)
{
	FOREACH(StatesMap, mStates, state)
		(*state).second->update(dt);
}

void UIWidget::UpdateLayout(bool force /*= false*/, bool onlyDownUpdates /*= false*/)
{
	UpdateAbsMinSize();

	if ( !LocalUpdateLayout() && !force )
		return;

	//hlog("Update layout on: %s on frame %i", mId.c_str(), mUpdatedAtFrame);

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->UpdateLayout(force, onlyDownUpdates);

	CheckResizingByChilds();

	if ( mParent && !onlyDownUpdates )
		mParent->UpdateLayout();

	LayoutUpdated();
}

bool UIWidget::LocalUpdateLayout()
{
	if ( mParent )
		mLayout.Update(mParent->mChildsLayout.mPosition, mParent->mChildsLayout.mSize);
	else
		mLayout.Update(Vec2F(), Vec2F());

	mLayout.mSize.x = Max(mLayout.mSize.x, mAbsMinSize.x);
	mLayout.mSize.y = Max(mLayout.mSize.y, mAbsMinSize.y);

	if ( Equals(mGlobalPosition, mLayout.mPosition) && Equals(mSize, mLayout.mSize) )
		return false;

	mGlobalPosition = mLayout.mPosition;
	mSize = mLayout.mSize;
	mBounds.set(mGlobalPosition, mGlobalPosition + mSize);

	mChildsLayout.Update(mLayout.mPosition, mLayout.mSize);

	return true;
}

bool UIWidget::IsInside(const Vec2F& point) const
{
	if ( !mBounds.isInside(point) )
		return false;

	if ( !IsLocalInside(point) )
		return false;

	return true;
}

bool UIWidget::ProcessInputMessage(const InputMessage& msg)
{
	if ( !mVisible || mProcessedInputAtFrame == AppTimer()->GetCurrentFrame() )
		return false;

	mProcessedInputAtFrame = AppTimer()->GetCurrentFrame();
	mCursorInside = IsInside(msg.GetCursorPos());

	if ( !mFocused && !mCursorInside )
		return false;

	if ( LocalProcessInputMessage(msg) )
		return true;

	FOREACH_BACK(WidgetsVec, mChildWidgets, it)
	if ( (*it)->processInputMessage(msg) )
		return true;

	return false;
}

UIWidget* UIWidget::Clone() const
{
	return mnew UIWidget(*this);
}

UIWidget* UIWidget::AddChild(UIWidget* widget, int position /*= -1*/)
{
	if ( widget->mParent )
		widget->mParent->RemoveChild(widget, false);

	widget->mParent = this;

	if ( position < 0 )
		mChildWidgets.push_back(widget);
	else
		mChildWidgets.insert(mChildWidgets.begin() + position, widget);

	transparency.onChangeEvent.Add(widget->mCheckParentTransparency);

	UpdateLayout(true);
	UpdateLayout(true);

	return widget;
}

void UIWidget::RemoveChild(UIWidget* widget, bool release /*= true*/)
{
	WidgetsVec::iterator fnd = FIND(mChildWidgets, widget);
	if ( fnd != mChildWidgets.end() )
		mChildWidgets.erase(fnd);

	transparency.onChangeEvent.remove(widget->mCheckParentTransparency, false);

	widget->mParent = NULL;

	if ( release )
		SafeRelease(widget);
}

void UIWidget::RemoveAllChilds()
{
	FOREACH(WidgetsVec, mChildWidgets, it)
	{
		(*it)->mParent = NULL;
		transparency.onChangeEvent.remove((*it)->mCheckParentTransparency, false);
		SafeRelease(*it);
	}

	mChildWidgets.clear();
}

void UIWidget::SetParent(UIWidget* parent)
{
	if ( mParent )
		mParent->RemoveChild(this, false);

	if ( parent )
		parent->AddChild(this);
	else
		mParent = NULL;

	UpdateLayout();
}

UIWidget* UIWidget::GetParent()
{
	return mParent;
}

UIWidget* UIWidget::GetWidget(const String& id)
{
	int delPos = id.find("/");
	String pathPart = id.substr(0, delPos);

	if ( pathPart == ".." )
	{
		if ( mParent )
		{
			if ( delPos == id.npos )
				return mParent;
			else
				return mParent->GetWidget(id.substr(delPos + 1));
		}

		return NULL;
	}

	FOREACH(WidgetsVec, mChildWidgets, it)
	{
		if ( (*it)->mId == pathPart )
		{
			if ( delPos == id.npos )
				return (*it);
			else
				return (*it)->getWidget(id.substr(delPos + 1));
		}
	}

	return NULL;
}

String UIWidget::GetWidgetPath(UIWidget* widget) const
{
	String path = widget->mId;
	UIWidget* parent = widget->mParent;
	while ( parent )
	{
		path = parent->mId + "/" + path;
		parent = parent->mParent;
	}

	return path;
}

void UIWidget::SetPosition(const Vec2F& position)
{
	mLayout.mLTAbsolute = position;
	UpdateLayout();
}

Vec2F UIWidget::GetPosition() const
{
	return mLayout.mLTAbsolute;
}

void UIWidget::SetId(const String& id)
{
	mId = id;
}

String UIWidget::GetId() const
{
	return mId;
}

void UIWidget::SetGlobalPosition(const Vec2F& position)
{
	mLayout.mLTAbsolute = position;

	if ( mParent )
		mLayout.mLTAbsolute -= mParent->mGlobalPosition;

	UpdateLayout();
}

Vec2F UIWidget::GetGlobalPosition() const
{
	return mGlobalPosition;
}

void UIWidget::SetSize(const Vec2F& size)
{
	if ( Equals(size, mLayout.mRBAbsolute) )
		return;

	mLayout.mRBAbsolute = size;
	mLayout.mMinSize = size;
	UpdateLayout();
}

Vec2F UIWidget::GetSize() const
{
	return mSize;
}

void UIWidget::SetGeometry(Geometry* geometry)
{
	SafeRelease(mGeometry);
	mGeometry = geometry;
}

Geometry* UIWidget::GetGeometry() const
{
	return mGeometry;
}

bool UIWidget::IsFocusable() const
{
	return false;
}

void UIWidget::SetFocused(bool focused)
{
	if ( focused == mFocused )
		return;

	if ( focused )
		AppUI()->FocusOnWidget(this);
	else
		AppUI()->FocusOnWidget(NULL);
}

bool UIWidget::IsFocused() const
{
	return mFocused;
}

void UIWidget::MakeFocused()
{
	SetFocused(true);
}

void UIWidget::ReleaseFocus()
{
	SetFocused(false);
}

UIState* UIWidget::AddState(UIState* state)
{
	mStates[state->mName] = state;
	state->SetOwnerWidget(this);

	if ( state->mName == "visible" )
	{
		mVisibleState = state;
		state->onActiveStateEvent.Add(&onVisibleOn);
		state->onDeactiveStateEvent.Add(&onVisibleOff);
		state->SetState(mVisible, true);
	}

	AddedState(state);

	return state;
}

void UIWidget::SetState(const String& stateId, bool value)
{
	UIState* state = GetState(stateId);
	if ( state )
		state->SetState(value);
}

UIState* UIWidget::GetState(const String& stateId)
{
	StatesMap::iterator fnd = mStates.find(stateId);
	if ( fnd == mStates.end() )
		return NULL;

	return mStates[stateId];
}

void UIWidget::SetVisible(bool visible)
{
	if ( mVisibleState )
	{
		mVisibleState->SetState(visible);
	}
	else
	{
		mVisible = visible;
		if ( mVisible )
			onVisibleOn.Invoke();
		else
			onVisibleOff.Invoke();
	}
}

void UIWidget::SetVisibleParam(bool param)
{
	mVisible = param;
}

void UIWidget::GetLayout(const layout& layout)
{
	SetLayoutParams(layout, false);
}

Layout UIWidget::GetLayout() const
{
	return mLayout;
}

void UIWidget::SetChildsLayout(const layout& layout)
{
	mChildsLayout = layout;
	UpdateLayout();
}

Layout UIWidget::GetChildsLayout() const
{
	return mChildsLayout;
}

void UIWidget::SetTransparency(float transparency)
{
	mBasicTransparency = transparency;
	UpdateResTransparency();
}

float UIWidget::GetTransparency() const
{
	return mBasicTransparency;
}

void UIWidget::SetRisizeByChilds(bool flag)
{
	mResizeByChilds = flag;
	UpdateLayout();
}

bool UIWidget::IsResizingByChilds() const
{
	return mResizeByChilds;
}

bool UIWidget::IsVisible() const
{
	if ( mVisibleState )
		return mVisibleState->GetState();

	return mVisible;
}

void UIWidget::OnFocused()
{
	mFocused = true;
}

void UIWidget::OnFocusLost()
{
	mFocused = false;
}

void UIWidget::SetLayoutParams(const layout& layout, bool onlyDownUpdate)
{
	mLayout = layout;
	mLayout.mMinSize.x = Max(mLayout.mMinSize.x, mFixedMinSize.x);
	mLayout.mMinSize.y = Max(mLayout.mMinSize.y, mFixedMinSize.y);
	UpdateAbsMinSize();
	UpdateLayout(false, onlyDownUpdate);
}

void UIWidget::UpdateResTransparency()
{
	if ( mParent )
		mResTransparency = mParent->mResTransparency*mBasicTransparency;
	else
		mResTransparency = mBasicTransparency;

	mResTransparencyChanged.Invoke();
}

void UIWidget::UpdateAbsMinSize()
{
	Vec2F newAbsMinSize = mLayout.mMinSize;

	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		newAbsMinSize.x = Max((*child)->mAbsMinSize.x + (*child)->mGlobalPosition.x - mGlobalPosition.x, newAbsMinSize.x);
		newAbsMinSize.y = Max((*child)->mAbsMinSize.y + (*child)->mGlobalPosition.y - mGlobalPosition.y, newAbsMinSize.y);
	}

	if ( !Equals(newAbsMinSize, mAbsMinSize) )
	{
		mAbsMinSize = newAbsMinSize;
		UpdateLayout();
	}
}

void UIWidget::CheckResizingByChilds()
{
	if ( !mResizeByChilds )
		return;

	Vec2F childSize = Vec2F();
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		childSize.x = Max(childSize.x, (*child)->mSize.x + (*child)->mGlobalPosition.x - mGlobalPosition.x);
		childSize.y = Max(childSize.y, (*child)->mSize.y + (*child)->mGlobalPosition.y - mGlobalPosition.y);
	}

	mLayout.mRBAbsolute += childSize - mChildsLayout.mSize;

	UpdateLayout(false, true);
}

void UIWidget::InitializeProperties()
{
	REG_PROPERTY(UIWidget, position, SetPosition, GetPosition);
	REG_PROPERTY_NONCONST(UIWidget, parent, SetParent, GetParent);
	REG_PROPERTY(UIWidget, id, SetId, GetId);
	REG_PROPERTY(UIWidget, globalPosition, SetGlobalPosition, GetGlobalPosition);
	REG_PROPERTY(UIWidget, size, SetSize, GetSize);
	REG_PROPERTY_SETTER_NONCONST(UIWidget, visible, SetVisible, IsVisible);
	REG_PROPERTY(UIWidget, layout, GetLayout, GetLayout);
	REG_PROPERTY_SETTER_NONCONST(UIWidget, transparency, SetTransparency, GetTransparency);
}

int widgetDbgFrameIdx = 0;
void UIWidget::DrawDebugFrame()
{
	if ( mParent == NULL )
		widgetDbgFrameIdx = 0;

	AppRender()->DrawRectFrame(mGlobalPosition, mGlobalPosition + mSize, Color4::SomeColor(widgetDbgFrameIdx));
	AppRender()->DrawRectFrame(mGlobalPosition, mGlobalPosition + mAbsMinSize, Color4::SomeColor(widgetDbgFrameIdx));
	AppRender()->DrawLine(mGlobalPosition, mGlobalPosition + mAbsMinSize, Color4::SomeColor(widgetDbgFrameIdx));

	widgetDbgFrameIdx++;
}

CLOSE_O2_NAMESPACE
