#include "PropertiesWindow/ActorsViewer/IActorAnimationViewer.h"

#include "Scene/Actor.h"
#include "UI/Widget.h"

namespace Editor
{
	IActorAnimationViewer::IActorAnimationViewer()
	{
		BREAK_ON_REFLECTION_STAGE;

		mDataView = mnew UIWidget();
		mDataView->name = "animation viewer";
	}

	IActorAnimationViewer::~IActorAnimationViewer()
	{
		if (mDataView)
			delete mDataView;
	}

	UIWidget* IActorAnimationViewer::GetWidget() const
	{
		return mDataView;
	}

	void IActorAnimationViewer::Expand()
	{

	}

	void IActorAnimationViewer::Collapse()
	{

	}

}

CLASS_META(Editor::IActorAnimationViewer)
{
	BASE_CLASS(o2::IObject);

	PROTECTED_FIELD(mDataView);

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
}
END_META;
