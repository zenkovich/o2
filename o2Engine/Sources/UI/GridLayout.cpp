#include "GridLayout.h"

namespace o2
{
	UIGridLayout::UIGridLayout(): UIWidget()
	{
		InitializeProperties();
		UpdateLayout();
	}

	UIGridLayout::UIGridLayout(const UIGridLayout& other):
		UIWidget(other), mBaseCorner(other.mBaseCorner), mSpacing(other.mSpacing), mBorder(other.mBorder),
		mFitByChildren(other.mFitByChildren), mCellSize(other.mCellSize), mArrangeAxis(other.mArrangeAxis),
		mArrangeAxisMaxCells(other.mArrangeAxisMaxCells)
	{
		RetargetStatesAnimations();
		UpdateLayout();
		InitializeProperties();
	}

	UIGridLayout::~UIGridLayout()
	{}

	UIGridLayout& UIGridLayout::operator=(const UIGridLayout& other)
	{
		mBaseCorner          = other.mBaseCorner;
		mSpacing             = other.mSpacing;
		mBorder              = other.mBorder;
		mCellSize            = other.mCellSize;
		mArrangeAxis         = other.mArrangeAxis;
		mArrangeAxisMaxCells = other.mArrangeAxisMaxCells;

		UIWidget::operator=(other);

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIGridLayout::SetBaseCorner(BaseCorner baseCorner)
	{
		mBaseCorner = baseCorner;
		RearrangeChilds();
	}

	BaseCorner UIGridLayout::GetBaseCorner() const
	{
		return mBaseCorner;
	}

	void UIGridLayout::SetSpacing(float spacing)
	{
		mSpacing = spacing;
		RearrangeChilds();
	}

	float UIGridLayout::GetSpacing() const
	{
		return mSpacing;
	}

	void UIGridLayout::SetBorder(const RectF& border)
	{
		mBorder = border;
		RearrangeChilds();
	}

	RectF UIGridLayout::GetBorder() const
	{
		return mBorder;
	}

	void UIGridLayout::SetBorderLeft(float value)
	{
		mBorder.left = value;
		RearrangeChilds();
	}

	float UIGridLayout::GetBorderLeft() const
	{
		return mBorder.left;
	}

	void UIGridLayout::SetBorderRight(float value)
	{
		mBorder.right = value;
		RearrangeChilds();
	}

	float UIGridLayout::GetBorderRight() const
	{
		return mBorder.right;
	}

	void UIGridLayout::SetBorderTop(float value)
	{
		mBorder.top = value;
		RearrangeChilds();
	}

	float UIGridLayout::GetBorderTop() const
	{
		return mBorder.top;
	}

	void UIGridLayout::SetBorderBottom(float value)
	{
		mBorder.bottom = value;
		RearrangeChilds();
	}

	float UIGridLayout::GetBorderBottom() const
	{
		return mBorder.bottom;
	}

	void UIGridLayout::SetCellSize(const Vec2F& size)
	{
		mCellSize = size;
		UpdateLayout();
	}

	Vec2F UIGridLayout::GetCellSize() const
	{
		return mCellSize;
	}

	void UIGridLayout::SetArrangeAxis(TwoDirection type)
	{
		mArrangeAxis = type;
		UpdateLayout();
	}

	TwoDirection UIGridLayout::GetArrangeAxis() const
	{
		return mArrangeAxis;
	}

	void UIGridLayout::SetArrangeAxisMaxCells(int count)
	{
		mArrangeAxisMaxCells = count;
		UpdateLayout();
	}

	int UIGridLayout::GetArrangeAxisMaxCells() const
	{
		return mArrangeAxisMaxCells;
	}

	void UIGridLayout::SetFitByChildren(bool fit)
	{
		mFitByChildren = fit;
		RearrangeChilds();
	}

	bool UIGridLayout::IsFittingByChildren() const
	{
		return mFitByChildren;
	}

	void UIGridLayout::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		RecalculateAbsRect();
		UpdateLayersLayouts();

		if (withChildren)
			RearrangeChilds();

		if (mFitByChildren && withChildren)
			ExpandSizeByChilds();
	}

