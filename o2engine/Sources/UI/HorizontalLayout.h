#pragma once

#include "UI/Widget.h"

namespace o2
{
	// ------------------------
	// Horizontal layout widget
	// ------------------------
	class UIHorizontalLayout: public UIWidget
	{
	public:
		Property<BaseCorner> baseCorner;    // Base corder property
		Property<float>      spacing;       // Space between widgets property
		Property<RectF>      border;        // Border property
		Property<float>      borderLeft;    // Left border property
		Property<float>      borderRight;   // Right border property
		Property<float>      borderTop;     // Top border property
		Property<float>      borderBottom;  // Bottom border property
		Property<bool>       expandWidth;   // Expand children by width property
		Property<bool>       expandHeight;  // Expand children by height property
		Property<bool>       fitByChildren; // Fitting size by children property

		// Default constructor
		UIHorizontalLayout();

		// Copy-constructor
		UIHorizontalLayout(const UIHorizontalLayout& other);

		// Destructor
		~UIHorizontalLayout();

		// Copy operator
		UIHorizontalLayout& operator=(const UIHorizontalLayout& other);

		// Sets base corner
		void SetBaseCorner(BaseCorner baseCorner);

		// Returns base corner
		BaseCorner GetBaseCorner() const;

		// Sets space between widgets
		void SetSpacing(float spacing);

		// Returns space between widgets
		float GetSpacing() const;

		// Sets border
		void SetBorder(const RectF& border);

		// Returns border
		RectF GetBorder() const;

		// Sets left border
		void SetBorderLeft(float value);

		// Returns left border
		float GetBorderLeft() const;

		// Sets right border
		void SetBorderRight(float value);

		// Returns right border
		float GetBorderRight() const;

		// Sets top border
		void SetBorderTop(float value);

		// Returns top border
		float GetBorderTop() const;

		// Sets bottom border
		void SetBorderBottom(float value);

		// Returns bottom border
		float GetBorderBottom() const;

		// Sets expanding by width
		void SetWidthExpand(bool expand);

		// Returns expanding by width
		bool IsWidthExpand() const;

		// Sets expanding by height
		void SetHeightExpand(bool expand);

		// Returns height expand
		bool IsHeightExpand() const;

		// Sets fitting size by children
		void SetFitByChildren(bool fit);

		// Returns fitting by children
		bool IsFittingByChildren() const;

		SERIALIZABLE(UIHorizontalLayout);

	protected:
		BaseCorner mBaseCorner;    // Base corner of widgets arranging
		float      mSpacing;       // Space between widgets
		RectF      mBorder;        // Border
		bool       mExpandWidth;   // Expanding by width
		bool       mExpandHeight;  // Expanding by height
		bool       mFitByChildren; // Fitting by children

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Calls when child widget was added
		void OnChildAdded(Ptr<UIWidget> child);

		// Calls when child widget was removed
		void OnChildRemoved(Ptr<UIWidget> child);

		// Invokes reque function for childs arranging
		void RearrangeChilds();

		// Arranging child from left to right by bottom, middle and top
		void ArrangeFromLeftToRight();

		// Arranging child from right to left by bottom, middle and top
		void ArrangeFromRightToLeft();

		// Arranging child from center by bottom, middle and top
		void ArrangeFromCenter();

		// Expands size by children
		void ExpandSizeByChilds();

		// Calculates children widths by weights and minimal sizes
		Vector<float> CalculateExpandedWidths();

		// Aligns widget by height with base corner
		void AlignWidgetByHeight(Ptr<UIWidget> child, float widthAnchor);

		// Updates layout's weight and minimal size
		void UpdateLayoutParametres();

		// Initializes properties
		void InitializeProperties();
	};
}
