#pragma once

#include "AnimationTrack.h"
#include "o2/Utils/Math/Color.h"

namespace o2
{
	// --------------------
	// Animated Color4 value
	// --------------------
	template<>
	class AnimationTrack<o2::Color4>: public IAnimationTrack
	{
	public:
		typedef o2::Color4 ValueType;
		class Key;

	public:
		PROPERTIES(AnimationTrack<o2::Color4>);
		PROPERTY(Vector<Key>, keys, SetKeys, GetKeysNonContant); // Keys property

	public:
		// Default constructor
		AnimationTrack();

		// Copy-constructor
		AnimationTrack(const AnimationTrack<Color4>& other);

		// Assign operator
		AnimationTrack<Color4>& operator=(const AnimationTrack<Color4>& other);

		// Returns value at time
		Color4 GetValue(float position) const;

		// Returns value at time
		Color4 GetValue(float position, bool direction, int& cacheKey, int& cacheKeyApprox) const;

		// It is called when beginning keys batch change. After this call all keys modifications will not be update pproximation
		// Used for optimizing many keys change
		void BeginKeysBatchChange() override;

		// It is called when keys batch change completed. Updates approximation
		void CompleteKeysBatchingChange() override;

		// Returns track duration
		float GetDuration() const override;

		// Creates track-type specific player
		IPlayer* CreatePlayer() const override;

		// Adds keys
		void AddKeys(const Vector<Key>& keys);

		// Adds single key
		int AddKey(const Key& key);

		// Adds key at position
		int AddKey(const Key& key, float position);

		// Adds key
		int AddKey(float position, const Color4& value);

		// Removes key at position
		bool RemoveKey(float position);

		// Removes key by index
		bool RemoveKeyAt(int idx);

		// Removes all keys
		void RemoveAllKeys();

		// Returns true if animation contains key at position
		bool ContainsKey(float position) const;

		// Returns keys array
		const Vector<Key>& GetKeys() const;

		// Returns key at position
		Key GetKey(float position) const;

		// Returns key at index
		Key GetKeyAt(int idx) const;

		// Returns key by uid
		Key FindKey(UInt64 uid) const;

		// Returns key index by uid
		int FindKeyIdx(UInt64 uid) const;

		// Sets keys
		void SetKeys(const Vector<Key>& keys);

		// Returns key by position
		Key operator[](float position) const;

		// Returns tween animation from begin to end in duration with linear transition
		static AnimationTrack<Color4> Linear(const Color4& begin, const Color4& end, float duration = 1.0f);

		SERIALIZABLE(AnimationTrack<Color4>);

	public:
		// ----------------------
		// Animation track player
		// ----------------------
		class Player: public IPlayer
		{
		public:
			PROPERTIES(Player);
			GETTER(o2::Color4, value, GetValue);                           // Current value getter
			SETTER(o2::Color4*, target, SetTarget);                        // Bind target setter
			SETTER(Function<void()>, targetDelegate, SetTargetDelegate);   // Bind target change event setter  
			SETTER(IValueProxy<o2::Color4>*, targetProxy, SetTargetProxy); // Bind proxy setter

		public:
			// Default constructor
			Player();

			// Destructor
			~Player();

			// Value type cast operator
			operator Color4() const;

			// Sets target pointer
			void SetTarget(Color4* value);

			// Sets target pointer and change event
			void SetTarget(Color4* value, const Function<void()>& changeEvent);

			// Sets target change event
			void SetTargetDelegate(const Function<void()>& changeEvent) override;

			// Sets target property pointer
			void SetTargetProxy(IValueProxy<Color4>* setter);

			// Sets animation track
			void SetTrack(AnimationTrack<Color4>* track);

			// Returns animation track
			AnimationTrack<Color4>* GetTrackT() const;

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
			Color4 GetValue() const;

			IOBJECT(Player);

		protected:
			AnimationTrack<Color4>* mTrack = nullptr; // Animation track

			Color4 mCurrentValue; // Current animation track

			float mPrevInDurationTime = 0.0f; // Previous evaluation in duration time
			int   mPrevKey = 0;               // Previous evaluation key index
			int   mPrevKeyApproximation = 0;  // Previous evaluation key approximation index

			Color4*              mTarget = nullptr;      // Animation target value pointer
			Function<void()>    mTargetDelegate;         // Animation target value change event
			IValueProxy<Color4>* mTargetProxy = nullptr; // Animation target proxy pointer

		protected:
			// Evaluates value
			void Evaluate() override;

			// Registering this in animatable value agent
			void RegMixer(AnimationState* state, const String& path) override;
		};

	public:
		// -------------
		// Animation key
		// -------------
		class Key: public ISerializable
		{
		public:
			UInt64 uid;      // Random unique id @SERIALIZABLE
			float  position; // Position on time line, in seconds @SERIALIZABLE
			Color4  value;	 // Value @SERIALIZABLE

