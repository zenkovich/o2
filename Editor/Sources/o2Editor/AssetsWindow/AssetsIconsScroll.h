#pragma once

#include "o2/Assets/AssetInfo.h"
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Scene/UI/Widgets/GridLayoutScrollArea.h"
#include "o2/Utils/Editor/DragAndDrop.h"
#include "o2/Utils/Types/Containers/Pair.h"
#include "o2/Utils/Pointers/Ref.h"
#include "o2/Utils/Pointers/WeakRef.h"
#include "o2/Utils/Pointers/make_ref.h"

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
	class SceneHierarchyTree;
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

		// Creates asset and starts renaming
		void CreateAsset(const Type* assetType);

		// Highlights asset
		void HighlightAsset(const UID& id);

		// Selects asset
		void SelectAsset(const UID& id, bool scroll = true);

		// Deselects all assets
		void DeselectAllAssets();

		// Returns selected assets infos
		const Vector<Ref<AssetInfo>>& GetSelectedAssets() const;

		// Return asset icon under point
		AssetIcon* GetIconUnderPoint(const Vec2F& point) const;

		// Returns node highlight drawable
		Sprite* GetHighlightDrawable() const;

		// Sets highlight animation
		void SetHighlightAnimation(const Ref<AnimationClip>& animation);

		// Sets highlight layout
		void SetHighlightLayout(const Layout& layout);

		// Returns selecting rectangle drawable
		Sprite* GetSelectingDrawable() const;

		// Returns is this widget can be selected
		bool IsFocusable() const override;

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

        Vector<Ref<AssetInfo>> mAssetInfos;        // Asset infos in path @IGNORE
        Vector<AssetIcon*>     mVisibleAssetIcons; // Visible asset icons

		ContextMenu* mContextMenu = nullptr; // Assets Context menu
						        
		Vector<Ref<AssetInfo>> mSelectedAssets;          // Selected assets icons @IGNORE
		Vector<Ref<Asset>>     mSelectedPreloadedAssets; // Preloaded selected assets

		Ref<Asset> mNewAsset; // Temporary new asset. Used when creating new asset
						        
		AssetIcon*           mHighlightIcon = nullptr;                  // Current highlighting asset icon
		Ref<AnimationClip>   mHighlighClip;                             // Node highlight animation clip @SERIALIZABLE 
		Ref<AnimationPlayer> mHighlightAnim = make_ref<AnimationPlayer>(); // Icon highlight animation
		Sprite*              mHighlightSprite = nullptr;                // Icon highlight sprite @SERIALIZABLE
		Layout               mHighlightLayout;                          // Icon highlight sprite layout @SERIALIZABLE
						        
		Map<String, Vector<AssetIcon*>> mIconsPool; // Assets icons pool
						        
		Sprite*                mSelectionSprite = nullptr;  // Icons selection rectangle sprite @SERIALIZABLE
		bool                   mSelecting = false;          // Is selecting icons 
		Vec2F                  mPressedPoint;               // Pressed point
		Vector<Ref<AssetInfo>> mCurrentSelectingInfos;      // Selecting icons at current selection @IGNORE
						        
		bool                         mIsDraggingIcons = false;      // Is dragging icons
		bool                         mDragEnded = false;            // Is dragging ended
		AssetIcon*                   mDragIcon = nullptr;           // Dragging icon
		Vec2F                        mDragOffset;                   // Dragging offset from cursor to icon center
		Vector<SceneEditableObject*> mInstantiatedSceneDragObjects; // Instantiated objects when dragging asset above scene

		Vector<Pair<UID, String>> mC;#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <string>

template<typename T>
class Ref {
public:
    Ref() : ptr(nullptr) {}
    explicit Ref(T* p) : ptr(p) {}
    Ref(const Ref& other) = default;
    Ref(Ref&& other) = default;
    template<typename U>
    Ref(const Ref<U>& other) : ptr(other.Get()) {}

    Ref& operator=(const Ref& other) = default;
    Ref& operator=(Ref&& other) = default;
    template<typename U>
    Ref& operator=(const Ref<U>& other) {
        ptr = other.Get();
        return *this;
    }
    
    T* operator->() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    T* Get() const {
        return ptr;
    }

    T* operator[](int index) const {
        return ptr[index];
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    bool operator==(const Ref& other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const Ref& other) const {
        return ptr != other.ptr;
    }


private:
    T* ptr;
};

template<typename T>
class WeakRef {
public:
    WeakRef() : ptr(nullptr) {}
    explicit WeakRef(T* p) : ptr(p) {}
    WeakRef(const WeakRef& other) = default;
    WeakRef(WeakRef&& other) = default;

