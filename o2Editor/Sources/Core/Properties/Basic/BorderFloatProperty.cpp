#include "stdafx.h"
#include "BorderFloatProperty.h"

#include "Core/Properties/Basic/FloatProperty.h"

namespace Editor
{
	BorderFProperty::BorderFProperty()
	{}

	BorderFProperty::BorderFProperty(const BorderFProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	BorderFProperty& BorderFProperty::operator=(const BorderFProperty& other)
	{
		IPropertyField::operator=(other);
		return *this;
	}

	void BorderFProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void BorderFProperty::InitializeControls()
	{
		mLeftProperty = GetChildByType<FloatProperty>("layout/properties/left");
		mLeftProperty->SetValuePath("left");
		mLeftProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
		mLeftProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mBottomProperty = GetChildByType<FloatProperty>("layout/properties/bottom");
		mBottomProperty->SetValuePath("bottom");
		mBottomProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
		mBottomProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mRightProperty = GetChildByType<FloatProperty>("layout/properties/right");
		mRightProperty->SetValuePath("right");
		mRightProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
		mRightProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mTopProperty = GetChildByType<FloatProperty>("layout/properties/top");
		mTopProperty->SetValuePath("top");
		mTopProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
		mTopProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};
	}

	void BorderFProperty::SetValue(const BorderF& value)
	{
		mLeftProperty->SetValue(value.left);
		mBottomProperty->SetValue(value.bottom);
		mRightProperty->SetValue(value.right);
		mTopProperty->SetValue(value.top);
	}

	void BorderFProperty::SetValueLeft(float value)
	{
		mLeftProperty->SetValue(value);
	}

	void BorderFProperty::SetValueRight(float value)
	{
		mRightProperty->SetValue(value);
	}

	void BorderFProperty::SetValueTop(float value)
	{
		mTopProperty->SetValue(value);
	}

	void BorderFProperty::SetValueBottom(float value)
	{
		mBottomProperty->SetValue(value);
	}

	void BorderFProperty::SetUnknownValue(const BorderF& defaultValue /*= BorderF()*/)
	{
		mLeftProperty->SetUnknownValue(defaultValue.left);
		mRightProperty->SetUnknownValue(defaultValue.right);
		mTopProperty->SetUnknownValue(defaultValue.top);
		mBottomProperty->SetUnknownValue(defaultValue.bottom);
	}

	void BorderFProperty::SetLeftUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mLeftProperty->SetUnknownValue(defaultValue);
	}

	void BorderFProperty::SetRightUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mRightProperty->SetUnknownValue(defaultValue);
	}

	void BorderFProperty::SetTopUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mTopProperty->SetUnknownValue(defaultValue);
	}

	void BorderFProperty::SetBottomUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mBottomProperty->SetUnknownValue(defaultValue);
	}

	void BorderFProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
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

	void BorderFProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		mLeftProperty->Refresh();
		mRightProperty->Refresh();
		mTopProperty->Refresh();
		mBottomProperty->Refresh();

		CheckRevertableState();
	}

	BorderF BorderFProperty::GetCommonValue() const
	{
		return BorderF(mLeftProperty->GetCommonValue(), mBottomProperty->GetCommonValue(),
					   mRightProperty->GetCommonValue(), mTopProperty->GetCommonValue());
	}

	bool BorderFProperty::IsValuesDifferent() const
	{
		return mLeftProperty->IsValuesDifferent() || mRightProperty->IsValuesDifferent() ||
			mTopProperty->IsValuesDifferent() || mBottomProperty->IsValuesDifferent();
	}

	const Type* BorderFProperty::GetFieldType() const
	{
		return &TypeOf(BorderF);
	}

	BorderFProperty::LeftValueProxy::LeftValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	BorderFProperty::LeftValueProxy::LeftValueProxy()
	{}

	void BorderFProperty::LeftValueProxy::SetValue(const float& value)
	{
		BorderF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.left = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float BorderFProperty::LeftValueProxy::GetValue() const
	{
		BorderF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.left;
	}

	BorderFProperty::RightValueProxy::RightValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	BorderFProperty::RightValueProxy::RightValueProxy()
	{}

	void BorderFProperty::RightValueProxy::SetValue(const float& value)
	{
		BorderF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.right = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float BorderFProperty::RightValueProxy::GetValue() const
	{
		BorderF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.right;
	}

	BorderFProperty::TopValueProxy::TopValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	BorderFProperty::TopValueProxy::TopValueProxy()
	{}

	void BorderFProperty::TopValueProxy::SetValue(const float& value)
	{
		BorderF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.top = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float BorderFProperty::TopValueProxy::GetValue() const
	{
		BorderF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.top;
	}

	BorderFProperty::BottomValueProxy::BottomValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	BorderFProperty::BottomValueProxy::BottomValueProxy()
	{}

	void BorderFProperty::BottomValueProxy::SetValue(const float& value)
	{
		BorderF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.bottom = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float BorderFProperty::BottomValueProxy::GetValue() const
	{
		BorderF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.bottom;
	}

}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::BorderF>);

DECLARE_CLASS(Editor::BorderFProperty);
