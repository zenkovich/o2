#pragma once

#include "Utils/CommonTypes.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Reflection/Attribute.h"
#include "Utils/String.h"

namespace o2
{
	class Type;

	// -----------------------
	// Class field information
	// -----------------------
	class FieldInfo
	{
	public:
		typedef Vector<Ptr<IAttribute>> AttributesVec;

	public:
		// Default constructor
		FieldInfo();

		// Constructor
		FieldInfo(const String& name, UInt offset, bool isProperty, bool isPtr, Ptr<Type> type);

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

		// Returns value pointer of field in specified object
		template<typename _type>
		_type* GetValuePtr(void* object) const;

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

		// Returns attributes array
		const AttributesVec& Attributes() const;

	protected:
		String        mName;       // Name of field
		UInt          mOffset;     // Offset of field in bytes from owner address
		bool          mIsProperty; // Is it property or field
		bool          mIsPtr;      // Is property Ptr<>
		Ptr<Type>     mType;       // Field type
		AttributesVec mAttributes; // Attributes array

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
		AccessorFieldInfo(const String& name, UInt offset, Ptr<Type> type):
			FieldInfo(name, offset, false, false, type) {}

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
			_attr_type* res = dynamic_cast<_attr_type*>(attr.Get());
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
	_type* FieldInfo::GetValuePtrStrong(void* object) const
	{
		return (_type*)((char*)object + mOffset);
	}

	template<typename _type>
	_type* FieldInfo::GetValuePtr(void* object) const
	{
		if (mIsPtr) return *(Ptr<_type>*)((char*)object + mOffset);

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

		Accessor<Ptr<_type>, const String&>* accessor = ((Accessor<Ptr<_type>, const String&>*)obj);

		auto allFromAccessor = accessor->GetAll();

		for (auto kv : allFromAccessor)
		{
			for (auto field : mType->mFields)
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

		Accessor<Ptr<_type>, const String&>* accessor = ((Accessor<Ptr<_type>, const String&>*)obj);
		auto allFromAccessor = accessor->GetAll();

		for (auto kv : allFromAccessor)
		{
			if (kv.Key() == pathPart)
				return mType->GetFieldPtr<char>(kv.Value(), path.SubStr(delPos + 1), fieldInfo);
		}

		return nullptr;
	}

}
