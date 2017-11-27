#include "stdafx.h"
#include "Core/Dialogs/ColorPickerDlg.h"

#include "Core/UIRoot.h"
#include "UI/DropDown.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/HorizontalProgress.h"
#include "UI/Image.h"
#include "UI/Label.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/VerticalProgress.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/Window.h"

DECLARE_SINGLETON(Editor::ColorPickerDlg);

namespace Editor
{
	ColorPickerDlg::ColorPickerDlg()
	{
		mWindow = dynamic_cast<UIWindow*>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Color picker")));

		InitializeControls();

		mWindow->Hide(true);
		mWindow->layout->size = Vec2F(200, 300);

		mWindow->GetBackCursorListener().onCursorReleased = [&](const Input::Cursor& c) { OnCursorPressedOutside(); };
		mWindow->onHide = Func(this, &ColorPickerDlg::OnHide);
	}

	ColorPickerDlg::~ColorPickerDlg()
	{
		if (mWindow)
			delete mWindow;
	}

	void ColorPickerDlg::Show(const Color4& color, Function<void(const Color4&)> onChanged,
							  Function<void()> onCompleted/* = Function<void()>()*/)
	{
		mInstance->mColorValue = color;
		mInstance->mWindow->ShowModal();
		mInstance->mOnChangedCallback = onChanged;
		mInstance->mOnCompletedCallback = onCompleted;
		mInstance->UpdateHandlesValues();
	}

	void ColorPickerDlg::OnHide()
	{
		mOnCompletedCallback();
	}

	void ColorPickerDlg::InitializeControls()
	{
		InitializeColorPreview();
		InitializePickArea();
		InitializeColorParams();
	}

	void ColorPickerDlg::InitializeColorPreview()
	{
		UIWidget* colorPreviewContainer = mnew UIWidget();
		*colorPreviewContainer->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 30, 0);

		UIWidget* colorPreview = o2UI.CreateWidget<UIWidget>("colorProperty");
		*colorPreview->layout = UIWidgetLayout::BothStretch(5, 5, 5, 5);

		Color4 color1(1.0f, 1.0f, 1.0f, 1.0f), color2(0.7f, 0.7f, 0.7f, 1.0f);
		Bitmap backLayerBitmap(Bitmap::Format::R8G8B8A8, Vec2I(20, 20));
		backLayerBitmap.Fill(color1);
		backLayerBitmap.FillRect(0, 0, 10, 10, color2);
		backLayerBitmap.FillRect(10, 10, 20, 20, color2);
		mChessBackTexture = TextureRef(&backLayerBitmap);

		UIImage* backImage = mnew UIImage();
		backImage->image = mnew Sprite(mChessBackTexture, RectI(0, 0, 20, 20));
		backImage->GetImage()->mode = SpriteMode::Tiled;
		*backImage->layout = UIWidgetLayout::BothStretch(1, 1, 1, 1);
		colorPreview->AddChild(backImage);

		Bitmap colorLayerBitmap(Bitmap::Format::R8G8B8A8, Vec2I(20, 20));
		colorLayerBitmap.Fill(color1);
		mColorSampleImage = mnew UIImage();
		mColorSampleImage->image = mnew Sprite(&colorLayerBitmap);
		*mColorSampleImage->layout = UIWidgetLayout::BothStretch(1, 1, 1, 1);
		colorPreview->AddChild(mColorSampleImage);
		colorPreviewContainer->AddChild(colorPreview);

