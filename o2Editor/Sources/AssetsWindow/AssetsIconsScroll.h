#pragma once

#include "Assets/ActorAsset.h"
#include "Assets/AssetInfo.h"
#include "Assets/ImageAsset.h"
#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "UI/ScrollArea.h"
#include "Utils/Types/Containers/Pair.h"
#include "Utils/Editor/DragAndDrop.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
	class Sprite;
	class UIContextMenu;
	class UIGridLayout;
}

namespace Editor
{
	class ActorProperty;
	class ComponentProperty;
	class IAssetProperty;
	class UISceneTree;
	class UIAssetIcon;

	// ------------------------
	// Assets icons scroll area
	// ------------------------
	class UIAssetsIconsScrollArea: public UIScrollArea, public DragDropArea, public KeyboardEventsListener, 
		public ISelectableDragableObjectsGroup
	{
	public:
		Function<void(const Vector<String>&)> onAssetsSelected; // Assets selected event

		// Default constructor
		UIAssetsIconsScrollArea();

		// Copy-constructor
		UIAssetsIconsScrollArea(const UIAssetsIconsScrollArea& other);

		// Destructor
		~UIAssetsIconsScrollArea();

		// Copy-operator
		UIAssetsIconsScrollArea& operator=(const UIAssetsIconsScrollArea& other);

		// Draws widget
		void Draw() override;

		// Updates widget
		void Update(float dt) override;

		// Sets current assets viewing path
		void SetViewingPath(const String& path);

		// Returns viewing path
		String GetViewingPath() const;

		// Updates assets path contents
		void UpdateAssetsPath();

		// Returns is this widget can be selected
		bool IsFocusable() const;

		// Hightlights asset
		void HightlightAsset(UID id);

		// Selects asset
		void SelectAsset(UID id, bool scroll = true);

		// Deselects all assets
		void DeselectAllAssets();

		// Returns selected assets infos
		Vector<AssetInfo> GetSelectedAssets() const;

		// Return asset icon under point
		UIAssetIcon* GetIconUnderPoint(const Vec2F& point) const;

		// Returns node hightlight drawable
		Sprite* GetHightlightDrawable() const;

		// Sets hightlight animation
		void SetHightlightAnimation(const Animation& animation);

		// Sets hightlight layout
		void SetHightlightLayout(const Layout& layout);

		// Returns selecting rectangle drawable
		Sprite* GetSelectingDrawable() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Updates layout
		void UpdateTransform(bool withChildren = true) override;

		SERIALIZABLE(UIAssetsIconsScrollArea);

	protected:
		typedef Vector<UIAssetIcon*> AssetsIconsVec;
		typedef Vector<Sprite*> SpritesVec;
		typedef Dictionary<String, AssetsIconsVec> IconArrsDict;
		typedef Vector<Pair<UID, String>> AssetIdPathVec;
		typedef Vector<SceneEditableObject*> SceneEditableObjectsVec;
		typedef Vector<AssetRef*> AssetsVec;

		const Vec2F             mAssetIconSize = Vec2F(50, 60);
						        
		String                  mCurrentPath = "_";          // Current viewing path
						        
		UIGridLayout*           mGrid = nullptr;             // Assets icons grid
		UIContextMenu*          mContextMenu = nullptr;      // Assets Context menu
						        
		AssetsIconsVec          mSelectedAssetsIcons;        // Selected assets icons
		AssetsVec               mSelectedPreloadedAssets;    // Preloaded selected assets
						        
		UIAssetIcon*            mHightlightIcon = nullptr;  // Current hightlighting asset icon
		Animation               mHightlightAnim;             // Icon hightlight animation @SERIALIZABLE
		Sprite*                 mHightlightSprite = nullptr; // Icon hightlight sprite @SERIALIZABLE
		Layout                  mHightlightLayout;           // Icon hightlight sprite layout @SERIALIZABLE
						        
		IconArrsDict            mIconsPool;                  // Assets icons pool
						        
		Sprite*                 mSelectionSprite = nullptr;  // Icons selection rectangle sprite @SERIALIZABLE
		bool                    mSelecting = false;          // Is selecting icons 
		Vec2F                   mPressedPoint;               // Pressed point
		AssetsIconsVec          mCurrentSelectingIcons;      // Selecting icons at current selection
						        
		bool                    mIsDraggingIcons = false;    // Is dragging icons
		bool                    mDragEnded = false;          // Is dragging ended
		UIAssetIcon*            mDragIcon = nullptr;         // Dragging icon
		Vec2F                   mDragOffset;                 // Dragging offset from cursor to icon center
		SceneEditableObjectsVec mInstSceneDragObjects;       // Instantiated objects when dragging asset above scene

		AssetIdPathVec          mCuttingAssets;              // Current cutted assets
						        
		bool                    mNeedRebuildAssets = false;  // Is assets needs to rebuild
						        
		bool                    mChangePropertiesTargetsFromThis = false;

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when widget was selected
		void OnFocused() override;

		// It is called when widget was deselected
		void OnUnfocused() override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// It is called when key was released
		void OnKeyReleased(const Input::Key& key) override;

		// It is called when assets selection was changed
		void OnAssetsSelected();

		// Updates cutting assets
		void UpdateCuttingAssets();

		// Begins selecting icons
		void BeginSelecting();

		// Updates selection frame
		void UpdateSelection(const Input::Cursor& cursor);

		// Completes selecting
		void CompleteSelecting();

		// Registers objects creation undo action
		void RegObjectssCreationAction();

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

		// It is called when asset icon double clicked
		void OnIconDblClicked(UIAssetIcon* icon);

		// It is called when context copy pressed
		void OnContextCopyPressed();

		// It is called when context cut pressed
		void OnContextCutPressed();

		// It is called when context paste pressed
		void OnContextPastePressed();

		// It is called when context delete pressed
		void OnContextDeletePressed();

		// It is called when context open pressed
		void OnContextOpenPressed();

		// It is called when context show in explorer pressed
		void OnContextShowInExplorerPressed();

		// It is called when context import pressed
		void OnContextImportPressed();

		// It is called when context create folder pressed
		void OnContextCreateFolderPressed();

		// It is called when context create prefab pressed
		void OnContextCreatePrefabPressed();

		// It is called when context create script pressed
		void OnContextCreateScriptPressed();

		// It is called when context create animation pressed
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
		Actor* InstantiateAsset(const ImageAssetRef& asset);

		// Instantiate actor from actor asset
		Actor* InstantiateAsset(const ActorAssetRef& asset);

		// Checks preloaded assets for saving
		void CheckPreloadedAssetsSaving();
		
// ISelectableDragableObjectsGroup implementation

		// Returns selected objects in group
		SelectDragObjectsVec GetSelectedDragObjects() const override;

		// Returns all objects in group 
		SelectDragObjectsVec GetAllObjects() const override;

		// Selects object
		void Select(SelectableDragableObject* object) override;

		// Selects object
		void Select(SelectableDragableObject* object, bool sendOnSelectionChanged);

		// Deselects object
		void Deselect(SelectableDragableObject* object) override;

		// Adds selectable object to group
		void AddSelectableObject(SelectableDragableObject* object) override;

		// Removes selectable object from group
		void RemoveSelectableObject(SelectableDragableObject* object) override;

		// It is called when selectable draggable object was released
		void OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor) override;

