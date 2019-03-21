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
#include "Render/Sprite.h"
#include "Render/Texture.h"

namespace Editor
{
	ImageAssetPropertiesViewer::ImageAssetPropertiesViewer()
	{
		PushScopeEnterOnStack scope;

		mContent = mnew VerticalLayout();
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

		mBorderProperty->SelectValuesPointers<BorderI, ImageAssetRef>(mTargetAssets,
			[](const ImageAssetRef* x) { return &((*x)->GetMeta()->mSliceBorder); });

		mDefaultTypeProperty->SelectValuesPointers<int, ImageAssetRef>(mTargetAssets,
			[](const ImageAssetRef* x) { return (int*)&((*x)->GetMeta()->mDefaultMode);});

		mWindowsProperties->SelectValuesPointers<ImageAsset::PlatformMeta, ImageAssetRef>(mTargetAssets,
			[](const ImageAssetRef* x) { return &((*x)->GetMeta()->mWindows); });

		mOSXProperties->SelectValuesPointers<ImageAsset::PlatformMeta,  ImageAssetRef>(mTargetAssets,
			[](const ImageAssetRef* x) { return &((*x)->GetMeta()->mMacOS); });

		mAndroidProperties->SelectValuesPointers<ImageAsset::PlatformMeta,  ImageAssetRef>(mTargetAssets,
			[](const ImageAssetRef* x) { return &((*x)->GetMeta()->mAndroid); });

		mIOSProperties->SelectValuesPointers<ImageAsset::PlatformMeta,  ImageAssetRef>(mTargetAssets,
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

	Widget* ImageAssetPropertiesViewer::GetWidget() const
	{
		return mContent;
	}

	void ImageAssetPropertiesViewer::InitializeImagePreview()
	{
		mPreviewImageContent = mnew Widget();
		mPreviewImageContent->layout->minHeight = 200;

		auto separatorImg = o2UI.CreateImage("ui/UI_Separator.png");
		*separatorImg->layout = WidgetLayout::HorStretch(VerAlign::Bottom, -6, -15, 5, -4);
		mPreviewImageContent->AddChild(separatorImg);

		mPreviewImageBack = mnew Image();
		mPreviewImageBack->SetImage(CreateGridSprite());
		*mPreviewImageBack->layout = WidgetLayout::BothStretch();
		mPreviewImageContent->AddChild(mPreviewImageBack);

		mPreviewImage = mnew PreviewImage();
		*mPreviewImage->layout = WidgetLayout::BothStretch();
		mPreviewImageContent->AddChild(mPreviewImage);

		InitializeLeftHandle();
		InitializeRightHandle();
		InitializeTopHandle();
		InitializeBottomHandle();

		mContent->AddChild(mPreviewImageContent);
	}

	void ImageAssetPropertiesViewer::InitializeLeftHandle()
	{
		mBorderLeftHandle = new WidgetDragHandle(mnew Sprite(Color4(0.0f, 1.0f, 0.0f, 1.0f)), 
												 mnew Sprite(Color4(0.5f, 1.0f, 0.5f, 1.0f)), 
												 mnew Sprite(Color4(0.0f, 0.6f, 0.0f, 1.0f)));

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

		mPreviewImage->AddChild(mBorderLeftHandle);
	}

	void ImageAssetPropertiesViewer::InitializeRightHandle()
	{
		mBorderRightHandle = new WidgetDragHandle(mnew Sprite(Color4(0.0f, 1.0f, 0.0f, 1.0f)),
												  mnew Sprite(Color4(0.5f, 1.0f, 0.5f, 1.0f)),
												  mnew Sprite(Color4(0.0f, 0.6f, 0.0f, 1.0f)));

		mBorderRightHandle->localToWidgetOffsetTransformFunc = [&](const Vec2F& point) {
			return point / mPreviewImage->GetImage()->GetOriginalSize() * mPreviewImage->layout->GetSize();
		};

		mBorderRightHandle->widgetOffsetToLocalTransformFunc = [&](const Vec2F& point) {
			return point / mPreviewImage->layout->GetSize() * mPreviewImage->GetImage()->GetOriginalSize();
		};

		mBorderRightHandle->checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(Math::Clamp(point.x, 0.0f, (float)mPreviewImage->GetImage()->GetOriginalSize().x), mPreviewImage->GetImage()->GetOriginalSize().y*0.5f);
		};

		mBorderRightHandle->onChangedPos = [&](const Vec2F& point) {
			mBordersSmoothValue.right = (float)mPreviewImage->GetImage()->GetOriginalSize().x - point.x;
			UpdateBordersValue();
		};

		mBorderRightHandle->cursorType = CursorType::SizeWE;

		mPreviewImage->AddChild(mBorderRightHandle);
	}

	void ImageAssetPropertiesViewer::InitializeTopHandle()
	{
		mBorderTopHandle = new WidgetDragHandle(mnew Sprite(Color4(0.0f, 1.0f, 0.0f, 1.0f)),
												mnew Sprite(Color4(0.5f, 1.0f, 0.5f, 1.0f)),
												mnew Sprite(Color4(0.0f, 0.6f, 0.0f, 1.0f)));

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

		mPreviewImage->AddChild(mBorderTopHandle);
	}

	void ImageAssetPropertiesViewer::InitializeBottomHandle()
	{
		mBorderBottomHandle = new WidgetDragHandle(mnew Sprite(Color4(0.0f, 1.0f, 0.0f, 1.0f)),
												   mnew Sprite(Color4(0.5f, 1.0f, 0.5f, 1.0f)),
												   mnew Sprite(Color4(0.0f, 0.6f, 0.0f, 1.0f)));

		mBorderBottomHandle->localToWidgetOffsetTransformFunc = [&](const Vec2F& point) {
			return point / mPreviewImage->GetImage()->GetOriginalSize() * mPreviewImage->layout->GetSize();
		};

		mBorderBottomHandle->widgetOffsetToLocalTransformFunc = [&](const Vec2F& point) {
			return point / mPreviewImage->layout->GetSize() * mPreviewImage->GetImage()->GetOriginalSize();
		};

		mBorderBottomHandle->checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(mPreviewImage->GetImage()->GetOriginalSize().x*0.5f, Math::Clamp(point.y, 0.0f, (float)mPreviewImage->GetImage()->GetOriginalSize().y));
		};

		mBorderBottomHandle->onChangedPos = [&](const Vec2F& point) {
			mBordersSmoothValue.bottom = point.y;
			UpdateBordersValue();
		};

		mBorderBottomHandle->cursorType = CursorType::SizeNS;

		mPreviewImage->AddChild(mBorderBottomHandle);
	}

