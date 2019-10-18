#include "stdafx.h"
#include "CurveEditor.h"

#include "Application/Application.h"
#include "Core/Dialogs/KeyEditDlg.h"
#include "Core/EditorScope.h"
#include "Core/UI/CurveEditor/Actions.h"
#include "Core/UIRoot.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"
#include "Scene/UI/Widgets/ContextMenu.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/HorizontalLayout.h"
#include "Scene/UI/Widgets/HorizontalScrollBar.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Scene/UI/Widgets/VerticalScrollBar.h"
#include "Scene/UI/Widgets/Window.h"
#include "Utils/System/Clipboard.h"
 
namespace Editor
{
	CurveEditor::CurveEditor():
		FrameScrollView()
	{
		mReady = false;

		mSelectionSprite = mnew Sprite();
		mTextFont = FontRef("stdFont.ttf");
		mTextFont->CheckCharacters("0123456789.,+-", 10);

		InitializeTextDrawables();
		InitializeContextMenu();

		mTransformFrame.SetPivotEnabled(false);
		mTransformFrame.SetRotationEnabled(false);
		mTransformFrame.onTransformed = THIS_FUNC(OnTransformFrameTransformed);
		mTransformFrame.onPressed = THIS_FUNC(OnTransformBegin);
		mTransformFrame.onChangeCompleted = THIS_FUNC(OnTransformCompleted);
		mTransformFrame.messageFallDownListener = this;

		mBackColor = Color4(225, 232, 232, 255);

		mReady = true;
	}

	CurveEditor::CurveEditor(const CurveEditor& other):
		FrameScrollView(other), mSelectionSprite(other.mSelectionSprite->CloneAs<Sprite>()), mTextFont(other.mTextFont)
	{
		mReady = false;

		mContextMenu = FindChildByType<ContextMenu>();
		if (mContextMenu)
			delete mContextMenu;

		InitializeTextDrawables();
		InitializeContextMenu();
		RetargetStatesAnimations();

		mReady = true;
	}

	CurveEditor::~CurveEditor()
	{
		for (auto curve : mCurves)
			delete curve;

		if (mSelectionSprite)
			delete mSelectionSprite;

		if (mTextLeft)
			delete mTextLeft;

		if (mTextRight)
			delete mTextRight;

		if (mTextTop)
			delete mTextTop;

		if (mTextBottom)
			delete mTextBottom;
	}

	Editor::CurveEditor& CurveEditor::operator=(const CurveEditor& other)
	{
		FrameScrollView::operator=(other);
		return *this;
	}

	void CurveEditor::Draw()
	{
		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		FrameScrollView::Draw();

		DrawTransformFrame();
		DrawHandles();
		DrawSelection();
	}

	void CurveEditor::Update(float dt)
	{
		FrameScrollView::Update(dt);

		if (mReady && mResEnabledInHierarchy && !mIsClipped && mNeedAdjustView)
		{
			mNeedAdjustView = false;
			mViewCameraTargetScale = mAvailableArea.Size()/mViewCamera.GetSize();
			mViewCamera.center = mAvailableArea.Center();
		}
	}

	Dictionary<String, Curve*> CurveEditor::GetCurves() const
	{
		Dictionary<String, Curve*> res;
		for (auto curveInfo : mCurves)
			res.Add(curveInfo->curveId, curveInfo->curve);

		return res;
	}

	void CurveEditor::AddCurve(const String& id, Curve* curve, const Color4& color /*= Color4::Green()*/)
	{
		UpdateSelfTransform();

		CurveInfo* info = mnew CurveInfo();
		info->curveEditor = this;
		info->curveId = id;
		info->curve = curve;
		info->viewScale = Vec2F();
		info->UpdateApproximatedPoints();
		info->curve->onKeysChanged += MakeSubscription(info, &CurveInfo::OnCurveChanged, [=]() { info->curve = nullptr; });

		if (color == Color4::Green())
		{
			if (mCurves.IsEmpty())
				info->color = Color4::Green();
			else
				info->color = Color4::SomeColor(mCurves.Count());
		}
		else info->color = color;

		for (int i = 0; i < curve->GetKeys().Count(); i++)
			AddCurveKeyHandles(info, i);

		mCurves.Add(info);
		RecalculateViewArea();

		if (mCurves.Count() == 1)
			mNeedAdjustView = true;
	}

	void CurveEditor::RemoveCurve(Curve* curve)
	{
		for (auto info : mCurves)
		{
			if (info->curve == curve)
			{
				for (auto handle : info->handles)
				{
					handle->mainHandle.SetSelectionGroup(nullptr);

					handle->leftSupportHandle.SetSelectionGroup(nullptr);
					mSupportHandles.Remove(&handle->leftSupportHandle);

					handle->rightSupportHandle.SetSelectionGroup(nullptr);
					mSupportHandles.Remove(&handle->rightSupportHandle);
				}

				delete info;
				mCurves.Remove(info);
				break;
			}
		}
	}

	void CurveEditor::RemoveCurve(const String& id)
	{
		Curve* curve = FindCurve(id);
		if (curve)
			RemoveCurve(curve);
	}

	void CurveEditor::RemoveAllCurves()
	{
		auto curves = mCurves;
		for (auto info : curves)
			RemoveCurve(info->curve);
	}

	void CurveEditor::AddCurvesRange(Curve* curveA, Curve* curveB, const Color4& color /*= Color4(-1, -1, -1, -1)*/)
	{

	}

	void CurveEditor::AddCurvesRange(const String& idA, const String& idB, const Color4& color /*= Color4::Green()*/)
	{

	}

	void CurveEditor::RemoveCurvesRange(Curve* curveA, Curve* curveB)
	{

	}

	void CurveEditor::RemoveCurvesRange(const String& idA, const String& idB)
	{

	}

	void CurveEditor::UpdateSelfTransform()
{
		FrameScrollView::UpdateSelfTransform();

		UpdateLocalScreenTransforms();
		OnCameraTransformChanged();
	}

	void CurveEditor::SetSelectionSpriteImage(const ImageAssetRef& image)
	{
		mSelectionSprite->LoadFromImage(image);
	}

	void CurveEditor::SetTextFont(const FontRef& font)
	{
		mTextFont = font;
		mTextLeft->SetFont(mTextFont);
		mTextRight->SetFont(mTextFont);
		mTextTop->SetFont(mTextFont);
		mTextBottom->SetFont(mTextFont);
	}

	void CurveEditor::SetMainHandleImages(const ImageAssetRef& regular, const ImageAssetRef& hover,
											const ImageAssetRef& pressed, const ImageAssetRef& selected)
	{
		mMainHandleSample = DragHandle(mnew Sprite(regular), mnew Sprite(hover),
												 mnew Sprite(pressed), mnew Sprite(selected));
	}

	void CurveEditor::SetSupportHandleImages(const ImageAssetRef& regular, const ImageAssetRef& hover, const ImageAssetRef& pressed, const ImageAssetRef& selected)
	{
		mSupportHandleSample = DragHandle(mnew Sprite(regular), mnew Sprite(hover),
													mnew Sprite(pressed), mnew Sprite(selected));
	}

	void CurveEditor::CopyData(const Actor& otherActor)
	{
		const CurveEditor& other = dynamic_cast<const CurveEditor&>(otherActor);

		FrameScrollView::CopyData(other);

		mReady = false;

		delete mSelectionSprite;
		delete mTextLeft;
		delete mTextRight;
		delete mTextTop;
		delete mTextBottom;

		mSelectionSprite = other.mSelectionSprite->CloneAs<Sprite>();
		mTextFont        = other.mTextFont;

		mMainHandleSample = other.mMainHandleSample;
		mSupportHandleSample = other.mSupportHandleSample;

		InitializeTextDrawables();
		RetargetStatesAnimations();

		mReady = true;
	}

	void CurveEditor::OnResEnableInHierarchyChanged()
	{
		if (mResEnabled)
			mContextMenu->SetItemsMaxPriority();
		else
			mContextMenu->SetItemsMinPriority();
	}

	void CurveEditor::OnScrolled(float scroll)
	{
		Vec2F newScale = mViewCameraTargetScale;

		if (o2Input.IsKeyDown(VK_CONTROL))
			newScale.x *= 1.0f - (scroll*mViewCameraScaleSence);
		else if (o2Input.IsKeyDown(VK_SHIFT))
			newScale.y *= 1.0f - (scroll*mViewCameraScaleSence);
		else
			newScale *= 1.0f - (scroll*mViewCameraScaleSence);

		ChangeCameraScaleRelativeToCursor(newScale);
	}

	Curve* CurveEditor::FindCurve(const String& id)
	{
		for (auto curve : mCurves)
		{
			if (curve->curveId == id)
				return curve->curve;
		}

		return nullptr;
	}

