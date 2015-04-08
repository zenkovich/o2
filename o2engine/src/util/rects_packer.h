#pragma once

#include "public.h"
#include "util/containers/array.h"
#include "util/containers/pool.h"
#include "util/containers/tree_interface.h"

OPEN_O2_NAMESPACE

class RectsPacker
{
public:
	struct Rect
	{
		int   mPage;
		RectF mRect;
		Vec2F mSize;

		Rect(const Vec2F& size = Vec2F()):mSize(size), mPage(-1) {}
	};

	typedef Array< Rect* > RectsArr;

protected:
	struct QuadNode: public ITreeNode<QuadNode>
	{
		RectF mRect;
		int   mPage;
		bool  mFree;

		QuadNode(int page = 0, const RectF& rect = RectF()):mPage(page), mRect(rect), mFree(true) {}

		void OnChildAdded(QuadNode* child);

		bool operator==(const QuadNode& other);
	};
	typedef Array<QuadNode> NodesArr;

	Pool<Rect> mRectsPool;
	RectsArr   mRects;
	NodesArr   mQuadNodes;
	Vec2F      mMaxSize;

public:
	RectsPacker(const Vec2F&  maxSize = Vec2F(512, 512));
	~RectsPacker();

	Rect* AddRect(const Vec2F&  size);
	void RemoveRect(Rect* remRect);
	void Clear();

	void SetMaxSize(const Vec2F&  maxSize);
	Vec2F GetMaxSize() const;

	bool Pack();

protected:
	bool InsertRect(Rect& rt);
	bool TryInsertRect(Rect& rt, QuadNode& node);
	void CreateNewPage();

	static bool RectSizeCompare(Rect*& a, Rect*& b);
};

CLOSE_O2_NAMESPACE