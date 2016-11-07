#pragma once

#include "Utils/CommonTypes.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Reflection/Attribute.h"
#include "Utils/String.h"

namespace o2
{
	class Type;
	class DataNode;

	// -----------------------
	// Class field information
	// -----------------------
	class FieldInfo
	{
	public:
		typedef Vector<IAttribute*> AttributesVec;

		struct IFieldSerializer
		{
			virtual void Serialize(void* object, DataNode& data) const {}
			virtual void Deserialize(void* object, DataNode& data) const {}
			virtual IFieldSerializer* Clone() const { return mnew IFieldSerializer(); }
		};

		template<typename _type>
		struct FieldSerializer: public IFieldSerializer
		{
			void Serialize(void* object, DataNode& data) const;
			void Deserialize(void* object, DataNode& data) const;
			IFieldSerializer* Clone() const;
		};

	public:
		// Default constructor
		FieldInfo();

		// Constructor
		FieldInfo(const String& name, UInt offset, bool isProperty, bool isPtr, const Type* type, ProtectSection sect,
				  IFieldSerializer* serializer);

		// Destructor
		~FieldInfo();

		// Equal operator
		bool operator==(const FieldInfo& other) const;

		// Adds attribute
		FieldInfo& AddAttribute(IAttribute* attribute);

		// Adds attribute by type and parameters
		template<typename _attr_type, typename ... _args>
		FieldInfo& AddAttribute(_args ... args);

		// Returns name of field
		const String& Name() const;

		// Protection section
		ProtectSection GetProtectionSection() const;

		// Returns type
		const Type& GetType() const;

		// Returns is it property or not
		bool IsProperty() const;

		// Return is field is pointer
		bool IsPointer() const;

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
		_attr_type* Attribute() const;

		// Returns true if exist attribute with specified type
		template<typename _attr_type>
		bool HaveAttribute() const;

		// Returns attributes array
		const AttributesVec& Attributes() const;

		// Serializes object by pointer
		void SerializeObject(void* object, DataNode& data) const;

		// Deserializes object by pointer
		void DeserializeObject(void* object, DataNode& data) const;

	protected:
		ProtectSection    mProtectSection; // Protection section
		String            mName;           // Name of field
		UInt              mOffset;         // Offset of field in bytes from owner address
		bool              mIsProperty;     // Is it property or field
		bool              mIsPointer;      // Is property pointer
		const Type*       mType;           // Field type
		AttributesVec     mAttributes;     // Attributes array
		IFieldSerializer* mSerializer;     // field serializer

	protected:
		// Searches field recursively by pointer
		virtual FieldInfo* SearchFieldPath(void* obj, void* target, const String& path, String& res,
										   Vector<void*>& passedObjects);

		// Searches field recursively by path
		virtual void* SearchFieldPtr(void* obj, const String& path, FieldInfo*& fieldInfo);

	private:
		// Deprecated Copy-constructor
		FieldInfo(const FieldInfo& other);

		// Deprecated Copy-operator
		FieldInfo& operator=(const FieldInfo& other);


		friend class Type;
		friend class VectorType;

		template<typename _type>
		friend class StringPointerAccessorType;
	};

	template<typename _attr_type>
	bool FieldInfo::HaveAttribute() const
	{
		for (auto attr : mAttributes)
		{
			if (dynamic_cast<_attr_type*>(attr))
				return true;
		}

		return false;
	}

	template<typename _attr_type>
	_attr_type* FieldInfo::Attribute() const
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
		*(_type*)((char*)object + mOffset) = value;
	}

	template<typename _type>
	_type FieldInfo::GetValue(void* object) const
	{
		return *(_type*)((char*)object + mOffset);
	}

	template<typename _attr_type, typename ... _args>
	FieldInfo& FieldInfo::AddAttribute(_args ... args)
	{
		_attr_type* attribute = mnew _attr_type(args ...);
		attribute->mOwnerFieldInfo = this;
		mAttributes.Add(attribute);
		return *this;
	}

	template<typename _type>
	void FieldInfo::FieldSerializer<_type>::Serialize(void* object, DataNode& data) const
	{
		data.SetValue(*(_type*)object);
	}

	template<typename _type>
	void FieldInfo::FieldSerializer<_type>::Deserialize(void* object, DataNode& data) const
	{
		data.GetValue(*(_type*)object);
	}

	template<typename _type>
	FieldInfo::IFieldSerializer* FieldInfo::FieldSerializer<_type>::Clone() const
	{
		return mnew FieldSerializer();
	}
}
