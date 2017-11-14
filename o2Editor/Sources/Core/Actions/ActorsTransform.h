#pragma once

#include "Core/Actions/IAction.h"
#include "Scene/ActorTransform.h"
#include "Utils/Math/Basis.h"

using namespace o2;

namespace o2
{
	class Actor;
}

namespace Editor
{
	class ActorsTransformAction: public IAction
	{
	public:
		Vector<UInt64>         actorsIds;
		Vector<ActorTransform> beforeTransforms;
		Vector<ActorTransform> doneTransforms;

	public:
		ActorsTransformAction();
		ActorsTransformAction(const Vector<Actor*>& actors, const Vector<ActorTransform>& beforeTransforms);

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(ActorsTransformAction);
	};
}

CLASS_BASES_META(Editor::ActorsTransformAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::ActorsTransformAction)
{
	PUBLIC_FIELD(actorsIds);
	PUBLIC_FIELD(beforeTransforms);
	PUBLIC_FIELD(doneTransforms);
}
END_META;
CLASS_METHODS_META(Editor::ActorsTransformAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