	void CurveEditor::InitializeContextMenu()
	{
		mContextMenu = o2UI.CreateWidget<ContextMenu>();

		mContextMenu->AddItems({

			ContextMenu::Item("Auto smooth", false, THIS_FUNC(OnAutoSmoothChecked)),
			ContextMenu::Item("Flat", false, THIS_FUNC(OnFlatChecked)),
			ContextMenu::Item("Free", false, THIS_FUNC(OnFreeChecked)),
			ContextMenu::Item("Broken", false, THIS_FUNC(OnBrokenChecked)),
			ContextMenu::Item("Discrete", false, THIS_FUNC(OnDiscreteChecked)),

			ContextMenu::Item::Separator(),

			ContextMenu::Item("Edit", THIS_FUNC(OnEditPressed), ImageAssetRef()),

			ContextMenu::Item::Separator(),

			ContextMenu::Item("Copy keys", THIS_FUNC(OnCopyPressed), ImageAssetRef(), ShortcutKeys('C', true)),
			ContextMenu::Item("Cut keys", THIS_FUNC(OnCutPressed), ImageAssetRef(), ShortcutKeys('X', true)),
			ContextMenu::Item("Paste keys", THIS_FUNC(OnPastePressed), ImageAssetRef(), ShortcutKeys('V', true)),
			ContextMenu::Item("Delete keys", THIS_FUNC(OnDeletePressed), ImageAssetRef(), ShortcutKeys(VK_DELETE)),
			ContextMenu::Item("Insert key", THIS_FUNC(OnInsertPressed), ImageAssetRef()),

			ContextMenu::Item::Separator(),

			ContextMenu::Item("Undo", THIS_FUNC(OnUndoPressed), ImageAssetRef(), ShortcutKeys('Z', true)),
			ContextMenu::Item("Redo", THIS_FUNC(OnRedoPressed), ImageAssetRef(), ShortcutKeys('Z', true, true))
		});

		onShow = [&]() { mContextMenu->SetItemsMaxPriority(); };
		onHide = [&]() { mContextMenu->SetItemsMinPriority(); };

		AddChild(mContextMenu);
	}

	void CurveEditor::InitializeTextDrawables()
	{
		mTextLeft = mnew Text(mTextFont);
		mTextLeft->SetHeight(10);
		mTextLeft->SetHorAlign(HorAlign::Left);
		mTextLeft->SetVerAlign(VerAlign::Bottom);
		mTextLeft->color = Color4(96, 125, 139);

		mTextRight = mnew Text(mTextFont);
		mTextRight->SetHeight(10);
		mTextRight->SetHorAlign(HorAlign::Right);
		mTextRight->SetVerAlign(VerAlign::Bottom);
		mTextRight->color = Color4(96, 125, 139);


		mTextTop = mnew Text(mTextFont);
		mTextTop->SetHeight(10);
		mTextTop->SetHorAlign(HorAlign::Left);
		mTextTop->SetVerAlign(VerAlign::Bottom);
		mTextTop->SetAngleDegrees(-90.0f);
		mTextTop->color = Color4(96, 125, 139);


		mTextBottom = mnew Text(mTextFont);
		mTextBottom->SetHeight(10);
		mTextBottom->SetHorAlign(HorAlign::Right);
		mTextBottom->SetVerAlign(VerAlign::Bottom);
		mTextBottom->SetAngleDegrees(-90.0f);
		mTextBottom->color = Color4(96, 125, 139);
	}

	void CurveEditor::RecalculateViewArea()
	{
		if (mCurves.IsEmpty())
		{
			mAvailableArea = RectF(0, 0, 1, 1);
			return;
		}

		Vec2F firstPoint;
		if (!mCurves.Last()->approximatedPoints.IsEmpty())
			firstPoint = mCurves.Last()->approximatedPoints.Last();

		mAvailableArea = RectF(firstPoint, firstPoint);

		for (auto curve : mCurves)
		{
			for (auto& p : curve->approximatedPoints)
			{
				mAvailableArea.left = Math::Min(mAvailableArea.left, p.x);
				mAvailableArea.right = Math::Max(mAvailableArea.right, p.x);
				mAvailableArea.top = Math::Max(mAvailableArea.top, p.y);
				mAvailableArea.bottom = Math::Min(mAvailableArea.bottom, p.y);
			}
		}

		float bordersCoef = 1.5f;
		Vec2F size = mAvailableArea.Size();
		mAvailableArea.left -= size.x*bordersCoef;
		mAvailableArea.right += size.x*bordersCoef;
		mAvailableArea.top += size.y*bordersCoef;
		mAvailableArea.bottom -= size.y*bordersCoef;

		mHorScrollbar->SetValueRange(mAvailableArea.left, mAvailableArea.right);
		mVerScrollbar->SetValueRange(mAvailableArea.bottom, mAvailableArea.top);
	}

	void CurveEditor::RedrawContent()
	{
		DrawGrid();
		DrawCurves();

// 		o2Render.DrawRectFrame(mAvailableArea);
// 
// 		RectF outside = mAvailableArea;
// 		outside.left -= mAvailableArea.Width()*0.1f;
// 		outside.right += mAvailableArea.Width()*0.1f;
// 		outside.bottom -= mAvailableArea.Height()*0.1f;
// 		outside.top += mAvailableArea.Height()*0.1f;
// 
// 		o2Render.DrawRectFrame(outside, Color4::Green());
// 
// 		RectF inside = mAvailableArea;
// 		inside.left += mAvailableArea.Width()*0.1f;
// 		inside.right -= mAvailableArea.Width()*0.1f;
// 		inside.bottom += mAvailableArea.Height()*0.1f;
// 		inside.top -= mAvailableArea.Height()*0.1f;
// 
// 		o2Render.DrawRectFrame(inside, Color4::Red());
	}

	void CurveEditor::DrawGrid()
	{
		float cameraMaxSize = Math::Max(mViewCamera.GetSize().x*mViewCamera.GetScale().x,
										mViewCamera.GetSize().y*mViewCamera.GetScale().y);

		float x = cameraMaxSize / 2.0f;
		float minCellSize = 0.000001f;
		float maxCellSize = 1000000.0f;
		float cellSize = minCellSize;
		while (cellSize < maxCellSize)
		{
			float next = cellSize*10.0f;
			if (x > cellSize && x <= next)
				break;

			cellSize = next;
		}

		Vec2F gridOrigin(Math::Round(mViewCamera.GetPosition().x / cellSize)*cellSize,
						 Math::Round(mViewCamera.GetPosition().y / cellSize)*cellSize);

		int cellsCount = Math::CeilToInt(cameraMaxSize / cellSize);
		float tenCeilsSize = cellSize*10.0f;
		float screenCellSize = cellSize / mViewCamera.GetScale().x;
		Color4 cellColorSmoothed = Math::Lerp(mGridColor, mBackColor, 0.7f);

		mTextLeft->SetScale(mViewCamera.GetScale());
		mTextRight->SetScale(mViewCamera.GetScale());

		Vec2F invScale(mViewCamera.GetScale().y, mViewCamera.GetScale().x);
		mTextTop->SetScale(invScale);
		mTextBottom->SetScale(invScale);

		for (int i = -cellsCount / 2; i < cellsCount / 2; i++)
		{
			float d = (float)i*cellSize;
			Vec2F dorigin = gridOrigin + Vec2F(d, d);

			float rdx = Math::Abs(dorigin.x / tenCeilsSize - Math::Floor(dorigin.x / tenCeilsSize));
			float rdy = Math::Abs(dorigin.y / tenCeilsSize - Math::Floor(dorigin.y / tenCeilsSize));
			bool xTen = rdx < 0.05f || rdx > 0.95f;
			bool yTen = rdy < 0.05f || rdy > 0.95f;

			Vec2F xBegin = Vec2F(-cameraMaxSize, d) + gridOrigin;
			Vec2F xEnd = Vec2F(cameraMaxSize, d) + gridOrigin;
			Vec2F yBegin = Vec2F(d, -cameraMaxSize) + gridOrigin;
			Vec2F yEnd = Vec2F(d, cameraMaxSize) + gridOrigin;

			if (horGridEnabled)
				o2Render.DrawAALine(xBegin, xEnd, yTen ? mGridColor : cellColorSmoothed);

			if (verGridEnabled) 
				o2Render.DrawAALine(yBegin, yEnd, xTen ? mGridColor : cellColorSmoothed);
		}

		char buf[255];
		for (int i = -cellsCount / 2; i < cellsCount / 2; i++)
		{
			float x = (float)i*cellSize + gridOrigin.x;
			float y = (float)i*cellSize + gridOrigin.y;

			sprintf(buf, "%.1f", (Math::Round(x*10.0f)/10.0f));
			String xCaption = buf;

			sprintf(buf, "%.1f", (Math::Round(y*10.0f)/10.0f));
			String yCaption = buf;

			if (horGridEnabled)
			{
				mTextLeft->SetText(yCaption);
				mTextLeft->SetPosition(Vec2F(mViewCamera.GetRect().left, y));
				mTextLeft->Draw();

				mTextRight->SetText(yCaption);
				mTextRight->SetPosition(Vec2F(mViewCamera.GetRect().right, y));
				mTextRight->Draw();
			}

			if (verGridEnabled)
			{
				mTextTop->SetText(xCaption);
				mTextTop->SetPosition(Vec2F(x, mViewCamera.GetRect().top));
				mTextTop->Draw();

				mTextBottom->SetText(xCaption);
				mTextBottom->SetPosition(Vec2F(x, mViewCamera.GetRect().bottom));
				mTextBottom->Draw();
			}
		}
	}

