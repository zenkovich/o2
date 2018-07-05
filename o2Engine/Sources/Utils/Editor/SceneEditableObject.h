#pragma once

#include "Utils/Math/Basis.h"
#include "Utils/Math/Layout.h"
#include "Utils/Serialization/Serializable.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Types/String.h"

namespace o2
{
	// ----------------------------------------------------------
	// Object, that can be shown in editor's tree view and edited
	// ----------------------------------------------------------
	class SceneEditableObject: virtual public ISerializable
	{
	public:
		// Default constructor. Registers itself in scene editable objects list
		SceneEditableObject();

		// Destructor, unregisters from scene editable objects list
		virtual ~SceneEditableObject();


		// Returns unique id
		virtual SceneUID GetID() const;

		// Generates new random id 
		virtual void GenerateNewID(bool childs = true);


		// Returns name of object
		virtual String GetName() const;

		// Sets name of object
		virtual void SetName(const String& name);


		// Returns list of object's children
		virtual Vector<SceneEditableObject*> GetEditablesChildren() const;

		// Returns object's parent object. Return nullptr when it is a root scene object
		virtual SceneEditableObject* GetEditableParent();

		// Sets parent object. nullptr means make this object as root. idx is place in parent children. idx == -1 means last
		virtual void SetEditableParent(SceneEditableObject* object);

		// Adds child. idx is place in parent children. idx == -1 means last
		virtual void AddChild(SceneEditableObject* object, int idx = -1);

		// Sets index in siblings - children of parent
		virtual void SetIndexInSiblings(int idx);


		// Returns is that type of object can be enabled and disabled
		virtual bool IsSupportsDisabling() const;

		// Returns is object enabled, override when it's supports
		virtual bool IsEnabled() const;

		// Returns is object enabled and all parent are enabled too
		virtual bool IsEnabledInHierarchy() const;

		// Sets enabling of object, override when it's supports
		virtual void SetEnabled(bool enabled);
		

		// Returns is that type of object can be locked
		virtual bool IsSupportsLocking() const;

		// Returns is object locked, override when it's supports
		virtual bool IsLocked() const;

		// Returns is object locked and all parent are locked too
		virtual bool IsLockedInHierarchy() const;

		// Sets locking of object, override when it's supports
		virtual void SetLocked(bool locked);


		// Returns is that type of object can be transformed
		virtual bool IsSupportsTransforming() const;

		// Returns transform, override when it's supports
		virtual Basis GetTransform() const;

		// Sets transform of object, override when it's supports
		virtual void SetTransform(const Basis& transform);

		// Updates transform immediately
		virtual void UpdateTransform(bool withChildren = true);


		// Returns is object supports pivot 
		virtual bool IsSupportsPivot() const;

		// Sets transform pivot point
		virtual void SetPivot(const Vec2F& pivot);

		// Returns transform pivot
		virtual Vec2F GetPivot() const;


		// Returns is that type of object can be transformed with layout
		virtual bool IsSupportsLayout() const;

		// Returns layout, override when it's supports
		virtual Layout GetLayout() const;

		// Sets layout of object, override when it's supports
		virtual void SetLayout(const Layout& layout);


		// It is called when something changed in this object
		virtual void OnChanged();

		// It is called when actor's locking was changed
		virtual void OnLockChanged();

		// It is called when actor's name was changed
		virtual void OnNameChanged();

		// It is called when child changed
		virtual void OnChildrenChanged();

		SERIALIZABLE(SceneEditableObject);
	};
}

CLASS_BASES_META(o2::SceneEditableObject)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::SceneEditableObject)
{
}
END_META;
CLASS_METHODS_META(o2::SceneEditableObject)
{

	PUBLIC_FUNCTION(SceneUID, GetID);
	PUBLIC_FUNCTION(void, GenerateNewID, bool);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetEditablesChildren);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableParent);
	PUBLIC_FUNCTION(void, SetEditableParent, SceneEditableObject*);
	PUBLIC_FUNCTION(void, AddChild, SceneEditableObject*, int);
	PUBLIC_FUNCTION(void, SetIndexInSiblings, int);
	PUBLIC_FUNCTION(bool, IsSupportsDisabling);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(bool, IsEnabledInHierarchy);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsSupportsLocking);
	PUBLIC_FUNCTION(bool, IsLocked);
	PUBLIC_FUNCTION(bool, IsLockedInHierarchy);
	PUBLIC_FUNCTION(void, SetLocked, bool);
	PUBLIC_FUNCTION(bool, IsSupportsTransforming);
	PUBLIC_FUNCTION(Basis, GetTransform);
	PUBLIC_FUNCTION(void, SetTransform, const Basis&);
	PUBLIC_FUNCTION(void, UpdateTransform, bool);
	PUBLIC_FUNCTION(bool, IsSupportsPivot);
	PUBLIC_FUNCTION(void, SetPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPivot);
	PUBLIC_FUNCTION(bool, IsSupportsLayout);
	PUBLIC_FUNCTION(Layout, GetLayout);
	PUBLIC_FUNCTION(void, SetLayout, const Layout&);
	PUBLIC_FUNCTION(void, OnChanged);
	PUBLIC_FUNCTION(void, OnLockChanged);
	PUBLIC_FUNCTION(void, OnNameChanged);
	PUBLIC_FUNCTION(void, OnChildrenChanged);
}
END_META;
