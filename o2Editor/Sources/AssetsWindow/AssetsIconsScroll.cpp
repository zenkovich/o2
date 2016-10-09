#include "AssetsIconsScroll.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/Animation.h"
#include "Application/Application.h"
#include "Assets/ActorAsset.h"
#include "Assets/AnimationAsset.h"
#include "Assets/Assets.h"
#include "Assets/DataAsset.h"
#include "Assets/FolderAsset.h"
#include "Assets/ImageAsset.h"
#include "AssetsWindow/AssetsWindow.h"
#include "Core/Actions/CreateActors.h"
#include "Core/EditorApplication.h"
#include "Events/CursorEventsListener.h"
#include "Events/EventSystem.h"
#include "PropertiesWindow/Properties/ActorProperty.h"
#include "PropertiesWindow/Properties/AssetProperty.h"
#include "PropertiesWindow/Properties/ComponentProperty.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Components/ImageComponent.h"
#include "SceneWindow/SceneEditScreen.h"
#include "SceneWindow/SceneEditWidget.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/GridLayout.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "UIAssetIcon.h"
#include "Utils/Clipboard.h"
#include "Utils/FileSystem/FileSystem.h"

namespace Editor
{

	UIAssetsIconsScrollArea::UIAssetsIconsScrollArea():
		UIScrollArea(), DrawableCursorEventsListener(this), mSelection(nullptr), mPressTime(10), mIconHoverSprite(nullptr),
		mContextMenu(nullptr), mDragComponentPropertyField(nullptr)
	{
		mGrid = mnew UIGridLayout();
		mGrid->layout = UIWidgetLayout::BothStretch();
		mGrid->baseCorner = BaseCorner::LeftTop;
		mGrid->fitByChildren = true;
		mGrid->cellSize = mAssetIconSize;
		mGrid->border = RectF(5, 5, 5, 5);
		mGrid->arrangeAxis = TwoDirection::Horizontal;
		mGrid->arrangeAxisMaxCells = 5;
		onLayoutChanged += Function<void()>(this, &UIAssetsIconsScrollArea::UpdateAssetsGridSize);
		AddChild(mGrid);

		mSelection = mnew Sprite("ui/UI_Window_place.png");
		InitializeSelectionSprite();

		mDragIcon = mnew UIAssetIcon();
	}

	UIAssetsIconsScrollArea::UIAssetsIconsScrollArea(const UIAssetsIconsScrollArea& other):
		UIScrollArea(other), mSelection(other.mSelection->Clone()), DrawableCursorEventsListener(this), mPressTime(10),
		mIconHoverSprite(nullptr), mContextMenu(nullptr), mDragComponentPropertyField(nullptr)
	{
		mGrid = FindChild<UIGridLayout>();
		onLayoutChanged += Function<void()>(this, &UIAssetsIconsScrollArea::UpdateAssetsGridSize);
		InitializeSelectionSprite();

		mDragIcon = o2UI.CreateWidget<UIAssetIcon>();

		RetargetStatesAnimations();
		UpdateLayout();
		InitializeContext();
	}

	UIAssetsIconsScrollArea::~UIAssetsIconsScrollArea()
	{
		delete mSelection;
		delete mDragIcon;

		for (auto kv : mIconsPool)
			for (auto icon : kv.Value())
				delete icon;
	}

