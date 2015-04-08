#pragma once

#include "ui_drawables_list_widget.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class Text;
class Font;
class Sprite;
class Mesh;
class UIScrollBar;

class UIEditBox: public UIDrawablesListWidget
{
	friend class UIStdSkinInitializer;

	UIState*     mHoverState;           /** Hover state. */
	UIState*     mFocusedState;         /** Focused state. */

	Text*        mText;                 /** Text drawable. */
	Vec2F        mScroll;               /** Scrolling in pixels. */
	Vec2F        mSmoothScroll;         /** Smoothed scrolling. */
	Layout       mClippingLayout;       /** Clipping area layout. */
	Layout       mTextLayout;           /** Text layout. */

	Sprite*      mCursorSprite;         /** Cursor sprite drawable. */
	Mesh*        mSelectionMesh;        /** Selection mesh drawable. */
	Color4       mSelectionColor;
	int          mSelectionStart;       /** Selection start character idx. */
	int          mSelectionEnd;         /** Selection end character idx. */

	float        mCursorVisibleTimer;   /** Cursor visible timer. When in greater than mCursorVisibleDelay, it will hide. */
	float        mCursorVisibleDelay;   /** Cursor visible delay. */
	bool         mMultiLine;            /** True, if multi line. */

	bool         mCursorInsideClipping; /** True, if cursor inside clipping rect. */

	UIScrollBar* mHorScrollbar;         /** Horizontal scroll bar child widget. */
	UIScrollBar* mVerScrollbar;         /** Vertical scroll bar child widget. */


public:
	DEFINE_TYPE(UIEditBox);

	PROPERTY(UIEditBox, String) ctext; /** C Text caption property. Using set/getCCaption. */
	PROPERTY(UIEditBox, WString) text; /** Caption property. Using set/getCaption. */

	CallbackChain onTextChangeEvent;  /** On text changing event. */


	/** ctor. */
	UIEditBox(Font* font, const Layout& layout, UIScrollBar* horBarSample = NULL, UIScrollBar* verBarSample = NULL,
			  const String& id = "");

	/** copy-ctor. */
	UIEditBox(const UIEditBox& editbox);

	/** dtor. */
	~UIEditBox();

	/** Returns copy of widget. */
	UIWidget* Clone() const;

	/** Returns true, if widget can take focus. */
	virtual bool IsFocusable() const;

	/** Sets c text caption. */
	void SetCText(const String& text);

	/** Returns c text caption. */
	String GetCText() const;

	/** Sets caption text. */
	void SetText(const WString& text);

	/** Returns caption text. */
	WString GetText() const;

	/** Sets word wrapping. */
	void SetWordWrap(bool wordWrap);

	/** returns word wrapping flag. */
	bool IsWordWrap() const;

	/** Sets horizontal scroll bar sample. */
	void SetHorScrollbar(UIScrollBar* scrollbar);

	/** Sets vertical scroll bar sample. */
	void SetVerScrollbar(UIScrollBar* scrollbar);

	/** Sets current scroll. */
	void SetScroll(const Vec2F& scroll);

	/** Returns current scroll. */
	Vec2F SetScroll() const;

protected:
	/** Sets cursor color. */
	void SetCursorColor(const Color4& color);

	/** Sets selection color. */
	void SetSelectionColor(const Color4& color);

	/** Sets cursor visible state delay. */
	void SetCursorVisibleDelay(float delay);

	/** Calls when added some state. */
	virtual void AddedState(UIState* state);

	/** Updating current widget. */
	virtual void LocalUpdate(float dt);

	/** Processing input message in current widget. */
	virtual bool LocalProcessInputMessage(const InputMessage& msg);

	/** Processing characters input. */
	void ProcessInputCharacters(const InputMessage &msg);

	/** Processing erasing. */
	void ProcessErasing(const InputMessage &msg);

	/** Processing navigation. */
	void ProcessNavigation(const InputMessage &msg);

	/** Drawing current widget. */
	virtual void LocalDraw();

	/** Calls when widget's layout updated. */
	virtual void LayoutUpdated();

	/** Calls when widget focused. */
	virtual void OnFocused();

	/** Calls when widget lost focus. */
	virtual void OnFocusLost();

	/** Returns character index at point. */
	int GetCharacterIdxAtPoint(const Vec2F& point);

	/** Returns character position by index. */
	Vec2F GetCharacterPosition(int idx);

	/** Jumping at direction to next jump symbol. */
	void JumpSelection(bool forward, bool selecting);

	/** Adding rect to selection mesh. */
	void SelectionAddRect(const RectF& rect);

	/** Drawing symbols debug frames. */
	void DrawDebug();

	/** Updates cursor visible state. */
	void UpdateCursorVisible(float dt);

	/** Updates end selection position. */
	void UpdateSelectionEndPosition(int position, bool selecting);

	/** Updates cursor sprite position by end selection index. */
	void UpdateCursorSpritePos();

	/** Updates selection mesh by selection start and end. */
	void UpdateSelection();

	/** Updates scrolling for visible cursor. */
	void CheckScrollingToCursor();

	/** Calls when hor or vertical bar moved. */
	void ScrollChanged();

	/** Initializing properties. */
	void InitializeProperties();
};

CLOSE_O2_NAMESPACE
