#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Render/FontRef.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Editor/DragHandle.h"

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
		Function<void()> onViewChanged; // it is called when scroll or zoom were changed

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
		void SetAnimation(AnimationClip* animation, AnimationPlayer* player = nullptr);

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
		Text* GetText() const;

		// Sets scrollbar widget, used for view scroll
		void SetScrollBar(HorizontalScrollBar* scrollBar);

		// Returns scrollbar widget, used for view scroll
		HorizontalScrollBar* GetScrollBar() const;

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
		const float mScaleSense = 0.1f/120.0f;    // Mouse wheel scroll sense coefficient

		const float mScrollSmoothCoef = 15.0f;        // Scroll smooth to target interpolation coefficient
		const float mScrollSpeedDecreaseCoef = 10.0f; // Scroll speed decrease coefficient. More - speed decreasing faster
		const float mScrollBorderBounceCoef = 10.0f;  // Smooth scroll bounds bounce coefficient

	private:
		AnimationWindow* mAnimationWindow = nullptr; // Animation window

		AnimationPlayer* mPlayer = nullptr;    // Animation player
		AnimationClip*   mAnimation = nullptr; // Animation, used for sibscribing on duration change

		float mTimeCursor = 0; // Current time of red cursor

		float mSmoothViewScroll = 0.0f;          // Time scroll in seconds smoothed, tends to target value mViewScroll
		float mViewScroll = 0.0f;                // Time scroll in seconds
		bool  mDragViewScroll = false;           // It is true when user is dragging view with right button
		float mBeginDragViewScrollOffset = 0.0f; // Offset from cursor to view scroll before starting drag
		float mViewScrollSpeed = 0.0f;           // Speed of inertial scrolling, when user dragged view and released mouse

		float mViewZoom = 1.0f;       // Time scale zoom
		float mSmoothViewZoom = 1.0f; // Smoothed and animated time scale zoom
		bool  mViewHasZoomed = false; // True when area has scrolled at this frame, resets to false on update

		bool mViewMoveDisabled = false; // Is moving and zooming view disabled. It is disabling when editor has switched to curves edit mode

		float mDuration = 0.0f; // Scale length in seconds

		FontRef mTextFont;       // Captions font
		Text*   mText = nullptr; // Captions text

		Sprite* mBeginMark = nullptr; // Begin animation mark sprite. Begin is always at zero
		Sprite* mEndMark = nullptr;   // End animation mark sprite, at duration

		Layout mBeginMarkLayout = Layout(Vec2F(0, 1), Vec2F(0, 1), Vec2F(-6.0f, 3.0f), Vec2F(6.0f, -21.0f));
		Layout mEndMarkLayout = Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(-4.0f, 3.0f), Vec2F(6.0f, -21.0f));

		HorizontalScrollBar* mScrollBar = nullptr; // Scroll bar. Limited by animation duration

		Sprite*          mTimeLine = nullptr; // Red time line
		CursorEventsArea mTimeLineEventsArea; // Top area events listeners, used for moving red line of time

	private:
		// Updates duration and scrollbars
		void UpdateDuration(float duration);

		// Draws time scale with scroll and zoom
		void DrawTimeScale();

		// Chooses well view of time scale by zoom
		void ChooseScaleParams(int& bigLinePeriod, double& bigLineTimeAmount);

		// Tracks user's right mouse button and updates scroll by speed
		void UpdateScrolling(float dt);

		// Tracks user's mouse wheel and updates view scroll
		void UpdateZooming(float dt);

		// Updates scroll bar handle size by view width
		void UpdateScrollBarHandleSize();

		// It is called when transformation was changed and updated. Updates scroll bar handle size
		void OnTransformUpdated() override;

		// Sets animation time by cursor screen position
		void SetAnimationTimeByCursor(const Input::Cursor& cursor);

		// It is called when right mouse button was pressed on this, begin scrolling
		void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

		// It is called when right mouse button stay down on this, scrolling
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time), ends scrolling
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// It is called when scrolling
		void OnScrolled(float scroll) override;

		friend class AnimationWindow;

		template<typename AnimationTrackType>
		friend class KeyFramesTrackControl;
	};
}

