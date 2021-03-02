
#pragma once

#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Types/String.h"
#include "o2/Utils/Singleton.h"

namespace o2
{
	// -----------------------------
	// Serializable object interface
	// -----------------------------
	class ISerializable: public IObject
	{
	public:
		// Serializing object into data node
		virtual void Serialize(DataValue& node) const {}

		// Deserializing object from data node
		virtual void Deserialize(const DataValue& node) {};

		// Serializing object delta from origin into data node
		virtual void SerializeDelta(DataValue& node, const IObject& origin) const {}

		// Deserializing object delta from origin from data node
		virtual void DeserializeDelta(const DataValue& node, const IObject& origin) {};

		// Serializes data to string
		String SerializeToString() const;

		// Deserializes data from string
		void DeserializeFromString(const String& str);

		// Assign operator from data node
		ISerializable& operator=(const DataValue& node) { return *this; };

		// DataDocument converting operator
		operator DataDocument() const { return DataDocument(); }

		IOBJECT(ISerializable);

	protected:
		// Serializing object into data node
		virtual void SerializeBasic(DataValue& node) const {}

		// Deserializing object from data node
		virtual void DeserializeBasic(const DataValue& node) {}

		// Serializing object into data node
		virtual void SerializeDeltaBasic(DataValue& node, const IObject& origin) const {}

		// Deserializing object from data node
		virtual void DeserializeDeltaBasic(const DataValue& node, const IObject& origin) {}

		// Beginning serialization callback
		virtual void OnSerialize(DataValue& node) const {}

		// Completion deserialization callback
		virtual void OnDeserialized(const DataValue& node) {}

		// Beginning serialization delta callback
		virtual void OnSerializeDelta(DataValue& node, const IObject& origin) const {}

		// Completion deserialization delta callback
		virtual void OnDeserializedDelta(const DataValue& node, const IObject& origin) {}
	};

