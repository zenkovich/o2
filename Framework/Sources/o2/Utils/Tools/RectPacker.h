#pragma once

#include "o2/Utils/Basic/ITree.h"
#include "o2/Utils/Math/Rect.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Types/Containers/Pool.h"
#include "o2/Utils/Types/Containers/Vector.h"

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
			int   page; // Page index
			RectF rect; // Rectangle on page
			Vec2F size; // Size of rectangle

		public:
			// Constructor
			Rect(const Vec2F& size = Vec2F());
		};

		typedef Vector<Rect*> RectsVec;

	public:
		// Constructor
		RectsPacker(const Vec2F&  maxSize = Vec2F(512, 512));

		// Destructor
		~RectsPacker();

		// Adds rectangle with size
		Rect* AddRect(const Vec2F&  size);

		// Removes rectangle
		void RemoveRect(Rect* remRect);

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
			RectF rect; // Quad rectangle on page
			int   page; // Page index
			bool  free; // Is quad free

		public:
			// Constructor
			QuadNode(int page = 0, const RectF& rect = RectF());

			//Calls when child quad was added
			void OnChildAdded(QuadNode* child);

			// Check equals operator
			bool operator==(const QuadNode& other);
		};
		typedef Vector<QuadNode*> NodesVec;

		Pool<Rect> mRectsPool; // Rectangles pool
		RectsVec   mRects;     // Rectangles
		NodesVec   mQuadNodes; // Quad nodes 
		Vec2F      mMaxSize;   // Max page size

	protected:
		// Tries to insert rectangle
		bool InsertRect(Rect& rt);

		// Tries to insert rectangle in specified node
		bool TryInsertRect(Rect& rt, QuadNode* node);

		// Tries to insert rectangle in specified node
		bool TryInsertRectInChilds(Rect& rt, QuadNode* node);

		// Creates new page
		void CreateNewPage();
	};

}
