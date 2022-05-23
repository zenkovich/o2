#pragma once

#include "o2/Utils/Reflection/Type.h"

#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)

namespace o2
{
	template<typename _type>
	IObject* ScriptValueBase::DataContainer<_type>::TryCastToIObject() const
	{
		if constexpr (std::is_base_of<IObject, _type>::value)
			return dynamic_cast<IObject*>(data);

		return nullptr;
	}

	template<typename _type>
	const Type* ScriptValueBase::DataContainer<_type>::GetType() const
	{
		return &TypeOf(_type);
	}


	template<typename _type>
	ScriptValue::ScriptValue(const _type& value)
	{
		Converter<_type>::Write(value, *this);
	}

	template<typename _type>
	ScriptValue::operator _type() const
	{
		_type res;
		Converter<_type>::Read(res, *this);
		return std::move(res);
	}

	template<typename _type>
	ScriptValue& ScriptValue::operator=(const _type& value)
	{
		SetValue(value);
		return *this;
	}

	template<typename _type>
	void ScriptValue::SetValue(const _type& value)
	{
		jerry_release_value(jvalue);
		Converter<_type>::Write(value, *this);
	}

	template<typename _type>
	_type ScriptValue::GetValue() const
	{
		_type res;
		Converter<_type>::Read(res, *this);
		return std::move(res);
	}

	template<typename ... _args>
	ScriptValue ScriptValue::Construct(_args ... args) const
	{
		Vector<ScriptValue> argsValues;

		if constexpr (sizeof...(_args) > 0)
			PackArgs(argsValues, args ...);

		return Construct(argsValues);
	}

	template<typename _type>
	void ScriptValue::SetProperty(const char* name, const _type& value)
	{
		SetProperty(ScriptValue(name), ScriptValue(value));
	}

	template<typename _type>
	void ScriptValue::SetPropertyWrapper(const ScriptValue& name, _type& value)
	{
		if (GetValueType() != ValueType::Object)
		{
			jerry_release_value(jvalue);
			jvalue = jerry_create_object();
		}

		jerry_property_descriptor_t propertyDescriptor;
		jerry_init_property_descriptor_fields(&propertyDescriptor);

		propertyDescriptor.is_enumerable = true;
		propertyDescriptor.is_enumerable_defined = true;

		propertyDescriptor.is_get_defined = true;
		propertyDescriptor.getter = jerry_create_external_function(DescriptorGetter);

		if constexpr (IsProperty<_type>::value)
		{
			auto getterWrapperContainer = new PropertyGetterWrapperContainer<_type>();
			getterWrapperContainer->propertyPtr = &value;
			jerry_set_object_native_pointer(propertyDescriptor.getter, getterWrapperContainer, &GetDataDeleter().info);
		}
		else
		{
			auto getterWrapperContainer = new PointerGetterWrapperContainer<_type>();
			getterWrapperContainer->dataPtr = &value;
			jerry_set_object_native_pointer(propertyDescriptor.getter, getterWrapperContainer, &GetDataDeleter().info);
		}

		propertyDescriptor.is_set_defined = true;
		propertyDescriptor.setter = jerry_create_external_function(DescriptorSetter);

		if constexpr (IsProperty<_type>::value)
		{
			auto setterWrapperContainer = new PropertySetterWrapperContainer<_type>();
			setterWrapperContainer->propertyPtr = &value;
			jerry_set_object_native_pointer(propertyDescriptor.setter, setterWrapperContainer, &GetDataDeleter().info);
		}
		else
		{
			auto setterWrapperContainer = new PointerSetterWrapperContainer<_type>();
			setterWrapperContainer->dataPtr = &value;
			jerry_set_object_native_pointer(propertyDescriptor.setter, setterWrapperContainer, &GetDataDeleter().info);
		}

		jerry_value_t newPropertyValue = jerry_define_own_property(jvalue, name.jvalue, &propertyDescriptor);
		jerry_release_value(newPropertyValue);

		jerry_free_property_descriptor_fields(&propertyDescriptor);
	}

	template<typename _type>
	void ScriptValue::SetPropertyWrapper(const ScriptValue& name, const Function<void(const _type& value)>& setter,
										 const Function<_type()>& getter)
	{
		if (GetValueType() != ValueType::Object)
		{
			jerry_release_value(jvalue);
			jvalue = jerry_create_object();
		}

		jerry_property_descriptor_t propertyDescriptor;
		jerry_init_property_descriptor_fields(&propertyDescriptor);

		propertyDescriptor.is_enumerable = true;
		propertyDescriptor.is_enumerable_defined = true;

		propertyDescriptor.is_get_defined = true;
		propertyDescriptor.getter = jerry_create_external_function(DescriptorGetter);
		auto getterWrapperContainer = new FunctionalGetterWrapperContainer<_type>();
		getterWrapperContainer->getter = getter;
		jerry_set_object_native_pointer(propertyDescriptor.getter, getterWrapperContainer, &GetDataDeleter().info);

		propertyDescriptor.is_set_defined = true;
		propertyDescriptor.setter = jerry_create_external_function(DescriptorSetter);
		auto setterWrapperContainer = new FunctionalSetterWrapperContainer<_type>();
		setterWrapperContainer->setter = setter;
		jerry_set_object_native_pointer(propertyDescriptor.setter, setterWrapperContainer, &GetDataDeleter().info);

		jerry_value_t newPropertyValue = jerry_define_own_property(jvalue, name.jvalue, &propertyDescriptor);
		jerry_release_value(newPropertyValue);

		jerry_free_property_descriptor_fields(&propertyDescriptor);
	}

