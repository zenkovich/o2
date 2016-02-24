#include "ReparentActors.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"

EditorReparentActorsAction::EditorReparentActorsAction()
{}

EditorReparentActorsAction::EditorReparentActorsAction(const Vector<Actor*>& beginActors)
{
	for (auto actor : beginActors)
	{
		ActorInfo* info = mnew ActorInfo();
		Actor* parent = actor->GetParent();
		Vector<Actor*> parentChilds = parent ? parent->GetChilds() : o2Scene.GetRootActors();
		int actorIdx = parentChilds.Find(actor);

		info->actorId = actor->GetId();
		info->actorHierarchyIdx = o2Scene.GetActorHierarchyIdx(actor);
		info->lastParentId = parent ? parent->GetId() : 0;
		info->lastPrevActorId = actorIdx > 0 ? parentChilds[actorIdx - 1]->GetId() : 0;
		info->transform = actor->transform.GetWorldNonSizedBasis();

		actorsInfos.Add(info);
	}

	actorsInfos.Sort([](ActorInfo* a, ActorInfo* b) { return a->actorHierarchyIdx < b->actorHierarchyIdx; });
}

EditorReparentActorsAction::~EditorReparentActorsAction()
{
	for (auto info : actorsInfos)
		delete info;
}

void EditorReparentActorsAction::ActorsReparented(Actor* newParent, Actor* prevActor)
{
	newParentId = newParent ? newParent->GetId() : 0;
	newPrevActorId = prevActor ? prevActor->GetId() : 0;
}

String EditorReparentActorsAction::GetName() const
{
	return "Actors rearrange";
}

void EditorReparentActorsAction::Redo()
{
	Actor* parent = o2Scene.GetActorByID(newParentId);
	Actor* prevActor = o2Scene.GetActorByID(newPrevActorId);

	if (parent)
	{
		int insertIdx = parent->GetChilds().Find(prevActor) + 1;

		for (auto info : actorsInfos)
		{
			Actor* actor = o2Scene.GetActorByID(info->actorId);

			actor->SetParent(nullptr);
			parent->AddChild(actor, insertIdx++);
			actor->transform.SetWorldNonSizedBasis(info->transform);
		}
	}
	else
	{
		int insertIdx = o2Scene.GetRootActors().Find(prevActor) + 1;

		for (auto info : actorsInfos)
		{
			Actor* actor = o2Scene.GetActorByID(info->actorId);

			actor->SetParent(nullptr);
			actor->SetPositionIndexInParent(insertIdx++);
			actor->transform.SetWorldNonSizedBasis(info->transform);
		}
	}

	o2EditorTree.GetActorsTree()->RebuildTree();
}

void EditorReparentActorsAction::Undo()
{
	for (auto info : actorsInfos)
	{
		Actor* actor = o2Scene.GetActorByID(info->actorId);
		Actor* parent = o2Scene.GetActorByID(info->lastParentId);
		Actor* prevActor = o2Scene.GetActorByID(info->lastPrevActorId);

		actor->SetParent(nullptr);

		if (parent)
		{
			int idx = parent->GetChilds().Find(prevActor) + 1;
			parent->AddChild(actor, idx);
			actor->transform.SetWorldNonSizedBasis(info->transform);
		}
		else
		{
			int idx = o2Scene.GetRootActors().Find(prevActor) + 1;
			actor->SetPositionIndexInParent(idx);
			actor->transform.SetWorldNonSizedBasis(info->transform);
		}
	}

	o2EditorTree.GetActorsTree()->RebuildTree();
}
