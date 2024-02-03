#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Render/FontRef.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Editor/DragHandle.h"
#include <cmath>

using namespace o2;

namespace o2
{
	class HorizontalScrollBar;
}

namespace Editor
{
	class AnimationWindow;

	// ------------------------------------
	// Draws time scale in animation window
	// ------------------------------------
	class AnimationTimeline : public Widget, public CursorAreaEventsListener
	{
	public:
		Function<void()> onViewChanged; // Called when scroll or zoom were changed

	public:
		// Default constructor
		AnimationTimeline();

		// Copy-constructor
		AnimationTimeline(const AnimationTimeline& other);

		// Destructor
		~AnimationTimeline();

		// Copy-operator
		AnimationTimeline& operator=(const AnimationTimeline& other);

		// Draws time scale and captions
		void Draw() override;

		// Updates as regular widget and changes zoom by mouse wheel
		void Update(float dt) override;

		// Sets animation. Subscribes on animation duration change, controls playing time
		void SetAnimation(const Ref<AnimationClip>& animation, const Ref<AnimationPlayer>& player = nullptr);

		// Sets current time scroll in seconds
		void SetScroll(float scroll);

		// Sets view range, left view border by left, and right border by right
		void SetViewRange(float left, float right, bool force = true);

		// Sets red time cursor
		void SetTimeCursor(float time);

		// Returns time of red cursor
		float GetTimeCursor() const;

		// Returns current time scroll in seconds
		float GetScroll() const;

		// Sets time scale zoom
		void SetScale(float scale);

		// Returns time scale zoom
		float GetScale() const;

		// Sets lock on zoom and scroll
		void SetViewMoveDisabled(bool disabled);

		// Converts local time to world x position
		float LocalToWorld(float pos) const;

		// Converts world x position to local time
		float WorldToLocal(float pos) const;

		// Returns captions text drawable
		const Ref<Text>& GetText() const;

		// Sets scrollbar widget, used for view scroll
		void SetScrollBar(const Ref<HorizontalScrollBar>& scrollBar);

		// Returns scrollbar widget, used for view scroll
		const Ref<HorizontalScrollBar>& GetScrollBar() const;

		// Checks is this timeA and timeB are same on screen. Dependent on zoom, thershold - max pixels distance on screen
		bool IsSameTime(float timeA, float timeB, float threshold = 3.0f) const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(AnimationTimeline);

	private:
		const float mTextOffset = 12.0f;              // Text position offset from top border
		const float mBigLineOffset = 13.0f;           // Big scale line offset from top border
		const float mSmallLineOffset = 15.0f;         // Small scale line offset from top border
		const float mOneSecondDefaultSize = 100.0f;   // One second with scale == 1 size in pixels
		const float mPerfectScaleSegmentSize = 10.0f; // The perfect size of scale segment in pixels. Scale period and segment size will be chosen more suitable
		const float mScaleOffset = 10.0f;             // First scale line offset from left border

		const Color4 mBigLineColor = Color4(16, 20, 23, 64);   // Big scale line color
		const Color4 mSmallLineColor = Color4(16, 20, 23, 38); // Small scale line color

		const float mScaleSmoothLerpCoef = 10.0f; // Scale smooth animation coefficient. More is faster
		const float mMinScale = 0.1f;             // Minimal view zoom 
		const float mMaxScale = 10.0f;            // Maximal view zoom
		const float mScaleSense = 0.1f / 120.0f;    // Mouse wheel scroll sense coefficient

		const float mScrollSmoothCoef = 15.0f;        // Scroll smooth to target interpolation coefficient
		const float mScrollSpeedDecreaseCoef = 10.0f; // Scroll speed decrease coefficient. More - speed decreasing faster
		const float mScrollBorderBounceCoef = 10.0f;  // Smooth scroll bounds bounce coefficient

	private:
		Ref<AnimationWindow> mAnimationWindow; // Animation window

		Ref<AnimationPlayer> mPlayer;    // Animation player
		Ref<AnimationClip>   mAnimation; // Animation, used for sibscribing on duration change

		float mTimeCursor = 0; // Current time of red cursor

		float mSmoothViewScroll = 0.0f;          // Time scroll in seconds smoothed, tends to target value mViewScroll
		float mViewScroll = 0.0f;                // Time scroll in seconds
		bool  mDragViewScroll = false;           // It is true when user is dragging view with right button
		float mBeginDragViewScrollOffset = 0.0f; // Offset from cursor to view scroll before starting drag
		float mViewScrollSpeed = 0.0f;           // Speed of inertial scrolling, when user dragged view and release MouseButton_Right
		Ref<FontRef> mFont;                      // Font for captions

