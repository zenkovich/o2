#include "o2Editor/stdafx.h"
#include "GameWindow.h"

namespace Editor
{
	GameWindow::GameWindow() :
		IEditorWindow()
	{
		InitializeWindow();
	}

	GameWindow::GameWindow(const GameWindow& other) :
		IEditorWindow(other)
	{
		InitializeWindow();
	}

	GameWindow::~GameWindow()
	{}

	void GameWindow::InitializeWindow()
	{
		mWindow->caption = "Game";
		mWindow->name = "game window";
		mWindow->SetIcon(mnew Sprite("ui/UI4_game_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 2)));
		mWindow->SetViewLayout(Layout::BothStretch(-1, 0, 0, 18));

		auto gameView = mnew GameView();
		*gameView->layout = WidgetLayout::BothStretch(0, 0, 0, 19);
		mWindow->AddChild(gameView);
		
		Widget* upPanel = mnew Widget();
		upPanel->name = "up panel";
		*upPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		upPanel->AddLayer("back", mnew Sprite("ui/UI4_small_panel_back.png"), Layout::BothStretch(-5, -4, -4, -5));
		mWindow->AddChild(upPanel);

		auto layersButton = o2UI.CreateWidget<Button>("panel down");
		layersButton->caption = "Resolution";
		*layersButton->layout = WidgetLayout::VerStretch(HorAlign::Right, 0, 0, 100, 0);
		upPanel->AddChild(layersButton);
	}

	GameWindow::GameView::GameView()
	{
		mRenderTarget = TextureRef(Vec2I(256, 256), PixelFormat::R8G8B8A8, Texture::Usage::RenderTarget);
		mRenderTargetSprite = mnew Sprite(mRenderTarget, RectI(0, 0, 256, 256));
	}

	void GameWindow::GameView::Draw()
	{
		Widget::Draw();

		o2Render.SetRenderTexture(mRenderTarget);

		o2Render.Clear();
		o2Render.SetCamera(Camera());

		o2Scene.Draw();

		o2Render.UnbindRenderTexture();
		o2Render.SetCamera(Camera());

		mRenderTargetSprite->Draw();
	}

	void GameWindow::GameView::OnTransformUpdated()
	{
		Widget::OnTransformUpdated();

		Vec2I size = layout->size.Get();
		size.x = Math::Max(size.x, 32);
		size.y = Math::Max(size.y, 32);

		mRenderTarget = TextureRef(size, PixelFormat::R8G8B8A8, Texture::Usage::RenderTarget);
		*mRenderTargetSprite = Sprite(mRenderTarget, RectI(Vec2I(), size));
		mRenderTargetSprite->SetRect(layout->worldRect);
	}

}

DECLARE_CLASS(Editor::GameWindow);

DECLARE_CLASS(Editor::GameWindow::GameView);
