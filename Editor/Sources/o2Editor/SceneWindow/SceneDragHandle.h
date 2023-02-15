#pragma once

#include "o2/Utils/Editor/DragHandle.h"

using namespace o2;

namespace Editor
{
	// -----------------
	// Scene drag handle
	// -----------------
	class SceneDragHandle: public DragHandle
	{
	public:
		// Handle drawing type. 
		// ScreenSpace - draws without zooming, in screen space coordinates
		// SceneSpace - draws with zooming and depends on camera, in local scene coordinates
		enum class Mode { ScreenSpace, SceneSpace };

	public:
		Mode mode = Mode::ScreenSpace; // Handle drawing mode, screen or scene space

	public:
		// Default constructor
		SceneDragHandle();

		// Constructor with views
		SceneDragHandle(IRectDrawable* regular, IRectDrawable* hover = nullptr, IRectDrawable* pressed = nullptr);

		// Copy-constructor
		SceneDragHandle(const SceneDragHandle& other);

		// Destructor
		~SceneDragHandle();

		// Copy-operator
		SceneDragHandle& operator=(const SceneDragHandle& other);

		// Draws handle, applies scale if mode is scene space
		void Draw() override;

		// Set handle enabled. Disabled handle don't drawn and interact
		void SetEnabled(bool enabled) override;

		// Converts point from screen to local space
		Vec2F ScreenToLocal(const Vec2F& point) override;

		// Converts point from local to screen space
		Vec2F LocalToScreen(const Vec2F& point) override;

		SERIALIZABLE(SceneDragHandle);

	protected:
		friend class SceneEditScreen;
	};
}

PRE_ENUM_META(Editor::SceneDragHandle::Mode);

CLASS_BASES_META(Editor::SceneDragHandle)
{
	BASE_CLASS(o2::DragHandle);
}
END_META;
CLASS_FIELDS_META(Editor::SceneDragHandle)
{
	FIELD().PUBLIC().DEFAULT_VALUE(Mode::ScreenSpace).NAME(mode);
}
END_META;
CLASS_METHODS_META(Editor::SceneDragHandle)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(IRectDrawable*, IRectDrawable*, IRectDrawable*);
	FUNCTION().PUBLIC().CONSTRUCTOR(const SceneDragHandle&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, ScreenToLocal, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, LocalToScreen, const Vec2F&);
}
END_META;
