#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/Camera.h"
#include "UI/Widget.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace Editor
{
	class SceneEditScreen;

	// --------------------
	// Scene editing widget
	// --------------------
	class SceneEditWidget: public UIWidget
	{

	public:
		// Default constructor
		SceneEditWidget();

		// Copy-constructor
		SceneEditWidget(const SceneEditWidget& other);

		// Destructor
		~SceneEditWidget();

		// Copy-operator
		SceneEditWidget& operator=(const SceneEditWidget& other);

		// Draws widget
		void Draw();

		// Updates drawables, states and widget
		void Update(float dt);

		SERIALIZABLE(SceneEditWidget);

	protected:
		SceneEditScreen* mSceneEditScreen; // Scene editing screen

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);
	};
}