#include "o2Editor/stdafx.h"
#include "BorderIntProperty.h"

#include "o2Editor/Core/Properties/Basic/IntegerProperty.h"
#include "o2Editor/Core/Pointers/Ref.h"
#include "o2Editor/Core/Pointers/WeakRef.h"

namespace Editor
{
    using IntegerPropertyRef = Ref<IntegerProperty>;

    BorderIProperty::BorderIProperty()
    {}

    BorderIProperty::BorderIProperty(const BorderIProperty& other) :
        IPropertyField(other)
    {
        InitializeControls();
    }

    BorderIProperty& BorderIProperty::operator=(const BorderIProperty& other)
    {
        IPropertyField::operator=(other);
        InitializeControls();
        return *this;
    }

    void BorderIProperty::InitializeControls()
    {
        mLeftProperty = GetChildByType<IntegerPropertyRef>("container/layout/properties/left");
        mLeftProperty->SetValuePath("left");
        mLeftProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
        mLeftProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
        {
            onChangeCompleted(mValuesPath + "/" + path, before, after);
        };

        mBottomProperty = GetChildByType<IntegerPropertyRef>("container/layout/properties/bottom");
        mBottomProperty->SetValuePath("bottom");
        mBottomProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
        mBottomProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
        {
            onChangeCompleted(mValuesPath + "/" + path, before, after);
        };

        mRightProperty = GetChildByType<IntegerPropertyRef>("container/layout/properties/right");
        mRightProperty->SetValuePath("right");
        mRightProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
        mRightProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
        {
            onChangeCompleted(mValuesPath + "/" + path, before, after);
        };

        mTopProperty = GetChildByType<IntegerPropertyRef>("container/layout/properties/top");
        mTopProperty->SetValuePath("top");
        mTopProperty->onChanged = [&](IPropertyField* field) { onChanged(field); };
        mTopProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
        {
            onChangeCompleted(mValuesPath + "/" + path, before, after);
        };
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

    void BorderIProperty::SetUnknownValue(const BorderI& defaultValue)
    {
        mLeftProperty->SetUnknownValue(defaultValue.left);
        mRightProperty->SetUnknownValue(defaultValue.right);
        mTopProperty->SetUnknownValue(defaultValue.top);
        mBottomProperty->SetUnknownValue(defaultValue.bottom);
    }

    void BorderIProperty::SetLeftUnknownValue(int defaultValue)
    {
        mLeftProperty->SetUnknownValue(defaultValue);
    }

    void BorderIProperty::SetRightUnknownValue(int defaultValue)
    {
        mRightProperty->SetUnknownValue(defaultValue);
    }

    void BorderIProperty::SetTopUnknownValue(int defaultValue)
    {
        mTopProperty->SetUnknownValue(defaultValue);
    }

    void BorderIProperty::SetBottomUnknownValue(int defaultValue)
    {
        mBottomProperty->SetUnknownValue(defaultValue);
    }

    void BorderIProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
    {
        mValuesProxies = targets;

        mLeftProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
            return TargetPair(mmake<LeftValueProxy>(x.first), x.second ? mmake<LeftValueProxy>(x.second) : nullptr); }));

        mRightProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
            return TargetPair(mmake<RightValueProxy>(x.first), x.second ? mmake<RightValueProxy>(x.second) : nullptr); }));

        mTopProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
            return TargetPair(mmake<TopValueProxy>(x.first), x.second ? mmake<TopValueProxy>(x.second) : nullptr); }));

        mBottomProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
            return TargetPair(mmake<BottomValueProxy>(x.first), x.second ? mmake<BottomValueProxy>(x.second) : nullptr); }));
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
        return mLeftP#include <memory>
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

class Type
{

};

// Class BorderI and methods skipped for brevity

const Type* BorderIProperty::GetValueType() const
{
    return GetValueTypeStatic();
}

const Type* BorderIProperty::GetValueTypeStatic()
{
    return &TypeOf(BorderI);
}

class BorderIProperty::LeftValueProxy
{
public:
    LeftValueProxy(IAbstractValueProxy* proxy) : mProxy(proxy) {}
    LeftValueProxy() {}

    void SetValue(const int& value)
    {
        BorderI proxyValue;
        mProxy->GetValuePtr(&proxyValue);
        proxyValue.left = value;
        mProxy->SetValuePtr(&proxyValue);
    }

    int GetValue() const
    {
        BorderI proxyValue;
        mProxy->GetValuePtr(&proxyValue);
        return proxyValue.left;
    }

private:
    IAbstractValueProxy* mProxy;
};

class BorderIProperty::RightValueProxy
{
public:
    RightValueProxy(IAbstractValueProxy* proxy) : mProxy(proxy) {}
    RightValueProxy() {}

    void SetValue(const int& value)
    {
        BorderI proxyValue;
        mProxy->GetValuePtr(&proxyValue);
        proxyValue.right = value;
        mProxy->SetValuePtr(&proxyValue);
    }

    int GetValue() const
    {
        BorderI proxyValue;
        mProxy->GetValuePtr(&proxyValue);
        return proxyValue.right;
    }

private:
    IAbstractValueProxy* mProxy;
};

class BorderIProperty::TopValueProxy
{
public:
    TopValueProxy(IAbstractValueProxy* proxy) : mProxy(proxy) {}
    TopValueProxy() {}

    void SetValue(const int& value)
    {
        BorderI proxyValue;
        mProxy->GetValuePtr(&proxyValue);
        proxyValue.top = value;
        mProxy->SetValuePtr(&proxyValue);
    }

    int GetValue() const
    {
        BorderI proxyValue;
        mProxy->GetValuePtr(&proxyValue);
        return proxyValue.top;
    }

private:
    IAbstractValueProxy* mProxy;
};

class BorderIProperty::BottomValueProxy
{
public:
    BottomValueProxy(IAbstractValueProxy* proxy) : mProxy(proxy) {}
    BottomValueProxy() {}

    void SetValue(const int& value)
    {
        BorderI proxyValue;
        mProxy->GetValuePtr(&proxyValue);
        proxyValue.bottom = value;
        mProxy->SetValuePtr(&proxyValue);
    }

    int GetValue() const
    {
        BorderI proxyValue;
        mProxy->GetValuePtr(&proxyValue);
        return proxyValue.bottom;
    }

private:
    IAbstractValueProxy* mProxy;
};

DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<o2::BorderI>);

// --- META ---

DECLARE_CLASS(Editor::BorderIProperty, Editor__BorderIProperty);

// --- END META ---