#include "stdafx.h"
#include "BorderIntProperty.h"

#include "Core/Properties/Widgets/IntegerProperty.h"

namespace Editor
{
	BorderIProperty::BorderIProperty()
	{}

	BorderIProperty::BorderIProperty(const BorderIProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	BorderIProperty& BorderIProperty::operator=(const BorderIProperty& other)
	{
		CopyData(other);
		return *this;
	}

	void BorderIProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void BorderIProperty::InitializeControls()
	{
		mLeftProperty = GetChildByType<IntegerProperty>("layout/properties/left");
		mBottomProperty = GetChildByType<IntegerProperty>("layout/properties/bottom");
		mRightProperty = GetChildByType<IntegerProperty>("layout/properties/right");
		mTopProperty = GetChildByType<IntegerProperty>("layout/properties/top");
	}

	void BorderIProperty::SetValue(const BorderI& value)
	{
		mLeftProperty->SetValue(value.left);
		mBottomProperty->SetValue(value.bottom);
		mRightProperty->SetValue(value.right);
		mTopProperty->SetValue(value.top);
	}

	void BorderIProperty::SetValueLeft(int value)
	{
		mLeftProperty->SetValue(value);
	}

	void BorderIProperty::SetValueRight(int value)
	{
		mRightProperty->SetValue(value);
	}

	void BorderIProperty::SetValueTop(int value)
	{
		mTopProperty->SetValue(value);
	}

	void BorderIProperty::SetValueBottom(int value)
	{
		mBottomProperty->SetValue(value);
	}

	void BorderIProperty::SetUnknownValue(const BorderI& defaultValue /*= BorderI()*/)
	{
		mLeftProperty->SetUnknownValue(defaultValue.left);
		mRightProperty->SetUnknownValue(defaultValue.right);
		mTopProperty->SetUnknownValue(defaultValue.top);
		mBottomProperty->SetUnknownValue(defaultValue.bottom);
	}

	void BorderIProperty::SetLeftUnknownValue(int defaultValue /*= 0*/)
	{
		mLeftProperty->SetUnknownValue(defaultValue);
	}

	void BorderIProperty::SetRightUnknownValue(int defaultValue /*= 0*/)
	{
		mRightProperty->SetUnknownValue(defaultValue);
	}

	void BorderIProperty::SetTopUnknownValue(int defaultValue /*= 0*/)
	{
		mTopProperty->SetUnknownValue(defaultValue);
	}

	void BorderIProperty::SetBottomUnknownValue(int defaultValue /*= 0*/)
	{
		mBottomProperty->SetUnknownValue(defaultValue);
	}

	void BorderIProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		mLeftProperty->SetValueAndPrototypeProxy(targets.Select<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew LeftValueProxy(x.first), x.second ? mnew LeftValueProxy(x.second) : nullptr); }));

		mRightProperty->SetValueAndPrototypeProxy(targets.Select<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew RightValueProxy(x.first), x.second ? mnew RightValueProxy(x.second) : nullptr); }));

		mTopProperty->SetValueAndPrototypeProxy(targets.Select<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew TopValueProxy(x.first), x.second ? mnew TopValueProxy(x.second) : nullptr); }));

		mBottomProperty->SetValueAndPrototypeProxy(targets.Select<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew BottomValueProxy(x.first), x.second ? mnew BottomValueProxy(x.second) : nullptr); }));
	}

	void BorderIProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		mLeftProperty->Refresh();
		mRightProperty->Refresh();
		mTopProperty->Refresh();
		mBottomProperty->Refresh();

		CheckRevertableState();
	}

	BorderI BorderIProperty::GetCommonValue() const
	{
		return BorderI(mLeftProperty->GetCommonValue(), mBottomProperty->GetCommonValue(),
					   mRightProperty->GetCommonValue(), mTopProperty->GetCommonValue());
	}

	bool BorderIProperty::IsValuesDifferent() const
	{
		return mLeftProperty->IsValuesDifferent() || mRightProperty->IsValuesDifferent() ||
			mTopProperty->IsValuesDifferent() || mBottomProperty->IsValuesDifferent();
	}

	const Type* BorderIProperty::GetFieldType() const
	{
		return &TypeOf(BorderI);
	}

	BorderIProperty::LeftValueProxy::LeftValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	BorderIProperty::LeftValueProxy::LeftValueProxy()
	{}

	void BorderIProperty::LeftValueProxy::SetValue(const int& value)
	{
		BorderI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.left = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int BorderIProperty::LeftValueProxy::GetValue() const
	{
		BorderI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.left;
	}

	BorderIProperty::RightValueProxy::RightValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	BorderIProperty::RightValueProxy::RightValueProxy()
	{}

	void BorderIProperty::RightValueProxy::SetValue(const int& value)
	{
		BorderI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.right = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int BorderIProperty::RightValueProxy::GetValue() const
	{
		BorderI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.right;
	}

	BorderIProperty::TopValueProxy::TopValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	BorderIProperty::TopValueProxy::TopValueProxy()
	{}

	void BorderIProperty::TopValueProxy::SetValue(const int& value)
	{
		BorderI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.top = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int BorderIProperty::TopValueProxy::GetValue() const
	{
		BorderI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.top;
	}

	BorderIProperty::BottomValueProxy::BottomValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	BorderIProperty::BottomValueProxy::BottomValueProxy()
	{}

	void BorderIProperty::BottomValueProxy::SetValue(const int& value)
	{
		BorderI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.bottom = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int BorderIProperty::BottomValueProxy::GetValue() const
	{
		BorderI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.bottom;
	}

}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::BorderI>);

DECLARE_CLASS(Editor::BorderIProperty);
