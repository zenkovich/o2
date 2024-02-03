#include "o2Editor/stdafx.h"
#include "DockableWindow.h"

#include "o2/Animation/Tracks/AnimationFloatTrack.h"
#include "o2/Animation/Tracks/AnimationVec2FTrack.h"
#include "o2/Animation/Tracks/AnimationColor4Track.h"
#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Events/EventSystem.h"
#include "o2/Render/Render.h"
#include "o2/Render/Text.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/WindowsSystem/DockWindowPlace.h"

namespace Editor
{
	const char* DockableWindow::mTabLayerPath = "tab/main";
	const char* DockableWindow::mTabIconLayerPath = "tab/main/icon";
	const char* DockableWindow::mTabCaptionLayerPath = "tab/main/caption";
	const char* DockableWindow::mIconLayerPath = "back/icon";
	const char* DockableWindow::mCaptionLayerPath = "back/caption";

	DockableWindow::DockableWindow():
		Window(),
        mDockingFrameSample(mmake<Ref<Sprite>>())
	{
		InitializeDragHandles();
		SetDocked(false);

		RetargetStatesAnimations();
	}

	DockableWindow::DockableWindow(const DockableWindow& other):
		Window(other),
        mDockingFrameSample(other.mDockingFrameSample->CloneAs<Sprite>())
	{
		PushEditorScopeOnStack scope;

		InitializeDragHandles();
		SetDocked(false);

		InitializeDockFrameAppearanceAnim();

		if (mVisibleState)
			mVisibleState->onStateFullyFalse += THIS_FUNC(Undock);

		RetargetStatesAnimations();
	}

	DockableWindow::~DockableWindow()
	{

	}

	DockableWindow& DockableWindow::operator=(const DockableWindow& other)
	{
		Window::operator=(other);

		mDockingFrameSample = other.mDockingFrameSample->CloneAs<Sprite>();

		if (mVisibleState)
			mVisibleState->onStateFullyFalse += THIS_FUNC(Undock);

		return *this;
	}

	void DockableWindow::Update(float dt)
	{
		Window::Update(dt);

		const float lerpFrameCoef = 10.0f;
		if (mDockingFrameTarget != mDockingFrameCurrent)
		{
			mDockingFrameCurrent = Math::Lerp(mDockingFrameCurrent, mDockingFrameTarget, dt*lerpFrameCoef);
			mDockingFrameSample->Get()->rect = mDockingFrameCurrent;
		}

		mDockingFrameAppearance->Update(dt);
	}

	void DockableWindow::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		mBackCursorArea.OnDrawn();

		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		for (const auto& layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

		if (!mTabState || mTabActive)
		{
			o2Render.EnableScissorTest(mAbsoluteClipArea);

			for (const auto& child : mChildrenInheritedDepth)
				child->Draw();

			o2Render.DisableScissorTest();

			mTopDragHandle.OnDrawn();
			mBottomDragHandle.OnDrawn();
			mLeftDragHandle.OnDrawn();
			mRightDragHandle.OnDrawn();
			mLeftTopDragHandle.OnDrawn();
			mRightTopDragHandle.OnDrawn();
			mLeftBottomDragHandle.OnDrawn();
			mRightBottomDragHandle.OnDrawn();
			mHeadDragHandle.OnDrawn();

			CursorAreaEventsListener::OnDrawn();

			for (const auto& child : mInternalWidgets)
				child->Draw();

			for (const auto& layer : mTopDrawingLayers)
				layer->Draw();
		}
		else
			mHeadDragHandle.OnDrawn();

		DrawDebugFrame();

		if (mDockingFrameSample->Get()->GetTransparency() > 0.001f)
			mDockingFrameSample->Get()->Draw();
	}

	void DockableWindow::SetDocked(bool docked)
	{
		mDocked = docked;

		auto dockedState = state["docked"];
		if (dockedState)
			*dockedState = docked;

		mTopDragHandle.interactable         = !docked;
		mBottomDragHandle.interactable      = !docked;
		mLeftDragHandle.interactable        = !docked;
		mRightDragHandle.interactable       = !docked;
		mLeftTopDragHandle.interactable     = !docked;
		mLeftBottomDragHandle.interactable  = !docked;
		mRightTopDragHandle.interactable    = !docked;
		mRightBottomDragHandle.interactable = !docked;
	}

