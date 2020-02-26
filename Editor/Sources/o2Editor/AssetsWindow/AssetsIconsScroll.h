#pragma once

#include "o2/Assets/AssetInfo.h"
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Scene/UI/Widgets/GridLayoutScrollArea.h"
#include "o2/Utils/Editor/DragAndDrop.h"
#include "o2/Utils/Types/Containers/Pair.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
	class Sprite;
	class ContextMenu;
	class GridLayout;
}

namespace Editor
{
	class ActorProperty;
	class ComponentProperty;
	class IAssetProperty;
	class SceneTree;
	class AssetIcon;

	// ------------------------
	// Assets icons scroll area
	// ------------------------
	class AssetsIconsScrollArea: public GridLayoutScrollArea, public DragDropArea, public KeyboardEventsListener, 
		public ISelectableDragableObjectsGroup
	{
	public:
		Function<void(const Vector<String>&)> onAssetsSelected; // Assets selected event

	public:
		// Default constructor
		AssetsIconsScrollArea();

		// Copy-constructor
		AssetsIconsScrollArea(const AssetsIconsScrollArea& other);

		// Destructor
		~AssetsIconsScrollArea();

		// Copy-operator
		AssetsIconsScrollArea& operator=(const AssetsIconsScrollArea& other);

		// Draws widget
		void Draw() override;

		// Updates widget
		void Update(float dt) override;

		// Sets current assets viewing path
		void SetViewingPath(const String& path);

		// Returns viewing path
		const String& GetViewingPath() const;

		// Updates assets path contents
		void UpdateAssetsByCurrentPath();

		// Returns is this widget can be selected
		bool IsFocusable() const;

		// Highlights asset
		void HighlightAsset(const UID& id);

		// Selects asset
		void SelectAsset(const UID& id, bool scroll = true);

		// Deselects all assets
		void DeselectAllAssets();

		// Returns selected assets infos
		const Vector<const AssetInfo*>& GetSelectedAssets() const;

		// Return asset icon under point
		AssetIcon* GetIconUnderPoint(const Vec2F& point) const;

		// Returns node highlight drawable
		Sprite* GetHighlightDrawable() const;

		// Sets highlight animation
		void SetHighlightAnimation(const Animation& animation);

		// Sets highlight layout
		void SetHighlightLayout(const Layout& layout);

		// Returns selecting rectangle drawable
		Sprite* GetSelectingDrawable() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

		SERIALIZABLE(AssetsIconsScrollArea);

	protected:
		const Vec2F mAssetIconSize = Vec2F(50, 60);
						        
		String mCurrentPath = "_"; // Current viewing path

		Vector<const AssetInfo*> mAssetInfos;        // Asset infos in path @IGNORE
		Vector<AssetIcon*>       mVisibleAssetIcons; // Visible asset icons

		ContextMenu* mContextMenu = nullptr; // Assets Context menu
						        
		Vector<const AssetInfo*> mSelectedAssets;          // Selected assets icons @IGNORE
		Vector<AssetRef*>        mSelectedPreloadedAssets; // Preloaded selected assets
						        
		AssetIcon* mHighlightIcon = nullptr;   // Current highlighting asset icon
		Animation  mHighlightAnim;             // Icon highlight animation @SERIALIZABLE
		Sprite*    mHighlightSprite = nullptr; // Icon highlight sprite @SERIALIZABLE
		Layout     mHighlightLayout;           // Icon highlight sprite layout @SERIALIZABLE
						        
		Map<String, Vector<AssetIcon*>> mIconsPool; // Assets icons pool
						        
		Sprite*                  mSelectionSprite = nullptr;  // Icons selection rectangle sprite @SERIALIZABLE
		bool                     mSelecting = false;          // Is selecting icons 
		Vec2F                    mPressedPoint;               // Pressed point
		Vector<const AssetInfo*> mCurrentSelectingInfos;      // Selecting icons at current selection @IGNORE
						        
		bool                         mIsDraggingIcons = false;      // Is dragging icons
		bool                         mDragEnded = false;            // Is dragging ended
		AssetIcon*                   mDragIcon = nullptr;           // Dragging icon
		Vec2F                        mDragOffset;                   // Dragging offset from cursor to icon center
		Vector<SceneEditableObject*> mInstantiatedSceneDragObjects; // Instantiated objects when dragging asset above scene

		Vector<Pair<UID, String>> mCuttingAssets; // Current cutted assets
						        
		bool mNeedRebuildAssets = false; // Is assets needs to rebuild
						        
		bool mChangePropertiesTargetsFromThis = false;

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Returns items count, calls getItemsCountFunc
		int GetItemsCount() const override;

		// Returns items in range from start to end, calls getItemsRangeFunc
		Vector<void*> GetItemsRange(int start, int end) const override;

		// Sets item widget, calls setupItemFunc
		void SetupItemWidget(Widget* widget, void* item) override;

		// Updates visible items
		void UpdateVisibleItems() override;

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

		// It is called when scrolling
		void OnScrolled(float scroll) override;

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
		void RegObjectsCreationAction();

		// Initializes assets context menu
		void InitializeContext();

		// Initializes assets create context menu
		void InitializeCreateContext();

		// Returns asset icon from pool or creates new by style name
		AssetIcon* GetAssetIconFromPool(const String& style);

		// Frees icon to pool
		void FreeAssetIconToPool(AssetIcon* icon);

		// Returns asset icon if visible
		AssetIcon* FindVisibleIcon(const AssetInfo* info);

		// It is called when asset icon double clicked, starting editing name
		void OnAssetDblClick(AssetIcon* icon);

		// Starts asset icon renaming, calls onCompletedwhen completed
		void StartAssetRenaming(AssetIcon* icon, const String& name, const Function<void(const String&)>& onCompleted);

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

		// It is called when context create asset pressed
		void OnContextCreateAssetPressed(const Type* assetType);

		// It is called when context create folder pressed
		void OnContextCreateFolderPressed();

		// Instantiates dragging assets
		void InstantiateDraggingAssets();

		// Removes and clears instantiated dragging assets
		void ClearInstantiatedDraggingAssets();

		// Instantiate actor from asset info
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
		Vector<SelectableDragableObject*> GetSelectedDragObjects() const override;

		// Returns all objects in group 
		Vector<SelectableDragableObject*> GetAllObjects() const override;

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
		void OnDroppedFromSceneTree(SceneTree* sceneTree);

		// It is called when dropped dragged assets icons selected and started dragging from this
		void OnDroppedFromThis();

		// Begins dragging selected items
		void BeginDragging(AssetIcon* icon);

		// Ends dragging items
		void EndDragging(bool droppedToThis = false);

		// Updates dragging graphics
		void UpdateDraggingGraphics();

		friend class AssetsWindow;
		friend class SceneEditScreen;
		friend class SceneTree;
		friend class AssetIcon;
	};

