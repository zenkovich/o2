#pragma once

#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)

namespace o2
{
	class ReflectScriptValueTypeProcessor
	{
	public:
		struct FieldProcessor
		{
			ScriptValue& value;

		public:
			FieldProcessor(ScriptValue& value):value(value) {}

			template<typename _attribute_type, typename ... _args>
			FieldProcessor& AddAttribute(_args ... args) { return *this; }

			template<typename _type>
			FieldProcessor& SetDefaultValue(const _type& value) { return *this; }

			void SetProtectSection(ProtectSection section) {}

			template<typename _object_type, typename _field_type>
			FieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void* (*pointerGetter)(void*),
										_field_type& field)
			{
				if constexpr (std::is_copy_constructible<_field_type>::value)
				{
					_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
					ScriptValue prop;
					prop.SetValue(*fieldPtr);
					value.SetProperty(ScriptValue(name), prop);
				}

				return *this;
			}
		};

	public:
		ScriptValue& value;

	public:
		ReflectScriptValueTypeProcessor(ScriptValue& value):value(value) {}

		template<typename _object_type>
		void Start(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartBases(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartFields(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartMethods(_object_type* object, Type* type) {}

		FieldProcessor StartField() { return FieldProcessor(value); }

		template<typename _object_type, typename _base_type>
		void BaseType(_object_type* object, Type* type, const char* name)
		{
			if constexpr (std::is_base_of<ISerializable, _base_type>::value && !std::is_same<ISerializable, _base_type>::value)
				_base_type::ProcessType<ReflectScriptValueTypeProcessor>(object, *this);
		}

		template<typename _object_type, typename _res_type, typename ... _args>
		void Method(_object_type* object, Type* type, const char* name,
					_res_type(_object_type::* pointer)(_args ...), ProtectSection protection)
		{

		}

		template<typename _object_type, typename _res_type, typename ... _args>
		void Method(_object_type* object, Type* type, const char* name,
					_res_type(_object_type::* pointer)(_args ...) const, ProtectSection protection)
		{
		}

		template<typename _object_type, typename _res_type, typename ... _args>
		void StaticMethod(_object_type* object, Type* type, const char* name,
						  _res_type(*pointer)(_args ...), ProtectSection protection)
		{
		}
	};

	template<typename _type>
	ScriptValue::ScriptValue(const _type& value)
	{
		Converter<_type>::Write(value, *this);
		// 		if constexpr (std::is_same<DataType, bool>::value)
		// 			mValue = jerry_create_boolean(value);
		// 		else if constexpr (std::is_floating_point<DataType>::value)
		// 			mValue = jerry_create_number((double)value);
		// 		else if constexpr (std::is_same<DataType, String>::value)
		// 			mValue = jerry_create_string((jerry_char_t*)value.Data());
		// 		else if constexpr (std::is_same<DataType, const char*>::value)
		// 			mValue = jerry_create_string((jerry_char_t*)value);
		// 		else if constexpr (std::is_invocable<DataType>::value)
		// 		{
		// 			mValue = jerry_create_external_function(&CallFunction);
		// 
		// 			IDataContainer* funcContainer = new FunctionContainer<DataType>(value);
		// 			jerry_set_object_native_pointer(mValue, funcContainer, &GetDataDeleter().info);
		// 		}
		// 		else
		// 		{
		// 			mValue = jerry_create_object();
		// 
		// 			auto dataContainer = new DataContainer<DataType>(value);
		// 			jerry_set_object_native_pointer(mValue, (IDataContainer*)dataContainer, &GetDataDeleter().info);
		// 
		// 			if constexpr (std::is_base_of<IObject, DataType>::value)
		// 			{
		// 				ReflectScriptValueTypeProcessor processor(*this);
		// 				DataType::ProcessType<ReflectScriptValueTypeProcessor>(&dataContainer->data, processor);
		// 			}
		// 		}
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

		// 		if constexpr (std::is_same<DataType, bool>::value)
		// 			return ToBool();
		// 		else if constexpr (std::is_floating_point<DataType>::value)
		// 			return ToNumber();
		// 		else if constexpr (std::is_same<DataType, String>::value)
		// 			return ToString()
		// 		else
		// 		{
		// 			void* dataPtr = nullptr;
		// 			jerry_get_object_native_pointer(mValue, &dataPtr, &GetDataDeleter().info);
		// 			auto dataContainer = dynamic_cast<DataContainer<DataType*>>((IDataContainer*)dataPtr);
		// 			if (dataContainer)
		// 				return dataContainer->data;
		// 			else
		// 				return DataType();
		// 		}
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

		PackArgs(argsValues, args ...);

		if constexpr (std::is_same<_res_type, void>::value)
			InvokeRaw(thisValue, argsValues);
		else
			return InvokeRaw(thisValue, argsValues).GetValue<_res_type>();
	}

	template<typename _invocable_type, typename _res_type, typename ... _args>
	jerry_value_t ScriptValueBase::FunctionContainer<_invocable_type, _res_type, _args ...>::Invoke(jerry_value_t thisValue, jerry_value_t* args, int argsCount)
	{
		std::tuple<_args ...> argst;
		UnpackArgs(argst, args, argsCount);

		if constexpr (std::is_same<_res_type, void>::value)
		{
			std::apply(data, argst);
			return jerry_create_undefined();
		}
		else
		{
			ScriptValue res(std::apply(data, argst));
			return jerry_acquire_value(res.jvalue);
		}
	}

}

#endif // SCRIPTING_BACKEND_JERRYSCRIPT