		mWindow->AddChild(colorPreviewContainer);
	}

	void ColorPickerDlg::InitializePickArea()
	{
		UIWidget* pickAreaContainer = mnew UIWidget();
		*pickAreaContainer->layout = UIWidgetLayout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 1.0f), Vec2F(0, 0), Vec2F(0, -30));

		UIWidget* pickArea = mnew UIWidget();
		*pickArea->layout = UIWidgetLayout::BothStretch(5, 5, 30, 5);

		pickArea->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"),
						   Layout::BothStretch(-9, -9, -9, -9));

		mColorPickAreaBitmap = mnew Bitmap(Bitmap::Format::R8G8B8A8, Vec2I(80, 80));
		mColorPickAreaBitmap->Clear(Color4::White());
		mColorPickAreaTexture = TextureRef(mColorPickAreaBitmap);
		mColorPickAreaColor = pickArea->AddLayer("color", mnew Sprite(mColorPickAreaTexture, RectI(0, 0, 80, 80)),
												 Layout::BothStretch(1, 1, 1, 1));

		mColorPickAreaHandle = o2UI.CreateImage("ui/circle_hole_handle.png");
		*mColorPickAreaHandle->layout = UIWidgetLayout::Based(BaseCorner::Center, Vec2F(15, 15));
		pickArea->AddChild(mColorPickAreaHandle);

		pickAreaContainer->AddChild(pickArea);

		mHUEBar = o2UI.CreateWidget<UIVerticalProgress>("wide");
		*mHUEBar->layout = UIWidgetLayout::VerStretch(HorAlign::Right, 5, 5, 20, 5);

		mHUEBarBitmap = mnew Bitmap(Bitmap::Format::R8G8B8A8, Vec2I(20, 256));
		InitHUEBarBitmap();
		mHUEBarTexture = TextureRef(mHUEBarBitmap);
		mHUEBar->AddLayer("color", mnew Sprite(mHUEBarTexture, RectI(0, 0, 20, 256)), Layout::BothStretch(1, 1, 1, 1),
						  0.5f);

		mHUEBar->onChange = Func(this, &ColorPickerDlg::OnHUEEdited);

		pickAreaContainer->AddChild(mHUEBar);

		mWindow->AddChild(pickAreaContainer);

		mColorPickAreaHandle->onDraw += [&]() { mColorPickHandle.OnDrawn(); };
		mColorPickHandle.isUnderPoint = [&](const Vec2F& point) { return mColorPickAreaColor->IsUnderPoint(point); };
		mColorPickHandle.onMoved = Func(this, &ColorPickerDlg::OnColorPickHandleMoved);
	}

	void ColorPickerDlg::InitializeColorParams()
	{
		mTypeDropdown = o2UI.CreateDropdown();
		*mTypeDropdown->layout = UIWidgetLayout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -20), Vec2F(0, 0));

		Vector<WString> colorTypes;
		for (auto kv : EnumTypeContainer<ColorType>::GetEntries())
			colorTypes.Add(kv.Value());

		mTypeDropdown->AddItems(colorTypes);
		mTypeDropdown->onSelectedText = Func(this, &ColorPickerDlg::OnColorTypeSelected);

		mWindow->AddChild(mTypeDropdown);

		auto colorParamsArea = mnew UIVerticalLayout();
		*colorParamsArea->layout = UIWidgetLayout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 0.5f), Vec2F(0, 0), Vec2F(0, -20));
		colorParamsArea->border = RectF(5, 5, 5, 5);

		colorParamsArea->AddChild(InitializeColorParameter(mColor1ParamName, mColor1ParamBar,
								  mColor1ParamEdit, mColor1ParamBarBitmap, mColor1ParamBarTexture, 
								  Func(this, &ColorPickerDlg::OnColor1ParameterEdited)));

		colorParamsArea->AddChild(InitializeColorParameter(mColor2ParamName, mColor2ParamBar, 
								  mColor2ParamEdit, mColor2ParamBarBitmap, mColor2ParamBarTexture, 
								  Func(this, &ColorPickerDlg::OnColor2ParameterEdited)));

		colorParamsArea->AddChild(InitializeColorParameter(mColor3ParamName, mColor3ParamBar, 
								  mColor3ParamEdit, mColor3ParamBarBitmap, mColor3ParamBarTexture, 
								  Func(this, &ColorPickerDlg::OnColor3ParameterEdited)));

		colorParamsArea->AddChild(InitializeColorParameter(mColorAParamName, mColorAParamBar, 
								  mColorAParamEdit, mColorAParamBarBitmap, mColorAParamBarTexture, 
								  Func(this, &ColorPickerDlg::OnColorAParameterEdited)));

	mWindow->AddChild(colorParamsArea);

	mColorAParamName->text = "A";
	mTypeDropdown->value = "RGB";
}

