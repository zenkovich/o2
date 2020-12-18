#pragma once

#include "o2Editor/PropertiesWindow/IPropertiesViewer.h"

using namespace o2;

namespace Editor
{
	class ObjectViewer;

	// -------------------------
	// Default properties viewer
	// -------------------------
	class DefaultPropertiesViewer : public IPropertiesViewer
	{
	public:
		DefaultPropertiesViewer();

		// Virtual destructor
		~DefaultPropertiesViewer();

		// Updates properties values
		void Refresh() override;

		IOBJECT(DefaultPropertiesViewer);

	protected:
		ObjectViewer* mViewer; // Object viewer

		Vector<IObject*> mTargets; // Viewing targets

	protected:
		// Sets target objects
		void SetTargets(const Vector<IObject*> targets) override;
	};

}

CLASS_BASES_META(Editor::DefaultPropertiesViewer)
{
	BASE_CLASS(Editor::IPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultPropertiesViewer)
{
	FIELD().NAME(mViewer).PROTECTED();
	FIELD().NAME(mTargets).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::DefaultPropertiesViewer)
{

	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, SetTargets, const Vector<IObject*>);
}
END_META;
