#include "Selection.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

EditorSelectionAction::EditorSelectionAction()
{}

EditorSelectionAction::EditorSelectionAction(const Vector<Actor*>& selectedActors, const Vector<Actor*>& prevSelectedActors)
{
	selectedActorsIds = selectedActors.Select<UInt64>([](Actor* actor) { return actor->GetId(); });
	prevSelectedActorsIds = prevSelectedActors.Select<UInt64>([](Actor* actor) { return actor->GetId(); });
}

EditorSelectionAction::~EditorSelectionAction()
{}

String EditorSelectionAction::GetName() const
{
	return "Actors selection";
}

void EditorSelectionAction::Redo()
{
	auto& selScreen = o2EditorSceneScreen;

	selScreen.mSelectedActors = selectedActorsIds.Select<Actor*>([&](UInt64 id) { return o2Scene.GetActorByID(id); });
	selScreen.UpdateTopSelectedActors();
	selScreen.OnActorsSelectedFromThis();
	selScreen.mNeedRedraw = true;
}

void EditorSelectionAction::Undo()
{
	auto& selScreen = o2EditorSceneScreen;

	selScreen.mSelectedActors = prevSelectedActorsIds.Select<Actor*>([&](UInt64 id) { return o2Scene.GetActorByID(id); });
	selScreen.UpdateTopSelectedActors();
	selScreen.OnActorsSelectedFromThis();
	selScreen.mNeedRedraw = true;
}
