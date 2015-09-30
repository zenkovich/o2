#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	// ------------------------
	// Tree structure interface
	// ------------------------
	template<typename _type>
	class ITreeNode
	{
	public:
		typedef Vector<Ptr<_type>> ChildsVec;

	public:
		// Default constructor
		ITreeNode();

		// Virtual destructor
		virtual ~ITreeNode();

		// Adds new child node and returns him
		virtual Ptr<_type> AddChild(Ptr<_type> node);

		// Remove child node and releases him if needs
		virtual bool RemoveChild(Ptr<_type> node, bool release = true);

		// Removes and releases all children nodes
		virtual void RemoveAllChilds();

		// Sets parent node
		virtual void SetParent(Ptr<_type> parent);

		// Returns parent node
		virtual Ptr<_type> GetParent() const;

		// Return child nodes
		virtual ChildsVec& GetChilds();

		// Returns constant child nodes
		virtual const ChildsVec& GetChilds() const;

	protected:
		_type*    _this;    // Template this pointer
		Ptr<_type> mParent; // Pointer to parent node
		ChildsVec  mChilds; // Children nodes

	protected:
		// Calls when added new child
		virtual void OnChildAdded(Ptr<_type> child) {}

		// Calls when child was removed
		virtual void OnChildRemoved(Ptr<_type> child) {}
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
	Ptr<_type> ITreeNode<_type>::AddChild(Ptr<_type> node)
	{
		if (node->GetParent())
			node->GetParent()->RemoveChild(node, false);

		node->mParent = _this;

		mChilds.Add(node);

		OnChildAdded(node);

		return node;
	}

	template<typename _type>
	bool ITreeNode<_type>::RemoveChild(Ptr<_type> node, bool release /*= true*/)
	{
		node->mParent = nullptr;

		if (!mChilds.Remove(node))
			return false;

		OnChildRemoved(node);

		if (release && node)
			node.Release();

		return true;
	}

	template<typename _type>
	void ITreeNode<_type>::RemoveAllChilds()
	{
		for (auto child : mChilds)
			OnChildRemoved(child);

		for (auto child : mChilds)
			if (child)
				child.Release();

		mChilds.Clear();
	}

	template<typename _type>
	void ITreeNode<_type>::SetParent(Ptr<_type> parent)
	{
		if (parent)
		{
			parent->AddChild(_this);
		}
		else
		{
			if (mParent)
				mParent->RemoveChild(_this, false);

			mParent = nullptr;
		}
	}

	template<typename _type>
	Ptr<_type> ITreeNode<_type>::GetParent() const
	{
		return mParent;
	}

	template<typename _type>
	typename ITreeNode<_type>::ChildsVec& ITreeNode<_type>::GetChilds()
	{
		return mChilds;
	}

	template<typename _type>
	typename const ITreeNode<_type>::ChildsVec& o2::ITreeNode<_type>::GetChilds() const
	{
		return mChilds;
	}
}
