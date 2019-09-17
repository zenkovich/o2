#pragma once

#include "Render/FontRef.h"
#include "Scene/UI/Widget.h"
#include "Utils/Editor/DragHandle.h"
#include "Events/CursorEventsArea.h"

using namespace o2;

namespace o2
{
	class HorizontalScrollBar;
}

namespace Editor
{
	// ------------------------------------
	// Draws time scale in animation window
	// ------------------------------------
	class AnimationTimeline : public Widget
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
		void SetAnimation(Animation* animation);

		// Sets current time scroll in seconds
		void SetScroll(float scroll);

		// Sets view range, left view border by left, and right border by right
		void SetViewRange(float left, float right);

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
		bool IsSameTime(float timeA, float timeB, float threshold = 1.7f) const;

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
		Animation* mAnimation = nullptr; // Editing animation

		float mSmoothViewScroll = 0.0f;          // Time scroll in seconds smoothed, tends to target value mViewScroll
		float mViewScroll = 0.0f;                // Time scroll in seconds
		bool  mDragViewScroll = false;           // It is true when user is dragging view with right button
		float mBeginDragViewScrollOffset = 0.0f; // Offset from cursor to view scroll before starting drag
		float mViewScrollSpeed = 0.0f;           // Speed of inertial scrolling, when user dragged view and released mouse

		float mViewZoom = 1.0f;       // Time scale zoom
		float mSmoothViewZoom = 1.0f; // Smoothed and animated time scale zoom

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
		void UpdateDuration();

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
	};
}

CLASS_BASES_META(Editor::AnimationTimeline)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationTimeline)
{
	PUBLIC_FIELD(onViewChanged);
	PRIVATE_FIELD(mTextOffset);
	PRIVATE_FIELD(mBigLineOffset);
	PRIVATE_FIELD(mSmallLineOffset);
	PRIVATE_FIELD(mOneSecondDefaultSize);
	PRIVATE_FIELD(mPerfectScaleSegmentSize);
	PRIVATE_FIELD(mScaleOffset);
	PRIVATE_FIELD(mBigLineColor);
	PRIVATE_FIELD(mSmallLineColor);
	PRIVATE_FIELD(mScaleSmoothLerpCoef);
	PRIVATE_FIELD(mMinScale);
	PRIVATE_FIELD(mMaxScale);
	PRIVATE_FIELD(mScaleSense);
	PRIVATE_FIELD(mScrollSmoothCoef);
	PRIVATE_FIELD(mScrollSpeedDecreaseCoef);
	PRIVATE_FIELD(mScrollBorderBounceCoef);
	PRIVATE_FIELD(mAnimation);
	PRIVATE_FIELD(mSmoothViewScroll);
	PRIVATE_FIELD(mViewScroll);
	PRIVATE_FIELD(mDragViewScroll);
	PRIVATE_FIELD(mBeginDragViewScrollOffset);
	PRIVATE_FIELD(mViewScrollSpeed);
	PRIVATE_FIELD(mViewZoom);
	PRIVATE_FIELD(mSmoothViewZoom);
	PRIVATE_FIELD(mViewMoveDisabled);
	PRIVATE_FIELD(mDuration);
	PRIVATE_FIELD(mTextFont);
	PRIVATE_FIELD(mText);
	PRIVATE_FIELD(mBeginMark);
	PRIVATE_FIELD(mEndMark);
	PRIVATE_FIELD(mBeginMarkLayout);
	PRIVATE_FIELD(mEndMarkLayout);
	PRIVATE_FIELD(mScrollBar);
	PRIVATE_FIELD(mTimeLine);
	PRIVATE_FIELD(mTimeLineEventsArea);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTimeline)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetAnimation, Animation*);
	PUBLIC_FUNCTION(void, SetScroll, float);
	PUBLIC_FUNCTION(void, SetViewRange, float, float);
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
	PRIVATE_FUNCTION(void, UpdateDuration);
	PRIVATE_FUNCTION(void, DrawTimeScale);
	PRIVATE_FUNCTION(void, ChooseScaleParams, int&, double&);
	PRIVATE_FUNCTION(void, UpdateScrolling, float);
	PRIVATE_FUNCTION(void, UpdateZooming, float);
	PRIVATE_FUNCTION(void, UpdateScrollBarHandleSize);
	PRIVATE_FUNCTION(void, OnTransformUpdated);
	PRIVATE_FUNCTION(void, SetAnimationTimeByCursor, const Input::Cursor&);
}
END_META;
