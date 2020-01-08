#pragma once

#include "o2/Scene/UI/Widget.h"

namespace o2
{
	// ----------------------
	// Vertical layout widget
	// ----------------------
	class VerticalLayout: public Widget
	{
	public:
		PROPERTIES(VerticalLayout);
		PROPERTY(BaseCorner, baseCorner, SetBaseCorner, GetBaseCorner); // Base corder property

		PROPERTY(float, spacing, SetSpacing, GetSpacing); // Space between widgets property

		PROPERTY(BorderF, border, SetBorder, GetBorder);                 // Border property
		PROPERTY(float, borderLeft, SetBorderLeft, GetBorderLeft);       // Left border property
		PROPERTY(float, borderRight, SetBorderRight, GetBorderRight);    // Right border property
		PROPERTY(float, borderTop, SetBorderTop, GetBorderTop);          // Top border property
		PROPERTY(float, borderBottom, SetBorderBottom, GetBorderBottom); // Bottom border property

		PROPERTY(bool, expandWidth, SetWidthExpand, IsWidthExpand);    // Expand children by width property
		PROPERTY(bool, expandHeight, SetHeightExpand, IsHeightExpand); // Expand children by height property

		PROPERTY(bool, fitByChildren, SetFitByChildren, IsFittingByChildren); // Fitting size by children property
	public:
		
		// Default constructor
		VerticalLayout();

		// Copy-constructor
		VerticalLayout(const VerticalLayout& other);

		// Destructor
		~VerticalLayout();

		// Copy operator
		VerticalLayout& operator=(const VerticalLayout& other);

		// Sets base corner
		void SetBaseCorner(BaseCorner baseCorner);

		// Returns base corner
		BaseCorner GetBaseCorner() const;

		// Sets space between widgets
		void SetSpacing(float spacing);

		// Returns space between widgets
		float GetSpacing() const;

		// Sets border
		void SetBorder(const BorderF& border);

		// Returns border
		BorderF GetBorder() const;

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

		// Updates layout
		void UpdateSelfTransform() override;

		SERIALIZABLE(VerticalLayout);

	protected:
		BaseCorner mBaseCorner = BaseCorner::Top;  // Base corner of widgets arranging @SERIALIZABLE
		float      mSpacing = 0.0f;                // Space between widgets @SERIALIZABLE
		BorderF    mBorder;                        // Border @SERIALIZABLE
		bool       mExpandWidth = true;            // Expanding by width @SERIALIZABLE
		bool       mExpandHeight = true;           // Expanding by height @SERIALIZABLE
		bool       mFitByChildren = false;         // Fitting by children @SERIALIZABLE

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Returns layout width with children
		float GetMinWidthWithChildren() const override;

		// Returns layout height
		float GetMinHeightWithChildren() const override;

		// Returns layout height weight with children
		float GetHeightWeightWithChildren() const override;

		// It is called when child widget was added
		void OnChildAdded(Widget* child) override;

		// It is called when child widget was removed
		void OnChildRemoved(Widget* child) override;

		// Invokes required function for childs arranging
		virtual void RearrangeChilds();

		// Arranging child from left to right by bottom, middle and top
		void ArrangeFromTopToBottom();

		// Arranging child from right to left by bottom, middle and top
		void ArrangeFromBottomToTop();

		// Arranging child from center by bottom, middle and top
		void ArrangeFromCenter();

		// Expands size by children
		virtual void ExpandSizeByChilds();

		// Aligns widget by height with base corner
		void AlignWidgetByWidth(Widget* child, float heightAnchor);

		// Updates layout's weight and minimal size
		virtual void UpdateLayoutParametres();
	};
}

CLASS_BASES_META(o2::VerticalLayout)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(o2::VerticalLayout)
{
	PUBLIC_FIELD(baseCorner);
	PUBLIC_FIELD(spacing);
	PUBLIC_FIELD(border);
	PUBLIC_FIELD(borderLeft);
	PUBLIC_FIELD(borderRight);
	PUBLIC_FIELD(borderTop);
	PUBLIC_FIELD(borderBottom);
	PUBLIC_FIELD(expandWidth);
	PUBLIC_FIELD(expandHeight);
	PUBLIC_FIELD(fitByChildren);
	PROTECTED_FIELD(mBaseCorner).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSpacing).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mBorder).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mExpandWidth).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mExpandHeight).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mFitByChildren).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::VerticalLayout)
{

	PUBLIC_FUNCTION(void, SetBaseCorner, BaseCorner);
	PUBLIC_FUNCTION(BaseCorner, GetBaseCorner);
	PUBLIC_FUNCTION(void, SetSpacing, float);
	PUBLIC_FUNCTION(float, GetSpacing);
	PUBLIC_FUNCTION(void, SetBorder, const BorderF&);
	PUBLIC_FUNCTION(BorderF, GetBorder);
	PUBLIC_FUNCTION(void, SetBorderLeft, float);
	PUBLIC_FUNCTION(float, GetBorderLeft);
	PUBLIC_FUNCTION(void, SetBorderRight, float);
	PUBLIC_FUNCTION(float, GetBorderRight);
	PUBLIC_FUNCTION(void, SetBorderTop, float);
	PUBLIC_FUNCTION(float, GetBorderTop);
	PUBLIC_FUNCTION(void, SetBorderBottom, float);
	PUBLIC_FUNCTION(float, GetBorderBottom);
	PUBLIC_FUNCTION(void, SetWidthExpand, bool);
	PUBLIC_FUNCTION(bool, IsWidthExpand);
	PUBLIC_FUNCTION(void, SetHeightExpand, bool);
	PUBLIC_FUNCTION(bool, IsHeightExpand);
	PUBLIC_FUNCTION(void, SetFitByChildren, bool);
	PUBLIC_FUNCTION(bool, IsFittingByChildren);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(float, GetMinWidthWithChildren);
	PROTECTED_FUNCTION(float, GetMinHeightWithChildren);
	PROTECTED_FUNCTION(float, GetHeightWeightWithChildren);
	PROTECTED_FUNCTION(void, OnChildAdded, Widget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Widget*);
	PROTECTED_FUNCTION(void, RearrangeChilds);
	PROTECTED_FUNCTION(void, ArrangeFromTopToBottom);
	PROTECTED_FUNCTION(void, ArrangeFromBottomToTop);
	PROTECTED_FUNCTION(void, ArrangeFromCenter);
	PROTECTED_FUNCTION(void, ExpandSizeByChilds);
	PROTECTED_FUNCTION(void, AlignWidgetByWidth, Widget*, float);
	PROTECTED_FUNCTION(void, UpdateLayoutParametres);
}
END_META;
