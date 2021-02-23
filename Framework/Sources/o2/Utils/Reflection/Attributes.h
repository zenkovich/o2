#pragma once

namespace o2
{
	class FieldInfo;
	class Type;

	// ----------------------------------------
	// Attribute interface. Using in reflection
	// ----------------------------------------
	class IAttribute
	{
	public:
		// Default constructor
		IAttribute(): mOwnerFieldInfo(nullptr) {}

		// Virtual destructor
		virtual ~IAttribute() {}

		template<typename _base>
		struct SerializeProcessorMixin: public _base
		{
			template<typename _object_type, typename _field_type>
			SerializeProcessorMixin<_base>& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
														_field_type& field)
			{
				_base::FieldBasics(object, type, name, pointerGetter, field);
				return *this;
			}
		};

	protected:
		FieldInfo* mOwnerFieldInfo; // Pointer to owner field info object

		friend class FieldInfo;
	};

#define ATTRIBUTE_COMMENT_DEFINITION(X)
#define ATTRIBUTE_SHORT_DEFINITION(X)
}
