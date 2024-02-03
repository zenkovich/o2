#pragma once
#include "o2/Events/CursorAreaEventsListenersLayer.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"
#include "o2/SmartPointers/Ref.h"
#include "o2/SmartPointers/WeakRef.h"

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

			void Draw() override;

			// Returns create menu category in editor
			static String GetCreateMenuCategory();

			SERIALIZABLE(GameView);

		protected:
			Ref<Sprite>    mRenderTargetSprite; // Render target sprite, using for drawing scene render target
			Ref<Texture> mRenderTarget;                 // Render target texture, using for rendering scene

		protected:
			// Called when transformation was changed and updated, updates render texture and sprite
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
		Ref<GameView> mGameView;

		Ref<Button> mResolutionsButton;

		Map<String, SimulationDevice> mDevicesList;

		Ref<ContextMenu>     mDevicesMenu;
		Ref<ContextMenuItem> mCurrentWindowSizeItem;
		Ref<ContextMenuItem> mCustomSizeItem;

		Ref<Vec2IProperty> mCustomSizeProperty;

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes devices menu
		void InitializeDevicesMenu();

		// Sets game resolution
		void SetResolution(const Ref<Vec2I>& resolution);

		// When current window size resolution checked
		void OnCurrentWindowSize(bool enabled);

		// When custom resolution checked
		void OnCustomResolution(bool enabled);

		// When device selected
		void OnDeviceSelected(const String& name, const Ref<ContextMenuItem>& item);

		// Sets device menu checked item by name
		void SetDeviceMenuCheckedItem(const Ref<ContextMenuItem>& item);
	};
}
// --- META ---

CLASS_BASES_META(Editor::GameWindow)
{
    BASE_CLASS(Editor::IEditorWindow);
}
END_META;
CLASS_FIELDS_META(Editor::GameWindow)
{
    FIELD().PROTECTED().NAME(mGameView);
    FIELD().PROTECTED().NAME(mResolutionsButton);
    FIELD().PROTECTED().NAME(mDevicesList);
    FIELD().PROTECTED().NAME(mDevicesMenu);
    FIELD().PROTECTED().NAME(mCurrentWindowSizeItem);
    FIELD().PROTECTED().NAME(mCustomSizeItem);
    FIELD().PROTECTED().NAME(mCustomSizeProperty);
}
END_META;
CLASS_METHODS_META(Editor::GameWindow)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const GameWindow&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeDevicesMenu);
    FUNCTION().PROTECTED().SIGNATURE(void, SetResolution, const Ref<Vec2I>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCurrentWindowSize, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCustomResolution, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeviceSelected, const String&, const Ref<ContextMenuItem>&);
    FUNCTION().PROTECTED().SIGNATURE(void, SetDeviceMenuCheckedItem, const Ref<ContextMenuItem>&);
}
END_META;

CLASS_BASES_META(Editor::GameWindow::GameView)
{
    BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::GameWindow::GameView)
{
    FIELD().PUBLIC().NAME(fixedResolution);
    FIELD().PUBLIC().NAME(resolution);
    FIELD().PROTECTED().NAME(mRenderTargetSprite);
    FIELD().PROTECTED().NAME(mRenderTarget);
}
END_META;
CLASS_METHODS_META(Editor::GameWindow::GameView)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
}
END_META;

CLASS_BASES_META(Editor::GameWindow::SimulationDevice)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::GameWindow::SimulationDevice)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(deviceName);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(resolution);
}
END_META;
CLASS_METHODS_META(Editor::GameWindow::SimulationDevice)
{
}
END_META;