	void UIGridLayout::OnChildAdded(UIWidget* child)
	{
		child->layout.mDrivenByParent = true;
	}

	void UIGridLayout::OnChildRemoved(UIWidget* child)
	{
		child->layout.mDrivenByParent = false;
	}

	void UIGridLayout::RearrangeChilds()
	{
		switch (mBaseCorner)
		{
			case BaseCorner::LeftTop: ArrangeFromLeftTop(); break;
			case BaseCorner::Top: ArrangeFromTop(); break;
			case BaseCorner::RightTop: ArrangeFromRightTop(); break;
			case BaseCorner::Left: ArrangeFromLeft(); break;
			case BaseCorner::Center: ArrangeFromCenter(); break;
			case BaseCorner::Right: ArrangeFromRight(); break;
			case BaseCorner::LeftBottom: ArrangeFromLeftBottom(); break;
			case BaseCorner::Bottom: ArrangeFromBottom(); break;
			case BaseCorner::RightBottom: ArrangeFromRightBottom(); break;
		}
	}

	void UIGridLayout::ArrangeFromLeftTop()
	{
		int cellsCount = mChilds.Count();
		int rem = cellsCount % mArrangeAxisMaxCells;
		float maxCells = (float)mArrangeAxisMaxCells;

		if (mArrangeAxis == TwoDirection::Horizontal)
		{
			Vec2F anchor(0.0f, 1.0f);
			Vec2F cellPos(mBorder.left, -mBorder.top);

			int rowIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (rowIdx == mArrangeAxisMaxCells)
				{
					cellPos.x = mBorder.left;
					cellPos.y -= mCellSize.y + mSpacing;
					rowIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.x += mCellSize.x + mSpacing;

				rowIdx++;
			}
		}
		else
		{
			Vec2F anchor(0.0f, 1.0f);
			Vec2F cellPos(mBorder.left, -mBorder.top);

			int colIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (colIdx == mArrangeAxisMaxCells)
				{
					cellPos.x += mCellSize.y + mSpacing;
					cellPos.y = -mBorder.top;
					colIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.y -= mCellSize.y + mSpacing;

				colIdx++;
			}
		}
	}

