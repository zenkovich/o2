#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Singleton.h"

using namespace o2;

class IEditorWindow;

// ----------------------
// Editor windows manager
// ----------------------
class WindowsManager: public Singleton<WindowsManager>
{
public:
	typedef Vector<Ptr<IEditorWindow>> EditorWindowsVec;

public:
	// Adds new window
	void AddWindow(Ptr<IEditorWindow> window);

protected:
	const bool mNeedRebuildWndStyle = true; // Is need to rebuild dockable windows style

	EditorWindowsVec mEditorWindows; // Editors windows list

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

	friend class EditorApplication;
	friend class ITemplPtr<WindowsManager>;
};
