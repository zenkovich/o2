#pragma once
#include "o2/Scene/UI/Widgets/PopupWidget.h"
#include "o2/Utils/Editor/DragHandle.h"

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
	class LayersPopup: public PopupWidget
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
		LayerPopupItem* mItemSample = nullptr; // Layer item sample @SERIALIZABLE

		Vector<LayerPopupItem*> mItemsCache; // Cached items widgets

		LayerPopupItem* mDraggingItem = nullptr;            // Current dragging item
		Vec2F           mDragOffset;                        // Offset from left bottom corner of dragging item to cursor
		float           mDragAnimTime = 0.4f;               // Node expanding time
		Curve           mDragAnimFunc = Curve::EaseInOut(); // Expanding easing node curve

		HorizontalLayout* mAddButtonLayout = nullptr; // Add new layer button container layout
		Button*           mAddButton = nullptr;       // Add new layer button

	private:
		// Special drawing for contexts
		void SpecialDraw() override;

		// Returns content size for fitting by children
		Vec2F GetContentSize() const override;

		// Initializes item sample, add button
		void InitializeControls();

		// Updates item layout by index and instert coef
		void UpdateItemLayout(LayerPopupItem* item, int idx);

		// Updates layers list
		void UpdateLayersList();

		// Updates layers list and fits size
		void UpdateLayersListAndFit();

		// Called when item drag began
		void BeginDragging(LayerPopupItem* item);

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
	class LayerPopupItem: public Widget, public SelectableDragableObject
	{
	public:
		// Default constructor
		LayerPopupItem();

		// Copy-constructor
		LayerPopupItem(const LayerPopupItem& other);

		// Copy-operator
		LayerPopupItem& operator=(const LayerPopupItem& other);

		// Sets layer and updates view
		void SetLayer(SceneLayer* layer);

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
		SceneLayer* mLayer = nullptr;

		LayersPopup* mPopup = nullptr;

		DragHandle mDragHandle;

		Toggle*  mVisibleToggle = nullptr;
		Label*   mNameCaption = nullptr;
		Button*  mRemoveBtn = nullptr;
		EditBox* mEditBox = nullptr;

		float mDragInsertCoef = 0.0f;
		float mDragTargetInsertCoef = 0.0f;

	private:
		// Called when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// Called when started dragging
		void OnDragStart(const Input::Cursor& cursor) override;

		// Called when dragged
		void OnDragged(const Input::Cursor& cursor, DragDropArea* area) override;

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
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateItemLayout, LayerPopupItem*, int);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateLayersList);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateLayersListAndFit);
    FUNCTION().PRIVATE().SIGNATURE(void, BeginDragging, LayerPopupItem*);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateDragging);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateDragAnimation, float);
    FUNCTION().PRIVATE().SIGNATURE(void, EndDragging);
}
END_META;

CLASS_BASES_META(Editor::LayerPopupItem)
{
    BASE_CLASS(o2::Widget);
    BASE_CLASS(o2::SelectableDragableObject);
}
END_META;
CLASS_FIELDS_META(Editor::LayerPopupItem)
{
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mLayer);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mPopup);
    FIELD().PRIVATE().NAME(mDragHandle);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mVisibleToggle);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mNameCaption);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mRemoveBtn);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mEditBox);
    FIELD().PRIVATE().DEFAULT_VALUE(0.0f).NAME(mDragInsertCoef);
    FIELD().PRIVATE().DEFAULT_VALUE(0.0f).NAME(mDragTargetInsertCoef);
}
END_META;
CLASS_METHODS_META(Editor::LayerPopupItem)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const LayerPopupItem&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetLayer, SceneLayer*);
    FUNCTION().PUBLIC().SIGNATURE(void, BeginEditName);
    FUNCTION().PUBLIC().SIGNATURE(void, BreakEditName);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorDblClicked, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDragStart, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDragged, const Input::Cursor&, DragDropArea*);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDragEnd, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnNameEditChanged, const WString&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnVisibleChanged, bool);
    FUNCTION().PRIVATE().SIGNATURE(void, OnRemovePressed);
}
END_META;
// --- END META ---
