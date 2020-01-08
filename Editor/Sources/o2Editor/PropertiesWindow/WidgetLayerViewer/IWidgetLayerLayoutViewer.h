#pragma once

#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Reflection/Type.h"

using namespace o2;

namespace o2
{
	class Widget;
	class WidgetLayer;
}

namespace Editor
{
	class SpoilerWithHead;

	// -------------------------------------------
	// Editor widget layer layout viewer interface
	// -------------------------------------------
	class IWidgetLayerLayoutViewer : public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IWidgetLayerLayoutViewer();

		// Virtual destructor
		virtual ~IWidgetLayerLayoutViewer();

		// Sets target actors
		virtual void SetTargetLayers(const Vector<WidgetLayer*>& layers) {}

		// Returns data widget
		virtual Widget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates properties values
		virtual void Refresh();

		IOBJECT(IWidgetLayerLayoutViewer);

	protected:
		SpoilerWithHead* mSpoiler;
	};
}

CLASS_BASES_META(Editor::IWidgetLayerLayoutViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IWidgetLayerLayoutViewer)
{
	PROTECTED_FIELD(mSpoiler);
}
END_META;
CLASS_METHODS_META(Editor::IWidgetLayerLayoutViewer)
{

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