	template<typename _res_type, typename ... _args>
	_res_type ScriptValue::Invoke(_args ... args) const
	{
		return Invoke<_res_type, _args ...>(ScriptValue(), args ...);
	}

	template<typename _res_type, typename ... _args>
	_res_type ScriptValue::Invoke(const ScriptValue& thisValue, _args ... args) const
	{
		Vector<ScriptValue> argsValues;

		if constexpr (sizeof...(_args) > 0)
			PackArgs(argsValues, args ...);

		if constexpr (std::is_same<_res_type, void>::value)
			InvokeRaw(thisValue, argsValues);
		else
			return InvokeRaw(thisValue, argsValues).GetValue<_res_type>();
	}

	template<typename _invocable_type, typename _res_type, typename ... _args>
	jerry_value_t ScriptValueBase::FunctionContainer<_invocable_type, _res_type, _args ...>::Invoke(jerry_value_t thisValue, jerry_value_t* args, int argsCount)
	{
		if constexpr (sizeof...(_args) > 0)
		{
			std::tuple<typename std::remove_const<typename std::remove_reference<_args>::type>::type...> argst;
			UnpackArgs(argst, args, argsCount);

			if constexpr (std::is_same<_res_type, void>::value)
			{
				std::apply(*data, argst);
				return jerry_create_undefined();
			}
			else
			{
				ScriptValue res(std::apply(*data, argst));
				return jerry_acquire_value(res.jvalue);
			}
		}
		else
		{
			if constexpr (std::is_same<_res_type, void>::value)
			{
				(*data)();
				return jerry_create_undefined();
			}
			else
			{
				ScriptValue res((*data)());
				return jerry_acquire_value(res.jvalue);
			}
		}
	}

	template<typename _type>
	jerry_value_t ScriptValueBase::PointerGetterWrapperContainer<_type>::Get()
	{
		ScriptValue tmp;
		tmp.SetValue<_type>(*dataPtr);
		return jerry_acquire_value(tmp.jvalue);
	}

	template<typename _type>
	void ScriptValueBase::PointerSetterWrapperContainer<_type>::Set(jerry_value_t value)
	{
		ScriptValue tmp;
		tmp.AcquireValue(value);
		*dataPtr = tmp.GetValue<_type>();
	}

	template<typename _property_type>
	jerry_value_t ScriptValueBase::PropertyGetterWrapperContainer<_property_type>::Get()
	{
		ScriptValue tmp;
		tmp.SetValue<ExtractPropertyValueType<_property_type>::type>(propertyPtr->Get());
		return jerry_acquire_value(tmp.jvalue);
	}

	template<typename _property_type>
	void ScriptValueBase::PropertySetterWrapperContainer<_property_type>::Set(jerry_value_t value)
	{
		ScriptValue tmp;
		tmp.AcquireValue(value);
		propertyPtr->Set(tmp.GetValue<ExtractPropertyValueType<_property_type>::type>());
	}

	template<typename _type>
	jerry_value_t ScriptValueBase::FunctionalGetterWrapperContainer<_type>::Get()
	{
		ScriptValue tmp;
		tmp.SetValue<_type>(getter());
		return jerry_acquire_value(tmp.jvalue);
	}

	template<typename _type>
	void ScriptValueBase::FunctionalSetterWrapperContainer<_type>::Set(jerry_value_t value)
	{
		ScriptValue tmp;
		tmp.AcquireValue(value);
		setter(tmp.GetValue<_type>());
	}

	class Type;

	struct ScriptConstructorTypeProcessor: public BaseTypeProcessor
	{
		struct BaseFunctionProcessor: public BaseTypeProcessor::FunctionProcessor
		{
			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args);

			BaseFunctionProcessor& SetProtectSection(ProtectSection section) { return *this; }
		};

		struct FunctionProcessor: public BaseFunctionProcessor
		{
			template<typename _object_type, typename ... _args>
			void Constructor(_object_type* object, Type* type);

			template<typename _object_type, typename _res_type, typename ... _args>
			void SignatureStatic(_object_type* object, Type* type, const char* name,
								 _res_type(*pointer)(_args ...));
		};

		BaseFunctionProcessor StartFunction() { return BaseFunctionProcessor(); }

		static void RegisterTypeConstructor(Type* type, const ScriptValue& constructorFunc);
		static void RegisterTypeStaticFunction(Type* type, const char* name, const ScriptValue& func);
	};

	template<typename _attribute_type, typename ... _args>
	auto ScriptConstructorTypeProcessor::BaseFunctionProcessor::AddAttribute(_args ... args)
	{
		if constexpr (std::is_same<ScriptableAttribute, _attribute_type>::value)
			return ScriptConstructorTypeProcessor::FunctionProcessor();
		else
			return *this;
	}

	template<typename _object_type, typename ... _args>
	void ScriptConstructorTypeProcessor::FunctionProcessor::Constructor(_object_type* object, Type* type)
	{
		ScriptConstructorTypeProcessor::RegisterTypeConstructor(type, ScriptValue(Function<ScriptValue(_args ...)>([](_args ... args) {
			_object_type* sample = mnew _object_type(args ...);
			auto res = sample->GetScriptValue();
			res.SetObjectOwnership(true);
			return res;
		})));
	}

	template<typename _object_type, typename _res_type, typename ... _args>
	void ScriptConstructorTypeProcessor::FunctionProcessor::SignatureStatic(_object_type* object, Type* type, 
																			const char* name, _res_type(*pointer)(_args ...))
	{
		ScriptConstructorTypeProcessor::RegisterTypeStaticFunction(type, name, ScriptValue(Function<_res_type(_args ...)>([=](_args ... args) {
			return (*pointer)(args ...);
		})));
	}

}

#endif // SCRIPTING_BACKEND_JERRYSCRIPT
