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

		o2EditorProperties.BuildTypeViewer((UIVerticalLayout*)mDataView,
										   &components[0]->GetType(),
										   components.Select<IObject*>([](auto x) { return (IObject*)x; }),
										   mUsedPropertyFields);
	}

	const Type* DefaultActorComponentViewer::GetComponentType() const
	{
		return &TypeOf(Component);
	}
}