	void CurveEditor::DrawCurves()
	{
		o2Render.camera = Camera();

		Basis transform = mViewCamera.GetBasis().Inverted()*Camera().GetBasis();

		for (auto curve : mCurves)
		{
			if (curve->approximatedPoints.IsEmpty())
				continue;

			//o2Render.DrawLine(curve->approximatedPoints, curve->color);

			float cameraLeftPos = mViewCamera.GetRect().left;
			float cameraRightPos = mViewCamera.GetRect().right;
			for (int i = 1; i < curve->approximatedPoints.Count(); i++)
			{
				Vec2F& a = curve->approximatedPoints[i - 1];
				Vec2F& b = curve->approximatedPoints[i];

				if (b.x < cameraLeftPos)
					continue;

				if (a.x > cameraRightPos)
					break;

				o2Render.DrawAALine(a*transform, b*transform, curve->color);
			}

// 			int idx = 0;
// 			for (auto key : curve->curve->GetKeys())
// 			{
// 				mTextLeft->position = Vec2F(key.position, key.value - 0.1f);
// 				mTextLeft->text = (String)idx + ": \n" + (String)key.leftSupportPosition + ", " + (String)key.leftSupportValue +
// 					"\n" + (String)key.rightSupportPosition + ", " + (String)key.rightSupportValue;
// 				mTextLeft->Draw();
// 				idx++;
// 			}
		}

		o2Render.camera = mViewCamera;
	}

	void CurveEditor::DrawHandles()
	{
		RectF camRect = mViewCamera.GetRect();
		for (auto curve : mCurves)
		{
			for (auto keyHandles : curve->handles)
				keyHandles->Draw(camRect);
		}
	}

	void CurveEditor::DrawSelection()
	{
		if (mIsPressed)
		{
			mSelectionSprite->rect = RectF(LocalToScreenPoint(mSelectingPressedPoint), o2Input.cursorPos);
			mSelectionSprite->Draw();
		}
	}

	void CurveEditor::DrawTransformFrame()
	{
		if (!mTransformFrameVisible)
			return;

		Vec2F borders(10, 10);

		mTransformFrame.SetBasis(Basis(LocalToScreenPoint(mTransformFrameBasis.origin) - borders,
								 mTransformFrameBasis.xv/mViewCamera.GetScale() + Vec2F(borders.x*2.0f, 0),
								 mTransformFrameBasis.yv/mViewCamera.GetScale() + Vec2F(0, borders.y*2.0f)));

		mTransformFrame.Draw();

		if (o2Input.IsKeyDown(VK_CONTROL))
		{
			Vec2F left = mTransformFrame.GetCurrentBasis().origin;
			Vec2F right = mTransformFrame.GetCurrentBasis().origin + mTransformFrame.GetCurrentBasis().xv;
			RectF rect = layout->worldRect;

			o2Render.DrawAALine(Vec2F(right.x, rect.bottom), Vec2F(right.x, rect.top), mTransformFrame.GetFrameColor());
			o2Render.DrawAALine(Vec2F(left.x, rect.bottom), Vec2F(left.x, rect.top), mTransformFrame.GetFrameColor());
		}
	}

	void CurveEditor::AddCurveKeyHandles(CurveInfo* info, int keyId)
	{
		PushScopeEnterOnStack scope;

		KeyHandles* keyHandles = mnew KeyHandles(mMainHandleSample, mSupportHandleSample, this);
		keyHandles->curveKeyIdx = keyId;
		keyHandles->curveKeyUid = info->curve->GetKeyAt(keyId).uid;

		Curve::Key curveKey = info->curve->GetKeyAt(keyId);
		Curve::Key lastCurveKey = info->curve->GetKeyAt(Math::Max(0, keyId - 1));
		Curve::Key nextCurveKey = info->curve->GetKeyAt(Math::Min(keyId + 1, info->curve->GetKeys().Count()));

		// main handle
		keyHandles->mainHandle.SetPosition(Vec2F(curveKey.position, curveKey.value));
		keyHandles->mainHandle.onChangedPos = [=](const Vec2F& pos) { OnCurveKeyMainHandleDragged(info, keyHandles, pos); };
		keyHandles->mainHandle.localToScreenTransformFunc = [&](const Vec2F& p) { return LocalToScreenPoint(p); };
		keyHandles->mainHandle.screenToLocalTransformFunc = [&](const Vec2F& p) { return ScreenToLocalPoint(p); };
		keyHandles->mainHandle.onRightButtonReleased = THIS_FUNC(OnCursorRightMouseReleased);
		keyHandles->mainHandle.onPressed = THIS_FUNC(OnTransformBegin);
		keyHandles->mainHandle.onChangeCompleted = THIS_FUNC(OnTransformCompleted);
		keyHandles->mainHandle.messageFallDownListener = this;


		// left support handle
		keyHandles->leftSupportHandle.SetPosition(Vec2F(curveKey.position + curveKey.leftSupportPosition,
												  curveKey.value + curveKey.leftSupportValue));

		keyHandles->leftSupportHandle.localToScreenTransformFunc =
			[&](const Vec2F& p) { return LocalToScreenPoint(p); };

		keyHandles->leftSupportHandle.screenToLocalTransformFunc =
			[&](const Vec2F& p) { return ScreenToLocalPoint(p); };

		keyHandles->leftSupportHandle.onChangedPos =
			[=](const Vec2F& pos) { OnCurveKeyLeftSupportHandleDragged(info, keyHandles, pos); };

		keyHandles->leftSupportHandle.checkPositionFunc =
			[=](const Vec2F& pos) { return CheckLeftSupportHandlePosition(info, keyHandles, pos); };

		keyHandles->leftSupportHandle.enabled = false;
		keyHandles->leftSupportHandle.onRightButtonReleased = THIS_FUNC(OnCursorRightMouseReleased);
		keyHandles->leftSupportHandle.onPressed = THIS_FUNC(OnTransformBegin);
		keyHandles->leftSupportHandle.onChangeCompleted = THIS_FUNC(OnTransformCompleted);
		keyHandles->leftSupportHandle.messageFallDownListener = this;


		// right support handle
		keyHandles->rightSupportHandle.SetPosition(Vec2F(curveKey.position + curveKey.rightSupportPosition,
												   curveKey.value + curveKey.rightSupportValue));

		keyHandles->rightSupportHandle.localToScreenTransformFunc =
			[&](const Vec2F& p) { return LocalToScreenPoint(p); };

		keyHandles->rightSupportHandle.screenToLocalTransformFunc =
			[&](const Vec2F& p) { return ScreenToLocalPoint(p); };

		keyHandles->rightSupportHandle.onChangedPos =
			[=](const Vec2F& pos) { OnCurveKeyRightSupportHandleDragged(info, keyHandles, pos); };

		keyHandles->rightSupportHandle.checkPositionFunc =
			[=](const Vec2F& pos) { return CheckRightSupportHandlePosition(info, keyHandles, pos); };

		keyHandles->rightSupportHandle.enabled = false;
		keyHandles->rightSupportHandle.onRightButtonReleased = THIS_FUNC(OnCursorRightMouseReleased);
		keyHandles->rightSupportHandle.onPressed = THIS_FUNC(OnTransformBegin);
		keyHandles->rightSupportHandle.onChangeCompleted = THIS_FUNC(OnTransformCompleted);
		keyHandles->rightSupportHandle.messageFallDownListener = this;

		for (int i = keyId; i < info->handles.Count(); i++)
			info->handles[i]->curveKeyIdx++;

		// Register handles
		info->handles.Insert(keyHandles, keyId);

		mSupportHandles.Add(&keyHandles->leftSupportHandle);
		mSupportHandles.Add(&keyHandles->rightSupportHandle);

		keyHandles->mainHandle.SetSelectionGroup(this);
		keyHandles->leftSupportHandle.SetSelectionGroup(&mSupportHandlesGroup);
		keyHandles->rightSupportHandle.SetSelectionGroup(&mSupportHandlesGroup);
	}

