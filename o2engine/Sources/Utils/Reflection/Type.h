#pragma once

#include "Utils/String.h"
#include "Utils/Reflection/FieldInfo.h"

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

	protected:
		String             mName;      // Name of object type
		Vector<FieldInfo*> mFields;    // Fields information
		Id                 mId;        // Id of type
		Vector<Type*>      mBaseTypes; // Base types ids
		IObject*           mSample;    // Object sample

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
		FieldInfo& RegField(const String& name, UInt offset);

		// Initializes type parameters
		static void Initialize(Type& type, const String& name, UInt id, IObject* sample);

		// Adds basic type
		static void SetupBaseType(Type& type, Type* baseType);
	};
}