UIWidget* ColorPickerDlg::InitializeColorParameter(UILabel*& name, UIHorizontalProgress*& bar,
												   UIEditBox*& edit, Bitmap*& bitmap, TextureRef& texture,
												   const Function<void(float)>& changeCallback)
{
	auto resLayout = mnew UIWidget();
	name = o2UI.CreateLabel("R");
	*name->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(30, 20));
	name->horAlign = HorAlign::Left;
	resLayout->AddChild(name);

	bar = o2UI.CreateWidget<UIHorizontalProgress>("wide");
	*bar->layout = UIWidgetLayout::HorStretch(VerAlign::Middle, 30, 50, 20);
	bar->onChange = changeCallback;

	bitmap = mnew Bitmap(Bitmap::Format::R8G8B8A8, Vec2F(256, 256));
	texture = TextureRef(bitmap);
	bar->AddLayer("color", mnew Sprite(texture, RectI(0, 0, 256, 256)), Layout::BothStretch(1, 1, 1, 1), 0.5f);

	Sprite* backSprite = mnew Sprite(mChessBackTexture, RectI(0, 0, 20, 20));
	backSprite->mode = SpriteMode::Tiled;
	bar->AddLayer("colorBack", backSprite, Layout::BothStretch(1, 1, 1, 1), 0.4f);

	resLayout->AddChild(bar);

	edit = o2UI.CreateEditBox("singleline");
	*edit->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(45, 20));
	edit->SetFilterInteger();

	edit->onChangeCompleted = [&](const WString& text) { bar->SetValue(((float)text/255.0f)); };

	resLayout->AddChild(edit);

	return resLayout;
}

void ColorPickerDlg::InitHUEBarBitmap()
{
	for (int i = 0; i < 256; i++)
	{
		Color4 c; c.SetHSL(1.0f - (float)i/255.0f, 1.0f, 0.5f);
		UInt cl = c.ABGR();

		for (int j = 0; j < 20; j++)
			memcpy(mHUEBarBitmap->GetData() + 4*20*i + j*4, &cl, 4);
	}
}

void ColorPickerDlg::OnColorPickHandleMoved(const Input::Cursor& cursor)
{
	auto rt = mColorPickAreaColor->GetRect();

	Vec2F anchorPos = (cursor.position - rt.LeftBottom())/rt.Size();
	anchorPos.x = Math::Clamp01(anchorPos.x);
	anchorPos.y = Math::Clamp01(anchorPos.y);

	mColorPickAreaHandle->layout->anchorMin = anchorPos;
	mColorPickAreaHandle->layout->anchorMax = anchorPos;

	mColorValue.SetHSL(mHUEBar->value, anchorPos.y, anchorPos.x);
	UpdateHandlesValues();
	UpdateHandlesBitmaps();
}

void ColorPickerDlg::OnColorTypeSelected(const WString& name)
{
	mColorType = o2Reflection.GetEnumValue<ColorType>(name);

	switch (mColorType)
	{
		case ColorType::RGB:
		mColor1ParamName->text = "R";
		mColor2ParamName->text = "G";
		mColor3ParamName->text = "B";
		break;

		case ColorType::HSL:
		mColor1ParamName->text = "H";
		mColor2ParamName->text = "S";
		mColor3ParamName->text = ":";
		break;
	}

	UpdateHandlesValues();
	UpdateHandlesBitmaps();
}

