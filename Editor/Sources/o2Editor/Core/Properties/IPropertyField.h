#pragma once

#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/ValueProxy.h"

using namespace o2;

namespace o2
{
	class Widget;
	class Button;
}

namespace Editor
{
	class PropertiesContext;

	// -------------------------------
	// Editor property field interface
	// -------------------------------
	class IPropertyField : public HorizontalLayout
	{
	public:
		typedef Ref<IAbstractValueProxy> AbstractValueProxyRef;
		typedef Pair<AbstractValueProxyRef, AbstractValueProxyRef> TargetPair;
		typedef Vector<TargetPair> TargetsVec;

		typedef Function<void(IPropertyField&)> OnChangedFunc;
		typedef Function<void(const String&, const Vector<DataDocument>&, const Vector<DataDocument>&)> OnChangeCompletedFunc;

	public:
		OnChangedFunc onChanged;         // Immediate change value by user event
		OnChangeCompletedFunc onChangeCompleted; // Change completed by user event

	public:
		// Default constructor. Searches revert button and sets click action
		IPropertyField();

		// Copy-constructor
		IPropertyField(const IPropertyField& other);

		// Virtual destructor
		virtual ~IPropertyField();

		// Copy-operator
		IPropertyField& operator=(const IPropertyField& other);

		// Sets targets proxies
		virtual void SetValueAndPrototypeProxy(const TargetsVec& targets);

		// Returns targets proxies
		const TargetsVec& GetValueAndPrototypeProxy() const;

		// Sets targets proxies
		virtual void SetValueProxy(const Vector<AbstractValueProxyRef>& targets);

		// Sets parent context
		virtual void SetParentContext(PropertiesContext* context);

		// Checks common value and fill fields
		virtual void Refresh() {}

		// Reverts value to prototype value
		virtual void Revert() {}

		// Sets property caption
		virtual void SetCaption(const WString& text);

		// Returns property caption
		virtual WString GetCaption() const;

		// Returns remove button, creates if required
		virtual Button* GetRemoveButton();

		// Returns editing by this field type
		virtual const Type* GetValueType() const;

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets reflection path f target values
		void SetValuePath(const String& path);

		// Returns reflection path of target values
		const String& GetValuePath() const;

		// Sets caption
		void SetCaptionLabel(Label* label);

		// Returns caption
		Label* GetCaptionLabel() const;

		// Sets is property revertable - it's able to check difference between origin value and prototype
		void SetRevertable(bool revertable);

		// Returns is property revertable - it's able to check difference between origin value and prototype
		bool IsRevertable() const;

		// Specializes field info
		virtual void SetFieldInfo(const FieldInfo* fieldInfo);

		// Sets targets pointers
		template<typename _type>
		void SetValuePointers(const Vector<_type*>& targets);

		// Sets targets pointers
		template<typename _property_type>
		void SetValuePropertyPointers(const Vector<_property_type*>& targets);

		// Sets targets pointers
		template<typename _type, typename _object_type>
		void SelectValuesPointers(const Vector<_object_type*>& targets, std::function<_type*(_object_type*)> getter);

		// Sets targets properties
		template<typename _object_type, typename _property_type>
		void SelectValuesProperties(const Vector<_object_type*>& targets,
									std::function<_property_type*(_object_type*)> getter);

		// Sets targets pointers
		template<typename _type, typename _object_type>
		void SelectValueAndPrototypePointers(const Vector<_object_type*>& targets,
											 const Vector<_object_type*>& prototypes,
											 std::function<_type*(_object_type*)> getter);

		// Sets targets properties
		template<typename _object_type, typename _property_type>
		void SelectValueAndPrototypeProperties(const Vector<_object_type*>& targets,
											   const Vector<_object_type*>& prototypes,
											   std::function<_property_type* (_object_type*)> getter);

