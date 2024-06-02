#pragma once

#include "o2/Utils/Types/Containers/Vector.h"

namespace o2
{
    // ----------------------
    // Pool objects container
    // ----------------------
    template<typename _type>
    class Pool
    {
        Vector<Ref<_type>> mObjects;   // Cached objects
        int                mChunkSize; // Cache resize size

    public:
        // Constructor
        Pool(int initialCount = 5, int chunkSize = 5);

        // Destructor
        ~Pool();

        // Sets chunk size - cache resize size
        void SetChunkSize(int chunkSize);

        // Returns chunk size - cache resize size
        int  GetChunkSize() const;

        // Takes object from cached and returns him
        Ref<_type> Take();

        // Frees object and puts into cached
        void Free(const Ref<_type>& obj);

        // Creates cached object
        void CreateObjects(int count);
    };


    template<typename _type>
    Pool<_type>::Pool(int initialCount /*= 5*/, int chunkSize /*= 5*/):
        mChunkSize(chunkSize)
    {
        CreateObjects(initialCount);
    }

    template<typename _type>
    Pool<_type>::~Pool()
    {
        mObjects.Clear();
    }

    template<typename _type>
    void Pool<_type>::SetChunkSize(int chunkSize)
    {
        mChunkSize = chunkSize;
    }

    template<typename _type>
    int Pool<_type>::GetChunkSize() const
    {
        return mChunkSize;
    }

    template<typename _type>
    Ref<_type> Pool<_type>::Take()
    {
        if (mObjects.Count() == 0)
            CreateObjects(mChunkSize);

        return mObjects.PopBack();
    }

    template<typename _type>
    void Pool<_type>::Free(const Ref<_type>& obj)
    {
        mObjects.Add(obj);
    }

    template<typename _type>
    void Pool<_type>::CreateObjects(int count)
    {
        for (int i = 0; i < count; i++)
            mObjects.Add(mmake<_type>());
    }
}
