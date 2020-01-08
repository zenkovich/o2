#pragma once

#include "o2/Assets/AssetInfo.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Editor/DragAndDrop.h"

using namespace o2;

namespace o2
{
	class Label;
}

namespace Editor
{
	class AssetsIconsScrollArea;

	// ---------------------------
	// Draggable asset icon widget
	// ---------------------------
	class AssetIcon: public Widget, public SelectableDragableObject, public DragDropArea
	{
	public:
		PROPERTIES(AssetIcon);
		PROPERTY(WString, assetName, SetAssetName, GetAssetName); // Asset name label text property

	public:
		// Default constructor
		AssetIcon();

		// Copy-constructor
		AssetIcon(const AssetIcon& other);

		// Destructor
		~AssetIcon();

		// Copy operator
		AssetIcon& operator=(const AssetIcon& other);

		// Sets asset info
		void SetAssetInfo(const AssetInfo& info);

		// Returns asset info
		const AssetInfo& GetAssetInfo() const;

		// Sets name label text
		void SetAssetName(const WString& name);

		// Returns name label text
		WString GetAssetName() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

		SERIALIZABLE(AssetIcon);

	protected:
		Label*                 mNameText = nullptr;      // Asset name text
		AssetInfo              mAssetInfo;               // Asset information
		WidgetState*           mSelectedState = nullptr; // Node selected state
		AssetsIconsScrollArea* mOwner = nullptr;         // Owner assets scroll area

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

		friend class AssetsIconsScrollArea;
	};
}

CLASS_BASES_META(Editor::AssetIcon)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::SelectableDragableObject);
	BASE_CLASS(o2::DragDropArea);
}
END_META;
CLASS_FIELDS_META(Editor::AssetIcon)
{
	PUBLIC_FIELD(assetName);
	PROTECTED_FIELD(mNameText);
	PROTECTED_FIELD(mAssetInfo);
	PROTECTED_FIELD(mSelectedState);
	PROTECTED_FIELD(mOwner);
}
END_META;
CLASS_METHODS_META(Editor::AssetIcon)
{

	PUBLIC_FUNCTION(void, SetAssetInfo, const AssetInfo&);
	PUBLIC_FUNCTION(const AssetInfo&, GetAssetInfo);
	PUBLIC_FUNCTION(void, SetAssetName, const WString&);
	PUBLIC_FUNCTION(WString, GetAssetName);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsInputTransparent);
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