    WeakRef& operator=(const WeakRef& other) = default;
    WeakRef& operator=(WeakRef&& other) = default;

    operator Ref<T>() const {
        return Ref<T>(ptr);
    }

    T* operator->() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    T* Get() const {
        return ptr;
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    bool operator==(const WeakRef& other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const WeakRef& other) const {
        return ptr != other.ptr;
    }

private:
    T* ptr;
};

namespace Input {
    struct Cursor {};
    struct Key {};
}

struct Widget {
    void Update() {}
};

class AssetIcon {
public:
    void SetImage(const std::string& img) {}
};

class String {
public:
    String() : value("") {}
    explicit String(const char* str) : value(str) {}
    explicit String(const std::string& str) : value(str) {}

    operator const char*() const {
        return value.c_str();
    }

    std::string value;
};

class SelectableDragableObject {
public:
    virtual ~SelectableDragableObject() = default;
};

class Actor {
public:
    virtual ~Actor() = default;
};

template<typename T>
T* mmake() {
    return new T();
}

template<typename T>
int DynamicCast(const Ref<T>& ref) {
    return dynamic_cast<T*>(ref.Get());
}

class ImageAssetRef {
public:
    ImageAssetRef() : asset(nullptr) {}
    explicit ImageAssetRef(Ref<AssetIcon>* a) : asset(a) {}

    Ref<AssetIcon>* asset;
};

class ActorAsset {
public:
    ActorAsset() : asset(nullptr) {}
    explicit ActorAsset(Ref<AssetIcon>* a) : asset(a) {}
    
    Ref<AssetIcon>* asset;
};

class ISelectableDragableObjectsGroup {
public:
    virtual ~ISelectableDragableObjectsGroup() = default;

    virtual std::vector<SelectableDragableObject*> GetSelectedDragObjects() const = 0;

    virtual std::vector<SelectableDragableObject*> GetAllObjects() const = 0;

    virtual void Select(SelectableDragableObject* object) = 0;

    virtual void Select(SelectableDragableObject* object, bool sendOnSelectionChanged) = 0;

    virtual void Deselect(SelectableDragableObject* object) = 0;

    virtual void AddSelectableObject(SelectableDragableObject* object) = 0;

    virtual void RemoveSelectableObject(const Ref<SelectableDragableObject>& object) = 0;

    virtual void OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor) = 0;
};

class AssetInfo {
public:
    AssetInfo() : icon(nullptr) {}
    explicit AssetInfo(Ref<AssetIcon>* i) : icon(i) {}

    Ref<AssetIcon>* icon;
};

template<typename T>
Actor* AssetInstantiate(const T& asset);

template<>
Actor* AssetInstantiate<>(const ImageAssetRef& asset) {
    return nullptr;
}

template<>
Actor* AssetInstantiate<>(const Ref<ActorAsset>& asset) {
    return nullptr;
}

class AssetsManager : public ISelectableDragableObjectsGroup {
public:
    AssetsManager() : mChangePropertiesTargetsFromThis(false) {}

    int GetItemsCount() const override {
        return 0;
    }

    std::vector<void*> GetItemsRange(int start, int end) const override {
        return std::vector<void*>();
    }

    void SetupItemWidget(Widget* widget, void* item) override {
        widget->Update();
    }

    void UpdateVisibleItems() override {

    }

    void OnFocused() override {

    }

    void OnUnfocused() override {

    }

    void OnCursorPressed(const Input::Cursor& cursor) override {

    }

    void OnCursorReleased(const Input::Cursor& cursor) override {

    }

    void OnCursorPressBreak(const Input::Cursor& cursor) override {

    }

    void OnCursorStillDown(const Input::Cursor& cursor) override {

    }

    void OnCursorMoved(const Input::Cursor& cursor) override {

    }

    void OnCursorRightMouseReleased(const Input::Cursor& cursor) override {

    }

    void OnScrolled(float scroll) override {

    }

    void OnKeyReleased(const Input::Key& key) override {

    }

    void OnAssetsSelected() {

    }

    void UpdateCuttingAssets() {

    }

    void BeginSelecting() {

    }

    void UpdateSelection(const Input::Cursor& cursor) {

    }

    void CompleteSelecting() {

    }

