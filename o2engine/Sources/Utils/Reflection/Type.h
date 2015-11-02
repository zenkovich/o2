#pragma once

#include "Utils/Memory/Ptr.h"
#include "Utils/Property.h"
#include "Utils/Reflection/FieldInfo.h"
#include "Utils/String.h"

namespace o2
{
	class IObject;

	// -----------------------
	// Object type information
	// -----------------------
	class Type
	{
	public:
		typedef UInt Id;
		typedef Vector<Ptr<FieldInfo>> FieldInfosVec;
		typedef Vector<Ptr<Type>> TypesVec;

		struct Dummy
		{
			static Ptr<Type> type;
		};

	public:
		// Default constructor
		Type();

		// Destructor
		~Type();

		// Check equals operator
		bool operator==(const Type& other) const;

		// Check not equals operator
		bool operator!=(const Type& other) const;

		// Returns name of type
		const String& Name() const;

		// Returns id of type
		Id ID() const;

		// Returns vector of base types
		const TypesVec& BaseTypes() const;

		// Returns fields informations array
		const FieldInfosVec& Fields() const;

		// Returns field information by name
		const FieldInfo* Field(const String& name) const;

		// Returns inherited types
		TypesVec InheritedTypes() const;

		// Returns sample pointer
		const IObject* Sample() const;

		// Registers field in type
		template<typename _type>
		FieldInfo& RegField(const String& name, UInt offset, Ptr<_type> value);

		// Registers field in type
		template<typename _type>
		FieldInfo& RegField(const String& name, UInt offset, _type& value);

		// Registers field in type
		template<typename _type>
		FieldInfo& RegField(const String& name, UInt offset, Property<_type>& value);

		// Registers field in type
		template<typename _type>
		FieldInfo& RegField(const String& name, UInt offset, Accessor<Ptr<_type>, const String&>& value);

		template<typename _type>
		_type* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

		String GetFieldPath(void* sourceObject, void *targetObject) const;

		FieldInfo* FindFieldInfo(void* sourceObject, void *targetObject, String &res) const;

		// Initializes type parameters
		static void Initialize(Type& type, const String& name, UInt id, IObject* sample);

		// Adds basic type
		static void SetupBaseType(Type& type, Type* baseType);

	protected:
		String        mName;      // Name of object type
		FieldInfosVec mFields;    // Fields information
		Id            mId;        // Id of type
		TypesVec      mBaseTypes; // Base types ids
		Ptr<IObject>  mSample;    // Object sample

		friend class FieldInfo;

		template<typename _type>
		friend class AccessorFieldInfo;
	};


	template<typename _type>
	FieldInfo& Type::RegField(const String& name, UInt offset, Ptr<_type> value)
	{
		Ptr<Type> type = std::conditional<std::is_base_of<IObject, _type>::value, _type, Dummy>::type::type;
		mFields.Add(mnew FieldInfo(name, offset, false, true, type));
		return *mFields.Last();
	}

	template<typename _type>
	FieldInfo& Type::RegField(const String& name, UInt offset, _type& value)
	{
		Ptr<Type> type = std::conditional<std::is_base_of<IObject, _type>::value, _type, Dummy>::type::type;
		mFields.Add(mnew FieldInfo(name, offset, false, false, type));
		return *mFields.Last();
	}

	template<typename _type>
	FieldInfo& Type::RegField(const String& name, UInt offset, Property<_type>& value)
	{
		Ptr<Type> type = std::conditional<std::is_base_of<IObject, _type>::value, _type, Dummy>::type::type;
		mFields.Add(mnew FieldInfo(name, offset, true, false, type));
		return *mFields.Last();
	}

	template<typename _type>
	FieldInfo& Type::RegField(const String& name, UInt offset, Accessor<Ptr<_type>, const String&>& value)
	{
		Ptr<Type> type = std::conditional<std::is_base_of<IObject, _type>::value, _type, Dummy>::type::type;
		mFields.Add(mnew AccessorFieldInfo<_type>(name, offset, type));
		return *mFields.Last();
	}

	template<typename _type>
	_type* Type::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		for (auto field : mFields)
		{
			if (field->mName == pathPart)
			{
				if (delPos == -1)
				{
					fieldInfo = field;
					return field->GetValuePtr<_type>(object);
				}
				else
				{
					char* val = field->GetValuePtr<char>(object);

					if (!val)
						return nullptr;

					return (_type*)(field->SearchFieldPtr(val, path.SubStr(delPos + 1), fieldInfo));
				}
			}
		}

		return nullptr;
	}
}
