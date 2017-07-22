#include "Animation.h"

#include "Animation/Animatable.h"
#include "Animation/AnimatedValue.h"
#include "Utils/Debug.h"
#include "Utils/IObject.h"

namespace o2
{
	Animation::Animation(IObject* target /*= nullptr*/):
		mTarget(nullptr), mAnimationState(nullptr)
	{
		SetTarget(target);
	}

	Animation::Animation(const Animation& other):
		mTarget(nullptr), IAnimation(other), mAnimationState(nullptr)
	{
		for (auto& val : other.mAnimatedValues)
		{
			AnimatedValueDef def(val);
			def.mAnimatedValue = val.mAnimatedValue->Clone();
			mAnimatedValues.Add(def);

			OnAnimatedValueAdded(def);
		}

		SetTarget(other.mTarget);
	}

	Animation::~Animation()
	{
		Clear();
	}

	Animation& Animation::operator=(const Animation& other)
	{
		Clear();

		IAnimation::operator=(other);

		for (auto& val : other.mAnimatedValues)
		{
			AnimatedValueDef def(val);
			def.mAnimatedValue = val.mAnimatedValue->Clone();
			def.mAnimatedValue->onKeysChanged += THIS_FUNC(RecalculateDuration);

			if (mTarget)
			{
				IObject* targetObj = dynamic_cast<IObject*>(mTarget);
				if (targetObj)
				{
					FieldInfo* fieldInfo = nullptr;
					def.mTargetPtr = targetObj->GetType().GetFieldPtr(mTarget, def.mTargetPath, fieldInfo);

					if (!fieldInfo)
						o2Debug.LogWarning("Can't find object %s for animating", def.mTargetPath);
					else
					{
						if (fieldInfo->GetType()->GetUsage() == Type::Usage::Property)
							def.mAnimatedValue->SetTargetPropertyVoid(def.mTargetPtr);
						else
							def.mAnimatedValue->SetTargetVoid(def.mTargetPtr);
					}
				}
			}

			mAnimatedValues.Add(def);

			OnAnimatedValueAdded(def);
		}

		RecalculateDuration();

		return *this;
	}

	void Animation::SetTarget(IObject* target, bool errors /*= true*/)
	{
		mTarget = target;

		if (mTarget)
		{
			for (auto& val : mAnimatedValues)
			{
				FieldInfo* fieldInfo = nullptr;
				val.mTargetPtr = mTarget->GetType().GetFieldPtr(mTarget, val.mTargetPath, fieldInfo);

				if (!fieldInfo)
				{
					if (errors)
						o2Debug.LogWarning("Can't find object %s for animating", val.mTargetPath);
				}
				else
				{
					if (fieldInfo->GetType()->GetUsage() == Type::Usage::Property)
						val.mAnimatedValue->SetTargetPropertyVoid(val.mTargetPtr);
					else
						val.mAnimatedValue->SetTargetVoid(val.mTargetPtr);
				}
			}
		}
		else
		{
			for (auto& val : mAnimatedValues)
			{
				val.mTargetPtr = nullptr;
				val.mAnimatedValue->SetTargetVoid(val.mTargetPtr);
			}
		}
	}

	IObject* Animation::GetTarget() const
	{
		return mTarget;
	}

	void Animation::Clear()
	{
		for (auto& val : mAnimatedValues)
			delete val.mAnimatedValue;

		mAnimatedValues.Clear();
	}

	Animation::AnimatedValuesVec& Animation::GetAnimationsValues()
	{
		return mAnimatedValues;
	}

	const Animation::AnimatedValuesVec& Animation::GetAnimationsValues() const
	{
		return mAnimatedValues;
	}

	bool Animation::RemoveAnimationValue(const String& path)
	{
		for (auto& val : mAnimatedValues)
		{
			if (val.mTargetPath == path)
			{
				delete val.mAnimatedValue;
				mAnimatedValues.Remove(val);
				return true;
			}
		}

		return false;
	}

	void Animation::Evaluate()
	{
		for (auto& val : mAnimatedValues)
			val.mAnimatedValue->ForceSetTime(mInDurationTime, mDuration);
	}

	void Animation::RecalculateDuration()
	{
		float lastDuration = mDuration;
		mDuration = 0.0f;

		for (auto& val : mAnimatedValues)
			mDuration = Math::Max(mDuration, val.mAnimatedValue->mDuration);

		if (Math::Equals(lastDuration, mEndTime))
			mEndTime = mDuration;
	}

	void Animation::OnDeserialized(const DataNode& node)
	{
		for (auto& val : mAnimatedValues)
			val.mAnimatedValue->onKeysChanged += THIS_FUNC(RecalculateDuration);

		RecalculateDuration();
		mEndTime = mDuration;
	}

	void Animation::OnAnimatedValueAdded(AnimatedValueDef& valueDef)
	{
		if (mAnimationState)
			valueDef.mAnimatedValue->RegInAnimatable(mAnimationState, valueDef.mTargetPath);
	}

	bool Animation::AnimatedValueDef::operator==(const AnimatedValueDef& other) const
	{
		return mAnimatedValue == other.mAnimatedValue;
	}
}

CLASS_META(o2::Animation)
{
	BASE_CLASS(o2::IAnimation);

	PROTECTED_FIELD(mAnimatedValues).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTarget);
	PROTECTED_FIELD(mAnimationState);

	PUBLIC_FUNCTION(void, SetTarget, IObject*, bool);
	PUBLIC_FUNCTION(IObject*, GetTarget);
	PUBLIC_FUNCTION(void, Clear);
	PUBLIC_FUNCTION(AnimatedValuesVec&, GetAnimationsValues);
	PUBLIC_FUNCTION(const AnimatedValuesVec&, GetAnimationsValues);
	PUBLIC_FUNCTION(bool, RemoveAnimationValue, const String&);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(void, RecalculateDuration);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, OnAnimatedValueAdded, AnimatedValueDef&);
}
END_META;

CLASS_META(o2::Animation::AnimatedValueDef)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(mTargetPath).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mTargetPtr);
	PUBLIC_FIELD(mAnimatedValue).SERIALIZABLE_ATTRIBUTE();
}
END_META;
