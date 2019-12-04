#include "stdafx.h"
#include "RectangleFloatProperty.h"

#include "Core/Properties/Basic/FloatProperty.h"

namespace Editor
{
	RectFProperty::RectFProperty()
	{}

	RectFProperty::RectFProperty(const RectFProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	RectFProperty& RectFProperty::operator=(const RectFProperty& other)
	{
		IPropertyField::operator=(other);
		return *this;
	}

	void RectFProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void RectFProperty::InitializeControls()
	{
		mLeftProperty = GetChildByType<FloatProperty>("container/layout/properties/left");
		mLeftProperty->SetValuePath("left");
		mLeftProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
		mLeftProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mBottomProperty = GetChildByType<FloatProperty>("container/layout/properties/bottom");
		mBottomProperty->SetValuePath("bottom");
		mBottomProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
		mBottomProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mRightProperty = GetChildByType<FloatProperty>("container/layout/properties/right");
		mRightProperty->SetValuePath("right");
		mRightProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
		mRightProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mTopProperty = GetChildByType<FloatProperty>("container/layout/properties/top");
		mTopProperty->SetValuePath("top");
		mTopProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
		mTopProperty->onChangeCompleted = [&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};
	}

	void RectFProperty::SetValue(const RectF& value)
	{
		mLeftProperty->SetValue(value.left);
		mBottomProperty->SetValue(value.bottom);
		mRightProperty->SetValue(value.right);
		mTopProperty->SetValue(value.top);
	}

	void RectFProperty::SetValueLeft(float value)
	{
		mLeftProperty->SetValue(value);
	}

	void RectFProperty::SetValueRight(float value)
	{
		mRightProperty->SetValue(value);
	}

	void RectFProperty::SetValueTop(float value)
	{
		mTopProperty->SetValue(value);
	}

	void RectFProperty::SetValueBottom(float value)
	{
		mBottomProperty->SetValue(value);
	}

	void RectFProperty::SetUnknownValue(const RectF& defaultValue /*= RectF()*/)
	{
		mLeftProperty->SetUnknownValue(defaultValue.left);
		mRightProperty->SetUnknownValue(defaultValue.right);
		mTopProperty->SetUnknownValue(defaultValue.top);
		mBottomProperty->SetUnknownValue(defaultValue.bottom);
	}

	void RectFProperty::SetLeftUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mLeftProperty->SetUnknownValue(defaultValue);
	}

	void RectFProperty::SetRightUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mRightProperty->SetUnknownValue(defaultValue);
	}

	void RectFProperty::SetTopUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mTopProperty->SetUnknownValue(defaultValue);
	}

	void RectFProperty::SetBottomUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mBottomProperty->SetUnknownValue(defaultValue);
	}

	void RectFProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
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

	void RectFProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		mLeftProperty->Refresh();
		mRightProperty->Refresh();
		mTopProperty->Refresh();
		mBottomProperty->Refresh();

		CheckRevertableState();
	}

	RectF RectFProperty::GetCommonValue() const
	{
		return RectF(mLeftProperty->GetCommonValue(), mBottomProperty->GetCommonValue(),
					 mRightProperty->GetCommonValue(), mTopProperty->GetCommonValue());
	}

	bool RectFProperty::IsValuesDifferent() const
	{
		return mLeftProperty->IsValuesDifferent() || mRightProperty->IsValuesDifferent() ||
			mTopProperty->IsValuesDifferent() || mBottomProperty->IsValuesDifferent();
	}

	const Type* RectFProperty::GetFieldType() const
	{
		return &TypeOf(RectF);
	}

	RectFProperty::LeftValueProxy::LeftValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	RectFProperty::LeftValueProxy::LeftValueProxy()
	{}

	void RectFProperty::LeftValueProxy::SetValue(const float& value)
	{
		RectF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.left = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float RectFProperty::LeftValueProxy::GetValue() const
	{
		RectF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.left;
	}

	RectFProperty::RightValueProxy::RightValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	RectFProperty::RightValueProxy::RightValueProxy()
	{}

	void RectFProperty::RightValueProxy::SetValue(const float& value)
	{
		RectF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.right = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float RectFProperty::RightValueProxy::GetValue() const
	{
		RectF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.right;
	}

	RectFProperty::TopValueProxy::TopValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	RectFProperty::TopValueProxy::TopValueProxy()
	{}

	void RectFProperty::TopValueProxy::SetValue(const float& value)
	{
		RectF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.top = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float RectFProperty::TopValueProxy::GetValue() const
	{
		RectF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.top;
	}

	RectFProperty::BottomValueProxy::BottomValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
	{}

	RectFProperty::BottomValueProxy::BottomValueProxy()
	{}

	void RectFProperty::BottomValueProxy::SetValue(const float& value)
	{
		RectF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		proxyValue.bottom = value;
		mProxy->SetValuePtr(&proxyValue);
	}

	float RectFProperty::BottomValueProxy::GetValue() const
	{
		RectF proxyValue;
		mProxy->GetValuePtr(&proxyValue);
		return proxyValue.bottom;
	}

}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::RectF>);

DECLARE_CLASS(Editor::RectFProperty);
