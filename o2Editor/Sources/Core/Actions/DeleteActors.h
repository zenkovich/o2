#pragma once

#include "Core/Actions/IAction.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Data/DataDoc.h"

using namespace o2;

namespace o2
{
	class Actor;
}

class EditorDeleteActorsAction: public IEditorAction
{
public:
	class ActorInfo: public ISerializable
	{
	public:
		DataNode actor;		  // @SERIALIZABLE
		UInt64   parentId;	  // @SERIALIZABLE
		UInt64   prevActorId; // @SERIALIZABLE
		int      idx;         // @SERIALIZABLE

		bool operator==(const ActorInfo& other) const;

		SERIALIZABLE(ActorInfo);
	};

public:
	Vector<ActorInfo> actorsInfos;

public:
	EditorDeleteActorsAction();
	EditorDeleteActorsAction(const Vector<Actor*>& deletingActors);
	~EditorDeleteActorsAction();

	String GetName() const;
	void Redo();
	void Undo();

	SERIALIZABLE(EditorDeleteActorsAction);

protected:
	int GetActorIdx(Actor* actor);
};