	void CurveEditor::RemoveCurveKeyHandles(CurveInfo* info, int keyId)
	{
		for (int i = keyId + 1; i < info->handles.Count(); i++)
			info->handles[i]->curveKeyIdx--;

		info->handles[keyId]->mainHandle.SetSelectionGroup(nullptr);
		info->handles[keyId]->leftSupportHandle.SetSelectionGroup(nullptr);
		info->handles[keyId]->rightSupportHandle.SetSelectionGroup(nullptr);

		mSupportHandles.Remove(&info->handles[keyId]->leftSupportHandle);
		mSupportHandles.Remove(&info->handles[keyId]->rightSupportHandle);

		delete info->handles[keyId];
		info->handles.RemoveAt(keyId);
	}

	void CurveEditor::OnCurveKeyMainHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
	{
		info->BeginCurveManualChange();

		Curve::Key key = info->curve->GetKeyAt(handles->curveKeyIdx);

		Vec2F initialDragPoint = handles->mainHandle.GetDraggingBeginPosition();

		if (handles->mainHandle.IsPressed())
		{
			if (o2Input.IsKeyDown(VK_CONTROL) && o2Input.IsKeyDown(VK_SHIFT))
			{
				key.position = Math::Round(position.x*10.0f)/10.0f;
				key.value = Math::Round(position.y*10.0f)/10.0f;
			}
			else if (o2Input.IsKeyDown(VK_CONTROL))
			{
				key.position = position.x;
				key.value = initialDragPoint.y;
			}
			else if (o2Input.IsKeyDown(VK_SHIFT))
			{
				key.position = initialDragPoint.x;
				key.value = position.y;
			}
			else
			{
				key.position = position.x;
				key.value = position.y;
			}
		}
		else
		{
			key.position = position.x;
			key.value = position.y;
		}

		int newKeyIdx = 0;
		for (auto& k : info->curve->GetKeys())
		{
			if (k.position > key.position)
				break;

			newKeyIdx++;
		}

		if (newKeyIdx > handles->curveKeyIdx)
			newKeyIdx--;

		if (newKeyIdx != handles->curveKeyIdx)
		{
			info->curve->RemoveKeyAt(handles->curveKeyIdx);
			info->curve->InsertKey(key);

			info->handles.RemoveAt(handles->curveKeyIdx);
			info->handles.Insert(handles, newKeyIdx);

			for (int i = 0; i < info->handles.Count(); i++)
			{
				info->handles[i]->curveKeyIdx = i;
				info->handles[i]->curveKeyUid = key.uid;
			}
		}

		info->curve->SetKey(key, handles->curveKeyIdx);

		info->UpdateHandles();
		info->UpdateApproximatedPoints();

		CheckHandlesVisible();
		UpdateTransformFrame();
		RecalculateViewArea();
		mNeedRedraw = true;

		info->CompleteCurveManualChange();
	}

	void CurveEditor::OnCurveKeyLeftSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
	{
		info->BeginCurveManualChange();

		Curve::Key key = info->curve->GetKeyAt(handles->curveKeyIdx);
		Curve::Key prevKey = info->curve->GetKeyAt(Math::Max(handles->curveKeyIdx - 1, 0));

		if (o2Input.IsKeyDown(VK_CONTROL) && o2Input.IsKeyDown(VK_SHIFT))
		{
			Vec2F initialDragPoint = handles->leftSupportHandle.GetDraggingBeginPosition();
			float dst = (initialDragPoint - Vec2F(key.position, key.value)).Length();
			Vec2F v = (position - Vec2F(key.position, key.value)).Normalized()*dst;
			key.leftSupportPosition = v.x;
			key.leftSupportValue = v.y;
		}
		else if (o2Input.IsKeyDown(VK_CONTROL))
		{
			key.leftSupportPosition = position.x - key.position;
			key.leftSupportValue = 0;
		}
		else if (o2Input.IsKeyDown(VK_SHIFT))
		{
			Vec2F initialDragPoint = handles->leftSupportHandle.GetDraggingBeginPosition();

			Vec2F v = initialDragPoint - Vec2F(key.position, key.value);
			float dst = (position - Vec2F(key.position, key.value)).Length();
			v = v.Normalized()*dst;
			key.leftSupportPosition = v.x;
			key.leftSupportValue = v.y;
		}
		else
		{
			key.leftSupportPosition = position.x - key.position;
			key.leftSupportValue = position.y - key.value;
		}

		if (key.supportsType == Curve::Key::Type::Flat || key.supportsType == Curve::Key::Type::Discrete ||
			key.supportsType == Curve::Key::Type::Smooth)
		{
			key.supportsType = Curve::Key::Type::Free;
		}

		if (key.supportsType == Curve::Key::Type::Free && handles->curveKeyIdx < info->curve->GetKeys().Count() - 1)
		{
			Curve::Key nextKey = info->curve->GetKeyAt(Math::Min(handles->curveKeyIdx + 1,
													   info->curve->GetKeys().Count() - 1));

			key.rightSupportPosition = -key.leftSupportPosition;
			key.rightSupportValue = -key.leftSupportValue;
		}

		info->curve->SetKey(key, handles->curveKeyIdx);

		info->UpdateHandles();
		info->UpdateApproximatedPoints();

		CheckHandlesVisible();
		RecalculateViewArea();
		mNeedRedraw = true;

		info->CompleteCurveManualChange();
	}

	void CurveEditor::OnCurveKeyRightSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
	{
		info->BeginCurveManualChange();

		Curve::Key key = info->curve->GetKeyAt(handles->curveKeyIdx);
		Curve::Key nextKey = info->curve->GetKeyAt(Math::Min(handles->curveKeyIdx + 1, info->curve->GetKeys().Count() - 1));

		if (o2Input.IsKeyDown(VK_CONTROL) && o2Input.IsKeyDown(VK_SHIFT))
		{
			Vec2F initialDragPoint = handles->rightSupportHandle.GetDraggingBeginPosition();
			float dst = (initialDragPoint - Vec2F(key.position, key.value)).Length();
			Vec2F v = (position - Vec2F(key.position, key.value)).Normalized()*dst;
			key.rightSupportPosition = v.x;
			key.rightSupportValue = v.y;
		}
		else if (o2Input.IsKeyDown(VK_CONTROL))
		{
			key.rightSupportPosition = position.x - key.position;
			key.rightSupportValue = 0;
		}
		else if (o2Input.IsKeyDown(VK_SHIFT))
		{
			Vec2F initialDragPoint = handles->rightSupportHandle.GetDraggingBeginPosition();

			Vec2F v = initialDragPoint - Vec2F(key.position, key.value);
			float dst = (position - Vec2F(key.position, key.value)).Length();
			v = v.Normalized()*dst;
			key.rightSupportPosition = v.x;
			key.rightSupportValue = v.y;
		}
		else
		{
			key.rightSupportPosition = position.x - key.position;
			key.rightSupportValue = position.y - key.value;
		}

		if (key.supportsType == Curve::Key::Type::Flat || key.supportsType == Curve::Key::Type::Discrete ||
			key.supportsType == Curve::Key::Type::Smooth)
		{
			key.supportsType = Curve::Key::Type::Free;
		}

		if (key.supportsType == Curve::Key::Type::Free && handles->curveKeyIdx > 0)
		{
			Curve::Key prevKey = info->curve->GetKeyAt(Math::Max(handles->curveKeyIdx - 1, 0));

			Vec2F rightSupportVector = Vec2F(key.rightSupportPosition, key.rightSupportValue);
			Vec2F leftSupportVector = rightSupportVector.Inverted();

			key.leftSupportPosition = -key.rightSupportPosition;
			key.leftSupportValue = -key.rightSupportValue;

			handles->leftSupportHandle.position = Vec2F(key.position + key.leftSupportPosition,
														key.value + key.leftSupportValue);
		}

		info->curve->SetKey(key, handles->curveKeyIdx);

		info->UpdateHandles();
		info->UpdateApproximatedPoints();

		CheckHandlesVisible();
		RecalculateViewArea();
		mNeedRedraw = true;

		info->CompleteCurveManualChange();
	}

	Vec2F CurveEditor::CheckLeftSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
	{
		Curve::Key key = info->curve->GetKeyAt(handles->curveKeyIdx);
		Curve::Key prevKey = info->curve->GetKeyAt(Math::Max(handles->curveKeyIdx - 1, 0));

		Vec2F relativePos = position - Vec2F(key.position, key.value);

		if (relativePos.x > 0.0f)
			relativePos.x = 0.0f;

		if (relativePos.x < prevKey.position - key.position)
			relativePos *= (prevKey.position - key.position)/relativePos.x;

		return Vec2F(key.position, key.value) + relativePos;
	}

