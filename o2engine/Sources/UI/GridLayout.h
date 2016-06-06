#pragma once

#include "UI/Widget.h"

namespace o2
{
	// ------------------
	// Grid layout widget
	// ------------------
	class UIGridLayout: public UIWidget
	{
	public:
		Property<BaseCorner>   baseCorner;          // Base corder property
		Property<Vec2F>        cellSize;            // Grid cell size
		Property<int>          arrangeAxisMaxCells; // Maximum cells at arranging axis
		Property<TwoDirection> arrangeAxis;         // Arrange axis
		Property<float>        spacing;             // Space between widgets property
		Property<RectF>        border;              // Border property
		Property<float>        borderLeft;          // Left border property
		Property<float>        borderRight;         // Right border property
		Property<float>        borderTop;           // Top border property
		Property<float>        borderBottom;        // Bottom border property
		Property<bool>         fitByChildren;       // Fitting size by children property

		// Default constructor
		UIGridLayout();

		// Copy-constructor
		UIGridLayout(const UIGridLayout& other);

		// Destructor
		~UIGridLayout();

		// Copy operator
		UIGridLayout& operator=(const UIGridLayout& other);

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

		// Sets cell size
		void SetCellSize(const Vec2F& size);

		// Returns cell size
		Vec2F GetCellSize() const;

		// Sets arrange type
		void SetArrangeAxis(TwoDirection type);

		// Returns arrange type
		TwoDirection GetArrangeAxis() const;

		// Sets arrange axis maximum cells count
		void SetArrangeAxisMaxCells(int count);

		// Returns arrange axis maximum cells count
		int GetArrangeAxisMaxCells() const;

		// Sets fitting size by children
		void SetFitByChildren(bool fit);

		// Returns fitting by children
		bool IsFittingByChildren() const;

		SERIALIZABLE(UIGridLayout);

	protected:
		BaseCorner   mBaseCorner = BaseCorner::Left;          // Base corner of widgets arranging
		float        mSpacing = 0.0f;                         // Space between widgets
		Vec2F        mCellSize = Vec2F(50, 50);               // Grid cell size
		TwoDirection mArrangeAxis = TwoDirection::Horizontal; // Cells arrange axis
		int          mArrangeAxisMaxCells = 5;                // Arrange axis maximum cells count
		RectF        mBorder;                                 // Border
		bool         mFitByChildren = false;                  // Fitting by children

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		// Calls when child widget was added
		void OnChildAdded(UIWidget* child);

		// Calls when child widget was removed
		void OnChildRemoved(UIWidget* child);

		// Invokes required function for childs arranging
		void RearrangeChilds();

		// Arrange childs from left top
		void ArrangeFromLeftTop();

		// Arrange childs from top
		void ArrangeFromTop();

		// Arrange childs from right top
		void ArrangeFromRightTop();

		// Arrange childs from left
		void ArrangeFromLeft();

		// Arrange childs from center
		void ArrangeFromCenter();

		// Arrange childs from right
		void ArrangeFromRight();

		// Arrange childs from left bottom
		void ArrangeFromLeftBottom();

		// Arrange childs from bottom
		void ArrangeFromBottom();

		// Arrange childs from right bottom
		void ArrangeFromRightBottom();

		// Expands size by children
		void ExpandSizeByChilds();

		// Initializes properties
		void InitializeProperties();
	};
}