		Ref<Text> mText;              // Captions text. Filled on each Draw() call
		Ref<FontRef> mTextFont;       // Text font
		Ref<HorizontalScrollBar> mScrollBar; // Scroll Bar
	};
}

using namespace Editor;

AnimationTimeline::AnimationTimeline()
{
	mFont = mmake<FontRef>("Arial", 8.0f);
	mTextFont = mFont.get();
}

AnimationTimeline::AnimationTimeline(const AnimationTimeline& other)
{
	mFont = mmake<FontRef>(*other.mFont);
	mTextFont = mFont.get();
}

AnimationTimeline::~AnimationTimeline()
{
}

AnimationTimeline& AnimationTimeline::operator=(const AnimationTimeline& other)
{
	mFont = mmake<FontRef>(*other.mFont);
	mTextFont = mFont.get();
	return *this;
}

void AnimationTimeline::Draw()
{
	// Draw time scale and captions
}

void AnimationTimeline::Update(float dt)
{
	// Update as regular widget and change zoom by mouse wheel
}

void AnimationTimeline::SetAnimation(const Ref<AnimationClip>& animation, const Ref<AnimationPlayer>& player /*= nullptr*/)
{
	mAnimation = animation;
	mPlayer = player;
}

void AnimationTimeline::SetScroll(float scroll)
{
	mViewScroll = scroll;
}

void AnimationTimeline::SetViewRange(float left, float right, bool force /*= true*/)
{
}

void AnimationTimeline::SetTimeCursor(float time)
{
	mTimeCursor = time;
}

float AnimationTimeline::GetTimeCursor() const
{
	return mTimeCursor;
}

float AnimationTimeline::GetScroll() const
{
	return mViewScroll;
}

void AnimationTimeline::SetScale(float scale)
{
}

float AnimationTimeline::GetScale() const
{
	return 0.0f;
}

void AnimationTimeline::SetViewMoveDisabled(bool disabled)
{
}

float AnimationTimeline::LocalToWorld(float pos) const
{
	return 0.0f;
}

float AnimationTimeline::WorldToLocal(float pos) const
{
	return 0.0f;
}

const Ref<Text>& AnimationTimeline::GetText() const
{
	return mText;
}

void AnimationTimeline::SetScrollBar(const Ref<HorizontalScrollBar>& scrollBar)
{
	mScrollBar = scrollBar;
}

const Ref<HorizontalScrollBar>& AnimationTimeline::GetScrollBar() const
{
	return mScrollBar;
}

bool AnimationTimeline::IsSameTime(float timeA, float timeB, float threshold /*= 3.0f*/) const
{
	return std::abs(timeA - timeB) < threshold;
}

bool AnimationTimeline::IsUnderPoint(const Vec2F& point)
{
	return false;
}

bool AnimationTimeline::IsScrollable() const
{
	return true;
}

bool AnimationTimeline::IsInputTransparent() const
{
	return false;
}

