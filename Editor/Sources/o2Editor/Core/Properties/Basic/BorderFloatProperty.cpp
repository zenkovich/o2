#include "o2Editor/stdafx.h"
#include "BorderFloatProperty.h"

#include "o2Editor/Core/Properties/Basic/FloatProperty.h"
#include "o2Editor/Core/Utils/SmartPointers/Ref.h"

namespace Editor
{
    using Ref = SmartPointers::Ref;
	using WeakRef = SmartPointers::WeakRef;

    BorderFProperty::BorderFProperty()
		: mLeftProperty(mmake<FloatProperty>())
		, mBottomProperty(mmake<FloatProperty>())
		, mRightProperty(mmake<FloatProperty>())
		, mTopProperty(mmake<FloatProperty>())
	{
		InitializeControls();
	}

	BorderFProperty::BorderFProperty(const BorderFProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	BorderFProperty& BorderFProperty::operator=(const BorderFProperty& other)
	{
		IPropertyField::operator=(other);
		InitializeControls();
		return *this;
	}

	void BorderFProperty::InitializeControls()
	{
		mLeftProperty->SetValuePath("left");
		mLeftProperty->onChanged = [&](const Ref<IPropertyField>& field) { onChanged(field); };
		mLeftProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mBottomProperty->SetValuePath("bottom");
		mBottomProperty->onChanged = [&](const Ref<IPropertyField>& field) { onChanged(field); };
		mBottomProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mRightProperty->SetValuePath("right");
		mRightProperty->onChanged = [&](const Ref<IPropertyField>& field) { onChanged(field); };
		mRightProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		mTopProperty->SetValuePath("top");
		mTopProperty->onChanged = [&](const Ref<IPropertyField>& field) { onChanged(field); };
		mTopProperty->onChangeCompleted = [&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
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

	void BorderFProperty::SetUnknownValue(const BorderF& defaultValue)
	{
		mLeftProperty->SetUnknownValue(defaultValue.left);
		mRightProperty->SetUnknownValue(defaultValue.right);
		mTopProperty->SetUnknownValue(defaultValue.top);
		mBottomProperty->SetUnknownValue(defaultValue.bottom);
	}

	void BorderFProperty::SetLeftUnknownValue(float defaultValue)
	{
		mLeftProperty->SetUnknownValue(defaultValue);
	}

	void BorderFProperty::SetRightUnknownValue(float defaultValue)
	{
		mRightProperty->SetUnknownValue(defaultValue);
	}

	void BorderFProperty::SetTopUnknownValue(float defaultValue)
	{
		mTopProperty->SetUnknownValue(defaultValue);
	}

	void BorderFProperty::SetBottomUnknownValue(float defaultValue)
	{
		mBottomProperty->SetUnknownValue(defaultValue);
	}

	void BorderFProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		mLeftProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew LeftValueProxy(x.first), x.second ? mnew LeftValueProxy(x.second) : nullptr); }));

		mRightProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew RightValueProxy(x.first), x.second ? mnew RightValueProxy(x.second) : nullptr); }));

		mTopProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
			return TargetPair(mnew TopValueProxy(x.first), x.second ? mnew TopValueProxy(x.second) : nullptr); }));

		mBottomProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
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
        return mLeftProperty->IsValuesDifferent() || mBottomProperty->IsValuesDifferent()
            || mRightProperty->IsValuesDifferent() || mTopProperty->IsValuesDifferent();
    }
}#include <iostream>
template <typename T>
class Ref {
public:
    Ref(T* ptr) : mPtr(ptr) {}
    ~Ref() { delete mPtr; }
    
    T* operator->() { return mPtr; }
    const T* operator->() const { return mPtr; }
    
    T& operator*() { return *mPtr; }
    const T& operator*() const { return *mPtr; }
    
    T* get() { return mPtr; }
    const T* get() const { return mPtr; }
    
private:
    T* mPtr;
};

 template <typename T>
Ref<T> make_ref() {
    return Ref<T>(new T());
}

class WeakRef {};

class IAbstractValueProxy {
public:
    virtual void GetValuePtr(void* value) const = 0;
    virtual void SetValuePtr(const void* value) = 0;
};

template <typename T>
class TPropertyField {};

