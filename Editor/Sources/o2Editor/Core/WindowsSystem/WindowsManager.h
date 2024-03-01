#pragma once

#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2Editor/Core/Dialogs/KeyEditDlg.h"
#include "o2Editor/Core/WindowsSystem/WindowsLayout.h"

using namespace o2;

namespace o2
{
	class MenuPanel;
	class Window;
}

// Editor windows accessor macros
#define o2EditorWindows WindowsManager::Instance()

namespace Editor
{
	class ColorPickerDlg;
	class CurveEditorDlg;
	class IEditorWindow;
	class NameEditDlg;
	class DockWindowPlace;

	// ----------------------
	// Editor windows manager
	// ----------------------
	class WindowsManager: public Singleton<WindowsManager>, public RefCounterable
	{
    public:
        // Default constructor
        WindowsManager();

        // Destructor
        ~WindowsManager();

		// Adds new window
		void AddWindow(const Ref<IEditorWindow>& window);

		// Returns window by type
		template<typename _type>
		Ref<_type> GetWindow() const;

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
		Vector<Ref<IEditorWindow>> mEditorWindows;    // Editors windows list
		Ref<DockWindowPlace>       mMainDockPlace;    // Main windows dock place
		Map<String, WindowsLayout> mAvailableLayouts; // Available layouts

		Ref<ColorPickerDlg> mColorPickerDlg;   // Color picker dialog
		Ref<CurveEditorDlg> mCurveEditorDlg;   // Curve editor dialog
		Ref<NameEditDlg>    mNameEditDlg;      // Name edit dialog
		Ref<KeyEditDlg>     mKeyEditDlg;       // Key edit dialog

	protected:
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
}

#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

namespace Editor
{
	template<typename _type>
	Ref<_type> WindowsManager::GetWindow() const
	{
		for (auto& wnd : mEditorWindows)
		{
			if (wnd->GetType() == TypeOf(_type))
				return DynamicCast<_type>(wnd);
		}

		return nullptr;
	}
}
