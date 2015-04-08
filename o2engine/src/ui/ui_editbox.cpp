#include "ui_editbox.h"

#include "render_system/text.h"
#include "render_system/render_system.h"
#include "render_system/sprite.h"
#include "render_system/mesh.h"
#include "ui_scroll_bar.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UIEditBox);

UIEditBox::UIEditBox(Font* font, const Layout& layout, UIScrollBar* horBarSample /*= NULL*/,
					 UIScrollBar* verBarSample /*= NULL*/, const String& id /*= ""*/):
					 UIDrawablesListWidget(layout, id), mHoverState(NULL), mFocusedState(NULL), mText(NULL), mCursorVisibleTimer(0.0f),
					 mMultiLine(false), mHorScrollbar(NULL), mVerScrollbar(NULL), mCursorInsideClipping(false)
{
	mText = mnew Text(font);
	mText->SetHorAlign(Font::HA_LEFT);
	mText->SetVerAlign(Font::VA_CENTER);

	mCursorSprite = mnew Sprite();
	mCursorSprite->SetSize(Vec2F(1.0f, font->GetLineHeight()*mText->GetLinesDistCoef()));
	mCursorSprite->SetRelativePivot(Vec2F(0.0f, 1.0f));

	mSelectionColor = Color4::Green();
	mSelectionMesh = mnew Mesh(TextureRef(), 4096, 2048);

	mCursorVisibleDelay = 0.7f;

	mSelectionStart = mSelectionEnd = 0;
	SetHorScrollbar(horBarSample);
	SetVerScrollbar(verBarSample);

	InitializeProperties();
}

UIEditBox::UIEditBox(const UIEditBox& editbox):
UIDrawablesListWidget(editbox), mMultiLine(false), mHorScrollbar(NULL), mVerScrollbar(NULL), mCursorInsideClipping(false)
{
	mHoverState = GetState("hover");
	mFocusedState = GetState("focus");
	mText = mnew Text(*editbox.mText);
	mClippingLayout = editbox.mClippingLayout;
	mTextLayout = editbox.mTextLayout;
	mMultiLine = editbox.mMultiLine;

	mCursorSprite = mnew Sprite();
	mCursorSprite->SetSize(Vec2F(1.0f, mText->GetFont()->GetLineHeight()*mText->GetLinesDistCoef()));
	mCursorSprite->SetRelativePivot(Vec2F(0.0f, 1.0f));
	mCursorSprite->SetColor(editbox.mCursorSprite->GetColor());

	mSelectionColor = Color4::Green();
	mSelectionMesh = mnew Mesh(TextureRef(), 4096, 2048);
	mSelectionColor = editbox.mSelectionColor;
	mCursorVisibleDelay = editbox.mCursorVisibleDelay;
	mCursorVisibleTimer = 0.0f;

	mSelectionStart = mSelectionEnd = 0;

	if (editbox.mHorScrollbar)
		SetHorScrollbar(GetWidgetByType<UIScrollBar>(editbox.mHorScrollbar->GetId()));
	else
		SetHorScrollbar(NULL);

	if (editbox.mVerScrollbar)
		SetVerScrollbar(GetWidgetByType<UIScrollBar>(editbox.mVerScrollbar->GetId()));
	else
		SetVerScrollbar(NULL);

	InitializeProperties();
}

UIEditBox::~UIEditBox()
{
	SafeRelease(mText);
	SafeRelease(mCursorSprite);
	SafeRelease(mSelectionMesh);
}

UIWidget* UIEditBox::Clone() const
{
	return mnew UIEditBox(*this);
}

bool UIEditBox::IsFocusable() const
{
	return true;
}

void UIEditBox::SetCText(const String& text)
{
	mText->SetCText(text);
}

String UIEditBox::GetCText() const
{
	return mText->GetCText();
}

void UIEditBox::SetText(const WString& text)
{
	mText->GetText(text);
}