	void DockableWindow::RecalculateTabWidth()
	{
		float expand = 35;
		if (const auto& textLayer = GetLayer(mTabCaptionLayerPath))
		{
			if (const auto& textDrawable = mDynamicCast<Text*>(textLayer->GetDrawable()))
			{
				Text::SymbolsSet symbolsSet;
				symbolsSet.Initialize(textDrawable->GetFont(), textDrawable->GetText(), textDrawable->GetFontHeight(),
                                      Vec2F(), Vec2F(), HorAlign::Left, VerAlign::Bottom, false, false, 1.0f, 1.0f);

				SetTabWidth(symbolsSet.mRealSize.x + textLayer->layout.offsetLeft - textLayer->layout.offsetRight + expand);
			}
		}
	}

	bool DockableWindow::IsDocked() const
	{
		return mDocked;
	}

	Sprite* DockableWindow::GetDockingFrameSample() const
	{
		return mDockingFrameSample.Get();
	}

	void DockableWindow::SetIcon(Sprite* icon)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
		{
			if (iconLayer->GetDrawable())
				delete iconLayer->GetDrawable();

			iconLayer->SetDrawable(icon);
		}

		auto tabIconLayer = GetLayer(mTabIconLayerPath);
		if (tabIconLayer)
		{
			if (tabIconLayer->GetDrawable())
				delete tabIconLayer->GetDrawable();

			tabIconLayer->SetDrawable(mmake<Ref<Sprite>>(icon->CloneAs<Sprite>>()));
		}
	}
}#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename ... Args>
Ref<T> mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
class DynamicCast {
public:
    template<typename U>
    static Ref<T> Convert(const Ref<U>& arg) {
        return std::dynamic_pointer_cast<T>(arg);
    }
};


class DockableWindow {
    public:
        virtual ~DockableWindow() = default;

        Sprite* GetIcon() const;
        void SetIconLayout(const Layout& layout);
        Layout GetIconLayout() const;
        void SetCaption(const WString& caption);
        WString GetCaption() const;
        void SetTabWidth(float width);
        float GetTabWidth() const;
        void SetTabActive();
        bool IsTabActive() const;
        void SetAutoCalcuclatingTabWidth(bool enable);
        bool IsAutoCalcuclatingTabWidth() const;
        void UpdateSelfTransform();
        bool IsUnderPoint(const Vec2F& point);
        String GetCreateMenuCategory();
        void OnEnabled();
        void OnDisabled();
        void OnFocused();
        void InitializeDockFrameAppearanceAnim();
        void InitializeDragHandles();
        void OnHeadDblCKicked(const Input::Cursor& cursor);
        void OnMoved(const Input::Cursor& cursor);

    private:
        Ref<Layer> GetLayer(const String& path);
        void RecalculateTabWidth();
        void Undock();
        void PlaceDock(DockWindowPlace* dockWindowPlace);
        bool IsDocked() const;

        Ref<Layer> GetLayer(const String& path) const;

        Ref<Layer> GetLayer(const String& path) {
            return nullptr;
        }

        void SetLayoutDirty() {}


        Ref<Layer> GetLayer(const String& path) const {
            return nullptr;
        }

        void SetLayoutDirty() const {}


        bool mTabActive = false;
        float mTabWidth = 0.0f;
        bool mAutoCalculateTabWidth = false;
        RectF mHeadDragAreaRect;
        bool mDocked = false;
        Ref<Animation> mDockingFrameAppearance;
        Layout mIconLayout;
        Layout mTabFrameLayout;
        Layout mCaptionLayout;
        Layout mHeadDragAreaLayout;
        float mTabHeadHeight = 0.0f;
        Vec2F mDragOffset;
        Vec2F mTabAreaSize;
        Ref<UnicodeTranslator> mUnicodeTranslator;

        static constexpr const char* mIconLayerPath = "Sprite.Icon";
        static constexpr const char* mTabLayerPath = "DockTab.Icon";
        static constexpr const char* mCaptionLayerPath = "TextLabel";
        static constexpr const char* mTabIconLayerPath = "DockTab.Icon";
        static constexpr const char* mTabCaptionLayerPath = "DockTab.Label";
        Ref<WindowLayout> layout;
};#include <memory>