	void ImageAssetPropertiesViewer::InitializeProperties()
	{
		auto borderProperty = o2EditorProperties.CreateFieldProperty(&TypeOf(BorderI), "Slice border");
		mBorderProperty = (BorderIProperty*)borderProperty;
		mBorderProperty->onChanged += [&](auto x) { UpdateBordersAnchors(); mBordersSmoothValue = mBorderProperty->GetCommonValue(); };
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

	void ImageAssetPropertiesViewer::UpdateBordersAnchors()
	{
		BorderI borders = mBorderProperty->GetCommonValue();
		Vec2F imageSize = mPreviewImage->GetImage()->GetOriginalSize();

		auto setHandleSize = [](WidgetDragHandle& handle, const Vec2F& size) {
			handle.GetRegularSprite()->SetSize(size);
			handle.GetHoverSprite()->SetSize(size);
			handle.GetPressedSprite()->SetSize(size);
		};

		mPreviewImage->UpdateTransform();
		setHandleSize(*mBorderLeftHandle, Vec2F(3.0f, mPreviewImage->layout->GetHeight()));
		setHandleSize(*mBorderRightHandle, Vec2F(3.0f, mPreviewImage->layout->GetHeight()));
		setHandleSize(*mBorderTopHandle, Vec2F(mPreviewImage->layout->GetWidth(), 3.0f));
		setHandleSize(*mBorderBottomHandle, Vec2F(mPreviewImage->layout->GetWidth(), 3.0f));

		mBorderLeftHandle->SetPosition(Vec2F((float)borders.left, 0));
		mBorderRightHandle->SetPosition(Vec2F(imageSize.x - (float)borders.right, 0));
		mBorderTopHandle->SetPosition(Vec2F(0, imageSize.y - (float)borders.top));
		mBorderBottomHandle->SetPosition(Vec2F(0, (float)borders.bottom));
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

	void ImageAssetPropertiesViewer::PreviewImage::Draw()
	{
		auto prevFilter = mImage->GetTexture()->GetFilter();
		mImage->GetTexture()->SetFilter(Texture::Filter::Nearest);

		Image::Draw();

		mImage->GetTexture()->SetFilter(prevFilter);
	}

}

DECLARE_CLASS(Editor::ImageAssetPropertiesViewer);

DECLARE_CLASS(Editor::ImageAssetPropertiesViewer::PreviewImage);
