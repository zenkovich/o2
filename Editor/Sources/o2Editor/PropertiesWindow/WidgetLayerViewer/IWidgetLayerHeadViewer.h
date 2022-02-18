#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Widget;
	class WidgetLayer;
}

namespace Editor
{
	// -------------------------------------------
	// Editor widget layer header viewer interface
	// -------------------------------------------
	class IWidgetLayerHeaderViewer : public IObject
	{
	public:
		// Virtual destructor
		virtual ~IWidgetLayerHeaderViewer() {}

		// Sets target actors
		virtual void SetTargetLayers(const Vector<WidgetLayer*>& layers) {}

		// Returns data widget
		virtual Widget* GetWidget() const { return nullptr; }

		// Updates properties values
		virtual void Refresh() {}

		// Sets viewer enabled
		void SetEnabled(bool enabled);

		// Returns is viewer enabled
		bool IsEnabled() const;

		IOBJECT(IWidgetLayerHeaderViewer);

	protected:
		bool mEnabled = false; // Is viewer enabled 

	protected:
		// Enable viewer event function
		virtual void OnEnabled() {}

		// Disable viewer event function
		virtual void OnDisabled() {}
	};
}

CLASS_BASES_META(Editor::IWidgetLayerHeaderViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IWidgetLayerHeaderViewer)
{
	FIELD().DEFAULT_VALUE(false).NAME(mEnabled).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::IWidgetLayerHeaderViewer)
{

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
}
END_META;
