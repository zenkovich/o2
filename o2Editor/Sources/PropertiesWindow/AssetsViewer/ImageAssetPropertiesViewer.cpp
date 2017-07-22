#include "ImageAssetPropertiesViewer.h"

#include "Assets/ImageAsset.h"
#include "PropertiesWindow/Properties/BorderIntProperty.h"
#include "PropertiesWindow/Properties/EnumProperty.h"
#include "PropertiesWindow/Properties/ObjectProperty.h"
#include "PropertiesWindow/Properties/RectangleIntProperty.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "UI/Image.h"
#include "UI/Label.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Bitmap.h"

namespace Editor
{
	ImageAssetPropertiesViewer::ImageAssetPropertiesViewer()
	{
		mContent = mnew UIVerticalLayout();
		mContent->spacing = 5.0f;
		mContent->border = RectF(5, 5, 12, 5);
		mContent->expandHeight = false;
		mContent->expandWidth = true;
		mContent->fitByChildren = true;
		mContent->baseCorner = BaseCorner::Top;

		InitializeImagePreview();
		InitializeProperties();

		mContent->UpdateLayout(true);
	}

	ImageAssetPropertiesViewer::~ImageAssetPropertiesViewer()
	{
		delete mContent;
	}

	void ImageAssetPropertiesViewer::SetTargetAssets(const Vector<AssetRef*>& assets)
	{
		mTargetAssets = assets.Cast<ImageAssetRef*>();

		auto borderTargets = mTargetAssets.Select<void*>([](const ImageAssetRef* x) { return &((*x)->GetMeta()->mSliceBorder); });
		mBorderProperty->SetValuePtr(borderTargets, false);

		auto modeTargets = mTargetAssets.Select<void*>([](const ImageAssetRef* x) { return &((*x)->GetMeta()->mDefaultMode); });
		mDefaultTypeProperty->SetValuePtr(modeTargets, false);

		auto windowsTargets = mTargetAssets.Select<void*>([](const ImageAssetRef* x) { return &((*x)->GetMeta()->mWindows); });
		mWindowsProperties->SetValuePtr(windowsTargets, false);

		auto osxTargets = mTargetAssets.Select<void*>([](const ImageAssetRef* x) { return &((*x)->GetMeta()->mMacOS); });
		mOSXProperties->SetValuePtr(osxTargets, false);

		auto androidTargets = mTargetAssets.Select<void*>([](const ImageAssetRef* x) { return &((*x)->GetMeta()->mAndroid); });
		mAndroidProperties->SetValuePtr(androidTargets, false);

		auto iosTargets = mTargetAssets.Select<void*>([](const ImageAssetRef* x) { return &((*x)->GetMeta()->mIOS); });
		mIOSProperties->SetValuePtr(iosTargets, false);

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
		mPreviewImageContent->layout.minHeight = 200;

		auto separatorImg = o2UI.CreateImage("ui/UI_Separator.png");
		separatorImg->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, -6, -15, 5, -4);
		mPreviewImageContent->AddChild(separatorImg);

		mPreviewImageBack = mnew UIImage();
		mPreviewImageBack->SetImage(CreateGridSprite());
		mPreviewImageBack->layout = UIWidgetLayout::BothStretch();
		mPreviewImageContent->AddChild(mPreviewImageBack);

