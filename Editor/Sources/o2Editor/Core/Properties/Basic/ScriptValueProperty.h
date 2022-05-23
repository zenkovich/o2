#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

using namespace o2;

namespace o2
{
	class Spoiler;
}

namespace Editor
{
	// --------------------
	// ScriptValue property
	// --------------------
	class ScriptValueProperty : public IPropertyField
	{
	public:
		// Default constructor
		ScriptValueProperty();

		// Copy constructor
		ScriptValueProperty(const ScriptValueProperty& other);

		// Copy operator
		ScriptValueProperty& operator=(const ScriptValueProperty& other);

		// Sets targets proxies
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		// Specializes field info, processing attributes
		void SetFieldInfo(const FieldInfo* fieldInfo) override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Adds remove button
		Button* GetRemoveButton() override;

		// Expands property fields
		void Expand();

		// Collapses property field
		void Collapse();

		// Sets properties expanding
		void SetExpanded(bool expanded);

		// Returns is properties expanded
		bool IsExpanded() const;

		// Enables or disables spoiler header
		void SetHeaderEnabled(bool enabled);

		// Returns is spoiler header enabled
		bool IsHeaderEnabled() const;

		IOBJECT(ScriptValueProperty);

	protected:
		struct Property
		{
			ScriptValue object;
			ScriptValue name;

			ScriptValue Get() const;
			void Set(const ScriptValue& value);

			bool operator==(const Property& other) const;
		};

		template<typename _type>
		class ScriptValueProxy : public IValueProxy<_type>, public Property
		{
		public:
			ScriptValueProxy();
			ScriptValueProxy(const Property& prop);

			void SetValue(const _type& value) override;
			_type GetValue() const override;
		};

		class SimpleScriptValueProxy : public IAbstractValueProxy, public Property
		{
		public:
			SimpleScriptValueProxy();
			SimpleScriptValueProxy(const Property& prop);

			void SetValuePtr(void* value) override;
			void GetValuePtr(void* value) const override;
			const Type& GetType() const override;
		};

		template<typename _type>
		class PtrScriptValueProxy : public IValueProxy<_type>, public Property
		{
		public:
			Property prop;

		public:
			PtrScriptValueProxy();
			PtrScriptValueProxy(const Property& prop);

			void SetValue(const _type& value) override;
			_type GetValue() const override;

			const Type& GetType() const override;
		};

		Spoiler* mSpoiler = nullptr; // Properties spoiler. Expands forcible when viewer hasn't header

		Map<String, ScriptValue::ValueType> mPreviousBuiltTypes; // Built types of fields, used to check 
																 // changed properties structure
		Map<String, IPropertyField*>        mBuiltProperties;    // Built properties by names

		bool mHeaderEnabled = false; // Is no header attribute exists
		bool mExpanded = false; // True when must be expanded after creating object viewer

		bool mNeedUpdateProxies = false; // True when targets changed and required to refresh script values proxies

	protected:
		// It is called when property puts in buffer. Releases properties
		void OnFreeProperty() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Returns mapped common properties
		Map<String, Vector<Pair<Property, Property>>> GetCommonProperties(const Vector<Pair<ScriptValue, ScriptValue>>& values) const;

		//Adds property by type
		void AddProperty(const String& name, const Type* type);

		// Sets property proxies
		template<typename _type>
		void SetFieldProxies(Map<String, Vector<Pair<ScriptValueProperty::Property, ScriptValueProperty::Property>>>& commonProperties,
							 const String& name, IPropertyField* field);

		// Sets property proxies
		template<typename _type>
		void SetFieldPtrProxies(Map<String, Vector<Pair<ScriptValueProperty::Property, ScriptValueProperty::Property>>>& commonProperties,
								const String& name, IPropertyField* field);

