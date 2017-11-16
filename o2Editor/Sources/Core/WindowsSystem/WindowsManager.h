#pragma once

#include "Core/WindowsSystem/WindowsLayout.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Singleton.h"

using namespace o2;


namespace o2
{
	class UIMenuPanel;
	class UIWindow;
}

// Editor windows accessor macros
#define o2EditorWindows WindowsManager::Instance()

namespace Editor
{
	class ColorPickerDlg;
	class CurveEditorDlg;
	class IEditorWindow;
	class NameEditDlg;
	class UIDockWindowPlace;

	// ----------------------
	// Editor windows manager
	// ----------------------
	class WindowsManager: public Singleton<WindowsManager>
	{
	public:
		typedef Vector<IEditorWindow*> EditorWindowsVec;
		typedef Dictionary<String, WindowsLayout> WndLayoutsDict;

	public:
		// Adds new window
		void AddWindow(IEditorWindow* window);

		// Returns window by type
		template<typename _type>
		_type* GetWindow() const;

		// Returns current windows layout
		WindowsLayout GetWindowsLayout();

		// Sets windows layout
		void SetWindowsLayout(WindowsLayout layout);

		// Sets windows layout by name
		void SetWindowsLayout(const String& name);

		// Sets default windows layout
		void SetDefaultWindowsLayout();

		// Saves current windows layout with name
		void SaveCurrentWindowsLayout(const String& name);

	protected:
		const bool mNeedRebuildWndStyle = true; // Is need to rebuild dockable windows style

		EditorWindowsVec   mEditorWindows;            // Editors windows list
		UIDockWindowPlace* mMainDockPlace = nullptr;  // Main windows dock place
		WndLayoutsDict     mAvailableLayouts;         // Available layouts

		ColorPickerDlg*    mColorPickerDlg = nullptr; // Shared color picker dialog
		CurveEditorDlg*    mCurveEditorDlg = nullptr; // Shared curves editing dialog
		NameEditDlg*       mNameEditDlg = nullptr;    // Name editing dialog

	protected:
		// Default constructor
		WindowsManager();

		// Destructor
		~WindowsManager();

		// Searches derived from IEditorWindow and creates them
		void InitializeWindows();

		// Initializes docking 
		void InitializeDock();

		// Updates windows
		void Update(float dt);

		// Draws windows
		void Draw();

		friend class EditorConfig;
		friend class EditorApplication;
		friend class ToolsPanel;
		friend class WindowsLayout;
	};

	template<typename _type>
	_type* WindowsManager::GetWindow() const
	{
		for (auto wnd : mEditorWindows)
		{
			if (wnd->GetType() == TypeOf(_type))
				return (_type*)wnd;
		}

		return nullptr;
	}
}
