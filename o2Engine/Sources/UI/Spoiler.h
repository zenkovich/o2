#pragma once

#include "UI/Widget.h"

namespace o2
{
	// -------------------------------------
	// UI Spoiler. Can hide children widgets
	// -------------------------------------
	class UISpoiler: public UIWidget
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
		void Draw();

		SERIALIZABLE(UISpoiler);

	protected:
		UIWidgetState* mExpandState = nullptr; // Expanding state
		float          mExpandCoef = 0.0f;     // Expanding animation coefficient 0...1 
		float          mTargetHeight = 0.0f;   // target expanding height

	protected:
		// Updates expanding
		void UpdateExpanding(float dt);

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		// Expands size by childs
		void ExpandSizeByChilds();

		// Returns is spoiler fully expanded and not animating
		bool IsFullyExpanded() const;
	};
}