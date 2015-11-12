#pragma once

#include "Events/CursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "UI/Widget.h"

namespace o2
{
	class Text;
	class Mesh;
	class Sprite;

	// --------------------
	// Text edit box widget
	// --------------------
	class UIEditBox: public UIWidget, public CursorEventsListener, public KeyboardEventsListener
	{
	public:
		Property<WString> text;			  // Text property
		Property<int>     caret;		  // Caret position property
		Property<Vec2F>   scroll;		  // Scroll position property
		Property<float>   horScroll;	  // Horizontal scroll position property
		Property<float>   verScroll;	  // Vertical scroll position property
		Property<int>     selectionBegin; // Selection begin index property
		Property<int>     selectionEnd;	  // Selection end index property

		// Default constructor
		UIEditBox();

		// Copy-constructor
		UIEditBox(const UIEditBox& other);

		// Destructor
		~UIEditBox();

		// Copy-operator
		UIEditBox& operator=(const UIEditBox& other);

		// Draws widget
		void Draw();

		// Updates widget
		void Update(float dt);

		// Sets text
		void SetText(const WString& text);

		// returns text
		WString GetText() const;

		// Sets caret position
		void SetCaretPosition(int caretPosition);

		// Returns caret position
		int GetCaretPosition();

		// Sets selection begin and end indexes
		void Select(int begin, int end);

		// Sets selection begin position
		void SetSelectionBegin(int position);

		// Returns selection begin position
		int GetSelectionBegin() const;

		// Sets selection end position
		void SetSelectionEnd(int position);

		// Returns selection end position
		int GetSelectionEnd() const;

		// Resets selection
		void Deselect();

		// Selects all text
		void SelectAll();

		// Sets scroll position
		void SetScroll(const Vec2F& scroll);

		// Returns scroll position
		Vec2F GetScroll() const;

		// Returns scroll available range
		RectF GetScrollRange() const;

		// Resets scroll to zero
		void ResetScroll();

		// Sets horizontal scroll position
		void SetHorizontalScroll(float scroll);

		// Returns horizontal scroll position
		float GetHorizontalScroll() const;

		// Sets vertical scroll position
		void SetVerticalScroll(float scroll);

		// Returns vertical scroll position
		float GetVerticalScroll() const;

		// Sets clipping area layout
		void SetClipArea(const Layout& layout);

		// Returns clipping area layout
		Layout GetClipArea() const;

		// Sets text layout
		void SetTextLayout(const Layout& layout);

		// Returns text layout
		Layout GetTextLayout() const;

		// Sets horizontal scroll bar
		void SetHorizontalScrollBar(Ptr<UIHorizontalScrollBar> scrollbar, bool owner = true);

		// Returns horizontal scroll bar
		Ptr<UIHorizontalScrollBar> GetHorizontalScrollbar() const;

		// Sets Vertical scroll bar
		void SetVerticalScrollBar(Ptr<UIVerticalScrollBar> scrollbar, bool owner = true);

		// Returns Vertical scroll bar
		Ptr<UIVerticalScrollBar> GetVerticalScrollbar() const;

		// Returns text drawable
		Ptr<Text> GetTextDrawable();

		// Returns caret drawable
		Ptr<Sprite> GetCaretDrawable();

		// Sets selection color
		void SetSelectionColor(const Color4& color);

		// Returns selection color
		Color4 GetSelectionColor() const;

		// Sets filter characters for integer numbers
		void SetFilterInteger();

		// Sets filter characters for decimal numbers
		void SetFilterFloat();

		//Sets filter characters for names
		void SetFilterNames();

		// Sets available characters
		void SetAvailableSymbols(const WString& availableSymbols);

		// Returns available characters
		WString GetAvailableSymbols() const;

		// Sets max sizes of line and lines count
		void SetMaxSizes(int maxLineCharactersCount, int maxLinesCount);

		// Sets maximum characters in line
		void SetMaxLineCharactersCount(int count);

		// Returns maximum characters in line
		int GetMaxLineCharactersCount() const;

		// Sets maximum lines count
		void SetMaxLinesCount(int count);

		// Returns maximum lines count
		int GetMaxLinesCount() const;

		// Sets text multiline
		void SetMultiLine(bool multiline);

		// Returns is text multiline
		bool IsMultiLine() const;
		 
		// Sets word wrapping
		void SetWordWrap(bool wordWrap);

		// returns is word wrapping
		bool IsWordWrap() const;

		// Sets caret blinking delay in seconds
		void SetCaretBlinkingDelay(float delay);

