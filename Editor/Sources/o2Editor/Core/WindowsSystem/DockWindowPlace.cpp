#include "o2Editor/stdafx.h"
#include "DockWindowPlace.h"

#include "o2/Render/Render.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2Editor/Core/WindowsSystem/DockableWindow.h"

#include <o2/Core/Memory\Ref.hpp>

namespace Editor
{
	DockWindowPlace::DockWindowPlace() :
		Widget(), DrawableCursorEventsListener(this), mDragHandleLayoutMin(Vec2F(), Vec2F(), Vec2F(), Vec2F()),
		mDragHandleLayoutMax(Vec2F(), Vec2F(), Vec2F(), Vec2F()), mNeighborMax(nullptr), mNeighborMin(nullptr)
	{
		InitializeDragHandle();
		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	DockWindowPlace::DockWindowPlace(const DockWindowPlace& other) :
		Widget(other), DrawableCursorEventsListener(this), mNeighborMax(nullptr), mNeighborMin(nullptr)
	{
		InitializeDragHandle();
		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	DockWindowPlace::~DockWindowPlace()
	{}

	DockWindowPlace& DockWindowPlace::operator=(const DockWindowPlace& other)
	{
		Widget::operator=(other);
		mDragHandleLayoutMin = other.mDragHandleLayoutMin;
		return *this;
	}

	void DockWindowPlace::Draw()
	{
		Widget::Draw();

		mDragHandleMin.OnDrawn();
		mDragHandleMax.OnDrawn();

		if (o2Input.IsKeyDown(VK_F1))
		{
			if (mDragHandleMin.IsInteractable())
				o2Render.DrawAARectFrame(mDragHandleAreaMin, Color4::Blue());

			if (mDragHandleMax.IsInteractable())
				o2Render.DrawAARectFrame(mDragHandleAreaMax, Color4::Red());

			if (mNeighborMin)
			{
				Vec2F c1 = layout->GetWorldRect().Center();
				Vec2F c2 = mNeighborMin->layout->GetWorldRect().Center();
				Vec2F n = (c2 - c1).Normalized().Perpendicular() * 30.0f;
				o2Render.DrawAABezierCurveArrow(c1, c1 + n, c2 + n, c2, Color4::Blue());
			}

			if (mNeighborMax)
			{
				Vec2F c1 = layout->GetWorldRect().Center();
				Vec2F c2 = mNeighborMax->layout->GetWorldRect().Center();
				Vec2F n = (c2 - c1).Normalized().Perpendicular() * 30.0f;
				o2Render.DrawAABezierCurveArrow(c1, c1 + n, c2 + n, c2, Color4::Red());
			}
		}
	}

	bool DockWindowPlace::IsUnderPoint(const Vec2F& point)
	{
		return layout->GetWorldRect().IsInside(point);
	}

	void DockWindowPlace::SetResizibleDir(TwoDirection dir, float border,
		DockWindowPlace* neighborMin, DockWindowPlace* neighborMax)
	{
		mResizibleDir = dir;
		float border2 = border * 2.0f;

		mDragHandleMin.interactable = neighborMin != nullptr;
		mNeighborMin = neighborMin;

		mNeighborMax = neighborMax;
		mDragHandleMax.interactable = neighborMax != nullptr;

		if (dir == TwoDirection::Horizontal)
		{
			mDragHandleLayoutMin = Layout(Vec2F(0, 0), Vec2F(0, 1), Vec2F(-border2, 0), Vec2F(0, 0));
			mDragHandleLayoutMax = Layout(Vec2F(1, 0), Vec2F(1, 1), Vec2F(border2, 0), Vec2F(0, 0));
			mDragHandleMin.cursorType = CursorType::SizeWE;
			mDragHandleMax.cursorType = CursorType::SizeWE;

			if (neighborMin != nullptr)
				layout->offsetLeft = border;

			if (neighborMax != nullptr)
				layout->offsetRight = -border;
		}
		else
		{
			mDragHandleLayoutMin = Layout(Vec2F(0, 0), Vec2F(1, 0), Vec2F(0, -border2), Vec2F(0, 0));
			mDragHandleLayoutMax = Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(0, 0), Vec2F(0, border2));
			mDragHandleMin.cursorType = CursorType::SizeNS;
			mDragHandleMax.cursorType = CursorType::SizeNS;

			if (neighborMin != nullptr)
				layout->offsetBottom = border;

			if (neighborMax != nullptr)
				layout->offsetTop = -border;
		}

		mDragHandleAreaMin = mDragHandleLayoutMin.Calculate(layout->GetWorldRect());
		mDragHandleAreaMax = mDragHandleLayoutMax.Calculate(layout->GetWorldRect());

		mDragHandleMin = o2::make<Widget>();
		mDragHandleMin->layout = Ref<Layout>(new Layout(mDragHandleLayoutMin));
		mDragHandleMin->cursorType = Ref<CursorType>(new CursorType(mDragHandleMin.cursorType));
	}

	TwoDirection DockWindowPlace::GetResizibleDir() const
	{
		return mResizibleDir;
	}

	void DockWindowPlace::ArrangeChildWindows()
	{
		Vector< Ref<DockableWindow> > windows;
		for (auto child : mChildren)
		{
			if (child->GetType() == TypeOf(DockableWindow))
				windows.Add(DynamicCast<Ref<DockableWindow>>(child));
		}

		windows.Sort([](const Ref<DockableWindow>& a, const Ref<DockableWindow>& b) { return a->mTabPosition < b->mTabPosition; });

		if (windows.Count() == 1)
		{
			windows[0]->SetNonTabState();
		}
		else
		{
			float offset = 0;
			int pos = 0;
			for (auto window : windows)
			{
				window->SetTabState(offset, pos, window == mChildren[0]);
				offset += window->GetTabWidth();

				pos++;
			}

			if (!windows.IsEmpty())
			{
				if (!windows.Any([&](const Ref<DockableWindow>& x) { return x->mTabActive; }))
					SetActiveTab(windows[0]);
			}
		}
	}

	void DockWindowPlace::SetActiveTab(const Ref<DockableWindow>& window)
	{
		Vector<Ref<DockableWindow>> tabWindows;
		for (auto child : mChildren)
		{
			if (child->GetType() == TypeOf(DockableWindow))
				tabWindows.Add(DynamicCast<Ref<DockableWindow>>(child));
		}

		mChildrenInheritedDepth.SortBy<int>([&](const Ref<ISceneDrawable>& child) { return DynamicCast<Ref<DockableWindow>>(child)->mTabPosition; });

		for (const auto& tabWindow : tabWindows)
		{
			if (tabWindow->mTabActive)
			{
				tabWindow->mTabActive = false;

				if (auto state = tabWindow->GetStateObject("tabActive"))
					state->SetState(false);
			}
		}

		if (window)
		{
			window->mTabActive = true;

			if (auto state = window->GetStateObject("tabActive"))
				state->SetState(true);
		}
	}
}#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

template<typename T>
class Ref
{
public:
    Ref(T* ptr = nullptr) : m_ptr(ptr)
    {
        if (m_ptr)
            m_refCount = new int(1);
    }