void ColorPickerDlg::OnColor1ParameterEdited(float value)
{
	switch (mColorType)
	{
		case ColorType::RGB:
		mColorValue.SetRF(value);
		break;

		case ColorType::HSL:
		float h, s, l;
		mColorValue.ToHSL(h, s, l);
		h = value;
		mColorValue.SetHSL(h, s, l);
		mHUEBar->value = value;
		break;
	}

	mColor1ParamEdit->text = (WString)(int)(value*255.0f);
	mOnChangedCallback(mColorValue);
	UpdateHandlesBitmaps();
	UpdateColorPickBitmap();
}

void ColorPickerDlg::OnColor2ParameterEdited(float value)
{
	switch (mColorType)
	{
		case ColorType::RGB:
		mColorValue.SetGF(value);
		break;

		case ColorType::HSL:
		float h, s, l;
		mColorValue.ToHSL(h, s, l);
		s = value;
		mColorValue.SetHSL(h, s, l);
		break;
	}

	mColor2ParamEdit->text = (WString)(int)(value*255.0f);
	mOnChangedCallback(mColorValue);
	UpdateHandlesBitmaps();
}

void ColorPickerDlg::OnColor3ParameterEdited(float value)
{
	switch (mColorType)
	{
		case ColorType::RGB:
		mColorValue.SetBF(value);
		break;

		case ColorType::HSL:
		float h, s, l;
		mColorValue.ToHSL(h, s, l);
		l = value;
		mColorValue.SetHSL(h, s, l);
		break;
	}

	mColor3ParamEdit->text = (WString)(int)(value*255.0f);
	mOnChangedCallback(mColorValue);
	UpdateHandlesBitmaps();
}

void ColorPickerDlg::OnColorAParameterEdited(float value)
{
	mColorValue.SetAF(value);
	mColorAParamEdit->text = (WString)(int)(value*255.0f);
	mOnChangedCallback(mColorValue);
	UpdateHandlesBitmaps();
}

void ColorPickerDlg::OnHUEEdited(float value)
{
	static bool hueLocked = false;
	if (hueLocked)
		return;

	float h, s, l;
	mColorValue.ToHSL(h, s, l);
	h = value;
	mColorValue.SetHSL(h, s, l);

	mOnChangedCallback(mColorValue);

	hueLocked = true;
	UpdateHandlesValues();
	hueLocked = false;

	UpdateHandlesBitmaps();
	UpdateColorPickBitmap();
}

void ColorPickerDlg::UpdateHandlesBitmaps()
{
	mColorSampleImage->GetImage()->SetColor(mColorValue);

	switch (mColorType)
	{
		case ColorType::RGB:
		UpdateRGBABitmaps();
		break;

		case ColorType::HSL:
		UpdateHSLABitmaps();
		break;
	}
}

void ColorPickerDlg::UpdateRGBABitmaps()
{
	int bpp = 4;
	UInt8* c1 = mColor1ParamBarBitmap->GetData();
	UInt8* c2 = mColor2ParamBarBitmap->GetData();
	UInt8* c3 = mColor3ParamBarBitmap->GetData();
	UInt8* cA = mColorAParamBarBitmap->GetData();
	int h = mColorAParamBarBitmap->GetSize().y;
	int w = mColorAParamBarBitmap->GetSize().x;

	for (int i = 0; i < 256; i++)
	{
		int off = bpp*i;

		*(ULong*)(c1 + off) = Color4(i, mColorValue.g, mColorValue.b, mColorValue.a).ABGR();
		*(ULong*)(c2 + off) = Color4(mColorValue.r, i, mColorValue.b, mColorValue.a).ABGR();
		*(ULong*)(c3 + off) = Color4(mColorValue.r, mColorValue.g, i, mColorValue.a).ABGR();
		*(ULong*)(cA + off) = Color4(mColorValue.r, mColorValue.g, mColorValue.b, i).ABGR();
	}

	for (int i = 1; i < h; i++)
	{
		memcpy(c1 + bpp*w*i, c1, bpp*w);
		memcpy(c2 + bpp*w*i, c2, bpp*w);
		memcpy(c3 + bpp*w*i, c3, bpp*w);
		memcpy(cA + bpp*w*i, cA, bpp*w);
	}

	mColor1ParamBarTexture->SetData(mColor1ParamBarBitmap);
	mColor2ParamBarTexture->SetData(mColor2ParamBarBitmap);
	mColor3ParamBarTexture->SetData(mColor3ParamBarBitmap);
	mColorAParamBarTexture->SetData(mColorAParamBarBitmap);
}