	UIAssetsIconsScrollArea& UIAssetsIconsScrollArea::operator=(const UIAssetsIconsScrollArea& other)
	{
		delete mSelection;

		UIScrollArea::operator=(other);

		mGrid = FindChild<UIGridLayout>();
		mSelection = other.mSelection->Clone();

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIAssetsIconsScrollArea::Draw()
	{
		if (mFullyDisabled)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		mIconHoverSprite->Draw();

		for (auto& sel : mSelectedAssetsIcons)
			sel.selectionSprite->Draw();

		for (auto& sel : mCurrentSelectingIcons)
			sel.selectionSprite->Draw();

		for (auto child : mChilds)
			child->Draw();

		if (mSelecting)
			mSelection->Draw();

		o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();

		if (mDragState == DragState::Regular || mDragState == DragState::AssetField || mDragState == DragState::ActorField ||
			mDragState == DragState::ComponentField)
			o2UI.DrawWidgetAtTop(mDragIcon);

		if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
			DrawDebugFrame();
	}

	void UIAssetsIconsScrollArea::Update(float dt)
	{
		UIScrollArea::Update(dt);

		mPressTime += dt;

		UpdateHover();

		float hoverRectTransitionCoef = 20.0f;
		if (mCurrentHoverSpriteRect != mTargetHoverSpriteRect)
		{
			mCurrentHoverSpriteRect = Math::Lerp(mCurrentHoverSpriteRect, mTargetHoverSpriteRect, dt*hoverRectTransitionCoef);
			mIconHoverSprite->SetRect(mCurrentHoverSpriteRect);
		}

		if (mNeedRebuildAssets)
		{
			mNeedRebuildAssets = false;
			o2Assets.RebuildAssets();
		}
	}

	void UIAssetsIconsScrollArea::SetViewingPath(const String& path)
	{
		mCurrentPath = path;
		UpdateAssetsPath();
	}

	String UIAssetsIconsScrollArea::GetViewingPath() const
	{
		return mCurrentPath;
	}

	void UIAssetsIconsScrollArea::UpdateAssetsPath()
	{
		o2Debug.LogWarning("Update assets folder: %s", mCurrentPath);

		DeselectAllAssets();

		auto prevIcons = mGrid->GetChilds().Select<UIAssetIcon*>([](auto x) { return (UIAssetIcon*)x; });
		prevIcons.ForEach([&](auto x) { FreeAssetIconToPool(x); });
		mGrid->RemoveAllChilds(false);

		AssetInfosVec folderAssetsInfos;
		if (mCurrentPath != "")
			folderAssetsInfos = FolderAsset(mCurrentPath).GetContainingAssetsInfos();
		else
			folderAssetsInfos = o2Assets.GetAssetsTree().mRootAssets.Select<AssetInfo>([](auto x) { return (AssetInfo)*x; });

		folderAssetsInfos.Reverse();

		Vector<UIWidget*> addingIcons;
		for (auto asset : folderAssetsInfos)
		{
			UIAssetIcon* assetIcon;

			if (asset.mType == TypeOf(FolderAsset).ID())
			{
				assetIcon = GetAssetIconFromPool("folder");
				assetIcon->name = "folder";
			}
			else if (asset.mType == TypeOf(ImageAsset).ID())
			{
				assetIcon = GetImageAssetIcon(asset);
				assetIcon->name = "image preview";
			}
			else if (asset.mType == TypeOf(DataAsset).ID())
			{
				assetIcon = GetAssetIconFromPool("text");
				assetIcon->name = "text";
			}
			else if (asset.mType == TypeOf(AnimationAsset).ID())
			{
				assetIcon = GetAssetIconFromPool("animation");
				assetIcon->name = "animation";
			}
			else if (asset.mType == TypeOf(ActorAsset).ID())
			{
				assetIcon = GetAssetIconFromPool("prefab");
				assetIcon->name = "prefab";
			}
			else
			{
				assetIcon = GetAssetIconFromPool("standard");
				assetIcon->name = "standard";
			}

			assetIcon->SetAssetInfo(asset);
			assetIcon->SetState("halfHide", mCuttingAssets.ContainsPred([&](auto x) { return x.mFirst == asset.mId; }));

			addingIcons.Add(assetIcon);
		}

		mGrid->AddChilds(addingIcons);

		UpdateCuttingAssets();
	}

	bool UIAssetsIconsScrollArea::IsFocusable() const
	{
		return true;
	}

	void UIAssetsIconsScrollArea::SelectAsset(AssetId id, bool scroll /*= true*/)
	{
		UIAssetIcon* icon = (UIAssetIcon*)(mGrid->GetChilds().FindMatch([=](UIWidget* x) {
			return ((UIAssetIcon*)x)->GetAssetInfo().mId == id; }));

		if (!icon)
			return;

		if (!mSelectedAssetsIcons.ContainsPred([=](auto x) { return x.icon == icon; }))
		{
			icon->SetState("assetSelection", true);
			auto selSprite = GetSelectionSprite();
			selSprite->SetRect(mSelectionSpriteLayout.Calculate(icon->layout.GetAbsoluteRect()));
			mSelectedAssetsIcons.Add(IconSelection(icon, selSprite));
		}

		if (scroll)
		{
			SetScroll(Vec2F(0.0f, -icon->layout.absTop + layout.absTop + GetScroll().y));
		}
	}

	void UIAssetsIconsScrollArea::DeselectAllAssets()
	{
		for (auto icon : mSelectedAssetsIcons)
			FreeSelectionSprite(icon.selectionSprite);

		mSelectedAssetsIcons.Clear();
	}

	Vector<AssetInfo> UIAssetsIconsScrollArea::GetSelectedAssets() const
	{
		return mSelectedAssetsIcons.Select<AssetInfo>([](const IconSelection& x) { return x.icon->GetAssetInfo(); });
	}

	void UIAssetsIconsScrollArea::UpdateLayout(bool forcible /*= false*/)
	{
		Vec2F localPressPoint = mPressedPoint - mChildsAbsRect.LeftBottom();

		UIScrollArea::UpdateLayout(forcible);

		mPressedPoint = localPressPoint + mChildsAbsRect.LeftBottom();

		if (mSelecting)
			UpdateSelection(*o2Input.GetCursor(0));

		for (auto& sel : mSelectedAssetsIcons)
			sel.selectionSprite->SetRect(mSelectionSpriteLayout.Calculate(sel.icon->layout.GetAbsoluteRect()));

		for (auto& sel : mCurrentSelectingIcons)
			sel.selectionSprite->SetRect(mSelectionSpriteLayout.Calculate(sel.icon->layout.GetAbsoluteRect()));

		UpdateHover();
	}

	void UIAssetsIconsScrollArea::UpdateCuttingAssets()
	{
		for (auto child : mGrid->GetChilds())
		{
			UIAssetIcon* icon = (UIAssetIcon*)child;
			icon->SetState("halfHide", o2EditorAssets.mCuttingAssets.ContainsPred([=](auto x) {
				return x.mFirst == icon->GetAssetInfo().mId; }));
		}
	}

	void UIAssetsIconsScrollArea::OnFocused()
	{
		for (auto& sel : mSelectedAssetsIcons)
			sel.selectionSprite->SetColor(mSelectedColor);

		UIWidget::OnFocused();
	}

	void UIAssetsIconsScrollArea::OnUnfocused()
	{
		for (auto& sel : mSelectedAssetsIcons)
			sel.selectionSprite->SetColor(mUnselectedColor);

		UIWidget::OnFocused();
	}

	void UIAssetsIconsScrollArea::OnCursorPressed(const Input::Cursor& cursor)
	{
		mPressedPoint = cursor.position;
		o2UI.FocusWidget(this);
	}

	void UIAssetsIconsScrollArea::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (cursor.delta == Vec2F())
			return;

		float beginDragOrSelectThreshold = 5.0f;

		if (!mSelecting && mDragState == DragState::Off && (cursor.position - mPressedPoint).Length() > beginDragOrSelectThreshold)
		{
			UIAssetIcon* iconUnderCursor = GetIconUnderPoint(mPressedPoint);
			if (iconUnderCursor && mSelectedAssetsIcons.ContainsPred([=](auto x) { return x.icon == iconUnderCursor; }))
				BeginDragging(iconUnderCursor, cursor);
			else
				BeginSelecting();
		}

		if (mSelecting)
			UpdateSelection(cursor);

		if (mDragState != DragState::Off)
			UpdateDragging(cursor);
	}

