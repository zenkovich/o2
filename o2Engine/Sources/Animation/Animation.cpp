#include "stdafx.h"
#include "Animation.h"

#include "Animation/AnimatedValue.h"
#include "Scene/Components/AnimationComponent.h"
#include "Utils/Basic/IObject.h"
#include "Utils/Debug/Debug.h"

namespace o2
{
	Animation::Animation(IObject* target /*= nullptr*/) :
		mTarget(nullptr), mAnimationState(nullptr)
	{
		SetTarget(target);
	}

	Animation::Animation(const Animation& other) :
		mTarget(nullptr), IAnimation(other), mAnimationState(nullptr)
	{
		for (auto& val : other.mAnimatedValues)
		{
			AnimatedValueDef def(val);
			def.animatedValue = val.animatedValue->CloneAs<IAnimatedValue>();
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
			def.animatedValue = val.animatedValue->CloneAs<IAnimatedValue>();
			def.animatedValue->onKeysChanged = THIS_FUNC(OnAnimatedValueChanged);

			if (mTarget)
			{
				IObject* targetObj = dynamic_cast<IObject*>(mTarget);
				if (targetObj)
				{
					FieldInfo* fieldInfo = nullptr;
					const ObjectType* type = dynamic_cast<const ObjectType*>(&targetObj->GetType());
					void* castedTarget = type->DynamicCastFromIObject(targetObj);
					def.targetPtr = type->GetFieldPtr(castedTarget, def.targetPath, fieldInfo);

					if (!fieldInfo)
						o2Debug.LogWarning("Can't find object " + def.targetPath + "for animating");
					else
					{
						if (fieldInfo->GetType()->GetUsage() == Type::Usage::Property)
							def.animatedValue->SetTargetProxyVoid(fieldInfo->GetType()->GetValueProxy(def.targetPtr));
						else
							def.animatedValue->SetTargetVoid(def.targetPtr);
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
				const ObjectType* type = dynamic_cast<const ObjectType*>(&mTarget->GetType());
				void* castedTarget = type->DynamicCastFromIObject(mTarget);
				val.targetPtr = type->GetFieldPtr(castedTarget, val.targetPath, fieldInfo);

				if (!fieldInfo)
				{
					if (errors)
						o2Debug.LogWarning("Can't find object " + val.targetPath + "for animating");
				}
				else
				{
					if (fieldInfo->GetType()->GetUsage() == Type::Usage::Property)
						val.animatedValue->SetTargetProxyVoid(fieldInfo->GetType()->GetValueProxy(val.targetPtr));
					else
						val.animatedValue->SetTargetVoid(val.targetPtr);
				}
			}
		}
		else
		{
			for (auto& val : mAnimatedValues)
			{
				val.targetPtr = nullptr;
				val.animatedValue->SetTargetVoid(val.targetPtr);
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
		{
			val.animatedValue->onKeysChanged -= THIS_FUNC(OnAnimatedValueChanged);
			delete val.animatedValue;
		}

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

	bool Animation::ContainsAnimationValue(const String& path) const
	{
		for (auto& val : mAnimatedValues)
		{
			if (val.targetPath == path)
				return true;
		}

		return false;
	}

	IAnimatedValue* Animation::AddAnimationValueNoType(const String& path)
	{
		if (!mTarget)
			return nullptr;

		AnimatedValueDef def;

		FieldInfo* fieldInfo = nullptr;
		const ObjectType* type = dynamic_cast<const ObjectType*>(&mTarget->GetType());
		void* castedTarget = type->DynamicCastFromIObject(mTarget);
		def.targetPtr = mTarget->GetType().GetFieldPtr(castedTarget, path, fieldInfo);

		if (!fieldInfo)
		{
			o2Debug.LogWarning("Can't create animated value: field info not found " + path);
			return nullptr;
		}

		auto fieldType = fieldInfo->GetType();
		if (fieldType->GetUsage() == Type::Usage::Property)
			fieldType = dynamic_cast<const PropertyType*>(fieldType)->GetValueType();

		if (fieldType == &TypeOf(float))
			def.animatedValue = mnew AnimatedValue<float>();
		else if (fieldType == &TypeOf(Color4))
			def.animatedValue = mnew AnimatedValue<Color4>();
		else if (fieldType == &TypeOf(Vec2F))
			def.animatedValue = mnew AnimatedValue<Vec2F>();
		else if (fieldType == &TypeOf(bool))
			def.animatedValue = mnew AnimatedValue<bool>();

		def.animatedValue->onKeysChanged += THIS_FUNC(OnAnimatedValueChanged);

		if (fieldInfo->GetType()->GetUsage() == Type::Usage::Property)
			def.animatedValue->SetTargetProxyVoid(fieldInfo->GetType()->GetValueProxy(def.targetPtr));
		else
			def.animatedValue->SetTargetVoid(def.targetPtr);

		def.targetPath = path;
		mAnimatedValues.Add(def);

		OnAnimatedValueAdded(def);

		return def.animatedValue;
	}

	bool Animation::RemoveAnimationValue(const String& path)
	{
		for (auto& val : mAnimatedValues)
		{
			if (val.targetPath == path)
			{
				delete val.animatedValue;
				mAnimatedValues.Remove(val);
				onChanged();

				return true;
			}
		}

		return false;
	}

	void Animation::Evaluate()
	{
		for (auto& val : mAnimatedValues)
			val.animatedValue->ForceSetTime(mInDurationTime, mDuration);
	}

	void Animation::OnAnimatedValueChanged()
	{
		RecalculateDuration();

		onChanged();
	}

	void Animation::RecalculateDuration()
	{
		float lastDuration = mDuration;
		mDuration = 0.0f;

		for (auto& val : mAnimatedValues)
			mDuration = Math::Max(mDuration, val.animatedValue->mDuration);

		if (Math::Equals(lastDuration, mEndTime))
			mEndTime = mDuration;

		if (!Math::Equals(lastDuration, mDuration))
			onDurationChange();
	}

	void Animation::OnDeserialized(const DataNode& node)
	{
		for (auto& val : mAnimatedValues)
			val.animatedValue->onKeysChanged += THIS_FUNC(OnAnimatedValueChanged);

		OnAnimatedValueChanged();
		mEndTime = mDuration;
	}

	void Animation::OnAnimatedValueAdded(AnimatedValueDef& valueDef)
	{
		if (mAnimationState)
			valueDef.animatedValue->RegInAnimatable(mAnimationState, valueDef.targetPath);

		onChanged();
	}

	bool Animation::AnimatedValueDef::operator==(const AnimatedValueDef& other) const
	{
		return animatedValue == other.animatedValue;
	}
}

DECLARE_CLASS(o2::Animation);

DECLARE_CLASS(o2::Animation::AnimatedValueDef);