String AnimationTimeline::GetCreateMenuCategory()
{
	return String();
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

class AnimationTimeline : public Widget, public CursorAreaEventsListener
{
public:
    AnimationTimeline()
    {
        mTextFont = mmake<Font>();
        mText = mmake<Text>();
        mBeginMark = mmake<Sprite>();
        mEndMark = mmake<Sprite>();
        mScrollBar = mmake<HorizontalScrollBar>();
        mTimeLine = mmake<Sprite>();
    }

    ~AnimationTimeline() = default;

    void UpdateDuration(float duration)
    {
        mDuration = duration;
    }

    void DrawTimeScale()
    {
        // Drawing time scale implementation
    }

    void ChooseScaleParams(int& bigLinePeriod, double& bigLineTimeAmount)
    {
        // Chooses scale params implementation
    }

    void UpdateScrolling(float dt)
    {
        // Update scrolling implementation
    }

    void UpdateZooming(float dt)
    {
        // Update zooming implementation
    }

    void UpdateScrollBarHandleSize()
    {
        // Update scroll bar handle size implementation
    }

    void OnTransformUpdated() override
    {
        // Update scroll bar handle size implementation
    }

    void SetAnimationTimeByCursor(const Input::Cursor& cursor)
    {
        // Set animation time by cursor implementation
    }

    void OnCursorRightMousePressed(const Input::Cursor& cursor) override
    {
        // On cursor right mouse pressed implementation
    }

    void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override
    {
        // On cursor right mouse stay down implementation
    }

    void OnCursorRightMouseReleased(const Input::Cursor& cursor) override
    {
        // On cursor right mouse released implementation
    }

    void OnScrolled(float scroll) override
    {
        // On scrolled implementation
    }

private:
    Ref<Font> mTextFont;
    Ref<Text> mText;
    Ref<Sprite> mBeginMark;
    Ref<Sprite> mEndMark;
    Ref<HorizontalScrollBar> mScrollBar;
    Ref<Sprite> mTimeLine;
    CursorEventsArea mTimeLineEventsArea;

    float mViewZoom = 1.0f;
    float mSmoothViewZoom = 1.0f;
    bool mViewHasZoomed = false;
    bool mViewMoveDisabled = false;
    float mDuration = 0.0f;

    Layout mBeginMarkLayout = Layout(Vec2F(0, 1), Vec2F(0, 1), Vec2F(-6.0f, 3.0f), Vec2F(6.0f, -21.0f));
    Layout mEndMarkLayout = Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(-4.0f, 3.0f), Vec2F(6.0f, -21.0f));

    float mTextOffset = 12.0f;
    float mBigLineOffset = 13.0f;
    float mSmallLineOffset = 15.0f;
    float mOneSecondDefaultSize = 100.0f;
    float mPerfectScaleSegmentSize = 10.0f;
    float mScaleOffset = 10.0f;
    Color4 mBigLineColor = Color4(16, 20, 23, 64);
    Color4 mSmallLineColor = Color4(16, 20, 23, 38);
    float mScaleSmoothLerpCoef = 10.0f;
    float mMinScale = 0.1f;
    float mMaxScale = 10.0f;
    float mScaleSense = 0.1f / 120.0f;
    float mScrollSmoothCoef = 15.0f;
    float mScrollSpeedDecreaseCoef = 10.0f;
    float mScrollBorderBounceCoef = 10.0f;
    Ref<AnimationWindow> mAnimationWindow = nullptr;
    Ref<Player> mPlayer = nullptr;
    Ref<Animation> mAnimation = nullptr;
    int mTimeCursor = 0;
    float mSmoothViewScroll = 0.0f;
    float mViewScroll = 0.0f;
    bool mDragViewScroll = false;
    float mBeginDragViewScrollOffset = 0.0f;
    float mViewScrollSpeed = 0.0f;

    Signal<> onViewChanged;
};

// --- META ---

CLASS_BASES_META(AnimationTimeline)
{
    BASE_CLASS(o2::Widget);
    BASE_CLASS(o2::CursorAreaEventsListener);
}
END_META;

CLASS_FIELDS_META(AnimationTimeline)
{
    FIELD().PUBLIC().NAME(onViewChanged);
    FIELD().PRIVATE().DEFAULT_VALUE(12.0f).NAME(mTextOffset);
    FIELD().PRIVATE().DEFAULT_VALUE(13.0f).NAME(mBigLineOffset);
    FIELD().PRIVATE().DEFAULT_VALUE(15.0f).NAME(mSmallLineOffset);
    FIELD().PRIVATE().DEFAULT_VALUE(100.0f).NAME(mOneSecondDefaultSize);
    FIELD().PRIVATE().DEFAULT_VALUE(10.0f).NAME(mPerfectScaleSegmentSize);
    FIELD().PRIVATE().DEFAULT_VALUE(10.0f).NAME(mScaleOffset);
    FIELD().PRIVATE().DEFAULT_VALUE(Color4(16, 20, 23, 64)).NAME(mBigLineColor);
    FIELD().PRIVATE().DEFAULT_VALUE(Color4(16, 20, 23, 38)).NAME(mSmallLineColor);
    FIELD().PRIVATE().DEFAULT_VALUE(10.0f).NAME(mScaleSmoothLerpCoef);
    FIELD().PRIVATE().DEFAULT_VALUE(0.1f).NAME(mMinScale);
    FIELD().PRIVATE().DEFAULT_VALUE(10.0f).NAME(mMaxScale);
    FIELD().PRIVATE().DEFAULT_VALUE(0.1f / 120.0f).NAME(mScaleSense);
    FIELD().PRIVATE().DEFAULT_VALUE(15.0f).NAME(mScrollSmoothCoef);
    FIELD().PRIVATE().DEFAULT_VALUE(10.0f).NAME(mScrollSpeedDecreaseCoef);
    FIELD().PRIVATE().DEFAULT_VALUE(10.0f).NAME(mScrollBorderBounceCoef);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mAnimationWindow);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mPlayer);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mAnimation);
    FIELD().PRIVATE().DEFAULT_VALUE(0).NAME(mTimeCursor);
    FIELD().PRIVATE().DEFAULT_VALUE(0.0f).NAME(mSmoothViewScroll);
    FIELD().PRIVATE().DEFAULT_VALUE(0.0f).NAME(mViewScroll);
    FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mDragViewScroll);
    FIELD().PRIVATE().DEFAULT_VALUE(0.0f).NAME(mBeginDragViewScrollOffset);
    FIELD().PRIVATE().DEFAULT_VALUE(0.0f).NAME(mViewScrollSpeed);
    FIELD().PRIVATE().NAME(mTextFont);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mText);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mBeginMark);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mEndMark);
    FIELD().PRIVATE().NAME(mScrollBar);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mTimeLine);
    FIELD().PRIVATE().DEFAULT_VALUE(mTimeLineEventsArea).NAME(mTimeLineEventsArea);
};
}#include <iostream>
#include <memory>

