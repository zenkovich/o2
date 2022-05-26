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
		Map<String, Vector<Pair<o2::ScriptValueProperty, o2::ScriptValueProperty>>> GetCommonProperties(const Vector<Pair<ScriptValue, ScriptValue>>& values) const;

		//Adds property by type
		void AddProperty(const String& name, const Type* type);

		// Sets property proxies
		template<typename _type>
		void SetFieldProxies(Map<String, Vector<Pair<o2::ScriptValueProperty, o2::ScriptValueProperty>>>& commonProperties,
							 const String& name, IPropertyField* field);

		// Sets property proxies
		template<typename _type>
		void SetFieldPtrProxies(Map<String, Vector<Pair<ScriptValueProperty, ScriptValueProperty>>>& commonProperties,
								const String& name, IPropertyField* field);

		// It is called when some property changed, sets value via proxy
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
							   const Vector<DataDocument>& after);
	};

	template<typename _type>
	void ScriptValueProperty::SetFieldProxies(Map<String, Vector<Pair<o2::ScriptValueProperty, o2::ScriptValueProperty>>>& commonProperties,
											  const String& name, IPropertyField* field)
	{
		auto proxies = commonProperties[name].Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[](const Pair<o2::ScriptValueProperty, o2::ScriptValueProperty>& x)
			{
				Pair<IAbstractValueProxy*, IAbstractValueProxy*> res;
				res.first = mnew TypeScriptValueProxy<_type>(x.first);
				if (x.second.object.IsObject())
					res.second = mnew TypeScriptValueProxy<_type>(x.second);

				return res;
			});

		field->SetValueAndPrototypeProxy(proxies);
	}

	template<typename _type>
	void ScriptValueProperty::SetFieldPtrProxies(Map<String, Vector<Pair<ScriptValueProperty, ScriptValueProperty>>>& commonProperties,
												 const String& name, IPropertyField* field)
	{
		auto proxies = commonProperties[name].Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[](const Pair<ScriptValueProperty, ScriptValueProperty>& x)
			{
				Pair<IAbstractValueProxy*, IAbstractValueProxy*> res;
				res.first = mnew PtrScriptValueProxy<_type>(x.first);
				if (x.second.object.IsObject())
					res.second = mnew PtrScriptValueProxy<_type>(x.second);

				return res;
			});

		field->SetValueAndPrototypeProxy(proxies);
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

	typedef Map<String, Vector<Pair<o2::ScriptValueProperty, o2::ScriptValueProperty>>> _tmp1;
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
