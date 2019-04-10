#pragma once

#include "Scene/UI/Widget.h"
#include "Render/FontRef.h"

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


		// Sets scale length in seconds
		void SetDuration(float duration);

		// Returns scale length in seconds
		float GetDuration() const;


		// Sets current time scroll in seconds
		void SetScroll(float scroll);

		// Returns current time scroll in seconds
		float GetScroll() const;


		// Sets time scale zoom
		void SetScale(float scale);

		// Returns time scale zoom
		float GetScale() const;


		// Returns captions text drawable
		Text* GetText() const;


		// Sets scrollbar widget, used for view scroll
		void SetScrollBar(HorizontalScrollBar* scrollBar);

		// Returns scrollbar widget, used for view scroll
		HorizontalScrollBar* GetScrollBar() const;

		SERIALIZABLE(AnimationTimeline);

	private:
		const float mTextOffset = 12.0f;            // Text position offset from top border
		const float mBigLineOffset = 13.0f;         // Big scale line offset from top border
		const float mSmallLineOffset = 15.0f;       // Small scale line offset from top border
		const float mOneSecondDefaultSize = 100.0f; // One second with scale == 1 size in pixels
		const float mScaleOffset = 10.0f;           // First scale line offset from left border

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
		float mSmoothViewScroll = 0.0f;          // Time scroll in seconds smoothed, tends to target value mViewScroll
		float mViewScroll = 0.0f;                // Time scroll in seconds
		bool  mDragViewScroll = false;           // It is true when user is dragging view with right button
		float mBeginDragViewScrollOffset = 0.0f; // Offset from cursor to view scroll before starting drag
		float mViewScrollSpeed = 0.0f;           // Speed of inertial scrolling, when user dragged view and released mouse

		float mViewZoom = 1.0f;       // Time scale zoom
		float mSmoothViewZoom = 1.0f; // Smoothed and animated time scale zoom

		float mDuration = 0.0f; // Scale length in seconds

		FontRef mTextFont;       // Captions font
		Text*   mText = nullptr; // Captions text

		HorizontalScrollBar* mScrollBar = nullptr; // Scroll bar. Limited by animation duration

	private:
		// Tracks user's right mouse button and updates scroll by speed
		void UpdateScrolling(float dt);

		// Tracks user's mouse wheel and updates view scroll
		void UpdateZooming(float dt);

		// Updates scroll bar handle size by view width
		void UpdateScrollBarHandleSize();

		// Converts local time to world x position
		float LocalToWorld(float pos) const;

		// Converts world x position to local time
		float WorldToLocal(float pos) const;

		// It is called when transformation was changed and updated. Updates scroll bar handle size
		void OnTransformUpdated() override;
	};
}

CLASS_BASES_META(Editor::AnimationTimeline)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationTimeline)
{
	PRIVATE_FIELD(mTextOffset);
	PRIVATE_FIELD(mBigLineOffset);
	PRIVATE_FIELD(mSmallLineOffset);
	PRIVATE_FIELD(mOneSecondDefaultSize);
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
	PRIVATE_FIELD(mSmoothViewScroll);
	PRIVATE_FIELD(mViewScroll);
	PRIVATE_FIELD(mDragViewScroll);
	PRIVATE_FIELD(mBeginDragViewScrollOffset);
	PRIVATE_FIELD(mViewScrollSpeed);
	PRIVATE_FIELD(mViewZoom);
	PRIVATE_FIELD(mSmoothViewZoom);
	PRIVATE_FIELD(mDuration);
	PRIVATE_FIELD(mTextFont);
	PRIVATE_FIELD(mText);
	PRIVATE_FIELD(mScrollBar);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTimeline)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetDuration, float);
	PUBLIC_FUNCTION(float, GetDuration);
	PUBLIC_FUNCTION(void, SetScroll, float);
	PUBLIC_FUNCTION(float, GetScroll);
	PUBLIC_FUNCTION(void, SetScale, float);
	PUBLIC_FUNCTION(float, GetScale);
	PUBLIC_FUNCTION(Text*, GetText);
	PUBLIC_FUNCTION(void, SetScrollBar, HorizontalScrollBar*);
	PUBLIC_FUNCTION(HorizontalScrollBar*, GetScrollBar);
	PRIVATE_FUNCTION(void, UpdateScrolling, float);
	PRIVATE_FUNCTION(void, UpdateZooming, float);
	PRIVATE_FUNCTION(void, UpdateScrollBarHandleSize);
	PRIVATE_FUNCTION(float, LocalToWorld, float);
	PRIVATE_FUNCTION(float, WorldToLocal, float);
	PRIVATE_FUNCTION(void, OnTransformUpdated);
}
END_META;
