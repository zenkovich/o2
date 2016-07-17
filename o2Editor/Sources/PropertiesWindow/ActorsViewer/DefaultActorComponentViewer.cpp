#include "DefaultActorComponentViewer.h"

#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Component.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"

namespace Editor
{
	DefaultActorComponentViewer::DefaultActorComponentViewer()
	{

	}

	DefaultActorComponentViewer::~DefaultActorComponentViewer()
	{

	}

	void DefaultActorComponentViewer::SetTargetComponents(const Vector<Component*>& components)
	{
		mTargetComponents = components;
		mFieldProperties.Set(components.Select<IObject*>([](auto x) { return (IObject*)x; }));
	}

	const Type* DefaultActorComponentViewer::GetComponentType() const
	{
		return mComponentType;
	}

	void DefaultActorComponentViewer::SepcializeComponentType(const Type* type)
	{
		mComponentType = type;
		o2EditorProperties.BuildTypeViewer((UIVerticalLayout*)mDataView, type, mFieldProperties);
	}

}