CLASS_BASES_META(Editor::AnimationTimeline)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::CursorAreaEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationTimeline)
{
	PUBLIC_FIELD(onViewChanged);
	PRIVATE_FIELD(mTextOffset).DEFAULT_VALUE(12.0f);
	PRIVATE_FIELD(mBigLineOffset).DEFAULT_VALUE(13.0f);
	PRIVATE_FIELD(mSmallLineOffset).DEFAULT_VALUE(15.0f);
	PRIVATE_FIELD(mOneSecondDefaultSize).DEFAULT_VALUE(100.0f);
	PRIVATE_FIELD(mPerfectScaleSegmentSize).DEFAULT_VALUE(10.0f);
	PRIVATE_FIELD(mScaleOffset).DEFAULT_VALUE(10.0f);
	PRIVATE_FIELD(mBigLineColor).DEFAULT_VALUE(Color4(16, 20, 23, 64));
	PRIVATE_FIELD(mSmallLineColor).DEFAULT_VALUE(Color4(16, 20, 23, 38));
	PRIVATE_FIELD(mScaleSmoothLerpCoef).DEFAULT_VALUE(10.0f);
	PRIVATE_FIELD(mMinScale).DEFAULT_VALUE(0.1f);
	PRIVATE_FIELD(mMaxScale).DEFAULT_VALUE(10.0f);
	PRIVATE_FIELD(mScaleSense).DEFAULT_VALUE(0.1f/120.0f);
	PRIVATE_FIELD(mScrollSmoothCoef).DEFAULT_VALUE(15.0f);
	PRIVATE_FIELD(mScrollSpeedDecreaseCoef).DEFAULT_VALUE(10.0f);
	PRIVATE_FIELD(mScrollBorderBounceCoef).DEFAULT_VALUE(10.0f);
	PRIVATE_FIELD(mAnimationWindow).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mPlayer).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mAnimation).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mTimeCursor).DEFAULT_VALUE(0);
	PRIVATE_FIELD(mSmoothViewScroll).DEFAULT_VALUE(0.0f);
	PRIVATE_FIELD(mViewScroll).DEFAULT_VALUE(0.0f);
	PRIVATE_FIELD(mDragViewScroll).DEFAULT_VALUE(false);
	PRIVATE_FIELD(mBeginDragViewScrollOffset).DEFAULT_VALUE(0.0f);
	PRIVATE_FIELD(mViewScrollSpeed).DEFAULT_VALUE(0.0f);
	PRIVATE_FIELD(mViewZoom).DEFAULT_VALUE(1.0f);
	PRIVATE_FIELD(mSmoothViewZoom).DEFAULT_VALUE(1.0f);
	PRIVATE_FIELD(mViewHasZoomed).DEFAULT_VALUE(false);
	PRIVATE_FIELD(mViewMoveDisabled).DEFAULT_VALUE(false);
	PRIVATE_FIELD(mDuration).DEFAULT_VALUE(0.0f);
	PRIVATE_FIELD(mTextFont);
	PRIVATE_FIELD(mText).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mBeginMark).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mEndMark).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mBeginMarkLayout).DEFAULT_VALUE(Layout(Vec2F(0, 1), Vec2F(0, 1), Vec2F(-6.0f, 3.0f), Vec2F(6.0f, -21.0f)));
	PRIVATE_FIELD(mEndMarkLayout).DEFAULT_VALUE(Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(-4.0f, 3.0f), Vec2F(6.0f, -21.0f)));
	PRIVATE_FIELD(mScrollBar).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mTimeLine).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mTimeLineEventsArea);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTimeline)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetAnimation, AnimationClip*, AnimationPlayer*);
	PUBLIC_FUNCTION(void, SetScroll, float);
	PUBLIC_FUNCTION(void, SetViewRange, float, float, bool);
	PUBLIC_FUNCTION(void, SetTimeCursor, float);
	PUBLIC_FUNCTION(float, GetTimeCursor);
	PUBLIC_FUNCTION(float, GetScroll);
	PUBLIC_FUNCTION(void, SetScale, float);
	PUBLIC_FUNCTION(float, GetScale);
	PUBLIC_FUNCTION(void, SetViewMoveDisabled, bool);
	PUBLIC_FUNCTION(float, LocalToWorld, float);
	PUBLIC_FUNCTION(float, WorldToLocal, float);
	PUBLIC_FUNCTION(Text*, GetText);
	PUBLIC_FUNCTION(void, SetScrollBar, HorizontalScrollBar*);
	PUBLIC_FUNCTION(HorizontalScrollBar*, GetScrollBar);
	PUBLIC_FUNCTION(bool, IsSameTime, float, float, float);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(bool, IsInputTransparent);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PRIVATE_FUNCTION(void, UpdateDuration, float);
	PRIVATE_FUNCTION(void, DrawTimeScale);
	PRIVATE_FUNCTION(void, ChooseScaleParams, int&, double&);
	PRIVATE_FUNCTION(void, UpdateScrolling, float);
	PRIVATE_FUNCTION(void, UpdateZooming, float);
	PRIVATE_FUNCTION(void, UpdateScrollBarHandleSize);
	PRIVATE_FUNCTION(void, OnTransformUpdated);
	PRIVATE_FUNCTION(void, SetAnimationTimeByCursor, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnScrolled, float);
}
END_META;
