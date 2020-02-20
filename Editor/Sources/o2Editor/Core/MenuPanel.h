#pragma once

#include "o2/Scene/UI/Widgets/MenuPanel.h"
#include "o2/Utils/Singleton.h"

using namespace o2;

// Editors menu panel accessor macros
#define o2EditorMenu MenuPanel::Instance()

namespace Editor
{
	// -----------------
	// Editor menu panel
	// -----------------
	class MenuPanel: public Singleton<MenuPanel>
	{
	public:
		// Default constructor. Initializes basic menu items
		MenuPanel();

		// Destructor
		~MenuPanel();

		// Add item
		Widget* AddItem(const o2::MenuPanel::Item& item);

		// Adds item by path ("node/sub node/target")
		Widget* AddItem(const WString& path, const Function<void()>& clickFunc = Function<void()>(),
						  const ImageAssetRef& icon = ImageAssetRef(), const ShortcutKeys& shortcut = ShortcutKeys());

		// Inserts item at position
		Widget* InsertItem(const o2::MenuPanel::Item& item, int position);

		// Adds array of items
		void AddItems(Vector<o2::MenuPanel::Item> items);

		// Inserts array of items at position
		void InsertItems(Vector<o2::MenuPanel::Item> items, int position);

		// Returns item at position
		o2::MenuPanel::Item GetItem(int position);

		// Returns array of all items
		Vector<o2::MenuPanel::Item> GetItems() const;

		// Removes item at position
		void RemoveItem(int position);

		// Removes item by path
		void RemoveItem(const WString& path);

	protected:
		o2::MenuPanel* mMenuPanel; // Menu panel

	protected:
		// On File/New scene pressed in menu
		void OnNewScenePressed();

		// On File/Open scene pressed in menu
		void OnOpenScenePressed();

		// On File/Save scene pressed in menu
		void OnSaveScenePressed();

		// On File/Save scene as pressed in menu
		void OnSaveSceneAsPressed();

		// On File/New project pressed in menu
		void OnNewProjectPressed();

		// On File/Open project pressed in menu
		void OnOpenProjectPressed();

		// On File/Save project pressed in menu
		void OnSaveProjectPressed();

		// On File/Exit pressed in menu
		void OnExitPressed();

		// On Edit/Undo pressed in menu
		void OnUndoPressed();

		// On Edit/Redo pressed in menu
		void OnRedoPressed();

		// On View/Tree pressed in menu
		void OnShowTreePressed();

		// On View/Scene pressed in menu
		void OnShowScenePressed();

		// On View/Assets pressed in menu
		void OnShowAssetsPressed();

		// On View/Properties pressed in menu
		void OnShowPropertiesPressed();

		// On View/Animation pressed in menu
		void OnShowAnimationPressed();

		// On View/Log pressed in menu
		void OnShowLogPressed();

		// On View/Game pressed in menu
		void OnShowGamePressed();

		// On View/Reset layout pressed in menu
		void OnResetLayoutPressed();

		// On Build/Run pressed in menu
		void OnRunPressed();

		// On Build/Build and run pressed in menu
		void OnBuildAndRunPressed();

		// On Build/Build pressed in menu
		void OnBuildPressed();

		// On Help/About pressed in menu
		void OnAboutPressed();

		// On Help/Documentation pressed in menu
		void OnDocumentationPressed();

		// On Debug/Save layout as default pressed in menu
		void OnSaveDefaultLayoutPressed();

		// On Debug/Curve editor test pressed
		void OnCurveEditorTestPressed();

		// Show open file dialog and returns name
		String GetOpenFileNameDialog(const String& title, const Map<String, String>& extensions);

		// Show save file dialog and returns name
		String GetSaveFileNameDialog(const String& title, const Map<String, String>& extensions);
	};
}
