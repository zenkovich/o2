#include "stdafx.h"
#include "RectangleIntProperty.h"

#include "Core/Properties/Basic/IntegerProperty.h"

namespace Editor
{
	RectIProperty::RectIProperty()
	{}

	RectIProperty::RectIProperty(const RectIProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	RectIProperty& RectIProperty::operator=(const RectIProperty& other)
	{
		IPropertyField::operator=(other);
		return *this;
	}

	void RectIProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void RectIProperty::InitializeControls()
	{
		mLeftProperty = GetChildByType<IntegerProperty>("layout/properties/left");
		mLeftProperty->SetValuePath("left");
		mLeftProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mBottomProperty = GetChildByType<IntegerProperty>("layout/properties/bottom");
		mBottomProperty->SetValuePath("bottom");
		mBottomProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mRightProperty = GetChildByType<IntegerProperty>("layout/properties/right");
		mRightProperty->SetValuePath("right");
		mRightProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mTopProperty = GetChildByType<IntegerProperty>("layout/properties/top");
		mTopProperty->SetValuePath("top");
		mTopProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};
	}

	void RectIProperty::SetValue(const RectI& value)
	{
		mLeftProperty->SetValue(value.left);
		mBottomProperty->SetValue(value.bottom);
		mRightProperty->SetValue(value.right);
		mTopProperty->SetValue(value.top);
	}

	void RectIProperty::SetValueLeft(int value)
	{
		mLeftProperty->SetValue(value);
	}

	void RectIProperty::SetValueRight(int value)
	{
		mRightProperty->SetValue(value);
	}

	void RectIProperty::SetValueTop(int value)
	{
		mTopProperty->SetValue(value);
	}

	void RectIProperty::SetValueBottom(int value)
	{
		mBottomProperty->SetValue(value);
	}

	void RectIProperty::SetUnknownValue(const RectI& defaultValue /*= RectI()*/)
	{
		mLeftProperty->SetUnknownValue(defaultValue.left);
		mRightProperty->SetUnknownValue(defaultValue.right);
		mTopProperty->SetUnknownValue(defaultValue.top);
		mBottomProperty->SetUnknownValue(defaultValue.bottom);
	}

	void RectIProperty::SetLeftUnknownValue(int defaultValue /*= 0*/)
	{
		mLeftProperty->SetUnknownValue(defaultValue);
	}

	void RectIProperty::SetRightUnknownValue(int defaultValue /*= 0*/)
	{
		mRightProperty->SetUnknownValue(defaultValue);
	}

	void RectIProperty::SetTopUnknownValue(int defaultValue /*= 0*/)
	{
		mTopProperty->SetUnknownValue(defaultValue);
	}

	void RectIProperty::SetBottomUnknownValue(int defaultValue /*= 0*/)
	{
		mBottomProperty->SetUnknownValue(defaultValue);
	}

	void RectIProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
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

	void RectIProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		mLeftProperty->Refresh();
		mRightProperty->Refresh();
		mTopProperty->Refresh();
		mBottomProperty->Refresh();

		CheckRevertableState();
	}

	RectI RectIProperty::GetCommonValue() const
	{
		return RectI(mLeftProperty->GetCommonValue(), mBottomProperty->GetCommonValue(),
					 mRightProperty->GetCommonValue(), mTopProperty->GetCommonValue());
	}

	bool RectIProperty::IsValuesDifferent() const
	{
		return mLeftProperty->IsValuesDifferent() || mRightProperty->IsValuesDifferent() ||
			mTopProperty->IsValuesDifferent() || mBottomProperty->IsValuesDifferent();
	}

	const Type* RectIProperty::GetFieldType() const
	{
		return &TypeOf(RectI);
	}

	RectIProperty::LeftValueProxy::LeftValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	RectIProperty::LeftValueProxy::LeftValueProxy()
	{}

	void RectIProperty::LeftValueProxy::SetValue(const int& value)
	{
		RectI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.left = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int RectIProperty::LeftValueProxy::GetValue() const
	{
		RectI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.left;
	}

	RectIProperty::RightValueProxy::RightValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	RectIProperty::RightValueProxy::RightValueProxy()
	{}

	void RectIProperty::RightValueProxy::SetValue(const int& value)
	{
		RectI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.right = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int RectIProperty::RightValueProxy::GetValue() const
	{
		RectI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.right;
	}

	RectIProperty::TopValueProxy::TopValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	RectIProperty::TopValueProxy::TopValueProxy()
	{}

	void RectIProperty::TopValueProxy::SetValue(const int& value)
	{
		RectI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.top = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int RectIProperty::TopValueProxy::GetValue() const
	{
		RectI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.top;
	}

	RectIProperty::BottomValueProxy::BottomValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	RectIProperty::BottomValueProxy::BottomValueProxy()
	{}

	void RectIProperty::BottomValueProxy::SetValue(const int& value)
	{
		RectI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.bottom = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	int RectIProperty::BottomValueProxy::GetValue() const
	{
		RectI proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.bottom;
	}

}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::RectI>);

DECLARE_CLASS(Editor::RectIProperty);
