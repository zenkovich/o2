#pragma once

#include "UIDockableWindow.h"
#include "Utils/Serialization.h"

using namespace o2;

class IEditorWindow: public ISerializable
{
public:
	Property<bool> visible;

	IEditorWindow();
	IEditorWindow(const IEditorWindow& other);

	virtual ~IEditorWindow();

	virtual void SetVisible(bool visible);
	virtual void Update(float dt);
	virtual void Draw();

	bool IsVisible();

	void Show();
	void Hide();

	SERIALIZABLE(IEditorWindow);

protected:
	Ptr<UIDockableWindow> mWindow;

	friend class WindowsManager;
};