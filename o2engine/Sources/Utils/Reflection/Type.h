#pragma once

#include "Utils/String.h"
#include "Utils/Reflection/FieldInfo.h"
#include "Utils/Memory/Ptr.h"

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

		static Type unknown;

		struct Dummy
		{
			static Type type;
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
		const Vector<Type*>& BaseTypes() const;

		// Returns fields informations array
		const Vector<FieldInfo*>& Fields() const;

		// Returns field information by name
		const FieldInfo* Field(const String& name) const;

		// Returns inherited types
		Vector<Type*> InheritedTypes() const;

		// Returns sample pointer
		const IObject* Sample() const;

		// Registers field in type
		template<typename _type>
		FieldInfo& RegField(const String& name, UInt offset, Ptr<_type>& value);

		// Registers field in type
		template<typename _type>
		FieldInfo& RegField(const String& name, UInt offset, _type& value);

		template<typename _type>
		_type* GetFieldPtr(void* object, const String& path) const 
		{
			int delPos = path.Find("/");
			WString pathPart = path.SubStr(0, delPos);

			for (auto field : mFields)
			{
				if (field->mName == pathPart)
				{
					if (delPos == -1)
						return field->GetValuePtr<_type>(object);
					else
						return field->mType->GetFieldPtr<_type>(field->GetValuePtr<_type>(object), path.SubStr(delPos + 1));
				}
			}

			return nullptr;
		}

		// Initializes type parameters
		static void Initialize(Type& type, const String& name, UInt id, IObject* sample);

		// Adds basic type
		static void SetupBaseType(Type& type, Type* baseType);

	protected:
		String             mName;      // Name of object type
		Vector<FieldInfo*> mFields;    // Fields information
		Id                 mId;        // Id of type
		Vector<Type*>      mBaseTypes; // Base types ids
		IObject*           mSample;    // Object sample
	};


	// Registers field in type
	template<typename _type>
	FieldInfo& Type::RegField(const String& name, UInt offset, Ptr<_type>& value)
	{
		Type* type = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Dummy>::type::type;
		mFields.Add(new FieldInfo(name, offset, false, true, type));
		return *mFields.Last();
	}

	// Registers field in type
	template<typename _type>
	FieldInfo& Type::RegField(const String& name, UInt offset, _type& value)
	{
		Type* type = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Dummy>::type::type;
		mFields.Add(new FieldInfo(name, offset, false, false, type));
		return *mFields.Last();
	}
}