WString UIEditBox::GetText() const
{
	return mText->GetText();
}

void UIEditBox::SetWordWrap(bool wordWrap)
{
	mText->SetWordWrap(wordWrap);
	mText->ForceUpdateMesh();
	UpdateSelectionEndPosition(mSelectionEnd, true);
}

bool UIEditBox::IsWordWrap() const
{
	return mText->GetWordWrap();
}

void UIEditBox::SetHorScrollbar(UIScrollBar* scrollbar)
{
	if (mHorScrollbar)
		RemoveChild(mHorScrollbar);

	if (!scrollbar)
		return;

	AddChild(scrollbar);
	mHorScrollbar = scrollbar;
	mHorScrollbar->onValueChangedEvent.Add(callback<UIEditBox>(this, &UIEditBox::ScrollChanged));
}

void UIEditBox::SetVerScrollbar(UIScrollBar* scrollbar)
{
	if (mVerScrollbar)
		RemoveChild(mVerScrollbar);

	if (!scrollbar)
		return;

	AddChild(scrollbar);
	mVerScrollbar = scrollbar;
	mVerScrollbar->onValueChangedEvent.Add(callback<UIEditBox>(this, &UIEditBox::ScrollChanged));
}

void UIEditBox::AddedState(UIState* state)
{
	if (state->GetName() == "hover")
		mHoverState = state;
	else if (state->GetName() == "focus")
		mFocusedState = state;
}

void UIEditBox::LocalUpdate(float dt)
{
	if (mHoverState)
		mHoverState->SetState(mCursorInside);

	UpdateCursorVisible(dt);

	//update scrolling
	float scrollingCoef = 20.0f;
	Vec2F lastSmoothScroll = mSmoothScroll;
	mSmoothScroll = Lerp(mSmoothScroll, mScroll, Clamp(dt*scrollingCoef, 0.0f, 1.0f));

	if (!Equals(lastSmoothScroll, mSmoothScroll))
	{
		UpdateLayout();
	}
}

bool UIEditBox::LocalProcessInputMessage(const InputMessage& msg)
{
	mCursorInsideClipping = mClippingLayout.GetRect().isInside(msg.GetCursorPos());

	float scrollCoef = 0.1f;
	float mouseWheelDelta = msg.GetMouseWheelDelta()*scrollCoef;
	if (!Equals(mouseWheelDelta, 0.0f) && mCursorInside)
	{
		if (mVerScrollbar && mVerScrollbar->IsVisible())
			mVerScrollbar->SetValueClamped(mVerScrollbar->GetValue() - mouseWheelDelta);
		else if (mHorScrollbar)
			mHorScrollbar->SetValueClamped(mHorScrollbar->GetValue() - mouseWheelDelta);
	}

	if (msg.IsCursorPressed())
	{
		if (mCursorInside)
			MakeFocused();
		else
			ReleaseFocus();
	}

	ProcessNavigation(msg);
	ProcessErasing(msg);
	ProcessInputCharacters(msg);

	return false;
}

