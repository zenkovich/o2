#pragma once

#include "Utils/CommonTypes.h"
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

		// Copy-constructor
		FieldInfo(const FieldInfo& other);

		// Destructor
		~FieldInfo();

		// Copy-operator
		FieldInfo& operator=(const FieldInfo& other);

		// Equal operator
		bool operator==(const FieldInfo& other) const;

		// Returns cloned copy
		virtual FieldInfo* Clone() const;

		// Adds attribute
		FieldInfo& AddAttribute(IAttribute* attribute);

		// Adds attribute by type and parameters
		template<typename _attr_type, typename ... _args>
		FieldInfo& AddAttribute(_args ... args);

		// Returns name of field
		const String& Name() const;

		// Returns is it property or not
		bool IsProperty() const;

		// Returns value of field in specified object
		template<typename _type>
		_type GetValue(void* object) const;

		// Returns value pointer of field in specified object (checks is it pointer)
		template<typename _type>
		_type* GetValuePtr(void* object) const;

		// Returns value pointer of field in specified object
		template<typename _type>
		const _type* GetValuePtrStrong(const void* object) const;

		// Returns value pointer of field in specified object
		template<typename _type>
		_type* GetValuePtrStrong(void* object) const;

		// Sets value of field in specified object
		template<typename _type>
		void SetValue(void* object, _type value) const;

		// Returns attribute of field, if it is not exist - returns nullptr
		template<typename _attr_type>
		_attr_type* Attribute() const;

		// Returns true if exist attribute with specified type
		template<typename _attr_type>
		bool HaveAttribute() const;

		// Protection section
		ProtectSection GetProtectionSection() const;

		// Returns type
		const Type& GetType() const;

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
		bool              mIsPtr;          // Is property Ptr<>
		const Type*       mType;           // Field type
		AttributesVec     mAttributes;     // Attributes array
		IFieldSerializer* mSerializer;     // field serializer

	protected:
		// Searches field recursively by pointer
		virtual FieldInfo* SearchFieldPath(void* obj, void* target, const String& path, String& res, 
										   Vector<void*>& passedObjects);

		// Searches field recursively by path
		virtual void* SearchFieldPtr(void* obj, const String& path, FieldInfo*& fieldInfo);

		friend class Type;

		template<typename _type>
		friend class AccessorFieldInfo;
	};

	// -------------------
	// Accessor field info
	// -------------------
	template<typename _type>
	class AccessorFieldInfo: public FieldInfo
	{
	public:
		// Default constructor
		AccessorFieldInfo():FieldInfo() {}

		// Constructor
		AccessorFieldInfo(const String& name, UInt offset, const Type* type, ProtectSection sect, 
						  IFieldSerializer* serializer):
			FieldInfo(name, offset, false, false, type, sect, serializer) {}

		// Returns cloned copy
		virtual FieldInfo* Clone() const;

	protected:
		// Searches field recursively by pointer
		FieldInfo* SearchFieldPath(void* obj, void* target, const String& path, String& res,
								   Vector<void*>& passedObjects);

		// Searches field recursively by path
		void* SearchFieldPtr(void* obj, const String& path, FieldInfo*& fieldInfo);
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

	template<typename _type>
	const _type* FieldInfo::GetValuePtrStrong(const void* object) const
	{
		return (_type*)((char*)object + mOffset);
	}

	template<typename _type>
	_type* FieldInfo::GetValuePtrStrong(void* object) const
	{
		return (_type*)((char*)object + mOffset);
	}

	template<typename _type>
	_type* FieldInfo::GetValuePtr(void* object) const
	{
		if (mIsPtr) return *(_type**)((char*)object + mOffset);

		return (_type*)((char*)object + mOffset);
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
	FieldInfo* AccessorFieldInfo<_type>::Clone() const
	{
		return mnew AccessorFieldInfo<_type>(*this);
	}

	template<typename _type>
	FieldInfo* AccessorFieldInfo<_type>::SearchFieldPath(void* obj, void* target, const String& path, String& res,
														 Vector<void*>& passedObjects)
	{
		if (!mType)
			return false;

		Accessor<_type*, const String&>* accessor = ((Accessor<_type*, const String&>*)obj);

		auto allFromAccessor = accessor->GetAll();

		for (auto kv : allFromAccessor)
		{
			for (auto field : mType->AllFields())
			{
				char* fieldObj = field->GetValuePtr<char>(kv.Value());

				if (fieldObj == nullptr)
					continue;

				if (passedObjects.Contains(fieldObj))
					continue;

				passedObjects.Add(fieldObj);

				String newPath = path + "/" + kv.Key() + "/" + field->mName;
				if (fieldObj == target)
				{
					res = newPath;
					return field;
				}

				FieldInfo* childField = field->SearchFieldPath(fieldObj, target, newPath, res, passedObjects);
				if (childField)
					return childField;
			}
		}

		return nullptr;
	}

	template<typename _type>
	void* AccessorFieldInfo<_type>::SearchFieldPtr(void* obj, const String& path, FieldInfo*& fieldInfo)
	{
		int delPos = path.Find("/");
		String pathPart = path.SubStr(0, delPos);

		Accessor<_type*, const String&>* accessor = ((Accessor<_type*, const String&>*)obj);
		auto allFromAccessor = accessor->GetAll();

		for (auto kv : allFromAccessor)
		{
			if (kv.Key() == pathPart)
				return mType->GetFieldPtr<char>(kv.Value(), path.SubStr(delPos + 1), fieldInfo);
		}

		return nullptr;
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
