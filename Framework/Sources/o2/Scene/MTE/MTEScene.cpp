#include "o2/stdafx.h"
#include "MTEScene.h"

#include "Box2D/Dynamics/b2Body.h"
#include "o2/Physics/PhysicsWorld.h"
#include "o2/Scene/Physics/ICollider.h"
#include "o2/Scene/DrawableComponent.h"

namespace o2
{
	MTESceneActor::MTESceneActor()
	{}

	MTESceneActor::MTESceneActor(const MTESceneActor& other):
		Actor(other)
	{}

	MTESceneActor::~MTESceneActor()
	{
	}

	MTESceneActor& MTESceneActor::operator=(const MTESceneActor& other)
	{
		Actor::operator=(other);
		return *this;
	}


	void MTESceneActor::OnEnableInHierarchyChanged()
	{
		Actor::OnEnableInHierarchyChanged();
	}

	void MTESceneActor::OnAddToScene()
	{
		Actor::OnAddToScene();
	}

	void MTESceneActor::OnRemoveFromScene()
	{
		Actor::OnRemoveFromScene();
	}

	void MTESceneActor::OnUpdate(float dt)
	{
		m_scene->update(dt);
	}

	void MTESceneActor::Draw()
	{
		m_scene->draw();
	}

	void MTESceneActor::Initialize(SceneFactory sceneFactory, std::string_view options)
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

	SceneLayer* MTESceneActor::GetSceneDrawableSceneLayer() const
	{
		return mLayer;
	}

	bool MTESceneActor::IsSceneDrawableEnabled() const
	{
		return mResEnabledInHierarchy;
	}

	ISceneDrawable* MTESceneActor::GetParentDrawable()
	{
		if (mParent)
		{
			auto itParent = mParent;
			while (itParent)
			{
				if (auto drawable = dynamic_cast<ISceneDrawable*>(itParent))
					return drawable;

				auto comp = itParent->GetComponent<DrawableComponent>();
				if (comp)
					return (ISceneDrawable*)comp;

				itParent = itParent->GetParent();
			}
		}

		return nullptr;
	}

	int MTESceneActor::GetIndexInParentDrawable() const
	{
		if (mParent)
			return mParent->GetChildren().IndexOf((Actor*)this);

		return 0;
	}
}

DECLARE_TEMPLATE_CLASS(o2::Ref<o2::MTESceneActor>);

DECLARE_CLASS(o2::MTESceneActor);
