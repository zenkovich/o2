#include "rects_packer.h"

#include "util/containers/utils.h"

OPEN_O2_NAMESPACE

RectsPacker::RectsPacker(const Vec2F& maxSize):mMaxSize(maxSize), mRectsPool(25, 25)
{
}

RectsPacker::~RectsPacker()
{
	Clear();
}

RectsPacker::Rect* RectsPacker::AddRect(const Vec2F& size)
{
	Rect* newRect = mRectsPool.Take();
	newRect->mSize = size;
	newRect->mRect = RectF();
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

bool RectsPacker::Pack()
{
	mQuadNodes.Clear();

	mRects.Sort(&RectSizeCompare);

	foreach(mRects, rt)
	if (!InsertRect(**rt))
		return false;

	return true;
}

bool RectsPacker::RectSizeCompare(Rect*& a, Rect*& b)
{
	return a->mSize.x*a->mSize.y > b->mSize.x*b->mSize.y;
}

void RectsPacker::CreateNewPage()
{
	int maxPage = -1;
	foreach(mRects, rt)
		maxPage = Max(maxPage, (*rt)->mPage);

	mQuadNodes.Add(QuadNode(maxPage + 1, RectF(Vec2F(), mMaxSize)));
}

bool RectsPacker::InsertRect(Rect& rt)
{
	foreach(mQuadNodes, node)
	if (TryInsertRect(rt, *node))
		return true;

	CreateNewPage();

	foreach(mQuadNodes, node)
	if (TryInsertRect(rt, *node))
		return true;

	return false;
}

bool RectsPacker::TryInsertRect(Rect& rt, QuadNode& node)
{
	if (node.mFree && node.mRect.Width() >= rt.mSize.x &&
		node.mRect.Height() >= rt.mSize.y)
	{
		Vec2F leftTop = node.mRect.LeftTop();
		Vec2F center = leftTop + rt.mSize;
		Vec2F rightDown = node.mRect.RightBottom();

		node.mFree = false;
		node.AddChild(mnew QuadNode(node.mPage, RectF(center.x, leftTop.y, rightDown.x, center.y)));
		node.AddChild(mnew QuadNode(node.mPage, RectF(center.x, center.y, rightDown.x, rightDown.y)));
		node.AddChild(mnew QuadNode(node.mPage, RectF(leftTop.x, center.y, center.x, rightDown.y)));

		rt.mPage = node.mPage;
		rt.mRect = RectF(leftTop, center);

		return true;
	}

	for (auto childNode : node.GetChilds())
	{
		if (TryInsertRect(rt, *childNode))
			return true;
	}

	return false;
}


void RectsPacker::QuadNode::OnChildAdded(QuadNode* child)
{
	child->mPage = mPage;
}

bool RectsPacker::QuadNode::operator==(const QuadNode& other)
{
	return mRect == other.mRect && mPage == other.mPage && mFree == other.mFree;
}

CLOSE_O2_NAMESPACE