#include "ui_std_skin_initializer.h"

#include "render_system/render_system.h"
#include "render_system/sprite.h"
#include "render_system/text.h"
#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_controller.h"
#include "ui_editbox.h"
#include "ui_label.h"
#include "ui_progressbar.h"
#include "ui_rect.h"
#include "ui_scroll_bar.h"
#include "ui_scrollarea.h"
#include "ui_transition_state.h"
#include "util/geometry/geometry.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(UIStdSkinInitializer);

UISkinManager* UIStdSkinInitializer::mSkinManager = NULL;
Font*          UIStdSkinInitializer::mStdFont = NULL;


void UIStdSkinInitializer::Initialize()
{
	mSkinManager = mnew UISkinManager();

	mStdFont = FontsManager()->loadBMFont("o2_font/myriad");

	InitButton();
	InitBackground();
	InitProgressBar();
	InitCheckBox();
	InitHorScrollBar();
	InitHorThinScrollBar();
	InitVerThinScrollBar();
	InitSingleLineEditBox();
	InitMultilineEditBox();
	InitLabel();
	InitScrollArea();
	InitHint();
	InitRectPad();
}

void UIStdSkinInitializer::Deinitialize()
{
	SafeRelease(mSkinManager);
}

void UIStdSkinInitializer::InitButton()
{
	const String regDrawableTexture = "ui_skin/btn_regular";
	const String shadowDrawableTexture = "ui_skin/btn_shadow";
	const String hoverDrawableTexture = "ui_skin/btn_hover";
	const String focusDrawableTexture = "ui_skin/btn_glow";
	const String pressedDrawableTexture = "ui_skin/btn_pressed";
	const int rectBorderLeft = 10;
	const int rectBorderTop = 10;
	const int rectBorderRight = 14;
	const int rectBorderBottom = 14;

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float pressingDelayOn = 0.01f;
	const float pressingDelayOff= 0.1f;
	const float focusingDelayOn = 0.1f;
	const float focusingDelayOff= 0.6f;

	const Vec2F fixedMinSize(10.0f, 10.0f);

	UIButton* button = mnew UIButton(Layout::Fixed(Vec2F(50, 50), Vec2F()));
	button->mFixedMinSize = fixedMinSize;

	//drawables
	StretchRect* regDrawable = mnew StretchRect(
		TextureRef::CreateFromFile(regDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);

	StretchRect* shadowDrawable = mnew StretchRect(
		TextureRef::CreateFromFile(shadowDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);

	StretchRect* hoverDrawable = mnew StretchRect(
		TextureRef::CreateFromFile(hoverDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);

	StretchRect* focusDrawable = mnew StretchRect(
		TextureRef::CreateFromFile(focusDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);

	StretchRect* pressedDrawable = mnew StretchRect(
		TextureRef::CreateFromFile(pressedDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);

	Text* captionDrawable = mnew Text(mStdFont);
	captionDrawable->horAlign = Font::HA_CENTER;
	captionDrawable->verAlign = Font::VA_CENTER;
	captionDrawable->wordWrap = true;
	captionDrawable->linesDistCoef = 0.8f;

	//adding drawables
	Layout drawablesLayout = Layout::Both(RectF(-5, -5, -6, -7));
	button->AddDrawable(shadowDrawable, "shadow", drawablesLayout);
	button->AddDrawable(focusDrawable, "focus", drawablesLayout);
	button->AddDrawable(regDrawable, "regular", drawablesLayout);
	button->AddDrawable(hoverDrawable, "hover", drawablesLayout);
	button->AddDrawable(pressedDrawable, "pressed", drawablesLayout);
	button->AddDrawable(captionDrawable, "caption", Layout::Both(RectF(-5, -5, -9, -7)));

	//hover state
	UITransitionState* hoverState = mnew UITransitionState("hover");
	hoverState->AddProperty(&hoverDrawable->transparency,
							AnimationFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//pressed state
	UITransitionState* pressedState = mnew UITransitionState("pressed");
	pressedState->AddProperty(&pressedDrawable->transparency,
							  AnimationFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
							  AnimationFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	pressedState->AddProperty(&shadowDrawable->transparency,
							  AnimationFrame<float>(1.0f, pressingDelayOff, false, IT_LINEAR),
							  AnimationFrame<float>(0.0f, pressingDelayOn, false, IT_LINEAR));

	//focus state
	UITransitionState* focusState = mnew UITransitionState("focus");
	focusState->AddProperty(&focusDrawable->transparency,
							AnimationFrame<float>(0.0f, focusingDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, focusingDelayOn, false, IT_LINEAR));

	//adding states
	button->AddState(hoverState);
	button->AddState(pressedState);
	button->AddState(focusState);

	//set as sample in skin manager
	mSkinManager->SetButtonSample(button);
}

void UIStdSkinInitializer::InitBackground()
{
	const String basicBackgroundTexName = "ui_skin/background";
	const String shadowTexName = "ui_skin/background_shadow";

	UIRect* background = mnew UIRect(Layout::Fixed(Vec2F(50, 50), Vec2F()), "bgSample");
	UIRect* shadow = mnew UIRect(Layout::Both(), "shadow");


	TextureRef basicBackgroundTex = TextureRef::CreateFromFile(basicBackgroundTexName);
	TextureRef shadowTex = TextureRef::CreateFromFile(shadowTexName);

	shadow->mStretchRect = StretchRect(1, shadowTex);
	shadow->mStretchRect.AddPart(Vec2F(0, 0), Vec2F(0, 0), Vec2F(1, 1), Vec2F(0, 0),
								 RectF(Vec2F(), shadowTex.getSize()));

	background->mStretchRect = StretchRect(1, basicBackgroundTex);
	background->mStretchRect.AddPart(Vec2F(0, 0), Vec2F(0, 0), Vec2F(1, 1), Vec2F(0, 0),
									 RectF(Vec2F(), basicBackgroundTex.getSize()), true);

	background->AddChild(shadow);

	mSkinManager->SetBackgroundSample(background);
}

void UIStdSkinInitializer::InitProgressBar()
{
	const String backgroundTexName = "ui_skin/bar_bk";
	const String barTexName = "ui_skin/bar_bar";

	TextureRef backgroundTex = TextureRef::CreateFromFile(backgroundTexName);
	TextureRef barTex = TextureRef::CreateFromFile(barTexName);

	//drawables
	const float backgroundLeftBorder = 11;
	const float backgroundRightBorder = 12;
	StretchRect* backgroundDrawable = mnew StretchRect(backgroundTex, (int)backgroundLeftBorder, 0, (int)backgroundRightBorder, 0);

	const float barLeftOffset = 6, barRightOffset = 8;
	const float barLeftBorder = 11, barRightBorder = 12;
	StretchRect* barDrawable = mnew StretchRect(barTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);

	//progerss bar
	UIProgressBar* progressbar = mnew UIProgressBar(Layout::Both());

	// adding drawables
	Layout backgroundLayout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, -backgroundTex.getSize().y*0.5f),
							Vec2F(1.0f, 0.5f), Vec2F(0.0f, backgroundTex.getSize().y*0.5f));
	progressbar->AddDrawable(backgroundDrawable, "background", backgroundLayout);

	UIProgressBar::Drawable* barBasicDrw = progressbar->AddDrawable(NULL, "barBasic",
																	Layout(Vec2F(0.0f, 0.0f), Vec2F(barLeftOffset, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(-barRightOffset, 0.0)));

	UIProgressBar::Drawable* barDrw = barBasicDrw->AddChildDrawable("bar", NULL, Layout::Both());

	float hs = barTex.getSize().y*0.5f;
	barDrw->AddChildDrawable("bar", barDrawable, Layout(Vec2F(0.0f, 0.5f), Vec2F(-barLeftOffset, -hs),
		Vec2F(1.0f, 0.5f), Vec2F(barRightOffset, hs)));

	progressbar->SetBarDrawable(barDrw);


	mSkinManager->SetProgressbarSample(progressbar);
}

void UIStdSkinInitializer::InitCheckBox()
{
	const String bkTexName = "ui_skin/checkbox_bk";
	const String glowTexName = "ui_skin/checkbox_glow";
	const String checkTexName = "ui_skin/checkbox_check";
	const String hoverTexName = "ui_skin/checkbox_hover";
	const String pressedTexName = "ui_skin/checkbox_pressed";

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float pressingDelayOn = 0.01f;
	const float pressingDelayOff= 0.1f;
	const float focusingDelayOn = 0.1f;
	const float focusingDelayOff= 0.6f;

	const float boxBorder = 3.0f;

	UICheckBox* checkBox = mnew UICheckBox(Layout::Both());

	//drawables
	Sprite* bkDrawable = mnew Sprite(TextureRef::CreateFromFile(bkTexName));
	Sprite* glowDrawable = mnew Sprite(TextureRef::CreateFromFile(glowTexName));
	Sprite* checkDrawable = mnew Sprite(TextureRef::CreateFromFile(checkTexName));
	Sprite* hoverDrawable = mnew Sprite(TextureRef::CreateFromFile(hoverTexName));
	Sprite* pressedDrawable = mnew Sprite(TextureRef::CreateFromFile(pressedTexName));

	Text* captionDrawable = mnew Text(mStdFont);
	captionDrawable->horAlign = Font::HA_LEFT;
	captionDrawable->verAlign = Font::VA_CENTER;

	//adding drawables
	Vec2F bkSize = bkDrawable->GetSize();
	Layout bkLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(boxBorder, -bkSize.y*0.5f),
							 Vec2F(0.0f, 0.5f), Vec2F(bkSize.x + boxBorder, bkSize.y*0.5f));

	Layout textLayout = Layout(Vec2F(0.0f, 0.0f), Vec2F(bkSize.x + boxBorder*2.0f, 0.0f),
							   Vec2F(1.0f, 1.0f), Vec2F(0.0f, 4.0f));

	checkBox->AddDrawable(glowDrawable, "glow", bkLayout);
	checkBox->AddDrawable(bkDrawable, "back", bkLayout);
	checkBox->AddDrawable(hoverDrawable, "hover", bkLayout);
	checkBox->AddDrawable(pressedDrawable, "pressed", bkLayout);
	checkBox->AddDrawable(checkDrawable, "check", bkLayout);
	checkBox->AddDrawable(captionDrawable, "caption", textLayout);

	//hover state
	UITransitionState* hoverState = mnew UITransitionState("hover");
	hoverState->AddProperty(&hoverDrawable->transparency,
							AnimationFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//pressed state
	UITransitionState* pressedState = mnew UITransitionState("pressed");
	pressedState->AddProperty(&pressedDrawable->transparency,
							  AnimationFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
							  AnimationFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	//focus state
	UITransitionState* focusState = mnew UITransitionState("focus");
	focusState->AddProperty(&glowDrawable->transparency,
							AnimationFrame<float>(0.0f, focusingDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, focusingDelayOn, false, IT_LINEAR));

	//check state
	UITransitionState* checkState = mnew UITransitionState("checked");
	checkState->AddProperty(&checkDrawable->transparency,
							AnimationFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	//adding states
	checkBox->AddState(hoverState);
	checkBox->AddState(pressedState);
	checkBox->AddState(focusState);
	checkBox->AddState(checkState);

	//set as sample in skin manager
	mSkinManager->SetCheckBoxSample(checkBox);
}

void UIStdSkinInitializer::InitHorScrollBar()
{
	const String backgroundTexName = "ui_skin/bar_bk";
	const String barTexName = "ui_skin/bar_bar";
	const String barHoverTexName = "ui_skin/bar_bar_hover";
	const String barPressedTexName = "ui_skin/bar_bar_pressed";
	const String barIconTexName = "ui_skin/bar_drag_icn";

	TextureRef backgroundTex = TextureRef::CreateFromFile(backgroundTexName);
	TextureRef barTex = TextureRef::CreateFromFile(barTexName);
	TextureRef barHoverTex = TextureRef::CreateFromFile(barHoverTexName);
	TextureRef barPressedTex = TextureRef::CreateFromFile(barPressedTexName);

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float pressingDelayOn = 0.01f;
	const float pressingDelayOff= 0.1f;
	const float barHeight = 13.0f;

	//drawables
	const float backgroundLeftBorder = 11;
	const float backgroundRightBorder = 12;
	StretchRect* backgroundDrawable = mnew StretchRect(backgroundTex, (int)backgroundLeftBorder, 0, (int)backgroundRightBorder, 0);

	const float barLeftOffset = 6, barRightOffset = 8;
	const float barLeftBorder = 11, barRightBorder = 12;
	StretchRect* barDrawable = mnew StretchRect(barTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);
	StretchRect* barHoverDrawable = mnew StretchRect(barHoverTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);
	StretchRect* barPresedDrawable = mnew StretchRect(barPressedTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);

	Sprite* barIconDrawable = new Sprite(TextureRef::CreateFromFile(barIconTexName));

	//scrollbar
	UIScrollBar* scrollbar = mnew UIScrollBar(Layout::Both());

	// adding drawables
	Layout backgroundLayout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, -backgroundTex.getSize().y*0.5f),
							Vec2F(1.0f, 0.5f), Vec2F(0.0f, backgroundTex.getSize().y*0.5f));
	scrollbar->AddDrawable(backgroundDrawable, "background", backgroundLayout);

	UIScrollBar::Drawable* barDrw = scrollbar->AddDrawable(NULL, "bar");
	scrollbar->SetBarDrawable(barDrw);

	float hs = barTex.getSize().y*0.5f;
	Layout barLayout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, -hs), Vec2F(1.0f, 0.5f), Vec2F(0.0f, hs));
	barDrw->AddChildDrawable("bar", barDrawable, barLayout);
	barDrw->AddChildDrawable("hover", barHoverDrawable, barLayout);
	barDrw->AddChildDrawable("Pressed", barPresedDrawable, barLayout);

	Vec2F icnSize = barIconDrawable->GetSize();
	Vec2F icnOffs(0, 0.0f);
	barDrw->AddChildDrawable("icon", barIconDrawable,
							 Layout(Vec2F(0.5f, 0.5f), icnSize*(-0.5f) + icnOffs, Vec2F(0.5f, 0.5f), icnSize*0.5f + icnOffs));

	//states
	UITransitionState* hoverState = mnew UITransitionState("hover");
	hoverState->AddProperty(&barHoverDrawable->transparency,
							AnimationFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//pressed state
	UITransitionState* pressedState = mnew UITransitionState("pressed");
	pressedState->AddProperty(&barPresedDrawable->transparency,
							  AnimationFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
							  AnimationFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	scrollbar->AddState(hoverState);
	scrollbar->AddState(pressedState);

	//geometry
	scrollbar->SetBackgroundGeometryLayout(
		Layout(Vec2F(0.0f, 0.5f), Vec2F(barLeftOffset, -barHeight*0.5f), Vec2F(1.0f, 0.5f), Vec2F(-barRightOffset, barHeight*0.5f)));

	scrollbar->GetBarGeometryLayout(
		Layout(Vec2F(0.0f, 0.5f), Vec2F(barLeftOffset, -barHeight*0.5f), Vec2F(1.0f, 0.5f), Vec2F(-barRightOffset, barHeight*0.5f)));

	mSkinManager->SetHorScrollbarSample(scrollbar);
}

void UIStdSkinInitializer::InitHorThinScrollBar()
{
	const String backgroundTexName = "ui_skin/small_hor_bar_bk";
	const String barTexName = "ui_skin/small_hor_bar_bar";
	const String barHoverTexName = "ui_skin/small_hor_bar_bar_hover";
	const String barPressedTexName = "ui_skin/small_hor_bar_bar_pressed";

	TextureRef backgroundTex = TextureRef::CreateFromFile(backgroundTexName);
	TextureRef barTex = TextureRef::CreateFromFile(barTexName);
	TextureRef barHoverTex = TextureRef::CreateFromFile(barHoverTexName);
	TextureRef barPressedTex = TextureRef::CreateFromFile(barPressedTexName);

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float pressingDelayOn = 0.01f;
	const float pressingDelayOff= 0.1f;
	const float barHeight = 5.0f;

	//drawables
	const float backgroundLeftBorder = 6;
	const float backgroundRightBorder = 6;
	StretchRect* backgroundDrawable = mnew StretchRect(backgroundTex, (int)backgroundLeftBorder, 0, (int)backgroundRightBorder, 0);

	const float barLeftOffset = 3, barRightOffset = 3;
	const float barLeftBorder = 6, barRightBorder = 6;
	StretchRect* barDrawable = mnew StretchRect(barTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);
	StretchRect* barHoverDrawable = mnew StretchRect(barHoverTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);
	StretchRect* barPresedDrawable = mnew StretchRect(barPressedTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);

	//scrollbar
	UIScrollBar* scrollbar = mnew UIScrollBar(Layout::Both());

	// adding drawables
	Layout backgroundLayout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, -backgroundTex.getSize().y*0.5f),
							Vec2F(1.0f, 0.5f), Vec2F(0.0f, backgroundTex.getSize().y*0.5f));
	scrollbar->AddDrawable(backgroundDrawable, "background", backgroundLayout);

	UIScrollBar::Drawable* barDrw = scrollbar->AddDrawable(NULL, "bar");
	scrollbar->SetBarDrawable(barDrw);

	float hs = barTex.getSize().y*0.5f;
	Layout barLayout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, -hs), Vec2F(1.0f, 0.5f), Vec2F(0.0f, hs));
	barDrw->AddChildDrawable("bar", barDrawable, barLayout);
	barDrw->AddChildDrawable("hover", barHoverDrawable, barLayout);
	barDrw->AddChildDrawable("Pressed", barPresedDrawable, barLayout);

	//states
	UITransitionState* hoverState = mnew UITransitionState("hover");
	hoverState->AddProperty(&barHoverDrawable->transparency,
							AnimationFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//pressed state
	UITransitionState* pressedState = mnew UITransitionState("pressed");
	pressedState->AddProperty(&barPresedDrawable->transparency,
							  AnimationFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
							  AnimationFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	scrollbar->AddState(hoverState);
	scrollbar->AddState(pressedState);

	//geometry
	scrollbar->SetBackgroundGeometryLayout(
		Layout(Vec2F(0.0f, 0.5f), Vec2F(barLeftOffset, -barHeight*0.5f), Vec2F(1.0f, 0.5f), Vec2F(-barRightOffset, barHeight*0.5f)));

	scrollbar->GetBarGeometryLayout(
		Layout(Vec2F(0.0f, 0.5f), Vec2F(barLeftOffset, -barHeight*0.5f), Vec2F(1.0f, 0.5f), Vec2F(-barRightOffset, barHeight*0.5f)));

	mSkinManager->SetHorThinScrollbarSample(scrollbar);
}

void UIStdSkinInitializer::InitVerThinScrollBar()
{
	const String backgroundTexName = "ui_skin/small_vert_bar_bk";
	const String barTexName = "ui_skin/small_vert_bar_bar";
	const String barHoverTexName = "ui_skin/small_vert_bar_bar_hover";
	const String barPressedTexName = "ui_skin/small_vert_bar_bar_pressed";

	TextureRef backgroundTex = TextureRef::CreateFromFile(backgroundTexName);
	TextureRef barTex = TextureRef::CreateFromFile(barTexName);
	TextureRef barHoverTex = TextureRef::CreateFromFile(barHoverTexName);
	TextureRef barPressedTex = TextureRef::CreateFromFile(barPressedTexName);

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float pressingDelayOn = 0.01f;
	const float pressingDelayOff= 0.1f;
	const float barWidth = 5.0f;

	//drawables
	const float backgroundTopBorder = 6;
	const float backgroundBottomBorder = 6;
	StretchRect* backgroundDrawable = mnew StretchRect(backgroundTex, 0, (int)backgroundTopBorder, 0, (int)backgroundBottomBorder);

	const float barTopOffset = 3, barBottomOffset = 3;
	const float barTopBorder = 6, barBottomBorder = 6;
	StretchRect* barDrawable = mnew StretchRect(barTex, 0, (int)barTopBorder, 0, (int)barBottomBorder);
	StretchRect* barHoverDrawable = mnew StretchRect(barHoverTex, 0, (int)barTopBorder, 0, (int)barBottomBorder);
	StretchRect* barPresedDrawable = mnew StretchRect(barPressedTex, 0, (int)barTopBorder, 0, (int)barBottomBorder);

	//scrollbar
	UIScrollBar* scrollbar = mnew UIScrollBar(Layout::Both(), "", UIScrollBar::TP_VERTICAL);

	// adding drawables
	Layout backgroundLayout(Vec2F(0.5f, 0.0f), Vec2F(-backgroundTex.getSize().x*0.5f, 0.0f),
							Vec2F(0.5f, 1.0f), Vec2F(backgroundTex.getSize().x*0.5f, 0.0f));
	scrollbar->AddDrawable(backgroundDrawable, "background", backgroundLayout);

	UIScrollBar::Drawable* barDrw = scrollbar->AddDrawable(NULL, "bar");
	scrollbar->SetBarDrawable(barDrw);

	float hs = barTex.getSize().x*0.5f;
	Layout barLayout(Vec2F(0.5f, 0.0f), Vec2F(-hs, 0.0f),
					 Vec2F(0.5f, 1.0f), Vec2F(hs, 0.0f));
	barDrw->AddChildDrawable("bar", barDrawable, barLayout);
	barDrw->AddChildDrawable("hover", barHoverDrawable, barLayout);
	barDrw->AddChildDrawable("Pressed", barPresedDrawable, barLayout);

	//states
	UITransitionState* hoverState = mnew UITransitionState("hover");
	hoverState->AddProperty(&barHoverDrawable->transparency,
							AnimationFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//pressed state
	UITransitionState* pressedState = mnew UITransitionState("pressed");
	pressedState->AddProperty(&barPresedDrawable->transparency,
							  AnimationFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
							  AnimationFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	scrollbar->AddState(hoverState);
	scrollbar->AddState(pressedState);

	//geometry
	scrollbar->SetBackgroundGeometryLayout(
		Layout(Vec2F(0.5f, 0.0f), Vec2F(-barWidth*0.5f, barTopOffset),
		Vec2F(0.5f, 1.0f), Vec2F(barWidth*0.5f, -barBottomOffset)));

	scrollbar->GetBarGeometryLayout(
		Layout(Vec2F(0.5f, 0.0f), Vec2F(-barWidth*0.5f, barTopOffset),
		Vec2F(0.5f, 1.0f), Vec2F(barWidth*0.5f, -barBottomOffset)));

	mSkinManager->SetVerThinScrollbarSample(scrollbar);
}

void UIStdSkinInitializer::InitSingleLineEditBox()
{
	const String bgTexName = "ui_skin/editbox_bk";
	const String hoverTexName = "ui_skin/editbox_hover";
	const String glowTexName = "ui_skin/editbox_glow";

	const Color4 cursorColor(180, 180, 180, 255);
	const Color4 selectionColor(150, 150, 230, 130);

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float focusingDelayOn = 0.1f;
	const float focusingDelayOff= 0.6f;
	const float cursorBlinkDelay = 0.5f;

	TextureRef bgTex = TextureRef::CreateFromFile(bgTexName);
	TextureRef glowTex = TextureRef::CreateFromFile(glowTexName);
	TextureRef hoverTex = TextureRef::CreateFromFile(hoverTexName);

	//drawables
	RectF borders(6.0f, 6.0f, 8.0f, 7.0f);
	StretchRect* bgDrawable = mnew StretchRect(bgTex, (int)borders.left, (int)borders.top, (int)borders.right, (int)borders.down);
	StretchRect* hoverDrawable = mnew StretchRect(hoverTex, (int)borders.left, (int)borders.top, (int)borders.right, (int)borders.down);
	StretchRect* glowDrawable = mnew StretchRect(glowTex, (int)borders.left, (int)borders.top, (int)borders.right, (int)borders.down);

	//editbox
	UIEditBox* editbox = mnew UIEditBox(mStdFont, Layout::Both());

	//adding drawables
	float hs = bgTex.getSize().y*0.5f;
	Layout drawablesLayout(Vec2F(0.0f, 0.5f), Vec2F(-5.0f, -hs), Vec2F(1.0f, 0.5f), Vec2F(5.0f, hs));// = cLayout::both(fRect(-5, -5, -5, -5));
	editbox->AddDrawable(glowDrawable, "glow", drawablesLayout);
	editbox->AddDrawable(bgDrawable, "background", drawablesLayout);
	editbox->AddDrawable(hoverDrawable, "hover", drawablesLayout);

	hs -= 2.0f;
	editbox->mClippingLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(3.0f, -hs), Vec2F(1.0f, 0.5f), Vec2F(-3.0f, hs));
	editbox->mTextLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(3.0f, -hs), Vec2F(1.0f, 0.5f), Vec2F(-3.0f, hs + 2.0f));
	editbox->mText->SetLinesDistCoef(0.9f);

	//states
	// //hover state
	UITransitionState* hoverState = mnew UITransitionState("hover");
	hoverState->AddProperty(&hoverDrawable->transparency,
							AnimationFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//focus state
	UITransitionState* focusState = mnew UITransitionState("focus");
	focusState->AddProperty(&glowDrawable->transparency,
							AnimationFrame<float>(0.0f, focusingDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, focusingDelayOn, false, IT_LINEAR));

	//adding states
	editbox->AddState(hoverState);
	editbox->AddState(focusState);

	//colors
	editbox->SetCursorColor(cursorColor);
	editbox->SetSelectionColor(selectionColor);

	editbox->SetCursorVisibleDelay(cursorBlinkDelay);

	mSkinManager->SetSingleLineEditBoxSample(editbox);
}

void UIStdSkinInitializer::InitLabel()
{
	UILabel* label = mnew UILabel(mStdFont, Layout::Both());
	label->SetHorAlign(Font::HA_CENTER);
	label->SetVerAlign(Font::VA_CENTER);
	mSkinManager->SetLabelSample(label);
}

void UIStdSkinInitializer::InitMultilineEditBox()
{
	const String bgTexName = "ui_skin/pad_bk";
	const String hoverTexName = "ui_skin/pad_hover";
	const String glowTexName = "ui_skin/pad_glow";

	const Color4 cursorColor(180, 180, 180, 255);
	const Color4 selectionColor(150, 150, 230, 130);

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float focusingDelayOn = 0.1f;
	const float focusingDelayOff= 0.6f;
	const float cursorBlinkDelay = 0.5f;

	TextureRef bgTex = TextureRef::CreateFromFile(bgTexName);
	TextureRef glowTex = TextureRef::CreateFromFile(glowTexName);
	TextureRef hoverTex = TextureRef::CreateFromFile(hoverTexName);

	//drawables
	RectF borders(6.0f, 6.0f, 8.0f, 7.0f);
	StretchRect* bgDrawable = mnew StretchRect(bgTex, (int)borders.left, (int)borders.top, (int)borders.right, (int)borders.down);
	StretchRect* hoverDrawable = mnew StretchRect(hoverTex, (int)borders.left, (int)borders.top, (int)borders.right, (int)borders.down);
	StretchRect* glowDrawable = mnew StretchRect(glowTex, (int)borders.left, (int)borders.top, (int)borders.right, (int)borders.down);


	//scrollbars
	float scrollBarSize = 11.0f;
	UIScrollBar* horScrollbar = mSkinManager->HorThinScrollBar(
		Layout(Vec2F(0.0f, 1.0f), Vec2F(0.0f, -scrollBarSize), Vec2F(1.0f, 1.0f), Vec2F(-scrollBarSize, 0.0f)),
		"horScrollbar");

	UIScrollBar* verScrollbar = mSkinManager->VerThinScrollBar(
		Layout(Vec2F(1.0f, 0.0f), Vec2F(-scrollBarSize, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(0.0f, 0.0f)),
		"verScrollbar");

	//editbox
	UIEditBox* editbox = mnew UIEditBox(mStdFont, Layout::Both(), horScrollbar, verScrollbar);

	//adding drawables
	Layout drawablesLayout = Layout::Both(RectF(-5.0f, -5.0f, -5.0f, -5.0f));
	editbox->AddDrawable(glowDrawable, "glow", drawablesLayout);
	editbox->AddDrawable(bgDrawable, "background", drawablesLayout);
	editbox->AddDrawable(hoverDrawable, "hover", drawablesLayout);

	editbox->mClippingLayout = Layout::Both(RectF(3.0f, 3.0f, scrollBarSize + 3.0f, scrollBarSize + 3.0f));
	editbox->mTextLayout = editbox->mClippingLayout;
	editbox->mText->SetVerAlign(Font::VA_TOP);
	editbox->mText->SetLinesDistCoef(0.9f);
	editbox->mMultiLine = true;

	//states
	// //hover state
	UITransitionState* hoverState = mnew UITransitionState("hover");
	hoverState->AddProperty(&hoverDrawable->transparency,
							AnimationFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//focus state
	UITransitionState* focusState = mnew UITransitionState("focus");
	focusState->AddProperty(&glowDrawable->transparency,
							AnimationFrame<float>(0.0f, focusingDelayOff, false, IT_LINEAR),
							AnimationFrame<float>(1.0f, focusingDelayOn, false, IT_LINEAR));

	//adding states
	editbox->AddState(hoverState);
	editbox->AddState(focusState);

	//colors
	editbox->SetCursorColor(cursorColor);
	editbox->SetSelectionColor(selectionColor);

	editbox->SetCursorVisibleDelay(cursorBlinkDelay);

	mSkinManager->SetMultilineEditBoxSample(editbox);
}

void UIStdSkinInitializer::InitScrollArea()
{
	const String bgTexName = "ui_skin/pad_bk";

	TextureRef bgTex = TextureRef::CreateFromFile(bgTexName);

	//drawables
	RectF borders(8.0f, 8.0f, 8.0f, 8.0f);
	StretchRect* bgDrawable = mnew StretchRect(bgTex, (int)borders.left, (int)borders.top, (int)borders.right, (int)borders.down);

	//scrollbars
	float scrollBarSize = 11.0f;
	UIScrollBar* horScrollbar = mSkinManager->HorThinScrollBar(
		Layout(Vec2F(0.0f, 1.0f), Vec2F(0.0f, -scrollBarSize), Vec2F(1.0f, 1.0f), Vec2F(-scrollBarSize, 0.0f)),
		"horScrollbar");

	UIScrollBar* verScrollbar = mSkinManager->VerThinScrollBar(
		Layout(Vec2F(1.0f, 0.0f), Vec2F(-scrollBarSize, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(0.0f, 0.0f)),
		"verScrollbar");

	//scrollarea
	UIScrollArea* scrollArea = mnew UIScrollArea(Layout::Both(), horScrollbar, verScrollbar);

	//adding drawables
	scrollArea->AddDrawable(bgDrawable, "background", Layout::Both(RectF(-5.0f, -5.0f, -5.0f, -5.0f)));

	//clipping area
	scrollArea->mClippingLayout = Layout::Both(RectF(0.0f, 0.0f, scrollBarSize, scrollBarSize));

	mSkinManager->SetScrollAreaSample(scrollArea);
}

void UIStdSkinInitializer::InitHint()
{
	const String bgTexName = "ui_skin/hint_bk";
	TextureRef bgTex = TextureRef::CreateFromFile(bgTexName);

	UIRect* hintWidget = mnew UIRect(Layout::Fixed(Vec2F(100.0f, 100.0f), Vec2F()), "hint");
	hintWidget->mStretchRect = StretchRect(bgTex, 9, 9, 10, 10);

	UILabel* label = mnew UILabel(mStdFont, Layout::Both(RectF(6.0f, 5.0f, 6.0f, 6.0f)), "label");
	label->SetLinesDistCoef(0.8f);

	hintWidget->AddChild(label);
	mSkinManager->AddVisibleState(hintWidget, 0.3f);

	AppUI()->mHintController.setupWidget(hintWidget, label);
}

void UIStdSkinInitializer::InitRectPad()
{
	const String texName = "ui_skin/pad_bk";

	TextureRef tex = TextureRef::CreateFromFile(texName);
	UIRect* pad = mnew UIRect(Layout::Both(), "pad");
	pad->mStretchRect = StretchRect(tex, 9, 9, 10, 10);
	pad->SetChildsLayout(Layout::Both(RectF(10.0f, 10.0f, 11.0f, 11.0f)));

	mSkinManager->SetRectPadSample(pad);
}


CLOSE_O2_NAMESPACE