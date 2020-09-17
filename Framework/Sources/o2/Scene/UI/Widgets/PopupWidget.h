#pragma once

#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"

namespace o2
{
	// ----------------------------------------------------------------------------------------
	// Popup widget. Draws on top of all UI, can be opened only one. Closes other automatically
	// ----------------------------------------------------------------------------------------
	class PopupWidget: public ScrollArea, public KeyboardEventsListener
	{
	public:
		bool fitByChildren = true; // Fit size when opening by children @SERIALIZABLE

	public:
		// Default constructor
		PopupWidget();

		// Copy-constructor
		PopupWidget(const PopupWidget& other);

		// Destructor
		~PopupWidget();

		// Copy operator
		PopupWidget& operator=(const PopupWidget& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

		// Show from parent context
		virtual void Show(PopupWidget* parent, const Vec2F& position = o2Input.GetCursorPos());

		// Shows context
		void Show(const Vec2F& position = o2Input.GetCursorPos());

		// Sets minimal fitting size
		void SetMinFitSize(float size);

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

		// Fits size by items
		virtual void FitSizeAndPosition(const Vec2F& position);

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(PopupWidget);

	protected:
		static PopupWidget* mVisiblePopup; // Current visible popup widget

		float mFitSizeMin = 40.0f;    // Minimal fitting size @SERIALIZABLE

		PopupWidget* mParentPopup = nullptr; // Parent visible popup widget
		PopupWidget* mChildPopup = nullptr;  // Child visible popup widget

		bool mShownAtFrame = false; // Is popup was shown at current frame

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Checks widget clipping by area
		void CheckClipping(const RectF& clipArea) override;

		// It is called when visible was changed
		void OnEnableInHierarchyChanged() override;

		// Fits position by size and screen
		virtual void FitPosition(const Vec2F& position, Vec2F size);

		// Returns content size for fitting by children
		virtual Vec2F GetContentSize() const;

		// Hides context with his parent
		virtual void HideWithParent();

		// Hides context with his child
		virtual void HideWithChild();

		// Special drawing for contexts
		virtual void SpecialDraw();

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor released outside this(only when cursor pressed this at previous time)
		void OnCursorReleasedOutside(const Input::Cursor& cursor) override;

		// It is called when key was released
		void OnKeyPressed(const Input::Key& key) override;

		friend class ContextMenu;
		friend class MenuPanel;
		friend class UIManager;
	};
}

CLASS_BASES_META(o2::PopupWidget)
{
	BASE_CLASS(o2::ScrollArea);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::PopupWidget)
{
	PUBLIC_FIELD(fitByChildren).DEFAULT_VALUE(true).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mFitSizeMin).DEFAULT_VALUE(40.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mParentPopup).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mChildPopup).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mShownAtFrame).DEFAULT_VALUE(false);
}
END_META;
CLASS_METHODS_META(o2::PopupWidget)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Show, PopupWidget*, const Vec2F&);
	PUBLIC_FUNCTION(void, Show, const Vec2F&);
	PUBLIC_FUNCTION(void, SetMinFitSize, float);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(bool, IsInputTransparent);
	PUBLIC_FUNCTION(void, FitSizeAndPosition, const Vec2F&);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, CheckClipping, const RectF&);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, FitPosition, const Vec2F&, Vec2F);
	PROTECTED_FUNCTION(Vec2F, GetContentSize);
	PROTECTED_FUNCTION(void, HideWithParent);
	PROTECTED_FUNCTION(void, HideWithChild);
	PROTECTED_FUNCTION(void, SpecialDraw);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleasedOutside, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
}
END_META;
