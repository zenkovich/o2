#include "stdafx.h"
#include "Label.h"

#include "Application/Input.h"
#include "Render/Render.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"

namespace o2
{
	UILabel::UILabel()
	{}

	UILabel::UILabel(const UILabel& other):
		UIWidget(other), mHorOverflow(other.mHorOverflow), mVerOverflow(other.mVerOverflow), 
		mExpandBorder(other.mExpandBorder), text(this), font(this), height(this), verAlign(this), horAlign(this),
		horOverflow(this), verOverflow(this), expandBorder(this), symbolsDistanceCoef(this), linesDistanceCoef(this),
		color(this)
	{
		mTextLayer = GetLayerDrawable<Text>("text");
		RetargetStatesAnimations();

		SetLayoutDirty();
	}

	UILabel& UILabel::operator=(const UILabel& other)
	{
		UIWidget::operator=(other);
		return *this;
	}

	void UILabel::Draw()
	{
		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		bool enabledClipping = false;
		if (mHorOverflow == HorOverflow::Cut || mVerOverflow == VerOverflow::Cut)
		{
			enabledClipping = true;

			Vec2I halfRes =  (Vec2F)o2Render.GetResolution()*0.5f;
			RectI cutRect(-halfRes.x, halfRes.y, halfRes.x, -halfRes.y);

			if (mHorOverflow == HorOverflow::Cut)
			{
				cutRect.left = (int)layout->worldLeft;
				cutRect.right = (int)layout->worldRight;
			}

			if (mVerOverflow == VerOverflow::Cut)
			{
				cutRect.top = (int)layout->worldTop;
				cutRect.bottom = (int)layout->worldBottom;
			}

			o2Render.EnableScissorTest(cutRect);
		}

		for (auto layer : mDrawingLayers)
			layer->Draw();

		if (enabledClipping)
			o2Render.DisableScissorTest();

		OnDrawn();

		for (auto child : mDrawingChildren)
			child->Draw();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		DrawDebugFrame();
	}

	void UILabel::SetFont(FontRef font)
	{
		if (mTextLayer)
			mTextLayer->SetFont(font);

		SetLayoutDirty();
	}

	FontRef UILabel::GetFont() const
	{
		if (mTextLayer)
			return mTextLayer->GetFont();

		return FontRef();
	}

	void UILabel::SetText(const WString& text)
	{
		if (mTextLayer)
			mTextLayer->SetText(text);

		if (mHorOverflow == HorOverflow::Expand || mVerOverflow == VerOverflow::Expand)
			SetLayoutDirty();
	}

	WString UILabel::GetText() const
	{
		if (mTextLayer)
			return mTextLayer->GetText();

		return WString();
	}

	void UILabel::SetColor(const Color4& color)
	{
		if (mTextLayer)
			mTextLayer->SetColor(color);
	}

	Color4 UILabel::GetColor() const
	{
		if (mTextLayer)
			return mTextLayer->GetColor();

		return Color4();
	}

	void UILabel::SetHorAlign(HorAlign align)
	{
		if (mTextLayer)
			mTextLayer->SetHorAlign(align);

		SetLayoutDirty();
	}

	HorAlign UILabel::GetHorAlign() const
	{
		if (mTextLayer)
			return mTextLayer->GetHorAlign();

		return HorAlign::Left;
	}

	void UILabel::SetVerAlign(VerAlign align)
	{
		if (mTextLayer)
			mTextLayer->SetVerAlign(align);

		SetLayoutDirty();
	}

	VerAlign UILabel::GetVerAlign() const
	{
		if (mTextLayer)
			return mTextLayer->GetVerAlign();

		return VerAlign::Top;
	}

	void UILabel::SetHorOverflow(HorOverflow overflow)
	{
		mHorOverflow = overflow;

		if (mTextLayer)
		{
			mTextLayer->wordWrap = mHorOverflow == HorOverflow::Wrap;
			mTextLayer->dotsEngings = mHorOverflow == HorOverflow::Dots;
		}

		SetLayoutDirty();
	}

	UILabel::HorOverflow UILabel::GetHorOverflow()
	{
		return mHorOverflow;
	}

	void UILabel::SetVerOverflow(VerOverflow overflow)
	{
		mVerOverflow = overflow;
		SetLayoutDirty();
	}