		mPreviewImage = mnew UIImage();
		mPreviewImage->layout = UIWidgetLayout::BothStretch();
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
			auto rt = mBorderLeftHandleWidget->layout.GetAbsoluteRect();
			rt.left -= 2; rt.right += 2;
			return rt.IsInside(p);
		};

		mBorderLeftHandle.onMoved = [&](const Input::Cursor& cursor) {
			float px = mPreviewImage->layout.GetWidth()/mPreviewImage->GetImage()->GetOriginalSize().x;
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
			auto rt = mBorderRightHandleWidget->layout.GetAbsoluteRect();
			rt.left -= 2; rt.right += 2;
			return rt.IsInside(p);
		};

		mBorderRightHandle.onMoved = [&](const Input::Cursor& cursor) {
			float px = mPreviewImage->layout.GetWidth()/mPreviewImage->GetImage()->GetOriginalSize().x;
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
			auto rt = mBorderBottomHandleWidget->layout.GetAbsoluteRect();
			rt.bottom -= 2; rt.top += 2;
			return rt.IsInside(p);
		};

		mBorderTopHandle.onMoved = [&](const Input::Cursor& cursor) {
			float px = mPreviewImage->layout.GetHeight()/mPreviewImage->GetImage()->GetOriginalSize().y;
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
			auto rt = mBorderTopHandleWidget->layout.GetAbsoluteRect();
			rt.bottom -= 2; rt.top += 2;
			return rt.IsInside(p);
		};

		mBorderBottomHandle.onMoved = [&](const Input::Cursor& cursor) {
			float px = mPreviewImage->layout.GetHeight()/mPreviewImage->GetImage()->GetOriginalSize().y;
			mBordersSmoothValue.bottom -= cursor.delta.y/px;

			if (mBorderProperty->GetCommonValue().bottom != (int)Math::Round(mBordersSmoothValue.bottom))
				UpdateBordersValue();
		};
	}

	void ImageAssetPropertiesViewer::InitializeProperties()
	{
		auto borderPropertyPair = o2EditorProperties.CreateFieldProperty(&TypeOf(RectI));
		auto nameLabel = borderPropertyPair.second->FindChild<UILabel>();
		nameLabel->text = "Slice border";
		mBorderProperty = (BorderIProperty*)borderPropertyPair.first;
		mBorderProperty->onChanged += [&]() { UpdateBordersAnchors(); /*mBordersSmoothValue = mBorderProperty->GetCommonValue();*/ };
		mContent->AddChild(borderPropertyPair.second);

		auto modePropertyPair = o2EditorProperties.CreateFieldProperty(&TypeOf(SpriteMode));
		nameLabel = modePropertyPair.second->FindChild<UILabel>();
		nameLabel->text = "Default mode";
		mDefaultTypeProperty = (EnumProperty*)modePropertyPair.first;
		mDefaultTypeProperty->SpecializeType(&TypeOf(SpriteMode));
		mContent->AddChild(modePropertyPair.second);

		auto atlasPropertyPair = o2EditorProperties.CreateFieldProperty(&TypeOf(AtlasAssetRef));
		nameLabel = atlasPropertyPair.second->FindChild<UILabel>();
		nameLabel->text = "Atlas";
		mAtlasProperty = (AssetProperty<AtlasAssetRef>*)atlasPropertyPair.first;
		mAtlasProperty->onChanged = THIS_FUNC(OnAtlasPropertyChanged);
		mContent->AddChild(atlasPropertyPair.second);

		auto windowsPropertyPair = o2EditorProperties.CreateFieldProperty(&TypeOf(ImageAsset::PlatformMeta));
		nameLabel = windowsPropertyPair.second->FindChild<UILabel>();
		nameLabel->text = "Windows";
		mWindowsProperties = (ObjectProperty*)windowsPropertyPair.first;
		mWindowsProperties->SpecializeType(&TypeOf(ImageAsset::PlatformMeta));
		mContent->AddChild(windowsPropertyPair.second);

		auto osxPropertyPair = o2EditorProperties.CreateFieldProperty(&TypeOf(ImageAsset::PlatformMeta));
		nameLabel = osxPropertyPair.second->FindChild<UILabel>();
		nameLabel->text = "OSX";
		mOSXProperties = (ObjectProperty*)osxPropertyPair.first;
		mOSXProperties->SpecializeType(&TypeOf(ImageAsset::PlatformMeta));
		mContent->AddChild(osxPropertyPair.second);

		auto androidPropertyPair = o2EditorProperties.CreateFieldProperty(&TypeOf(ImageAsset::PlatformMeta));
		nameLabel = androidPropertyPair.second->FindChild<UILabel>();
		nameLabel->text = "Android";
		mAndroidProperties = (ObjectProperty*)androidPropertyPair.first;
		mAndroidProperties->SpecializeType(&TypeOf(ImageAsset::PlatformMeta));
		mContent->AddChild(androidPropertyPair.second);

		auto iosPropertyPair = o2EditorProperties.CreateFieldProperty(&TypeOf(ImageAsset::PlatformMeta));
		nameLabel = iosPropertyPair.second->FindChild<UILabel>();
		nameLabel->text = "iOS";
		mIOSProperties = (ObjectProperty*)iosPropertyPair.first;
		mIOSProperties->SpecializeType(&TypeOf(ImageAsset::PlatformMeta));
		mContent->AddChild(iosPropertyPair.second);
	}

	void ImageAssetPropertiesViewer::FitImage()
	{
		Vec2F maxSize = mPreviewImageContent->layout.size;
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
			mPreviewImage->layout = UIWidgetLayout(Vec2F(0.5f - d, 0.0f), Vec2F(0.5f + d, 1.0f), Vec2F(), Vec2F());
		}
		else
		{
			float d = 1.0f/imageSizeAspect*0.5f*contentAspect;
			mPreviewImage->layout = UIWidgetLayout(Vec2F(0.0f, 0.5f - d), Vec2F(1.0f, 0.5f + d), Vec2F(), Vec2F());
		}

		mPreviewImageBack->layout = mPreviewImage->layout;

		UpdateBordersAnchors();
		mBordersSmoothValue = mBorderProperty->GetCommonValue();
	}

	void ImageAssetPropertiesViewer::UpdateBordersAnchors()
	{
		Vec2F imageSize = mPreviewImage->GetImage()->GetOriginalSize();
		BorderI borders = mBorderProperty->GetCommonValue();
		BorderF bordersAnchors((float)borders.left/imageSize.x, (float)borders.top/imageSize.y,
							 1.0f - (float)borders.right/imageSize.x, 1.0f - (float)borders.bottom/imageSize.y);

		mBorderLeftHandleWidget->layout = UIWidgetLayout(Vec2F(bordersAnchors.left, 0.0f), Vec2F(bordersAnchors.left, 1.0f), Vec2F(0, 0), Vec2F(1, 0));
		mBorderRightHandleWidget->layout = UIWidgetLayout(Vec2F(bordersAnchors.right, 0.0f), Vec2F(bordersAnchors.right, 1.0f), Vec2F(0, 0), Vec2F(1, 0));
		mBorderTopHandleWidget->layout = UIWidgetLayout(Vec2F(0.0f, bordersAnchors.top), Vec2F(1.0f, bordersAnchors.top), Vec2F(0, 0), Vec2F(0, 1));
		mBorderBottomHandleWidget->layout = UIWidgetLayout(Vec2F(0.0f, bordersAnchors.bottom), Vec2F(1.0f, bordersAnchors.bottom), Vec2F(0, 0), Vec2F(0, 1));
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

	void ImageAssetPropertiesViewer::OnAtlasPropertyChanged()
	{
		UID id = mAtlasProperty->GetCommonValue()->GetAssetId();

		for (auto target : mTargetAssets)
			(*target)->SetAtlasId(id);
	}

	Sprite* ImageAssetPropertiesViewer::CreateGridSprite()
	{
		Vec2I size(20, 20);
		Color4 c1 = Color4::White();
		Color4 c2(200, 200, 200, 255);

		Bitmap bmp(Bitmap::Format::R8G8B8A8, size);
		bmp.Fill(c1);
		bmp.FillRect(0, 0, 10, 10, c2);
		bmp.FillRect(10, 10, 20, 20, c2);

		Sprite* res = mnew Sprite(&bmp);
		res->SetMode(SpriteMode::Tiled);
		return res;
	}

}