	Vec2F CurveEditor::CheckRightSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
	{
		Curve::Key key = info->curve->GetKeyAt(handles->curveKeyIdx);
		Curve::Key nextKey = info->curve->GetKeyAt(Math::Min(handles->curveKeyIdx + 1, info->curve->GetKeys().Count() - 1));

		Vec2F relativePos = position - Vec2F(key.position, key.value);

		if (relativePos.x < 0.0f)
			relativePos.x = 0.0f;

		if (relativePos.x > nextKey.position - key.position)
			relativePos *= (nextKey.position - key.position)/relativePos.x;

		return Vec2F(key.position, key.value) + relativePos;
	}

	void CurveEditor::OnCursorDblClicked(const Input::Cursor& cursor)
	{
		const float createPointDistanceThreshold = 7;
		const float supportValuesCoef = 0.15f;

		CurveInfo* clickedCurveInfo = nullptr;
		Curve::Key newKey;
		Vec2F localCursorPos = ScreenToLocalPoint(cursor.position);

		for (auto info : mCurves)
		{
			auto& keys = info->curve->GetKeys();
			int keyIdx = 0;
			for (auto& key : keys)
			{
				if (keyIdx == 0)
				{
					keyIdx++;
					continue;
				}

				const Vec2F* points = key.GetApproximatedPoints();
				for (int i = 1; i < key.GetApproximatedPointsCount(); i++)
				{
					Vec2F a = points[i - 1], b = points[i];
					Vec2F ab = b - a;
					float abl = ab.Length();
					Vec2F abn = ab/abl;
					Vec2F abnp = abn.Perpendicular();
					Vec2F ac = cursor.position - LocalToScreenPoint(a);
					Vec2F lac = ScreenToLocalPoint(cursor.position) - a;

					float pointDistance = (LocalToScreenPoint(b) - cursor.position).Length();
					float lineDistance = Math::Abs(abnp.Dot(ac));
					float proj = abn.Dot(lac);

					if (pointDistance < createPointDistanceThreshold ||
						(lineDistance < createPointDistanceThreshold && proj > 0.0f && proj < abl))
					{
						const Curve::Key& lastKey = keys[keyIdx - 1];

						Vec2F p = abnp*(abnp.Dot(localCursorPos - a)) + localCursorPos;
						newKey.position = p.x;
						newKey.value = p.y;
						newKey.supportsType = Curve::Key::Type::Smooth;

						clickedCurveInfo = info;

						break;
					}
				}

				if (clickedCurveInfo)
					break;

				keyIdx++;
			}

			if (clickedCurveInfo)
				break;
		}

		bool addedKey = false;
		if (clickedCurveInfo)
		{
			int idx = clickedCurveInfo->curve->InsertKey(newKey);

			AddCurveKeyHandles(clickedCurveInfo, idx);
			clickedCurveInfo->UpdateApproximatedPoints();

			mNeedRedraw = true;

			SelectHandle(&clickedCurveInfo->handles[idx]->mainHandle);
			CheckHandlesVisible();

			addedKey = true;
		}
		else if (mCurves.Count() == 1)
		{
			clickedCurveInfo = mCurves[0];
			newKey.position = localCursorPos.x;
			newKey.value = localCursorPos.y;
			int idx = clickedCurveInfo->curve->InsertKey(newKey);

			AddCurveKeyHandles(mCurves[0], idx);
			clickedCurveInfo->UpdateApproximatedPoints();

			mNeedRedraw = true;

			SelectHandle(&clickedCurveInfo->handles[idx]->mainHandle);
			CheckHandlesVisible();

			addedKey = true;
		}

		if (addedKey)
		{
			CurveKeysInfosVec keyInfos;
			keyInfos.Add(CurveKeysInfo());
			keyInfos.Last().curveId = clickedCurveInfo->curveId;
			keyInfos.Last().keys.Add(newKey);

			AddKeysAction* action = mnew AddKeysAction(keyInfos, this);
			DoneAction(action);
		}
	}

	void CurveEditor::SmoothKey(CurveInfo* info, int idx)
	{
		Curve::Key key = info->curve->GetKeyAt(idx);
		Vec2F thisKeyPoint(key.position, key.value);

		const float autoSmoothCoef = 0.5f;

		if (idx == 0)
		{
			Curve::Key nextKey = info->curve->GetKeyAt(Math::Min(idx + 1, info->curve->GetKeys().Count()));

			Vec2F nextKeyPoint(nextKey.position, nextKey.value);
			Vec2F supportVec = (nextKeyPoint - thisKeyPoint)*autoSmoothCoef;

			key.rightSupportPosition = supportVec.x;
			key.rightSupportValue = supportVec.y;
		}
		else if (idx == info->curve->GetKeys().Count() - 1)
		{
			Curve::Key lastKey = info->curve->GetKeyAt(Math::Max(0, idx - 1));

			Vec2F lastKeyPoint(lastKey.position, lastKey.value);
			Vec2F supportVec = (lastKeyPoint - thisKeyPoint)*autoSmoothCoef;

			key.leftSupportPosition = supportVec.x;
			key.leftSupportValue = supportVec.y;
		}
		else
		{
			Curve::Key lastKey = info->curve->GetKeyAt(Math::Max(0, idx - 1));
			Curve::Key nextKey = info->curve->GetKeyAt(Math::Min(idx + 1, info->curve->GetKeys().Count()));

			Vec2F lastKeyPoint(lastKey.position, lastKey.value);
			Vec2F thisKeyPoint(key.position, key.value);
			Vec2F nextKeyPoint(nextKey.position, nextKey.value);

			Vec2F thisToLast = lastKeyPoint - thisKeyPoint;
			Vec2F thisToNext = nextKeyPoint - thisKeyPoint;

			Vec2F lengthNorm = (nextKeyPoint - lastKeyPoint).Normalized();
			float supportLength = Math::Min(thisToNext.Length(), thisToLast.Length())*autoSmoothCoef;
			Vec2F supportVec = Math::CalculateEllipseTangent(lastKeyPoint, thisKeyPoint, nextKeyPoint)*supportLength;

			key.leftSupportPosition = -supportVec.x; key.leftSupportValue = -supportVec.y;
			key.rightSupportPosition = supportVec.x; key.rightSupportValue = supportVec.y;
		}

		info->curve->SetKey(key, idx);

		info->UpdateApproximatedPoints();

		info->handles[idx]->leftSupportHandle.SetPosition(Vec2F(key.position + key.leftSupportPosition,
														  key.value + key.leftSupportValue));

		info->handles[idx]->rightSupportHandle.SetPosition(Vec2F(key.position + key.rightSupportPosition,
														   key.value + key.rightSupportValue));

		if (idx > 0)
		{
			Curve::Key lastKey = info->curve->GetKeyAt(Math::Max(0, idx - 1));

			info->handles[idx - 1]->rightSupportHandle.SetPosition(
				Vec2F(lastKey.position + lastKey.rightSupportPosition, lastKey.value + lastKey.rightSupportValue));
		}

		if (idx < info->curve->GetKeys().Count() - 1)
		{
			Curve::Key nextKey = info->curve->GetKeyAt(Math::Min(idx + 1, info->curve->GetKeys().Count()));

			info->handles[idx + 1]->leftSupportHandle.SetPosition(
				Vec2F(nextKey.position + nextKey.leftSupportPosition, nextKey.value + nextKey.leftSupportValue));
		}

		RecalculateViewArea();

		mNeedRedraw = true;
	}

	void CurveEditor::OnCursorPressed(const Input::Cursor& cursor)
	{
		Focus();

		mSelectingPressedPoint = ScreenToLocalPoint(cursor.position);

		if (!o2Input.IsKeyDown(VK_CONTROL))
		{
			DeselectAll();
			mSupportHandlesGroup.DeselectAll();
		}
	}

	void CurveEditor::OnCursorReleased(const Input::Cursor& cursor)
	{
		for (auto handle : mSelectingHandlesBuf)
		{
			SetHandleSelectedState(handle, false);
			handle->SetSelected(true);
		}

		mSelectingHandlesBuf.Clear();
		UpdateTransformFrame();
		CheckHandlesVisible();
	}

