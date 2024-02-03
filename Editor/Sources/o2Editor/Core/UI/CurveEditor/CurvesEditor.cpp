#include "o2Editor/stdafx.h"
#include "CurvesEditor.h"

#include "o2/Application/Application.h"
#include "o2/Render/Render.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/HorizontalScrollBar.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Scene/UI/Widgets/VerticalScrollBar.h"
#include "o2/Scene/UI/Widgets/Window.h"
#include "o2/Utils/System/Clipboard.h"
#include "o2Editor/Core/Dialogs/KeyEditDlg.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/UI/CurveEditor/CurveActions.h"
#include "o2Editor/Core/UIRoot.h"

namespace Editor
{
	CurvesEditor::CurvesEditor():
		FrameScrollView()
	{
		mReady = false;

		mSelectionSprite = Ref<Sprite>::make();
		mTextFont = Ref<Font>::make("stdFont.ttf");
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

		mHandleSamplesStubInfo.editor = this;

		mReady = true;
	}

	CurvesEditor::CurvesEditor(const CurvesEditor& other):
		FrameScrollView(other), mSelectionSprite(other.mSelectionSprite->CloneAs<Sprite>()), mTextFont(other.mTextFont)
	{
		mReady = false;

		mHandleSamplesStubInfo.editor = this;

		mContextMenu = FindChildByType<ContextMenu>().release();
		if (mContextMenu)
			delete mContextMenu;

		InitializeTextDrawables();
		InitializeContextMenu();
		RetargetStatesAnimations();

		mReady = true;
	}

	CurvesEditor::~CurvesEditor()
	{
		for (auto curve : mCurves)
			delete curve;

		if (mSelectionSprite)
			delete mSelectionSprite.get();

		if (mTextLeft)
			delete mTextLeft;

		if (mTextRight)
			delete mTextRight;

		if (mTextTop)
			delete mTextTop;

		if (mTextBottom)
			delete mTextBottom;
	}

	Editor::CurvesEditor& CurvesEditor::operator=(const CurvesEditor& other)
	{
		FrameScrollView::operator=(other);

		mReady = false;

		delete mSelectionSprite.get();
		delete mTextLeft;
		delete mTextRight;
		delete mTextTop;
		delete mTextBottom;

		mSelectionSprite = other.mSelectionSprite->CloneAs<Sprite>();
		mTextFont = other.mTextFont;

		mMainHandleSample = other.mMainHandleSample;
		mSupportHandleSample = other.mSupportHandleSample;

		InitializeTextDrawables();
		RetargetStatesAnimations();

		mReady = true;

		return *this;
	}

	void CurvesEditor::Draw()
	{
		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		FrameScrollView::Draw();

		o2Render.EnableScissorTest(layout->GetWorldRect());

		DrawTransformFrame();
		DrawHandles();
		DrawSelection();

		o2Render.DisableScissorTest();
	}

	void CurvesEditor::Update(float dt)
	{
		FrameScrollView::Update(dt);

		if (mReady && mResEnabledInHierarchy && !mIsClipped && mNeedAdjustView)
		{
			mNeedAdjustView = false;
			mViewCameraTargetScale = mAvailableArea.Size()/mViewCamera.GetSize();
			mViewCamera.center = mAvailableArea.Center();
			mViewCameraTargetPos = mViewCamera.position;
		}
	}

	Map<String, Ref<Curve>> CurvesEditor::GetCurves() const
	{
		Map<String, Ref<Curve>> res;
		for (auto curveInfo : mCurves)
			res.Add(curveInfo->curveId, curveInfo->curve);

		return res;
	}

	void CurvesEditor::AddCurve(const String& id, Ref<Curve> curve, const Color4& color /*= Color4(44, 62, 80)*/)
	{
		UpdateSelfTransform();

		auto info = new CurveInfo();
		info->editor = this;
		info->curveId = id;
		info->curve = curve.release();

		info->AdjustScale();

		info->curve->onKeysChanged += MakeSubscription(info, &CurveInfo::OnCurveChanged, [=]() { info->curve = nullptr; });

		if (color == Color4(44, 62, 80) && !mCurves.IsEmpty())
			info->color = Color4::SomeColor(mCurves.Count());
		else
			info->color = color;

		for (int i = 0; i < curve->GetKeys().Count(); i++)
			AddCurveKeyHandles(info.get(), i);

		mCurves.Add(info.get());
		RecalculateViewArea();

		if (mCurves.Count() == 1)
			mNeedAdjustView = true;
	}

