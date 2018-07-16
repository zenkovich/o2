#pragma once

#include "Utils/Types/Containers/Vector.h"
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"

using namespace o2;

namespace o2
{
	class UIWidget;
	class UIWidgetLayer;
}

namespace Editor
{
	class UISpoilerWithHead;

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
		virtual void SetTargetLayers(const Vector<UIWidgetLayer*>& layers) {}

		// Returns data widget
		virtual UIWidget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates properties values
		virtual void Refresh();

		IOBJECT(IWidgetLayerLayoutViewer);

	protected:
		UISpoilerWithHead* mSpoiler;
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

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<UIWidgetLayer*>&);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