	void UIAssetsIconsScrollArea::BeginSelecting()
	{
		mSelecting = true;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAllAssets();
	}

	void UIAssetsIconsScrollArea::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mPressTime < 0.3f && !mSelecting)
		{
			UIAssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.position);
			if (iconUnderCursor)
				OnIconDblClicked(iconUnderCursor);
		}
		mPressTime = 0.0f;

		if (mSelecting) CompleteSelecting();
		else if (mDragState != DragState::Off) CompleteDragging();
		else
		{
			if (!o2Input.IsKeyDown(VK_CONTROL))
				DeselectAllAssets();

			UIAssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.position);
			if (iconUnderCursor && !mSelectedAssetsIcons.ContainsPred([=](auto x) { return x.icon == iconUnderCursor; }))
			{
				iconUnderCursor->SetState("assetSelection", true);
				auto selSprite = GetSelectionSprite();
				selSprite->SetRect(mSelectionSpriteLayout.Calculate(iconUnderCursor->layout.GetAbsoluteRect()));
				mSelectedAssetsIcons.Add(IconSelection(iconUnderCursor, selSprite));
			}
		}
	}


	void UIAssetsIconsScrollArea::BeginDragging(UIAssetIcon* iconUnderCursor, const Input::Cursor &cursor)
	{
		mDragState = DragState::Regular;
		for (auto sel : mSelectedAssetsIcons)
			sel.icon->Hide();

		*mDragIcon = *mSelectedAssetsIcons.Last().icon;
		mDragOffset = iconUnderCursor->layout.absRect->Center() - cursor.position;

		if (mSelectedAssetsIcons.Count() > 1)
			((Text*)mDragIcon->layer["assetName"]->drawable)->text = (String)mSelectedAssetsIcons.Count() + " items";
	}

	void UIAssetsIconsScrollArea::UpdateDragging(const Input::Cursor& cursor)
	{
		mDragIcon->SetParent(nullptr);
		mDragIcon->layout.SetRect(RectF(cursor.position - mAssetIconSize*0.5f + mDragOffset,
										cursor.position + mAssetIconSize*0.5f + mDragOffset));

		CursorAreaEventsListener* listenerUnderCursor = o2Events.GetCursorListenerUnderCursor(0);

		SceneEditScreen* sceneEdit = dynamic_cast<SceneEditScreen*>(listenerUnderCursor);
		UIActorsTree* actorsTree = dynamic_cast<UIActorsTree*>(listenerUnderCursor);
		IAssetProperty* assetProperty = dynamic_cast<IAssetProperty*>(listenerUnderCursor);
		ActorProperty* actorProperty = dynamic_cast<ActorProperty*>(listenerUnderCursor);
		ComponentProperty* componentProperty = dynamic_cast<ComponentProperty*>(listenerUnderCursor);

		if (!sceneEdit && mDragState == DragState::Scene)
		{
			ClearInstantiatedDraggingAssets();
			mDragState = DragState::Regular;
		}

		if (!actorsTree && mDragState == DragState::Tree)
		{
			o2EditorTree.GetActorsTree()->BreakDragging();
			ClearInstantiatedDraggingAssets();
			mDragState = DragState::Regular;
		}

		if (!assetProperty && mDragState == DragState::AssetField)
		{
			mDragState = DragState::Regular;
			mDragAssetPropertyField->GetWidget()->SetState("selected", false);
			o2Application.SetCursor(CursorType::Arrow);
		}

		if (!actorProperty && mDragState == DragState::ActorField)
		{
			mDragState = DragState::Regular;
			mDragActorPropertyField->GetWidget()->SetState("selected", false);
			o2Application.SetCursor(CursorType::Arrow);
		}

		if (!componentProperty && mDragState == DragState::ComponentField)
		{
			mDragState = DragState::Regular;
			mDragComponentPropertyField->GetWidget()->SetState("selected", false);
			o2Application.SetCursor(CursorType::Arrow);
		}

		if (componentProperty && mDragState != DragState::ComponentField)
		{
			if (mSelectedAssetsIcons.Count() == 1 && mSelectedAssetsIcons[0].icon->GetAssetInfo().mType ==
				TypeOf(ActorAsset).ID())
			{
				Actor* assetActor = o2Scene.GetAssetActorByID(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId);

				if (assetActor->GetComponent(componentProperty->GetSpecializedType()))
				{
					mDragState = DragState::ComponentField;
					componentProperty->GetWidget()->SetState("selected", true);
					mDragComponentPropertyField = componentProperty;
					o2Application.SetCursor(CursorType::Hand);
				}
			}
		}

		if (actorProperty && mDragState != DragState::ActorField)
		{
			if (mSelectedAssetsIcons.Count() == 1 && mSelectedAssetsIcons[0].icon->GetAssetInfo().mType ==
				TypeOf(ActorAsset).ID())
			{
				mDragState = DragState::ActorField;
				actorProperty->GetWidget()->SetState("selected", true);
				mDragActorPropertyField = actorProperty;
				o2Application.SetCursor(CursorType::Hand);
			}
		}

		if (assetProperty && mDragState != DragState::AssetField)
		{
			if (mSelectedAssetsIcons.Count() == 1 && assetProperty->GetFieldType()->ID() ==
				mSelectedAssetsIcons[0].icon->GetAssetInfo().mType)
			{
				mDragState = DragState::AssetField;
				assetProperty->GetWidget()->SetState("selected", true);
				mDragAssetPropertyField = assetProperty;
				o2Application.SetCursor(CursorType::Hand);
			}
		}

		if (sceneEdit && mDragState != DragState::Scene)
		{
			InstantiateDraggingAssets();

			if (!mInstSceneDragActors.IsEmpty())
				mDragState = DragState::Scene;
		}

		if (actorsTree && mDragState != DragState::Tree)
		{
			InstantiateDraggingAssets();

			if (!mInstSceneDragActors.IsEmpty())
			{
				o2EditorTree.GetActorsTree()->ManualBeginDraggingActors(mInstSceneDragActors);
				mDragState = DragState::Tree;
			}
		}

		if (mDragState == DragState::Scene)
		{
			Vec2F sceneCursorPos = o2EditorSceneScreen.ScreenToScenePoint(cursor.position);
			for (auto actor : mInstSceneDragActors)
				actor->transform.worldPosition = sceneCursorPos;
		}

		if (mDragState == DragState::Tree)
			o2EditorTree.GetActorsTree()->ManualUpdateDraggingActors(cursor);
	}

	void UIAssetsIconsScrollArea::CompleteDragging()
	{
		for (auto sel : mSelectedAssetsIcons)
			sel.icon->Show();

		CursorAreaEventsListener* listenerUnderCursor = o2Events.GetCursorListenerUnderCursor(0);

		if (mDragState == DragState::Tree)
		{
			o2EditorTree.GetActorsTree()->CompleteManualDraggingActors();
			RegActorsCreationAction();
			mInstSceneDragActors.Clear();

			o2UI.FocusWidget(o2EditorTree.GetActorsTree());
		}

		if (mDragState == DragState::AssetField)
		{
			mDragAssetPropertyField->SetAssetId(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId);
			mDragAssetPropertyField->GetWidget()->SetState("selected", false);
			o2Application.SetCursor(CursorType::Arrow);
		}

		if (mDragState == DragState::ActorField)
		{
			mDragActorPropertyField->SetValue(o2Scene.GetAssetActorByID(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId));
			mDragActorPropertyField->GetWidget()->SetState("selected", false);
			o2Application.SetCursor(CursorType::Arrow);
		}

		if (mDragState == DragState::ComponentField)
		{
			Actor* actorAsset = o2Scene.GetAssetActorByID(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId);
			mDragComponentPropertyField->SetValue(actorAsset->GetComponent(mDragComponentPropertyField->GetSpecializedType()));
			mDragComponentPropertyField->GetWidget()->SetState("selected", false);
			o2Application.SetCursor(CursorType::Arrow);
		}

		if (mDragState == DragState::Regular)
		{
			UITree* treeWindow = dynamic_cast<UITree*>(listenerUnderCursor);
			if (treeWindow)
			{
				UITreeNode* nodeUnderCursor = treeWindow->GetTreeNodeUnderPoint(o2Input.GetCursorPos());
				if (nodeUnderCursor)
				{
					AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)nodeUnderCursor->GetObject();

					String destPath = assetTreeNode->mPath;
					auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x.icon->GetAssetInfo(); });
					o2Assets.MoveAssets(assetsInfos, destPath, true);

					DeselectAllAssets();
				}
			}

			UIAssetsIconsScrollArea* scrollArea = dynamic_cast<UIAssetsIconsScrollArea*>(listenerUnderCursor);
			if (scrollArea)
			{
				UIAssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
				if (iconUnderCursor && iconUnderCursor->GetAssetInfo().mType == TypeOf(FolderAsset).ID())
				{
					String destPath = iconUnderCursor->GetAssetInfo().mPath;
					auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x.icon->GetAssetInfo(); });
					o2Assets.MoveAssets(assetsInfos, destPath, true);

					DeselectAllAssets();
				}
			}
		}

		if (mDragState == DragState::Scene)
		{
			RegActorsCreationAction();

			o2UI.FocusWidget(o2EditorTree.GetActorsTree());
			o2EditorTree.GetActorsTree()->SetSelectedActors(mInstSceneDragActors);

			mInstSceneDragActors.Clear();
		}

		mDragState = DragState::Off;
	}

	void UIAssetsIconsScrollArea::RegActorsCreationAction()
	{
		auto firstInstActor = mInstSceneDragActors[0];
		auto parent = firstInstActor->GetParent();
		auto parentChilds = parent ? parent->GetChilds() : o2Scene.GetRootActors();
		int idx = parentChilds.Find(firstInstActor);
		auto prevActor = idx > 0 ? parentChilds[idx - 1] : nullptr;

		auto createAction = mnew CreateActorsAction(mInstSceneDragActors, parent, prevActor);

		o2EditorApplication.DoneAction(createAction);
	}

	void UIAssetsIconsScrollArea::InstantiateDraggingAssets()
	{
		if (mDragState != DragState::Regular)
			return;

		for (auto& sel : mSelectedAssetsIcons)
		{
			auto info = sel.icon->GetAssetInfo();
			Actor* actor = InstantiateAsset(info);
			if (actor)
			{
				actor->name = o2FileSystem.GetPathWithoutDirectories(o2FileSystem.GetFileNameWithoutExtension(info.mPath));
				mInstSceneDragActors.Add(actor);
			}
		}
	}

	void UIAssetsIconsScrollArea::ClearInstantiatedDraggingAssets()
	{
		for (auto actor : mInstSceneDragActors)
			delete actor;

		mInstSceneDragActors.Clear();
	}

	void UIAssetsIconsScrollArea::OnCursorPressBreak(const Input::Cursor& cursor)
	{}

	void UIAssetsIconsScrollArea::UpdateSelection(const Input::Cursor& cursor)
	{
		RectF selectionRect(cursor.position, mPressedPoint);
		mSelection->SetRect(selectionRect);

		for (auto icon : mCurrentSelectingIcons)
			FreeSelectionSprite(icon.selectionSprite);

		mCurrentSelectingIcons.Clear();

		for (auto child : mGrid->GetChilds())
		{
			if (child->layout.GetAbsoluteRect().IsIntersects(selectionRect))
			{
				UIAssetIcon* icon = (UIAssetIcon*)child;
				icon->SetState("assetSelection", true);
				auto selSprite = GetSelectionSprite();
				selSprite->SetRect(mSelectionSpriteLayout.Calculate(icon->layout.GetAbsoluteRect()));
				mCurrentSelectingIcons.Add(IconSelection(icon, selSprite));
			}
		}
	}

	void UIAssetsIconsScrollArea::CompleteSelecting()
	{
		mSelectedAssetsIcons.Add(mCurrentSelectingIcons);
		mCurrentSelectingIcons.Clear();
		mSelecting = false;
	}

	void UIAssetsIconsScrollArea::OnCursorMoved(const Input::Cursor& cursor)
	{}

	void UIAssetsIconsScrollArea::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		UIAssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.position);
		if (iconUnderCursor)
		{
			if (!mSelectedAssetsIcons.ContainsPred([=](const IconSelection& x) { return x.icon == iconUnderCursor; }))
			{
				DeselectAllAssets();
				iconUnderCursor->SetState("assetSelection", true);
				auto selSprite = GetSelectionSprite();
				selSprite->SetRect(mSelectionSpriteLayout.Calculate(iconUnderCursor->layout.GetAbsoluteRect()));
				mCurrentSelectingIcons.Add(IconSelection(iconUnderCursor, selSprite));
			}
		}

		o2UI.FocusWidget(this);
		mContextMenu->Show();
	}

	void UIAssetsIconsScrollArea::OnKeyReleased(const Input::Key& key)
	{
		if (mIsFocused && key == VK_BACK)
		{
			if (mCurrentPath.CountOf("/") > 0)
				o2EditorAssets.OpenFolder(o2FileSystem.GetParentPath(mCurrentPath));
			else
				o2EditorAssets.OpenFolder("");
		}
	}

	void UIAssetsIconsScrollArea::InitializeContext()
	{
		mContextMenu = o2UI.CreateWidget<UIContextMenu>();

		mContextMenu->AddItem("Import", [&]() { OnContextImportPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Open", [&]() { OnContextOpenPressed(); });
		mContextMenu->AddItem("Show in folder", [&]() { OnContextShowInExplorerPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Create/Folder", [&]() { OnContextCreateFolderPressed(); });
		mContextMenu->AddItem("Create/Prefab", [&]() { OnContextCreatePrefabPressed(); });
		mContextMenu->AddItem("Create/Script", [&]() { OnContextCreateScriptPressed(); });
		mContextMenu->AddItem("Create/Animation", [&]() { OnContextCreateAnimationPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Copy", [&]() { OnContextCopyPressed(); }, nullptr, ShortcutKeys('C', true));
		mContextMenu->AddItem("Cut", [&]() { OnContextCutPressed(); }, nullptr, ShortcutKeys('X', true));
		mContextMenu->AddItem("Paste", [&]() { OnContextPastePressed(); }, nullptr, ShortcutKeys('V', true));
		mContextMenu->AddItem("Delete", [&]() { OnContextDeletePressed(); }, nullptr, ShortcutKeys(VK_DELETE));

		AddChild(mContextMenu);
	}

	void UIAssetsIconsScrollArea::PrepareIconsPools()
	{
		int poolSize = 10;
		String iconsStyles[] = { "standard", "folder", "prefab", "prefab preview", "image preview", "text", "animation" };

		for (auto style : iconsStyles)
		{
			mIconsPool.Add(style, Vector<UIAssetIcon*>());

			for (int i = 0; i < poolSize; i++)
			{
				UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>(style);
				mIconsPool[style].Add(sample);
			}
		}
	}

	UIAssetIcon* UIAssetsIconsScrollArea::GetAssetIconFromPool(const String& style)
	{
		if (!mIconsPool.ContainsKey(style))
			mIconsPool.Add(style, Vector<UIAssetIcon*>());

		int poolResizeStep = 10;

		if (mIconsPool[style].Count() == 0)
		{
			for (int i = 0; i < poolResizeStep; i++)
			{
				UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>(style);
				mIconsPool[style].Add(sample);
			}
		}

		UIAssetIcon* sample = mIconsPool[style].PopBack();
		return sample;
	}

	void UIAssetsIconsScrollArea::FreeAssetIconToPool(UIAssetIcon* icon)
	{
		if (mIconsPool.ContainsKey(icon->name))
			mIconsPool[icon->name].Add(icon);
		else
			delete icon;
	}

	UIAssetIcon* UIAssetsIconsScrollArea::GetImageAssetIcon(const AssetInfo& asset)
	{
		UIAssetIcon* assetIcon = GetAssetIconFromPool("image preview");

		auto previewLayer = assetIcon->layer["preview"];
		Sprite* previewSprite = (Sprite*)previewLayer->drawable;

		ImageAsset previewSpriteAsset(asset.mPath);
		float previewMaxSize = 30;

		if (previewSpriteAsset.width > previewSpriteAsset.height)
		{
			float cf = previewSpriteAsset.height / previewSpriteAsset.width;
			previewLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize, previewMaxSize*cf),
												 Vec2F(0, 10));
		}
		else
		{
			float cf = previewSpriteAsset.width / previewSpriteAsset.height;
			previewLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize*cf, previewMaxSize),
												 Vec2F(0, 10));
		}

		previewSprite->imageAsset = previewSpriteAsset;
		previewSprite->mode = SpriteMode::Default;

		return assetIcon;
	}

	void UIAssetsIconsScrollArea::UpdateAssetsGridSize()
	{
		mGrid->arrangeAxisMaxCells = Math::Max(1, Math::FloorToInt(layout.GetWidth() / mAssetIconSize.x));
	}

	void UIAssetsIconsScrollArea::InitializeSelectionSprite()
	{
		mIconSelectionSprite = mnew Sprite("ui/UI_Context_menu_white.png");
		mSelectionSpriteLayout = Layout::BothStretch(-10, -7, -10, -15);

		mIconHoverSprite = mnew Sprite("ui/UI_ListBox_selection_hover.png");
		mIconHoverSprite->SetColor(mHoverColor);
		AddState("hover", Animation::EaseInOut<float>(this, &mIconHoverSprite->transparency, 0.0f, 1.0f, 0.1f));

		int initialPoolSize = 40;
		for (int i = 0; i < initialPoolSize; i++)
			mSelectionSpritesPool.Add(mIconSelectionSprite->Clone());
	}

	Sprite* UIAssetsIconsScrollArea::GetSelectionSprite()
	{
		if (mSelectionSpritesPool.Count() == 0)
		{
			int poolResizeCount = 20;
			for (int i = 0; i < poolResizeCount; i++)
				mSelectionSpritesPool.Add(mIconSelectionSprite->Clone());
		}

		Sprite* res = mSelectionSpritesPool.PopBack();
		res->SetColor(mIsFocused ? mSelectedColor : mUnselectedColor);
		return res;
	}

	void UIAssetsIconsScrollArea::FreeSelectionSprite(Sprite* sprite)
	{
		mSelectionSpritesPool.Add(sprite);
	}

	void UIAssetsIconsScrollArea::OnIconDblClicked(UIAssetIcon* icon)
	{
		AssetInfo iconAssetInfo = icon->GetAssetInfo();
		auto assetNameLayer = icon->layer["assetName"];
		if (assetNameLayer && assetNameLayer->drawable->IsUnderPoint(o2Input.cursorPos))
		{
			icon->SetState("edit", true);

			auto editBox = (UIEditBox*)icon->GetChild("nameEditBox");

			editBox->text = o2FileSystem.GetFileNameWithoutExtension(
				o2FileSystem.GetPathWithoutDirectories(iconAssetInfo.mPath));

			editBox->SelectAll();
			editBox->UIWidget::Focus();
			editBox->ResetSroll();

			editBox->onChangeCompleted = [=](const WString& text) {
				icon->SetState("edit", false);
				String ext = o2FileSystem.GetFileExtension(iconAssetInfo.mPath);
				if (ext.IsEmpty())
					o2Assets.RenameAsset(iconAssetInfo, text, false);
				else
					o2Assets.RenameAsset(iconAssetInfo, text + "." + o2FileSystem.GetFileExtension(iconAssetInfo.mPath), false);
				mNeedRebuildAssets = true;
			};

			return;
		}

		if (iconAssetInfo.mType == TypeOf(FolderAsset).ID())
			o2EditorAssets.OpenFolder(iconAssetInfo.mPath);
		else
			o2EditorAssets.OpenAndEditAsset(iconAssetInfo.mId);
	}

	UIAssetIcon* UIAssetsIconsScrollArea::GetIconUnderPoint(const Vec2F& point) const
	{
		for (auto child : mGrid->GetChilds())
		{
			if (child->layout.IsUnderPoint(point))
			{
				UIAssetIcon* icon = (UIAssetIcon*)child;
				return icon;
			}
		}

		return nullptr;
	}

	void UIAssetsIconsScrollArea::UpdateHover()
	{
		if (!mIconHoverSprite)
			return;

		if (!mSelecting)
		{
			UIAssetIcon* hoverIcon = GetIconUnderPoint(o2Input.GetCursorPos());

			if (hoverIcon)
			{
				SetState("hover", true);
				mTargetHoverSpriteRect = mSelectionSpriteLayout.Calculate(hoverIcon->layout.GetAbsoluteRect());
			}
			else SetState("hover", false);

			mHoverIcon = hoverIcon;
		}
		else SetState("hover", false);
	}

#undef CopyFile

	void UIAssetsIconsScrollArea::OnContextCopyPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CopyAssets(
			mSelectedAssetsIcons.Select<String>([](const IconSelection& x) { return x.icon->GetAssetInfo().mPath; }));
	}

	void UIAssetsIconsScrollArea::OnContextCutPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CutAssets(
			mSelectedAssetsIcons.Select<String>([](const IconSelection& x) { return x.icon->GetAssetInfo().mPath; }));
	}

	void UIAssetsIconsScrollArea::OnContextPastePressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.PasteAssets(mCurrentPath);
	}


	void UIAssetsIconsScrollArea::OnContextDeletePressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.DeleteAssets(
			mSelectedAssetsIcons.Select<String>([](const IconSelection& x) { return x.icon->GetAssetInfo().mPath; }));
	}

	void UIAssetsIconsScrollArea::OnContextOpenPressed()
	{
		if (!IsFocused())
			return;

		if (mSelectedAssetsIcons.Count() > 0)
			o2EditorAssets.OpenAndEditAsset(mSelectedAssetsIcons.Last().icon->GetAssetInfo().mId);
	}

	void UIAssetsIconsScrollArea::OnContextShowInExplorerPressed()
	{
		if (!IsFocused())
			return;

		if (mSelectedAssetsIcons.Count() > 0)
			o2EditorAssets.OpenAsset(mSelectedAssetsIcons.Last().icon->GetAssetInfo().mId);
	}

	void UIAssetsIconsScrollArea::OnContextImportPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.ImportAssets(mCurrentPath);
	}

	void UIAssetsIconsScrollArea::OnContextCreateFolderPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CreateFolderAsset(mCurrentPath);
	}

	void UIAssetsIconsScrollArea::OnContextCreatePrefabPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CreatePrefabAsset(mCurrentPath);
	}

	void UIAssetsIconsScrollArea::OnContextCreateScriptPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CreateScriptAsset(mCurrentPath);
	}

	void UIAssetsIconsScrollArea::OnContextCreateAnimationPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CreateAnimationAsset(mCurrentPath);
	}

	Actor* UIAssetsIconsScrollArea::InstantiateAsset(const ImageAsset& asset)
	{
		return mnew Actor({ mnew ImageComponent(asset) });
	}

	Actor* UIAssetsIconsScrollArea::InstantiateAsset(const ActorAsset& asset)
	{
		return mnew Actor(asset.actor);
	}

	Actor* UIAssetsIconsScrollArea::InstantiateAsset(const AssetInfo& assetInfo)
	{
		if (assetInfo.mType == TypeOf(ImageAsset).ID())
			return InstantiateAsset(ImageAsset(assetInfo.mId));
		else if (assetInfo.mType == TypeOf(ActorAsset).ID())
			return InstantiateAsset(ActorAsset(assetInfo.mId));

		return nullptr;
	}

	UIAssetsIconsScrollArea::IconSelection::IconSelection():
		icon(nullptr), selectionSprite(nullptr)
	{}

	UIAssetsIconsScrollArea::IconSelection::IconSelection(UIAssetIcon* icon, Sprite* selectionSprite) :
		icon(icon), selectionSprite(selectionSprite)
	{}

	bool UIAssetsIconsScrollArea::IconSelection::operator==(const IconSelection& other) const
	{
		return icon == other.icon;
	}
}
 
