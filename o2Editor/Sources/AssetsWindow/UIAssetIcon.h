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

		// Calls when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor);

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// Calls when cursor enters this object, moving hover of tree to this
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object, moving hover of tree to this
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when started dragging
		void OnDragStart(const Input::Cursor& cursor);

		// Calls when dragged
		void OnDragged(const Input::Cursor& cursor, DragDropArea* area);

		// Calls when dragging completed
		void OnDragEnd(const Input::Cursor& cursor);

		// Calls when this was selected
		void OnSelected();

		// Calls when this was unselected
		void OnDeselected();

		// Calls when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group);

		friend class UIAssetsIconsScrollArea;
	};
}