	void CurveEditor::OnCursorStillDown(const Input::Cursor& cursor)
	{
		for (auto handle : mSelectingHandlesBuf)
			SetHandleSelectedState(handle, false);

		mSelectingHandlesBuf.Clear();

		RectF selectionLocalRect(mSelectingPressedPoint, ScreenToLocalPoint(cursor.position));

		for (auto handle : mHandles)
		{
			if (handle->IsEnabled() && selectionLocalRect.IsInside(handle->GetPosition()) &&
				!mSelectedHandles.Contains(handle))
			{
				mSelectingHandlesBuf.Add(handle);
				SetHandleSelectedState(handle, true);
			}
		}

		for (auto handle : mSupportHandlesGroup.GetAllHandles())
		{
			if (handle->IsEnabled() && selectionLocalRect.IsInside(handle->GetPosition()) &&
				!mSupportHandlesGroup.GetSelectedHandles().Contains(handle))
			{
				mSelectingHandlesBuf.Add(handle);
				SetHandleSelectedState(handle, true);
			}
		}
	}

	void CurveEditor::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{
		if (cursor.delta.Length() > 0.1f)
			mIsViewScrolling = true;

		ScrollView::OnCursorRightMouseStayDown(cursor);
	}

	void CurveEditor::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		Focus();

		if (!mIsViewScrolling)
		{
			Curve::Key::Type supportsType = Curve::Key::Type::Free;
			bool supportsDifferent = false;
			bool first = true;
			bool someSelected = false;

			for (auto curve : mCurves)
			{
				for (auto handles : curve->handles)
				{
					if (!handles->mainHandle.IsSelected())
						continue;

					someSelected = true;

					if (first)
					{
						first = false;
						supportsType = curve->curve->GetKeyAt(handles->curveKeyIdx).supportsType;
					}
					else if (supportsType != curve->curve->GetKeyAt(handles->curveKeyIdx).supportsType)
					{
						supportsDifferent = true;
						break;
					}
				}

				if (supportsDifferent)
					break;
			}

			if (supportsDifferent || !someSelected)
			{
				for (int i = 0; i < 5; i++)
					mContextMenu->SetItemChecked(i, false);
			}
			else
			{
				for (int i = 0; i < 5; i++)
					mContextMenu->SetItemChecked(i, i == (int)supportsType);
			}

			mContextMenu->Show();
		}

		mIsViewScrolling = false;