template <typename T>
class Ref
{
public:
    Ref() : m_ptr(nullptr) {}
    Ref(T* ptr) : m_ptr(ptr) {}
    Ref(const Ref<T>& other) : m_ptr(other.m_ptr) {}
    ~Ref() {}

    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }

    Ref<T>& operator=(const Ref<T>& other)
    {
        if (this != &other)
            m_ptr = other.m_ptr;
        return *this;
    }

    bool operator==(const Ref<T>& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const Ref<T>& other) const { return m_ptr != other.m_ptr; }

private:
    T* m_ptr;
};

template <typename T>
Ref<T> mmake()
{
    return Ref<T>(new T());
}

class DockableWindow
{
public:
    void OnMoveEnd(const Input::Cursor& cursor)
    {
        Ref<DockWindowPlace> targetDock;
        Side dockPosition = Side::None;
        RectF dockZoneRect;

        if (TraceDock(targetDock, dockPosition, dockZoneRect) && targetDock)
        {
            mDockingFrameAppearance->PlayForward();
            mDockingFrameTarget = dockZoneRect;
        }
        else
        {
            mDockingFrameAppearance->PlayBack();
            mDockingFrameTarget = layout->worldRect;
        }
    }

    void OnMoveCompleted(const Input::Cursor& cursor)
    {
        if (mDocked)
            return;

        Ref<DockWindowPlace> targetDock;
        Side dockPosition = Side::None;
        RectF dockZoneRect;

        if (!TraceDock(targetDock, dockPosition, dockZoneRect) || !targetDock)
            return;

        bool allOnLine = targetDock->mParent && targetDock->mParentWidget->GetChildWidgets().All([&](const Ref<Widget>& x) {
            if (x->GetType() != TypeOf(DockWindowPlace))
                return false;

            Ref<DockWindowPlace> dock = dynamic_cast<DockWindowPlace*>(x.Get());
            TwoDirection rd = dock->GetResizibleDir();

            if (dockPosition == Side::Left || dockPosition == Side::Right)
                return rd == TwoDirection::Horizontal;

            return rd == TwoDirection::Vertical;
        });

        if (allOnLine)
            PlaceLineDock(targetDock.Get(), dockPosition, dockZoneRect);
        else
            PlaceNonLineDock(targetDock.Get(), dockPosition);
    }

    void OnMoveBegin(const Input::Cursor& cursor)
    {
        OnFocused();

        if (mDocked)
            mDragOffset = (Vec2F)o2Input.cursorPos - layout->worldLeftTop;
    }

    bool TraceDock(Ref<DockWindowPlace>& targetDock, Side& dockPosition, RectF& dockZoneRect)
    {
        Vec2F cursorPos = o2Input.cursorPos;
        auto listenersUnderCursor = o2Events.GetAllCursorListenersUnderCursor(0);
        auto dockPlaceListener = listenersUnderCursor.FindOrDefault([](const Ref<CursorAreaEventsListener>& x) {
            return DynamicCast<DockWindowPlace>(x.Get()) != nullptr;
        });

        if (dockPlaceListener)
        {
            auto dockPlace = DynamicCast<DockWindowPlace>(dockPlaceListener);

            RectF dockPlaceRect = dockPlace->layout->worldRect;

            RectF leftZone(dockPlaceRect.left, dockPlaceRect.bottom,
                Math::Lerp(dockPlaceRect.left, dockPlaceRect.right, mDockSizeCoef), dockPlaceRect.top);

            RectF rightZone(Math::Lerp(dockPlaceRect.right, dockPlaceRect.left, mDockSizeCoef), dockPlaceRect.bottom,
                dockPlaceRect.right, dockPlaceRect.top);

            RectF bottomZone(dockPlaceRect.left, dockPlaceRect.bottom,
                dockPlaceRect.right, Math::Lerp(dockPlaceRect.bottom, dockPlaceRect.top, mDockSizeCoef));

            if (bottomZone.IsInside(cursorPos))
            {
                targetDock = dockPlace;
                dockPosition = Side::Bottom;
                dockZoneRect = bottomZone;
            }
            else if (rightZone.IsInside(cursorPos))
            {
                targetDock = dockPlace;
                dockPosition = Side::Right;
                dockZoneRect = rightZone;
            }
            else if (leftZone.IsInside(cursorPos))
            {
                targetDock = dockPlace;
                dockPosition = Side::Left;
                dockZoneRect = leftZone;
            }
        }

        return targetDock != nullptr;
    }

