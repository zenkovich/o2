#pragma once
#include "AnimationTrack.h"
#include "o2/Utils/Math/Curve.h"
#include "o2/Utils/Math/Spline.h"
#include "o2/Utils/Math/Vector2.h"

namespace o2
{
	// --------------------
	// Animated Vec2F value
	// --------------------
	template<>
	class AnimationTrack<o2::Vec2F>: public IAnimationTrack
	{
	public:
		typedef o2::Vec2F ValueType;

	public:
		Curve  timeCurve; // Time curve. represents speed on spline @SERIALIZABLE
		Spline spline;    // Movement trajectory spline @SERIALIZABLE

	public:
		// Default constructor
		AnimationTrack();

		// Copy-constructor
		AnimationTrack(const AnimationTrack<Vec2F>& other);

		// Assign operator
		AnimationTrack<Vec2F>& operator=(const AnimationTrack<Vec2F>& other);

		// Returns value at time
		Vec2F GetValue(float position) const;

		// Returns value at time with caching
		Vec2F GetValue(float position, bool direction, int& cacheTimeKey, int& cacheTimeKeyApprox,
					   int& cacheSplineKey, int& cacheSplineKeyApprox) const;

		// It is called when beginning keys batch change. After this call all keys modifications will not be update pproximation
		// Used for optimizing many keys change
		void BeginKeysBatchChange() override;

		// It is called when keys batch change completed. Updates approximation
		void CompleteKeysBatchingChange() override;

		// Returns track duration
		float GetDuration() const override;

		// Creates track-type specific player
		IPlayer* CreatePlayer() const override;

		// Returns parametric specified Animation trackp
		// Sample: Parametric(someBegin, someEnd, 1.0f, 0.0f, 0.4f, 1.0f, 0.6f) 
		static AnimationTrack<Vec2F> Parametric(const Vec2F& begin, const Vec2F& end, float duration,
												float beginCoef, float beginCoefPosition,
												float endCoef, float endCoefPosition);

		// Returns tween animation from begin to end in duration with ease in
		static AnimationTrack<Vec2F> EaseIn(const Vec2F& begin, const Vec2F& end, float duration = 1.0f, float strongness = 0.4f);

		// Returns tween animation from begin to end in duration with ease out
		static AnimationTrack<Vec2F> EaseOut(const Vec2F& begin, const Vec2F& end, float duration = 1.0f, float strongness = 0.4f);

		// Returns tween animation from begin to end in duration with ease in-out
		static AnimationTrack<Vec2F> EaseInOut(const Vec2F& begin, const Vec2F& end, float duration = 1.0f, float strongness = 0.4f);

