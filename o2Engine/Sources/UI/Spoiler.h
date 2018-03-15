#pragma once

#include "UI/VerticalLayout.h"

namespace o2
{
	class UIButton;

	// -------------------------------------
	// UI Spoiler. Can hide children widgets
	// -------------------------------------
	class UISpoiler: public UIVerticalLayout
	{
	public:
		PROPERTY(WString> caption;    // Head caption property
		PROPERTY(float>   headHeight; // Head height property
		PROPERTY(bool>    expanded;   // Expanded state property

		Function<void()> onExpand;    // Expand starting event

		// Default constructor
		UISpoiler();

		// Copy-constructor
		UISpoiler(const UISpoiler& other);

		// Copy-operator
		UISpoiler& operator=(const UISpoiler& other);

		// Expands spoiler
		void Expand();

		// Collapses spoiler
		void Collapse();

		// Sets expanded state
		void SetExpanded(bool expand);

		// Returns is expanded
		bool IsExpanded() const;

		// Draws spoiler
		void Draw() override;

		// Sets caption, if text layer exist
		void SetCaption(const WString& caption);

		// Returns caption, if text layer exist
		WString GetCaption() const;

		// Sets head height
		void SetHeadHeight(float height);

		// Returns head's height
		float GetHeadHeight() const;

		SERIALIZABLE(UISpoiler);

	protected:
		UIWidgetState* mExpandState = nullptr; // Expanding state
		float          mExpandCoef = 0.0f;     // Expanding animation coefficient 0...1 
		float          mTargetHeight = 0.0f;   // target expanding height

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Updates expanding
		void UpdateExpanding(float dt);

		// Returns layout height
		float GetMinHeightWithChildren() const override;

		// Expands size by children
		//virtual void ExpandSizeByChilds();

		// Updates layout's weight and minimal size
		virtual void UpdateLayoutParametres();

		// Checks is expand button exist and sets click callback
		void CheckExpandButton();

		// Searches expand button by name and type
		UIButton* FindExpandButton() const;

		// Returns is spoiler fully expanded and not animating
		bool IsFullyExpanded() const;

		// Returns is spoiler fully collapsed and not animating
		bool IsFullyCollapsed() const;

		// Initializes properties
		void InitializeProperties();
	};
}

CLASS_BASES_META(o2::UISpoiler)
{
	BASE_CLASS(o2::UIVerticalLayout);
}
END_META;
CLASS_FIELDS_META(o2::UISpoiler)
{
	PUBLIC_FIELD(caption);
	PUBLIC_FIELD(headHeight);
	PUBLIC_FIELD(expanded);
	PUBLIC_FIELD(onExpand);
	PROTECTED_FIELD(mExpandState);
	PROTECTED_FIELD(mExpandCoef);
	PROTECTED_FIELD(mTargetHeight);
}
END_META;
CLASS_METHODS_META(o2::UISpoiler)
{

	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, SetHeadHeight, float);
	PUBLIC_FUNCTION(float, GetHeadHeight);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateExpanding, float);
	PROTECTED_FUNCTION(float, GetMinHeightWithChildren);
	PROTECTED_FUNCTION(void, UpdateLayoutParametres);
	PROTECTED_FUNCTION(void, CheckExpandButton);
	PROTECTED_FUNCTION(UIButton*, FindExpandButton);
	PROTECTED_FUNCTION(bool, IsFullyExpanded);
	PROTECTED_FUNCTION(bool, IsFullyCollapsed);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
