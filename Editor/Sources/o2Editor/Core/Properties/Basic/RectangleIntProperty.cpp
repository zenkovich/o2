#include "o2Editor/stdafx.h"
#include "RectangleIntProperty.h"

#include "o2Editor/Core/Properties/Basic/IntegerProperty.h"

namespace Editor
{
    RectIProperty::RectIProperty()
    {}

    RectIProperty::RectIProperty(const RectIProperty& other) :
        IPropertyField(other),
        mLeftProperty(other.mLeftProperty),
        mBottomProperty(other.mBottomProperty),
        mRightProperty(other.mRightProperty),
        mTopProperty(other.mTopProperty),
        mValuesProxies(other.mValuesProxies)
    {
        InitializeControls();
    }

    RectIProperty& RectIProperty::operator=(const RectIProperty& other)
    {
        IPropertyField::operator=(other);
        mLeftProperty = other.mLeftProperty;
        mBottomProperty = other.mBottomProperty;
        mRightProperty = other.mRightProperty;
        mTopProperty = other.mTopProperty;
        mValuesProxies = other.mValuesProxies;
        InitializeControls();
        return *this;
    }

    void RectIProperty::InitializeControls()
    {
        mLeftProperty = GetChildByType<IntegerProperty>("container/layout/properties/left");
        mLeftProperty->SetValuePath("left");
        mLeftProperty->onChanged = [&](const Ref<IPropertyField>& field) { onChanged(field); };
        mLeftProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
        {
            onChangeCompleted(mValuesPath + "/" + path, before, after);
        };

        mBottomProperty = GetChildByType<IntegerProperty>("container/layout/properties/bottom");
        mBottomProperty->SetValuePath("bottom");
        mBottomProperty->onChanged = [&](const Ref<IPropertyField>& field) { onChanged(field); };
        mBottomProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
        {
            onChangeCompleted(mValuesPath + "/" + path, before, after);
        };

        mRightProperty = GetChildByType<IntegerProperty>("container/layout/properties/right");
        mRightProperty->SetValuePath("right");
        mRightProperty->onChanged = [&](const Ref<IPropertyField>& field) { onChanged(field); };
        mRightProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
        {
            onChangeCompleted(mValuesPath + "/" + path, before, after);
        };

        mTopProperty = GetChildByType<IntegerProperty>("container/layout/properties/top");
        mTopProperty->SetValuePath("top");
        mTopProperty->onChanged = [&](const Ref<IPropertyField>& field) { onChanged(field); };
        mTopProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
        {
            onChangeCompleted(mValuesPath + "/" + path, before, after);
        };
    }

