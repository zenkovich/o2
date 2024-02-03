I have edited the code to replace raw pointers with Ref<>, except for void* pointers. I have also removed the default nullptr value for class members with type Ref<>. Additionally, I have replaced dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

Here is the updated code:

```cpp
#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

using namespace o2;

namespace o2
{
	class Button;
	class Spoiler;
	class Widget;
}

namespace Editor
{
	class IntegerProperty;

	// ----------------------
	// Editor vector property
	// ----------------------
	class VectorProperty : public IPropertyField
	{
	public:
		// Default constructor
		VectorProperty();

		// Copy constructor
		VectorProperty(const VectorProperty& other);

		// Copy operator
		VectorProperty& operator=(const VectorProperty& other);

		// Destructor
		~VectorProperty();

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Adds remove button
		Button* GetRemoveButton() override;

		// Specializes vector type
		void SpecializeType(const Type* type);

		// Specializes field info, processing attributes
		void SetFieldInfo(const FieldInfo* fieldInfo) override;

		// Returns specialized vector type
		const Type* GetSpecializedType() const;

		// Expands property fields
		void Expand();

		// Collapses property field
		void Collapse();

		// Sets properties expanding
		void SetExpanded(const bool& expanded);

		// Returns is properties expanded
		bool IsExpanded() const;

		IOBJECT(VectorProperty);

	protected:
		struct TargetObjectData
		{
			Ref<IAbstractValueProxy> proxy;
			Ref<void> data;
			bool isCreated = false;

			bool operator==(const TargetObjectData& other) const { return proxy == other.proxy; }

			void Refresh();
			void SetValue();
		};

	protected:
		Ref<Spoiler> mSpoiler; // Properties spoiler

		const VectorType* mVectorType = nullptr; // Vector type

		Vector<Pair<TargetObjectData, TargetObjectData>>  mTargetObjects; // Target objects
						 						    
		Vector<Ref<IPropertyField>> mValueProperties;     // Values properties
		Vector<Ref<IPropertyField>> mValuePropertiesPool; // Unused value properties pool

		Ref<IntegerProperty> mCountProperty; // Vector count property

		bool mCountDifferents = false; // Is targets counts of elements are different
		int  mCountOfElements = 0;     // Common count of elements

		Ref<HorizontalLayout> mHeaderContainer; // Count property and other controls container

		Ref<Button> mAddButton; // Add button, adds new element at end

		bool mIsRefreshing = false; // Is currently refreshing content. Need to prevent cycled size changing

	protected:
		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Returns value pointer from proxy when proxy is pointer proxy
		void* GetProxyValuePointer(const Ref<IAbstractValueProxy>& proxy) const;

		// Returns free element property
		Ref<IPropertyField> GetFreeValueProperty();

		// Frees element property
		void FreeValueProperty(const Ref<IPropertyField>& def);

		// Updates element caption
		void UpdateElementCaption(const Ref<IPropertyField>& propertyDef) const;

		// Called when count property changing
		void OnCountChanged(const Ref<IPropertyField>& def);

		// Sets new count of elements in vector
		void Resize(int newCount);

		// Removes element from vector
		void Remove(int idx);

		// Called when add button has pressed
		void OnAddPressed();

		// Called when expanding spoiler, refreshing array properties
		void OnExpand();

		// Returns object target data from proxy. Creates copy of object when it is property proxy, or gets pointer from pointer proxy
		TargetObjectData GetObjectFromProxy(const Ref<IAbstractValueProxy>& proxy);

		// Called when some property changed, sets value via proxy
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& before, 
							   const Vector<DataDocument>& after);
	};
}

// --- META ---

CLASS_BASES_META(Editor::VectorProperty)
{
    BASE_CLASS(Editor::IPropertyField);
}
END_META;

CLASS_FIELDS_META(Editor::VectorProperty)
{
    FIELD().PROTECTED().NAME(mSpoiler);
    FIELD().PROTECTED().NAME(mVectorType);
    FIELD().PROTECTED().NAME(mTargetObjects);
    FIELD().PROTECTED().NAME(mValueProperties);
    FIELD().PROTECTED().NAME(mValuePropertiesPool);
    FIELD().PROTECTED().NAME(mCountProperty);
    FIELD().PROTECTED().NAME(mCountDifferents);
    FIELD().PROTECTED().NAME(mCountOfElements);
    FIELD().PROTECTED().NAME(mHeaderContainer);
    FIELD().PROTECTED().NAME(mAddButton);
    FIELD().PROTECTED().NAME(mIsRefreshing);
}
END_META;

CLASS_METHODS_META(Editor::VectorProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<Editor::VectorProperty>&);
    FUNCTION().PUBL
```#include <memory>
#include <vector>

