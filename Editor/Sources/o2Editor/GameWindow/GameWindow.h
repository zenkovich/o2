#pragma once

#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

namespace o2
{
	class Widget;
}

namespace Editor
{
	// ---------------------------------------
	// Game window. Draws scene, handles input
	// ---------------------------------------
	class GameWindow: public IEditorWindow
	{
	public:
		class GameView: public Widget
		{
		public:
			GameView();

			void Draw();

			SERIALIZABLE(GameView);

		protected:
			Sprite*    mRenderTargetSprite = nullptr; // Render target sprite, using for drawing scene render target
			TextureRef mRenderTarget;                 // Render target texture, using for rendering scene

		protected:
			// It is called when transformation was changed and updated, updates render texture and sprite
			void OnTransformUpdated() override;
		};

	public:
		GameWindow();
		GameWindow(const GameWindow& other);
		~GameWindow();

		IOBJECT(GameWindow);

	protected:
		void InitializeWindow();
	};
}

CLASS_BASES_META(Editor::GameWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
}
END_META;
CLASS_FIELDS_META(Editor::GameWindow)
{
}
END_META;
CLASS_METHODS_META(Editor::GameWindow)
{

	PROTECTED_FUNCTION(void, InitializeWindow);
}
END_META;

CLASS_BASES_META(Editor::GameWindow::GameView)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::GameWindow::GameView)
{
	PROTECTED_FIELD(mRenderTargetSprite);
	PROTECTED_FIELD(mRenderTarget);
}
END_META;
CLASS_METHODS_META(Editor::GameWindow::GameView)
{

	PUBLIC_FUNCTION(void, Draw);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
}
END_META;
