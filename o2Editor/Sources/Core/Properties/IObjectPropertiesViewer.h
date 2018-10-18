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
		// Refreshing controls and properties by target objects
		virtual void Refresh(const TargetsVec& targetObjets, const String& path, const OnChangeCompletedFunc& onChangeCompleted,
							 const OnChangedFunc& onChanged);

		// Returns viewing objects base type
		virtual const Type* GetViewingObjectType() const;

		// Returns view widget
		UIWidget* GetViewWidget() const;

		IOBJECT(IObjectPropertiesViewer);

	protected:
		UIWidget* mViewWidget = nullptr; // View layout
	};
}

CLASS_BASES_META(Editor::IObjectPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IObjectPropertiesViewer)
{
	PROTECTED_FIELD(mViewWidget);
}
END_META;
CLASS_METHODS_META(Editor::IObjectPropertiesViewer)
{

	PUBLIC_FUNCTION(UIWidget*, InitializeControls, const String&, const OnChangeCompletedFunc&, const OnChangedFunc&);
	PUBLIC_FUNCTION(void, Refresh, const TargetsVec&);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_FUNCTION(UIWidget*, GetViewWidget);
}
END_META;