void UIEditBox::ProcessNavigation(const InputMessage &msg)
{
	if (msg.IsCursorPressed() && mCursorInsideClipping)
	{
		if (mCursorInside)
		{
			int charIdx = GetCharacterIdxAtPoint(msg.GetCursorPos());
			UpdateSelectionEndPosition(charIdx, false);
			mSelectionStart = mSelectionEnd;
			UpdateSelection();
		}
	}

	if (msg.IsCursorDown())
	{
		int charIdx = GetCharacterIdxAtPoint(msg.GetCursorPos());
		UpdateSelectionEndPosition(charIdx, true);
	}

	bool selecting = msg.IsKeyDown(VK_SHIFT);
	if (msg.IsKeyPressed(VK_LEFT) || msg.IsKeyRepeating(VK_LEFT))
	{
		if (msg.IsKeyDown(VK_CONTROL))
			JumpSelection(false, selecting);
		else
			UpdateSelectionEndPosition(mSelectionEnd - 1, selecting);
	}

	if (msg.IsKeyPressed(VK_RIGHT) || msg.IsKeyRepeating(VK_RIGHT))
	{
		if (msg.IsKeyDown(VK_CONTROL))
			JumpSelection(true, selecting);
		else
			UpdateSelectionEndPosition(mSelectionEnd + 1, selecting);
	}

	if (msg.IsKeyPressed(VK_UP) || msg.IsKeyRepeating(VK_UP))
		UpdateSelectionEndPosition(GetCharacterIdxAtPoint(GetCharacterPosition(mSelectionEnd) -
		Vec2F(0.0f, mText->GetFont()->GetLineHeight()*1.5f)), selecting);

	if (msg.IsKeyPressed(VK_DOWN) || msg.IsKeyRepeating(VK_DOWN))
		UpdateSelectionEndPosition(GetCharacterIdxAtPoint(GetCharacterPosition(mSelectionEnd) +
		Vec2F(0.0f, mText->GetFont()->GetLineHeight()*0.5f)), selecting);

	if (msg.IsKeyDown(VK_END))
	{
		WString text = mText->GetText();
		int endLineSymbol = mSelectionEnd;
		for (int i = mSelectionEnd; i < (int)text.length(); i++)
		{
			endLineSymbol = i + 1;
			if (text[i] == '\n')
			{
				endLineSymbol--;
				break;
			}
		}

		UpdateSelectionEndPosition(endLineSymbol, selecting);
	}

	if (msg.IsKeyDown(VK_HOME))
	{
		WString text = mText->GetText();
		int endLineSymbol = mSelectionEnd;
		for (int i = mSelectionEnd -1; i >= 0; i--)
		{
			endLineSymbol = i;
			if (text[i] == '\n')
			{
				endLineSymbol++;
				break;
			}
		}

		UpdateSelectionEndPosition(endLineSymbol, selecting);
	}
}

void UIEditBox::JumpSelection(bool forward, bool selecting)
{
	static char jumpSymbols[] = " \n()-=_+\\|/**&^%$#@!~,.?";
	int jumpSymbolsCount = strlen(jumpSymbols);

	WString text = mText->GetText();
	int jumpIdx = mSelectionEnd + 1;

	if (forward)
	{
		bool stop = false;
		for (int i = mSelectionEnd; i < (int)text.length() && !stop; i++)
		{
			jumpIdx = i;
			for (int j = 0; j < jumpSymbolsCount; j++)
			{
				if (text[i] == jumpSymbols[j])
				{
					stop = true;
					break;
				}
			}
		}

		if (!stop) jumpIdx++;
	}
	else
	{
		bool stop = false;
		for (int i = Min(mSelectionEnd - 1, (int)text.length() - 1); i >= 0 && !stop; i--)
		{
			jumpIdx = i;
			for (int j = 0; j < jumpSymbolsCount; j++)
			{
				if (text[i] == jumpSymbols[j])
				{
					jumpIdx++;
					stop = true;
					break;
				}
			}
		}

		if (!stop) jumpIdx--;
	}

	if (jumpIdx == mSelectionEnd)
	{
		if (forward)
			jumpIdx++;
		else
			jumpIdx--;
	}

	UpdateSelectionEndPosition(jumpIdx, selecting);
}

