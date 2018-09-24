#pragma once

#include "Utils/Types/Containers/Vector.h"


namespace o2
{
	// ------------------------
	// Tree structure interface
	// ------------------------
	template<typename _type>
	class ITreeNode
	{
	public:
		typedef Vector<_type*> ChildrenVec;

	public:
		// Default constructor
		ITreeNode();

		// Virtual destructor
		virtual ~ITreeNode();

		// Adds new child node and returns him
		virtual _type* AddChild(_type* node);

		// Remove child node and releases him if needs
		virtual bool RemoveChild(_type* node, bool release = true);

		// Removes and releases all children nodes
		virtual void RemoveAllChilds();

		// Sets parent node
		virtual void SetParent(_type* parent);

		// Returns parent node
		virtual _type* GetParent() const;

		// Return child nodes
		virtual ChildrenVec& GetChilds();

		// Returns constant child nodes
		virtual const ChildrenVec& GetChilds() const;

	protected:
		_type*      _this;    // Template this pointer
		_type*      mParent; // Pointer to parent node
		ChildrenVec mChildren; // Children nodes @SERIALIZABLE

	protected:
		// It is called when added new child
		virtual void OnChildAdded(_type* child) {}

		// It is called when child was removed
		virtual void OnChildRemoved(_type* child) {}
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
	_type* ITreeNode<_type>::AddChild(_type* node)
	{
		if (node->GetParent())
			node->GetParent()->RemoveChild(node, false);

		node->mParent = _this;

		mChildren.Add(node);

		OnChildAdded(node);

		return node;
	}

	template<typename _type>
	bool ITreeNode<_type>::RemoveChild(_type* node, bool release /*= true*/)
	{
		node->mParent = nullptr;

		if (!mChildren.Remove(node))
			return false;

		OnChildRemoved(node);

		if (release && node)
			delete node;

		return true;
	}

	template<typename _type>
	void ITreeNode<_type>::RemoveAllChilds()
	{
		for (auto child : mChildren)
			OnChildRemoved(child);

		for (auto child : mChildren)
			if (child)
				delete child;

		mChildren.Clear();
	}

	template<typename _type>
	void ITreeNode<_type>::SetParent(_type* parent)
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
	_type* ITreeNode<_type>::GetParent() const
	{
		return mParent;
	}

	template<typename _type>
	typename ITreeNode<_type>::ChildrenVec& ITreeNode<_type>::GetChilds()
	{
		return mChildren;
	}

	template<typename _type>
	const typename ITreeNode<_type>::ChildrenVec& o2::ITreeNode<_type>::GetChilds() const
	{
		return mChildren;
	}
}
