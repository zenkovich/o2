#include "stdafx.h"
#include "Vector2FloatProperty.h"

#include "Core/Properties/Basic/FloatProperty.h"

namespace Editor
{
	Vec2FProperty::Vec2FProperty()
	{}

	Vec2FProperty::Vec2FProperty(const Vec2FProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	Vec2FProperty& Vec2FProperty::operator=(const Vec2FProperty& other)
	{
		IPropertyField::operator=(other);
		return *this;
	}

	void Vec2FProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void Vec2FProperty::InitializeControls()
	{
		mXProperty = GetChildByType<FloatProperty>("layout/properties/x");
		mYProperty = GetChildByType<FloatProperty>("layout/properties/y");
	}

	void Vec2FProperty::SetValue(const Vec2F& value)
	{
		mXProperty->SetValue(value.x);
		mYProperty->SetValue(value.y);
	}

	void Vec2FProperty::SetValueX(float value)
	{
		mXProperty->SetValue(value);
	}

	void Vec2FProperty::SetValueY(float value)
	{
		mYProperty->SetValue(value);
	}

	void Vec2FProperty::SetUnknownValue(const Vec2F& defaultValue /*= Vec2F()*/)
	{
		mXProperty->SetUnknownValue(defaultValue.x);
		mYProperty->SetUnknownValue(defaultValue.y);
	}

	void Vec2FProperty::SetXUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mXProperty->SetUnknownValue(defaultValue);
	}

	void Vec2FProperty::SetYUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mYProperty->SetUnknownValue(defaultValue);
	}

	void Vec2FProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		mXProperty->SetValueAndPrototypeProxy(targets.Select<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew XValueProxy(x.first), x.second ? mnew XValueProxy(x.second) : nullptr); }));

		mYProperty->SetValueAndPrototypeProxy(targets.Select<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew YValueProxy(x.first), x.second ? mnew YValueProxy(x.second) : nullptr); }));
	}

	void Vec2FProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		mXProperty->Refresh();
		mXProperty->Refresh();

		CheckRevertableState();
	}

	void Vec2FProperty::Revert()
	{
		if (mValuesProxies.IsEmpty())
			return;

		mXProperty->Refresh();
		mXProperty->Refresh();

		CheckRevertableState();
	}

	Vec2F Vec2FProperty::GetCommonValue() const
	{
		return Vec2F(mXProperty->GetCommonValue(), mYProperty->GetCommonValue());
	}

	bool Vec2FProperty::IsValuesDifferent() const
	{
		return mXProperty->IsValuesDifferent() || mYProperty->IsValuesDifferent();
	}

	const Type* Vec2FProperty::GetFieldType() const
	{
		return &TypeOf(Vec2F);
	}

	Vec2FProperty::XValueProxy::XValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	Vec2FProperty::XValueProxy::XValueProxy()
	{}

	void Vec2FProperty::XValueProxy::SetValue(const float& value)
	{
		Vec2F proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.x = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float Vec2FProperty::XValueProxy::GetValue() const
	{
		Vec2F proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.x;
	}

	Vec2FProperty::YValueProxy::YValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	Vec2FProperty::YValueProxy::YValueProxy()
	{}

	void Vec2FProperty::YValueProxy::SetValue(const float& value)
	{
		Vec2F proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.y = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float Vec2FProperty::YValueProxy::GetValue() const
	{
		Vec2F proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.y;
	}
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::Vec2F>);

DECLARE_CLASS(Editor::Vec2FProperty);
