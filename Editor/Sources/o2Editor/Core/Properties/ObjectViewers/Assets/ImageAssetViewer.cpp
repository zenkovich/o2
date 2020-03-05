#include "o2Editor/stdafx.h"
#include "ImageAssetViewer.h"

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/BorderIntProperty.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2/Utils/Editor/DragHandle.h"

namespace Editor
{
	ImageAssetViewer::ImageAssetViewer() :
		DefaultObjectViewer()
	{
		PushEditorScopeOnStack scope;

		InitializeImagePreview();
	}

	void ImageAssetViewer::Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		DefaultObjectViewer::Refresh(targetObjets);

		mBorderProperty = dynamic_cast<BorderIProperty*>(
			mPropertiesContext.properties[TypeOf(ImageAsset).GetField("sliceBorder")]);

		mPreviewImage->SetImageAsset(ImageAssetRef(dynamic_cast<ImageAsset*>(targetObjets.Last().first)->GetUID()));
		mPreviewImage->GetImage()->mode = SpriteMode::Default;

		FitImage();
	}

	const Type* ImageAssetViewer::GetViewingObjectType() const
	{
		return GetViewingObjectTypeStatic();
	}

	const Type* ImageAssetViewer::GetViewingObjectTypeStatic()
	{
		return &TypeOf(ImageAsset);
	}

	void ImageAssetViewer::InitializeImagePreview()
	{
		mPreviewImageContent = mnew Widget();
		mPreviewImageContent->layout->minHeight = 200;

		auto separatorImg = o2UI.CreateImage("ui/UI4_Separator.png");
		*separatorImg->layout = WidgetLayout::HorStretch(VerAlign::Bottom, -6, -15, 5, -4);
		mPreviewImageContent->AddChild(separatorImg);

		mPreviewImageBack = mnew Image();
		mPreviewImageBack->SetImage(CreateGridSprite());
		*mPreviewImageBack->layout = WidgetLayout::BothStretch();
		mPreviewImageContent->AddChild(mPreviewImageBack);

		mPreviewImage = mnew PreviewImage();
		*mPreviewImage->layout = WidgetLayout::BothStretch();
		mPreviewImageContent->AddChild(mPreviewImage);

		InitializeSliceHandles();

		mLayout->AddChild(mPreviewImageContent);
	}

	void ImageAssetViewer::InitializeSliceHandles()
	{
		// Left handle
		mBorderLeftHandle = mnew WidgetDragHandle(mnew Sprite("ui/ver_slice_line.png"));

		mBorderLeftHandle->localToWidgetOffsetTransformFunc = [&](const Vec2F& point) {
			return point / mPreviewImage->GetImage()->GetOriginalSize() * mPreviewImage->layout->GetSize();
		};

		mBorderLeftHandle->widgetOffsetToLocalTransformFunc = [&](const Vec2F& point) {
			return point / mPreviewImage->layout->GetSize() * mPreviewImage->GetImage()->GetOriginalSize();
		};

		mBorderLeftHandle->checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(Math::Clamp(point.x, 0.0f, (float)mPreviewImage->GetImage()->GetOriginalSize().x), mPreviewImage->GetImage()->GetOriginalSize().y*0.5f);
		};

		mBorderLeftHandle->onChangedPos = [&](const Vec2F& point) {
			mBordersSmoothValue.left = point.x;
			UpdateBordersValue();
		};

		mBorderLeftHandle->cursorType = CursorType::SizeWE;

		// Right handle
		mBorderRightHandle = mnew WidgetDragHandle(*mBorderLeftHandle);

		mBorderRightHandle->onChangedPos = [&](const Vec2F& point) {
			mBordersSmoothValue.right = (float)mPreviewImage->GetImage()->GetOriginalSize().x - point.x;
			UpdateBordersValue();
		};

		mPreviewImage->AddChild(mBorderLeftHandle);
		mPreviewImage->AddChild(mBorderRightHandle);

		// Top handle
		mBorderTopHandle = mnew WidgetDragHandle(mnew Sprite("ui/hor_slice_line.png"));

		mBorderTopHandle->localToWidgetOffsetTransformFunc = [&](const Vec2F& point) {
			return point / mPreviewImage->GetImage()->GetOriginalSize() * mPreviewImage->layout->GetSize();
		};

		mBorderTopHandle->widgetOffsetToLocalTransformFunc = [&](const Vec2F& point) {
			return point / mPreviewImage->layout->GetSize() * mPreviewImage->GetImage()->GetOriginalSize();
		};

		mBorderTopHandle->checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(mPreviewImage->GetImage()->GetOriginalSize().x*0.5f, Math::Clamp(point.y, 0.0f, (float)mPreviewImage->GetImage()->GetOriginalSize().y));
		};

		mBorderTopHandle->onChangedPos = [&](const Vec2F& point) {
			mBordersSmoothValue.top = (float)mPreviewImage->GetImage()->GetOriginalSize().y - point.y;
			UpdateBordersValue();
		};

		mBorderTopHandle->cursorType = CursorType::SizeNS;

		// Bottom handle
		mBorderBottomHandle = mnew WidgetDragHandle(*mBorderTopHandle);

		mBorderBottomHandle->onChangedPos = [&](const Vec2F& point) {
			mBordersSmoothValue.bottom = point.y;
			UpdateBordersValue();
		};

		mPreviewImage->AddChild(mBorderTopHandle);
		mPreviewImage->AddChild(mBorderBottomHandle);
	}

	void ImageAssetViewer::FitImage()
	{
		mLayout->UpdateTransform();

		Vec2F maxSize = mPreviewImageContent->layout->size;
		Vec2F imageSize = mPreviewImage->GetImage()->GetOriginalSize();

		float heightFitScale = maxSize.y/imageSize.y;
		float widthFitDifference = maxSize.x - imageSize.x*heightFitScale;

		float widthFitScale = maxSize.x/imageSize.x;
		float heightFitDifference = maxSize.y - imageSize.y*widthFitScale;

		bool fitByHeight = true;
		if (widthFitDifference > 0.0f && heightFitDifference > 0.0f)
			fitByHeight = heightFitDifference < widthFitDifference;
		else if (widthFitDifference > 0.0f)
			fitByHeight = true;
		else
			fitByHeight = false;

		float imageSizeAspect = imageSize.x/imageSize.y;
		float contentAspect = maxSize.x/maxSize.y;

		if (fitByHeight)
		{
			float d = imageSizeAspect*0.5f/contentAspect;
			*mPreviewImage->layout = WidgetLayout(Vec2F(0.5f - d, 0.0f), Vec2F(0.5f + d, 1.0f), Vec2F(), Vec2F());
		}
		else
		{
			float d = 1.0f/imageSizeAspect*0.5f*contentAspect;
			*mPreviewImage->layout = WidgetLayout(Vec2F(0.0f, 0.5f - d), Vec2F(1.0f, 0.5f + d), Vec2F(), Vec2F());
		}

		*mPreviewImageBack->layout = *mPreviewImage->layout;

		UpdateBordersAnchors();
		mBordersSmoothValue = mBorderProperty->GetCommonValue();
	}

	void ImageAssetViewer::UpdateBordersAnchors()
	{
		BorderI borders = mBorderProperty->GetCommonValue();
		Vec2F imageSize = mPreviewImage->GetImage()->GetOriginalSize();

		mPreviewImage->UpdateTransform();
		mBorderLeftHandle->GetRegularSprite()->SetSize(Vec2F(10.0f, mPreviewImage->layout->GetHeight()));
		mBorderRightHandle->GetRegularSprite()->SetSize(Vec2F(10.0f, mPreviewImage->layout->GetHeight()));
		mBorderTopHandle->GetRegularSprite()->SetSize(Vec2F(mPreviewImage->layout->GetWidth(), 10.0f));
		mBorderBottomHandle->GetRegularSprite()->SetSize(Vec2F(mPreviewImage->layout->GetWidth(), 10.0f));

		mBorderLeftHandle->SetPosition(Vec2F((float)borders.left, 0));
		mBorderRightHandle->SetPosition(Vec2F(imageSize.x - (float)borders.right, 0));
		mBorderTopHandle->SetPosition(Vec2F(0, imageSize.y - (float)borders.top));
		mBorderBottomHandle->SetPosition(Vec2F(0, (float)borders.bottom));
	}

	void ImageAssetViewer::UpdateBordersValue()
	{
		BorderI value = mBordersSmoothValue;
		for (auto target : mPropertiesContext.targets)
			dynamic_cast<ImageAsset*>(target.first)->sliceBorder = value;

		mBorderProperty->Refresh();
		UpdateBordersAnchors();
	}

	Sprite* ImageAssetViewer::CreateGridSprite()
	{
		Color4 color1(1.0f, 1.0f, 1.0f, 1.0f), color2(0.7f, 0.7f, 0.7f, 1.0f);
		Bitmap backLayerBitmap(PixelFormat::R8G8B8A8, Vec2I(20, 20));
		backLayerBitmap.Fill(color1);
		backLayerBitmap.FillRect(0, 10, 10, 0, color2);
		backLayerBitmap.FillRect(10, 20, 20, 10, color2);

		Sprite* res = mnew Sprite(&backLayerBitmap);
		res->SetMode(SpriteMode::Tiled);
		return res;
	}

	void ImageAssetViewer::PreviewImage::Draw()
	{
		mIsClipped = false;
		auto prevFilter = mImage->GetTexture()->GetFilter();
		mImage->GetTexture()->SetFilter(Texture::Filter::Nearest);

		Image::Draw();

		mImage->GetTexture()->SetFilter(prevFilter);
	}


}

DECLARE_CLASS(Editor::ImageAssetViewer);

DECLARE_CLASS(Editor::ImageAssetViewer::PreviewImage);
