#pragma once

#include "o2/Events/CursorAreaEventsListenersLayer.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

namespace o2
{
	class Widget;
	class DropDown;
}

namespace Editor
{
	class Vec2IProperty;

	// ---------------------------------------
	// Game window. Draws scene, handles input
	// ---------------------------------------
	class GameWindow: public IEditorWindow
	{
	public:
		class GameView: public Widget
		{
		public:
			bool  fixedResolution = true;
			Vec2I resolution;

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

		class SimulationDevice: public ISerializable
		{
		public:
			String deviceName; // @SERIALIZABLE
			Vec2I  resolution; // @SERIALIZABLE

			bool operator==(const SimulationDevice& other) const;

			SERIALIZABLE(SimulationDevice);
		};

	public:
		GameWindow();
		GameWindow(const GameWindow& other);
		~GameWindow();

		IOBJECT(GameWindow);

	protected:
		GameView* mGameView = nullptr;

		Button* mResolutionsButton = nullptr;

		Map<String, SimulationDevice> mDevicesList;

		ContextMenu*     mDevicesMenu = nullptr;
		ContextMenuItem* mCurrentWindowSizeItem = nullptr;
		ContextMenuItem* mCustomSizeItem = nullptr;

		Vec2IProperty* mCustomSizeProperty = nullptr;

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes devices menu
		void InitializeDevicesMenu();

		// Sets game resolution
		void SetResolution(const Vec2I& resolution);

		// When current window size resolution checked
		void OnCurrentWindowSize(bool enabled);

		// When custom resolution checked
		void OnCustomResolution(bool enabled);

		// When device selected
		void OnDeviceSelected(const String& name, const ContextMenuItem* item);

		// Sets device menu checked item by name
		void SetDeviceMenuCheckedItem(const ContextMenuItem* item);
	};
}

CLASS_BASES_META(Editor::GameWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
}
END_META;
CLASS_FIELDS_META(Editor::GameWindow)
{
	PROTECTED_FIELD(mGameView).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mResolutionsButton).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mDevicesList);
	PROTECTED_FIELD(mDevicesMenu).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mCurrentWindowSizeItem).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mCustomSizeItem).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mCustomSizeProperty).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(Editor::GameWindow)
{

	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeDevicesMenu);
	PROTECTED_FUNCTION(void, SetResolution, const Vec2I&);
	PROTECTED_FUNCTION(void, OnCurrentWindowSize, bool);
	PROTECTED_FUNCTION(void, OnCustomResolution, bool);
	PROTECTED_FUNCTION(void, OnDeviceSelected, const String&, const ContextMenuItem*);
	PROTECTED_FUNCTION(void, SetDeviceMenuCheckedItem, const ContextMenuItem*);
}
END_META;

CLASS_BASES_META(Editor::GameWindow::GameView)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::GameWindow::GameView)
{
	PUBLIC_FIELD(fixedResolution).DEFAULT_VALUE(true);
	PUBLIC_FIELD(resolution);
	PROTECTED_FIELD(mRenderTargetSprite).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mRenderTarget);
}
END_META;
CLASS_METHODS_META(Editor::GameWindow::GameView)
{

	PUBLIC_FUNCTION(void, Draw);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
}
END_META;

CLASS_BASES_META(Editor::GameWindow::SimulationDevice)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::GameWindow::SimulationDevice)
{
	PUBLIC_FIELD(deviceName).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(resolution).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::GameWindow::SimulationDevice)
{
}
END_META;