    Ref(const Ref<T>& ref) : m_ptr(ref.m_ptr), m_refCount(ref.m_refCount)
    {
        if (m_refCount)
            (*m_refCount)++;
    }

    ~Ref()
    {
        Release();
    }

    Ref<T>& operator=(T* ptr)
    {
        Release();
        m_ptr = ptr;
        if (m_ptr)
            m_refCount = new int(1);
        return *this;
    }

    Ref<T>& operator=(const Ref<T>& ref)
    {
        if (m_ptr != ref.m_ptr)
        {
            Release();
            m_ptr = ref.m_ptr;
            m_refCount = ref.m_refCount;
            if (m_refCount)
                (*m_refCount)++;
        }
        return *this;
    }

    T* operator->() const
    {
        return m_ptr;
    }

    T& operator*() const
    {
        return *m_ptr;
    }

    explicit operator bool() const
    {
        return m_ptr != nullptr;
    }

private:
    void Release()
    {
        if (m_refCount)
        {
            (*m_refCount)--;
            if (*m_refCount == 0)
            {
                delete m_ptr;
                delete m_refCount;
            }
        }
    }

    T* m_ptr;
    int* m_refCount;
};

template<typename T>
class WeakRef
{
public:
    WeakRef(T* ptr = nullptr) : m_ptr(ptr), m_refCount(nullptr)
    {
        if (m_ptr)
            m_refCount = new int(1);
    }

    WeakRef(const WeakRef<T>& ref) : m_ptr(ref.m_ptr), m_refCount(ref.m_refCount)
    {
        if (m_refCount)
            (*m_refCount)++;
    }

    WeakRef(const Ref<T>& ref) : m_ptr(ref.m_ptr), m_refCount(ref.m_refCount)
    {
        if (m_refCount)
            (*m_refCount)++;
    }

    ~WeakRef()
    {
        Release();
    }

    WeakRef<T>& operator=(T* ptr)
    {
        Release();
        m_ptr = ptr;
        if (m_ptr)
            m_refCount = new int(1);
        return *this;
    }

    WeakRef<T>& operator=(const WeakRef<T>& ref)
    {
        if (m_ptr != ref.m_ptr)
        {
            Release();
            m_ptr = ref.m_ptr;
            m_refCount = ref.m_refCount;
            if (m_refCount)
                (*m_refCount)++;
        }
        return *this;
    }

