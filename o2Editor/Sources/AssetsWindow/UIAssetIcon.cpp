#include "UIAssetIcon.h"

#include "AssetsWindow/AssetsIconsScroll.h"
#include "Render/Text.h"
#include "UI/Label.h"
#include "UI/WidgetState.h"
#include "Utils/FileSystem/FileSystem.h"

namespace Editor
{
	UIAssetIcon::UIAssetIcon():
		UIWidget(), mNameText(nullptr)
	{
		onDraw += [&]() { CursorAreaEventsListener::OnDrawn(); };
		RetargetStatesAnimations();
	}

	UIAssetIcon::UIAssetIcon(const UIAssetIcon& other):
		UIWidget(other)
	{
		mNameText = GetChildByType<UILabel>();

		onDraw += [&]() { CursorAreaEventsListener::OnDrawn(); };
		RetargetStatesAnimations();
		UpdateLayout();
	}

	UIAssetIcon::~UIAssetIcon()
	{}

	UIAssetIcon& UIAssetIcon::operator=(const UIAssetIcon& other)
	{
		UIWidget::operator=(other);
		mNameText = GetChildByType<UILabel>();

		return *this;
	}

	void UIAssetIcon::SetAssetInfo(const AssetInfo& info)
	{
		mAssetInfo = info;

		if (mNameText)
			mNameText->text = o2FileSystem.GetPathWithoutDirectories(info.path);
	}

	const AssetInfo& UIAssetIcon::GetAssetInfo() const
	{
		return mAssetInfo;
	}

	bool UIAssetIcon::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}

	void UIAssetIcon::SetSelected(bool selected)
	{
		SetState("selected", selected);
		if (mOwner)
			SetState("focused", mOwner->IsFocused());

		mIsSelected = selected;

		if (selected)
			OnSelected();
		else
			OnDeselected();
	}

	void UIAssetIcon::OnCursorDblClicked(const Input::Cursor& cursor)
	{
		if (mOwner)
			mOwner->OnIconDblClicked(this);
	}

	void UIAssetIcon::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		if (mOwner)
			mOwner->OnCursorRightMouseReleased(cursor);
	}

	void UIAssetIcon::OnCursorEnter(const Input::Cursor& cursor)
	{
		SetState("hover", true);
	}

	void UIAssetIcon::OnCursorExit(const Input::Cursor& cursor)
	{
		SetState("hover", false);
	}

	void UIAssetIcon::OnDragStart(const Input::Cursor& cursor)
	{
		if (mOwner)
			mOwner->BeginDragging(this);
	}

	void UIAssetIcon::OnDragged(const Input::Cursor& cursor, DragDropArea* area)
	{
		if (mOwner)
			mOwner->UpdateDraggingGraphics();
	}

	void UIAssetIcon::OnDragEnd(const Input::Cursor& cursor)
	{
		if (mOwner)
			mOwner->mDragEnded = true;
	}

	void UIAssetIcon::OnSelected()
	{
		if (mOwner)
			mOwner->Focus();
	}

	void UIAssetIcon::OnDeselected()
	{}

	void UIAssetIcon::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		if (mOwner)
			mOwner->OnDropped(group);
	}

}

CLASS_META(Editor::UIAssetIcon)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::SelectableDragableObject);
	BASE_CLASS(o2::DragDropArea);

	PROTECTED_FIELD(mNameText);
	PROTECTED_FIELD(mAssetInfo);
	PROTECTED_FIELD(mSelectedState);
	PROTECTED_FIELD(mOwner);

	PUBLIC_FUNCTION(void, SetAssetInfo, const AssetInfo&);
	PUBLIC_FUNCTION(const AssetInfo&, GetAssetInfo);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
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
