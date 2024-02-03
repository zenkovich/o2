#include "o2Editor/stdafx.h"
#include "RectangleFloatProperty.h"

#include "o2Editor/Core/Properties/Basic/FloatProperty.h"

namespace Editor
{
	RectFProperty::RectFProperty()
		: mLeftProperty(mmake<Ref<FloatProperty>>()), mBottomProperty(mmake<Ref<FloatProperty>>()), 
		  mRightProperty(mmake<Ref<FloatProperty>>()), mTopProperty(mmake<Ref<FloatProperty>>())
	{
		InitializeControls();
	}

	RectFProperty::RectFProperty(const RectFProperty& other)
		: IPropertyField(other), mLeftProperty(other.mLeftProperty), mBottomProperty(other.mBottomProperty),
		  mRightProperty(other.mRightProperty), mTopProperty(other.mTopProperty)
	{
		InitializeControls();
	}

	RectFProperty& RectFProperty::operator=(const RectFProperty& other)
	{
		IPropertyField::operator=(other);
		mLeftProperty = other.mLeftProperty;
		mBottomProperty = other.mBottomProperty;
		mRightProperty = other.mRightProperty;
		mTopProperty = other.mTopProperty;
		InitializeControls();
		return *this;
	}

	void RectFProperty::InitializeControls()
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

		mLeftProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
			return TargetPair(mmake<LeftValueProxy>(x.first), x.second ? mmake<LeftValueProxy>(x.second) : nullptr); }));

		mRightProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
			return TargetPair(mmake<RightValueProxy>(x.first), x.second ? mmake<RightValueProxy>(x.second) : nullptr); }));

		mTopProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
			return TargetPair(mmake<TopValueProxy>(x.first), x.second ? mmake<TopValueProxy>(x.second) : nullptr); }));

		mBottomProperty->SetValueAndPrototypeProxy(targets.Convert<TargetPair>([](const TargetPair& x) {
			return TargetPair(mmake<BottomValueProxy>(x.first), x.second ? mmake<BottomValueProxy>(x.second) : nullptr); }));
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
		return mLeftProperty->IsValuesDifferent() || mRightProperty->IsValuesDifferent()
			|| mTopProperty->IsValuesDifferent() || mBottomProperty->IsValuesDifferent();
	}
}#include <memory>

template <typename T>
class Ref
{
public:
    Ref() : m_ptr(nullptr) {}
    Ref(T* ptr) : m_ptr(ptr) {}
    Ref(const Ref& other) : m_ptr(other.m_ptr) {}
    Ref& operator=(const Ref& other)
    {
        m_ptr = other.m_ptr;
        return *this;
    }
    ~Ref() {}

    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }

private:
    T* m_ptr;
};

template <typename T>
class WeakRef
{
public:
    WeakRef() : m_ptr(nullptr) {}
    WeakRef(const std::shared_ptr<T>& ptr) : m_ptr(ptr) {}
    WeakRef(const WeakRef& other) : m_ptr(other.m_ptr) {}
    WeakRef& operator=(const WeakRef& other)
    {
        m_ptr = other.m_ptr;
        return *this;
    }
    ~WeakRef() {}

    std::shared_ptr<T> lock()
    {
        return m_ptr.lock();
    }

private:
    std::weak_ptr<T> m_ptr;
};

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template <typename Derived, typename Base>
Ref<Derived> DynamicCast(const Ref<Base>& ptr)
{
    return std::dynamic_pointer_cast<Derived>(ptr);
}

namespace Editor
{
    using o2::RectF;

    class IAbstractValueProxy
    {
    public:
        virtual ~IAbstractValueProxy() {}
        virtual void GetValuePtr(RectF* value) const = 0;
        virtual void SetValuePtr(const RectF* value) = 0;
    };

    class TPropertyFieldBase {};

    template <typename T>
    class TPropertyField : public TPropertyFieldBase {};

    class RectFProperty : public TPropertyField<o2::RectF>
    {
    public:
        class LeftValueProxy
        {
        public:
            LeftValueProxy(const Ref<IAbstractValueProxy>& proxy) : mProxy(proxy) {}
            LeftValueProxy() {}

            void SetValue(const float& value)
            {
                RectF proxyValue;
                mProxy->GetValuePtr(&proxyValue);
                proxyValue.left = value;
                mProxy->SetValuePtr(&proxyValue);
            }

            float GetValue() const
            {
                RectF proxyValue;
                mProxy->GetValuePtr(&proxyValue);
                return proxyValue.left;
            }

        private:
            Ref<IAbstractValueProxy> mProxy;
        };

        class RightValueProxy
        {
        public:
            RightValueProxy(const Ref<IAbstractValueProxy>& proxy) : mProxy(proxy) {}
            RightValueProxy() {}

            void SetValue(const float& value)
            {
                RectF proxyValue;
                mProxy->GetValuePtr(&proxyValue);
                proxyValue.right = value;
                mProxy->SetValuePtr(&proxyValue);
            }

            float GetValue() const
            {
                RectF proxyValue;
                mProxy->GetValuePtr(&proxyValue);
                return proxyValue.right;
            }

        private:
            Ref<IAbstractValueProxy> mProxy;
        };

        class TopValueProxy
        {
        public:
            TopValueProxy(const Ref<IAbstractValueProxy>& proxy) : mProxy(proxy) {}
            TopValueProxy() {}

            void SetValue(const float& value)
            {
                RectF proxyValue;
                mProxy->GetValuePtr(&proxyValue);
                proxyValue.top = value;
                mProxy->SetValuePtr(&proxyValue);
            }

            float GetValue() const
            {
                RectF proxyValue;
                mProxy->GetValuePtr(&proxyValue);
                return proxyValue.top;
            }

        private:
            Ref<IAbstractValueProxy> mProxy;
        };

        class BottomValueProxy
        {
        public:
            BottomValueProxy(const Ref<IAbstractValueProxy>& proxy) : mProxy(proxy) {}
            BottomValueProxy() {}

            void SetValue(const float& value)
            {
                RectF proxyValue;
                mProxy->GetValuePtr(&proxyValue);
                proxyValue.bottom = value;
                mProxy->SetValuePtr(&proxyValue);
            }

            float GetValue() const
            {
                RectF proxyValue;
                mProxy->GetValuePtr(&proxyValue);
                return proxyValue.bottom;
            }

        private:
            Ref<IAbstractValueProxy> mProxy;
        };

        RectFProperty::LeftValueProxy mLeftValueProxy;
        RectFProperty::RightValueProxy mRightValueProxy;
        RectFProperty::TopValueProxy mTopValueProxy;
        RectFProperty::BottomValueProxy mBottomValueProxy;

        RectFProperty(const Ref<IAbstractValueProxy>& proxy) :
            mLeftValueProxy(proxy),
            mRightValueProxy(proxy),
            mTopValueProxy(proxy),
            mBottomValueProxy(proxy)
        {}

        const RectF* operator->() const { return nullptr; }
        RectF* operator->() { return nullptr; }

        bool IsValuesDifferent() const { return mLeftValueProxy.GetValue() != mRightValueProxy.GetValue() ||
            mTopValueProxy.GetValue() || mBottomValueProxy.GetValue(); }

        const Type* GetValueType() const;
        static const Type* GetValueTypeStatic();

    private:
        class Type {};

        const Ref<Type>& TypeOf(const Ref<Type>& type);
    };

    DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<o2::RectF>);
    // --- META ---

    DECLARE_CLASS(Editor::RectFProperty, Editor__RectFProperty);
    // --- END META ---
}