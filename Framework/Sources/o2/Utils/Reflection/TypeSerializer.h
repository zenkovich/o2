#pragma once

namespace o2
{
	class DataValue;

	struct ITypeSerializer
	{
		virtual ~ITypeSerializer() { }

		virtual void Serialize(void* object, DataValue& data) const { }
		virtual void Deserialize(void* object, const DataValue& data) const { }

		virtual bool Equals(void* objectA, void* objectB) const { return false; }
		virtual void Copy(void* objectA, void* objectB) const { }
		virtual ITypeSerializer* Clone() const { return mnew ITypeSerializer(); }
	};
	template<typename _type>
	struct TypeSerializer : public ITypeSerializer
	{
		static constexpr bool isSerializable = DataValue::IsSupports<_type>::value;
		static constexpr bool isEqualsSupport = SupportsEqualOperator<_type>::value;
		static constexpr bool isCopyable = std::is_assignable<_type&, _type>::value;

		typedef typename std::conditional<std::is_copy_constructible<_type>::value, _type, void>::type defaultValueType;

	public:
		defaultValueType* defaultValue = nullptr;

	public:
		void Serialize(void* object, DataValue& data) const;
		void Deserialize(void* object, const DataValue& data) const;
		bool Equals(void* objectA, void* objectB) const;
		void Copy(void* objectA, void* objectB) const;

		ITypeSerializer* Clone() const;
	};

	template<typename _type>
	void TypeSerializer<_type>::Serialize(void* object, DataValue& data) const
	{
		if constexpr (isSerializable)
			data.Set(*(_type*)object);
	}

	template<typename _type>
	void TypeSerializer<_type>::Deserialize(void* object, const DataValue& data) const
	{
		if constexpr (isSerializable)
			data.Get(*(_type*)object);
	}

	template<typename _type>
	bool TypeSerializer<_type>::Equals(void* objectA, void* objectB) const
	{
		if constexpr (isEqualsSupport)
			return Math::Equals(*(_type*)objectA, *(_type*)objectB);

		return false;
	}

	template<typename _type>
	void TypeSerializer<_type>::Copy(void* objectA, void* objectB) const
	{
		if constexpr (isCopyable)
			*(_type*)objectA = *(_type*)objectB;
	}

	template<typename _type>
	ITypeSerializer* TypeSerializer<_type>::Clone() const
	{
		return mnew TypeSerializer();
	}
}
