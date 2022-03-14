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

	// --------------------------
	// Dragable asset icon widget
	// --------------------------
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
		void SetAssetInfo(const AssetInfo* info);

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

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(AssetIcon);

	protected:
		Label*                 mNameText = nullptr;      // Asset name text
		const AssetInfo*       mAssetInfo;               // Asset information
		WidgetState*           mSelectedState = nullptr; // Node selected state
		AssetsIconsScrollArea* mOwner = nullptr;         // Owner assets scroll area

	protected:
		using SelectableDragableObject::OnDrawn;
		using SelectableDragableObject::OnCursorPressed;
		using SelectableDragableObject::OnCursorReleased;

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
	FIELD().PUBLIC().NAME(assetName);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mNameText);
	FIELD().PROTECTED().NAME(mAssetInfo);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSelectedState);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOwner);
}
END_META;
CLASS_METHODS_META(Editor::AssetIcon)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const AssetIcon&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAssetInfo, const AssetInfo*);
	FUNCTION().PUBLIC().SIGNATURE(const AssetInfo&, GetAssetInfo);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAssetName, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(WString, GetAssetName);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsInputTransparent);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PROTECTED().SIGNATURE(void, SetSelected, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorDblClicked, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragStart, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragged, const Input::Cursor&, DragDropArea*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnd, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSelected);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeselected);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDropped, ISelectableDragableObjectsGroup*);
}
END_META;
