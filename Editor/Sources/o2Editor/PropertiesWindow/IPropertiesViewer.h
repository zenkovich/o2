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
		virtual void SetTargets(const Vector<IObject*>& targets) {}

		// Refreshes viewing properties
		virtual void Refresh();

		// Sets viewer enabled
		void SetEnabled(bool enabled);

		// Returns is viewer enabled
		bool IsEnabled() const;

		IOBJECT(IPropertiesViewer);

	protected:
		Ref<Widget> mContentWidget; // Data content widget (turning on/off on enabling/disabling)

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
// --- META ---

CLASS_BASES_META(Editor::IPropertiesViewer)
{
    BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IPropertiesViewer)
{
    FIELD().PROTECTED().NAME(mContentWidget);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mEnabled);
}
END_META;
CLASS_METHODS_META(Editor::IPropertiesViewer)
{

    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargets, const Vector<IObject*>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
    FUNCTION().PROTECTED().SIGNATURE(void, Update, float);
    FUNCTION().PROTECTED().SIGNATURE(void, Draw);
}
END_META;
// --- END META ---
