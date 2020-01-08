#pragma once

#include "o2Editor/Core/Actions/IAction.h"
#include "o2/Scene/ActorTransform.h"
#include "o2/Utils/Math/Basis.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
}

namespace Editor
{
	// ---------------------------
	// Objects transforming action
	// ---------------------------
	class TransformAction: public IAction
	{
	public:
		struct Transform
		{
			Basis  transform;
			Layout layout;

			bool operator==(const Transform& other) const;
		};

		Vector<SceneUID>  objectsIds;
		Vector<Transform> beforeTransforms;
		Vector<Transform> doneTransforms;

	public:
		// Default constructor
		TransformAction();

		// Constructor with objects, stores theirs before changing transforms
		TransformAction(const Vector<SceneEditableObject*>& objects);

		// It is called when transform completed, stores changed transforms
		void Completed();

		// Returns name of action
		String GetName() const;

		// Sets new transforms again
		void Redo();

		// Sets transformations before transform
		void Undo();

		SERIALIZABLE(TransformAction);

	private:
		// Gets objects transforms and puts into transforms vector
		void GetTransforms(const Vector<SceneUID>& objectIds, Vector<Transform>& transforms);

		// Sets transformations and layouts to objects
		void SetTransforms(const Vector<SceneUID>& objectIds, Vector<Transform>& transforms);
	};
}

CLASS_BASES_META(Editor::TransformAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::TransformAction)
{
	PUBLIC_FIELD(objectsIds);
	PUBLIC_FIELD(beforeTransforms);
	PUBLIC_FIELD(doneTransforms);
}
END_META;
CLASS_METHODS_META(Editor::TransformAction)
{

	PUBLIC_FUNCTION(void, Completed);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
	PRIVATE_FUNCTION(void, GetTransforms, const Vector<SceneUID>&, Vector<Transform>&);
	PRIVATE_FUNCTION(void, SetTransforms, const Vector<SceneUID>&, Vector<Transform>&);
}
END_META;
