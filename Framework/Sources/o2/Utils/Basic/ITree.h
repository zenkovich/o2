#pragma once

#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/Ref.h"


namespace o2
{
    // ------------------------
    // Tree structure interface
    // ------------------------
    template<typename _type>
    class ITreeNode
    {
    public:
        // Default constructor
        ITreeNode();

        // Virtual destructor
        virtual ~ITreeNode();

        // Adds new child node and returns him
        virtual Ref<_type> AddChild(const Ref<_type>& node);

        // Remove child node and releases him if needs
        virtual void RemoveChild(const Ref<_type>& node);

        // Removes and releases all children nodes
        virtual void RemoveAllChilds();

        // Sets parent node
        virtual void SetParent(const Ref<_type>& parent);

        // Returns parent node
        virtual const WeakRef<_type>& GetParent() const;

        // Return child nodes
        virtual Vector<Ref<_type>>& GetChildren();

        // Returns constant child nodes
        virtual const Vector<Ref<_type>>& GetChildren() const;

    protected:
        _type*             _this;     // Template this pointer
        WeakRef<_type>     mParent;   // Pointer to parent node
        Vector<Ref<_type>> mChildren; // Children nodes @SERIALIZABLE

    protected:
        // Called when added new child
        virtual void OnChildAdded(const Ref<_type>& child) {}

        // Called when child was removed
        virtual void OnChildRemoved(const Ref<_type>& child) {}
    };


    template<typename _type>
    ITreeNode<_type>::ITreeNode():
        mParent(nullptr)
    {
        _this = static_cast<_type*>(this);
    }

    template<typename _type>
    ITreeNode<_type>::~ITreeNode()
    {
        RemoveAllChilds();
    }

    template<typename _type>
    Ref<_type> ITreeNode<_type>::AddChild(const Ref<_type>& node)
    {
        if (node->GetParent())
            node->GetParent().Lock()->RemoveChild(node);

        node->mParent = WeakRef(_this);
        mChildren.Add(node);

        OnChildAdded(node);

        return node;
    }

    template<typename _type>
    void ITreeNode<_type>::RemoveChild(const Ref<_type>& node)
    {
        node->mParent = nullptr;
        mChildren.Remove(node);

        OnChildRemoved(node);
    }

    template<typename _type>
    void ITreeNode<_type>::RemoveAllChilds()
    {
        for (auto& child : mChildren)
            OnChildRemoved(child);

        mChildren.Clear();
    }

    template<typename _type>
    void ITreeNode<_type>::SetParent(const Ref<_type>& parent)
    {
        if (parent)
        {
            parent->AddChild(Ref(_this));
        }
        else
        {
            if (mParent)
                mParent.Lock()->RemoveChild(Ref(_this));

            mParent = nullptr;
        }
    }

    template<typename _type>
    const WeakRef<_type>& ITreeNode<_type>::GetParent() const
    {
        return mParent;
    }

    template<typename _type>
    Vector<Ref<_type>>& ITreeNode<_type>::GetChildren()
    {
        return mChildren;
    }

    template<typename _type>
    const Vector<Ref<_type>>& ITreeNode<_type>::GetChildren() const
    {
        return mChildren;
    }
}