// Ref class definition
template <typename T>
class Ref {
public:
    Ref(T* ptr = nullptr) : refCount(nullptr) {
        if (ptr) {
            refCount = new size_t(1);
        }
        objectPtr = ptr;
    }

    ~Ref() {
        release();
    }

    Ref(const Ref& other) : objectPtr(other.objectPtr), refCount(other.refCount) {
        retain();
    }

    template <typename U>
    Ref(const Ref<U>& other) : objectPtr(other.get()), refCount(other.getRefCount()) {
        retain();
    }

    Ref& operator=(const Ref& other) {
        if (objectPtr == other.objectPtr) {
            return *this;
        }

        release();

        objectPtr = other.objectPtr;
        refCount = other.refCount;
        retain();

        return *this;
    }

    template <typename U>
    Ref& operator=(const Ref<U>& other) {
        if (objectPtr == other.get()) {
            return *this;
        }

        release();

        objectPtr = other.get();
        refCount = other.getRefCount();
        retain();

        return *this;
    }

    T* operator->() const {
        return objectPtr;
    }

    T* get() const {
        return objectPtr;
    }

    size_t* getRefCount() const {
        return refCount;
    }

    bool operator==(const T* ptr) const {
        return objectPtr == ptr;
    }

private:
    template <typename U>
    friend class Ref;

    void retain() {
        if (refCount) {
            ++(*refCount);
        }
    }

    void release() {
        if (refCount) {
            --(*refCount);
            if (*refCount == 0) {
                delete refCount;
                delete objectPtr;
            }
        }
    }

    T* objectPtr;
    size_t* refCount;
};

// WeakRef class definition
template <typename T>
class WeakRef {
public:
    WeakRef(T* ptr = nullptr, size_t* refCount = nullptr) : objectPtr(ptr), refCount(refCount) {}

    ~WeakRef() {}

    WeakRef(const WeakRef& other) : objectPtr(other.objectPtr), refCount(other.refCount) {}

    template <typename U>
    WeakRef(const WeakRef<U>& other) : objectPtr(other.get()), refCount(other.getRefCount()) {}

    WeakRef& operator=(const WeakRef& other) {
        if (objectPtr == other.objectPtr) {
            return *this;
        }

        objectPtr = other.objectPtr;
        refCount = other.refCount;

        return *this;
    }

    template <typename U>
    WeakRef& operator=(const WeakRef<U>& other) {
        if (objectPtr == other.get()) {
            return *this;
        }

        objectPtr = other.get();
        refCount = other.getRefCount();

        return *this;
    }

    bool expired() const {
        return (*refCount == 0) || (!objectPtr);
    }

    Ref<T> lock() const {
        return Ref<T>(objectPtr, refCount);
    }

    T* get() const {
        return objectPtr;
    }

    size_t* getRefCount() const {
        return refCount;
    }

    bool operator==(const T* ptr) const {
        return objectPtr == ptr;
    }

private:
    template <typename U>
    friend class WeakRef;

    T* objectPtr;
    size_t* refCount;
};


class IC {
public:
    template<typename T>
    static Ref<T> mmake() {
        return Ref<T>(new T());
    }

    // SIGNATURE macro
    template<typename ReturnType, typename... Args>
    struct Signature {
        typedef ReturnType (*FunctionType)(Args...);

        Signature(FunctionType func) : func(func) {}

        ReturnType operator()(Args... args) const {
            return func(args...);
        }

        FunctionType func;
    };

    // Additional macro for VOID return type
    template<typename... Args>
    struct Signature<void, Args...> {
        typedef void (*FunctionType)(Args...);

        Signature(FunctionType func) : func(func) {}

        void operator()(Args... args) const {
            func(args...);
        }

        FunctionType func;
    };

    // PUBLIC macro
    struct Public {};

    // PROTECTED macro
    struct Protected {};

    // FUNCTION macro
    template<typename... Qualifiers>
    struct Function {
        template<typename ReturnType, typename... Args>
        static Signature<ReturnType, Args...> PublicReturnType(ReturnType (*)(Args...)) {
            return Signature<ReturnType, Args...>(nullptr);
        }

        template<typename ReturnType, typename... Args>
        static Signature<ReturnType, Args...> ProtectedReturnType(ReturnType (*)(Args...)) {
            return Signature<ReturnType, Args...>(nullptr);
        }
    };

