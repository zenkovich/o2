#include "CurvePreview.h"

#include "Render/Sprite.h"
#include "Utils/Math/Curve.h"
#include "Render/Render.h"
#include "Render/Camera.h"

namespace Editor
{
	UICurvePreview::UICurvePreview()
	{
		mSprite = mnew Sprite();
		AddLayer("image", mSprite);
	}

	UICurvePreview::UICurvePreview(const UICurvePreview& other):
		mBackColor(other.mBackColor), mCurveColor(other.mCurveColor)
	{
		mSprite = GetLayerDrawable<Sprite>("image");
		RetargetStatesAnimations();
	}

	UICurvePreview& UICurvePreview::operator=(const UICurvePreview& other)
	{
		mBackColor = other.mBackColor;
		mCurveColor = other.mCurveColor;

		mSprite = GetLayerDrawable<Sprite>("image");
		RetargetStatesAnimations();

		mNeedRedraw = true;

		return *this;
	}

	void UICurvePreview::SetCurve(Curve* curve)
	{
		if (mCurve)
			mCurve->onKeysChanged -= Func(this, &UICurvePreview::OnCurveChanged);

		mCurve = curve;

		if (mCurve)
			mCurve->onKeysChanged += Func(this, &UICurvePreview::OnCurveChanged);
	}

	void UICurvePreview::Draw()
	{
		if (mNeedRedraw)
			Redraw();

		UIWidget::Draw();
	}

	void UICurvePreview::SetBackColor(const Color4& color)
	{
		mBackColor = color;
		mNeedRedraw = true;
	}

	Color4 UICurvePreview::GetBackColor() const
	{
		return mBackColor;
	}

	void UICurvePreview::SetCurveColor(const Color4& color)
	{
		mCurveColor = color;
		mNeedRedraw = true;
	}

	Color4 UICurvePreview::GetCurveColor() const
	{
		return mCurveColor;
	}

	void UICurvePreview::UpdateLayersLayouts()
	{
		mNeedRedraw = true;
		UIWidget::UpdateLayersLayouts();
	}

	void UICurvePreview::Redraw()
	{
		TextureRef texture = mSprite->GetTexture();
		if (!texture || texture->GetSize() != layout.GetSize())
		{
			texture = TextureRef(layout.GetSize(), Texture::Format::Default, Texture::Usage::RenderTarget);
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

		auto& keys = mCurve->GetKeys();
		for (auto& key : keys)
		{
			auto points = key.GetApproximatedPoints();
			for (int i = 1; i < key.GetApproximatedPointsCount(); i++)
				o2Render.DrawLine(points[i - 1], points[i], curveColor);
		}

		o2Render.UnbindRenderTexture();
		o2Render.SetCamera(prevCamera);
	}

	void UICurvePreview::OnCurveChanged()
	{
		mNeedRedraw = true;
	}

}

CLASS_META(Editor::UICurvePreview)
{
	BASE_CLASS(o2::UIWidget);

	PROTECTED_FIELD(mCurve);
	PROTECTED_FIELD(mNeedRedraw);
	PROTECTED_FIELD(mSprite);
	PROTECTED_FIELD(mBackColor);
	PROTECTED_FIELD(mCurveColor);

	PUBLIC_FUNCTION(void, SetCurve, Curve*);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetBackColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetBackColor);
	PUBLIC_FUNCTION(void, SetCurveColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetCurveColor);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, Redraw);
	PROTECTED_FUNCTION(void, OnCurveChanged);
}
END_META;
