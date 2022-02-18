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

		// Sets target objects
		virtual void SetTargets(const Vector<IObject*> targets) {}

		// Refreshes viewing properties
		virtual void Refresh();

		// Sets viewer enabled
		void SetEnabled(bool enabled);

		// Returns is viewer enabled
		bool IsEnabled() const;

		IOBJECT(IPropertiesViewer);

	protected:
		Widget* mContentWidget; // Data content widget (turning on/off on enabling/disabling)

		bool mEnabled = false; // Is viewer enabled

	protected:
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
	FIELD().NAME(mContentWidget).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mEnabled).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::IPropertiesViewer)
{

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_FUNCTION(void, SetTargets, const Vector<IObject*>);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, Draw);
}
END_META;
