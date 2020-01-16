#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Types/Containers/Pair.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/ValueProxy.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

using namespace o2;

namespace o2
{
	class VerticalLayout;
}

namespace Editor
{
	class IPropertyField;

	// ----------------------------------------------------------------------------------
	// Object properties viewer interface. Used in IObjectProperty and IObjectPtrProperty
	// Override this class to create new object properties viewer
	// ----------------------------------------------------------------------------------
	class IObjectPropertiesViewer : public IObject
	{
	public:
		typedef Function<void(IPropertyField*)> OnChangedFunc;
		typedef Function<void(const String&, const Vector<DataNode>&, const Vector<DataNode>&)> OnChangeCompletedFunc;

	public:
		OnChangedFunc onChanged;                 // Immediate change value by user event
		OnChangeCompletedFunc onChangeCompleted; // Change completed by user event

		String path; // Path to viewing object fields

	public:
		// Default constructor
		IObjectPropertiesViewer();

		// Refreshing controls and properties by target objects
		virtual void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets);

		// Returns viewing objects base type
		virtual const Type* GetViewingObjectType() const;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		// Returns view widget
		VerticalLayout* GetLayout() const;

		// Returns is viewer empty
		bool IsEmpty() const;

		IOBJECT(IObjectPropertiesViewer);

	protected:
		VerticalLayout* mLayout = nullptr; // View layout

		PropertiesContext mPropertiesContext; // Field properties information

		OnChangeCompletedFunc mOnChildFieldChangeCompleted; // Default field change completed callback, calls
		                                                    // inChangeCompleted from this with full combined path

	protected:
		// It is called when some child field were changed
		void OnFieldChangeCompleted(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after);
	};

	template<typename _object_type>
	class TObjectPropertiesViewer : public IObjectPropertiesViewer
	{
	public:
		// Returns viewing objects base type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(TObjectPropertiesViewer<_object_type>);
	};

	template<typename _object_type>
	const Type* TObjectPropertiesViewer<_object_type>::GetViewingObjectType() const
	{
		return GetViewingObjectTypeStatic();
	}

	template<typename _object_type>
	const Type* TObjectPropertiesViewer<_object_type>::GetViewingObjectTypeStatic()
	{
		return &TypeOf(_object_type);
	}

}

CLASS_BASES_META(Editor::IObjectPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IObjectPropertiesViewer)
{
	PUBLIC_FIELD(onChanged);
	PUBLIC_FIELD(onChangeCompleted);
	PUBLIC_FIELD(path);
	PROTECTED_FIELD(mLayout);
	PROTECTED_FIELD(mPropertiesContext);
	PROTECTED_FIELD(mOnChildFieldChangeCompleted);
}
END_META;
CLASS_METHODS_META(Editor::IObjectPropertiesViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
	PUBLIC_FUNCTION(VerticalLayout*, GetLayout);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PROTECTED_FUNCTION(void, OnFieldChangeCompleted, const String&, const Vector<DataNode>&, const Vector<DataNode>&);
}
END_META;

META_TEMPLATES(typename _object_type)
CLASS_BASES_META(Editor::TObjectPropertiesViewer<_object_type>)
{
	BASE_CLASS(Editor::IObjectPropertiesViewer);
}
END_META;
META_TEMPLATES(typename _object_type)
CLASS_FIELDS_META(Editor::TObjectPropertiesViewer<_object_type>)
{
}
END_META;
META_TEMPLATES(typename _object_type)
CLASS_METHODS_META(Editor::TObjectPropertiesViewer<_object_type>)
{

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
}
END_META;
