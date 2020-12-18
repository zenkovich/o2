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

		// It is called when item drag began
		void BeginDragging(LayerPopupItem* item);

		// It is called until dragging
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
		// It is called when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// It is called when started dragging
		void OnDragStart(const Input::Cursor& cursor) override;

		// It is called when dragged
		void OnDragged(const Input::Cursor& cursor, DragDropArea* area) override;

		// It is called when dragging completed
		void OnDragEnd(const Input::Cursor& cursor) override;

		// it is called when name edit box has changed
		void OnNameEditChanged(const WString& str);

		// It is called when visible toggle changed
		void OnVisibleChanged(bool visible);

		// it is called when remove button has pressed
		void OnRemovePressed();

		friend class LayersPopup;
	};
}

CLASS_BASES_META(Editor::LayersPopup)
{
	BASE_CLASS(o2::PopupWidget);
}
END_META;
CLASS_FIELDS_META(Editor::LayersPopup)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mItemSample).PRIVATE();
	FIELD().NAME(mItemsCache).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mDraggingItem).PRIVATE();
	FIELD().NAME(mDragOffset).PRIVATE();
	FIELD().DEFAULT_VALUE(0.4f).NAME(mDragAnimTime).PRIVATE();
	FIELD().DEFAULT_VALUE(Curve::EaseInOut()).NAME(mDragAnimFunc).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAddButtonLayout).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAddButton).PRIVATE();
}
END_META;
CLASS_METHODS_META(Editor::LayersPopup)
{

	PUBLIC_FUNCTION(void, Show, const Vec2F&);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PRIVATE_FUNCTION(void, SpecialDraw);
	PRIVATE_FUNCTION(Vec2F, GetContentSize);
	PRIVATE_FUNCTION(void, InitializeControls);
	PRIVATE_FUNCTION(void, UpdateItemLayout, LayerPopupItem*, int);
	PRIVATE_FUNCTION(void, UpdateLayersList);
	PRIVATE_FUNCTION(void, UpdateLayersListAndFit);
	PRIVATE_FUNCTION(void, BeginDragging, LayerPopupItem*);
	PRIVATE_FUNCTION(void, UpdateDragging);
	PRIVATE_FUNCTION(void, UpdateDragAnimation, float);
	PRIVATE_FUNCTION(void, EndDragging);
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
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLayer).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPopup).PRIVATE();
	FIELD().NAME(mDragHandle).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mVisibleToggle).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mNameCaption).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mRemoveBtn).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mEditBox).PRIVATE();
	FIELD().DEFAULT_VALUE(0.0f).NAME(mDragInsertCoef).PRIVATE();
	FIELD().DEFAULT_VALUE(0.0f).NAME(mDragTargetInsertCoef).PRIVATE();
}
END_META;
CLASS_METHODS_META(Editor::LayerPopupItem)
{

	PUBLIC_FUNCTION(void, SetLayer, SceneLayer*);
	PUBLIC_FUNCTION(void, BeginEditName);
	PUBLIC_FUNCTION(void, BreakEditName);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PRIVATE_FUNCTION(void, OnCursorDblClicked, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnDragStart, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnDragged, const Input::Cursor&, DragDropArea*);
	PRIVATE_FUNCTION(void, OnDragEnd, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnNameEditChanged, const WString&);
	PRIVATE_FUNCTION(void, OnVisibleChanged, bool);
	PRIVATE_FUNCTION(void, OnRemovePressed);
}
END_META;
