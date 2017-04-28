#include "CurveEdit.h"

#include "Application/Application.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/VerticalScrollBar.h"

namespace Editor
{
	UICurveEditor::UICurveEditor():
		UIFrameScrollView()
	{
		mReady = false;

		mSelectionSprite = mnew Sprite();
		mTextFont = FontRef("stdFont.ttf");
		mTextFont->CheckCharacters("0123456789.,+-", 10);

		InitializeTextDrawables();

		mBackColor = Color4(130, 130, 130, 255);

		mReady = true;
	}

	UICurveEditor::UICurveEditor(const UICurveEditor& other):
		UIFrameScrollView(other), mSelectionSprite(other.mSelectionSprite->Clone()), mTextFont(other.mTextFont)
	{
		mReady = false;

		InitializeTextDrawables();
		RetargetStatesAnimations();

		mReady = true;
	}

	UICurveEditor::~UICurveEditor()
	{
		delete mSelectionSprite;
		delete mTextLeft;
		delete mTextRight;
		delete mTextTop;
		delete mTextBottom;
	}

	Editor::UICurveEditor& UICurveEditor::operator=(const UICurveEditor& other)
	{
		UIFrameScrollView::operator=(other);

		mReady = false;

		delete mSelectionSprite;
		delete mTextLeft;
		delete mTextRight;
		delete mTextTop;
		delete mTextBottom;

		mSelectionSprite = other.mSelectionSprite->Clone();
		mTextFont        = other.mTextFont;

		mMainHandleSample = other.mMainHandleSample;
		mSupportHandleSample = other.mSupportHandleSample;

		InitializeTextDrawables();
		RetargetStatesAnimations();

		mReady = true;

		return *this;
	}

	void UICurveEditor::Draw()
	{
		UIFrameScrollView::Draw();

		DrawHandles();
		DrawSelection();
	}

	void UICurveEditor::Update(float dt)
	{
		UIFrameScrollView::Update(dt);
	}

	void UICurveEditor::AddEditingCurve(Curve* curve, const Color4& color /*= Color4(-1, -1, -1, -1)*/)
	{
		CurveInfo* info = mnew CurveInfo();
		info->curve = curve;
		info->viewScale = Vec2F();
		info->UpdateApproximatedPoints();

		if (color == Color4(-1, -1, -1, -1))
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
		{
			mViewCameraTargetScale = Math::Min(mAvailableArea.Width()/mViewCamera.GetSize().x,
											   mAvailableArea.Height()/mViewCamera.GetSize().y);
		}
	}

	void UICurveEditor::RemoveEditingCurve(Curve* curve)
	{
		for (auto info : mCurves)
		{
			if (info->curve == curve)
			{
				for (auto handle : info->handles)
				{
					mHandles.Remove(&handle->mainHandle);
					mHandles.Remove(&handle->leftSupportHandle);
					mHandles.Remove(&handle->rightSupportHandle);
					mSelectedHandles.Remove(&handle->mainHandle);
					mSelectedHandles.Remove(&handle->leftSupportHandle);
					mSelectedHandles.Remove(&handle->rightSupportHandle);
				}

				delete info;
				mCurves.Remove(info);
				break;
			}
		}
	}

	void UICurveEditor::RemoveAllEditingCurves()
	{
		auto curves = mCurves;
		for (auto info : curves)
			RemoveEditingCurve(info->curve);
	}

	void UICurveEditor::AddCurvesRange(Curve* curveA, Curve* curveB, const Color4& color /*= Color4(-1, -1, -1, -1)*/)
	{

	}

	void UICurveEditor::RemoveCurvesRange(Curve* curveA, Curve* curveB)
	{

	}

