#pragma once

#include "Utils/DragHandle.h"

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

		SERIALIZABLE(SceneDragHandle);

	protected:
		// Converts point from screen to local space
		Vec2F ScreenToLocal(const Vec2F& point);

		// Converts point from local to screen space
		Vec2F LocalToScreen(const Vec2F& point);

		friend class SceneEditScreen;
	};
}
