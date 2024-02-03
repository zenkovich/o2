#pragma once

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class MyClass
{
public:
    MyClass() = default;

    MyClass(const Ref<SomeClass>& ptr)
        : m_memberPtr(ptr)
    {
    }

    void someFunction(const Ref<SomeOtherClass>& ptr)
    {
        m_memberPtr = ptr;
    }

    Ref<SomeClass> getMemberPtr() const
    {
        return m_memberPtr;
    }

protected:
    Ref<SomeClass> m_memberPtr;
};