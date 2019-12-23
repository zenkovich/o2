#pragma once

#include "Animation/Animation.h"
#include "Animation/AnimationState.h"
#include "Scene/Component.h"
#include "Utils/Debug/Debug.h"
#include "Utils/Editor/Attributes/DefaultTypeAttribute.h"
#include "Utils/Editor/Attributes/DontDeleteAttribute.h"
#include "Utils/Editor/Attributes/InvokeOnChangeAttribute.h"

namespace o2
{
	// -------------------
	// Animation component
	// -------------------
	class AnimationComponent: public Component
	{
	public:
		// Default constructor
		AnimationComponent();

		// Copy-constructor
		AnimationComponent(const AnimationComponent& other);

		// Destructor
		~AnimationComponent();

		// Copy-operator
		AnimationComponent& operator=(const AnimationComponent& other);

		// Updates animations, blendings and assigning blended values
		void Update(float dt) override;

		// Adds new animation state and returns him
		AnimationState* AddState(AnimationState* state);

		// Adds new animation state and returns him
		AnimationState* AddState(const String& name, const Animation& animation,
								 const AnimationMask& mask, float weight);

		// Adds new animation state and returns him
		AnimationState* AddState(const String& name);

		// Removes animation state
		void RemoveState(AnimationState* state);

		// Removes animation state by name
		void RemoveState(const String& name);

		// Removes all animation states
		void RemoveAllStates();

		// Returns state with specified name. Returns nullptr if can't find state with specified name
		AnimationState* GetState(const String& name);

		// Returns all states array
		const AnimationStatesVec& GetStates() const;

		// Creates new state and plays him
		AnimationState* Play(const Animation& animation, const String& name);

		// Creates new state and plays him
		AnimationState* Play(const Animation& animation);

		// Searches animation state with name and plays him
		AnimationState* Play(const String& name);

		// Creates new state, and blends animation with duration
		AnimationState* BlendTo(const Animation& animation, const String& name, float duration = 1.0f);

		// Creates new state, and blends animation with duration
		AnimationState* BlendTo(const Animation& animation, float duration = 1.0f);

		// Creates new state, and blends animation with duration
		AnimationState* BlendTo(const String& name, float duration = 1.0f);

		// Plays state and blends animation with duration
		AnimationState* BlendTo(AnimationState* state, float duration = 1.0f);

		// Stops animation with name
		void Stop(const String& animationName);

		// Stops all states
		void StopAll();

		// Returns name of component
		String GetName() const override;

		// Returns category of component
		String GetCategory() const override;

		// Returns name of component icon
		String GetIcon() const override;

		SERIALIZABLE(AnimationComponent);

	protected:
		// -------------------------------
		// Value assigning agent interface
		// -------------------------------
		struct IValueAgent
		{
			// Value path
			String path;

		public:
			virtual ~IValueAgent() {}

			// Updates value and blend
			virtual void Update() = 0;

			// Removes animated value from agent
			virtual void RemoveValue(IAnimatedValue* value) = 0;

			// Returns is agent hasn't no values
			virtual bool IsEmpty() const = 0;
		};
		typedef Vector<IValueAgent*> ValueAgentsVec;

		// ------------------------------
		// Template value assigning agent
		// ------------------------------
		template<typename _type>
		struct ValueAgent: public IValueAgent
		{
			using AnimatedValuesVec = Vector<Pair<AnimationState*, AnimatedValue<_type>*>>;

		public:
			AnimatedValuesVec   animValues; // Animated values associated with animation states
			IValueProxy<_type>* target;    // Target value proxy

		public:
			// Updates value and blend
			void Update();

			// Removes animated value from agent
			void RemoveValue(IAnimatedValue* value);

			// Returns is agent hasn't no values
			bool IsEmpty() const;

			// Assigns value as field
			void AssignField(_type& value);
		};

		// ----------------------
		// Blend state parameters
		// ----------------------
		struct BlendState
		{
			AnimationStatesVec  mBlendOffStates;           // Turning off states
			AnimationState*     mBlendOnState = nullptr;   // Turning on state
			float               duration;                  // Blending duration
			float               time = -1.0f;              // Current blending remaining time

			// Updates work weight by time
			void Update(float dt);
		};

	protected:
		AnimationStatesVec mStates; // Animation states array @SERIALIZABLE @DEFAULT_TYPE(o2::AnimationState) @DONT_DELETE @INVOKE_ON_CHANGE(OnStatesListChanged)
		ValueAgentsVec     mValues; // Assigning value agents
		BlendState         mBlend;  // Current blend parameters

	protected:
		// Removes animated value from agent by path
		void UnregAnimatedValue(IAnimatedValue* value, const String& path);

		// Registers value by path and state
		template<typename _type>
		void RegAnimatedValue(AnimatedValue< _type >* value, const String& path, AnimationState* state);

		// It is called from editor, refreshes states
		void OnStatesListChanged();

