#pragma once

#include "Utils/Types/Containers/Vector.h"
#include "Utils/Serialization/DataNode.h"
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/ValueProxy.h"

using namespace o2;

namespace o2
{
	class UIWidget;
}

namespace Editor
{
	// -------------------------------
	// Editor property field interface
	// -------------------------------
	class IPropertyField: public IObject
	{
	public:
		typedef Pair<IAbstractValueProxy*, const IAbstractValueProxy*> TargetPair;
		typedef Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> TargetsVec;

	public:
		Function<void()> onChanged; // Immediate change value by user event

		Function<void(const String&, const Vector<DataNode>&, const Vector<DataNode>&)> onChangeCompleted; // Change completed by user event

		// Virtual destructor
		virtual ~IPropertyField() {}

		// Sets targets pointers
		virtual void SetValueAndPrototypeProxy(const TargetsVec& targets) {}

		// Sets targets proxies
		virtual void SetValueProxy(const Vector<IAbstractValueProxy*>& targets);

		// Checks common value and fill fields
		virtual void Refresh() {}

		// Reverts value to prototype value
		virtual void Revert() {}

		// Sets property caption
		virtual void SetCaption(const WString& text);

		// Returns property caption
		virtual WString GetCaption() const;

		// Returns control widget
		virtual UIWidget* GetWidget() const { return nullptr; }

		// Returns editing by this field type
		virtual const Type* GetFieldType() const { return &TypeOf(void); }

		// Sets reflection path f target values
		void SetValuePath(const String& path);

		// Returns reflection path of target values
		const String& GetValuePath() const;

		// Sets caption
		void SetCaptionLabel(UILabel* label);

		// Returns caption
		UILabel* GetCaptionLabel() const;

		// Specializes field type
		virtual void SpecializeType(const Type* type) {}
		
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
		// Checks value for reverting to prototype
		virtual void CheckRevertableState() {}

		// It is called when field value changed
		virtual void OnChanged();

		// Stores values to data
		virtual void StoreValues(Vector<DataNode>& data) const {}

	protected:
		UILabel*         mCaption = nullptr;  // Caption label, null by default   

		String           mValuesPath;         // Reflection path of target values
		Vector<DataNode> mBeforeChangeValues; // Serialized value data before changes started

	protected:
		// Sets value via proxy
		template<typename T>
		void SetProxy(IAbstractValueProxy* proxy, const T& value) const;

		// Returns value from proxy
		template<typename T>
		T GetProxy(IAbstractValueProxy* proxy) const;
	};

	// -----------------------------
	// IPropertyField implementation
	// -----------------------------

	template<typename T>
	T Editor::IPropertyField::GetProxy(IAbstractValueProxy* proxy) const
	{
		T res;
		proxy->GetValuePtr(&res);
		return res;
	}

	template<typename T>
	void Editor::IPropertyField::SetProxy(IAbstractValueProxy* proxy, const T& value) const
	{
		proxy->SetValuePtr(&const_cast<T&>(value));
	}


	// ----------------------------------
	// SelectPropertyMixin implementation
	// ----------------------------------

	template<typename _type>
	void IPropertyField::SetValuePointers(const Vector<_type*>& targets)
	{
		SetValueAndPrototypeProxy(targets.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>([](_type* target)
		{
			return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(mnew PointerValueProxy<_type>(target), nullptr);
		}));
	}

	template<typename _property_type>
	void IPropertyField::SetValuePropertyPointers(const Vector<_property_type*>& targets)
	{
		SetValueAndPrototypeProxy(targets.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>([](_property_type* target)
		{
			return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
				mnew PropertyValueProxy<_property_type::valueType, _property_type>(target), nullptr);
		}));
	}

	template<typename _type, typename _object_type>
	void IPropertyField::SelectValuesPointers(const Vector<_object_type*>& targets, std::function<_type*(_object_type*)> getter)
	{
		SetValueAndPrototypeProxy(targets.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[&](_object_type* target)
		{
			return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
				mnew PointerValueProxy<_type>(getter(target)), nullptr);
		}));
	}

	template<typename _object_type, typename _property_type>
	void IPropertyField::SelectValuesProperties(const Vector<_object_type*>& targets,
												std::function<_property_type*(_object_type*)> getter)
	{
		SetValueAndPrototypeProxy(targets.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[&](_object_type* target)
		{
			return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
				mnew PropertyValueProxy<_property_type::valueType, _property_type>(getter(target)), nullptr);
		}));
	}

	template<typename _type, typename _object_type>
	void IPropertyField::SelectValueAndPrototypePointers(const Vector<_object_type*>& targets,
														 const Vector<_object_type*>& prototypes, 
														 std::function<_type*(_object_type*)> getter)
	{
		Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> targetPairs(targets.Count());

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
														   std::function<_property_type*(_object_type*)> getter)
	{
		Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> targetPairs(targets.Count());

		for (int i = 0; i < targets.Count() && i < prototypes.Count(); i++)
		{
			targetPairs.Add(Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
				mnew PropertyValueProxy<_property_type::valueType, _property_type>(getter(targets[i])),
				prototypes[i] ? mnew PropertyValueProxy<_property_type::valueType, _property_type>(getter(prototypes[i])) : nullptr
			));
		}

		SetValueAndPrototypeProxy(targetPairs);
	}

}

CLASS_BASES_META(Editor::IPropertyField)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IPropertyField)
{
	PUBLIC_FIELD(onChanged);
	PUBLIC_FIELD(onChangeCompleted);
	PROTECTED_FIELD(mCaption);
	PROTECTED_FIELD(mValuesPath);
	PROTECTED_FIELD(mBeforeChangeValues);
}
END_META;
CLASS_METHODS_META(Editor::IPropertyField)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, SetValueProxy, const Vector<IAbstractValueProxy*>&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SetValuePath, const String&);
	PUBLIC_FUNCTION(const String&, GetValuePath);
	PUBLIC_FUNCTION(void, SetCaptionLabel, UILabel*);
	PUBLIC_FUNCTION(UILabel*, GetCaptionLabel);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnChanged);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