		// Sets targets properties
		template<typename _type, typename _object_type>
		void SelectValueAndPrototypeFunctional(const Vector<_object_type*>& targets,
											   const Vector<_object_type*>& prototypes,
											   std::function<_type(_object_type*)> getter,
											   std::function<void(_object_type*, _type)> setter);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		IOBJECT(IPropertyField);

	protected:
		const FieldInfo* mFieldInfo = nullptr; // Specialized field info

		PropertiesContext* mParentContext = nullptr; // Parent context

		bool mRevertable = true; // Is property can be reverted

		Vector<TargetPair> mValuesProxies;          // Target values pro#include <Ref.h>

class Properties;

class IAbstractValueProxy {};

class DataDocument {};

template<typename T>
class Ref {};

template<typename T>
class WeakRef {};

template<typename _type>
class TPropertyField;

class Button {
public:
    // constructors
};

class Label {
public:
    // constructors
};

class Type {};

class IPropertyField {
public:
    // constructors
    void Refresh();
    void Revert();
    const Type* GetValueType() const;
    static const Type* GetValueTypeStatic();
    void SetValueByUser();
    virtual void OnTypeSpecialized(const Type& type) {}
    virtual bool IsValueRevertable() const {}

protected:
    template<typename T>
    T GetProxy(IAbstractValueProxy* proxy) const;

    template<typename T>
    void SetProxy(IAbstractValueProxy* proxy, const T& value) const;

    template<typename _type>
    void SetValuePointers(const Vector<_type*>& targets);

    template<typename _property_type>
    void SetValuePropertyPointers(const Vector<_property_type*>& targets);

    template<typename _type, typename _object_type>
    void SelectValuesPointers(const Vector<_object_type*>& targets, std::function<_type* (_object_type*)> getter);

private:
    void SetValueAndPrototypeProxy(const Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>&);
};

class IAbstractValueProxy {};

class DataDocument {};

template<typename T>
class Ref {};

template<typename T>
class WeakRef {};

template<typename _type>
class TPropertyField {
public:
    // constructors
    TPropertyField();
    TPropertyField(const TPropertyField& other);
    TPropertyField& operator=(const TPropertyField& other);
    void Refresh() override;
    void Revert() override;
    const Type* GetValueType() const override;
    static const Type* GetValueTypeStatic();
    void SetValue(const _type& value);
    void SetUnknownValue(const _type& defaultValue = _type());
    _type GetCommonValue() const;

protected:
    void OnTypeSpecialized(const Type& type) override;
    bool IsValueRevertable() const override;
    void StoreValues(Vector<DataDocument>& data) const override;
    _type GetProxy(IAbstractValueProxy* proxy) const;
    void SetProxy(IAbstractValueProxy* proxy, const _type& value);
    virtual void SetCommonValue(const _type& value);
    void SetValueByUser(const _type& value);
    virtual void UpdateValueView() {}
    bool IsAlwaysRefresh() const;

private:
    _type mCommonValue = _type();
    const Type* mRealType = nullptr;
};

bool                                                     mValuesDifferent = true;

Ref<Button> mRevertBtn;
Ref<Button> mRemoveBtn;
Ref<Label>  mCaption;

String               mValuesPath;
Vector<DataDocument> mBeforeChangeValues;

void OnTypeSpecialized(const Type& type) {}
void OnFreeProperty();
void StoreValues(Vector<DataDocument>& data) const {}
void CheckValueChangeCompleted();
void CheckRevertableState();
bool IsValueRevertable() const {}
void OnValueChanged();
void FreeValuesProxies();
void BeginUserChanging();
void EndUserChanging();

template<typename T>
void IPropertyField::SetProxy(IAbstractValueProxy* proxy, const T& value) const {
    proxy->SetValuePtr(&const_cast<T&>(value));
}

template<typename T>
T IPropertyField::GetProxy(IAbstractValueProxy* proxy) const {
    T res;
    proxy->GetValuePtr(&res);
    return res;
}	template<typename _type>
	TPropertyField<_type>::TPropertyField()
	{}

