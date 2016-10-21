#pragma once

#include "Assets/AssetInfo.h"
#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "UI/ScrollArea.h"
#include "Utils/Containers/Pair.h"

using namespace o2;

namespace o2
{
	class Actor;
	class ActorAsset;
	class ImageAsset;
	class Sprite;
	class UIContextMenu;
	class UIGridLayout;
}

namespace Editor
{
	class ActorProperty;
	class ComponentProperty;
	class IAssetProperty;
	class UIActorsTree;
	class UIAssetIcon;

	// ------------------------
	// Assets icons scroll area
	// ------------------------
	class UIAssetsIconsScrollArea: public UIScrollArea, public DrawableCursorEventsListener, public KeyboardEventsListener
	{
	public:
		enum class DragState { Off, Regular, Scene, Tree, AssetField, ActorField, ComponentField };

	public:
		// Default constructor
		UIAssetsIconsScrollArea();

		// Copy-constructor
		UIAssetsIconsScrollArea(const UIAssetsIconsScrollArea& other);

		// Destructor
		~UIAssetsIconsScrollArea();

		// Copy-operator
		UIAssetsIconsScrollArea& operator=(const UIAssetsIconsScrollArea& other);

		// Draws widget
		void Draw();

		// Updates widget
		void Update(float dt);

		// Sets current assets viewing path
		void SetViewingPath(const String& path);

		// Returns viewing path
		String GetViewingPath() const;

		// Updates assets path contents
		void UpdateAssetsPath();

		// Returns is this widget can be selected
		bool IsFocusable() const;

		// Selects asset
		void SelectAsset(UID id, bool scroll = true);

		// Deselects all assets
		void DeselectAllAssets();

		// Returns selected assets infos
		Vector<AssetInfo> GetSelectedAssets() const;

		// Return asset icon under point
		UIAssetIcon* GetIconUnderPoint(const Vec2F& point) const;

		SERIALIZABLE(UIAssetsIconsScrollArea);

	protected:
		// ------------------------
		// Asset icon selection def
		// ------------------------
		struct IconSelection
		{
			UIAssetIcon* icon;            // Asset icon widget pointer
			Sprite*      selectionSprite; // Selection sprite

			IconSelection();
			IconSelection(UIAssetIcon* icon, Sprite* selectionSprite);

			bool operator==(const IconSelection& other) const;
		};

		typedef Vector<IconSelection> IconSelectionsVec;
		typedef Vector<UIAssetIcon*> AssetsIconsVec;
		typedef Vector<Sprite*> SpritesVec;
		typedef Dictionary<String, AssetsIconsVec> IconArrsDict;
		typedef Vector<Pair<UID, String>> AssetIdPathVec;
		typedef Vector<Actor*> ActorsVec;


		const Vec2F        mAssetIconSize = Vec2F(50, 60);
		Color4             mSelectedColor = Color4(175, 175, 255, 150);
		Color4             mUnselectedColor = Color4(100, 100, 100, 100);
		Color4             mHoverColor = Color4(100, 100, 100, 100);

		String             mCurrentPath;                // Current viewing path

		UIGridLayout*      mGrid;                       // Assets icons grid
		Sprite*            mSelection;                  // Icons selection sprite

		UIContextMenu*     mContextMenu;                // Assets Context menu

		IconSelectionsVec  mSelectedAssetsIcons;        // Selected assets icons
		Sprite*            mIconSelectionSprite;        // Selection sprite drawable
		Layout             mSelectionSpriteLayout;      // Selection sprite layout 
		SpritesVec         mSelectionSpritesPool;       // Selection sprites pool

		UIAssetIcon*       mHoverIcon = nullptr;        // Current hovered asset icon
		Sprite*            mIconHoverSprite;            // icons hovering sprite
		RectF              mTargetHoverSpriteRect;      // Target hover rectangle
		RectF              mCurrentHoverSpriteRect;     // Current hover rectangle

		IconArrsDict       mIconsPool;                  // Assets icons pool

		bool               mSelecting = false;          // Is selecting icons 
		Vec2F              mPressedPoint;               // Pressed point
		float              mPressTime;                  // Time elapsed from pressing
		IconSelectionsVec  mCurrentSelectingIcons;      // Selecting icons at current selection

