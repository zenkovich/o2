#include "stdafx.h"
#include "CurvePreview.h"

#include "Render/Camera.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Utils/Math/Curve.h"

namespace Editor
{
	CurvePreview::CurvePreview()
	{
		mSprite = mnew Sprite();
		AddLayer("image", mSprite);
	}

	CurvePreview::CurvePreview(const CurvePreview& other):
		mBackColor(other.mBackColor), mCurveColor(other.mCurveColor)
	{
		mSprite = GetLayerDrawable<Sprite>("image");
		RetargetStatesAnimations();
	}

	CurvePreview& CurvePreview::operator=(const CurvePreview& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void CurvePreview::SetCurve(Curve* curve)
	{
		if (mCurve)
			mCurve->onKeysChanged -= THIS_FUNC(OnCurveChanged);

		mCurve = curve;

		if (mCurve)
			mCurve->onKeysChanged += THIS_FUNC(OnCurveChanged);
	}

	void CurvePreview::Draw()
	{
		if (mNeedRedraw)
			Redraw();

		Widget::Draw();
	}

	void CurvePreview::SetBackColor(const Color4& color)
	{
		mBackColor = color;
		mNeedRedraw = true;
	}

	Color4 CurvePreview::GetBackColor() const
	{
		return mBackColor;
	}

	void CurvePreview::SetCurveColor(const Color4& color)
	{
		mCurveColor = color;
		mNeedRedraw = true;
	}

	Color4 CurvePreview::GetCurveColor() const
	{
		return mCurveColor;
	}

	void CurvePreview::CopyData(const Actor& otherActor)
	{
		const CurvePreview& other = dynamic_cast<const CurvePreview&>(otherActor);

		Widget::CopyData(otherActor);

		mBackColor = other.mBackColor;
		mCurveColor = other.mCurveColor;

		mSprite = GetLayerDrawable<Sprite>("image");
		RetargetStatesAnimations();

		mNeedRedraw = true;
	}

	void CurvePreview::UpdateLayersLayouts()
	{
		mNeedRedraw = true;
		Widget::UpdateLayersLayouts();
	}

	void CurvePreview::Redraw()
	{
		if (!mCurve)
			return;

		TextureRef texture = mSprite->GetTexture();
		if (!texture || texture->GetSize() != layout->GetSize())
		{
			texture = TextureRef(layout->GetSize(), PixelFormat::R8G8B8A8, Texture::Usage::RenderTarget);
			mSprite->SetTexture(texture);
			mSprite->SetTextureSrcRect(RectI(Vec2I(), texture->GetSize()));
		}

		o2Render.SetRenderTexture(texture);
		o2Render.Clear(mBackColor);

		RectF rect(mCurve->GetRect());
		Camera camera; camera.SetRect(mCurve->GetRect());
		camera.SetScale(camera.GetScale().InvertedY());

		Camera defaultCamera = o2Render.GetCamera();
		defaultCamera.leftBottom += Vec2F(0.0f, 1.0f);
		defaultCamera.rightTop   -= Vec2F(0.0f, 1.0f);

		Basis transform = camera.GetBasis().Inverted()*defaultCamera.GetBasis();

		for (auto& key : mCurve->GetKeys())
		{
			auto points = key.GetApproximatedPoints();
			auto lastPoint = points[0]*transform;
			for (int i = 1; i < key.GetApproximatedPointsCount(); i++)
			{
				auto point = points[i]*transform;
				o2Render.DrawAALine(lastPoint, point, mCurveColor, 1.5f);
				lastPoint = point;
			}
		}

		o2Render.UnbindRenderTexture();

		mNeedRedraw = false;
	}

	void CurvePreview::OnCurveChanged()
	{
		mNeedRedraw = true;
	}
}

DECLARE_CLASS(Editor::CurvePreview);