void UIEditBox::ProcessErasing(const InputMessage &msg)
{
	if (msg.IsKeyPressed(VK_BACK) || msg.IsKeyRepeating(VK_BACK))
	{
		if (mSelectionEnd > 0)
		{
			WString text = mText->GetText();

			if (mSelectionEnd == mSelectionStart)
				text.erase(mSelectionEnd - 1, 1);
			else
				text.erase(Min(mSelectionStart, mSelectionEnd), Abs(mSelectionEnd - mSelectionStart));

			mText->GetText(text);
			mText->ForceUpdateMesh();

			if (mSelectionEnd == mSelectionStart)
				UpdateSelectionEndPosition(mSelectionEnd -1, false);
			else
				UpdateSelectionEndPosition(Min(mSelectionEnd, mSelectionStart), false);

			LayoutUpdated();
			CheckScrollingToCursor();
		}
	}

	if (msg.IsKeyPressed(VK_DELETE) || msg.IsKeyRepeating(VK_DELETE))
	{
		if (mSelectionEnd < (int)mText->GetText().length() + 1)
		{
			WString text = mText->GetText();

			if (mSelectionEnd == mSelectionStart)
				text.erase(mSelectionEnd, 1);
			else
				text.erase(Min(mSelectionStart, mSelectionEnd), Abs(mSelectionEnd - mSelectionStart));

			mText->GetText(text);
			mText->ForceUpdateMesh();
			UpdateSelectionEndPosition(Min(mSelectionEnd, mSelectionStart), false);

			LayoutUpdated();
			CheckScrollingToCursor();
		}
	}
}

void UIEditBox::ProcessInputCharacters(const InputMessage &msg)
{
	InputMessage::KeysArr pressedKeys = msg.GetPressedKeys();
	InputMessage::KeysArr downKeys = msg.GetDownKeys();

	FOREACH(InputMessage::KeysArr, downKeys, key)
	{
		if (msg.IsKeyRepeating(key->mKey))
			pressedKeys.push_back(*key);
	}

	FOREACH(InputMessage::KeysArr, pressedKeys, key)
	{
		char16_t ch = (char16_t)GetUnicodeFromVirtualCode(key->mKey);
		HightLog("char %i", (int)ch);
		if (ch != 0 && ch != 8)
		{
			if (ch == 13)
				ch = 10;

			if (ch == 10 && !mMultiLine)
				continue;

			WString text = mText->GetText();

			if (mSelectionStart != mSelectionEnd)
			{
				text.erase(Min(mSelectionStart, mSelectionEnd), Abs(mSelectionEnd - mSelectionStart));
				mSelectionEnd = Min(mSelectionStart, mSelectionEnd);
				mSelectionStart = mSelectionEnd;
			}

			text.insert(text.begin() + mSelectionEnd, ch);
			mText->GetText(text);
			mText->ForceUpdateMesh();
			UpdateSelectionEndPosition(mSelectionEnd + 1, false);
			mSelectionStart = mSelectionEnd;
			HightLog("char %i", (int)ch);

			LayoutUpdated();
			CheckScrollingToCursor();
		}
	}
}

void UIEditBox::LocalDraw()
{
	UIDrawablesListWidget::LocalDraw();

	AppRender()->SetupScissorRect(mClippingLayout.GetRect());
	AppRender()->EnableScissorTest();

	mText->Draw();
	mSelectionMesh->Draw();
	mCursorSprite->Draw();
	//drawDebug();

	AppRender()->DisableScissorTest();
}

void UIEditBox::LayoutUpdated()
{
	UIDrawablesListWidget::LayoutUpdated();

	mClippingLayout.Update(mGlobalPosition, mSize);
	mTextLayout.Update(mGlobalPosition - mSmoothScroll, mSize);
	mText->GetRect(mTextLayout.GetRect());
	mText->ForceUpdateMesh();

	Font::TextSymbolsSet* symbSet = mText->GetSymbolsSet();
	RectF contentSize(mTextLayout.mPosition, symbSet->mPosition + symbSet->mRealSize + Vec2F(5.0f, 5.0f));

	if (mHorScrollbar)
	{
		mHorScrollbar->SetValueRange(0.0f, Max(contentSize.getSizeX() - mClippingLayout.mSize.x, 0.1f));
		mHorScrollbar->SetBarSize(mClippingLayout.mSize.x/contentSize.getSizeX()*mHorScrollbar->GetMaxValue());
		mHorScrollbar->SetVisible(mHorScrollbar->GetBarSize() < mHorScrollbar->GetMaxValue() - mHorScrollbar->GetMinValue());
		if (!mHorScrollbar->IsVisible())
		{
			mScroll.x = 0;
		}
	}

	if (mVerScrollbar)
	{
		mVerScrollbar->SetValueRange(0.0f, Max(contentSize.getSizeY() - mClippingLayout.mSize.y, 0.1f));
		mVerScrollbar->SetBarSize(mClippingLayout.mSize.y/contentSize.getSizeY()*mVerScrollbar->GetMaxValue());
		mVerScrollbar->SetVisible(mVerScrollbar->GetBarSize() < mVerScrollbar->GetMaxValue() - mVerScrollbar->GetMinValue());
		if (!mVerScrollbar->IsVisible())
			mScroll.y = 0;
	}

	UpdateCursorSpritePos();
	UpdateSelection();
}