    void PlaceDock(DockWindowPlace* targetDock)
    {
        PushEditorScopeOnStack scope;

        mNonDockSize = layout->size;

        mTabPosition = targetDock->mChildren.Count();

        targetDock->AddChild(this);
        *layout = WidgetLayout::BothStretch();
        SetDocked(true);

        mDockingFrameAppearance->PlayBack();
        mDockingFrameTarget = layout->GetWorldRect();

        targetDock->ArrangeChildWindows();
        targetDock->SetLayoutDirty();
    }

    void PlaceNonLineDock(DockWindowPlace* targetDock, Side dockPosition)
    {
        PushEditorScopeOnStack scope;

        mNonDockSize = layout->size;

        Ref<DockWindowPlace> windowDock = mmake<DockWindowPlace>();
        windowDock->name = "window dock";
        *windowDock->layout = WidgetLayout::BothStretch();

        Ref<DockWindowPlace> windowNeighborDock = mmake<DockWindowPlace>();
        windowNeighborDock->name = "empty dock";
        *windowNeighborDock->layout = WidgetLayout::BothStretch();

        for (const Ref<Widget>& child : targetDock->GetChildWidgets())
            windowNeighborDock->AddChild(child);

        targetDock->AddChild(windowNeighborDock);
        targetDock->AddChild(windowDock);
        targetDock->interactable = false;

        if (dockPosition == Side::Bottom)
        {
            windowNeighborDock->layout->anchorBottom = mDockSizeCoef;
            windowDock->layout->anchorTop = mDockSizeCoef;
            windowDock->layout->offsetTop = -mDockBorder;

            windowDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, nullptr, windowNeighborDock.Get());
            windowNeighborDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, windowDock.Get(), nullptr);
        }
        else if (dockPosition == Side::Right)
        {
            windowNeighborDock->layout->anchorRight = 1.0f - mDockSizeCoef;
            windowDock->layout->anchorLeft = 1.0f - mDockSizeCoef;
            windowDock->layout->offsetLeft = mDockBorder;

            windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock.Get(), nullptr);
            windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nullptr, windowDock.Get());
        }
        else if (dockPosition == Side::Left)
        {
            windowNeighborDock->layout->anchorLeft = mDockSizeCoef;
            windowDock->layout->anchorRight = mDockSizeCoef;
            windowDock->layout->offsetRight = -mDockBorder;

            windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nullptr, windowNeighborDock.Get());
            windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowDock.Get(), nullptr);
        }
    }

private:
    Ref<Widget> mDockingFrameAppearance;
    RectF mDockingFrameTarget;
    bool mDocked;
    Vec2F mDragOffset;
    Size mNonDockSize;
    int mTabPosition;
    float mDockSizeCoef;
    float mDockBorder;
};

typedef std::shared_ptr<Widget> WidgetPtr;
typedef std::shared_ptr<CursorAreaEventsListener> CursorAreaEventsListenerPtr;
typedef std::shared_ptr<DockWindowPlace> DockWindowPlacePtr;

bool allOnLine = targetDock->mParent && std::all_of(targetDock->mParentWidget->GetChildWidgets().begin(), targetDock->mParentWidget->GetChildWidgets().end(), [&](const WidgetPtr& x) {
    if (x->GetType() != TypeOf(DockWindowPlace))
        return false;

    const DockWindowPlacePtr& dock = std::dynamic_pointer_cast<DockWindowPlace>(x);
    TwoDirection rd = dock->GetResizibleDir();

    if (dockPosition == Side::Left || dockPosition == Side::Right)
        return rd == TwoDirection::Horizontal;

    return rd == TwoDirection::Vertical;
});replace raw pointers with Ref<>, except void* pointers.Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

