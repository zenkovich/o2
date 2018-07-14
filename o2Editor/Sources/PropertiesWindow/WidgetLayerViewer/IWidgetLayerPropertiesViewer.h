#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"
#include "Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class UIWidget;
	class UIWidgetLayer;
}

namespace Editor
{
	class UISpoilerWithHead;

	// -----------------------------------------------
	// Editor widget layer properties viewer interface
	// -----------------------------------------------
	class IWidgetLayerPropertiesViewer : public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IWidgetLayerPropertiesViewer();

		// Virtual destructor
		virtual ~IWidgetLayerPropertiesViewer();

		// Sets target actors
		virtual void SetTargetLayers(const Vector<UIWidgetLayer*>& layers) {}

		// Returns viewing layer drawable type 
		virtual const Type* GetDrawableType() const { return nullptr; }

		// Returns data widget
		virtual UIWidget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates all actor values
		virtual void Refresh();

		// Rebuilds properties layout
		virtual void Rebuild();

		// Returns is there no properties
		virtual bool IsEmpty() const;

		// Returns true when properties was built with hidden fields
		virtual bool IsBuiltWithHiddenFields() const;

		IOBJECT(IWidgetLayerPropertiesViewer);

	protected:
		UISpoilerWithHead* mSpoiler;
	};
}

CLASS_BASES_META(Editor::IWidgetLayerPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IWidgetLayerPropertiesViewer)
{
	PROTECTED_FIELD(mSpoiler);
}
END_META;
CLASS_METHODS_META(Editor::IWidgetLayerPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<UIWidgetLayer*>&);
	PUBLIC_FUNCTION(const Type*, GetDrawableType);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Rebuild);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PUBLIC_FUNCTION(bool, IsBuiltWithHiddenFields);
}
END_META;
