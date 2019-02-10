#include "stdafx.h"
#include "SceneEditableObject.h"

#include "Scene/Scene.h"

namespace o2
{
	SceneEditableObject::SceneEditableObject()
	{ }

	SceneEditableObject::~SceneEditableObject()
	{ }

	bool SceneEditableObject::IsOnScene() const
	{
		return true;
	}

	bool SceneEditableObject::IsHieararchyOnScene() const
	{
		return true;
	}

	SceneUID SceneEditableObject::GetID() const
	{
		return 0;
	}

	void SceneEditableObject::GenerateNewID(bool childs /*= true*/)
	{ }

	String SceneEditableObject::GetName() const
	{
		return String();
	}

	void SceneEditableObject::SetName(const String& name)
	{ }

	Vector<SceneEditableObject*> SceneEditableObject::GetEditablesChildren() const
	{
		return Vector<SceneEditableObject*>();
	}

	SceneEditableObject* SceneEditableObject::GetEditableParent() const
	{
		return nullptr;
	}

	void SceneEditableObject::SetEditableParent(SceneEditableObject* object)
	{ }

	void SceneEditableObject::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{ }

	void SceneEditableObject::SetIndexInSiblings(int idx)
	{ }

	bool SceneEditableObject::CanBeParentedTo(const Type& parentType)
	{
		return parentType.IsBasedOn(TypeOf(Actor));
	}

	bool SceneEditableObject::IsSupportsDisabling() const
	{
		return false;
	}

	bool SceneEditableObject::IsEnabled() const
	{
		return true;
	}

	bool SceneEditableObject::IsEnabledInHierarchy() const
	{
		auto parent = GetEditableParent();
		return IsEnabled() && (parent ? parent->IsEnabledInHierarchy() : true);
	}

	void SceneEditableObject::SetEnabled(bool enabled)
	{ }

	bool SceneEditableObject::IsSupportsLocking() const
	{
		return false;
	}

	bool SceneEditableObject::IsLocked() const
	{
		return false;
	}

	bool SceneEditableObject::IsLockedInHierarchy() const
	{
		return false;
	}

	void SceneEditableObject::SetLocked(bool locked)
	{ }

	bool SceneEditableObject::IsSupportsTransforming() const
	{
		return false;
	}

	Basis SceneEditableObject::GetTransform() const
	{
		return Basis::Identity();
	}

	void SceneEditableObject::SetTransform(const Basis& transform)
	{ }

	void SceneEditableObject::UpdateTransform()
	{ }

	bool SceneEditableObject::IsSupportsPivot() const
	{
		return false;
	}

	void SceneEditableObject::SetPivot(const Vec2F& pivot)
	{ }

	Vec2F SceneEditableObject::GetPivot() const
	{
		return Vec2F();
	}

	bool SceneEditableObject::IsSupportsLayout() const
	{
		return false;
	}

	Layout SceneEditableObject::GetLayout() const
	{
		return Layout();
	}

	void SceneEditableObject::SetLayout(const Layout& layout)
	{ }

	bool SceneEditableObject::IsSupportsDeleting() const
	{
		return true;
	}

	void SceneEditableObject::OnChanged()
	{ }

	void SceneEditableObject::OnLockChanged()
	{ }

	void SceneEditableObject::OnNameChanged()
	{ }

	void SceneEditableObject::OnChildrenChanged()
	{ }

}

DECLARE_CLASS(o2::SceneEditableObject);
