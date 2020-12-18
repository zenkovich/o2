#pragma once

#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2Editor/Core/Actions/IAction.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
}

namespace Editor
{
	class DeleteAction: public IAction
	{
	public:
		class ObjectInfo: public ISerializable
		{
		public:
			DataDocument objectData;   // @SERIALIZABLE
			SceneUID     parentId;	   // @SERIALIZABLE
			SceneUID     prevObjectId; // @SERIALIZABLE
			int          idx;          // @SERIALIZABLE

			bool operator==(const ObjectInfo& other) const;

			SERIALIZABLE(ObjectInfo);
		};

	public:
		Vector<ObjectInfo> objectsInfos;

	public:
		// Default constructor
		DeleteAction();

		// Constructor with objects, that will be deleted
		DeleteAction(const Vector<SceneEditableObject*>& objects);

		// Returns name of action
		String GetName() const override;

		// Deletes objects again
		void Redo() override;

		// Reverting deleted objects
		void Undo() override;

		SERIALIZABLE(DeleteAction);
	};
}

CLASS_BASES_META(Editor::DeleteAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::DeleteAction)
{
	FIELD().NAME(objectsInfos).PUBLIC();
}
END_META;
CLASS_METHODS_META(Editor::DeleteAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::DeleteAction::ObjectInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::DeleteAction::ObjectInfo)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(objectData).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(parentId).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(prevObjectId).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(idx).PUBLIC();
}
END_META;
CLASS_METHODS_META(Editor::DeleteAction::ObjectInfo)
{
}
END_META;
