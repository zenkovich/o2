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
		typedef Pair<IAbstractValueProxy*, IAbstractValueProxy*> TargetPair;
		typedef Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> TargetsVec;

		typedef Function<void(IPropertyField*)> OnChangedFunc;
		typedef Function<void(const String&, const Vector<DataDocument>&, const Vector<DataDocument>&)> OnChangeCompletedFunc;

	public:
		OnChangedFunc         onChanged;         // Immediate change value by user event
		OnChangeCompletedFunc onChangeCompleted; // Change completed by user event

	public:
		// Default constructor. Searches revert button and sets click action
		IPropertyField();

		// Copy-constructor
		IPropertyField(const IPropertyField& other);

		// Virtual destructor
		virtual ~IPropertyField() {}

		// Copy-operator
		IPropertyField& operator=(const IPropertyField& other);

		// Sets targets proxies
		virtual void SetValueAndPrototypeProxy(const TargetsVec& targets);

		// Returns targets proxies
		const TargetsVec& GetValueAndPrototypeProxy() const;

		// Sets targets proxies
		virtual void SetValueProxy(const Vector<IAbstractValueProxy*>& targets);

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
											   std::function<_property_type*(_object_type*)> getter);

		IOBJECT(IPropertyField);

	protected:
		const FieldInfo* mFieldInfo = nullptr; // Specialized field info

		PropertiesContext* mParentContext = nullptr; // Parent context

		bool mRevertable = true; // Is property can be reverted

		TargetsVec mValuesProxies;          // Target values proxies
		bool       mValuesDifferent = true; // Are values different

		Button* mRevertBtn = nullptr; // Revert to source prototype button
		Button* mRemoveBtn = nullptr; // Remove from array button
		Label*  mCaption = nullptr;   // Caption label, null by default   

		String               mValuesPath;         // Reflection path of target values
		Vector<DataDocument> mBeforeChangeValues; // Serialized value data before changes started

	protected:
		// It is called when type specialized during setting value proxy
		virtual void OnTypeSpecialized(const Type& type) {}

		// It is called when property puts in buffer. Here you can release your shared resources
		virtual void OnFreeProperty();

		// Stores values to data
		virtual void StoreValues(Vector<DataDocument>& data) const {}

		// Checks that value was changed and calls onChangeCompleted
		virtual void CheckValueChangeCompleted();

		// Checks value for reverting to prototype and sets widget state "revert"
		virtual void CheckRevertableState();

		// Checks is value can be reverted
		virtual bool IsValueRevertable() const;

		// It is called when field value changed
		virtual void OnValueChanged();

		// It is called when user began to change value and we need to store initial value data
		void BeginUserChanging();

		// It is called when user completed changing value and we need to compare current value with previous and call onChangeCompleted
		void EndUserChanging();

		// Sets value via proxy
		template<typename T>
		void SetProxy(IAbstractValueProxy* proxy, const T& value) const;

		// Returns value from proxy
		template<typename T>
		T GetProxy(IAbstractValueProxy* proxy) const;

		friend class Properties;
	};

	// Returns editing by this field type by static function, can't be changed during runtime
	static const Type* GetValueTypeStatic();

	template<typename _type>
	class TPropertyField : public IPropertyField
	{
	public:
		// Default constructor
		TPropertyField();

		// Copy-constructor
		TPropertyField(const TPropertyField& other);

		// COpy operator
		TPropertyField& operator=(const TPropertyField& other);

		// Checks common value and fill fields
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		// Sets value
		void SetValue(const _type& value);

		// Sets value as unknown
		void SetUnknownValue(const _type& defaultValue = _type());

		// Returns value
		_type GetCommonValue() const;

		IOBJECT(TPropertyField<_type>);

	protected:
		_type mCommonValue = _type();

	protected:
		// Checks is value can be reverted
		bool IsValueRevertable() const override;

		// Stores values to data
		void StoreValues(Vector<DataDocument>& data) const override;

		// Returns value from proxy
		virtual _type GetProxy(IAbstractValueProxy* proxy) const;

		// Sets value to proxy
		virtual void SetProxy(IAbstractValueProxy* proxy, const _type& value);

		// Sets common value
		virtual void SetCommonValue(const _type& value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetValueByUser(const _type& value);

		// Updates value view
		virtual void UpdateValueView() {}

		// Is required refresh view every time
		virtual bool IsAlwaysRefresh() const;
	};

	// -----------------------------
	// IPropertyField implementation
	// -----------------------------

	template<typename T>
	T IPropertyField::GetProxy(IAbstractValueProxy* proxy) const
	{
		T res;
		proxy->GetValuePtr(&res);
		return res;
	}

	template<typename T>
	void IPropertyField::SetProxy(IAbstractValueProxy* proxy, const T& value) const
	{
		proxy->SetValuePtr(&const_cast<T&>(value));
	}


	// ----------------------------------
	// SelectPropertyMixin implementation
	// ----------------------------------

	template<typename _type>
	void IPropertyField::SetValuePointers(const Vector<_type*>& targets)
	{
		SetValueAndPrototypeProxy(targets.Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>([](_type* target)
		{
			return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(mnew PointerValueProxy<_type>(target), nullptr);
		}));
	}

	template<typename _property_type>
	void IPropertyField::SetValuePropertyPointers(const Vector<_property_type*>& targets)
	{
		SetValueAndPrototypeProxy(targets.Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>([](_property_type* target)
		{
			return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
				mnew PropertyValueProxy<_property_type::valueType, _property_type>(target), nullptr);
		}));
	}

	template<typename _type, typename _object_type>
	void IPropertyField::SelectValuesPointers(const Vector<_object_type*>& targets, std::function<_type* (_object_type*)> getter)
	{
		SetValueAndPrototypeProxy(targets.Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[&](_object_type* target)
		{
			return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
				TypeOf(_type).GetValueProxy(getter(target)), nullptr);
		}));
	}

	template<typename _object_type, typename _property_type>
	void IPropertyField::SelectValuesProperties(const Vector<_object_type*>& targets,
		std::function<_property_type* (_object_type*)> getter)
	{
		SetValueAndPrototypeProxy(targets.Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[&](_object_type* target)
		{
			return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
				mnew PropertyValueProxy<_property_type::valueType, _property_type>(getter(target)), nullptr);
		}));
	}

	template<typename _type, typename _object_type>
	void IPropertyField::SelectValueAndPrototypePointers(const Vector<_object_type*>& targets,
		const Vector<_object_type*>& prototypes,
		std::function<_type* (_object_type*)> getter)
	{
		Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> targetPairs;
		targetPairs.Reserve(targets.Count());

		for (int i = 0; i < targets.Count() && i < prototypes.Count(); i++)
		{
			targetPairs.Add(Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
				mnew PointerValueProxy<_type>(getter(targets[i])),
				prototypes[i] ? mnew PointerValueProxy<_type>(getter(prototypes[i])) : nullptr
				));
		}

		SetValueAndPrototypeProxy(targetPairs);
	}

	template<typename _object_type, typename _property_type>
	void IPropertyField::SelectValueAndPrototypeProperties(const Vector<_object_type*>& targets,
		const Vector<_object_type*>& prototypes,
		std::function<_property_type* (_object_type*)> getter)
	{
		Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> targetPairs;
		targetPairs.Reserve(targets.Count());

		for (int i = 0; i < targets.Count() && i < prototypes.Count(); i++)
		{
			targetPairs.Add(Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
				mnew PropertyValueProxy<_property_type::valueType, _property_type>(getter(targets[i])),
				prototypes[i] ? mnew PropertyValueProxy<_property_type::valueType, _property_type>(getter(prototypes[i])) : nullptr
				));
		}

		SetValueAndPrototypeProxy(targetPairs);
	}

	// -----------------------------
	// TPropertyField implementation
	// -----------------------------

	template<typename _type>
	TPropertyField<_type>::TPropertyField()
	{}

	template<typename _type>
	TPropertyField<_type>::TPropertyField(const TPropertyField& other) :
		IPropertyField(other)
	{}

	template<typename _type>
	TPropertyField<_type>& Editor::TPropertyField<_type>::operator=(const TPropertyField& other)
	{
		IPropertyField::operator=(other);
		return *this;
	}

	template<typename _type>
	void Editor::TPropertyField<_type>::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		bool lastDifferent = mValuesDifferent;

		_type newCommonValue = GetProxy(mValuesProxies[0].first);

		bool newDifferent = false;

		for (int i = 1; i < mValuesProxies.Count(); i++)
		{
			_type value; mValuesProxies[i].first->GetValuePtr(&value);
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

		UpdateValueView();
		OnValueChanged();
	}

	template<typename _type>
	void TPropertyField<_type>::StoreValues(Vector<DataDocument>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesProxies)
		{
			data.Add(DataDocument());
			data.Last() = GetProxy(ptr.first);
		}
	}

	template<typename _type>
	bool TPropertyField<_type>::IsValueRevertable() const
	{
		for (auto ptr : mValuesProxies)
		{
			if (ptr.second && !Math::Equals(GetProxy(ptr.first), GetProxy(ptr.second)))
				return true;
		}

		return false;
	}

	template<typename _type>
	_type TPropertyField<_type>::GetCommonValue() const
	{
		return mCommonValue;
	}

	template<typename _type>
	void TPropertyField<_type>::SetUnknownValue(const _type& defaultValue /*= _type()*/)
	{
		mCommonValue = defaultValue;
		mValuesDifferent = true;

		UpdateValueView();
		OnValueChanged();
	}

	template<typename _type>
	void TPropertyField<_type>::SetValue(const _type& value)
	{
		for (auto ptr : mValuesProxies)
			SetProxy(ptr.first, value);

		SetCommonValue(value);
	}

	template<typename _type>
	const Type* TPropertyField<_type>::GetValueType() const
	{
		return GetValueTypeStatic();
	}

	template<typename _type>
	const Type* TPropertyField<_type>::GetValueTypeStatic()
	{
		return &TypeOf(_type);
	}

	template<typename _type>
	void TPropertyField<_type>::Revert()
	{
		for (auto ptr : mValuesProxies)
		{
			if (ptr.second)
				SetProxy(ptr.first, GetProxy(ptr.second));
		}

		Refresh();
	}

	template<typename _type>
	_type TPropertyField<_type>::GetProxy(IAbstractValueProxy* proxy) const
	{
		return IPropertyField::GetProxy<_type>(proxy);
	}

	template<typename _type>
	void TPropertyField<_type>::SetProxy(IAbstractValueProxy* proxy, const _type& value)
	{
		IPropertyField::SetProxy<_type>(proxy, value);
	}

}

