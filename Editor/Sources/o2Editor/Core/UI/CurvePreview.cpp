#include "o2Editor/stdafx.h"
#include "CurvePreview.h"

#include "o2/Render/Camera.h"
#include "o2/Render/Render.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Utils/Math/Curve.h"

namespace Editor
{
	CurvePreview::CurvePreview()
	{
		mSprite = mmake<Sprite>();
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

		mBackColor = other.mBackColor;
		mCurveColor = other.mCurveColor;

		mSprite = GetLayerDrawable<Sprite>("image");
		RetargetStatesAnimations();

		mNeedRedraw = true;

		return *this;
	}

	void CurvePreview::SetCurve(const Ref<Curve>& curve)
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

	String CurvePreview::GetCreateMenuCategory()
	{
		return "UI/Editor";
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

		Ref<Texture> texture = mSprite->GetTexture();
		if (!texture || texture->GetSize() != (Vec2I)layout->GetSize())
		{
			texture = Ref<Texture>(layout->GetSize(), TextureFormat::R8G8B8A8, Texture::Usage::RenderTarget);
			mSprite->SetTexture(texture);
			mSprite->SetTextureSrcRect(RectI(Vec2I(), texture->GetSize()));
		}

		o2Render.BindRenderTexture(texture);
		o2Render.Clear(mBackColor);

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
// --- META ---

DECLARE_CLASS(Editor::CurvePreview, Editor__CurvePreview);
// --- END META ---
