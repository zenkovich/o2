#pragma once
#include "o2/Scene/UI/Widgets/PopupWidget.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Core/Ref.h"

using namespace o2;

namespace o2
{
	class Button;
	class EditBox;
	class HorizontalLayout;
	class Toggle;
}

namespace Editor
{
	class LayerPopupItem;

	// -----------------------
	// Scene layers list popup
	// -----------------------
	class LayersPopup : public PopupWidget
	{
	public:
		// Default constructor
		LayersPopup();

		// Destructor
		~LayersPopup();

		// Shows layers popup at position
		void Show(const Vec2F& position);

		// Updates widget and dragging animation
		void Update(float dt) override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(LayersPopup);

	private:
		Ref<LayerPopupItem> mItemSample; // Layer item sample @SERIALIZABLE

		Vector<Ref<LayerPopupItem>> mItemsCache; // Cached items widgets

		Ref<LayerPopupItem> mDraggingItem;                     // Current dragging item
		Vec2F               mDragOffset;                        // Offset from left bottom corner of dragging item to cursor
		float               mDragAnimTime = 0.4f;               // Node expanding time
		Curve               mDragAnimFunc = Curve::EaseInOut(); // Expanding easing node curve

		Ref<HorizontalLayout> mAddButtonLayout; // Add new layer button container layout
		Ref<Button>           mAddButton;       // Add new layer button

	private:
		// Special drawing for contexts
		void SpecialDraw() override;

		// Returns content size for fitting by children
		Vec2F GetContentSize() const override;

		// Initializes item sample, add button
		void InitializeControls();

		// Updates item layout by index and instert coef
		void UpdateItemLayout(Ref<LayerPopupItem> item, int idx);

		// Updates layers list
		void UpdateLayersList();

		// Updates layers list and fits size
		void UpdateLayersListAndFit();

		// Called when item drag began
		void BeginDragging(Ref<LayerPopupItem> item);

		// Called until dragging
		void UpdateDragging();

		// Updates drag animation of items
		void UpdateDragAnimation(float dt);

		// it is colled when item has dropped
		void EndDragging();

		friend class LayerPopupItem;
	};

	// ----------------------
	// Layers view popup item
	// ----------------------
	class LayerPopupItem : public Widget, public SelectableDragableObject
	{
	public:
		// Default constructor
		LayerPopupItem();

		// Copy-constructor
		LayerPopupItem(const LayerPopupItem& other);

		// Copy-operator
		LayerPopupItem& operator=(const LayerPopupItem& other);

		// Sets layer and updates view
		void SetLayer(Ref<SceneLayer> layer);

		// Sets name edit box active
		void BeginEditName();

		// Breaks name editing
		void BreakEditName();

		// Draws widget
		void Draw() override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(LayerPopupItem);

	private:
		Ref<SceneLayer> mLayer;

		Ref<LayersPopup> mPopup;

		DragHandle mDragHandle;

		Ref<Toggle>  mVisibleToggle;
		Ref<Label>   mNameCaption;
		Ref<Button>  mRemoveBtn;
		Ref<EditBox> mEditBox;

		float mDragInsertCoef = 0.0f;
		float mDragTargetInsertCoef = 0.0f;

	private:
		// Called when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// Called when started dragging
		void OnDragStart(const Input::Cursor& cursor) override;

		// Called when dragged
		void OnDragged(const Input::Cursor& cursor, const Ref<DragDropArea>& area) override;

		// Called when dragging completed
		void OnDragEnd(const Input::Cursor& cursor) override;

		// Called when name edit box has changed
		void OnNameEditChanged(const WString& str);

		// Called when visible toggle changed
		void OnVisibleChanged(bool visible);

		// Called when remove button has pressed
		void OnRemovePressed();

		friend class LayersPopup;
	};
}
// --- META ---

