#include "SceneEditWidget.h"

#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/DrawableComponent.h"
#include "Scene/Scene.h"
#include "Utils/Math/Math.h"


SceneEditWidget::SceneEditWidget():
	UIWidget(), mNeedRedraw(false)
{
	mRenderTargetSprite = mnew Sprite();
	RetargetStatesAnimations();
}

SceneEditWidget::SceneEditWidget(const SceneEditWidget& other):
	UIWidget(other), mNeedRedraw(false)
{
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

#undef DrawText

void SceneEditWidget::Update(float dt)
{
	UIWidget::Update(dt);
	UpdateCamera(dt);
}

void SceneEditWidget::UpdateCamera(float dt)
{
	if (mViewCameraTargetScale < mViewCameraMinScale)
	{
		mViewCameraTargetScale = Math::Lerp<float>(mViewCameraTargetScale, mViewCameraMinScale,
												   dt*mViewCameraScaleElasticyCoef);
	}

	if (mViewCameraTargetScale > mViewCameraMaxScale)
	{
		mViewCameraTargetScale = Math::Lerp<float>(mViewCameraTargetScale, mViewCameraMaxScale,
												   dt*mViewCameraScaleElasticyCoef);
	}

	if (!Math::Equals<float>(mViewCamera.scale->x, mViewCameraTargetScale))
	{
		mViewCamera.scale =
			Math::Lerp<Vec2F>(mViewCamera.scale, Vec2F(mViewCameraTargetScale, mViewCameraTargetScale),
							  dt*mViewCameraScaleElasticyCoef);

		mNeedRedraw = true;
	}

	if (mViewCameraVelocity.Length() > 0.05f && !o2Input.IsCursorDown())
	{
		mViewCameraVelocity = Math::Lerp<Vec2F>(mViewCameraVelocity, Vec2F(), dt*mViewCameraVelocityDampingCoef);
		mViewCameraTargetPos += mViewCameraVelocity*dt;
	}

	if (mViewCamera.position != mViewCameraTargetPos)
	{
		mViewCamera.position = Math::Lerp<Vec2F>(mViewCamera.position, mViewCameraTargetPos,
												 dt*mViewCameraPosElasticyCoef);
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
		mNeedRedraw = true;
		mViewCamera.size = absRect.Size();
	}
}

void SceneEditWidget::OnScrolled(float scroll)
{
	mViewCameraTargetScale *= 1.0f - (scroll*mViewCameraScaleSence);
}

void SceneEditWidget::OnCursorStillDown(const Input::Cursor& cursor)
{
	if (cursor.mDelta.Length() > 0.5f)
	{
		Vec2F delta = cursor.mDelta*mViewCamera.scale*-1.0f;
		mViewCameraVelocity = delta / o2Time.GetDeltaTime();
		mViewCameraTargetPos += delta;
		mNeedRedraw = true;
	}
}

void SceneEditWidget::RedrawScene()
{
	o2Render.SetRenderTexture(mRenderTarget);

	o2Render.Clear(mBackColor);
	o2Render.SetCamera(mViewCamera);

	DrawGrid();

	for (auto layer : o2Scene.GetLayers())
		for (auto drw : layer->enabledDrawables)
			drw->Draw();

	o2Render.UnbindRenderTexture();
	o2Render.SetCamera(Camera());
	mNeedRedraw = false;
}

void SceneEditWidget::DrawGrid()
{
	float cameraMaxSize = Math::Max(mViewCamera.size->x*mViewCamera.scale->x,
									mViewCamera.size->y*mViewCamera.scale->y);

	float x = cameraMaxSize / 4.0f;
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

	for (int i = -cellsCount / 2; i < cellsCount / 2; i++)
	{
		float d = (float)i*cellSize;
		Vec2F dorigin = gridOrigin + Vec2F(d, d);

		float rdx = Math::Abs(dorigin.x / tenCeilsSize - Math::Floor(dorigin.x / tenCeilsSize));
		float rdy = Math::Abs(dorigin.y / tenCeilsSize - Math::Floor(dorigin.y / tenCeilsSize));
		bool xTen = rdx < 0.05f || rdx > 0.95f;
		bool yTen = rdy < 0.05f || rdy > 0.95f;

		o2Render.DrawLine(Vec2F(-cameraMaxSize, d) + gridOrigin,
						  Vec2F(cameraMaxSize, d) + gridOrigin,
						  yTen ? mGridColor : cellColorSmoothed);

		o2Render.DrawLine(Vec2F(d, -cameraMaxSize) + gridOrigin,
						  Vec2F(d, cameraMaxSize) + gridOrigin,
						  xTen ? mGridColor : cellColorSmoothed);
	}

	o2Render.DrawRectFrame(Vec2F(-100, -100), Vec2F(100, 100), Color4::Red());
	o2Render.DrawRectFrame(Vec2F(-0.5f, -0.5f), Vec2F(0.5f, 0.5f), Color4::Red());
}
