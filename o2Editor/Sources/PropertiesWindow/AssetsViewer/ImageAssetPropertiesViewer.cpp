#include "stdafx.h"
#include "ImageAssetPropertiesViewer.h"

#include "Assets/ImageAsset.h"
#include "Core/EditorScope.h"
#include "Core/Properties/Basic/BorderIntProperty.h"
#include "Core/Properties/Basic/EnumProperty.h"
#include "Core/Properties/Basic/ObjectProperty.h"
#include "Core/Properties/Basic/RectangleIntProperty.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/Widgets/Image.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/Spoiler.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Scene/UI/Widget.h"
#include "Scene/UI/WidgetLayout.h"
#include "Utils/Bitmap/Bitmap.h"
#include "Utils/Reflection/Reflection.h"

namespace Editor
{
	ImageAssetPropertiesViewer::ImageAssetPropertiesViewer()
	{
		PushScopeEnterOnStack scope;

		mContent = mnew UIVerticalLayout();
		mContent->spacing = 5.0f;
		mContent->border = BorderF(5, 5, 12, 5);
		mContent->expandHeight = false;
		mContent->expandWidth = true;
		mContent->fitByChildren = true;
		mContent->baseCorner = BaseCorner::Top;

		InitializeImagePreview();
		InitializeProperties();

		mContent->SetLayoutDirty();
	}

	ImageAssetPropertiesViewer::~ImageAssetPropertiesViewer()
	{
		delete mContent;
	}

	void ImageAssetPropertiesViewer::SetTargetAssets(const Vector<AssetRef*>& assets)
	{
		mTargetAssets = assets.Cast<ImageAssetRef*>();

		mBorderProperty->SelectValuesPointers<BorderI, ImageAssetRef>(
			mTargetAssets,
			[](const ImageAssetRef* x) { return &((*x)->GetMeta()->mSliceBorder); });

		mDefaultTypeProperty->SelectValuesPointers<int, ImageAssetRef>(
			mTargetAssets,
			[](const ImageAssetRef* x) { return (int*)&((*x)->GetMeta()->mDefaultMode);});

		mWindowsProperties->SelectValuesPointers<ImageAsset::PlatformMeta, ImageAssetRef>(
			mTargetAssets,
			[](const ImageAssetRef* x) { return &((*x)->GetMeta()->mWindows); });

		mOSXProperties->SelectValuesPointers<ImageAsset::PlatformMeta,  ImageAssetRef>(
			mTargetAssets,
			[](const ImageAssetRef* x) { return &((*x)->GetMeta()->mMacOS); });

		mAndroidProperties->SelectValuesPointers<ImageAsset::PlatformMeta,  ImageAssetRef>(
			mTargetAssets,
			[](const ImageAssetRef* x) { return &((*x)->GetMeta()->mAndroid); });

		mIOSProperties->SelectValuesPointers<ImageAsset::PlatformMeta,  ImageAssetRef>(
			mTargetAssets,
			[](const ImageAssetRef* x) { return &((*x)->GetMeta()->mIOS); });

		mPreviewImage->imageAsset = *mTargetAssets.Last();
		mPreviewImage->GetImage()->mode = SpriteMode::Default;

		FitImage();
		SetupAtlasProperty();
	}

	const Type* ImageAssetPropertiesViewer::GetAssetType() const
	{
		return &TypeOf(ImageAsset);
	}

	UIWidget* ImageAssetPropertiesViewer::GetWidget() const
	{
		return mContent;
	}

	void ImageAssetPropertiesViewer::InitializeImagePreview()
	{
		mPreviewImageContent = mnew UIWidget();
		mPreviewImageContent->layout->minHeight = 200;

		auto separatorImg = o2UI.CreateImage("ui/UI_Separator.png");
		*separatorImg->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, -6, -15, 5, -4);
		mPreviewImageContent->AddChild(separatorImg);

		mPreviewImageBack = mnew UIImage();
		mPreviewImageBack->SetImage(CreateGridSprite());
		*mPreviewImageBack->layout = UIWidgetLayout::BothStretch();
		mPreviewImageContent->AddChild(mPreviewImageBack);

		mPreviewImage = mnew UIImage();
		*mPreviewImage->layout = UIWidgetLayout::BothStretch();
		mPreviewImageContent->AddChild(mPreviewImage);

		InitializeLeftHandle();
		InitializeRightHandle();
		InitializeTopHandle();
		InitializeBottomHandle();

