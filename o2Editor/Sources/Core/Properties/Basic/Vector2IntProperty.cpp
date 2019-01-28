#include "stdafx.h"
#include "Vector2intProperty.h"

#include "Core/Properties/Basic/IntegerProperty.h"

namespace Editor
{
	Vec2IProperty::Vec2IProperty()
	{}

	Vec2IProperty::Vec2IProperty(const Vec2IProperty& other):
		IPropertyField(other)
	{
		InitializeControls();
	}

	Editor::Vec2IProperty& Vec2IProperty::operator=(const Vec2IProperty& other)
	{
		IPropertyField::operator=(other);
		return *this;
	}

	void Vec2IProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void Vec2IProperty::InitializeControls()
	{
		mXProperty = GetChildByType<IntegerProperty>("layout/properties/x");
		mXProperty->SetValuePath("x");
		mXProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mYProperty = GetChildByType<IntegerProperty>("layout/properties/y");
		mYProperty->SetValuePath("y");
		mYProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};
	}

	void Vec2IProperty::SetValue(const Vec2I& value)
	{
		mXProperty->SetValue(value.x);
		mYProperty->SetValue(value.y);
	}

	void Vec2IProperty::SetValueX(int value)
	{
		mXProperty->SetValue(value);
	}

	void Vec2IProperty::SetValueY(int value)
	{
		mYProperty->SetValue(value);
	}

	void Vec2IProperty::SetUnknownValue(const Vec2I& defaultValue /*= Vec2I()*/)
	{
		mXProperty->SetUnknownValue(defaultValue.x);
		mYProperty->SetUnknownValue(defaultValue.y);
	}

	void Vec2IProperty::SetXUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mXProperty->SetUnknownValue(defaultValue);
	}

	void Vec2IProperty::SetYUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mYProperty->SetUnknownValue(defaultValue);
	}

	void Vec2IProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		mXProperty->SetValueAndPrototypeProxy(targets.Select<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew XValueProxy(x.first), x.second ? mnew XValueProxy(x.second) : nullptr); }));

		mYProperty->SetValueAndPrototypeProxy(targets.Select<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew YValueProxy(x.first), x.second ? mnew YValueProxy(x.second) : nullptr); }));
	}

	void Vec2IProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		mXProperty->Refresh();
		mXProperty->Refresh();

		CheckRevertableState();
	}

	void Vec2IProperty::Revert()
	{
		if (mValuesProxies.IsEmpty())
			return;

		mXProperty->Refresh();
		mXProperty->Refresh();

		CheckRevertableState();
	}

	Vec2I Vec2IProperty::GetCommonValue() const
	{
		return Vec2I(mXProperty->GetCommonValue(), mYProperty->GetCommonValue());
	}

	bool Vec2IProperty::IsValuesDifferent() const
	{
		return mXProperty->IsValuesDifferent() || mYProperty->IsValuesDifferent();
	}

	const Type* Vec2IProperty::GetFieldType() const
	{
		return &TypeOf(Vec2I);
	}

	Vec2IProperty::XValueProxy::XValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	Vec2IProperty::XValueProxy::XValueProxy()
	{}

	void Vec2IProperty::XValueProxy::SetValue(const int& value)
	{
		Vec2I proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.x = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int Vec2IProperty::XValueProxy::GetValue() const
	{
		Vec2I proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.x;
	}

	Vec2IProperty::YValueProxy::YValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	Vec2IProperty::YValueProxy::YValueProxy()
	{}

	void Vec2IProperty::YValueProxy::SetValue(const int& value)
	{
		Vec2I proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.y = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int Vec2IProperty::YValueProxy::GetValue() const
	{
		Vec2I proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.y;
	}
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::Vec2I>);

DECLARE_CLASS(Editor::Vec2IProperty);
