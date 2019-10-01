#include "stdafx.h"
#include "ScrollView.h"

#include "Application/Application.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{
	ScrollView::ScrollView()
	{
		mRenderTarget = TextureRef(Vec2I(256, 256), PixelFormat::R8G8B8A8, Texture::Usage::RenderTarget);
		mRenderTargetSprite = mnew Sprite(mRenderTarget, RectI(0, 0, 256, 256));

		mBackColor = Color4(225, 232, 232, 255);
		mGridColor = Color4(190, 190, 190, 255);

		mReady = true;
	}

	ScrollView::ScrollView(const ScrollView& other):
		Widget(other), mBackColor(other.mBackColor), mGridColor(other.mGridColor)
	{
		mRenderTarget = TextureRef(Vec2I(256, 256), PixelFormat::R8G8B8A8, Texture::Usage::RenderTarget);
		mRenderTargetSprite = mnew Sprite(mRenderTarget, RectI(0, 0, 256, 256));

		RetargetStatesAnimations();

		mReady = true;
	}

	ScrollView::~ScrollView()
	{
		if (mRenderTargetSprite)
			delete mRenderTargetSprite;
	}

	ScrollView& ScrollView::operator=(const ScrollView& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void ScrollView::Draw()
	{
		Widget::Draw();

		if (!mReady)
			return;

		if (mNeedRedraw)
			RedrawRenderTarget();

		mRenderTargetSprite->Draw();

		CursorAreaEventsListener::OnDrawn();
	}

	void ScrollView::Update(float dt)
	{
		Widget::Update(dt);

		if (!mReady)
			return;

		UpdateCamera(dt);
	}

	void ScrollView::SetBackColor(const Color4& color)
	{
		mBackColor = color;
	}

	void ScrollView::SetGridColor(const Color4& color)
	{
		mGridColor = color;
	}

	void ScrollView::UpdateSelfTransform()
{
		Widget::UpdateSelfTransform();
	}

	bool ScrollView::IsUnderPoint(const Vec2F& point)
	{
		return Widget::IsUnderPoint(point);
	}

	bool ScrollView::IsScrollable() const
	{
		return true;
	}

	bool ScrollView::IsFocusable() const
	{
		return true;
	}

	void ScrollView::CopyData(const Actor& otherActor)
	{
		const ScrollView& other = dynamic_cast<const ScrollView&>(otherActor);

		Widget::CopyData(other);

		mReady = false;

		delete mRenderTargetSprite;

		mBackColor          = other.mBackColor;
		mGridColor          = other.mGridColor;
		mRenderTarget       = TextureRef(Vec2I(256, 256), PixelFormat::R8G8B8A8, Texture::Usage::RenderTarget);
		mRenderTargetSprite = mnew Sprite(mRenderTarget, RectI(0, 0, 256, 256));

		RetargetStatesAnimations();

		mReady = true;
	}

	void ScrollView::OnTransformUpdated()
	{
		if (!mReady)
			return;

		Vec2I size = layout->size.Get();
		size.x = Math::Max(size.x, 32);
		size.y = Math::Max(size.y, 32);

		mRenderTarget = TextureRef(size, PixelFormat::R8G8B8A8, Texture::Usage::RenderTarget);
		*mRenderTargetSprite = Sprite(mRenderTarget, RectI(Vec2I(), size));
		mRenderTargetSprite->SetRect(layout->worldRect);
		mNeedRedraw = true;

		mViewCamera.size = size;
	}

	void ScrollView::UpdateTransparency()
	{
		Widget::UpdateTransparency();
		mRenderTargetSprite->SetTransparency(mResTransparency);
	}

	void ScrollView::UpdateCamera(float dt)
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

	Basis ScrollView::GetCameraScreenToLocalTransform(const Camera& camera) const
	{
		RectF rectangle = layout->worldRect;
		Basis identityCamTransform = Transform(rectangle.Size()).basis;
		Basis cameraTransform = camera.basis;

		Basis sceneToCamTransform = identityCamTransform.Inverted()*cameraTransform;
		Basis drawRectTransform = Transform(rectangle.Size(), rectangle.Center()).basis;

		return Basis::Translated(rectangle.Center()*-1.0f)*sceneToCamTransform;
	}

	void ScrollView::UpdateLocalScreenTransforms()
	{
		mScreenToLocalTransform = GetCameraScreenToLocalTransform(mViewCamera);
		mLocalToScreenTransform = mScreenToLocalTransform.Inverted();
	}

	Vec2F ScrollView::ScreenToLocalPoint(const Vec2F& point)
	{
		return point*mScreenToLocalTransform;
	}

	Vec2F ScrollView::LocalToScreenPoint(const Vec2F& point)
	{
		return point*mLocalToScreenTransform;
	}

	Vec2F ScrollView::GetCameraScale() const
	{
		return Vec2F(mScreenToLocalTransform.xv.Length(), mScreenToLocalTransform.yv.Length());
	}

	void ScrollView::SetCamera(const Camera& camera)
	{
		mViewCamera = camera;
		mViewCameraTargetPos = mViewCamera.position;
		mViewCameraTargetScale = mViewCamera.scale;
	}

	const Camera& ScrollView::GetCamera() const
	{
		return mViewCamera;
	}

	void ScrollView::RedrawRenderTarget()
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

	void ScrollView::RedrawContent()
	{
		DrawGrid();
	}

	void ScrollView::DrawGrid()

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

			if (verGridEnabled)
			{
				o2Render.DrawLine(Vec2F(-cameraMaxSize, d) + gridOrigin,
								  Vec2F(cameraMaxSize, d) + gridOrigin,
								  yTen ? mGridColor : cellColorSmoothed);
			}

			if (horGridEnabled)
			{
				o2Render.DrawLine(Vec2F(d, -cameraMaxSize) + gridOrigin,
								  Vec2F(d, cameraMaxSize) + gridOrigin,
								  xTen ? mGridColor : cellColorSmoothed);
			}
		}
	}

	void ScrollView::OnCameraTransformChanged()
	{
	}

	void ScrollView::OnScrolled(float scroll)
	{
		ChangeCameraScaleRelativeToCursor(mViewCameraTargetScale*(1.0f - (scroll*mViewCameraScaleSence)));
	}

	void ScrollView::ChangeCameraScaleRelativeToCursor(const Vec2F& newScale)
	{
		Vec2F prevCursor = o2Input.GetCursorPos()*mScreenToLocalTransform;
		mViewCameraTargetScale = newScale;

		Camera targetValuesCamera = mViewCamera;
		targetValuesCamera.scale = mViewCameraTargetScale;
		targetValuesCamera.position = mViewCameraTargetPos;

		Basis screenToLocalTarget = GetCameraScreenToLocalTransform(targetValuesCamera);

		Vec2F newCursor = o2Input.GetCursorPos()*screenToLocalTarget;
		mViewCameraTargetPos -= newCursor - prevCursor;
	}

	void ScrollView::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(true);
	}

	void ScrollView::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{
		if (cursor.delta.Length() > 0.5f)
		{
			Vec2F delta = cursor.delta*mViewCamera.scale*-1.0f;
			mViewCameraVelocity = delta / o2Time.GetDeltaTime();
			mViewCameraTargetPos += delta;
			mNeedRedraw = true;
		}
	}

	void ScrollView::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
	}
}

DECLARE_CLASS(Editor::ScrollView);
