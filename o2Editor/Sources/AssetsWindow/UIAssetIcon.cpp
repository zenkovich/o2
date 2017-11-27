#include "stdafx.h"
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
		SetLayoutDirty();
	}

	UIAssetIcon::~UIAssetIcon()
	{}

	UIAssetIcon& UIAssetIcon::operator=(const UIAssetIcon& other)
	{
		CopyData(other);
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

	void UIAssetIcon::CopyData(const Actor& otherActor)
	{
		const UIAssetIcon& other = dynamic_cast<const UIAssetIcon&>(otherActor);

		UIWidget::CopyData(other);
		mNameText = GetChildByType<UILabel>();
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

DECLARE_CLASS(Editor::UIAssetIcon);
