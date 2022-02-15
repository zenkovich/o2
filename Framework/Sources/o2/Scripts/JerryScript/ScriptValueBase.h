#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "jerryscript/jerry-core/include/jerryscript-core.h"

namespace o2
{
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

	protected:
		struct IDataContainer
		{
			virtual ~IDataContainer() = default;
		};

		template<typename _type>
		struct DataContainer: public IDataContainer
		{
			_type data;

			DataContainer(const _type& data):data(data) {}
		};

		struct IFunctionContainer
		{
			virtual jerry_value_t Invoke(jerry_value_t thisValue, jerry_value_t* args, int argsCount) = 0;
		};

		template<typename _invocable_type, typename _res_type, typename ... _args>
		struct FunctionContainer: public DataContainer<_invocable_type>,  public IFunctionContainer
		{
			FunctionContainer(const _invocable_type& function):DataContainer(function) {}
			jerry_value_t Invoke(jerry_value_t thisValue, jerry_value_t* args, int argsCount) override;
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