template <typename T>
class Ref
{
public:
    Ref(T* ptr = nullptr) : m_ptr(ptr) {}

    T* operator->() const { return m_ptr; }

    T& operator*() const { return *m_ptr; }

private:
    T* m_ptr;
};

template <typename T>
class WeakRef
{
public:
    WeakRef(T* ptr = nullptr) : m_ptr(ptr) {}

    T* operator->() const { return m_ptr; }

    T& operator*() const { return *m_ptr; }

private:
    T* m_ptr;
};

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
class DynamicCast
{
public:
    template <typename U>
    static Ref<U> cast(Ref<T> ptr)
    {
        return Ref<U>(dynamic_cast<U*>(ptr.operator->()));
    }
};

class Vec2F
{
public:
    Vec2F(float x = 0, float y = 0) : m_x(x), m_y(y) {}

private:
    float m_x;
    float m_y;
};

class Layout
{
public:
    Layout(Vec2F vec1, Vec2F vec2, Vec2F vec3, Vec2F vec4)
        : m_vec1(vec1), m_vec2(vec2), m_vec3(vec3), m_vec4(vec4) {}

private:
    Vec2F m_vec1;
    Vec2F m_vec2;
    Vec2F m_vec3;
    Vec2F m_vec4;
};

class AnimationClip {};
class AnimationPlayer {};

class HorizontalScrollBar {};

class Text {};
class String {};

namespace Input
{
    class Cursor {};
}

class Editor
{
public:
    class AnimationTimeline
    {
    public:
        AnimationTimeline() {}
        AnimationTimeline(const AnimationTimeline&) {}

        void Draw() {}
        void Update(float) {}
        void SetAnimation(Ref<AnimationClip>*, Ref<AnimationPlayer>*)
        {
        }
        void SetScroll(float) {}
        void SetViewRange(float, float, bool) {}

        void SetTimeCursor(float) {}
        float GetTimeCursor() { return 0.0f; }
        float GetScroll() { return 0.0f; }
        void SetScale(float) {}
        float GetScale() { return 0.0f; }
        void SetViewMoveDisabled(bool) {}

        float LocalToWorld(float) { return 0.0f; }
        float WorldToLocal(float) { return 0.0f; }
        Ref<Text> GetText() { return m_text; }
        void SetScrollBar(Ref<HorizontalScrollBar>*) {}
        Ref<HorizontalScrollBar> GetScrollBar() { return m_scrollBar; }

        bool IsSameTime(float, float, float) { return false; }
        bool IsUnderPoint(const Vec2F&) { return false; }
        bool IsScrollable() { return false; }
        bool IsInputTransparent() { return false; }
        static String GetCreateMenuCategory() { return ""; }

    private:
        void UpdateDuration(float) {}
        void DrawTimeScale() {}
        void ChooseScaleParams(int&, double&) {}
        void UpdateScrolling(float) {}
        void UpdateZooming(float) {}
        void UpdateScrollBarHandleSize() {}
        void OnTransformUpdated() {}
        void SetAnimationTimeByCursor(const Input::Cursor&) {}
        void OnCursorRightMousePressed(const Input::Cursor&) {}
        void OnCursorRightMouseStayDown(const Input::Cursor&) {}
        void OnCursorRightMouseReleased(const Input::Cursor&) {}
        void OnScrolled(float) {}

        Ref<Text> m_text;
        Ref<HorizontalScrollBar> m_scrollBar;
        Ref<AnimationTimeline> mTimeLineEventsArea;
    };
};

int main()
{
    return 0;
}
```
Note: I have removed the `DEFAULT_VALUE(nullptr)` for member variables `mScrollBar` and `mTimeLine` as you mentioned. However, please make sure to initialize them properly before using them in your code.