	template<typename T>
	struct DataValue::Converter<T, typename std::enable_if<std::is_base_of<ISerializable, T>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, DataValue& data)
		{
			value.Serialize(data);
		}

		static void Read(T& value, const DataValue& data)
		{
			value.Deserialize(data);
		}
	};

	template<typename T>
	struct DataValue::DeltaConverter<T, typename std::enable_if<std::is_base_of<ISerializable, T>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, const T& origin, DataValue& data)
		{
			value.SerializeDelta(data, dynamic_cast<const IObject&>(origin));
		}

		static void Read(T& value, const T& origin, const DataValue& data)
		{
			value.DeserializeDelta(data, dynamic_cast<const IObject&>(origin));
		}
	};

	template<typename _type, typename _enable = void>
	struct CheckSerializeBasicOverridden
	{
		// Default serialization way
		static void Process(_type* object, DataValue& node)
		{
			object->SerializeBasic(node);
			object->OnSerialize(node);
		}
	};

	template<typename T>
	struct CheckSerializeBasicOverridden<T, typename void_t<decltype(&T::SerializeBasicOverride)>>
	{
		// Using overridden function
		static void Process(T* object, DataValue& node)
		{
			object->SerializeBasicOverride(node);
		}
	};

	template<typename _type, typename _enable = void>
	struct CheckDeserializeBasicOverridden
	{
		// Default serialization way
		static void Process(_type* object, const DataValue& node)
		{
			object->DeserializeBasic(node);
			object->OnDeserialized(node);
		}
	};

	template<typename T>
	struct CheckDeserializeBasicOverridden<T, typename void_t<decltype(&T::DeserializeBasicOverride)>>
	{
		// Using overridden function
		static void Process(T* object, const DataValue& node)
		{
			object->DeserializeBasicOverride(node);
		}
	};

	template<typename _type, typename _enable = void>
	struct CheckSerializeDeltaBasicOverridden
	{
		// Default serialization way
		static void Process(_type* object, const _type* origin, DataValue& node)
		{
			object->SerializeDeltaBasic(node, *origin);
			object->OnSerializeDelta(node, *origin);
		}
	};

	template<typename T>
	struct CheckSerializeDeltaBasicOverridden<T, typename void_t<decltype(&T::SerializeDeltaBasicOverride)>>
	{
		// Using overridden function
		static void Process(T* object, const T* origin, DataValue& node)
		{
			object->SerializeDeltaBasicOverride(node, *origin);
		}
	};

	template<typename _type, typename _enable = void>
	struct CheckDeserializeDeltaBasicOverridden
	{
		// Default serialization way
		static void Process(_type* object, const _type* origin, const DataValue& node)
		{
			object->DeserializeDeltaBasic(node, *origin);
			object->OnDeserializedDelta(node, *origin);
		}
	};

	template<typename T>
	struct CheckDeserializeDeltaBasicOverridden<T, typename void_t<decltype(&T::DeserializeDeltaBasicOverride)>>
	{
		// Using overridden function
		static void Process(T* object, const T* origin, const DataValue& node)
		{
			object->DeserializeDeltaBasicOverride(node, *origin);
		}
	};

	// ----------------------------
	// Serializable field attribute
	// ----------------------------
	class SerializableAttribute: public IAttribute
	{
	public:
		ATTRIBUTE_COMMENT_DEFINITION("SERIALIZABLE");
		ATTRIBUTE_SHORT_DEFINITION("SERIALIZABLE_ATTRIBUTE");

	public:
		static constexpr bool hasSerializeFieldProcessor = true;
		static constexpr bool hasDeserializeFieldProcessor = true;

		template<typename _base, typename _type>
		struct SerializeDefaultProcessorMixin: public _base
		{
			_type defaultValue;

			SerializeDefaultProcessorMixin(const _base& base, const _type& defaultValue):
				_base(base), defaultValue(defaultValue) {}

			template<typename _object_type, typename _field_type>
			bool CheckSerialize(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
								_field_type& field)
			{
				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
				return *fieldPtr != defaultValue;
			}

			template<typename _object_type, typename _field_type>
			auto& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
							  _field_type& field)
			{
				if (!CheckSerialize(object, type, name, pointerGetter, field))
					return *this;

				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
				node.AddMember(name).Set(*fieldPtr);

				return *this;
			}
		};

		template<typename _base>
		struct SerializeFieldProcessor: public _base
		{
			SerializeFieldProcessor(const _base& base): _base(base) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				return AddAttributeImpl<SerializeFieldProcessor<_base>, _attribute_type, _args ...>(*this, args ...);
			}

			template<typename _type>
			auto SetDefaultValue(const _type& value)
			{
				return SerializeDefaultProcessorMixin<SerializeFieldProcessor<_base>, _type>(*this, value);
			}

			template<typename _object_type, typename _field_type>
			bool CheckSerialize(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
								_field_type& field)
			{
				if (!_base::CheckSerialize(object, type, name, pointerGetter, field))
					return false;

				if constexpr (std::is_default_constructible<_field_type>::value && SupportsEqualOperator<_field_type>::value)
				{
					_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
					if (Math::Equals(*fieldPtr, _field_type()))
						return false;
				}

				return true;
			}

			template<typename _object_type, typename _field_type>
			auto& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
							  _field_type& field)
			{
				if (!CheckSerialize(object, type, name, pointerGetter, field))
					return *this;

				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
				node.AddMember(name).Set(*fieldPtr);

				return *this;
			}
		};

		template<typename _base>
		struct DeserializeFieldProcessor: public _base
		{
			DeserializeFieldProcessor(const _base& base): _base(base) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				return AddAttributeImpl<DeserializeFieldProcessor<_base>, _attribute_type, _args ...>(*this, args ...);
			}

			template<typename _type>
			auto& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			auto& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
							  _field_type& field)
			{
				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));

				if (auto m = node.FindMember(name))
					m->Get(*fieldPtr);

				return *this;
			}
		};

		template<typename _base>
		struct SerializeDeltaFieldProcessor: public _base
		{
			SerializeDeltaFieldProcessor(const _base& base):_base(base) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				return AddAttributeImpl<SerializeDeltaFieldProcessor<_base>, _attribute_type, _args ...>(*this, args ...);
			}

			template<typename _type>
			auto& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			bool CheckSerialize(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
								_field_type& field)
			{
				if (!_base::CheckSerialize(object, type, name, pointerGetter, field))
					return false;

				if constexpr (SupportsEqualOperator<_field_type>::value)
				{
					_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
					_field_type* originFieldPtr = (_field_type*)((*pointerGetter)(const_cast<_object_type*>(&origin)));

					if (EqualsForDeltaSerialize(*fieldPtr, *originFieldPtr))
						return false;
				}

				return true;
			}

			template<typename _object_type, typename _field_type>
			auto& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
												 _field_type& field)
			{
				if (!CheckSerialize(object, type, name, pointerGetter, field))
					return *this;

				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
				_field_type* originFieldPtr = (_field_type*)((*pointerGetter)(const_cast<_object_type*>(&origin)));

				DataValue& member = node.AddMember(name);
				member.SetDelta(*fieldPtr, *originFieldPtr);
				if (member.IsNull())
					node.RemoveMember(name);

				return *this;
			}
		};

		template<typename _base>
		struct DeserializeDeltaFieldProcessor: public _base
		{
			DeserializeDeltaFieldProcessor(const _base& base):_base(base) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				return AddAttributeImpl<DeserializeDeltaFieldProcessor<_base>, _attribute_type, _args ...>(*this, args ...);
			}

			template<typename _type>
			auto& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			auto& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
												   _field_type& field)
			{
				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
				_field_type* originFieldPtr = (_field_type*)((*pointerGetter)(const_cast<_object_type*>(&origin)));

				if (auto m = node.FindMember(name); m && !m->IsNull())
					m->GetDelta(*fieldPtr, *originFieldPtr);
				else
				{
					_field_type* originFieldPtr = (_field_type*)((*pointerGetter)(const_cast<_object_type*>(&origin)));
					if constexpr (std::is_pointer<_field_type>::value &&
								  std::is_copy_constructible<std::remove_pointer<_field_type>::type>::value)
					{
						typedef std::remove_pointer<_field_type>::type _field_type_unp;
						if constexpr (IsCloneable<_field_type_unp>::value)
						{
							if constexpr (std::is_same<std::invoke_result<decltype(&_field_type_unp::Clone)()>::type, _field_type>::value)
								*fieldPtr = (*originFieldPtr)->Clone();
							else
								*fieldPtr = dynamic_cast<_field_type>((*originFieldPtr)->Clone());
						}
						else
							*fieldPtr = mnew typename std::remove_pointer<_field_type>::type(**originFieldPtr);
					}
					else
					{
						*fieldPtr = *originFieldPtr;
					}
				}

				return *this;
			}
		};
	};

	// ----------------------------------
	// Serialize with condition attribute
	// ----------------------------------
	template<typename _class>
	class SerializeIfAttribute: public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("SERIALIZE_IF");
		ATTRIBUTE_SHORT_DEFINITION("SERIALIZE_IF_ATTRIBUTE");

	public:
		bool(_class::*functionPtr)() const;

		SerializeIfAttribute(bool(_class::*functionPtr)() const): functionPtr(functionPtr) {}

		bool Invoke(const _class* object) const { return (object->*functionPtr)(); }

	public:
		static constexpr bool hasSerializeFieldProcessor = true;

		template<typename _base>
		struct SerializeFieldProcessor: public _base
		{
			bool(_class::*functionPtr)() const;

			SerializeFieldProcessor(const _base& base, bool(_class::*functionPtr)() const):
				_base(base), functionPtr(functionPtr) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				return AddAttributeImpl<SerializeFieldProcessor<_base>, _attribute_type, _args ...>(*this, args ...);
			}

			template<typename _type>
			auto& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			bool CheckSerialize(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
								_field_type& field)
			{
				if (!(object->*functionPtr)())
					return false;

				return _base::CheckSerialize(object, type, name, pointerGetter, field);
			}

			template<typename _object_type, typename _field_type>
			auto& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
							  _field_type& field)
			{
				if (!CheckSerialize(object, type, name, pointerGetter, field))
					return *this;

				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
				node.AddMember(name).Set(*fieldPtr);

				return *this;
			}
		};

		template<typename _base>
		struct SerializeDeltaFieldProcessor: public _base
		{
			bool(_class::*functionPtr)() const;

			SerializeDeltaFieldProcessor(const _base& base, bool(_class::*functionPtr)() const):
				_base(base), functionPtr(functionPtr) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				return AddAttributeImpl<SerializeDeltaFieldProcessor<_base>, _attribute_type, _args ...>(*this, args ...);
			}

			template<typename _type>
			auto& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			bool CheckSerialize(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
								_field_type& field)
			{
				if (!(object->*functionPtr)())
					return false;

				return _base::CheckSerialize(object, type, name, pointerGetter, field);
			}

			template<typename _object_type, typename _field_type>
			auto& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
							  _field_type& field)
			{
				if (!CheckSerialize(object, type, name, pointerGetter, field))
					return *this;

				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
				_field_type* originFieldPtr = (_field_type*)((*pointerGetter)(const_cast<_base::OriginType*>(&origin)));

				DataValue& member = node.AddMember(name);
				member.SetDelta(*fieldPtr, *originFieldPtr);
				if (member.IsNull())
					node.RemoveMember(name);

				return *this;
			}
		};
	};

