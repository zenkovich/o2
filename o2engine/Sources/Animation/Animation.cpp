#include "Animation.h"

#include "Utils/IObject.h"

namespace o2
{
	IOBJECT_CPP(Animation);
	IOBJECT_CPP(Animation::AnimatedValueDef);

	Animation::Animation():
		mTarget(nullptr)
	{
	}

	Animation::Animation(const Animation& other):
		mTarget(other.mTarget), IAnimation(other)
	{
		for (auto val : other.mAnimatedValues)
		{
			AnimatedValueDef def(val);
			def.mAnimatedValue = val.mAnimatedValue->Clone();
			mAnimatedValues.Add(def);
		}
	}

	Animation::~Animation()
	{
		Clear();
	}

	Animation& Animation::operator=(const Animation& other)
	{
		Clear();

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
					FieldInfo* fieldInfo;
					def.mTargetPtr = targetObj->GetType().GetFieldPtr<char>(mTarget, def.mTargetPath, fieldInfo);

					if (fieldInfo->IsProperty())
						def.mAnimatedValue->SetTargetPropertyVoid(def.mTargetPtr);
					else
						def.mAnimatedValue->SetTargetVoid(def.mTargetPtr);
				}
			}

			mAnimatedValues.Add(def);
		}

		RecalculateDuration();

		return *this;
	}

	void Animation::SetTarget(IObject* target)
	{
		mTarget = target;

		if (mTarget)
		{
			for (auto val : mAnimatedValues)
			{
				FieldInfo* fieldInfo;
				val.mTargetPtr = mTarget->GetType().GetFieldPtr<char>(mTarget, val.mTargetPath, fieldInfo);

				if (fieldInfo->IsProperty())
					val.mAnimatedValue->SetTargetPropertyVoid(val.mTargetPtr);
				else
					val.mAnimatedValue->SetTargetVoid(val.mTargetPtr);
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
			val.mAnimatedValue->ForceSetTime(mTime, mDuration);
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

	bool Animation::AnimatedValueDef::operator==(const AnimatedValueDef& other) const
	{
		return mAnimatedValue == other.mAnimatedValue;
	}
}