#pragma once

#include "o2/Utils/Editor/DragHandle.h"

using namespace o2;

namespace Editor
{
	// -----------------
	// Scene drag handle
	// -----------------
	class SceneDragHandle: public DragHandle
	{
	public:
		// Default constructor
		SceneDragHandle();

		// Constructor with views
		SceneDragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr);

		// Copy-constructor
		SceneDragHandle(const SceneDragHandle& other);

		// Destructor
		~SceneDragHandle();

		// Copy-operator
		SceneDragHandle& operator=(const SceneDragHandle& other);

		// Set handle enabled. Disabled handle don't drawn and interact
		void SetEnabled(bool enabled);

		// Converts point from screen to local space
		Vec2F ScreenToLocal(const Vec2F& point);

		// Converts point from local to screen space
		Vec2F LocalToScreen(const Vec2F& point);

		SERIALIZABLE(SceneDragHandle);

	protected:
		friend class SceneEditScreen;
	};
}

CLASS_BASES_META(Editor::SceneDragHandle)
{
	BASE_CLASS(o2::DragHandle);
}
END_META;
CLASS_FIELDS_META(Editor::SceneDragHandle)
{
}
END_META;
CLASS_METHODS_META(Editor::SceneDragHandle)
{

	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(Vec2F, ScreenToLocal, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, LocalToScreen, const Vec2F&);
}
END_META;
