#pragma once

#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/SmartPointers/Ref.h"
#include "o2/Utils/SmartPointers/WeakRef.h"

using namespace o2;

namespace Editor
{
	// -----------------------------
	// Basic editor action interface
	// -----------------------------
	class IAction : public ISerializable
	{
	public:
		// Virtual destructor
		virtual ~IAction() {}

		// Returns name of action
		virtual String GetName() const { return "Unknown"; }

		// Does action again
		virtual void Redo() {}

		// Undoing action
		virtual void Undo() {}

		SERIALIZABLE(IAction);
	};
}

// --- META ---

CLASS_BASES_META(Editor::IAction)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;

CLASS_FIELDS_META(Editor::IAction) {}
END_META;

CLASS_METHODS_META(Editor::IAction)
{
	REFLECTION_METHOD().PUBLIC().SIGNATURE(String, GetName).CONST();
	REFLECTION_METHOD().PUBLIC().SIGNATURE(void, Redo);
	REFLECTION_METHOD().PUBLIC().SIGNATURE(void, Undo);
}
END_META;

// --- END META ---