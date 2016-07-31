#include "Label.h"

#include "Render/Render.h"
#include "Application/Input.h"

namespace o2
{
	UILabel::UILabel()
	{
		InitializeProperties();
	}

	UILabel::UILabel(const UILabel& other):
		UIWidget(other), mHorOverflow(other.mHorOverflow), mVerOverflow(other.mVerOverflow),
		mExpandBorder(other.mExpandBorder)
	{
		mTextLayer = GetLayerDrawable<Text>("text");
		RetargetStatesAnimations();
		InitializeProperties();

		UpdateLayout();
	}

	UILabel& UILabel::operator=(const UILabel& other)
	{
		UIWidget::operator=(other);

		mTextLayer    = GetLayerDrawable<Text>("text");
		mHorOverflow  = other.mHorOverflow;
		mVerOverflow  = other.mVerOverflow;
		mExpandBorder = other.mExpandBorder;

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UILabel::Draw()
	{
		if (mFullyDisabled || mIsClipped)
			return;

		bool enabledClipping = false;
		if (mHorOverflow == HorOverflow::Cut || mVerOverflow == VerOverflow::Cut)
		{
			enabledClipping = true;

			Vec2I halfRes =  (Vec2F)o2Render.GetResolution()*0.5f;
			RectI cutRect(-halfRes.x, halfRes.y, halfRes.x, -halfRes.y);

			if (mHorOverflow == HorOverflow::Cut)
			{
				cutRect.left = (int)layout.absLeft;
				cutRect.right = (int)layout.absRight;
			}

			if (mVerOverflow == VerOverflow::Cut)
			{
				cutRect.top = (int)layout.absTop;
				cutRect.bottom = (int)layout.absBottom;
			}

			o2Render.EnableScissorTest(cutRect);
		}

		for (auto layer : mDrawingLayers)
			layer->Draw();

		if (enabledClipping)
			o2Render.DisableScissorTest();

		for (auto child : mChilds)
			child->Draw();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		DrawDebugFrame();
	}

	void UILabel::SetFont(FontRef font)
	{
		if (mTextLayer)
			mTextLayer->SetFont(font);

		UpdateLayout();
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

		UpdateLayout();
	}

	WString UILabel::GetText() const
	{
		if (mTextLayer)
			return mTextLayer->GetText();

		return WString();
	}

	void UILabel::SetHorAlign(HorAlign align)
	{
		if (mTextLayer)
			mTextLayer->SetHorAlign(align);

		UpdateLayout();
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

		UpdateLayout();
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

		UpdateLayout();
	}

	UILabel::HorOverflow UILabel::GetHorOverflow()
	{
		return mHorOverflow;
	}

	void UILabel::SetVerOverflow(VerOverflow overflow)
	{
		mVerOverflow = overflow;
		UpdateLayout();
	}

	UILabel::VerOverflow UILabel::GetVerOverflow()
	{
		return mVerOverflow;
	}

	void UILabel::SetSymbolsDistanceCoef(float coef /*= 1*/)
	{
		if (mTextLayer)
			mTextLayer->SetSymbolsDistanceCoef(coef);

		UpdateLayout();
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

		UpdateLayout();
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
		UpdateLayout();
	}

	Vec2F UILabel::GetExpandBorder() const
	{
		return mExpandBorder;
	}

	void UILabel::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		if (mTextLayer)
		{
			if (mHorOverflow == HorOverflow::Expand)
			{
				RecalculateAbsRect();

				float realSize = mTextLayer->GetRealSize().x + mExpandBorder.x*2.0f;
				float thisSize = layout.width;
				float sizeDelta = realSize - thisSize;

				switch (mTextLayer->GetHorAlign())
				{
				case HorAlign::Left:
				layout.mOffsetMax.x += sizeDelta;
				break;

				case HorAlign::Middle:
				case HorAlign::Both:
				layout.mOffsetMax.x += sizeDelta*0.5f;
				layout.mOffsetMin.x -= sizeDelta*0.5f;
				break;

				case HorAlign::Right:
				layout.mOffsetMin.x -= sizeDelta;
				break;
				}
			}

			if (mVerOverflow == VerOverflow::Expand)
			{
				RecalculateAbsRect();

				float realSize = mTextLayer->GetRealSize().y + mExpandBorder.y*2.0f;
				float thisSize = layout.height;
				float sizeDelta = realSize - thisSize;

				switch (mTextLayer->GetVerAlign())
				{
				case VerAlign::Top:
				layout.mOffsetMin.y -= sizeDelta;
				break;

				case VerAlign::Middle:
				case VerAlign::Both:
				layout.mOffsetMax.y += sizeDelta*0.5f;
				layout.mOffsetMin.y -= sizeDelta*0.5f;
				break;

				case VerAlign::Bottom:
				layout.mOffsetMax.y += sizeDelta;
				break;
				}
			}
		}

		RecalculateAbsRect();
		UpdateLayersLayouts();

		if (withChildren)
			UpdateChildrenLayouts();
	}

	void UILabel::OnLayerAdded(UIWidgetLayer* layer)
	{
		if (layer->name == "text" && layer->drawable && layer->drawable->GetType() == TypeOf(Text))
			mTextLayer = (Text*)layer->drawable;
	}

	void UILabel::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UILabel, font, SetFont, GetFont);
		INITIALIZE_PROPERTY(UILabel, text, SetText, GetText);
		INITIALIZE_PROPERTY(UILabel, verAlign, SetVerAlign, GetVerAlign);
		INITIALIZE_PROPERTY(UILabel, horAlign, SetHorAlign, GetHorAlign);
		INITIALIZE_PROPERTY(UILabel, verOverflow, SetVerOverflow, GetVerOverflow);
		INITIALIZE_PROPERTY(UILabel, horOverflow, SetHorOverflow, GetHorOverflow);
		INITIALIZE_PROPERTY(UILabel, expandBorder, SetExpandBorder, GetExpandBorder);
		INITIALIZE_PROPERTY(UILabel, symbolsDistanceCoef, SetSymbolsDistanceCoef, GetSymbolsDistanceCoef);
		INITIALIZE_PROPERTY(UILabel, linesDistanceCoef, SetLinesDistanceCoef, GetLinesDistanceCoef);
	}
}