void ColorPickerDlg::UpdateHSLABitmaps()
{
	int bpp = 4;
	UInt8* c1 = mColor1ParamBarBitmap->GetData();
	UInt8* c2 = mColor2ParamBarBitmap->GetData();
	UInt8* c3 = mColor3ParamBarBitmap->GetData();
	UInt8* cA = mColorAParamBarBitmap->GetData();
	int h = mColorAParamBarBitmap->GetSize().y;
	int w = mColorAParamBarBitmap->GetSize().x;

	float hue, sat, light;
	mColorValue.ToHSL(hue, sat, light);

	for (int i = 0; i < 256; i++)
	{
		int off = bpp*i;

		Color4 h; h.SetHSL((float)i/255.0f, 1.0f, 0.5f);
		Color4 s; s.SetHSL(hue, (float)i/255.0f, light);
		Color4 l; l.SetHSL(hue, sat, (float)i/255.0f);

		*(ULong*)(c1 + off) = h.ABGR();
		*(ULong*)(c2 + off) = s.ABGR();
		*(ULong*)(c3 + off) = l.ABGR();
		*(ULong*)(cA + off) = Color4(mColorValue.r, mColorValue.g, mColorValue.b, i).ABGR();
	}

	for (int i = 1; i < h; i++)
	{
		memcpy(c1 + bpp*w*i, c1, bpp*w);
		memcpy(c2 + bpp*w*i, c2, bpp*w);
		memcpy(c3 + bpp*w*i, c3, bpp*w);
		memcpy(cA + bpp*w*i, cA, bpp*w);
	}

	mColor1ParamBarTexture->SetData(mColor1ParamBarBitmap);
	mColor2ParamBarTexture->SetData(mColor2ParamBarBitmap);
	mColor3ParamBarTexture->SetData(mColor3ParamBarBitmap);
	mColorAParamBarTexture->SetData(mColorAParamBarBitmap);
}

void ColorPickerDlg::UpdateColorPickBitmap()
{
	float h, s, l;
	mColorValue.ToHSL(h, s, l);
	Vec2I sz = mColorPickAreaBitmap->GetSize();

	for (int i = 0; i < sz.x; i++)
	{
		for (int j = 0; j < sz.y; j++)
		{
			Color4 c; c.SetHSL(h, (float)i/(float)sz.x, (float)j/(float)sz.y);
			UInt cc = c.ABGR();
			memcpy(mColorPickAreaBitmap->GetData() + 4*i*sz.x + 4*j, &cc, 4);
		}
	}

	mColorPickAreaTexture->SetData(mColorPickAreaBitmap);
}

void ColorPickerDlg::UpdateHandlesValues()
{
	switch (mColorType)
	{
		case ColorType::RGB:
		mColor1ParamBar->value = mColorValue.RF();
		mColor2ParamBar->value = mColorValue.GF();
		mColor3ParamBar->value = mColorValue.BF();
		break;

		case ColorType::HSL:
		float h, s, l;
		mColorValue.ToHSL(h, s, l);
		mColor1ParamBar->value = h;
		mColor2ParamBar->value = s;
		mColor3ParamBar->value = l;
		break;
	}

	mColorAParamBar->value = mColorValue.AF();
	mOnChangedCallback(mColorValue);
}

void ColorPickerDlg::OnCursorPressedOutside()
{
	mOnChangedCallback(mColorValue);
	mWindow->Hide();
}
}

ENUM_META_(Editor::ColorPickerDlg::ColorType, ColorType)
{
	ENUM_ENTRY(HSL);
	ENUM_ENTRY(RGB);
}
END_ENUM_META;
