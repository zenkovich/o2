#pragma once

#include "UIDockableWindow.h"
#include "Utils/Serializable.h"

using namespace o2;

namespace Editor
{
	// -----------------------------
	// Basic editor window interface
	// -----------------------------
	class IEditorWindow: public IObject
	{
	public:
		Property<bool> visible; // Window visibility property

		// Default constructor
		IEditorWindow();

		// Copy-constructor
		IEditorWindow(const IEditorWindow& other);

		// Virtual destructor
		virtual ~IEditorWindow();

		// Sets visibility
		virtual void SetVisible(bool visible);

		// Updates window logic
		virtual void Update(float dt);

		// Draws window stuff
		virtual void Draw();

		// Returns is window is visible
		bool IsVisible();

		// Shows window
		void Show();

		// Hides window
		void Hide();

		IOBJECT(IEditorWindow);

	protected:
		UIDockableWindow* mWindow; // Dockable UI window 

	protected:
		// Calls after that all windows was created
		virtual void PostInitializeWindow() {}

		friend class WindowsManager;
		friend class WindowsLayout;
	};
}
