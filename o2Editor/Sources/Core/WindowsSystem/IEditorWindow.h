#pragma once

#include "O2.h"
using namespace o2;

#include "Core/WindowsSystem/UIDockableWindow.h"

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

	SERIALIZABLE_IMPL(IEditorWindow);

	IOBJECT(IEditorWindow)
	{
		FIELD(mWindow);
	}

protected:
	Ptr<UIDockableWindow> mWindow;
};