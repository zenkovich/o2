#include "DeleteActors.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/Tree.h"

EditorDeleteActorsAction::EditorDeleteActorsAction()
{}

EditorDeleteActorsAction::EditorDeleteActorsAction(const Vector<Actor*>& deletingActors)
{
	for (auto actor : deletingActors)
	{
		ActorInfo info;
		info.actor = actor;
		info.idx = GetActorIdx(actor);

		if (actor->GetParent())
		{
			auto parent = actor->GetParent();
			info.parentId = parent->GetId();

			auto parentChilds = parent->GetChilds();
			info.prevActorId = 0;

			for (auto child : parentChilds)
			{
				if (child == actor)
					break;

				info.prevActorId = child->id;
			}
		}
		else
		{
			info.parentId = 0;

			auto sceneActors = o2Scene.GetRootActors();
			info.prevActorId = 0;

			for (auto child : sceneActors)
			{
				if (child == actor)
					break;

				info.prevActorId = child->id;
			}
		}

		actorsInfos.Add(info);
	}

	actorsInfos.Sort([](auto& a, auto& b) { return a.idx < b.idx; });
}

EditorDeleteActorsAction::~EditorDeleteActorsAction()
{}

String EditorDeleteActorsAction::GetName() const
{
	return "Actors deletion";
}

void EditorDeleteActorsAction::Redo()
{
	for (auto info : actorsInfos)
	{
		auto sceneActor = o2Scene.GetActorByID((UInt64)*info.actor["mId"]);
		if (sceneActor)
			delete sceneActor;
	}

	o2EditorSceneScreen.ClearSelectionWithoutAction();
	o2EditorTree.GetActorsTree()->RebuildTree();
}

void EditorDeleteActorsAction::Undo()
{
	for (auto info : actorsInfos)
	{
		Actor* parent = o2Scene.GetActorByID(info.parentId);
		if (parent)
		{
			UInt64 prevId = info.prevActorId;
			int idx = parent->GetChilds().FindIdx([=](Actor* x) { return x->GetId() == prevId; }) + 1;

			Actor* newActor = info.actor;
			parent->AddChild(newActor, idx);

			o2EditorSceneScreen.SelectActorWithoutAction(newActor);
		}
		else
		{
			int idx = o2Scene.GetRootActors().FindIdx([&](Actor* x) { return x->GetId() == info.prevActorId; }) + 1;

			Actor* newActor = info.actor;
			newActor->SetPositionIndexInParent(idx);

			o2EditorSceneScreen.SelectActorWithoutAction(newActor);
		}
	}

	o2EditorTree.GetActorsTree()->RebuildTree();
}

bool EditorDeleteActorsAction::ActorInfo::operator==(const ActorInfo& other) const
{
	return actor == other.actor && parentId == other.parentId && prevActorId == other.prevActorId;
}

int EditorDeleteActorsAction::GetActorIdx(Actor* actor)
{
	if (actor->GetParent())
	{
		return actor->GetParent()->GetChilds().Find(actor) + GetActorIdx(actor->GetParent());
	}

	return o2Scene.GetRootActors().Find(actor);
}