		// It is called when selectable object was began to drag
		void OnSelectableObjectBeganDragging(SelectableDragableObject* object) override;

// DragDropArea implementation
		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		// It is called when dropped dragged scene tree nodes selected and started dragging from scene tree
		void OnDroppedFromSceneTree(UISceneTree* sceneTree);

		// It is called when dropped dragged assets icons selected and started dragging from this
		void OnDroppedFromThis();

		// Begins dragging selected items
		void BeginDragging(UIAssetIcon* icon);

		// Ends dragging items
		void EndDragging(bool droppedToThis = false);

		// Updates dragging graphics
		void UpdateDraggingGraphics();

		friend class AssetsWindow;
		friend class SceneEditScreen;
		friend class UISceneTree;
		friend class UIAssetIcon;
	};

	template<typename _type>
	Actor* UIAssetsIconsScrollArea::InstantiateAsset(const _type& asset)
	{
		return nullptr;
	}
}

CLASS_BASES_META(Editor::UIAssetsIconsScrollArea)
{
	BASE_CLASS(o2::UIScrollArea);
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::ISelectableDragableObjectsGroup);
}
END_META;
CLASS_FIELDS_META(Editor::UIAssetsIconsScrollArea)
{
	PUBLIC_FIELD(onAssetsSelected);
	PROTECTED_FIELD(mAssetIconSize);
	PROTECTED_FIELD(mCurrentPath);
	PROTECTED_FIELD(mGrid);
	PROTECTED_FIELD(mContextMenu);
	PROTECTED_FIELD(mSelectedAssetsIcons);
	PROTECTED_FIELD(mSelectedPreloadedAssets);
	PROTECTED_FIELD(mHightlightIcon);
	PROTECTED_FIELD(mHightlightAnim).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHightlightSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHightlightLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIconsPool);
	PROTECTED_FIELD(mSelectionSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelecting);
	PROTECTED_FIELD(mPressedPoint);
	PROTECTED_FIELD(mCurrentSelectingIcons);
	PROTECTED_FIELD(mIsDraggingIcons);
	PROTECTED_FIELD(mDragEnded);
	PROTECTED_FIELD(mDragIcon);
	PROTECTED_FIELD(mDragOffset);
	PROTECTED_FIELD(mInstSceneDragObjects);
	PROTECTED_FIELD(mCuttingAssets);
	PROTECTED_FIELD(mNeedRebuildAssets);
	PROTECTED_FIELD(mChangePropertiesTargetsFromThis);
}
END_META;
CLASS_METHODS_META(Editor::UIAssetsIconsScrollArea)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetViewingPath, const String&);
	PUBLIC_FUNCTION(String, GetViewingPath);
	PUBLIC_FUNCTION(void, UpdateAssetsPath);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, HightlightAsset, UID);
	PUBLIC_FUNCTION(void, SelectAsset, UID, bool);
	PUBLIC_FUNCTION(void, DeselectAllAssets);
	PUBLIC_FUNCTION(Vector<AssetInfo>, GetSelectedAssets);
	PUBLIC_FUNCTION(UIAssetIcon*, GetIconUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(Sprite*, GetHightlightDrawable);
	PUBLIC_FUNCTION(void, SetHightlightAnimation, const Animation&);
	PUBLIC_FUNCTION(void, SetHightlightLayout, const Layout&);
	PUBLIC_FUNCTION(Sprite*, GetSelectingDrawable);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, UpdateTransform, bool);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnUnfocused);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnAssetsSelected);
	PROTECTED_FUNCTION(void, UpdateCuttingAssets);
	PROTECTED_FUNCTION(void, BeginSelecting);
	PROTECTED_FUNCTION(void, UpdateSelection, const Input::Cursor&);
	PROTECTED_FUNCTION(void, CompleteSelecting);
	PROTECTED_FUNCTION(void, RegObjectssCreationAction);
	PROTECTED_FUNCTION(void, InitializeContext);
	PROTECTED_FUNCTION(void, PrepareIconsPools);
	PROTECTED_FUNCTION(UIAssetIcon*, GetAssetIconFromPool, const String&);
	PROTECTED_FUNCTION(void, FreeAssetIconToPool, UIAssetIcon*);
	PROTECTED_FUNCTION(UIAssetIcon*, GetImageAssetIcon, const AssetInfo&);
	PROTECTED_FUNCTION(void, UpdateAssetsGridSize);
	PROTECTED_FUNCTION(void, OnIconDblClicked, UIAssetIcon*);
	PROTECTED_FUNCTION(void, OnContextCopyPressed);
	PROTECTED_FUNCTION(void, OnContextCutPressed);
	PROTECTED_FUNCTION(void, OnContextPastePressed);
	PROTECTED_FUNCTION(void, OnContextDeletePressed);
	PROTECTED_FUNCTION(void, OnContextOpenPressed);
	PROTECTED_FUNCTION(void, OnContextShowInExplorerPressed);
	PROTECTED_FUNCTION(void, OnContextImportPressed);
	PROTECTED_FUNCTION(void, OnContextCreateFolderPressed);
	PROTECTED_FUNCTION(void, OnContextCreatePrefabPressed);
	PROTECTED_FUNCTION(void, OnContextCreateScriptPressed);
	PROTECTED_FUNCTION(void, OnContextCreateAnimationPressed);
	PROTECTED_FUNCTION(void, InstantiateDraggingAssets);
	PROTECTED_FUNCTION(void, ClearInstantiatedDraggingAssets);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const AssetInfo&);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const ImageAssetRef&);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const ActorAssetRef&);
	PROTECTED_FUNCTION(void, CheckPreloadedAssetsSaving);
	PROTECTED_FUNCTION(SelectDragObjectsVec, GetSelectedDragObjects);
	PROTECTED_FUNCTION(SelectDragObjectsVec, GetAllObjects);
	PROTECTED_FUNCTION(void, Select, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, Select, SelectableDragableObject*, bool);
	PROTECTED_FUNCTION(void, Deselect, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, AddSelectableObject, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, RemoveSelectableObject, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, OnSelectableObjectCursorReleased, SelectableDragableObject*, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnSelectableObjectBeganDragging, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDroppedFromSceneTree, UISceneTree*);
	PROTECTED_FUNCTION(void, OnDroppedFromThis);
	PROTECTED_FUNCTION(void, BeginDragging, UIAssetIcon*);
	PROTECTED_FUNCTION(void, EndDragging, bool);
	PROTECTED_FUNCTION(void, UpdateDraggingGraphics);
}
END_META;