CLASS_META(Editor::ImageAssetPropertiesViewer)
{
	BASE_CLASS(Editor::IAssetPropertiesViewer);

	PROTECTED_FIELD(mTargetAssets);
	PROTECTED_FIELD(mContent);
	PROTECTED_FIELD(mPreviewImageContent);
	PROTECTED_FIELD(mPreviewImage);
	PROTECTED_FIELD(mPreviewImageBack);
	PROTECTED_FIELD(mBorderLeftHandleWidget);
	PROTECTED_FIELD(mBorderRightHandleWidget);
	PROTECTED_FIELD(mBorderTopHandleWidget);
	PROTECTED_FIELD(mBorderBottomHandleWidget);
	PROTECTED_FIELD(mBorderLeftHandle);
	PROTECTED_FIELD(mBorderRightHandle);
	PROTECTED_FIELD(mBorderTopHandle);
	PROTECTED_FIELD(mBorderBottomHandle);
	PROTECTED_FIELD(mBordersSmoothValue);
	PROTECTED_FIELD(mBorderProperty);
	PROTECTED_FIELD(mDefaultTypeProperty);
	PROTECTED_FIELD(mAtlasProperty);
	PROTECTED_FIELD(mWindowsProperties);
	PROTECTED_FIELD(mOSXProperties);
	PROTECTED_FIELD(mAndroidProperties);
	PROTECTED_FIELD(mIOSProperties);

	PUBLIC_FUNCTION(void, SetTargetAssets, const Vector<AssetRef*>&);
	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PROTECTED_FUNCTION(void, InitializeImagePreview);
	PROTECTED_FUNCTION(void, InitializeLeftHandle);
	PROTECTED_FUNCTION(void, InitializeRightHandle);
	PROTECTED_FUNCTION(void, InitializeTopHandle);
	PROTECTED_FUNCTION(void, InitializeBottomHandle);
	PROTECTED_FUNCTION(void, InitializeProperties);
	PROTECTED_FUNCTION(void, FitImage);
	PROTECTED_FUNCTION(void, UpdateBordersAnchors);
	PROTECTED_FUNCTION(void, UpdateBordersValue);
	PROTECTED_FUNCTION(void, SetupAtlasProperty);
	PROTECTED_FUNCTION(void, OnAtlasPropertyChanged);
	PROTECTED_FUNCTION(Sprite*, CreateGridSprite);
}
END_META;
