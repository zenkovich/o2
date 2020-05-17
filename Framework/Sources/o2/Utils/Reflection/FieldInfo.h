#pragma once

#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/SearchPassedObject.h"
#include "o2/Utils/Reflection/TypeSerializer.h"
#include "o2/Utils/Reflection/TypeSerializer.h"
#include "o2/Utils/Reflection/TypeTraits.h"
#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Types/CommonTypes.h"
#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Type;
	class DataValue;

	// -----------------------
	// Class field information
	// -----------------------
	class FieldInfo
	{
	public:
		typedef void*(*GetValuePointerFuncPtr)(void*);

	public:
		// Default constructor
		FieldInfo();

		// Constructor
		FieldInfo(const String& name, GetValuePointerFuncPtr pointerGetter, const Type* type, ProtectSection sect,
				  ITypeSerializer* serializer = nullptr);

		// Destructor
		virtual ~FieldInfo();

		// Equal operator
		bool operator==(const FieldInfo& other) const;

		// Adds attribute
		FieldInfo& AddAttribute(IAttribute* attribute);

		// Adds attribute by type and parameters
		template<typename _attr_type, typename ... _args>
		FieldInfo& AddAttribute(_args ... args);

		// Returns name of field
		const String& GetName() const;

		// Protection section
		ProtectSection GetProtectionSection() const;

		// Returns type
		const Type* GetType() const;

		// Returns field owner type
		const Type* GetOwnerType() const;

		// Returns value of field in specified object
		template<typename _type>
		_type GetValue(void* object) const;

		// Returns value pointer of field in specified object (checks is it pointer)
		void* GetValuePtr(void* object) const;

		// Returns value pointer of field in specified object
		const void* GetValuePtrStrong(const void* object) const;

		// Returns value pointer of field in specified object
		void* GetValuePtrStrong(void* object) const;

		// Sets value of field in specified object
		template<typename _type>
		void SetValue(void* object, _type value) const;

		// Returns attribute of field, if it is not exist - returns nullptr
		template<typename _attr_type>
		_attr_type* GetAttribute() const;

		// Returns true if exist attribute with specified type
		template<typename _attr_type>
		bool HasAttribute() const;

		// Returns attributes array
		const Vector<IAttribute*>& GetAttributes() const;

		// Serializes from object pointer, that contains this field
		void SerializeFromObject(void* object, DataValue& data) const;

		// Deserializes from object pointer, that contains this field
		void DeserializeFromObject(void* object, const DataValue& data) const;

		// Serializes value from ptr
		void Serialize(void* ptr, DataValue& data) const;

		// Deserializes value from ptr
		void Deserialize(void* ptr, const DataValue& data) const;

		// Returns is values getted from object A and object B equals
		bool IsValueEquals(void* objectA, void* objectB) const;

		// Copies value getter from objectB to value from objectA
		void CopyValue(void* objectA, void* objectB) const;

	protected:
		ProtectSection         mProtectSection = ProtectSection::Public; // Protection section
		String                 mName;                                    // Name of field
		const Type*            mType = nullptr;                          // Field type
		Type*                  mOwnerType = nullptr;                     // Field owner type
		Vector<IAttribute*>    mAttributes;                              // Attributes array
		ITypeSerializer*       mSerializer = nullptr;                    // field serializer
		GetValuePointerFuncPtr mPointerGetter = nullptr;                 // Value pointer getter function

	protected:
		// Searches field recursively by path
		virtual void* SearchFieldPtr(void* obj, const String& path, FieldInfo*& fieldInfo);

	private:
		// Deprecated Copy-constructor
		FieldInfo(const FieldInfo& other);

		// Deprecated Copy-operator
		FieldInfo& operator=(const FieldInfo& other);


		friend class Type;
		friend class VectorType;
		friend class TypeInitializer;

		template<typename _type, typename _accessor_type>
		friend class TStringPointerAccessorType;
	};

	template<typename _attr_type>
	bool FieldInfo::HasAttribute() const
	{
		for (auto attr : mAttributes)
		{
			if (dynamic_cast<_attr_type*>(attr))
				return true;
		}

		return false;
	}

	template<typename _attr_type>
	_attr_type* FieldInfo::GetAttribute() const
	{
		for (auto attr : mAttributes)
		{
			_attr_type* res = dynamic_cast<_attr_type*>(attr);
			if (res)
				return res;
		}

		return nullptr;
	}

	template<typename _type>
	void FieldInfo::SetValue(void* object, _type value) const
	{
		*(_type*)GetValuePtrStrong(object) = value;
	}

	template<typename _type>
	_type FieldInfo::GetValue(void* object) const
	{
		return *(_type*)GetValuePtrStrong(object);
	}

	template<typename _attr_type, typename ... _args>
	FieldInfo& FieldInfo::AddAttribute(_args ... args)
	{
		_attr_type* attribute = mnew _attr_type(args ...);
		attribute->mOwnerFieldInfo = this;
		mAttributes.Add(attribute);
		return *this;
	}
}