void UIEditBox::OnFocused()
{
	UIWidget::OnFocused();

	if (mFocusedState)
		mFocusedState->SetState(true);

	mCursorSprite->SetEnabled(true);
	mCursorVisibleTimer = 0;
}

void UIEditBox::OnFocusLost()
{
	UIWidget::OnFocusLost();

	if (mFocusedState)
		mFocusedState->SetState(false);

	mCursorSprite->SetEnabled(false);
	mCursorVisibleTimer = 0;
}

void UIEditBox::DrawDebug()
{
	Font::TextSymbolsSet* symbSet = mText->GetSymbolsSet();

	int idx = 0;
	FOREACH(Font::TextSymbolsSet::LineDefsArr, symbSet->mLineDefs, line)
	{
		AppRender()->DrawRectFrame(line->mPosition, line->mPosition + line->mSize);
		FOREACH(Font::TextSymbolsSet::SymbolDefsArr, line->mSymbols, symb)
		{
			AppRender()->DrawRectFrame(symb->mFrame.LeftTop(), symb->mFrame.RightBottom(), Color4::SomeColor(idx++));
		}
	}
}

int UIEditBox::GetCharacterIdxAtPoint(const Vec2F& point)
{
	Font::TextSymbolsSet* symbSet = mText->GetSymbolsSet();


	bool checkUp, checkDown, checkLeft, checkRight;
	int lineIdx = 0;
	FOREACH(Font::TextSymbolsSet::LineDefsArr, symbSet->mLineDefs, line)
	{
		checkUp = lineIdx > 0;
		checkDown = lineIdx < (int)symbSet->mLineDefs.size() - 1;

		if (line->mSymbols.size() == 0 && point.y > line->mPosition.y && point.y < line->mPosition.y + line->mSize.y)
			return line->mLineBegSymbol;

		int idx = 0;
		FOREACH(Font::TextSymbolsSet::SymbolDefsArr, line->mSymbols, symb)
		{
			checkLeft = idx > 0;
			checkRight = idx < (int)line->mSymbols.size() - 1;

			RectF sr = symb->mFrame;

			bool ls = checkLeft ? sr.left < point.x:true;
			bool rs = checkRight ? sr.right > point.x:true;
			bool ts = checkUp ? sr.top < point.y:true;
			bool bs = checkDown ? sr.down > point.y:true;

			if (ls && rs && ts && bs)
			{
				if (point.x > (symb->mFrame.left + symb->mFrame.right)*0.5f)
					return line->mLineBegSymbol + idx + 1;
				else
					return line->mLineBegSymbol + idx;
			}

			idx++;
		}

		lineIdx++;
	}

	return 0;
}

