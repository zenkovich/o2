#pragma once

#include "Assets/AssetInfo.h"
#include "UI/Widget.h"
#include "Utils/DragAndDrop.h"

using namespace o2;

namespace o2
{
	class UILabel;
}

namespace Editor
{
	class UIAssetsIconsScrollArea;

	// ---------------------------
	// Draggable asset icon widget
	// ---------------------------
	class UIAssetIcon: public UIWidget, public SelectableDragableObject, public DragDropArea
	{
	public:
		// Default constructor
		UIAssetIcon();

		// Copy-constructor
		UIAssetIcon(const UIAssetIcon& other);

		// Destructor
		~UIAssetIcon();

		// Copy operator
		UIAssetIcon& operator=(const UIAssetIcon& other);

		// Sets asset info
		void SetAssetInfo(const AssetInfo& info);

		// Returns asset info
		const AssetInfo& GetAssetInfo() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		SERIALIZABLE(UIAssetIcon);

	protected:
		UILabel*                 mNameText = nullptr;      // Asset name text
		AssetInfo                mAssetInfo;               // Asset information
		UIWidgetState*           mSelectedState = nullptr; // Node selected state
		UIAssetsIconsScrollArea* mOwner = nullptr;         // Owner assets scroll area

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Sets is this selected
		void SetSelected(bool selected);

		// It is called when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// It is called when cursor enters this object, moving hover of tree to this
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object, moving hover of tree to this
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when started dragging
		void OnDragStart(const Input::Cursor& cursor) override;

		// It is called when dragged
		void OnDragged(const Input::Cursor& cursor, DragDropArea* area) override;

		// It is called when dragging completed
		void OnDragEnd(const Input::Cursor& cursor) override;

		// It is called when this was selected
		void OnSelected() override;

		// It is called when this was unselected
		void OnDeselected() override;

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		friend class UIAssetsIconsScrollArea;
	};
}

CLASS_BASES_META(Editor::UIAssetIcon)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::SelectableDragableObject);
	BASE_CLASS(o2::DragDropArea);
}
END_META;
CLASS_FIELDS_META(Editor::UIAssetIcon)
{
	PROTECTED_FIELD(mNameText);
	PROTECTED_FIELD(mAssetInfo);
	PROTECTED_FIELD(mSelectedState);
	PROTECTED_FIELD(mOwner);
}
END_META;
CLASS_METHODS_META(Editor::UIAssetIcon)
{

	PUBLIC_FUNCTION(void, SetAssetInfo, const AssetInfo&);
	PUBLIC_FUNCTION(const AssetInfo&, GetAssetInfo);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, SetSelected, bool);
	PROTECTED_FUNCTION(void, OnCursorDblClicked, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnDragStart, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnDragged, const Input::Cursor&, DragDropArea*);
	PROTECTED_FUNCTION(void, OnDragEnd, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnSelected);
	PROTECTED_FUNCTION(void, OnDeselected);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
}
END_META;
