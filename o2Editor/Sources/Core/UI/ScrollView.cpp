#include "stdafx.h"
#include "ScrollView.h"

#include "Application/Application.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	UIScrollView::UIScrollView()
	{
		mRenderTarget = TextureRef(Vec2I(256, 256), Texture::Format::Default, Texture::Usage::RenderTarget);
		mRenderTargetSprite = mnew Sprite(mRenderTarget, RectI(0, 0, 256, 256));

		mBackColor = Color4(170, 170, 170, 255);
		mGridColor = Color4(190, 190, 190, 255);

		mReady = true;
	}

	UIScrollView::UIScrollView(const UIScrollView& other):
		UIWidget(other), mBackColor(other.mBackColor), mGridColor(other.mGridColor)
	{
		mRenderTarget = TextureRef(Vec2I(256, 256), Texture::Format::Default, Texture::Usage::RenderTarget);
		mRenderTargetSprite = mnew Sprite(mRenderTarget, RectI(0, 0, 256, 256));

		RetargetStatesAnimations();

		mReady = true;
	}

	UIScrollView::~UIScrollView()
	{
		if (mRenderTargetSprite)
			delete mRenderTargetSprite;
	}

	UIScrollView& UIScrollView::operator=(const UIScrollView& other)
	{
		CopyData(other);
		return *this;
	}

	void UIScrollView::Draw()
	{
		UIWidget::Draw();

		if (!mReady)
			return;

		if (mNeedRedraw)
			RedrawRenderTarget();

		mRenderTargetSprite->Draw();

		CursorAreaEventsListener::OnDrawn();
	}

	void UIScrollView::Update(float dt)
	{
		UIWidget::Update(dt);

		if (!mReady)
			return;

		UpdateCamera(dt);
	}

	void UIScrollView::SetBackColor(const Color4& color)
	{
		mBackColor = color;
	}

	void UIScrollView::SetGridColor(const Color4& color)
	{
		mGridColor = color;
	}

	void UIScrollView::UpdateTransform(bool withChildren /*= true*/)
	{
		UIWidget::UpdateTransform(withChildren);
	}

	bool UIScrollView::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}

	bool UIScrollView::IsScrollable() const
	{
		return true;
	}

	bool UIScrollView::IsFocusable() const
	{
		return true;
	}

	void UIScrollView::CopyData(const Actor& otherActor)
	{
		const UIScrollView& other = dynamic_cast<const UIScrollView&>(otherActor);

		UIWidget::CopyData(other);

		mReady = false;

		delete mRenderTargetSprite;

		mBackColor          = other.mBackColor;
		mGridColor          = other.mGridColor;
		mRenderTarget       = TextureRef(Vec2I(256, 256), Texture::Format::Default, Texture::Usage::RenderTarget);
		mRenderTargetSprite = mnew Sprite(mRenderTarget, RectI(0, 0, 256, 256));

		RetargetStatesAnimations();

		mReady = true;
	}

	void UIScrollView::OnTransformUpdated()
	{
		if (!mReady)
			return;

		Vec2I size = layout->size.Get();
		size.x = Math::Max(size.x, 32);
		size.y = Math::Max(size.y, 32);

		mRenderTarget = TextureRef(size, Texture::Format::Default, Texture::Usage::RenderTarget);
		*mRenderTargetSprite = Sprite(mRenderTarget, RectI(Vec2I(), size));
		mRenderTargetSprite->SetRect(layout->worldRect);
		mNeedRedraw = true;

		mViewCamera.size = size;
	}

	void UIScrollView::UpdateTransparency()
	{
		UIWidget::UpdateTransparency();
		mRenderTargetSprite->SetTransparency(mResTransparency);
	}

	void UIScrollView::UpdateCamera(float dt)
	{
		bool transformed = false;

		if (mViewCameraTargetScale.x < mViewCameraMinScale)
		{
			mViewCameraTargetScale.x = Math::Lerpc<float>(mViewCameraTargetScale.x, mViewCameraMinScale,
														  dt*mViewCameraScaleElasticyCoef);
		}
		if (mViewCameraTargetScale.y < mViewCameraMinScale)
		{
			mViewCameraTargetScale.y = Math::Lerpc<float>(mViewCameraTargetScale.y, mViewCameraMinScale,
														  dt*mViewCameraScaleElasticyCoef);
		}

		if (mViewCameraTargetScale.x > mViewCameraMaxScale)
		{
			mViewCameraTargetScale.x = Math::Lerpc<float>(mViewCameraTargetScale.x, mViewCameraMaxScale,
														  dt*mViewCameraScaleElasticyCoef);
		}

		if (mViewCameraTargetScale.y > mViewCameraMaxScale)
		{
			mViewCameraTargetScale.y = Math::Lerpc<float>(mViewCameraTargetScale.y, mViewCameraMaxScale,
														  dt*mViewCameraScaleElasticyCoef);
		}

		if (!Math::Equals<float>(mViewCamera.GetScale().x, mViewCameraTargetScale.x) ||
			!Math::Equals<float>(mViewCamera.GetScale().y, mViewCameraTargetScale.y))
		{
			mViewCamera.scale = Math::Lerpc<Vec2F>(mViewCamera.scale, mViewCameraTargetScale,
												   dt*mViewCameraScaleElasticyCoef);

			transformed = true;
		}

		if (mViewCameraVelocity.Length() > 0.05f && !o2Input.IsRightMouseDown())
		{
			mViewCameraVelocity = Math::Lerpc<Vec2F>(mViewCameraVelocity, Vec2F(), dt*mViewCameraVelocityDampingCoef);
			mViewCameraTargetPos += mViewCameraVelocity*dt;
		}

		if (mViewCamera.position != mViewCameraTargetPos)
		{
			mViewCamera.position = Math::Lerpc<Vec2F>(mViewCamera.position, mViewCameraTargetPos,
													  dt*mViewCameraPosElasticyCoef);

			transformed = true;
		}

		if (transformed)
		{
			UpdateLocalScreenTransforms();
			OnCameraTransformChanged();
			mNeedRedraw = true;
		}
	}

	void UIScrollView::UpdateLocalScreenTransforms()
	{
		RectF rectangle = layout->worldRect;
		Basis identityCamTransform = Transform(rectangle.Size()).basis;
		Basis cameraTransform = mViewCamera.basis;

		Basis sceneToCamTransform = identityCamTransform.Inverted()*cameraTransform;
		Basis drawRectTransform = Transform(rectangle.Size(), rectangle.Center()).basis;

		mScreenToLocalTransform = Basis::Translated(rectangle.Center()*-1.0f)*sceneToCamTransform;
		mLocalToScreenTransform = mScreenToLocalTransform.Inverted();
	}

	Vec2F UIScrollView::ScreenToLocalPoint(const Vec2F& point)
	{
		return point*mScreenToLocalTransform;
	}

	Vec2F UIScrollView::LocalToScreenPoint(const Vec2F& point)
	{
		return point*mLocalToScreenTransform;
	}

	const Camera& UIScrollView::GetCamera() const
	{
		return mViewCamera;
	}

	void UIScrollView::RedrawRenderTarget()
	{
		mNeedRedraw = false;
		UpdateLocalScreenTransforms();
		o2Render.SetRenderTexture(mRenderTarget);

		o2Render.Clear(mBackColor);
		o2Render.SetCamera(mViewCamera);

		RedrawContent();

		o2Render.UnbindRenderTexture();
		o2Render.SetCamera(Camera());
	}

	void UIScrollView::RedrawContent()
	{
		DrawGrid();
	}

	void UIScrollView::DrawGrid()
	{
		float cameraMaxSize = Math::Max(mViewCamera.GetSize().x*mViewCamera.GetScale().x,
										mViewCamera.GetSize().y*mViewCamera.GetScale().y);

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

		Vec2F gridOrigin(Math::Round(mViewCamera.GetPosition().x / cellSize)*cellSize,
						 Math::Round(mViewCamera.GetPosition().y / cellSize)*cellSize);

		int cellsCount = Math::CeilToInt(cameraMaxSize / cellSize);
		float tenCeilsSize = cellSize*10.0f;
		float screenCellSize = cellSize / mViewCamera.GetScale().x;
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
	}

	void UIScrollView::OnCameraTransformChanged()
	{
	}

	void UIScrollView::OnScrolled(float scroll)
	{
		mViewCameraTargetScale *= 1.0f - (scroll*mViewCameraScaleSence);
	}

	void UIScrollView::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(true);
	}

	void UIScrollView::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{
		if (cursor.delta.Length() > 0.5f)
		{
			Vec2F delta = cursor.delta*mViewCamera.scale*-1.0f;
			mViewCameraVelocity = delta / o2Time.GetDeltaTime();
			mViewCameraTargetPos += delta;
			mNeedRedraw = true;
		}
	}

	void UIScrollView::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
	}
}

DECLARE_CLASS(Editor::UIScrollView);