	template<typename _type>
	TPropertyField<_type>::TPropertyField(const TPropertyField& other):
		IPropertyField(other)
	{}

	template<typename _type>
	TPropertyField<_type>& TPropertyField<_type>::operator=(const TPropertyField& other)
	{
		IPropertyField::operator=(other);
		return *this;
	}

	template<typename _type>
	void TPropertyField<_type>::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		bool lastDifferent = mValuesDifferent;

		_type newCommonValue = *GetProxy(mValuesProxies[0].first);

		bool newDifferent = false;

		for (int i = 1; i < mValuesProxies.Count(); i++)
		{
			_type value = *GetProxy(mValuesProxies[i].first);
			if (newCommonValue != value)
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue();
		}
		else if (lastCommonValue != newCommonValue || lastDifferent || IsAlwaysRefresh())
			SetCommonValue(newCommonValue);

		CheckRevertableState();
	}

	template<typename _type>
	void TPropertyField<_type>::OnTypeSpecialized(const Type& type)
	{
		mRealType = &type;
	}

	template<typename _type>
	bool TPropertyField<_type>::IsAlwaysRefresh() const
	{
		return false;
	}

	template<typename _type>
	void TPropertyField<_type>::SetValueByUser(const _type& value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	template<typename _type>
	void TPropertyField<_type>::SetCommonValue(const _type& value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		UpdateVtemplate<typename _type>
class Ref {
public:
	Ref() : m_ptr(nullptr) {}
	Ref(_type* p) : m_ptr(p) {
		if (m_ptr)
			m_ptr->AddRef();
	}
	Ref(const Ref<_type>& r) : m_ptr(r.m_ptr) {
		if (m_ptr)
			m_ptr->AddRef();
	}
	~Ref() {
		if (m_ptr)
			m_ptr->Release();
	}
	Ref<_type>& operator=(const Ref<_type>& r) {
		if (m_ptr) {
			m_ptr->Release();
			m_ptr = nullptr;
		}
		m_ptr = r.m_ptr;
		if (m_ptr)
			m_ptr->AddRef();
		return *this;
	}
	Ref<_type>& operator=(_type* p) {
		if (m_ptr) {
			m_ptr->Release();
			m_ptr = nullptr;
		}
		m_ptr = p;
		if (m_ptr)
			m_ptr->AddRef();
		return *this;
	}
	operator bool() const {
		return (m_ptr != nullptr);
	}
	_type* operator->() const {
		return m_ptr;
	}
	_type& operator*() const {
		return *m_ptr;
	}
private:
	_type* m_ptr;
};

template<typename _type>
class WeakRef {
public:
	WeakRef() : m_ptr(nullptr) {}
	WeakRef(const Ref<_type>& r) : m_ptr(r.m_ptr) {}
	WeakRef(const WeakRef<_type>& r) : m_ptr(r.m_ptr) {}
	~WeakRef() {}

	Ref<_type> Lock() const {
		return Ref<_type>(m_ptr);
	}
	operator bool() const {
		return (m_ptr != nullptr);
	}
	_type* operator->() const {
		return m_ptr;
	}
	_type& operator*() const {
		return *m_ptr;
	}
private:
	_type* m_ptr;
};

template<typename _type>
Ref<_type> mmake() {
	return Ref<_type>(new _type());
}

class IAbstractValueProxy {
public:
	virtual ~IAbstractValueProxy() {}
	virtual void GetValuePtr(void* pValue) const = 0;
};

template<typename _type>
class TPropertyField : public o2::HorizontalLayout, public IPropertyField {
public:
	TPropertyField() : mRevertable(true) {
		mRevertBtn = mmake<Button>();
		mRemoveBtn = mmake<Button>();
		mCaption = mmake<Label>();
		mValuesDifferent = true;

		AddChild(mRevertBtn);
		AddChild(mRemoveBtn);
		AddChild(mCaption);

		mRevertBtn->OnClick.Add(Delegate(this, &TPropertyField<_type>::OnRevert));
		mRemoveBtn->OnClick.Add(Delegate(this, &TPropertyField<_type>::OnRemove));

		OnRefresh();
	}

	virtual ~TPropertyField() {
	}

	void SetValueAndPrototypeProxy(const TargetsVec& targets) {
		mValuesProxies.clear();

		for (auto& target : targets)
		{
			if (target.instance)
			{
				auto protoProxy = dynamic_cast<IPrototypeProxy*>(target.instance);
				if (protoProxy && protoProxy->IsDefault()) // optimize dev prone proxies
					continue;
			}

			IAbstractValueProxy* proxy = target.GetPropertyProxy(mValuesPath);
			if (!proxy)
				continue;

			auto it = mValuesProxies.find(proxy);
			if (it != mValuesProxies.end())
			{
				it->second = nullptr;
			}
			else
			{
				mValuesProxies.insert(std::make_pair(proxy, nullptr));
			}
		}

		CheckRevertableState();
		OnRefresh();
	}

	const TargetsVec GetValueAndPrototypeProxy() const {
		TargetsVec result;
		for (auto& proxy : mValuesProxies) {
			TargetDesc desc;
			desc.instance = proxy.first->GetInstance();
			desc.property = proxy.first->GetProperty();
			result.push_back(desc);
		}
		return result;
	}

	void SetValueProxy(const Vector<Ref<IAbstractValueProxy>>& proxies) {
		mValuesProxies.clear();
		for (const auto& proxy : proxies)
		{
			auto it = mValuesProxies.find(proxy.Get());
			if (it != mValuesProxies.end()) {
				it->second = nullptr;
			}
			else {
				mValuesProxies.insert(std::make_pair(proxy.Get(), nullptr));
			}
		}

		CheckRevertableState();
		OnRefresh();
	}

	void SetParentContext(Ref<PropertiesContext> parentContext) {
		mParentContext = parentContext;
	}

	void Refresh() {
		OnRefresh();
	}

	void Revert() {
		for (auto& proxy : mValuesProxies)
		{
			if (proxy.second)
				SetProxy(proxy.first, GetProxy(proxy.second));
		}

		mValuesDifferent = true;

		OnRefresh();
	}

	void SetCaption(const WString& caption) {
		mCaption->SetText(caption);
	}

	WString GetCaption() {
		return mCaption->GetText();
	}

	Button* GetRemoveButton() {
		return mRemoveBtn;
	}

	const Type* GetValueType() const {
		return GetValueTypeStatic();
	}

	static const Type* GetValueTypeStatic() {
		return &TypeOf(_type);
	}

	bool IsValuesDifferent() {
		return mValuesDifferent;
	}

	void SetValuePath(const String& valuePath) {
		mValuesPath = valuePath;
	}

	String GetValuePath() {
		return mValuesPath;
	}

	void SetCaptionLabel(Label* captionLabel) {
		mCaption = captionLabel;
	}

	Label* GetCaptionLabel() {
		return mCaption;
	}

	void SetRevertable(bool revertable) {
		mRevertable = revertable;
	}

	bool IsRevertable() {
		return mRevertable;
	}

	void SetFieldInfo(const Ref<FieldInfo>& fieldInfo) {
		mFieldInfo = fieldInfo;
	}

	virtual void OnRefresh() = 0;

	virtual void OnRevert() {
		Revert();
		mRevertBtn->SetEnabled(false);
		OnValueChanged();
	}

	virtual void OnRemove() {
		OnRemoveField.Raise(this);
	}

	virtual void OnValueChanged() {
		OnChanged.Raise(this);
	}

	static String GetCreateMenuCategory() {
		return "";
	}

protected:
	virtual void OnTypeSpecialized(const Type& newType) {}

	virtual void OnFreeProperty() {}

	void StoreValues(Vector<DataDocument>& data) const {
		data.Clear();
		for (auto& proxy : mValuesProxies) {
			data.Add(DataDocument());
			data.Last() = GetProxy(proxy.first);
		}
	}

	void CheckValueChangeCompleted() {
		if (!mRevertable || mValuesDifferent) {
			OnChangeCompleted.Raise(this);
		}
	}

	_type GetProxy(IAbstractValueProxy* proxy) const {
		if constexpr (std::is_polymorphic<_type>::value)
		{
			if (mRealType && mRealType->GetUsage() == Type::Usage::Object && TypeOf(_type) != *mRealType)
			{
				auto objectType = DynamicCast<const ObjectType*>(mRealType);
				void* typeSample = objectType->CreateSample();
				IObject* typeSampleObject = objectType->DynamicCastToIObject(typeSample);
				proxy->GetValuePtr(typeSample);
				_type res = *DynamicCast<_type*>(typeSampleObject);
				delete typeSampleObject;
				return res;
			}
		}

		return IPropertyField::GetProxy<_type>(proxy);
	}

	void SetProxy(IAbstractValueProxy* proxy, const _type& value) {
		IPropertyField::SetProxy<_type>(proxy, value);
	}

	Ref<Button> mRevertBtn;
	Ref<Button> mRemoveBtn;
	Ref<Label> mCaption;
	String mValuesPath;
	Ref<FieldInfo> mFieldInfo;
	Ref<PropertiesContext> mParentContext;
	bool mRevertable;
	Map<Ref<IAbstractValueProxy>, Ref<IAbstractValueProxy>> mValuesProxies;
	bool mValuesDifferent;
};

template<typename _type>
void TPropertyField<_type>::OnTypeSpecialized(const Type& newType) {
	IPropertyField::OnTypeSpecialized(newType);
	if (mValuesDifferent)
		mRevertBtn->SetEnabled(true);
}replace raw pointers with Ref<>, except void* pointers.Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

```cpp
URE(bool, IsValueRevertable);

FUNCTION().PROTECTED().SIGNATURE(void, OnValueChanged);
FUNCTION().PROTECTED().SIGNATURE(void, FreeValuesProxies);
FUNCTION().PROTECTED().SIGNATURE(void, BeginUserChanging);
FUNCTION().PROTECTED().SIGNATURE(void, EndUserChanging);
}
END_META;

META_TEMPLATES(typename _type)
CLASS_BASES_META(Editor::TPropertyField<_type>)
{
    BASE_CLASS(Editor::IPropertyField);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_FIELDS_META(Editor::TPropertyField<_type>)
{
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<_type>>()).NAME(mCommonValue);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRealType);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_METHODS_META(Editor::TPropertyField<_type>)
{
    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const TPropertyField&);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PUBLIC().SIGNATURE(void, Revert);
    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetValueType);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetValueTypeStatic);
    FUNCTION().PUBLIC().SIGNATURE(void, SetValue, const Ref<_type>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetUnknownValue, const Ref<_type>&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<_type>, GetCommonValue);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTypeSpecialized, const Type&);
    FUNCTION().PROTECTED().SIGNATURE(bool, IsValueRevertable);
    FUNCTION().PROTECTED().SIGNATURE(void, StoreValues, Vector<DataDocument>&);
    FUNCTION().PROTECTED().SIGNATURE(Ref<_type>, GetProxy, IAbstractValueProxy*);
    FUNCTION().PROTECTED().SIGNATURE(void, SetProxy, IAbstractValueProxy*, const Ref<_type>&);
    FUNCTION().PROTECTED().SIGNATURE(void, SetCommonValue, const Ref<_type>&);
    FUNCTION().PROTECTED().SIGNATURE(void, SetValueByUser, const Ref<_type>&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
    FUNCTION().PROTECTED().SIGNATURE(bool, IsAlwaysRefresh);
}
END_META;
// --- END META ---
```
