#include "o2/stdafx.h"
#include "RectPacker.h"

namespace o2
{
	RectsPacker::RectsPacker(const Vec2F& maxSize):
		mMaxSize(maxSize), mRectsPool(25, 25)
	{
	}

	RectsPacker::~RectsPacker()
	{
		Clear();
	}

	RectsPacker::Rect* RectsPacker::AddRect(const Vec2F& size)
	{
		Rect* newRect = mRectsPool.Take();
		newRect->size = size;
		newRect->rect = RectF();
		mRects.Add(newRect);
		return newRect;
	}

	void RectsPacker::RemoveRect(Rect* remRect)
	{
		mRects.Remove(remRect);
		mRectsPool.Free(remRect);
	}

	void RectsPacker::Clear()
	{
		for (auto rt : mRects)
			mRectsPool.Free(rt);

		mRects.Clear();
	}

	void RectsPacker::SetMaxSize(const Vec2F& maxSize)
	{
		mMaxSize = maxSize;
	}

	Vec2F RectsPacker::GetMaxSize() const
	{
		return mMaxSize;
	}

	int RectsPacker::GetPagesCount() const
	{
		if (mRects.IsEmpty())
			return 0;

		return mRects.Max<int>([&](Rect* rt) { return rt->page; })->page + 1;
	}

	bool RectsPacker::Pack()
	{
		for (auto node : mQuadNodes)
			delete node;

		mQuadNodes.Clear();

		mRects.ForEach([](Rect* rt) { rt->page = -1; rt->rect = RectI(); });
		mRects.Sort([](auto a, auto b) { return a->size.y > b->size.y; });

		for (auto rt : mRects)
		{
			if (!InsertRect(*rt))
				return false;
		}

		return true;
	}


	void RectsPacker::CreateNewPage()
	{
		int maxPage = -1;
		for (auto rt : mRects)
			maxPage = Math::Max(maxPage, rt->page);

		mQuadNodes.Add(mnew QuadNode(maxPage + 1, RectF(Vec2F(), mMaxSize)));
	}

	bool RectsPacker::InsertRect(Rect& rt)
	{
		for (auto node : mQuadNodes)
		{
			if (TryInsertRect(rt, node))
				return true;
		}

		for (auto node : mQuadNodes)
		{
			if (TryInsertRectInChilds(rt, node))
				return true;
		}

		CreateNewPage();

		for (auto node : mQuadNodes)
		{
			if (TryInsertRect(rt, node))
				return true;
		}

		for (auto node : mQuadNodes)
		{
			if (TryInsertRectInChilds(rt, node))
				return true;
		}

		return false;
	}

	bool RectsPacker::TryInsertRect(Rect& rt, QuadNode* node)
	{
		if (node->free && node->rect.Width() >= rt.size.x &&
			node->rect.Height() >= rt.size.y)
		{
			Vec2F leftDown = node->rect.LeftBottom();
			Vec2F center = leftDown + rt.size;
			Vec2F rightTop = node->rect.RightTop();

			node->free = false;
			node->AddChild(mnew QuadNode(node->page, RectF(leftDown.x, rightTop.y, rightTop.x, center.y)));
			node->AddChild(mnew QuadNode(node->page, RectF(center.x, center.y, rightTop.x, leftDown.y)));

			rt.page = node->page;
			rt.rect = RectF(leftDown, center);

			return true;
		}

		return false;
	}


	bool RectsPacker::TryInsertRectInChilds(Rect& rt, QuadNode* node)
	{
		for (auto childNode : node->GetChildren())
		{
			if (TryInsertRect(rt, childNode))
				return true;
		}

		for (auto childNode : node->GetChildren())
		{
			if (TryInsertRectInChilds(rt, childNode))
				return true;
		}

		return false;
	}

	RectsPacker::QuadNode::QuadNode(int page /*= 0*/, const RectF& rect /*= RectF()*/):
		page(page), rect(rect), free(true)
	{}

	void RectsPacker::QuadNode::OnChildAdded(QuadNode* child)
	{
		child->page = page;
	}

	bool RectsPacker::QuadNode::operator==(const QuadNode& other)
	{
		return rect == other.rect && page == other.page && free == other.free;
	}

	RectsPacker::Rect::Rect(const Vec2F& size /*= Vec2F()*/):
		size(size), page(-1)
	{}

}