void DockableWindow::PlaceLineDock(DockWindowPlace* targetDock, Side dockPosition, RectF dockZoneRect)
{
    PushEditorScopeOnStack scope;

    mNonDockSize = layout->size;

    Ref<DockWindowPlace> windowDock = mmake<DockWindowPlace>();
    windowDock->name = "window dock";
    *windowDock->layout = WidgetLayout::BothStretch();

    Ref<DockWindowPlace> windowNeighborDock = targetDock;
    targetDock->mParent->AddChild(windowDock.get());

    if (dockPosition == Side::Bottom)
    {
        windowDock->layout->anchorBottom = windowNeighborDock->layout->anchorBottom;

        windowNeighborDock->layout->anchorBottom +=
            windowNeighborDock->layout->height*mDockSizeCoef / windowNeighborDock->mParentWidget->layout->height;

        windowDock->layout->anchorTop = windowNeighborDock->layout->anchorBottom;

        if (targetDock->mNeighborMin)
        {
            auto nmin = targetDock->mNeighborMin.get();

            nmin->SetResizibleDir(TwoDirection::Vertical, mDockBorder, nmin->mNeighborMin, windowDock.get());
            windowDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, nmin, windowNeighborDock.get());
            windowNeighborDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, windowDock.get(), windowNeighborDock->mNeighborMax);
        }
        else
        {
            windowDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, nullptr, windowNeighborDock.get());
            windowNeighborDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, windowDock.get(), windowNeighborDock->mNeighborMax);
        }
    }
    else if (dockPosition == Side::Right)
    {
        windowDock->layout->anchorRight = windowNeighborDock->layout->anchorRight;

        windowNeighborDock->layout->anchorRight -=
            windowNeighborDock->layout->width*mDockSizeCoef / windowNeighborDock->mParentWidget->layout->width;

        windowDock->layout->anchorLeft = windowNeighborDock->layout->anchorRight;

        if (targetDock->mNeighborMax)
        {
            auto nmax = targetDock->mNeighborMax.get();

            nmax->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowDock.get(), nmax->mNeighborMax);
            windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock.get(), nmax);
            windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock->mNeighborMin, windowDock.get());
        }
        else
        {
            windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock.get(), nullptr);
            windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock->mNeighborMin, windowDock.get());
        }
    }
    else if (dockPosition == Side::Left)
    {
        windowDock->layout->anchorLeft = windowNeighborDock->layout->anchorLeft;

        windowNeighborDock->layout->anchorLeft +=
            windowNeighborDock->layout->width*mDockSizeCoef / windowNeighborDock->mParentWidget->layout->width;

        windowDock->layout->anchorRight = windowNeighborDock->layout->anchorLeft;

        if (targetDock->mNeighborMin)
        {
            auto nmin = targetDock->mNeighborMin.get();

            nmin->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nmin->mNeighborMin, windowDock.get());
            windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nmin, windowNeighborDock.get());
            windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowDock.get(), windowNeighborDock->mNeighborMax);
        }
        else
        {
            windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nullptr, windowNeighborDock.get());
            windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowDock.get(), windowNeighborDock->mNeighborMax);
        }
    }

    windowDock->AddChild(this);
    *layout = WidgetLayout::BothStretch();
    SetDocked(true);

    mDockingFrameAppearance->PlayBack();
    mDockingFrameTarget = layout->GetWorldRect();
}

void DockableWindow::SetTabState(float offset, int position, bool isFirst)
{
    SetStateForcible("tab", true);

    if (mAutoCalculateTabWidth)
        RecalculateTabWidth();

    Ref<StateObject> tabFirstState = GetStateObject("tabFirst");
    if (tabFirstState)
        tabFirstState->SetState(isFirst);

    mTabPosition = position;

    Ref<Layer> tabMainLayer = GetLayer(mTabLayerPath);
    if (tabMainLayer)
    {
        tabMainLayer->layout.offsetLeft = offset;
        tabMainLayer->layout.offsetRight = offset + mTabWidth;
        tabMainLayer->layout.anchorLeft = 0;
        tabMainLayer->layout.anchorRight = 0;
    }

    mTabState = true;
}

