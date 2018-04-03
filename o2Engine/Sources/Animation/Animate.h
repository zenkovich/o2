#pragma once

#include "AnimatedFloat.h"
#include "AnimatedValue.h"
#include "AnimatedVector.h"
#include "Animation.h"

namespace o2
{
	// --------------------------------------------
	// Class for building simple animation sequence
	// --------------------------------------------
	class Animate
	{
	public:
		// Constructor. Takes object as parameter
		Animate(IObject& object);

		// Destructor
		~Animate();

		// Animation cast operator. Needs to store as animation
		operator Animation() const;

		// Inserts delay for seconds
		Animate& Wait(float seconds);

		// Applies stored transformations after seconds
		Animate& For(float seconds);

		// Splits sequence
		Animate& Then();

		// Moves object in (x,y)
		Animate& Move(float x, float y);

		// Moves object into position
		Animate& Move(const Vec2F& point);

		// Moves object by path from points
		Animate& Move(const Vector<Vec2F>& points);

		// Changes alpha
		Animate& Alpha(float alpha);

		// Shows object
		Animate& Show();

		// Hides object
		Animate& Hide();

		// Sets color
		Animate& Color(const Color4& color);

		// Sets color
		Animate& Color(int r, int g, int b, int a);

		// Sets scale
		Animate& Scale(float scale);

		// Sets scale
		Animate& Scale(const Vec2F& scale);

		// Rotates object
		Animate& Rotate(float angle);

		// Invokes function
		Animate& Invoke(const Function<void()>& function);

		// Sets animation looped
		Animate& Looped();

		// Sets pin pong loop
		Animate& PingPong();

		// Changes specified parameter
		template<typename T>
		Animate& Change(T* target, const T& value);

	protected:
		// -----------------------
		// Key container interface
		// -----------------------
		struct IKeyContainer
		{
			// Virtual destructor
			virtual ~IKeyContainer() {}

			// Applies stored key to animation
			virtual void Apply(float time) = 0;
		};

		// ----------------------
		// Template key container
		template<typename T>
		struct KeyContainer: public IKeyContainer
		{
			typename AnimatedValue<T>::Key key;
			AnimatedValue<T>*              animatedValue;

			~KeyContainer() {}

			void Apply(float time)
			{
				key.position = time;
				animatedValue->AddKey(key);
			}
		};

		typedef Vector<IKeyContainer*> KeyContainerVec;

	protected:
		Animation              mAnimation;                       // Building animation
		bool                   mKeysApplied = false;             // Is stored keys was applied
		float                  mTime = 0.0f;                     // Current sequence time
		KeyContainerVec        mKeyContainers;                   // Stored keys that applies in For()
		Function<void()>       mFunction;                        // Stored callback that applies in For()
		AnimatedValue<Color4>* mColorAnimatedValue = nullptr;    // Color animated value, stores when needs
		AnimatedValue<Vec2F>*  mPositionAnimatedValue = nullptr; // Position animated value, stores when needs
		AnimatedValue<Vec2F>*  mScaleAnimatedValue = nullptr;    // Scale animated value, stores when needs
		AnimatedValue<float>*  mRotationAnimatedValue = nullptr; // Rotation animated value, stores when needs

	protected:
		// Returns animated value for target, or creates it
		template<typename T>
		AnimatedValue<T>* GetAnimatedValue(T* target)
		{
			auto res = mAnimation.GetAnimationValue(target);

			if (!res)
				res = mAnimation.AddAnimationValue(target);

			return res;
		}

		// Checks color animated value: creates them if needed
		void CheckColorAnimatedValue();

		// Checks position animated value: creates them if needed
		void CheckPositionAnimatedvalue();

		// Checks scale animated value: creates them if needed
		void CheckScaleAnimatedValue();

		// Checks rotate animated value: creates them if needed
		void CheckRotateAnimatedValue();

		// Checks applied keys: if keys was applied, clears keys containers
		void CheckAppliedKeys();

		// Applies keys and function to animation at current time
		void ApplyKeys();
	};

	template<typename T>
	Animate& Animate::Change(T* target, const T& value)
	{
		CheckAppliedKeys();

		KeyContainer<T>* container = mnew KeyContainer<T>();
		container->animatedValue = GetAnimatedValue(target);
		container->key.value = value;
		mKeyContainers.Add(container);

		return *this;
	}

}
