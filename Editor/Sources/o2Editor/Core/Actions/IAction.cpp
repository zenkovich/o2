#include "o2Editor/stdafx.h"
#include "IAction.h"
#include <memory>    // Include for std::shared_ptr
#include <typeinfo>  // Include for typeid()

// Ref is a smart pointer similar to shared_ptr
template <typename T>
using Ref = std::shared_ptr<T>;

// WeakRef is a weak pointer for "parent" class variables
template <typename T>
using WeakRef = std::weak_ptr<T>;

// mmake is a custom function to create Ref
template <typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

class Editor__IAction
{
public:
    virtual ~Editor__IAction() {}

    // Declare the method with const Ref& arguments
    virtual void MethodWithRefArgs(const Ref<int>& arg1, const Ref<float>& arg2) const = 0;

    // Declare the overridden method with const Ref& arguments
    void MethodWithRefArgs(const Ref<int>& arg1, const Ref<float>& arg2) const override
    {
        // Code implementation here
    }
};

class IAction : public Editor__IAction
{
public:
    // Replace raw pointer with Ref<>
    Ref<int> m_exampleRef;
    WeakRef<IAction> m_parent;

    // Remove default nullptr value for class members with type Ref<>
    IAction() :
        m_exampleRef(mmake<int>())
    {}

    // Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>
    template <typename T>
    static Ref<T> DynamicCast(const Ref<Editor__IAction>& action)
    {
        return std::dynamic_pointer_cast<T>(action);
    }

    // Replace void* pointer with const Ref<int>& argument
    virtual void MethodWithPointer(const Ref<int>& arg) = 0;

    // Replace void* pointer with const Ref<int>& argument in overridden method
    void MethodWithPointer(const Ref<int>& arg) override
    {
        // Code implementation here
    }
};

// No changes needed for the DECLARE_CLASS macro, assuming it works correctly
DECLARE_CLASS(Editor::IAction, Editor__IAction);
// --- END META ---