		public:
			// Default constructor
			Key();

			// Constructor from value
			Key(const Color4& value);

			// Constructor from position and value
			Key(float position, const Color4& value);

			// Copy-constructor
			Key(const Key& other);

			// Assign operator from other key
			Key& operator=(const Key& other);

			// Assign operator from value
			Key& operator=(const Color4& value);

			// Value type cast operator
			operator Color4() const;

			// Equals operator
			bool operator==(const Key& other) const;

			SERIALIZABLE(Key);
			};

	protected:
		bool mBatchChange = false; // It is true when began batch change
		bool mChangedKeys = false; // It is true when some keys changed during batch change

		Vector<Key> mKeys; // Animation keys @SERIALIZABLE

	protected:
		// Returns keys (for property)
		Vector<Key> GetKeysNonContant();
	};
}

CLASS_BASES_META(o2::AnimationTrack<o2::Color4>)
{
	BASE_CLASS(o2::IAnimationTrack);
}
END_META;
CLASS_FIELDS_META(o2::AnimationTrack<o2::Color4>)
{
	FIELD().NAME(keys).PUBLIC();
	FIELD().DEFAULT_VALUE(false).NAME(mBatchChange).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mChangedKeys).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mKeys).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::AnimationTrack<o2::Color4>)
{

	PUBLIC_FUNCTION(Color4, GetValue, float);
	PUBLIC_FUNCTION(Color4, GetValue, float, bool, int&, int&);
	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
	PUBLIC_FUNCTION(float, GetDuration);
	PUBLIC_FUNCTION(IPlayer*, CreatePlayer);
	PUBLIC_FUNCTION(void, AddKeys, const Vector<Key>&);
	PUBLIC_FUNCTION(int, AddKey, const Key&);
	PUBLIC_FUNCTION(int, AddKey, const Key&, float);
	PUBLIC_FUNCTION(int, AddKey, float, const Color4&);
	PUBLIC_FUNCTION(bool, RemoveKey, float);
	PUBLIC_FUNCTION(bool, RemoveKeyAt, int);
	PUBLIC_FUNCTION(void, RemoveAllKeys);
	PUBLIC_FUNCTION(bool, ContainsKey, float);
	PUBLIC_FUNCTION(const Vector<Key>&, GetKeys);
	PUBLIC_FUNCTION(Key, GetKey, float);
	PUBLIC_FUNCTION(Key, GetKeyAt, int);
	PUBLIC_FUNCTION(Key, FindKey, UInt64);
	PUBLIC_FUNCTION(int, FindKeyIdx, UInt64);
	PUBLIC_FUNCTION(void, SetKeys, const Vector<Key>&);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<Color4>, Linear, const Color4&, const Color4&, float);
	PROTECTED_FUNCTION(Vector<Key>, GetKeysNonContant);
}
END_META;

CLASS_BASES_META(o2::AnimationTrack<o2::Color4>::Player)
{
	BASE_CLASS(IPlayer);
}
END_META;
CLASS_FIELDS_META(o2::AnimationTrack<o2::Color4>::Player)
{
	FIELD().NAME(value).PUBLIC();
	FIELD().NAME(target).PUBLIC();
	FIELD().NAME(targetDelegate).PUBLIC();
	FIELD().NAME(targetProxy).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTrack).PROTECTED();
	FIELD().NAME(mCurrentValue).PROTECTED();
	FIELD().DEFAULT_VALUE(0.0f).NAME(mPrevInDurationTime).PROTECTED();
	FIELD().DEFAULT_VALUE(0).NAME(mPrevKey).PROTECTED();
	FIELD().DEFAULT_VALUE(0).NAME(mPrevKeyApproximation).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTarget).PROTECTED();
	FIELD().NAME(mTargetDelegate).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTargetProxy).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::AnimationTrack<o2::Color4>::Player)
{

	PUBLIC_FUNCTION(void, SetTarget, Color4*);
	PUBLIC_FUNCTION(void, SetTarget, Color4*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetDelegate, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxy, IValueProxy<Color4>*);
	PUBLIC_FUNCTION(void, SetTrack, AnimationTrack<Color4>*);
	PUBLIC_FUNCTION(AnimationTrack<Color4>*, GetTrackT);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxyVoid, void*);
	PUBLIC_FUNCTION(void, SetTrack, IAnimationTrack*);
	PUBLIC_FUNCTION(IAnimationTrack*, GetTrack);
	PUBLIC_FUNCTION(Color4, GetValue);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(void, RegMixer, AnimationState*, const String&);
}
END_META;

CLASS_BASES_META(o2::AnimationTrack<o2::Color4>::Key)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AnimationTrack<o2::Color4>::Key)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(uid).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(position).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(value).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::AnimationTrack<o2::Color4>::Key)
{
}
END_META;
