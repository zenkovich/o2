#include "stdafx.h"
#include "Label.h"

#include "Application/Input.h"
#include "Render/Render.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"

namespace o2
{
	Label::Label()
	{}

	Label::Label(const Label& other):
		Widget(other), mHorOverflow(other.mHorOverflow), mVerOverflow(other.mVerOverflow), 
		mExpandBorder(other.mExpandBorder), text(this), font(this), height(this), verAlign(this), horAlign(this),
		horOverflow(this), verOverflow(this), expandBorder(this), symbolsDistanceCoef(this), linesDistanceCoef(this),
		color(this)
	{
		mTextLayer = GetLayerDrawable<Text>("text");
		RetargetStatesAnimations();

		SetLayoutDirty();
	}

	Label& Label::operator=(const Label& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void Label::Draw()
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

	void Label::SetFont(FontRef font)
	{
		if (mTextLayer)
			mTextLayer->SetFont(font);

		SetLayoutDirty();
	}

	FontRef Label::GetFont() const
	{
		if (mTextLayer)
			return mTextLayer->GetFont();

		return FontRef();
	}

	void Label::SetText(const WString& text)
	{
		if (mTextLayer)
			mTextLayer->SetText(text);

		if (mHorOverflow == HorOverflow::Expand || mVerOverflow == VerOverflow::Expand)
			SetLayoutDirty();
	}

	WString Label::GetText() const
	{
		if (mTextLayer)
			return mTextLayer->GetText();

		return WString();
	}

	void Label::SetColor(const Color4& color)
	{
		if (mTextLayer)
			mTextLayer->SetColor(color);
	}

	Color4 Label::GetColor() const
	{
		if (mTextLayer)
			return mTextLayer->GetColor();

		return Color4();
	}

	void Label::SetHorAlign(HorAlign align)
	{
		if (mTextLayer)
			mTextLayer->SetHorAlign(align);

		SetLayoutDirty();
	}

	HorAlign Label::GetHorAlign() const
	{
		if (mTextLayer)
			return mTextLayer->GetHorAlign();

		return HorAlign::Left;
	}

	void Label::SetVerAlign(VerAlign align)
	{
		if (mTextLayer)
			mTextLayer->SetVerAlign(align);

		SetLayoutDirty();
	}

	VerAlign Label::GetVerAlign() const
	{
		if (mTextLayer)
			return mTextLayer->GetVerAlign();

		return VerAlign::Top;
	}

	void Label::SetHorOverflow(HorOverflow overflow)
	{
		mHorOverflow = overflow;

		if (mTextLayer)
		{
			mTextLayer->wordWrap = mHorOverflow == HorOverflow::Wrap;
			mTextLayer->dotsEngings = mHorOverflow == HorOverflow::Dots;
		}

		SetLayoutDirty();
	}

	Label::HorOverflow Label::GetHorOverflow()
	{
		return mHorOverflow;
	}

	void Label::SetVerOverflow(VerOverflow overflow)
	{
		mVerOverflow = overflow;
		SetLayoutDirty();
	}

	Label::VerOverflow Label::GetVerOverflow()
	{
		return mVerOverflow;
	}

	void Label::SetSymbolsDistanceCoef(float coef /*= 1*/)
	{
		if (mTextLayer)
			mTextLayer->SetSymbolsDistanceCoef(coef);

		SetLayoutDirty();
	}

	float Label::GetSymbolsDistanceCoef() const
	{
		if (mTextLayer)
			return mTextLayer->GetSymbolsDistanceCoef();

		return 1.0f;
	}

	void Label::SetLinesDistanceCoef(float coef /*= 1*/)
	{
		if (mTextLayer)
			mTextLayer->SetLinesDistanceCoef(coef);

		SetLayoutDirty();
	}

	float Label::GetLinesDistanceCoef() const
	{
		if (mTextLayer)
			return mTextLayer->GetLinesDistanceCoef();

		return 1.0f;
	}

	void Label::SetExpandBorder(const Vec2F& border)
	{
		mExpandBorder = border;
		SetLayoutDirty();
	}

	Vec2F Label::GetExpandBorder() const
	{
		return mExpandBorder;
	}

	void Label::SetHeight(int height)
	{
		if (mTextLayer)
			mTextLayer->SetHeight(height);
	}

	int Label::GetHeight() const
	{
		if (mTextLayer)
			return mTextLayer->GetHeight();

		return 0;
	}

	void Label::UpdateSelfTransform()
{
		if (mTextLayer)
		{
			if (mHorOverflow == HorOverflow::Expand)
			{
				layout->Update();

				float realSize = mTextLayer->GetRealSize().x + mExpandBorder.x*2.0f;
				float thisSize = layout->width;
				float sizeDelta = realSize - thisSize;
				GetLayoutData().minSize.x = realSize;

				switch (mTextLayer->GetHorAlign())
				{
					case HorAlign::Left:
					GetLayoutData().offsetMax.x += sizeDelta;
					break;

					case HorAlign::Middle:
					case HorAlign::Both:
					GetLayoutData().offsetMax.x += sizeDelta*0.5f;
					GetLayoutData().offsetMin.x -= sizeDelta*0.5f;
					break;

					case HorAlign::Right:
					GetLayoutData().offsetMin.x -= sizeDelta;
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
					GetLayoutData().offsetMin.y -= sizeDelta;
					break;

					case VerAlign::Middle:
					case VerAlign::Both:
					GetLayoutData().offsetMax.y += sizeDelta*0.5f;
					GetLayoutData().offsetMin.y -= sizeDelta*0.5f;
					break;

					case VerAlign::Bottom:
					GetLayoutData().offsetMax.y += sizeDelta;
					break;
				}
			}
		}

		layout->Update();
	}

	void Label::CopyData(const Actor& otherActor)
	{
		const Label& other = dynamic_cast<const Label&>(otherActor);

		Widget::CopyData(other);

		mTextLayer    = GetLayerDrawable<Text>("text");
		mHorOverflow  = other.mHorOverflow;
		mVerOverflow  = other.mVerOverflow;
		mExpandBorder = other.mExpandBorder;

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void Label::OnLayerAdded(WidgetLayer* layer)
	{
		if (layer->name == "text" && layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(Text))
			mTextLayer = (Text*)layer->GetDrawable();
	}
}

DECLARE_CLASS(o2::Label);

ENUM_META_(o2::Label::HorOverflow, HorOverflow)
{
	ENUM_ENTRY(Cut);
	ENUM_ENTRY(Dots);
	ENUM_ENTRY(Expand);
	ENUM_ENTRY(None);
	ENUM_ENTRY(Wrap);
}
END_ENUM_META;

ENUM_META_(o2::Label::VerOverflow, VerOverflow)
{
	ENUM_ENTRY(Cut);
	ENUM_ENTRY(Expand);
	ENUM_ENTRY(None);
}
END_ENUM_META;
