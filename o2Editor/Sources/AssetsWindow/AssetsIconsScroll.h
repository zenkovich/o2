#pragma once

#include "Assets/AssetInfo.h"
#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "UI/ScrollArea.h"
#include "Utils/Containers/Pair.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class UIGridLayout;
	class UIContextMenu;
}

class UIAssetIcon;

// ------------------------
// Assets icons scroll area
// ------------------------
class UIAssetsIconsScrollArea: public UIScrollArea, public DrawableCursorEventsListener, public KeyboardEventsListener
{
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
	bool IsSelectable() const;

	// Selects asset
	void SelectAsset(AssetId id);

	// Deselects all assets
	void DeselectAllAssets();

	// Returns selected assets infos
	Vector<AssetInfo> GetSelectedAssets() const;

	SERIALIZABLE(UIAssetsIconsScrollArea);

protected:
	struct IconSelection
	{
		UIAssetIcon* icon;
		Sprite*      selectionSprite;

		IconSelection();
		IconSelection(UIAssetIcon* icon, Sprite* selectionSprite);

		bool operator==(const IconSelection& other) const;
	};

	typedef Vector<IconSelection> IconSelectionsVec;
	typedef Vector<UIAssetIcon*> AssetsIconsVec;
	typedef Vector<Sprite*> SpritesVec;
	typedef Dictionary<String, AssetsIconsVec> IconArrsDict;
	typedef Vector<Pair<AssetId, String>> AssetIdPathVec;


	const Vec2F       mAssetIconSize = Vec2F(50, 60);
	Color4            mSelectedColor = Color4(175, 175, 255, 150);  
	Color4            mUnselectedColor = Color4(100, 100, 100, 100);
	Color4            mHoverColor = Color4(100, 100, 100, 100);     

	String            mCurrentPath;               // Current viewing path
											      
	UIGridLayout*     mGrid;                      // Assets icons grid
	Sprite*           mSelection;                 // Icons selection sprite
											      
	UIContextMenu*    mContextMenu;               // Assets Context menu
										   	      
	IconSelectionsVec mSelectedAssetsIcons;       // Selected assets icons
	Sprite*           mIconSelectionSprite;       // Selection sprite drawable
	Layout            mSelectionSpriteLayout;     // Selection sprite layout 
	SpritesVec        mSelectionSpritesPool;      // Selection sprites pool
											      
	UIAssetIcon*      mHoverIcon = nullptr;       // Current hovered asset icon
	Sprite*           mIconHoverSprite;           // icons hovering sprite
	RectF             mTargetHoverSpriteRect;     // Target hover rectangle
	RectF             mCurrentHoverSpriteRect;    // Current hover rectangle
										   	      
	IconArrsDict      mIconsPool;                 // Assets icons pool
											      
	bool              mSelecting = false;         // Is selecting icons 
	Vec2F             mPressedPoint;              // Pressed point
	float             mPressTime;                 // Time elapsed from pressing
	IconSelectionsVec mCurrentSelectingIcons;     // Selecting icons at current selection
											      
	AssetIdPathVec    mCuttingAssets;             // Current cutted assets

	bool              mNeedRebuildAssets = false; // Is assets needs to rebuild

protected:
	// Updates layout
	void UpdateLayout(bool forcible = false);

	// Updates cutting assets
	void UpdateCuttingAssets();

	// Calls when widget was selected
	void OnSelected();

	// Calls when widget was deselected
	void OnDeselected();

	// Calls when cursor pressed on this
	void OnCursorPressed(const Input::Cursor& cursor);

	// Calls when cursor released (only when cursor pressed this at previous time)
	void OnCursorReleased(const Input::Cursor& cursor);

	// Calls when cursor pressing was broken (when scrolled scroll area or some other)
	void OnCursorPressBreak(const Input::Cursor& cursor);

	// Calls when cursor stay down during frame
	void OnCursorStillDown(const Input::Cursor& cursor);

	// Updates selection frame
	void UpdateSelection(const Input::Cursor& cursor);

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

	// Return asset icon under point
	UIAssetIcon* GetIconUnderPoint(const Vec2F& point) const;

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

	friend class AssetsWindow;
};