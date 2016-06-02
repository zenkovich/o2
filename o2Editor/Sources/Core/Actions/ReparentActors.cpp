#include "ReparentActors.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"

namespace Editor
{
	ReparentActorsAction::ReparentActorsAction()
	{}

	ReparentActorsAction::ReparentActorsAction(const Vector<Actor*>& beginActors)
	{
		for (auto actor : beginActors)
		{
			ActorInfo* info = mnew ActorInfo();
			Actor* parent = actor->GetParent();
			Vector<Actor*> parentChilds = parent ? parent->GetChilds() : o2Scene.GetRootActors();
			int actorIdx = parentChilds.Find(actor);

			info->actorId = actor->GetID();
			info->actorHierarchyIdx = o2Scene.GetActorHierarchyIdx(actor);
			info->lastParentId = parent ? parent->GetID() : 0;
			info->lastPrevActorId = actorIdx > 0 ? parentChilds[actorIdx - 1]->GetID() : 0;
			info->transform = actor->transform.GetWorldNonSizedBasis();

			actorsInfos.Add(info);
		}

		actorsInfos.Sort([](ActorInfo* a, ActorInfo* b) { return a->actorHierarchyIdx < b->actorHierarchyIdx; });
	}

	ReparentActorsAction::~ReparentActorsAction()
	{
		for (auto info : actorsInfos)
			delete info;
	}

	void ReparentActorsAction::ActorsReparented(Actor* newParent, Actor* prevActor)
	{
		newParentId = newParent ? newParent->GetID() : 0;
		newPrevActorId = prevActor ? prevActor->GetID() : 0;
	}

	String ReparentActorsAction::GetName() const
	{
		return "Actors rearrange";
	}

	void ReparentActorsAction::Redo()
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

		o2EditorTree.GetActorsTree()->UpdateView();
	}

	void ReparentActorsAction::Undo()
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

		o2EditorTree.GetActorsTree()->UpdateView();
	}
}