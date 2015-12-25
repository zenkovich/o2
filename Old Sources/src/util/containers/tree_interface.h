#pragma once

#include "public.h"
#include "array.h"
#include "util/containers/utils.h"

OPEN_O2_NAMESPACE

template<typename _type>
class ITreeNode
{
public:
	typedef Array<_type*> ChildArr;

protected:
	_type*   _this;
	_type*   mParent;
	ChildArr mChilds;

public:
	ITreeNode();
	virtual ~ITreeNode();

	virtual _type* AddChild(_type* node);
	virtual bool RemoveChild(_type* node, bool release = true);
	virtual void RemoveAllChilds();

	virtual void SetParent(_type* parent);
	virtual _type* GetParent() const;

	virtual ChildArr& GetChilds();

protected:
	virtual void OnChildAdded(_type* child) {}
	virtual void OnChildRemoved(_type* child) {}
};

//implementation
template<typename _type>
ITreeNode<_type>::ITreeNode():
mParent(0)
{
	_this = dynamic_cast<_type*>(this);
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

	mChilds.Add(node);

	OnChildAdded(node);

	return node;
}

template<typename _type>
bool ITreeNode<_type>::RemoveChild(_type* node, bool release = true)
{
	node->mParent = NULL;

	if (!mChilds.Remove(node))
		return false;

	OnChildRemoved(node);

	if (release)
		SafeRelease(node);

	return true;
}

template<typename _type>
void ITreeNode<_type>::RemoveAllChilds()
{
	for (auto child : mChilds)
		OnChildRemoved(child);

	/*foreach(mChilds, child)
		onChildRemoved(*child);*/

	foreach(mChilds, child)
		SafeRelease(*child);

	mChilds.Clear();
}

template<typename _type>
void ITreeNode<_type>::SetParent(_type* parent)
{
	if (parent)
		parent->AddChild(_this);
	else
	{
		if (mParent)
			mParent->RemoveChild(_this, false);

		mParent = NULL;
	}
}

template<typename _type>
_type* ITreeNode<_type>::GetParent() const
{
	return mParent;
}

template<typename _type>
typename ITreeNode<_type>::ChildArr& ITreeNode<_type>::GetChilds()
{
	return mChilds;
}

CLOSE_O2_NAMESPACE
