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

		const Color4 backColor(120, 120, 120, 255);
		const Color4 curveColor(0, 255, 0, 255);

		Camera prevCamera = o2Render.GetCamera();
		Camera currCamera; currCamera.SetRect(mCurve->GetRect());
		currCamera.SetScale(currCamera.GetScale().InvertedY());

		o2Render.SetRenderTexture(texture);
		o2Render.SetCamera(currCamera);
		o2Render.Clear(backColor);

		static Vector<Vertex2> buffer;
		buffer.Clear();

		auto curveColorHex = curveColor.ARGB();

		auto& keys = mCurve->GetKeys();
		for (auto& key : keys)
		{
			buffer.Add(Vertex2(key.position, key.value, curveColorHex, 0, 0));

			auto points = key.GetApproximatedPoints();
			for (int i = 0; i < key.GetApproximatedPointsCount(); i++)
				buffer.Add(Vertex2(points[i], curveColorHex, 0, 0));

			o2Render.DrawAAPolyLine(buffer.Data(), buffer.Count(), 2);
		}

		o2Render.UnbindRenderTexture();
		o2Render.SetCamera(prevCamera);
	}

	void CurvePreview::OnCurveChanged()
	{
		mNeedRedraw = true;
	}

}

DECLARE_CLASS(Editor::CurvePreview);
