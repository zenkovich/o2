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
        struct Rect: public RefCounterable
        {
            int   page; // Page index
            RectF rect; // Rectangle on page
            Vec2F size; // Size of rectangle

        public:
            // Constructor
            Rect(const Vec2F& size = Vec2F());
        };

    public:
        // Constructor
        RectsPacker(const Vec2F&  maxSize = Vec2F(512, 512));

        // Destructor
        ~RectsPacker();

        // Adds rectangle with size
        Ref<Rect> AddRect(const Vec2F&  size);

        // Removes rectangle
        void RemoveRect(const Ref<Rect>& remRect);

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
        struct QuadNode: public ITreeNode<QuadNode>, public RefCounterable
        {
            RectF rect; // Quad rectangle on page
            int   page; // Page index
            bool  free; // Is quad free

        public:
            // Constructor
            QuadNode(int page = 0, const RectF& rect = RectF());

            //Calls when child quad was added
            void OnChildAdded(const Ref<QuadNode>& child) override;

            // Check equals operator
            bool operator==(const QuadNode& other);
        };

        Vec2F mMaxSize; // Max page size

        Pool<Rect>   mRectsPool; // Rectangles pool
        Vector<Ref<Rect>> mRects;     // Rectangles

        Vector<Ref<QuadNode>> mQuadNodes; // Quad nodes 

    protected:
        // Tries to insert rectangle
        bool InsertRect(Rect& rt);

        // Tries to insert rectangle in specified node
        bool TryInsertRect(Rect& rt, const Ref<QuadNode>& node);

        // Tries to insert rectangle in specified node
        bool TryInsertRectInChilds(Rect& rt, const Ref<QuadNode>& node);

        // Creates new page
        void CreateNewPage();
    };

}