		// It is called when some property changed, sets value via proxy
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
							   const Vector<DataDocument>& after);
	};

	template<typename _type>
	ScriptValueProperty::ScriptValueProxy<_type>::ScriptValueProxy(const Property& prop) :Property(prop)
	{}

	template<typename _type>
	ScriptValueProperty::ScriptValueProxy<_type>::ScriptValueProxy()
	{}

	template<typename _type>
	void ScriptValueProperty::ScriptValueProxy<_type>::SetValue(const _type& value)
	{
		Set(ScriptValue(value));
	}

	template<typename _type>
	_type ScriptValueProperty::ScriptValueProxy<_type>::GetValue() const
	{
		return Get().GetValue<_type>();
	}

	template<typename _type>
	void ScriptValueProperty::SetFieldProxies(Map<String, Vector<Pair<Property, Property>>>& commonProperties,
											  const String& name, IPropertyField* field)
	{
		auto proxies = commonProperties[name].Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[](const Pair<Property, Property>& x)
			{
				Pair<IAbstractValueProxy*, IAbstractValueProxy*> res;
				res.first = mnew ScriptValueProxy<_type>(x.first);
				if (x.second.object.IsObject())
					res.second = mnew ScriptValueProxy<_type>(x.second);

				return res;
			});

		field->SetValueAndPrototypeProxy(proxies);
	}

	template<typename _type>
	void ScriptValueProperty::SetFieldPtrProxies(Map<String, Vector<Pair<Property, Property>>>& commonProperties,
												 const String& name, IPropertyField* field)
	{
		auto proxies = commonProperties[name].Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[](const Pair<Property, Property>& x)
			{
				Pair<IAbstractValueProxy*, IAbstractValueProxy*> res;
				res.first = mnew PtrScriptValueProxy<_type>(x.first);
				if (x.second.object.IsObject())
					res.second = mnew PtrScriptValueProxy<_type>(x.second);

				return res;
			});

		field->SetValueAndPrototypeProxy(proxies);
	}

	template<typename _type>
	ScriptValueProperty::PtrScriptValueProxy<_type>::PtrScriptValueProxy(const Property& prop) :Property(prop)
	{}

	template<typename _type>
	ScriptValueProperty::PtrScriptValueProxy<_type>::PtrScriptValueProxy()
	{}

	template<typename _type>
	void ScriptValueProperty::PtrScriptValueProxy<_type>::SetValue(const _type& value)
	{
		Set(ScriptValue(mnew _type(value)));
	}

	template<typename _type>
	_type ScriptValueProperty::PtrScriptValueProxy<_type>::GetValue() const
	{
		return *Get().GetValue<_type*>();
	}

	template<typename _type>
	const Type& ScriptValueProperty::PtrScriptValueProxy<_type>::GetType() const
	{
		if constexpr (std::is_base_of<IObject, _type>::value)
			return Get().GetValue<_type*>()->GetType();
		else
			return TypeOf(_type);
	}
}

CLASS_BASES_META(Editor::ScriptValueProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::ScriptValueProperty)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSpoiler);
	FIELD().PROTECTED().NAME(mPreviousBuiltTypes);
	FIELD().PROTECTED().NAME(mBuiltProperties);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mHeaderEnabled);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mExpanded);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mNeedUpdateProxies);
}
END_META;
CLASS_METHODS_META(Editor::ScriptValueProperty)
{

	typedef Map<String, Vector<Pair<Property, Property>>> _tmp1;
	typedef const Vector<Pair<ScriptValue, ScriptValue>>& _tmp2;

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const ScriptValueProperty&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueAndPrototypeProxy, const TargetsVec&);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetValueType);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetValueTypeStatic);
	FUNCTION().PUBLIC().SIGNATURE(void, SetFieldInfo, const FieldInfo*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCaption, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(WString, GetCaption);
	FUNCTION().PUBLIC().SIGNATURE(Button*, GetRemoveButton);
	FUNCTION().PUBLIC().SIGNATURE(void, Expand);
	FUNCTION().PUBLIC().SIGNATURE(void, Collapse);
	FUNCTION().PUBLIC().SIGNATURE(void, SetExpanded, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsExpanded);
	FUNCTION().PUBLIC().SIGNATURE(void, SetHeaderEnabled, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsHeaderEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnFreeProperty);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(_tmp1, GetCommonProperties, _tmp2);
	FUNCTION().PROTECTED().SIGNATURE(void, AddProperty, const String&, const Type*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;