	template<typename _type>
	Actor* AssetsIconsScrollArea::InstantiateAsset(const _type& asset)
	{
		return nullptr;
	}
}

CLASS_BASES_META(Editor::AssetsIconsScrollArea)
{
	BASE_CLASS(o2::GridLayoutScrollArea);
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::ISelectableDragableObjectsGroup);
}
END_META;
CLASS_FIELDS_META(Editor::AssetsIconsScrollArea)
{
	PUBLIC_FIELD(onAssetsSelected);
	PROTECTED_FIELD(mAssetIconSize);
	PROTECTED_FIELD(mCurrentPath);
	PROTECTED_FIELD(mVisibleAssetIcons);
	PROTECTED_FIELD(mContextMenu);
	PROTECTED_FIELD(mSelectedPreloadedAssets);
	PROTECTED_FIELD(mHighlightIcon);
	PROTECTED_FIELD(mHighlightAnim).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHighlightSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHighlightLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIconsPool);
	PROTECTED_FIELD(mSelectionSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelecting);
	PROTECTED_FIELD(mPressedPoint);
	PROTECTED_FIELD(mIsDraggingIcons);
	PROTECTED_FIELD(mDragEnded);
	PROTECTED_FIELD(mDragIcon);
	PROTECTED_FIELD(mDragOffset);
	PROTECTED_FIELD(mInstantiatedSceneDragObjects);
	PROTECTED_FIELD(mCuttingAssets);
	PROTECTED_FIELD(mNeedRebuildAssets);
	PROTECTED_FIELD(mChangePropertiesTargetsFromThis);
}
END_META;
CLASS_METHODS_META(Editor::AssetsIconsScrollArea)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetViewingPath, const String&);
	PUBLIC_FUNCTION(const String&, GetViewingPath);
	PUBLIC_FUNCTION(void, UpdateAssetsByCurrentPath);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, HighlightAsset, const UID&);
	PUBLIC_FUNCTION(void, SelectAsset, const UID&, bool);
	PUBLIC_FUNCTION(void, DeselectAllAssets);
	PUBLIC_FUNCTION(const Vector<const AssetInfo*>&, GetSelectedAssets);
	PUBLIC_FUNCTION(AssetIcon*, GetIconUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(Sprite*, GetHighlightDrawable);
	PUBLIC_FUNCTION(void, SetHighlightAnimation, const Animation&);
	PUBLIC_FUNCTION(void, SetHighlightLayout, const Layout&);
	PUBLIC_FUNCTION(Sprite*, GetSelectingDrawable);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(bool, IsInputTransparent);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(int, GetItemsCount);
	PROTECTED_FUNCTION(Vector<void*>, GetItemsRange, int, int);
	PROTECTED_FUNCTION(void, SetupItemWidget, Widget*, void*);
	PROTECTED_FUNCTION(void, UpdateVisibleItems);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnUnfocused);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnAssetsSelected);
	PROTECTED_FUNCTION(void, UpdateCuttingAssets);
	PROTECTED_FUNCTION(void, BeginSelecting);
	PROTECTED_FUNCTION(void, UpdateSelection, const Input::Cursor&);
	PROTECTED_FUNCTION(void, CompleteSelecting);
	PROTECTED_FUNCTION(void, RegObjectsCreationAction);
	PROTECTED_FUNCTION(void, InitializeContext);
	PROTECTED_FUNCTION(void, InitializeCreateContext);
	PROTECTED_FUNCTION(AssetIcon*, GetAssetIconFromPool, const String&);
	PROTECTED_FUNCTION(void, FreeAssetIconToPool, AssetIcon*);
	PROTECTED_FUNCTION(AssetIcon*, FindVisibleIcon, const AssetInfo*);
	PROTECTED_FUNCTION(void, OnAssetDblClick, AssetIcon*);
	PROTECTED_FUNCTION(void, StartAssetRenaming, AssetIcon*, const String&, const Function<void(const String&)>&);
	PROTECTED_FUNCTION(void, OnContextCopyPressed);
	PROTECTED_FUNCTION(void, OnContextCutPressed);
	PROTECTED_FUNCTION(void, OnContextPastePressed);
	PROTECTED_FUNCTION(void, OnContextDeletePressed);
	PROTECTED_FUNCTION(void, OnContextOpenPressed);
	PROTECTED_FUNCTION(void, OnContextShowInExplorerPressed);
	PROTECTED_FUNCTION(void, OnContextCreateAssetPressed, const Type*);
	PROTECTED_FUNCTION(void, OnContextCreateFolderPressed);
	PROTECTED_FUNCTION(void, InstantiateDraggingAssets);
	PROTECTED_FUNCTION(void, ClearInstantiatedDraggingAssets);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const AssetInfo&);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const ImageAssetRef&);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const ActorAssetRef&);
	PROTECTED_FUNCTION(void, CheckPreloadedAssetsSaving);
	PROTECTED_FUNCTION(Vector<SelectableDragableObject*>, GetSelectedDragObjects);
	PROTECTED_FUNCTION(Vector<SelectableDragableObject*>, GetAllObjects);
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
	PROTECTED_FUNCTION(void, OnDroppedFromSceneTree, SceneTree*);
	PROTECTED_FUNCTION(void, OnDroppedFromThis);
	PROTECTED_FUNCTION(void, BeginDragging, AssetIcon*);
	PROTECTED_FUNCTION(void, EndDragging, bool);
	PROTECTED_FUNCTION(void, UpdateDraggingGraphics);
}
END_META;