#define SERIALIZABLE_ATTRIBUTE() \
    AddAttribute<SerializableAttribute>()

#define SERIALIZE_IF_ATTRIBUTE(FUNC) \
    AddAttribute<SerializeIfAttribute<thisclass>>(&thisclass::FUNC)

	// Serialization implementation macros
#define SERIALIZABLE_MAIN(CLASS)  							                                                                       \
    IOBJECT_MAIN(CLASS)																							                   \
                                                                                                                                   \
	template<typename _type, typename _enable>       															                   \
	friend struct CheckSerializeBasicOverridden;                                                                                   \
	                                                                                                                               \
	template<typename _type, typename _enable>       															                   \
	friend struct CheckDeserializeBasicOverridden;																                   \
                                                                                                                                   \
	template<typename _type, typename _enable>       															                   \
	friend struct CheckSerializeDeltaBasicOverridden;                                                                              \
	                                                                                                                               \
	template<typename _type, typename _enable>       															                   \
	friend struct CheckDeserializeDeltaBasicOverridden;																               \
                                                                                                                                   \
    void SerializeBasic(o2::DataValue& node) const override                                                                        \
    {						                                                                                                       \
    	SerializeTypeProcessor processor(node);                                                                                    \
		ProcessBaseTypes(const_cast<CLASS*>(this), processor);													                   \
		ProcessFields(const_cast<CLASS*>(this), processor);														                   \
	}	                                                                                                                           \
    void Serialize(o2::DataValue& node) const override                                                                             \
    {						                                                                                                       \
		CheckSerializeBasicOverridden<CLASS>::Process(const_cast<CLASS*>(this), node);                                             \
	}	                                                                                                                           \
    void DeserializeBasic(const o2::DataValue& node) override                                                                      \
    {												                                                                               \
    	DeserializeTypeProcessor processor(node);                                                                                  \
		ProcessBaseTypes(const_cast<CLASS*>(this), processor);													                   \
		ProcessFields(const_cast<CLASS*>(this), processor);														                   \
	}																											                   \
    void Deserialize(const o2::DataValue& node) override                                                                           \
    {												                                                                               \
		CheckDeserializeBasicOverridden<CLASS>::Process(const_cast<CLASS*>(this), node);                                           \
	}												                                                                               \
    void SerializeDeltaBasic(o2::DataValue& node, const IObject& origin) const override                                            \
    {						                                                                                                       \
    	SerializeDeltaTypeProcessor<CLASS> processor(node, dynamic_cast<const CLASS&>(origin));                                    \
		ProcessBaseTypes(const_cast<CLASS*>(this), processor);													                   \
		ProcessFields(const_cast<CLASS*>(this), processor);														                   \
	}	                                                                                                                           \
    void SerializeDelta(o2::DataValue& node, const IObject& origin) const override                                                 \
    {						                                                                                                       \
		CheckSerializeDeltaBasicOverridden<CLASS>::Process(const_cast<CLASS*>(this), dynamic_cast<const CLASS*>(&origin), node);   \
	}	                                                                                                                           \
    void DeserializeDeltaBasic(const o2::DataValue& node, const IObject& origin) override                                          \
    {												                                                                               \
    	DeserializeDeltaTypeProcessor<CLASS> processor(node, dynamic_cast<const CLASS&>(origin));                                  \
    	ProcessBaseTypes(const_cast<CLASS*>(this), processor);													                   \
    	ProcessFields(const_cast<CLASS*>(this), processor);														                   \
	}																											                   \
    void DeserializeDelta(const o2::DataValue& node, const IObject& origin) override                                               \
    {												                                                                               \
		CheckDeserializeDeltaBasicOverridden<CLASS>::Process(const_cast<CLASS*>(this), dynamic_cast<const CLASS*>(&origin), node); \
	}												                                                                               \
	CLASS& operator=(const o2::DataValue& node) 		                                                                           \
	{												                                                                               \
		Deserialize(node); return *this; 			                                                                               \
	}                                                                                                                              \
	operator DataDocument() const                                                                                                  \
	{                																							                   \
		DataDocument doc;																						                   \
		Serialize(doc);																							                   \
		return doc;                                                                                                                \
	}

