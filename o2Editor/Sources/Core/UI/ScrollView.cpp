#include "ScrollView.h"

#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Application/Application.h"

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
		delete mRenderTargetSprite;
	}

	UIScrollView& UIScrollView::operator=(const UIScrollView& other)
	{
		UIWidget::operator=(other);

		mReady = false;

		delete mRenderTargetSprite;

		mBackColor          = other.mBackColor;
		mGridColor          = other.mGridColor;
		mRenderTarget       = TextureRef(Vec2I(256, 256), Texture::Format::Default, Texture::Usage::RenderTarget);
		mRenderTargetSprite = mnew Sprite(mRenderTarget, RectI(0, 0, 256, 256));

		RetargetStatesAnimations();

		mReady = true;

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

	void UIScrollView::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		UIWidget::UpdateLayout(forcible, withChildren);

		if (!mReady)
			return;

		Vec2I size = layout.GetAbsoluteRect().Size();
		mRenderTarget = TextureRef(size, Texture::Format::Default, Texture::Usage::RenderTarget);
		*mRenderTargetSprite = Sprite(mRenderTarget, RectI(Vec2I(), size));
		mRenderTargetSprite->SetRect(layout.GetAbsoluteRect());
		mNeedRedraw = true;

		mViewCamera.size = size;
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

	void UIScrollView::UpdateCamera(float dt)
	{
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

		if (!Math::Equals<float>(mViewCamera.scale->x, mViewCameraTargetScale.x) ||
			!Math::Equals<float>(mViewCamera.scale->y, mViewCameraTargetScale.y))
		{
			mViewCamera.scale = Math::Lerpc<Vec2F>(mViewCamera.scale, mViewCameraTargetScale,
												   dt*mViewCameraScaleElasticyCoef);

			mNeedRedraw = true;
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

			OnCameraPositionChanged();

			mNeedRedraw = true;
		}
	}

	void UIScrollView::UpdateLocalScreenTransforms()
	{
		RectF rectangle = layout.GetAbsoluteRect();
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
	}

	void UIScrollView::OnCameraPositionChanged()
	{}

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

CLASS_META(Editor::UIScrollView)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::CursorAreaEventsListener);

	PROTECTED_FIELD(mReady);
	PROTECTED_FIELD(mRenderTargetSprite);
	PROTECTED_FIELD(mRenderTarget);
	PROTECTED_FIELD(mNeedRedraw);
	PROTECTED_FIELD(mBackColor).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mGridColor).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mViewArea);
	PROTECTED_FIELD(mViewCamera);
	PROTECTED_FIELD(mViewCameraTargetScale);
	PROTECTED_FIELD(mViewCameraScaleSence);
	PROTECTED_FIELD(mViewCameraScaleElasticyCoef);
	PROTECTED_FIELD(mViewCameraTargetPos);
	PROTECTED_FIELD(mViewCameraVelocity);
	PROTECTED_FIELD(mViewCameraPosElasticyCoef);
	PROTECTED_FIELD(mViewCameraVelocityDampingCoef);
	PROTECTED_FIELD(mViewCameraMinScale);
	PROTECTED_FIELD(mViewCameraMaxScale);
	PROTECTED_FIELD(mLocalToScreenTransform);
	PROTECTED_FIELD(mScreenToLocalTransform);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(Vec2F, ScreenToLocalPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, LocalToScreenPoint, const Vec2F&);
	PUBLIC_FUNCTION(const Camera&, GetCamera);
	PUBLIC_FUNCTION(void, SetBackColor, const Color4&);
	PUBLIC_FUNCTION(void, SetGridColor, const Color4&);
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PROTECTED_FUNCTION(void, UpdateCamera, float);
	PROTECTED_FUNCTION(void, UpdateLocalScreenTransforms);
	PROTECTED_FUNCTION(void, RedrawRenderTarget);
	PROTECTED_FUNCTION(void, RedrawContent);
	PROTECTED_FUNCTION(void, DrawGrid);
	PROTECTED_FUNCTION(void, OnCameraPositionChanged);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
}
END_META;