	UILabel::VerOverflow UILabel::GetVerOverflow()
	{
		return mVerOverflow;
	}

	void UILabel::SetSymbolsDistanceCoef(float coef /*= 1*/)
	{
		if (mTextLayer)
			mTextLayer->SetSymbolsDistanceCoef(coef);

		SetLayoutDirty();
	}

	float UILabel::GetSymbolsDistanceCoef() const
	{
		if (mTextLayer)
			return mTextLayer->GetSymbolsDistanceCoef();

		return 1.0f;
	}

	void UILabel::SetLinesDistanceCoef(float coef /*= 1*/)
	{
		if (mTextLayer)
			mTextLayer->SetLinesDistanceCoef(coef);

		SetLayoutDirty();
	}

	float UILabel::GetLinesDistanceCoef() const
	{
		if (mTextLayer)
			return mTextLayer->GetLinesDistanceCoef();

		return 1.0f;
	}

	void UILabel::SetExpandBorder(const Vec2F& border)
	{
		mExpandBorder = border;
		SetLayoutDirty();
	}

	Vec2F UILabel::GetExpandBorder() const
	{
		return mExpandBorder;
	}

	void UILabel::SetHeight(int height)
	{
		if (mTextLayer)
			mTextLayer->SetHeight(height);
	}

	int UILabel::GetHeight() const
	{
		if (mTextLayer)
			return mTextLayer->GetHeight();

		return 0;
	}

	void UILabel::UpdateSelfTransform()
{
		if (mTextLayer)
		{
			if (mHorOverflow == HorOverflow::Expand)
			{
				layout->Update();

				float realSize = mTextLayer->GetRealSize().x + mExpandBorder.x*2.0f;
				float thisSize = layout->width;
				float sizeDelta = realSize - thisSize;
				layout->mData->minSize.x = realSize;

				switch (mTextLayer->GetHorAlign())
				{
					case HorAlign::Left:
					layout->mData->offsetMax.x += sizeDelta;
					break;

					case HorAlign::Middle:
					case HorAlign::Both:
					layout->mData->offsetMax.x += sizeDelta*0.5f;
					layout->mData->offsetMin.x -= sizeDelta*0.5f;
					break;

					case HorAlign::Right:
					layout->mData->offsetMin.x -= sizeDelta;
					break;
				}
			}

			if (mVerOverflow == VerOverflow::Expand)
			{
				layout->Update();

				float realSize = mTextLayer->GetRealSize().y + mExpandBorder.y*2.0f;
				float thisSize = layout->height;
				float sizeDelta = realSize - thisSize;

				switch (mTextLayer->GetVerAlign())
				{
					case VerAlign::Top:
					layout->mData->offsetMin.y -= sizeDelta;
					break;

					case VerAlign::Middle:
					case VerAlign::Both:
					layout->mData->offsetMax.y += sizeDelta*0.5f;
					layout->mData->offsetMin.y -= sizeDelta*0.5f;
					break;

					case VerAlign::Bottom:
					layout->mData->offsetMax.y += sizeDelta;
					break;
				}
			}
		}

		layout->Update();
	}

	void UILabel::CopyData(const Actor& otherActor)
	{
		const UILabel& other = dynamic_cast<const UILabel&>(otherActor);

		UIWidget::CopyData(other);

		mTextLayer    = GetLayerDrawable<Text>("text");
		mHorOverflow  = other.mHorOverflow;
		mVerOverflow  = other.mVerOverflow;
		mExpandBorder = other.mExpandBorder;

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void UILabel::OnLayerAdded(UIWidgetLayer* layer)
	{
		if (layer->name == "text" && layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(Text))
			mTextLayer = (Text*)layer->GetDrawable();
	}
}

DECLARE_CLASS(o2::UILabel);

ENUM_META_(o2::UILabel::HorOverflow, HorOverflow)
{
	ENUM_ENTRY(Cut);
	ENUM_ENTRY(Dots);
	ENUM_ENTRY(Expand);
	ENUM_ENTRY(None);
	ENUM_ENTRY(Wrap);
}
END_ENUM_META;

ENUM_META_(o2::UILabel::VerOverflow, VerOverflow)
{
	ENUM_ENTRY(Cut);
	ENUM_ENTRY(Expand);
	ENUM_ENTRY(None);
}
END_ENUM_META;
