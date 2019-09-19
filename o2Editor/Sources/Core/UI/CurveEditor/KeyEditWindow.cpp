#include "stdafx.h"

#include "KeyEditWindow.h"

namespace Editor
{

	KeyEditWindow::KeyEditWindow()
	{

	}

	KeyEditWindow::KeyEditWindow(const KeyEditWindow& other)
	{

	}

	KeyEditWindow& KeyEditWindow::operator=(const KeyEditWindow& other)
	{
		return *this;
	}

	void KeyEditWindow::Show(const Curve::Key& key, const Function<void(const Curve::Key& key)>& onClosed)
	{

	}
}

DECLARE_CLASS(Editor::KeyEditWindow);
