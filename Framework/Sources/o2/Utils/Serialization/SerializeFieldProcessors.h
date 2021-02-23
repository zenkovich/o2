#pragma once
#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
	class ISerializable;

	template<class T, class = void_t<>>
	struct HasCheckSerialize: std::false_type { };

	template<class T>
	struct HasCheckSerialize<T, void_t<decltype(&T::CheckSerialize)>>: std::true_type { };

	template<class T, class = void_t<>>
	struct HasAttributeProcessor: std::false_type { };

	template<class T>
	struct HasAttributeProcessor<T, void_t<decltype(&T::hasSerializeFieldProcessor)>>: std::true_type { };

	class SerializeTypeProcessor
	{
	public:
		struct BaseFieldProcessor;

		template<typename _type>
		struct SerializeDefaultFieldProcessor;

	public:
		DataValue& node;

	public:
		SerializeTypeProcessor(DataValue& node):node(node) {}

		template<typename _object_type>
		void StartBases(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartFields(_object_type* object, Type* type) {}

		template<typename _object_type, typename _base_type>
		void BaseType(_object_type* object, Type* type, const char* name)
		{
			if constexpr (std::is_base_of<ISerializable, _base_type>::value && !std::is_same<ISerializable, _base_type>::value)
				object->_base_type::SerializeBasic(node);
		}

		BaseFieldProcessor StartField()
		{
			return BaseFieldProcessor(node);
		}

		struct BaseFieldProcessor
		{
			DataValue& node;

			BaseFieldProcessor(DataValue& node):node(node) {}

			template<typename _base, typename _attribute_type, typename ... _args>
			auto AddAttributeImpl(const _base& base, _args ... args)
			{
				if constexpr (HasAttributeProcessor<_attribute_type>::value)
					return _attribute_type::SerializeFieldProcessor<_base>(base, args ...);
				else
					return *this;
			}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				return AddAttributeImpl<BaseFieldProcessor, _attribute_type, _args ...>(*this, args ...);
			}

			template<typename _type>
			BaseFieldProcessor& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			BaseFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
											_field_type& field)
			{
				return *this;
			}

			void SetProtectSection(ProtectSection section) {}

			template<typename _object_type, typename _field_type>
			bool CheckSerialize(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
								_field_type& field)
			{
				return true;
			}
		};
	};

	class DeserializeTypeProcessor
	{
	public:
		struct BaseFieldProcessor;

		template<typename _type>
		struct DeserializeDefaultFieldProcessor;

	public:
		const DataValue& node;

	public:
		DeserializeTypeProcessor(const DataValue& node):node(node) {}

		template<typename _object_type>
		void StartBases(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartFields(_object_type* object, Type* type) {}

		template<typename _object_type, typename _base_type>
		void BaseType(_object_type* object, Type* type, const char* name)
		{
			if constexpr (std::is_base_of<ISerializable, _base_type>::value && !std::is_same<ISerializable, _base_type>::value)
				object->_base_type::DeserializeBasic(node);
		}

		BaseFieldProcessor StartField()
		{
			return BaseFieldProcessor(node);
		}

		struct BaseFieldProcessor
		{
			const DataValue& node;

			BaseFieldProcessor(const DataValue& node):node(node) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				if constexpr (std::is_same<_attribute_type, SerializableAttribute>::value)
					return DeserializeFieldProcessor(node);
				else
					return *this;
			}

			template<typename _type>
			BaseFieldProcessor& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			BaseFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
											_field_type& field)
			{
				return *this;
			}

			void SetProtectSection(ProtectSection section)
			{}
		};

		struct DeserializeFieldProcessor: public BaseFieldProcessor
		{
			DeserializeFieldProcessor(const DataValue& node):BaseFieldProcessor(node) {}

			template<typename _attribute_type, typename ... _args>
			DeserializeFieldProcessor& AddAttribute(_args ... args)
			{
				return *this;
			}

			template<typename _type>
			DeserializeFieldProcessor& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			DeserializeFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
												   _field_type& field)
			{
				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));

				if (auto m = node.FindMember(name))
					m->Get(*fieldPtr);

				return *this;
			}
		};
	};

	template<typename _origin_type>
	class SerializeDeltaTypeProcessor
	{
	public:
		struct BaseFieldProcessor;

	public:
		DataValue& node;
		const _origin_type& origin;

	public:
		SerializeDeltaTypeProcessor(DataValue& node, const _origin_type& origin):node(node), origin(origin) {}

		template<typename _object_type>
		void StartBases(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartFields(_object_type* object, Type* type) {}

		template<typename _object_type, typename _base_type>
		void BaseType(_object_type* object, Type* type, const char* name)
		{
			if constexpr (std::is_base_of<ISerializable, _base_type>::value && !std::is_same<ISerializable, _base_type>::value)
				object->_base_type::SerializeDeltaBasic(node, origin);
		}

		BaseFieldProcessor StartField()
		{
			return BaseFieldProcessor(node, origin);
		}

		struct BaseFieldProcessor
		{
			DataValue& node;
			const _origin_type& origin;

			BaseFieldProcessor(DataValue& node, const _origin_type& origin):node(node), origin(origin) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				if constexpr (std::is_same<_attribute_type, SerializableAttribute>::value)
					return SerializeFieldProcessor(node, origin);
				else
					return *this;
			}

			template<typename _type>
			BaseFieldProcessor& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			BaseFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
											_field_type& field)
			{
				return *this;
			}

			void SetProtectSection(ProtectSection section) {}
		};

		struct SerializeFieldProcessor: public BaseFieldProcessor
		{
			SerializeFieldProcessor(DataValue& node, const _origin_type& origin):BaseFieldProcessor(node, origin) {}

			template<typename _attribute_type, typename ... _args>
			SerializeFieldProcessor& AddAttribute(_args ... args)
			{
				return *this;
			}

			template<typename _type>
			SerializeFieldProcessor& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			SerializeFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
												 _field_type& field)
			{
				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
				_field_type* originFieldPtr = (_field_type*)((*pointerGetter)(const_cast<_origin_type*>(&origin)));

				if constexpr (SupportsEqualOperator<_field_type>::value)
				{
					if (EqualsForDeltaSerialize(*fieldPtr, *originFieldPtr))
						return *this;
				}

				DataValue& member = node.AddMember(name);
				member.SetDelta(*fieldPtr, *originFieldPtr);
				if (member.IsNull())
					node.RemoveMember(name);

				return *this;
			}
		};
	};

	template<typename _origin_type>
	class DeserializeDeltaTypeProcessor
	{
	public:
		struct BaseFieldProcessor;

		template<typename _type>
		struct DeserializeDefaultFieldProcessor;

	public:
		const DataValue& node;
		const _origin_type& origin;

	public:
		DeserializeDeltaTypeProcessor(const DataValue& node, const _origin_type& origin):node(node), origin(origin) {}

		template<typename _object_type>
		void StartBases(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartFields(_object_type* object, Type* type) {}

		template<typename _object_type, typename _base_type>
		void BaseType(_object_type* object, Type* type, const char* name)
		{
			if constexpr (std::is_base_of<ISerializable, _base_type>::value && !std::is_same<ISerializable, _base_type>::value)
				object->_base_type::DeserializeDeltaBasic(node, origin);
		}

		BaseFieldProcessor StartField()
		{
			return BaseFieldProcessor(node, origin);
		}

		struct BaseFieldProcessor
		{
			const DataValue& node;
			const _origin_type& origin;

			BaseFieldProcessor(const DataValue& node, const _origin_type& origin):node(node), origin(origin) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args)
			{
				if constexpr (std::is_same<_attribute_type, SerializableAttribute>::value)
					return DeserializeFieldProcessor(node, origin);
				else
					return *this;
			}

			template<typename _type>
			BaseFieldProcessor& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			BaseFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
											_field_type& field)
			{
				return *this;
			}

			void SetProtectSection(ProtectSection section)
			{}
		};

		struct DeserializeFieldProcessor: public BaseFieldProcessor
		{
			DeserializeFieldProcessor(const DataValue& node, const _origin_type& origin):BaseFieldProcessor(node, origin) {}

			template<typename _attribute_type, typename ... _args>
			DeserializeFieldProcessor& AddAttribute(_args ... args)
			{
				return *this;
			}

			template<typename _type>
			DeserializeFieldProcessor& SetDefaultValue(const _type& value)
			{
				return *this;
			}

			template<typename _object_type, typename _field_type>
			DeserializeFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
												   _field_type& field)
			{
				_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
				_field_type* originFieldPtr = (_field_type*)((*pointerGetter)(const_cast<_origin_type*>(&origin)));

				if (auto m = node.FindMember(name); m && !m->IsNull())
				{
					m->GetDelta(*fieldPtr, *originFieldPtr);
				}
				else
				{
					_field_type* originFieldPtr = (_field_type*)((*pointerGetter)(const_cast<_origin_type*>(&origin)));
					*fieldPtr = *originFieldPtr;
				}

				return *this;
			}
		};
	};
}