		DragState          mDragState = DragState::Off; // Is dragging icons
		UIAssetIcon*       mDragIcon;                   // Dragging icon
		Vec2F              mDragOffset;                 // Dragging offset from cursor to icon center
		ActorsVec          mInstSceneDragActors;        // Instantiated actors when dragging asset above scene

		IAssetProperty*    mDragAssetPropertyField = nullptr;     // Asset property field under cursor, when dragging asset
		ActorProperty*     mDragActorPropertyField = nullptr;     // Actor property field under cursor, when dragging asset
		ComponentProperty* mDragComponentPropertyField = nullptr; // Component property field under cursor, when dragging asset

		AssetIdPathVec     mCuttingAssets;              // Current cutted assets
						   
		bool               mNeedRebuildAssets = false;  // Is assets needs to rebuild

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Updates cutting assets
		void UpdateCuttingAssets();

		// Calls when widget was selected
		void OnFocused();

		// Calls when widget was deselected
		void OnUnfocused();

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Begins selecting icons
		void BeginSelecting();

		// Updates selection frame
		void UpdateSelection(const Input::Cursor& cursor);

		// Completes selecting
		void CompleteSelecting();

		// Begins dragging
		void BeginDragging(UIAssetIcon* iconUnderCursor, const Input::Cursor& cursor);

		// Updates dragging icons
		void UpdateDragging(const Input::Cursor& cursor);

		//Completes dragging
		void CompleteDragging();

		// Registers actors creation undo action
		void RegActorsCreationAction();

		// Calls when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor);

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// Calls when key was released
		void OnKeyReleased(const Input::Key& key);

		// Initializes assets context menu
		void InitializeContext();

		// Prepares icons pools
		void PrepareIconsPools();

		// Returns asset icon from pool
		UIAssetIcon* GetAssetIconFromPool(const String& style);

		// Frees icon to pool
		void FreeAssetIconToPool(UIAssetIcon* icon);

		// Returns image asset icon for asset
		UIAssetIcon* GetImageAssetIcon(const AssetInfo& asset);

		// Updates assets grid size
		void UpdateAssetsGridSize();

		// Initializes selection sprite and pool
		void InitializeSelectionSprite();

		// Returns selection sprite from pool
		Sprite* GetSelectionSprite();

		// Frees selection sprite in pool
		void FreeSelectionSprite(Sprite* sprite);

		// Calls when asset icon double clicked
		void OnIconDblClicked(UIAssetIcon* icon);

		// Update hover sprite
		void UpdateHover();

		// Calls when context copy pressed
		void OnContextCopyPressed();

		// Calls when context cut pressed
		void OnContextCutPressed();

		// Calls when context paste pressed
		void OnContextPastePressed();

		// Calls when context delete pressed
		void OnContextDeletePressed();

		// Calls when context open pressed
		void OnContextOpenPressed();

		// Calls when context show in explorer pressed
		void OnContextShowInExplorerPressed();

		// Calls when context import pressed
		void OnContextImportPressed();

		// Calls when context create folder pressed
		void OnContextCreateFolderPressed();

		// Calls when context create prefab pressed
		void OnContextCreatePrefabPressed();

		// Calls when context create script pressed
		void OnContextCreateScriptPressed();

		// Calls when context create animation pressed
		void OnContextCreateAnimationPressed();

		// Instantiates dragging assets
		void InstantiateDraggingAssets();

		// Removes and clears instantiated dragging assets
		void ClearInstantiatedDraggingAssets();

		// Instentiates actor from asset info
		Actor* InstantiateAsset(const AssetInfo& assetInfo);

		// Dummy asset instantiate function from asset
		template<typename _type>
		Actor* InstantiateAsset(const _type& asset);

		// Instantiate actor from image asset
		Actor* InstantiateAsset(const ImageAsset& asset);

		// Instantiate actor from actor asset
		Actor* InstantiateAsset(const ActorAsset& asset);

		friend class AssetsWindow;
	};

	template<typename _type>
	Actor* UIAssetsIconsScrollArea::InstantiateAsset(const _type& asset)
	{
		return nullptr;
	}
}