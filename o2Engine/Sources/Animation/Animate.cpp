#include "Animate.h"

namespace o2
{

	Animate::Animate(IObject& object):
		mAnimation(&object)
	{}

	Animate::~Animate()
	{
		for (auto container : mKeyContainers)
			delete container;
	}

	Animate::operator Animation() const
	{
		return mAnimation;
	}

	Animate& Animate::Wait(float seconds)
	{
		mTime += seconds;

		for (auto container : mKeyContainers)
			container->Apply(mTime);

		return *this;
	}

	Animate& Animate::For(float seconds)
	{
		mTime += seconds;
		ApplyKeys();

		return *this;
	}

	void Animate::ApplyKeys()
	{
		mAnimation.AddTimeEvent(mTime, mFunction);

		for (auto container : mKeyContainers)
			container->Apply(mTime);

		mKeysApplied = true;
	}

	Animate& Animate::Then()
	{
		if (!mKeysApplied)
			ApplyKeys();

		return *this;
	}

	Animate& Animate::Move(float x, float y)
	{
		CheckPositionAnimatedvalue();
		CheckAppliedKeys();

		KeyContainer<Vec2F>* container = mnew KeyContainer<Vec2F>();
		container->animatedValue = mPositionAnimatedValue;
		container->key.value = Vec2F(x, y);
		mKeyContainers.Add(container);

		return *this;
	}

	Animate& Animate::Move(const Vec2F& point)
	{
		CheckPositionAnimatedvalue();
		CheckAppliedKeys();

		KeyContainer<Vec2F>* container = mnew KeyContainer<Vec2F>();
		container->animatedValue = mPositionAnimatedValue;
		container->key.value = point;
		mKeyContainers.Add(container);

		return *this;
	}

	Animate& Animate::Move(const Vector<Vec2F>& points)
	{
		return *this;
	}

	Animate& Animate::Alpha(float alpha)
	{
		CheckColorAnimatedValue();
		CheckAppliedKeys();

		KeyContainer<Color4>* container = mnew KeyContainer<Color4>();
		container->animatedValue = mColorAnimatedValue;
		container->key.value = Color4(1.0f, 1.0f, 1.0f, alpha);
		mKeyContainers.Add(container);

		return *this;
	}

	Animate& Animate::Show()
	{
		return Alpha(1.0f);
	}

	Animate& Animate::Hide()
	{
		return Alpha(0.0f);
	}

	Animate& Animate::Color(const Color4& color)
	{
		CheckColorAnimatedValue();
		CheckAppliedKeys();

		KeyContainer<Color4>* container = mnew KeyContainer<Color4>();
		container->animatedValue = mColorAnimatedValue;
		container->key.value = color;
		mKeyContainers.Add(container);

		return *this;
	}

	Animate& Animate::Color(int r, int g, int b, int a)
	{
		return Color(Color4(r, g, b, a));
	}

	Animate& Animate::Scale(float scale)
	{
		return Scale(Vec2F(scale, scale));
	}

	Animate& Animate::Scale(const Vec2F& scale)
	{
		CheckScaleAnimatedValue();
		CheckAppliedKeys();

		KeyContainer<Vec2F>* container = mnew KeyContainer<Vec2F>();
		container->animatedValue = mScaleAnimatedValue;
		container->key = AnimatedValue<Vec2F>::Key(0.0f, scale);
		mKeyContainers.Add(container);

		return *this;
	}

	Animate& Animate::Rotate(float angle)
	{
		CheckRotateAnimatedValue();
		CheckAppliedKeys();

		KeyContainer<float>* container = mnew KeyContainer<float>();
		container->animatedValue = mRotationAnimatedValue;
		container->key.value = angle;
		mKeyContainers.Add(container);

		return *this;
	}

	Animate& Animate::Invoke(const Function<void()>& function)
	{
		CheckAppliedKeys();

		mFunction += function;

		return *this;
	}

	void Animate::CheckColorAnimatedValue()
	{
		if (mColorAnimatedValue)
			return;

		static Vector<String> nameVariants ={ "color", "mColor", "m_color", "_color" };

		for (auto nameVariant : nameVariants)
		{
			FieldInfo* fi;
			if (mAnimation.GetTarget()->GetType().GetFieldPtr<Color4>((void*)mAnimation.GetTarget(), nameVariant, fi))
			{
				mColorAnimatedValue = mAnimation.AddAnimationValue<Color4>(nameVariant);
				return;
			}
		}
	}

	void Animate::CheckPositionAnimatedvalue()
	{
		if (mPositionAnimatedValue)
			return;

		static Vector<String> nameVariants ={ "position", "mPosition", "m_position", "_position", "pos" };

		for (auto nameVariant : nameVariants)
		{
			FieldInfo* fi;
			if (mAnimation.GetTarget()->GetType().GetFieldPtr<Vec2F>((void*)mAnimation.GetTarget(), nameVariant, fi))
			{
				mPositionAnimatedValue = mAnimation.AddAnimationValue<Vec2F>(nameVariant);
				return;
			}
		}
	}

	void Animate::CheckScaleAnimatedValue()
	{
		if (mScaleAnimatedValue)
			return;

		static Vector<String> nameVariants ={ "scale", "mScale", "m_scale", "_scale" };

		for (auto nameVariant : nameVariants)
		{
			FieldInfo* fi;
			if (mAnimation.GetTarget()->GetType().GetFieldPtr<Vec2F>((void*)mAnimation.GetTarget(), nameVariant, fi))
			{
				mScaleAnimatedValue = mAnimation.AddAnimationValue<Vec2F>(nameVariant);
				return;
			}
		}
	}

	void Animate::CheckRotateAnimatedValue()
	{
		if (mRotationAnimatedValue)
			return;

		static Vector<String> nameVariants ={ "angle", "mAngle", "mRotation", "m_angle", "m_rotation", "rotation", "rot" };

		for (auto nameVariant : nameVariants)
		{
			FieldInfo* fi;
			if (mAnimation.GetTarget()->GetType().GetFieldPtr<float>((void*)mAnimation.GetTarget(), nameVariant, fi))
			{
				mRotationAnimatedValue = mAnimation.AddAnimationValue<float>(nameVariant);
				return;
			}
		}
	}

	void Animate::CheckAppliedKeys()
	{
		if (!mKeysApplied)
			return;

		for (auto container : mKeyContainers)
			delete container;

		mKeyContainers.Clear();
		mFunction.Clear();

		mKeysApplied = false;
	}

}