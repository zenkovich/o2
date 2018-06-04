#pragma once

#include "Core/Actions/IAction.h"

using namespace o2;

namespace o2
{
	class Actor;
}

namespace Editor
{
	class ActorsPropertyChangeAction: public IAction
	{
	public:
		Vector<UInt64>   actorsIds;
		String           propertyPath;
		Vector<DataNode> beforeValues;
		Vector<DataNode> afterValues;

	public:
		ActorsPropertyChangeAction();
		ActorsPropertyChangeAction(const Vector<Actor*>& actors,
								   const String& propertyPath, const Vector<DataNode>& beforeValues,
								   const Vector<DataNode>& afterValues);

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(ActorsPropertyChangeAction);

	protected:
		void SetProperties(Vector<DataNode>& value);
	};
}

CLASS_BASES_META(Editor::ActorsPropertyChangeAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::ActorsPropertyChangeAction)
{
	PUBLIC_FIELD(actorsIds);
	PUBLIC_FIELD(propertyPath);
	PUBLIC_FIELD(beforeValues);
	PUBLIC_FIELD(afterValues);
}
END_META;
CLASS_METHODS_META(Editor::ActorsPropertyChangeAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
	PROTECTED_FUNCTION(void, SetProperties, Vector<DataNode>&);
}
END_META;