	void CurvesEditor::RemoveCurve(Ref<Curve> curve)
	{
		for (auto info : mCurves)
		{
			if (info->curve.get() == curve.get())
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

	void CurvesEditor::RemoveCurve(const String& id)
	{
		Curve* curve = nullptr;
		for (auto info : mCurves)
		{
			if (info->curveId == id)
			{
				curve = info->curve.get();
				break;
			}
		}

		if (curve)
			RemoveCurve(curve);
	}

	void CurvesEditor::InitializeTextDrawables()
	{
		int fontSize = 8;

		if (!mTextLeft)
		{
			mTextLeft = Ref<Label>::make();
			mTextRight = Ref<Label>::make();
			mTextTop = Ref<Label>::make();
			mTextBottom = Ref<Label>::make();

			mTextBottom->SetZIndex(1000);
			mTextRight->SetZIndex(1001);

			mTextLeft->SetFontColor(Color4::Black());
			mTextRight->SetFontColor(Color4::Black());
			mTextTop->SetFontColor(Color4::Black());
			mTextBottom->SetFontColor(Color4::Black());

			mTextLeft->SetFontSize(fontSize);
			mTextRight->SetFontSize(fontSize);
			mTextTop->SetFontSize(fontSize);
			mTextBottom->SetFontSize(fontSize);

			mTextLeft->SetTextAlign(Label::IAlignment::Right);
			mTextRight->SetTextAlign(Label::IAlignment::Left);
			mTextBottom->SetTextAlign(Label::IAlignment::Center);

			mTextRight->SetText("0");
			mTextTop->SetText("0");
		}

		o2UIManager.GetGlobalState()->widgetLayer->SetRenderTextCallback(THIS_FUNC(RenderTextCallback));
	}

	void CurvesEditor::InitializeContextMenu()
	{
		if (mContextMenu)
		{
			mContextMenu->onAddCurve = std::function<void(CurvesEditor*)>{ THIS_FUNC(OnAddCurve) };
			mContextMenu->onExpand = std::function<void(CurvesEditor*)>{ THIS_FUNC(OnExpandMenu) };
			mContextMenu->onCollapse = std::function<void(CurvesEditor*)>{ THIS_FUNC(OnCollapseMenu) };
			mContextMenu->SetBehavior(this);
			mContextMenu->SetVisibleCallback(THIS_FUNC(IsMenuVisible));
			mContextMenu->SetParent(this);
		}
	}

	void CurvesEditor::RenderTextCallback(const Label& label, Rect rect, const std::shared_ptr<o2Render::Font>& font, unsigned color, Label::IAlignment align)
	{
		m2DRenderer.BeginText(font, color, align);
		m2DRenderer.DrawText(rect.position, rect.GetSize(), label.GetText());
		m2DRenderer.EndText();
	}

	void CurvesEditor::OnAddCurve()
	{
		auto editor = mmake<CurvesEditor>();
		editor->SetTextFont(mTextFont);
		editor->SetVisibleCallback(THIS_FUNC(IsModalVisible));
		editor->SetModal(new UIStates::EditCurveNameModal());

		CurveActions::CreateNewCurve(editor.get());
	}

	void CurvesEditor::OnExpandMenu()
	{
		SendEvent(new EventInHierarchy("ExpandContextMenu"));
	}

	void CurvesEditor::OnCollapseMenu()
	{
		SendEvent(new EventInHierarchy("CollapseContextMenu"));
	}

	void CurvesEditor::OnCurveChanged(CurveInfo* info)
	{
		if (!info->curve)
			return;

		for (auto handle : info->handles)
		{
			handle->mainHandle.SetSelected(false);
			handle->leftSupportHandle.SetSelected(false);
			handle->rightSupportHandle.SetSelected(false);
		}

		for (int i = 0; i < info->curve->GetKeys().Count(); i++)
		{
			AddCurveKeyHandles(info, i);
		}
	}

	void CurvesEditor::OnTransformFrameTransformed()
	{
		o2Assert(mCurves.Count() > 0);

		if (!mTransformFrame.HasTransform())
			return;

		auto pos = mTransformFrame.GetTransformedPos() - mTransformFrame.GetTransformedSize()*0.5f + mTransformFrame.GetTransformedSize()*0.5f;
		auto delta = mTransformFrame.GetTransformedSize() - mTransformFrame.GetOriginalSize();

		for (auto curveInfo : mCurves)
		{
			if (delta.x != 0.0f || delta.y != 0.0f)
				curveInfo->Move(point_cast<Vector2>(delta));

			if (pos != mTransformFrame.GetTransformedPos())
				curveInfo->MoveKeys(pos - mTransformFrame.GetTransformedPos());
		}

		mRecalculateMainKeyPoint = true;
	}

	void CurvesEditor::OnTransformBegin()
	{
		mRecalculateMainKeyPoint = false;
	}

	void CurvesEditor::OnTransformCompleted()
	{
		if (mRecalculateMainKeyPoint)
		{
			RecalculateMainKeyPoint();
			RecalculateSupportHandles();
		}
	}Replace raw pointers with Ref<> except void* pointers. Use const Ref<>& for function arguments with pointer types and in overridden methods. Remove the default nullptr value for class members with type Ref<>. If the class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<> just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

```cpp
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
Ref<T> DynamicCast(const Ref<T>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class CurvesEditor : public FrameScrollView
{
public:
    void AddCurve(Curve* curve, const String& id);

    void RemoveCurve(Curve* curve);

    void RemoveAllCurves();

    void AddCurvesRange(Curve* curveA, Curve* curveB, const Color4& color);

    void AddCurvesRange(const String& idA, const String& idB, const Color4& color);

    void SetCurveColor(Curve* curve, const Color4& color);

    void RemoveCurvesRange(Curve* curveA, Curve* curveB);

    void RemoveCurvesRange(const String& idA, const String& idB);

    void UpdateSelfTransform() override;

    ContextMenu* GetContextMenu() const;

    void SetSelectionSpriteImage(const Ref<ImageAsset>& image);

    void SetTextFont(const Ref<Font>& font);

    void SetTextBorder(const BorderF& border);

    void SetMainHandleImages(const Ref<ImageAsset>& regular, const Ref<ImageAsset>& hover,
                             const Ref<ImageAsset>& pressed, const Ref<ImageAsset>& selected);

    void SetSupportHandleImages(const Ref<ImageAsset>& regular, const Ref<ImageAsset>& hover,
                                const Ref<ImageAsset>& pressed, const Ref<ImageAsset>& selected);

    void SetAdjustCurvesScale(bool enable);

    void OnEnabled() override;

    void OnDisabled() override;

    void OnScrolled(float scroll) override;

    Curve* FindCurve(const String& id);

private:
    void InitializeContextMenu();

    void InitializeTextDrawables();

private:
    std::vector<Ref<CurveInfo>> mCurves;
    Ref<ContextMenu> mContextMenu;
    Ref<Sprite> mSelectionSprite;
    Ref<Font> mTextFont;
    BorderF mTextBorder;
    CurveHandle mMainHandleSample;
    CurveHandle mSupportHandleSample;
    bool mAdjustCurvesScale;
};
```Ref<Text> mTextLeft = mmake<Text>(mTextFont);
mTextLeft->SetHeight(10);
mTextLeft->SetHorAlign(HorAlign::Left);
mTextLeft->SetVerAlign(VerAlign::Bottom);
mTextLeft->color = Color4(96, 125, 139);

Ref<Text> mTextRight = mmake<Text>(mTextFont);
mTextRight->SetHeight(10);
mTextRight->SetHorAlign(HorAlign::Right);
mTextRight->SetVerAlign(VerAlign::Bottom);
mTextRight->color = Color4(96, 125, 139);

Ref<Text> mTextTop = mmake<Text>(mTextFont);
mTextTop->SetHeight(10);
mTextTop->SetHorAlign(HorAlign::Left);
mTextTop->SetVerAlign(VerAlign::Bottom);
mTextTop->SetAngleDegrees(-90.0f);
mTextTop->color = Color4(96, 125, 139);

Ref<Text> mTextBottom = mmake<Text>(mTextFont);
mTextBottom->SetHeight(10);
mTextBottom->SetHorAlign(HorAlign::Right);
mTextBottom->SetVerAlign(VerAlign::Bottom);
mTextBottom->SetAngleDegrees(-90.0f);
mTextBottom->color = Color4(96, 125, 139);


void CurvesEditor::RecalculateViewArea()
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

	for (const auto& curve : mCurves)
	{
		for (const auto& p : curve->approximatedPoints)
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

void CurvesEditor::RedrawContent()
{
	DrawGrid();
	DrawCurves();
}

void CurvesEditor::DrawGrid()
{
	// Screen -> Local -> Curve View

	Vec2F gridScale(1, 1);
	Vec2F gridOffset(0, 0);

	bool allSameCurve = !mSelectedHandles.IsEmpty() && mSelectedHandles.All([&](Ref<DragHandle> x) {
		return ((Ref<CurveHandle>)x)->curveInfo == ((Ref<CurveHandle>)mSelectedHandles[0])->curveInfo;
	});

	if (allSameCurve)
	{
		Ref<CurveHandle> curvehandle = (Ref<CurveHandle>)mSelectedHandles[0];
		gridScale = curvehandle->curveInfo->viewScale;
		gridOffset = curvehandle->curveInfo->viewOffset;
	}

	bool unknownScale = mAdjustCurvesScale && !allSameCurve;

	if (unknownScale && mCurves.Count() == 1) {
		gridScale = mCurves[0]->viewScale;
		gridOffset = mCurves[0]->viewOffset;
		unknownScale = false;
	}

	RectF localCameraRect = mViewCamera.GetRect();
	RectF curveViewCameraRect(LocalToCurveView(localCameraRect.LeftTop(), gridScale, gridOffset),
							  LocalToCurveView(localCameraRect.RightBottom(), gridScale, gridOffset));

	Camera screenCamera;
	RectF screenCameraRect = screenCamera.GetRect();

	auto camTransformBasis = mViewCamera.GetBasis().Inverted()*screenCamera.GetBasis();
	auto localToScreen = [&](const Vec2F& point) { return point*camTransformBasis; };

	o2Render.SetCamera(screenCamera);

	float minCellSize = 0.000001f;
	float maxCellSize = 1000000.0f;
	Vec2F curveViewCellSize(minCellSize, minCellSize);
	Vec2F optimalCurveViewCellSize = curveViewCameraRect.Size()/2.0f;

	while (curveViewCellSize.x < maxCellSize)
	{
		float next = curveViewCellSize.x*10.0f;
		if (optimalCurveViewCellSize.x > curveViewCellSize.x && optimalCurveViewCellSize.x <= next)
			break;

		curveViewCellSize.x = next;
	}

	while (curveViewCellSize.y < maxCellSize)
	{
		float next = curveViewCellSize.y*10.0f;
		if (optimalCurveViewCellSize.y > curveViewCellSize.y && optimalCurveViewCellSize.y <= next)
			break;

		curveViewCellSize.y = next;
	}

	Vec2F localCellSize = CurveViewToLocal(curveViewCellSize, gridScale, gridOffset) -
		CurveViewToLocal(Vec2F(), gridScale, gridOffset);

	Vec2F screenCellSize = localToScreen(localCellSize) - localToScreen(Vec2F());

	Vec2F gridCurveViewOrigin(Math::Round(curveViewCameraRect.Center().x/curveViewCellSize.x)*curveViewCellSize.x,
							  Math::Round(curveViewCameraRect.Center().y/curveViewCellSize.y)*curveViewCellSize.y);

	Vec2F gridLocalOrigin = CurveViewToLocal(gridCurveViewOrigin, gridScale, gridOffset);
	Vec2F gridScreenOrigin = localToScreen(gridLocalOrigin);

	Vec2I cellsCount(Math::CeilToInt(curveViewCameraRect.Width()/curveViewCellSize.x),
					 Math::CeilToInt(curveViewCameraRect.Height()/curveViewCellSize.y));

	Color4 cellColorSmoothed = Math::Lerp(mGridColor, mBackColor, 0.7f);

	Vec2F tenCurveViewCellSize = curveViewCellSize*10.0f;

	if (o2Input.IsKeyDown(VK_F1))
	{
		o2Render.DrawCross(gridScreenOrigin, 5, Color4::Red());
		mTextLeft->SetScale(Vec2F(1, 1));
		mTextLeft->SetText(String(gridCurveViewOrigin) + "\n" + String(curveViewCellSize));
		mTextLeft->SetPosition(gridScreenOrigin);
		mT```cpp
void CurvesEditor::DrawGrid()
{
	if (mUnknownScale)
		return;

	Ref<IText> mTextTop = mTextFactory->Create();
	Ref<IText> mTextBottom = mTextFactory->Create();
	Ref<IText> mTextLeft = mTextFactory->Create();
	Ref<IText> mTextRight = mTextFactory->Create();
	const Ref<ICamera>& mViewCamera = mCameraManager->GetViewCamera();

	float screenCellSizeX = mViewCamera->GetRect().GetWidth() / mCellsCountX;
	float screenCellSizeY = mViewCamera->GetRect().GetHeight() / mCellsCountY;
	float curveViewCellSizeX = mCurveViewCameraRect.GetWidth() / mCellsCountX;
	float curveViewCellSizeY = mCurveViewCameraRect.GetHeight() / mCellsCountY;
	float tenCurveViewCellSizeX = curveViewCellSizeX * 10.0f;
	float tenCurveViewCellSizeY = curveViewCellSizeY * 10.0f;

	Vec2F gridScreenOrigin = mViewCamera->GetRect().GetTopLeft();
	Vec2F gridCurveViewOrigin = mCurveViewCameraRect.GetTopLeft();

	Color cellColorSmoothed = mGridColor.Smooth(0.4f, mBackgroundSmoothColor);
	bool horGridEnabled = (mCellsCountY > 0 && VerGridEnabled);
	bool verGridEnabled = (mCellsCountX > 0 && HorGridEnabled);

	if (horGridEnabled)
	{
		for (int i = -mCellsCountY / 2; i < mCellsCountY / 2; i++)
		{
			float d = (float)i * screenCellSizeY;
			float f = (gridCurveViewOrigin.y + (float)i * curveViewCellSizeY) / tenCurveViewCellSizeY;
			float rdy = Math::Abs(f - Math::Floor(f));
			bool yTen = rdy < 0.05f || rdy > 0.95f;

			Vec2F xBegin = Vec2F(mViewCamera->GetRect().GetLeft(), Math::Round(d + gridScreenOrigin.y));
			Vec2F xEnd = Vec2F(mViewCamera->GetRect().GetRight(), Math::Round(d + gridScreenOrigin.y));

			o2Render.DrawAALine(xBegin, xEnd, yTen ? mGridColor : cellColorSmoothed);
		}
	}

	if (verGridEnabled)
	{
		for (int i = -mCellsCountX / 2; i < mCellsCountX / 2; i++)
		{
			float d = (float)i * screenCellSizeX;

			float f = (gridCurveViewOrigin.x + (float)i * curveViewCellSizeX) / tenCurveViewCellSizeX;
			float rdx = Math::Abs(f - Math::Floor(f));
			bool xTen = rdx < 0.05f || rdx > 0.95f;

			Vec2F yBegin = Vec2F(Math::Round(d + gridScreenOrigin.x), mViewCamera->GetRect().GetBottom());
			Vec2F yEnd = Vec2F(Math::Round(d + gridScreenOrigin.x), mViewCamera->GetRect().GetTop());

			o2Render.DrawAALine(yBegin, yEnd, xTen ? mGridColor : cellColorSmoothed);
		}
	}

	char buf[255];

	if (horGridEnabled)
	{
		for (int i = -mCellsCountY / 2; i < mCellsCountY / 2; i++)
		{
			float y = (float)i * screenCellSizeY + gridScreenOrigin.y;
			float yc = (float)i * curveViewCellSizeY + gridCurveViewOrigin.y;

			sprintf_s(buf, "%.1f", (Math::Round(yc * 10.0f) / 10.0f));
			String yCaption = buf;

			if (horGridEnabled)
			{
				mTextLeft->SetText(yCaption);
				mTextLeft->SetPosition(Vec2F(mViewCamera->GetRect().GetLeft() + mTextBorder.left, y));
				mTextLeft->Draw();

				mTextRight->SetText(yCaption);
				mTextRight->SetPosition(Vec2F(mViewCamera->GetRect().GetRight() - mTextBorder.right, y));
				mTextRight->Draw();
			}
		}
	}

	if (verGridEnabled)
	{
		for (int i = -mCellsCountX / 2; i < mCellsCountX / 2; i++)
		{
			float x = (float)i * screenCellSizeX + gridScreenOrigin.x;
			float xc = (float)i * curveViewCellSizeX + gridCurveViewOrigin.x;

			sprintf_s(buf, "%.1f", (Math::Round(xc * 10.0f) / 10.0f));
			String xCaption = buf;

			if (verGridEnabled)
			{
				mTextTop->SetText(xCaption);
				mTextTop->SetPosition(Vec2F(x, mViewCamera->GetRect().GetTop() - mTextBorder.top));
				mTextTop->Draw();

				mTextBottom->SetText(xCaption);
				mTextBottom->SetPosition(Vec2F(x, mViewCamera->GetRect().GetBottom() + mTextBorder.bottom));
				mTextBottom->Draw();
			}
		}
	}

	o2Render.SetCamera(mViewCamera);
}

void CurvesEditor::DrawCurves()
{
	o2Render.camera = nullptr;

	Basis transform = mViewCamera->GetBasis().Inverted() * Camera()->GetBasis();

	for (auto curve : mCurves)
	{
		if (curve->approximatedPoints.IsEmpty())
			continue;

		//o2Render.DrawLine(curve->approximatedPoints, curve->color);

		float cameraLeftPos = mViewCamera->GetRect().GetLeft();
		float cameraRightPos = mViewCamera->GetRect().GetRight();
		for (int i = 1; i < curve->approximatedPoints.Count(); i++)
		{
			Vec2F& a = curve->approximatedPoints[i - 1];
			Vec2F& b = curve->approximatedPoints[i];

			if (b.x < cameraLeftPos)
				continue;

			if (a.x > cameraRightPos)
				break;

			o2Render.DrawAALine(a * transform, b * transform, curve->color);
		}
	}

	o2Render.camera = mViewCamera;
}

void CurvesEditor::DrawHandles()
{
	for (auto curve : mCurves)
	{
		for (auto keyHandles : curve->handles)
			keyHandles->Draw(layout->GetWorldRect());
	}
}

void CurvesEditor::DrawSelection()
{
	if (mIsPressed)
	{
		mSelectionSprite->rect = RectF(LocalToScreenPoint(mSelectingPressedPoint), o2Input.cursorPos);
		mSelectionSprite->Draw();
	}
}

void CurvesEditor::DrawTransformFrame()
{
	if (!mTransformFrameVisible)
		return;

	Vec2F borders(10, 10);

	mTransformFrame.SetBasis(Basis(LocalToScreenPoint(mTransformFrameBasis.origin) - borders,
		mTransformFrameBasis.xv / mViewCamera->GetScale() + Vec2F(borders.x * 2.0f, 0),
		mTransformFrameBasis.yv / mViewCamera->GetScale() + Vec2F(0, borders.y * 2.0f)));

	mTransformFrame.Draw();

	if (o2Input.IsKeyDown(VK_CONTROL))
	{
		Vec2F left = mTransformFrame.GetCurrentBasis().origin;
		Vec2F right = mTransformFrame.GetCurrentBasis().origin + mTransformFrame.Ge
```replace raw pointers with Ref<>, except void* pointers.Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

```
void CurvesEditor::DrawGrid()
{
    // ...
    float startX = mathUtil.Round(space.x / currentBasis.xv);
    float startY = mathUtil.Round(space.y / currentBasis.yv);
    // ...
}

void CurvesEditor::AddCurveKeyHandles(Ref<CurveInfo> info, int keyId)
{
    PushEditorScopeOnStack scope;

    Ref<KeyHandles> keyHandles = mmake<KeyHandles>(mMainHandleSample, mSupportHandleSample, this, info->color);
    keyHandles->curveKeyIdx = keyId;
    keyHandles->curveKeyUid = info->curve->GetKeyAt(keyId).uid;

    Curve::Key curveKey = info->curve->GetKeyAt(keyId);
    Curve::Key lastCurveKey = info->curve->GetKeyAt(MathUtil::Max(0, keyId - 1));
    Curve::Key nextCurveKey = info->curve->GetKeyAt(MathUtil::Min(keyId + 1, info->curve->GetKeys().Count()));

    // main handle
    keyHandles->mainHandle.SetPosition(Vec2F(curveKey.position, curveKey.value));
    keyHandles->mainHandle.curveInfo = info;
    keyHandles->mainHandle.onChangedPos = [=](const Vec2F& pos) { OnCurveKeyMainHandleDragged(info, keyHandles, pos); };
    keyHandles->mainHandle.onRightButtonReleased = THIS_FUNC(OnCursorRightMouseReleased);
    keyHandles->mainHandle.onPressed = THIS_FUNC(OnTransformBegin);
    keyHandles->mainHandle.onChangeCompleted = THIS_FUNC(OnTransformCompleted);
    keyHandles->mainHandle.messageFallDownListener = this;

    keyHandles->mainHandle.onDraw = [=]() {
        if (o2Input.IsKeyDown(VK_F1))
            o2Debug.DrawText(keyHandles->mainHandle.GetScreenPosition(), String(keyHandles->curveKeyIdx));
    };

    // left support handle
    keyHandles->leftSupportHandle.curveInfo = info;
    keyHandles->leftSupportHandle.SetPosition(Vec2F(curveKey.position + curveKey.leftSupportPosition,
                                                   curveKey.value + curveKey.leftSupportValue));

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
    keyHandles->rightSupportHandle.curveInfo = info;
    keyHandles->rightSupportHandle.SetPosition(Vec2F(curveKey.position + curveKey.rightSupportPosition,
                                                    curveKey.value + curveKey.rightSupportValue));

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

    mNeedRedraw = true;
}

void CurvesEditor::RemoveCurveKeyHandles(Ref<CurveInfo> info, int keyId)
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

    mNeedRedraw = true;
}

void CurvesEditor::OnCurveKeyMainHandleDragged(Ref<CurveInfo> info, Ref<KeyHandles> handles, const Vec2F& position)
{
    info->BeginCurveManualChange();

    Curve::Key key = info->curve->GetKeyAt(handles->curveKeyIdx);

    Vec2F initialDragPoint = handles->mainHandle.GetDraggingBeginPosition();

    if (handles->mainHandle.IsPressed())
    {
        if (o2Input.IsKeyDown(VK_CONTROL) && o2Input.IsKeyDown(VK_SHIFT))
        {
            key.position = MathUtil::Round(position.x*10.0f)/10.0f;
            key.value = MathUtil::Round(position.y*10.0f)/10.0f;
        }
        else if (o2Input.IsKey
```#include <Ref.h>

void CurvesEditor::OnCurveKeyDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
{
	info->BeginCurveManualChange();

	Ref<Curve::Key> key = mmake<Curve::Key>(info->curve->GetKeyAt(handles->curveKeyIdx));

	if (o2Input.IsKeyDown(VK_CONTROL) && o2Input.IsKeyDown(VK_SHIFT))
	{
		Vec2F initialDragPoint = handles->valueHandle.GetDraggingBeginPosition();
		float dst = (initialDragPoint - Vec2F(key->position, key->value)).Length();
		Vec2F v = (position - Vec2F(key->position, key->value)).Normalized() * dst;
		key->position = v.x;
		key->value = v.y;
	}
	else if (o2Input.IsKeyDown(VK_CONTROL))
	{
		key->position = position.x;
		key->value = initialDragPoint.y;
	}
	else if (o2Input.IsKeyDown(VK_SHIFT))
	{
		key->position = initialDragPoint.x;
		key->value = position.y;
	}
	else
	{
		key->position = position.x;
		key->value = position.y;
	}

	int newKeyIdx = 0;
	for (auto& k : info->curve->GetKeys())
	{
		if (k.position > key->position)
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
			info->handles[i]->curveKeyUid = key->uid;
		}
	}

	info->curve->SetKey(*key, handles->curveKeyIdx);

	info->UpdateHandles();
	info->UpdateApproximatedPoints();

	CheckHandlesVisible();
	UpdateTransformFrame();
	RecalculateViewArea();
	mNeedRedraw = true;

	info->CompleteCurveManualChange();
}

void CurvesEditor::OnCurveKeyLeftSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
{
	info->BeginCurveManualChange();

	Ref<Curve::Key> key = mmake<Curve::Key>(info->curve->GetKeyAt(handles->curveKeyIdx));
	Ref<Curve::Key> prevKey = mmake<Curve::Key>(info->curve->GetKeyAt(Math::Max(handles->curveKeyIdx - 1, 0)));

	if (o2Input.IsKeyDown(VK_CONTROL) && o2Input.IsKeyDown(VK_SHIFT))
	{
		Vec2F initialDragPoint = handles->leftSupportHandle.GetDraggingBeginPosition();
		float dst = (initialDragPoint - Vec2F(key->position, key->value)).Length();
		Vec2F v = (position - Vec2F(key->position, key->value)).Normalized() * dst;
		key->leftSupportPosition = v.x;
		key->leftSupportValue = v.y;
	}
	else if (o2Input.IsKeyDown(VK_CONTROL))
	{
		key->leftSupportPosition = position.x - key->position;
		key->leftSupportValue = 0;
	}
	else if (o2Input.IsKeyDown(VK_SHIFT))
	{
		Vec2F initialDragPoint = handles->leftSupportHandle.GetDraggingBeginPosition();

		Vec2F v = initialDragPoint - Vec2F(key->position, key->value);
		float dst = (position - Vec2F(key->position, key->value)).Length();
		v = v.Normalized() * dst;
		key->leftSupportPosition = v.x;
		key->leftSupportValue = v.y;
	}
	else
	{
		key->leftSupportPosition = position.x - key->position;
		key->leftSupportValue = position.y - key->value;
	}

	if (key->supportsType == Curve::Key::Type::Flat || key->supportsType == Curve::Key::Type::Discrete ||
		key->supportsType == Curve::Key::Type::Smooth)
	{
		key->supportsType = Curve::Key::Type::Free;
	}

	if (key->supportsType == Curve::Key::Type::Free && handles->curveKeyIdx < info->curve->GetKeys().Count() - 1)
	{
		Ref<Curve::Key> nextKey = mmake<Curve::Key>(info->curve->GetKeyAt(Math::Min(handles->curveKeyIdx + 1,
															 info->curve->GetKeys().Count() - 1)));

		key->rightSupportPosition = -key->leftSupportPosition;
		key->rightSupportValue = -key->leftSupportValue;
	}

	info->curve->SetKey(*key, handles->curveKeyIdx);

	info->UpdateHandles();
	info->UpdateApproximatedPoints();

	CheckHandlesVisible();
	RecalculateViewArea();
	mNeedRedraw = true;

	info->CompleteCurveManualChange();
}

void CurvesEditor::OnCurveKeyRightSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
{
	info->BeginCurveManualChange();

	Ref<Curve::Key> key = mmake<Curve::Key>(info->curve->GetKeyAt(handles->curveKeyIdx));
	Ref<Curve::Key> nextKey = mmake<Curve::Key>(info->curve->GetKeyAt(Math::Min(handles->curveKeyIdx + 1, info->curve->GetKeys().Count() - 1)));

	if (o2Input.IsKeyDown(VK_CONTROL) && o2Input.IsKeyDown(VK_SHIFT))
	{
		Vec2F initialDragPoint = handles->rightSupportHandle.GetDraggingBeginPosition();
		float dst = (initialDragPoint - Vec2F(key->position, key->value)).Length();
		Vec2F v = (position - Vec2F(key->position, key->value)).Normalized() * dst;
		key->rightSupportPosition = v.x;
		key->rightSupportValue = v.y;
	}
	else if (o2Input.IsKeyDown(VK_CONTROL))
	{
		key->rightSupportPosition = position.x - key->position;
		key->rightSupportValue = 0;
	}
	else if (o2Input.IsKeyDown(VK_SHIFT))
	{
		Vec2F initialDragPoint = handles->rightSupportHandle.GetDraggingBeginPosition();

		Vec2F v = initialDragPoint - Vec2F(key->position, key->value);
		float dst = (position - Vec2F(key->position, key->value)).Length();
		v = v.Normalized() * dst;
		key->rightSupportPosition = v.x;
		key->rightSupportValue = v.y;
	}
	else
	{
		key->rightSupportPosition = position.x - key->position;
		key->rightSupportValue = position.y - key->value;
	}

	if (key->supportsType == Curve::Key::Type::Flat || key->supportsType == Curve::Key::Type::Discrete ||
		key->supportsType == Curve::Key::Type::Smooth)
	{
		key->supportsType = Curve::Key::Type::Free;
	}

	if (key->supportsType == Curve::Key::Type::Free && handles->curveKeyIdx > 0)
	{
		Ref<Curve::Key> prevKey = mmake<Curve::Key>(info->curve->GetKeyAt(Math::Max(handles->curveKeyIdx - 1, 0)));

		Vec2F rtemplate<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
const Ref<T>& mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
inline Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

template<typename T>
inline Ref<T> DynamicCast(const Ref<T>& ptr)
{
    return ptr;
}

class CurveInfo;
class KeyHandles;
struct CurveKeysInfo;

class CurvesEditor {
public:
    void OnCursorDblClicked(const Input::Cursor& cursor);

    void SmoothKey(const Ref<CurveInfo>& info, int idx);

    Vec2F CheckLeftSupportHandlePosition(const Ref<CurveInfo>& info, const Ref<KeyHandles>& handles, const Vec2F& position);

    Vec2F CheckRightSupportHandlePosition(const Ref<CurveInfo>& info, const Ref<KeyHandles>& handles, const Vec2F& position);

    void Apply()
    {
        mNeedRedraw = true;

        info->CompleteCurveManualChange();
    }

private:
    struct CurveInfo {
        Ref<Curve> curve;
        float viewScale;
        Vec2F viewOffset;
        bool disableChangesHandling;
        int curveId;
        RefArray<KeyHandles> handles;

        void UpdateHandles();
        void UpdateApproximatedPoints();
        void CompleteCurveManualChange();
    };

    struct KeyHandles {
        Curve::Key mainHandle;
        Curve* curve;
        int curveKeyIdx;
    };

    Ref<CurveInfo> GetClickedCurveInfo(const Vec2F& localCursorPos);

    void AddCurveKeyHandles(const Ref<CurveInfo>& info, int idx);

    void CheckHandlesVisible();

    Vec2F CurveViewToLocal(const Vec2F& point, float scale, const Vec2F& offset);

    Vec2F LocalToCurveView(const Vec2F& point, float scale, const Vec2F& offset);

    Vec2F ScreenToLocalPoint(const Vec2F& point);

    Vec2F LocalToScreenPoint(const Vec2F& point);

    template<typename T>
    static Vec2F PointToLocal(const Vec2F& point, const T& info);

    template<typename T>
    static Vec2F LocalToPoint(const Vec2F& point, const T& info);

    void RecalculateViewArea();

    void DoneAction(CurveAddKeysAction* action);

    void SelectHandle(const Curve::Key* mainHandle);

private:
    Vector<Ref<CurveInfo>> mCurves;
    bool mNeedRedraw;
};Ref<CurveHandle> handle = o2Make<CurveHandle>();
handle->mainHandle.SetSelected(false);
handle->SetSelected(true);
mSelectingHandlesBuf.Add(handle);
SetHandleSelectedState(handle, true);#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class CurvesEditor : public ScrollView
{
public:
	void ShowContextMenu(const Input::Cursor& cursor) const
	{
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

	void OnCursorRightMouseReleased(const Input::Cursor& cursor) override
	{
		mIsViewScrolling = false;

		ScrollView::OnCursorRightMouseReleased(cursor);
	}

	void CheckHandlesVisible()
	{
		for (const auto& info : mCurves)
		{
			for (const auto& handles : info->handles)
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

	void UpdateTransformFrame()
	{
		mTransformFrameVisible = IsTransformFrameVisible();

		if (!mTransformFrameVisible)
			return;

		RectF aabb(((CurveHandle*)mSelectedHandles[0])->GetLocalPosition(), ((CurveHandle*)mSelectedHandles[0])->GetLocalPosition());

		for (const auto& handle : mSelectedHandles)
		{
			CurveHandle* curveHandle = (CurveHandle*)handle;

			aabb.left = std::min(curveHandle->GetLocalPosition().x, aabb.left);
			aabb.right = std::max(curveHandle->GetLocalPosition().x, aabb.right);
			aabb.top = std::max(curveHandle->GetLocalPosition().y, aabb.top);
			aabb.bottom = std::min(curveHandle->GetLocalPosition().y, aabb.bottom);
		}

		mTransformFrameBasis = Basis(aabb.LeftBottom(), Vec2F::Right()*aabb.Width(), Vec2F::Up()*aabb.Height());
	}

	bool IsTransformFrameVisible() const
	{
		int selectedMainHandles = 0;
		for (const auto& handle : mSelectedHandles)
		{
			if (!mSupportHandles.Contains((CurveHandle*)handle))
				selectedMainHandles++;
		}

		return selectedMainHandles > 1;
	}

	void OnHandleCursorReleased(const Ref<DragHandle>& handle, const Input::Cursor& cursor)
	{
		SelectableDragHandlesGroup::OnHandleCursorReleased(handle, cursor);
		UpdateTransformFrame();
		CheckHandlesVisible();
	}

	void OnHandleBeganDragging(const Ref<DragHandle>& handle)
	{
		if (mSupportHandles.Contains((CurveHandle*)handle))
			return;

		SelectableDragHandlesGroup::OnHandleBeganDragging(handle);
	}

	void OnHandleMoved(const Ref<DragHandle>& handle, const Vec2F& cursorPos)
	{
		if (mSupportHandles.Contains((CurveHandle*)handle))
		{
			if (!handle->IsSelected())
				return;

			for (const auto& handle : mSupportHandles)
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

	void OnHandleCompletedChange(const Ref<DragHandle>& handle)
	{
		OnTransformCompleted();
	}

	void OnSelectionChanged()
	{
		mNeedRedraw = true;
	}

	void SetSelectedKeysSupportsType(Curve::Key::Type type)
	{
		for (const auto& info : mCurves)
		{
			auto keys = info->curve->GetKeys();

			for (const auto& handles : info->handles)
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

					case Curve::Key::Type::Linear:
						key.leftSupportPosition = 0;
						key.rightSupportPosition = 0;
						key.leftSupportValue = 0;
						key.rightSupportValue = 0;
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

	void OnTransformFrameTransformed(const Basis& basis)
	{
		Vec2F border(10, 10);
		Basis localBasis(ScreenToLocalPoint(basis.origin + border),
			(basis.xv - Vec2F(border.x*2.0f, 0))*mViewCamera.GetScale(),
			(basis.yv - Vec2F(0, border.y*2.0f))*mViewCamera.GetScale());

		Basis lastTransformBasis = mTransformFrameBasis;
		Basis delta = mTransformFrameBasis.Inverted()*localBasis;

		if (delta.origin.Length() > 0.01f || delta.xv != Vec2F(1, 0) || delta.yv != Vec2F(0, 1))
		{
			for (const auto& handle : mSelectedHandles)
			{
				CurveHandle* curveHandle = (CurveHandle*)handle;
				curveHandle->SetPosition(curveHandle->LocalToCurveView(curveHandle->GetLocalPosition()*delta));
				curveHandle->onChangedPos(handle->GetPosition());
			}

			if (o2Input.IsKeyDown(VK_CO#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
concept NotVoidPtr = !std::is_same_v<T, void*>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
Ref<T> DynamicCast(const Ref<T>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class CurvesEditor {
public:
	void UpdateTransformFrame() {}
	
	void TransformHandles(const Mat2F& lastTransformBasis, const Mat2F& localBasis) 
	{
		float right = lastTransformBasis.origin.x + lastTransformBasis.xv.x;
		float left = lastTransformBasis.origin.x;
		
		float rightOffset = localBasis.origin.x + localBasis.xv.x - right;
		float leftOffset = localBasis.origin.x - left;
		
		bool rightChanged = !Math::Equals(rightOffset, 0.0f) && rightOffset > FLT_EPSILON;
		bool leftChanged = !Math::Equals(leftOffset, 0.0f) && leftOffset < FLT_EPSILON;
		
		for (auto& handle : mHandles)
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
		
		UpdateTransformFrame();
	}
	
	void OnTransformBegin()
	{
		mBeforeTransformKeys.Clear();
		
		for (auto& curveInfo : mCurves)
		{
			CurveKeysInfo keysInfo;
			keysInfo.keys.Clear();
			keysInfo.selectedHandles.Clear();
			
			for (auto handles : curveInfo->handles)
			{
				if (handles->IsSomeHandleSelected())
				{
					keysInfo.keys.Add(curveInfo->curve->GetKeyAt(handles->curveKeyIdx));
					
					SelectedHandlesInfo selectedHandlesInfo;
					selectedHandlesInfo.index = handles->curveKeyIdx;
					selectedHandlesInfo.mainHandle = handles->mainHandle.IsSelected();
					selectedHandlesInfo.leftSupportHandle = handles->leftSupportHandle.IsSelected();
					selectedHandlesInfo.rightSupportHandle = handles->rightSupportHandle.IsSelected();
					keysInfo.selectedHandles.Add(selectedHandlesInfo);
				}
			}
			
			if (!keysInfo.keys.IsEmpty())
			{
				keysInfo.curveId = curveInfo->curveId;
				mBeforeTransformKeys.Add(keysInfo);
			}
		}
	}
	
	void OnTransformCompleted()
	{
		bool changed = false;
		
		for (auto& keysInfo : mBeforeTransformKeys)
		{
			CurveInfo* curveInfo = mCurves.FindOrDefault([&](CurveInfo* x) { return x->curveId == keysInfo.curveId; });
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
			Vector<CurveKeysChangeAction::KeysInfo> actionKeysInfos;
			
			for (auto& keysInfo : mBeforeTransformKeys)
			{
				CurveInfo* curveInfo = mCurves.FindOrDefault([&](CurveInfo* x) { return x->curveId == keysInfo.curveId; });
				if (!curveInfo)
					continue;
				
				CurveKeysChangeAction::KeysInfo actionCurveKeysInfo;
				actionCurveKeysInfo.curveId = keysInfo.curveId;
				actionCurveKeysInfo.beforeKeys = keysInfo.keys;
				actionCurveKeysInfo.selectedHandles = keysInfo.selectedHandles;
				
				for (auto handles : curveInfo->handles)
				{
					if (handles->IsSomeHandleSelected())
						actionCurveKeysInfo.afterKeys.Add(curveInfo->curve->GetKeyAt(handles->curveKeyIdx));
				}
				
				actionKeysInfos.Add(actionCurveKeysInfo);
			}
			
			CurveKeysChangeAction* action = mmake<CurveKeysChangeAction>(actionKeysInfos, this);
			DoneAction(action);
		}
	}
	
	Vec2F CurveViewToLocal(const Vec2F& point, const Vec2F& viewScale, const Vec2F& viewOffset) const
	{
		return (point + viewOffset)*viewScale;
	}
	
	Vec2F LocalToCurveView(const Vec2F& point, const Vec2F& viewScale, const Vec2F& viewOffset) const
	{
		return point/viewScale - viewOffset;
	}
	
	void DoneAction(IAction* action)
	{
		if (actionsListDelegate) {
			actionsListDelegate->DoneAction(action);
			return;
		}
		
		mActionsList.DoneAction(action);
	}
	
	void OnEditPressed()
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
	
	void OnAutoSmoothChecked(bool checked)
	{
		mContextMenu->SetItemChecked(0, true);  // auto smooth
		mContextMenu->SetItemChecked(1, false); // flat
		mContextMenu->SetItemChecked(2, false); // free
		mContextMenu->SetItemChecked(3, false); // linear
		mContextMenu->SetItemChecked(4, false); // broken
		mContextMenu->SetItemChecked(5, false); // discrete
		
		SetSelectedKeysSupportsType(Curve::Key::Type::Smooth);
	}
	
	void OnFlatChecked(bool checked)
	{
		mContextMenu->SetItemChecked(0, false);  // auto smooth
		mContextMenu->SetItemChecked(1, true); // flat
		mContextMenu->SetItemChecked(2, false); // free
		mContextMenu->SetItemChecked(3, false); // linear
		mContextMenu->SetItemChecked(4, false); // broken
		mContextMenu->SetItemChecked(5, false); // discrete
		
		SetSelectedKeysSupportsType(Curve::Key::Type::Flat);
	}
	
private:
	struct CurveKeysChangeAction {
		struct KeysInfo {
			int curveId;
			Vector<Curve::Key> beforeKeys;
			Vector<SelectedHandlesInfo> selectedHandles;
			Vector<Curve::Key> afterKeys;
		};
		
		Vector<KeysInfo> mKeysInfos;
		CurvesEditor* mEditor;
		
		CurveKeysChangeAction(const Vector<KeysInfo>& keysInfos, CurvesEditor* editor)
			: mKeysInfos(keysInfos), mEditor(editor) {}
	};
	
	struct CurveKeysInfo {
		int curveId;
		Vector<Curve::Key> keys;
		Vector<SelectedHandlesInfo> selectedHandles;
	};
	
	struct CurveInfo {
		int curveId;
		Ref<Curve> curve;
		Vector<Ref<Handles>> handles;
	};
	
	struct Vec2F {
		float x, y;
	};
	
	struct Mat2F {
		Vec2F origin, xv, yv;
	};
	
	struct Handles {
		Vec2F position;
		bool onChangedPos(const Vec2F& pos) { return true; }
		
		int curveKeyIdx;
		Ref<Handle> mainHandle;
		Ref<Handle> leftSupportHandle;
		Ref<Handle> rightSupportHandle;
		
		bool IsSomeHandleSelected() const {
			return mainHandle->IsSelected() || leftSupportHandle->IsSelected() || rightSupportHandle->IsSelected();
		}
		
		const Vec2F& GetPosition() const { return position; }
	};
	
	struct Handle {
		bool IsSelected() const { return true; }
	};
	
	struct SelectedHandlesInfo {
		int index;
		bool mainHandle;
		bool leftSupportHandle;
		bool rightSupportHandle;
	};
	
	class Math {
	public:
		static bool Equals(float a, float b) { return a == b; }
	};
	
	class Curve {
	public:
		struct Key {
			enum class Type {
				Smooth,
				Flat
			};
		};
		
		Vector<Key>& GetKeys() { return keys; }
		const Key& GetKeyAt(int index) const { return keys.at(index); }
		void SetKey(const Key& key, int index) {}
		
		Vector<Key> keys;
	};
	
	class KeyEditDlg {
	public:
		static void Show(const typename Curve::Key& key, const std::function<void(const typename Curve::Key&)>& callback) {}
	};
	
	class IAction {};
	
	class ActionsListDelegate {
	public:
		void DoneAction(IAction* action) {}
	};
	
	class ActionsList {
	public:
		void DoneAction(IAction* action) {}
	};
	
	Vector<Handle*> mSelectedHandles;
	Vector<Handles*> mHandles;
	Vector<CurveInfo*> mCurves;
	Vector<CurveKeysInfo> mBeforeTransformKeys;
	ActionsList mActionsList;
	ActionsListDelegate* actionsListDelegate;
	Ref<ContextMenu> mContextMenu;
};#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class CurveCopyInfo {
    // existing code
};

class DataDocument {
    // existing code
};

class CurveInfo {
    // existing code
};

class CurveAddKeysAction {
    // existing code
};

class CurveDeleteKeysAction {
    // existing code
};

void CurvesEditor::SetSelectedKeysSupportsType(const Curve::Key::Type& type)
{
    // existing code
}

void CurvesEditor::OnFreeChecked(bool checked)
{
    // existing code
}

void CurvesEditor::OnLinearChecked(bool checked)
{
    // existing code
}

void CurvesEditor::OnBrokenChecked(bool checked)
{
    // existing code
}

void CurvesEditor::OnDiscreteChecked(bool checked)
{
    // existing code
}

void CurvesEditor::OnCopyPressed()
{
    if (!IsFocused())
        return;

    Vector<Ref<CurveCopyInfo>> copyKeys;

    for (const Ref<CurveInfo>& curve : mCurves)
    {
        Ref<CurveCopyInfo> copyInfo = mmake<CurveCopyInfo>();
        copyInfo->curveId = curve->curveId;

        for (const Ref<CurveHandle>& handles : curve->handles)
        {
            if (!handles->mainHandle.IsSelected())
                continue;

            copyInfo->keys.Add(curve->curve->GetKeyAt(handles->curveKeyIdx));
        }

        if (copyInfo->keys.IsEmpty())
            copyInfo.reset();
        else
            copyKeys.Add(copyInfo);
    }

    DataDocument copyData;
    copyData = copyKeys;
    String copyDataStr = copyData.SaveAsString();

    Clipboard::SetText(copyDataStr);
}

void CurvesEditor::OnCutPressed()
{
    if (!IsFocused())
        return;

    OnCopyPressed();
    OnDeletePressed();
}

void CurvesEditor::OnPastePressed()
{
    if (!IsFocused())
        return;

    float insertPos = ScreenToLocalPoint(o2Input.cursorPos).x;
    DataDocument data;
    data.LoadFromData(Clipboard::GetText());

    Vector<Ref<CurveCopyInfo>> copyKeys;
    copyKeys = data;

    Vector<CurveKeysInfo> keyInfos;

    for (const Ref<CurveCopyInfo>& curve : copyKeys)
    {
        Ref<CurveInfo> curveInfo = mCurves.FindOrDefault([&](const Ref<CurveInfo>& x) { return x->curveId == curve->curveId; });

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
        for (const Curve::Key& key : curve->keys)
        {
            key.position += positionDelta;
            key.uid = Math::Random();

            int position = curveInfo->curve->InsertKey(key);
            AddCurveKeyHandles(curveInfo, position);

            keyInfos.Last().keys.Add(key);
        }

        curveInfo->UpdateApproximatedPoints();
        curveInfo->UpdateHandles();
        mNeedRedraw = true;

        curveInfo->CompleteCurveManualChange();
    }

    if (!keyInfos.IsEmpty())
    {
        CurveAddKeysAction* action = mnew CurveAddKeysAction(keyInfos, this);
        DoneAction(action);
    }
}

void CurvesEditor::OnDeletePressed()
{
    if (!IsFocused())
        return;

    Vector<CurveKeysInfo> keyInfos;

    for (const Ref<CurveInfo>& curveInfo : mCurves)
    {
        curveInfo->BeginCurveManualChange();

        Ref<CurveCopyInfo> copyInfo = mmake<CurveCopyInfo>();
        copyInfo->curveId = curveInfo->curveId;

        Vector<int> removingIdxs;
        for (const Ref<CurveHandle>& handles : curveInfo->handles)
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
        CurveDeleteKeysAction* action = mnew CurveDeleteKeysAction(keyInfos, this);
        DoneAction(action);
    }
}#include <memory>

template<typename T>
class Ref
{
public:
	Ref(T* ptr = nullptr) : mPtr(ptr) {}

	Ref(const Ref& other) : mPtr(other.mPtr) {}

	T* operator->() const { return mPtr; }

	T& operator*() const { return *mPtr; }

	T* get() const { return mPtr; }

	operator bool() const { return mPtr != nullptr; }

private:
	T* mPtr;
};

template<typename T>
Ref<T> mmake()
{
	return Ref<T>(new T());
}

template<typename Base, typename Derived>
Ref<Base> DynamicCast(const Ref<Derived>& derived)
{
	return Ref<Base>(dynamic_cast<Base*>(derived.get()));
}

template<typename T>
class WeakRef
{
public:
	WeakRef(T* ptr = nullptr) : mPtr(ptr) {}

	WeakRef(const WeakRef& other) : mPtr(other.mPtr) {}

	T* operator->() const { return mPtr; }

	T& operator*() const { return *mPtr; }

	T* get() const { return mPtr; }

	operator bool() const { return mPtr != nullptr; }

private:
	T* mPtr;
};

class CurvesEditor
{
public:
	class CurveInfo;

	CurvesEditor() {}

	~CurvesEditor() {}

	void OnDeletePressed();

	void OnInsertPressed();

	void OnUndoPressed();

	void OnRedoPressed();

	class CurveInfo
	{
	public:
		CurveInfo() {}

		~CurveInfo() {}

		void UpdateHandles();

		void UpdateApproximatedPoints();

		void AdjustScale();

		void OnCurveChanged();

		void BeginCurveManualChange();

		void CompleteCurveManualChange();

	private:
		Ref<Curve> curve;
		Vector<KeyHandle*> handles;
		Vector<Vec2F> approximatedPoints;
		Vec2F viewScale;
		Vec2F viewOffset;
		bool disableChangesHandling;
	};

private:
	Vector<Ref<CurveInfo>> mCurves;
	Ref<ActionsListBase> mActionsList;
	bool mAdjustCurvesScale;
};

void CurvesEditor::OnDeletePressed()
{
	if (!IsFocused())
		return;

	Vector<Ref<CurveInfo>> selectedCurves;

	for (auto curveInfo : mCurves)
	{
		bool deleteCurve = curveInfo->handles.Any([](const KeyHandle* handle) { return handle->mainHandle.IsSelected(); });

		if (deleteCurve)
		{
			auto action = mmake<CurveDeleteKeysAction>(keyInfos, this);
			DoneAction(action);
		}
	}
}

void CurvesEditor::OnInsertPressed()
{
	if (!IsFocused())
		return;

	float insertPos = ScreenToLocalPoint(o2Input.cursorPos).x;
	DataDocument data;
	data.LoadFromData(Clipboard::GetText());

	Vector<Ref<CurveInfo>> copyKeys;
	copyKeys = data;

	for (auto curve : copyKeys)
	{
		Ref<CurveInfo> curveInfo = mCurves.FindOrDefault([=](const CurveInfo* x) { return x->curveId == curve->curveId; });

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
	}
}

void CurvesEditor::OnUndoPressed()
{
	if (actionsListDelegate)
		actionsListDelegate->UndoAction();
	else
		mActionsList->UndoAction();
}

void CurvesEditor::OnRedoPressed()
{
	if (actionsListDelegate)
		actionsListDelegate->RedoAction();
	else
		mActionsList->RedoAction();
}

CurvesEditor::CurveInfo::CurveInfo()
{ }

CurvesEditor::CurveInfo::~CurveInfo()
{
	for (auto x : handles)
	{
		x->mainHandle.SetSelectionGroup(nullptr);
		x->leftSupportHandle.SetSelectionGroup(nullptr);
		x->rightSupportHandle.SetSelectionGroup(nullptr);
	}

	if (curve)
		curve->onKeysChanged -= MakeSubscription(this, &CurveInfo::OnCurveChanged, []() {});
}

void CurvesEditor::CurveInfo::UpdateHandles()
{
	for (int i = 0; i < handles.Count(); i++)
	{
		Curve::Key key = curve->GetKeyAt(i);
		handles[i]->mainHandle.position = Vec2F(key.position, key.value);

		if (i > 0)
		{
			handles[i]->leftSupportHandle.position = Vec2F(key.position + key.leftSupportPosition, key.value + key.leftSupportValue);
			handles[i]->leftSupportHandle.enabled = handles[i]->leftSupportHandle.IsSelected() || handles[i]->mainHandle.IsSelected();
		}

		else handles[i]->leftSupportHandle.enabled = false;

		if (i < handles.Count() - 1)
		{
			handles[i]->rightSupportHandle.position = Vec2F(key.position + key.rightSupportPosition, key.value + key.rightSupportValue);
			handles[i]->rightSupportHandle.enabled = handles[i]->rightSupportHandle.IsSelected() || handles[i]->mainHandle.IsSelected();
		}

		else handles[i]->rightSupportHandle.enabled = false;
	}
}

void CurvesEditor::CurveInfo::UpdateApproximatedPoints()
{
	approximatedPoints.Clear();

	auto keys = curve->GetKeys();
	for (int i = 1; i < keys.Count(); i++)
	{
		const ApproximationValue* keyPoints = keys[i].GetApproximatedPoints();
		for (int j = 0; j < keys[i].GetApproximatedPointsCount(); j++)
			approximatedPoints.Add(((Vec2F)keyPoints[j] + viewOffset) * viewScale);
	}
}

void CurvesEditor::CurveInfo::AdjustScale()
{
	if (editor->mAdjustCurvesScale)
	{
		RectF rect = curve->GetRect();
		viewScale = Vec2F(1, Math::Min(100.0f, rect.Height() < FLT_EPSILON ? 1.0f : 1.0f / rect.Height()));
		viewOffset = Vec2F(0, -rect.bottom);
	}

	else
	{
		viewScale = Vec2F(1, 1);
		viewOffset = Vec2F();
	}

	UpdateApproximatedPoints();
	UpdateHandles();
}

void CurvesEditor::CurveInfo::OnCurveChanged()
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
				handle->leftSupportHandle.SetSelectionGroup(nullptr)();
			}

			editor->mSupportHandles.Remove(&handle->leftSupportHandle);

			if (handle->rightSupportHandle.IsSelected())
			{
				selectedRight.Add(handle->curveKeyUid);
				handle->rightSupportHandle.SetSelectionGroup(nullptr)();
			}

			editor->mSupportHandles.Remove(&handle->rightSupportHandle);

			if (handle->mainHandle.IsSelected())
			{ 
				selectedMain.Add(handle->curveKeyUid);
				handle->mainHandle.SetSelected(false);
			}

			editor->mHandles.Remove(&handle->mainHandle);

			delete handle;
		}

		handles.clear();

		for (int i = 0; i < curve->GetKeys().Count(); i++)
			editor->AddCurveKeyHandles(this, i);

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

	AdjustScale();
	UpdateHandles();
}

void CurvesEditor::CurveInfo::BeginCurveManualChange()
{
	disableChangesHandling = true;
}

void CurvesEditor::CurveInfo::CompleteCurveManualChange()
{
	disableChangesHandling = false;
	OnCurveChanged();
}#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
class DynamicCast
{
public:
	template<typename U>
	static Ref<U> Cast(const Ref<T>& ptr)
	{
		return std::dynamic_pointer_cast<U>(ptr);
	}
};


namespace Editor
{

class CurvesEditor
{
public:
	bool disableChangesHandling = false;

	class RangeInfo
	{
	public:
		RangeInfo();

		virtual ~RangeInfo();

		void UpdateMesh();
	};

	class KeyHandles
	{
	public:
		KeyHandles(const Ref<CurveHandle>& mainSample, const Ref<CurveHandle>& supportSample,
				   CurvesEditor* editor, const Color4& color);

		void Draw(const RectF& camRect);

		bool IsSomeHandleSelected() const;

	private:
		Ref<CurveHandle> mainHandle;
		Ref<CurveHandle> leftSupportHandle;
		Ref<CurveHandle> rightSupportHandle;
		CurvesEditor* curveEditor;
	};

	class CurveHandle : public DragHandle
	{
	public:
		CurveHandle();

		CurveHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr,
					Sprite* selected = nullptr, Sprite* selectedHovered = nullptr, Sprite* selectedPressed = nullptr);

		CurveHandle(const DragHandle& other);

		~CurveHandle();

		CurveHandle& operator=(const CurveHandle& other);

		Vec2F GetLocalPosition() const;

		Vec2F LocalToCurveView(const Vec2F& point) const;

		Vec2F CurveViewToLocal(const Vec2F& point) const;

		Vec2F ScreenToLocal(const Vec2F& point);

		Vec2F LocalToScreen(const Vec2F& point);
	};

	class CurveCopyInfo
	{
	};

	bool operator==(const RangeInfo& other) const;

	bool operator==(const CurveKeysInfo& other) const;

	bool operator==(const SelectedHandlesInfo& other) const;

private:
	WeakRef<RangeInfo> range;
	Ref<CurveCopyInfo> copyInfo;
};

} // namespace Editor

// --- META ---

DECLARE_CLASS(Editor::CurvesEditor, Editor__CurvesEditor);

DECLARE_CLASS(Editor::CurvesEditor::CurveHandle, Editor__CurvesEditor__CurveHandle);

DECLARE_CLASS(Editor::CurvesEditor::CurveCopyInfo, Editor__CurvesEditor__CurveCopyInfo);
// --- END META ---