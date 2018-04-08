#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"

using namespace o2;

namespace o2
{
	class UIWidget;
}

namespace Editor
{
	// ----------------------------------
	// Object properties viewer interface
	// ----------------------------------
	class IObjectPropertiesViewer: public IObject
	{
	public:
		// Virtual destructor
		virtual ~IObjectPropertiesViewer() {}

		// Returns viewing object type
		virtual const Type* GetViewingObjectType() const { return nullptr; }

		IOBJECT(IObjectPropertiesViewer);

	protected:
		UIWidget* mContentWidget; // Data content widget (turning on/off on enabling/disabling)

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

CLASS_BASES_META(Editor::IObjectPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IObjectPropertiesViewer)
{
	PROTECTED_FIELD(mContentWidget);
}
END_META;
CLASS_METHODS_META(Editor::IObjectPropertiesViewer)
{

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PROTECTED_FUNCTION(void, SetTargets, const Vector<IObject*>);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, Draw);
}
END_META;
