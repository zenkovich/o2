#pragma once

#include "UI/VerticalLayout.h"

namespace o2
{
	// -------------------------------------
	// UI Spoiler. Can hide children widgets
	// -------------------------------------
	class UISpoiler: public UIVerticalLayout
	{
	public:
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

		// Updates layout
		void UpdateLayout(bool withChildren = true) override;

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

		// Returns is spoiler fully expanded and not animating
		bool IsFullyExpanded() const;

		// Returns is spoiler fully collapsed and not animating
		bool IsFullyCollapsed() const;
	};
}

CLASS_BASES_META(o2::UISpoiler)
{
	BASE_CLASS(o2::UIVerticalLayout);
}
END_META;
CLASS_FIELDS_META(o2::UISpoiler)
{
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
	PUBLIC_FUNCTION(void, UpdateLayout, bool);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateExpanding, float);
	PROTECTED_FUNCTION(float, GetMinHeightWithChildren);
	PROTECTED_FUNCTION(void, UpdateLayoutParametres);
	PROTECTED_FUNCTION(bool, IsFullyExpanded);
	PROTECTED_FUNCTION(bool, IsFullyCollapsed);
}
END_META;
