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
		bool IsUnderPoint(const Vec2F& point);

		SERIALIZABLE(UIAssetIcon);

	protected:
		UILabel*                 mNameText = nullptr;      // Asset name text
		AssetInfo                mAssetInfo;               // Asset information
		UIWidgetState*           mSelectedState = nullptr; // Node selected state
		UIAssetsIconsScrollArea* mOwner = nullptr;         // Owner assets scroll area

	protected:
		// Sets is this selected
		void SetSelected(bool selected);

		// It is called when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor);

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// It is called when cursor enters this object, moving hover of tree to this
		void OnCursorEnter(const Input::Cursor& cursor);

		// It is called when cursor exits this object, moving hover of tree to this
		void OnCursorExit(const Input::Cursor& cursor);

		// It is called when started dragging
		void OnDragStart(const Input::Cursor& cursor);

		// It is called when dragged
		void OnDragged(const Input::Cursor& cursor, DragDropArea* area);

		// It is called when dragging completed
		void OnDragEnd(const Input::Cursor& cursor);

		// It is called when this was selected
		void OnSelected();

		// It is called when this was unselected
		void OnDeselected();

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group);

		friend class UIAssetsIconsScrollArea;
	};
}