    void SortAssetInfos() {

    }

    void RegObjectsCreationAction() {

    }

    void InitializeContext() {

    }

    void InitializeCreateContext() {

    }

    AssetIcon* GetAssetIconFromPool(const String& style) {
        return nullptr;
    }

    void FreeAssetIconToPool(AssetIcon* icon) {

    }

    AssetIcon* FindVisibleIcon(const Ref<AssetInfo>& info) {
        return nullptr;
    }

    void OnAssetDblClick(AssetIcon* icon) {

    }

    void StartAssetRenaming(AssetIcon* icon, const String& name, const std::function<void(const String&)>& onCompleted) {

    }

    void OnContextCopyPressed() {

    }

    void OnContextCutPressed() {

    }

    void OnContextPastePressed() {

    }

    void OnContextDeletePressed() {

    }

    void OnContextOpenPressed() {

    }

    void OnContextShowInExplorerPressed() {

    }

    void InstantiateDraggingAssets() {

    }

    void ClearInstantiatedDraggingAssets() {

    }

    Actor* InstantiateAsset(const AssetInfo& assetInfo) {
        return nullptr;
    }

    template<typename _type>
    Actor* InstantiateAsset(const _type& asset) {
        return AssetInstantiate(asset);
    }

    void OnAssetsPropertiesChanged() {

    }

    void CheckPreloadedAssetsSaving() {

    }

    std::vector<SelectableDragableObject*> GetSelectedDragObjects() const override {
        return std::vector<SelectableDragableObject*>();
    }

    std::vector<SelectableDragableObject*> GetAllObjects() const override {
        return std::vector<SelectableDragableObject*>();
    }

    void Select(SelectableDragableObject* object) override {

    }

    void Select(SelectableDragableObject* object, bool sendOnSelectionChanged) override {

    }

    void Deselect(SelectableDragableObject* object) override {

    }

    void AddSelectableObject(SelectableDragableObject* object) override {

    }

    void RemoveSelectableObject(const Ref<SelectableDragableObject>& object) override {

    }

    void OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor) override {

    }

private:
    Ref<AssetIcon> mAssets;
    std::vector<WeakRef<AssetIcon>> nuttingAssets;
    bool mChangePropertiesTargetsFromThis;
};void OnSelectableObjectBeganDragging(const Ref<SelectableDragableObject>& object) override;

// DragDropArea implementation
void OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group) override;
void OnDraggedAbove(const Ref<ISelectableDragableObjectsGroup>& group) override;
void OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group) override;
void OnDropped(const Ref<ISelectableDragableObjectsGroup>& group) override;
void OnDroppedFromSceneTree(const Ref<SceneHierarchyTree>& sceneTree);
void OnDroppedFromThis();
void BeginDragging(const Ref<AssetIcon>& icon);
void EndDragging(bool droppedToThis = false);
void UpdateDraggingGraphics();
friend class AssetsWindow;
friend class SceneEditScreen;
friend class SceneHierarchyTree;
friend class AssetIcon;
friend class SceneTree;
};

template<typename _type>
Actor* AssetsIconsScrollArea::InstantiateAsset(const Ref<_type>& asset)
{
    return nullptr;
}
}
// --- META ---

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
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<AnimationPlayer>()).NAME(mHighlightAnim);
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
    FUNCTION().PUBLIC().SIGNATURE(void, CreateAsset, const Type*);
    FUNCTION().PUBLIC().SIGNATURE(void, HighlightAsset, const UID&);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAsset, const UID&, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, DeselectAllAssets);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Ref<AssetInfo>>&, GetSelectedAssets);
    FUNCTION().PUBLIC().SIGNATURE(Ref<AssetIcon>, GetIconUnderPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Sprite>, GetHighlightDrawable);
    FUNCTION().PUBLIC().SIGNATURE(void, SetHighlightAnimation, const Ref<AnimationClip>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetHighlightLayout, const Layout&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Sprite>, GetSelectingDrawable);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsFocusable);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateSelfTransform);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsScrollable);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsInputTransparent);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PROTECTED().SIGNATURE(int, GetItemsCount);
    FUNCTION().PROTECTED().SIGNATURE(Vector<Ref<void>>, GetItemsRange, int, int);
    FUNCTION().PROTECTED().SIGNATURE(void, SetupItemWidget, Ref<Widget>, Ref<void>);
}Ref<AssetIcon> mmake<AssetIcon>(const String& path) {
  return mmake<AssetIcon>();
}

