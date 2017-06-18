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
			virtual bool Equals(void* objectA, void* objectB) const { return false; }
			virtual void Copy(void* objectA, void* objectB) const {}
			virtual IFieldSerializer* Clone() const { return mnew IFieldSerializer(); }
		};

		template<typename T> struct RealEquals { static bool Check(const T& a, const T& b) { return Math::Equals(a, b); } };
		template<typename T> struct FakeEquals { static bool Check(const T& a, const T& b) { return false; } };

		template<typename T> struct RealCopy { static void Copy(T& a, const T& b) { a = b; } };
		template<typename T> struct FakeCopy { static void Copy(T& a, const T& b) {  } };

		template<typename _type, 
			     typename _checker = std::conditional<EqualsTrait::IsExists<_type>::value, RealEquals<_type>, FakeEquals<_type>>::type,
			     typename _copier = std::conditional<std::is_assignable<_type&, _type>::value, RealCopy<_type>, FakeCopy<_type>>::type>
		struct FieldSerializer: public IFieldSerializer
		{
			void Serialize(void* object, DataNode& data) const;
			void Deserialize(void* object, DataNode& data) const;
			bool Equals(void* objectA, void* objectB) const;
			void Copy(void* objectA, void* objectB) const;
			IFieldSerializer* Clone() const;
		};

	public:
		// Default constructor
		FieldInfo();

		// Constructor
		FieldInfo(const String& name, UInt offset, const Type* type, ProtectSection sect,
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
		const String& GetName() const;

		// Protection section
		ProtectSection GetProtectionSection() const;

		// Returns type
		const Type* GetType() const;

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
		const AttributesVec& GetAttributes() const;

		// Serializes from object pointer, that contains this field
		void SerializeFromObject(void* object, DataNode& data) const;

		// Deserializes from object pointer, that contains this field
		void DeserializeFromObject(void* object, DataNode& data) const;

		// Serializes value from ptr
		void Serialize(void* ptr, DataNode& data) const;

		// Deserializes value from ptr
		void Deserialize(void* ptr, DataNode& data) const;

		// Returns is values getted from object A and object B equals
		bool IsValueEquals(void* objectA, void* objectB) const;

		// Copies value getter from objectB to value from objectA
		void CopyValue(void* objectA, void* objectB) const;

	protected:
		ProtectSection    mProtectSection = ProtectSection::Public; // Protection section
		String            mName;                                    // Name of field
		UInt              mOffset = 0;                              // Offset of field in bytes from owner address
		const Type*       mType = nullptr;                          // Field type
		AttributesVec     mAttributes;                              // Attributes array
		IFieldSerializer* mSerializer = nullptr;                    // field serializer

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

	template<typename _type, typename _checker, typename _copier>
	void FieldInfo::FieldSerializer<_type, _checker, _copier>::Serialize(void* object, DataNode& data) const
	{
		data.SetValue(*(_type*)object);
	}

	template<typename _type, typename _checker, typename _copier>
	void FieldInfo::FieldSerializer<_type, _checker, _copier>::Deserialize(void* object, DataNode& data) const
	{
		data.GetValue(*(_type*)object);
	}

	template<typename _type, typename _checker, typename _copier>
	bool FieldInfo::FieldSerializer<_type, _checker, _copier>::Equals(void* objectA, void* objectB) const
	{
		return _checker::Check(*(_type*)objectA, *(_type*)objectB);
	}

	template<typename _type, typename _checker, typename _copier>
	void FieldInfo::FieldSerializer<_type, _checker, _copier>::Copy(void* objectA, void* objectB) const
	{
		_copier::Copy(*(_type*)objectA, *(_type*)objectB);
	}

	template<typename _type, typename _checker, typename _copier>
	FieldInfo::IFieldSerializer* FieldInfo::FieldSerializer<_type, _checker, _copier>::Clone() const
	{
		return mnew FieldSerializer();
	}
}