		mContent->AddChild(mPreviewImageContent);
	}

	void ImageAssetPropertiesViewer::InitializeLeftHandle()
	{
		mBorderLeftHandleWidget = mnew UIImage();
		mBorderLeftHandleWidget->SetImage(mnew Sprite(Color4::Green()));
		mPreviewImage->AddChild(mBorderLeftHandleWidget);

		mBorderLeftHandle.cursorType = CursorType::SizeWE;
		mBorderLeftHandleWidget->onDraw += [&]() { mBorderLeftHandle.OnDrawn(); };
		mBorderLeftHandle.isUnderPoint = [&](const Vec2F& p) {
			auto rt = mBorderLeftHandleWidget->layout->GetWorldRect();
			rt.left -= 2; rt.right += 2;
			return rt.IsInside(p);
		};

		mBorderLeftHandle.onMoved = [&](const Input::Cursor& cursor) {
			float px = mPreviewImage->layout->GetWidth()/mPreviewImage->GetImage()->GetOriginalSize().x;
			mBordersSmoothValue.left += cursor.delta.x/px;

			if (mBorderProperty->GetCommonValue().left != (int)Math::Round(mBordersSmoothValue.left))
				UpdateBordersValue();
		};
	}

	void ImageAssetPropertiesViewer::InitializeRightHandle()
	{
		mBorderRightHandleWidget = mnew UIImage();
		mBorderRightHandleWidget->SetImage(mnew Sprite(Color4::Green()));
		mPreviewImage->AddChild(mBorderRightHandleWidget);

		mBorderRightHandle.cursorType = CursorType::SizeWE;
		mBorderRightHandleWidget->onDraw += [&]() { mBorderRightHandle.OnDrawn(); };
		mBorderRightHandle.isUnderPoint = [&](const Vec2F& p) {
			auto rt = mBorderRightHandleWidget->layout->GetWorldRect();
			rt.left -= 2; rt.right += 2;
			return rt.IsInside(p);
		};

		mBorderRightHandle.onMoved = [&](const Input::Cursor& cursor) {
			float px = mPreviewImage->layout->GetWidth()/mPreviewImage->GetImage()->GetOriginalSize().x;
			mBordersSmoothValue.right -= cursor.delta.x/px;

			if (mBorderProperty->GetCommonValue().right != (int)Math::Round(mBordersSmoothValue.right))
				UpdateBordersValue();
		};
	}

	void ImageAssetPropertiesViewer::InitializeTopHandle()
	{
		mBorderTopHandleWidget = mnew UIImage();
		mBorderTopHandleWidget->SetImage(mnew Sprite(Color4::Green()));
		mPreviewImage->AddChild(mBorderTopHandleWidget);

		mBorderTopHandle.cursorType = CursorType::SizeNS;
		mBorderTopHandleWidget->onDraw += [&]() { mBorderTopHandle.OnDrawn(); };
		mBorderTopHandle.isUnderPoint = [&](const Vec2F& p) {
			auto rt = mBorderBottomHandleWidget->layout->GetWorldRect();
			rt.bottom -= 2; rt.top += 2;
			return rt.IsInside(p);
		};

		mBorderTopHandle.onMoved = [&](const Input::Cursor& cursor) {
			float px = mPreviewImage->layout->GetHeight()/mPreviewImage->GetImage()->GetOriginalSize().y;
			mBordersSmoothValue.top += cursor.delta.y/px;

			if (mBorderProperty->GetCommonValue().top != (int)Math::Round(mBordersSmoothValue.top))
				UpdateBordersValue();
		};
	}

	void ImageAssetPropertiesViewer::InitializeBottomHandle()
	{
		mBorderBottomHandleWidget = mnew UIImage();
		mBorderBottomHandleWidget->SetImage(mnew Sprite(Color4::Green()));
		mPreviewImage->AddChild(mBorderBottomHandleWidget);

		mBorderBottomHandle.cursorType = CursorType::SizeNS;
		mBorderBottomHandleWidget->onDraw += [&]() { mBorderBottomHandle.OnDrawn(); };
		mBorderBottomHandle.isUnderPoint = [&](const Vec2F& p) {
			auto rt = mBorderTopHandleWidget->layout->GetWorldRect();
			rt.bottom -= 2; rt.top += 2;
			return rt.IsInside(p);
		};

		mBorderBottomHandle.onMoved = [&](const Input::Cursor& cursor) {
			float px = mPreviewImage->layout->GetHeight()/mPreviewImage->GetImage()->GetOriginalSize().y;
			mBordersSmoothValue.bottom -= cursor.delta.y/px;

			if (mBorderProperty->GetCommonValue().bottom != (int)Math::Round(mBordersSmoothValue.bottom))
				UpdateBordersValue();
		};
	}

	void ImageAssetPropertiesViewer::InitializeProperties()
	{
		auto borderProperty = o2EditorProperties.CreateFieldProperty(&TypeOf(RectI), "Slice border");
		mBorderProperty = (BorderIProperty*)borderProperty;
		mBorderProperty->onChanged += [&](auto x) { UpdateBordersAnchors(); /*mBordersSmoothValue = mBorderProperty->GetCommonValue();*/ };
		mContent->AddChild(borderProperty);

		auto modeProperty = o2EditorProperties.CreateFieldProperty(&TypeOf(SpriteMode), "Default mode");
		mDefaultTypeProperty = (EnumProperty*)modeProperty;
		mDefaultTypeProperty->SpecializeType(&TypeOf(SpriteMode));
		mContent->AddChild(modeProperty);

		auto atlasProperty = o2EditorProperties.CreateFieldProperty(&TypeOf(AtlasAssetRef), "Atlas");
		mAtlasProperty = (AssetProperty<AtlasAssetRef>*)atlasProperty;
		mAtlasProperty->onChanged = THIS_FUNC(OnAtlasPropertyChanged);
		mContent->AddChild(atlasProperty);

		auto windowsProperty = o2EditorProperties.CreateFieldProperty(&TypeOf(ImageAsset::PlatformMeta), "Windows");
		mWindowsProperties = (ObjectProperty*)windowsProperty;
		mWindowsProperties->SpecializeType(&TypeOf(ImageAsset::PlatformMeta));
		mContent->AddChild(windowsProperty);

		auto osxProperty = o2EditorProperties.CreateFieldProperty(&TypeOf(ImageAsset::PlatformMeta), "OSX");
		mOSXProperties = (ObjectProperty*)osxProperty;
		mOSXProperties->SpecializeType(&TypeOf(ImageAsset::PlatformMeta));
		mContent->AddChild(osxProperty);

		auto androidProperty = o2EditorProperties.CreateFieldProperty(&TypeOf(ImageAsset::PlatformMeta), "Android");
		mAndroidProperties = (ObjectProperty*)androidProperty;
		mAndroidProperties->SpecializeType(&TypeOf(ImageAsset::PlatformMeta));
		mContent->AddChild(androidProperty);

		auto iosProperty = o2EditorProperties.CreateFieldProperty(&TypeOf(ImageAsset::PlatformMeta), "iOS");
		mIOSProperties = (ObjectProperty*)iosProperty;
		mIOSProperties->SpecializeType(&TypeOf(ImageAsset::PlatformMeta));
		mContent->AddChild(iosProperty);
	}

	void ImageAssetPropertiesViewer::FitImage()
	{
		mContent->UpdateTransform();

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
			*mPreviewImage->layout = UIWidgetLayout(Vec2F(0.5f - d, 0.0f), Vec2F(0.5f + d, 1.0f), Vec2F(), Vec2F());
		}
		else
		{
			float d = 1.0f/imageSizeAspect*0.5f*contentAspect;
			*mPreviewImage->layout = UIWidgetLayout(Vec2F(0.0f, 0.5f - d), Vec2F(1.0f, 0.5f + d), Vec2F(), Vec2F());
		}

		*mPreviewImageBack->layout = *mPreviewImage->layout;

		UpdateBordersAnchors();
		mBordersSmoothValue = mBorderProperty->GetCommonValue();
	}

	void ImageAssetPropertiesViewer::UpdateBordersAnchors()
	{
		Vec2F imageSize = mPreviewImage->GetImage()->GetOriginalSize();
		BorderI borders = mBorderProperty->GetCommonValue();
		BorderF bordersAnchors((float)borders.left/imageSize.x, (float)borders.top/imageSize.y,
							   1.0f - (float)borders.right/imageSize.x, 1.0f - (float)borders.bottom/imageSize.y);

		*mBorderLeftHandleWidget->layout = UIWidgetLayout(Vec2F(bordersAnchors.left, 0.0f), Vec2F(bordersAnchors.left, 1.0f), Vec2F(0, 0), Vec2F(1, 0));
		*mBorderRightHandleWidget->layout = UIWidgetLayout(Vec2F(bordersAnchors.right, 0.0f), Vec2F(bordersAnchors.right, 1.0f), Vec2F(0, 0), Vec2F(1, 0));
		*mBorderTopHandleWidget->layout = UIWidgetLayout(Vec2F(0.0f, bordersAnchors.top), Vec2F(1.0f, bordersAnchors.top), Vec2F(0, 0), Vec2F(0, 1));
		*mBorderBottomHandleWidget->layout = UIWidgetLayout(Vec2F(0.0f, bordersAnchors.bottom), Vec2F(1.0f, bordersAnchors.bottom), Vec2F(0, 0), Vec2F(0, 1));
	}

	void ImageAssetPropertiesViewer::UpdateBordersValue()
	{
		BorderI value = mBordersSmoothValue;
		for (auto target : mTargetAssets)
			(*target)->GetMeta()->mSliceBorder = value;

		mBorderProperty->Refresh();
		UpdateBordersAnchors();
	}

	void ImageAssetPropertiesViewer::SetupAtlasProperty()
	{
		if (mTargetAssets.IsEmpty())
			return;

		UID commonValue = (*mTargetAssets[0])->GetAtlasId();
		bool valuesDifferent = false;

		for (int i = 1; i < mTargetAssets.Count(); i++)
		{
			UID id = (*mTargetAssets[i])->GetAtlasId();
			if (id != commonValue)
			{
				valuesDifferent = true;
				break;
			}
		}

		if (valuesDifferent)
			mAtlasProperty->SetUnknownValue();
		else
			mAtlasProperty->SetAssetId(commonValue);
	}

	void ImageAssetPropertiesViewer::OnAtlasPropertyChanged(IPropertyField* field)
	{
		UID id = mAtlasProperty->GetCommonValue()->GetAssetId();

		for (auto target : mTargetAssets)
			(*target)->SetAtlasId(id);
	}

	Sprite* ImageAssetPropertiesViewer::CreateGridSprite()
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

}

DECLARE_CLASS(Editor::ImageAssetPropertiesViewer);