		friend class Animation;
		friend class IAnimatedValue;

		template<typename _type>
		friend class AnimatedValue;
	};

	template<typename _type>
	void AnimationComponent::RegAnimatedValue(AnimatedValue<_type>* value, const String& path, AnimationState* state)
	{
		for (auto val : mValues)
		{
			if (val->path == path)
			{
				ValueAgent< _type>* agent = dynamic_cast<ValueAgent<_type>*>(val);

				if (!agent)
				{
					o2Debug.LogWarning("Can't work with animated value: " + path);
					return;
				}

				agent->animValues.Add({ state, value });
				return;
			}
		}

		ValueAgent<_type>* newAgent = mnew ValueAgent <_type>();
		mValues.Add(newAgent);
		newAgent->path = path;
		newAgent->animValues.Add({ state, value });

		FieldInfo* fieldInfo = nullptr;
		_type* fieldPtr = (_type*)GetType().GetFieldPtr(mOwner, path, fieldInfo);

		if (!fieldInfo)
		{
			o2Debug.LogWarning("Can't animate value " + path + ": can't find field");
			return;
		}

		newAgent->target = dynamic_cast<IValueProxy<_type>*>(fieldInfo->GetType()->GetValueProxy(fieldPtr));
	}

	template<typename _type>
	bool AnimationComponent::ValueAgent<_type>::IsEmpty() const
	{
		return animValues.IsEmpty();
	}

	template<typename _type>
	void AnimationComponent::ValueAgent<_type>::RemoveValue(IAnimatedValue* value)
	{
		animValues.RemoveAll([&](auto x) { return x.second == value; });
	}

	template<typename _type>
	void AnimationComponent::ValueAgent<_type>::Update()
	{
		AnimationState* firstValueState = animValues[0].first;
		AnimatedValue<_type>* firstValue = animValues[0].second;

		float weightsSum = firstValueState->weight*firstValueState->workWeight*firstValueState->mask.GetNodeWeight(path);
		_type valueSum = firstValue->GetValue();

		for (int i = 1; i < animValues.Count(); i++)
		{
			AnimationState* valueState = animValues[i].first;
			AnimatedValue<_type>* value = animValues[i].second;

			weightsSum += valueState->weight*valueState->workWeight*valueState->mask.GetNodeWeight(path);
			valueSum += value->GetValue();
		}

		_type resValue = valueSum / weightsSum;
		target->SetValue(resValue);
	}

	template<typename _type>
	void AnimationComponent::ValueAgent<_type>::AssignField(_type& value)
	{
		target->SetValue(value);
	}
}

CLASS_BASES_META(o2::AnimationComponent)
{
	BASE_CLASS(o2::Component);
}
END_META;
CLASS_FIELDS_META(o2::AnimationComponent)
{
	PROTECTED_FIELD(mStates).SERIALIZABLE_ATTRIBUTE().DEFAULT_TYPE_ATTRIBUTE(o2::AnimationState).DONT_DELETE_ATTRIBUTE().INVOKE_ON_CHANGE_ATTRIBUTE(OnStatesListChanged);
	PROTECTED_FIELD(mValues);
	PROTECTED_FIELD(mBlend);
}
END_META;
CLASS_METHODS_META(o2::AnimationComponent)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(AnimationState*, AddState, AnimationState*);
	PUBLIC_FUNCTION(AnimationState*, AddState, const String&, const Animation&, const AnimationMask&, float);
	PUBLIC_FUNCTION(AnimationState*, AddState, const String&);
	PUBLIC_FUNCTION(void, RemoveState, AnimationState*);
	PUBLIC_FUNCTION(void, RemoveState, const String&);
	PUBLIC_FUNCTION(void, RemoveAllStates);
	PUBLIC_FUNCTION(AnimationState*, GetState, const String&);
	PUBLIC_FUNCTION(const AnimationStatesVec&, GetStates);
	PUBLIC_FUNCTION(AnimationState*, Play, const Animation&, const String&);
	PUBLIC_FUNCTION(AnimationState*, Play, const Animation&);
	PUBLIC_FUNCTION(AnimationState*, Play, const String&);
	PUBLIC_FUNCTION(AnimationState*, BlendTo, const Animation&, const String&, float);
	PUBLIC_FUNCTION(AnimationState*, BlendTo, const Animation&, float);
	PUBLIC_FUNCTION(AnimationState*, BlendTo, const String&, float);
	PUBLIC_FUNCTION(AnimationState*, BlendTo, AnimationState*, float);
	PUBLIC_FUNCTION(void, Stop, const String&);
	PUBLIC_FUNCTION(void, StopAll);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(String, GetCategory);
	PUBLIC_FUNCTION(String, GetIcon);
	PROTECTED_FUNCTION(void, UnregAnimatedValue, IAnimatedValue*, const String&);
	PROTECTED_FUNCTION(void, OnStatesListChanged);
}
END_META;
