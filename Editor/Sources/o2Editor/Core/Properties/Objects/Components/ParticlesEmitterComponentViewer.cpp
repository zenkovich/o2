#include "o2Editor/stdafx.h"
#include "ParticlesEmitterComponentViewer.h"

#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	ParticlesEmitterComponentViewer::ParticlesEmitterComponentViewer() = default;

	ParticlesEmitterComponentViewer::~ParticlesEmitterComponentViewer()
	{
	}

	ParticlesEmitterComponentViewer& ParticlesEmitterComponentViewer::operator=(const ParticlesEmitterComponentViewer& other)
	{
		TObjectPropertiesViewer<ParticlesEmitterComponent>::operator=(other);
		return *this;
	}

	void ParticlesEmitterComponentViewer::RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		mControls = mmake<VerticalLayout>();
		mControls->expandHeight = false;
		mControls->expandWidth = true;
		mControls->fitByChildren = true;
		mSpoiler->AddChild(mControls);

		auto playControls = mmake<HorizontalLayout>();
		playControls->layout->minHeight = 20;
		playControls->expandWidth = true;
		playControls->expandHeight = false;
		playControls->borderLeft = 5;
		playControls->spacing = 5;
		mControls->AddChild(playControls);

		auto playPauseToggle = o2UI.CreateWidget<Toggle>("animation state play-stop");
		playPauseToggle->name = "play-stop";
		playPauseToggle->layout->minSize = Vec2F(20, 20);
		playPauseToggle->layout->maxSize = Vec2F(20, 20);
		playControls->AddChild(playPauseToggle);

		auto timeProgress = o2UI.CreateWidget<HorizontalProgress>("animation state bar");
		timeProgress->name = "bar";
		timeProgress->layout->maxHeight = 3;
		playControls->AddChild(timeProgress);

		auto loopedToggle = o2UI.CreateWidget<Toggle>("animation state loop");
		loopedToggle->name = "loop";
		loopedToggle->layout->minSize = Vec2F(20, 20);
		loopedToggle->layout->maxSize = Vec2F(20, 20);
		playControls->AddChild(loopedToggle);

		auto horSeparatorContainer = mmake<Widget>();
		horSeparatorContainer->layout->minHeight = 3;
		mControls->AddChild(horSeparatorContainer);

		auto horSeparator = o2UI.CreateWidget<Widget>("hor separator");
		*horSeparator->layout = WidgetLayout::BothStretch(-10, 0, -10, 0);
		horSeparatorContainer->AddChild(horSeparator);

		o2EditorProperties.BuildObjectProperties(mSpoiler, &TypeOf(ParticlesEmitterComponent), mPropertiesContext, "",
												 mOnChildFieldChangeCompleted, onChanged);
	}

	void ParticlesEmitterComponentViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		auto prevTargetObjects = mTypeTargetObjects;

		TObjectPropertiesViewer<ParticlesEmitterComponent>::OnRefreshed(targetObjets);
	}

	void ParticlesEmitterComponentViewer::OnEnabled()
	{
	}

	void ParticlesEmitterComponentViewer::OnDisabled()
	{
	}
}

DECLARE_TEMPLATE_CLASS(Editor::TObjectPropertiesViewer<ParticlesEmitterComponent>);
// --- META ---

DECLARE_CLASS(Editor::ParticlesEmitterComponentViewer, Editor__ParticlesEmitterComponentViewer);
// --- END META ---