		// Returns tween animation from begin to end in duration with linear transition
		static AnimationTrack<Vec2F> Linear(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

		SERIALIZABLE(AnimationTrack<Vec2F>);

	public:
		// ----------------------
		// Animation track player
		// ----------------------
		class Player: public IPlayer
		{
		public:
			PROPERTIES(Player);
			GETTER(o2::Vec2F, value, GetValue);                           // Current value getter
			SETTER(o2::Vec2F*, target, SetTarget);                        // Bind target setter
			SETTER(Function<void()>, targetDelegate, SetTargetDelegate);  // Bind target change event setter  
			SETTER(IValueProxy<o2::Vec2F>*, targetProxy, SetTargetProxy); // Bind proxy setter

		public:
			// Default constructor
			Player();

			// Destructor
			~Player();

			// Value type cast operator
			operator Vec2F() const;

			// Sets target pointer
			void SetTarget(Vec2F* value);

			// Sets target pointer and change event
			void SetTarget(Vec2F* value, const Function<void()>& changeEvent);

			// Sets target change event
			void SetTargetDelegate(const Function<void()>& changeEvent) override;

			// Sets target property pointer
			void SetTargetProxy(IValueProxy<Vec2F>* setter);

			// Sets animation track
			void SetTrack(AnimationTrack<Vec2F>* track);

			// Returns animation track
			AnimationTrack<Vec2F>* GetTrackT() const;

			// Sets target by void pointer
			void SetTargetVoid(void* target) override;

			// Sets target by void pointer and change event
			void SetTargetVoid(void* target, const Function<void()>& changeEvent) override;

			// Sets target property by void pointer
			void SetTargetProxyVoid(void* target) override;

			// Sets animation track
			void SetTrack(IAnimationTrack* track) override;

			// Returns animation track
			IAnimationTrack* GetTrack() const override;

			// Returns current value
			Vec2F GetValue() const;

			IOBJECT(Player);

		protected:
			AnimationTrack<Vec2F>* mTrack = nullptr; // Animation track

			Vec2F mCurrentValue; // Current animation track value

			float mPrevInDurationTime = 0.0f;       // Previous evaluation in duration time
			int   mPrevTimeKey = 0;                 // Previous evaluation time key index
			int   mPrevTimeKeyApproximation = 0;    // Previous evaluation time key approximation index
			int   mPrevSplineKey = 0;               // Previous evaluation spline key index
			int   mPrevSplineKeyApproximation = 0;  // Previous evaluation spline key approximation index

			Vec2F*              mTarget = nullptr;      // Animation target value pointer
			Function<void()>    mTargetDelegate;        // Animation target value change event
			IValueProxy<Vec2F>* mTargetProxy = nullptr; // Animation target proxy pointer

		protected:
			// Evaluates value
			void Evaluate() override;

			// Registering this in animatable value agent
			void RegMixer(AnimationState* state, const String& path) override;
		};

	protected:
		// It is called when curve updated keys and calculated duration
		void OnCurveChanged();
	};
}

CLASS_BASES_META(o2::AnimationTrack<o2::Vec2F>)
{
	BASE_CLASS(o2::IAnimationTrack);
}
END_META;
CLASS_FIELDS_META(o2::AnimationTrack<o2::Vec2F>)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(timeCurve).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(spline).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::AnimationTrack<o2::Vec2F>)
{

	PUBLIC_FUNCTION(Vec2F, GetValue, float);
	PUBLIC_FUNCTION(Vec2F, GetValue, float, bool, int&, int&, int&, int&);
	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
	PUBLIC_FUNCTION(float, GetDuration);
	PUBLIC_FUNCTION(IPlayer*, CreatePlayer);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, Parametric, const Vec2F&, const Vec2F&, float, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, EaseIn, const Vec2F&, const Vec2F&, float, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, EaseOut, const Vec2F&, const Vec2F&, float, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, EaseInOut, const Vec2F&, const Vec2F&, float, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Vec2F>, Linear, const Vec2F&, const Vec2F&, float);
	PROTECTED_FUNCTION(void, OnCurveChanged);
}
END_META;

CLASS_BASES_META(o2::AnimationTrack<o2::Vec2F>::Player)
{
	BASE_CLASS(IPlayer);
}
END_META;
CLASS_FIELDS_META(o2::AnimationTrack<o2::Vec2F>::Player)
{
	FIELD().NAME(value).PUBLIC();
	FIELD().NAME(target).PUBLIC();
	FIELD().NAME(targetDelegate).PUBLIC();
	FIELD().NAME(targetProxy).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTrack).PROTECTED();
	FIELD().NAME(mCurrentValue).PROTECTED();
	FIELD().DEFAULT_VALUE(0.0f).NAME(mPrevInDurationTime).PROTECTED();
	FIELD().DEFAULT_VALUE(0).NAME(mPrevTimeKey).PROTECTED();
	FIELD().DEFAULT_VALUE(0).NAME(mPrevTimeKeyApproximation).PROTECTED();
	FIELD().DEFAULT_VALUE(0).NAME(mPrevSplineKey).PROTECTED();
	FIELD().DEFAULT_VALUE(0).NAME(mPrevSplineKeyApproximation).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTarget).PROTECTED();
	FIELD().NAME(mTargetDelegate).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTargetProxy).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::AnimationTrack<o2::Vec2F>::Player)
{

	PUBLIC_FUNCTION(void, SetTarget, Vec2F*);
	PUBLIC_FUNCTION(void, SetTarget, Vec2F*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetDelegate, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxy, IValueProxy<Vec2F>*);
	PUBLIC_FUNCTION(void, SetTrack, AnimationTrack<Vec2F>*);
	PUBLIC_FUNCTION(AnimationTrack<Vec2F>*, GetTrackT);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxyVoid, void*);
	PUBLIC_FUNCTION(void, SetTrack, IAnimationTrack*);
	PUBLIC_FUNCTION(IAnimationTrack*, GetTrack);
	PUBLIC_FUNCTION(Vec2F, GetValue);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(void, RegMixer, AnimationState*, const String&);
}
END_META;
