#include "SceneWindow.h"

SceneWindow::SceneWindow():
	IEditorWindow()
{}

SceneWindow::SceneWindow(const SceneWindow& other) :
	IEditorWindow(other)
{}

SceneWindow::~SceneWindow()
{}