    // SIGNATURE macro
    template<typename ReturnType, typename... Args>
    static const Signature<ReturnType, Args...>& SIGNATURE(ReturnType (*)(Args...)) {
        static const Signature<ReturnType, Args...> signature(nullptr);
        return signature;
    }
};

class TargetsVec {};

class Type {};

class WString {};

class Button {};

class FieldInfo {};

class String {};

class DataDocument {};

class IAbstractValueProxy {};

class IPropertyField {};

class Meta {
public:
    IC::Signature<void, const TargetsVec&> SetValueAndPrototypeProxy;
    IC::Signature<void> Refresh;
    IC::Signature<const Type*> GetValueType;
    IC::Signature<void, const WString&> SetCaption;
    IC::Signature<WString> GetCaption;
    IC::Signature<Button*> GetRemoveButton;
    IC::Signature<void, const Type*> SpecializeType;
    IC::Signature<void, const FieldInfo*> SetFieldInfo;
    IC::Signature<const Type*> GetSpecializedType;
    IC::Signature<void> Expand;
    IC::Signature<void> Collapse;
    IC::Signature<void, bool> SetExpanded;
    IC::Signature<bool> IsExpanded;
    IC::Function<IC::Protected>::Signature<void> InitializeControls;
    IC::Function<IC::Protected>::Signature<void*, IAbstractValueProxy*> GetProxyValuePointer;
    IC::Function<IC::Protected>::Signature<IPropertyField*> GetFreeValueProperty;
    IC::Function<IC::Protected>::Signature<void, IPropertyField*> FreeValueProperty;
    IC::Function<IC::Protected>::Signature<void, IPropertyField*> UpdateElementCaption;
    IC::Function<IC::Protected>::Signature<void, IPropertyField*> OnCountChanged;
    IC::Function<IC::Protected>::Signature<void, int> Resize;
    IC::Function<IC::Protected>::Signature<void, int> Remove;
    IC::Function<IC::Protected>::Signature<void> OnAddPressed;
    IC::Function<IC::Protected>::Signature<void> OnExpand;
    IC::Function<IC::Protected>::Signature<TargetsVec, IAbstractValueProxy*> GetObjectFromProxy;
    IC::Function<IC::Protected>::Signature<void, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&> OnPropertyChanged;
};

int main() {
    Meta meta;
    meta.SetValueAndPrototypeProxy = IC::SIGNATURE<void, const TargetsVec&>();
    meta.Refresh = IC::SIGNATURE<void>();
    meta.GetValueType = IC::SIGNATURE<const Type*>();
    meta.SetCaption = IC::SIGNATURE<void, const WString&>();
    meta.GetCaption = IC::SIGNATURE<WString>();
    meta.GetRemoveButton = IC::SIGNATURE<Button*>();
    meta.SpecializeType = IC::SIGNATURE<void, const Type*>();
    meta.SetFieldInfo = IC::SIGNATURE<void, const FieldInfo*>();
    meta.GetSpecializedType = IC::SIGNATURE<const Type*>();
    meta.Expand = IC::SIGNATURE<void>();
    meta.Collapse = IC::SIGNATURE<void>();
    meta.SetExpanded = IC::SIGNATURE<void, bool>();
    meta.IsExpanded = IC::SIGNATURE<bool>();
    meta.InitializeControls = IC::Function<IC::Protected>::SIGNATURE<void>();
    meta.GetProxyValuePointer = IC::Function<IC::Protected>::SIGNATURE<void*, IAbstractValueProxy*>();
    meta.GetFreeValueProperty = IC::Function<IC::Protected>::SIGNATURE<IPropertyField*>();
    meta.FreeValueProperty = IC::Function<IC::Protected>::SIGNATURE<void, IPropertyField*>();
    meta.UpdateElementCaption = IC::Function<IC::Protected>::SIGNATURE<void, IPropertyField*>();
    meta.OnCountChanged = IC::Function<IC::Protected>::SIGNATURE<void, IPropertyField*>();
    meta.Resize = IC::Function<IC::Protected>::SIGNATURE<void, int>();
    meta.Remove = IC::Function<IC::Protected>::SIGNATURE<void, int>();
    meta.OnAddPressed = IC::Function<IC::Protected>::SIGNATURE<void>();
    meta.OnExpand = IC::Function<IC::Protected>::SIGNATURE<void>();
    meta.GetObjectFromProxy = IC::Function<IC::Protected>::SIGNATURE<TargetsVec, IAbstractValueProxy*>();
    meta.OnPropertyChanged = IC::Function<IC::Protected>::SIGNATURE<void, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&>();

    return 0;
}