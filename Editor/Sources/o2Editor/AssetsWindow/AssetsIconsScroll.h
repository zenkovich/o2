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
		bool IsFocusable() const override;

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
		void SetHighlightAnimation(const AnimationClip& animation);

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

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(AssetsIconsScrollArea);

	protected:
		const Vec2F mAssetIconSize = Vec2F(50, 60);
						        
		String mCurrentPath = "_"; // Current viewing path

		Vector<const AssetInfo*> mAssetInfos;        // Asset infos in path @IGNORE
		Vector<AssetIcon*>       mVisibleAssetIcons; // Visible asset icons

		ContextMenu* mContextMenu = nullptr; // Assets Context menu
						        
		Vector<const AssetInfo*> mSelectedAssets;          // Selected assets icons @IGNORE
		Vector<AssetRef*>        mSelectedPreloadedAssets; // Preloaded selected assets

		AssetRef mNewAsset; // Temporary new asset. Used when creating new asset
						        
		AssetIcon*      mHighlightIcon = nullptr;   // Current highlighting asset icon
		AnimationClip   mHighlighClip;              // Node highlight animation clip @SERIALIZABLE 
		AnimationPlayer mHighlightAnim;             // Icon highlight animation
		Sprite*         mHighlightSprite = nullptr; // Icon highlight sprite @SERIALIZABLE
		Layout          mHighlightLayout;           // Icon highlight sprite layout @SERIALIZABLE
						        
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
		// Returns items count, calls getItemsCountFunc
		int GetItemsCount() const override;

		// Returns items in range from start to end, calls getItemsRangeFunc
		Vector<void*> GetItemsRange(int start, int end) const override;

		// Sets item widget, calls setupItemFunc
		void SetupItemWidget(Widget* widget, void* item) override;

		// Updates visible items
		void UpdateVisibleItems() override;

		// Called when widget was selected
		void OnFocused() override;

		// Called when widget was deselected
		void OnUnfocused() override;

		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// Called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// Called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// Called when scrolling
		void OnScrolled(float scroll) override;

		// Called when key was released
		void OnKeyReleased(const Input::Key& key) override;

		// Called when assets selection was changed
		void OnAssetsSelected();

		// Updates cutting assets
		void UpdateCuttingAssets();

		// Begins selecting icons
		void BeginSelecting();

		// Updates selection frame
		void UpdateSelection(const Input::Cursor& cursor);

		// Completes selecting
		void CompleteSelecting();

		// Sorts current asset infos
		void SortAssetInfos();

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

		// Called when asset icon double clicked, starting editing name
		void OnAssetDblClick(AssetIcon* icon);

		// Starts asset icon renaming, calls onCompletedwhen completed
		void StartAssetRenaming(AssetIcon* icon, const String& name, const Function<void(const String&)>& onCompleted);

		// Called when context copy pressed
		void OnContextCopyPressed();

		// Called when context cut pressed
		void OnContextCutPressed();

		// Called when context paste pressed
		void OnContextPastePressed();

		// Called when context delete pressed
		void OnContextDeletePressed();

		// Called when context open pressed
		void OnContextOpenPressed();

		// Called when context show in explorer pressed
		void OnContextShowInExplorerPressed();

		// Called when context create asset pressed
		void OnContextCreateAssetPressed(const Type* assetType);

		// Called when context create folder pressed
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

		// Called when assets was changed from properties
		void OnAssetsPropertiesChanged();

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

		// Called when selectable draggable object was released
		void OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor) override;

		// Called when selectable object was began to drag
		void OnSelectableObjectBeganDragging(SelectableDragableObject* object) override;

