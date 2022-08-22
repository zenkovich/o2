#pragma once

namespace o2
{
	template<typename _res_type, typename ... _args>
	struct DataValue::Converter<Function<_res_type(_args ...)>>
	{
		static constexpr bool isSupported = true;

		static void Write(const Function<_res_type(_args ...)>& value, DataValue& data)
		{
			value.ForEach([&](const IFunction<_res_type(_args ...)>* x)
						  {
							  if (auto serializable = dynamic_cast<const ISerializableFunction*>(x))
								  serializable->Serialize(data.AddElement());
						  });
		}

		static void Read(Function<_res_type(_args ...)>& value, const DataValue& data)
		{
			if (data.IsArray())
			{
				value.Clear();
				for (auto& element : data)
				{
					ISerializableFunction* serializable = nullptr;
					String type = element["type"];
					if (type == "ActorFunction")
						serializable = mnew ActorSubscription<_res_type(_args ...)>();
					// 					else if (type == "ComponentFunction")
					// 						serializable = mnew ComponentSubscription<_res_type(_args ...)>();

					serializable->Deserialize(element);
					value.Add(dynamic_cast<IFunction<_res_type(_args ...)>*>(serializable));
				}
			}
		}
	};
}
