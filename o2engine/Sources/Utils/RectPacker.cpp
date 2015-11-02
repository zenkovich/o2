#include "RectPacker.h"

#include <windows.h>

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

	Ptr<RectsPacker::Rect> RectsPacker::AddRect(const Vec2F& size)
	{
		Ptr<Rect> newRect = mRectsPool.Take();
		newRect->mSize = size;
		newRect->mRect = RectF();
		mRects.Add(newRect);
		return newRect;
	}

	void RectsPacker::RemoveRect(Ptr<Rect> remRect)
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
		return mRects.Max<int>([&](const Ptr<Rect>& rt) { return rt->mPage; })->mPage + 1;
	}

	bool RectsPacker::Pack()
	{
		for (auto node : mQuadNodes)
			node.Release();

		mQuadNodes.Clear();

		mRects.ForEach([](const Ptr<Rect>& rt) { rt->mPage = -1; rt->mRect = RectI(); });
		mRects.Sort([](auto a, auto b) { return a->mSize.y > b->mSize.y; });

		for (auto rt : mRects)
			if (!InsertRect(*rt))
				return false;

		return true;
	}


	void RectsPacker::CreateNewPage()
	{
		int maxPage = -1;
		for (auto rt : mRects)
			maxPage = Math::Max(maxPage, rt->mPage);

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

	bool RectsPacker::TryInsertRect(Rect& rt, Ptr<QuadNode> node)
	{
		if (node->mFree && node->mRect.Width() >= rt.mSize.x &&
			node->mRect.Height() >= rt.mSize.y)
		{
			Vec2F leftDown = node->mRect.LeftBottom();
			Vec2F center = leftDown + rt.mSize;
			Vec2F rightTop = node->mRect.RightTop();

			node->mFree = false;
			node->AddChild(mnew QuadNode(node->mPage, RectF(leftDown.x, rightTop.y, rightTop.x, center.y)));
			node->AddChild(mnew QuadNode(node->mPage, RectF(center.x, center.y, rightTop.x, leftDown.y)));

			rt.mPage = node->mPage;
			rt.mRect = RectF(leftDown, center);

			return true;
		}

		return false;
	}


	bool RectsPacker::TryInsertRectInChilds(Rect& rt, Ptr<QuadNode> node)
	{
		for (auto childNode : node->GetChilds())
		{
			if (TryInsertRect(rt, childNode))
				return true;
		}

		for (auto childNode : node->GetChilds())
		{
			if (TryInsertRectInChilds(rt, childNode))
				return true;
		}

		return false;
	}

	RectsPacker::QuadNode::QuadNode(int page /*= 0*/, const RectF& rect /*= RectF()*/):
		mPage(page), mRect(rect), mFree(true)
	{}

	void RectsPacker::QuadNode::OnChildAdded(Ptr<QuadNode> child)
	{
		child->mPage = mPage;
	}

	bool RectsPacker::QuadNode::operator==(const QuadNode& other)
	{
		return mRect == other.mRect && mPage == other.mPage && mFree == other.mFree;
	}

	RectsPacker::Rect::Rect(const Vec2F& size /*= Vec2F()*/):
		mSize(size), mPage(-1)
	{}

}