CLASS_BASES_META(Editor::LayersPopup)
{
    BASE_CLASS(o2::PopupWidget);
}
END_META;
CLASS_FIELDS_META(Editor::LayersPopup)
{
    FIELD().PRIVATE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mItemSample);
    FIELD().PRIVATE().NAME(mItemsCache);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mDraggingItem);
    FIELD().PRIVATE().NAME(mDragOffset);
    FIELD().PRIVATE().DEFAULT_VALUE(0.4f).NAME(mDragAnimTime);
    FIELD().PRIVATE().DEFAULT_VALUE(Curve::EaseInOut()).NAME(mDragAnimFunc);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mAddButtonLayout);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mAddButton);
}
END_META;
CLASS_METHODS_META(Editor::LayersPopup)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, Show, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PRIVATE().SIGNATURE(void, SpecialDraw);
    FUNCTION().PRIVATE().SIGNATURE(Vec2F, GetContentSize);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeControls);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateItemLayout, Ref<LayerPopupItem>, int);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateLayersList);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateLayersListAndFit);
    FUNCTION().PRIVATE().SIGNATURE(void, BeginDragging, Ref<LayerPopupItem>);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateDragging);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateDragAnimation, float);
    FUNCTION().PRIVATE().SIGNATURE(void, EndDragging);
    
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorDblClicked, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDragStart, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDragged, const Input::Cursor&, const Ref<DragDropArea>&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDragEnd, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnNameEditChanged, const WString&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnVisibleChanged, bool);
    FUNCTION().PRIVATE().SIGNATURE(void, OnRemovePressed);

    FUNCTION().PRIVATE().NUMBER_TYPE_VIRTUAL(float, Alpha, GetAlpha, SetAlpha);
    FUNCTION().PRIVATE().SIGNATURE_VOID_VIRTUAL(void, Draw);
    FUNCTION().PRIVATE().SIGNATURE_BOOL(IsUnderPoint, const Vec2F&);
}
END_META;#include <Ref.h>

Meta(LayerPopupItem,
     ClassBase(o2::Widget),
     ClassBase(o2::SelectableDragableObject),
     MetaNoDefaultPtrRef(mLayer),
     MetaNoDefaultPtrRef(mPopup),
     Meta(mDragHandle),
     MetaNoDefaultPtrRef(mVisibleToggle),
     MetaNoDefaultPtrRef(mNameCaption),
     MetaNoDefaultPtrRef(mRemoveBtn),
     MetaNoDefaultPtrRef(mEditBox),
     Meta(0.0f, mDragInsertCoef),
     Meta(0.0f, mDragTargetInsertCoef),
     MetaConstructor(),
     MetaConstructor(const LayerPopupItem&),
     Meta(mLayer, SetLayer, SceneLayer*),
     Meta(BeginEditName),
     Meta(BreakEditName),
     Meta(Draw),
     Meta(IsUnderPoint, const Vec2F&),
     Meta(static_cast<String (*)(void)>(GetCreateMenuCategory)),
     Meta(OnCursorDblClicked, const Input::Cursor&),
     Meta(OnDragStart, const Input::Cursor&),
     Meta(OnDragged, const Input::Cursor&, DragDropArea*),
     Meta(OnDragEnd, const Input::Cursor&),
     Meta(OnNameEditChanged, const WString&),
     Meta(OnVisibleChanged, bool),
     Meta(OnRemovePressed),
)

CLASS_MEMBER(LayerPopupItem)
{
    Ref<SceneLayer> mLayer;
    Ref<Popup> mPopup;
    WeakRef<LayerDragHandle> mDragHandle;
    Ref<VisibleToggle> mVisibleToggle;
    Ref<NameCaption> mNameCaption;
    Ref<RemoveButton> mRemoveBtn;
    Ref<EditBox> mEditBox;
    float mDragInsertCoef = 0.0f;
    float mDragTargetInsertCoef = 0.0f;
};

void LayerPopupItem::InitializeControls()
{
    mLayer = mmake<SceneLayer>();
    mPopup = mmake<Popup>();
    mVisibleToggle = mmake<VisibleToggle>();
    mNameCaption = mmake<NameCaption>();
    mRemoveBtn = mmake<RemoveButton>();
    mEditBox = mmake<EditBox>();
}

void LayerPopupItem::UpdateItemLayout(LayerPopupItem* parent, int index)
{
    Ref<LayerPopupItem> refParent = parent;
    Ref<LayerPopupItem, false> refParent2 = parent;
    // do something
}

void LayerPopupItem::UpdateLayersList()
{
    const Ref<LayerPopupItem>& selfRef = *this;
    // do something
}

void LayerPopupItem::UpdateLayersListAndFit()
{
    const Ref<LayerPopupItem>& selfRef = *this;
    // do something
}

void LayerPopupItem::BeginDragging(LayerPopupItem* item)
{
    Ref<LayerPopupItem> refItem = item;
    // do something
}

void LayerPopupItem::UpdateDragging()
{
    const Ref<LayerPopupItem>& selfRef = *this;
    // do something
}

void LayerPopupItem::UpdateDragAnimation(float dt)
{
    const Ref<LayerPopupItem>& selfRef = *this;
    // do something
}

void LayerPopupItem::EndDragging()
{
    const Ref<LayerPopupItem>& selfRef = *this;
    // do something
}