#include "Selection.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	SelectionAction::SelectionAction()
	{}

	SelectionAction::SelectionAction(const Vector<Actor*>& selectedActors, const Vector<Actor*>& prevSelectedActors)
	{
		selectedActorsIds = selectedActors.Select<UInt64>([](Actor* actor) { return actor->GetId(); });
		prevSelectedActorsIds = prevSelectedActors.Select<UInt64>([](Actor* actor) { return actor->GetId(); });
	}

	SelectionAction::~SelectionAction()
	{}

	String SelectionAction::GetName() const
	{
		return "Actors selection";
	}

	void SelectionAction::Redo()
	{
		auto& selScreen = o2EditorSceneScreen;

		selScreen.mSelectedActors = selectedActorsIds.Select<Actor*>([&](UInt64 id) { return o2Scene.GetActorByID(id); });
		selScreen.UpdateTopSelectedActors();
		selScreen.OnActorsSelectedFromThis();
		selScreen.mNeedRedraw = true;
	}

	void SelectionAction::Undo()
	{
		auto& selScreen = o2EditorSceneScreen;

		selScreen.mSelectedActors = prevSelectedActorsIds.Select<Actor*>([&](UInt64 id) { return o2Scene.GetActorByID(id); });
		selScreen.UpdateTopSelectedActors();
		selScreen.OnActorsSelectedFromThis();
		selScreen.mNeedRedraw = true;
	}

}