		// Returns caret blinking delay in seconds
		float GetCaretBlinkingDelay() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		SERIALIZABLE_IMPL(UIEditBox);

		IOBJECT(UIEditBox)
		{
			BASE_CLASS(UIWidget);

			FIELD(text);
			FIELD(caret);
			FIELD(scroll);
			FIELD(horScroll);
			FIELD(verScroll);
			FIELD(selectionBegin);
			FIELD(selectionEnd);
		}

	protected:
		Ptr<Text>                  mTextDrawable;     // Text drawable
		Ptr<Mesh>                  mSelectionMesh;    // Selection mesh
		Ptr<Sprite>                mCaretDrawable;    // Caret drawable
		float                      mCaretBlinkDelay;  // Caret blinking delay
		float                      mCaretBlinkTime;   // Caret blinking timer
		Ptr<UIHorizontalScrollBar> mHorScrollBar;     // horizontal scroll bar
		Ptr<UIVerticalScrollBar>   mVerScrollBar;     // Vertical scroll bar
		bool                       mOwnHorScrollBar;  // True, if this widget is owner of mHorScrollBar
		bool                       mOwnVerScrollBar;  // True, if this widget is owner of mVerScrollBar
		Layout                     mTextLayout;       // Text drawable layout
		Layout                     mClipArea;         // Clipping area layout
		RectF                      mAbsoluteClipArea; // Clipping area
		RectF                      mAbsoluteTextArea; // Text drawable area
		Vec2F                      mScrollPos;        // Scroll position
		RectF                      mScrollArea;       // Maximum scroll area size
		RectF                      mScrollRange;      // Scroll range by width and height
		float                      mDrawDepth;        // Drawing depth at current frame
		WString                    mText;             // Current text
		WString                    mAvailableSymbols; // Available symbols
		int                        mSelectionBegin;	  // Selection begin index
		int                        mSelectionEnd;	  // Selection end index
		Color4                     mSelectionColor;   // Text selection color
		bool                       mMultiLine;		  // True if text is multiline
		bool                       mWordWrap;		  // True if text words wrapping
		int                        mMaxLineChars;	  // Count of maximum characters in line
		int                        mMaxLinesCount;	  // Count of maximum lines count
		float                      mLastClickTime;    // Time of last clicking
		Vec2F                      mLastCursorPos;    // Last pressed cursor position
		bool                       mSelectingByWords; // Selection works by solid words
		int                        mSelWordBegin;     // Selection by words begin index
		int                        mSelWordEnd;       // Selection by words end index

	protected:
		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor);

		// Calls when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor);

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// Calls when scrolling
		void OnScrolled(float scroll);

		// Calls when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// Calls when key was released
		void OnKeyReleased(const Input::Key& key);

		// Calls when key stay down during frame
		void OnKeyStayDown(const Input::Key& key);

		// Returns text filtered by available characters set
		WString GetFilteredText(const WString& text);

		// Updates scroll parameters: clip area, scroll size
		void UpdateScrollParams();

		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Calls when horizontal scroll bar value was changed
		void OnHorScrollChanged(float value);

		// Calls when vertical scroll bar value was changed
		void OnVerScrollChanged(float value);

		// Beginning serialization callback
		void OnSerialize(DataNode& node);

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Checks text for maximal characters in line and lines count
		void CheckCharactersAndLinesBounds();

		// Updates selection mesh and fixing selection bounds
		void UpdateSelectionAndCaret();

		// Returns coordinates of caret position by caret text position
		Vec2F GetTextCaretPosition(int position);

		//Returns of caret text position caret position by caret coordinates
		int GetTextCaretPosition(const Vec2F& point);

		// Updates cursor blinking
		void UpdateCaretBlinking(float dt);

		// Adds rectangle geometry to selection mesh
		void AddSelectionRect(const RectF& rect);

		// Checks scroll to cursor
		void CheckScrollingToCaret();

		// Jump selection over word
		void JumpSelection(bool forward, bool selecting);

		// Checks character typing by keyboard key
		void CheckCharacterTyping(KeyboardKey key);

		// Check for erasing commands
		void CheckCharactersErasing(KeyboardKey key);

		// Checks caret moving commands
		void CheckCaretMoving(KeyboardKey key);

		// Checks clipboard commands (copy cut paste)
		void CheckClipboard(KeyboardKey key);

		// Moves mSelectionEnd position and mSelectionBegin, when selecting
		void MoveCaret(int newPosition, bool selecting);

		// initializes properties
		void InitializeProperties();
	};
}