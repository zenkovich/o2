#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Types/Containers/Pair.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/ValueProxy.h"

using namespace o2;

namespace o2
{
	class UIVerticalLayout;
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
		typedef Vector<Pair<IObject*, IObject*>> TargetsVec;

		typedef Function<void(IPropertyField*)> OnChangedFunc;
		typedef Function<void(const String&, const Vector<DataNode>&, const Vector<DataNode>&)> OnChangeCompletedFunc;

	public:
		OnChangedFunc         onChanged;         // Immediate change value by user event
		OnChangeCompletedFunc onChangeCompleted; // Change completed by user event

		String                path;              // Path to viewing object fields

	public:
		// Default constructor
		IObjectPropertiesViewer();

		// Refreshing controls and properties by target objects
		virtual void Refresh(const TargetsVec& targetObjets);

		// Returns viewing objects base type
		virtual const Type* GetViewingObjectType() const;

		// Returns view widget
		UIWidget* GetViewWidget() const;

		IOBJECT(IObjectPropertiesViewer);

	protected:
		UIWidget* mViewWidget = nullptr; // View layout

		OnChangeCompletedFunc mOnChildFieldChangeCompleted; // Default field change completed callback, calls
		                                                    // inChangeCompleted from this with full combined path

	protected:
		// It is called when some child field were changed
		void OnFieldChangeCompleted(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after);
	};
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
	PROTECTED_FIELD(mViewWidget);
	PROTECTED_FIELD(mOnChildFieldChangeCompleted);
}
END_META;
CLASS_METHODS_META(Editor::IObjectPropertiesViewer)
{

	PUBLIC_FUNCTION(void, Refresh, const TargetsVec&);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_FUNCTION(UIWidget*, GetViewWidget);
	PROTECTED_FUNCTION(void, OnFieldChangeCompleted, const String&, const Vector<DataNode>&, const Vector<DataNode>&);
}
END_META;
