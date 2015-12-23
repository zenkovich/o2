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
		mTarget(other.mTarget), IAnimation(other), mAnimationState(nullptr)
	{
		for (auto val : other.mAnimatedValues)
		{
			AnimatedValueDef def(val);
			def.mAnimatedValue = val.mAnimatedValue->Clone();
			mAnimatedValues.Add(def);

			OnAnimatedValueAdded(def);
		}
	}

	Animation::~Animation()
	{
		Clear();
	}

	Animation& Animation::operator=(const Animation& other)
	{
		Clear();

		IAnimation::operator=(other);

		for (auto val : other.mAnimatedValues)
		{
			AnimatedValueDef def(val);
			def.mAnimatedValue = val.mAnimatedValue->Clone();
			def.mAnimatedValue->onKeysChanged += Function<void()>(this, &Animation::RecalculateDuration);

			if (mTarget)
			{
				IObject* targetObj = dynamic_cast<IObject*>(mTarget);
				if (targetObj)
				{
					FieldInfo* fieldInfo = nullptr;
					def.mTargetPtr = targetObj->GetType().GetFieldPtr<char>(mTarget, def.mTargetPath, fieldInfo);

					if (!fieldInfo)
						o2Debug.LogWarning("Can't find object %s for animating", def.mTargetPath);
					else
					{
						if (fieldInfo->IsProperty())
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
			for (auto val : mAnimatedValues)
			{
				FieldInfo* fieldInfo = nullptr;
				val.mTargetPtr = mTarget->GetType().GetFieldPtr<char>(mTarget, val.mTargetPath, fieldInfo);

				if (!fieldInfo)
				{
					if (errors)
						o2Debug.LogWarning("Can't find object %s for animating", val.mTargetPath);
				}
				else
				{
					if (fieldInfo->IsProperty())
						val.mAnimatedValue->SetTargetPropertyVoid(val.mTargetPtr);
					else
						val.mAnimatedValue->SetTargetVoid(val.mTargetPtr);
				}
			}
		}
		else
		{
			for (auto val : mAnimatedValues)
			{
				val.mTargetPtr = nullptr;
				val.mAnimatedValue->SetTargetVoid(val.mTargetPtr);
			}
		}
	}

	void Animation::Clear()
	{
		for (auto val : mAnimatedValues)
			val.mAnimatedValue.Release();

		mAnimatedValues.Clear();
	}

	bool Animation::RemoveAnimationValue(const String& path)
	{
		for (auto& val : mAnimatedValues)
		{
			if (val.mTargetPath == path)
			{
				val.mAnimatedValue.Release();
				mAnimatedValues.Remove(val);
				return true;
			}
		}

		return false;
	}

	void Animation::Evaluate()
	{
		for (auto val : mAnimatedValues)
			val.mAnimatedValue->ForceSetTime(mInDurationTime, mDuration);
	}

	void Animation::RecalculateDuration()
	{
		float lastDuration = mDuration;
		mDuration = 0.0f;

		for (auto val : mAnimatedValues)
			mDuration = Math::Max(mDuration, val.mAnimatedValue->mDuration);

		if (Math::Equals(lastDuration, mEndTime))
			mEndTime = mDuration;
	}

	void Animation::OnDeserialized(const DataNode& node)
	{
		for (auto val : mAnimatedValues)
			val.mAnimatedValue->onKeysChanged += Function<void()>(this, &Animation::RecalculateDuration);

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