	void UICurveEditor::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		UIFrameScrollView::UpdateLayout(forcible, withChildren);
	}

	void UICurveEditor::SetSelectionSpriteImage(const ImageAssetRef& image)
	{
		mSelectionSprite->LoadFromImage(image);
	}

	void UICurveEditor::SetTextFont(const FontRef& font)
	{
		mTextFont = font;
		mTextLeft->SetFont(mTextFont);
		mTextRight->SetFont(mTextFont);
		mTextTop->SetFont(mTextFont);
		mTextBottom->SetFont(mTextFont);
	}

	void UICurveEditor::SetMainHandleImages(const ImageAssetRef& regular, const ImageAssetRef& hover,
											const ImageAssetRef& pressed, const ImageAssetRef& selected)
	{
		mMainHandleSample = SelectableDragHandle(mnew Sprite(regular), mnew Sprite(hover),
												 mnew Sprite(pressed), mnew Sprite(selected));
	}

	void UICurveEditor::SetSupportHandleImages(const ImageAssetRef& regular, const ImageAssetRef& hover, const ImageAssetRef& pressed, const ImageAssetRef& selected)
	{
		mSupportHandleSample = SelectableDragHandle(mnew Sprite(regular), mnew Sprite(hover),
													mnew Sprite(pressed), mnew Sprite(selected));
	}

	void UICurveEditor::InitializeTextDrawables()
	{
		mTextLeft = mnew Text(mTextFont);
		mTextLeft->SetHeight(10);
		mTextLeft->SetHorAlign(HorAlign::Left);
		mTextLeft->SetVerAlign(VerAlign::Bottom);

		mTextRight = mnew Text(mTextFont);
		mTextRight->SetHeight(10);
		mTextRight->SetHorAlign(HorAlign::Right);
		mTextRight->SetVerAlign(VerAlign::Bottom);

		mTextTop = mnew Text(mTextFont);
		mTextTop->SetHeight(10);
		mTextTop->SetHorAlign(HorAlign::Left);
		mTextTop->SetVerAlign(VerAlign::Bottom);
		mTextTop->SetAngleDegrees(-90.0f);

		mTextBottom = mnew Text(mTextFont);
		mTextBottom->SetHeight(10);
		mTextBottom->SetHorAlign(HorAlign::Right);
		mTextBottom->SetVerAlign(VerAlign::Bottom);
		mTextBottom->SetAngleDegrees(-90.0f);
	}

	void UICurveEditor::RecalculateViewArea()
	{
		if (mCurves.IsEmpty())
		{
			mAvailableArea = RectF(0, 0, 1, 1);
			return;
		}

		Vec2F firstPoint = mCurves.Last()->approximatedPoints.Last();
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

	void UICurveEditor::RedrawContent()
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

	void UICurveEditor::DrawGrid()
	{
		float cameraMaxSize = Math::Max(mViewCamera.size->x*mViewCamera.scale->x,
										mViewCamera.size->y*mViewCamera.scale->y);

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

		Vec2F gridOrigin(Math::Round(mViewCamera.position->x / cellSize)*cellSize,
						 Math::Round(mViewCamera.position->y / cellSize)*cellSize);

		int cellsCount = Math::CeilToInt(cameraMaxSize / cellSize);
		float tenCeilsSize = cellSize*10.0f;
		float screenCellSize = cellSize / mViewCamera.scale->x;
		Color4 cellColorSmoothed = Math::Lerp(mGridColor, mBackColor, 0.7f);

		mTextLeft->SetScale(mViewCamera.GetScale());
		mTextRight->SetScale(mViewCamera.GetScale());
		mTextTop->SetScale(mViewCamera.GetScale());
		mTextBottom->SetScale(mViewCamera.GetScale());

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

			o2Render.DrawLine(xBegin, xEnd, yTen ? mGridColor : cellColorSmoothed);
			o2Render.DrawLine(yBegin, yEnd, xTen ? mGridColor : cellColorSmoothed);
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

			mTextLeft->SetText(yCaption);
			mTextLeft->SetPosition(Vec2F(mViewCamera.GetRect().left, y));
			mTextLeft->Draw();

			mTextRight->SetText(yCaption);
			mTextRight->SetPosition(Vec2F(mViewCamera.GetRect().right, y));
			mTextRight->Draw();

			mTextTop->SetText(xCaption);
			mTextTop->SetPosition(Vec2F(x, mViewCamera.GetRect().top));
			mTextTop->Draw();

			mTextBottom->SetText(xCaption);
			mTextBottom->SetPosition(Vec2F(x, mViewCamera.GetRect().bottom));
			mTextBottom->Draw();
		}
	}

	void UICurveEditor::DrawCurves()
	{
		for (auto curve : mCurves)
		{
			if (curve->approximatedPoints.IsEmpty())
				continue;

			o2Render.DrawLine(curve->approximatedPoints, curve->color);

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
	}

	void UICurveEditor::DrawHandles()
	{
		for (auto curve : mCurves)
		{
			for (auto keyHandles : curve->handles)
				keyHandles->Draw();
		}
	}

	void UICurveEditor::DrawSelection()
	{
		if (mIsPressed)
		{
			mSelectionSprite->rect = RectF(LocalToScreenPoint(mSelectingPressedPoint), o2Input.cursorPos);
			mSelectionSprite->Draw();
		}
	}

	void UICurveEditor::AddCurveKeyHandles(CurveInfo* info, int keyId)
	{
		KeyHandles* keyHandles = mnew KeyHandles(mMainHandleSample, mSupportHandleSample, this);
		keyHandles->curveKeyIdx = keyId;

		Curve::Key curveKey = info->curve->GetKeyAt(keyId);
		Curve::Key lastCurveKey = info->curve->GetKeyAt(Math::Max(0, keyId - 1));
		Curve::Key nextCurveKey = info->curve->GetKeyAt(Math::Min(keyId + 1, info->curve->GetKeys().Count()));

		// main handle
		keyHandles->mainHandle.SetPosition(Vec2F(curveKey.position, curveKey.value));
		keyHandles->mainHandle.onChangedPos = [=](const Vec2F& pos) { OnCurveKeyMainHandleDragged(info, keyHandles, pos); };
		keyHandles->mainHandle.localToScreenTransformFunc = [&](const Vec2F& p) { return LocalToScreenPoint(p); };
		keyHandles->mainHandle.screenToLocalTransformFunc = [&](const Vec2F& p) { return ScreenToLocalPoint(p); };


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

		info->handles.Insert(keyHandles, keyId);

		mHandles.Add(&keyHandles->mainHandle);
		mHandles.Add(&keyHandles->leftSupportHandle);
		mHandles.Add(&keyHandles->rightSupportHandle);

		keyHandles->mainHandle.SetSelectionGroup(this);
		keyHandles->leftSupportHandle.SetSelectionGroup(this);
		keyHandles->rightSupportHandle.SetSelectionGroup(this);
	}

	void UICurveEditor::OnCurveKeyMainHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
	{
		Curve::Key key = info->curve->GetKeyAt(handles->curveKeyIdx);

		key.position = position.x;
		key.value = position.y;

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
				info->handles[i]->leftSupportHandle.enabled = i > 0;
				info->handles[i]->rightSupportHandle.enabled = i < info->curve->GetKeys().Count() - 1;
			}
		}

		info->curve->SetKey(key, handles->curveKeyIdx);

		info->UpdateHandles();
		info->UpdateApproximatedPoints();

		RecalculateViewArea();
		mNeedRedraw = true;
	}

	void UICurveEditor::OnCurveKeyLeftSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
	{
		Curve::Key key = info->curve->GetKeyAt(handles->curveKeyIdx);
		Curve::Key prevKey = info->curve->GetKeyAt(Math::Max(handles->curveKeyIdx - 1, 0));

		key.leftSupportPosition = position.x - key.position;
		key.leftSupportValue = position.y - key.value;

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

		RecalculateViewArea();
		mNeedRedraw = true;
	}

	void UICurveEditor::OnCurveKeyRightSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
	{
		Curve::Key key = info->curve->GetKeyAt(handles->curveKeyIdx);
		Curve::Key nextKey = info->curve->GetKeyAt(Math::Min(handles->curveKeyIdx + 1, info->curve->GetKeys().Count() - 1));

		key.rightSupportPosition = position.x - key.position;
		key.rightSupportValue = position.y - key.value;

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

		RecalculateViewArea();
		mNeedRedraw = true;
	}

	Vec2F UICurveEditor::CheckLeftSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
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

	Vec2F UICurveEditor::CheckRightSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position)
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

	void UICurveEditor::OnCursorDblClicked(const Input::Cursor& cursor)
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
					Vec2F ac = cursor.position - LocalToScreenPoint(a);
					Vec2F lac = ScreenToLocalPoint(cursor.position) - a;

					float pointDistance = (LocalToScreenPoint(b) - cursor.position).Length();
					float lineDistance = Math::Abs(abn.Perpendicular().Dot(ac));
					float proj = abn.Dot(lac);

					if (pointDistance < createPointDistanceThreshold ||
						(lineDistance < createPointDistanceThreshold && proj > 0.0f && proj < abl))
					{
						const Curve::Key& lastKey = keys[keyIdx - 1];

						newKey.position = localCursorPos.x;
						newKey.value = localCursorPos.y;
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

		if (clickedCurveInfo)
		{
			int idx = clickedCurveInfo->curve->InsertKey(newKey);

			for (int i = idx; i < clickedCurveInfo->handles.Count(); i++)
				clickedCurveInfo->handles[i]->curveKeyIdx++;

			AddCurveKeyHandles(clickedCurveInfo, idx);
			clickedCurveInfo->UpdateApproximatedPoints();

			mNeedRedraw = true;
		}
	}

	void UICurveEditor::SmoothKey(CurveInfo* info, int idx)
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

	void UICurveEditor::OnCursorPressed(const Input::Cursor& cursor)
	{
		mSelectingPressedPoint = ScreenToLocalPoint(cursor.position);

		if (!o2Input.IsKeyDown(VK_SHIFT))
		{
			auto handles = mSelectedHandles;
			for (auto handle : handles)
				handle->SetSelected(false);

			mSelectedHandles.Clear();
		}
	}

	void UICurveEditor::OnCursorReleased(const Input::Cursor& cursor)
	{
		mSelectedHandles.Add(mSelectingHandlesBuf);
		mSelectingHandlesBuf.Clear();
		CheckHandlesVisible();
	}

	void UICurveEditor::OnCursorStillDown(const Input::Cursor& cursor)
	{
		for (auto handle : mSelectingHandlesBuf)
			handle->SetSelected(false);

		mSelectingHandlesBuf.Clear();

		RectF selectionLocalRect(mSelectingPressedPoint, ScreenToLocalPoint(cursor.position));

		for (auto handle : mHandles)
		{
			if (handle->IsEnabled() && selectionLocalRect.IsInside(handle->GetPosition()) &&
				!mSelectedHandles.Contains(handle))
			{
				mSelectingHandlesBuf.Add(handle);
				handle->SetSelected(true);
			}
		}
	}

	void UICurveEditor::CheckHandlesVisible()
	{
		for (auto info : mCurves)
		{
			for (auto handles : info->handles)
			{
				handles->leftSupportHandle.enabled = (handles->mainHandle.IsSelected() ||
													  handles->leftSupportHandle.IsSelected()) && handles->curveKeyIdx > 0;

				handles->rightSupportHandle.enabled = (handles->mainHandle.IsSelected() ||
													   handles->rightSupportHandle.IsSelected()) && handles->curveKeyIdx < info->handles.Count() - 1;
			}
		}
	}

	UICurveEditor::CurveInfo::CurveInfo()
	{

	}

	UICurveEditor::CurveInfo::~CurveInfo()
	{

	}

	void UICurveEditor::CurveInfo::UpdateHandles()
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

	void UICurveEditor::CurveInfo::UpdateApproximatedPoints()
	{
		approximatedPoints.Clear();

		auto keys = curve->GetKeys();
		for (int i = 1; i < keys.Count(); i++)
		{
			const Vec2F* keyPoints = keys[i].GetApproximatedPoints();
			for (int i = 0; i < keys[i].GetApproximatedPointsCount(); i++)
				approximatedPoints.Add(keyPoints[i]);
		}
	}

	UICurveEditor::RangeInfo::RangeInfo()
	{

	}

	UICurveEditor::RangeInfo::~RangeInfo()
	{

	}

	void UICurveEditor::RangeInfo::UpdateMesh()
	{

	}

	UICurveEditor::KeyHandles::KeyHandles(const SelectableDragHandle& mainSample, const SelectableDragHandle& supportSample,
										  UICurveEditor* editor):
		mainHandle(mainSample), leftSupportHandle(supportSample), rightSupportHandle(supportSample), curveEditor(editor)
	{}

	void UICurveEditor::KeyHandles::Draw()
	{
		bool selected = true;

		Vec2F mainHandleScreenPos = curveEditor->LocalToScreenPoint(mainHandle.GetPosition());
		Vec2F leftSupportHandleScreenPos = curveEditor->LocalToScreenPoint(leftSupportHandle.GetPosition());
		Vec2F rightSupportHandleScreenPos = curveEditor->LocalToScreenPoint(rightSupportHandle.GetPosition());

		if (leftSupportHandle.enabled)
			o2Render.DrawLine(mainHandleScreenPos, leftSupportHandleScreenPos, curveEditor->mGridColor);

		if (rightSupportHandle.enabled)
			o2Render.DrawLine(mainHandleScreenPos, rightSupportHandleScreenPos, curveEditor->mGridColor);

		mainHandle.Draw();
		leftSupportHandle.Draw();
		rightSupportHandle.Draw();
	}

}