template <typename T>
class DynamicCast {};

using Type = int; // This is just a placeholder, replace with your actual type definition

class BorderF {
public:
    float left;
    float right;
    float top;
    float bottom;
    
    bool IsValuesDifferent() {
        return left != 0.0f || right != 0.0f || top != 0.0f || bottom != 0.0f;
    }
};

class BorderFProperty {
public:
    BorderFProperty(const Ref<TPropertyField<BorderF>>& leftProperty, const Ref<TPropertyField<BorderF>>& rightProperty,
                    const Ref<TPropertyField<BorderF>>& topProperty, const Ref<TPropertyField<BorderF>>& bottomProperty)
        : mLeftProperty(leftProperty), mRightProperty(rightProperty), mTopProperty(topProperty), mBottomProperty(bottomProperty) {}
    
    bool IsValuesDifferent() const {
        return mLeftProperty->IsValuesDifferent() || mRightProperty->IsValuesDifferent() ||
                mTopProperty->IsValuesDifferent() || mBottomProperty->IsValuesDifferent();
    }
    
    const Type* GetValueType() const {
        return GetValueTypeStatic();
    }
    
    static const Type* GetValueTypeStatic() {
        return &TypeOf(BorderF);
    }
    
    class LeftValueProxy {
    public:
        LeftValueProxy(const Ref<IAbstractValueProxy>& proxy) : mProxy(proxy) {}
        LeftValueProxy() {}
        
        void SetValue(const float& value) {
            BorderF proxyValue;
            mProxy->GetValuePtr(&proxyValue);
            proxyValue.left = value;
            mProxy->SetValuePtr(&proxyValue);
        }
        
        float GetValue() const {
            BorderF proxyValue;
            mProxy->GetValuePtr(&proxyValue);
            return proxyValue.left;
        }
        
    private:
        const Ref<IAbstractValueProxy> mProxy;
    };
    
    class RightValueProxy {
    public:
        RightValueProxy(const Ref<IAbstractValueProxy>& proxy) : mProxy(proxy) {}
        RightValueProxy() {}
        
        void SetValue(const float& value) {
            BorderF proxyValue;
            mProxy->GetValuePtr(&proxyValue);
            proxyValue.right = value;
            mProxy->SetValuePtr(&proxyValue);
        }
        
        float GetValue() const {
            BorderF proxyValue;
            mProxy->GetValuePtr(&proxyValue);
            return proxyValue.right;
        }
        
    private:
        const Ref<IAbstractValueProxy> mProxy;
    };
    
    class TopValueProxy {
    public:
        TopValueProxy(const Ref<IAbstractValueProxy>& proxy) : mProxy(proxy) {}
        TopValueProxy() {}
        
        void SetValue(const float& value) {
            BorderF proxyValue;
            mProxy->GetValuePtr(&proxyValue);
            proxyValue.top = value;
            mProxy->SetValuePtr(&proxyValue);
        }
        
        float GetValue() const {
            BorderF proxyValue;
            mProxy->GetValuePtr(&proxyValue);
            return proxyValue.top;
        }
        
    private:
        const Ref<IAbstractValueProxy> mProxy;
    };
    
    class BottomValueProxy {
    public:
        BottomValueProxy(const Ref<IAbstractValueProxy>& proxy) : mProxy(proxy) {}
        BottomValueProxy() {}
        
        void SetValue(const float& value) {
            BorderF proxyValue;
            mProxy->GetValuePtr(&proxyValue);
            proxyValue.bottom = value;
            mProxy->SetValuePtr(&proxyValue);
        }
        
        float GetValue() const {
            BorderF proxyValue;
            mProxy->GetValuePtr(&proxyValue);
            return proxyValue.bottom;
        }
        
    private:
        const Ref<IAbstractValueProxy> mProxy;
    };
    
private:
    const Ref<TPropertyField<BorderF>> mLeftProperty;
    const Ref<TPropertyField<BorderF>> mRightProperty;
    const Ref<TPropertyField<BorderF>> mTopProperty;
    const Ref<TPropertyField<BorderF>> mBottomProperty;
};

DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<BorderF>);
// --- META ---
DECLARE_CLASS(Editor::BorderFProperty, Editor__BorderFProperty);
// --- END META ---