#pragma once

#include "Utils/Containers/Pool.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Tree.h"

namespace o2
{
	// -----------------
	// Rectangles packer
	// -----------------
	class RectsPacker
	{
	public:
		// -----------------
		// Packing rectangle
		// -----------------
		struct Rect
		{
			int   mPage; // Page index
			RectF mRect; // Rectangle on page
			Vec2F mSize; // Size of rectangle

		public:
			// Constructor
			Rect(const Vec2F& size = Vec2F());
		};

		typedef Vector<Ptr<Rect>> RectsVec;

	public:
		// Constructor
		RectsPacker(const Vec2F&  maxSize = Vec2F(512, 512));

		// Destructor
		~RectsPacker();

		// Adds rectangle with size
		Ptr<Rect> AddRect(const Vec2F&  size);

		// Removes rectangle
		void RemoveRect(Ptr<Rect> remRect);

		// Removes all rectangles and quads
		void Clear();

		// Sets page maximum size
		void SetMaxSize(const Vec2F&  maxSize);

		// Returns page maximum size
		Vec2F GetMaxSize() const;

		// Returns pages count
		int GetPagesCount() const;

		// Tries to pack, returns true if packed successfully
		bool Pack();

	protected:
		// ---------
		// Quad node
		// ---------
		struct QuadNode: public ITreeNode<QuadNode>
		{
			RectF mRect; // Quad rectangle on page
			int   mPage; // Page index
			bool  mFree; // Is quad free

		public:
			// Constructor
			QuadNode(int page = 0, const RectF& rect = RectF());

			//Calls when child quad was added
			void OnChildAdded(Ptr<QuadNode> child);

			// Check equals operator
			bool operator==(const QuadNode& other);
		};
		typedef Vector<Ptr<QuadNode>> NodesVec;

		Pool<Rect> mRectsPool; // Rectangles pool
		RectsVec   mRects;     // Rectangles
		NodesVec   mQuadNodes; // Quad nodes 
		Vec2F      mMaxSize;   // Max page size

	protected:
		// Tries to insert rectangle
		bool InsertRect(Rect& rt);

		// Tries to insert rectangle in specified node
		bool TryInsertRect(Rect& rt, Ptr<QuadNode> node);

		// Tries to insert rectangle in specified node
		bool TryInsertRectInChilds(Rect& rt, Ptr<QuadNode> node);

		// Creates new page
		void CreateNewPage();
	};

}