// DragDropArea implementation
		// Called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// Called when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group) override;

		// Called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		// Called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		// Called when dropped dragged scene tree nodes selected and started dragging from scene tree
		void OnDroppedFromSceneTree(SceneTree* sceneTree);

		// Called when dropped dragged assets icons selected and started dragging from this
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
	FIELD().PUBLIC().NAME(onAssetsSelected);
	FIELD().PROTECTED().DEFAULT_VALUE(Vec2F(50, 60)).NAME(mAssetIconSize);
	FIELD().PROTECTED().DEFAULT_VALUE("_").NAME(mCurrentPath);
	FIELD().PROTECTED().NAME(mVisibleAssetIcons);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mContextMenu);
	FIELD().PROTECTED().NAME(mSelectedPreloadedAssets);
	FIELD().PROTECTED().NAME(mNewAsset);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHighlightIcon);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mHighlighClip);
	FIELD().PROTECTED().NAME(mHighlightAnim);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mHighlightSprite);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mHighlightLayout);
	FIELD().PROTECTED().NAME(mIconsPool);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSelectionSprite);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mSelecting);
	FIELD().PROTECTED().NAME(mPressedPoint);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsDraggingIcons);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mDragEnded);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDragIcon);
	FIELD().PROTECTED().NAME(mDragOffset);
	FIELD().PROTECTED().NAME(mInstantiatedSceneDragObjects);
	FIELD().PROTECTED().NAME(mCuttingAssets);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mNeedRebuildAssets);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mChangePropertiesTargetsFromThis);
}
END_META;
CLASS_METHODS_META(Editor::AssetsIconsScrollArea)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const AssetsIconsScrollArea&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetViewingPath, const String&);
	FUNCTION().PUBLIC().SIGNATURE(const String&, GetViewingPath);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateAssetsByCurrentPath);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsFocusable);
	FUNCTION().PUBLIC().SIGNATURE(void, HighlightAsset, const UID&);
	FUNCTION().PUBLIC().SIGNATURE(void, SelectAsset, const UID&, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, DeselectAllAssets);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<const AssetInfo*>&, GetSelectedAssets);
	FUNCTION().PUBLIC().SIGNATURE(AssetIcon*, GetIconUnderPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Sprite*, GetHighlightDrawable);
	FUNCTION().PUBLIC().SIGNATURE(void, SetHighlightAnimation, const AnimationClip&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetHighlightLayout, const Layout&);
	FUNCTION().PUBLIC().SIGNATURE(Sprite*, GetSelectingDrawable);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateSelfTransform);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsScrollable);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsInputTransparent);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PROTECTED().SIGNATURE(int, GetItemsCount);
	FUNCTION().PROTECTED().SIGNATURE(Vector<void*>, GetItemsRange, int, int);
	FUNCTION().PROTECTED().SIGNATURE(void, SetupItemWidget, Widget*, void*);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateVisibleItems);
	FUNCTION().PROTECTED().SIGNATURE(void, OnFocused);
	FUNCTION().PROTECTED().SIGNATURE(void, OnUnfocused);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressBreak, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnScrolled, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnKeyReleased, const Input::Key&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAssetsSelected);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateCuttingAssets);
	FUNCTION().PROTECTED().SIGNATURE(void, BeginSelecting);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateSelection, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, CompleteSelecting);
	FUNCTION().PROTECTED().SIGNATURE(void, SortAssetInfos);
	FUNCTION().PROTECTED().SIGNATURE(void, RegObjectsCreationAction);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeContext);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeCreateContext);
	FUNCTION().PROTECTED().SIGNATURE(AssetIcon*, GetAssetIconFromPool, const String&);
	FUNCTION().PROTECTED().SIGNATURE(void, FreeAssetIconToPool, AssetIcon*);
	FUNCTION().PROTECTED().SIGNATURE(AssetIcon*, FindVisibleIcon, const AssetInfo*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAssetDblClick, AssetIcon*);
	FUNCTION().PROTECTED().SIGNATURE(void, StartAssetRenaming, AssetIcon*, const String&, const Function<void(const String&)>&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnContextCopyPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnContextCutPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnContextPastePressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnContextDeletePressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnContextOpenPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnContextShowInExplorerPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnContextCreateAssetPressed, const Type*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnContextCreateFolderPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, InstantiateDraggingAssets);
	FUNCTION().PROTECTED().SIGNATURE(void, ClearInstantiatedDraggingAssets);
	FUNCTION().PROTECTED().SIGNATURE(Actor*, InstantiateAsset, const AssetInfo&);
	FUNCTION().PROTECTED().SIGNATURE(Actor*, InstantiateAsset, const ImageAssetRef&);
	FUNCTION().PROTECTED().SIGNATURE(Actor*, InstantiateAsset, const ActorAssetRef&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAssetsPropertiesChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, CheckPreloadedAssetsSaving);
	FUNCTION().PROTECTED().SIGNATURE(Vector<SelectableDragableObject*>, GetSelectedDragObjects);
	FUNCTION().PROTECTED().SIGNATURE(Vector<SelectableDragableObject*>, GetAllObjects);
	FUNCTION().PROTECTED().SIGNATURE(void, Select, SelectableDragableObject*);
	FUNCTION().PROTECTED().SIGNATURE(void, Select, SelectableDragableObject*, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, Deselect, SelectableDragableObject*);
	FUNCTION().PROTECTED().SIGNATURE(void, AddSelectableObject, SelectableDragableObject*);
	FUNCTION().PROTECTED().SIGNATURE(void, RemoveSelectableObject, SelectableDragableObject*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSelectableObjectCursorReleased, SelectableDragableObject*, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSelectableObjectBeganDragging, SelectableDragableObject*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragExit, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDropped, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDroppedFromSceneTree, SceneTree*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDroppedFromThis);
	FUNCTION().PROTECTED().SIGNATURE(void, BeginDragging, AssetIcon*);
	FUNCTION().PROTECTED().SIGNATURE(void, EndDragging, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateDraggingGraphics);
}
END_META;