CLASS_META(Editor::UICurveEditor)
{
	BASE_CLASS(Editor::UIFrameScrollView);
	BASE_CLASS(o2::SelectableDragHandlesGroup);

	PROTECTED_FIELD(mMainHandleSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSupportHandleSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCurves);
	PROTECTED_FIELD(mRanges);
	PROTECTED_FIELD(mSelectingHandlesBuf);
	PROTECTED_FIELD(mSelectionSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTextFont).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTextLeft);
	PROTECTED_FIELD(mTextRight);
	PROTECTED_FIELD(mTextTop);
	PROTECTED_FIELD(mTextBottom);
	PROTECTED_FIELD(mSelectingPressedPoint);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, AddEditingCurve, Curve*, const Color4&);
	PUBLIC_FUNCTION(void, RemoveEditingCurve, Curve*);
	PUBLIC_FUNCTION(void, RemoveAllEditingCurves);
	PUBLIC_FUNCTION(void, AddCurvesRange, Curve*, Curve*, const Color4&);
	PUBLIC_FUNCTION(void, RemoveCurvesRange, Curve*, Curve*);
	PUBLIC_FUNCTION(void, SetSelectionSpriteImage, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, SetTextFont, const FontRef&);
	PUBLIC_FUNCTION(void, SetMainHandleImages, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, SetSupportHandleImages, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PROTECTED_FUNCTION(void, InitializeTextDrawables);
	PROTECTED_FUNCTION(void, RecalculateViewArea);
	PROTECTED_FUNCTION(void, RedrawContent);
	PROTECTED_FUNCTION(void, DrawGrid);
	PROTECTED_FUNCTION(void, DrawCurves);
	PROTECTED_FUNCTION(void, DrawHandles);
	PROTECTED_FUNCTION(void, DrawSelection);
	PROTECTED_FUNCTION(void, AddCurveKeyHandles, CurveInfo*, int);
	PROTECTED_FUNCTION(void, OnCurveKeyMainHandleDragged, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCurveKeyLeftSupportHandleDragged, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCurveKeyRightSupportHandleDragged, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftSupportHandlePosition, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightSupportHandlePosition, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCursorDblClicked, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SmoothKey, CurveInfo*, int);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, CheckHandlesVisible);
}
END_META;
 