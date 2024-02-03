#include "o2Editor/stdafx.h"
#include <memory>
#include <typeinfo>

template <typename T>
class Ref
{
public:
    Ref() : ptr(nullptr) {}
    Ref(T* p) : ptr(p) {}
    Ref(const Ref<T>& other) : ptr(other.ptr) {}

    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }

    operator bool() const { return ptr != nullptr; }
    bool operator==(const Ref<T>& other) const { return ptr == other.ptr; }
    bool operator!=(const Ref<T>& other) const { return ptr != other.ptr; }

    template <typename U>
    friend class Ref;

private:
    T* ptr;
};

template <typename T>
class WeakRef
{
public:
    WeakRef() : ptr(nullptr) {}
    WeakRef(const Ref<T>& other) : ptr(other.ptr) {}

    bool expired() const { return ptr.expired(); }

    Ref<T> lock() const { return Ref<T>(ptr.lock()); }

    template <typename U>
    friend class WeakRef;

private:
    std::weak_ptr<T> ptr;
};

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template <typename Derived, typename Base>
Ref<Derived> DynamicCast(const Ref<Base>& ptr)
{
    if (ptr)
    {
        if (typeid(ptr) == typeid(Ref<Derived>))
        {
            return Ref<Derived>(dynamic_cast<Derived*>(ptr.ptr));
        }
    }
    return Ref<Derived>();
}

class MyClass
{
public:
    MyClass() : refMember(mmake<int>()), weakParent() {}
    MyClass(const Ref<int>& ref) : refMember(ref), weakParent() {}

    void SetParent(const WeakRef<MyClass>& parent) { weakParent = parent; }

private:
    Ref<int> refMember;
    WeakRef<MyClass> weakParent;
};

void ProcessData(const Ref<int>& data)
{
    // process data
}

int main()
{
    Ref<int> ref = mmake<int>(42);
    ProcessData(ref);

    MyClass obj(ref);
    MyClass child;
    child.SetParent(WeakRef<MyClass>(ref));

    Ref<MyClass> objRef = mmake<MyClass>();
    MyClass* rawPtr = objRef.operator->();

    Ref<MyClass> derived = DynamicCast<MyClass>(objRef);
    if (derived)
    {
        // do something with derived
    }

    return 0;
}