CLASS_BASES_META(Editor::IPropertyField)
{
	BASE_CLASS(o2::HorizontalLayout);
}
END_META;
CLASS_FIELDS_META(Editor::IPropertyField)
{
	PUBLIC_FIELD(onChanged);
	PUBLIC_FIELD(onChangeCompleted);
	PROTECTED_FIELD(mFieldInfo).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mParentContext).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mRevertable).DEFAULT_VALUE(true);
	PROTECTED_FIELD(mValuesProxies);
	PROTECTED_FIELD(mValuesDifferent).DEFAULT_VALUE(true);
	PROTECTED_FIELD(mRevertBtn).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mRemoveBtn).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mCaption).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mValuesPath);
	PROTECTED_FIELD(mBeforeChangeValues);
}
END_META;
CLASS_METHODS_META(Editor::IPropertyField)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(const TargetsVec&, GetValueAndPrototypeProxy);
	PUBLIC_FUNCTION(void, SetValueProxy, const Vector<IAbstractValueProxy*>&);
	PUBLIC_FUNCTION(void, SetParentContext, PropertiesContext*);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(Button*, GetRemoveButton);
	PUBLIC_FUNCTION(const Type*, GetValueType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetValueTypeStatic);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValuePath, const String&);
	PUBLIC_FUNCTION(const String&, GetValuePath);
	PUBLIC_FUNCTION(void, SetCaptionLabel, Label*);
	PUBLIC_FUNCTION(Label*, GetCaptionLabel);
	PUBLIC_FUNCTION(void, SetRevertable, bool);
	PUBLIC_FUNCTION(bool, IsRevertable);
	PUBLIC_FUNCTION(void, SetFieldInfo, const FieldInfo*);
	PROTECTED_FUNCTION(void, OnTypeSpecialized, const Type&);
	PROTECTED_FUNCTION(void, OnFreeProperty);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataDocument>&);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(bool, IsValueRevertable);
	PROTECTED_FUNCTION(void, OnValueChanged);
	PROTECTED_FUNCTION(void, BeginUserChanging);
	PROTECTED_FUNCTION(void, EndUserChanging);
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
	PROTECTED_FIELD(mCommonValue).DEFAULT_VALUE(_type());
}
END_META;
META_TEMPLATES(typename _type)
CLASS_METHODS_META(Editor::TPropertyField<_type>)
{

	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(const Type*, GetValueType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetValueTypeStatic);
	PUBLIC_FUNCTION(void, SetValue, const _type&);
	PUBLIC_FUNCTION(void, SetUnknownValue, const _type&);
	PUBLIC_FUNCTION(_type, GetCommonValue);
	PROTECTED_FUNCTION(bool, IsValueRevertable);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataDocument>&);
	PROTECTED_FUNCTION(_type, GetProxy, IAbstractValueProxy*);
	PROTECTED_FUNCTION(void, SetProxy, IAbstractValueProxy*, const _type&);
	PROTECTED_FUNCTION(void, SetCommonValue, const _type&);
	PROTECTED_FUNCTION(void, SetValueByUser, const _type&);
	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(bool, IsAlwaysRefresh);
}
END_META;
