#include "DefaultEditorActorComponentViewer.h"

#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Component.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"

DefaultEditorActorComponentViewer::DefaultEditorActorComponentViewer()
{

}

DefaultEditorActorComponentViewer::~DefaultEditorActorComponentViewer()
{

}

void DefaultEditorActorComponentViewer::SetTargetComponents(const Vector<Component*>& components)
{
	mTargetComponents = components;

	o2EditorProperties.BuildTypeViewer((UIVerticalLayout*)mDataView, 
									   &components[0]->GetType(), 
									   components.Select<IObject*>([](auto x) { return (IObject*)x; }),
									   mUsedPropertyFields);
}

const Type* DefaultEditorActorComponentViewer::GetComponentType() const
{
	return &TypeOf(Component);
}
