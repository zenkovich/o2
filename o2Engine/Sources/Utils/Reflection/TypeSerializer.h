#pragma once

namespace o2
{
	class DataNode;

	struct ITypeSerializer
	{
		virtual ~ITypeSerializer() { }

		virtual void Serialize(void* object, DataNode& data) const { }
		virtual void Deserialize(void* object, const DataNode& data) const { }
		virtual bool Equals(void* objectA, void* objectB) const { return false; }
		virtual void Copy(void* objectA, void* objectB) const { }
		virtual ITypeSerializer* Clone() const { return mnew ITypeSerializer(); }
	};

	template<typename T> struct RealEquals { static bool Check(const T& a, const T& b) { return Math::Equals(a, b); } };
	template<typename T> struct FakeEquals { static bool Check(const T& a, const T& b) { return false; } };

	template<typename T> struct RealCopy { static void Copy(T& a, const T& b) { a = b; } };
	template<typename T> struct FakeCopy { static void Copy(T& a, const T& b) { } };

	template<typename T> struct RealSerialize { static void Serialize(const T& value, DataNode& data) { data.SetValue(value); } };
	template<typename T> struct FakeSerialize { static void Serialize(const T& value, DataNode& data) { } };

	template<typename T> struct RealDeserialize { static void Deserialize(T& value, const DataNode& data) { data.GetValue(value); } };
	template<typename T> struct FakeDeserialize { static void Deserialize(T& value, const DataNode& data) { } };

	template<typename _type,
		typename _serializer = typename std::conditional<DataNode::IsSupport<_type>::value, RealSerialize<_type>, FakeSerialize<_type>>::type,
		typename _deserializer = typename std::conditional<DataNode::IsSupport<_type>::value, RealDeserialize<_type>, FakeDeserialize<_type>>::type,
		typename _checker = typename std::conditional<EqualsOperator::IsExists<_type>::value, RealEquals<_type>, FakeEquals<_type>>::type,
		typename _copier = typename std::conditional<std::is_assignable<_type&, _type>::value, RealCopy<_type>, FakeCopy<_type>>::type>
	struct TypeSerializer : public ITypeSerializer
	{
		static constexpr bool isSerializable = DataNode::IsSupport<_type>::value;
		static constexpr bool isEqualsSupport = EqualsOperator::IsExists<_type>::value;
		static constexpr bool isCopyable = std::is_assignable<_type&, _type>::value;

		void Serialize(void* object, DataNode& data) const;
		void Deserialize(void* object, const DataNode& data) const;
		bool Equals(void* objectA, void* objectB) const;
		void Copy(void* objectA, void* objectB) const;

		ITypeSerializer* Clone() const;
	};

	template<typename _type, typename _serializer, typename _deserializer, typename _checker, typename _copier>
	void TypeSerializer<_type, _serializer, _deserializer, _checker, _copier>::Serialize(void* object, DataNode& data) const
	{
		_serializer::Serialize(*(_type*)object, data);
	}

	template<typename _type, typename _serializer, typename _deserializer, typename _checker, typename _copier>
	void TypeSerializer<_type, _serializer, _deserializer, _checker, _copier>::Deserialize(void* object, const DataNode& data) const
	{
		_deserializer::Deserialize(*(_type*)object, data);
	}

	template<typename _type, typename _serializer, typename _deserializer, typename _checker, typename _copier>
	bool TypeSerializer<_type, _serializer, _deserializer, _checker, _copier>::Equals(void* objectA, void* objectB) const
	{
		return _checker::Check(*(_type*)objectA, *(_type*)objectB);
	}

	template<typename _type, typename _serializer, typename _deserializer, typename _checker, typename _copier>
	void TypeSerializer<_type, _serializer, _deserializer, _checker, _copier>::Copy(void* objectA, void* objectB) const
	{
		_copier::Copy(*(_type*)objectA, *(_type*)objectB);
	}

	template<typename _type, typename _serializer, typename _deserializer, typename _checker, typename _copier>
	ITypeSerializer* TypeSerializer<_type, _serializer, _deserializer, _checker, _copier>::Clone() const
	{
		return mnew TypeSerializer();
	}
}