    WeakRef<T>& operator=(const Ref<T>& ref)
    {
        Release();
        m_ptr = ref.m_ptr;
        m_refCount = ref.m_refCount;
        if (m_refCount)
            (*m_refCount)++;
        return *this;
    }

    T* operator->() const
    {
        return m_ptr;
    }

    T& operator*() const
    {
        return *m_ptr;
    }

    explicit operator bool() const
    {
        return m_ptr != nullptr;
    }

private:
    void Release()
    {
        if (m_refCount)
        {
            (*m_refCount)--;
            if (*m_refCount == 0)
            {
                delete m_refCount;
            }
        }
    }

    T* m_ptr;
    int* m_refCount;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ref)
{
    return Ref<T>(dynamic_cast<T*>(ref.operator->()));
}

namespace Editor
{
class DockWindowPlace
{
public:
    DockWindowPlace(Ref<Widget> parentWidget) : mParentWidget(parentWidget)
    {
        InitializeDragHandle();
    }

    virtual ~DockWindowPlace() = default;

    void OnVisibleChanged(bool visible)
    {
        if (visible)
            CheckInteractable();
    }

    void OnUpdate() override
    {
        if (interactable)
        {
            auto state = mState.Pin();
            if (state)
            {
                if (state->GetState() == false)
                {
                    state->SetState(true);
                }
            }
            else if (auto window = mParentWidget.Pin())
            {
                auto state = window->GetStateObject("tabActive");
                if (state)
                {
                    state->SetState(true);
                }
            }
        }
    }

    void UpdateSelfTransform() override
    {
        Widget::UpdateSelfTransform();
        mDragHandleAreaMin = mDragHandleLayoutMin.Calculate(layout->GetWorldRect());
        mDragHandleAreaMax = mDragHandleLayoutMax.Calculate(layout->GetWorldRect());
    }

    String GetCreateMenuCategory()
    {
        return "UI/Editor";
    }

    void OnDragHandleMinMoved(const Vec2F& delta)
    {
        if (mResizibleDir == TwoDirection::Horizontal)
        {
            float anchorDelta = delta.x / mParentWidget->layout->width;
            layout->anchorLeft += anchorDelta;

            mNeighborMin->layout->anchorRight += anchorDelta;
        }
        else
        {
            float anchorDelta = delta.y / mParentWidget->layout->height;
            layout->anchorBottom += anchorDelta;
            mNeighborMin->layout->anchorTop += anchorDelta;
        }
    }

    void OnDragHandleMaxMoved(const Vec2F& delta)
    {
        if (mResizibleDir == TwoDirection::Horizontal)
        {
            float anchorDelta = delta.x / mParentWidget->layout->width;
            layout->anchorRight += anchorDelta;

            mNeighborMax->layout->anchorLeft += anchorDelta;
        }
        else
        {
            float anchorDelta = delta.y / mParentWidget->layout->height;
            layout->anchorTop += anchorDelta;
            mNeighborMax->layout->anchorBottom += anchorDelta;
        }
    }

    void CheckInteractable()
    {
        interactable = mChildren.Count([](const Ref<DockWindowPlace>& x) { return x->GetType() == TypeOf(DockWindowPlace); }) == 0;

        for (const auto& child : mChildren)
        {
            if (child->GetType() == TypeOf(DockWindowPlace))
            {
                DynamicCast<DockWindowPlace>(child)->CheckInteractable();
            }
        }
    }

    void InitializeDragHandle()
    {
        mDragHandleMin.isUnderPoint = [&](const Vec2F& point) { return mDragHandleAreaMin.IsInside(point); };
        mDragHandleMin.onMoved = [&](const Input::Cursor& cursor) { OnDragHandleMinMoved(cursor.delta); };

        mDragHandleMax.isUnderPoint = [&](const Vec2F& point) { return mDragHandleAreaMax.IsInside(point); };
        mDragHandleMax.onMoved = [&](const Input::Cursor& cursor) { OnDragHandleMaxMoved(cursor.delta); };
    }

private:
    Ref<Widget> mParentWidget;
    WeakRef<State> mState;
    TwoDirection mResizibleDir;

    LayoutItem mDragHandleLayoutMin;
    LayoutItem mDragHandleLayoutMax;

    RectF mDragHandleAreaMin;
    RectF mDragHandleAreaMax;

    Draggable mDragHandleMin;
    Draggable mDragHandleMax;

    Ref<DockWindowPlace> mNeighborMin;
    Ref<DockWindowPlace> mNeighborMax;
    String mUniqueId;
    bool interactable;

    Vector<Ref<DockWindowPlace>> mChildren;
};
}

DECLARE_TEMPLATE_CLASS(o2::Ref<Editor::DockWindowPlace>);
// --- META ---

DECLARE_CLASS(Editor::DockWindowPlace, Editor__DockWindowPlace);
// --- END META ---