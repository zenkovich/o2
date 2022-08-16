#pragma once

#include "o2/Utils/Editor/Attributes/ScriptableAttribute.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	template<typename _type, typename _enable = void>
	struct CheckReflectValueOverridden
	{
		static void Process(_type* object, ScriptValue& value) {}
	};
	
	template<typename T>
	struct CheckReflectValueOverridden<T, typename std::void_t<decltype(&T::ReflectValue)>>
	{
		static void Process(T* object, ScriptValue& value)
		{
			if (object)
				object->ReflectValue(value);
		}
	};
	
	class ReflectScriptValueTypeProcessor : public BaseTypeProcessor
	{
	public:
		struct FieldProcessor;

		struct BaseFieldProcessor
		{
			ScriptValue& value;
			ProtectSection section;

		public:
			BaseFieldProcessor(ScriptValue& value) :value(value) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args);

			template<typename _type>
			BaseFieldProcessor& SetDefaultValue(const _type& value) { return *this; }

			BaseFieldProcessor& SetProtectSection(ProtectSection section)
			{
				this->section = section;
				return *this;
			}

			template<typename _object_type, typename _field_type>
			BaseFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void* (*pointerGetter)(void*),
											_field_type& field)
			{
				return *this;
			}
		};

		struct FieldProcessor : public BaseFieldProcessor
		{
		public:
			FieldProcessor(ScriptValue& value, ProtectSection section) :BaseFieldProcessor(value) { this->section = section; }

			template<typename _attribute_type, typename ... _args>
			FieldProcessor& AddAttribute(_args ... args) { return *this; }

			template<typename _type>
			FieldProcessor& SetDefaultValue(const _type& value) { return *this; }

			template<typename _object_type, typename _field_type>
			FieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void* (*pointerGetter)(void*),
										_field_type& field)
			{
				if (section != ProtectSection::Public)
					return *this;

				if constexpr (std::is_copy_constructible<_field_type>::value)
				{
					_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
					value.SetPropertyWrapper(ScriptValue(name), *fieldPtr);
				}

				return *this;
			}
		};

	public:
		ScriptValue& value;

	public:
		ReflectScriptValueTypeProcessor(ScriptValue& value) :value(value) {}

		template<typename _object_type>
		void Start(_object_type* object, Type* type)
		{
			CheckReflectValueOverridden<_object_type>::Process(object, value);
		}

		template<typename _object_type>
		void StartBases(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartFields(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartMethods(_object_type* object, Type* type) {}

		template<typename _object_type, typename _base_type>
		void BaseType(_object_type* object, Type* type, const char* name)
		{
			if constexpr (std::is_base_of<ISerializable, _base_type>::value && !std::is_same<ISerializable, _base_type>::value)
				_base_type::template ProcessType<ReflectScriptValueTypeProcessor>(object, *this);
		}

		BaseFieldProcessor StartField() { return BaseFieldProcessor(value); }
	};

	template<typename _attribute_type, typename ... _args>
	auto ReflectScriptValueTypeProcessor::BaseFieldProcessor::AddAttribute(_args ... args)
	{
		if constexpr (std::is_same<_attribute_type, ScriptableAttribute>::value)
			return FieldProcessor(value, section);
		else
			return *this;
	}
}
