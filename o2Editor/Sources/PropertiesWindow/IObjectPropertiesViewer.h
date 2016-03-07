#pragma once

#include "Utils/IObject.h"
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