void AssetBrowser::OnAssetsSelected(const Ref<AssetInfo>& assetInfo) {
  // implementation
}

void AssetBrowser::UpdateCuttingAssets() {
  // implementation
}

void AssetBrowser::BeginSelecting() {
  // implementation
}

void AssetBrowser::UpdateSelection(const Input::Cursor& cursor) {
  // implementation
}

void AssetBrowser::CompleteSelecting() {
  // implementation
}

void AssetBrowser::SortAssetInfos() {
  // implementation
}

void AssetBrowser::RegObjectsCreationAction() {
  // implementation
}

void AssetBrowser::InitializeContext() {
  // implementation
}

void AssetBrowser::InitializeCreateContext() {
  // implementation
}

Ref<AssetIcon> AssetBrowser::GetAssetIconFromPool(const String& path) {
  return mmake<AssetIcon>();
}

void AssetBrowser::FreeAssetIconToPool(AssetIcon* assetIcon) {
  // implementation
}

Ref<AssetIcon> AssetBrowser::FindVisibleIcon(const Ref<AssetInfo>& assetInfo) {
  return mmake<AssetIcon>();
}

void AssetBrowser::OnAssetDblClick(AssetIcon* assetIcon) {
  // implementation
}

void AssetBrowser::StartAssetRenaming(AssetIcon* assetIcon, const String& newName, const Function<void(const String&)>& callback) {
  // implementation
}

void AssetBrowser::OnContextCopyPressed() {
  // implementation
}

void AssetBrowser::OnContextCutPressed() {
  // implementation
}

void AssetBrowser::OnContextPastePressed() {
  // implementation
}

void AssetBrowser::OnContextDeletePressed() {
  // implementation
}

void AssetBrowser::OnContextOpenPressed() {
  // implementation
}

void AssetBrowser::OnContextShowInExplorerPressed() {
  // implementation
}

void AssetBrowser::InstantiateDraggingAssets() {
  // implementation
}

void AssetBrowser::ClearInstantiatedDraggingAssets() {
  // implementation
}

Actor* AssetBrowser::InstantiateAsset(const AssetInfo& assetInfo) {
  return nullptr;
}

Actor* AssetBrowser::InstantiateAsset(const ImageAssetRef& imageAsset) {
  return nullptr;
}

Actor* AssetBrowser::InstantiateAsset(const Ref<ActorAsset>& actorAsset) {
  return nullptr;
}

void AssetBrowser::OnAssetsPropertiesChanged() {
  // implementation
}

void AssetBrowser::CheckPreloadedAssetsSaving() {
  // implementation
}

Vector<SelectableDragableObject*> AssetBrowser::GetSelectedDragObjects() {
  return Vector<SelectableDragableObject*>();
}

Vector<SelectableDragableObject*> AssetBrowser::GetAllObjects() {
  return Vector<SelectableDragableObject*>();
}

void AssetBrowser::Select(SelectableDragableObject* object) {
  // implementation
}

void AssetBrowser::Select(SelectableDragableObject* object, bool multiple) {
  // implementation
}

void AssetBrowser::Deselect(SelectableDragableObject* object) {
  // implementation
}

void AssetBrowser::AddSelectableObject(SelectableDragableObject* object) {
  // implementation
}

void AssetBrowser::RemoveSelectableObject(SelectableDragableObject* object) {
  // implementation
}

void AssetBrowser::OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor) {
  // implementation
}

void AssetBrowser::OnSelectableObjectBeganDragging(SelectableDragableObject* object) {
  // implementation
}

void AssetBrowser::OnDragEnter(ISelectableDragableObjectsGroup* group) {
  // implementation
}

void AssetBrowser::OnDraggedAbove(ISelectableDragableObjectsGroup* group) {
  // implementation
}

void AssetBrowser::OnDragExit(ISelectableDragableObjectsGroup* group) {
  // implementation
}

void AssetBrowser::OnDropped(ISelectableDragableObjectsGroup* group) {
  // implementation
}

void AssetBrowser::OnDroppedFromSceneTree(SceneHierarchyTree* tree) {
  // implementation
}

void AssetBrowser::OnDroppedFromThis() {
  // implementation
}

void AssetBrowser::BeginDragging(AssetIcon* assetIcon) {
  // implementation
}

void AssetBrowser::EndDragging(bool cancel) {
  // implementation
}

void AssetBrowser::UpdateDraggingGraphics() {
  // implementation
}