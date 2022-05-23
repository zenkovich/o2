#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "jerryscript/jerry-core/include/jerryscript-core.h"
#include "o2/Utils/Function.h"

namespace o2
{
	class IObject;
	class Type;

	class ScriptValueBase
	{
	public:
		mutable jerry_value_t jvalue;

	public:
		virtual ~ScriptValueBase();

		// Releases current value and acquires new
		void AcquireValue(jerry_value_t v);

		// Releases current value and accepts new. After this class will release value
		void Accept(jerry_value_t v);

	public:
		struct IDataContainer
		{
			bool isDataOwner = true;

			virtual ~IDataContainer() = default;

			virtual void* GetData() const { return nullptr; }
			virtual IObject* TryCastToIObject() const { return nullptr; }
			virtual const Type* GetType() const { return nullptr; }
		};

		template<typename _type>
		struct DataContainer: public IDataContainer
		{
			_type* data;

			DataContainer(_type* data):data(data) {}
			~DataContainer() override 
			{ 
				if (isDataOwner && data) 
					delete data; 
			}

			void* GetData() const override { return data; }

			IObject* TryCastToIObject() const override;

			const Type* GetType() const override;
		};

		struct IFunctionContainer
		{
			virtual jerry_value_t Invoke(jerry_value_t thisValue, jerry_value_t* args, int argsCount) = 0;
		};

		template<typename _invocable_type, typename _res_type, typename ... _args>
		struct FunctionContainer: public DataContainer<_invocable_type>, public IFunctionContainer
		{
			FunctionContainer(_invocable_type* function):DataContainer(function) {}
			jerry_value_t Invoke(jerry_value_t thisValue, jerry_value_t* args, int argsCount) override;
		};

		struct ISetterWrapperContainer: public IDataContainer
		{
			virtual void Set(jerry_value_t value) = 0;
		};

		struct IGetterWrapperContainer: public IDataContainer
		{
			virtual jerry_value_t Get() = 0;
		};

		template<typename _type>
		struct PointerSetterWrapperContainer: public ISetterWrapperContainer
		{
			_type* dataPtr = nullptr;

			void Set(jerry_value_t value) override;
		};

		template<typename _type>
		struct PointerGetterWrapperContainer: public IGetterWrapperContainer
		{
			_type* dataPtr = nullptr;

			jerry_value_t Get() override;
		};

		template<typename _property_type>
		struct PropertySetterWrapperContainer: public ISetterWrapperContainer
		{
			_property_type* propertyPtr = nullptr;

			void Set(jerry_value_t value) override;
		};

		template<typename _property_type>
		struct PropertyGetterWrapperContainer: public IGetterWrapperContainer
		{
			_property_type* propertyPtr = nullptr;

			jerry_value_t Get() override;
		};

		template<typename _type>
		struct FunctionalSetterWrapperContainer: public ISetterWrapperContainer
		{
			Function<void(const _type& value)> setter;

			void Set(jerry_value_t value) override;
		};

		template<typename _type>
		struct FunctionalGetterWrapperContainer: public IGetterWrapperContainer
		{
			Function<_type()> getter;

			jerry_value_t Get() override;
		};

		struct DataContainerDeleter
		{
			jerry_object_native_info_t info;

			DataContainerDeleter();
			static void Free(void* ptr);
		};

	protected:
		static DataContainerDeleter& GetDataDeleter();

		static jerry_value_t CallFunction(const jerry_value_t function_obj,
										  const jerry_value_t this_val,
										  const jerry_value_t args_p[],
										  const jerry_length_t args_count);

		static jerry_value_t DescriptorSetter(const jerry_value_t function_obj,
											  const jerry_value_t this_val,
											  const jerry_value_t args_p[],
											  const jerry_length_t args_count);

		static jerry_value_t DescriptorGetter(const jerry_value_t function_obj,
											  const jerry_value_t this_val,
											  const jerry_value_t args_p[],
											  const jerry_length_t args_count);

		template<size_t _i = 0, typename... _args>
		static void UnpackArgs(std::tuple<_args ...>& argst, jerry_value_t* args, int argsCount)
		{
			if (_i < argsCount)
			{
				ScriptValue tmp;
				tmp.AcquireValue(args[_i]);
				std::get<_i>(argst) = tmp.GetValue<std::remove_reference<decltype(std::get<_i>(argst))>::type>();

				if constexpr (_i + 1 != sizeof...(_args))
					UnpackArgs<_i + 1>(argst, args, argsCount);
			}
		}

		friend class ScriptEngine;
	};
}

#endif