		ScrollView::OnCursorRightMouseReleased(cursor);
	}

	void CurveEditor::CheckHandlesVisible()
	{
		for (auto info : mCurves)
		{
			for (auto handles : info->handles)
			{
				handles->leftSupportHandle.enabled = (handles->mainHandle.IsSelected() ||
													  handles->leftSupportHandle.IsSelected() ||
													  handles->rightSupportHandle.IsSelected()) && handles->curveKeyIdx > 0;

				handles->rightSupportHandle.enabled = (handles->mainHandle.IsSelected() ||
													   handles->leftSupportHandle.IsSelected() ||
													   handles->rightSupportHandle.IsSelected()) && handles->curveKeyIdx < info->handles.Count() - 1;
			}
		}
	}

	void CurveEditor::UpdateTransformFrame()
	{
		mTransformFrameVisible = IsTransformFrameVisible();

		if (!mTransformFrameVisible)
			return;

		RectF aabb(mSelectedHandles[0]->GetPosition(), mSelectedHandles[0]->GetPosition());

		for (auto handle : mSelectedHandles)
		{
			aabb.left   = Math::Min(handle->GetPosition().x, aabb.left);
			aabb.right  = Math::Max(handle->GetPosition().x, aabb.right);
			aabb.top    = Math::Max(handle->GetPosition().y, aabb.top);
			aabb.bottom = Math::Min(handle->GetPosition().y, aabb.bottom);
		}

		mTransformFrameBasis = Basis(aabb.LeftBottom(), Vec2F::Right()*aabb.Width(), Vec2F::Up()*aabb.Height());
	}

	bool CurveEditor::IsTransformFrameVisible() const
	{
		int selectedMainHandles = 0;
		for (auto handle : mSelectedHandles)
		{
			if (!mSupportHandles.Contains(handle))
				selectedMainHandles++;
		}

		return selectedMainHandles > 1;
	}

	void CurveEditor::OnHandleCursorReleased(DragHandle* handle, const Input::Cursor& cursor)
	{
		SelectableDragHandlesGroup::OnHandleCursorReleased(handle, cursor);
		UpdateTransformFrame();
		CheckHandlesVisible();
	}

	void CurveEditor::OnHandleBeganDragging(DragHandle* handle)
	{
		if (mSupportHandles.Contains(handle))
			return;

		SelectableDragHandlesGroup::OnHandleBeganDragging(handle);
	}

	void CurveEditor::OnHandleMoved(DragHandle* handle, const Vec2F& cursorPos)
	{
		if (mSupportHandles.Contains(handle))
		{
			if (!handle->IsSelected())
				return;

			for (auto handle : mSupportHandles)
			{
				if (!handle->IsSelected())
					continue;

				handle->SetDragPosition(handle->screenToLocalTransformFunc(cursorPos) + handle->GetDraggingOffset());
				handle->onChangedPos(handle->GetPosition());
			}

			return;
		}

		SelectableDragHandlesGroup::OnHandleMoved(handle, cursorPos);
	}

	void CurveEditor::OnHandleCompletedChange(DragHandle* handle)
	{
		OnTransformCompleted();
	}

	void CurveEditor::SetSelectedKeysSupportsType(Curve::Key::Type type)
	{
		for (auto info : mCurves)
		{
			auto keys = info->curve->GetKeys();

			for (auto handles : info->handles)
			{
				if (handles->mainHandle.IsSelected())
				{
					Curve::Key& key = keys[handles->curveKeyIdx];
					key.supportsType = type;

					switch (type)
					{
						case Curve::Key::Type::Flat:
						key.leftSupportValue = 0;
						key.rightSupportValue = 0;
						break;

						case Curve::Key::Type::Free:
						key.rightSupportPosition = -key.leftSupportPosition;
						key.rightSupportValue = -key.leftSupportValue;
						break;
					}
				}
			}

			info->BeginCurveManualChange();

			info->curve->SetKeys(keys);
			info->UpdateHandles();
			info->UpdateApproximatedPoints();

			info->CompleteCurveManualChange();
		}

		CheckHandlesVisible();
		RecalculateViewArea();
		mNeedRedraw = true;
	}

	void CurveEditor::OnTransformFrameTransformed(const Basis& basis)
	{
		Vec2F border(10, 10);
		Basis localBasis(ScreenToLocalPoint(basis.origin + border),
			(basis.xv - Vec2F(border.x*2.0f, 0))*mViewCamera.GetScale(),
						 (basis.yv - Vec2F(0, border.y*2.0f))*mViewCamera.GetScale());

		Basis lastTransformBasis = mTransformFrameBasis;
		Basis delta = mTransformFrameBasis.Inverted()*localBasis;

		if (delta.origin.Length() > 0.01f || delta.xv != Vec2F(1, 0) || delta.yv != Vec2F(0, 1))
		{
			for (auto handle : mSelectedHandles)
			{
				handle->position = handle->position*delta;
				handle->onChangedPos(handle->GetPosition());
			}

			if (o2Input.IsKeyDown(VK_CONTROL))
			{
				float right = lastTransformBasis.origin.x + lastTransformBasis.xv.x;
				float left = lastTransformBasis.origin.x;

				float rightOffset = localBasis.origin.x + localBasis.xv.x - right;
				float leftOffset = localBasis.origin.x - left;

				bool rightChanged = !Math::Equals(rightOffset, 0.0f) && rightOffset > FLT_EPSILON;
				bool leftChanged = !Math::Equals(leftOffset, 0.0f) && leftOffset < FLT_EPSILON;

				for (auto handle : mHandles)
				{
					if (mSelectedHandles.Contains(handle))
						continue;

					if (handle->GetPosition().x >= right && rightChanged)
					{
						handle->position += Vec2F(rightOffset, 0);
						handle->onChangedPos(handle->GetPosition());
					}

					if (handle->GetPosition().x <= left && leftChanged)
					{
						handle->position += Vec2F(leftOffset, 0);
						handle->onChangedPos(handle->GetPosition());
					}
				}
			}

			UpdateTransformFrame();
		}
	}

	void CurveEditor::OnTransformBegin()
	{
		mBeforeTransformKeys.Clear();

		CurveKeysInfo keysInfo;
		for (auto curveInfo : mCurves)
		{
			keysInfo.keys.Clear();
			keysInfo.selectedHandles.Clear();

			for (auto handles : curveInfo->handles)
			{
				if (handles->IsSomeHandleSelected())
				{
					keysInfo.keys.Add(curveInfo->curve->GetKeyAt(handles->curveKeyIdx));

					keysInfo.selectedHandles.Add(SelectedHandlesInfo());
					keysInfo.selectedHandles.Last().index = handles->curveKeyIdx;
					keysInfo.selectedHandles.Last().mainHandle = handles->mainHandle.IsSelected();
					keysInfo.selectedHandles.Last().leftSupportHandle = handles->leftSupportHandle.IsSelected();
					keysInfo.selectedHandles.Last().rightSupportHandle = handles->rightSupportHandle.IsSelected();
				}
			}

			if (!keysInfo.keys.IsEmpty())
			{
				keysInfo.curveId = curveInfo->curveId;
				mBeforeTransformKeys.Add(keysInfo);
			}
		}
	}

	void CurveEditor::OnTransformCompleted()
	{
		bool changed = false;
		for (auto& keysInfo : mBeforeTransformKeys)
		{
			CurveInfo* curveInfo = mCurves.FindMatch([&](CurveInfo* x) { return x->curveId == keysInfo.curveId; });
			if (!curveInfo)
				continue;

			for (auto& key : curveInfo->curve->GetKeys())
			{
				if (!keysInfo.keys.Contains(key))
				{
					changed = true;
					break;
				}
			}

			if (changed)
				break;
		}

		if (changed)
		{
			KeysChangeAction::KeysInfosVec actionKeysInfos;
			for (auto& keysInfo : mBeforeTransformKeys)
			{
				CurveInfo* curveInfo = mCurves.FindMatch([&](CurveInfo* x) { return x->curveId == keysInfo.curveId; });
				if (!curveInfo)
					continue;

				actionKeysInfos.Add(KeysChangeAction::KeysInfo());
				auto& actionCurveKeysInfo = actionKeysInfos.Last();

				actionCurveKeysInfo.curveId = keysInfo.curveId;
				actionCurveKeysInfo.beforeKeys = keysInfo.keys;
				actionCurveKeysInfo.selectedHandles = keysInfo.selectedHandles;

				for (auto handles : curveInfo->handles)
				{
					if (handles->IsSomeHandleSelected())
						actionCurveKeysInfo.afterKeys.Add(curveInfo->curve->GetKeyAt(handles->curveKeyIdx));
				}
			}

			KeysChangeAction* action = mnew KeysChangeAction(actionKeysInfos, this);
			DoneAction(action);
		}
	}

	void CurveEditor::DoneAction(IAction* action)
	{
		if (actionsListDelegate) {
			actionsListDelegate->DoneAction(action);
			return;
		}

		mActionsList.DoneAction(action);
	}

	void CurveEditor::OnEditPressed()
	{
		Curve::Key key;
		CurveInfo* curveInfo = nullptr;
		int keyIdx = 0;

		for (auto info : mCurves)
		{
			for (auto handles : info->handles)
			{
				if (handles->mainHandle.IsSelected())
				{
					curveInfo = info;
					keyIdx = handles->curveKeyIdx;
					key = info->curve->GetKeys()[keyIdx];

					break;
				}
			}

			if (curveInfo)
				break;
		}

		if (!curveInfo)
			return;

		KeyEditDlg::Show(key, [=](const Curve::Key& newKey) {
			curveInfo->curve->SetKey(newKey, keyIdx);
		});
	}

	void CurveEditor::OnAutoSmoothChecked(bool checked)
	{
		mContextMenu->SetItemChecked(0, true);  // auto smooth
		mContextMenu->SetItemChecked(1, false); // flat
		mContextMenu->SetItemChecked(2, false); // free
		mContextMenu->SetItemChecked(3, false); // broken
		mContextMenu->SetItemChecked(4, false); // discrete

		SetSelectedKeysSupportsType(Curve::Key::Type::Smooth);
	}

	void CurveEditor::OnFlatChecked(bool checked)
	{
		mContextMenu->SetItemChecked(0, false); // auto smooth
		mContextMenu->SetItemChecked(1, true);  // flat
		mContextMenu->SetItemChecked(2, false); // free
		mContextMenu->SetItemChecked(3, false); // broken
		mContextMenu->SetItemChecked(4, false); // discrete

		SetSelectedKeysSupportsType(Curve::Key::Type::Flat);
	}

	void CurveEditor::OnFreeChecked(bool checked)
	{
		mContextMenu->SetItemChecked(0, false);  // auto smooth
		mContextMenu->SetItemChecked(1, false); // flat
		mContextMenu->SetItemChecked(2, true); // free
		mContextMenu->SetItemChecked(3, false); // broken
		mContextMenu->SetItemChecked(4, false); // discrete

		SetSelectedKeysSupportsType(Curve::Key::Type::Free);
	}

	void CurveEditor::OnBrokenChecked(bool checked)
	{
		mContextMenu->SetItemChecked(0, false);  // auto smooth
		mContextMenu->SetItemChecked(1, false); // flat
		mContextMenu->SetItemChecked(2, false); // free
		mContextMenu->SetItemChecked(3, true); // broken
		mContextMenu->SetItemChecked(4, false); // discrete

		SetSelectedKeysSupportsType(Curve::Key::Type::Broken);
	}

	void CurveEditor::OnDiscreteChecked(bool checked)
	{
		mContextMenu->SetItemChecked(0, false);  // auto smooth
		mContextMenu->SetItemChecked(1, false); // flat
		mContextMenu->SetItemChecked(2, false); // free
		mContextMenu->SetItemChecked(3, false); // broken
		mContextMenu->SetItemChecked(4, true); // discrete

		SetSelectedKeysSupportsType(Curve::Key::Type::Discrete);
	}

	void CurveEditor::OnCopyPressed()
	{
		if (!IsFocused())
			return;

		CurveCopyInfosVec copyKeys;

		for (auto curve : mCurves)
		{
			CurveCopyInfo* copyInfo = mnew CurveCopyInfo();
			copyInfo->curveId = curve->curveId;

			for (auto handles : curve->handles)
			{
				if (!handles->mainHandle.IsSelected())
					continue;

				copyInfo->keys.Add(curve->curve->GetKeyAt(handles->curveKeyIdx));
			}

			if (copyInfo->keys.IsEmpty())
				delete copyInfo;
			else
				copyKeys.Add(copyInfo);
		}

		DataNode copyData;
		copyData = copyKeys;
		String copyDataStr = copyData.SaveAsWString();

		Clipboard::SetText(copyDataStr);
	}

	void CurveEditor::OnCutPressed()
	{
		if (!IsFocused())
			return;

		OnCopyPressed();
		OnDeletePressed();
	}

	void CurveEditor::OnPastePressed()
	{
		if (!IsFocused())
			return;

		float insertPos = ScreenToLocalPoint(o2Input.cursorPos).x;
		DataNode data;
		data.LoadFromData(Clipboard::GetText());

		CurveCopyInfosVec copyKeys;
		copyKeys = data;

		CurveKeysInfosVec keyInfos;

		for (auto curve : copyKeys)
		{
			CurveInfo* curveInfo = mCurves.FindMatch([=](const CurveInfo* x) { return x->curveId == curve->curveId; });

			if (curveInfo == nullptr)
			{
				if (copyKeys.Count() == 1 && mCurves.Count() == 1)
					curveInfo = mCurves[0];
			}

			if (!curveInfo)
				continue;

			curveInfo->BeginCurveManualChange();

			keyInfos.Add(CurveKeysInfo());
			keyInfos.Last().curveId = curveInfo->curveId;

			float positionDelta = insertPos - curve->keys[0].position;
			for (auto& key : curve->keys)
			{
				key.position += positionDelta;
				key.uid = Math::Random();

				int position = curveInfo->curve->InsertKey(key);
				AddCurveKeyHandles(curveInfo, position);

				keyInfos.Last().keys.Add(key);
			}

			delete curve;

			curveInfo->UpdateApproximatedPoints();
			curveInfo->UpdateHandles();
			mNeedRedraw = true;

			curveInfo->CompleteCurveManualChange();
		}

		if (!keyInfos.IsEmpty())
		{
			AddKeysAction* action = mnew AddKeysAction(keyInfos, this);
			DoneAction(action);
		}
	}

	void CurveEditor::OnDeletePressed()
	{
		if (!IsFocused())
			return;

		CurveKeysInfosVec keyInfos;

		for (auto curveInfo : mCurves)
		{
			curveInfo->BeginCurveManualChange();

			CurveCopyInfo* copyInfo = mnew CurveCopyInfo();
			copyInfo->curveId = curveInfo->curveId;

			Vector<int> removingIdxs;
			for (auto handles : curveInfo->handles)
			{
				if (!handles->mainHandle.IsSelected())
					continue;

				removingIdxs.Add(handles->curveKeyIdx);
			}

			if (!removingIdxs.IsEmpty())
			{
				keyInfos.Add(CurveKeysInfo());
				keyInfos.Last().curveId = curveInfo->curveId;
			}

			for (int i = removingIdxs.Count() - 1; i >= 0; i--)
			{
				keyInfos.Last().keys.Add(curveInfo->curve->GetKeyAt(removingIdxs[i]));

				curveInfo->curve->RemoveKeyAt(removingIdxs[i]);
				RemoveCurveKeyHandles(curveInfo, removingIdxs[i]);
			}

			curveInfo->UpdateApproximatedPoints();
			curveInfo->UpdateHandles();
			mNeedRedraw = true;

			curveInfo->CompleteCurveManualChange();
		}

		mSelectedHandles.Clear();
		mSupportHandlesGroup.DeselectAll();
		UpdateTransformFrame();

		if (!keyInfos.IsEmpty())
		{
			DeleteKeysAction* action = mnew DeleteKeysAction(keyInfos, this);
			DoneAction(action);
		}
	}

	void CurveEditor::OnInsertPressed()
	{
		if (!IsFocused())
			return;

		float insertPos = ScreenToLocalPoint(o2Input.cursorPos).x;
		DataNode data;
		data.LoadFromData(Clipboard::GetText());

		CurveCopyInfosVec copyKeys;
		copyKeys = data;

		for (auto curve : copyKeys)
		{
			CurveInfo* curveInfo = mCurves.FindMatch([=](const CurveInfo* x) { return x->curveId == curve->curveId; });

			if (curveInfo == nullptr)
			{
				if (copyKeys.Count() == 1 && mCurves.Count() == 1)
					curveInfo = mCurves[0];
			}

			if (!curveInfo)
				continue;

			float length = curve->keys.Last().position - curve->keys[0].position;
			curveInfo->curve->MoveKeysFrom(insertPos, length);

			float positionDelta = insertPos - curve->keys[0].position;
			for (auto& key : curve->keys)
			{
				key.position += positionDelta;

				int position = curveInfo->curve->InsertKey(key);
				AddCurveKeyHandles(curveInfo, position);
			}

			delete curve;
		}
	}

	void CurveEditor::OnUndoPressed()
	{
		if (actionsListDelegate)
			actionsListDelegate->UndoAction();
		else
			mActionsList.UndoAction();
	}

	void CurveEditor::OnRedoPressed()
	{
		if (actionsListDelegate)
			actionsListDelegate->RedoAction();
		else
			mActionsList.RedoAction();
	}

	CurveEditor::CurveInfo::CurveInfo()
	{}

	CurveEditor::CurveInfo::~CurveInfo()
	{
		for (auto x : handles)
		{
			x->mainHandle.SetSelectionGroup(nullptr);
			x->leftSupportHandle.SetSelectionGroup(nullptr);
			x->rightSupportHandle.SetSelectionGroup(nullptr);

			delete x;
		}

		if (curve)
			curve->onKeysChanged -= MakeSubscription(this, &CurveInfo::OnCurveChanged, []() {});
	}

	void CurveEditor::CurveInfo::UpdateHandles()
	{
		for (int i = 0; i < handles.Count(); i++)
		{
			Curve::Key key = curve->GetKeyAt(i);
			handles[i]->mainHandle.position = Vec2F(key.position, key.value);

			if (i > 0)
			{
				handles[i]->leftSupportHandle.position = Vec2F(key.position + key.leftSupportPosition,
															   key.value + key.leftSupportValue);

				handles[i]->leftSupportHandle.enabled = handles[i]->leftSupportHandle.IsSelected() ||
					handles[i]->mainHandle.IsSelected();
			}
			else handles[i]->leftSupportHandle.enabled = false;

			if (i < handles.Count() - 1)
			{
				handles[i]->rightSupportHandle.position = Vec2F(key.position + key.rightSupportPosition,
																key.value + key.rightSupportValue);

				handles[i]->rightSupportHandle.enabled = handles[i]->rightSupportHandle.IsSelected() ||
					handles[i]->mainHandle.IsSelected();
			}
			else handles[i]->rightSupportHandle.enabled = false;
		}
	}

	void CurveEditor::CurveInfo::UpdateApproximatedPoints()
	{
		approximatedPoints.Clear();

		auto keys = curve->GetKeys();
		for (int i = 1; i < keys.Count(); i++)
		{
			const Vec2F* keyPoints = keys[i].GetApproximatedPoints();
			for (int j = 0; j < keys[i].GetApproximatedPointsCount(); j++)
				approximatedPoints.Add(keyPoints[j]);
		}
	}

	void CurveEditor::CurveInfo::OnCurveChanged()
	{
		if (disableChangesHandling)
			return;

		if (handles.Count() != curve->GetKeys().Count())
		{
			Vector<UInt64> selectedMain, selectedLeft, selectedRight;

			for (auto handle : handles)
			{
				if (handle->leftSupportHandle.IsSelected())
				{
					selectedLeft.Add(handle->curveKeyUid);
					handle->leftSupportHandle.SetSelectionGroup(nullptr);
				}

				curveEditor->mSupportHandles.Remove(&handle->leftSupportHandle);				

				if (handle->rightSupportHandle.IsSelected())
				{
					selectedRight.Add(handle->curveKeyUid);
					handle->rightSupportHandle.SetSelectionGroup(nullptr);
				}

				curveEditor->mSupportHandles.Remove(&handle->rightSupportHandle);

				if (handle->mainHandle.IsSelected())
				{
					selectedMain.Add(handle->curveKeyUid);
					handle->mainHandle.SetSelected(false);
				}

				curveEditor->mHandles.Remove(&handle->mainHandle);

				delete handle;
			}

			handles.clear();

			for (int i = 0; i < curve->GetKeys().Count(); i++)
				curveEditor->AddCurveKeyHandles(this, i);

			for (auto handle : handles)
			{
				if (selectedMain.Contains(handle->curveKeyUid))
					handle->mainHandle.Select();

				if (selectedLeft.Contains(handle->curveKeyUid))
					handle->leftSupportHandle.Select();

				if (selectedRight.Contains(handle->curveKeyUid))
					handle->rightSupportHandle.Select();
			}
		}

		UpdateApproximatedPoints();
		UpdateHandles();
	}

	void CurveEditor::CurveInfo::BeginCurveManualChange()
	{
		disableChangesHandling = true;
	}

	void CurveEditor::CurveInfo::CompleteCurveManualChange()
	{
		disableChangesHandling = false;
	}

	CurveEditor::RangeInfo::RangeInfo()
	{

	}

	CurveEditor::RangeInfo::~RangeInfo()
	{

	}

	void CurveEditor::RangeInfo::UpdateMesh()
	{

	}

	CurveEditor::KeyHandles::KeyHandles(const DragHandle& mainSample, const DragHandle& supportSample,
										  CurveEditor* editor):
		mainHandle(mainSample), leftSupportHandle(supportSample), rightSupportHandle(supportSample), curveEditor(editor)
	{}

	void CurveEditor::KeyHandles::Draw(const RectF& camRect)
	{
		bool selected = true;

		Vec2F mainHandleScreenPos = curveEditor->LocalToScreenPoint(mainHandle.GetPosition());

		if (leftSupportHandle.enabled)
		{
			Vec2F leftSupportHandleScreenPos = curveEditor->LocalToScreenPoint(leftSupportHandle.GetPosition());
			o2Render.DrawAALine(mainHandleScreenPos, leftSupportHandleScreenPos, curveEditor->mGridColor);
		}

		if (rightSupportHandle.enabled)
		{
			Vec2F rightSupportHandleScreenPos = curveEditor->LocalToScreenPoint(rightSupportHandle.GetPosition());
			o2Render.DrawAALine(mainHandleScreenPos, rightSupportHandleScreenPos, curveEditor->mGridColor);
		}

		if (camRect.IsInside(mainHandle.GetPosition()))
			mainHandle.Draw();

		if (camRect.IsInside(leftSupportHandle.GetPosition()))
			leftSupportHandle.Draw();

		if (camRect.IsInside(rightSupportHandle.GetPosition()))
			rightSupportHandle.Draw();
	}

	bool CurveEditor::KeyHandles::IsSomeHandleSelected() const
	{
		return mainHandle.IsSelected() || leftSupportHandle.IsSelected() || rightSupportHandle.IsSelected();
	}

	bool CurveEditor::CurveKeysInfo::operator==(const CurveKeysInfo& other) const
	{
		return curveId == other.curveId && keys == other.keys;
	}

	bool CurveEditor::SelectedHandlesInfo::operator==(const SelectedHandlesInfo& other) const
	{
		return index == other.index && mainHandle == other.mainHandle && leftSupportHandle == other.leftSupportHandle &&
			rightSupportHandle == other.rightSupportHandle;
	}

}

DECLARE_CLASS(Editor::CurveEditor);

DECLARE_CLASS(Editor::CurveEditor::CurveCopyInfo);
