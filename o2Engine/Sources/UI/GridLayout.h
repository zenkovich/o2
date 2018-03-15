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
		PROPERTIES(UIGridLayout);
		PROPERTY(BaseCorner, baseCorner, SetBaseCorner, GetBaseCorner);                     // Base corder property
		PROPERTY(Vec2F, cellSize, SetCellSize, GetCellSize);                                // Grid cell size
		PROPERTY(int, arrangeAxisMaxCells, SetArrangeAxisMaxCells, GetArrangeAxisMaxCells); // Maximum cells at arranging axis
		PROPERTY(TwoDirection, arrangeAxis, SetArrangeAxis, GetArrangeAxis);                // Arrange axis

		PROPERTY(float, spacing, SetSpacing, GetSpacing);                // Space between widgets property
		PROPERTY(RectF, border, SetBorder, GetBorder);                   // Border property
		PROPERTY(float, borderLeft, SetBorderLeft, GetBorderLeft);       // Left border property
		PROPERTY(float, borderRight, SetBorderRight, GetBorderRight);    // Right border property
		PROPERTY(float, borderTop, SetBorderTop, GetBorderTop);          // Top border property
		PROPERTY(float, borderBottom, SetBorderBottom, GetBorderBottom); // Bottom border property

		PROPERTY(bool, fitByChildren, SetFitByChildren, IsFittingByChildren); // Fitting size by children property

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

		// Updates layout
		void UpdateTransform(bool withChildren = true) override;

		SERIALIZABLE(UIGridLayout);

	protected:
		BaseCorner   mBaseCorner = BaseCorner::Left;          // Base corner of widgets arranging @SERIALIZABLE
		float        mSpacing = 0.0f;                         // Space between widgets @SERIALIZABLE
		Vec2F        mCellSize = Vec2F(50, 50);               // Grid cell size @SERIALIZABLE
		TwoDirection mArrangeAxis = TwoDirection::Horizontal; // Cells arrange axis @SERIALIZABLE
		int          mArrangeAxisMaxCells = 5;                // Arrange axis maximum cells count @SERIALIZABLE
		RectF        mBorder;                                 // Border @SERIALIZABLE
		bool         mFitByChildren = false;                  // Fitting by children @SERIALIZABLE

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when child widget was added
		void OnChildAdded(UIWidget* child) override;

		// It is called when child widget was removed
		void OnChildRemoved(UIWidget* child) override;

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
	};
}

CLASS_BASES_META(o2::UIGridLayout)
{
	BASE_CLASS(o2::UIWidget);
}
END_META;
CLASS_FIELDS_META(o2::UIGridLayout)
{
	PUBLIC_FIELD(baseCorner);
	PUBLIC_FIELD(cellSize);
	PUBLIC_FIELD(arrangeAxisMaxCells);
	PUBLIC_FIELD(arrangeAxis);
	PUBLIC_FIELD(spacing);
	PUBLIC_FIELD(border);
	PUBLIC_FIELD(borderLeft);
	PUBLIC_FIELD(borderRight);
	PUBLIC_FIELD(borderTop);
	PUBLIC_FIELD(borderBottom);
	PUBLIC_FIELD(fitByChildren);
	PROTECTED_FIELD(mBaseCorner).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSpacing).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCellSize).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mArrangeAxis).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mArrangeAxisMaxCells).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mBorder).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mFitByChildren).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::UIGridLayout)
{

	PUBLIC_FUNCTION(void, SetBaseCorner, BaseCorner);
	PUBLIC_FUNCTION(BaseCorner, GetBaseCorner);
	PUBLIC_FUNCTION(void, SetSpacing, float);
	PUBLIC_FUNCTION(float, GetSpacing);
	PUBLIC_FUNCTION(void, SetBorder, const RectF&);
	PUBLIC_FUNCTION(RectF, GetBorder);
	PUBLIC_FUNCTION(void, SetBorderLeft, float);
	PUBLIC_FUNCTION(float, GetBorderLeft);
	PUBLIC_FUNCTION(void, SetBorderRight, float);
	PUBLIC_FUNCTION(float, GetBorderRight);
	PUBLIC_FUNCTION(void, SetBorderTop, float);
	PUBLIC_FUNCTION(float, GetBorderTop);
	PUBLIC_FUNCTION(void, SetBorderBottom, float);
	PUBLIC_FUNCTION(float, GetBorderBottom);
	PUBLIC_FUNCTION(void, SetCellSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetCellSize);
	PUBLIC_FUNCTION(void, SetArrangeAxis, TwoDirection);
	PUBLIC_FUNCTION(TwoDirection, GetArrangeAxis);
	PUBLIC_FUNCTION(void, SetArrangeAxisMaxCells, int);
	PUBLIC_FUNCTION(int, GetArrangeAxisMaxCells);
	PUBLIC_FUNCTION(void, SetFitByChildren, bool);
	PUBLIC_FUNCTION(bool, IsFittingByChildren);
	PUBLIC_FUNCTION(void, UpdateTransform, bool);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnChildAdded, UIWidget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, UIWidget*);
	PROTECTED_FUNCTION(void, RearrangeChilds);
	PROTECTED_FUNCTION(void, ArrangeFromLeftTop);
	PROTECTED_FUNCTION(void, ArrangeFromTop);
	PROTECTED_FUNCTION(void, ArrangeFromRightTop);
	PROTECTED_FUNCTION(void, ArrangeFromLeft);
	PROTECTED_FUNCTION(void, ArrangeFromCenter);
	PROTECTED_FUNCTION(void, ArrangeFromRight);
	PROTECTED_FUNCTION(void, ArrangeFromLeftBottom);
	PROTECTED_FUNCTION(void, ArrangeFromBottom);
	PROTECTED_FUNCTION(void, ArrangeFromRightBottom);
	PROTECTED_FUNCTION(void, ExpandSizeByChilds);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
