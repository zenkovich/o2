#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"
#include "Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Widget;
	class WidgetLayer;
}

namespace Editor
{
	class SpoilerWithHead;

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
		virtual void SetTargetLayers(const Vector<WidgetLayer*>& layers) {}

		// Returns viewing layer drawable type 
		virtual const Type* GetDrawableType() const { return nullptr; }

		// Returns data widget
		virtual Widget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates all actor values
		virtual void Refresh();

		// Returns is there no properties
		virtual bool IsEmpty() const;
		IOBJECT(IWidgetLayerPropertiesViewer);

	protected:
		SpoilerWithHead* mSpoiler = nullptr;
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

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	PUBLIC_FUNCTION(const Type*, GetDrawableType);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(bool, IsEmpty);
}
END_META;
