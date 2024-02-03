#include "o2Editor/stdafx.h"
#include "AssetIcon.h"

#include "o2/Render/Text.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2Editor/AssetsWindow/AssetsIconsScroll.h"

namespace Editor
{
	AssetIcon::AssetIcon():
		Widget(), mNameText(nullptr)
	{
		onDraw += [&]() { CursorAreaEventsListener::OnDrawn(); };
		RetargetStatesAnimations();
	}

	AssetIcon::AssetIcon(const AssetIcon& other):
		Widget(other), assetName(this)
	{
		mNameText = FindChildByType<Label>();

		onDraw += [&]() { CursorAreaEventsListener::OnDrawn(); };
		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	AssetIcon::~AssetIcon()
	{}

	AssetIcon& AssetIcon::operator=(const AssetIcon& other)
	{
		Widget::operator=(other);
		mNameText = FindChildByType<Label>();
		return *this;
	}

	void AssetIcon::SetAssetInfo(const Ref<AssetInfo>& info)
	{
		mAssetInfo = info;

		if (mNameText)
			mNameText->text = o2FileSystem.GetPathWithoutDirectories(info->path);
	}

	const Ref<AssetInfo>& AssetIcon::GetAssetInfo() const
	{
		return mAssetInfo;
	}

	void AssetIcon::SetAssetName(const WString& name)
	{
		if (mNameText)
			mNameText->SetText(name);
	}

	WString AssetIcon::GetAssetName() const
	{
		if (mNameText)
			return mNameText->GetText();

		return WString();
	}

	bool AssetIcon::IsUnderPoint(const Vec2F& point)
	{
		return Widget::IsUnderPoint(point);
	}

	bool AssetIcon::IsInputTransparent() const
	{
		return false;
	}

	void AssetIcon::OnDrawn()
	{
		Widget::OnDrawn();
		SelectableDragableObject::OnDrawn();
		DragDropArea::OnDrawn();
	}

	String AssetIcon::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void AssetIcon::SetSelected(bool selected)
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

	void AssetIcon::OnCursorDblClicked(const Input::Cursor& cursor)
	{
		if (mOwner)
			mOwner->OnAssetDblClick(this);
	}

	void AssetIcon::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		if (mOwner)
			mOwner->OnCursorRightMouseReleased(cursor);
	}

	void AssetIcon::OnCursorEnter(const Input::Cursor& cursor)
	{
		SetState("hover", true);
	}

	void AssetIcon::OnCursorExit(const Input::Cursor& cursor)
	{
		SetState("hover", false);
	}

	void AssetIcon::OnCursorPressed(const Input::Cursor& cursor)
	{
		SelectableDragableObject::OnCursorPressed(cursor);
	}

	void AssetIcon::OnCursorStillDown(const Input::Cursor& cursor)
	{
		SelectableDragableObject::OnCursorStillDown(cursor);
	}

	void AssetIcon::OnCursorReleased(const Input::Cursor& cursor)
	{
		SelectableDragableObject::OnCursorReleased(cursor);
	}

	void AssetIcon::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{
		SelectableDragableObject::OnCursorReleasedOutside(cursor);
	}

	void AssetIcon::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		SelectableDragableObject::OnCursorPressBreak(cursor);
	}

	void AssetIcon::OnDragStart(const Input::Cursor& cursor)
	{
		if (mOwner)
			mOwner->BeginDragging(this);
	}

	void AssetIcon::OnDragged(const Input::Cursor& cursor, const Ref<DragDropArea>& area)
	{
		if (mOwner)
			mOwner->UpdateDraggingGraphics();
	}

	void AssetIcon::OnDragEnd(const Input::Cursor& cursor)
	{
		if (mOwner)
			mOwner->mDragEnded = true;
	}

	void AssetIcon::OnSelected()
	{
		if (mOwner)
			mOwner->Focus();
	}

	void AssetIcon::OnDeselected()
	{}

	void AssetIcon::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		if (mOwner)
			mOwner->OnDropped(group);
	}

}
// --- META ---

DECLARE_CLASS(Editor::AssetIcon, Editor__AssetIcon);
// --- END META ---