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

		OnDrawn();

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

		if (mHorOverflow == HorOverflow::Expand || mVerOverflow == VerOverflow::Expand)
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
				layout.mMinSize.x = realSize;

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
		INITIALIZE_PROPERTY(UILabel, height, SetHeight, GetHeight);
		INITIALIZE_PROPERTY(UILabel, verAlign, SetVerAlign, GetVerAlign);
		INITIALIZE_PROPERTY(UILabel, horAlign, SetHorAlign, GetHorAlign);
		INITIALIZE_PROPERTY(UILabel, verOverflow, SetVerOverflow, GetVerOverflow);
		INITIALIZE_PROPERTY(UILabel, horOverflow, SetHorOverflow, GetHorOverflow);
		INITIALIZE_PROPERTY(UILabel, expandBorder, SetExpandBorder, GetExpandBorder);
		INITIALIZE_PROPERTY(UILabel, symbolsDistanceCoef, SetSymbolsDistanceCoef, GetSymbolsDistanceCoef);
		INITIALIZE_PROPERTY(UILabel, linesDistanceCoef, SetLinesDistanceCoef, GetLinesDistanceCoef);
	}
}

CLASS_META(o2::UILabel)
{
	BASE_CLASS(o2::UIWidget);

	PUBLIC_FIELD(font);
	PUBLIC_FIELD(text);
	PUBLIC_FIELD(height);
	PUBLIC_FIELD(verAlign);
	PUBLIC_FIELD(horAlign);
	PUBLIC_FIELD(horOverflow);
	PUBLIC_FIELD(verOverflow);
	PUBLIC_FIELD(expandBorder);
	PUBLIC_FIELD(symbolsDistanceCoef);
	PUBLIC_FIELD(linesDistanceCoef);
	PROTECTED_FIELD(mTextLayer);
	PROTECTED_FIELD(mHorOverflow).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mVerOverflow).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mExpandBorder).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetFont, FontRef);
	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_FUNCTION(void, SetText, const WString&);
	PUBLIC_FUNCTION(WString, GetText);
	PUBLIC_FUNCTION(void, SetHorAlign, HorAlign);
	PUBLIC_FUNCTION(HorAlign, GetHorAlign);
	PUBLIC_FUNCTION(void, SetVerAlign, VerAlign);
	PUBLIC_FUNCTION(VerAlign, GetVerAlign);
	PUBLIC_FUNCTION(void, SetHorOverflow, HorOverflow);
	PUBLIC_FUNCTION(HorOverflow, GetHorOverflow);
	PUBLIC_FUNCTION(void, SetVerOverflow, VerOverflow);
	PUBLIC_FUNCTION(VerOverflow, GetVerOverflow);
	PUBLIC_FUNCTION(void, SetSymbolsDistanceCoef, float);
	PUBLIC_FUNCTION(float, GetSymbolsDistanceCoef);
	PUBLIC_FUNCTION(void, SetLinesDistanceCoef, float);
	PUBLIC_FUNCTION(float, GetLinesDistanceCoef);
	PUBLIC_FUNCTION(void, SetExpandBorder, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetExpandBorder);
	PUBLIC_FUNCTION(void, SetHeight, int);
	PUBLIC_FUNCTION(int, GetHeight);
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PROTECTED_FUNCTION(void, OnLayerAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

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
