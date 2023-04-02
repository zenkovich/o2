#include "o2/stdafx.h"
#include "MTESceneComponent.h"

#include "Box2D/Dynamics/b2Body.h"
#include "o2/Physics/PhysicsWorld.h"
#include "o2/Scene/Physics/ICollider.h"
#include "o2/Scene/DrawableComponent.h"

namespace o2
{
	MTESceneComponent::MTESceneComponent()
	{}

	MTESceneComponent::~MTESceneComponent()
	{
	}

	void MTESceneComponent::Update(float dt)
	{
		m_scene->update(dt);
	}

	void MTESceneComponent::Draw()
	{
		o2Render.BeginCustomRender();

		//m_scene->update(o2Time.GetDeltaTime());
		m_scene->draw();

		o2Render.EndCustomRender();
	}

	void MTESceneComponent::Initialize(SceneFactory sceneFactory, std::string_view options)
	{
		m_render = mte::makeRenderer();
		m_video = mte::make_video_factory();
		m_fileserver = mte::make_fileserver();
		m_events = mte::make_event_manager();
		mte::ResourceSubsystem subsystems{ m_video.get(), m_fileserver.get(), m_render.get() };
		m_scene = sceneFactory(m_events.get(), subsystems, options);

		m_render->init();
		m_scene->init();
		m_scene->attach();

		Vec2F renderSize = o2Render.GetCurrentResolution();
		std::int32_t width = (std::int32_t)renderSize.x;
		std::int32_t height = (std::int32_t)renderSize.y;
		m_scene->setViewport(width, height);
		m_render->setViewport({ 0, 0, width, height });
	}
}

DECLARE_CLASS(o2::MTESceneComponent);
