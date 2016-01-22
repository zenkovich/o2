#include "SceneEditWidget.h"

#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Utils/Math/Math.h"


SceneEditWidget::SceneEditWidget():
	UIWidget(), mNeedRedraw(false)
{
	test = mnew Sprite("ui/UI_Background.png");
	mRenderTargetSprite = mnew Sprite();
	RetargetStatesAnimations();
}

SceneEditWidget::SceneEditWidget(const SceneEditWidget& other):
	UIWidget(other), mNeedRedraw(false)
{
	test = mnew Sprite("ui/UI_Background.png");
	mRenderTargetSprite = mnew Sprite();
	RetargetStatesAnimations();
}

SceneEditWidget::~SceneEditWidget()
{
	delete mRenderTargetSprite;
}

SceneEditWidget& SceneEditWidget::operator=(const SceneEditWidget& other)
{
	UIWidget::operator=(other);
	return *this;
}

void SceneEditWidget::Draw()
{
	mDrawDepth = o2Render.GetDrawingDepth();

	if (mNeedRedraw)
		RedrawScene();

	mRenderTargetSprite->Draw();
}

void SceneEditWidget::Update(float dt)
{
	UIWidget::Update(dt);

	if (!Math::Equals<float>(mViewCamera.scale->x, mViewCameraTargetScale))
	{
		mViewCamera.scale = 
			Math::Lerp<Vec2F>(mViewCamera.scale, Vec2F(mViewCameraTargetScale, mViewCameraTargetScale), dt*30.0f);

		mNeedRedraw = true;
	}
}

bool SceneEditWidget::IsUnderPoint(const Vec2F& point)
{
	return layout.GetAbsoluteRect().IsInside(point);
}

float SceneEditWidget::Depth()
{
	return mDrawDepth + 1.0f;
}

bool SceneEditWidget::IsScrollable() const
{
	return true;
}

void SceneEditWidget::UpdateLayout(bool forcible /*= false*/)
{
	UIWidget::UpdateLayout(forcible);

	RectF absRect = layout.GetAbsoluteRect();
	if (absRect.Size().Length() > 1.0f)
	{
		mRenderTarget = TextureRef(absRect.Size(), Texture::Format::Default, Texture::Usage::RenderTarget);
		mRenderTargetSprite->SetTexture(mRenderTarget);
		mRenderTargetSprite->SetTextureSrcRect(RectI(Vec2I(), mRenderTarget->GetSize()));
		mRenderTargetSprite->SetRect(absRect);
		test->SetRect(RectF(-50, -50, 50, 50));
		mNeedRedraw = true;
		mViewCamera.size = absRect.Size();
	}
}

void SceneEditWidget::OnScrolled(float scroll)
{
	mViewCameraTargetScale *= 1.0f - (scroll / 120.0f * 0.1f);
}

void SceneEditWidget::OnCursorStillDown(const Input::Cursor& cursor)
{
	if (cursor.mDelta.Length() > 0.5f)
	{
		mViewCamera.position -= cursor.mDelta*mViewCamera.scale;
		mNeedRedraw = true;
	}
}

void SceneEditWidget::RedrawScene()
{
	o2Render.SetRenderTexture(mRenderTarget);

	o2Render.Clear(Color4(170, 170, 170, 255));
	o2Render.SetCamera(mViewCamera);

	Color4 clr(200, 200, 200, 255);
	for (int i = -15; i < 15; i++)
	{
		o2Render.DrawLine(Vec2F(-150, i * 10), Vec2F(150, i * 10), clr);
		o2Render.DrawLine(Vec2F(i * 10, -150), Vec2F(i * 10, 150), clr);
	}

	test->Draw();

	o2Render.UnbindRenderTexture();
	o2Render.SetCamera(Camera());
	mNeedRedraw = false;
}
