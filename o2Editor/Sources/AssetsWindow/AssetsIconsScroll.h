#pragma once

#include "Assets/AssetInfo.h"
#include "Events/DrawableCursorEventsListener.h"
#include "UI/ScrollArea.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class UIGridLayout;
}

class UIAssetIcon;

// ------------------------
// Assets icons scroll area
// ------------------------
class UIAssetsIconsScrollArea: public UIScrollArea, public DrawableCursorEventsListener
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

	// Sets current assets viewing path
	void SetAssetPath(const String& path);

	// Returns is this widget can be selected
	bool IsSelectable() const;

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


	const Vec2F       mAssetIconSize = Vec2F(40, 50);
	Color4            mSelectedColor = Color4(175, 175, 255, 150);  
	Color4            mUnselectedColor = Color4(100, 100, 100, 100);
	Color4            mHoverColor = Color4(100, 100, 100, 100);     

	String            mCurrentPath;           // Current viewing path

	UIGridLayout*     mGrid;                  // Assets icons grid
	Sprite*           mSelection;             // Icons selection sprite
										   
	IconSelectionsVec mSelectedAssetsIcons;   // Selected assets icons
	Sprite*           mIconSelectionSprite;   // Selection sprite drawable
	Layout            mSelectionSpriteLayout; // Selection sprite layout 
	SpritesVec        mSelectionSpritesPool;  // Selection sprites pool
										   
	IconArrsDict      mIconsPool;             // Assets icons pool

	bool              mSelecting = false;     // Is selecting icons 
	Vec2F             mPressedPoint;          // Pressed point
	IconSelectionsVec mCurrentSelectingIcons; // Selecting icons at current selection

protected:
	// Updates layout
	void UpdateLayout(bool forcible = false);

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

	// Calls when cursor moved on this (or moved outside when this was pressed)
	void OnCursorMoved(const Input::Cursor& cursor);

	// Prepares icons pools
	void PrepareIconsPools();

	// Returns asset icon from pool
	UIAssetIcon* GetAssetIconFromPool(const String& style);

	// Frees icon to pool
	void FreeAssetIconToPool(UIAssetIcon* icon);
	
	// Clears icons selection
	void ClearAssetIconsSelection();

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
};