Vec2F UIEditBox::GetCharacterPosition(int idx)
{
	if (idx < 0)
		return Vec2F();

	Font::TextSymbolsSet* symbSet = mText->GetSymbolsSet();

	FOREACH(Font::TextSymbolsSet::LineDefsArr, symbSet->mLineDefs, line)
	{
		if (line->mLineBegSymbol + (int)line->mSymbols.size() < idx)
			continue;

		if (line->mSymbols.size() == 0)
			return line->mPosition + Vec2F(0.0f, line->mSize.y);

		int locIdx = idx - line->mLineBegSymbol;
		Vec2F res;
		if (locIdx >= (int)line->mSymbols.size())
		{
			Font::TextSymbolsSet::SymbolDef* symb = &(line->mSymbols[line->mSymbols.size() - 1]);
			res = symb->mFrame.getldCorner() - symb->mOffset + Vec2F(symb->mAdvance, 0.0f);
		}
		else
		{
			Font::TextSymbolsSet::SymbolDef* symb = &(line->mSymbols[locIdx]);
			res = symb->mFrame.getldCorner() - symb->mOffset;
		}
		res.y = line->mPosition.y + line->mSize.y;

		return res;
	}

	return mTextLayout.mPosition + Vec2F(0.0f, mText->GetFont()->GetLineHeight()*(1.0f + 1.0f - mText->GetLinesDistCoef()));
}

void UIEditBox::SetCursorColor(const Color4& color)
{
	mCursorSprite->SetColor(color);
}

void UIEditBox::SetSelectionColor(const Color4& color)
{
	mSelectionColor = color;
}

void UIEditBox::SetCursorVisibleDelay(float delay)
{
	mCursorVisibleDelay = delay;
}

void UIEditBox::SelectionAddRect(const RectF& rect)
{
	unsigned long color = mSelectionColor.AsULong();

	mSelectionMesh->mVerticies[mSelectionMesh->mVertexCount++] = Vertex2(rect.getltCorner(), color, 0.0f, 0.0f);
	mSelectionMesh->mVerticies[mSelectionMesh->mVertexCount++] = Vertex2(rect.getrtCorner(), color, 0.0f, 0.0f);
	mSelectionMesh->mVerticies[mSelectionMesh->mVertexCount++] = Vertex2(rect.getrdCorner(), color, 0.0f, 0.0f);
	mSelectionMesh->mVerticies[mSelectionMesh->mVertexCount++] = Vertex2(rect.getldCorner(), color, 0.0f, 0.0f);

	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3] = mSelectionMesh->mVertexCount - 4;
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3 + 1] = mSelectionMesh->mVertexCount - 3;
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3 + 2] = mSelectionMesh->mVertexCount - 2;
	mSelectionMesh->mPolyCount++;

	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3] = mSelectionMesh->mVertexCount - 4;
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3 + 1] = mSelectionMesh->mVertexCount - 2;
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3 + 2] = mSelectionMesh->mVertexCount - 1;
	mSelectionMesh->mPolyCount++;
}

void UIEditBox::UpdateCursorVisible(float dt)
{
	mCursorVisibleTimer += dt;
	if (mCursorVisibleTimer > mCursorVisibleDelay)
	{
		mCursorVisibleTimer = 0.0f;
		mCursorSprite->SetEnabled(!mCursorSprite->IsEnabled());
	}

	mCursorSprite->SetEnabled(mCursorSprite->IsEnabled() && mSelectionEnd >= 0 && mFocused);
}

void UIEditBox::UpdateSelectionEndPosition(int position, bool selecting)
{
	mSelectionEnd = Clamp(position, 0, (int)mText->GetText().length());

	UpdateCursorSpritePos();

	if (!selecting)
		mSelectionStart = mSelectionEnd;

	UpdateSelection();
	CheckScrollingToCursor();
}

void UIEditBox::UpdateCursorSpritePos()
{
	mCursorSprite->SetPosition(GetCharacterPosition(mSelectionEnd));
	mCursorSprite->SetEnabled(true);
	mCursorVisibleTimer = 0;
}

