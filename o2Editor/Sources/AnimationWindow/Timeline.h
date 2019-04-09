#pragma once

#include "Scene/UI/Widget.h"
#include "Render/FontRef.h"

using namespace o2;

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

		SERIALIZABLE(AnimationTimeline);

	private:
		const float mTextOffset = 12.0f;            // Text position offset from top border
		const float mBigLineOffset = 13.0f;         // Big scale line offset from top border
		const float mSmallLineOffset = 15.0f;       // Small scale line offset from top border
		const float mOneSecondDefaultSize = 100.0f; // One second with scale == 1 size in pixels
		const float mScaleOffset = 10.0f;           // First scale line offset from left border

		const Color4 mBigLineColor = Color4(16, 20, 23, 64);
		const Color4 mSmallLineColor = Color4(16, 20, 23, 38);

		const float mScaleSmoothLerpCoef = 10.0f;
		const float mMinScale = 0.1f;
		const float mMaxScale = 10.0f;
		const float mScaleSense = 0.1f/120.0f;

		float mScroll = 0.0f; // Time scroll in seconds

		float mScale = 1.0f;       // Time scale zoom
		float mSmoothScale = 1.0f; // Smoothed and animated time scale zoom

		FontRef mTextFont;       // Captions font
		Text*   mText = nullptr; // Captions text
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
	PRIVATE_FIELD(mScroll);
	PRIVATE_FIELD(mScale);
	PRIVATE_FIELD(mSmoothScale);
	PRIVATE_FIELD(mTextFont);
	PRIVATE_FIELD(mText);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTimeline)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetScroll, float);
	PUBLIC_FUNCTION(float, GetScroll);
	PUBLIC_FUNCTION(void, SetScale, float);
	PUBLIC_FUNCTION(float, GetScale);
	PUBLIC_FUNCTION(Text*, GetText);
}
END_META;
