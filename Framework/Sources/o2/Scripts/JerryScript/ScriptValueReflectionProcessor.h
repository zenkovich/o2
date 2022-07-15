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
	
	class ReflectScriptValueTypeProcessor
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

		struct BaseFunctionProcessor
		{
			ScriptValue& value;
			ProtectSection section;

		public:
			BaseFunctionProcessor(ScriptValue& value) :value(value) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args);

			BaseFunctionProcessor& SetProtectSection(ProtectSection section)
			{
				this->section = section;
				return *this;
			}

			template<typename _object_type, typename ... _args>
			void Constructor(_object_type* object, Type* type) {}

			template<typename _object_type, typename _res_type, typename ... _args>
			void Signature(_object_type* object, Type* type, const char* name,
						   _res_type(_object_type::* pointer)(_args ...)) {}

			template<typename _object_type, typename _res_type, typename ... _args>
			void Signature(_object_type* object, Type* type, const char* name,
						   _res_type(_object_type::* pointer)(_args ...) const) {}

			template<typename _object_type, typename _res_type, typename ... _args>
			void SignatureStatic(_object_type* object, Type* type, const char* name,
								 _res_type(*pointer)(_args ...)) {}
		};

		struct FunctionProcessor : public BaseFunctionProcessor
		{
		public:
			const char* name = 0;

		public:
			FunctionProcessor(BaseFunctionProcessor& base, const char* name) :BaseFunctionProcessor(base), name(name) {}

			template<typename _object_type, typename _res_type, typename ... _args>
			void Signature(_object_type* object, Type* type, const char* name,
						   _res_type(_object_type::* pointer)(_args ...))
			{
				if constexpr (std::is_same<void, _res_type>::value)
					value.SetProperty(ScriptValue(this->name ? this->name : name), ScriptValue(Function<_res_type(_args ...)>(object, pointer)));
				else
				{
					using x = typename std::remove_reference<_res_type>::type;
					typedef typename std::remove_const<x>::type __res_type;
					value.SetProperty(ScriptValue(this->name ? this->name : name),
									  ScriptValue(Function<__res_type(_args ...)>([=](_args ... args)
																				  {
																					  __res_type res = (object->*pointer)(args ...);
																					  return res;
																				  })));
				}
			}

			template<typename _object_type, typename _res_type, typename ... _args>
			void Signature(_object_type* object, Type* type, const char* name,
						   _res_type(_object_type::* pointer)(_args ...) const)
			{
				if constexpr (std::is_same<void, _res_type>::value)
					value.SetProperty(ScriptValue(name), ScriptValue(Function<_res_type(_args ...)>(object, pointer)));
				else
				{
					using x = typename std::remove_reference<_res_type>::type;
					using __res_type = typename std::remove_const<x>::type;
					value.SetProperty(ScriptValue(name),
									  ScriptValue(Function<__res_type(_args ...)>([=](_args ... args)
																				  {
																					  __res_type res = (object->*pointer)(args ...);
																					  return res;
																				  })));
				}
			}

			template<typename _object_type, typename _res_type, typename ... _args>
			void SignatureStatic(_object_type* object, Type* type, const char* name,
								 _res_type(*pointer)(_args ...)) {}
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

		BaseFunctionProcessor StartFunction() { return BaseFunctionProcessor(value); }
	};

	template<typename _attribute_type, typename ... _args>
	auto ReflectScriptValueTypeProcessor::BaseFieldProcessor::AddAttribute(_args ... args)
	{
		if constexpr (std::is_same<_attribute_type, ScriptableAttribute>::value)
			return FieldProcessor(value, section);
		else
			return *this;
	}

	template<typename _attribute_type, typename ... _args>
	auto ReflectScriptValueTypeProcessor::BaseFunctionProcessor::AddAttribute(_args ... args)
	{
		if constexpr (std::is_same<_attribute_type, ScriptableAttribute>::value)
			return FunctionProcessor(*this, 0);
		else if constexpr (std::is_same<_attribute_type, ScriptableNameAttribute>::value)
			return FunctionProcessor(*this, args ...);
		else
			return *this;
	}
}
