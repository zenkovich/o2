#pragma once

#include "Utils/Serialization/Serializable.h"

using namespace o2;

namespace Editor
{
	class IAction: public ISerializable
	{
	public:
		virtual ~IAction() {}

		virtual String GetName() const { return "Unknown"; }
		virtual void Redo() {}
		virtual void Undo() {}

		SERIALIZABLE(IAction);
	};
}

CLASS_BASES_META(Editor::IAction)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::IAction)
{
}
END_META;
CLASS_METHODS_META(Editor::IAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