#define SERIALIZABLE(CLASS)                                                                                     \
    SERIALIZABLE_MAIN(CLASS)                                                                                    \
		                                                                                                        \
    template<typename _type_processor> static void ProcessBaseTypes(CLASS* object, _type_processor& processor); \
    template<typename _type_processor> static void ProcessFields(CLASS* object, _type_processor& processor);    \
    template<typename _type_processor> static void ProcessMethods(CLASS* object, _type_processor& processor);   

#define SERIALIZABLE_MANUAL(CLASS) \
    SERIALIZABLE(CLASS)
}

#include "o2/Utils/Serialization/SerializeFieldProcessors.h"

CLASS_BASES_META(o2::ISerializable)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::ISerializable)
{
}
END_META;
CLASS_METHODS_META(o2::ISerializable)
{

	PUBLIC_FUNCTION(void, Serialize, DataValue&);
	PUBLIC_FUNCTION(void, Deserialize, const DataValue&);
	PUBLIC_FUNCTION(void, SerializeDelta, DataValue&, const IObject&);
	PUBLIC_FUNCTION(void, DeserializeDelta, const DataValue&, const IObject&);
	PUBLIC_FUNCTION(String, SerializeToString);
	PUBLIC_FUNCTION(void, DeserializeFromString, const String&);
	PROTECTED_FUNCTION(void, SerializeBasic, DataValue&);
	PROTECTED_FUNCTION(void, DeserializeBasic, const DataValue&);
	PROTECTED_FUNCTION(void, SerializeDeltaBasic, DataValue&, const IObject&);
	PROTECTED_FUNCTION(void, DeserializeDeltaBasic, const DataValue&, const IObject&);
	PROTECTED_FUNCTION(void, OnSerialize, DataValue&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, OnSerializeDelta, DataValue&, const IObject&);
	PROTECTED_FUNCTION(void, OnDeserializedDelta, const DataValue&, const IObject&);
}
END_META;