    void RectIProperty::SetValue(const Ref<const RectI>& value)
    {
        mLeftProperty->SetValue(value->left);
        mBottomProperty->SetValue(value->bottom);
        mRightProperty->SetValue(value->right);
        mTopProperty->SetValue(value->top);
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

    void RectIProperty::SetUnknownValue(const Ref<const RectI>& defaultValue /*= nullptr*/)
    {
        mLeftProperty->SetUnknownValue(defaultValue->left);
        mRightProperty->SetUnknownValue(defaultValue->right);
        mTopProperty->SetUnknownValue(defaultValue->top);
        mBottomProperty->SetUnknownValue(defaultValue->bottom);
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

        mLeftProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
            return TargetPair(mmake<LeftValueProxy>(x.first), x.second ? mmake<LeftValueProxy>(x.second) : nullptr); }));

        mRightProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
            return TargetPair(mmake<RightValueProxy>(x.first), x.second ? mmake<RightValueProxy>(x.second) : nullptr); }));

        mTopProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
            return TargetPair(mmake<TopValueProxy>(x.first), x.second ? mmake<TopValueProxy>(x.second) : nullptr); }));

        mBottomProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
            return TargetPair(mmake<BottomValueProxy>(x.first), x.second ? mmake<BottomValueProxy>(x.second) : nullptr); }));
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

    Ref<const RectI> RectIProperty::GetCommonValue() const
    {
        return mmake<const RectI>(mLeftProperty->GetCommonValue(), mBottomProperty->GetCommonValue(),
            mRightProperty->GetCommonValue(), mTopProperty->GetCommonValue());
    }

    bool RectIProperty::IsValuesDifferent() const
    {
        return mLeftProperty->IsValuesDifferent() || mRightProperty->IsValueReplace raw pointers with Ref<> and use const Ref<>& for function arguments with pointer types:

Replace:
```cpp
IAbstractValueProxy* mProxy;
```
With:
```cpp
Ref<IAbstractValueProxy> mProxy;
```

Replace:
```cpp
RectIProperty(const char* name, IAbstractValueProxy* proxy, const Ref<ISceneNode>& node);
```
With:
```cpp
RectIProperty(const char* name, const Ref<IAbstractValueProxy>& proxy, const Ref<ISceneNode>& node);
```

Replace:
```cpp
ValueProxyPtr& GetProxyPtr() { return mProxy; }
```
With:
```cpp
const Ref<IAbstractValueProxy>& GetProxyPtr() const { return mProxy; }
```

Replace:
```cpp
ValueProxyPtr mProxy;
```
With:
```cpp
Ref<IAbstractValueProxy> mProxy;
```

Replace:
```cpp
LeftValueProxy(IAbstractValueProxy* proxy);
```
With:
```cpp
LeftValueProxy(const Ref<IAbstractValueProxy>& proxy);
```

Replace:
```cpp
RightValueProxy(IAbstractValueProxy* proxy);
```
With:
```cpp
RightValueProxy(const Ref<IAbstractValueProxy>& proxy);
```

Replace:
```cpp
TopValueProxy(IAbstractValueProxy* proxy);
```
With:
```cpp
TopValueProxy(const Ref<IAbstractValueProxy>& proxy);
```

Replace:
```cpp
BottomValueProxy(IAbstractValueProxy* proxy);
```
With:
```cpp
BottomValueProxy(const Ref<IAbstractValueProxy>& proxy);
```

Replace:
```cpp
void RectIProperty::SetValuePtr(RectI* src)
```
With:
```cpp
void RectIProperty::SetValuePtr(const Ref<RectI>& src)
```

Replace:
```cpp
void RectIProperty::GetValuePtr(RectI* dest) const
```
With:
```cpp
void RectIProperty::GetValuePtr(Ref<RectI>& dest) const
```

Replace:
```cpp
const Type* RectIProperty::GetValueType() const
```
With:
```cpp
const Type* RectIProperty::GetValueType() const
```

Replace:
```cpp
const Type* RectIProperty::GetValueTypeStatic()
```
With:
```cpp
const Type* RectIProperty::GetValueTypeStatic()
```

Replace:
```cpp
RectIProperty::LeftValueProxy::LeftValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
{}
```
With:
```cpp
RectIProperty::LeftValueProxy::LeftValueProxy(const Ref<IAbstractValueProxy>& proxy) :mProxy(proxy)
{}
```

Replace:
```cpp
RectIProperty::LeftValueProxy::LeftValueProxy()
{}
```
With:
```cpp
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
```

Replace:
```cpp
RectIProperty::RightValueProxy::RightValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
{}
```
With:
```cpp
RectIProperty::RightValueProxy::RightValueProxy(const Ref<IAbstractValueProxy>& proxy) :mProxy(proxy)
{}
```

Replace:
```cpp
RectIProperty::RightValueProxy::RightValueProxy()
{}
```
With:
```cpp
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
```

Replace:
```cpp
RectIProperty::TopValueProxy::TopValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
{}
```
With:
```cpp
RectIProperty::TopValueProxy::TopValueProxy(const Ref<IAbstractValueProxy>& proxy) :mProxy(proxy)
{}
```

Replace:
```cpp
RectIProperty::TopValueProxy::TopValueProxy()
{}
```
With:
```cpp
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
```

Replace:
```cpp
RectIProperty::BottomValueProxy::BottomValueProxy(IAbstractValueProxy* proxy) :mProxy(proxy)
{}
```
With:
```cpp
RectIProperty::BottomValueProxy::BottomValueProxy(const Ref<IAbstractValueProxy>& proxy) :mProxy(proxy)
{}
```

Replace:
```cpp
RectIProperty::BottomValueProxy::BottomValueProxy()
{}
```
With:
```cpp
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
```

Remove the default nullptr value for class members with type Ref<>:

Replace:
```cpp
Ref<ISceneNode> mNode = nullptr;
```
With:
```cpp
Ref<ISceneNode> mNode;
```

Replace:
```cpp
const ValueProxyPtr mProxy = nullptr;
```
With:
```cpp
ValueProxyPtr mProxy;
```

Replace:
```cpp
Ref<IAbstractValueProxy> mProxy;
```
With:
```cpp
Ref<IAbstractValueProxy> mProxy;
```

Replace:
```cpp
Ref<ISceneNode> mNode = nullptr;
```
With:
```cpp
Ref<ISceneNode> mNode;
```

Replace:
```cpp
Ref<ISceneNode> mNode;
```
With:
```cpp
WeakRef<ISceneNode> mNode;
```

Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>:

Replace:
```cpp
dynamic_cast<RectIProperty*>(property)
```
With:
```cpp
DynamicCast<RectIProperty>(property)
```

Replace:
```cpp
dynamic_cast<const RectIProperty*>(property)
```
With:
```cpp
DynamicCast<const RectIProperty>(property)
```