	void UIGridLayout::ArrangeFromTop()
	{
		int cellsCount = mChilds.Count();
		int rem = cellsCount % mArrangeAxisMaxCells;
		float maxCells = (float)mArrangeAxisMaxCells;

		if (mArrangeAxis == TwoDirection::Horizontal)
		{
			int rowCells = Math::Min(cellsCount, mArrangeAxisMaxCells);

			Vec2F anchor(0.5f, 1.0f);
			Vec2F cellPos(-((float)rowCells*mCellSize.x + (float)(rowCells - 1)*mSpacing)*0.5f, -mBorder.top);

			int rowIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (rowIdx == mArrangeAxisMaxCells)
				{
					rowCells = Math::Min(cellsCount - i, mArrangeAxisMaxCells);
					cellPos.x = -((float)rowCells*mCellSize.x + (float)(rowCells - 1)*mSpacing)*0.5f;
					cellPos.y -= mCellSize.y + mSpacing;
					rowIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.x += mCellSize.x + mSpacing;

				rowIdx++;
			}
		}
		else
		{
			int cols = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0)
			{
				if (rem > 1) cols += 2;
				else cols++;
			}

			Vec2F anchor(0.5f, 1.0f);
			Vec2F cellPos(-((float)cols*mCellSize.x + (float)(cols - 1)*mSpacing)*0.5f, -mBorder.top);

			int upColCells = rem - rem / 2;

			int colIdx = upColCells > 0 ? mArrangeAxisMaxCells - upColCells : 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (colIdx == mArrangeAxisMaxCells)
				{
					cellPos.x += mCellSize.x + mSpacing;
					cellPos.y = -mBorder.top;
					colIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.y -= mCellSize.y + mSpacing;

				colIdx++;
			}
		}
	}

	void UIGridLayout::ArrangeFromRightTop()
	{
		int cellsCount = mChilds.Count();
		int rem = cellsCount % mArrangeAxisMaxCells;
		float maxCells = (float)mArrangeAxisMaxCells;

		if (mArrangeAxis == TwoDirection::Horizontal)
		{
			Vec2F anchor(1.0f, 1.0f);
			Vec2F cellPos(-mBorder.right, -mBorder.top);

			int rowIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (rowIdx == mArrangeAxisMaxCells)
				{
					cellPos.x = -mBorder.right;
					cellPos.y -= mCellSize.y + mSpacing;
					rowIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = cellPos - mCellSize;
				child->layout.mOffsetMax = cellPos;
				child->UpdateLayout(true);

				cellPos.x -= mCellSize.x + mSpacing;

				rowIdx++;
			}
		}
		else
		{
			Vec2F anchor(1.0f, 1.0f);
			Vec2F cellPos(-mBorder.right, -mBorder.top);

			int colIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (colIdx == mArrangeAxisMaxCells)
				{
					cellPos.x -= mCellSize.x + mSpacing;
					cellPos.y = -mBorder.top;
					colIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = cellPos - mCellSize;
				child->layout.mOffsetMax = cellPos;
				child->UpdateLayout(true);

				cellPos.y -= mCellSize.y + mSpacing;

				colIdx++;
			}
		}
	}

	void UIGridLayout::ArrangeFromLeft()
	{
		int cellsCount = mChilds.Count();
		int rem = cellsCount % mArrangeAxisMaxCells;
		float maxCells = (float)mArrangeAxisMaxCells;

		if (mArrangeAxis == TwoDirection::Horizontal)
		{
			int rows = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0)
			{
				if (rem > 1) rows += 2;
				else rows++;
			}

			Vec2F anchor(0.0f, 0.5f);
			Vec2F cellPos(mBorder.left, ((float)rows*mCellSize.y + (float)(rows - 1)*mSpacing)*0.5f);

			int upRowCells = rem - rem / 2;

			int rowIdx = upRowCells > 0 ? mArrangeAxisMaxCells - upRowCells : 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (rowIdx == mArrangeAxisMaxCells)
				{
					cellPos.x = mBorder.left;
					cellPos.y -= mCellSize.y + mSpacing;
					rowIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.x += mCellSize.x + mSpacing;

				rowIdx++;
			}
		}
		else
		{
			int cols = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0) cols++;

			int colCells = Math::Min(cellsCount, mArrangeAxisMaxCells);

			Vec2F anchor(0.0f, 0.5f);
			Vec2F cellPos(mBorder.left, ((float)colCells*mCellSize.y + (float)(colCells - 1)*mSpacing)*0.5f);

			int colIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (colIdx == mArrangeAxisMaxCells)
				{
					colCells = Math::Min(cellsCount - i, mArrangeAxisMaxCells);

					cellPos.x += mCellSize.x + mSpacing;
					cellPos.y = ((float)colCells*mCellSize.y + (float)(colCells - 1)*mSpacing)*0.5f;
					colIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.y -= mCellSize.y + mSpacing;

				colIdx++;
			}
		}
	}

	void UIGridLayout::ArrangeFromCenter()
	{
		int cellsCount = mChilds.Count();
		int rem = cellsCount % mArrangeAxisMaxCells;
		float maxCells = (float)mArrangeAxisMaxCells;

		if (mArrangeAxis == TwoDirection::Horizontal)
		{
			int rows = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0)
			{
				if (rem > 1) rows += 2;
				else rows++;
			}

			int upRowCells = rem / 2;
			int rowCells = Math::Min(cellsCount, mArrangeAxisMaxCells);

			Vec2F anchor(0.5f, 0.5f);
			Vec2F cellPos(-((float)rowCells*mCellSize.x + (float)(rowCells - 1)*mSpacing)*0.5f,
						  ((float)rows*mCellSize.y + (float)(rows - 1)*mSpacing)*0.5f);

			int rowIdx = 0;
			if (upRowCells > 0)
			{
				cellPos.x = -((float)upRowCells*mCellSize.x + (float)(upRowCells - 1)*mSpacing)*0.5f;
				rowIdx = mArrangeAxisMaxCells - upRowCells;
			}

			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (rowIdx == mArrangeAxisMaxCells)
				{
					rowCells = Math::Min(cellsCount - i, mArrangeAxisMaxCells);
					cellPos.x = -((float)rowCells*mCellSize.x + (float)(rowCells - 1)*mSpacing)*0.5f;
					cellPos.y -= mCellSize.y + mSpacing;
					rowIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.x += mCellSize.x + mSpacing;

				rowIdx++;
			}
		}
		else
		{
			int cols = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0)
			{
				if (rem > 1) cols += 2;
				else cols++;
			}

			int upColCells = rem / 2;
			int colCells = Math::Min(cellsCount, mArrangeAxisMaxCells);

			Vec2F anchor(0.5f, 0.5f);
			Vec2F cellPos(-((float)cols*mCellSize.x + (float)(cols - 1)*mSpacing)*0.5f,
						  ((float)colCells*mCellSize.y + (float)(colCells - 1)*mSpacing)*0.5f);

			int colIdx = 0;
			if (upColCells > 0)
			{
				cellPos.y = ((float)upColCells*mCellSize.y + (float)(upColCells - 1)*mSpacing)*0.5f;
				colIdx = mArrangeAxisMaxCells - upColCells;
			}

			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (colIdx == mArrangeAxisMaxCells)
				{
					colCells = Math::Min(cellsCount - i, mArrangeAxisMaxCells);
					cellPos.x += mCellSize.x + mSpacing;
					cellPos.y = ((float)colCells*mCellSize.y + (float)(colCells - 1)*mSpacing)*0.5f;
					colIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.y -= mCellSize.y + mSpacing;

				colIdx++;
			}
		}
	}

	void UIGridLayout::ArrangeFromRight()
	{
		int cellsCount = mChilds.Count();
		int rem = cellsCount % mArrangeAxisMaxCells;
		float maxCells = (float)mArrangeAxisMaxCells;

		if (mArrangeAxis == TwoDirection::Horizontal)
		{
			int rows = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0)
			{
				if (rem > 1) rows += 2;
				else rows++;
			}

			Vec2F anchor(1.0f, 0.5f);
			Vec2F cellPos(-mBorder.right, ((float)rows*mCellSize.y + (float)(rows - 1)*mSpacing)*0.5f);

			int upRowCells = rem - rem / 2;

			int rowIdx = upRowCells > 0 ? mArrangeAxisMaxCells - upRowCells : 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (rowIdx == mArrangeAxisMaxCells)
				{
					cellPos.x = -mBorder.right;
					cellPos.y -= mCellSize.y + mSpacing;
					rowIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = cellPos - mCellSize;
				child->layout.mOffsetMax = cellPos;
				child->UpdateLayout(true);

				cellPos.x -= mCellSize.x + mSpacing;

				rowIdx++;
			}
		}
		else
		{
			int cols = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0) cols++;

			int colCells = Math::Min(cellsCount, mArrangeAxisMaxCells);

			Vec2F anchor(1.0f, 0.5f);
			Vec2F cellPos(-mBorder.right, ((float)colCells*mCellSize.y + (float)(colCells - 1)*mSpacing)*0.5f);

			int colIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (colIdx == mArrangeAxisMaxCells)
				{
					colCells = Math::Min(cellsCount - i, mArrangeAxisMaxCells);

					cellPos.x -= mCellSize.x + mSpacing;
					cellPos.y = ((float)colCells*mCellSize.y + (float)(colCells - 1)*mSpacing)*0.5f;
					colIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = cellPos - mCellSize;
				child->layout.mOffsetMax = cellPos;
				child->UpdateLayout(true);

				cellPos.y -= mCellSize.y + mSpacing;

				colIdx++;
			}
		}
	}

	void UIGridLayout::ArrangeFromLeftBottom()
	{
		int cellsCount = mChilds.Count();
		int rem = cellsCount % mArrangeAxisMaxCells;
		float maxCells = (float)mArrangeAxisMaxCells;

		if (mArrangeAxis == TwoDirection::Horizontal)
		{
			int rows = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0) rows++;

			Vec2F anchor(0.0f, 0.0f);
			Vec2F cellPos(mBorder.left, mBorder.bottom + (float)rows*mCellSize.y + (float)(rows - 1)*mSpacing);

			int rowIdx = rem != 0 ? mArrangeAxisMaxCells - rem : 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (rowIdx == mArrangeAxisMaxCells)
				{
					cellPos.x = mBorder.left;
					cellPos.y -= mCellSize.y + mSpacing;
					rowIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.x += mCellSize.x + mSpacing;

				rowIdx++;
			}
		}
		else
		{
			Vec2F anchor(0.0f, 0.0f);
			Vec2F cellPos(mBorder.left, mBorder.bottom);

			int colIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (colIdx == mArrangeAxisMaxCells)
				{
					cellPos.x += mCellSize.x + mSpacing;
					cellPos.y = mBorder.bottom;
					colIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = cellPos;
				child->layout.mOffsetMax = cellPos + mCellSize;
				child->UpdateLayout(true);

				cellPos.y += mCellSize.y + mSpacing;

				colIdx++;
			}
		}
	}

	void UIGridLayout::ArrangeFromBottom()
	{
		int cellsCount = mChilds.Count();
		int rem = cellsCount % mArrangeAxisMaxCells;
		float maxCells = (float)mArrangeAxisMaxCells;

		if (mArrangeAxis == TwoDirection::Horizontal)
		{
			int rows = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0) rows++;

			int colCells = Math::Min(cellsCount, mArrangeAxisMaxCells);

			Vec2F anchor(0.5f, 0.0f);
			Vec2F cellPos(-((float)colCells*mCellSize.x + (float)(colCells - 1)*mSpacing)*0.5f,
						  mBorder.bottom + (float)rows*mCellSize.y + (float)(rows - 1)*mSpacing);

			int rowIdx = 0;
			if (rem != 0)
			{
				cellPos.x = -((float)rem*mCellSize.x + (float)(rem - 1)*mSpacing)*0.5f;
				rowIdx = mArrangeAxisMaxCells - rem;
			}

			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (rowIdx == mArrangeAxisMaxCells)
				{
					colCells = Math::Min(cellsCount - i, mArrangeAxisMaxCells);
					cellPos.x = -((float)colCells*mCellSize.x + (float)(colCells - 1)*mSpacing)*0.5f;
					cellPos.y -= mCellSize.y + mSpacing;
					rowIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x, cellPos.y - mCellSize.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x + mCellSize.x, cellPos.y);
				child->UpdateLayout(true);

				cellPos.x += mCellSize.x + mSpacing;

				rowIdx++;
			}
		}
		else
		{
			int cols = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0)
			{
				if (rem > 1) cols += 2;
				else cols++;
			}

			int colCells = Math::Min(cellsCount, mArrangeAxisMaxCells);

			Vec2F anchor(0.5f, 0.0f);
			Vec2F cellPos(-((float)cols*mCellSize.x + (float)(cols - 1)*mSpacing)*0.5f, mBorder.bottom);

			int upColCells = rem / 2;
			int colIdx = upColCells > 0 ? mArrangeAxisMaxCells - upColCells : 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (colIdx == mArrangeAxisMaxCells)
				{
					colCells = Math::Min(cellsCount - i, mArrangeAxisMaxCells);
					cellPos.x += mCellSize.x + mSpacing;
					cellPos.y = mBorder.bottom;
					colIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = cellPos;
				child->layout.mOffsetMax = cellPos + mCellSize;
				child->UpdateLayout(true);

				cellPos.y += mCellSize.y + mSpacing;

				colIdx++;
			}
		}
	}

	void UIGridLayout::ArrangeFromRightBottom()
	{
		int cellsCount = mChilds.Count();
		int rem = cellsCount % mArrangeAxisMaxCells;
		float maxCells = (float)mArrangeAxisMaxCells;

		if (mArrangeAxis == TwoDirection::Horizontal)
		{
			int rows = cellsCount / mArrangeAxisMaxCells;
			if (rem != 0) rows++;

			Vec2F anchor(1.0f, 0.0f);
			Vec2F cellPos(-mBorder.right, mBorder.bottom);

			int rowIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (rowIdx == mArrangeAxisMaxCells)
				{
					cellPos.x = -mBorder.right;
					cellPos.y += mCellSize.y + mSpacing;
					rowIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x - mCellSize.x, cellPos.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x, cellPos.y + mCellSize.y);
				child->UpdateLayout(true);

				cellPos.x -= mCellSize.x + mSpacing;

				rowIdx++;
			}
		}
		else
		{
			Vec2F anchor(1.0f, 0.0f);
			Vec2F cellPos(-mBorder.right, mBorder.bottom);

			int colIdx = 0;
			for (int i = 0; i < cellsCount; i++)
			{
				UIWidget* child = mChilds[i];

				if (colIdx == mArrangeAxisMaxCells)
				{
					cellPos.x -= mCellSize.x + mSpacing;
					cellPos.y = mBorder.bottom;
					colIdx = 0;
				}

				child->layout.mAnchorMin = anchor;
				child->layout.mAnchorMax = anchor;
				child->layout.mOffsetMin = Vec2F(cellPos.x - mCellSize.x, cellPos.y);
				child->layout.mOffsetMax = Vec2F(cellPos.x, cellPos.y + mCellSize.y);
				child->UpdateLayout(true);

				cellPos.y += mCellSize.y + mSpacing;

				colIdx++;
			}
		}
	}

	void UIGridLayout::ExpandSizeByChilds()
	{
		const static Vec2F relativePivots[] = {
			Vec2F(0.0f, 0.5f), // Left
			Vec2F(1.0f, 0.5f), // Right
			Vec2F(0.5f, 1.0f), // Top
			Vec2F(0.5f, 0.0f), // Bottom
			Vec2F(0.5f, 0.5f), // Center
			Vec2F(0.0f, 0.0f), // LeftBottom
			Vec2F(0.0f, 1.0f), // LeftTop
			Vec2F(1.0f, 0.0f), // RightBottom
			Vec2F(1.0f, 1.0f)  // RightTop
		};

		Vec2F relativePivot = relativePivots[(int)mBaseCorner];

		RectF childrenRect;
		if (mChilds.Count() > 0)
			childrenRect = mChilds[0]->layout.mLocalRect;

		for (auto child : mChilds)
		{
			childrenRect.left = Math::Min(childrenRect.left, child->layout.mLocalRect.left);
			childrenRect.right = Math::Max(childrenRect.right, child->layout.mLocalRect.right);
			childrenRect.bottom = Math::Min(childrenRect.bottom, child->layout.mLocalRect.bottom);
			childrenRect.top = Math::Max(childrenRect.top, child->layout.mLocalRect.top);
		}

		Vec2F szDelta = (childrenRect.Size() + mBorder.LeftBottom() + mBorder.RightTop()) - mChildsAbsRect.Size();

		if (szDelta != Vec2F())
		{
			layout.mOffsetMax += szDelta*(Vec2F::One() - relativePivot);
			layout.mOffsetMin -= szDelta*relativePivot;

			UpdateLayout();
		}
	}

	void UIGridLayout::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIGridLayout, baseCorner, SetBaseCorner, GetBaseCorner);
		INITIALIZE_PROPERTY(UIGridLayout, spacing, SetSpacing, GetSpacing);
		INITIALIZE_PROPERTY(UIGridLayout, cellSize, SetCellSize, GetCellSize);
		INITIALIZE_PROPERTY(UIGridLayout, arrangeAxis, SetArrangeAxis, GetArrangeAxis);
		INITIALIZE_PROPERTY(UIGridLayout, arrangeAxisMaxCells, SetArrangeAxisMaxCells, GetArrangeAxisMaxCells);
		INITIALIZE_PROPERTY(UIGridLayout, border, SetBorder, GetBorder);
		INITIALIZE_PROPERTY(UIGridLayout, borderLeft, SetBorderLeft, GetBorderLeft);
		INITIALIZE_PROPERTY(UIGridLayout, borderRight, SetBorderRight, GetBorderRight);
		INITIALIZE_PROPERTY(UIGridLayout, borderTop, SetBorderTop, GetBorderTop);
		INITIALIZE_PROPERTY(UIGridLayout, borderBottom, SetBorderBottom, GetBorderBottom);
		INITIALIZE_PROPERTY(UIGridLayout, fitByChildren, SetFitByChildren, IsFittingByChildren);
	}
}

CLASS_META(o2::UIGridLayout)
{
	BASE_CLASS(o2::UIWidget);

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
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
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