void DockableWindow::SetNonTabState()
{
    SetStateForcible("tab", false);

    mTabActive = false;

    Ref<StateObject> state = GetStateObject("tabActive");
    if (state)
        state->SetState(false);

    Ref<Layer> tabMainLayer = GetLayer(mTabLayerPath);
    if (tabMainLayer)
    {
        tabMainLayer->layout.offsetLeft = 0;
        tabMainLayer->layout.offsetRight = 0;
        tabMainLayer->layout.anchorLeft = 0;
        tabMainLayer->layout.anchorRight = 1;
    }

    mTabState = false;
}#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
	return std::dynamic_pointer_cast<T>(ptr);
}

void Editor::DockableWindow::SetActiveTab()
{
	if (!mTabState)
		return;

	mTabActive = true;

	if (auto state = GetStateObject("tabActive"))
		state->SetState(true);
}

void Editor::DockableWindow::Undock()
{
	PushEditorScopeOnStack scope;

	if (!IsDocked())
		return;

	auto topDock = DynamicCast<DockWindowPlace>(mParent->GetParent());

	if (!topDock)
	{
		EditorUIRoot.AddWidget(this);
	}
	else
	{
		auto parent = DynamicCast<DockWindowPlace>(mParent);
		auto parentNeighbors = topDock->GetChildWidgets().FindAll([&](const auto& x) { return x != parent; })
			.Convert<DockWindowPlace*>([](const auto& x) { return DynamicCast<DockWindowPlace>(x); });

		EditorUIRoot.AddWidget(this);

		if (!parent->GetChildWidgets().IsEmpty())
		{
			parent->ArrangeChildWindows();
			parent->SetLayoutDirty();
		}
		else
		{
			if (parent->mNeighborMin)
			{
				parent->mNeighborMin->SetResizibleDir(parent->mNeighborMin->mResizibleDir, mDockBorder,
													  parent->mNeighborMin->mNeighborMin, parent->mNeighborMax);
			}

			if (parent->mNeighborMax)
			{
				parent->mNeighborMax->SetResizibleDir(parent->mNeighborMax->mResizibleDir, mDockBorder,
													  parent->mNeighborMin, parent->mNeighborMax->mNeighborMax);
			}

			if (parent->mResizibleDir == TwoDirection::Horizontal)
			{
				if (parent->mNeighborMin && parent->mNeighborMax)
				{
					float anchor = (parent->mNeighborMin->layout->anchorRight + parent->mNeighborMax->layout->anchorLeft) / 2.0f;
					parent->mNeighborMin->layout->anchorRight = anchor;
					parent->mNeighborMax->layout->anchorLeft = anchor;
				}
				else if (parent->mNeighborMin && !parent->mNeighborMax)
					parent->mNeighborMin->layout->anchorRight = 1.0f;
				else if (!parent->mNeighborMin && parent->mNeighborMax)
					parent->mNeighborMax->layout->anchorLeft = 0.0f;
			}

			if (parent->mResizibleDir == TwoDirection::Vertical)
			{
				if (parent->mNeighborMin && parent->mNeighborMax)
				{
					float anchor = (parent->mNeighborMin->layout->anchorTop + parent->mNeighborMax->layout->anchorBottom) / 2.0f;
					parent->mNeighborMin->layout->anchorTop = anchor;
					parent->mNeighborMax->layout->anchorBottom = anchor;
				}
				else if (parent->mNeighborMin && !parent->mNeighborMax)
					parent->mNeighborMin->layout->anchorTop = 1.0f;
				else if (!parent->mNeighborMin && parent->mNeighborMax)
					parent->mNeighborMax->layout->anchorBottom = 0.0f;
			}

			topDock->RemoveChild(parent);

			if (parentNeighbors.Count() == 1)
			{
				auto children = parentNeighbors[0]->GetChildWidgets();
				for (const auto& child : children)
					topDock->AddChild(child);

				topDock->RemoveChild(parentNeighbors[0]);
			}

			topDock->CheckInteractable();
		}
	}

	SetDocked(false);
	SetNonTabState();

	Vec2F leftTop = layout->worldLeftTop;
	layout->anchorMin = Vec2F(); layout->anchorMax = Vec2F();
	layout->worldLeftTop = leftTop;
	layout->worldRightBottom = layout->worldLeftTop + mNonDockSize.InvertedY();
}

}

// --- META ---

DECLARE_CLASS(Editor::DockableWindow, Editor__DockableWindow);
// --- END META ---