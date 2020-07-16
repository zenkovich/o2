#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Reflection/Type.h"

using namespace o2;

namespace o2
{
	class Widget;
}

namespace Editor
{
	// ----------------------------------
	// Object properties viewer interface
	// ----------------------------------
	class IPropertiesViewer: public IObject
	{
	public:
		// Virtual destructor
		virtual ~IPropertiesViewer();

		// Returns viewing object type
		virtual const Type* GetViewingObjectType() const;

		// Refreshes viewing properties
		virtual void Refresh();

		IOBJECT(IPropertiesViewer);

	protected:
		Widget* mContentWidget; // Data content widget (turning on/off on enabling/disabling)

	protected:
		// Sets target objects
		virtual void SetTargets(const Vector<IObject*> targets) {}

		// Enable viewer event function
		virtual void OnEnabled() {}

		// Disable viewer event function
		virtual void OnDisabled() {}

		// Updates viewer
		virtual void Update(float dt) {}

		// Draws something
		virtual void Draw() {}

		friend class PropertiesWindow;
	};
}

CLASS_BASES_META(Editor::IPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IPropertiesViewer)
{
	PROTECTED_FIELD(mContentWidget);
}
END_META;
CLASS_METHODS_META(Editor::IPropertiesViewer)
{

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, SetTargets, const Vector<IObject*>);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, Draw);
}
END_META;