CLASS_META(Editor::UIAssetsIconsScrollArea)
{
	BASE_CLASS(o2::UIScrollArea);
	BASE_CLASS(o2::DrawableCursorEventsListener);
	BASE_CLASS(o2::KeyboardEventsListener);

	PROTECTED_FIELD(mAssetIconSize);
	PROTECTED_FIELD(mSelectedColor);
	PROTECTED_FIELD(mUnselectedColor);
	PROTECTED_FIELD(mHoverColor);
	PROTECTED_FIELD(mCurrentPath);
	PROTECTED_FIELD(mGrid);
	PROTECTED_FIELD(mSelection);
	PROTECTED_FIELD(mContextMenu);
	PROTECTED_FIELD(mSelectedAssetsIcons);
	PROTECTED_FIELD(mIconSelectionSprite);
	PROTECTED_FIELD(mSelectionSpriteLayout);
	PROTECTED_FIELD(mSelectionSpritesPool);
	PROTECTED_FIELD(mHoverIcon);
	PROTECTED_FIELD(mIconHoverSprite);
	PROTECTED_FIELD(mTargetHoverSpriteRect);
	PROTECTED_FIELD(mCurrentHoverSpriteRect);
	PROTECTED_FIELD(mIconsPool);
	PROTECTED_FIELD(mSelecting);
	PROTECTED_FIELD(mPressedPoint);
	PROTECTED_FIELD(mPressTime);
	PROTECTED_FIELD(mCurrentSelectingIcons);
	PROTECTED_FIELD(mDragState);
	PROTECTED_FIELD(mDragIcon);
	PROTECTED_FIELD(mDragOffset);
	PROTECTED_FIELD(mInstSceneDragActors);
	PROTECTED_FIELD(mDragAssetPropertyField);
	PROTECTED_FIELD(mDragActorPropertyField);
	PROTECTED_FIELD(mDragComponentPropertyField);
	PROTECTED_FIELD(mCuttingAssets);
	PROTECTED_FIELD(mNeedRebuildAssets);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetViewingPath, const String&);
	PUBLIC_FUNCTION(String, GetViewingPath);
	PUBLIC_FUNCTION(void, UpdateAssetsPath);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, SelectAsset, AssetId, bool);
	PUBLIC_FUNCTION(void, DeselectAllAssets);
	PUBLIC_FUNCTION(Vector<AssetInfo>, GetSelectedAssets);
	PUBLIC_FUNCTION(UIAssetIcon*, GetIconUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, UpdateLayout, bool);
	PROTECTED_FUNCTION(void, UpdateCuttingAssets);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnUnfocused);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, BeginSelecting);
	PROTECTED_FUNCTION(void, UpdateSelection, const Input::Cursor&);
	PROTECTED_FUNCTION(void, CompleteSelecting);
	PROTECTED_FUNCTION(void, BeginDragging, UIAssetIcon*, const Input::Cursor&);
	PROTECTED_FUNCTION(void, UpdateDragging, const Input::Cursor&);
	PROTECTED_FUNCTION(void, CompleteDragging);
	PROTECTED_FUNCTION(void, RegActorsCreationAction);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, InitializeContext);
	PROTECTED_FUNCTION(void, PrepareIconsPools);
	PROTECTED_FUNCTION(UIAssetIcon*, GetAssetIconFromPool, const String&);
	PROTECTED_FUNCTION(void, FreeAssetIconToPool, UIAssetIcon*);
	PROTECTED_FUNCTION(UIAssetIcon*, GetImageAssetIcon, const AssetInfo&);
	PROTECTED_FUNCTION(void, UpdateAssetsGridSize);
	PROTECTED_FUNCTION(void, InitializeSelectionSprite);
	PROTECTED_FUNCTION(Sprite*, GetSelectionSprite);
	PROTECTED_FUNCTION(void, FreeSelectionSprite, Sprite*);
	PROTECTED_FUNCTION(void, OnIconDblClicked, UIAssetIcon*);
	PROTECTED_FUNCTION(void, UpdateHover);
	PROTECTED_FUNCTION(void, OnContextCopyPressed);
	PROTECTED_FUNCTION(void, OnContextCutPressed);
	PROTECTED_FUNCTION(void, OnContextPastePressed);
	PROTECTED_FUNCTION(void, OnContextDeletePressed);
	PROTECTED_FUNCTION(void, OnContextOpenPressed);
	PROTECTED_FUNCTION(void, OnContextShowInExplorerPressed);
	PROTECTED_FUNCTION(void, OnContextImportPressed);
	PROTECTED_FUNCTION(void, OnContextCreateFolderPressed);
	PROTECTED_FUNCTION(void, OnContextCreatePrefabPressed);
	PROTECTED_FUNCTION(void, OnContextCreateScriptPressed);
	PROTECTED_FUNCTION(void, OnContextCreateAnimationPressed);
	PROTECTED_FUNCTION(void, InstantiateDraggingAssets);
	PROTECTED_FUNCTION(void, ClearInstantiatedDraggingAssets);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const AssetInfo&);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const ImageAsset&);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const ActorAsset&);
}
END_META;

ENUM_META_(Editor::UIAssetsIconsScrollArea::DragState, DragState)
{
	ENUM_ENTRY(ActorField);
	ENUM_ENTRY(AssetField);
	ENUM_ENTRY(ComponentField);
	ENUM_ENTRY(Off);
	ENUM_ENTRY(Regular);
	ENUM_ENTRY(Scene);
	ENUM_ENTRY(Tree);
}
END_ENUM_META;
 