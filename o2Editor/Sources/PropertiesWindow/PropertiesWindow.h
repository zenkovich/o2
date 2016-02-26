#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"
#include "Utils/Singleton.h"

using namespace o2;

class IObjectPropertiesViewer;

// Editor properties window accessor macros
#define o2EditorProperties PropertiesWindow::Instance()

// ------------------------
// Editor properties window
// ------------------------
class PropertiesWindow: public IEditorWindow, public Singleton<PropertiesWindow>
{
public:
	// Default constructor
	PropertiesWindow();

	// Destructor
	~PropertiesWindow();

	// Sets target object
	void SetTarget(IObject* target);

	// Sets target objects
	void SetTargets(const Vector<IObject*> targets);

	// Returns target object
	Vector<IObject*> GetTargets() const;

	// Updates window: check next viewer and targets and updates current viewer
	void Update(float dt);

	// Draws current viewer
	void Draw();

	IOBJECT(PropertiesWindow);

protected:
	typedef Vector<IObjectPropertiesViewer*> PropViewersVec;

	Vector<IObject*>         mTargets;       // Target objects
	IObjectPropertiesViewer* mCurrentViewer; // Current properties viewer
	PropViewersVec           mViewers;       // All available object types viewers

protected:
	// Initializes window
	void InitializeWindow();

	// Initializes viewers
	void InitializeViewers();
};
