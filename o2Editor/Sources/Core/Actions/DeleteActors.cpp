#include "DeleteActors.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"

namespace Editor
{

	DeleteActorsAction::DeleteActorsAction()
	{}

	DeleteActorsAction::DeleteActorsAction(const Vector<Actor*>& deletingActors)
	{
		for (auto actor : deletingActors)
		{
			ActorInfo info;
			info.actorData.SetValueRaw(actor);
			info.idx = GetActorIdx(actor);

			if (actor->GetParent())
			{
				auto parent = actor->GetParent();
				info.parentId = parent->GetID();

				auto parentChilds = parent->GetChildren();
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

	DeleteActorsAction::~DeleteActorsAction()
	{}

	String DeleteActorsAction::GetName() const
	{
		return "Actors deletion";
	}

	void DeleteActorsAction::Redo()
	{
		for (auto info : actorsInfos)
		{
			auto sceneActor = o2Scene.GetActorByID((UInt64)info.actorData["Value"]["Id"]);
			if (sceneActor)
				delete sceneActor;
		}

		o2EditorSceneScreen.ClearSelectionWithoutAction();
		o2EditorTree.GetActorsTree()->UpdateNodesView();
	}

	void DeleteActorsAction::Undo()
	{
		for (auto info : actorsInfos)
		{
			Actor* parent = o2Scene.GetActorByID(info.parentId);
			if (parent)
			{
				UInt64 prevId = info.prevActorId;
				int idx = parent->GetChildren().FindIdx([=](Actor* x) { return x->GetID() == prevId; }) + 1;

				Actor* newActor;
				info.actorData.GetValueRaw(newActor);
				parent->AddChild(newActor, idx);

				o2EditorSceneScreen.SelectActorWithoutAction(newActor);
			}
			else
			{
				int idx = o2Scene.GetRootActors().FindIdx([&](Actor* x) { return x->GetID() == info.prevActorId; }) + 1;

				Actor* newActor;
				info.actorData.GetValueRaw(newActor);
				newActor->SetPositionIndexInParent(idx);

				o2EditorSceneScreen.SelectActorWithoutAction(newActor);
			}
		}

		o2EditorTree.GetActorsTree()->UpdateNodesView();
	}

	bool DeleteActorsAction::ActorInfo::operator==(const ActorInfo& other) const
	{
		return actorData == other.actorData && parentId == other.parentId && prevActorId == other.prevActorId;
	}

	int DeleteActorsAction::GetActorIdx(Actor* actor)
	{
		if (actor->GetParent())
		{
			return actor->GetParent()->GetChildren().Find(actor) + GetActorIdx(actor->GetParent());
		}

		return o2Scene.GetRootActors().Find(actor);
	}
}

CLASS_META(Editor::DeleteActorsAction)
{
	BASE_CLASS(Editor::IAction);

	PUBLIC_FIELD(actorsInfos);

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
	PROTECTED_FUNCTION(int, GetActorIdx, Actor*);
}
END_META;

CLASS_META(Editor::DeleteActorsAction::ActorInfo)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(actorData).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(parentId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(prevActorId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(idx).SERIALIZABLE_ATTRIBUTE();
}
END_META;