void UIEditBox::UpdateSelection()
{
	mSelectionMesh->mVertexCount = 0;
	mSelectionMesh->mPolyCount = 0;

	if (mSelectionStart == mSelectionEnd)
		return;

	int start = Min(mSelectionStart, mSelectionEnd);
	int end = Max(mSelectionStart, mSelectionEnd);

	Font::TextSymbolsSet* symbSet = mText->GetSymbolsSet();

	FOREACH(Font::TextSymbolsSet::LineDefsArr, symbSet->mLineDefs, line)
	{
		if (start > line->mLineBegSymbol + (int)line->mSymbols.size() || end < line->mLineBegSymbol)
			continue;

		if (line->mSymbols.size() == 0)
		{
			Font* font = mText->GetFont();
			Vec2F lt = line->mPosition + Vec2F(0, font->GetLineHeight()*(1.0f - mText->GetLinesDistCoef()));
			Vec2F rd = lt + Vec2F(font->GetSymbolAdvance(' '), font->GetLineHeight()*mText->GetLinesDistCoef());
			SelectionAddRect(RectF(lt, rd));

			continue;
		}

		int begSymbol = Max(start - line->mLineBegSymbol, 0);
		int endSymbol = end - line->mLineBegSymbol;
		float endOffs = 0, begOffs = 0;

		if (begSymbol >= (int)line->mSymbols.size())
		{
			begOffs = line->mSymbols.back().mAdvance;
			begSymbol = line->mSymbols.size() - 1;
		}

		if (endSymbol >= (int)line->mSymbols.size())
		{
			endOffs = line->mSymbols.back().mAdvance;
			if (line->mEndedNewLine && endSymbol > (int)line->mSymbols.size())
				endOffs += mText->GetFont()->GetSymbolAdvance(' ');

			endSymbol = line->mSymbols.size() - 1;
		}

		Vec2F lt(line->mSymbols[begSymbol].mFrame.left - line->mSymbols[begSymbol].mOffset.x + begOffs,
				 line->mPosition.y + line->mSize.y*(1.0f - mText->GetLinesDistCoef()));
		Vec2F rd(line->mSymbols[endSymbol].mFrame.left - line->mSymbols[endSymbol].mOffset.x + endOffs,
				 line->mPosition.y + line->mSize.y);

		SelectionAddRect(RectF(lt, rd));
	}
}

void UIEditBox::ScrollChanged()
{
	if (mHorScrollbar)
		mScroll.x = mHorScrollbar->GetValue();

	if (mVerScrollbar)
		mScroll.y = mVerScrollbar->GetValue();

	UpdateLayout();
}

void UIEditBox::CheckScrollingToCursor()
{
	Vec2F cursorPos = mCursorSprite->GetPosition();
	RectF clipRect = mClippingLayout.GetRect();

	float rightOffs = Max(cursorPos.x - clipRect.right + 5.0f, 0.0f);
	float leftOffs = Max(clipRect.left - cursorPos.x + 5.0f, 0.0f);


	float downOffs = Max(cursorPos.y - clipRect.down + 5.0f, 0.0f);
	float topOffs = Max(clipRect.top - cursorPos.y + 5.0f + mText->GetFont()->GetLineHeight(), 0.0f);

	if (mHorScrollbar)
		mHorScrollbar->SetValueClamped(mScroll.x + rightOffs - leftOffs);
	else
	{
		mScroll.x = Clamp(mScroll.x + rightOffs - leftOffs,
						  0.0f, Max(mText->GetSymbolsSet()->mRealSize.x - clipRect.getSizeX(), 0.0f) + 5.0f);
	}

	if (mMultiLine)
	{
		if (mVerScrollbar)
			mVerScrollbar->SetValueClamped(mScroll.y + downOffs - topOffs);
		else
			mScroll.y = mScroll.y + downOffs - topOffs;
	}
}

void UIEditBox::InitializeProperties()
{
	REG_PROPERTY(UIEditBox, ctext, SetCText, GetCText);
	REG_PROPERTY(UIEditBox, text, SetText, GetText);
}

CLOSE_O2_NAMESPACE